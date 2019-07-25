/* Includes ------------------------------------------------------------------*/

#include "PE_nRF24.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static uint8_t PE_nRF24_NONE;

/* Private function prototypes -----------------------------------------------*/

static PE_nRF24_RESULT_t PE_nRF24_getRegister(PE_nRF24_t *handle, uint8_t addr, uint8_t *byte);
static PE_nRF24_RESULT_t PE_nRF24_setRegister(PE_nRF24_t *handle, uint8_t addr, uint8_t *byte);
static PE_nRF24_RESULT_t PE_nRF24_attachIRQ(PE_nRF24_t *handle, PE_nRF24_IRQ_t mask);
static PE_nRF24_RESULT_t PE_nRF24_detachIRQ(PE_nRF24_t *handle, PE_nRF24_IRQ_t mask);
static PE_nRF24_RESULT_t PE_nRF24_clearIRQ(PE_nRF24_t *handle, PE_nRF24_IRQ_t mask);
static PE_nRF24_RESULT_t PE_nRF24_flushTX(PE_nRF24_t *handle);
static PE_nRF24_RESULT_t PE_nRF24_flushRX(PE_nRF24_t *handle);
static PE_nRF24_RESULT_t PE_nRF24_setAddressWidth(PE_nRF24_t *handle, PE_nRF24_ADDR_WIDTH_t width);
static PE_nRF24_RESULT_t PE_nRF24_getAddressWidth(PE_nRF24_t *handle, PE_nRF24_ADDR_WIDTH_t *width);
static PE_nRF24_RESULT_t PE_nRF24_setTXAddress(PE_nRF24_t *handle, uint8_t *address);
static PE_nRF24_RESULT_t PE_nRF24_setRXAddress(PE_nRF24_t *handle, uint8_t *address, PE_nRF24_PIPE_t pipe);
static PE_nRF24_RESULT_t PE_nRF24_setDataRate(PE_nRF24_t *handle, PE_nRF24_DATA_RATE_t rate);
static PE_nRF24_RESULT_t PE_nRF24_setCRCScheme(PE_nRF24_t *handle, PE_nRF24_CRC_SCHEME_t scheme);
static PE_nRF24_RESULT_t PE_nRF24_setTXPower(PE_nRF24_t *handle, PE_nRF24_TX_POWER_t power);
static PE_nRF24_RESULT_t PE_nRF24_setRetransmit(PE_nRF24_t *handle, PE_nRF24_RETRY_COUNT_t count, PE_nRF24_RETRY_DELAY_t delay);
static PE_nRF24_RESULT_t PE_nRF24_setDirection(PE_nRF24_t *handle, PE_nRF24_DIRECTION_t direction);
static PE_nRF24_RESULT_t PE_nRF24_setPowerMode(PE_nRF24_t *handle, PE_nRF24_POWER_t value);
static PE_nRF24_RESULT_t PE_nRF24_setRFChannel(PE_nRF24_t *handle, uint8_t value);
static PE_nRF24_RESULT_t PE_nRF24_handleIRQ_RX_DR(PE_nRF24_t *handle);
static PE_nRF24_RESULT_t PE_nRF24_handleIRQ_TX_DS(PE_nRF24_t *handle);
static PE_nRF24_RESULT_t PE_nRF24_handleIRQ_MAX_RT(PE_nRF24_t *handle);

/* Private functions ---------------------------------------------------------*/

