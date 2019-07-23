/* Includes ------------------------------------------------------------------*/

#include "PE_nRF24.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static PE_nRF24_STATUS_t PE_nRF24_readByte(PE_nRF24_t *handle, uint8_t addr, uint8_t *byte)
{
    //TODO
    (void) handle;
    (void) addr;
    (void) byte;
    return PE_nRF24_STATUS_OK;
}

static PE_nRF24_STATUS_t PE_nRF24_sendByte(PE_nRF24_t *handle, uint8_t addr, uint8_t *byte)
{
    //TODO
    (void) handle;
    (void) addr;
    (void) byte;
    return PE_nRF24_STATUS_OK;
}

PE_nRF24_STATUS_t PE_nRF24_handleIRQ(PE_nRF24_t *handle)
{
    uint8_t status;

    if (PE_nRF24_readByte(handle, PE_nRF24_REG_STATUS, &status) != PE_nRF24_STATUS_OK) {
        return PE_nRF24_STATUS_ERROR;
    }

    // Process RX data ready (RX_DR bit)
    if ((status & PE_nRF24_STATUS_RX_DR) != 0U) {
        uint8_t statusFIFO;

        handle->setCE(0);

        do {
            //TODO read bytes to specific pipe
            uint8_t data[32];
            uint8_t size;
            uint8_t pipe;

            PE_nRF24_readPayload(handle, data, &size, &pipe);

            //TODO execute callback

            status |= PE_nRF24_STATUS_RX_DR;

            PE_nRF24_sendByte(handle, PE_nRF24_REG_STATUS, &status);
            PE_nRF24_readByte(handle, PE_nRF24_REG_FIFO_STATUS, &statusFIFO);
        } while ((statusFIFO & PE_nRF24_FIFO_STATUS_RX_EMPTY) == 0x00);

        handle->setCE(1);
    }

    // Process TX sent (TX_DS bit)
    if ((status & PE_nRF24_STATUS_TX_DS) != 0) {
        handle->setCE(0);

        status |= PE_nRF24_STATUS_TX_DS;

        PE_nRF24_setDirection(handle, PE_nRF24_DIRECTION_RX);
        PE_nRF24_sendByte(handle, PE_nRF24_REG_STATUS, &status);

        handle->setCE(1);

        handle->status = PE_nRF24_STATUS_OK;
    }

    // Process reach retransmission count (MAX_RT bit)
    if ((status & PE_nRF24_STATUS_MAX_RT) != 0) {
        status |= PE_nRF24_STATUS_MAX_RT;

        PE_nRF24_flushTX(handle);

        // Toggle RF power up bit
        PE_nRF24_setPowerMode(handle, PE_nRF24_POWER_OFF);
        PE_nRF24_setPowerMode(handle, PE_nRF24_POWER_ON);

        handle->setCE(0);

        PE_nRF24_setDirection(handle, PE_nRF24_DIRECTION_RX);
        PE_nRF24_sendByte(handle, PE_nRF24_REG_STATUS, &status);

        handle->setCE(1);

        handle->status = PE_nRF24_STATUS_OK;
    }

    return PE_nRF24_STATUS_OK;
}