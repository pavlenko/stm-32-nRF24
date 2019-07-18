/* Includes ------------------------------------------------------------------*/

#include <PE_nRF24.h>
#include <stdint.h>
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

PE_nRF24_t PE_nRF24;

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

    while (1) {
        puts(">:");
        //TODO print payload
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