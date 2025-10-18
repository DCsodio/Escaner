/*
 * i2c.h
 *
 *  Created on: 12 ago. 2021
 *      Author: mariano
 */

#ifndef I2C_H_
#define I2C_H_

#include "../../../inc/LPC845.h"
#include "../../../inc/LPC845v2.h"
#include<stdbool.h>
#include"assert.h"
#include<string.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define BAUDRATE	100000
#define FRECUENCY	(2400000UL)

typedef int32_t status_t;
/* register offset */
#define SYS_AHB_CLK_CTRL0 (0U)
#define SYS_AHB_CLK_CTRL1 (4U)
/*! @brief Internal used Clock definition only. */
#define CLK_GATE_DEFINE(reg, bit)  ((((reg)&0xFFU) << 8U) | ((bit)&0xFFU))
/*! @brief Computes the number of elements in an array. */
#if !defined(ARRAY_SIZE)
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

/*! @brief Retry times for waiting flag. */
#ifndef I2C_RETRY_TIMES
#define I2C_RETRY_TIMES 0U /* Define to zero means keep waiting until the flag is assert/deassert. */
#endif

/* definitions for MSTCODE bits in I2C Status register STAT */
#define I2C_STAT_MSTCODE_IDLE    (0)   /*!< Master Idle State Code */
#define I2C_STAT_MSTCODE_RXREADY (1UL) /*!< Master Receive Ready State Code */
#define I2C_STAT_MSTCODE_TXREADY (2UL) /*!< Master Transmit Ready State Code */
#define I2C_STAT_MSTCODE_NACKADR (3UL) /*!< Master NACK by slave on address State Code */
#define I2C_STAT_MSTCODE_NACKDAT (4UL) /*!< Master NACK by slave on data State Code */

/* definitions for SLVSTATE bits in I2C Status register STAT */
#define I2C_STAT_SLVST_ADDR (0)
#define I2C_STAT_SLVST_RX   (1)
#define I2C_STAT_SLVST_TX   (2)


/*! @brief Construct a status code value from a group and code number. */
#define MAKE_STATUS(group, code) ((((group)*100) + (code)))

#define CLK_MUX_DEFINE(reg, mux)             (((offsetof(SYSCON_Type, reg) & 0xFFU) << 8U) | ((mux)&0xFFU))
#define CLK_MUX_GET_REG(x)                   ((volatile uint32_t *)(((uint32_t)(SYSCON)) + (((uint32_t)(x) >> 8U) & 0xFFU)))
#define CLK_MUX_GET_MUX(x)                   (((uint32_t)(x)) & 0xFFU)
#define CLK_MAIN_CLK_MUX_DEFINE(preMux, mux) ((preMux) << 8U | (mux))
#define CLK_MAIN_CLK_MUX_GET_PRE_MUX(x)      (((uint32_t)(x) >> 8U) & 0xFFU)
#define CLK_MAIN_CLK_MUX_GET_MUX(x)          ((uint32_t)(x)&0xFFU)

/*! @brief Internal used Clock definition only. */
#define CLK_GATE_DEFINE(reg, bit)  ((((reg)&0xFFU) << 8U) | ((bit)&0xFFU))
#define CLK_GATE_GET_REG(x)        (((uint32_t)(x) >> 8U) & 0xFFU)
#define CLK_GATE_GET_BITS_SHIFT(x) ((uint32_t)(x)&0xFFU)

#define I2C_CFG_MASK 0x1fUL

/*! @brief Direction of master and slave transfers. */
typedef enum _i2c_direction
{
    kI2C_Write = 0U, /*!< Master transmit. */
    kI2C_Read  = 1U  /*!< Master receive. */
} i2c_direction_t;



