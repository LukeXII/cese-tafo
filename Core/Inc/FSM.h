/*
 * API_FSM.h
 *
 *  Created on: Apr 10, 2022
 *      Author: Lucas
 */

#ifndef API_INC_API_FSM_H_
#define API_INC_API_FSM_H_

#include <stdio.h>
//#include "API_uart.h"

typedef enum {
	UART_NEXT,
	UART_PREV,
	NO_EVENT
}FSMEvent_t;

typedef struct tablecell_t{
	struct tablecell_t * nextState;
	FSMEvent_t event;
	void (*action)(void);
}FSMTableCell_t;

void FSM_Init();
void FSM_Update(FSMEvent_t newEvent);

void actionA(void);
void actionB(void);
void actionC(void);

void no_Action(void);


#endif /* API_INC_API_FSM_H_ */
