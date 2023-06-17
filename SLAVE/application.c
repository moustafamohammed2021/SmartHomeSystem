/* 
 * File:   application.c
 * Author: MOUSTAFA
 *
 * Created on June 10, 2023, 10:54 PM
 */

#include "application.h"
/*
 * 
 */

volatile uint16 u16RequiredTemperature=24;//Set The Default Temperature of The AIR Conditioning
volatile uint16 u16ReadingCounter=0;//Counter For Reading in The Interrupt Of The Timer1
volatile uint16 u16TempSensorReading=0;//The Reading Value of The Temperature Sensor 
volatile uint16 u16LastValueAirCondtioning=AIR_CONDITIONING_OFF;//The Status of The Air Conditioning Led

volatile uint8 u8Request=DEFAULT_ACK;//This Request From The Master To Control The Rooms And Devices 
volatile uint8 u8Response=DEFAULT_ACK;//This Response To Tell The Master if The Device is ON Or OFF 
int main(void){
    ADCON1bits.PCFG=1110;//Make PA0 Analog Only And The Other Channels of The ADC is Used As Digital I/o Pins
    ADC_Init(&ADC_Obj);//Initialize The ADC Converter 
    Timer1_Init(&Timer1Obj);//Initialize The Timer1 Module 
    SPI_Init(&SPIObject);//Initialize The SPI Module
    led_initialize(&Room1_Led);//Initialize Room1 Led
    led_initialize(&Room2_Led);//Initialize Room2 Led 
    led_initialize(&Room3_Led);//Initialize Room3 Led 
    led_initialize(&Room4_Led);//Initialize Room4 Led
    led_initialize(&TV_Led);//Initialize TV Led 
    led_initialize(&AIR_COND_Led);//Initialize Air Conditioning Led 
    while(1){
        u8SPITranmitRecieve(u8Response);//Receive Data From The Master 
        switch(u8Request){//Switch Cases To Send The Status Of The Device That The Master Will Request
            /**************************************Status Commands *******************************/
            /*************************************Send The Status Value To The Master*************/
            case ROOM1_STATUS://In Case Room1 Request 
                if((READ_BIT(PORTD,0))==1){//if The Led Is ON
                    u8Response=ON_STATUS;//Set The Response ON
                }
                else if((READ_BIT(PORTD,0))==0){//if The Led Is off
                    u8Response=OFF_STATUS;//Set The Response off
                }
                else {}
                //u8SPITranmitRecieve(u8Response);//Send The Status of The Led 
                break;
            case ROOM2_STATUS://In Case Room2 Request 
                if((READ_BIT(PORTD,1))==1){//if The Led Is ON
                    u8Response=ON_STATUS;//Set The Response ON
                }
                else if((READ_BIT(PORTD,1))==0){//if The Led Is off
                    u8Response=OFF_STATUS;//Set The Response off
                }
                else {}
                //u8SPITranmitRecieve(u8Response);//Send The Status of The Led 
                break;
            case ROOM3_STATUS://In Case Room3 Request 
                if((READ_BIT(PORTD,2))==1){//if The Led Is ON
                    u8Response=ON_STATUS;//Set The Response ON
                }
                else if((READ_BIT(PORTD,2))==0){//if The Led Is off
                    u8Response=OFF_STATUS;//Set The Response off
                }
                else {}
                //u8SPITranmitRecieve(u8Response);//Send The Status of The Led 
                break;
            case ROOM4_STATUS://In Case Room4 Request 
                if((READ_BIT(PORTD,3))==1){//if The Led Is ON
                    u8Response=ON_STATUS;//Set The Response ON
                }
                else if((READ_BIT(PORTD,3))==0){//if The Led Is off
                    u8Response=OFF_STATUS;//Set The Response off
                }
                else {}
                //u8SPITranmitRecieve(u8Response);//Send The Status of The Led 
                break;
            case TV_STATUS://In Case TV Request 
                if((READ_BIT(PORTD,4))==1){//if The Led Is ON
                    u8Response=ON_STATUS;//Set The Response ON
                }
                else if((READ_BIT(PORTD,4))==0){//if The Led Is off
                    u8Response=OFF_STATUS;//Set The Response off
                }
                else {}
               // u8SPITranmitRecieve(u8Response);//Send The Status of The Led 
                break;
            case AIR_COND_STATUS://In Case Air Conditioning Request 
                if((READ_BIT(PORTD,5))==1){//if The Led Is ON
                    u8Response=ON_STATUS;//Set The Response ON
                }
                else if((READ_BIT(PORTD,5))==0){//if The Led Is off
                    u8Response=OFF_STATUS;//Set The Response off
                }
                else {}
                //u8SPITranmitRecieve(u8Response);//Send The Status of The Led 
                break;
            /****************************Commands Of ON Status *************************/
                
            case ROOM1_TURN_ON://When Receive On The Room1 Led
                led_turn_on(&Room1_Led);//Turn On The Led of Room1
                break;//break From The Switch Case
            case ROOM2_TURN_ON://When Receive On The Room2 Led
                led_turn_on(&Room2_Led);//Turn On The Led of Room2
                break;//break From The Switch Case 
            case ROOM3_TURN_ON://When Receive On The Room3 Led
                led_turn_on(&Room3_Led);//Turn On The Led of Room3
                break;//break From The Switch Case 
            case ROOM4_TURN_ON://When Receive On The Room1 Led
                led_turn_on(&Room4_Led);//Turn On The Led of Room4
                break;//break From The Switch Case 
            case TV_TURN_ON://When Receive On The TV Led
                led_turn_on(&TV_Led);//Turn On The Led of TV
                break;//break From The Switch Case 
            case AIR_COND_TURN_ON://When Receive On The Air Conditioning  Led
                Timer1_Init(&Timer1Obj);//Initialize The Timer1 Module 
                led_turn_on(&AIR_COND_Led);//Turn On The Led of Air Conditioning
                break;//break From The Switch Case
                /*******************************Commands of The off Status******************/
            case ROOM1_TURN_OFF://When Receive Off The Room1 Led
                led_turn_off(&Room1_Led);//Turn Off The Led of Room1
                break;//break From The Switch Case
            case ROOM2_TURN_OFF://When Receive Off The Room2 Led
                led_turn_off(&Room2_Led);//Turn Off The Led of Room2
                break;//break From The Switch Case 
            case ROOM3_TURN_OFF://When Receive Off The Room3 Led
                led_turn_off(&Room3_Led);//Turn Off The Led of Room3
                break;//break From The Switch Case 
            case ROOM4_TURN_OFF://When Receive Off The Room4 Led
                led_turn_off(&Room4_Led);//Turn Off The Led of Room4
                break;//break From The Switch Case 
            case TV_TURN_OFF://When Receive Off The TV Led
                led_turn_off(&TV_Led);//Turn Off The Led of TV
                break;//break From The Switch Case 
            case AIR_COND_TURN_OFF://When Receive Off The Air Conditioning  Led
                Timer1_DeInit(&Timer1Obj);//Initialize The Timer1 Module 
                led_turn_off(&AIR_COND_Led);//Turn Off The Led of Air Conditioning
                break;//break From The Switch Case 
            case SET_TEMPERATURE://In The Case Of Set The Temperature
                u16RequiredTemperature=SSPBUF;//Send The Status of The Led 
                break;//break From The Switch Case
        }
        }
    return (0);
}

