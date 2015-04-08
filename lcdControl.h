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

#define LCD_TITLE_ROW		0x00
#define LCD_DATA_ROW		0x01

#define LCD_LEVEL_LOC		0x00
#define LCD_UV_IND_LOC          0x06
#define LCD_TIME_LOC		0x09

#define LCD_HEARTBEAT_LOC       0x0E
#define LCD_HEARBEAT_ON_CHAR    ":)"
#define LCD_HEARTBEAT_OFF_CHAR	":D"

#define LCD_OFFSCREEN_LOC       0x11

//void DelayFor18TCY(void);
void DelayPORXLCD(void);
void DelayXLCD(void);


void LCDInit( void );

void LCDWrite(unsigned char LineNum, unsigned char Offset, char * buffer);
void LCDWriteLine(unsigned char LineNum, char * buffer);
void LCDTitleRow();
void LCDToggleHeartbeat(void);
void LCDUpdateLevel(char);
void LCDUpdateUVStatus(char);
void LCDUpdateTimer(unsigned int);

#endif	/* LCDCONTROL_H */

