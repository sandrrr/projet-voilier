#include "MyTimer.h"
#include <string.h>
#include "stm32f1xx_ll_gpio.h" 
#include "stm32f1xx_ll_bus.h"            
#include "stm32f1xx_ll_usart.h" 

void conf_emetteur(USART_TypeDef *USARTx){
	
	LL_USART_InitTypeDef USARTx_init;
	USARTx_init.BaudRate=19200;
	USARTx_init.DataWidth=LL_USART_DATAWIDTH_8B;
	USARTx_init.HardwareFlowControl=LL_USART_HWCONTROL_NONE;
	USARTx_init.OverSampling=LL_USART_OVERSAMPLING_16;
	USARTx_init.Parity=LL_USART_PARITY_NONE;
	USARTx_init.TransferDirection= LL_USART_DIRECTION_TX;
	
	if (USARTx==USART1){
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
		LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_9,LL_GPIO_MODE_ALTERNATE);		
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

	}else if (USARTx==USART2){
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
		LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_2,LL_GPIO_MODE_ALTERNATE);
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

	}else if (USARTx==USART3){

		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
		LL_GPIO_SetPinMode(GPIOB,LL_GPIO_PIN_10, LL_GPIO_MODE_ALTERNATE);
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
	}
	
	//Initilisation de la structure USART
	LL_USART_Init(USARTx, &USARTx_init);
	LL_USART_Enable(USARTx);
}

void send(USART_TypeDef *USARTx, char * msg ){
		int i=0;
		while(i<strlen(msg)){
			LL_USART_TransmitData8(USARTx,msg[i]);
		while (!(LL_USART_IsActiveFlag_TC(USARTx))){}
	}
		
}
