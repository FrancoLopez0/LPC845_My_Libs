/*
 * Delay.c
 *
 *  Created on: 3 jul. 2024
 *      Author: Franco A. López
 */
#include "Delay.h"
int flag_tick = 0;

void SysTick_Handler(void){
	flag_tick++;
}

void delay_ms(int final){
	while(flag_tick <= final);
	flag_tick = 0;
}
