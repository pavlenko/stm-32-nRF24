/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PE_NRF24_H
#define PE_NRF24_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

#include "PE_nRF24_def.h"

/* Exported function prototypes --------------------------------------------- */

PE_nRF24_RESULT_t PE_nRF24_configureRF(PE_nRF24_handle_t *handle);
PE_nRF24_RESULT_t PE_nRF24_configureRX(PE_nRF24_handle_t *handle, PE_nRF24_configRX_t *config, PE_nRF24_PIPE_t pipe);
PE_nRF24_RESULT_t PE_nRF24_attachRXPipe(PE_nRF24_handle_t *handle, PE_nRF24_PIPE_t pipe);
PE_nRF24_RESULT_t PE_nRF24_detachRXPipe(PE_nRF24_handle_t *handle, PE_nRF24_PIPE_t pipe);
PE_nRF24_RESULT_t PE_nRF24_clearIRQ(PE_nRF24_handle_t *handle, PE_nRF24_IRQ_t mask);
PE_nRF24_RESULT_t PE_nRF24_checkIRQ(PE_nRF24_handle_t *handle, PE_nRF24_IRQ_t mask);
PE_nRF24_RESULT_t PE_nRF24_readPacket(PE_nRF24_handle_t *handle, uint8_t *data, uint8_t size, uint16_t timeout);
PE_nRF24_RESULT_t PE_nRF24_sendPacket(PE_nRF24_handle_t *handle, uint8_t *addr, uint8_t *data, uint8_t size, uint16_t timeout);
PE_nRF24_RESULT_t PE_nRF24_getLostCount(PE_nRF24_handle_t *handle, PE_nRF24_RETRY_COUNT_t *value);
PE_nRF24_RESULT_t PE_nRF24_getRetryCount(PE_nRF24_handle_t *handle, PE_nRF24_RETRY_COUNT_t *value);
PE_nRF24_RESULT_t PE_nRF24_getCarrierDetect(PE_nRF24_handle_t *handle, PE_nRF24_BIT_t *value);
PE_nRF24_RESULT_t PE_nRF24_handleIRQ(PE_nRF24_handle_t *handle);

/**
 * @param handle
 * @param state
 */
void PE_nRF24_SPI_setCE(PE_nRF24_handle_t *handle, PE_nRF24_BIT_t state);

/**
 * @param handle
 * @param state
 */
void PE_nRF24_SPI_setCS(PE_nRF24_handle_t *handle, PE_nRF24_BIT_t state);

/**
 * @param handle
 * @param addr
 * @param data
 * @param size
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_SPI_read(PE_nRF24_handle_t *handle, uint8_t addr, uint8_t *data, uint8_t size);

/**
 * @param handle
 * @param addr
 * @param data
 * @param size
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_SPI_send(PE_nRF24_handle_t *handle, uint8_t addr, uint8_t *data, uint8_t size);

void PE_nRF24_TXComplete(PE_nRF24_handle_t *handle);
void PE_nRF24_RXComplete(PE_nRF24_handle_t *handle);
uint32_t PE_nRF24_clock(void);

/* Exported variables --------------------------------------------------------*/

static const uint8_t PE_nRF24_REG_RX_PW[6] = {
    PE_nRF24_REG_RX_PW_P0,
    PE_nRF24_REG_RX_PW_P1,
    PE_nRF24_REG_RX_PW_P2,
    PE_nRF24_REG_RX_PW_P3,
    PE_nRF24_REG_RX_PW_P4,
    PE_nRF24_REG_RX_PW_P5,
};

static const uint8_t PE_nRF24_REG_RX_ADDR[7] = {
    PE_nRF24_REG_RX_ADDR_P0,
    PE_nRF24_REG_RX_ADDR_P1,
    PE_nRF24_REG_RX_ADDR_P2,
    PE_nRF24_REG_RX_ADDR_P3,
    PE_nRF24_REG_RX_ADDR_P4,
    PE_nRF24_REG_RX_ADDR_P5,
};

/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif //PE_NRF24_H
