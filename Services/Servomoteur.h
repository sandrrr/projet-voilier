#include "stm32f103xb.h" 

//initialisation timer en mode PWM
void MyTimer_Conf_PWM(TIM_TypeDef * Timer,int Channel, int Arr, int Psc);
//initialisation servomoteur
void servomoteur_init();

//bordage des voiles
void border_voiles(int angle_girouette);
void test_servomoteur(int angle);
