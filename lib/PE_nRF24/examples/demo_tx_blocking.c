/* Includes ------------------------------------------------------------------*/

#include "demo_common.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

void Error_Handler(const char *file, int line);

/* Private functions ---------------------------------------------------------*/

int main(void)
{
    // Configure adapters
    //nRF24_handle.setCE = nRF24_setCE;
    //nRF24_handle.setCS = nRF24_setCS;
    //nRF24_handle.read  = nRF24_read;
    //nRF24_handle.send  = nRF24_send;

    // Initialize RF
    nRF24_handle.config.addressWidth = PE_nRF24_ADDR_WIDTH_5BIT;
    nRF24_handle.config.dataRate     = PE_nRF24_DATA_RATE_1000KBPS;
    nRF24_handle.config.crcScheme    = PE_nRF24_CRC_SCHEME_2BYTE;
    nRF24_handle.config.rfChannel    = 100;
    nRF24_handle.config.txPower      = PE_nRF24_TX_POWER__0dBm;

    // Configure RF
    if (PE_nRF24_configureRF(&nRF24_handle) != PE_nRF24_RESULT_OK) {
        Error_Handler(__FILE__, __LINE__);
    }

    const char addr[] = PE_nRF24_TEST_ADDRESS;
    const char data[] = "Hello";

    uint32_t start = 0;

    // Main loop
    while (1) {
        // Wait for timeout
        if (start > 0 && PE_nRF24_clock() - start < 500) {
            continue;
        }

        start = PE_nRF24_clock();

        // Send
        if (PE_nRF24_sendPacket(&nRF24_handle, (uint8_t *) addr, (uint8_t *) data, strlen(data), 10) != PE_nRF24_RESULT_OK) {
            Error_Handler(__FILE__, __LINE__);
        }
    }
}

void Error_Handler(const char *file, int line)
{
    (void) file;
    (void) line;

    while (1);
}