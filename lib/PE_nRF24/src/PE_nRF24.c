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
    PE_nRF24_sendByte(handle, PE_nRF24_REG_CONFIG, PE_nRF24_CONFIG_RESET);
    PE_nRF24_sendByte(handle, PE_nRF24_REG_EN_AA, PE_nRF24_EN_AA_RESET);
    PE_nRF24_sendByte(handle, PE_nRF24_REG_EN_RXADDR, PE_nRF24_EN_RXADDR_RESET);
    PE_nRF24_sendByte(handle, PE_nRF24_REG_SETUP_AW, PE_nRF24_SETUP_AW_RESET);
    PE_nRF24_sendByte(handle, PE_nRF24_REG_SETUP_RETR, PE_nRF24_SETUP_RETR_RESET);
    PE_nRF24_sendByte(handle, PE_nRF24_REG_RF_CH, PE_nRF24_RF_CH_RESET);
    PE_nRF24_sendByte(handle, PE_nRF24_REG_RF_SETUP, PE_nRF24_RF_SETUP_RESET);
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
    handle->setCS(PE_nRF24_PIN_H);//TODO remove, this used in all data read/send logic
}

void PE_nRF24_flushRX(PE_nRF24_t *handle)
{
    PE_nRF24_sendByte(handle, PE_nRF24_CMD_FLUSH_RX, PE_nRF24_CMD_NOP);
}

void PE_nRF24_flushTX(PE_nRF24_t *handle)
{
    PE_nRF24_sendByte(handle, PE_nRF24_CMD_FLUSH_TX, PE_nRF24_CMD_NOP);
}

void PE_nRF24_clearIRQFlags(PE_nRF24_t *handle)
{
    uint8_t reg;

    // Clear RX_DR, TX_DS and MAX_RT bits of the STATUS register
    reg  = PE_nRF24_readByte(handle, PE_nRF24_REG_STATUS);
    reg |= PE_nRF24_STATUS_RX_DR|PE_nRF24_STATUS_TX_DS|PE_nRF24_STATUS_MAX_RT;

    PE_nRF24_sendByte(handle, PE_nRF24_REG_STATUS, reg);
}

void PE_nRF24_setPowerMode(PE_nRF24_t *handle, PE_nRF24_PowerMode_t mode)
{
    uint8_t reg;

    reg = PE_nRF24_readByte(handle, PE_nRF24_REG_CONFIG);

    if (mode == PE_nRF24_POWER_UP) {
        reg |= PE_nRF24_CONFIG_PWR_UP;
    } else {
        reg &= ~PE_nRF24_CONFIG_PWR_UP;
    }

    //TODO check this instead of if
    reg &= ~PE_nRF24_CONFIG_PWR_UP;
    reg |= mode << PE_nRF24_CONFIG_PWR_UP_Pos;

    PE_nRF24_sendByte(handle, PE_nRF24_REG_CONFIG, reg);
}

void PE_nRF24_setDirection(PE_nRF24_t *handle, PE_nRF24_Direction_t dir)
{
    uint8_t reg;

    reg  = PE_nRF24_readByte(handle, PE_nRF24_REG_CONFIG);

    reg &= ~PE_nRF24_CONFIG_PRIM_RX;
    reg |= (dir & PE_nRF24_CONFIG_PRIM_RX);

    PE_nRF24_sendByte(handle, PE_nRF24_REG_CONFIG, reg);
}

void PE_nRF24_setCRC(PE_nRF24_t *handle, uint8_t crc)
{}

void PE_nRF24_setRFChannel(PE_nRF24_t *handle, uint8_t channel)
{
    PE_nRF24_sendByte(handle, PE_nRF24_REG_RF_CH, (channel & PE_nRF24_RF_CH));
}

void PE_nRF24_setAutoRetry(PE_nRF24_t *handle, PE_nRF24_AutoRetryDelay_t delay, uint8_t tries)
{
    PE_nRF24_sendByte(
        handle,
        PE_nRF24_REG_SETUP_RETR,
        ((delay & PE_nRF24_SETUP_RETR_ARD) << PE_nRF24_SETUP_RETR_ARD_Pos) | ((tries & PE_nRF24_SETUP_RETR_ARC) << PE_nRF24_SETUP_RETR_ARC_Pos)
    );
}

void PE_nRF24_setAddressWidth(PE_nRF24_t *handle, PE_nRF24_AddressWidth_t width)
{
    PE_nRF24_sendByte(handle, PE_nRF24_REG_SETUP_AW, (width & PE_nRF24_SETUP_AW));
}

uint8_t PE_nRF24_getAddressWidth(PE_nRF24_t *handle)
{
    return PE_nRF24_readByte(handle, PE_nRF24_REG_SETUP_AW);
}

void PE_nRF24_setAddressValue(PE_nRF24_t *handle, PE_nRF24_PipeN_t pipe, const uint8_t *address)
{
    uint8_t width;

    switch (pipe) {
        case PE_nRF24_PIPE_TX:
        case PE_nRF24_PIPE0:
        case PE_nRF24_PIPE1:
            // Get address width
            width = PE_nRF24_getAddressWidth(handle) + 1;

            // Write address in reverse order (LSB byte first)
            address += width;

            handle->setCS(PE_nRF24_PIN_L);

            handle->RW(PE_nRF24_CMD_W_REGISTER | pipe);

            do {
                handle->RW(*address--);
            } while (width--);

            handle->setCS(PE_nRF24_PIN_H);
            break;
        case PE_nRF24_PIPE2:
        case PE_nRF24_PIPE3:
        case PE_nRF24_PIPE4:
        case PE_nRF24_PIPE5:
            // Write address LSB byte (only first byte from the addr buffer)
            PE_nRF24_sendByte(handle, pipe, *address);
            break;
        default:
            // Incorrect pipe number -> do nothing
            break;
    }
}

void PE_nRF24_setTXPower(PE_nRF24_t *handle, PE_nRF24_TXPower_t level)
{
    uint8_t reg = PE_nRF24_readByte(handle, PE_nRF24_REG_RF_SETUP);

    // Configure RF_PWR[2:1] bits of the RF_SETUP register
    reg &= ~PE_nRF24_RF_SETUP_RF_PWR;
    reg |= (level << PE_nRF24_RF_SETUP_RF_PWR_Pos);

    PE_nRF24_sendByte(handle, PE_nRF24_REG_RF_SETUP, reg);
}

void PE_nRF24_setDataRate(PE_nRF24_t *handle, PE_nRF24_DataRate_t rate)
{
    uint8_t reg = PE_nRF24_readByte(handle, PE_nRF24_REG_RF_SETUP);

    // Configure RF_DR_LOW[5] and RF_DR_HIGH[3] bits of the RF_SETUP register
    reg &= ~(PE_nRF24_RF_SETUP_RF_DR_HIGH|PE_nRF24_RF_SETUP_RF_DR_LOW);
    reg |= rate;

    PE_nRF24_sendByte(handle, PE_nRF24_REG_RF_SETUP, reg);
}