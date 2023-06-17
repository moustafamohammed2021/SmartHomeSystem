/* 
 * File:   hal_spi.c
 * Author: MOUSTAFA
 *
 * Created on March 16, 2023, 6:04 AM
 */

#include "hal_spi.h"

#if SPI_INTERRUPT_FEATURE==INTERRUPT_FREATUERE_ENABLE 
static void (*HandelerSPI)(void)=NULL;  
#endif
static inline void SPI_Select_mode_and_pin_initialize(const SPI_Config_t *_spi_obj);
static inline void SPI_Select_the_clock_polarity(const SPI_Config_t *_spi_obj);
static inline void SPI_Select_the_clock_phase(const SPI_Config_t *_spi_obj);
static inline void SPI_Select_the_Sampling_receiving(const SPI_Config_t *_spi_obj);
static inline void SPI_configure_the_interrupt(const SPI_Config_t *_spi_obj);

Std_ReturnType SPI_Init(const SPI_Config_t *_spi_obj){
     Std_ReturnType ret=E_OK;
        if (NULL==_spi_obj){
            ret=E_NOT_OK;
        }
        else {
             /* disable the SPI */
            SPI_DISABLE_MODEULE();
            /* select the mode of the SPI and configure the pins  */
            SPI_Select_mode_and_pin_initialize(_spi_obj);
            SPI_SELECT_MODE(_spi_obj->SPI_mode);
            /* configure the clock polarity  */
           SPI_Select_the_clock_polarity(_spi_obj);
            /*configure the clock phase  */
            SPI_Select_the_clock_phase(_spi_obj);
            /* configure the sampling receiving */
            SPI_Select_the_Sampling_receiving(_spi_obj);
            /* configure the interrupt of the spi */
            SPI_configure_the_interrupt(_spi_obj);
            /* enable the SPI*/
            SPI_ENABLE_MODEULE();
            ret=E_OK;
        }
     return ret;
}
Std_ReturnType SPI_DeInit(const SPI_Config_t *_spi_obj){
    Std_ReturnType ret=E_OK;
        if (NULL==_spi_obj){
            ret=E_NOT_OK;
        }
        else {
            SPI_DISABLE_MODEULE();
#if SPI_INTERRUPT_FEATURE==INTERRUPT_FREATUERE_ENABLE            
            SPI_INTERRUPT_DISABLE();
#endif            
            ret=E_OK;
        }
     return ret;
    
}

Std_ReturnType SPI_SendByteBlocking(uint8 M_Data){
    Std_ReturnType ret=E_OK;
        
          SSPBUF=M_Data;
          while(PIR1bits.SSPIF==1);
          PIR1bits.SSPIF=0;
            ret=E_OK;
        
     return ret;
    
}
Std_ReturnType SPI_ReadByteBlocking(uint8 *M_Data){
    Std_ReturnType ret=E_OK;
        if (NULL==M_Data){
            ret=E_NOT_OK;
        }
        else {
            while(SSPSTATbits.BF==1);
            *M_Data=SSPBUF;
            ret=E_OK;
        }
     return ret;
    
}

Std_ReturnType SPI_SendByteNonBlocking(uint8 M_Data){
    Std_ReturnType ret=E_OK;
        
          if (!SSPCON1bits.WCOL){
               SSPBUF=M_Data;
               PIR1bits.SSPIF=0;
          }
          
            ret=E_OK;
        
     return ret;
    
}
Std_ReturnType SPI_ReadByteNonBlocking(uint8 *M_Data){
   Std_ReturnType ret=E_OK;
        if (NULL==M_Data){
            ret=E_NOT_OK;
        }
        else {
            if (0==SSPSTATbits.BF){
                *M_Data=SSPBUF;
                 PIR1bits.SSPIF=0;
            }
            ret=E_OK;
        }
     return ret;
}
static inline void SPI_Select_mode_and_pin_initialize(const SPI_Config_t *_spi_obj){
    Std_ReturnType ret=E_NOT_OK;
    if ((SPI_MASTER_MODE_CLOCK_FOSC_DIV_4==_spi_obj->SPI_mode) || 
        (SPI_MASTER_MODE_CLOCK_FOSC_DIV_16==_spi_obj->SPI_mode) || 
        (SPI_MASTER_MODE_CLOCK_FOSC_DIV_64==_spi_obj->SPI_mode) ||
        (SPI_MASTER_MODE_CLOCK_TIMR2_DIV_2==_spi_obj->SPI_mode)){
        pin_config_t M_MOSI={.port=PORTC_INDEX,.pin=GPIO_PIN5,.direction=GPIO_DIRECTION_OUTPUT};
         pin_config_t M_MISO={.port=PORTC_INDEX,.pin=GPIO_PIN4,.direction=GPIO_DIRECTION_INPUT};
        pin_config_t M_CLOCK={.port=PORTC_INDEX,.pin=GPIO_PIN3,.direction=GPIO_DIRECTION_OUTPUT};
        
       ret= gpio_Pin_Intialize(&M_MOSI);
       ret=gpio_Pin_Intialize(&M_CLOCK);
       ret=gpio_Pin_Intialize(&M_MISO);
    }
    else if (SPI_SLAVE_MODE_SS_BIN_ENABLED==_spi_obj->SPI_mode){
        pin_config_t S_MOSI={.port=PORTC_INDEX,.pin=GPIO_PIN5,.direction=GPIO_DIRECTION_OUTPUT};
        pin_config_t S_MISO={.port=PORTC_INDEX,.pin=GPIO_PIN4,.direction=GPIO_DIRECTION_INPUT};
        pin_config_t S_CLOCK={.port=PORTC_INDEX,.pin=GPIO_PIN3,.direction=GPIO_DIRECTION_INPUT};
        pin_config_t SS={.port=PORTA_INDEX,.pin=GPIO_PIN5,.direction=GPIO_DIRECTION_INPUT};
        ret= gpio_Pin_Intialize(&S_MOSI);
        ret= gpio_Pin_Intialize(&S_MISO);
        ret= gpio_Pin_Intialize(&S_CLOCK);
        ret= gpio_Pin_Intialize(&SS);
        SPI_MASTER_MODE_INPUT_DATA_SAMPLED_AT_MIDDLE();
    }
}
static inline void SPI_Select_the_clock_polarity(const SPI_Config_t *_spi_obj){
    if (SPI_MASTER_MODE_IDLE_CLOCK_STATE_HIGH_CFG==_spi_obj->SPI_config.clock_polarity){
        SPI_MASTER_MODE_IDLE_CLOCK_STATE_HIGH();
    }
    else if (SPI_MASTER_MODE_IDLE_CLOCK_STATE_LOW_CFG==_spi_obj->SPI_config.clock_polarity){
        SPI_MASTER_MODE_IDLE_CLOCK_STATE_LOW();
    }
}

