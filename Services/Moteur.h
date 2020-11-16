#ifndef MOTEUR_H
#define MOTEUR_H

#include "stm32f103xb.h" 

//configuration du GPIO 
void init_moteur (void);

//Variation de vitesse 
void vitesse_moteur (int vitesse);

//Sens de variation
void sens_moteur (int sens);
#endif