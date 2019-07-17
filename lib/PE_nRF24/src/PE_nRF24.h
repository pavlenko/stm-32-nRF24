/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PE_NRF24_H
#define PE_NRF24_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/* Exported constants --------------------------------------------------------*/

// Fake address to test transceiver presence (5 bytes long)
#define PE_nRF24_TEST_ADDRESS    "nRF24"

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

/** CONFIG bits ***************************************************************/

// 1 == PRX, 0 == PTX
#define PE_nRF24_CONFIG_PRIM_RX_Pos     (0U)
#define PE_nRF24_CONFIG_PRIM_RX_Msk     (1U << PE_nRF24_CONFIG_PRIM_RX_Pos)
#define PE_nRF24_CONFIG_PRIM_RX         PE_nRF24_CONFIG_PRIM_RX_Msk

// 1 == POWER UP, 0 == POWER DOWN
#define PE_nRF24_CONFIG_PWR_UP_Pos      (1U)
#define PE_nRF24_CONFIG_PWR_UP_Msk      (1U << PE_nRF24_CONFIG_PWR_UP_Pos)
#define PE_nRF24_CONFIG_PWR_UP          PE_nRF24_CONFIG_PWR_UP_Msk

// CRC encoding scheme
#define PE_nRF24_CONFIG_CRCO_Pos        (2U)
#define PE_nRF24_CONFIG_CRCO_Msk        (1U << PE_nRF24_CONFIG_CRCO_Pos)
#define PE_nRF24_CONFIG_CRCO            PE_nRF24_CONFIG_CRCO_Msk

// Enable CRC
#define PE_nRF24_CONFIG_EN_CRC_Pos      (3U)
#define PE_nRF24_CONFIG_EN_CRC_Msk      (1U << PE_nRF24_CONFIG_EN_CRC_Pos)
#define PE_nRF24_CONFIG_EN_CRC          PE_nRF24_CONFIG_EN_CRC_Msk

// Mask interrupt caused by MAX_RT
#define PE_nRF24_CONFIG_MASK_MAX_RT_Pos (4U)
#define PE_nRF24_CONFIG_MASK_MAX_RT_Msk (1U << PE_nRF24_CONFIG_MASK_MAX_RT_Pos)
#define PE_nRF24_CONFIG_MASK_MAX_RT     PE_nRF24_CONFIG_MASK_MAX_RT_Msk

// Mask interrupt caused by TX_DS
#define PE_nRF24_CONFIG_MASK_TX_DS_Pos  (5U)
#define PE_nRF24_CONFIG_MASK_TX_DS_Msk  (1U << PE_nRF24_CONFIG_MASK_TX_DS_Pos)
#define PE_nRF24_CONFIG_MASK_TX_DS      PE_nRF24_CONFIG_MASK_TX_DS_Msk

// Mask interrupt caused by RX_DR
#define PE_nRF24_CONFIG_MASK_RX_DR_Pos  (6U)
#define PE_nRF24_CONFIG_MASK_RX_DR_Msk  (1U << PE_nRF24_CONFIG_MASK_RX_DR_Pos)
#define PE_nRF24_CONFIG_MASK_RX_DR      PE_nRF24_CONFIG_MASK_RX_DR_Msk

#define PE_nRF24_CONFIG_RESET           PE_nRF24_CONFIG_EN_CRC

/** EN_AA bits ****************************************************************/

// Enable ‘Auto Acknowledgment’ Function
#define PE_nRF24_EN_AA_P0_Pos (0U)
#define PE_nRF24_EN_AA_P0_Msk (1U << PE_nRF24_EN_AA_P0_Pos)
#define PE_nRF24_EN_AA_P0     PE_nRF24_EN_AA_P0_Msk

#define PE_nRF24_EN_AA_P1_Pos (1U)
#define PE_nRF24_EN_AA_P1_Msk (1U << PE_nRF24_EN_AA_P1_Pos)
#define PE_nRF24_EN_AA_P1     PE_nRF24_EN_AA_P1_Msk

#define PE_nRF24_EN_AA_P2_Pos (2U)
#define PE_nRF24_EN_AA_P2_Msk (1U << PE_nRF24_EN_AA_P2_Pos)
#define PE_nRF24_EN_AA_P2     PE_nRF24_EN_AA_P2_Msk

