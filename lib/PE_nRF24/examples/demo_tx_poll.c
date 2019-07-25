/* Includes ------------------------------------------------------------------*/

#include <PE_nRF24.h>
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

PE_nRF24_t nRF24_handle;
PE_nRF24_configRF_t nRF24_configRF;
PE_nRF24_configTX_t nRF24_configTX;

/* Private function prototypes -----------------------------------------------*/

static void nRF24_setCE(PE_nRF24_BIT_t state);
static void nRF24_setCS(PE_nRF24_BIT_t state);
PE_nRF24_RESULT_t nRF24_read(uint8_t addr, uint8_t *data, uint8_t size);
PE_nRF24_RESULT_t nRF24_send(uint8_t addr, uint8_t *data, uint8_t size);

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
        return 1;// Error
    }

    // Initialize TX
    nRF24_configTX.address = (uint8_t *) PE_nRF24_TEST_ADDRESS;
    nRF24_configTX.txPower = PE_nRF24_TX_POWER__0dBm;

    // Configure RF
    if (PE_nRF24_configureTX(&nRF24_handle, &nRF24_configTX) != PE_nRF24_RESULT_OK) {
        return 1;// Error
    }

    const char data[] = "Hello";

    // Main loop
    while (1) {
        // Send demo packet in blocking mode
        if (PE_nRF24_sendPacket(&nRF24_handle, (uint8_t *) data, strlen(data), 10) != PE_nRF24_RESULT_OK) {
            return 1;// Error
        }

        // Wait next iteration
        PE_nRF24_delay(5000);
    }
}

void PE_nRF24_delay(uint16_t ms)
{
    (void) ms;
}

static void nRF24_setCE(PE_nRF24_BIT_t state)
{
    (void) state;
}

static void nRF24_setCS(PE_nRF24_BIT_t state)
{
    (void) state;
}