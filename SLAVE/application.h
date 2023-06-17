/* 
 * File:   application.h
 * Author: MOUSTAFA
 *
 * Created on June 10, 2023, 10:53 PM
 */

#ifndef APPLICATION_H
#define	APPLICATION_H

/* Section Includes*/
#include "ECU_LAYER/led/ecu_led.h"
#include "MCAL_LAYER/ADC/hal_adc.h"
#include "MCAL_LAYER/Delay/M_Delay.h"
#include "MCAL_LAYER/GPIO/hal_gpio.h"
#include "MCAL_LAYER/SPI/hal_spi.h"
#include "MCAL_LAYER/TIMER0/hal_timer0.h"
#include "MCAL_LAYER/TIMER1/hal_timer1.h"
#include "STD_Messeges.h"

void Timer1Handler(void);//Decleration Of The Timer1 Function Interrupt
uint16 GetResultFromTheADC(void);
void SPI_InterrutHandler(void);
/* Section : Macros Declarations  */

/* Section : macros Function Declarations */
#define AIR_CONDITIONING_ON   (uint8)0X01
#define AIR_CONDITIONING_OFF (uint8)0X00


/* Data types Declarations */
/**Structure Definition of The Leds That Indication That is A Room*/
led_t Room1_Led={.port=PORTD_INDEX,.pin=GPIO_PIN0,.status=LED_OFF};//led Of The Room1
led_t Room2_Led={.port=PORTD_INDEX,.pin=GPIO_PIN1,.status=LED_OFF};//led Of The Room2
led_t Room3_Led={.port=PORTD_INDEX,.pin=GPIO_PIN2,.status=LED_OFF};//led Of The Room3
led_t Room4_Led={.port=PORTD_INDEX,.pin=GPIO_PIN3,.status=LED_OFF};//led Of The Room4
led_t TV_Led={.port=PORTD_INDEX,.pin=GPIO_PIN4,.status=LED_OFF};//led Of The TV
led_t AIR_COND_Led={.port=PORTD_INDEX,.pin=GPIO_PIN5,.status=LED_OFF};//led Of The Air Conditioning

/* Structure for The spi Initialize Slave Mode*/
SPI_Config_t SPIObject={.SPI_InterruptHandler=SPI_InterrutHandler,.SPI_mode=SPI_SLAVE_MODE_SS_BIN_ENABLED,
                        .SPI_config.SPI_interrupt_enable=SPI_INTERRUPT_ENABLE_CFG,
                        .SPI_config.clock_phase=SPI_MASTER_MODE_TRANSMITING_IDLE_TO_ACTIVE_CFG,
                        .SPI_config.clock_polarity=SPI_MASTER_MODE_IDLE_CLOCK_STATE_LOW_CFG,
                        .SPI_config.sampling_receiving=SPI_MASTER_MODE_INPUT_DATA_SAMPLED_AT_MIDDLE_CFG};

/*****************Timer Structure It Interrupt Every 10 Milli Seconds*/
timer1_config_t Timer1Obj={.TIMER1_HandlerInterrupt=Timer1Handler,.preloaded_value=45536,
                           .timer1_mode_timer_or_counter=TIMER1_TIMER_MODE_CFG,
                           .timer1_mode_wr_reg=TIMER1_16BIT_MODE_CFG,
                           .timer1_pre_scaler_val=TIMER1_INPUT_CLK_PRESCALER_DIV_1,
                           .timer1_oscillator_enable=TIMER1_OSCILLATOR_DISABLE_CFG};

/**Analog To Digital Converter Structure Definition*/
adc_config_t ADC_Obj={.ADC_HandlerInterrupt=NULL,.adc_channel=ADC_CHANNEL_AN0,
                      .adc_clock_freq=ADC_CONVERSION_CLOCK_FOSC_DIV_4,
                      .adc_time_conversion=ADC_12_TAD,
                      .result_format=ADC_RIGHT_RESULT_FORMAT,
                      .volatge_reference=ADC_VOLTAGE_REFERNCE_DISABLE_BIT};
/* Function Declarations*/

#endif	/* APPLICATION_H */