#define PE_nRF24_EN_AA_P3_Pos (3U)
#define PE_nRF24_EN_AA_P3_Msk (1U << PE_nRF24_EN_AA_P3_Pos)
#define PE_nRF24_EN_AA_P3     PE_nRF24_EN_AA_P3_Msk

#define PE_nRF24_EN_AA_P4_Pos (4U)
#define PE_nRF24_EN_AA_P4_Msk (1U << PE_nRF24_EN_AA_P4_Pos)
#define PE_nRF24_EN_AA_P4     PE_nRF24_EN_AA_P4_Msk

#define PE_nRF24_EN_AA_P5_Pos (5U)
#define PE_nRF24_EN_AA_P5_Msk (1U << PE_nRF24_EN_AA_P5_Pos)
#define PE_nRF24_EN_AA_P5     PE_nRF24_EN_AA_P5_Msk

#define PE_nRF24_EN_AA_RESET  (PE_nRF24_EN_AA_P5|PE_nRF24_EN_AA_P4|PE_nRF24_EN_AA_P3|PE_nRF24_EN_AA_P2|PE_nRF24_EN_AA_P1|PE_nRF24_EN_AA_P0)

/** EN_RXADDR bits ************************************************************/

// Enable data pipe 0, default 1
#define PE_nRF24_EN_RXADDR_P0_Pos (0U)
#define PE_nRF24_EN_RXADDR_P0_Msk (1U << PE_nRF24_EN_RXADDR_P0_Pos)
#define PE_nRF24_EN_RXADDR_P0     PE_nRF24_EN_RXADDR_P0_Msk

// Enable data pipe 1, default 1
#define PE_nRF24_EN_RXADDR_P1_Pos (1U)
#define PE_nRF24_EN_RXADDR_P1_Msk (1U << PE_nRF24_EN_RXADDR_P1_Pos)
#define PE_nRF24_EN_RXADDR_P1     PE_nRF24_EN_RXADDR_P1_Msk

// Enable data pipe 2, default 0
#define PE_nRF24_EN_RXADDR_P2_Pos (2U)
#define PE_nRF24_EN_RXADDR_P2_Msk (1U << PE_nRF24_EN_RXADDR_P2_Pos)
#define PE_nRF24_EN_RXADDR_P2     PE_nRF24_EN_RXADDR_P2_Msk

// Enable data pipe 3, default 0
#define PE_nRF24_EN_RXADDR_P3_Pos (3U)
#define PE_nRF24_EN_RXADDR_P3_Msk (1U << PE_nRF24_EN_RXADDR_P3_Pos)
#define PE_nRF24_EN_RXADDR_P3     PE_nRF24_EN_RXADDR_P3_Msk

// Enable data pipe 4, default 0
#define PE_nRF24_EN_RXADDR_P4_Pos (4U)
#define PE_nRF24_EN_RXADDR_P4_Msk (1U << PE_nRF24_EN_RXADDR_P4_Pos)
#define PE_nRF24_EN_RXADDR_P4     PE_nRF24_EN_RXADDR_P4_Msk

// Enable data pipe 5, default 0
#define PE_nRF24_EN_RXADDR_P5_Pos (5U)
#define PE_nRF24_EN_RXADDR_P5_Msk (1U << PE_nRF24_EN_RXADDR_P5_Pos)
#define PE_nRF24_EN_RXADDR_P5     PE_nRF24_EN_RXADDR_P5_Msk

#define PE_nRF24_EN_RXADDR_ALL    (PE_nRF24_EN_RXADDR_P5|PE_nRF24_EN_RXADDR_P4|PE_nRF24_EN_RXADDR_P3|PE_nRF24_EN_RXADDR_P2|PE_nRF24_EN_RXADDR_P1|PE_nRF24_EN_RXADDR_P0)
#define PE_nRF24_EN_RXADDR_RESET  (PE_nRF24_EN_RXADDR_P1|PE_nRF24_EN_RXADDR_P0)

/** SETUP_AW bits *************************************************************/

