#include <xc.h>
#include <stdio.h>
#include "LCDControl.h"

char heartBeat      = 1;
char heartBeatMode  = LCD_HEARTBEAT_MODE_IDLE;
char heartOn[2][2]    = {LCD_HEARBEAT1_ON_CHAR, LCD_HEARBEAT2_ON_CHAR};
char heartOff[2][2]   = {LCD_HEARTBEAT1_OFF_CHAR, LCD_HEARTBEAT2_OFF_CHAR};

void LCDInit(void) {

  OpenXLCD(FOUR_BIT & LINES_5X7);

  while (BusyXLCD());

  // This is the important line that was keeping
  // the interface from workign properly. It resets the hd44780 chip.
  // For 8 bit interface, you also need to make sure T1OSCEN=0
  // if using PORTC as an output port for controlling the LCD.
  WriteCmdXLCD(0x01);

  _LCDTitleRow();
  LCDToggleHeartbeat();

}


void LCDToggleHeartbeat() {

  if (heartBeat == 1) {
    _LCDWrite(LCD_TITLE_ROW, LCD_HEARTBEAT_LOC, heartOn[heartBeatMode] );
  } else {
    _LCDWrite(LCD_TITLE_ROW, LCD_HEARTBEAT_LOC, heartOff[heartBeatMode] );
  }  
  heartBeat = ~heartBeat;
}

void LCDSetHeartbeatMode(char mode) {
  if (mode) {
    heartBeatMode = LCD_HEARTBEAT_MODE_ACTIVE;
  } else {
    heartBeatMode = LCD_HEARTBEAT_MODE_IDLE;
  }
}

void LCDUpdateLevel(signed char intensity){
  char s[4];
  sprintf(s, "%3d%%", intensity);
  _LCDWrite(LCD_DATA_ROW, LCD_LEVEL_LOC, s);
}

void LCDUpdateUVStatus(char onOff){
  char *sOn = "On";
  char *sOff = "--";
  if (onOff) {
    _LCDWrite(LCD_DATA_ROW, LCD_UV_IND_LOC, sOn);
  } else {
    _LCDWrite(LCD_DATA_ROW, LCD_UV_IND_LOC, sOff);
  }
}

void LCDUpdateTimer(unsigned int exposureTime) {
  char s[6];
  sprintf(s, "%3d sec", exposureTime);
  _LCDWrite(LCD_DATA_ROW, LCD_TIME_LOC, s);
}

void LCDBacklightOn() {

}

void LCDBackLightOff() {

}


void _LCDTitleRow() {
  char titleRow[] = "Level UV Time";
  _LCDWriteLine(LCD_TITLE_ROW, titleRow);
  SetDDRamAddr(LCD_OFFSCREEN_LOC);
}


void _LCDWrite(unsigned char LineNum, unsigned char offset, char * buffer) {
  while (BusyXLCD());
  SetDDRamAddr(64 * LineNum + offset);
  putsXLCD(buffer);
}

void _LCDWriteLine(unsigned char LineNum, char * buffer) {
  while (BusyXLCD());
  SetDDRamAddr(64 * LineNum);
  putsXLCD(buffer);
}

