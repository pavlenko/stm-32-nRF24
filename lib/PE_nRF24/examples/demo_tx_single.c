/* Includes ------------------------------------------------------------------*/

#include <PE_nRF24.h>
#include <stdint.h>
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

PE_nRF24_t PE_nRF24;

uint8_t payloadData[32];
uint8_t payloadSize;

/* Private function prototypes -----------------------------------------------*/

void nRF24_delay_ms(uint32_t ms);
void nRF24_setCE(PE_nRF24_pinState_t state);
void nRF24_setCS(PE_nRF24_pinState_t state);
uint8_t nRF24_RW(uint8_t data);

/* Private functions ---------------------------------------------------------*/

int main()
{
    // Configure adapters
    PE_nRF24.setCE = nRF24_setCE;
    PE_nRF24.setCS = nRF24_setCS;
    PE_nRF24.RW    = nRF24_RW;

    // Initialize transceiver defaults
    PE_nRF24_initialize(&PE_nRF24);

    // Disable ShockBurst for all RX pipes
    PE_nRF24_setAutoAcknowledgment(&PE_nRF24, 0);

    // Set RF channel
    PE_nRF24_setRFChannel(&PE_nRF24, 115);

    // Set data rate
    PE_nRF24_setDataRate(&PE_nRF24, PE_nRF24_DR_1000kbps);

    // Set CRC scheme
    PE_nRF24_setCRCScheme(&PE_nRF24, PE_nRF24_CRC_2byte);

    // Set address width, its common for all pipes (RX and TX)
    PE_nRF24_setAddressWidth(&PE_nRF24, PE_nRF24_ADDRESS_3BIT);

    // Configure TX PIPE
    static const uint8_t nRF24_ADDR[] = { 0xE7, 0x1C, 0xE3 };
    PE_nRF24_setAddressValue(&PE_nRF24, PE_nRF24_PIPE_TX, nRF24_ADDR);

    // Set TX power (maximum)
    PE_nRF24_setTXPower(&PE_nRF24, PE_nRF24_TX_POWER_0dBm);

    // Set operational mode (PTX == transmitter)
    PE_nRF24_setDirection(&PE_nRF24, PE_nRF24_DIRECTION_TX);

    // Clear any pending IRQ flags
    PE_nRF24_clearIRQFlags(&PE_nRF24);

    // Wake the transceiver
    PE_nRF24_setPowerMode(&PE_nRF24, PE_nRF24_POWER_UP);

    uint32_t i = 0, j = 0, wait;
    uint8_t status;
    while (1) {
        wait = 0x000FFFFFU;

        // Prepare data packet
        for (i = 0; i < payloadSize; i++) {
            payloadData[i] = j++;
            if (j > 0x000000FF) j = 0;
        }

        // Print payload
        puts(">:");
        puts((char *) payloadData);

        // Print result
        puts("<:");

        //TODO maybe wrap logic to some function & put into lib
        //TODO like HAL_SPI_Transmit() & HAL_SPI_Transmit_IT()

        // De-assert the CE pin (in case if it still high)
        PE_nRF24.setCE(PE_nRF24_PIN_L);

        // Transfer a data from the specified buffer to the TX FIFO
        PE_nRF24_sendPayload(&PE_nRF24, payloadData, payloadSize);

        // Start a transmission by asserting CE pin (must be held at least 10us)
        PE_nRF24.setCE(PE_nRF24_PIN_H);

        // Poll the transceiver status register until one of the following flags will be set:
        //   TX_DS  - means the packet has been transmitted
        //   MAX_RT - means the maximum number of TX retransmits happened
        // note: this solution is far from perfect, better to use IRQ instead of polling the status
        do {
            status = PE_nRF24_getStatus(&PE_nRF24);
            if (status & (PE_nRF24_STATUS_TX_DS | PE_nRF24_STATUS_MAX_RT)) {
                break;
            }
        } while (wait--);

        // De-assert the CE pin (Standby-II --> Standby-I)
        PE_nRF24.setCE(PE_nRF24_PIN_L);

        if (!wait) {
            // Timeout
            //return nRF24_TX_TIMEOUT;
            continue;
        }

        // Check the flags in STATUS register
        //UART_SendStr("[");
        //UART_SendHex8(status);
        //UART_SendStr("] ");

        // Clear pending IRQ flags
        PE_nRF24_clearIRQFlags(&PE_nRF24);

        if (status & PE_nRF24_STATUS_MAX_RT) {
            // Auto retransmit counter exceeds the programmed maximum limit (FIFO is not removed)
            //return nRF24_TX_MAXRT;
            continue;
        }

        if (status & PE_nRF24_STATUS_TX_DS) {
            // Successful transmission
            //return nRF24_TX_SUCCESS;
            continue;
        }

        // Some banana happens, a payload remains in the TX FIFO, flush it
        PE_nRF24_flushTX(&PE_nRF24);

        //return nRF24_TX_ERROR;

        nRF24_delay_ms(500);
    }
}

void nRF24_delay_ms(uint32_t ms)
{
    //TODO implement function logic
    (void) ms;
}

void nRF24_setCE(PE_nRF24_pinState_t state)
{
    //TODO implement function logic
    (void) state;
}

void nRF24_setCS(PE_nRF24_pinState_t state)
{
    //TODO implement function logic
    (void) state;
}

uint8_t nRF24_RW(uint8_t data)
{
    //TODO implement function logic
    (void) data;
    return 0;
}