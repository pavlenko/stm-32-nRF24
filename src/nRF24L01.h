/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef NRF24L01_H
#define NRF24L01_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/* Exported types ------------------------------------------------------------*/

typedef enum {
    nRF24L01_PIN_L = 0,
    nRF24L01_PIN_H = 1,
} nRF24L01_pin_t;

typedef struct {
    void (*setCE) (nRF24L01_pin_t state);
    void (*setCS) (nRF24L01_pin_t state);
} nRF24L01_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported function prototypes --------------------------------------------- */
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

//TODO transmitter handle for allow use multiple devices

void nRF24L01_Init(void);

void nRF24L01_readByte(uint8_t reg, uint8_t byte);
void nRF24L01_readData(uint8_t reg, uint8_t *data, uint8_t size);

void nRF24L01_sendByte(uint8_t reg, uint8_t byte);
void nRF24L01_sendData(uint8_t reg, uint8_t *data, uint8_t size);

#ifdef __cplusplus
}
#endif

#endif //NRF24L01_H
