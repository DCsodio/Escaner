/*!
 * @file Adafruit_HMC5883_U_LPC845.cpp
 * @brief Implementación adaptada para LPC845 del driver HMC5883L
 */

#include "hmc5883l.h"

// Factores de conversión para diferentes ganancias
static const float gain_factors_xy[7] = {1100.0F, 855.0F, 670.0F, 450.0F, 400.0F, 330.0F, 230.0F};
static const float gain_factors_z[7] = {980.0F, 760.0F, 600.0F, 400.0F, 255.0F, 295.0F, 205.0F};

/***************************************************************************
 CONSTRUCTOR
 ***************************************************************************/

HMC5883L_LPC845::HMC5883L_LPC845(){
  _magGain = HMC5883_MAGGAIN_1_3;
  _gauss_LSB_XY = 1100.0F;
  _gauss_LSB_Z = 980.0F;
  _offset_x = _offset_y = _offset_z = 0.0f;
  _scale_x = _scale_y = _scale_z = 1.0f;
}

/***************************************************************************
 PRIVATE FUNCTIONS
 ***************************************************************************/

void HMC5883L_LPC845::write8(uint8_t reg, uint8_t value) {
  i2c_master_transfer_t transfer;

  transfer.slaveAddress = HMC5883_ADDRESS_MAG;
  transfer.direction = kI2C_Write;
  transfer.subaddress = reg;
  transfer.subaddressSize = 1;
  transfer.data = &value;
  transfer.dataSize = 1;
  transfer.flags = kI2C_TransferDefaultFlag;

  I2C_MasterTransferBlocking(I2C1, &transfer);
}

uint8_t HMC5883L_LPC845::read8(uint8_t reg) {
  uint8_t value = 0;
  i2c_master_transfer_t transfer;

  // Primero escribimos el registro que queremos leer
  transfer.slaveAddress = HMC5883_ADDRESS_MAG;
  transfer.direction = kI2C_Write;
  transfer.subaddress = reg;
  transfer.subaddressSize = 1;
  transfer.data = NULL;
  transfer.dataSize = 0;
  transfer.flags = kI2C_TransferDefaultFlag;

  I2C_MasterTransferBlocking(I2C1, &transfer);

  // Ahora leemos el dato
  transfer.direction = kI2C_Read;
  transfer.subaddress = 0;
  transfer.subaddressSize = 0;
  transfer.data = &value;
  transfer.dataSize = 1;
  transfer.flags = kI2C_TransferDefaultFlag;

  I2C_MasterTransferBlocking(I2C1, &transfer);

  return value;
}

void HMC5883L_LPC845::readRawData(void) {
  uint8_t buffer[6];
  i2c_master_transfer_t transfer;

  transfer.slaveAddress = HMC5883_ADDRESS_MAG;
  transfer.direction = kI2C_Write;
  transfer.subaddress = HMC5883_REGISTER_MAG_OUT_X_H_M;
  transfer.subaddressSize = 1;
  transfer.data = NULL;
  transfer.dataSize = 0;
  transfer.flags = kI2C_TransferDefaultFlag;

  I2C_MasterTransferBlocking(I2C1, &transfer);

  // Leemos 6 bytes: X, Z, Y (en ese orden según el datasheet)
  transfer.direction = kI2C_Read;
  transfer.subaddress = 0;
  transfer.subaddressSize = 0;
  transfer.data = buffer;
  transfer.dataSize = 6;
  transfer.flags = kI2C_TransferDefaultFlag;

  I2C_MasterTransferBlocking(I2C1, &transfer);

  // Convertir los datos (big-endian)
  _magData.x = (int16_t)((buffer[0] << 8) | buffer[1]);
  _magData.z = (int16_t)((buffer[2] << 8) | buffer[3]);
  _magData.y = (int16_t)((buffer[4] << 8) | buffer[5]);
}

bool HMC5883L_LPC845::verifyConnection(void) {
  // Leer el registro de identificación A (debería ser 'H' = 0x48)
  uint8_t id_a = read8(HMC5883_REGISTER_MAG_IRA_REG_M);
  return (id_a == 0x48);
}

/***************************************************************************
 PUBLIC FUNCTIONS
 ***************************************************************************/

