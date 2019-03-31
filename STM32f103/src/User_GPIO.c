#include "User_GPIO.h"

void User_GPIO_Init() {
	/*--------------------RCC-------------------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/*--------------------GPIOA-----------------*/
	/*
	//LED
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	//BUZZ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	//Hall sensor
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	*/
	/*--------------------GPIOC-----------------*/
	//button
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 |GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	/*--------------------GPIOB-----------------*/
	//chot cua
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void SC_bit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t Status) {
	if(Status) {
		GPIOx->BSRR|=(GPIO_Pin);
	}
	else {
		GPIOx->BRR|=(GPIO_Pin);
	}
}


uint8_t Read_status(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	return GPIO_ReadInputDataBit(GPIOx,GPIO_Pin);
}
