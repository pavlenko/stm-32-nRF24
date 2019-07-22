/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PE_NRF24_H
#define PE_NRF24_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include "PE_nRF24_def.h"

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

typedef enum {
    PE_nRF24_PIN_L = 0,
    PE_nRF24_PIN_H = 1,
} PE_nRF24_pinState_t;

typedef enum {
    PE_nRF24_POWER_STANDBY = 0U,
    PE_nRF24_POWER_UP      = 1U,
} PE_nRF24_PowerMode_t;

typedef enum {
    PE_nRF24_DIRECTION_TX = 0U,
    PE_nRF24_DIRECTION_RX = 1U,
} PE_nRF24_Direction_t;

typedef enum {
    PE_nRF24_CRC_OFF   = 0x00U,
    PE_nRF24_CRC_1byte = PE_nRF24_CONFIG_EN_CRC,
    PE_nRF24_CRC_2byte = PE_nRF24_CONFIG_EN_CRC|PE_nRF24_CONFIG_CRCO,
} PE_nRF24_CRCScheme_t;

typedef enum {
    PE_nRF24_ARD_250us  = 0x0U,
    PE_nRF24_ARD_500us  = 0x1U,
    PE_nRF24_ARD_750us  = 0x2U,
    PE_nRF24_ARD_1000us = 0x3U,
    PE_nRF24_ARD_1250us = 0x4U,
    PE_nRF24_ARD_1500us = 0x5U,
    PE_nRF24_ARD_1750us = 0x6U,
    PE_nRF24_ARD_2000us = 0x7U,
    PE_nRF24_ARD_2250us = 0x8U,
    PE_nRF24_ARD_2500us = 0x9U,
    PE_nRF24_ARD_2750us = 0xAU,
    PE_nRF24_ARD_3000us = 0xBU,
    PE_nRF24_ARD_3250us = 0xCU,
    PE_nRF24_ARD_3500us = 0xDU,
    PE_nRF24_ARD_3750us = 0xEU,
    PE_nRF24_ARD_4000us = 0xFU,
} PE_nRF24_AutoRetryDelay_t;

typedef enum {
    PE_nRF24_ADDRESS_3BIT = PE_nRF24_SETUP_AW_0,
    PE_nRF24_ADDRESS_4BIT = PE_nRF24_SETUP_AW_1,
    PE_nRF24_ADDRESS_5BIT = PE_nRF24_SETUP_AW_1|PE_nRF24_SETUP_AW_0,
} PE_nRF24_AddressWidth_t;

typedef enum {
    PE_nRF24_PIPE_RX0,
    PE_nRF24_PIPE_RX1,
    PE_nRF24_PIPE_RX2,
    PE_nRF24_PIPE_RX3,
    PE_nRF24_PIPE_RX4,
    PE_nRF24_PIPE_RX5,
    PE_nRF24_PIPE_TX,
} PE_nRF24_Pipe_t;

static const uint8_t PE_nRF24_REG_RX_PW_Pn[6] = {
    PE_nRF24_REG_RX_PW_P0,
    PE_nRF24_REG_RX_PW_P1,
    PE_nRF24_REG_RX_PW_P2,
    PE_nRF24_REG_RX_PW_P3,
    PE_nRF24_REG_RX_PW_P4,
    PE_nRF24_REG_RX_PW_P5,
};

static const uint8_t PE_nRF24_REG_mX_ADDR_Pn[7] = {
    PE_nRF24_REG_RX_ADDR_P0,
    PE_nRF24_REG_RX_ADDR_P1,
    PE_nRF24_REG_RX_ADDR_P2,
    PE_nRF24_REG_RX_ADDR_P3,
    PE_nRF24_REG_RX_ADDR_P4,
    PE_nRF24_REG_RX_ADDR_P5,
    PE_nRF24_REG_TX_ADDR,
};

typedef enum {
    PE_nRF24_TX_POWER_18dBm,
    PE_nRF24_TX_POWER_12dBm,
    PE_nRF24_TX_POWER_6dBm,
    PE_nRF24_TX_POWER_0dBm,
} PE_nRF24_TXPower_t;

typedef enum {
    PE_nRF24_DR__250kbps = PE_nRF24_RF_SETUP_RF_DR_LOW,
    PE_nRF24_DR_1000kbps = 0x00U,
    PE_nRF24_DR_2000kbps = PE_nRF24_RF_SETUP_RF_DR_HIGH,
} PE_nRF24_DataRate_t;

