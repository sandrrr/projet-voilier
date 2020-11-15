#include "MyTimer.h"
#include "stm32f1xx_ll_tim.h" 
#include "stm32f1xx_ll_bus.h" // Pour l'activation des horloges

//configuration du servomoteur avec le timer et comme paramètres  le PWM (20ms)


//Configuration du Timer en mode PWM
void MyTimer_Conf_PWM(TIM_TypeDef * Timer,int Arr, int Psc){
	
		LL_TIM_OC_InitTypeDef My_LL_PWM_Tim_Init_Struct;

	// Validation horloge locale
	if (Timer==TIM1) LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
	else if (Timer==TIM2) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	else if (Timer==TIM3) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	else  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
	
	My_LL_PWM_Tim_Init_Struct.OCMode=LL_TIM_OCMODE_PWM1;
	My_LL_PWM_Tim_Init_Struct.OCState=LL_TIM_OCSTATE_DISABLE;
	My_LL_PWM_Tim_Init_Struct.OCNState=LL_TIM_OCSTATE_DISABLE;
	My_LL_PWM_Tim_Init_Struct.CompareValue=
	My_LL_PWM_Tim_Init_Struct.OCPolarity=
}



void servomoteur_init(){
	

}


//fonction permettant le bordage des voiles