/*! @brief Status group numbers. */
enum _status_groups
{
    kStatusGroup_Generic = 0,                 /*!< Group number for generic status codes. */
    kStatusGroup_FLASH = 1,                   /*!< Group number for FLASH status codes. */
    kStatusGroup_LPSPI = 4,                   /*!< Group number for LPSPI status codes. */
    kStatusGroup_FLEXIO_SPI = 5,              /*!< Group number for FLEXIO SPI status codes. */
    kStatusGroup_DSPI = 6,                    /*!< Group number for DSPI status codes. */
    kStatusGroup_FLEXIO_UART = 7,             /*!< Group number for FLEXIO UART status codes. */
    kStatusGroup_FLEXIO_I2C = 8,              /*!< Group number for FLEXIO I2C status codes. */
    kStatusGroup_LPI2C = 9,                   /*!< Group number for LPI2C status codes. */
    kStatusGroup_UART = 10,                   /*!< Group number for UART status codes. */
    kStatusGroup_I2C = 11,                    /*!< Group number for UART status codes. */
    kStatusGroup_LPSCI = 12,                  /*!< Group number for LPSCI status codes. */
    kStatusGroup_LPUART = 13,                 /*!< Group number for LPUART status codes. */
    kStatusGroup_SPI = 14,                    /*!< Group number for SPI status code.*/
    kStatusGroup_XRDC = 15,                   /*!< Group number for XRDC status code.*/
    kStatusGroup_SEMA42 = 16,                 /*!< Group number for SEMA42 status code.*/
    kStatusGroup_SDHC = 17,                   /*!< Group number for SDHC status code */
    kStatusGroup_SDMMC = 18,                  /*!< Group number for SDMMC status code */
    kStatusGroup_SAI = 19,                    /*!< Group number for SAI status code */
    kStatusGroup_MCG = 20,                    /*!< Group number for MCG status codes. */
    kStatusGroup_SCG = 21,                    /*!< Group number for SCG status codes. */
    kStatusGroup_SDSPI = 22,                  /*!< Group number for SDSPI status codes. */
    kStatusGroup_FLEXIO_I2S = 23,             /*!< Group number for FLEXIO I2S status codes */
    kStatusGroup_FLEXIO_MCULCD = 24,          /*!< Group number for FLEXIO LCD status codes */
    kStatusGroup_FLASHIAP = 25,               /*!< Group number for FLASHIAP status codes */
    kStatusGroup_FLEXCOMM_I2C = 26,           /*!< Group number for FLEXCOMM I2C status codes */
    kStatusGroup_I2S = 27,                    /*!< Group number for I2S status codes */
    kStatusGroup_IUART = 28,                  /*!< Group number for IUART status codes */
    kStatusGroup_CSI = 29,                    /*!< Group number for CSI status codes */
    kStatusGroup_MIPI_DSI = 30,               /*!< Group number for MIPI DSI status codes */
    kStatusGroup_SDRAMC = 35,                 /*!< Group number for SDRAMC status codes. */
    kStatusGroup_POWER = 39,                  /*!< Group number for POWER status codes. */
    kStatusGroup_ENET = 40,                   /*!< Group number for ENET status codes. */
    kStatusGroup_PHY = 41,                    /*!< Group number for PHY status codes. */
    kStatusGroup_TRGMUX = 42,                 /*!< Group number for TRGMUX status codes. */
    kStatusGroup_SMARTCARD = 43,              /*!< Group number for SMARTCARD status codes. */
    kStatusGroup_LMEM = 44,                   /*!< Group number for LMEM status codes. */
    kStatusGroup_QSPI = 45,                   /*!< Group number for QSPI status codes. */
    kStatusGroup_DMA = 50,                    /*!< Group number for DMA status codes. */
    kStatusGroup_EDMA = 51,                   /*!< Group number for EDMA status codes. */
    kStatusGroup_DMAMGR = 52,                 /*!< Group number for DMAMGR status codes. */
    kStatusGroup_FLEXCAN = 53,                /*!< Group number for FlexCAN status codes. */
    kStatusGroup_LTC = 54,                    /*!< Group number for LTC status codes. */
    kStatusGroup_FLEXIO_CAMERA = 55,          /*!< Group number for FLEXIO CAMERA status codes. */
    kStatusGroup_LPC_SPI = 56,                /*!< Group number for LPC_SPI status codes. */
    kStatusGroup_LPC_USART = 57,              /*!< Group number for LPC_USART status codes. */
    kStatusGroup_DMIC = 58,                   /*!< Group number for DMIC status codes. */
    kStatusGroup_SDIF = 59,                   /*!< Group number for SDIF status codes.*/
    kStatusGroup_SPIFI = 60,                  /*!< Group number for SPIFI status codes. */
    kStatusGroup_OTP = 61,                    /*!< Group number for OTP status codes. */
    kStatusGroup_MCAN = 62,                   /*!< Group number for MCAN status codes. */
    kStatusGroup_CAAM = 63,                   /*!< Group number for CAAM status codes. */
    kStatusGroup_ECSPI = 64,                  /*!< Group number for ECSPI status codes. */
    kStatusGroup_USDHC = 65,                  /*!< Group number for USDHC status codes.*/
    kStatusGroup_LPC_I2C = 66,                /*!< Group number for LPC_I2C status codes.*/
    kStatusGroup_DCP = 67,                    /*!< Group number for DCP status codes.*/
    kStatusGroup_MSCAN = 68,                  /*!< Group number for MSCAN status codes.*/
    kStatusGroup_ESAI = 69,                   /*!< Group number for ESAI status codes. */
    kStatusGroup_FLEXSPI = 70,                /*!< Group number for FLEXSPI status codes. */
    kStatusGroup_MMDC = 71,                   /*!< Group number for MMDC status codes. */
    kStatusGroup_PDM = 72,                    /*!< Group number for MIC status codes. */
    kStatusGroup_SDMA = 73,                   /*!< Group number for SDMA status codes. */
    kStatusGroup_ICS = 74,                    /*!< Group number for ICS status codes. */
    kStatusGroup_SPDIF = 75,                  /*!< Group number for SPDIF status codes. */
    kStatusGroup_LPC_MINISPI = 76,            /*!< Group number for LPC_MINISPI status codes. */
    kStatusGroup_HASHCRYPT = 77,              /*!< Group number for Hashcrypt status codes */
    kStatusGroup_LPC_SPI_SSP = 78,            /*!< Group number for LPC_SPI_SSP status codes. */
    kStatusGroup_I3C = 79,                    /*!< Group number for I3C status codes */
    kStatusGroup_LPC_I2C_1 = 97,              /*!< Group number for LPC_I2C_1 status codes. */
    kStatusGroup_NOTIFIER = 98,               /*!< Group number for NOTIFIER status codes. */
    kStatusGroup_DebugConsole = 99,           /*!< Group number for debug console status codes. */
    kStatusGroup_SEMC = 100,                  /*!< Group number for SEMC status codes. */
    kStatusGroup_ApplicationRangeStart = 101, /*!< Starting number for application groups. */
    kStatusGroup_IAP = 102,                   /*!< Group number for IAP status codes */
    kStatusGroup_SFA = 103,                   /*!< Group number for SFA status codes*/
    kStatusGroup_SPC = 104,                   /*!< Group number for SPC status codes. */
    kStatusGroup_PUF = 105,                   /*!< Group number for PUF status codes. */
    kStatusGroup_TOUCH_PANEL = 106,           /*!< Group number for touch panel status codes */

    kStatusGroup_HAL_GPIO = 121,              /*!< Group number for HAL GPIO status codes. */
    kStatusGroup_HAL_UART = 122,              /*!< Group number for HAL UART status codes. */
    kStatusGroup_HAL_TIMER = 123,             /*!< Group number for HAL TIMER status codes. */
    kStatusGroup_HAL_SPI = 124,               /*!< Group number for HAL SPI status codes. */
    kStatusGroup_HAL_I2C = 125,               /*!< Group number for HAL I2C status codes. */
    kStatusGroup_HAL_FLASH = 126,             /*!< Group number for HAL FLASH status codes. */
    kStatusGroup_HAL_PWM = 127,               /*!< Group number for HAL PWM status codes. */
    kStatusGroup_HAL_RNG = 128,               /*!< Group number for HAL RNG status codes. */
    kStatusGroup_TIMERMANAGER = 135,          /*!< Group number for TiMER MANAGER status codes. */
    kStatusGroup_SERIALMANAGER = 136,         /*!< Group number for SERIAL MANAGER status codes. */
    kStatusGroup_LED = 137,                   /*!< Group number for LED status codes. */
    kStatusGroup_BUTTON = 138,                /*!< Group number for BUTTON status codes. */
    kStatusGroup_EXTERN_EEPROM = 139,         /*!< Group number for EXTERN EEPROM status codes. */
    kStatusGroup_SHELL = 140,                 /*!< Group number for SHELL status codes. */
    kStatusGroup_MEM_MANAGER = 141,           /*!< Group number for MEM MANAGER status codes. */
    kStatusGroup_LIST = 142,                  /*!< Group number for List status codes. */
    kStatusGroup_OSA = 143,                   /*!< Group number for OSA status codes. */
    kStatusGroup_COMMON_TASK = 144,           /*!< Group number for Common task status codes. */
    kStatusGroup_MSG = 145,                   /*!< Group number for messaging status codes. */
    kStatusGroup_SDK_OCOTP = 146,             /*!< Group number for OCOTP status codes. */
    kStatusGroup_SDK_FLEXSPINOR = 147,        /*!< Group number for FLEXSPINOR status codes.*/
    kStatusGroup_CODEC = 148,                 /*!< Group number for codec status codes. */
    kStatusGroup_ASRC = 149,                  /*!< Group number for codec status ASRC. */
    kStatusGroup_OTFAD = 150,                 /*!< Group number for codec status codes. */
    kStatusGroup_SDIOSLV = 151,               /*!< Group number for SDIOSLV status codes. */
    kStatusGroup_MECC = 152,                  /*!< Group number for MECC status codes. */
    kStatusGroup_ENET_QOS = 153,              /*!< Group number for ENET_QOS status codes. */
    kStatusGroup_LOG = 154,                   /*!< Group number for LOG status codes. */
};

