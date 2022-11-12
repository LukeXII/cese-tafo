/*
 * API_ADC.c
 *
 *  Created on: Nov 7, 2022
 *      Author: Lucas
 */

#include "API_ADC.h"

#define 	NUMBER_ADC_CHANNELS 		4
#define 	NUMBER_SAMPLES_PER_CHANNEL 	10
#define		ADC_VOLTAGE_RES				3.3/4095

static ADC_HandleTypeDef hadc3;
static uint16_t adc_dma_buffer[NUMBER_ADC_CHANNELS * NUMBER_SAMPLES_PER_CHANNEL] = {0};

/**
  * @brief ADC3 Initialization Function
  * @param None
  * @retval None
  */
void ADC_Init(void)
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.ScanConvMode = ENABLE;
  hadc3.Init.ContinuousConvMode = ENABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 4;
  hadc3.Init.DMAContinuousRequests = ENABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
//    ADC_Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
//	  ADC_Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
//	  ADC_Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
//	  ADC_Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_15;
  sConfig.Rank = 4;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
//	  ADC_Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
void DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_DisableIRQ(DMA2_Stream0_IRQn);

}

void ADC_DMA_StartConversion(void)
{
	HAL_ADC_Start_DMA(&hadc3, (uint32_t *)adc_dma_buffer, NUMBER_ADC_CHANNELS * NUMBER_SAMPLES_PER_CHANNEL);
}

uint16_t DEBUG_getValue(uint8_t index)
{
	return adc_dma_buffer[index];
}

float ADC_getChannelVoltage(uint8_t channelRank)
{
	uint32_t acc_value = 0;
	uint8_t i;

	if(channelRank <= NUMBER_ADC_CHANNELS)
	{
		for(i = 0; i<NUMBER_SAMPLES_PER_CHANNEL; i++)
			acc_value += adc_dma_buffer[channelRank - 1 + i*NUMBER_ADC_CHANNELS];
	}
	else
		return -1;

	return acc_value*ADC_VOLTAGE_RES/NUMBER_SAMPLES_PER_CHANNEL;
}

