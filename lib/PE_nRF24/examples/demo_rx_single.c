/* Includes ------------------------------------------------------------------*/

#include <PE_nRF24.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

PE_nRF24_t PE_nRF24;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void nRF24_setCE(PE_nRF24_pinState_t state)
{
    (void) state;
}

void nRF24_setCS(PE_nRF24_pinState_t state)
{
    (void) state;
}

int main()
{
    // Configure adapters
    PE_nRF24.setCE = nRF24_setCE;
    PE_nRF24.setCS = nRF24_setCS;

    // Initialize transceiver defaults
    PE_nRF24_initialize(&PE_nRF24);

    while (1) {}
}