/* 
 * File:   main.h
 * Author: developer
 *
 * Created on April 6, 2015, 10:43 AM
 */

#ifndef MAIN_H
#define	MAIN_H

#define USE_OR_MASKS

#include "panel.h"

#ifdef	__cplusplus
extern "C" {
#endif

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



/**  S Y S T E M   C O N S T A N T S   *************************************/
#define FOSC                        8000000
#define SYS_CLK_FREQ_HZ             FOSC / 4
#define SYS_CLK_FREQ_kHZ            SYS_CLK_FREQ_HZ / 1000
#define HIGH_PRIORITY_INT           1
#define LOW_PRIORITY_INT            0
#define SYSTEM_INIT_STATE           0
#define SYSTEM_IDLE_STATE           1
#define SYSTEM_RUNNING_STATE        2
#define SYSTEM_BUTTON_HOLD_TIME_MS  200 // must be integer multiple of system timer tick
#define SYSTEM_MAX_EXPOSURE_TIME_S  300U

/**  G E R I C   U S E   M A C R O S   ************************************/
#define MAKE_PORT_OUTPUT(port, pin)             port &= ~(1 << pin)
#define MAKE_PORT_INPUT(port, pin)              port |= (1 << pin )

    
/** C O N F I G U R E   S Y S T E M   P I N S  ***************************/
#define SYSTEM_UV_PWM_TRIS                      TRISA
#define SYSTEM_UV_PWM_PIN                       5
#define SYSTEM_UV_PWM_LATCH                     LATA5
#define SYSTEM_BUZZER_TRIS                      TRISA
#define SYSTEM_BUZZER_PIN                       6
#define SYSTEM_BUZZER_LATCH                     LATA6

#define SYSTEM_UV_PWM(x)                        SYSTEM_UV_PWM_LATCH=x

/** C O N F I G U R E    D I A G   P I N S   *****************************/
#define SYSTEM_HEARTBEAT_LED_TRIS               TRISC
#define SYSTEM_HEARTBEAT_LED_PIN                1
#define SYSTEM_HEARTBEAT_LED_LATCH              LATC1
#define SYSTEM_TEST_LED_TRIS                    TRISC
#define SYSTEM_TEST_LED_PIN                     2
#define SYSTEM_TEST_LED_LATCH                   LATC2

#define PORT_IO_INIT()  MAKE_PORT_OUTPUT(SYSTEM_UV_PWM_TRIS, SYSTEM_UV_PWM_PIN ); \
                        MAKE_PORT_OUTPUT(SYSTEM_BUZZER_TRIS, SYSTEM_BUZZER_PIN ); \
                        MAKE_PORT_OUTPUT(SYSTEM_HEARTBEAT_LED_TRIS, SYSTEM_HEARTBEAT_LED_PIN); \
                        MAKE_PORT_OUTPUT(SYSTEM_TEST_LED_TRIS, SYSTEM_TEST_LED_PIN);

/** C O N F I G U R E    T I M E R 1  ************************************/
#define TIMER_PRESCALER         8
#define TIMER_CLK_FREQ_kHZ      SYS_CLK_FREQ_kHZ / TIMER_PRESCALER
#define TIMER_PERIOD_MS         50
#define TIMER_FREQ_HZ           (unsigned int )(1000 / TIMER_PERIOD_MS)
#define TIMER_START_VALUE(x)    (unsigned int) (65536 - x * TIMER_CLK_FREQ_kHZ)
#define TIMER_CONFIG            T1_16BIT_RW     | \
                                T1_PS_1_8       | \
                                T1_OSC1EN_OFF   | \
                                T1_SYNC_EXT_ON  | \
                                T1_SOURCE_INT   | \
                                TIMER_INT_ON

#define TIMER_INIT(x,p)         OpenTimer1(TIMER_CONFIG);   \
                                TMR1IP    = p;              \
                                WriteTimer1(TIMER_START_VALUE(x));

#define TIMER_RESTART(x)         WriteTimer1(TIMER_START_VALUE(x))

#define TIMER_INT_CLR()         TMR1IF=0

    
#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

