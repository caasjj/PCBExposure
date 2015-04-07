#include <xc.h>
#include <stdio.h>
#include "LCDControl.h"

void LCDInit(void) {
  char buffer1[] = "Initializing..";
  char buffer2[] = "Please Wait...";
  OpenXLCD(FOUR_BIT & LINES_5X7);

  while (BusyXLCD());

  // This is the important line that was keeping
  // the interface from workign properly.
  // For 8 bit interface, you also need to make sure T1OSCEN=0
  // if using PORTC as an output port for controlling the LCD>
  WriteCmdXLCD(0x01);

  while (BusyXLCD());
  LCDWrite(LCD_LINE1_VAL, buffer1);
  LCDWrite(LCD_LINE2_VAL, buffer2);
  while (BusyXLCD());
  SetDDRamAddr(LCD_OFFSCREEN_LOC);

}

void LCDWelcomeMessage(void) {
  char buffer1[] = "Welcome dude";
  char buffer2[] = "Hello World!";

  while (BusyXLCD());
  LCDWrite(LCD_LINE1_VAL, buffer1);
  LCDWrite(LCD_LINE2_VAL, buffer2);
  while (BusyXLCD());
  SetDDRamAddr(LCD_OFFSCREEN_LOC);

}

void LCDRunningMessage(void) {
  char buffer1[] = "Temp =          ";
  char buffer2[] = "SetPt=          ";

  while (BusyXLCD());
  SetDDRamAddr(LCD_TEMP_LOC - 6);
  putsXLCD(buffer1);
  while (BusyXLCD());
  SetDDRamAddr(LCD_SETPT_LOC - 6);
  putsXLCD(buffer2);

}

void LCDWrite(unsigned char LineNum, char * buffer) {
  while (BusyXLCD());
  SetDDRamAddr(64 * LineNum);
  putsXLCD(buffer);
}


void LCDSetContrastLevel(unsigned char LedLevel) {

  // Turn on the LED
  // Disable CCP2 (RB3) output driver - write 1 to bit Bit3 of TRISB
  TRISBbits.TRISB3 = 1;

  // Load the timer 2 period = (PR2+1) * 0.5uS * TMR2 Prescale (=4) =512uS
  PR2 = 255;

  // Set the PWM duty cycle
  CCP2M2 = 1;
  CCP2M3 = 1;
  CCPR2L = LedLevel;

  // Start Timer2
  TMR2IF = 0;
  T2CKPS0 = 1;
  T2CKPS1 = 0;
  TMR2ON = 1;

  // Wait for Timer overflow
  //while ( PIR1bits.TMR2IF == 0 );
  TRISB3 = 0;

}


