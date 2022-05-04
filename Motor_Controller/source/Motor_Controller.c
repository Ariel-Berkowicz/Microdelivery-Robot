/*
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    Motor_Controller.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "fsl_ftm.h"
#include "arm_math.h"
/* The Flextimer instance/channel used for board */
#define PWM_BASEADDR FTM0
#define PWMA_CHANNEL  0U	//Pin 36 on Robot | J1 Pin 5 on Board
#define PWMB_CHANNEL 1U		//Pin 33 on Robot | J1 Pin 14 on Board

/* Get source clock for FTM driver */
#define FTM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)
#ifndef FTM_PWM_ON_LEVEL
#define FTM_PWM_ON_LEVEL kFTM_HighTrue
#endif
#ifndef DEMO_PWM_FREQUENCY
#define DEMO_PWM_FREQUENCY (24000U)
#endif
#define A1_PIN 1U	//Pin 40 on Robot | J1 Pin 8 on Frdm Board
#define A2_PIN 9U	//Pin 38 on Robot | J1 Pin 6 on Frdm Board
#define B1_PIN 18U	//Pin 37 on Robot | J1 Pin 1 on Frdm Board
#define B2_PIN 19U	//Pin 35 on Robot | J1 Pin 3 on Frdm Board
#define PID_PARAM_KP 100
#define PID_PARAM_KD 0.025
#define PID_PARAM_KI 20
/* TODO: insert other definitions and declarations here. */
int forward(ftm_chnl_pwm_signal_param_t ftmParam[2]);

void delay(void) {
	volatile uint32_t i = 0U;
	for (i = 0U; i < 800000U; ++i) {
		__asm("NOP");
		/* delay */
	}
}

/*
 * @brief   Application entry point.
 */
int main(void) {

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
#endif
		ftm_config_t ftmInfo;
		ftm_chnl_pwm_signal_param_t ftmParam[2];
		/* Fill in the FTM config struct with the default settings */
		FTM_GetDefaultConfig(&ftmInfo);
		/* Calculate the clock division based on the PWM frequency to be obtained */
		ftmInfo.prescale = FTM_CalculateCounterClkDiv(PWM_BASEADDR,
				DEMO_PWM_FREQUENCY, FTM_SOURCE_CLOCK);
		/* Initialize FTM module */
		FTM_Init(PWM_BASEADDR, &ftmInfo);

		/* Configure ftm params with frequency 24kHZ */
		ftmParam[0].chnlNumber = (ftm_chnl_t) PWMA_CHANNEL;
		ftmParam[0].level = FTM_PWM_ON_LEVEL;
		ftmParam[0].dutyCyclePercent = 0U;
		ftmParam[0].firstEdgeDelayPercent = 0U;
		ftmParam[0].enableComplementary = false;
		ftmParam[0].enableDeadtime = false;

		ftmParam[1].chnlNumber = (ftm_chnl_t) PWMB_CHANNEL;
		ftmParam[1].level = FTM_PWM_ON_LEVEL;
		ftmParam[1].dutyCyclePercent = 0U;
		ftmParam[1].firstEdgeDelayPercent = 0U;
		ftmParam[1].enableComplementary = false;
		ftmParam[1].enableDeadtime = false;
		/* Init GPIO pins*/
		gpio_pin_config_t Motor_config = { kGPIO_DigitalOutput, 0, };

		GPIO_PinInit(GPIOA, A1_PIN, &Motor_config);
		GPIO_PinInit(GPIOB, A2_PIN, &Motor_config);
		GPIO_PinInit(GPIOB, B1_PIN, &Motor_config);
		GPIO_PinInit(GPIOB, B2_PIN, &Motor_config);
		/* GPIO Init End*/
	while (1) {

		forward(ftmParam);

	}

}

int forward(ftm_chnl_pwm_signal_param_t ftmParam[2]) {

	/*Writing to GPIO Pins*/
	GPIO_PinWrite(GPIOA, A1_PIN, 1);
	GPIO_PinWrite(GPIOA, A2_PIN, 0);
	GPIO_PinWrite(GPIOA, B1_PIN, 0);
	GPIO_PinWrite(GPIOA, B2_PIN, 1);
	/*End Writing to GPIO Pins*/
	if (kStatus_Success
			!= FTM_SetupPwm(PWM_BASEADDR, ftmParam, 2U, kFTM_EdgeAlignedPwm,
					DEMO_PWM_FREQUENCY, FTM_SOURCE_CLOCK)) {
		PRINTF(
				"\r\nSetup PWM fail, please check the configuration parameters!\r\n");
		return -1;
	}
	FTM_StartTimer(PWM_BASEADDR, kFTM_SystemClock);
	if ((kStatus_Success
			!= FTM_UpdatePwmDutycycle(PWM_BASEADDR, (ftm_chnl_t) PWMA_CHANNEL,
					kFTM_EdgeAlignedPwm, 20U))
			|| (kStatus_Success
					!= FTM_UpdatePwmDutycycle(PWM_BASEADDR,
							(ftm_chnl_t) PWMB_CHANNEL, kFTM_EdgeAlignedPwm, 20U))) {
		PRINTF(
				"Update duty cycle fail, the target duty cycle may out of range!\r\n");
	}
	return 0;
}

