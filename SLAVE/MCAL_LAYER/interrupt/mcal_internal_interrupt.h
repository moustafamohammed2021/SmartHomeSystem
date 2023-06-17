/* 
 * File:   mcal_internal_interrupt.h
 * Author: MOUSTAFA
 *
 * Created on February 28, 2023, 5:29 AM
 */

#ifndef MCAL_INTERNAL_INTERRUPT_H
#define	MCAL_INTERNAL_INTERRUPT_H
/* Section: Includes */
#include "mcal_interrupt_config.h"
#include "mcal_interrupt_gen_config.h"

/* Section : Macros Declarations  */

/* Section : macros Function Declarations */
/* Section : macros Function Declarations */
#if ADC_INTERRUPT_FEATURE==INTERRUPT_FREATUERE_ENABLE
/*macro enable the ADC_internal interrupt enable*/
#define ADC_INTERRUPT_ENABLE()       (PIE1bits.ADIE=1)
/*macro enable the ADC_internal interrupt enable*/
#define ADC_INTERRUPT_DISABLE()      (PIE1bits.ADIE=0)
/* macro that clear the ADC_internal flag*/
#define ADC_FLAG_INTERRUPT_CLEAR()   (PIR1bits.ADIF=0)

#if  INTERRUPT_PRIORITY_LEVELS==INTERRUPT_FREATUERE_ENABLE
/*macro make the ADC_internal high priority  */
#define ADC_INTERRUPT_HIGHY_PRIORITY()  (IPR1bits.ADIP=1)
/*macro make the ADC_internal low priority  */
#define ADC_INTERRUPT_LOW_PRIORITY()    (IPR1bits.ADIP=0)
#endif
#endif
#if TIMER0_INTERRUPT_FEATURE==INTERRUPT_FREATUERE_ENABLE
    /*macro enable the TIMER0_internal interrupt enable*/
#define TIMER0_INTERRUPT_ENABLE()       (INTCONbits.TMR0IE=1)
/*macro enable the TIMER0_internal interrupt enable*/
#define TIMER0_INTERRUPT_DISABLE()      (INTCONbits.TMR0IE=0)
/* macro that clear the TIMER0_internal flag*/
#define TIMER0_FLAG_INTERRUPT_CLEAR()   (INTCONbits.TMR0IF=0)
#if  INTERRUPT_PRIORITY_LEVELS==INTERRUPT_FREATUERE_ENABLE
    /*macro make the TIMER0_internal high priority  */
#define TIMER0_INTERRUPT_HIGHY_PRIORITY()  (INTCON2bits.TMR0IP=1)
/*macro make the TIMER0_internal low priority  */
#define TIMER0_INTERRUPT_LOW_PRIORITY()    (INTCON2bits.TMR0IP=0)
#endif
#endif 

#if TIMER1_INTERRUPT_FEATURE==INTERRUPT_FREATUERE_ENABLE
    /*macro enable the TIMER1_internal interrupt enable*/
#define TIMER1_INTERRUPT_ENABLE()       (PIE1bits.TMR1IE=1)
/*macro enable the TIMER1_internal interrupt enable*/
#define TIMER1_INTERRUPT_DISABLE()      (PIE1bits.TMR1IE=0)
/* macro that clear the TIMER1_internal flag*/
#define TIMER1_FLAG_INTERRUPT_CLEAR()   (PIR1bits.TMR1IF=0)
#if  INTERRUPT_PRIORITY_LEVELS==INTERRUPT_FREATUERE_ENABLE
    /*macro make the TIMER1_internal high priority  */
#define TIMER1_INTERRUPT_HIGHY_PRIORITY()  (IPR1bits.TMR1IP=1)
/*macro make the TIMER1_internal low priority  */
#define TIMER1_INTERRUPT_LOW_PRIORITY()    (IPR1bits.TMR1IP=0)
#endif
#endif 

#if SPI_INTERRUPT_FEATURE==INTERRUPT_FREATUERE_ENABLE
    /*macro enable the SPI_internal interrupt enable*/
#define SPI_INTERRUPT_ENABLE()       (PIE1bits.SSPIE=1)
/*macro enable the SPI_internal interrupt enable*/
#define SPI_INTERRUPT_DISABLE()      (PIE1bits.SSPIE=0)
/* macro that clear the SPI_internal flag*/
#define SPI_FLAG_INTERRUPT_CLEAR()   (PIR1bits.SSPIF=0)
#if  INTERRUPT_PRIORITY_LEVELS==INTERRUPT_FREATUERE_ENABLE
    /*macro make the SPI_internal high priority  */
#define SPI_INTERRUPT_HIGHY_PRIORITY()  (IPR1bits.SSPIP=1)
/*macro make the SPI_internal low priority  */
#define SPI_INTERRUPT_LOW_PRIORITY()    (IPR1bits.SSPIP=0)
#endif
#endif

/* Data types Declarations */

/* Function Declarations*/

#endif	/* MCAL_INTERNAL_INTERRUPT_H */

