#include "stm32f30x.h"                  // Device header
#include "serial_stdio.h"
#include "retarget_stm32f3.h"
#include <string.h>
/*Led PB13, Button PC13*/

void delay_ms(int delay_time);
void led_init(void);
void spi_master_init(void);
uint16_t spi_xfer(uint16_t tx_data);

Serial_t USART1_Serial={USART1_getChar,USART1_sendChar};
Serial_t USART2_Serial={USART2_getChar,USART2_sendChar};

char mybf[80];/*Input buffer*/
char wordBuffer[80];

#define DAC_CONF	(0x3<<12) 

unsigned short adc_val=0;
unsigned short receivedData=0;
int main(){
	int lineCounter=1;
	//led_init();
	USART2_init(9600);
	serial_puts(USART2_Serial,"\nSystem ready\n");
	spi_master_init();
	while(1){
		receivedData=spi_xfer(adc_val);
		serial_printf(USART2_Serial,"transmitted data= 0x%03hX\n",adc_val);
		serial_printf(USART2_Serial,"received data = 0x%04hX\n",receivedData);
		lineCounter++;
		adc_val=lineCounter;
	}
}


void led_init(void){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	GPIO_InitTypeDef myGPIO;
	GPIO_StructInit(&myGPIO);
	myGPIO.GPIO_Pin=GPIO_Pin_13;
	myGPIO.GPIO_Mode=GPIO_Mode_OUT;
	myGPIO.GPIO_OType=GPIO_OType_PP;
	myGPIO.GPIO_PuPd=GPIO_PuPd_NOPULL;
	myGPIO.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&myGPIO);
	GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
}

void delay_ms(int delay_time){
	for(int i=0; i<delay_time; i++);
}

/* SPI1
 * PB12=NSS PB13=SCK PB14=MISO PB15=MOSI*/
void spi_master_init(void){
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 ,ENABLE);
	
	GPIO_InitTypeDef myGPIO;
	GPIO_StructInit(&myGPIO);
	myGPIO.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	myGPIO.GPIO_Mode=GPIO_Mode_AF;
	myGPIO.GPIO_OType=GPIO_OType_PP;
	myGPIO.GPIO_Speed=GPIO_Speed_10MHz;
	myGPIO.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&myGPIO);
	
	/*Remap pins to SPI2*/
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_5);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_5);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_5);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_5);
	
	SPI_InitTypeDef mySPI;
	SPI_StructInit(&mySPI);
	mySPI.SPI_Mode=SPI_Mode_Slave;
	mySPI.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_128;//16MHz/128=12.5KHz
	mySPI.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	mySPI.SPI_DataSize=SPI_DataSize_16b;
	mySPI.SPI_CPOL = SPI_CPOL_Low;//Clock steady state = low
	mySPI.SPI_CPHA = SPI_CPHA_1Edge;//CPHA=0;
	mySPI.SPI_NSS = SPI_NSS_Hard;	
	mySPI.SPI_FirstBit=SPI_FirstBit_MSB;
	SPI_Init(SPI2,&mySPI);
	SPI_SSOutputCmd(SPI2,ENABLE);//SSOE = 1
}

uint16_t spi_xfer(uint16_t tx_data){
	uint16_t rx_data;
	SPI_Cmd(SPI2,ENABLE);
	SPI_I2S_SendData16(SPI2,tx_data);
	while(!SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE));//Wait until tx buffer empty
	while(!SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE));//Wait until rx buffer full
	rx_data=SPI_I2S_ReceiveData16(SPI2);
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_BSY));//Wait until transactions completed
	SPI_Cmd(SPI2,DISABLE);//
	return rx_data;
}
