/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM_32_NRF24_PE_NRF24_2_H
#define STM_32_NRF24_PE_NRF24_2_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include "PE_nRF24.h"

/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

typedef enum {
    PE_nRF24_STATUS_OK      = (uint8_t) 0U,
    PE_nRF24_STATUS_ERROR   = (uint8_t) 1U,
    PE_nRF24_STATUS_TIMEOUT = (uint8_t) 2U,
} PE_nRF24_status_t;

typedef enum {
    PE_nRF24_POWER_OFF = 0U,
    PE_nRF24_POWER_ON  = 1U,
} PE_nRF24_POWER_t;

typedef enum {
    PE_nRF24_PIPE_0 = (uint8_t) 0U,
    PE_nRF24_PIPE_1 = (uint8_t) 1U,
    PE_nRF24_PIPE_2 = (uint8_t) 2U,
    PE_nRF24_PIPE_3 = (uint8_t) 3U,
    PE_nRF24_PIPE_4 = (uint8_t) 4U,
    PE_nRF24_PIPE_5 = (uint8_t) 5U,
} PE_nRF24_pipe_t;

typedef struct {
    uint8_t addressWidth;
    uint8_t txPower;
    uint8_t rfChannel;
    uint8_t dataRate;
    uint8_t retransmitCount;
    uint8_t retransmitDelay;
} PE_nRF24_configTX_t;

typedef struct {
    uint8_t *address;
    uint8_t autoACK;
    uint8_t payloadSize;
} PE_nRF24_configRX_t;

typedef struct {
    void (*setCE) ();
    void (*setCS) ();
    void (*RW) ();
} PE_nRF24_instance_t;

typedef struct {
    PE_nRF24_instance_t *instance;
    PE_nRF24_status_t status;
    PE_nRF24_configTX_t configTX;
} PE_nRF24_handle_t;


/* Exported macro ------------------------------------------------------------*/
/* Exported function prototypes --------------------------------------------- */

PE_nRF24_status_t PE_nRF24_initializeTX(PE_nRF24_handle_t *handle, PE_nRF24_configTX_t *config);

/**
 * Configure RX pipe
 *
 * Set address
 * Set payload length
 * Enable or disable Auto Acknowledgement
 *
 * @param handle
 * @param config
 * @param pipe
 *
 * @return
 */
PE_nRF24_status_t PE_nRF24_initializeRX(PE_nRF24_handle_t *handle, PE_nRF24_configRX_t *config, PE_nRF24_pipe_t pipe);

/**
 * Enable RX pipe
 *
 * @param handle
 * @param pipe
 *
 * @return
 */
PE_nRF24_status_t PE_nRF24_attachRX(PE_nRF24_handle_t *handle, PE_nRF24_pipe_t pipe);

/**
 * Disable RX pipe
 *
 * @param handle
 * @param pipe
 *
 * @return
 */
PE_nRF24_status_t PE_nRF24_detachRX(PE_nRF24_handle_t *handle, PE_nRF24_pipe_t pipe);

//TODO func to wait until event is occurred in without interrupts mode
//TODO send/read data poll/IRQ
PE_nRF24_status_t PE_nRF24_sendPacket(PE_nRF24_handle_t *handle);
PE_nRF24_status_t PE_nRF24_sendPacketIRQ(PE_nRF24_handle_t *handle);
PE_nRF24_status_t PE_nRF24_readPacket(PE_nRF24_handle_t *handle);
PE_nRF24_status_t PE_nRF24_readPacketIRQ(PE_nRF24_handle_t *handle);

/**
 * Handle IRQ triggered by transmitter (if enabled)
 *
 * Must be called inside of concrete CPU external pin IRQ handler
 *
 * @param handle
 *
 * @return
 */
PE_nRF24_status_t PE_nRF24_handleIRQ(PE_nRF24_handle_t *handle);

/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif //STM_32_NRF24_PE_NRF24_2_H
