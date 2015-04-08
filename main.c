//******************************************************************************
//Software License Agreement                                         
//                                                                    
//The software supplied herewith by Microchip Technology             
//Incorporated (the "Company") is intended and supplied to you, the  
//Company�s customer, for use solely and exclusively on Microchip    
//products. The software is owned by the Company and/or its supplier,
//and is protected under applicable copyright laws. All rights are   
//reserved. Any use in violation of the foregoing restrictions may   
//subject the user to criminal sanctions under applicable laws, as   
//well as to civil liability for the breach of the terms and         
//conditions of this license.                                        
//                                                                    
//THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,  
//WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED  
//TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A       
//PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,  
//IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR         
//CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.       
// *******************************************************************
// PIC18F45K20 Starter Kit Lesson 2 - Blink LED
//
// This lesson blinks LED 7 on the demo board by using a delay and 
// loop.
//
// *******************************************************************
// *    See included documentation for Lesson instructions           *
// *******************************************************************

/***********
 * At start, display shows Timer in Seconds and Intensity in mA through UV Leds
 * User selects 3 position FunctionSelectAdc for ExposureTime/OFF/IntensityValue
 * When user presses up/down buttons, Timer goes up and down in increments of 5 seconds
 * When user presses Start/Stop button, red UV LED Active led goes on and current is fed to the
 * UV LEDs. All other buttons are disabled, and current settings of Timer and LED Intensity are
 * locked in and stored in EEPROM. If user presses Start/Stop again, UV LEDs current
 * is disabled and red UV LED Active led goes off.  Timer button and LED Intensity Pot are
 * activated and whatever values are there will be used on the Start cycle and stored to EEPROM.
 */

#undef _PLIB

#include "main.h"

/** I N C L U D E S **************************************************/
#include <xc.h>
#include <stdio.h>
#include <plib/timers.h>
#include <plib/adc.h>
#include <plib/EEP.h>

/** D E C L A R A T I O N S *******************************************/
unsigned int    exposureTime_sec            = 10;
unsigned char   exposureIntensity_percent   = 20;
unsigned long   systemTime_tick             = 0L;


unsigned char   systemState                 = SYSTEM_INIT_STATE;
unsigned int    systemUvTimer_ticks         = 0;
char            systemTick                  = 0;

void            systemInit(void);
char            systemRun(void);
char            systemIdle(void);
char            systemToggle(void);
char            systemProcessCommand(char);
char            command;

void main (void)
{


//  dummy1 = Read_b_eep(0x00);
//  dummy2 = Read_b_eep(0x01);

  //TRISA     = 0b01000001; 	    // PORTA bit 3 to output (0) ; other bits are inputs (1)
  //PORT_IO_INIT();

  OSCCON    = 0x73;             // internal clock at 8MHz

  systemInit();

  IPEN      = 1;                // enable interrupt prioritization
  PEIE      = 1;                // enable peripheral interrupts
  ei();

 // PANEL_PWM_ON_OFF_STATUS_LED(1);

  while (1){
      if (systemState == SYSTEM_RUNNING_STATE) {
        if (systemUvTimer_ticks == 0 || command == START_STOP_PRESSED) {
         command = systemIdle();
        } 
      }
      else if (command == START_STOP_PRESSED) {
        command = systemRun();
      }
      else if (command != NULL_COMMAND) {
        command = systemProcessCommand(command);
      }
  };
}

void interrupt InterruptServiceHigh(void) {


  if (TMR1IF) {
    systemTime_tick++;

    if (systemTime_tick % 5 == 0) {
      panelToggleSystemOnOffStatusLed();
    }
    
    command = panelGetCommand();
    
    if (systemUvTimer_ticks) {
      systemUvTimer_ticks--;
    }

    TIMER_INT_CLR();
    TIMER_RESTART(TIMER_PERIOD_MS);

  } 

}

//// -------------------- Iterrupt Service Routines --------------------------
void interrupt low_priority InterruptServiceLow(void)
{
    LATA2 = ~LATA2; // toggle LATD
    INT1IF = 0;
    INT2IF = 0;
}

void systemInit() {

    // Initialize PORTA IO pins (UV PWM, Buzzer, Heartbeat LED and Test LED)
    PORT_IO_INIT();
    
    // Initialize LCD
    LCDInit();
    LCDWelcomeMessage();

    // Initialize panel buttons
    panelInit();
    
    // set up timer to run at 100ms period with high priority interrupt
    TIMER_INIT(TIMER_PERIOD_MS, HIGH_PRIORITY_INT);
    
    // read ExposureTime and IntensityValue from EEPROM

    // Update Display with Timer and Intensity Values

    systemIdle();
}

char systemProcessCommand(char cmd) {

    if (systemState == SYSTEM_IDLE_STATE) {
      switch(cmd) {
        case NULL_COMMAND:
          break;
        case COUNT_DOWN_PRESSED(COUNT_BUTTON_TIMER_OFFSET):
          break;
        case COUNT_DOWN_HELD(COUNT_BUTTON_TIMER_OFFSET):
          break;
        case COUNT_DOWN_RELEASED(COUNT_BUTTON_TIMER_OFFSET):
          break;
        case COUNT_UP_PRESSED(COUNT_BUTTON_TIMER_OFFSET):
          break;
        case COUNT_UP_HELD(COUNT_BUTTON_TIMER_OFFSET):
          break;
        case COUNT_UP_RELEASED(COUNT_BUTTON_TIMER_OFFSET):
          break;
        case COUNT_DOWN_PRESSED(COUNT_BUTTON_INTENSITY_OFFSET):
          break;
        case COUNT_DOWN_HELD(COUNT_BUTTON_INTENSITY_OFFSET):
          break;
        case COUNT_DOWN_RELEASED(COUNT_BUTTON_INTENSITY_OFFSET):
          break;
        case COUNT_UP_PRESSED(COUNT_BUTTON_INTENSITY_OFFSET):
          break;
        case COUNT_UP_HELD(COUNT_BUTTON_INTENSITY_OFFSET):
          break;
        case COUNT_UP_RELEASED(COUNT_BUTTON_INTENSITY_OFFSET):
          break;

        default: break;
      }
    }
  return NULL_COMMAND;
}

char systemIdle() {

  // turn off UV
  SYSTEM_UV_PWM(0);
  
  // turn off UV timer
  systemUvTimer_ticks = 0;
  
  systemState = SYSTEM_IDLE_STATE;

  panelSetUiState( PANEL_UI_IDLE );

  return NULL_COMMAND;
}

char systemRun() {

  // record timer and intensity into EEPROM

  // start UV timer
  systemUvTimer_ticks = (unsigned int) (exposureTime_sec * TIMER_FREQ_HZ );
  
  // turn on UV
  SYSTEM_UV_PWM(1);

  // set system state
  systemState = SYSTEM_RUNNING_STATE;

  // change UI to a single counter (buttons are disabled), turn on UvOnOff status LED
  panelSetUiState( PANEL_UI_RUNNING );

  return NULL_COMMAND;
}

char systemToggle() {

  if (systemState == SYSTEM_RUNNING_STATE) {
    systemIdle();
  } else {
    systemRun();
  }

  return NULL_COMMAND;
}