void Timer1Handler(void){
    u16ReadingCounter++;//Increment The Counter Every Interrupt
    if(u16ReadingCounter >= 10){//Read The Temperature Value Every 100Milli Seconds 
        u16ReadingCounter=0;//Reset The Reading Counter To Read The Value Again
        u16TempSensorReading=(0.5*GetResultFromTheADC());//Get Value Of The Sensor
        if(u16TempSensorReading >= (u16RequiredTemperature+1)){//Do That If The Reading Temperature Is Greater Then by One Or More The Required Temp
            led_turn_on(&AIR_COND_Led);//Turn On The Led of Air Conditioning
            u16LastValueAirCondtioning=AIR_CONDITIONING_ON;//Set The Status Of The Air Conditioning
        }
        else if(u16TempSensorReading <= (u16RequiredTemperature-1)){//Do That If The Reading Temperature Is Smaller Then by One Or Less The Required Temp
            led_turn_off(&AIR_COND_Led);//Turn Off The Led of Air Conditioning
            u16LastValueAirCondtioning=AIR_CONDITIONING_OFF ;//Set The Status Of The Air Conditioning
        }
        else if(u16TempSensorReading == (u16RequiredTemperature+1)){//Do That If The Reading Temperature Is Equal The Required Temp
            if(u16LastValueAirCondtioning==AIR_CONDITIONING_ON){
                led_turn_on(&AIR_COND_Led);//Turn On The Led of Air Conditioning
            }
            else if(u16LastValueAirCondtioning==AIR_CONDITIONING_OFF){
                led_turn_off(&AIR_COND_Led);//Turn Off The Led of Air Conditioning
            }
        }
        
    }
}
/***This For Get The Value Of the ADC On The Channel*/
uint16 GetResultFromTheADC(void){
    uint16 u16LOC_ResultConversion;
    ADC_GetResultConversionBlocking(&ADC_Obj,ADC_CHANNEL_AN0,&u16LOC_ResultConversion);
    return u16LOC_ResultConversion;
}

void SPI_InterrutHandler(void){
    SPI_FLAG_INTERRUPT_CLEAR();
    while(SSPSTATbits.BF==0);
    u8Request=SSPBUF;
}