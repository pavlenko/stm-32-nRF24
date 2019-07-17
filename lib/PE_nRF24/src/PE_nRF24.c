/* Includes ------------------------------------------------------------------*/

#include "PE_nRF24.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static uint8_t PE_nRF24_readByte(PE_nRF24_t *handle, uint8_t addr) {
    uint8_t value;

    handle->setCS(PE_nRF24_PIN_L);

    handle->RW(addr & PE_nRF24_REG_MASK);
    value = handle->RW(PE_nRF24_CMD_NOP);

    handle->setCS(PE_nRF24_PIN_H);

    return value;
}

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

static void PE_nRF24_readData(PE_nRF24_t *handle, uint8_t addr, uint8_t *data, uint8_t size)
{
    handle->setCS(PE_nRF24_PIN_L);

    handle->RW(addr);

    while (size--) {
        *data++ = handle->RW(PE_nRF24_CMD_NOP);
    }

    handle->setCS(PE_nRF24_PIN_H);
}

static void PE_nRF24_sendData(PE_nRF24_t *handle, uint8_t addr, uint8_t *data, uint8_t size)
{
    handle->setCS(PE_nRF24_PIN_L);

    handle->RW(addr);

    while (size--) {
        handle->RW(*data++);
    }

    handle->setCS(PE_nRF24_PIN_H);
}

uint8_t PE_nRF24_check(PE_nRF24_t *handle)
{
    uint8_t buf[5];
    uint8_t i;
    uint8_t *ptr = (uint8_t *) PE_nRF24_TEST_ADDRESS;

    // Write test TX address and read TX_ADDR register
    PE_nRF24_sendData(handle, PE_nRF24_CMD_W_REGISTER | PE_nRF24_REG_TX_ADDR, ptr, 5);
    PE_nRF24_readData(handle, PE_nRF24_CMD_R_REGISTER | PE_nRF24_REG_TX_ADDR, buf, 5);

    // Compare buffers, return error on first mismatch
    for (i = 0; i < 5; i++) {
        if (buf[i] != *ptr++) return 0;
    }

    return 1;
}

void PE_nRF24_initialize(PE_nRF24_t *handle)
{
    // Write registers initial values
    PE_nRF24_sendByte(handle, PE_nRF24_REG_CONFIG, 0x08);
    PE_nRF24_sendByte(handle, PE_nRF24_REG_EN_AA, 0x3F);
    PE_nRF24_sendByte(handle, PE_nRF24_REG_EN_RXADDR, 0x03);
    PE_nRF24_sendByte(handle, PE_nRF24_REG_SETUP_AW, 0x03);
    PE_nRF24_sendByte(handle, PE_nRF24_REG_SETUP_RETR, 0x03);
    PE_nRF24_sendByte(handle, PE_nRF24_REG_RF_CH, 0x02);
    PE_nRF24_sendByte(handle, PE_nRF24_REG_RF_SETUP, 0x0E);
    PE_nRF24_sendByte(handle, PE_nRF24_REG_STATUS, 0x00);
    PE_nRF24_sendByte(handle, PE_nRF24_REG_RX_PW_P0, 0x00);
    PE_nRF24_sendByte(handle, PE_nRF24_REG_RX_PW_P1, 0x00);
    PE_nRF24_sendByte(handle, PE_nRF24_REG_RX_PW_P2, 0x00);
    PE_nRF24_sendByte(handle, PE_nRF24_REG_RX_PW_P3, 0x00);
    PE_nRF24_sendByte(handle, PE_nRF24_REG_RX_PW_P4, 0x00);
    PE_nRF24_sendByte(handle, PE_nRF24_REG_RX_PW_P5, 0x00);
    PE_nRF24_sendByte(handle, PE_nRF24_REG_DYNPD, 0x00);
    PE_nRF24_sendByte(handle, PE_nRF24_REG_FEATURE, 0x00);

    // Clear the FIFO's
    PE_nRF24_flushRX(handle);
    PE_nRF24_flushTX(handle);

    // Clear any pending interrupt flags
    PE_nRF24_clearIRQFlags(handle);

    // De-assert CSN pin (chip release)
    handle->setCS(PE_nRF24_PIN_H);
}

inline void PE_nRF24_flushRX(PE_nRF24_t *handle)
{
    PE_nRF24_sendByte(handle, PE_nRF24_CMD_FLUSH_RX, PE_nRF24_CMD_NOP);
}

inline void PE_nRF24_flushTX(PE_nRF24_t *handle)
{
    PE_nRF24_sendByte(handle, PE_nRF24_CMD_FLUSH_TX, PE_nRF24_CMD_NOP);
}

inline void PE_nRF24_clearIRQFlags(PE_nRF24_t *handle)
{
    uint8_t reg;

    // Clear RX_DR, TX_DS and MAX_RT bits of the STATUS register
    reg  = PE_nRF24_readByte(handle, PE_nRF24_REG_STATUS);
    reg |= PE_nRF24_STATUS_RX_DR|PE_nRF24_STATUS_TX_DS|PE_nRF24_STATUS_MAX_RT;

    PE_nRF24_sendByte(handle, PE_nRF24_REG_STATUS, reg);
}