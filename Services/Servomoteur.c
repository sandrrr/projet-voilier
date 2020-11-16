#include "servomoteur.h"
#include "MyTimer.h"
#include "stm32f1xx_ll_tim.h" 
#include "stm32f1xx_ll_bus.h" 
#include "stm32f1xx_ll_gpio.h" 
//configuration du servomoteur avec le timer et comme param�tres  le PWM (20ms)


//Configuration du Timer en mode PWM (il doit d�border � la fr�quence PWM)
void MyTimer_Conf_PWM(TIM_TypeDef * Timer,int Channel, int Arr, int Psc){
	
	LL_TIM_OC_InitTypeDef My_LL_PWM_Tim_Init_Struct;

	// Validation horloge locale
	if (Timer==TIM1) LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
	else if (Timer==TIM2) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	else if (Timer==TIM3) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	else  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
	
	//initialisation de la structure par d�faut
	//mode 1 de la PWM - en d�compte channel active tant que le compteur (TIM1_CNT) < TIM1_CCR, l'inverse en d�compte
	My_LL_PWM_Tim_Init_Struct.OCMode=LL_TIM_OCMODE_PWM1;
	//OCState enable avec TIM_CCER_CC1E, lors de l'initialisation
	My_LL_PWM_Tim_Init_Struct.OCState=LL_TIM_OCSTATE_DISABLE;
	My_LL_PWM_Tim_Init_Struct.OCNState=LL_TIM_OCSTATE_DISABLE;
	//polarit� de la structure
	My_LL_PWM_Tim_Init_Struct.OCPolarity=LL_TIM_OCPOLARITY_HIGH;
	My_LL_PWM_Tim_Init_Struct.OCNPolarity=LL_TIM_OCPOLARITY_HIGH;
	
	//initialisation structure avec param�tres
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
	
	//on souhaite une p�riode de 20ms du PWM
	int Arr = 7200;
	int Psc = 199;
	
	//Initialisation TIM1 avec la channel 1
	MyTimer_Conf_PWM(TIM1, 1, Arr,Psc);
	
	
	//OC1 � configurer avec le registre CCER du TIM1 - CC1E Capture/Compare 1 output enable bit 0 de CCER
	TIM1->CCER &= 0xFFFE;
	TIM1->CCER |= 0x01;
		
	//Timer1 plus complexe, un bit sup doit �tre mis � 1 pour valider la sortie 1 car TIM1 (bit MOE)
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

//angle (position) fction affine de la dur� d'impulsion
//angle 45 degr� et voiles bord�s maximum (1 ms)
//angle augmente, voiles s'ouvrent jusqu'� 90 degr� (2 ms)
/*void border_voiles(int angle_girouette){
	
	//check l'angle de la girouette, si l'angle est inf�rieur ou �gal � 45 degr� d'un c�t� ou de l'autre
	if ( (angle_girouette<=45) || (angle_girouette>=360-45)){
				TIM1->CCR1 =  360;
		
	}else{
		if (angle_girouette > 45 && angle_girouette < 180){
			TIM1->CCR1 =  ;
		}else if (angle_girouette > 180 && angle_girouette < (360-45)) {
			TIM1->CCR1 =  ;
		}
	
	}*/
	void test_servomoteur(int angle){
			if ( (angle<=45) || (angle>=360-45)){
				TIM1->CCR1 =  360;
		
	}else{
		if (angle > 45 && angle < 180){
			TIM1->CCR1 = 360 ;
		}else if (angle > 180 && angle < (360-45)) {
			TIM1->CCR1 = 45 ;
		}
		
	}
	
}
