/* 
 * File:   menu.c
 * Author: MOUSTAFA
 *
 * Created on June 8, 2023, 4:55 PM
 */

#include "menu.h"

extern volatile uint16 SessionCounter;//Store The Session Time 
extern uint8 FlagTimeOut;//Indicate That The time Session is finish

uint8 u8ComparePass(const uint8 *Pass1,const uint8 *Pass2,uint8 Size){
    uint8 PassCounter=0;//The offset of The Array  
    uint8 PassFlag=TRUE;
    while(PassCounter < Size ){ //loop in The Array To Check in it 
        if(*(Pass1+PassCounter) != *(Pass2+PassCounter)){
            PassFlag=FALSE;//put The Flag False Indicate That The Pass is not true
            break;//break The Loop if on Element is not Equal to anther one  
        }
        PassCounter++;//Increment The Offset of The Array
    }
    return PassFlag;//return The Flag it may be True or False 
}

void vMenuOption(const uint8 u8SelectedRoom,const uint8 u8LonginMode){
    uint8 u8StatusCode=0;//Demand For The Status of Location 
    uint8 u8TurnOnCode=0;//Demand For The On Status on the Selected device 
    uint8 u8TurnoffCode=0;//Demand For The off Status on The Selected Device
    uint8 u8Response=DEFAULT_ACK;
    uint8 u8keyPressed=NOT_PRESSED;
    do{
     lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Send Command Clear Screen in The first 
     switch(u8SelectedRoom){
        case ROOM1_MENU:
            u8StatusCode=ROOM1_STATUS;
            u8TurnOnCode=ROOM1_TURN_ON;
            u8TurnoffCode=ROOM1_TURN_OFF;
            lcd_4bit_send_string_data(&Lcd_Obj,"Room1 S:");
            break;
        case ROOM2_MENU:
            u8StatusCode=ROOM2_STATUS;
            u8TurnOnCode=ROOM2_TURN_ON;
            u8TurnoffCode=ROOM2_TURN_OFF;
            lcd_4bit_send_string_data(&Lcd_Obj,"Room2 S:");
            break;
        case ROOM3_MENU:
            u8StatusCode=ROOM3_STATUS;
            u8TurnOnCode=ROOM3_TURN_ON;
            u8TurnoffCode=ROOM3_TURN_OFF;
            lcd_4bit_send_string_data(&Lcd_Obj,"Room3 S:");
            break;
        case ROOM4_MENU:
            u8StatusCode=ROOM4_STATUS;
            u8TurnOnCode=ROOM4_TURN_ON;
            u8TurnoffCode=ROOM4_TURN_OFF;
            lcd_4bit_send_string_data(&Lcd_Obj,"Room4 S:");
            break;
        case TV_MENU:
            u8StatusCode=TV_STATUS;
            u8TurnOnCode=TV_TURN_ON;
            u8TurnoffCode=TV_TURN_OFF;
            lcd_4bit_send_string_data(&Lcd_Obj,"TV S:");
            break;
        case AIR_CONTROL_MENU:
            u8StatusCode=AIR_COND_STATUS;
            u8TurnOnCode=AIR_COND_TURN_ON;
            u8TurnoffCode=AIR_COND_TURN_OFF;
            lcd_4bit_send_string_data(&Lcd_Obj,"AIR COND. S:");
            break;   
          }
        /**************************************************************************/
          u8SpiTranmitRecieve(u8StatusCode);//Master asks The Status of The Specific Device
          _TimerDelay_ms(100);//Halt The System until the Slave Respond
          u8Response=u8SpiTranmitRecieve(u8StatusCode);//Get The Status of The Device if it Turn on or off
          if(u8Response==ON_STATUS){
              lcd_4bit_send_string_data(&Lcd_Obj,"ON");//Show Status in The Screen ON or OFF
          }
          else if (u8Response==OFF_STATUS){
              lcd_4bit_send_string_data(&Lcd_Obj,"OFF");
          }
          else {/*No Thing */}
          set_cursor_4bit_mode(&Lcd_Obj,2,1);//Set The Cursor In The Row2 And Column 11
          lcd_4bit_send_string_data(&Lcd_Obj,"1-ON 2-OFF 0-RET");//Sow on lcd if I Want ON or OFF or Return
          u8keyPressed=u8GetPressedValue(u8LonginMode);//Still here until The user Enter The Status or Time out Finish
          _TimerDelay_ms(100);//Halt The System To avoid The Duplicate Input 
          if(u8keyPressed=='1'){
              u8SpiTranmitRecieve(u8TurnOnCode);//Send Turn on The Device if The Input is '1'
          }
          else if(u8keyPressed=='2'){
              u8SpiTranmitRecieve(u8TurnoffCode);//Send Turn off The Device if The Input is '0'
          }
          else if ((u8keyPressed != NOT_PRESSED) && (u8keyPressed != '0')){//show Wrong Answer if The Input is Invalid 
              lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear Screen if The Input is Wrong Number
              lcd_4bit_send_string_data(&Lcd_Obj,"Wrong Input");//Display Wrong Input if The Input is Invalid
              _TimerDelay_ms(500);//Halt The System To Show The "Wrong Input"
          }
    }while(((u8keyPressed < '0') || (u8keyPressed > '2')) && (FlagTimeOut == FALSE));//Still in The Loop if The user enter invalid Input or FlagTimeout Equal TRUE
}

uint8 u8GetPressedValue(const uint8 u8LoginMode){
    uint8 u8LockeyPressed=NOT_PRESSED;
    while (u8LockeyPressed == NOT_PRESSED){ //Repeat until the user Enter Valid Input
        if((SessionCounter >= ADMIN_TIMEOUT && u8LoginMode==ADMIN_MODE) || (SessionCounter >= GUEST_TIMEOUT && u8LoginMode==GUEST_MODE)){
            FlagTimeOut=TRUE;//Put Flag True if Time is Exceed
            break;//break The Loop if The Input not Specified
        }
        keypad_get_value(&Keypad_Obj,&u8LockeyPressed);//Save The Value in The Variable u8keyPressed
    }
    return u8LockeyPressed;//Return The Value of The Input 
}
/**Function To Receive And Transmit on The Same Time */
uint8 u8SpiTranmitRecieve(uint8 u8Data){
    uint8 Loc_Data=0xff;
    gpio_Pin_Write_Logic(&PIN_SelectChannel,GPIO_LOW);
    SSPBUF=u8Data;
    while((PIR1bits.SSPIF == 0) && (SSPSTATbits.BF == 0));
    PIR1bits.SSPIF=0;
    Loc_Data=SSPBUF;
    gpio_Pin_Write_Logic(&PIN_SelectChannel,GPIO_HIGH);
    return Loc_Data;//Return The Received Data By The Spi
}