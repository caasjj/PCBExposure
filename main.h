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

// Configure the hardware pins
#define TIMER_INTENSITY_FUNC_SEL    ADC_0ANA
    

// Configure Timer1 for 100Hz interrupts
#define FOSC                    8000000
#define SYS_CLK_FREQ_HZ         FOSC / 4
#define SYS_CLK_FREQ_kHZ        SYS_CLK_FREQ_HZ / 1000
#define TIMER_PRESCALER         8
#define TIMER_CLK_FREQ_kHZ      SYS_CLK_FREQ_kHZ / TIMER_PRESCALER
#define TIMER_PERIOD_MS         100
#define TMR1_LOAD_VALUE         (unsigned int) (65536 - TIMER_PERIOD_MS * TIMER_CLK_FREQ_kHZ)
#define TMR1_CONFIG             T1_16BIT_RW     | \
                                T1_PS_1_8       | \
                                T1_OSC1EN_OFF   | \
                                T1_SYNC_EXT_ON  | \
                                T1_SOURCE_INT   | \
                                TIMER_INT_ON

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

#define PANEL_SEL_SWITCH        ADC_CH0
#define PANEL_UP_DOWN_SWOTCH    ADC_CH1
#define PANEL_START_STOP_SWITCH ADC_CH4
    
#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

