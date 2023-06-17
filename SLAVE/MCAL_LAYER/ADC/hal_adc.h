/* 
 * File:   hal_adc.h
 * Author: MOUSTAFA
 *
 * Created on March 4, 2023, 12:49 PM
 */

#ifndef HAL_ADC_H
#define	HAL_ADC_H
/* Section: Includes */
#include "hal_adc_config.h"
#include "../GPIO/hal_gpio.h"
#include "../mcal_std_types.h"
#include <./pic18f4620.h>
#include "../interrupt/mcal_internal_interrupt.h"

/* Section : Macros Declarations  */

/*
 * @brief macros that configure the bit that analog or digital 
 * @note ADC_AN4_ANALOG_CONFIGURATION that mean AN4 ,AN3,AN2,AN1,AN0 
 *       is analog bits functionality 
 */
#define ADC_AN0_ANALOG_CONFIGURATION   0x0E
#define ADC_AN1_ANALOG_CONFIGURATION   0x0D
#define ADC_AN2_ANALOG_CONFIGURATION   0x0C
#define ADC_AN3_ANALOG_CONFIGURATION   0x0B
#define ADC_AN4_ANALOG_CONFIGURATION   0x0A
#define ADC_AN5_ANALOG_CONFIGURATION   0x09
#define ADC_AN6_ANALOG_CONFIGURATION   0x08
#define ADC_AN7_ANALOG_CONFIGURATION   0x07
#define ADC_AN8_ANALOG_CONFIGURATION   0x06
#define ADC_AN9_ANALOG_CONFIGURATION   0x05
#define ADC_AN10_ANALOG_CONFIGURATION  0x04
#define ADC_AN11_ANALOG_CONFIGURATION  0x03
#define ADC_AN12_ANALOG_CONFIGURATION  0x02
#define ADC_ALL_ANALOG_CONFIGURATION   0x00
#define ADC_ALL_DIGITAL_CONFIGURATION  0x0F

#define ADC_VOLTAGE_REFERNCE_ENABLE_BIT     0x01U
#define ADC_VOLTAGE_REFERNCE_DISABLE_BIT    0x00U

#define ADC_RIGHT_RESULT_FORMAT         0x01U
#define ADC_LEFT_RESULT_FORMAT          0x00U


/* Section : macros Function Declarations */
/*
 * @brief macros that configure the bit that analog or digital 
 * @note ADC_AN4_ANALOG_CONFIGURATION that mean AN4 ,AN3,AN2,AN1,AN0 
 *       is analog bits functionality 
 */
#define ADC_ANALOG_DIGITAL_PORT_CONFIG(_CONFIG)       (ADCON1bits.PCFG=_CONFIG)

/* know the conversion status if it reach the idle state */
#define ADC_CONVERSION_STATUS()    (ADCON0bits.GO_nDONE)
/* START the conversion of the adc */
#define ADC_START_CONVERSION()     (ADCON0bits.GODONE=1)
/*
 * @brief two function enable the adc 
 *  and disable the adc 
 * @note ADC_CONVERTER_ENABLE() enable the conversion 
 * %note ADC_CONVERTER_DISABLE() disable the conversion 
 */
#define ADC_CONVERTER_ENABLE()     (ADCON0bits.ADON=1)
#define ADC_CONVERTER_DISABLE()    (ADCON0bits.ADON=0)

/*
 *@brief voltage reference configration 
 * @note  ADC_VOLTAGE_REFERNCE_ENANBLE : voltage reference is vref- & vref+
 *        ADC_VOLTAGE_REFERNCE_ENANBLE : voltage reference is Vdd & Vss
 */
#define ADC_VOLTAGE_REFERNCE_ENANBLE()     do{ADCON1bits.VCFG0=1;\
                                              ADCON1bits.VCFG1=1;\
                                              }while(0)
#define ADC_VOLTAGE_REFERNCE_DISABLE()     do{ADCON1bits.VCFG0=0;\
                                              ADCON1bits.VCFG1=0;\
                                              }while(0)

/*
 * @brief A\D the result formate select  
 */
#define ADC_RESULT_RIGHT_FORMAT()     (ADCON2bits.ADFM=1)
#define ADC_RESULT_LEFT_FORMAT()      (ADCON2bits.ADFM=0)
/* Data types Declarations */


/*
 * @brief channel pin to adc select 
 */
typedef enum {
    ADC_CHANNEL_AN0=0,
    ADC_CHANNEL_AN1,
    ADC_CHANNEL_AN2,
    ADC_CHANNEL_AN3,
    ADC_CHANNEL_AN4,
    ADC_CHANNEL_AN5,
    ADC_CHANNEL_AN6,
    ADC_CHANNEL_AN7,
    ADC_CHANNEL_AN8,
    ADC_CHANNEL_AN9,
    ADC_CHANNEL_AN10,
    ADC_CHANNEL_AN11,
    ADC_CHANNEL_AN12         
}adc_channel_select_t;

/*
 * @brief the time that adc takes to conversion 
 * @NOTE time takes to sampling and holding the input voltage
 */
typedef enum {
    ADC_0_TAD=0,
    ADC_2_TAD,
    ADC_4_TAD,
    ADC_6_TAD,
    ADC_8_TAD,
    ADC_12_TAD,
    ADC_16_TAD,        
    ADC_20_TAD,        
}acquisition_time_t;

