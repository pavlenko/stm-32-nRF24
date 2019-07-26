/* Includes ------------------------------------------------------------------*/

#include "demo_common.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void nRF24_setCE(PE_nRF24_BIT_t state)
{
    (void) state;
}

void nRF24_setCS(PE_nRF24_BIT_t state)
{
    (void) state;
}

PE_nRF24_RESULT_t nRF24_read(uint8_t addr, uint8_t *data, uint8_t size)
{
    (void) addr;
    (void) data;
    (void) size;
    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t nRF24_send(uint8_t addr, uint8_t *data, uint8_t size)
{
    (void) addr;
    (void) data;
    (void) size;
    return PE_nRF24_RESULT_OK;
}