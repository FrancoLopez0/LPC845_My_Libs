/*
 * DELAY.c
 *
 *  Created on: 6 jul. 2024
 *      Author: Franco
 */
#include "DELAY.h"

uint32_t flag_tick;

void SYSTICK_INIT(void){
	SysTick_Config(SystemCoreClock/1000);
	flag_tick = 0;
}

void SysTick_Handler(void){
	flag_tick++;
}

void delay_ms(int final){
	while(flag_tick <= final);
	flag_tick = 0;
}
