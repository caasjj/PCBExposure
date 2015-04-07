//******************************************************************************
//Software License Agreement                                         
//                                                                    
//The software supplied herewith by Microchip Technology             
//Incorporated (the "Company") is intended and supplied to you, the  
//Company’s customer, for use solely and exclusively on Microchip    
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

/** C O N F I G U R A T I O N   B I T S ******************************/

#pragma config OSC    = INTIO7, FCMEN   = OFF,     IESO   = OFF                  // CONFIG1H
#pragma config PWRT   = OFF,    BOREN   = SBORDIS, BORV   = 3                    // CONFIG2L
#pragma config WDT    = OFF,    WDTPS   = 32768                                  // CONFIG2H
#pragma config MCLRE  = ON,     LPT1OSC = OFF,     PBADEN = OFF, CCP2MX = PORTBE // CONFIG3H
#pragma config STVREN = ON,     LVP     = OFF,     XINST  = OFF                  // CONFIG4L
#pragma config CP0    = OFF,    CP1     = OFF,     CP2    = OFF, CP3    = OFF    // CONFIG5L
#pragma config CPB    = OFF,    CPD     = OFF                                    // CONFIG5H
#pragma config WRT0   = OFF,    WRT1    = OFF,     WRT2   = OFF, WRT3   = OFF    // CONFIG6L
#pragma config WRTB   = OFF,    WRTC    = OFF,     WRTD   = OFF                  // CONFIG6H
#pragma config EBTR0  = OFF,    EBTR1   = OFF,     EBTR2  = OFF, EBTR3   = OFF   // CONFIG7L
#pragma config EBTRB  = OFF                                                      // CONFIG7H


/** D E C L A R A T I O N S *******************************************/
unsigned int exposureTime_sec    = 240;
unsigned int exposureCurrent_mA  = 20;
unsigned int timerLoadValue = TMR1_LOAD_VALUE;

char dummy1, dummy2;

unsigned int dummy3 = 600;
unsigned int dummy4;

void init(void);

void main (void)
{

  dummy1 = Read_b_eep(0x00);
  dummy2 = Read_b_eep(0x01);

  char dummyConfig = TMR1_CONFIG;

  WriteTimer1(timerLoadValue);
  OpenTimer1(dummyConfig);  //API configures the tmer1 as per user defined

  // WARNING: MAIN REASON WHY LCD WSA NOT WORKING ... T1OSC used instead
  // of portc bit 0 digital output %$@#!$. This doesn't matter anymore,
  // since we switched to 4 bit interface.
  // T1OSCEN   = 0;

  TRISA     = 0b01000001; 	    // PORTA bit 3 to output (0) ; other bits are inputs (1)
  TRISB     = 0xf0;             // make lower 4 bits of PORTB outputs for the LCD RW/EN/RS signals

  PEIE      = 1;                // enable peripheral interrupts
//  INT1IE    = 1;                // enable interrupt 1
  TMR1IE    = 1;                // enable timer 1 interrupt

  OSCCON    = 0x73;             // internal clock at 8MHz
  TMR1IP    = 1;                // timer 1 at high priority interrupt
//  INT1IP    = 0;                // interrupt 1 is low priority interrupt
//  INT2IP    = 0;                // interrupt 2 is also low priority
  IPEN      = 1;                // enable interrupt prioritization
  ei();
  
  printf("Hello, world!\n");

  // Initialize the LCD
  LCDInit();
  LCDWelcomeMessage();

  init();
  
  LATA3 = 1;
  while (1){};

}

void interrupt InterruptServiceHigh(void) {

  if (TMR1IF) {

    // If Timer Interrupt
    //   If !CounterButtonActive
    //       If RisingEdge
    //          pressedCount = 0
    //          numUpdates = 1
    //          Target = Read Intensity/Timer function select
    //          Increment Target by TargetUpdateDelta
    //          CountButtonActive=true
    //       else If FallingEdge
    //          pressedCount = 0
    //          numUpdates = 1
    //          Target = Read Intensity/Timer function select
    //          Decrement Target by TargetUpdateDelta
    //          CountButtonActive=true
    //   else If button still pressed
    //           If pressedCount++ % TargetUpdateRate == 0
    //              UpdateTarget by TargetUpdateDelta
    //              numUpdates++
    //              if numUpdates > consecutiveUpdateThrshold, TargetUpdateDelta *= 2
    //        else
    //           numUpdates = 0
    //           TargetUpdateDelta back to original value

    LATA2 = ~LATA2;
    WriteTimer1(timerLoadValue);

    if (--dummy3 == 0) {
      LATA3 = 0;
    }
    
    TMR1IF = 0;
    INT1IF = 0;
    ConvertADC();

  } else if (ADIF) {
    dummy4 = ReadADC();
    ADIF = 0;
  }

}

//// -------------------- Iterrupt Service Routines --------------------------
void interrupt low_priority InterruptServiceLow(void)
{
    LATA2 = ~LATA2; // toggle LATD
    INT1IF = 0;
    INT2IF = 0;
}

void init() {

    // Initialize LCD

    // Initialize ADC
    OpenADC(ADC_CONFIG, ADC_CONFIG2, ADC_PORT_CONFIG);
    ADC_INT_ENABLE();
    
    // assign ADC pin RA1/AN1 to CountUp/CountDown of ExposureTime/Intensity as selected above

    // Assign digital pin RA4 to Start/Stop (30mS time debounce time constant)

    // assign pin RA5 to LED PWM

    // assign pin RC0 to Power ON status LED ( green )

    // assign pin RC1 to UV Led ON status LED ( red )

    // assign pin RC2 to buzzer

    // assign pin
    // set up timer to run at 100Hz with high priority interrupt

    // read ExposureTime and IntensityValue from EEPROM

    // Update Display with Timer and Intensity Values
}