/*! \public
 * @brief Generic status return codes.
 */
enum
{
    kStatus_Success = MAKE_STATUS(kStatusGroup_Generic, 0),  /*!< Generic status for Success. */
    kStatus_Fail = MAKE_STATUS(kStatusGroup_Generic, 1),      /*!< Generic status for Fail. */
    kStatus_ReadOnly = MAKE_STATUS(kStatusGroup_Generic, 2),    /*!< Generic status for read only failure. */
    kStatus_OutOfRange = MAKE_STATUS(kStatusGroup_Generic, 3),   /*!< Generic status for out of range access. */
    kStatus_InvalidArgument = MAKE_STATUS(kStatusGroup_Generic, 4),   /*!< Generic status for invalid argument check. */
    kStatus_Timeout = MAKE_STATUS(kStatusGroup_Generic, 5),   /*!< Generic status for timeout. */
    kStatus_NoTransferInProgress = MAKE_STATUS(kStatusGroup_Generic, 6),   /*!< Generic status for no transfer in progress. */
};

/*! @brief I2C status return codes. */
enum
{
    kStatus_I2C_Busy = MAKE_STATUS(kStatusGroup_LPC_I2C, 0), /*!< The master is already performing a transfer. */
    kStatus_I2C_Idle = MAKE_STATUS(kStatusGroup_LPC_I2C, 1), /*!< The slave driver is idle. */
    kStatus_I2C_Nak  = MAKE_STATUS(kStatusGroup_LPC_I2C, 2), /*!< The slave device sent a NAK in response to a byte. */
    kStatus_I2C_InvalidParameter =
        MAKE_STATUS(kStatusGroup_LPC_I2C, 3), /*!< Unable to proceed due to invalid parameter. */
    kStatus_I2C_BitError        = MAKE_STATUS(kStatusGroup_LPC_I2C, 4), /*!< Transferred bit was not seen on the bus. */
    kStatus_I2C_ArbitrationLost = MAKE_STATUS(kStatusGroup_LPC_I2C, 5), /*!< Arbitration lost error. */
    kStatus_I2C_NoTransferInProgress =
        MAKE_STATUS(kStatusGroup_LPC_I2C, 6), /*!< Attempt to abort a transfer when one is not in progress. */
    kStatus_I2C_DmaRequestFail  = MAKE_STATUS(kStatusGroup_LPC_I2C, 7),  /*!< DMA request failed. */
    kStatus_I2C_StartStopError  = MAKE_STATUS(kStatusGroup_LPC_I2C, 8),  /*!< Start and stop error. */
    kStatus_I2C_UnexpectedState = MAKE_STATUS(kStatusGroup_LPC_I2C, 9),  /*!< Unexpected state. */
    kStatus_I2C_Addr_Nak        = MAKE_STATUS(kStatusGroup_LPC_I2C, 10), /*!< NAK received during the address probe. */
    kStatus_I2C_Timeout         = MAKE_STATUS(kStatusGroup_LPC_I2C, 11), /*!< Timeout polling status flags. */
};


