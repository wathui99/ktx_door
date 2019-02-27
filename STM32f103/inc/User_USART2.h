/*
luu y:
	+ khong the ep kieu char* ve uint16_t* duoc
	+ cang khong the ep uint16_t ve char* duoc
*/

#ifndef __USER_USART2_H
#define __USER_USART2_H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include <stdio.h>

#define User_USART2                   USART2
#define User_USART2_GPIO              GPIOA
#define User_USART2_CLK               RCC_APB1Periph_USART2
#define User_USART2_GPIO_CLK          RCC_APB2Periph_GPIOA
#define User_USART2_RxPin             GPIO_Pin_3
#define User_USART2_TxPin             GPIO_Pin_2
#define User_USART2_IRQn              USART2_IRQn
#define FALSE 0
#define TRUE 1

/* Private variable -----------------------------------------------*/
//buffer + receive_flag for USART2
extern volatile uint16_t buff_pos;
extern volatile uint16_t buff[100];
extern volatile uint8_t receive_flag;

/* Private function prototypes -----------------------------------------------*/

/* RCC+GPIO+NVIC -----------------------------------------------*/
void User_USART2_RCC_Configuration(void);
void User_USART2_GPIO_Configuration(void);
void User_USART2_NVIC_Configuration(void);

/* Init function -----------------------------------------------*/
//USART2==============
void User_USART2_Init(uint32_t BaudRate);
void User_USART2_SendChar(char ch);
void User_USART2_SendSuint(uint16_t *s); //for uint16_t* data type
void User_USART2_SendSchar(char *s); //for char* data type
extern uint16_t User_USART2_ReceiveChar(void); //receive a character
extern uint16_t *User_USART2_ReceiveString(char *des, char delim, uint16_t leng); //receive data and copy to des + return
extern uint16_t *User_USART2_ReceiveString2(char delim, uint16_t leng);
//for interrupt==============
extern void USART2_IRQHandler(void); //get each byte when receive

#endif /*__USER_USART2_H */

/*******END OF FILE****/
