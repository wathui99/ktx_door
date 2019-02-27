/*
-> USART3 dung cho esp
luu y:
	+ khong the ep kieu char* ve uint16_t* duoc
	+ cang khong the ep uint16_t ve char* duoc
*/

#ifndef __USER_USART3_H
#define __USER_USART3_H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include <stdio.h>

#define User_USART3                   USART3
#define User_USART3_GPIO              GPIOB
#define User_USART3_CLK               RCC_APB1Periph_USART3
#define User_USART3_GPIO_CLK          RCC_APB2Periph_GPIOB
#define User_USART3_RxPin             GPIO_Pin_11
#define User_USART3_TxPin             GPIO_Pin_10
#define FALSE 0
#define TRUE 1


/* Private function prototypes -----------------------------------------------*/

/* RCC+GPIO -----------------------------------------------*/
void User_USART3_RCC_Configuration(void);
void User_USART3_GPIO_Configuration(void);

/* Init function -----------------------------------------------*/
//USART3==============
void User_USART3_Init(uint32_t BaudRate);
void User_USART3_SendChar(char ch);
void User_USART3_SendSuint(uint16_t *s); //for uint16_t* data type
void User_USART3_SendSchar(char *s); //for char* data type
extern uint16_t User_USART3_ReceiveChar(void); //receive a character
extern uint16_t *User_USART3_ReceiveString(char *des, char delim, uint16_t leng); //receive data and copy to des + return
extern uint16_t *User_USART3_ReceiveString2(char delim, uint16_t leng);

#endif /*__USER_USART3_H */

/*******END OF FILE****/
