#include <xc.h>
#include <stdio.h>
#include "LCDControl.h"

char heartBeat = 1;

void LCDInit(void) {
  char buffer1[] = "Initializing..";
  char buffer2[] = "Please Wait...";
  OpenXLCD(FOUR_BIT & LINES_5X7);

  while (BusyXLCD());

  // This is the important line that was keeping
  // the interface from workign properly. It resets the hd44780 chip.
  // For 8 bit interface, you also need to make sure T1OSCEN=0
  // if using PORTC as an output port for controlling the LCD.
  WriteCmdXLCD(0x01);

  LCDTitleRow();
  LCDToggleHeartbeat();

}

void LCDTitleRow() {
  char titleRow[] = "Level UV Time";
  LCDWriteLine(LCD_TITLE_ROW, titleRow);
  SetDDRamAddr(LCD_OFFSCREEN_LOC);
}

void LCDToggleHeartbeat() {
  char heartOn[] = LCD_HEARBEAT_ON_CHAR;
  char heartOff[] = LCD_HEARTBEAT_OFF_CHAR;
  if (heartBeat == 1) {
    LCDWrite(LCD_TITLE_ROW, LCD_HEARTBEAT_LOC, heartOn );
  } else {
     LCDWrite(LCD_TITLE_ROW, LCD_HEARTBEAT_LOC, heartOff );
  }  
  heartBeat = ~heartBeat;
}

void LCDUpdateLevel(char intensity){
  char s[4];
  sprintf(s, "%3d%%", intensity);
  LCDWrite(LCD_DATA_ROW, LCD_LEVEL_LOC, s);
}

void LCDUpdateUVStatus(char onOff){
  if (onOff) {
    LCDWrite(LCD_DATA_ROW, LCD_UV_IND_LOC, "On");
  } else {
    LCDWrite(LCD_DATA_ROW, LCD_UV_IND_LOC, "--");
  }
}

void LCDUpdateTimer(unsigned int exposureTime) {
  char s[6];
  sprintf(s, "%3d sec", exposureTime);
  LCDWrite(LCD_DATA_ROW, LCD_TIME_LOC, s);
}


void LCDWrite(unsigned char LineNum, unsigned char offset, char * buffer) {
  while (BusyXLCD());
  SetDDRamAddr(64 * LineNum + offset);
  putsXLCD(buffer);
}

void LCDWriteLine(unsigned char LineNum, char * buffer) {
  while (BusyXLCD());
  SetDDRamAddr(64 * LineNum);
  putsXLCD(buffer);
}

