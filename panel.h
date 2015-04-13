/* 
 * File:   panel.h
 * Author: developer
 *
 * Created on April 7, 2015, 12:16 PM
 */

#ifndef PANEL_H
#define	PANEL_H

#define USE_OR_MASKS

#include <xc.h>
#include <stdbool.h>
#include <plib/adc.h>

#ifdef	__cplusplus
extern "C" {
#endif

/**  G E R I C   U S E   M A C R O S   *****************************************/
#define MAKE_PORT_OUTPUT(port, pin)             port &= ~(1 << pin)
#define MAKE_PORT_INPUT(port, pin)              port |= (1 << pin )

/** P O R T   A N D   P I N   A S S I G N M E N T S  ***************************/
#define PANEL_SEL_SWITCH_PORT                   TRISA
#define PANEL_SEL_SWITCH_PIN                    0
#define PANEL_UP_DOWN_BUTTON_PORT               TRISA
#define PANEL_UP_DOWN_BUTTON_PIN                1
#define PANEL_START_STOP_BUTTON_PORT            TRISA
#define PANEL_START_STOP_BUTTON_PIN             2
#define PANEL_SYSTEM_ON_OFF_STATUS_LED_PORT     TRISA
#define PANEL_SYSTEM_ON_OFF_STATUS_LED_PIN      4
#define PANEL_SYSTEM_ON_OFF_STATUS_LED_LATCH    LATA4
#define PANEL_UV_ON_OFF_STATUS_LED_PORT         TRISA
#define PANEL_UV_ON_OFF_STATUS_LED_PIN          5
#define PANEL_UV_ON_OFF_STATUS_LED_LATCH        LATA5

//#define PANEL_LCD_BACKLIGHT_ON_OFF_PORT         TRISB
//#define PANEL_LCD_BACKLIGHT_ON_OFF_PIN          4
//#define PANEL_LCD_BACKLIGHT_ON_OFF_LATCH        LATA4

/** L C D    B A C K L I G H T   M A C R O *************************************/
//#define PANEL_LCD_BACKLIGHT(x)                  PANEL_LCD_BACKLIGHT_ON_OFF_LATCH(x)

/** C O N F I G U R E   A D C   ************************************************/
// for use with 'chan' parameter in _panelReadInput( chan )
#define PANEL_SEL_SWITCH                        ADC_CH0
#define PANEL_UP_DOWN_BUTTON                    ADC_CH1
#define PANEL_START_STOP_BUTTON                 ADC_CH2
#define PANEL_LCD_BACKLIGHT_ON_OFF_BUTTON       ADC_CH3

#define ADC_CONFIG2                             PANEL_SEL_SWITCH    | \
                                                ADC_INT_OFF         | \
                                                ADC_REF_VDD_VSS

#define ADC_CONFIG                              ADC_FOSC_8          | \
                                                ADC_RIGHT_JUST      | \
                                                ADC_4_TAD

#define ADC_PORT_CONFIG                         ADC_4ANA  // AN0, AN1, AN2 and AN3 are analog - 4 analog channels

#define ADC_INT_CLR()                           ADIF = 0
#define ADC_SWITCH_HIGH_THRESHOLD               0x240
#define ADC_SWITCH_LOW_THRESHOLD                0x140

/** C O M M A N D   C O D E S   ************************************************/
#define NULL_COMMAND                            (char)-1
#define COUNT_BUTTON_TIMER_OFFSET               0
#define COUNT_BUTTON_INTENSITY_OFFSET           10
#define COUNT_DOWN_PRESSED(x)                   x
#define COUNT_DOWN_HELD(x)                      x + 1
#define COUNT_DOWN_RELEASED(x)                  x + 2
#define COUNT_UP_PRESSED(x)                     x + 3
#define COUNT_UP_HELD(x)                        x + 4
#define COUNT_UP_RELEASED(x)                    x + 5

#define START_STOP_PRESSED                      20
#define START_STOP_HELD                         21
#define START_STOP_RELEASED                     22

#define LCD_BACKLIGHT_PRESSED                   30
#define LCD_BACKLIGHT_HELD                      31
#define LCD_BACKLIGHT_RELEASED                  32
/** F U N C T I O N   D E C L A R A T I O N S  *********************************/
void panelInit(signed char, int );
unsigned char panelGetCommand(void);

void panelSetUiState( unsigned char );

void panelSetSystemOnOffStatusLed( char );
void panelToggleSystemOnOffStatusLed();

void panelSetUvOnOffStatusLed( char );
void panelTogglePwmOnOffStatusLed();

void panelToggleHeartbeat();

void panelUpdateUi(signed char, int);
void panelUpdateValues(signed char , int );

void panelToggleBacklight(void);

int _panelReadInput(unsigned char);


/** U I   C O N F I G U R A T I O N */
#define PANEL_UI_IDLE                         0
#define PANEL_UI_RUNNING                      1

#ifdef	__cplusplus
}
#endif

#endif	/* PANEL_H */

