/* Includes ------------------------------------------------------------------*/

#include "PE_nRF24_2.h"

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static PE_nRF24_status_t PE_nRF24_readByte(PE_nRF24_handle_t *handle, uint8_t addr, uint8_t *byte)
{
    //TODO
    (void) handle;
    (void) addr;
    (void) byte;
    return PE_nRF24_STATUS_OK;
}

static PE_nRF24_status_t PE_nRF24_sendByte(PE_nRF24_handle_t *handle, uint8_t addr, uint8_t byte)
{
    //TODO
    (void) handle;
    (void) addr;
    (void) byte;
    return PE_nRF24_STATUS_OK;
}

PE_nRF24_status_t PE_nRF24_setDirection2(PE_nRF24_handle_t *handle, PE_nRF24_Direction_t direction)
{
    uint8_t reg;

    PE_nRF24_readByte(handle, PE_nRF24_REG_CONFIG, &reg);

    reg &= ~PE_nRF24_CONFIG_PRIM_RX;
    reg |= (direction << PE_nRF24_CONFIG_PRIM_RX_Pos);

    PE_nRF24_sendByte(handle, PE_nRF24_REG_CONFIG, reg);

    return PE_nRF24_STATUS_OK;
}

PE_nRF24_status_t PE_nRF24_setPowerMode2(PE_nRF24_handle_t *handle, PE_nRF24_POWER_t mode)
{
    uint8_t reg;

    PE_nRF24_readByte(handle, PE_nRF24_REG_CONFIG, &reg);

    reg &= ~PE_nRF24_CONFIG_PWR_UP;
    reg |= (mode << PE_nRF24_CONFIG_PWR_UP_Pos);

    PE_nRF24_sendByte(handle, PE_nRF24_REG_CONFIG, reg);

    return PE_nRF24_STATUS_OK;
}

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
            PE_nRF24_readByte(handle, PE_nRF24_REG_SETUP_AW, &addressWidth);

            addressValue += (addressWidth + 1);

            handle->instance->setCS(0);

            handle->instance->RW(PE_nRF24_CMD_W_REGISTER | PE_nRF24_REG_mX_ADDR_Pn[pipe]);

            // Write address in reverse order
            do {
                handle->instance->RW(*addressValue--);
            } while (addressWidth-- >= 0);

            handle->instance->setCS(1);
            break;
        case PE_nRF24_PIPE_2:
        case PE_nRF24_PIPE_3:
        case PE_nRF24_PIPE_4:
        case PE_nRF24_PIPE_5:
            // Write address LSB byte (only first byte from the addr buffer)
            PE_nRF24_sendByte(handle, PE_nRF24_REG_mX_ADDR_Pn[pipe], *addressValue);
            break;
        default:
            // Incorrect pipe number -> do nothing
            break;
    }

    // Set RX payload length (RX_PW_Px register)
    PE_nRF24_sendByte(handle, PE_nRF24_REG_RX_PW_Pn[pipe], config->payloadSize & 0x3FU);

    // Configure Auto Acknowledgement
    PE_nRF24_readByte(handle, PE_nRF24_REG_EN_AA, &reg);

    reg &= ~(1U << pipe);
    reg |= (uint8_t) (config->autoACK << pipe);

    PE_nRF24_sendByte(handle, PE_nRF24_REG_EN_AA, reg);

    return PE_nRF24_STATUS_OK;
}

PE_nRF24_status_t PE_nRF24_attachRX(PE_nRF24_handle_t *handle, PE_nRF24_pipe_t pipe)
{
    uint8_t reg;

    // Enable specific pipe
    PE_nRF24_readByte(handle, PE_nRF24_REG_EN_RXADDR, &reg);

    reg |= (1U << pipe);

    PE_nRF24_sendByte(handle, PE_nRF24_REG_EN_RXADDR, reg);

    return PE_nRF24_STATUS_OK;
}

PE_nRF24_status_t PE_nRF24_detachRX(PE_nRF24_handle_t *handle, PE_nRF24_pipe_t pipe)
{
    uint8_t reg;

    // disable specific pipe
    PE_nRF24_readByte(handle, PE_nRF24_REG_EN_RXADDR, &reg);

    reg &= ~(1U << pipe);

    PE_nRF24_sendByte(handle, PE_nRF24_REG_EN_RXADDR, reg);

    return PE_nRF24_STATUS_OK;
}

PE_nRF24_status_t PE_nRF24_handleIRQ(PE_nRF24_handle_t *handle)
{
    uint8_t status;

    if (PE_nRF24_readByte(handle, PE_nRF24_REG_STATUS, &status) != PE_nRF24_STATUS_OK) {
        return PE_nRF24_STATUS_ERROR;
    }

    // Process RX data ready (RX_DR bit)
    if ((status & PE_nRF24_STATUS_RX_DR) != 0U) {
        uint8_t statusFIFO;

        handle->instance->setCE(0);

        do {
            //TODO read bytes to specific pipe
            HAL_nRF24L01P_ReadRXPayload(nRF, nRF->RX_Buffer);

            status |= PE_nRF24_STATUS_RX_DR;

            PE_nRF24_sendByte(handle, PE_nRF24_REG_STATUS, status);
            PE_nRF24_readByte(handle, PE_nRF24_REG_FIFO_STATUS, &statusFIFO);
        } while ((statusFIFO & PE_nRF24_FIFO_STATUS_RX_EMPTY) == 0x00);

        handle->instance->setCE(1);
    }

    // Process TX sent (TX_DS bit)
    if ((status & PE_nRF24_STATUS_TX_DS) != 0) {
        handle->instance->setCE(0);

        status |= PE_nRF24_STATUS_TX_DS;

        PE_nRF24_setDirection2(handle, PE_nRF24_DIRECTION_RX);
        PE_nRF24_sendByte(handle, PE_nRF24_REG_STATUS, status);

        handle->instance->setCE(1);

        //TODO clear busy flag
        //nRF->Busy = 0;
    }

    if ((status & PE_nRF24_STATUS_MAX_RT) != 0) {
        status |= PE_nRF24_STATUS_MAX_RT;

        PE_nRF24_flushTX(handle);

        // Toggle RF power up bit
        PE_nRF24_setPowerMode2(handle, PE_nRF24_POWER_OFF);
        PE_nRF24_setPowerMode2(handle, PE_nRF24_POWER_ON);

        handle->instance->setCE(0);

        PE_nRF24_setDirection2(handle, PE_nRF24_DIRECTION_RX);
        PE_nRF24_sendByte(handle, PE_nRF24_REG_STATUS, status);

        handle->instance->setCE(1);

        //TODO clear busy flag
        //nRF->Busy = 0;
    }

    return PE_nRF24_STATUS_OK;
}
