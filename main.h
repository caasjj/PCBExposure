/* 
 * File:   main.h
 * Author: developer
 *
 * Created on April 6, 2015, 10:43 AM
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif

#define USE_OR_MASKS

// System constants
#define FOSC                    8000000
#define SYS_CLK_FREQ_HZ         FOSC / 4
#define SYS_CLK_FREQ_kHZ        SYS_CLK_FREQ_HZ / 1000

// Generic Macros
#define MAKE_PORT_OUTPUT(port, pin)             port &= ~(1 << pin)
#define MAKE_PORT_INPUT(port, pin)              port |= (1 << pin )

    
// Configure PORTA pins
#define PANEL_SEL_SWITCH_TRIS                   TRISA
#define PANEL_SEL_SWITCH_PIN                    0
#define PANEL_UP_DOWN_SWITCH_TRIS               TRISA
#define PANEL_UP_DOWN_SWITCH_PIN                1
#define PANEL_START_STOP_SWITCH_TRIS            TRISA
#define PANEL_START_STOP_SWITCH_PIN             2
    
#define PANEL_SYSTEM_ON_OFF_STATUS_LED_TRIS     TRISA
#define PANEL_SYSTEM_ON_OFF_STATUS_LED_PIN      3
#define PANEL_SYSTEM_ON_OFF_STATUS_LED_LATCH    LATA3
#define PANEL_PWM_ON_OFF_STATUS_LED_TRIS        TRISA
#define PANEL_PWM_ON_OFF_STATUS_LED_PIN         4
#define PANEL_PWM_ON_OFF_STATUS_LED_LATCH       LATA4

#define SYSTEM_UV_LED_PWM_TRIS                  TRISA
#define SYSTEM_UV_LED_PWM_PIN                   5
#define SYSTEM_UV_LED_PWM_LATCH                 LATA5
#define SYSTEM_BUZZER_TRIS                      TRISA
#define SYSTEM_BUZZER_PIN                       6
#define SYSTEM_BUZZER_LATCH                     LATA6

#define PANEL_SYSTEM_ON_OFF_STATUS_LED(x)       PANEL_SYSTEM_ON_OFF_STATUS_LED_LATCH=x
#define PANEL_SYSTEM_ON_OFF_STATUS_LED_TOGGLE() PANEL_SYSTEM_ON_OFF_STATUS_LED_LATCH=~PANEL_SYSTEM_ON_OFF_STATUS_LED_LATCH
#define PANEL_PWM_ON_OFF_STATUS_LED(x)          PANEL_PWM_ON_OFF_STATUS_LED_LATCH=x
#define PANEL_PWM_ON_OFF_STATUS_LED_TOGGLE()    PANEL_PWM_ON_OFF_STATUS_LED_LATCH=~PANEL_PWM_ON_OFF_STATUS_LED_LATCH

#define PORT_IO_INIT()  MAKE_PORT_INPUT(PANEL_SEL_SWITCH_TRIS, PANEL_SEL_SWITCH_PIN); \
                        MAKE_PORT_INPUT(PANEL_UP_DOWN_SWITCH_TRIS, PANEL_UP_DOWN_SWITCH_PIN); \
                        MAKE_PORT_INPUT(PANEL_START_STOP_SWITCH_TRIS, PANEL_START_STOP_SWITCH_PIN); \
                        MAKE_PORT_OUTPUT(PANEL_SYSTEM_ON_OFF_STATUS_LED_TRIS, PANEL_SYSTEM_ON_OFF_STATUS_LED_PIN ); \
                        MAKE_PORT_OUTPUT(PANEL_PWM_ON_OFF_STATUS_LED_TRIS, PANEL_PWM_ON_OFF_STATUS_LED_PIN ); \
                        MAKE_PORT_OUTPUT(SYSTEM_UV_LED_PWM_TRIS, SYSTEM_UV_LED_PWM_PIN ); \
                        MAKE_PORT_OUTPUT(SYSTEM_BUZZER_TRIS, SYSTEM_BUZZER_PIN ); \

// Configure Timer1 for 100Hz interrupts
#define TIMER_PRESCALER         8
#define TIMER_CLK_FREQ_kHZ      SYS_CLK_FREQ_kHZ / TIMER_PRESCALER
#define TIMER_PERIOD_MS         100
#define TIMER_START_VALUE       (unsigned int) (65536 - TIMER_PERIOD_MS * TIMER_CLK_FREQ_kHZ)
#define TIMER_CONFIG            T1_16BIT_RW     | \
                                T1_PS_1_8       | \
                                T1_OSC1EN_OFF   | \
                                T1_SYNC_EXT_ON  | \
                                T1_SOURCE_INT   | \
                                TIMER_INT_ON

#define TIMER_INIT()            OpenTimer1(TIMER_CONFIG);   \
                                TMR1IP    = 1;              \
                                WriteTimer1(TIMER_START_VALUE);

#define TIMER_RESTART()         WriteTimer1(TIMER_START_VALUE)

#define TIMER_INT_CLR()         TMR1IF=0
    
// Configure ADC
#define ADC_CONFIG2             ADC_CH0     | \
                                ADC_INT_OFF | \
                                ADC_REF_VDD_VSS

#define ADC_CONFIG              ADC_FOSC_8 | \
                                ADC_RIGHT_JUST | \
                                ADC_4_TAD

#define ADC_PORT_CONFIG         ADC_0ANA | \
                                ADC_1ANA | \
                                ADC_2ANA

#define ADC_INIT()              OpenADC(ADC_CONFIG, ADC_CONFIG2, ADC_PORT_CONFIG); \
                                ADC_INT_ENABLE();
    
#define ADC_INT_CLR()           ADIF = 0
#define ADC_SWITCH_HI_THRESH    0x300
#define ADC_SWITCH_LO_THRESH    0x100
    
#define PANEL_SEL_SWITCH        ADC_CH0
#define PANEL_UP_DOWN_SWITCH    ADC_CH1
#define PANEL_START_STOP_SWITCH ADC_CH2
    
#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

