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
static PE_nRF24_RESULT_t PE_nRF24_setDirection(PE_nRF24_t *handle, PE_nRF24_DIRECTION_t direction);
static PE_nRF24_RESULT_t PE_nRF24_setPowerMode(PE_nRF24_t *handle, PE_nRF24_POWER_t value);
static PE_nRF24_RESULT_t PE_nRF24_setRFChannel(PE_nRF24_t *handle, uint8_t value);
static PE_nRF24_RESULT_t PE_nRF24_handleIRQ_RX_DR(PE_nRF24_t *handle);
static PE_nRF24_RESULT_t PE_nRF24_handleIRQ_TX_DS(PE_nRF24_t *handle);
static PE_nRF24_RESULT_t PE_nRF24_handleIRQ_MAX_RT(PE_nRF24_t *handle);

/* Private functions ---------------------------------------------------------*/

PE_nRF24_RESULT_t PE_nRF24_configureRF(PE_nRF24_t *handle, PE_nRF24_configRF_t *config)
{
    return PE_nRF24_RESULT_OK;//TODO
}

PE_nRF24_RESULT_t PE_nRF24_configureTX(PE_nRF24_t *handle, PE_nRF24_configTX_t *config)
{
    return PE_nRF24_RESULT_OK;//TODO
}

PE_nRF24_RESULT_t PE_nRF24_configureRX(PE_nRF24_t *handle, PE_nRF24_configRX_t *config, PE_nRF24_PIPE_t pipe)
{
    //TODO Configure pipe address

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

static PE_nRF24_RESULT_t PE_nRF24_setDirection(PE_nRF24_t *handle, PE_nRF24_DIRECTION_t direction)
{
    uint8_t reg;
    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_STATUS_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg &= ~PE_nRF24_CONFIG_PRIM_RX;
    reg |= (direction << PE_nRF24_CONFIG_PRIM_RX_Pos);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_STATUS_OK) {
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

    handle->status = PE_nRF24_STATUS_OK;

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

    handle->status = PE_nRF24_STATUS_OK;

    return PE_nRF24_RESULT_OK;
}