/* Includes ------------------------------------------------------------------*/

#include <PE_nRF24.h>
#include "nRF24L01.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

extern SPI_HandleTypeDef SPI_Handle;

PE_nRF24_t PE_nRF24;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static void nRF24_setCE(PE_nRF24_pinState_t state)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, (GPIO_PinState) state);
}

static void nRF24_setCS(PE_nRF24_pinState_t state)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, (GPIO_PinState) state);
}

static uint8_t nRF24_RW(uint8_t byte)
{
    uint8_t data;

    if (HAL_SPI_TransmitReceive(&SPI_Handle, &byte, &data, 1, 1000)) {
        Error_Handler(__FILE__, __LINE__);
    }

    return data;
}

void MX_nRF24_Init()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // Configure chip enable pin
    GPIO_InitStruct.Pin   = GPIO_PIN_4;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    //TODO configure interrupt pin

    // Configure callbacks
    PE_nRF24.setCE = nRF24_setCE;
    PE_nRF24.setCS = nRF24_setCS;
    PE_nRF24.RW    = nRF24_RW;
}