static inline void SPI_Select_the_clock_phase(const SPI_Config_t *_spi_obj){
    if(SPI_MASTER_MODE_TRANSMITING_ACTIVE_TO_IDLE_CFG==_spi_obj->SPI_config.clock_phase){
        SPI_MASTER_MODE_TRANSMITING_ACTIVE_TO_IDLE();
    }
    else if(SPI_MASTER_MODE_TRANSMITING_IDLE_TO_ACTIVE_CFG==_spi_obj->SPI_config.clock_phase){
        SPI_MASTER_MODE_TRANSMITING_IDLE_TO_ACTIVE();
    }
}
static inline void SPI_Select_the_Sampling_receiving(const SPI_Config_t *_spi_obj){
    if (SPI_MASTER_MODE_INPUT_DATA_SAMPLED_AT_END_CFG==_spi_obj->SPI_config.sampling_receiving){
        SPI_MASTER_MODE_INPUT_DATA_SAMPLED_AT_END();
    }
    else  if (SPI_MASTER_MODE_INPUT_DATA_SAMPLED_AT_MIDDLE_CFG==_spi_obj->SPI_config.sampling_receiving){
        SPI_MASTER_MODE_INPUT_DATA_SAMPLED_AT_MIDDLE();
    }
}
static inline void SPI_configure_the_interrupt(const SPI_Config_t *_spi_obj){
    if (SPI_INTERRUPT_ENABLE_CFG==_spi_obj->SPI_config.SPI_interrupt_enable){
#if SPI_INTERRUPT_FEATURE==INTERRUPT_FREATUERE_ENABLE
       SPI_INTERRUPT_ENABLE();
       SPI_FLAG_INTERRUPT_CLEAR();
#if  INTERRUPT_PRIORITY_LEVELS==INTERRUPT_FREATUERE_ENABLE
            INTERRUPT_PRIORITY_LEVEVLS_ENABLE();
     if (INTERRUPT_HIGH_PRIORITY==_spi_obj->priority){
        INTERRUPT_PRIORITY_HIGH_LEVEVL_ENABLE();
             SPI_INTERRUPT_HIGHY_PRIORITY();
          }
    else if (INTERRUPT_LOW_PRIORITY==_spi_obj->priority){
        INTERRUPT_PRIORITY_LOW_LEVEVL_ENABLE();
         SPI_INTERRUPT_LOW_PRIORITY();
         }
    else {/* nothing */}
#else 
            INTERRUPT_GLOBAL_INTERRUPT_ENABLE();
            INTERRUPT_PERiPHERALS_INTERRUPT_ENABLE();
#endif
           HandelerSPI =_spi_obj->SPI_InterruptHandler;
#endif 
    }
    else if (SPI_INTERRUPT_DISABLE_CFG==_spi_obj->SPI_config.SPI_interrupt_enable){
        SPI_INTERRUPT_DISABLE();
    }
       
}

#if SPI_INTERRUPT_FEATURE==INTERRUPT_FREATUERE_ENABLE
void SPI_ISR(void){
     SPI_FLAG_INTERRUPT_CLEAR();
    if (HandelerSPI){
        HandelerSPI();
    }
}
#endif

/*This Function Will Transmit And Receive At The Same Time*/
uint8 u8SPITranmitRecieve(uint8 u8Data){
    uint8 LOC_DataReceived=0XFF;//Initialize The Data The Will Be Received
    SPI_SendByteBlocking(u8Data);//Send Data by The SPI Protocol 
    SPI_ReadByteBlocking(&LOC_DataReceived);//Receive The Data By The SPI Protocol
    return LOC_DataReceived;//Return The Received Data By The Spi
}