/*! @brief SWM port_pin number */
typedef enum _swm_port_pin_type_t
{
    kSWM_PortPin_P0_0 = 0U,   /*!< port_pin number P0_0. */
    kSWM_PortPin_P0_1 = 1U,   /*!< port_pin number P0_1. */
    kSWM_PortPin_P0_2 = 2U,   /*!< port_pin number P0_2. */
    kSWM_PortPin_P0_3 = 3U,   /*!< port_pin number P0_3. */
    kSWM_PortPin_P0_4 = 4U,   /*!< port_pin number P0_4. */
    kSWM_PortPin_P0_5 = 5U,   /*!< port_pin number P0_5. */
    kSWM_PortPin_P0_6 = 6U,   /*!< port_pin number P0_6. */
    kSWM_PortPin_P0_7 = 7U,   /*!< port_pin number P0_7. */
    kSWM_PortPin_P0_8 = 8U,   /*!< port_pin number P0_8. */
    kSWM_PortPin_P0_9 = 9U,   /*!< port_pin number P0_9. */
    kSWM_PortPin_P0_10 = 10U, /*!< port_pin number P0_10. */
    kSWM_PortPin_P0_11 = 11U, /*!< port_pin number P0_11. */
    kSWM_PortPin_P0_12 = 12U, /*!< port_pin number P0_12. */
    kSWM_PortPin_P0_13 = 13U, /*!< port_pin number P0_13. */
    kSWM_PortPin_P0_14 = 14U, /*!< port_pin number P0_14. */
    kSWM_PortPin_P0_15 = 15U, /*!< port_pin number P0_15. */
    kSWM_PortPin_P0_16 = 16U, /*!< port_pin number P0_16. */
    kSWM_PortPin_P0_17 = 17U, /*!< port_pin number P0_17. */
    kSWM_PortPin_P0_18 = 18U, /*!< port_pin number P0_18. */
    kSWM_PortPin_P0_19 = 19U, /*!< port_pin number P0_19. */
    kSWM_PortPin_P0_20 = 20U, /*!< port_pin number P0_20. */
    kSWM_PortPin_P0_21 = 21U, /*!< port_pin number P0_21. */
    kSWM_PortPin_P0_22 = 22U, /*!< port_pin number P0_22. */
    kSWM_PortPin_P0_23 = 23U, /*!< port_pin number P0_23. */
    kSWM_PortPin_P0_24 = 24U, /*!< port_pin number P0_24. */
    kSWM_PortPin_P0_25 = 25U, /*!< port_pin number P0_25. */
    kSWM_PortPin_P0_26 = 26U, /*!< port_pin number P0_26. */
    kSWM_PortPin_P0_27 = 27U, /*!< port_pin number P0_27. */
    kSWM_PortPin_P0_28 = 28U, /*!< port_pin number P0_28. */
    kSWM_PortPin_P0_29 = 29U, /*!< port_pin number P0_29. */
    kSWM_PortPin_P0_30 = 30U, /*!< port_pin number P0_30. */
    kSWM_PortPin_P0_31 = 31U, /*!< port_pin number P0_31. */

    kSWM_PortPin_P1_0 = 32U,  /*!< port_pin number P1_0. */
    kSWM_PortPin_P1_1 = 33U,  /*!< port_pin number P1_1. */
    kSWM_PortPin_P1_2 = 34U,  /*!< port_pin number P1_2. */
    kSWM_PortPin_P1_3 = 35U,  /*!< port_pin number P1_3. */
    kSWM_PortPin_P1_4 = 36U,  /*!< port_pin number P1_4. */
    kSWM_PortPin_P1_5 = 37U,  /*!< port_pin number P1_5. */
    kSWM_PortPin_P1_6 = 38U,  /*!< port_pin number P1_6. */
    kSWM_PortPin_P1_7 = 39U,  /*!< port_pin number P1_7. */
    kSWM_PortPin_P1_8 = 40U,  /*!< port_pin number P1_8. */
    kSWM_PortPin_P1_9 = 41U,  /*!< port_pin number P1_9. */
    kSWM_PortPin_P1_10 = 42U, /*!< port_pin number P1_10. */
    kSWM_PortPin_P1_11 = 43U, /*!< port_pin number P1_11. */
    kSWM_PortPin_P1_12 = 44U, /*!< port_pin number P1_12. */
    kSWM_PortPin_P1_13 = 45U, /*!< port_pin number P1_13. */
    kSWM_PortPin_P1_14 = 46U, /*!< port_pin number P1_14. */
    kSWM_PortPin_P1_15 = 47U, /*!< port_pin number P1_15. */
    kSWM_PortPin_P1_16 = 48U, /*!< port_pin number P1_16. */
    kSWM_PortPin_P1_17 = 49U, /*!< port_pin number P1_17. */
    kSWM_PortPin_P1_18 = 50U, /*!< port_pin number P1_18. */
    kSWM_PortPin_P1_19 = 51U, /*!< port_pin number P1_19. */
    kSWM_PortPin_P1_20 = 52U, /*!< port_pin number P1_20. */
    kSWM_PortPin_P1_21 = 53U, /*!< port_pin number P1_21. */
    kSWM_PortPin_Reset = 0xffU /*!< port_pin reset number. */
} swm_port_pin_type_t;



/*! @brief SWM movable selection */
typedef enum _swm_select_movable_t
{
    kSWM_USART0_TXD = 0U,  /*!< Movable function as USART0_TXD. */
    kSWM_USART0_RXD = 1U,  /*!< Movable function as USART0_RXD. */
    kSWM_USART0_RTS = 2U,  /*!< Movable function as USART0_RTS. */
    kSWM_USART0_CTS = 3U,  /*!< Movable function as USART0_CTS. */
    kSWM_USART0_SCLK = 4U, /*!< Movable function as USART0_SCLK. */

    kSWM_USART1_TXD = 5U,  /*!< Movable function as USART1_TXD. */
    kSWM_USART1_RXD = 6U,  /*!< Movable function as USART1_RXD. */
    kSWM_USART1_RTS = 7U,  /*!< Movable function as USART1_RTS. */
    kSWM_USART1_CTS = 8U,  /*!< Movable function as USART1_CTS. */
    kSWM_USART1_SCLK = 9U, /*!< Movable function as USART1_SCLK. */

    kSWM_USART2_TXD = 10U,  /*!< Movable function as USART2_TXD. */
    kSWM_USART2_RXD = 11U,  /*!< Movable function as USART2_RXD. */
    kSWM_USART2_RTS = 12U,  /*!< Movable function as USART2_RTS. */
    kSWM_USART2_CTS = 13U,  /*!< Movable function as USART2_CTS. */
    kSWM_USART2_SCLK = 14U, /*!< Movable function as USART2_SCLK. */

    kSWM_SPI0_SCK = 15U,   /*!< Movable function as SPI0_SCK. */
    kSWM_SPI0_MOSI = 16U,  /*!< Movable function as SPI0_MOSI. */
    kSWM_SPI0_MISO = 17U,  /*!< Movable function as SPI0_MISO. */
    kSWM_SPI0_SSEL0 = 18U, /*!< Movable function as SPI0_SSEL0. */
    kSWM_SPI0_SSEL1 = 19U, /*!< Movable function as SPI0_SSEL1. */
    kSWM_SPI0_SSEL2 = 20U, /*!< Movable function as SPI0_SSEL2. */
    kSWM_SPI0_SSEL3 = 21U, /*!< Movable function as SPI0_SSEL3. */

    kSWM_SPI1_SCK = 22U,   /*!< Movable function as SPI1_SCK. */
    kSWM_SPI1_MOSI = 23U,  /*!< Movable function as SPI1_MOSI. */
    kSWM_SPI1_MISO = 24U,  /*!< Movable function as SPI1_MISO. */
    kSWM_SPI1_SSEL0 = 25U, /*!< Movable function as SPI1_SSEL0. */
    kSWM_SPI1_SSEL1 = 26U, /*!< Movable function as SPI1_SSEL1. */

    kSWM_SCT_PIN0 = 27U, /*!< Movable function as SCT_PIN0. */
    kSWM_SCT_PIN1 = 28U, /*!< Movable function as SCT_PIN1. */
    kSWM_SCT_PIN2 = 29U, /*!< Movable function as SCT_PIN2. */
    kSWM_SCT_PIN3 = 30U, /*!< Movable function as SCT_PIN3. */
    kSWM_SCT_OUT0 = 31U, /*!< Movable function as SCT_OUT0. */
    kSWM_SCT_OUT1 = 32U, /*!< Movable function as SCT_OUT1. */
    kSWM_SCT_OUT2 = 33U, /*!< Movable function as SCT_OUT2. */
    kSWM_SCT_OUT3 = 34U, /*!< Movable function as SCT_OUT3. */
    kSWM_SCT_OUT4 = 35U, /*!< Movable function as SCT_OUT4. */
    kSWM_SCT_OUT5 = 36U, /*!< Movable function as SCT_OUT5. */
    kSWM_SCT_OUT6 = 37U, /*!< Movable function as SCT_OUT6. */

    kSWM_I2C1_SDA = 38U, /*!< Movable function as I2C1_SDA. */
    kSWM_I2C1_SCL = 39U, /*!< Movable function as I2C1_SCL. */
    kSWM_I2C2_SDA = 40U, /*!< Movable function as I2C2_SDA. */
    kSWM_I2C2_SCL = 41U, /*!< Movable function as I2C2_SCL. */
    kSWM_I2C3_SDA = 42U, /*!< Movable function as I2C3_SDA. */
    kSWM_I2C3_SCL = 43U, /*!< Movable function as I2C3_SCL. */

    kSWM_ACMP_OUT = 44U,      /*!< Movable function as ACMP_OUT. */
    kSWM_CLKOUT = 45U,        /*!< Movable function as CLKOUT. */
    kSWM_GPIO_INT_BMAT = 46U, /*!< Movable function as GPIO_INT_BMAT. */

    kSWM_USART3_TXD = 47U,  /*!< Movable function as USART3_TXD. */
    kSWM_USART3_RXD = 48U,  /*!< Movable function as USART3_RXD. */
    kSWM_USART3_SCLK = 49U, /*!< Movable function as USART3_SCLK. */
    kSWM_USART4_TXD = 50U,  /*!< Movable function as USART4_TXD. */
    kSWM_USART4_RXD = 51U,  /*!< Movable function as USART4_RXD. */
    kSWM_USART4_SCLK = 52U, /*!< Movable function as USART4_SCLK. */
    kSWM_T0_MAT_CHN0 = 53U, /*!< Movable function as Timer Match Channel 0. */
    kSWM_T0_MAT_CHN1 = 54U, /*!< Movable function as Timer Match Channel 1. */
    kSWM_T0_MAT_CHN2 = 55U, /*!< Movable function as Timer Match Channel 2. */
    kSWM_T0_MAT_CHN3 = 56U, /*!< Movable function as Timer Match Channel 3. */
    kSWM_T0_CAP_CHN0 = 57U, /*!< Movable function as Timer Capture Channel 0. */
    kSWM_T0_CAP_CHN1 = 58U, /*!< Movable function as Timer Capture Channel 1. */
    kSWM_T0_CAP_CHN2 = 59U, /*!< Movable function as Timer Capture Channel 2. */

    kSWM_MOVABLE_NUM_FUNCS = 60U, /*!< Movable function number. */
} swm_select_movable_t;


