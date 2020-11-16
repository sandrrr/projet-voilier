#include "stm32f1xx_ll_gpio.h" 
#include "MyTimer.h"
#include "Moteur.h"
#include "servomoteur.h"

//configuration du GPIO 
//PWM sur PA1 CH2 -> TIM2
//sens PA2
void init_moteur (void){
	//configuration clock de gpio
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	//initialisation pwm pin 1 en output push-pull pour la variation
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_1,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_1, LL_GPIO_OUTPUT_PUSHPULL);
	
	//conf PWM
	MyTimer_Conf_PWM(TIM2,2,7200, 199);
	
	//initialisation pin 2 en output push-pull pour le sens
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_2,LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_2, LL_GPIO_OUTPUT_PUSHPULL);
	
}

//Variation de vitesse 
void vitesse_moteur (int vitesse){
	//TIM2 car PA1 CH2 et ccr2 facteur de résolution de la pwm
	TIM2->CCR2 = vitesse;
}

//Sens de variation
void sens_moteur (int sens){
	//S1 en haut : le courant (provenant du +12V) circule dans le moteur A vers B 
	//S1 en bas : le courant circule dans le moteur de B vers A
	if (sens==0){
		LL_GPIO_SetOutputPin (GPIOA,LL_GPIO_PIN_2);
	}
	else {
		LL_GPIO_ResetOutputPin (GPIOA,LL_GPIO_PIN_2);
	}
}