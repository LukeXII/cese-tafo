/* Copyright 2020, Juan Manuel Cruz.
 * All rights reserved.
 *
 * This file is part of Project => freertos_book_Example6_6
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */


/*--------------------------------------------------------------------*-

    taskButton.c (Released 2022-05)

--------------------------------------------------------------------

    task file for FreeRTOS - Event Driven System (EDS) - Project for
    STM32F429ZI_NUCLEO_144.

    See readme.txt for project information.

-*--------------------------------------------------------------------*/


// ------ Includes -------------------------------------------------
/* Project includes. */
#include "main.h"
#include "cmsis_os.h"

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <taskApp.h>
//#include <taskDisplay.h>

/* Demo includes. */
#include "supportingFunctions.h"

/* Application includes. */
#include "FSM.h"
#include "API_ADC.h"

#if( TASKS_SCOPE == TASKS_OUTSIDE_MAIN)
// ------ Private constants ----------------------------------------

/* Define the strings that will be passed in as the Supporting Functions parameters.
 * These are defined const and off the stack to ensure they remain valid when the
 * tasks are executing. */
DisplayData_t DisplayData;
const char *pcTextForTask_BinSemGiven	= " - Binary Semaphore was given\r\n";

#define		buttonTickCntMAX			1000

// ------ Public variables -----------------------------------------
extern SemaphoreHandle_t mutexSemaphoreHandle;
// ------ Public functions prototypes ------------------------------

/* Task Button thread */
void vTaskApp( void const * argument );
static FSMEvent_t eventGenerator(void);

// ------ Public functions -----------------------------------------

/*------------------------------------------------------------------*/
/* Task Button thread */
void vTaskApp( void const * argument )
{
	DisplayData_t * ptrDisplayData = (DisplayData_t *) argument;
	FSMEvent_t newEvent;
	char *pcTaskName = (char *)pcTaskGetName( NULL );

	char dummy[8];

	FSM_Init();

	/* Print out the name of this task. */
	vPrintTwoStrings(pcTaskName, "- is running\r\n" );

	ADC_DMA_StartConversion();

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{

		newEvent = eventGenerator();
		FSM_Update(newEvent);

//		if( HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin) == GPIO_PIN_SET )
//		{
		xSemaphoreTake( mutexSemaphoreHandle, portMAX_DELAY );
		{
			ptrDisplayData->opticalInputPower = ADC_getChannelVoltage(1);
			ptrDisplayData->edfaVoltage = ADC_getChannelVoltage(2);
			ptrDisplayData->opticalOutputPower = ADC_getChannelVoltage(3);
			ptrDisplayData->edfaCurrent = ADC_getChannelVoltage(4);
		}
		xSemaphoreGive( mutexSemaphoreHandle );
//		}

//		vPrintTwoStrings(itoa((uint16_t)ADC_getChannelVoltage(1), dummy, 10), " PROMEDIO\r\n" );



		osDelay( buttonTickCntMAX );
	}
}

FSMEvent_t eventGenerator(void)
{
	FSMEvent_t newEvent = NO_EVENT;


	return newEvent;
}

#endif

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
