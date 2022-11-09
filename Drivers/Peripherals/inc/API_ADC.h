/*
 * API_ADC.h
 *
 *  Created on: Nov 7, 2022
 *      Author: Lucas
 */

#ifndef PERIPHERALS_INC_API_ADC_H_
#define PERIPHERALS_INC_API_ADC_H_

#include <stdbool.h>
#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_hal_spi.h"

void ADC_Init(void);
void DMA_Init(void);
void ADC_DMA_StartConversion(void);
uint16_t DEBUG_getValue(uint8_t);
float ADC_getChannelVoltage(uint8_t channel);

#endif /* PERIPHERALS_INC_API_ADC_H_ */