// RX/TX Address field width
// 0x00 == Illegal
// 0x01 == 3 bytes
// 0x10 == 4 bytes
// 0x11 == 5 bytes (default)
#define PE_nRF24_SETUP_AW_Pos (0U)
#define PE_nRF24_SETUP_AW_Msk (0x3U << PE_nRF24_SETUP_AW_Pos)
#define PE_nRF24_SETUP_AW     PE_nRF24_SETUP_AW_Msk
#define PE_nRF24_SETUP_AW_0   (0x1U << PE_nRF24_SETUP_AW_Pos)
#define PE_nRF24_SETUP_AW_1   (0x2U << PE_nRF24_SETUP_AW_Pos)

#define PE_nRF24_SETUP_AW_RESET (PE_nRF24_SETUP_AW_1|PE_nRF24_SETUP_AW_0)

/** SETUP_RETR bits ***********************************************************/

// Auto Retransmit Count, default 0b0000
#define PE_nRF24_SETUP_RETR_ARC_Pos (0U)
#define PE_nRF24_SETUP_RETR_ARC_Msk (0x0FU << PE_nRF24_SETUP_RETR_ARC_Pos)
#define PE_nRF24_SETUP_RETR_ARC     PE_nRF24_SETUP_RETR_ARC_Msk
#define PE_nRF24_SETUP_RETR_ARC_0   (0x1U << PE_nRF24_SETUP_RETR_ARC_Pos)
#define PE_nRF24_SETUP_RETR_ARC_1   (0x2U << PE_nRF24_SETUP_RETR_ARC_Pos)
#define PE_nRF24_SETUP_RETR_ARC_2   (0x4U << PE_nRF24_SETUP_RETR_ARC_Pos)
#define PE_nRF24_SETUP_RETR_ARC_3   (0x8U << PE_nRF24_SETUP_RETR_ARC_Pos)

// Auto Re-transmit Delay, default 0b0011
#define PE_nRF24_SETUP_RETR_ARD_Pos (4U)
#define PE_nRF24_SETUP_RETR_ARD_Msk (0x0FU << PE_nRF24_SETUP_RETR_ARD_Pos)
#define PE_nRF24_SETUP_RETR_ARD     PE_nRF24_SETUP_RETR_ARD_Msk
#define PE_nRF24_SETUP_RETR_ARD_0   (0x1U << PE_nRF24_SETUP_RETR_ARD_Pos)
#define PE_nRF24_SETUP_RETR_ARD_1   (0x2U << PE_nRF24_SETUP_RETR_ARD_Pos)
#define PE_nRF24_SETUP_RETR_ARD_2   (0x4U << PE_nRF24_SETUP_RETR_ARD_Pos)
#define PE_nRF24_SETUP_RETR_ARD_3   (0x8U << PE_nRF24_SETUP_RETR_ARD_Pos)

#define PE_nRF24_SETUP_RETR_RESET   (PE_nRF24_SETUP_RETR_ARD_1|PE_nRF24_SETUP_RETR_ARD_0)

/** RF_CH bits ****************************************************************/

// Sets the frequency channel, default 0b0000010
#define PE_nRF24_RF_CH_Pos   (0U)
#define PE_nRF24_RF_CH_Msk   (0x7FU << PE_nRF24_RF_CH_Pos)
#define PE_nRF24_RF_CH       PE_nRF24_RF_CH_Msk
#define PE_nRF24_RF_CH_0     (0x01U << PE_nRF24_RF_CH_Pos)
#define PE_nRF24_RF_CH_1     (0x02U << PE_nRF24_RF_CH_Pos)
#define PE_nRF24_RF_CH_2     (0x04U << PE_nRF24_RF_CH_Pos)
#define PE_nRF24_RF_CH_3     (0x08U << PE_nRF24_RF_CH_Pos)
#define PE_nRF24_RF_CH_4     (0x10U << PE_nRF24_RF_CH_Pos)
#define PE_nRF24_RF_CH_5     (0x20U << PE_nRF24_RF_CH_Pos)
#define PE_nRF24_RF_CH_6     (0x40U << PE_nRF24_RF_CH_Pos)

#define PE_nRF24_RF_CH_RESET PE_nRF24_RF_CH_1

