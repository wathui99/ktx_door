/**	
 * |----------------------------------------------------------------------
 * | Copyright (C) LeTrongThuc, 2018
 * | 
 * | ADC12bits use DMA
 * | use ADC1 with address is 0x4001244C
 * | ADC chanel0 PA0
 * |----------------------------------------------------------------------
 */
#ifndef __USER_ADC_H
#define __USER_ADC_H
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
void User_ADC_DMA_RCC_Configuration(void); 
/* setting GPIO for ADC and DMA*/
void User_ADC_DMA_GPIO_Configuration(void);
/* Initilize ADC and DMA-------------------------------*/
void User_ADC_Init(void);

#endif /*__USER_ADC_H */
/*******END OF FILE****/
