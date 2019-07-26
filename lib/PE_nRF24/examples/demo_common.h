#ifndef PE_NRF24_DEMO_COMMON_H
#define PE_NRF24_DEMO_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include <PE_nRF24.h>
#include <string.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported function prototypes --------------------------------------------- */

void nRF24_setCE(PE_nRF24_BIT_t state);
void nRF24_setCS(PE_nRF24_BIT_t state);
PE_nRF24_RESULT_t nRF24_read(uint8_t addr, uint8_t *data, uint8_t size);
PE_nRF24_RESULT_t nRF24_send(uint8_t addr, uint8_t *data, uint8_t size);

/* Exported variables --------------------------------------------------------*/

PE_nRF24_handle_t nRF24_handle;
PE_nRF24_configRX_t nRF24_configRX;

/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif //PE_NRF24_DEMO_COMMON_H
