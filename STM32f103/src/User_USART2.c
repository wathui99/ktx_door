#include "User_USART2.h"

/* RCC+GPIO+NVIC -----------------------------------------------*/
void User_USART2_RCC_Configuration(void){
	/* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(User_USART2_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

  /* Enable USART2 Clock */
  RCC_APB1PeriphClockCmd(User_USART2_CLK, ENABLE); 
}


void User_USART2_GPIO_Configuration(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Configure USART2 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = User_USART2_RxPin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(User_USART2_GPIO , &GPIO_InitStructure);  
  
  /* Configure USART2 Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = User_USART2_TxPin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(User_USART2_GPIO, &GPIO_InitStructure);
}


void User_USART2_NVIC_Configuration(void) {
	NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the User_USART2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = User_USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}



/* Init function -----------------------------------------------*/
//USART2==============
USART_InitTypeDef USART2_InitStructure;
void User_USART2_Init(uint32_t BaudRate) {
	/* System Clocks Configuration */
	User_USART2_RCC_Configuration();
	/* Configure the GPIO ports */
	User_USART2_GPIO_Configuration();
	/* NVIC configuration */
	User_USART2_NVIC_Configuration();
	/* USARTy and USARTz configuration -------------------------------------------*/
  /* USARTy and USARTz configured as follow:
        - BaudRate
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART2_InitStructure.USART_BaudRate = BaudRate;
  USART2_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART2_InitStructure.USART_StopBits = USART_StopBits_1;
  USART2_InitStructure.USART_Parity = USART_Parity_No;
  USART2_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART2_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* Configure USART2z */
  USART_Init(User_USART2, &USART2_InitStructure);

  /* Enable the USART2z Receive Interrupt */
  USART_ITConfig(User_USART2, USART_IT_RXNE, ENABLE);
  
  /* Enable USART2z */
  USART_Cmd(User_USART2, ENABLE);
}



void User_USART2_SendChar(char ch) {
	USART_SendData(User_USART2,(uint16_t)ch);
	while(USART_GetFlagStatus(User_USART2, USART_FLAG_TXE) == RESET) {}
}



void User_USART2_SendSuint(uint16_t *s) {
	int i=0;
	for (i=0; s[i]; i++)
		User_USART2_SendChar(s[i]);
}



void User_USART2_SendSchar(char *s) {
	int i=0;
	for (i=0; s[i]; i++)
		User_USART2_SendChar(s[i]);
}



uint16_t User_USART2_ReceiveChar(void) {
	return USART_ReceiveData(User_USART2);
}



uint16_t *User_USART2_ReceiveString(char *des, char delim, uint16_t leng) {
	
	//reset buffer + buffer position
	uint16_t i;
	
	for (i=0; i<100; i++) {
		buff[i]='\0';
	}
	buff_pos=0;
	//start receive
//	User_USART2_SendSchar("\nSTART\n");
	receive_flag=TRUE;
	while (buff_pos<leng) { //until reach enough length or buff[lastest byte]==delim
		if (((char)buff[(buff_pos==0?0:buff_pos-1)])==delim) { //prevent case buff_pos==0
			buff[(buff_pos==0?0:buff_pos-1)]='\0';
			break;
		}
	};
//	User_USART2_SendSchar("DONE\n");
	
	//finish receive
	receive_flag=FALSE; //prevent receive data
	
	//copy to des
	for (i=0; buff[i]; i++)
		des[i]=buff[i];
	des[i]='\0';
	
	return (uint16_t *) buff;
}

extern uint16_t *User_USART2_ReceiveString2(char delim, uint16_t leng){
	//reset buffer + buffer position
	uint16_t i;
	
	for (i=0; i<100; i++) {
		buff[i]='\0';
	}
	buff_pos=0;
	//start receive
//	User_USART2_SendSchar("\nSTART\n");
	receive_flag=TRUE;
	while (buff_pos<leng) { //until reach enough length or buff[lastest byte]==delim
		if (((char)buff[(buff_pos==0?0:buff_pos-1)])==delim) { //prevent case buff_pos==0
			buff[(buff_pos==0?0:buff_pos-1)]='\0';
			break;
		}
	};
//	User_USART2_SendSchar("DONE\n");
	
	//finish receive
	receive_flag=FALSE; //prevent receive data
	
	return (uint16_t *) buff;
}
