/*
 * SWM_config.c
 *
 *  Created on: 4 jul. 2024
 *      Author: Franco A. López
 */

#include "SWM_config.h"

void SWM_CONFIG(void){
	 CLOCK_EnableClock(kCLOCK_Swm);
	 SWM_SetMovablePinSelect(SWM0, kSWM_SCT_OUT1, kSWM_PortPin_P0_16);
	 //SWM_SetMovablePinSelect(SWM0, kSWM_I2C1_SCL, kSWM_PortPin_P0_26);
	 CLOCK_DisableClock(kCLOCK_Swm);
}
