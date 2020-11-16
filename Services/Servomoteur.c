#include "servomoteur.h"
#include "MyTimer.h"
#include "stm32f1xx_ll_tim.h" 
#include "stm32f1xx_ll_bus.h" 
#include "stm32f1xx_ll_gpio.h" 
//configuration du servomoteur avec le timer et comme paramètres  le PWM (20ms)


//Configuration du Timer en mode PWM (il doit déborder à la fréquence PWM)
void MyTimer_Conf_PWM(TIM_TypeDef * Timer,int Channel, int Arr, int Psc){
	
	LL_TIM_OC_InitTypeDef My_LL_PWM_Tim_Init_Struct;

	// Validation horloge locale
	if (Timer==TIM1) LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
	else if (Timer==TIM2) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	else if (Timer==TIM3) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	else  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
	
	//initialisation de la structure par défaut
	//mode 1 de la PWM - en décompte channel active tant que le compteur (TIM1_CNT) < TIM1_CCR, l'inverse en décompte
	My_LL_PWM_Tim_Init_Struct.OCMode=LL_TIM_OCMODE_PWM1;
	//OCState enable avec TIM_CCER_CC1E, lors de l'initialisation
	My_LL_PWM_Tim_Init_Struct.OCState=LL_TIM_OCSTATE_DISABLE;
	My_LL_PWM_Tim_Init_Struct.OCNState=LL_TIM_OCSTATE_DISABLE;
	//polarité de la structure
	My_LL_PWM_Tim_Init_Struct.OCPolarity=LL_TIM_OCPOLARITY_HIGH;
	My_LL_PWM_Tim_Init_Struct.OCNPolarity=LL_TIM_OCPOLARITY_HIGH;
	
	//initialisation structure avec paramètres
	LL_TIM_OC_Init(Timer, Channel, &My_LL_PWM_Tim_Init_Struct);
	
	LL_TIM_CC_EnableChannel(Timer, Channel);
	
	//Arr config
	LL_TIM_SetAutoReload(Timer, Arr);
	//PSC config
	LL_TIM_SetPrescaler(Timer, Psc);

	//Activation compteur
	LL_TIM_EnableCounter(Timer);
	
}



void servomoteur_init(){
	
	//on souhaite une période de 20ms du PWM
	int Arr = 7200;
	int Psc = 199;
	
	//Initialisation TIM1 avec la channel 1
	MyTimer_Conf_PWM(TIM1, 1, Arr,Psc);
	
	
	//OC1 à configurer avec le registre CCER du TIM1 - CC1E Capture/Compare 1 output enable bit 0 de CCER
	TIM1->CCER &= 0xFFFE;
	TIM1->CCER |= 0x01;
		
	//Timer1 plus complexe, un bit sup doit être mis à 1 pour valider la sortie 1 car TIM1 (bit MOE)
	TIM1->BDTR &=0x7FFF;
	TIM1->BDTR |= 0x8000;
	
	//Configurer GPIOA8 et GPIOB8 pour servomoteur
	//activation horloge --> branchement de l'horloge sur une source, TIM1 sur APB2
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_8,LL_GPIO_MODE_ALTERNATE);


}


//fonction permettant le bordage des voiles
void border_voiles(int angle_girouette){
	
}
