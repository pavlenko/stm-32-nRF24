/* Includes ------------------------------------------------------------------*/

#include "PE_nRF24.h"
#include "PE_nRF24_api.h"

#include <stddef.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static PE_nRF24_RESULT_t PE_nRF24_handleIRQ_RX_DR(PE_nRF24_handle_t *handle);
static PE_nRF24_RESULT_t PE_nRF24_handleIRQ_TX_DS(PE_nRF24_handle_t *handle);
static PE_nRF24_RESULT_t PE_nRF24_handleIRQ_MAX_RT(PE_nRF24_handle_t *handle);

/* Private functions ---------------------------------------------------------*/

/**
 * Initialize transmitter RF options and TX options
 *
 * @param handle
 *
 * @return Operation result
 */
PE_nRF24_RESULT_t PE_nRF24_configureRF(PE_nRF24_handle_t *handle)
{
    uint8_t reg;
    PE_nRF24_RESULT_t result = PE_nRF24_RESULT_OK;

    // Set device power up
    if (PE_nRF24_setPowerMode(handle, PE_nRF24_POWER_ON) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    // Check if prev operation success
    do {
        if (PE_nRF24_getRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
            return PE_nRF24_RESULT_ERROR;
        }
    } while ((reg & PE_nRF24_CONFIG_PWR_UP) == 0x00);

    result |= PE_nRF24_setAddressWidth(handle, handle->config.addressWidth);
    result |= PE_nRF24_setDataRate(handle, handle->config.dataRate);
    result |= PE_nRF24_setRFChannel(handle, handle->config.rfChannel);
    result |= PE_nRF24_setCRCScheme(handle, handle->config.crcScheme);
    result |= PE_nRF24_setTXPower(handle, handle->config.txPower);
    result |= PE_nRF24_setRetransmit(handle, handle->config.retryCount, handle->config.retryDelay);
    result |= PE_nRF24_detachIRQ(handle, PE_nRF24_IRQ_ALL);
    result |= PE_nRF24_clearIRQ(handle, PE_nRF24_IRQ_ALL);
    result |= PE_nRF24_flushTX(handle);
    result |= PE_nRF24_flushRX(handle);

    if (result != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

/**
 * Initialize transmitter RX pipe
 *
 * @param handle
 * @param config
 * @param pipe
 *
 * @return Operation result
 */
PE_nRF24_RESULT_t PE_nRF24_configureRX(PE_nRF24_handle_t *handle, PE_nRF24_configRX_t *config, PE_nRF24_PIPE_t pipe)
{
    PE_nRF24_RESULT_t result = PE_nRF24_RESULT_OK;

    result |= PE_nRF24_setRXAddress(handle, config->address, pipe);
    result |= PE_nRF24_setAutoACK(handle, config->autoACK, pipe);
    result |= PE_nRF24_setRegister(handle, PE_nRF24_REG_RX_PW[pipe], &(config->payloadSize));

    if (result != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

/**
 * Enable listen RX pipe address
 *
 * @param handle
 * @param pipe
 *
 * @return Operation result
 */
PE_nRF24_RESULT_t PE_nRF24_attachRXPipe(PE_nRF24_handle_t *handle, PE_nRF24_PIPE_t pipe)
{
    uint8_t reg;

    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_EN_RXADDR, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg |= (1U << pipe);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_EN_RXADDR, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

/**
 * Disable listen RX pipe address
 *
 * @param handle
 * @param pipe
 *
 * @return Operation result
 */
PE_nRF24_RESULT_t PE_nRF24_detachRXPipe(PE_nRF24_handle_t *handle, PE_nRF24_PIPE_t pipe)
{
    uint8_t reg;

    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_EN_RXADDR, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg &= ~(1U << pipe);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_EN_RXADDR, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

/**
 * Get lost packets count
 *
 * @param handle
 * @param value
 *
 * @return Operation result
 */
PE_nRF24_RESULT_t PE_nRF24_getLostCount(PE_nRF24_handle_t *handle, PE_nRF24_RETRY_COUNT_t *value)
{
    uint8_t reg;

    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_OBSERVE_TX, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    *value = (PE_nRF24_RETRY_COUNT_t) ((reg & PE_nRF24_OBSERVE_TX_PLOS_CNT) >> PE_nRF24_OBSERVE_TX_PLOS_CNT_Pos);

    return PE_nRF24_RESULT_OK;
}

/**
 * Get retries count
 *
 * @param handle
 * @param value
 *
 * @return Operation result
 */
PE_nRF24_RESULT_t PE_nRF24_getRetryCount(PE_nRF24_handle_t *handle, PE_nRF24_RETRY_COUNT_t *value)
{
    uint8_t reg;

    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_OBSERVE_TX, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    *value = (PE_nRF24_RETRY_COUNT_t) ((reg & PE_nRF24_OBSERVE_TX_ARC_CNT) >> PE_nRF24_OBSERVE_TX_ARC_CNT_Pos);

    return PE_nRF24_RESULT_OK;
}

/**
 * Get carrier detect bit value
 *
 * @param handle
 * @param value
 *
 * @return Operation result
 */
PE_nRF24_RESULT_t PE_nRF24_getCarrierDetect(PE_nRF24_handle_t *handle, PE_nRF24_BIT_t *value)
{
    uint8_t reg;

    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_CD, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    *value = (PE_nRF24_BIT_t) (reg & PE_nRF24_CD_BIT);

    return PE_nRF24_RESULT_OK;
}

/**
 * Handle IRQ triggered by transmitter (if enabled)
 *
 * Must be called inside of concrete CPU external pin IRQ handler
 *
 * @param handle
 *
 * @return Operation status
 */
PE_nRF24_RESULT_t PE_nRF24_handleIRQ(PE_nRF24_handle_t *handle)
{
    uint8_t status;

    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_STATUS, &status) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    // Process RX data ready (RX_DR bit)
    if ((status & (uint8_t) PE_nRF24_IRQ_RX_DR) != 0U) {
        PE_nRF24_handleIRQ_RX_DR(handle);
    }

    // Process TX sent (TX_DS bit)
    if ((status & (uint8_t) PE_nRF24_IRQ_TX_DS) != 0) {
        PE_nRF24_handleIRQ_TX_DS(handle);
    }

    // Process reach retransmission count (MAX_RT bit)
    if ((status & (uint8_t) PE_nRF24_IRQ_MAX_RT) != 0) {
        PE_nRF24_handleIRQ_MAX_RT(handle);
    }

    return PE_nRF24_RESULT_OK;
}

__attribute__((weak))
void PE_nRF24_SPI_setCE(PE_nRF24_handle_t *handle, PE_nRF24_BIT_t state) {
    (void) handle;
    (void) state;
}

__attribute__((weak))
void PE_nRF24_SPI_setCS(PE_nRF24_handle_t *handle, PE_nRF24_BIT_t state) {
    (void) handle;
    (void) state;
}

/**
 * Callback for handle transmission completed IRQ
 *
 * When the callback is needed, the callback can be implemented in the user file
 *
 * @param handle
 */
__attribute__((weak)) void PE_nRF24_TXComplete(PE_nRF24_handle_t *handle)
{
    (void) handle;
}

/**
 * Callback for handle reception completed IRQ
 *
 * When the callback is needed, the callback can be implemented in the user file
 *
 * @param handle
 */
__attribute__((weak)) void PE_nRF24_RXComplete(PE_nRF24_handle_t *handle)
{
    (void) handle;
}

/**
 * Platform dependent get clock ms, MUST be implemented in user file
 *
 * @return
 */
__attribute__((weak)) uint32_t PE_nRF24_clock(void)
{
    return 0;
}

/**
 * Process RX data ready IRQ
 *
 * @param handle
 *
 * @return Operation result
 */
static PE_nRF24_RESULT_t PE_nRF24_handleIRQ_RX_DR(PE_nRF24_handle_t *handle)
{
    uint8_t statusFIFO;

    PE_nRF24_SPI_setCE(handle, PE_nRF24_BIT_CLR);

    do {
        // Read payload to internal buffer
        if (handle->bufferData != NULL && handle->bufferSize > 0) {
            PE_nRF24_getPayload(handle, handle->bufferData, handle->bufferSize);
        }

        // Clear pending IRQ
        PE_nRF24_clearIRQ(handle, PE_nRF24_IRQ_RX_DR);

        PE_nRF24_getRegister(handle, PE_nRF24_REG_FIFO_STATUS, &statusFIFO);

        PE_nRF24_RXComplete(handle);
    } while ((statusFIFO & PE_nRF24_FIFO_STATUS_RX_EMPTY) == 0x00);

    PE_nRF24_SPI_setCE(handle, PE_nRF24_BIT_SET);

    return PE_nRF24_RESULT_OK;
}

/**
 * Process TX data send IRQ
 *
 * @param handle
 *
 * @return Operation result
 */
static PE_nRF24_RESULT_t PE_nRF24_handleIRQ_TX_DS(PE_nRF24_handle_t *handle)
{
    PE_nRF24_SPI_setCE(handle, PE_nRF24_BIT_CLR);

    // Set direction to RX
    PE_nRF24_setDirection(handle, PE_nRF24_DIRECTION_RX);

    // Clear pending IRQ
    PE_nRF24_clearIRQ(handle, PE_nRF24_IRQ_TX_DS);

    PE_nRF24_SPI_setCE(handle, PE_nRF24_BIT_SET);

    handle->status = PE_nRF24_STATUS_READY;

    PE_nRF24_TXComplete(handle);

    return PE_nRF24_RESULT_OK;
}

/**
 * Process TX max retransmit reached IRQ
 *
 * @param handle
 *
 * @return Operation result
 */
static PE_nRF24_RESULT_t PE_nRF24_handleIRQ_MAX_RT(PE_nRF24_handle_t *handle)
{
    PE_nRF24_flushTX(handle);

    // Toggle RF power up bit
    PE_nRF24_setPowerMode(handle, PE_nRF24_POWER_OFF);
    PE_nRF24_setPowerMode(handle, PE_nRF24_POWER_ON);

    PE_nRF24_SPI_setCE(handle, PE_nRF24_BIT_CLR);

    // Set direction to RX
    PE_nRF24_setDirection(handle, PE_nRF24_DIRECTION_RX);

    // Clear pending IRQ
    PE_nRF24_clearIRQ(handle, PE_nRF24_IRQ_MAX_RT);

    PE_nRF24_SPI_setCE(handle, PE_nRF24_BIT_SET);

    handle->status = PE_nRF24_STATUS_READY;

    return PE_nRF24_RESULT_OK;
}