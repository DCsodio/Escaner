#include "../../../inc/VL53L1/vl53l1_platform.h"

#include "../I2C/i2c.h"
/**
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include <string.h>

#define VL53L1_DEFAULT_I2C_ADDRESS 0x29
//static uint16_t vl53l1_dev_address = VL53L1_DEFAULT_I2C_ADDRESS;

void delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < (ms * 12000); i++) { // aprox para 12MHz
        __asm__("nop");
    }
}

int8_t VL53L1_WriteMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count) {
    status_t status;
    i2c_master_transfer_t xfer;

    xfer.slaveAddress   = dev & 0x7F;   // dirección en 7 bits
    xfer.direction      = kI2C_Write;
    xfer.subaddress     = index;
    xfer.subaddressSize = 2;            // registro de 16 bits
    xfer.data           = pdata;
    xfer.dataSize       = count;
    xfer.flags          = kI2C_TransferDefaultFlag;

    status = I2C_MasterTransferBlocking(I2C1, &xfer);

    return (status == kStatus_Success) ? 0 : -1;
}

int8_t VL53L1_ReadMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count) {
    status_t status;
    i2c_master_transfer_t xfer;

    xfer.slaveAddress   = dev & 0x7F;   // dirección en 7 bits
    xfer.direction      = kI2C_Read;
    xfer.subaddress     = index;
    xfer.subaddressSize = 2;            // registro de 16 bits
    xfer.data           = pdata;
    xfer.dataSize       = count;
    xfer.flags          = kI2C_TransferDefaultFlag;

    status = I2C_MasterTransferBlocking(I2C1, &xfer);

    return (status == kStatus_Success) ? 0 : -1;
}

int8_t VL53L1_WrByte(uint16_t dev, uint16_t index, uint8_t data) {
    return VL53L1_WriteMulti(dev, index, &data, 1);
}

int8_t VL53L1_WrWord(uint16_t dev, uint16_t index, uint16_t data) {
    uint8_t buffer[2];
    buffer[0] = (data >> 8) & 0xFF;   // MSB primero
    buffer[1] = data & 0xFF;          // LSB después
    return VL53L1_WriteMulti(dev, index, buffer, 2);
}
int8_t VL53L1_WrDWord(uint16_t dev, uint16_t index, uint32_t data) {
    uint8_t buffer[4];
    buffer[0] = (data >> 24) & 0xFF;
    buffer[1] = (data >> 16) & 0xFF;
    buffer[2] = (data >> 8) & 0xFF;
    buffer[3] = data & 0xFF;
    return VL53L1_WriteMulti(dev, index, buffer, 4);
}

int8_t VL53L1_RdByte(uint16_t dev, uint16_t index, uint8_t *data) {
    return VL53L1_ReadMulti(dev, index, data, 1);
}

int8_t VL53L1_RdWord(uint16_t dev, uint16_t index, uint16_t *data) {
    uint8_t buffer[2];
    int8_t status = VL53L1_ReadMulti(dev, index, buffer, 2);
    if (status == 0) {
        *data = ((uint16_t)buffer[0] << 8) | buffer[1];  // MSB primero
    }
    return status;
}

int8_t VL53L1_RdDWord(uint16_t dev, uint16_t index, uint32_t *data) {
    uint8_t buffer[4];
    int8_t status = VL53L1_ReadMulti(dev, index, buffer, 4);
    if (status == 0) {
        *data = ((uint32_t)buffer[0] << 24) |
                ((uint32_t)buffer[1] << 16) |
                ((uint32_t)buffer[2] << 8)  |
                buffer[3];
    }
    return status;
}

int8_t VL53L1_WaitMs(uint16_t dev, int32_t wait_ms) {
    delay_ms(wait_ms);
    return 0;
}
