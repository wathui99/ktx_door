/*
*	repair port for devices, button

button 1: GPIOC (14)
button 2: GPIOC (15)

LED open door: GPIOA (4)
LED close door: GPIOA (5)

Buzz: GPIOA (6)

switch: GPIOA (1) // cong tac hanh trinh

*/

#ifndef __USER_GPIO_H
#define __USER_GPIO_H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#define SET 1
#define CLEAR 0

#define ON 1
#define OFF 0

#define GPIO_BT GPIOC
#define BT1_Pin GPIO_Pin_14
#define BT2_Pin GPIO_Pin_15

#define LED_Open_Pin GPIO_Pin_4
#define LED_Close_Pin GPIO_Pin_5

#define BUZZ_Pin GPIO_Pin_6

#define SWITCH_Pin GPIO_Pin_9

static GPIO_InitTypeDef GPIO_InitStructure;

/*---------------------------
*	RCC
*	GPIO
*/
void User_GPIO_Init(void);

/*---------------------------
*	set or clear GPIOx's bit base on status
*/
void SC_bit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t Status);

/*---------------------------
*	Read status of Pin
*/
uint8_t Read_status(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);


#endif /*__USER_GPIO_H */
/*******END OF FILE****/
