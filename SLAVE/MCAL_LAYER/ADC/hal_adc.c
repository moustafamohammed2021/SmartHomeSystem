/* 
 * File:   hal_adc.c
 * Author: MOUSTAFA
 *
 * Created on March 4, 2023, 12:49 PM
 */

#include "hal_adc.h"

static inline void adc_input_channel_port_select(adc_channel_select_t Channel);
static inline void adc_select_voltageRefernce(const adc_config_t *_adc);
static inline void adc_selectTheResultFormat(const adc_config_t *_adc);
#if ADC_INTERRUPT_FEATURE==INTERRUPT_FREATUERE_ENABLE
static void(*ADC_interruptHandler)(void)=NULL;
#endif

/**
 * @brief initialize  the adc 
 * @description this routine initialize the adc to start the conversion 
 *              it must call before any conversion 
 * @param _adc  pointer to adc @ref adc_config_t
 * @return the status of the function 
 *          (E_OK) the function done successfully
 *           (E_NOT_OK) the function don't done successfully
 */
Std_ReturnType ADC_Init(const adc_config_t *_adc){
    Std_ReturnType ret=E_OK;
        if (NULL==_adc){
            ret=E_NOT_OK;
        }
        else {
            /*disable the adc */
            ADC_CONVERTER_DISABLE();
            /*select the input channel */
            ADCON0bits.CHS=_adc->adc_channel;
            adc_input_channel_port_select(_adc->adc_channel); 
            /*select the F_clock for the conversion of adc  */
            ADCON2bits.ADCS=_adc->adc_clock_freq;
            /*select the acquisition time for conversion*/
            ADCON2bits.ACQT=_adc->adc_time_conversion;
            
            /**interrupt of the adc conversion */
#if ADC_INTERRUPT_FEATURE==INTERRUPT_FREATUERE_ENABLE
    ADC_INTERRUPT_ENABLE();
    ADC_FLAG_INTERRUPT_CLEAR();
#if  INTERRUPT_PRIORITY_LEVELS==INTERRUPT_FREATUERE_ENABLE
    INTERRUPT_PRIORITY_LEVEVLS_ENABLE();
    if (INTERRUPT_HIGH_PRIORITY==_adc->priority){
        INTERRUPT_PRIORITY_HIGH_LEVEVL_ENABLE();
        ADC_INTERRUPT_HIGHY_PRIORITY();
    }
    else if (INTERRUPT_LOW_PRIORITY==_adc->priority){
        INTERRUPT_PRIORITY_LOW_LEVEVL_ENABLE();
        ADC_INTERRUPT_LOW_PRIORITY();
    }
    else {/* nothing */}
#else 
    INTERRUPT_GLOBAL_INTERRUPT_ENABLE();
    INTERRUPT_PERiPHERALS_INTERRUPT_ENABLE();
#endif
    ADC_interruptHandler=_adc->ADC_HandlerInterrupt;
#endif
            /*select the voltage reference */
            adc_select_voltageRefernce(_adc);
            /*select the result format for the adc output */
            adc_selectTheResultFormat(_adc);
            /*enable the adc */
            ADC_CONVERTER_ENABLE();
            ret=E_OK;
        }
    return ret;
}
/**
 *  @brief disable the adc conversion  
 * @param _adc pointer to adc @ref adc_config_t
 * @return the status of the function 
 *          (E_OK) the function done successfully
 *           (E_NOT_OK) the function don't done successfully
 */
Std_ReturnType ADC_DeInit(const adc_config_t *_adc){
     Std_ReturnType ret=E_OK;
        if (NULL==_adc){
            ret=E_NOT_OK;
        }
        else {
            /*disable the adc */
            ADC_CONVERTER_DISABLE();
            ret=E_OK;
        }
    return ret;
}
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
Std_ReturnType ADC_SelectChannel(const adc_config_t *_adc,adc_channel_select_t Channel){
     Std_ReturnType ret=E_OK;
        if (NULL==_adc){
            ret=E_NOT_OK;
        }
        else {
            /*select the input channel */
            ADCON0bits.CHS=Channel;
            adc_input_channel_port_select(Channel); 
            ret=E_OK;
        }
    return ret;
}
/**
 * 
 * @brief this start the conversion of the adc  
 * @preconditions this routine called after the ADC_init() 
 * @param _adc pointer to adc @ref adc_config_t
 * @return the status of the function 
 *          (E_OK) the function done successfully
 *           (E_NOT_OK) the function don't done successfully
 */
Std_ReturnType ADC_StartConversion(const adc_config_t *_adc){
     Std_ReturnType ret=E_OK;
        if (NULL==_adc){
            ret=E_NOT_OK;
        }
        else {
            /**start the conversion of the adc */
            ADC_START_CONVERSION();
            ret=E_OK;
        }
    return ret;
}
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
Std_ReturnType ADC_IsConversionDone(const adc_config_t *_adc , uint8 *conversion_status){
    Std_ReturnType ret=E_OK;
        if ((NULL==_adc) || (conversion_status==NULL)){
            ret=E_NOT_OK;
        }
        else {
            *conversion_status=(uint8)ADC_CONVERSION_STATUS();
        }
    return ret;
}
/**
 * @brief get the conversion result 
 * @precondition this routine called after ADC_Init() and ADC_StartConversion()
 * @param _adc pointer to adc @ref adc_config_t
 * @param conversion_result pointer to the result of the conversion 
 * @return the status of the function 
 *          (E_OK) the function done successfully
 *           (E_NOT_OK) the function don't done successfully
 */
