/**	
 * |----------------------------------------------------------------------
 * | Copyright (C) LTT, 2018
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
 
 #include "User_ADC_DMA.h"
 
__IO uint16_t ADCConvertedValue;
 
 /* setting RCC for ADC and DMA---------------------------------*/
void User_ADC_DMA_RCC_Configuration(void) {
	/* ADCCLK = PCLK2/6 */
  RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    
  /* Enable peripheral clocks ------------------------------------------------*/
  /* Enable DMA1 clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  /* Enable ADC1 and GPIOA GPIOB clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
}

/* setting GPIO for ADC and DMA---------------------------------*/
void User_ADC_DMA_GPIO_Configuration(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Configure 
	*	PA1 - ADC channel 1
	*/
	
	/*Init PORTA*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/* Initilize ADC and DMA-------------------------------*/
void User_ADC_DMA_Init(void) {
	
	/*initilize value for adc*/
	ADCConvertedValue=0;
	
	/* System clocks configuration ---------------------------------------------*/
  User_ADC_DMA_RCC_Configuration();

  /* GPIO configuration ------------------------------------------------------*/
  User_ADC_DMA_GPIO_Configuration();

  DMA_InitStructure.DMA_BufferSize =1;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValue;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1 , ENABLE ) ;

  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);
	
	ADC_Cmd(ADC1 , ENABLE ) ;
	ADC_DMACmd(ADC1 , ENABLE ) ;
	ADC_ResetCalibration(ADC1);

	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);

	while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd ( ADC1 , ENABLE ) ;
}
