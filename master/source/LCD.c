#include "LCD.h"
#include "LCD_hw.h"

static void lcd_sendData(unsigned char data);

void lcd_sendChar(char c){
  if(c=='\f'){
		lcd_sendCommand(0x01);
	}else if(c=='\n'){
		lcd_sendCommand(0xC0);
	}else{
		lcd_sendData(c);
	}
}

void lcd_init(void){
	lcd_hw_init();
	lcd_hw_outputCtrl(PIN_RW,RW_WRITE);
	//set outputs to default state
	lcd_hw_output_LowNibble(0xF);
	lcd_hw_outputCtrl(PIN_RW,RW_WRITE);
	lcd_hw_outputCtrl(PIN_E,STATE_LOW);
	lcd_hw_outputCtrl(PIN_RS,RS_INSTRUCTION);
	lcd_hw_delay_ms(30);//wait for lcd initialization
	//
	lcd_hw_output_LowNibble(0x3);
	lcd_hw_delay_us(1);
	lcd_hw_outputCtrl(PIN_E,STATE_HIGH);
	lcd_hw_delay_us(1);
	lcd_hw_outputCtrl(PIN_E,STATE_LOW);
  lcd_hw_delay_ms(4);
	//
	for(int i=0;i<2;i++){
		lcd_hw_output_LowNibble(0x3);
		lcd_hw_delay_us(1);
		lcd_hw_outputCtrl(PIN_E,STATE_HIGH);
		lcd_hw_delay_us(1);
		lcd_hw_outputCtrl(PIN_E,STATE_LOW);
		lcd_hw_delay_ms(1);
	}
	//configure two 4 wire mode 
	lcd_hw_output_LowNibble(0x2);
	lcd_hw_delay_us(1);
	lcd_hw_outputCtrl(PIN_E,STATE_HIGH);
	lcd_hw_delay_us(1);
	lcd_hw_outputCtrl(PIN_E,STATE_LOW);
  lcd_hw_delay_ms(1);
	//	
	lcd_sendCommand(0x2C);
	lcd_sendCommand(0x0C);
	lcd_sendCommand(0x01);
	lcd_hw_delay_ms(1);
}

void lcd_sendData(unsigned char data){
	lcd_hw_outputCtrl(PIN_RW,RW_WRITE);
	lcd_hw_outputCtrl(PIN_E,STATE_LOW);
	lcd_hw_outputCtrl(PIN_RS,RS_DATA);
	lcd_hw_output_LowNibble(data>>4);
	lcd_hw_delay_us(1);
	lcd_hw_outputCtrl(PIN_E,STATE_HIGH);
	lcd_hw_delay_us(1);
	lcd_hw_outputCtrl(PIN_E,STATE_LOW);
	lcd_hw_delay_us(1);
	lcd_hw_output_LowNibble(data&0xF);
	lcd_hw_delay_us(1);
	lcd_hw_outputCtrl(PIN_E,STATE_HIGH);
	lcd_hw_delay_us(1);
	lcd_hw_outputCtrl(PIN_E,STATE_LOW);
	lcd_hw_delay_us(40);
}

void lcd_sendCommand(unsigned char command){
	lcd_hw_outputCtrl(PIN_RW,RW_WRITE);
	lcd_hw_outputCtrl(PIN_E,STATE_LOW);
	lcd_hw_outputCtrl(PIN_RS,RS_INSTRUCTION);
	lcd_hw_output_LowNibble(command>>4);
	lcd_hw_delay_us(1);
	lcd_hw_outputCtrl(PIN_E,STATE_HIGH);
	lcd_hw_delay_us(1);
	lcd_hw_outputCtrl(PIN_E,STATE_LOW);
	lcd_hw_delay_us(1);
	lcd_hw_output_LowNibble(command&0xF);
	lcd_hw_delay_us(1);
	lcd_hw_outputCtrl(PIN_E,STATE_HIGH);
	lcd_hw_delay_us(1);
	lcd_hw_outputCtrl(PIN_E,STATE_LOW);
	lcd_hw_delay_us(1600);
}
