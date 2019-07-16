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

    //TODO optimize & use handle for read/write operations
    if (addr < nRF24_CMD_W_REGISTER) {
        // This is a register access
        nRF24_LL_RW(nRF24_CMD_W_REGISTER | (addr & nRF24_MASK_REG_MAP));
        nRF24_LL_RW(value);
    } else {
        // This is a single byte command or future command/register
        nRF24_LL_RW(reg);
        if ((addr != nRF24_CMD_FLUSH_TX) && (addr != nRF24_CMD_FLUSH_RX) && (addr != nRF24_CMD_REUSE_TX_PL) && (addr != nRF24_CMD_NOP)) {
            // Send register value
            nRF24_LL_RW(value);
        }
    }

    handle->setCS(PE_nRF24_PIN_H);
}

void PE_nRF24_initialize(PE_nRF24_t *handle)
{
    PE_nRF24_sendByte(handle, /*nRF24_REG_CONFIG*/0x00, 0x08);
}