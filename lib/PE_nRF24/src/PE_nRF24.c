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
 * Platform dependent get clock ms, MUST be implemented in user file
 *
 * @return
 */
__attribute__((weak)) uint32_t PE_nRF24_clock(void)
{
    return 0;
}
