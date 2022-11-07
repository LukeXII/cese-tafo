/* Copyright 2020, Lucas Constantino.
 * All rights reserved.
 *
 * This file is part of Project => Tester para Amplificador Optico
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

    taskDisplay.c (Released 2022-05)

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
#include <taskDisplay.h>

/* Demo includes. */
#include "supportingFunctions.h"

/* Application includes. */
#include "API_LCD.h"
#include "XPT2046.h"

#if( TASKS_SCOPE == TASKS_OUTSIDE_MAIN)
// ------ Private constants ----------------------------------------

/* Define the strings that will be passed in as the Supporting Functions parameters.
 * These are defined const and off the stack to ensure they remain valid when the
 * tasks are executing. */
const char *pcTextForTask_LDXTOn	= "    - LDX turn On \r\n";
const char *pcTextForTask_LDXTOff	= "    - LDX turn Off\r\n";

#define	ledTickCntMAX	1000

LDX_Config_t	LDX_Config[3];

// ------ Public variables -----------------------------------------
extern SemaphoreHandle_t mutexSemaphoreHandle;
extern ledFlag_t ledBlinkingFlag;
// ------ Public functions prototypes ------------------------------

/* Task Led thread */
void vTaskDisplay( void const * argument );

// ------ Public functions -----------------------------------------

/*------------------------------------------------------------------*/
/* Task Led thread */
void vTaskDisplay( void const * argument )
{
	/*  Declare & Initialize Task Function variables for argument, led, button and task */
	LDX_Config_t * ptr = (LDX_Config_t *) argument;
	uint32_t cont = 0;
	char var[4];
	uint32_t xLastWakeTime;
	uint16_t xcoord, ycoord, pressure;
	char str[10], str2[10];

	/* The xLastWakeTime variable needs to be initialized with the current tick count. */
	xLastWakeTime = osKernelSysTick();

	char *pcTaskName = ( char * ) pcTaskGetName( NULL );

	/* Print out the name of this task. */
	vPrintTwoStrings(pcTaskName, " is running\r\n" );

    /* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{

		xcoord = XPT2046_getCoord(COORD_X);
		ycoord = XPT2046_getCoord(COORD_Y);
		pressure = XPT2046_getPressure();

		XPT2046_convertToPixelCoord(&xcoord, &ycoord);

		LCD_Fill_Screen(RED);

		sprintf(str, "%d", xcoord);
		str2[0] = 'X';
		str2[1] = ':';
		str2[2] = '\0';

		if(xcoord <= SCREEN_PIXELSIZE_X)
			LCD_Draw_Text(strcat(str2, str), 20, 20, PURPLE, 4, CYAN);
		else
			LCD_Draw_Text(strcat(str2, "-"), 20, 20, PURPLE, 4, CYAN);

		sprintf(str, "%d", ycoord);
		str2[0] = 'Y';
		str2[1] = ':';
		str2[2] = '\0';

		if(xcoord <= SCREEN_PIXELSIZE_Y)
			LCD_Draw_Text(strcat(str2, str), 20, 50, PURPLE, 4, CYAN);
		else
			LCD_Draw_Text(strcat(str2, "-"), 20, 50, PURPLE, 4, CYAN);

		sprintf(str, "%d", pressure);
		str2[0] = 'P';
		str2[1] = ':';
		str2[2] = '\0';
		LCD_Draw_Text(strcat(str2, str), 20, 80, PURPLE, 4, CYAN);

		str2[0] = 'P';
		str2[1] = 'E';
		str2[2] = 'N';
		str2[3] = ':';
		str2[4] = '\0';
		if(XPT2046_isPressed())
			LCD_Draw_Text(strcat(str2, "SI"), 20, 110, PURPLE, 4, CYAN);
		else
			LCD_Draw_Text(strcat(str2, "NO"), 20, 110, PURPLE, 4, CYAN);

		/* Check Binary Semaphore */
//		xSemaphoreTake( mutexSemaphoreHandle, portMAX_DELAY );
//		{
//        	vPrintTwoStrings( pcTaskName, pcTextForTask_BinSemTaken );
//		}
//		xSemaphoreGive(mutexSemaphoreHandle);
		/* We want this task to execute exactly every 250 milliseconds. */

//		itoa(cont, var, 10);
//		LCD_Draw_Text(var, 80, 120, BLACK, 5, RED);

//		cont++;

		osDelayUntil( &xLastWakeTime, ledTickCntMAX );

	}
}
#endif

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
