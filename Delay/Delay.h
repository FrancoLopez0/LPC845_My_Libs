/*
 * DELAY.h
 *
 *  Created on: 6 jul. 2024
 *      Author: Franco
 */

#ifndef DELAY_H_
#define DELAY_H_
#include "LPC845.h"

extern uint32_t flag_tick;

void SysTick_Handler(void);

void delay_ms(int);

void SYSTICK_INIT(void);
#endif /* DELAY_H_ */