/*!
 * @brief Transfer option flags.
 *
 * @note These enumerations are intended to be OR'd together to form a bit mask of options for
 * the #_i2c_master_transfer::flags field.
 */
enum _i2c_master_transfer_flags
{
    kI2C_TransferDefaultFlag       = 0x00U, /*!< Transfer starts with a start signal, stops with a stop signal. */
    kI2C_TransferNoStartFlag       = 0x01U, /*!< Don't send a start condition, address, and sub address */
    kI2C_TransferRepeatedStartFlag = 0x02U, /*!< Send a repeated start condition */
    kI2C_TransferNoStopFlag        = 0x04U, /*!< Don't send a stop condition. */
};

/*!
 * @brief I2C master peripheral flags.
 *
 * @note These enums are meant to be OR'd together to form a bit mask.
 */
enum _i2c_master_flags
{
    kI2C_MasterPendingFlag = I2C_STAT_MSTPENDING_MASK, /*!< The I2C module is waiting for software interaction. */
    kI2C_MasterArbitrationLostFlag =
        I2C_STAT_MSTARBLOSS_MASK, /*!< The arbitration of the bus was lost. There was collision on the bus */
    kI2C_MasterStartStopErrorFlag =
        I2C_STAT_MSTSTSTPERR_MASK /*!< There was an error during start or stop phase of the transaction. */
};

/*!
 * @brief Non-blocking transfer descriptor structure.
 *
 * This structure is used to pass transaction parameters to the I2C_MasterTransferNonBlocking() API.
 */
struct _i2c_master_transfer
{
    uint32_t flags; /*!< Bit mask of options for the transfer. See enumeration #_i2c_master_transfer_flags for available
                       options. Set to 0 or #kI2C_TransferDefaultFlag for normal transfers. */
    uint16_t slaveAddress;     /*!< The 7-bit slave address. */
    i2c_direction_t direction; /*!< Either #kI2C_Read or #kI2C_Write. */
    uint32_t subaddress;       /*!< Sub address. Transferred MSB first. */
    size_t subaddressSize;     /*!< Length of sub address to send in bytes. Maximum size is 4 bytes. */
    void *data;                /*!< Pointer to data to transfer. */
    size_t dataSize;           /*!< Number of bytes to transfer. */
};

/* Forward declaration of the transfer descriptor and handle typedefs. */
/*! @brief I2C master transfer typedef */
typedef struct _i2c_master_transfer i2c_master_transfer_t;

/*! @brief I2C master handle typedef */
typedef struct _i2c_master_handle i2c_master_handle_t;


/*! @brief Clock ip name array for I2C. */
/*!
 * @brief Enumeration for peripheral reset control bits
 *
 * Defines the enumeration for peripheral reset control bits in PRESETCTRL/ASYNCPRESETCTRL registers
 */