Std_ReturnType ADC_GetResultConversion(const adc_config_t *_adc , adc_result_t *conversion_result){
     Std_ReturnType ret=E_OK;
        if ((NULL==_adc) || (NULL==conversion_result)){
            ret=E_NOT_OK;
        }
        else {
            if (ADC_RIGHT_RESULT_FORMAT==_adc->result_format){
                    *conversion_result= (adc_result_t)(ADRESL+(ADRESH<<8));
        }
        else if (ADC_LEFT_RESULT_FORMAT==_adc->result_format){
           *conversion_result=(adc_result_t)((ADRESL+(ADRESH<<8))>>6);
        }
        else {
          *conversion_result= (adc_result_t)(ADRESL+(ADRESH<<8));
        }
            ret=E_OK;
        }
    return ret;
}
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
                                                , adc_result_t *conversion_result){
        Std_ReturnType ret=E_OK;
        if ((NULL==_adc) || (NULL==conversion_result)){
            ret=E_NOT_OK;
        }
        else {
            /*select channel of the adc */
            ret=ADC_SelectChannel(_adc,Channel);
            /*start the conversion of adc */
            ret=ADC_StartConversion(_adc);
            /*get the result of the adc */
            while (ADCON0bits.GO_nDONE);
            ret=ADC_GetResultConversion(_adc,conversion_result);
        }
    return ret;
}

#if ADC_INTERRUPT_FEATURE==INTERRUPT_FREATUERE_ENABLE
/**
 * @brief get the conversion result  but by using interrupt 
 * @precondition this routine called after ADC_Init() and ADC_StartConversion()
 * @param _adc pointer to adc @ref adc_config_t
 * @param Channel  Channel the channel that you want to select 
 * @return the status of the function 
 *          (E_OK) the function done successfully
 *           (E_NOT_OK) the function don't done successfully
 */
Std_ReturnType ADC_GetResultConversionInterrupt(const adc_config_t *_adc , adc_channel_select_t Channel){
    Std_ReturnType ret=E_OK;
        if (NULL==_adc){
            ret=E_NOT_OK;
        }
        else {
            /*clear the flag */
            ADC_FLAG_INTERRUPT_CLEAR();
            /*select channel of the adc */
            ret=ADC_SelectChannel(_adc,Channel);
            /*start the conversion of adc */
            ret=ADC_StartConversion(_adc);
        }
    return ret; 
}
#endif
/**
 * 
 * @param Channel Channel the channel that you want to select 
 */
static inline void adc_input_channel_port_select(adc_channel_select_t Channel){
    switch (Channel){
        case ADC_CHANNEL_AN0: SET_BIT(TRISA,_TRISA_TRISA0_POSN);break;
        case ADC_CHANNEL_AN1: SET_BIT(TRISA,_TRISA_TRISA1_POSN);break;
        case ADC_CHANNEL_AN2: SET_BIT(TRISA,_TRISA_TRISA2_POSN);break;
        case ADC_CHANNEL_AN3: SET_BIT(TRISA,_TRISA_TRISA3_POSN);break;
        case ADC_CHANNEL_AN4: SET_BIT(TRISA,_TRISA_TRISA5_POSN);break;
        case ADC_CHANNEL_AN5: SET_BIT(TRISE,_TRISE_TRISE0_POSN);break;
        case ADC_CHANNEL_AN6: SET_BIT(TRISE,_TRISE_TRISE1_POSN);break;
        case ADC_CHANNEL_AN7: SET_BIT(TRISE,_TRISE_TRISE2_POSN);break;
        case ADC_CHANNEL_AN8: SET_BIT(TRISB,_TRISB_TRISB2_POSN);break;
        case ADC_CHANNEL_AN9: SET_BIT(TRISB,_TRISB_TRISB3_POSN);break;
        case ADC_CHANNEL_AN10: SET_BIT(TRISB,_TRISB_TRISB1_POSN);break;
        case ADC_CHANNEL_AN11: SET_BIT(TRISB,_TRISB_TRISB4_POSN);break;
        case ADC_CHANNEL_AN12: SET_BIT(TRISB,_TRISB_TRISB0_POSN);break;
        default : /*nothing */;
    }
    
}
/**
 * 
 * @param _adc  pointer to adc @ref adc_config_t
 */
static inline void adc_select_voltageRefernce(const adc_config_t *_adc){
    if (ADC_VOLTAGE_REFERNCE_ENABLE_BIT==_adc->volatge_reference){
        ADC_VOLTAGE_REFERNCE_ENANBLE();
    }
    else if (ADC_VOLTAGE_REFERNCE_DISABLE_BIT==_adc->volatge_reference){
        ADC_VOLTAGE_REFERNCE_DISABLE();
    }
    else {
        ADC_VOLTAGE_REFERNCE_DISABLE();
    }
}
/**
 * 
 * @param _adc  pointer to adc @ref adc_config_t
 */
static inline void adc_selectTheResultFormat(const adc_config_t *_adc){
    if (ADC_RIGHT_RESULT_FORMAT==_adc->result_format){
        ADC_RESULT_RIGHT_FORMAT();
    }
    else if (ADC_LEFT_RESULT_FORMAT==_adc->result_format){
        ADC_RESULT_LEFT_FORMAT();
    }
    else {
        ADC_RESULT_RIGHT_FORMAT();
    }
}
#if ADC_INTERRUPT_FEATURE==INTERRUPT_FREATUERE_ENABLE
void ADC_ISR(void){
    /*clear the flag */
    ADC_FLAG_INTERRUPT_CLEAR();
    if (ADC_interruptHandler){
        ADC_interruptHandler();
    }
}
#endif