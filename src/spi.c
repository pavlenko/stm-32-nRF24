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
    if (spi->Instance == SPI1) {
        __HAL_RCC_SPI1_CLK_ENABLE();

        //TODO GPIO
    }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *spi)
{
    if (spi->Instance == SPI1) {
        __HAL_RCC_SPI1_CLK_DISABLE();

        //TODO GPIO
    }
}