typedef enum _SYSCON_RSTn
{
    kFLASH_RST_N_SHIFT_RSTn = 0 | 4U,    /**< Flash controller reset control */
    kI2C0_RST_N_SHIFT_RSTn = 0 | 5U,     /**< I2C0 reset control */
    kGPIO0_RST_N_SHIFT_RSTn = 0 | 6U,    /**< GPIO0 reset control */
    kSWM_RST_N_SHIFT_RSTn = 0 | 7U,      /**< SWM reset control */
    kSCT_RST_N_SHIFT_RSTn = 0 | 8U,      /**< SCT reset control */
    kWKT_RST_N_SHIFT_RSTn = 0 | 9U,      /**< Self-wake-up timer(WKT) reset control */
    kMRT_RST_N_SHIFT_RSTn = 0 | 10U,     /**< Multi-rate timer(MRT) reset control */
    kSPI0_RST_N_SHIFT_RSTn = 0 | 11U,    /**< SPI0 reset control. */
    kSPI1_RST_N_SHIFT_RSTn = 0 | 12U,    /**< SPI1 reset control */
    kCRC_RST_SHIFT_RSTn = 0 | 13U,     /**< CRC reset control */
    kUART0_RST_N_SHIFT_RSTn = 0 | 14U,   /**< UART0 reset control */
    kUART1_RST_N_SHIFT_RSTn = 0 | 15U,   /**< UART1 reset control */
    kUART2_RST_N_SHIFT_RSTn = 0 | 16U,   /**< UART2 reset control */
    kIOCON_RST_N_SHIFT_RSTn = 0 | 18U,   /**< IOCON reset control */
    kACMP_RST_N_SHIFT_RSTn = 0 | 19U,    /**< Analog comparator reset control */
    kGPIO1_RST_N_SHIFT_RSTn = 0 | 20U,   /**< GPIO1 reset control */
    kI2C1_RST_N_SHIFT_RSTn = 0 | 21U,    /**< I2C1 reset control */
    kI2C2_RST_N_SHIFT_RSTn = 0 | 22U,    /**< I2C2 reset control */
    kI2C3_RST_N_SHIFT_RSTn = 0 | 23U,    /**< I2C3 reset control */
    kADC_RST_N_SHIFT_RSTn = 0 | 24U,     /**< ADC reset control */
    kCTIMER0_RST_N_SHIFT_RSTn = 0 | 25U, /**< CTIMER0 reset control */
    kDAC0_RST_N_SHIFT_RSTn = 0 | 27U,    /**< DAC0 reset control */
    kGPIOINT_RST_N_SHIFT_RSTn = 0 | 28U, /**< GPIOINT reset control */
    kDMA_RST_N_SHIFT_RSTn = 0 | 29U,     /**< DMA reset control */
    kUART3_RST_N_SHIFT_RSTn = 0 | 30U,   /**< UART3 reset control */
    kUART4_RST_N_SHIFT_RSTn = 0 | 31U,   /**< UART4 reset control */

    kCAPT_RST_N_SHIFT_RSTn = 65536 | 0U, /**< Capacitive Touch reset control */
    kDAC1_RST_N_SHIFT_RSTn = 65536 | 1U, /**< DAC1 reset control */
    kFRG0_RST_N_SHIFT_RSTn = 65536 | 3U, /**< Fractional baud rate generator 0 reset control */
    kFRG1_RST_N_SHIFT_RSTn = 65536 | 4U, /**< Fractional baud rate generator 1 reset control */

} SYSCON_RSTn_t;

typedef SYSCON_RSTn_t reset_ip_name_t;

#define I2C_CLOCKS                                          \
    {                                                       \
        kCLOCK_I2c0, kCLOCK_I2c1, kCLOCK_I2c2, kCLOCK_I2c3, \
    }
#define I2C_RSTS_N                                                                                     \
    {                                                                                                  \
        kI2C0_RST_N_SHIFT_RSTn, kI2C1_RST_N_SHIFT_RSTn, kI2C2_RST_N_SHIFT_RSTn, kI2C3_RST_N_SHIFT_RSTn \
    } /* Reset bits for I2C peripheral */
/*! @brief Clock gate name used for CLOCK_EnableClock/CLOCK_DisableClock. */
typedef enum _clock_ip_name
{
    kCLOCK_IpInvalid = 0U,
    kCLOCK_Rom       = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 1U),
    kCLOCK_Ram0_1    = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 2U),
    kCLOCK_I2c0      = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 5U),
    kCLOCK_Gpio0     = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 6U),
    kCLOCK_Swm       = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 7U),
    kCLOCK_Sct       = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 8U),
    kCLOCK_Wkt       = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 9U),
    kCLOCK_Mrt       = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 10U),
    kCLOCK_Spi0      = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 11U),
    kCLOCK_Spi1      = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 12U),
    kCLOCK_Crc       = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 13U),
    kCLOCK_Uart0     = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 14U),
    kCLOCK_Uart1     = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 15U),
    kCLOCK_Uart2     = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 16U),
    kCLOCK_Wwdt      = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 17U),
    kCLOCK_Iocon     = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 18U),
    kCLOCK_Acmp      = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 19U),
    kCLOCK_Gpio1     = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 20U),
    kCLOCK_I2c1      = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 21U),
    kCLOCK_I2c2      = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 22U),
    kCLOCK_I2c3      = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 23U),
    kCLOCK_Adc       = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 24U),
    kCLOCK_Ctimer0   = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 25U),
    kCLOCK_Mtb       = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 26U),
    kCLOCK_Dac0      = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 27U),
    kCLOCK_GpioInt   = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 28U),
    kCLOCK_Dma       = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 29U),
    kCLOCK_Uart3     = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 30U),
    kCLOCK_Uart4     = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL0, 31U),
    kCLOCK_Capt      = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL1, 0U),
    kCLOCK_Dac1      = CLK_GATE_DEFINE(SYS_AHB_CLK_CTRL1, 1U),
} clock_ip_name_t;

/*! @brief Clock Mux Switches
 *CLK_MUX_DEFINE(reg, mux)
 *reg is used to define the mux register
 *mux is used to define the mux value
 *
 */
