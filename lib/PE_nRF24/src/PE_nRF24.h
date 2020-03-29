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
PE_nRF24_RESULT_t PE_nRF24_readPacket(PE_nRF24_handle_t *handle, uint8_t *data, uint8_t size, uint16_t timeout);
PE_nRF24_RESULT_t PE_nRF24_sendPacket(PE_nRF24_handle_t *handle, uint8_t *addr, uint8_t *data, uint8_t size, uint16_t timeout);
PE_nRF24_RESULT_t PE_nRF24_getLostCount(PE_nRF24_handle_t *handle, PE_nRF24_RETRY_COUNT_t *value);
PE_nRF24_RESULT_t PE_nRF24_getRetryCount(PE_nRF24_handle_t *handle, PE_nRF24_RETRY_COUNT_t *value);
PE_nRF24_RESULT_t PE_nRF24_getCarrierDetect(PE_nRF24_handle_t *handle, PE_nRF24_BIT_t *value);
PE_nRF24_RESULT_t PE_nRF24_handleIRQ(PE_nRF24_handle_t *handle);

void PE_nRF24_TXComplete(PE_nRF24_handle_t *handle);
void PE_nRF24_RXComplete(PE_nRF24_handle_t *handle);
uint32_t PE_nRF24_clock(void);

/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif //PE_NRF24_H
