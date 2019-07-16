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

    if (addr < PE_nRF24_CMD_W_REGISTER) {
        // This is a register access
        handle->RW(PE_nRF24_CMD_W_REGISTER | (addr & PE_nRF24_REG_MASK));
        handle->RW(byte);
    } else {
        // This is a single byte command or future command/register
        handle->RW(addr);

        if (
            (addr != PE_nRF24_CMD_FLUSH_TX) &&
            (addr != PE_nRF24_CMD_FLUSH_RX) &&
            (addr != PE_nRF24_CMD_REUSE_TX_PL) &&
            (addr != PE_nRF24_CMD_NOP)
        ) {
            // Send register value
            handle->RW(byte);
        }
    }

    handle->setCS(PE_nRF24_PIN_H);
}

void PE_nRF24_initialize(PE_nRF24_t *handle)
{
    PE_nRF24_sendByte(handle, /*nRF24_REG_CONFIG*/0x00, 0x08);
}