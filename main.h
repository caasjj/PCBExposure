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
#pragma config MCLRE  = ON,     LPT1OSC = OFF,     PBADEN = OFF, CCP2MX = PORTC // CONFIG3H
#pragma config STVREN = ON,     LVP     = OFF,     XINST  = OFF                  // CONFIG4L
#pragma config CP0    = OFF,    CP1     = OFF,     CP2    = OFF, CP3    = OFF    // CONFIG5L
#pragma config CPB    = OFF,    CPD     = OFF                                    // CONFIG5H
#pragma config WRT0   = OFF,    WRT1    = OFF,     WRT2   = OFF, WRT3   = OFF    // CONFIG6L
#pragma config WRTB   = OFF,    WRTC    = OFF,     WRTD   = OFF                  // CONFIG6H
#pragma config EBTR0  = OFF,    EBTR1   = OFF,     EBTR2  = OFF, EBTR3   = OFF   // CONFIG7L
#pragma config EBTRB  = OFF                                                      // CONFIG7H


/**  S Y S T E M   C O N S T A N T S   *************************************/
#define FOSC                                    8000000
#define SYS_CLK_FREQ_HZ                         FOSC / 4
#define SYS_CLK_FREQ_kHZ                        SYS_CLK_FREQ_HZ / 1000
#define HIGH_PRIORITY_INT                       1
#define LOW_PRIORITY_INT                        0
#define SYSTEM_INIT_STATE                       0
#define SYSTEM_IDLE_STATE                       1
#define SYSTEM_RUNNING_STATE                    2
#define SYSTEM_BUTTON_HOLD_TIME_MS              200 // must be integer multiple of system timer tick
#define SYSTEM_MAX_EXPOSURE_TIME_S              300
#define ON                                      1
#define OFF                                     0

/**  G E R I C   U S E   M A C R O S   ************************************/
#define MAKE_PORT_OUTPUT(port, pin)             port &= ~(1 << pin)
#define MAKE_PORT_INPUT(port, pin)              port |= (1 << pin )

    
/** C O N F I G U R E   S Y S T E M   P I N S  ***************************/
#define SYSTEM_DIAG_TRIS                        TRISC
#define SYSTEM_DIAG_PIN                         0
#define SYSTEM_DIAG_LATCH                       LATC0

#define SYSTEM_UV_BIAS_PWM_TRIS                 TRISC
#define SYSTEM_UV_BIAS_PWM_PIN                  1
#define SYSTEM_UV_BIAS_CCPCON                   CCP2CON
#define SYSTEM_UV_BIAS_CCPRL                    CCPR2L

#define SYSTEM_UV_SUPPLY_TRIS                   TRISC
#define SYSTEM_UV_SUPPLY_PIN                    2
#define SYSTEM_UV_SUPPLY_LATCH                  LATC2

#define SYSTEM_BUZZER_TRIS                      TRISC
#define SYSTEM_BUZZER_PIN                       3
#define SYSTEM_BUZZER_LATCH                     LATC3

#define SYSTEM_UV_PWM_POLARITY                  -1
#define SYSTEM_UV_PWM_MAX                       (SYSTEM_UV_PWM_POLARITY > 0) * 0x3E8
#define SYSTEM_UV_PWM_MIN                       (SYSTEM_UV_PWM_POLARITY < 0) * 0x3E8

#define SET_UV_PWM(x)                           SYSTEM_UV_BIAS_CCPCON |= (x | 0x3) << 4; \
                                                SYSTEM_UV_BIAS_CCPRL = (x >> 2) & 0xFF

/** I O    D R I V E R    M A C R O S ************************************/
#define SYSTEM_DIAG(OnOff)                      SYSTEM_DIAG_LATCH=OnOff
#define SET_UV_INTENSITY_PERCENT(x)             SET_UV_PWM( SYSTEM_UV_PWM_MIN + SYSTEM_UV_PWM_POLARITY*x*10 )
#define UV(OnOff)                               SYSTEM_UV_SUPPLY_LATCH=OnOff
#define BUZZER(OnOff)                           SYSTEM_BUZZER_LATCH=OnOff
    
/** C O N F I G U R E    D I A G   P I N S   *****************************/
#define SYSTEM_TEST_LED_TRIS                    TRISC
#define SYSTEM_TEST_LED_PIN                     0
#define SYSTEM_TEST_LED_LATCH                   LATC0

#define PORT_IO_INIT()                          MAKE_PORT_OUTPUT(SYSTEM_DIAG_TRIS, SYSTEM_DIAG_PIN); \
                                                MAKE_PORT_OUTPUT(SYSTEM_UV_SUPPLY_TRIS, SYSTEM_UV_SUPPLY_PIN ); \
                                                MAKE_PORT_OUTPUT(SYSTEM_BUZZER_TRIS, SYSTEM_BUZZER_PIN )
                                                
#define SYSTEM_UV_PWM_INIT()                    MAKE_PORT_OUTPUT(SYSTEM_UV_BIAS_PWM_TRIS, SYSTEM_UV_BIAS_PWM_PIN);\
                                                SYSTEM_UV_BIAS_CCPCON |= 0x0C; \
                                                PR2=0xFF; \
                                                T2CON=0x04; \
                                                SET_UV_INTENSITY_PERCENT(0); \
                                                UV(OFF);

/** C O N F I G U R E    T I M E R 1  ************************************/
#define TIMER_PRESCALER                         8
#define TIMER_CLK_FREQ_kHZ                      SYS_CLK_FREQ_kHZ / TIMER_PRESCALER
#define TIMER_PERIOD_MS                         50
#define TIMER_FREQ_HZ                           (unsigned int )(1000 / TIMER_PERIOD_MS)
#define TIMER_START_VALUE(x)                    (unsigned int) (65536 - x * TIMER_CLK_FREQ_kHZ)
#define TIMER_CONFIG                            T1_16BIT_RW     | \
                                                T1_PS_1_8       | \
                                                T1_OSC1EN_OFF   | \
                                                T1_SYNC_EXT_ON  | \
                                                T1_SOURCE_INT   | \
                                                TIMER_INT_ON

#define TIMER_INIT(x,p)                         OpenTimer1(TIMER_CONFIG);   \
                                                TMR1IP    = p;              \
                                                WriteTimer1(TIMER_START_VALUE(x));

#define TIMER_RESTART(x)                        WriteTimer1(TIMER_START_VALUE(x))

#define TIMER_INT_CLR()                         TMR1IF=0
#define TIMER_SEC_TO_TICKS(x)                   (unsigned int) (x * TIMER_FREQ_HZ)

/*** F U N C T I O N S   **********************************************/
void            systemInit(void);
void            systemRun(void);
void            systemIdle(void);
void            systemProcessCommand(char);
void            saveToEE(void);
void            readFromEE(void);
void            uvOn(int);
void            uvOff(void);
void            buzzerOn(void);
void            buzzerOff(void);
void            buzz(int);
#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

