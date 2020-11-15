#ifndef EMETTEURRF_H
#define EMETTEURRF_H

#include "stm32f103xb.h" 

//on configure l'uart pour l'emetteur
void conf_emetteur(USART_TypeDef *USARTx); 
//transmettre informations
void send(USART_TypeDef *USARTx, char * msg ); 

#endif