typedef struct {
    /**
     * Chip enable, activates RX or TX mode
     *
     * @param state
     */
    void (*setCE) (PE_nRF24_pinState_t state);

    /**
     * SPI Chip select
     *
     * @param state
     */
    void (*setCS) (PE_nRF24_pinState_t state);

    /**
     * SPI transceive data
     *
     * @param data
     */
    uint8_t (*RW) (uint8_t data);
} PE_nRF24_t;

/* Exported function prototypes --------------------------------------------- */
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

uint8_t PE_nRF24_check(PE_nRF24_t *handle);
void PE_nRF24_initialize(PE_nRF24_t *handle);
void PE_nRF24_flushRX(PE_nRF24_t *handle);
void PE_nRF24_flushTX(PE_nRF24_t *handle);
void PE_nRF24_clearIRQFlags(PE_nRF24_t *handle);

void PE_nRF24_setPowerMode(PE_nRF24_t *handle, PE_nRF24_PowerMode_t mode);

void PE_nRF24_setDirection(PE_nRF24_t *handle, PE_nRF24_Direction_t dir);

void PE_nRF24_setCRCScheme(PE_nRF24_t *handle, PE_nRF24_CRCScheme_t scheme);

void PE_nRF24_setAutoAcknowledgment(PE_nRF24_t *handle, uint8_t pipes);

/**
 * Set frequency channel, frequency will be (2400 + channel) MHz
 *
 * @param handle  connection
 * @param channel value in range 0...127
 */
void PE_nRF24_setRFChannel(PE_nRF24_t *handle, uint8_t channel);

/**
 * Set automatic retransmission parameters
 *
 * @param handle connection
 * @param delay  one of PE_nRF24_AutoRetryDelay_t
 * @param tries  in range 1...15 or 0 for disable
 */
void PE_nRF24_setAutoRetry(PE_nRF24_t *handle, PE_nRF24_AutoRetryDelay_t delay, uint8_t tries);

/**
 * Set address width
 *
 * @param handle connection
 * @param width  one of PE_nRF24_AddressWidth_t
 */
void PE_nRF24_setAddressWidth(PE_nRF24_t *handle, PE_nRF24_AddressWidth_t width);

/**
 * Get address width
 *
 * @param handle connection
 *
 * @return Address width from device
 */
uint8_t PE_nRF24_getAddressWidth(PE_nRF24_t *handle);

/**
 * Set static RX address for a specified pipe
 *
 * @param handle  connection
 * @param pipe    one of PE_nRF24_PipeN_t, each item is register address internally
 * @param address pointer to address array
 */
void PE_nRF24_setAddressValue(PE_nRF24_t *handle, PE_nRF24_Pipe_t pipe, const uint8_t *address);

/**
 * Set transmitter output RF power level
 *
 * @param handle connection
 * @param level  one of PE_nRF24_TXPower_t
 */
void PE_nRF24_setTXPower(PE_nRF24_t *handle, PE_nRF24_TXPower_t level);

/**
 * Set transmitter data rate speed
 *
 * Note: PE_nRF24_DR__250kbps can be used only for plus version
 *
 * @param handle connection
 * @param rate   one of PE_nRF24_DataRate_t
 */
void PE_nRF24_setDataRate(PE_nRF24_t *handle, PE_nRF24_DataRate_t rate);

uint8_t PE_nRF24_getStatus(PE_nRF24_t *handle);

void PE_nRF24_attachRXPipe(PE_nRF24_t *handle, PE_nRF24_Pipe_t pipe);

void PE_nRF24_detachRXPipe(PE_nRF24_t *handle, PE_nRF24_Pipe_t pipe);

void PE_nRF24_sendPayload(PE_nRF24_t *handle, uint8_t *data, uint8_t size);

/**
 * IRQ handler, must be called from interrupt if used
 *
 * @param handle
 */
void PE_nRF24_IRQHandler(PE_nRF24_t *handle);

void PE_nRF24_RXDataReadyHandler(PE_nRF24_t *handle, uint8_t pipe);

void PE_nRF24_TXDataSentHandler(PE_nRF24_t *handle);

void PE_nRF24_MaxRetriesHandler(PE_nRF24_t *handle);

#ifdef __cplusplus
}
#endif

#endif //PE_NRF24_H