/** RF_SETUP bits *************************************************************/

// ONLY FOR PE_nRF24L01
// Setup LNA gain
#define PE_nRF24_RF_SETUP_LNA_HCURR_Pos  (0U)
#define PE_nRF24_RF_SETUP_LNA_HCURR_Msk  (1U << PE_nRF24_RF_SETUP_LNA_HCURR_Pos)
#define PE_nRF24_RF_SETUP_LNA_HCURR      PE_nRF24_RF_SETUP_LNA_HCURR_Msk

// Set RF output power in TX mode
#define PE_nRF24_RF_SETUP_RF_PWR_Pos     (1U)
#define PE_nRF24_RF_SETUP_RF_PWR_Msk     (0x3U << PE_nRF24_RF_SETUP_RF_PWR_Pos)
#define PE_nRF24_RF_SETUP_RF_PWR         PE_nRF24_RF_SETUP_RF_PWR_Msk
#define PE_nRF24_RF_SETUP_RF_PWR_0       (0x1U << PE_nRF24_RF_SETUP_RF_PWR_Pos)
#define PE_nRF24_RF_SETUP_RF_PWR_1       (0x2U << PE_nRF24_RF_SETUP_RF_PWR_Pos)

// Select between the high speed data rates: 0x1 == 2 Mbps, 0x0 == 1 Mbps
#define PE_nRF24_RF_SETUP_RF_DR_HIGH_Pos (3U)
#define PE_nRF24_RF_SETUP_RF_DR_HIGH_Msk (1U << PE_nRF24_RF_SETUP_RF_DR_HIGH_Pos)
#define PE_nRF24_RF_SETUP_RF_DR_HIGH     PE_nRF24_RF_SETUP_RF_DR_HIGH_Msk

// Force PLL lock signal. Only used in test
#define PE_nRF24_RF_SETUP_PLL_LOCK_Pos   (4U)
#define PE_nRF24_RF_SETUP_PLL_LOCK_Msk   (1U << PE_nRF24_RF_SETUP_PLL_LOCK_Pos)
#define PE_nRF24_RF_SETUP_PLL_LOCK       PE_nRF24_RF_SETUP_PLL_LOCK_Msk

// ONLY FOR PE_nRF24L01+
// Set RF Data Rate to 250kbps
#define PE_nRF24_RF_SETUP_RF_DR_LOW_Pos  (5U)
#define PE_nRF24_RF_SETUP_RF_DR_LOW_Msk  (1U << PE_nRF24_RF_SETUP_RF_DR_LOW_Pos)
#define PE_nRF24_RF_SETUP_RF_DR_LOW      PE_nRF24_RF_SETUP_RF_DR_LOW_Msk

// ONLY FOR PE_nRF24L01+
// Enables continuous carrier transmit when high
#define PE_nRF24_RF_SETUP_CONT_WAVE_Pos  (7U)
#define PE_nRF24_RF_SETUP_CONT_WAVE_Msk  (1U << PE_nRF24_RF_SETUP_CONT_WAVE_Pos)
#define PE_nRF24_RF_SETUP_CONT_WAVE      PE_nRF24_RF_SETUP_CONT_WAVE_Msk

#define PE_nRF24_RF_SETUP_RESET          (PE_nRF24_RF_SETUP_RF_DR_HIGH|PE_nRF24_RF_SETUP_RF_PWR_1|PE_nRF24_RF_SETUP_RF_PWR_0)

/** STATUS bits ***************************************************************/

// TX FIFO full flag, readonly, default 0
#define PE_nRF24_STATUS_TX_FULL_Pos (0U)
#define PE_nRF24_STATUS_TX_FULL_Msk (1U << PE_nRF24_STATUS_TX_FULL_Pos)
#define PE_nRF24_STATUS_TX_FULL     PE_nRF24_STATUS_TX_FULL_Msk

// Data pipe number for the payload, readonly, default 0b111
#define PE_nRF24_STATUS_RX_P_NO_Pos (1U)
#define PE_nRF24_STATUS_RX_P_NO_Msk (0x7U << PE_nRF24_STATUS_RX_P_NO_Pos)
#define PE_nRF24_STATUS_RX_P_NO     PE_nRF24_STATUS_RX_P_NO_Msk

