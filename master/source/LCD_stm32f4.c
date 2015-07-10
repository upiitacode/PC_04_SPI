#include "LCD_hw.h"
#include "stm32f4xx.h"                  // Device header;

/*
	Connection diagram
	LCD uC
	D0->PC4
	D1->PC5
	D2->PC6
	D3->PC7
	E->PC8
	RW->PC9
	RS->PC10
	
	All pins must have a external 10k  pull-up resistor to 5v
*/

void lcd_hw_init(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef myGPIO;
	GPIO_StructInit(&myGPIO);
	myGPIO.GPIO_Mode=GPIO_Mode_OUT;
	myGPIO.GPIO_OType=GPIO_OType_OD;
	myGPIO.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
	myGPIO.GPIO_Speed=GPIO_Speed_2MHz;
	myGPIO.GPIO_Speed=GPIO_Speed_25MHz;
	myGPIO.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&myGPIO);
}

void lcd_hw_outputCtrl(unsigned char ctrl_pin, unsigned char newState){
	if(PIN_E==ctrl_pin){
		GPIO_WriteBit(GPIOC,GPIO_Pin_8,((newState)?(Bit_SET):(Bit_RESET)));
	}else if(PIN_RW==ctrl_pin){
		GPIO_WriteBit(GPIOC,GPIO_Pin_9,((newState)?(Bit_SET):(Bit_RESET)));
	}else if(PIN_RS==ctrl_pin){
		GPIO_WriteBit(GPIOC,GPIO_Pin_10,((newState)?(Bit_SET):(Bit_RESET)));
	}
}

void lcd_hw_output_LowNibble(unsigned char lowNibbleData){
	uint16_t setBits=0;
	uint16_t resetBits=0;
	setBits=lowNibbleData&0xF;
	resetBits=(~lowNibbleData)&0xF;
	GPIO_SetBits(GPIOC,setBits<<4);
	GPIO_ResetBits(GPIOC,resetBits<<4);
}

void lcd_hw_output_data(unsigned char data){
	//not implemented
}

void lcd_hw_delay_us(int delay_time){
	for(int i=0;i<(delay_time*0xF);i++){
	}
}

void lcd_hw_delay_ms(int delay_time){
	for(int i=0;i<delay_time;i++){
		lcd_hw_delay_us(1000);
	}
}
