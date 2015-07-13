#include "stm32f30x.h"                  // Device header

static void delay_ms(int delay_time){
	for(int i=0; i<delay_time; i++);
}


void adc_init_injected(void){
	//Confiure pins PC0[AN6], PC1[AN7] for analog input operation
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
	GPIO_InitTypeDef myGPIO;
	GPIO_StructInit(&myGPIO);
	myGPIO.GPIO_Pin=(GPIO_Pin_1|GPIO_Pin_0);
	myGPIO.GPIO_Mode=GPIO_Mode_AN;
	GPIO_Init(GPIOC,&myGPIO);
	//Configure ADC
	RCC_ADCCLKConfig(RCC_ADC12PLLCLK_OFF);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_ADC12EN,ENABLE); 
	delay_ms(0xFFF);
	ADC_CommonInitTypeDef myADC_Comm;
	ADC_CommonStructInit(&myADC_Comm);
	myADC_Comm.ADC_Clock=ADC_Clock_SynClkModeDiv1;
	ADC_CommonInit(ADC1,&myADC_Comm);
	ADC_VoltageRegulatorCmd(ADC1,ENABLE);
	delay_ms(0xFFF);
	
	/*Initial calibration*/
	ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1) != RESET);
	ADC_GetCalibrationValue(ADC1);
	
	ADC_InjectedInitTypeDef myADC;
	ADC_InjectedStructInit(&myADC);
	myADC.ADC_ExternalTrigInjecEventEdge=ADC_ExternalTrigInjecEventEdge_None;
	myADC.ADC_ExternalTrigInjecConvEvent=ADC_ExternalTrigInjecConvEvent_0;
	myADC.ADC_NbrOfInjecChannel=4;
	myADC.ADC_InjecSequence1=ADC_InjectedChannel_6;
	myADC.ADC_InjecSequence2=ADC_InjectedChannel_6;
	myADC.ADC_InjecSequence3=ADC_InjectedChannel_6;
	myADC.ADC_InjecSequence4=ADC_InjectedChannel_6;
	ADC_InjectedInit(ADC1,&myADC);
	ADC_InjectedChannelSampleTimeConfig(ADC1,ADC_InjectedChannel_6,ADC_SampleTime_601Cycles5);
	//
	/* wait for ADRDY */
	ADC_Cmd(ADC1,ENABLE);
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY));
}

unsigned int adc_injected_read(void){
	ADC_ClearFlag(ADC1,ADC_FLAG_JEOS);
		ADC_StartInjectedConversion(ADC1);
		while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_JEOS));
		delay_ms(0xFF);
		return ADC_GetInjectedConversionValue(ADC1,ADC_InjectedSequence_4);
}
