#include "User_USART3.h"

/* RCC+GPIO+NVIC -----------------------------------------------*/
void User_USART3_RCC_Configuration(void){
	/* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(User_USART3_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

  /* Enable USART3 Clock */
  RCC_APB1PeriphClockCmd(User_USART3_CLK, ENABLE); 
}


void User_USART3_GPIO_Configuration(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Configure USART3 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = User_USART3_RxPin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(User_USART3_GPIO , &GPIO_InitStructure);  
  
  /* Configure USART3 Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = User_USART3_TxPin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(User_USART3_GPIO, &GPIO_InitStructure);
}



/* Init function -----------------------------------------------*/
//USART3==============
USART_InitTypeDef USART3_InitStructure;
void User_USART3_Init(uint32_t BaudRate) {
	/* System Clocks Configuration */
	User_USART3_RCC_Configuration();
	/* Configure the GPIO ports */
	User_USART3_GPIO_Configuration();
	/*
        - BaudRate
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART3_InitStructure.USART_BaudRate = BaudRate;
  USART3_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART3_InitStructure.USART_StopBits = USART_StopBits_1;
  USART3_InitStructure.USART_Parity = USART_Parity_No;
  USART3_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART3_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* Configure USART3z */
  USART_Init(User_USART3, &USART3_InitStructure);
  
  /* Enable USART3z */
  USART_Cmd(User_USART3, ENABLE);
}



void User_USART3_SendChar(char ch) {
	USART_SendData(User_USART3,(uint16_t)ch);
	while(USART_GetFlagStatus(User_USART3, USART_FLAG_TXE) == RESET) {}
}



void User_USART3_SendSuint(uint16_t *s) {
	int i=0;
	for (i=0; s[i]; i++)
		User_USART3_SendChar(s[i]);
}



void User_USART3_SendSchar(char *s) {
	int i=0;
	for (i=0; s[i]; i++)
		User_USART3_SendChar(s[i]);
}



uint16_t User_USART3_ReceiveChar(void) {
	return USART_ReceiveData(User_USART3);
}



uint16_t *User_USART3_ReceiveString(char *des, char delim, uint16_t leng) {
	
	//reset buffer + buffer position
	uint16_t i;
	static uint16_t buff[100]={};
	uint8_t buff_pos;
	//Init value
	for (i=0; i<100; i++) {
		buff[i]='\0';
	}
	buff_pos=0;
	while (buff_pos<leng) { //until reach enough length or buff[lastest byte]==delim
		while (USART_GetFlagStatus(User_USART3, USART_FLAG_RXNE) == RESET){}
		buff[buff_pos++]=User_USART3_ReceiveChar();
		if (((char)buff[(buff_pos==0?0:buff_pos-1)])==delim) { //prevent case buff_pos==0
			buff[(buff_pos==0?0:buff_pos-1)]='\0';
			break;
		}
	};	
	//finish receive
	
	//copy to des
	for (i=0; buff[i]; i++)
		des[i]=buff[i];
	des[i]='\0';
	
	return (uint16_t *) buff;
}

extern uint16_t *User_USART3_ReceiveString2(char delim, uint16_t leng){
	//reset buffer + buffer position
	uint16_t i;
	static uint16_t buff[100]={};
	uint8_t buff_pos;
	//Init value
	for (i=0; i<100; i++) {
		buff[i]='\0';
	}
	buff_pos=0;
	while (buff_pos<leng) { //until reach enough length or buff[lastest byte]==delim
		while (USART_GetFlagStatus(User_USART3, USART_FLAG_RXNE) == RESET){}
		buff[buff_pos++]=User_USART3_ReceiveChar();
		if (((char)buff[(buff_pos==0?0:buff_pos-1)])==delim) { //prevent case buff_pos==0
			buff[(buff_pos==0?0:buff_pos-1)]='\0';
			break;
		}
	};	
	//finish receive
	
	return (uint16_t *) buff;
}