typedef enum _clock_select
{
    kCAPT_Clk_From_Fro     = CLK_MUX_DEFINE(CAPTCLKSEL, 0U),
    kCAPT_Clk_From_MainClk = CLK_MUX_DEFINE(CAPTCLKSEL, 1U),
    kCAPT_Clk_From_SysPll  = CLK_MUX_DEFINE(CAPTCLKSEL, 2U),
    kCAPT_Clk_From_Fro_Div = CLK_MUX_DEFINE(CAPTCLKSEL, 3U),
    kCAPT_Clk_From_WdtOsc  = CLK_MUX_DEFINE(CAPTCLKSEL, 4U),

    kADC_Clk_From_Fro    = CLK_MUX_DEFINE(ADCCLKSEL, 0U),
    kADC_Clk_From_SysPll = CLK_MUX_DEFINE(ADCCLKSEL, 1U),

    kSCT_Clk_From_Fro     = CLK_MUX_DEFINE(SCTCLKSEL, 0U),
    kSCT_Clk_From_MainClk = CLK_MUX_DEFINE(SCTCLKSEL, 1U),
    kSCT_Clk_From_SysPll  = CLK_MUX_DEFINE(SCTCLKSEL, 2U),

    kEXT_Clk_From_SysOsc = CLK_MUX_DEFINE(EXTCLKSEL, 0U),
    kEXT_Clk_From_ClkIn  = CLK_MUX_DEFINE(EXTCLKSEL, 1U),

    kUART0_Clk_From_Fro     = CLK_MUX_DEFINE(FCLKSEL[0U], 0U),
    kUART0_Clk_From_MainClk = CLK_MUX_DEFINE(FCLKSEL[0U], 1U),
    kUART0_Clk_From_Frg0Clk = CLK_MUX_DEFINE(FCLKSEL[0U], 2U),
    kUART0_Clk_From_Frg1Clk = CLK_MUX_DEFINE(FCLKSEL[0U], 3U),
    kUART0_Clk_From_Fro_Div = CLK_MUX_DEFINE(FCLKSEL[0U], 4U),

    kUART1_Clk_From_Fro     = CLK_MUX_DEFINE(FCLKSEL[1U], 0U),
    kUART1_Clk_From_MainClk = CLK_MUX_DEFINE(FCLKSEL[1U], 1U),
    kUART1_Clk_From_Frg0Clk = CLK_MUX_DEFINE(FCLKSEL[1U], 2U),
    kUART1_Clk_From_Frg1Clk = CLK_MUX_DEFINE(FCLKSEL[1U], 3U),
    kUART1_Clk_From_Fro_Div = CLK_MUX_DEFINE(FCLKSEL[1U], 4U),

    kUART2_Clk_From_Fro     = CLK_MUX_DEFINE(FCLKSEL[2U], 0U),
    kUART2_Clk_From_MainClk = CLK_MUX_DEFINE(FCLKSEL[2U], 1U),
    kUART2_Clk_From_Frg0Clk = CLK_MUX_DEFINE(FCLKSEL[2U], 2U),
    kUART2_Clk_From_Frg1Clk = CLK_MUX_DEFINE(FCLKSEL[2U], 3U),
    kUART2_Clk_From_Fro_Div = CLK_MUX_DEFINE(FCLKSEL[2U], 4U),

    kUART3_Clk_From_Fro     = CLK_MUX_DEFINE(FCLKSEL[3U], 0U),
    kUART3_Clk_From_MainClk = CLK_MUX_DEFINE(FCLKSEL[3U], 1U),
    kUART3_Clk_From_Frg0Clk = CLK_MUX_DEFINE(FCLKSEL[3U], 2U),
    kUART3_Clk_From_Frg1Clk = CLK_MUX_DEFINE(FCLKSEL[3U], 3U),
    kUART3_Clk_From_Fro_Div = CLK_MUX_DEFINE(FCLKSEL[3U], 4U),

    kUART4_Clk_From_Fro     = CLK_MUX_DEFINE(FCLKSEL[4U], 0U),
    kUART4_Clk_From_MainClk = CLK_MUX_DEFINE(FCLKSEL[4U], 1U),
    kUART4_Clk_From_Frg0Clk = CLK_MUX_DEFINE(FCLKSEL[4U], 2U),
    kUART4_Clk_From_Frg1Clk = CLK_MUX_DEFINE(FCLKSEL[4U], 3U),
    kUART4_Clk_From_Fro_Div = CLK_MUX_DEFINE(FCLKSEL[4U], 4U),

    kI2C0_Clk_From_Fro     = CLK_MUX_DEFINE(FCLKSEL[5U], 0U),
    kI2C0_Clk_From_MainClk = CLK_MUX_DEFINE(FCLKSEL[5U], 1U),
    kI2C0_Clk_From_Frg0Clk = CLK_MUX_DEFINE(FCLKSEL[5U], 2U),
    kI2C0_Clk_From_Frg1Clk = CLK_MUX_DEFINE(FCLKSEL[5U], 3U),
    kI2C0_Clk_From_Fro_Div = CLK_MUX_DEFINE(FCLKSEL[5U], 4U),

    kI2C1_Clk_From_Fro     = CLK_MUX_DEFINE(FCLKSEL[6U], 0U),
    kI2C1_Clk_From_MainClk = CLK_MUX_DEFINE(FCLKSEL[6U], 1U),
    kI2C1_Clk_From_Frg0Clk = CLK_MUX_DEFINE(FCLKSEL[6U], 2U),
    kI2C1_Clk_From_Frg1Clk = CLK_MUX_DEFINE(FCLKSEL[6U], 3U),
    kI2C1_Clk_From_Fro_Div = CLK_MUX_DEFINE(FCLKSEL[6U], 4U),

    kI2C2_Clk_From_Fro     = CLK_MUX_DEFINE(FCLKSEL[7U], 0U),
    kI2C2_Clk_From_MainClk = CLK_MUX_DEFINE(FCLKSEL[7U], 1U),
    kI2C2_Clk_From_Frg0Clk = CLK_MUX_DEFINE(FCLKSEL[7U], 2U),
    kI2C2_Clk_From_Frg1Clk = CLK_MUX_DEFINE(FCLKSEL[7U], 3U),
    kI2C2_Clk_From_Fro_Div = CLK_MUX_DEFINE(FCLKSEL[7U], 4U),

    kI2C3_Clk_From_Fro     = CLK_MUX_DEFINE(FCLKSEL[8U], 0U),
    kI2C3_Clk_From_MainClk = CLK_MUX_DEFINE(FCLKSEL[8U], 1U),
    kI2C3_Clk_From_Frg0Clk = CLK_MUX_DEFINE(FCLKSEL[8U], 2U),
    kI2C3_Clk_From_Frg1Clk = CLK_MUX_DEFINE(FCLKSEL[8U], 3U),
    kI2C3_Clk_From_Fro_Div = CLK_MUX_DEFINE(FCLKSEL[8U], 4U),

    kSPI0_Clk_From_Fro     = CLK_MUX_DEFINE(FCLKSEL[9U], 0U),
    kSPI0_Clk_From_MainClk = CLK_MUX_DEFINE(FCLKSEL[9U], 1U),
    kSPI0_Clk_From_Frg0Clk = CLK_MUX_DEFINE(FCLKSEL[9U], 2U),
    kSPI0_Clk_From_Frg1Clk = CLK_MUX_DEFINE(FCLKSEL[9U], 3U),
    kSPI0_Clk_From_Fro_Div = CLK_MUX_DEFINE(FCLKSEL[9U], 4U),

    kSPI1_Clk_From_Fro     = CLK_MUX_DEFINE(FCLKSEL[10U], 0U),
    kSPI1_Clk_From_MainClk = CLK_MUX_DEFINE(FCLKSEL[10U], 1U),
    kSPI1_Clk_From_Frg0Clk = CLK_MUX_DEFINE(FCLKSEL[10U], 2U),
    kSPI1_Clk_From_Frg1Clk = CLK_MUX_DEFINE(FCLKSEL[10U], 3U),
    kSPI1_Clk_From_Fro_Div = CLK_MUX_DEFINE(FCLKSEL[10U], 4U),

    kFRG0_Clk_From_Fro     = CLK_MUX_DEFINE(FRG[0U].FRGCLKSEL, 0U),
    kFRG0_Clk_From_MainClk = CLK_MUX_DEFINE(FRG[0U].FRGCLKSEL, 1U),
    kFRG0_Clk_From_SysPll  = CLK_MUX_DEFINE(FRG[0U].FRGCLKSEL, 2U),

    kFRG1_Clk_From_Fro     = CLK_MUX_DEFINE(FRG[1U].FRGCLKSEL, 0U),
    kFRG1_Clk_From_MainClk = CLK_MUX_DEFINE(FRG[1U].FRGCLKSEL, 1U),
    kFRG1_Clk_From_SysPll  = CLK_MUX_DEFINE(FRG[1U].FRGCLKSEL, 2U),

    kCLKOUT_From_Fro     = CLK_MUX_DEFINE(CLKOUTSEL, 0U),
    kCLKOUT_From_MainClk = CLK_MUX_DEFINE(CLKOUTSEL, 1U),
    kCLKOUT_From_SysPll  = CLK_MUX_DEFINE(CLKOUTSEL, 2U),
    kCLKOUT_From_ExtClk  = CLK_MUX_DEFINE(CLKOUTSEL, 3U),
    kCLKOUT_From_WdtOsc  = CLK_MUX_DEFINE(CLKOUTSEL, 4U),
} clock_select_t;


