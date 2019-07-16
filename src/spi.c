/* Includes ------------------------------------------------------------------*/

#include "spi.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void MX_SPI_Init(SPI_TypeDef *spi, SPI_HandleTypeDef *handle)
{
    handle->Instance               = spi;
    handle->Init.Mode              = SPI_MODE_MASTER;
    handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;//TODO check
    handle->Init.Direction         = SPI_DIRECTION_2LINES;
    handle->Init.CLKPolarity       = SPI_POLARITY_LOW;
    handle->Init.CLKPhase          = SPI_PHASE_1EDGE;
    handle->Init.CRCPolynomial     = 7;
    handle->Init.DataSize          = SPI_DATASIZE_8BIT;
    handle->Init.FirstBit          = SPI_FIRSTBIT_MSB;
    handle->Init.NSS               = SPI_NSS_SOFT;

    if (HAL_SPI_Init(handle) != HAL_OK) {
        Error_Handler(__FILE__, __LINE__);
    }
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *spi)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    if (spi->Instance == SPI1) {
        /* SPI clock enable */
        __HAL_RCC_SPI1_CLK_ENABLE();

        /* GPIO clock enable */
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /**
         * SPI GPIO Configuration
         * PA4 ------> SPI1_NSS
         * PA5 ------> SPI1_SCK
         * PA6 ------> SPI1_MISO
         * PA7 ------> SPI1_MOSI
         *
         * Do not initialize PA4/NSS because a software SS will be used
         */
        GPIO_InitStruct.Pin   = GPIO_PIN_5|GPIO_PIN_7;
        GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin   = GPIO_PIN_6;
        GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* SPI interrupt Init */
        HAL_NVIC_SetPriority(SPI1_IRQn, 1, 1);
        HAL_NVIC_EnableIRQ(SPI1_IRQn);
    }

    if (spi->Instance == SPI2) {
        /* SPI clock enable */
        __HAL_RCC_SPI1_CLK_ENABLE();

        /* GPIO clock enable */
        __HAL_RCC_GPIOB_CLK_ENABLE();

        /**
         * SPI GPIO Configuration
         * PB12 ------> SPI2_NSS
         * PB13 ------> SPI2_SCK
         * PB14 ------> SPI2_MISO
         * PB15 ------> SPI2_MOSI
         *
         * Do not initialize PB12/NSS because a software SS will be used
         */
        GPIO_InitStruct.Pin   = GPIO_PIN_13|GPIO_PIN_15;
        GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin   = GPIO_PIN_14;
        GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* SPI interrupt Init */
        HAL_NVIC_SetPriority(SPI2_IRQn, 1, 1);
        HAL_NVIC_EnableIRQ(SPI2_IRQn);
    }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *spi)
{
    if (spi->Instance == SPI1) {
        /* SPI clock disable */
        __HAL_RCC_SPI1_CLK_DISABLE();

        /**
         * SPI GPIO Configuration
         * PA4 ------> SPI1_NSS
         * PA5 ------> SPI1_SCK
         * PA6 ------> SPI1_MISO
         * PA7 ------> SPI1_MOSI
         *
         * Do not de-initialize PB12/NSS because a software SS will be used
         */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

        /* SPI interrupt Deinit */
        HAL_NVIC_DisableIRQ(SPI1_IRQn);
    }

    if (spi->Instance == SPI2) {
        /* SPI clock disable */
        __HAL_RCC_SPI2_CLK_DISABLE();

        /**
         * SPI GPIO Configuration
         * PB12 ------> SPI2_NSS
         * PB13 ------> SPI2_SCK
         * PB14 ------> SPI2_MISO
         * PB15 ------> SPI2_MOSI
         *
         * Do not de-initialize PB12/NSS because a software SS will be used
         */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);

        /* SPI interrupt Deinit */
        HAL_NVIC_DisableIRQ(SPI2_IRQn);
    }
}