// Maximum number of TX retries interrupt, default 0
#define PE_nRF24_STATUS_MAX_RT_Pos (4U)
#define PE_nRF24_STATUS_MAX_RT_Msk (1U << PE_nRF24_STATUS_MAX_RT_Pos)
#define PE_nRF24_STATUS_MAX_RT     PE_nRF24_STATUS_MAX_RT_Msk

// Data Sent TX FIFO interrupt, default 0
#define PE_nRF24_STATUS_TX_DS_Pos (5U)
#define PE_nRF24_STATUS_TX_DS_Msk (1U << PE_nRF24_STATUS_TX_DS_Pos)
#define PE_nRF24_STATUS_TX_DS     PE_nRF24_STATUS_TX_DS_Msk

// Data Ready RX FIFO interrupt, default 0
#define PE_nRF24_STATUS_RX_DR_Pos (6U)
#define PE_nRF24_STATUS_RX_DR_Msk (1U << PE_nRF24_STATUS_RX_DR_Pos)
#define PE_nRF24_STATUS_RX_DR     PE_nRF24_STATUS_RX_DR_Msk

/** OBSERVE_TX bits ***********************************************************/

// Count resent packets, read only
#define PE_nRF24_OBSERVE_TX_ARC_CNT_Pos (0U)
#define PE_nRF24_OBSERVE_TX_ARC_CNT_Msk (0xFU << PE_nRF24_OBSERVE_TX_ARC_CNT_Pos)
#define PE_nRF24_OBSERVE_TX_ARC_CNT     PE_nRF24_OBSERVE_TX_ARC_CNT_Msk

// Count lost packets, read only
#define PE_nRF24_OBSERVE_TX_PLOS_CNT_Pos (4U)
#define PE_nRF24_OBSERVE_TX_PLOS_CNT_Msk (0xFU << PE_nRF24_OBSERVE_TX_PLOS_CNT_Pos)
#define PE_nRF24_OBSERVE_TX_PLOS_CNT     PE_nRF24_OBSERVE_TX_PLOS_CNT_Msk

/** FIFO_STATUS bits **********************************************************/

// RX FIFO empty flag, default 1
#define PE_nRF24_FIFO_STATUS_RX_EMPTY_Pos (0U)
#define PE_nRF24_FIFO_STATUS_RX_EMPTY_Msk (1U << PE_nRF24_FIFO_STATUS_RX_EMPTY_Pos)
#define PE_nRF24_FIFO_STATUS_RX_EMPTY     PE_nRF24_FIFO_STATUS_RX_EMPTY_Msk

// RX FIFO full flag, default 0
#define PE_nRF24_FIFO_STATUS_RX_FULL_Pos (1U)
#define PE_nRF24_FIFO_STATUS_RX_FULL_Msk (1U << PE_nRF24_FIFO_STATUS_RX_FULL_Pos)
#define PE_nRF24_FIFO_STATUS_RX_FULL     PE_nRF24_FIFO_STATUS_RX_FULL_Msk

// TX FIFO empty flag, default 1
#define PE_nRF24_FIFO_STATUS_TX_EMPTY_Pos (4U)
#define PE_nRF24_FIFO_STATUS_TX_EMPTY_Msk (1U << PE_nRF24_FIFO_STATUS_TX_EMPTY_Pos)
#define PE_nRF24_FIFO_STATUS_TX_EMPTY     PE_nRF24_FIFO_STATUS_TX_EMPTY_Msk

// TX FIFO full flag, default 0
#define PE_nRF24_FIFO_STATUS_TX_FULL_Pos (5U)
#define PE_nRF24_FIFO_STATUS_TX_FULL_Msk (1U << PE_nRF24_FIFO_STATUS_TX_FULL_Pos)
#define PE_nRF24_FIFO_STATUS_TX_FULL     PE_nRF24_FIFO_STATUS_TX_FULL_Msk

