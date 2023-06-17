/* 
 * File:   menu.h
 * Author: MOUSTAFA
 *
 * Created on June 8, 2023, 4:55 PM
 */

#ifndef MENU_H
#define	MENU_H
#include "ECU_LAYER/char_lcd/ecu_char_lcd.h"
#include "ECU_LAYER/key_pad/ecu_keypad.h"
#include "MCAL_LAYER/SPI/hal_spi.h"
#include "Main_config.h"
#include "STD_Messeges.h"
#include "application.h"

/**
 * Function Name:u8ComparePass
 * Function Description:This Routin used To Conpare The password And Return is 
 * they the Same or not  
 * @param Pass1
 * @param Pass2
 * @param Size
 * @return  True if The two Array Identical False if Two Array are not Identical
 */
uint8 u8ComparePass(const uint8 *Pass1,const uint8 *Pass2,uint8 Size);
/**
 * Function Name:vMenuOption
 * Function Description: This Routin to Show Sub system in the lcd and show its Status 
 * Function Returns:void
 * @param u8SelectedRoom
 * @param u8LonginMode
 */
void vMenuOption(const uint8 u8SelectedRoom,const uint8 u8LonginMode);
/**
 * Function Name:u8GetPressedValue
 * Function Description: used To wait to When The user Pressed The key
 * Function Return:uint8
 * @param u8LoginMode
 * @return 
 */
uint8 u8GetPressedValue(const uint8 u8LoginMode);
/**
 * Function Name:u8SpiTranmitRecieve
 * Function Description:This Routine is Send Data From SPI And Receive it 
 * @param u8Data
 * @return it return Data Received
 */
uint8 u8SpiTranmitRecieve(uint8 u8Data);
#endif	/* MENU_H */

