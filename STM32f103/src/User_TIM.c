#include "User_TIM.h"

void User_TIM_RCC_Configuration() {
	/* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
}
void User_TIM_GPIO_Configuration() {
	/* GPIOB Configuration:TIM3 Channel 3 as alternate function push-pull */
	GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void User_TIM_Init() {
	/* System Clocks Configuration */
  User_TIM_RCC_Configuration();

  /* GPIO Configuration */
  User_TIM_GPIO_Configuration();
	
	/* Compute the prescaler value */
	/*
	- system_clk=72mhz
	- time/stick = 1/(2.250Mhz) => prescaler=31
	- Period=20ms
	- Period = (Tim_period+1)*(time/stick)
	<=> Tim_period= Period/(time/stick) - 1 = 44999
	*/
  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) (SystemCoreClock / 2250000) - 1;
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 44999;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
  /* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;

  TIM_OC3Init(TIM3, &TIM_OCInitStructure);

  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM3, ENABLE);

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);
	TIM3->CCR3=0;
}

void User_TIM_Handle (int angle) {
	if(angle>ANGLE_MAX) angle=ANGLE_MAX;
	if(angle<-ANGLE_MAX) angle=-ANGLE_MAX;
  TIM3->CCR3 = CENTER + angle*STEP;
}