// Pulse the rfce high for at least 10μs to Reuse last transmitted payload, default 0
#define PE_nRF24_FIFO_STATUS_TX_REUSE_Pos (6U)
#define PE_nRF24_FIFO_STATUS_TX_REUSE_Msk (1U << PE_nRF24_FIFO_STATUS_TX_REUSE_Pos)
#define PE_nRF24_FIFO_STATUS_TX_REUSE     PE_nRF24_FIFO_STATUS_TX_REUSE_Msk

/** DYNPD bits ****************************************************************/

// Enable dynamic payload length data pipe 0, default 0
#define PE_nRF24_DYNPD_DPL_P0_Pos (0U)
#define PE_nRF24_DYNPD_DPL_P0_Msk (1U << PE_nRF24_DYNPD_DPL_P0_Pos)
#define PE_nRF24_DYNPD_DPL_P0     PE_nRF24_DYNPD_DPL_P0_Msk

// Enable dynamic payload length data pipe 1, default 0
#define PE_nRF24_DYNPD_DPL_P1_Pos (1U)
#define PE_nRF24_DYNPD_DPL_P1_Msk (1U << PE_nRF24_DYNPD_DPL_P1_Pos)
#define PE_nRF24_DYNPD_DPL_P1     PE_nRF24_DYNPD_DPL_P1_Msk

// Enable dynamic payload length data pipe 2, default 0
#define PE_nRF24_DYNPD_DPL_P2_Pos (2U)
#define PE_nRF24_DYNPD_DPL_P2_Msk (1U << PE_nRF24_DYNPD_DPL_P2_Pos)
#define PE_nRF24_DYNPD_DPL_P2     PE_nRF24_DYNPD_DPL_P2_Msk

// Enable dynamic payload length data pipe 3, default 0
#define PE_nRF24_DYNPD_DPL_P3_Pos (3U)
#define PE_nRF24_DYNPD_DPL_P3_Msk (1U << PE_nRF24_DYNPD_DPL_P3_Pos)
#define PE_nRF24_DYNPD_DPL_P3     PE_nRF24_DYNPD_DPL_P3_Msk

// Enable dynamic payload length data pipe 4, default 0
#define PE_nRF24_DYNPD_DPL_P4_Pos (4U)
#define PE_nRF24_DYNPD_DPL_P4_Msk (1U << PE_nRF24_DYNPD_DPL_P4_Pos)
#define PE_nRF24_DYNPD_DPL_P4     PE_nRF24_DYNPD_DPL_P4_Msk

// Enable dynamic payload length data pipe 5, default 0
#define PE_nRF24_DYNPD_DPL_P5_Pos (5U)
#define PE_nRF24_DYNPD_DPL_P5_Msk (1U << PE_nRF24_DYNPD_DPL_P5_Pos)
#define PE_nRF24_DYNPD_DPL_P5     PE_nRF24_DYNPD_DPL_P5_Msk

/** FEATURE bits **************************************************************/

// Enables the W_TX_PAYLOAD_NOACK command, default 0
#define PE_nRF24_FEATURE_EN_DYN_ACK_Pos (0U)
#define PE_nRF24_FEATURE_EN_DYN_ACK_Msk (1U << PE_nRF24_FEATURE_EN_DYN_ACK_Pos)
#define PE_nRF24_FEATURE_EN_DYN_ACK     PE_nRF24_FEATURE_EN_DYN_ACK_Msk

// Enables Payload with ACK, default 0
#define PE_nRF24_FEATURE_EN_ACK_PAY_Pos (1U)
#define PE_nRF24_FEATURE_EN_ACK_PAY_Msk (1U << PE_nRF24_FEATURE_EN_ACK_PAY_Pos)
#define PE_nRF24_FEATURE_EN_ACK_PAY     PE_nRF24_FEATURE_EN_ACK_PAY_Msk

// Enables Dynamic Payload Length, default 0
#define PE_nRF24_FEATURE_EN_DPL_Pos (2U)
#define PE_nRF24_FEATURE_EN_DPL_Msk (1U << PE_nRF24_FEATURE_EN_DPL_Pos)
#define PE_nRF24_FEATURE_EN_DPL     PE_nRF24_FEATURE_EN_DPL_Msk

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
/* Exported types ------------------------------------------------------------*/

typedef enum {
    PE_nRF24_STATUS_OK    = 0U,
    PE_nRF24_STATUS_ERROR = 1U,
} PE_nRF24_status_t;

