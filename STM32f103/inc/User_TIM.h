/*
- Use Tim3/3 - (PB_0)
- PWM output Mode
- Period = 20ms
- system_clk=72mhz
*/

#ifndef __USER_TIM_H
#define __USER_TIM_H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

#define STEP 16 //step will be added 90(angle)/1500(2ms)=16
#define ANGLE_MAX 90
#define CENTER 3500 //max 5000, min 2000 => center =3500
/* Private variable -----------------------------------------------*/
static TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
static TIM_OCInitTypeDef  TIM_OCInitStructure;
static uint16_t PrescalerValue = 0;

/* Private function prototypes -----------------------------------------------*/
void User_TIM_RCC_Configuration(void);
void User_TIM_GPIO_Configuration(void);
void User_TIM_Init(void);

/* User's function prototype*/
void User_TIM_Handle (int angle);

#endif /*__USER_TIM_H */
/*******END OF FILE****/
