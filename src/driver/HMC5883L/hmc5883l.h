#ifndef HMC5883L_H_
#define HMC5883L_H_

#include "../I2C/i2c.h"
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/*!
 * @brief I2C address/bits
 */
#define HMC5883_ADDRESS_MAG (0x3C >> 1) // 0011110x - dirección I2C 0x1E

/*!
 @brief Registers
 */
typedef enum {
  HMC5883_REGISTER_MAG_CRA_REG_M = 0x00,
  HMC5883_REGISTER_MAG_CRB_REG_M = 0x01,
  HMC5883_REGISTER_MAG_MR_REG_M = 0x02,
  HMC5883_REGISTER_MAG_OUT_X_H_M = 0x03,
  HMC5883_REGISTER_MAG_OUT_X_L_M = 0x04,
  HMC5883_REGISTER_MAG_OUT_Z_H_M = 0x05,
  HMC5883_REGISTER_MAG_OUT_Z_L_M = 0x06,
  HMC5883_REGISTER_MAG_OUT_Y_H_M = 0x07,
  HMC5883_REGISTER_MAG_OUT_Y_L_M = 0x08,
  HMC5883_REGISTER_MAG_SR_REG_Mg = 0x09,
  HMC5883_REGISTER_MAG_IRA_REG_M = 0x0A,
  HMC5883_REGISTER_MAG_IRB_REG_M = 0x0B,
  HMC5883_REGISTER_MAG_IRC_REG_M = 0x0C,
  HMC5883_REGISTER_MAG_TEMP_OUT_H_M = 0x31,
  HMC5883_REGISTER_MAG_TEMP_OUT_L_M = 0x32
} hmc5883MagRegisters_t;

/*!
 * @brief Magnetometer gain settings
 */
typedef enum {
  HMC5883_MAGGAIN_1_3 = 0x20, // +/- 1.3
  HMC5883_MAGGAIN_1_9 = 0x40, // +/- 1.9
  HMC5883_MAGGAIN_2_5 = 0x60, // +/- 2.5
  HMC5883_MAGGAIN_4_0 = 0x80, // +/- 4.0
  HMC5883_MAGGAIN_4_7 = 0xA0, // +/- 4.7
  HMC5883_MAGGAIN_5_6 = 0xC0, // +/- 5.6
  HMC5883_MAGGAIN_8_1 = 0xE0  // +/- 8.1
} hmc5883MagGain;

/*!
 * @brief Internal magnetometer data type
 */
typedef struct hmc5883MagData_s {
  int16_t x;           //!< Magnetometer x value (raw)
  int16_t y;           //!< Magnetometer y value (raw)
  int16_t z;           //!< Magnetometer z value (raw)
  float orientation;   //!< Magnetometer orientation
} hmc5883MagData;

/*!
 * @brief Chip ID
 */
#define HMC5883_ID (0b11010100)

/*!
 * @brief Sensor data structure
 */
typedef struct {
  float x;           //!< Magnetic field X component (microTesla)
  float y;           //!< Magnetic field Y component (microTesla)
  float z;           //!< Magnetic field Z component (microTesla)
  float heading;     //!< Heading in radians
  float heading_deg; //!< Heading in degrees
} hmc5883_data_t;

//! Driver para el magnetómetro HMC5883L adaptado para LPC845
class HMC5883L_LPC845 {

public:
  /*!
   * @brief Constructor
   */
  HMC5883L_LPC845();

  /*!
   * @brief Inicializa el sensor
   * @return true si la inicialización fue exitosa, false en caso contrario
   */
  bool begin(hmc5883MagGain ganancia);

  /*!
   * @brief Configura la ganancia del magnetómetro
   * @param gain Ganancia deseada
   */
  void setMagGain(hmc5883MagGain gain);

  /*!
   * @brief Lee los datos del sensor
   * @param data Puntero a estructura donde guardar los datos
   * @return true si la lectura fue exitosa, false en caso contrario
   */
  bool readData(hmc5883_data_t *data);

  /*!
   * @brief Obtiene el heading (rumbo) en grados
   * @return Heading en grados (0-359.9)
   */
  float getHeading(void);

  /*!
   * @brief Calibra el sensor (debe llamarse con el sensor rotando)
   */
  void calibrate(void);

  /*!
   * @brief Establece los offsets de calibración
   * @param x_offset Offset en eje X
   * @param y_offset Offset en eje Y
   * @param z_offset Offset en eje Z
   */
  void setCalibrationOffsets(float x_offset, float y_offset, float z_offset, float sx, float sy);

private:
  hmc5883MagGain _magGain;
  hmc5883MagData _magData;
  float _gauss_LSB_XY;
  float _gauss_LSB_Z;
  float _offset_x, _offset_y, _offset_z;
  float _scale_x, _scale_y, _scale_z;

  void write8(uint8_t reg, uint8_t value);
  uint8_t read8(uint8_t reg);
  void readRawData(void);
  bool verifyConnection(void);
};


#endif /* HMC5883L_H_ */
