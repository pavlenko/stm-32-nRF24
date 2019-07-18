/* Includes ------------------------------------------------------------------*/

#include "PE_nRF24_2.h"

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

PE_nRF24_status_t PE_nRF24_initializeRX(PE_nRF24_handle_t *handle, PE_nRF24_configRX_t *config, PE_nRF24_pipe_t pipe)
{
    uint8_t reg;

    // Copy ptr to prevent change
    uint8_t *addressValue = config->address;
    uint8_t  addressWidth = 0;

    // Configure pipe address
    switch (pipe) {
        case PE_nRF24_PIPE_0:
        case PE_nRF24_PIPE_1:
            addressWidth = PE_nRF24_readByte(PE_nRF24_REG_SETUP_AW) + 1;
            addressValue += addressWidth;

            handle->instance->setCS(0);

            handle->instance->RW(PE_nRF24_CMD_W_REGISTER | PE_nRF24_REG_mX_ADDR_Pn[pipe]);

            // Write address in reverse order
            do {
                handle->instance->RW(*addressValue--);
            } while (addressWidth--);

            handle->instance->setCS(1);
            break;
        case PE_nRF24_PIPE_2:
        case PE_nRF24_PIPE_3:
        case PE_nRF24_PIPE_4:
        case PE_nRF24_PIPE_5:
            // Write address LSBbyte (only first byte from the addr buffer)
            PE_nRF24_sendByte(PE_nRF24_REG_mX_ADDR_Pn[pipe], *addressValue);
            break;
        default:
            // Incorrect pipe number -> do nothing
            break;
    }

    // Configure Auto Acknowledgement
    reg = PE_nRF24_readByte(PE_nRF24_REG_EN_AA);

    reg &= ~(1U << pipe);
    reg |= (uint8_t) (config->autoACK << pipe);

    PE_nRF24_sendByte(PE_nRF24_REG_EN_AA, reg);

    return PE_nRF24_STATUS_OK;
}
