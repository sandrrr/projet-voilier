#include "stm32f1xx_ll_gpio.h" 
#include "MyTimer.h"
#include "Moteur.h"

//configuration du GPIO 
void init_moteur (void){
	//configuration clock de gpio
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	//initialisation pwm pin 1 en output push-pull pour la variation
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_1,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_1, LL_GPIO_OUTPUT_PUSHPULL);
	//initialisation pin 2 en output push-pull pour le sens
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_2,LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_2, LL_GPIO_OUTPUT_PUSHPULL);
	
}

//Variation de vitesse 
void vitesse_moteur (int vitesse){
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