#include "PE_nRF24_api.h"
#include "PE_nRF24_spi.h"

/**
 * Get single byte register value
 *
 * @param handle
 * @param addr
 * @param byte
 *
 * @return Operation result
 */
PE_nRF24_RESULT_t PE_nRF24_getRegister(PE_nRF24_handle_t *handle, uint8_t addr, uint8_t *byte)
{
    PE_nRF24_setSS0(handle);

    if (PE_nRF24_readMem(handle, PE_nRF24_CMD_R_REGISTER | addr, byte, 1) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    PE_nRF24_setSS1(handle);

    return PE_nRF24_RESULT_OK;
}

/**
 * Set single byte register value
 *
 * @param handle
 * @param addr
 * @param byte
 *
 * @return Operation result
 */
PE_nRF24_RESULT_t PE_nRF24_setRegister(PE_nRF24_handle_t *handle, uint8_t addr, uint8_t *byte)
{
    PE_nRF24_setSS0(handle);

    if (PE_nRF24_sendMem(handle, PE_nRF24_CMD_W_REGISTER | addr, byte, 1) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    PE_nRF24_setSS1(handle);

    return PE_nRF24_RESULT_OK;
}