int backward(ftm_chnl_pwm_signal_param_t ftmParam[2]) {

	/*Writing to GPIO Pins*/
	GPIO_PinWrite(GPIOA, A1_PIN, 0);
	GPIO_PinWrite(GPIOA, A2_PIN, 1);
	GPIO_PinWrite(GPIOA, B1_PIN, 1);
	GPIO_PinWrite(GPIOA, B2_PIN, 0);
	/*End Writing to GPIO Pins*/
	if (kStatus_Success
			!= FTM_SetupPwm(PWM_BASEADDR, ftmParam, 2U, kFTM_EdgeAlignedPwm,
					DEMO_PWM_FREQUENCY, FTM_SOURCE_CLOCK)) {
		PRINTF(
				"\r\nSetup PWM fail, please check the configuration parameters!\r\n");
		return -1;
	}
	FTM_StartTimer(PWM_BASEADDR, kFTM_SystemClock);
	if ((kStatus_Success
			!= FTM_UpdatePwmDutycycle(PWM_BASEADDR, (ftm_chnl_t) PWMA_CHANNEL,
					kFTM_EdgeAlignedPwm, 20U))
			|| (kStatus_Success
					!= FTM_UpdatePwmDutycycle(PWM_BASEADDR,
							(ftm_chnl_t) PWMB_CHANNEL, kFTM_EdgeAlignedPwm, 20U))) {
		PRINTF(
				"Update duty cycle fail, the target duty cycle may out of range!\r\n");
	}
	return 0;
}

int turnLeft(ftm_chnl_pwm_signal_param_t ftmParam[2]) {

	/*Writing to GPIO Pins*/
	GPIO_PinWrite(GPIOA, A1_PIN, 1);
	GPIO_PinWrite(GPIOA, A2_PIN, 0);
	GPIO_PinWrite(GPIOA, B1_PIN, 0);
	GPIO_PinWrite(GPIOA, B2_PIN, 1);
	/*End Writing to GPIO Pins*/
	if (kStatus_Success
			!= FTM_SetupPwm(PWM_BASEADDR, ftmParam, 2U, kFTM_EdgeAlignedPwm,
					DEMO_PWM_FREQUENCY, FTM_SOURCE_CLOCK)) {
		PRINTF(
				"\r\nSetup PWM fail, please check the configuration parameters!\r\n");
		return -1;
	}
	FTM_StartTimer(PWM_BASEADDR, kFTM_SystemClock);
	if ((kStatus_Success
			!= FTM_UpdatePwmDutycycle(PWM_BASEADDR, (ftm_chnl_t) PWMA_CHANNEL,
					kFTM_EdgeAlignedPwm, 20U))
			|| (kStatus_Success
					!= FTM_UpdatePwmDutycycle(PWM_BASEADDR,
							(ftm_chnl_t) PWMB_CHANNEL, kFTM_EdgeAlignedPwm, 40U))) {
		PRINTF(
				"Update duty cycle fail, the target duty cycle may out of range!\r\n");
	}
	return 0;
}

int turnRight(ftm_chnl_pwm_signal_param_t ftmParam[2]) {

	/*Writing to GPIO Pins*/
	GPIO_PinWrite(GPIOA, A1_PIN, 1);
	GPIO_PinWrite(GPIOA, A2_PIN, 0);
	GPIO_PinWrite(GPIOA, B1_PIN, 0);
	GPIO_PinWrite(GPIOA, B2_PIN, 1);
	/*End Writing to GPIO Pins*/
	if (kStatus_Success
			!= FTM_SetupPwm(PWM_BASEADDR, ftmParam, 2U, kFTM_EdgeAlignedPwm,
					DEMO_PWM_FREQUENCY, FTM_SOURCE_CLOCK)) {
		PRINTF(
				"\r\nSetup PWM fail, please check the configuration parameters!\r\n");
		return -1;
	}
	FTM_StartTimer(PWM_BASEADDR, kFTM_SystemClock);
	if ((kStatus_Success
			!= FTM_UpdatePwmDutycycle(PWM_BASEADDR, (ftm_chnl_t) PWMA_CHANNEL,
					kFTM_EdgeAlignedPwm, 40U))
			|| (kStatus_Success
					!= FTM_UpdatePwmDutycycle(PWM_BASEADDR,
							(ftm_chnl_t) PWMB_CHANNEL, kFTM_EdgeAlignedPwm, 20U))) {
		PRINTF(
				"Update duty cycle fail, the target duty cycle may out of range!\r\n");
	}
	return 0;
}

/*void PID_Controller(ftm_chnl_pwm_signal_param_t ftmParam[2],float32_t x,float32_t y,float32_t des_angle){
	arm_pid_instance_f32 PID;
	float angle_error;
	float z;
	z = sqrt(x*x + y*y);
	des_angle = 0;


	PID.Kp = PID_PARAM_KP;
	PID.Kd = PID_PARAM_KD;
	PID.Ki = PID_PARAM_KI;
	arm_pid_init_f32(&PID, 0);
	angle_error = measured_angle - des_angle;

}
*/

