#ifndef __USER_ADC_DMA_H
#define __USER_ADC_DMA_H

/*this program use for stm32f103c8t6*/
/*
*	- Use DMA channel 1 and 2
* - ADC pinout
* ADC12_IN0 - PA0
* ADC12_IN1 - PA1
* ADC12_IN9 - PB1
*/

 /* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
 
 /* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADC1_DR_Address    ((uint32_t)0x4001244C)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static ADC_InitTypeDef ADC_InitStructure;
static DMA_InitTypeDef DMA_InitStructure;
extern __IO uint16_t ADCConvertedValue;

/* Private function prototypes -----------------------------------------------*/
/* setting RCC for ADC and DMA*/
extern void User_ADC_DMA_RCC_Configuration(void); 
/* setting GPIO for ADC and DMA*/
extern void User_ADC_DMA_GPIO_Configuration(void);
/* Initilize ADC and DMA-------------------------------*/
extern void User_ADC_DMA_Init(void);

#endif /*__USER_ADC_DMA_H */
/*******END OF FILE****/
