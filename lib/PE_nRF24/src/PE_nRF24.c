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

//TODO create functions like i2c_mem_read, i2c_mem_write

static PE_nRF24_STATUS_t PE_nRF24_getRegister(PE_nRF24_t *handle, uint8_t addr, uint8_t *byte)
{
    handle->setCS(PE_nRF24_BIT_CLR);

    addr |= PE_nRF24_CMD_R_REGISTER;

    if (handle->send(&addr, 1) != PE_nRF24_STATUS_OK) {
        return PE_nRF24_STATUS_ERROR;
    }

    if (handle->read(byte, 1) != PE_nRF24_STATUS_OK) {
        return PE_nRF24_STATUS_ERROR;
    }

    handle->setCS(PE_nRF24_BIT_SET);

    return PE_nRF24_STATUS_OK;
}

static PE_nRF24_STATUS_t PE_nRF24_setRegister(PE_nRF24_t *handle, uint8_t addr, uint8_t *byte)
{
    handle->setCS(PE_nRF24_BIT_CLR);

    addr |= PE_nRF24_CMD_W_REGISTER;

    if (handle->send(&addr, 1) != PE_nRF24_STATUS_OK) {
        return PE_nRF24_STATUS_ERROR;
    }

    if (handle->send(byte, 1) != PE_nRF24_STATUS_OK) {
        return PE_nRF24_STATUS_ERROR;
    }

    handle->setCS(PE_nRF24_BIT_SET);

    return PE_nRF24_STATUS_OK;
}

#define PE_nRF24_setDirection(handle, value) \
    do { \
        uint8_t reg; \
        if (PE_nRF24_getRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_STATUS_OK) return PE_nRF24_STATUS_ERROR; \
        reg &= ~PE_nRF24_CONFIG_PRIM_RX; \
        reg |= ((value & 0x1U) << PE_nRF24_CONFIG_PRIM_RX_Pos); \
        if (PE_nRF24_setRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_STATUS_OK) return PE_nRF24_STATUS_ERROR; \
    } while (0U);

#define PE_nRF24_setPowerMode(handle, value) \
    do { \
        uint8_t reg; \
        if (PE_nRF24_getRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_STATUS_OK) return PE_nRF24_STATUS_ERROR; \
        reg &= ~PE_nRF24_CONFIG_PWR_UP; \
        reg |= ((value & 0x1U) << PE_nRF24_CONFIG_PWR_UP_Pos); \
        if (PE_nRF24_setRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_STATUS_OK) return PE_nRF24_STATUS_ERROR; \
    } while (0U);


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

            //TODO PE_nRF24_readPayload(handle, data, &size, &pipe);

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

        //TODO PE_nRF24_flushTX(handle);

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