typedef enum {
    PE_nRF24_PIN_L = 0,
    PE_nRF24_PIN_H = 1,
} PE_nRF24_pinState_t;

typedef enum {
    PE_nRF24_POWER_STANDBY = 0U,
    PE_nRF24_POWER_UP      = 1U,
} PE_nRF24_PowerMode_t;

typedef enum {
    PE_nRF24_DIRECTION_TX = 0U,
    PE_nRF24_DIRECTION_RX = 1U,
} PE_nRF24_Direction_t;

typedef enum {
    PE_nRF24_CRC_OFF   = 0x00U,
    PE_nRF24_CRC_1byte = PE_nRF24_CONFIG_EN_CRC,
    PE_nRF24_CRC_2byte = PE_nRF24_CONFIG_EN_CRC|PE_nRF24_CONFIG_CRCO,
} PE_nRF24_CRCScheme_t;

typedef enum {
    PE_nRF24_ARD_250us  = 0x0U,
    PE_nRF24_ARD_500us  = 0x1U,
    PE_nRF24_ARD_750us  = 0x2U,
    PE_nRF24_ARD_1000us = 0x3U,
    PE_nRF24_ARD_1250us = 0x4U,
    PE_nRF24_ARD_1500us = 0x5U,
    PE_nRF24_ARD_1750us = 0x6U,
    PE_nRF24_ARD_2000us = 0x7U,
    PE_nRF24_ARD_2250us = 0x8U,
    PE_nRF24_ARD_2500us = 0x9U,
    PE_nRF24_ARD_2750us = 0xAU,
    PE_nRF24_ARD_3000us = 0xBU,
    PE_nRF24_ARD_3250us = 0xCU,
    PE_nRF24_ARD_3500us = 0xDU,
    PE_nRF24_ARD_3750us = 0xEU,
    PE_nRF24_ARD_4000us = 0xFU,
} PE_nRF24_AutoRetryDelay_t;

typedef enum {
    PE_nRF24_ADDRESS_3BIT = PE_nRF24_SETUP_AW_0,
    PE_nRF24_ADDRESS_4BIT = PE_nRF24_SETUP_AW_1,
    PE_nRF24_ADDRESS_5BIT = PE_nRF24_SETUP_AW_1|PE_nRF24_SETUP_AW_0,
} PE_nRF24_AddressWidth_t;

typedef enum {
    PE_nRF24_PIPE_RX0,
    PE_nRF24_PIPE_RX1,
    PE_nRF24_PIPE_RX2,
    PE_nRF24_PIPE_RX3,
    PE_nRF24_PIPE_RX4,
    PE_nRF24_PIPE_RX5,
    PE_nRF24_PIPE_TX,
} PE_nRF24_Pipe_t;

static const uint8_t PE_nRF24_REG_RX_PW_Pn[6] = {
    PE_nRF24_REG_RX_PW_P0,
    PE_nRF24_REG_RX_PW_P1,
    PE_nRF24_REG_RX_PW_P2,
    PE_nRF24_REG_RX_PW_P3,
    PE_nRF24_REG_RX_PW_P4,
    PE_nRF24_REG_RX_PW_P5,
};

static const uint8_t PE_nRF24_REG_mX_ADDR_Pn[7] = {
    PE_nRF24_REG_RX_ADDR_P0,
    PE_nRF24_REG_RX_ADDR_P1,
    PE_nRF24_REG_RX_ADDR_P2,
    PE_nRF24_REG_RX_ADDR_P3,
    PE_nRF24_REG_RX_ADDR_P4,
    PE_nRF24_REG_RX_ADDR_P5,
    PE_nRF24_REG_TX_ADDR,
};

typedef enum {
    PE_nRF24_TX_POWER_18dBm,
    PE_nRF24_TX_POWER_12dBm,
    PE_nRF24_TX_POWER_6dBm,
    PE_nRF24_TX_POWER_0dBm,
} PE_nRF24_TXPower_t;

