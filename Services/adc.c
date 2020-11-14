#include "adc.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_adc.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_usart.h"
#include <math.h>
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
    
    RCC->CFGR &= ~(1<<14);
    RCC->CFGR |= (1<<15);
    
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
  LL_ADC_InitTypeDef  ADC_InitStructure = {0};
    
    ADC1->CR2 |= (1<<20);//EXTTRIG =1 External trigger conversion mode for regular channels
    
    ADC1->CR2 |= (1<<17);//ext trigger = SWSTART bit 17-19 111
    ADC1->CR2 |= (1<<18);
    ADC1->CR2 |= (1<<19);
    
    ADC1->SQR1 &= ~(1<<20);//L =0000 (1 convertion)
    ADC1->SQR1 &= ~(1<<21); //11,12,9
    ADC1->SQR1 &= ~(1<<22);
    ADC1->SQR1 &= ~(1<<23);
    
    ADC_InitStructure.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT ; //?soit  LL_ADC_DATA_ALIGN_LEFT
  ADC_InitStructure.SequencersScanMode = LL_ADC_SEQ_SCAN_DISABLE ;   //?????
  LL_ADC_Init(ADC1,&ADC_InitStructure);
    
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

int get_batterie(){
    ADC1->SQR3 = 12;//ADC_IN12 sur PC2
    ADC1->CR2 |= (1<<22);//SWSTART, commence conversion
    while( !(ADC1->SR & (1<<1)) );//tant que la convesrion n'et pas finie
    LL_ADC_DisableIT_EOS(ADC1);
    batterie=ADC1->DR;
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
int waring_grand_angle(){ //detecete le seuil de 40 degre
    float angle_detecte ;
    angle_detecte = get_angle();
    if (angle_detecte>=40)
        return 1;
    else
        return 0;
}
int waring_low_batterie(){ //detecete la batterie
    float batterie_detecte ;
    batterie_detecte =get_batterie();
    if (batterie_detecte< limite_tension)
        return 1;
    else
        return 0;
}