bool HMC5883L_LPC845::begin(hmc5883MagGain ganancia) {
  // Verificar conexión
  if (!verifyConnection()) {
    return false;
  }

  // --- Soft Reset Magnético ---
  // 1. Pasar a modo Idle
  write8(HMC5883_REGISTER_MAG_MR_REG_M, 0x02);
  for (volatile int i = 0; i < 10000; i++); // pequeña espera (~10 ms)

  // 2. Alternar ganancias extremas (recalibra ADC interno)
  setMagGain(HMC5883_MAGGAIN_8_1); // máxima
  for (volatile int i = 0; i < 100; i++);
  setMagGain(HMC5883_MAGGAIN_1_3); // normal
  for (volatile int i = 0; i < 100; i++);
  // 3. Volver a modo continuo
  write8(HMC5883_REGISTER_MAG_MR_REG_M, 0x00);
  for (volatile int i = 0; i < 100; i++);
  // --- Configuración normal ---
  // Ganancia de trabajo
  setMagGain(ganancia);

  // Salida 15 Hz, medición normal
  write8(HMC5883_REGISTER_MAG_CRA_REG_M, 0x10);

  // Limpiar primeras lecturas
  for (int i = 0; i < 5; i++) readRawData();

  return true;
}

void HMC5883L_LPC845::setMagGain(hmc5883MagGain gain) {
  write8(HMC5883_REGISTER_MAG_CRB_REG_M, (uint8_t)gain);
  _magGain = gain;

  // Calcular índice de tabla (0..6)
  int8_t gain_index = ((uint8_t)gain >> 5) - 1;

  if (gain_index >= 0 && gain_index < 7) {
    _gauss_LSB_XY = gain_factors_xy[gain_index];
    _gauss_LSB_Z = gain_factors_z[gain_index];
  } else {
    // Valor fuera de rango → mantener último valor conocido
    // (opcional: podrías fijar defaults seguros)
    _gauss_LSB_XY = 1100.0F;
    _gauss_LSB_Z = 980.0F;
  }
}

bool HMC5883L_LPC845::readData(hmc5883_data_t *data) {
  if (data == NULL) return false;

  readRawData();

  const float GAUSS_TO_MICROTESLA = 100.0f;

  float x_gauss = _magData.x / _gauss_LSB_XY;
  float y_gauss = _magData.y / _gauss_LSB_XY;
  float z_gauss = _magData.z / _gauss_LSB_Z;

  // Convertir a µT y aplicar calibración
  data->x = (x_gauss * GAUSS_TO_MICROTESLA - _offset_x) * _scale_x;
  data->y = (y_gauss * GAUSS_TO_MICROTESLA - _offset_y) * _scale_y;
  data->z = (z_gauss * GAUSS_TO_MICROTESLA - _offset_z) * _scale_z;

  data->heading = atan2f(data->y, data->x);
  if (data->heading < 0) data->heading += 2 * M_PI;
  data->heading_deg = data->heading * 180.0f / M_PI;

  return true;
}

float HMC5883L_LPC845::getHeading(void) {
  hmc5883_data_t data;
  if (readData(&data)) {
    return data.heading_deg;
  }
  return 0.0f;
}

void HMC5883L_LPC845::calibrate(void) {
  // Esta es una calibración básica
  // Para una calibración completa, deberías rotar el sensor en todas las direcciones
  const int samples = 100;
  float min_x = 10000, max_x = -10000;
  float min_y = 10000, max_y = -10000;
  float min_z = 10000, max_z = -10000;

  for (int i = 0; i < samples; i++) {
    readRawData();

    float x = _magData.x / _gauss_LSB_XY * 100.0f;
    float y = _magData.y / _gauss_LSB_XY * 100.0f;
    float z = _magData.z / _gauss_LSB_Z * 100.0f;

    if (x < min_x) min_x = x;
    if (x > max_x) max_x = x;
    if (y < min_y) min_y = y;
    if (y > max_y) max_y = y;
    if (z < min_z) min_z = z;
    if (z > max_z) max_z = z;

    // Pequeña pausa entre lecturas
    for (volatile int d = 0; d < 1000; d++);
  }

  // Calcular offsets y escalas
  _offset_x = (max_x + min_x) / 2.0f;
  _offset_y = (max_y + min_y) / 2.0f;
  _offset_z = (max_z + min_z) / 2.0f;

  float avg_delta_x = (max_x - min_x) / 2.0f;
  float avg_delta_y = (max_y - min_y) / 2.0f;
  float avg_delta = (avg_delta_x + avg_delta_y) / 2.0f;

  _scale_x = avg_delta / avg_delta_x;
  _scale_y = avg_delta / avg_delta_y;
  _scale_z = 1.0f; // No escalamos Z para calibración 2D
}

void HMC5883L_LPC845::setCalibrationOffsets(float x_offset, float y_offset, float z_offset) {
  _offset_x = x_offset;
  _offset_y = y_offset;
  _offset_z = z_offset;
}
