#ifndef PE_NRF24_API_H
#define PE_NRF24_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PE_nRF24_def.h"

/**
 * @param handle
 * @param addr
 * @param byte
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_getRegister(PE_nRF24_handle_t *handle, uint8_t addr, uint8_t *byte);

/**
 * @param handle
 * @param addr
 * @param byte
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_setRegister(PE_nRF24_handle_t *handle, uint8_t addr, uint8_t *byte);

/**
 * @param handle
 * @param data
 * @param size
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_getPayload(PE_nRF24_handle_t *handle, uint8_t *data, uint8_t size);

/**
 * @param handle
 * @param data
 * @param size
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_setPayload(PE_nRF24_handle_t *handle, uint8_t *data, uint8_t size);

/**
 * Read packet in blocking mode
 *
 * @param handle
 * @param data
 * @param size
 * @param timeout
 *
 * @return Operation status
 */
PE_nRF24_RESULT_t PE_nRF24_readPacket(PE_nRF24_handle_t *handle, uint8_t *data, uint8_t size, uint16_t timeout);

/**
 * Send packet in blocking mode
 *
 * @param handle
 * @param data
 * @param size
 * @param timeout
 *
 * @return Operation status
 */
PE_nRF24_RESULT_t PE_nRF24_sendPacket(PE_nRF24_handle_t *handle, uint8_t *addr, uint8_t *data, uint8_t size, uint16_t timeout);

/**
 * @param handle
 * @param width
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_setAddressWidth(PE_nRF24_handle_t *handle, PE_nRF24_ADDR_WIDTH_t width);

/**
 * @param handle
 * @param width
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_getAddressWidth(PE_nRF24_handle_t *handle, PE_nRF24_ADDR_WIDTH_t *width);

/**
 * @param handle
 * @param address
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_setTXAddress(PE_nRF24_handle_t *handle, uint8_t *address);

/**
 * @param handle
 * @param address
 * @param pipe
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_setRXAddress(PE_nRF24_handle_t *handle, uint8_t *address, PE_nRF24_PIPE_t pipe);

/**
 * @param handle
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_flushTX(PE_nRF24_handle_t *handle);

/**
 * @param handle
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_flushRX(PE_nRF24_handle_t *handle);

/**
 * @param handle
 * @param mask
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_attachIRQ(PE_nRF24_handle_t *handle, PE_nRF24_IRQ_t mask);

/**
 * @param handle
 * @param mask
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_detachIRQ(PE_nRF24_handle_t *handle, PE_nRF24_IRQ_t mask);

/**
 * @param handle
 * @param mask
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_clearIRQ(PE_nRF24_handle_t *handle, PE_nRF24_IRQ_t mask);

/**
 * @param handle
 * @param mask
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_checkIRQ(PE_nRF24_handle_t *handle, PE_nRF24_IRQ_t mask);

/**
 * @param handle
 * @param direction
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_setDirection(PE_nRF24_handle_t *handle, PE_nRF24_DIRECTION_t direction);

/**
 * @return
 */
uint32_t PE_nRF24_getMillis(void);

extern const uint8_t PE_nRF24_REG_RX_PW[6];
extern const uint8_t PE_nRF24_REG_RX_ADDR[7];

#ifdef __cplusplus
}
#endif

#endif //PE_NRF24_API_H
