#include "girouette.h"

//Init IT giroutte
void Girouette_IT_Conf(void);

void Girouette_Conf(void) {
	// Activer les PINs utilisées par la girouette
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	// PA5 - Index
	GPIOA->CRL &= ~GPIO_CRL_MODE5;
	GPIOA->CRL &= ~GPIO_CRL_CNF5;
	// input pulldown
	GPIOA->CRL |= GPIO_CRL_CNF5_1;
	
	// PA6 - CHA
	GPIOA->CRL &= ~GPIO_CRL_MODE6;
	GPIOA->CRL &= ~GPIO_CRL_CNF6;
	// input pulldown
	GPIOA->CRL |= GPIO_CRL_CNF6_1;
	
	// PA7 - CHB
	GPIOA->CRL &= ~GPIO_CRL_MODE7;
	GPIOA->CRL &= ~GPIO_CRL_CNF7;
	// input pulldown
	GPIOA->CRL |= GPIO_CRL_CNF7_1;
	
	// Configurer timer incrémental
	// page 392 - Figure 134
	MyTimer_Conf(TIM3,360*4,1); // ARR = 360*4, PSC = 1
	TIM3->CCMR1 &= ~TIM_CCMR1_CC1S;
	TIM3->CCMR1 |= TIM_CCMR1_CC1S_0;
	TIM3->CCMR1 &= ~TIM_CCMR1_CC2S;
	TIM3->CCMR1 |= TIM_CCMR1_CC2S_0;
	TIM3->CCER &= ~TIM_CCER_CC1P;
	TIM3->CCER &= ~TIM_CCER_CC1NP;
	TIM3->CCER &= ~TIM_CCMR1_IC1F;
	TIM3->CCER &= ~TIM_CCER_CC2P;
	TIM3->CCER &= ~TIM_CCER_CC2NP;
	TIM3->CCER &= ~TIM_CCMR1_IC2F;
	TIM3->SMCR &= ~TIM_SMCR_SMS;
	TIM3->SMCR |= TIM_SMCR_SMS_0;
	TIM3->SMCR |= TIM_SMCR_SMS_1;
	TIM3->CR1 |= TIM_CR1_CEN;
	
	Girouette_IT_Conf();
}

int Girouette_Get_Angle(void) {
	return TIM3->CNT;
}

//Handle PA5 interrupt
void EXTI9_5_IRQHandler(void) {
	LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_5);
	TIM3->CNT = 0;
}

void Girouette_IT_Conf(void) {
	//PA5 - index
	LL_GPIO_AF_SetEXTISource(LL_GPIO_AF_EXTI_PORTA, LL_GPIO_AF_EXTI_LINE5);
	LL_EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_5;
	EXTI_InitStruct.LineCommand = ENABLE;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	LL_EXTI_Init(&EXTI_InitStruct);
	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_5);
	NVIC_SetPriority(EXTI9_5_IRQn, 0);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
}
