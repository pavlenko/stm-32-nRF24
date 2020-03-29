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

#ifdef __cplusplus
}
#endif

#endif //PE_NRF24_API_H
