/* 
 * File:   lcdControl.h
 * Author: developer
 *
 * Created on April 2, 2015, 12:22 PM
 */

#ifndef LCDCONTROL_H
#define	LCDCONTROL_H
#undef _PLIB
#include "xlcd.h"
#include "delays.h"


#define LCD_LED_OFF_VAL		0
#define	LCD_LED_LOW_VAL		64
#define LCD_LED_MED_VAL		128
#define LCD_LED_MAX_VAL		255
#define LCD_LINE1_VAL		0x00
#define LCD_LINE2_VAL		0x01

#define LCD_TEMP_LOC		0x06
#define LCD_SETPT_LOC		0x46
#define LCD_STATE_LOC		0x0E
#define LCD_PWM_LOC	    	0x4F
#define LCD_MODE_LOC		0x4E
#define LCD_OFFSCREEN_LOC	0x50

#define LCD_DEGREE_CHAR  	0xDF
#define LCD_FAHRENHEIT_CHAR	0x46
#define LCD_PWM_ON_CHAR		0x2A
#define LCD_PWM_OFF_CHAR	0x20

//void DelayFor18TCY(void);
void DelayPORXLCD(void);
void DelayXLCD(void);

void LCDInit( void );
void LCDWelcomeMessage(void);
void LCDWrite(unsigned char LineNum, char * buffer);
void LCDSetContrastLevel( unsigned char LedLevel);
void LCDRunningMessage(void);

#endif	/* LCDCONTROL_H */