typedef enum {
    PE_nRF24_DR__250kbps = PE_nRF24_RF_SETUP_RF_DR_LOW,
    PE_nRF24_DR_1000kbps = 0x00U,
    PE_nRF24_DR_2000kbps = PE_nRF24_RF_SETUP_RF_DR_HIGH,
} PE_nRF24_DataRate_t;

typedef struct {
    /**
     * Chip enable, activates RX or TX mode
     *
     * @param state
     */
    void (*setCE) (PE_nRF24_pinState_t state);

    /**
     * SPI Chip select
     *
     * @param state
     */
    void (*setCS) (PE_nRF24_pinState_t state);

    /**
     * SPI transceive data
     *
     * @param data
     */
    uint8_t (*RW) (uint8_t data);
} PE_nRF24_t;

/* Exported function prototypes --------------------------------------------- */
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

uint8_t PE_nRF24_check(PE_nRF24_t *handle);
void PE_nRF24_initialize(PE_nRF24_t *handle);
void PE_nRF24_flushRX(PE_nRF24_t *handle);
void PE_nRF24_flushTX(PE_nRF24_t *handle);
void PE_nRF24_clearIRQFlags(PE_nRF24_t *handle);

void PE_nRF24_setPowerMode(PE_nRF24_t *handle, PE_nRF24_PowerMode_t mode);

void PE_nRF24_setDirection(PE_nRF24_t *handle, PE_nRF24_Direction_t dir);

void PE_nRF24_setCRCScheme(PE_nRF24_t *handle, PE_nRF24_CRCScheme_t scheme);

/**
 * Set frequency channel, frequency will be (2400 + channel) MHz
 *
 * @param handle  connection
 * @param channel value in range 0...127
 */
void PE_nRF24_setRFChannel(PE_nRF24_t *handle, uint8_t channel);

/**
 * Set automatic retransmission parameters
 *
 * @param handle connection
 * @param delay  one of PE_nRF24_AutoRetryDelay_t
 * @param tries  in range 1...15 or 0 for disable
 */
void PE_nRF24_setAutoRetry(PE_nRF24_t *handle, PE_nRF24_AutoRetryDelay_t delay, uint8_t tries);

/**
 * Set address width
 *
 * @param handle connection
 * @param width  one of PE_nRF24_AddressWidth_t
 */
void PE_nRF24_setAddressWidth(PE_nRF24_t *handle, PE_nRF24_AddressWidth_t width);

/**
 * Get address width
 *
 * @param handle connection
 *
 * @return Address width from device
 */
uint8_t PE_nRF24_getAddressWidth(PE_nRF24_t *handle);

/**
 * Set static RX address for a specified pipe
 *
 * @param handle  connection
 * @param pipe    one of PE_nRF24_PipeN_t, each item is register address internally
 * @param address pointer to address array
 */
void PE_nRF24_setAddressValue(PE_nRF24_t *handle, PE_nRF24_Pipe_t pipe, const uint8_t *address);

/**
 * Set transmitter output RF power level
 *
 * @param handle connection
 * @param level  one of PE_nRF24_TXPower_t
 */
void PE_nRF24_setTXPower(PE_nRF24_t *handle, PE_nRF24_TXPower_t level);

/**
 * Set transmitter data rate speed
 *
 * Note: PE_nRF24_DR__250kbps can be used only for plus version
 *
 * @param handle connection
 * @param rate   one of PE_nRF24_DataRate_t
 */
void PE_nRF24_setDataRate(PE_nRF24_t *handle, PE_nRF24_DataRate_t rate);

void PE_nRF24_attachRXPipe(PE_nRF24_t *handle, PE_nRF24_Pipe_t pipe);

void PE_nRF24_detachRXPipe(PE_nRF24_t *handle, PE_nRF24_Pipe_t pipe);

/**
 * IRQ handler, must be called from interrupt if used
 *
 * @param handle
 */
void PE_nRF24_IRQHandler(PE_nRF24_t *handle);

void PE_nRF24_RXDataReadyHandler(PE_nRF24_t *handle, uint8_t pipe);

void PE_nRF24_TXDataSentHandler(PE_nRF24_t *handle);

void PE_nRF24_MaxRetriesHandler(PE_nRF24_t *handle);

#ifdef __cplusplus
}
#endif

#endif //PE_NRF24_H
