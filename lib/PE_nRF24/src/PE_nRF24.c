/* Includes ------------------------------------------------------------------*/

#include "PE_nRF24.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static void PE_nRF24_sendByte(PE_nRF24_t *handle, uint8_t addr, uint8_t byte)
{
    handle->setCS(PE_nRF24_PIN_L);

    //TODO

    handle->setCS(PE_nRF24_PIN_H);
}

void PE_nRF24_initialize(PE_nRF24_t *handle)
{
    PE_nRF24_sendByte(handle, /*nRF24_REG_CONFIG*/0x00, 0x08);
}