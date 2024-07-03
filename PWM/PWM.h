/*
 * PWM.h
 *
 *  Created on: 3 jul. 2024
 *      Author: Franco A. López
 */

#ifndef PWM_H_
#define PWM_H_

#include "fsl_sctimer.h"
#include "fsl_swm_connections.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define SCTIMER_CLK_FREQ CLOCK_GetFreq(kCLOCK_Fro)
#define DEMO_FIRST_SCTIMER_OUT kSCTIMER_Out_1
#define INITIAL_DUTY_CYCLE 60U

typedef struct {
	uint8_t dutyCycle;
	uint32_t event;
	sctimer_pwm_signal_param_t param;
}t_pwm;

/*******************************************************************************
 * API
 ******************************************************************************/
void INIT_PWM(sctimer_pwm_signal_param_t* , uint32_t*);
void UPDATE_PWM(t_pwm);

#endif /* PWM_H_ */
