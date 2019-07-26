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
    nRF24_handle.setCE = nRF24_setCE;
    nRF24_handle.setCS = nRF24_setCS;
    nRF24_handle.read  = nRF24_read;
    nRF24_handle.send  = nRF24_send;

    // Initialize RF
    nRF24_configRF.addressWidth = PE_nRF24_ADDR_WIDTH_5BIT;
    nRF24_configRF.dataRate     = PE_nRF24_DATA_RATE_1000KBPS;
    nRF24_configRF.crcScheme    = PE_nRF24_CRC_SCHEME_2BYTE;
    nRF24_configRF.rfChannel    = 100;

    // Configure RF
    if (PE_nRF24_configureRF(&nRF24_handle, &nRF24_configRF) != PE_nRF24_RESULT_OK) {
        Error_Handler(__FILE__, __LINE__);
    }

    // Initialize RX
    nRF24_configRX.address     = (uint8_t *) PE_nRF24_TEST_ADDRESS;
    nRF24_configRX.autoACK     = PE_nRF24_AUTO_ACK_OFF;
    nRF24_configRX.payloadSize = 32;

    if (PE_nRF24_configureRX(&nRF24_handle, &nRF24_configRX, PE_nRF24_PIPE_RX0) != PE_nRF24_RESULT_OK) {
        Error_Handler(__FILE__, __LINE__);
    }

    uint8_t data[32];

    // Main loop
    while (1) {
        // Try read packet
        if (PE_nRF24_readPacket(&nRF24_handle, data, 32, 10) == PE_nRF24_RESULT_OK) {
            // Do something with data
        }

        // Wait next iteration
        PE_nRF24_delay(500);
    }
}

void PE_nRF24_delay(uint16_t ms)
{
    (void) ms;
}

void Error_Handler(const char *file, int line)
{
    (void) file;
    (void) line;

    while (1);
}