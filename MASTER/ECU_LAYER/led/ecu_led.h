/* 
 * File:   led.h
 * Author: MOUSTAFA
 *
 * Created on February 14, 2023, 7:46 AM
 */

#ifndef ECU_LED_H
#define	ECU_LED_H
/* Section : Includes  */
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "ecu_led_config.h"


/* Section : Macros Declarations  */

/* Section : macros Function Declarations */

/* Data types Declarations */
typedef enum {
    LED_OFF=0,
    LED_ON        
}led_status;

typedef struct {
    uint8  port :4;
    uint8  pin  :3;
    uint8  status : 1;
}led_t;

/* Function Declarations*/
Std_ReturnType led_initialize(const led_t *led);
Std_ReturnType led_turn_on(const led_t *led);
Std_ReturnType led_turn_off(const led_t *led);
Std_ReturnType led_turn_toggle(const led_t *led);

#endif	/* LED_H */

