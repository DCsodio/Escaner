/*
 * i2c.c
 *
 *  Created on: 12 ago. 2021
 *      Author: mariano
 */
#include "i2c.h"

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Pointers to i2c clocks for each instance. */
static const clock_ip_name_t s_i2cClocks[] = I2C_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

#if !(defined(FSL_FEATURE_I2C_HAS_NO_RESET) && FSL_FEATURE_I2C_HAS_NO_RESET)
static const reset_ip_name_t s_i2cResets[] = I2C_RSTS_N;
#endif


/*! @brief Array to map i2c instance number to base address. */
static I2C_Type *const s_i2cBases[] = I2C_BASE_PTRS;
uint8_t rxBuff;
extern uint8_t demora_i2c;

/*!
 * brief Reset peripheral module.
 *
 * Reset peripheral module.
 *
 * param peripheral Peripheral to reset. The enum argument contains encoding of reset register
 *                   and reset bit position in the reset register.
 */
void RESET_PeripheralReset(reset_ip_name_t peripheral)
{
    RESET_SetPeripheralReset(peripheral);
    RESET_ClearPeripheralReset(peripheral);
}

void I2C_MasterInit(I2C_Type *base, const i2c_master_config_t *masterConfig, uint32_t srcClock_Hz)
{
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Enable the clock. */
    CLOCK_EnableClock(s_i2cClocks[I2C_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

#if !(defined(FSL_FEATURE_I2C_HAS_NO_RESET) && FSL_FEATURE_I2C_HAS_NO_RESET)
    RESET_PeripheralReset(s_i2cResets[I2C_GetInstance(base)]);
#endif

    I2C_MasterEnable(base, masterConfig->enableMaster);
    I2C_MasterSetBaudRate(base, masterConfig->baudRate_Bps, srcClock_Hz);
}

/*!
 * brief Provides a default configuration for the I2C master peripheral.
 *
 * This function provides the following default configuration for the I2C master peripheral:
 * code
 *  masterConfig->enableMaster            = true;
 *  masterConfig->baudRate_Bps            = 100000U;
 *  masterConfig->enableTimeout           = false;
 * endcode
 *
 * After calling this function, you can override any settings in order to customize the configuration,
 * prior to initializing the master driver with I2C_MasterInit().
 *
 * param[out] masterConfig User provided configuration structure for default values. Refer to #i2c_master_config_t.
 */
void I2C_MasterGetDefaultConfig(i2c_master_config_t *masterConfig)
{
    /* Initializes the configure structure to zero. */
    (void)memset(masterConfig, 0, sizeof(*masterConfig));

    masterConfig->enableMaster  = true;
    masterConfig->baudRate_Bps  = 100000U;
    masterConfig->enableTimeout = false;
}

/*!
 * brief Returns an instance number given a base address.
 *
 * If an invalid base address is passed, debug builds will assert. Release builds will just return
 * instance number 0.
 *
 * param base The I2C peripheral base address.
 * return I2C instance number starting from 0.
 */
uint32_t I2C_GetInstance(I2C_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0U; instance < ARRAY_SIZE(s_i2cBases); instance++)
    {
        if (s_i2cBases[instance] == base)
        {
            break;
        }
    }

    assert(instance < ARRAY_SIZE(s_i2cBases));

    return instance;
}


/*!
 * brief Sets the I2C bus frequency for master transactions.
 *
 * The I2C master is automatically disabled and re-enabled as necessary to configure the baud
 * rate. Do not call this function during a transfer, or the transfer is aborted.
 *
 * param base The I2C peripheral base address.
 * param srcClock_Hz I2C functional clock frequency in Hertz.
 * param baudRate_Bps Requested bus frequency in bits per second.
 */
void I2C_MasterSetBaudRate(I2C_Type *base, uint32_t baudRate_Bps, uint32_t srcClock_Hz)
{
    uint32_t scl, divider;
    uint32_t best_scl = 0;
    uint32_t best_div = 0;
    uint32_t err, best_err;

    best_err = 0U;

    for (scl = 9U; scl >= 2U; scl--)
    {
        /* calculated ideal divider value for given scl, round up the result */
        divider = ((srcClock_Hz * 10U) / (baudRate_Bps * scl * 2U) + 5U) / 10U;

        /* adjust it if it is out of range */
        divider = ((divider > 0x10000U) ? 0x10000U : divider);

        /* calculate error */
        err = srcClock_Hz - (baudRate_Bps * scl * 2U * divider);
        if ((err < best_err) || (best_err == 0U))
        {
            best_div = divider;
            best_scl = scl;
            best_err = err;
        }

        if ((err == 0U) || (divider >= 0x10000U))
        {
            /* either exact value was found
               or divider is at its max (it would even greater in the next iteration for sure) */
            break;
        }
    }

    base->CLKDIV  = I2C_CLKDIV_DIVVAL(best_div - 1U);
    base->MSTTIME = I2C_MSTTIME_MSTSCLLOW(best_scl - 2U) | I2C_MSTTIME_MSTSCLHIGH(best_scl - 2U);
}

/*!
 * brief Deinitializes the I2C master peripheral.
 *
 * This function disables the I2C master peripheral and gates the clock. It also performs a software
 * reset to restore the peripheral to reset conditions.
 *
 * param base The I2C peripheral base address.
 */
void I2C_MasterDeinit(I2C_Type *base)
{
    I2C_MasterEnable(base, false);

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Disable the clock. */
    CLOCK_DisableClock(s_i2cClocks[I2C_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}


uint32_t I2C_PendingStatusWait(I2C_Type *base)
{
    uint32_t status;
    do
    {
        status = I2C_GetStatusFlags(base);
    } while ((status & I2C_STAT_MSTPENDING_MASK) == 0U);

    /* Clear controller state. */
    I2C_MasterClearStatusFlags(base, I2C_STAT_MSTARBLOSS_MASK | I2C_STAT_MSTSTSTPERR_MASK);

    return status;
}

/*!
 * brief Sends a START on the I2C bus.
 *
 * This function is used to initiate a new master mode transfer by sending the START signal.
 * The slave address is sent following the I2C START signal.
 *
 * param base I2C peripheral base pointer
 * param address 7-bit slave device address.
 * param direction Master transfer directions(transmit/receive).
 * retval kStatus_Success Successfully send the start signal.
 * retval kStatus_I2C_Busy Current bus is busy.
 */
status_t I2C_MasterStart(I2C_Type *base, uint8_t address, i2c_direction_t direction)
{
    uint32_t status;

    if (I2C_PendingStatusWait(base) == (uint32_t)kStatus_I2C_Timeout)
    {
        return kStatus_I2C_Timeout;
    }

    base->MSTDAT = ((uint32_t)address << 1) | ((uint32_t)direction & 1u);
    base->MSTCTL = I2C_MSTCTL_MSTSTART_MASK;

    // Esperar a que se complete
    status = I2C_PendingStatusWait(base);

    uint32_t master_state = (status & I2C_STAT_MSTSTATE_MASK) >> I2C_STAT_MSTSTATE_SHIFT;

    if (master_state == I2C_STAT_MSTCODE_NACKADR) {
        return kStatus_I2C_Addr_Nak;   // esclavo no respondió
    }

    return kStatus_Success;  // esclavo respondió con ACK
}



/*!
 * brief Sends a STOP signal on the I2C bus.
 *
 * retval kStatus_Success Successfully send the stop signal.
 * retval kStatus_I2C_Timeout Send stop signal failed, timeout.
 */
status_t I2C_MasterStop(I2C_Type *base)
{
    if (I2C_PendingStatusWait(base) == (uint32_t)kStatus_I2C_Timeout)
    {
        return kStatus_I2C_Timeout;
    }

    base->MSTCTL = I2C_MSTCTL_MSTSTOP_MASK;
    return kStatus_Success;
}


/*!
 * brief Performs a polling send transfer on the I2C bus.
 *
 * Sends up to a txSize number of bytes to the previously addressed slave device. The slave may
 * reply with a NAK to any byte in order to terminate the transfer early. If this happens, this
 * function returns #kStatus_I2C_Nak.
 *
 * param base  The I2C peripheral base address.
 * param txBuff The pointer to the data to be transferred.
 * param txSize The length in bytes of the data to be transferred.
 * param flags Transfer control flag to control special behavior like suppressing start or stop, for normal transfers
 * use kI2C_TransferDefaultFlag
 * retval kStatus_Success Data was sent successfully.
 * retval #kStatus_I2C_Busy Another master is currently utilizing the bus.
 * retval #kStatus_I2C_Nak The slave device sent a NAK in response to a byte.
 * retval #kStatus_I2C_ArbitrationLost Arbitration lost error.
 */
status_t I2C_MasterWriteBlocking(I2C_Type *base, const void *txBuff, size_t txSize, uint32_t flags)
{
    assert(txBuff != NULL);

    uint32_t status;
    uint32_t master_state;
    status_t err;

    const uint8_t *buf = (const uint8_t *)txBuff;

    err = kStatus_Success;
    while (txSize != 0U)
    {
        status = I2C_PendingStatusWait(base);

#if I2C_RETRY_TIMES
        if (status == kStatus_I2C_Timeout)
        {
            return kStatus_I2C_Timeout;
        }
#endif

        if ((status & I2C_STAT_MSTARBLOSS_MASK) != 0U)
        {
            return kStatus_I2C_ArbitrationLost;
        }

        if ((status & I2C_STAT_MSTSTSTPERR_MASK) != 0U)
        {
            return kStatus_I2C_StartStopError;
        }

        master_state = (status & I2C_STAT_MSTSTATE_MASK) >> I2C_STAT_MSTSTATE_SHIFT;
        switch (master_state)
        {
            case I2C_STAT_MSTCODE_TXREADY:
                /* ready to send next byte */
                base->MSTDAT = *buf++;
                txSize--;
                base->MSTCTL = I2C_MSTCTL_MSTCONTINUE_MASK;
                break;

            case I2C_STAT_MSTCODE_NACKADR:
                /* slave nacked the address */
                err = kStatus_I2C_Addr_Nak;
                break;

            case I2C_STAT_MSTCODE_NACKDAT:
                /* slave nacked the last byte */
                err = kStatus_I2C_Nak;
                break;

            default:
                /* unexpected state */
                err = kStatus_I2C_UnexpectedState;
                break;
        }

        if (err != kStatus_Success)
        {
            return err;
        }
    }

    status = I2C_PendingStatusWait(base);

#if I2C_RETRY_TIMES
    if (status == kStatus_I2C_Timeout)
    {
        return kStatus_I2C_Timeout;
    }
#endif

    if ((status & (I2C_STAT_MSTARBLOSS_MASK | I2C_STAT_MSTSTSTPERR_MASK)) == 0U)
    {
        if ((flags & (uint32_t)kI2C_TransferNoStopFlag) == 0U)
        {
            /* Initiate stop */
            base->MSTCTL = I2C_MSTCTL_MSTSTOP_MASK;
            status       = I2C_PendingStatusWait(base);

#if I2C_RETRY_TIMES
            if (status == kStatus_I2C_Timeout)
            {
                return kStatus_I2C_Timeout;
            }
#endif
        }
    }

    if ((status & I2C_STAT_MSTARBLOSS_MASK) != 0U)
    {
        return kStatus_I2C_ArbitrationLost;
    }

    if ((status & I2C_STAT_MSTSTSTPERR_MASK) != 0U)
    {
        return kStatus_I2C_StartStopError;
    }

    return kStatus_Success;
}


/*!
 * brief Performs a polling receive transfer on the I2C bus.
 *
 * param base  The I2C peripheral base address.
 * param rxBuff The pointer to the data to be transferred.
 * param rxSize The length in bytes of the data to be transferred.
 * param flags Transfer control flag to control special behavior like suppressing start or stop, for normal transfers
 * use kI2C_TransferDefaultFlag
 * retval kStatus_Success Data was received successfully.
 * retval #kStatus_I2C_Busy Another master is currently utilizing the bus.
 * retval #kStatus_I2C_Nak The slave device sent a NAK in response to a byte.
 * retval #kStatus_I2C_ArbitrationLost Arbitration lost error.
 */
status_t I2C_MasterReadBlocking(I2C_Type *base, void *rxBuff, size_t rxSize, uint32_t flags)
{
    assert(rxBuff != NULL);

    uint32_t status = 0;
    uint32_t master_state;
    status_t err;

    uint8_t *buf = (uint8_t *)(rxBuff);

    err = kStatus_Success;
    while (rxSize != 0U)
    {
        status = I2C_PendingStatusWait(base);

#if I2C_RETRY_TIMES
        if (status == kStatus_I2C_Timeout)
        {
            return kStatus_I2C_Timeout;
        }
#endif

        if ((status & (I2C_STAT_MSTARBLOSS_MASK | I2C_STAT_MSTSTSTPERR_MASK)) != 0U)
        {
            break;
        }

        master_state = (status & I2C_STAT_MSTSTATE_MASK) >> I2C_STAT_MSTSTATE_SHIFT;
        switch (master_state)
        {
            case I2C_STAT_MSTCODE_RXREADY:
                /* ready to receive next byte */
                *(buf++) = (uint8_t)base->MSTDAT;
                if (--rxSize != 0U)
                {
                    base->MSTCTL = I2C_MSTCTL_MSTCONTINUE_MASK;
                }
                else
                {
                    if ((flags & (uint32_t)kI2C_TransferNoStopFlag) == 0U)
                    {
                        /* initiate NAK and stop */
                        base->MSTCTL = I2C_MSTCTL_MSTSTOP_MASK;
                        status       = I2C_PendingStatusWait(base);
#if I2C_RETRY_TIMES
                        if (status == kStatus_I2C_Timeout)
                        {
                            return kStatus_I2C_Timeout;
                        }
#endif
                    }
                }
                break;

            case I2C_STAT_MSTCODE_NACKADR:
            case I2C_STAT_MSTCODE_NACKDAT:
                /* slave nacked the last byte */
                err = kStatus_I2C_Nak;
                break;

            default:
                /* unexpected state */
                err = kStatus_I2C_UnexpectedState;
                break;
        }

        if (err != kStatus_Success)
        {
            return err;
        }
    }

    if ((status & I2C_STAT_MSTARBLOSS_MASK) != 0U)
    {
        return kStatus_I2C_ArbitrationLost;
    }

    if ((status & I2C_STAT_MSTSTSTPERR_MASK) != 0U)
    {
        return kStatus_I2C_StartStopError;
    }

    return kStatus_Success;
}

/*!
 * brief Performs a master polling transfer on the I2C bus.
 *
 * note The API does not return until the transfer succeeds or fails due
 * to arbitration lost or receiving a NAK.
 *
 * param base I2C peripheral base address.
 * param xfer Pointer to the transfer structure.
 * retval kStatus_Success Successfully complete the data transmission.
 * retval kStatus_I2C_Busy Previous transmission still not finished.
 * retval kStatus_I2C_Timeout Transfer error, wait signal timeout.
 * retval kStatus_I2C_ArbitrationLost Transfer error, arbitration lost.
 * retval kStataus_I2C_Nak Transfer error, receive NAK during transfer.
 */
status_t I2C_MasterTransferBlocking(I2C_Type *base, i2c_master_transfer_t *xfer)
{
    status_t result = kStatus_Success;
    uint32_t subaddress;
    uint8_t subaddrBuf[4];
    int i;

    assert(xfer);

    /* If repeated start is requested, send repeated start. */
    if ((xfer->flags & (uint32_t)kI2C_TransferNoStartFlag) == 0U)
    {
        if (xfer->subaddressSize != 0U)
        {
            result = I2C_MasterStart(base, (uint8_t)xfer->slaveAddress, kI2C_Write);
            if (result == kStatus_Success)
            {
                /* Prepare subaddress transmit buffer, most significant byte is stored at the lowest address */
                subaddress = xfer->subaddress;
                for (i = (int)xfer->subaddressSize - 1; i >= 0; i--)
                {
                    subaddrBuf[i] = (uint8_t)subaddress & 0xffU;
                    subaddress >>= 8;
                }
                /* Send subaddress. */
                result =
                    I2C_MasterWriteBlocking(base, subaddrBuf, xfer->subaddressSize, (uint32_t)kI2C_TransferNoStopFlag);
                if ((result == kStatus_Success) && (xfer->direction == kI2C_Read))
                {
                    result = I2C_MasterRepeatedStart(base, (uint8_t)xfer->slaveAddress, xfer->direction);
                }
            }
        }
        else if ((xfer->flags & (uint32_t)kI2C_TransferRepeatedStartFlag) != 0U)
        {
            result = I2C_MasterRepeatedStart(base, (uint8_t)xfer->slaveAddress, xfer->direction);
        }
        else
        {
            result = I2C_MasterStart(base, (uint8_t)xfer->slaveAddress, xfer->direction);
        }
    }

    if (result == kStatus_Success)
    {
        if ((xfer->direction == kI2C_Write) && (xfer->dataSize > 0U))
        {
            /* Transmit data. */
            result = I2C_MasterWriteBlocking(base, xfer->data, xfer->dataSize, xfer->flags);
        }
        else
        {
            if ((xfer->direction == kI2C_Read) && (xfer->dataSize > 0U))
            {
                /* Receive Data. */
                result = I2C_MasterReadBlocking(base, xfer->data, xfer->dataSize, xfer->flags);
            }
        }
    }

    if (result == kStatus_I2C_Nak)
    {
        (void)I2C_MasterStop(base);
    }

    return result;
}




/*!
 * @brief Performs a polling send transfer on the I2C bus.
 *
 * Sends up to @a txSize number of bytes to the previously addressed slave device. The slave may
 * reply with a NAK to any byte in order to terminate the transfer early. If this happens, this
 * function returns #kStatus_I2C_Nak.
 *
 * @param base  The I2C peripheral base address.
 * @param txBuff The pointer to the data to be transferred.
 * @param txSize The length in bytes of the data to be transferred.
 * @param flags Transfer control flag to control special behavior like suppressing start or stop, for normal transfers
 * use kI2C_TransferDefaultFlag
 * @retval kStatus_Success Data was sent successfully.
 * @retval #kStatus_I2C_Busy Another master is currently utilizing the bus.
 * @retval #kStatus_I2C_Nak The slave device sent a NAK in response to a byte.
 * @retval #kStatus_I2C_ArbitrationLost Arbitration lost error.
 */

//Seguir desde acá

//Adaptadas desde AVR


void i2c_start(uint8_t addr, i2c_direction_t dir)
{
    I2C_MasterStart(I2C1, addr, dir);
}

void i2c_stop(void)
{
    // i2c stop
	I2C_MasterStop(I2C1);
}

void i2c_byte(uint8_t byte)
{
	uint8_t * ptr = &byte;
	I2C_MasterWriteBlocking(I2C1, ptr, sizeof(byte), kI2C_TransferNoStartFlag | kI2C_TransferNoStopFlag);

}

uint8_t i2c_read_byte( void )
{
	I2C_MasterReadBlocking(I2C1, &rxBuff, sizeof(rxBuff), kI2C_TransferDefaultFlag);
	return rxBuff;
}


void i2c_init(void)
{
	i2c_master_config_t  i2config;                                    // config. variable. i2c

	CLOCK_Select(kI2C1_Clk_From_MainClk);
	CLOCK_EnableClock(kCLOCK_Swm);
	SWM_SetMovablePinSelect(SWM0, kSWM_I2C1_SDA, kSWM_PortPin_P0_11);  // SDA to PIO0_11
	SWM_SetMovablePinSelect(SWM0, kSWM_I2C1_SCL, kSWM_PortPin_P0_10);  // SCL to PIO0_10
	//CLOCK_DisableClock(kCLOCK_Swm);

	I2C_MasterGetDefaultConfig(&i2config);                             // get the default config of i2c

//------DEFAULT CONFIG---------------
//	  masterConfig.enableMaster  = true;
//    masterConfig.baudRate_Bps  = 100000U;
//    masterConfig.enableTimeout = false;


	i2config.baudRate_Bps = BAUDRATE;                                  //

	I2C_MasterInit(I2C1, &i2config, FRECUENCY);                        //initialization



}

uint8_t GetDemora_IIC( void )
{
	return demora_i2c;
}
void SetDemora_IIC( uint8_t value )
{
	demora_i2c = value;
}

//Otras necesarias


void RESET_SetPeripheralReset(reset_ip_name_t peripheral)
{
    const uint32_t regIndex = ((uint32_t)peripheral & 0xFFFF0000u) >> 16;
    const uint32_t bitPos = ((uint32_t)peripheral & 0x0000FFFFu);
    const uint32_t bitMask = 1UL << bitPos;

    assert(bitPos < 32UL);

    /* reset register is in SYSCON */

    if (0u == regIndex)
    {
        /* set bit */
        SYSCON->PRESETCTRL0 &= ~bitMask;
    }
    if (1u == regIndex)
    {
        /* set bit */
        SYSCON->PRESETCTRL1 &= ~bitMask;
    }
}

void RESET_ClearPeripheralReset(reset_ip_name_t peripheral)
{
    const uint32_t regIndex = ((uint32_t)peripheral & 0xFFFF0000u) >> 16;
    const uint32_t bitPos = ((uint32_t)peripheral & 0x0000FFFFu);
    const uint32_t bitMask = 1UL << bitPos;

    assert(bitPos < 32UL);

    /* reset register is in SYSCON */

    if (0u == regIndex)
    {
        /* clear bit */
        SYSCON->PRESETCTRL0 |= bitMask;
    }
    if (1u == regIndex)
    {
        /* clear bit */
        SYSCON->PRESETCTRL1 |= bitMask;
    }
}


/*!
 * brief Assignment of digital peripheral functions to pins.
 *
 * This function will selects a pin (designated by its GPIO port and
 * bit numbers) to a function.
 *
 * param base SWM peripheral base address.
 * param func any function name that is movable.
 * param swm_port_pin any pin which has a GPIO port number and bit number.
 */
void SWM_SetMovablePinSelect(SWM_Type *base, swm_select_movable_t func, swm_port_pin_type_t swm_port_pin)
{
    /* Check arguments */
    assert(NULL != base);
    assert(func < kSWM_MOVABLE_NUM_FUNCS);

    uint32_t temp;
    uint32_t pinassign = 0;
    uint32_t shifter   = 0;

    pinassign = (uint32_t)func / 4U;
    shifter   = ((uint32_t)func % 4U) * 8U;

    temp = base->PINASSIGN_DATA[pinassign];
    temp &= ~(0xFFUL << (shifter));
    temp |= ((uint32_t)swm_port_pin << shifter);
    base->PINASSIGN_DATA[pinassign] = temp;
}



