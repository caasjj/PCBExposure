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

#ifdef  BIT8
#define BUS_WIDTH               EIGHT_BIT
#else
#define BUS_WIDTH               FOUR_BIT
#endif
#define LCD_TITLE_ROW		0x00
#define LCD_DATA_ROW		0x01

#define LCD_LEVEL_LOC		0x00
#define LCD_UV_IND_LOC          0x06
#define LCD_TIME_LOC		0x09

#define LCD_HEARTBEAT_MODE_IDLE    0
#define LCD_HEARTBEAT_MODE_ACTIVE  1

#define LCD_HEARTBEAT_LOC       0x0E
#define LCD_HEARBEAT1_ON_CHAR   ":)"
#define LCD_HEARTBEAT1_OFF_CHAR	":D"
#define LCD_HEARBEAT2_ON_CHAR   ":o"
#define LCD_HEARTBEAT2_OFF_CHAR	":O"

#define LCD_OFFSCREEN_LOC       0x11

//void DelayFor18TCY(void);
void DelayPORXLCD(void);
void DelayXLCD(void);

void LCDInit( void );

void LCDSetHeartbeatMode(char);
void LCDToggleHeartbeat();
void LCDUpdateLevel(signed char);
void LCDUpdateUVStatus(char);
void LCDUpdateTimer(unsigned int);
void LCDBacklightOn(void);
void LCDBacklightOff(void);

void _LCDWrite(unsigned char LineNum, unsigned char Offset, char * buffer);
void _LCDWriteLine(unsigned char LineNum, char * buffer);
void _LCDTitleRow();

#endif	/* LCDCONTROL_H */

