/*
 * PWM.c
 *
 *  Created on: 3 jul. 2024
 *      Author: Franco A. López
 */

#include "PWM.h"

void INIT_PWM(sctimer_pwm_signal_param_t* pwmParam, uint32_t *event){

	sctimer_config_t sctimerInfo;

	uint32_t sctimerClock;


	sctimerClock = SCTIMER_CLK_FREQ;

	SCTIMER_GetDefaultConfig(&sctimerInfo);

	/* Initialize SCTimer module */
	SCTIMER_Init(SCT0, &sctimerInfo);

	/* Configure first PWM with frequency 24kHZ from first output */


	if (SCTIMER_SetupPwm(SCT0, pwmParam, kSCTIMER_CenterAlignedPwm, 24000U, sctimerClock, event) == kStatus_Fail)
	    {
	    	PRINTF ("Could not start SC Timer\n\r");
	    	return;
	    }

	/* Start the timer */
	SCTIMER_StartTimer(SCT0, kSCTIMER_Counter_U);

}

void UPDATE_PWM(t_pwm pwm){
	SCTIMER_UpdatePwmDutycycle(SCT0, DEMO_FIRST_SCTIMER_OUT, pwm.dutyCycle, pwm.event);
}

/*
t_pwm pwm;
pwm.dutyCycle = INITIAL_DUTY_CYCLE;
pwm.param.output = kSCTIMER_Out_1;
pwm.param.level = kSCTIMER_HighTrue;
pwm.param.dutyCyclePercent = pwm.dutyCycle;
*/
