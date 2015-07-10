/* 
 * File:   LCD_hw.h
 * Author: eLimones <limonesu.me@gmail.com>
 *
 * Created on July 10, 2015, 1:58 AM
 */

#ifndef _LCD_HW_H
#define	_LCD_HW_H

#define PIN_RS	0
#define PIN_RW  1
#define PIN_E		2
#define STATE_LOW  0
#define STATE_HIGH  1
#define RS_INSTRUCTION 0
#define RS_DATA 1
#define RW_WRITE 0
#define RW_READ 1

#ifdef	__cplusplus
extern "C" {
#endif
	
	void lcd_hw_init(void);
	void lcd_hw_outputCtrl(unsigned char ctrl_pin, unsigned char newState);
	void lcd_hw_output_LowNibble(unsigned char lowNibbleData);
	void lcd_hw_output_data(unsigned char data);
	void lcd_hw_delay_us(int delay_time);
	void lcd_hw_delay_ms(int delay_time);
	
#ifdef	__cplusplus
}
#endif

#endif	/* _LCD_HW_H */
