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

void PE_nRF24_setCRCScheme(PE_nRF24_t *handle, PE_nRF24_CRCScheme_t scheme)
{
    uint8_t reg = PE_nRF24_readByte(handle, PE_nRF24_REG_CONFIG);

    // Configure EN_CRC[3] and CRCO[2] bits of the CONFIG register
    reg &= ~(PE_nRF24_CONFIG_EN_CRC|PE_nRF24_CONFIG_CRCO);
    reg |= scheme;

    PE_nRF24_sendByte(handle, PE_nRF24_REG_CONFIG, reg);
}

void PE_nRF24_setAutoAcknowledgment(PE_nRF24_t *handle, uint8_t pipes)
{
    uint8_t reg = PE_nRF24_readByte(handle, PE_nRF24_REG_EN_AA);

    // Set bits in EN_AA register
    reg &= PE_nRF24_EN_AA_ALL;
    reg |= (pipes & PE_nRF24_EN_AA_ALL);

    PE_nRF24_sendByte(handle, PE_nRF24_REG_EN_AA, reg);
}

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

void PE_nRF24_setAddressValue(PE_nRF24_t *handle, PE_nRF24_Pipe_t pipe, const uint8_t *address)
{
    uint8_t width;

    switch (pipe) {
        case PE_nRF24_PIPE_TX:
        case PE_nRF24_PIPE_RX0:
        case PE_nRF24_PIPE_RX1:
            // Get address width
            width = PE_nRF24_getAddressWidth(handle) + 1;

            // Write address in reverse order (LSB byte first)
            address += width;

            handle->setCS(PE_nRF24_PIN_L);

            handle->RW(PE_nRF24_CMD_W_REGISTER | PE_nRF24_REG_mX_ADDR_Pn[pipe]);

            do {
                handle->RW(*address--);
            } while (width--);

            handle->setCS(PE_nRF24_PIN_H);
            break;
        case PE_nRF24_PIPE_RX2:
        case PE_nRF24_PIPE_RX3:
        case PE_nRF24_PIPE_RX4:
        case PE_nRF24_PIPE_RX5:
            // Write address LSB byte (only first byte from the addr buffer)
            PE_nRF24_sendByte(handle, PE_nRF24_REG_mX_ADDR_Pn[pipe], *address);
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

uint8_t PE_nRF24_getStatus(PE_nRF24_t *handle)
{
    return PE_nRF24_readByte(handle, PE_nRF24_REG_STATUS);
}

void PE_nRF24_attachRXPipe(PE_nRF24_t *handle, PE_nRF24_Pipe_t pipe)
{
    uint8_t reg = PE_nRF24_readByte(handle, PE_nRF24_REG_EN_RXADDR);

    reg |= (1U << pipe);
    reg &= PE_nRF24_EN_RXADDR_ALL;

    PE_nRF24_sendByte(handle, PE_nRF24_REG_EN_RXADDR, reg);

    //TODO set payload width, ?? how to receive less bytes
    //TODO enable/disable auto acknowledgment in nRF24_REG_EN_AA
}

void PE_nRF24_detachRXPipe(PE_nRF24_t *handle, PE_nRF24_Pipe_t pipe)
{
    uint8_t reg = PE_nRF24_readByte(handle, PE_nRF24_REG_EN_RXADDR);

    reg &= ~(1U << pipe);
    reg &= PE_nRF24_EN_RXADDR_ALL;

    PE_nRF24_sendByte(handle, PE_nRF24_REG_EN_RXADDR, reg);
}

void PE_nRF24_readPayload(PE_nRF24_t *handle, PE_nRF24_Pipe_t pipe, uint8_t *data, uint8_t *size)
{
    *size = PE_nRF24_readByte(handle, PE_nRF24_REG_RX_PW_Pn[pipe]);

    if (*size > 0) {
        PE_nRF24_readData(handle, PE_nRF24_CMD_R_RX_PAYLOAD, data, *size);
    }
}

void PE_nRF24_setPayload(PE_nRF24_t *handle, uint8_t *data, uint8_t size)
{
    PE_nRF24_sendData(handle, PE_nRF24_CMD_W_TX_PAYLOAD, data, size);
}

void PE_nRF24_IRQHandler(PE_nRF24_t *handle)
{
    uint8_t pipe;
    uint8_t fifo;
    uint8_t reg = PE_nRF24_readByte(handle, PE_nRF24_REG_STATUS);

    if ((reg & PE_nRF24_STATUS_MAX_RT) != 0U) {
        PE_nRF24_MaxRetriesHandler(handle);
        return;
    }

    if ((reg & PE_nRF24_STATUS_TX_DS) != 0U) {
        PE_nRF24_TXDataSentHandler(handle);
        return;
    }

    if ((reg & PE_nRF24_STATUS_RX_DR) != 0U) {
        pipe = (reg & PE_nRF24_STATUS_RX_P_NO) >> PE_nRF24_STATUS_RX_P_NO_Pos;

        // Read payload data
        PE_nRF24_readPayload(handle, pipe, handle->rxBufferData, handle->rxBufferSize);

        // Clear RX_DR interrupt
        PE_nRF24_clearIRQFlags(handle);

        // Check exists more payloads
        fifo = PE_nRF24_readByte(handle, PE_nRF24_REG_FIFO_STATUS);

        if ((fifo & PE_nRF24_FIFO_STATUS_RX_FULL)) {
            //TODO
        }

        PE_nRF24_RXDataReadyHandler(handle, pipe);
    }
    /**
     * The Data Ready interrupt is set by a new packet arrival event. The procedure for handling this interrupt should be:
     * 1) read payload via SPI
     * 2) clear RX_DR interrupt
     * 3) read FIFO_STATUS to check if there are more payloads available in RX FIFO
     * 4) if there are more data in RX FIFO repeat from 1).
     */
}

__attribute__((weak)) void PE_nRF24_RXDataReadyHandler(PE_nRF24_t *handle, uint8_t pipe)
{
    (void) handle;
    (void) pipe;
}

__attribute__((weak)) void PE_nRF24_TXDataSentHandler(PE_nRF24_t *handle)
{
    (void) handle;
}

__attribute__((weak)) void PE_nRF24_MaxRetriesHandler(PE_nRF24_t *handle)
{
    (void) handle;
}