PE_nRF24_RESULT_t PE_nRF24_configureRF(PE_nRF24_t *handle, PE_nRF24_configRF_t *config)
{
    // Configure address width
    if (PE_nRF24_setAddressWidth(handle, config->addressWidth) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    // Configure data rate
    if (PE_nRF24_setDataRate(handle, config->dataRate) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    // Configure RF channel
    if (PE_nRF24_setRFChannel(handle, config->rfChannel) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    // Configure CRC
    if (PE_nRF24_setCRCScheme(handle, config->crcScheme) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_configureTX(PE_nRF24_t *handle, PE_nRF24_configTX_t *config)
{
    // Configure tx address
    if (PE_nRF24_setTXAddress(handle, config->address) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    // Configure tx out power
    if (PE_nRF24_setTXPower(handle, config->txPower) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    // Configure tx retransmit
    if (PE_nRF24_setRetransmit(handle, config->retryCount, config->retryDelay) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_configureRX(PE_nRF24_t *handle, PE_nRF24_configRX_t *config, PE_nRF24_PIPE_t pipe)
{
    // Configure pipe address
    if (PE_nRF24_setRXAddress(handle, config->address, pipe) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    //TODO Configure pipe auto acknowledgement

    // Configure pipe payload width
    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_RX_PW[pipe], &(config->payloadSize)) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_handleIRQ(PE_nRF24_t *handle)
{
    uint8_t status;

    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_STATUS, &status) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    // Process RX data ready (RX_DR bit)
    if ((status & PE_nRF24_STATUS_RX_DR) != 0U) {
        PE_nRF24_handleIRQ_RX_DR(handle);
    }

    // Process TX sent (TX_DS bit)
    if ((status & PE_nRF24_STATUS_TX_DS) != 0) {
        PE_nRF24_handleIRQ_TX_DS(handle);
    }

    // Process reach retransmission count (MAX_RT bit)
    if ((status & PE_nRF24_STATUS_MAX_RT) != 0) {
        PE_nRF24_handleIRQ_MAX_RT(handle);
    }

    return PE_nRF24_RESULT_OK;
}

static PE_nRF24_RESULT_t PE_nRF24_getRegister(PE_nRF24_t *handle, uint8_t addr, uint8_t *byte)
{
    handle->setCS(PE_nRF24_BIT_CLR);

    if (handle->read(PE_nRF24_CMD_R_REGISTER | addr, byte, 1) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    handle->setCS(PE_nRF24_BIT_SET);

    return PE_nRF24_RESULT_OK;
}

static PE_nRF24_RESULT_t PE_nRF24_setRegister(PE_nRF24_t *handle, uint8_t addr, uint8_t *byte)
{
    handle->setCS(PE_nRF24_BIT_CLR);

    if (handle->send(PE_nRF24_CMD_W_REGISTER | addr, byte, 1) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    handle->setCS(PE_nRF24_BIT_SET);

    return PE_nRF24_RESULT_OK;
}

static PE_nRF24_RESULT_t PE_nRF24_attachIRQ(PE_nRF24_t *handle, PE_nRF24_IRQ_t mask)
{
    uint8_t reg;

    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg &= ~(mask & PE_nRF24_IRQ_MASK_ALL);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

static PE_nRF24_RESULT_t PE_nRF24_detachIRQ(PE_nRF24_t *handle, PE_nRF24_IRQ_t mask)
{
    uint8_t reg;

    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg |= (mask & PE_nRF24_IRQ_MASK_ALL);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

static PE_nRF24_RESULT_t PE_nRF24_clearIRQ(PE_nRF24_t *handle, PE_nRF24_IRQ_t mask)
{
    uint8_t reg;

    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_STATUS, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg |= (mask & PE_nRF24_IRQ_MASK_ALL);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_STATUS, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

static PE_nRF24_RESULT_t PE_nRF24_flushTX(PE_nRF24_t *handle)
{
    return handle->send(PE_nRF24_CMD_FLUSH_TX, &PE_nRF24_NONE, 0);
}

static PE_nRF24_RESULT_t PE_nRF24_flushRX(PE_nRF24_t *handle)
{
    return handle->send(PE_nRF24_CMD_FLUSH_RX, &PE_nRF24_NONE, 0);
}

static PE_nRF24_RESULT_t PE_nRF24_setAddressWidth(PE_nRF24_t *handle, PE_nRF24_ADDR_WIDTH_t width)
{
    uint8_t reg;
    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_SETUP_AW, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg &= ~PE_nRF24_SETUP_AW;
    reg |= width;

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_SETUP_AW, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

static PE_nRF24_RESULT_t PE_nRF24_getAddressWidth(PE_nRF24_t *handle, PE_nRF24_ADDR_WIDTH_t *width)
{
    uint8_t reg;
    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_SETUP_AW, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    *width = (PE_nRF24_ADDR_WIDTH_t) (reg & PE_nRF24_SETUP_AW);

    if (*width == PE_nRF24_ADDR_WIDTH_ILLEGAL) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

static PE_nRF24_RESULT_t PE_nRF24_setTXAddress(PE_nRF24_t *handle, uint8_t *address)
{
    uint8_t width;

    PE_nRF24_getAddressWidth(handle, (PE_nRF24_ADDR_WIDTH_t *) &width);

    if (handle->send(PE_nRF24_CMD_W_REGISTER + PE_nRF24_REG_TX_ADDR, address, width + 2) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

static PE_nRF24_RESULT_t PE_nRF24_setRXAddress(PE_nRF24_t *handle, uint8_t *address, PE_nRF24_PIPE_t pipe)
{
    uint8_t width;

    PE_nRF24_getAddressWidth(handle, (PE_nRF24_ADDR_WIDTH_t *) &width);

    if (pipe > PE_nRF24_PIPE_RX1) {
        width = 1;
    } else {
        width += 2;
    }

    if (handle->send(PE_nRF24_CMD_W_REGISTER + PE_nRF24_REG_RX_ADDR[pipe], address, width) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

static PE_nRF24_RESULT_t PE_nRF24_setDataRate(PE_nRF24_t *handle, PE_nRF24_DATA_RATE_t rate)
{
    uint8_t reg;
    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_RF_SETUP, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg &= ~(PE_nRF24_RF_SETUP_RF_DR_HIGH|PE_nRF24_RF_SETUP_RF_DR_LOW);
    reg |= rate;

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_RF_SETUP, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

static PE_nRF24_RESULT_t PE_nRF24_setCRCScheme(PE_nRF24_t *handle, PE_nRF24_CRC_SCHEME_t scheme)
{
    uint8_t reg;
    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg &= ~(PE_nRF24_CONFIG_EN_CRC|PE_nRF24_CONFIG_CRCO);
    reg |= (scheme << PE_nRF24_CONFIG_CRCO_Pos);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

static PE_nRF24_RESULT_t PE_nRF24_setTXPower(PE_nRF24_t *handle, PE_nRF24_TX_POWER_t power)
{
    uint8_t reg;
    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_RF_SETUP, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg &= ~PE_nRF24_RF_SETUP_RF_PWR;
    reg |= (power << PE_nRF24_RF_SETUP_RF_PWR_Pos);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_RF_SETUP, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

static PE_nRF24_RESULT_t PE_nRF24_setRetransmit(PE_nRF24_t *handle, PE_nRF24_RETRY_COUNT_t count, PE_nRF24_RETRY_DELAY_t delay)
{
    uint8_t reg;
    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_SETUP_RETR, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg &= ~(PE_nRF24_SETUP_RETR_ARD|PE_nRF24_SETUP_RETR_ARC);
    reg |= (count << PE_nRF24_SETUP_RETR_ARC_Pos);
    reg |= (delay << PE_nRF24_SETUP_RETR_ARD_Pos);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_SETUP_RETR, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

static PE_nRF24_RESULT_t PE_nRF24_setDirection(PE_nRF24_t *handle, PE_nRF24_DIRECTION_t direction)
{
    uint8_t reg;
    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg &= ~PE_nRF24_CONFIG_PRIM_RX;
    reg |= (direction << PE_nRF24_CONFIG_PRIM_RX_Pos);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

static PE_nRF24_RESULT_t PE_nRF24_setPowerMode(PE_nRF24_t *handle, PE_nRF24_POWER_t value)
{
    uint8_t reg;

    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg &= ~PE_nRF24_CONFIG_PWR_UP;
    reg |= (value << PE_nRF24_CONFIG_PWR_UP_Pos);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

static PE_nRF24_RESULT_t PE_nRF24_setRFChannel(PE_nRF24_t *handle, uint8_t value)
{
    uint8_t reg = (value & PE_nRF24_RF_CH);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_RF_CH, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

static PE_nRF24_RESULT_t PE_nRF24_handleIRQ_RX_DR(PE_nRF24_t *handle)
{
    uint8_t statusFIFO;

    handle->setCE(0);

    do {
        //TODO read bytes to specific pipe
        uint8_t data[32];
        uint8_t size;
        uint8_t pipe;

        //TODO PE_nRF24_readPayload(handle, data, &size, &pipe);

        //TODO execute callback

        // Clear pending IRQ
        PE_nRF24_clearIRQ(handle, PE_nRF24_IRQ_RX_DR);

        PE_nRF24_getRegister(handle, PE_nRF24_REG_FIFO_STATUS, &statusFIFO);
    } while ((statusFIFO & PE_nRF24_FIFO_STATUS_RX_EMPTY) == 0x00);

    handle->setCE(1);

    return PE_nRF24_RESULT_OK;
}

static PE_nRF24_RESULT_t PE_nRF24_handleIRQ_TX_DS(PE_nRF24_t *handle)
{
    handle->setCE(0);

    // Set direction to RX
    PE_nRF24_setDirection(handle, PE_nRF24_DIRECTION_RX);

    // Clear pending IRQ
    PE_nRF24_clearIRQ(handle, PE_nRF24_IRQ_TX_DS);

    handle->setCE(1);

    handle->status = PE_nRF24_STATUS_READY;

    return PE_nRF24_RESULT_OK;
}

static PE_nRF24_RESULT_t PE_nRF24_handleIRQ_MAX_RT(PE_nRF24_t *handle)
{
    PE_nRF24_flushTX(handle);

    // Toggle RF power up bit
    PE_nRF24_setPowerMode(handle, PE_nRF24_POWER_OFF);
    PE_nRF24_setPowerMode(handle, PE_nRF24_POWER_ON);

    handle->setCE(0);

    // Set direction to RX
    PE_nRF24_setDirection(handle, PE_nRF24_DIRECTION_RX);

    // Clear pending IRQ
    PE_nRF24_clearIRQ(handle, PE_nRF24_IRQ_MAX_RT);

    handle->setCE(1);

    handle->status = PE_nRF24_STATUS_READY;

    return PE_nRF24_RESULT_OK;
}