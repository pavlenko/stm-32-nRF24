/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PE_NRF24_H
#define PE_NRF24_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/* Exported types ------------------------------------------------------------*/

typedef enum {
    PE_nRF24_PIN_L = 0,
    PE_nRF24_PIN_H = 1,
} PE_nRF24_pinState_t;

typedef struct {
    void (*setCE) (PE_nRF24_pinState_t state);
    void (*setCS) (PE_nRF24_pinState_t state);
} PE_nRF24_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported function prototypes --------------------------------------------- */
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void PE_nRF24_initialize(PE_nRF24_t *handle);

#ifdef __cplusplus
}
#endif

#endif //PE_NRF24_H
