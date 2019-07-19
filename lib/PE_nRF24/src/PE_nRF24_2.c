/* Includes ------------------------------------------------------------------*/

#include "PE_nRF24_2.h"

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//TODO refactor to return status & write data to ptr
static uint8_t PE_nRF24_readByte(PE_nRF24_handle_t *handle, uint8_t addr)
{
    //TODO
    (void) addr;
    return 0;
}

//TODO refactor to return status
static void PE_nRF24_sendByte(PE_nRF24_handle_t *handle, uint8_t addr, uint8_t byte)
{
    //TODO
    (void) addr;
    (void) byte;
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
            addressWidth = PE_nRF24_readByte(handle, PE_nRF24_REG_SETUP_AW) + 1;
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
    reg = PE_nRF24_readByte(handle, PE_nRF24_REG_EN_AA);

    reg &= ~(1U << pipe);
    reg |= (uint8_t) (config->autoACK << pipe);

    PE_nRF24_sendByte(handle, PE_nRF24_REG_EN_AA, reg);

    return PE_nRF24_STATUS_OK;
}

PE_nRF24_status_t PE_nRF24_attachRX(PE_nRF24_handle_t *handle, PE_nRF24_pipe_t pipe)
{
    uint8_t reg;

    // Enable specific pipe
    reg = PE_nRF24_readByte(handle, PE_nRF24_REG_EN_RXADDR);

    reg |= (1U << pipe);

    PE_nRF24_sendByte(handle, PE_nRF24_REG_EN_RXADDR, reg);

    return PE_nRF24_STATUS_OK;
}

PE_nRF24_status_t PE_nRF24_detachRX(PE_nRF24_handle_t *handle, PE_nRF24_pipe_t pipe)
{
    uint8_t reg;

    // disable specific pipe
    reg = PE_nRF24_readByte(handle, PE_nRF24_REG_EN_RXADDR);

    reg &= ~(1U << pipe);

    PE_nRF24_sendByte(handle, PE_nRF24_REG_EN_RXADDR, reg);

    return PE_nRF24_STATUS_OK;
}

void PE_nRF24_handleIRQ(PE_nRF24_handle_t *handle)
{
    uint8_t status;

    status = PE_nRF24_readByte(handle, PE_nRF24_REG_STATUS);

    if ((status & PE_nRF24_STATUS_RX_DR) != 0U) {
        uint8_t FIFOStatus;

        handle->instance->setCE(0);

        do {
            HAL_nRF24L01P_ReadRXPayload(nRF, nRF->RX_Buffer);

            status |= PE_nRF24_STATUS_RX_DR;

            PE_nRF24_sendByte(handle, PE_nRF24_REG_STATUS, status);

            FIFOStatus = PE_nRF24_readByte(handle, PE_nRF24_REG_FIFO_STATUS);
        } while ((FIFOStatus & PE_nRF24_FIFO_STATUS_RX_EMPTY) == 0x00);

        handle->instance->setCE(1);
    }
}

//TODO convert to own implementation, maybe with separate methods for separate isr
HAL_StatusTypeDef HAL_nRF24L01P_IRQ_Handler(nRF24L01P *nRF)
{
    /* ---- Local Vars. ---- */
    uint8_t regStatus;
    /* ---- Pre Process ---- */
    if(HAL_nRF24L01P_ReadRegister(nRF, nRF_STATUS, &regStatus) != HAL_OK)
    {
        return HAL_ERROR;
    }
    /* ---- RX FIFO Int.---- */
    if((regStatus & (1 << 6)) != 0)
    {
        uint8_t regFIFO_Status;
        HAL_nRF24L01P_CE_Low(nRF);
        do {
            HAL_nRF24L01P_ReadRXPayload(nRF, nRF->RX_Buffer);
            regStatus |= (1 << 6);
            HAL_nRF24L01P_WriteRegister(nRF, nRF_STATUS, &regStatus);
            HAL_nRF24L01P_ReadRegister(nRF, nRF_FIFO_STATUS, &regFIFO_Status);
        } while((regFIFO_Status & 0x01) == 0x00);
        HAL_nRF24L01P_CE_High(nRF);
    }
    /* ---- TX Sent Int.---- */
    if((regStatus & (1 << 5)) != 0)
    {
        HAL_nRF24L01P_CE_Low(nRF);
        regStatus |= (1 << 5);
        HAL_nRF24L01P_TXRX(nRF, nRF_STATE_RX);
        HAL_nRF24L01P_WriteRegister(nRF, nRF_STATUS, &regStatus);
        HAL_nRF24L01P_CE_High(nRF);
        nRF->Busy = 0;
    }
    /* ---- MAXReTX Int.---- */
    if((regStatus & (1 << 4)) != 0)
    {
        regStatus |= (1 << 4);

        HAL_nRF24L01P_FlushTX(nRF);
        HAL_nRF24L01P_PowerUP(nRF, nRF_DISABLE);	// bi kapatip açalim da düzelsin...
        HAL_nRF24L01P_PowerUP(nRF, nRF_ENABLE);

        HAL_nRF24L01P_CE_Low(nRF);
        HAL_nRF24L01P_TXRX(nRF, nRF_STATE_RX);
        HAL_nRF24L01P_WriteRegister(nRF, nRF_STATUS, &regStatus);
        HAL_nRF24L01P_CE_High(nRF);
        nRF->Busy = 0;
    }
    return HAL_OK;
}