typedef struct _i2c_master_config
{
    bool enableMaster;     /*!< Whether to enable master mode. */
    uint32_t baudRate_Bps; /*!< Desired baud rate in bits per second. */
    bool enableTimeout;    /*!< Enable internal timeout function. */
} i2c_master_config_t;


/*!
 * @brief Enables or disables the I2C module as master.
 *
 * @param base The I2C peripheral base address.
 * @param enable Pass true to enable or false to disable the specified I2C as master.
 */
static inline void I2C_MasterEnable(I2C_Type *base, bool enable)
{
    if (enable)
    {
        base->CFG = (base->CFG & I2C_CFG_MASK) | (uint32_t)I2C_CFG_MSTEN_MASK;
    }
    else
    {
        base->CFG = (base->CFG & I2C_CFG_MASK) & (~(uint32_t)I2C_CFG_MSTEN_MASK);
    }
}

/*
 *! @brief enable ip clock.
 *
 * @param clk clock ip definition.
 */
static inline void CLOCK_EnableClock(clock_ip_name_t clk)
{
    *(volatile uint32_t *)(((uint32_t)(&SYSCON->SYSAHBCLKCTRL0)) + CLK_GATE_GET_REG(clk)) |=
        1UL << CLK_GATE_GET_BITS_SHIFT(clk);
}

/*
 *! @brief	Configure the clock selection muxes.
 * @param	mux	: Clock to be configured.
 * @return	Nothing
 */
static inline void CLOCK_Select(clock_select_t sel)
{
    *(CLK_MUX_GET_REG(sel)) = CLK_MUX_GET_MUX(sel);
}


/*
 *!@brief disable ip clock.
 *
 * @param clk clock ip definition.
 */
static inline void CLOCK_DisableClock(clock_ip_name_t clk)
{
    *(volatile uint32_t *)(((uint32_t)(&SYSCON->SYSAHBCLKCTRL0)) + CLK_GATE_GET_REG(clk)) &=
        ~(1UL << CLK_GATE_GET_BITS_SHIFT(clk));
}

static inline void I2C_MasterClearStatusFlags(I2C_Type *base, uint32_t statusMask)
{
    /* Allow clearing just master status flags */
    base->STAT = statusMask & (I2C_STAT_MSTARBLOSS_MASK | I2C_STAT_MSTSTSTPERR_MASK);
}

static inline uint32_t I2C_GetStatusFlags(I2C_Type *base)
{
    return base->STAT;
}

uint32_t I2C_GetInstance(I2C_Type *base);
void I2C_MasterGetDefaultConfig(i2c_master_config_t *masterConfig);
void I2C_MasterInit(I2C_Type *base, const i2c_master_config_t *masterConfig, uint32_t srcClock_Hz);
void I2C_MasterSetBaudRate(I2C_Type *base, uint32_t baudRate_Bps, uint32_t srcClock_Hz);
uint32_t I2C_PendingStatusWait(I2C_Type *base);
status_t I2C_MasterStart(I2C_Type *base, uint8_t address, i2c_direction_t direction);
status_t I2C_MasterStop(I2C_Type *base);
status_t I2C_MasterWriteBlocking(I2C_Type *base, const void *txBuff, size_t txSize, uint32_t flags);
status_t I2C_MasterReadBlocking(I2C_Type *base, void *rxBuff, size_t rxSize, uint32_t flags);
status_t I2C_MasterTransferBlocking(I2C_Type *base, i2c_master_transfer_t *xfer);

/*!
 * @brief Sends a REPEATED START on the I2C bus.
 *
 * @param base I2C peripheral base pointer
 * @param address 7-bit slave device address.
 * @param direction Master transfer directions(transmit/receive).
 * @retval kStatus_Success Successfully send the start signal.
 * @retval kStatus_I2C_Busy Current bus is busy but not occupied by current I2C master.
 */
static inline status_t I2C_MasterRepeatedStart(I2C_Type *base, uint8_t address, i2c_direction_t direction)
{
    // Para esta aplicación no usamos repeated start real,
    // así que simplemente hacemos un START normal.
    I2C_MasterStart(base, address, direction);
    return kStatus_Success;
}

//Funciones adaptadas desde AVR

void i2c_start(uint8_t addr, i2c_direction_t dir);
void i2c_stop(void);
void i2c_byte(uint8_t );
void i2c_init(void);
uint8_t i2c_read_byte( void );
uint8_t GetDemora_IIC( void );
void SetDemora_IIC( uint8_t value );

//Otras necesarias
void RESET_SetPeripheralReset(reset_ip_name_t peripheral);
void RESET_ClearPeripheralReset(reset_ip_name_t peripheral);
inline void CLOCK_EnableClock(clock_ip_name_t clk);
void SWM_SetMovablePinSelect(SWM_Type *base, swm_select_movable_t func, swm_port_pin_type_t swm_port_pin);



#ifdef __cplusplus
}
#endif

#endif /* I2C_H_ */
