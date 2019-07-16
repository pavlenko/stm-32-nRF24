/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PE_NRF24_H
#define PE_NRF24_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/* Exported types ------------------------------------------------------------*/

typedef enum {
    PE_nRF24_PIN_L = 0,
    PE_nRF24_PIN_H = 1,
} PE_nRF24_pinState_t;

typedef struct {
    void (*setCE) (PE_nRF24_pinState_t state);
    void (*setCS) (PE_nRF24_pinState_t state);
    uint8_t (*RW) (uint8_t data);
} PE_nRF24_t;

/* Exported constants --------------------------------------------------------*/

/** Registers *****************************************************************/

#define PE_nRF24_REG_CONFIG      0x00U
#define PE_nRF24_REG_EN_AA       0x01U
#define PE_nRF24_REG_EN_RXADDR   0x02U
#define PE_nRF24_REG_SETUP_AW    0x03U
#define PE_nRF24_REG_SETUP_RETR  0x04U
#define PE_nRF24_REG_RF_CH       0x05U
#define PE_nRF24_REG_RF_SETUP    0x06U
#define PE_nRF24_REG_STATUS      0x07U
#define PE_nRF24_REG_OBSERVE_TX  0x08U
#define PE_nRF24_REG_RPD         0x09U
#define PE_nRF24_REG_RX_ADDR_P0  0x0AU
#define PE_nRF24_REG_RX_ADDR_P1  0x0BU
#define PE_nRF24_REG_RX_ADDR_P2  0x0CU
#define PE_nRF24_REG_RX_ADDR_P3  0x0DU
#define PE_nRF24_REG_RX_ADDR_P4  0x0EU
#define PE_nRF24_REG_RX_ADDR_P5  0x0FU
#define PE_nRF24_REG_TX_ADDR     0x10U
#define PE_nRF24_REG_RX_PW_P0    0x11U
#define PE_nRF24_REG_RX_PW_P1    0x12U
#define PE_nRF24_REG_RX_PW_P2    0x13U
#define PE_nRF24_REG_RX_PW_P3    0x14U
#define PE_nRF24_REG_RX_PW_P4    0x15U
#define PE_nRF24_REG_RX_PW_P5    0x16U
#define PE_nRF24_REG_FIFO_STATUS 0x17U
#define PE_nRF24_REG_DYNPD       0x1CU
#define PE_nRF24_REG_FEATURE     0x1DU // Feature Register

#define PE_nRF24_REG_MASK        0x1FU

/** Instructions **************************************************************/

// Register read
#define PE_nRF24_CMD_R_REGISTER 0x00U
#define PE_nRF24_CMD_R_REGISTER_(_reg_) ((0x00U) | ((PE_nRF24_REG_MASK) & (_reg_)))

// Register write
#define PE_nRF24_CMD_W_REGISTER 0x20U
#define PE_nRF24_CMD_W_REGISTER_(_reg_) ((0x20U) | ((PE_nRF24_REG_MASK) & (_reg_)))

// Read RX payload
#define PE_nRF24_CMD_R_RX_PAYLOAD 0x61U

// Write TX payload
#define PE_nRF24_CMD_W_TX_PAYLOAD 0xA0U

// Flush TX FIFO
#define PE_nRF24_CMD_FLUSH_TX 0xE1U

// Flush RX FIFO
#define PE_nRF24_CMD_FLUSH_RX 0xE2U

// Reuse TX payload
#define PE_nRF24_CMD_REUSE_TX_PL 0xE3U

// ONLY FOR nRF24L01+
// Read RX payload width for the top R_RX_PAYLOAD in the RX FIFO
#define PE_nRF24_R_RX_PL_WID 0x60U

// ONLY FOR nRF24L01+
// Write Payload to be transmitted together with ACK packet on PIPE
#define PE_nRF24_W_ACK_PAYLOAD(_pipe_) ((0xA8U) | ((0x7U) & (_pipe_)))

// ONLY FOR nRF24L01+
// Disables AUTO ACK on this specific packet.
#define PE_nRF24_W_TX_PAYLOAD_NO_ACK 0xB0U

// No operation (used for reading status register)
#define PE_nRF24_CMD_NOP 0xFFU

/* Exported macro ------------------------------------------------------------*/
/* Exported function prototypes --------------------------------------------- */
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void PE_nRF24_initialize(PE_nRF24_t *handle);

#ifdef __cplusplus
}
#endif

#endif //PE_NRF24_H
