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
    //TODO nRF24_DisableAA(0xFF);

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

    uint32_t i = 0, j = 0;
    while (1) {
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

        // Deassert the CE pin (in case if it still high)
        nRF24_CE_L();

        // Transfer a data from the specified buffer to the TX FIFO
        nRF24_WritePayload(pBuf, length);

        // Start a transmission by asserting CE pin (must be held at least 10us)
        nRF24_CE_H();

        // Poll the transceiver status register until one of the following flags will be set:
        //   TX_DS  - means the packet has been transmitted
        //   MAX_RT - means the maximum number of TX retransmits happened
        // note: this solution is far from perfect, better to use IRQ instead of polling the status
        do {
            status = nRF24_GetStatus();
            if (status & (nRF24_FLAG_TX_DS | nRF24_FLAG_MAX_RT)) {
                break;
            }
        } while (wait--);

        // Deassert the CE pin (Standby-II --> Standby-I)
        nRF24_CE_L();

        if (!wait) {
            // Timeout
            return nRF24_TX_TIMEOUT;
        }

        // Check the flags in STATUS register
        UART_SendStr("[");
        UART_SendHex8(status);
        UART_SendStr("] ");

        // Clear pending IRQ flags
        nRF24_ClearIRQFlags();

        if (status & nRF24_FLAG_MAX_RT) {
            // Auto retransmit counter exceeds the programmed maximum limit (FIFO is not removed)
            return nRF24_TX_MAXRT;
        }

        if (status & nRF24_FLAG_TX_DS) {
            // Successful transmission
            return nRF24_TX_SUCCESS;
        }

        // Some banana happens, a payload remains in the TX FIFO, flush it
        nRF24_FlushTX();

        return nRF24_TX_ERROR;

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