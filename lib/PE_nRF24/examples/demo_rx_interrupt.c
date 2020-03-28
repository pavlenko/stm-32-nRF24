/* Includes ------------------------------------------------------------------*/

#include "demo_common.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint8_t received;

/* Private function prototypes -----------------------------------------------*/

void Error_Handler(const char *file, int line);
void EXTIRQ_Handler(void);

/* Private functions ---------------------------------------------------------*/

int main(void)
{
    // Configure adapters
    //nRF24_handle.setCE = nRF24_setCE;
    //nRF24_handle.setCS = nRF24_setCS;
    nRF24_handle.read  = nRF24_read;
    nRF24_handle.send  = nRF24_send;

    // Initialize RF
    nRF24_handle.config.addressWidth = PE_nRF24_ADDR_WIDTH_5BIT;
    nRF24_handle.config.dataRate     = PE_nRF24_DATA_RATE_1000KBPS;
    nRF24_handle.config.crcScheme    = PE_nRF24_CRC_SCHEME_2BYTE;
    nRF24_handle.config.rfChannel    = 100;

    // Configure RF
    if (PE_nRF24_configureRF(&nRF24_handle) != PE_nRF24_RESULT_OK) {
        Error_Handler(__FILE__, __LINE__);
    }

    // Initialize RX
    nRF24_configRX.address     = (uint8_t *) PE_nRF24_TEST_ADDRESS;
    nRF24_configRX.autoACK     = PE_nRF24_AUTO_ACK_OFF;
    nRF24_configRX.payloadSize = 32;

    if (PE_nRF24_configureRX(&nRF24_handle, &nRF24_configRX, PE_nRF24_PIPE_RX0) != PE_nRF24_RESULT_OK) {
        Error_Handler(__FILE__, __LINE__);
    }

    PE_nRF24_attachRXPipe(&nRF24_handle, PE_nRF24_PIPE_RX0);

    uint8_t data[5];
    uint8_t size = 5;

    uint32_t start = 0;

    // Main loop
    while (1) {
        // Wait for timeout
        if (start > 0 && PE_nRF24_clock() - start < 500) {
            continue;
        }

        start = PE_nRF24_clock();

        // Read
        if (PE_nRF24_readPacket(&nRF24_handle, (uint8_t *) data, size, 0) != PE_nRF24_RESULT_OK) {
            Error_Handler(__FILE__, __LINE__);
        }

        received = 0;

        // Wait for complete
        while (received == 0);

        // Do some work with data[5]
    }
}

void PE_nRF24_RXComplete(PE_nRF24_handle_t *handle)
{
    (void) handle;
    received = 1;
}

void Error_Handler(const char *file, int line)
{
    (void) file;
    (void) line;

    while (1);
}

void EXTIRQ_Handler(void)
{
    if (PE_nRF24_handleIRQ(&nRF24_handle) != PE_nRF24_RESULT_OK) {
        Error_Handler(__FILE__, __LINE__);
    }
}