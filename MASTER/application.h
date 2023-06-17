/* 
 * File:   application.h
 * Author: MOUSTAFA
 *
 * Created on June 8, 2023, 4:08 AM
 */


#ifndef APPLICATION_H
#define	APPLICATION_H
/* Section Includes*/
#include "MCAL_LAYER/GPIO/hal_gpio.h"
#include "MCAL_LAYER/EEPROM/hal_eeprom.h"
#include "MCAL_LAYER/SPI/hal_spi.h"
#include "MCAL_LAYER/TIMER0/hal_timer0.h"
#include "MCAL_LAYER/TIMER1/hal_timer1.h"
#include "ECU_LAYER/char_lcd/ecu_char_lcd.h"
#include "ECU_LAYER/key_pad/ecu_keypad.h"
#include "ECU_LAYER/led/ecu_led.h"
#include "MCAL_LAYER/Delay/M_Delay.h"

/* Section : macros Function Declarations */

/* Data types Declarations */
void Timer1Handler(void);
/* Structure Definition For The Leds of The Master*/
led_t ADMIN_Led={.port=PORTB_INDEX,.pin=GPIO_PIN0,.status=LED_OFF};
led_t GUEST_Led={.port=PORTB_INDEX,.pin=GPIO_PIN1,.status=LED_OFF};
led_t BLOCK_Led={.port=PORTB_INDEX,.pin=GPIO_PIN2,.status=LED_OFF};
/* Structure Definition For The Keypad */
keypad_t Keypad_Obj={.pin_keypad_rows[0].port=PORTA_INDEX,.pin_keypad_rows[0].pin=GPIO_PIN0,
                     .pin_keypad_rows[0].direction=GPIO_DIRECTION_OUTPUT,.pin_keypad_rows[0].logic=GPIO_LOW,
                     .pin_keypad_rows[1].port=PORTA_INDEX,.pin_keypad_rows[1].pin=GPIO_PIN1,
                     .pin_keypad_rows[1].direction=GPIO_DIRECTION_OUTPUT,.pin_keypad_rows[1].logic=GPIO_LOW,
                     .pin_keypad_rows[2].port=PORTA_INDEX,.pin_keypad_rows[2].pin=GPIO_PIN2,
                     .pin_keypad_rows[2].direction=GPIO_DIRECTION_OUTPUT,.pin_keypad_rows[2].logic=GPIO_LOW,
                     .pin_keypad_rows[3].port=PORTA_INDEX,.pin_keypad_rows[3].pin=GPIO_PIN3,
                     .pin_keypad_rows[3].direction=GPIO_DIRECTION_OUTPUT,.pin_keypad_rows[3].logic=GPIO_LOW,
                     .pin_keypad_colums[0].port=PORTA_INDEX,.pin_keypad_colums[0].pin=GPIO_PIN4,
                     .pin_keypad_colums[0].direction=GPIO_DIRECTION_INPUT,.pin_keypad_colums[0].logic=GPIO_LOW,
                     .pin_keypad_colums[1].port=PORTA_INDEX,.pin_keypad_colums[1].pin=GPIO_PIN5,
                     .pin_keypad_colums[1].direction=GPIO_DIRECTION_INPUT,.pin_keypad_colums[1].logic=GPIO_LOW,
                     .pin_keypad_colums[2].port=PORTA_INDEX,.pin_keypad_colums[2].pin=GPIO_PIN6,
                     .pin_keypad_colums[2].direction=GPIO_DIRECTION_INPUT,.pin_keypad_colums[2].logic=GPIO_LOW,
                     .pin_keypad_colums[3].port=PORTA_INDEX,.pin_keypad_colums[3].pin=GPIO_PIN7,
                     .pin_keypad_colums[3].direction=GPIO_DIRECTION_INPUT,.pin_keypad_colums[3].logic=GPIO_LOW};

/* Structure Definition For The Liquid Character Display  */
lcd_4bit_mode_t Lcd_Obj={.lcd_rs.port=PORTD_INDEX,.lcd_rs.pin=GPIO_PIN5,.lcd_rs.direction=GPIO_DIRECTION_OUTPUT,
                         .lcd_en.port=PORTD_INDEX,.lcd_en.pin=GPIO_PIN4,.lcd_rs.direction=GPIO_DIRECTION_OUTPUT,
                         .lcd_data[0].port=PORTD_INDEX,.lcd_data[0].pin=GPIO_PIN0,.lcd_data[0].direction=GPIO_DIRECTION_OUTPUT,
                         .lcd_data[1].port=PORTD_INDEX,.lcd_data[1].pin=GPIO_PIN1,.lcd_data[1].direction=GPIO_DIRECTION_OUTPUT,
                         .lcd_data[2].port=PORTD_INDEX,.lcd_data[2].pin=GPIO_PIN2,.lcd_data[2].direction=GPIO_DIRECTION_OUTPUT,
                         .lcd_data[3].port=PORTD_INDEX,.lcd_data[3].pin=GPIO_PIN3,.lcd_data[3].direction=GPIO_DIRECTION_OUTPUT};

/* Structure for The spi Initialize Master Mode*/
SPI_Config_t SPIObject={.SPI_InterruptHandler=NULL,.SPI_mode=SPI_MASTER_MODE_CLOCK_FOSC_DIV_4,
                        .SPI_config.SPI_interrupt_enable=SPI_INTERRUPT_DISABLE_CFG,
                        .SPI_config.clock_phase=SPI_MASTER_MODE_TRANSMITING_IDLE_TO_ACTIVE_CFG,
                        .SPI_config.clock_polarity=SPI_MASTER_MODE_IDLE_CLOCK_STATE_LOW_CFG,
                        .SPI_config.sampling_receiving=SPI_MASTER_MODE_INPUT_DATA_SAMPLED_AT_MIDDLE_CFG};

pin_config_t PIN_SelectChannel={.port=PORTC_INDEX,.pin=GPIO_PIN0,.direction=GPIO_DIRECTION_OUTPUT,.logic=GPIO_HIGH};


timer1_config_t Timer1Obj={.TIMER1_HandlerInterrupt=Timer1Handler,.preloaded_value=45536,
                           .timer1_mode_timer_or_counter=TIMER1_TIMER_MODE_CFG,
                           .timer1_mode_wr_reg=TIMER1_16BIT_MODE_CFG,
                           .timer1_pre_scaler_val=TIMER1_INPUT_CLK_PRESCALER_DIV_1,
                           .timer1_oscillator_enable=TIMER1_OSCILLATOR_DISABLE_CFG};
/* Function Declarations*/


#endif	/* APPLICATION_H */

