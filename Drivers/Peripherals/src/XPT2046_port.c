/*
 * XPT2046_port.c
 *
 *  Created on: May 28, 2022
 *      Author: Lucas
 */

#include "XPT2046.h"

#include "stm32f4xx_hal.h"  		/* <- HAL include */

#define	XPT2046_CS_PORT		GPIOA
#define	XPT2046_CS_PIN		GPIO_PIN_4

#define XPT2046_PENIRQ_PORT	GPIOC
#define	XPT2046_PENIRQ_PIN	GPIO_PIN_6

static SPI_HandleTypeDef XPT2046_SPIHandle;

void XPT2046_SPIConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

    /* USER CODE END SPI3_MspInit 0 */
      /* Peripheral clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

    XPT2046_SPIHandle.Instance 					= SPI1;
    XPT2046_SPIHandle.Init.Mode 				= SPI_MODE_MASTER;
    XPT2046_SPIHandle.Init.Direction 			= SPI_DIRECTION_2LINES;
    XPT2046_SPIHandle.Init.DataSize 			= SPI_DATASIZE_8BIT;
    XPT2046_SPIHandle.Init.CLKPolarity 			= SPI_POLARITY_LOW;
    XPT2046_SPIHandle.Init.CLKPhase 			= SPI_PHASE_1EDGE;
    XPT2046_SPIHandle.Init.NSS 					= SPI_NSS_SOFT;
    XPT2046_SPIHandle.Init.BaudRatePrescaler	= SPI_BAUDRATEPRESCALER_8;
    XPT2046_SPIHandle.Init.FirstBit 			= SPI_FIRSTBIT_MSB;
    XPT2046_SPIHandle.Init.TIMode 				= SPI_TIMODE_DISABLED;
    XPT2046_SPIHandle.Init.CRCCalculation 		= SPI_CRCCALCULATION_DISABLED;
    XPT2046_SPIHandle.Init.CRCPolynomial 		= 10;

    if (HAL_SPI_Init(&XPT2046_SPIHandle) != HAL_OK)
    {

    }

    // Configuration of chip select (CD) pin
    GPIO_InitStruct.Pin = XPT2046_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configuration of IRQ pin
    GPIO_InitStruct.Pin = XPT2046_PENIRQ_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOA, XPT2046_CS_PIN, GPIO_PIN_SET);
}

void XPT2046_chipEnable(void)
{
	HAL_GPIO_WritePin(XPT2046_CS_PORT, XPT2046_CS_PIN, GPIO_PIN_RESET);
}

void XPT2046_chipDisable(void)
{
	HAL_GPIO_WritePin(XPT2046_CS_PORT, XPT2046_CS_PIN, GPIO_PIN_SET);
}

void XPT2046_startConversion(unsigned char data)
{

	HAL_SPI_Transmit(&XPT2046_SPIHandle, &data, 1, 10);

}

void XPT2046_getConversionData(uint8_t * dataPtr, uint16_t length)
{
	uint8_t aux[2];

	aux[0] = 0x00;
	aux[1] = 0x00;

	HAL_SPI_TransmitReceive(&XPT2046_SPIHandle, aux, dataPtr, 2, 10);
}

bool XPT2046_readPENIRQ(void)
{
	bool penirqStatus = false;

	if(HAL_GPIO_ReadPin(XPT2046_PENIRQ_PORT, XPT2046_PENIRQ_PIN) == GPIO_PIN_RESET)
		penirqStatus = true;

	return penirqStatus;
}

