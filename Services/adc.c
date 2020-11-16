#include "adc.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_adc.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_usart.h"
#include <math.h>
#include "EmetteurRF.h"
#define precision 0.48
#define limite_tension 2.7


static float batterie;
static float angle;
void GPIOC_init(){ //initialsation PC0 PC2
    
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
    LL_GPIO_InitTypeDef GPIO_InitStruct;
    LL_GPIO_StructInit (&GPIO_InitStruct);
    //initialisation GPIO pour la tension batterie
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.OutputType = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
    
    LL_GPIO_Init (GPIOC, &GPIO_InitStruct);
    //initialisation GPIO pour l'accelo
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.OutputType = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
    
    LL_GPIO_Init (GPIOC, &GPIO_InitStruct);
}

void init_adc(){
    
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1); //initialisation ADC
	  
		LL_ADC_InitTypeDef  ADC_InitStructure = {0}; // adcclk = 36mhz
    LL_ADC_REG_InitTypeDef reg = {0};
		
		RCC->CFGR |= (1<<15)  ;  //change la clk frequence
	  RCC->CFGR |= (0<<14)  ; 
		
		reg.ContinuousMode = LL_ADC_REG_CONV_SINGLE ;
		reg.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
		reg.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
		reg.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
		reg.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
		
		LL_ADC_REG_Init(ADC1, & reg);
		
   /* ADC1->SQR1 &= ~(1<<20);//L=0000 (1 convertion)
    ADC1->SQR1 &= ~(1<<21); //chapitre 11/12/9
    ADC1->SQR1 &= ~(1<<22);
    ADC1->SQR1 &= ~(1<<23); */
    
    ADC_InitStructure.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT ; //soit  LL_ADC_DATA_ALIGN_LEFT plus facile a lire
		ADC_InitStructure.SequencersScanMode = LL_ADC_SEQ_SCAN_DISABLE ;   
		LL_ADC_Init(ADC1,&ADC_InitStructure);
		
	  //LL_ADC_CommonInit(ADC12_COMMON,LL_ADC_MULTI_INDEPENDENT);
		
		
    LL_ADC_SetChannelSamplingTime(ADC1,LL_ADC_CHANNEL_12,LL_ADC_SAMPLINGTIME_13CYCLES_5); 
    LL_ADC_SetChannelSamplingTime(ADC1,LL_ADC_CHANNEL_10,LL_ADC_SAMPLINGTIME_13CYCLES_5);
    LL_ADC_StartCalibration(ADC1);
    
  while( LL_ADC_IsCalibrationOnGoing(ADC1)) {};
    LL_ADC_Enable(ADC1); //ADC1->CR2 |= (1<<0);// A/DconverterON/OFF
}

//PC0 - ADC_IN10
 
//PC1 - ADC_IN11
 
//PC2 - ADC_IN12
 
//PC3 - ADC_IN13
 /*
int get_batterie(){ 
    ADC1->SQR3 = 12;//ADC_IN12 sur PC2
    ADC1->CR2 |= (1<<22);//SWSTART, commence conversion         
    while( !(ADC1->SR & (1<<1)) );//tant que la convesrion n'et pas finie
    LL_ADC_DisableIT_EOS(ADC1); //terminer
    batterie=ADC1->DR; //lire le resultat
    batterie = batterie/4096*3.3; //0-3,3V sur 24 bits
    return batterie;
}
//PC0 accelo X  document schema voilier
int get_angle(){
    ADC1->SQR3 = 10;//ADC_IN10 sur PC0
    ADC1->CR2 |= (1<<22);//SWSTART, commence conversion
    while( !(ADC1->SR & (1<<1)) );//tant que la convesrion n'et pas finie
    LL_ADC_DisableIT_EOS(ADC1);
    angle=ADC1->DR; //tension en 0x
    angle = angle*3.3/4096; //tension en volte
    angle = (angle - 1.65)/ (precision); //on estime que S2,5 donc 480mv/g comme precision
    angle = asin(angle); // passage en angle teste avec tension 1,79 degre 30
    return (int) angle;
} 
LL_ADC_REG_ReadConversionData32(adc)*/
int get_angle(){
	LL_ADC_REG_SetSequencerRanks(ADC1,LL_ADC_REG_RANK_1,LL_ADC_CHANNEL_10);
	LL_ADC_REG_StartConversionSWStart(ADC1);
	angle = LL_ADC_REG_ReadConversionData12(ADC1);
	while( !(ADC1->SR & (1<<1))){};
	LL_ADC_DisableIT_EOS(ADC1);
	angle = angle*3.3/4096; //tension en volte
  angle = (angle - 1.65)/ (precision); //on estime que S2,5 donc 480mv/g comme precision
  angle = asin(angle); // passage en angle teste avec tension 1,79 degre 30
  return (int) angle;
}

int get_batterie(){
	LL_ADC_REG_SetSequencerRanks(ADC1,LL_ADC_REG_RANK_1,LL_ADC_CHANNEL_12);
	LL_ADC_REG_StartConversionSWStart(ADC1);
	batterie = LL_ADC_REG_ReadConversionData12(ADC1);
	while( !(ADC1->SR & (1<<1))){};
	LL_ADC_DisableIT_EOS(ADC1);
	batterie = batterie*3.3/4096; //tension en volte
  return (int) batterie;
}




int warning_grand_angle(){ //detecete le seuil de 40 degre, a integrer dans servomoteur et emmeteur
    float angle_detecte ;
    angle_detecte = get_angle();
    if (angle_detecte>=40){
			  send(USART2, "Warning:angle roulis trop grand" );
			  return 1;
		}
    else{
				return 0;
		}
        
} 


int warning_low_batterie(){ //detecete la batterie, a intergrer dans emetteur
    float batterie_detecte ;
    batterie_detecte = get_batterie();
    if (batterie_detecte< limite_tension){
			 send(USART2, "batterie:angle roulis trop grand" );
			 return 1;
		}
        
    else
        return 0;
}