/*
 *@brief clock that select from the oscillator 
 * @note that to determine acquisition time 
 */
typedef enum {
    ADC_CONVERSION_CLOCK_FOSC_DIV_2=0,
    ADC_CONVERSION_CLOCK_FOSC_DIV_8,
    ADC_CONVERSION_CLOCK_FOSC_DIV_32,
    ADC_CONVERSION_CLOCK_FOSC_DIV_FRC,
    ADC_CONVERSION_CLOCK_FOSC_DIV_4,
    ADC_CONVERSION_CLOCK_FOSC_DIV_16,
    ADC_CONVERSION_CLOCK_FOSC_DIV_64        
}adc_conversion_clock_select_t;


/* adc configuration */
typedef struct {
#if ADC_INTERRUPT_FEATURE==INTERRUPT_FREATUERE_ENABLE
    void(*ADC_HandlerInterrupt)(void);
#if  INTERRUPT_PRIORITY_LEVELS==INTERRUPT_FREATUERE_ENABLE
    interrupt_priority_t priority;
#endif
#endif
    adc_channel_select_t adc_channel;    /* @ref adc_channel_select_t*/ 
    acquisition_time_t   adc_time_conversion;   /* @ref acquisition_time_t*/
    adc_conversion_clock_select_t  adc_clock_freq; /* @ref adc_conversion_clock_select_t */
    uint8 volatge_reference :1;        /* voltage reference configuration */
    uint8 result_format     :1;         /* result format configuration */
    uint8 pins_reserved     :6;          /* reserved pins */ 
}adc_config_t;


typedef uint16 adc_result_t;
/* Function Declarations*/
/**
 * @brief initialize  the adc 
 * @description this routine initialize the adc to start the conversion 
 *              it must call before any conversion 
 * @param _adc  pointer to adc @ref adc_config_t
 * @return the status of the function 
 *          (E_OK) the function done successfully
 *           (E_NOT_OK) the function don't done successfully
 */
Std_ReturnType ADC_Init(const adc_config_t *_adc);
/**
 *  @brief disable the adc conversion  
 * @param _adc pointer to adc @ref adc_config_t
 * @return the status of the function 
 *          (E_OK) the function done successfully
 *           (E_NOT_OK) the function don't done successfully
 */
Std_ReturnType ADC_DeInit(const adc_config_t *_adc);
/**
 * 
 * @brief this routine select the channel to be selected  
 * @preconditions this routine called after the ADC_init();
 * @param _adc pointer to adc @ref adc_config_t
 * @param Channel the channel that you want to select 
 * @return the status of the function 
 *          (E_OK) the function done successfully
 *           (E_NOT_OK) the function don't done successfully
 */
Std_ReturnType ADC_SelectChannel(const adc_config_t *_adc,adc_channel_select_t Channel);
/**
 * 
 * @brief this start the conversion of the adc  
 * @preconditions this routine called after the ADC_init() 
 * @param _adc pointer to adc @ref adc_config_t
 * @return the status of the function 
 *          (E_OK) the function done successfully
 *           (E_NOT_OK) the function don't done successfully
 */
Std_ReturnType ADC_StartConversion(const adc_config_t *_adc);
/**
 * 
 * @brief this routine check the conversion is dine or not 
 * @precondition this routine called after ADC_Init() and ADC_StartConversion()
 * @param _adc pointer to adc @ref adc_config_t  
 * @param conversion_status  conversion is done or not if (true) conversion done else (false) conversion not done
 * @return the status of the function 
 *          (E_OK) the function done successfully
 *           (E_NOT_OK) the function don't done successfully
 */
Std_ReturnType ADC_IsConversionDone(const adc_config_t *_adc , uint8 *conversion_status);
/**
 * @brief get the conversion result 
 * @precondition this routine called after ADC_Init() and ADC_StartConversion()
 * @param _adc pointer to adc @ref adc_config_t
 * @param conversion_result pointer to the result of the conversion 
 * @return the status of the function 
 *          (E_OK) the function done successfully
 *           (E_NOT_OK) the function don't done successfully
 */
Std_ReturnType ADC_GetResultConversion(const adc_config_t *_adc , adc_result_t *conversion_result);
/**
 * @brief get the conversion result 
 * @precondition this routine called after ADC_Init() and ADC_StartConversion()
 * @param _adc pointer to adc @ref adc_config_t
 * @param Channel  Channel the channel that you want to select 
 * @param conversion_result
 * @return the status of the function 
 *          (E_OK) the function done successfully
 *           (E_NOT_OK) the function don't done successfully
 */
Std_ReturnType ADC_GetResultConversionBlocking(const adc_config_t *_adc , adc_channel_select_t Channel
                                                , adc_result_t *conversion_result);
/**
 * @brief get the conversion result  but by using interrupt 
 * @precondition this routine called after ADC_Init() and ADC_StartConversion()
 * @param _adc pointer to adc @ref adc_config_t
 * @param Channel  Channel the channel that you want to select 
 * @return the status of the function 
 *          (E_OK) the function done successfully
 *           (E_NOT_OK) the function don't done successfully
 */
Std_ReturnType ADC_GetResultConversionInterrupt(const adc_config_t *_adc , adc_channel_select_t Channel);
#endif	/* HAL_ADC_H */

