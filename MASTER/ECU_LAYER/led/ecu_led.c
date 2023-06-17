/* 
 * File:   led.c
 * Author: MOUSTAFA
 *
 * Created on February 14, 2023, 7:46 AM
 */
#include "ecu_led.h"
/**
 * 
 * @brief function that initialize the led to be output and made it on or off
 * @param led pointer to led status @ref led_t
 * @return  the status of the function 
 *          (E_OK) the function done successfully
 *           (E_NOT_OK) the function don't done successfully
 */
Std_ReturnType led_initialize(const led_t *led){
    Std_ReturnType ret=E_OK;
    if (NULL==led){
        ret=E_NOT_OK;
    }
    else {
    pin_config_t pin_config={.direction=GPIO_DIRECTION_OUTPUT,
                             .logic=led->status,
                             .pin=led->pin,.port=led->port};
    gpio_Pin_Intialize(&pin_config);
    
    }
    return ret;
}
/**
 * 
 * @brief function make led on 
 * @param led led pointer to led status @ref led_t
 * @return the status of the function 
 *          (E_OK) the function done successfully
 *           (E_NOT_OK) the function don't done successfully
 */
Std_ReturnType led_turn_on( const led_t *led){
    Std_ReturnType ret=E_OK;
    if (NULL==led){
        ret=E_NOT_OK;
    }
    else {
    pin_config_t pin_config={.direction=GPIO_DIRECTION_OUTPUT,
                             .logic=led->status,
                             .pin=led->pin,.port=led->port};
    gpio_Pin_Write_Logic(&pin_config,GPIO_HIGH);
    
    }
    return ret;
}
/**
 * 
 * @param led led pointer to led status @ref led_t
 * @return the status of the function 
 *          (E_OK) the function done successfully
 *           (E_NOT_OK) the function don't done successfully
 */
Std_ReturnType led_turn_off(const led_t *led){
Std_ReturnType ret=E_OK;
    if (NULL==led){
        ret=E_NOT_OK;
    }
    else{
    pin_config_t pin_config={.direction=GPIO_DIRECTION_OUTPUT,
                             .logic=led->status,
                             .pin=led->pin,.port=led->port};
    gpio_Pin_Write_Logic(&pin_config,GPIO_LOW); 
    
}
   return ret;
}
/**
 * 
 * @brief function that toggle led 
 * @param led led pointer to led status @ref led_t
 * @return the status of the function 
 *          (E_OK) the function done successfully
 *           (E_NOT_OK) the function don't done successfully
 */
Std_ReturnType led_turn_toggle( const led_t *led){
Std_ReturnType ret=E_OK;
    if (NULL==led){
        ret=E_NOT_OK;
    }
    else {
    pin_config_t pin_config={.direction=GPIO_DIRECTION_OUTPUT,
                             .logic=led->status,
                             .pin=led->pin,.port=led->port};
    
    gpio_Pin_Toggle_Logic(&pin_config);
    
    }
    return ret;
}