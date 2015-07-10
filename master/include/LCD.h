/* 
 * File:   LCD.h
 * Author: eLimones <limonesu.me@gmail.com>
 *
 * Created on July 10, 2015, 1:57 AM
 */

#ifndef _LCD_H
#define	_LCD_H

#ifdef	__cplusplus
extern "C" {
#endif
	
	void lcd_init(void);
	void lcd_sendCommand(unsigned char cmd);
	void lcd_sendChar(char data);
	
#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */
