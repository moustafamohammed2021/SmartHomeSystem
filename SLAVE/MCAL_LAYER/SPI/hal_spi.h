/* 
 * File:   hal_spi.h
 * Author: MOUSTAFA
 *
 * Created on March 16, 2023, 6:04 AM
 */

#ifndef HAL_SPI_H
#define	HAL_SPI_H
/* **************** Section: Includes **************** */
#include "hal_spi_cfg.h"
#include "../GPIO/hal_gpio.h"
#include "../mcal_std_types.h"
#include <./pic18f4620.h>
#include "../interrupt/mcal_internal_interrupt.h"
/**************** Section : Macros Declarations*****************/
/* configure enable of the interrupt */
#define SPI_INTERRUPT_ENABLE_CFG  0x01
#define SPI_INTERRUPT_DISABLE_CFG 0x00
/* select the transmitting mode clock phase for bit  CKE*/
#define SPI_MASTER_MODE_TRANSMITING_ACTIVE_TO_IDLE_CFG   0x01
#define SPI_MASTER_MODE_TRANSMITING_IDLE_TO_ACTIVE_CFG   0x00
/* select the receiving mode  for bit in SMP FOR master mode or slave mode */
#define SPI_MASTER_MODE_INPUT_DATA_SAMPLED_AT_END_CFG         0x01
#define SPI_MASTER_MODE_INPUT_DATA_SAMPLED_AT_MIDDLE_CFG      0x00
/* select the clock polarity in bit CKP*/
#define SPI_MASTER_MODE_IDLE_CLOCK_STATE_HIGH_CFG  0x01
#define SPI_MASTER_MODE_IDLE_CLOCK_STATE_LOW_CFG   0x00
/* enable or disable the SPI BIT */
#define SPI_ENABLE_MODEULE_CFG    0x01
#define SPI_DISABLE_MODEULE_CFG    0x00
/*  to detect Receive Overflow Indicator bit for slave mode this bit is SSPOV  (cleared by software)*/
#define SPI_SLAVE_MODE_OVERFLOW_DETECT_CFG     0x01
#define SPI_SLAVE_MODE_OVERFLOW_NOT_HAPPEN_CFG 0x00
/* to detect the write collision bit for master mode  for slave or master mode (cleared by software)*/
#define SPI_COLLISION_DETECT_CFG       0x01
#define SPI_COLLISION_NOT_HAPPEN_CFG   0x00


/**************** Section : macros Function Declarations************* */
/* recieve mode detect the buffer is full or not the receiving is done */
#define SPI_SLAVE_MODE_SSPBUF_IS_FULL_OR_NOT()     (SSPSTATbits.BF)
/* select the transmitting mode for bit  CKE*/
#define SPI_MASTER_MODE_TRANSMITING_ACTIVE_TO_IDLE()   (SSPSTATbits.CKE=1)
#define SPI_MASTER_MODE_TRANSMITING_IDLE_TO_ACTIVE()   (SSPSTATbits.CKE=0)
/* select the receiving mode  for bit in SMP FOR master mode or slave mode */
#define SPI_MASTER_MODE_INPUT_DATA_SAMPLED_AT_END()     (SSPSTATbits.SMP=1)
#define SPI_MASTER_MODE_INPUT_DATA_SAMPLED_AT_MIDDLE()  (SSPSTATbits.SMP=0)    
/* select the mode of the spi master or slave */
#define SPI_SELECT_MODE(_CONFIG)   (SSPCON1bits.SSPM=_CONFIG)

/* select the clock polarity in bit CKP*/
#define SPI_MASTER_MODE_IDLE_CLOCK_STATE_HIGH()   (SSPCON1bits.CKP=1)
#define SPI_MASTER_MODE_IDLE_CLOCK_STATE_LOW()    (SSPCON1bits.CKP=0)
/* enable or disable the SPI BIT */
#define SPI_ENABLE_MODEULE()     (SSPCON1bits.SSPEN=1)
#define SPI_DISABLE_MODEULE()    (SSPCON1bits.SSPEN=0)
/***************** Data types Declarations *****************/
typedef enum {
    SPI_MASTER_MODE_CLOCK_FOSC_DIV_4=0,
    SPI_MASTER_MODE_CLOCK_FOSC_DIV_16,
    SPI_MASTER_MODE_CLOCK_FOSC_DIV_64,
    SPI_MASTER_MODE_CLOCK_TIMR2_DIV_2,
    SPI_SLAVE_MODE_SS_BIN_ENABLED,
    SPI_SLAVE_MODE_SS_BIN_DISABLED         
}SPI_mode_t;

typedef struct {
    uint8 clock_polarity:1;
    uint8 clock_phase:1;
    uint8 sampling_receiving:1;
    uint8 SPI_interrupt_enable:1;
    uint8 reserved_bits:4;
}SPI_Control_cfg_t;

typedef struct {
#if SPI_INTERRUPT_FEATURE==INTERRUPT_FREATUERE_ENABLE
    void (*SPI_InterruptHandler)(void);
#if  INTERRUPT_PRIORITY_LEVELS==INTERRUPT_FREATUERE_ENABLE
    interrupt_priority_t priority;
#endif
#endif
    uint8 SPI_mode;
    SPI_Control_cfg_t SPI_config;
}SPI_Config_t;
/***************** Function Declarations*****************/
Std_ReturnType SPI_Init(const SPI_Config_t *_spi_obj);
Std_ReturnType SPI_DeInit(const SPI_Config_t *_spi_obj);

Std_ReturnType SPI_SendByteBlocking(uint8 M_Data);
Std_ReturnType SPI_ReadByteBlocking(uint8 *M_Data);

Std_ReturnType SPI_SendByteNonBlocking(uint8 M_Data);
Std_ReturnType SPI_ReadByteNonBlocking(uint8 *M_Data);
/***Receive And Transmit At The Same Time*/
void u8SPITranmitRecieve(uint8 u8Data);
#endif	/* HAL_SPI_H */

