/* 
 * File:   application.c
 * Author: MOUSTAFA
 *
 * Created on June 8, 2023, 4:07 AM
 */

#include "application.h"
#include "menu.h"
/*
 * 
 */
uint8 u8PassIsSetOrNotSet(uint16 Address);//Deceleration of The Function of The Read certain Address
void vReadBlockOnEEPROM(uint16 u16TheStartAddress,uint8 *Pass,uint8 Size);//Deceleration of The Read Block in The EEPROM
void  vWriteBlockOnEEPROM(uint16 u16TheStartAddress,uint8 *Pass,uint8 Size);//Deceleration of The Write Block in The EEPROM


volatile uint16 SessionCounter=0;//Counter The Interrupt of Timer1 every 10Milli Seconds 
uint8 FlagTimeOut=FALSE;//Store The Session is Still Valid or Not
int main(void) {
    /**********************Initialize The Modules of The System****************/
    led_initialize(&ADMIN_Led);//Initialize Led of The Admin
    led_initialize(&GUEST_Led);//Initialize Led of The Guest
    led_initialize(&BLOCK_Led);//Initialize Led of The Block_Mode
    keypad_initialize(&Keypad_Obj);//Initialize For The Keypad
    lcd_4bit_initialize(&Lcd_Obj);//Initialize For The Liquid Character Display
    gpio_Pin_Intialize(&PIN_SelectChannel);//Initialize The pin of The SS
    SPI_Init(&SPIObject);//Initialize The SPI Module
    Timer1_Init(&Timer1Obj);//Initialize Timer1 Module
    ADCON1bits.PCFG=0x0F;//Make The Pin Of The ADC Is All Digital
    /********************************************************************************/
    uint8 Temperature=0;//The Average of The Temperature in The Air Conditioning
    uint8 TempOnes=NOT_SELECTED;//The Right Number Of The Input Temperature
    uint8 TempTens=NOT_SELECTED;//The Left Number Of The Input Temperature
    
    uint8 u8KeyPadPressed=NOT_PRESSED;//Store The Current Value of The Keypad
    uint8 u8LoginMode=NO_MODE;//Store The Mode of The System 
    uint8 BlockModeFlag=FALSE;//if The System in The Block Mode This Is Indication that The System in The Block Mode
    
    /***********************Welcome Screen For The First Time***************************************/
    lcd_4bit_send_string_data(&Lcd_Obj,"Welcome to Smart");//Show The The First Message of Welcome The System
    set_cursor_4bit_mode(&Lcd_Obj,2,3);//Set The Cursor On The Second Row And The First Column 
    lcd_4bit_send_string_data(&Lcd_Obj,"Home System");
    _TimerDelay_ms(2000);//Halt The System For two Second
    lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
    /***********************Starting Of The System To Set The Password of The Guest And The Admin ***************/
    /**Check if The password Is set Or Not */
    if((PASS_SET !=u8PassIsSetOrNotSet(ADMIN_PASS_ADDRESS_STATUS)) || (PASS_SET !=u8PassIsSetOrNotSet(GEUST_PASS_ADDRESS_STATUS))){
        lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
        lcd_4bit_send_string_data(&Lcd_Obj,"Login For The");//Show The Login For The First Time 
        set_cursor_4bit_mode(&Lcd_Obj,2,3);//Set The Cursor On The Second Row And The Third Column 
        lcd_4bit_send_string_data(&Lcd_Obj,"First Time");
        _TimerDelay_ms(2000);//Halt The System For two Second
        lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
        lcd_4bit_send_string_data(&Lcd_Obj,"Set Admin Pass ");//Display Message To Set The Admin Pass
        set_cursor_4bit_mode(&Lcd_Obj,2,1);//Set The Cursor On The Second Row And The First Column 
        lcd_4bit_send_string_data(&Lcd_Obj,"Admin Pass:");//Display Message To Set The Admin Pass
        /*************************To Set The Password for The Admin And The Guest******************************/
        uint8 PassCounter=0;//Iterator For Setting The Password That Stored in The EEPROM
        uint8 Pass[PASS_SIZE]={NOT_STORD,NOT_STORD,NOT_STORD,NOT_STORD};//The Array That Stored The Password That The User will Enter
        while (PassCounter < PASS_SIZE){
            u8KeyPadPressed=NOT_PRESSED;
            while(u8KeyPadPressed == NOT_PRESSED){//Loop TO Make The user To Enter The Specific Password
                keypad_get_value(&Keypad_Obj,&u8KeyPadPressed);//Get The Value And Sore it In u8KeyPadPressed
            }
            Pass[PassCounter]=u8KeyPadPressed;//Store The Password In The Array of The Password
            lcd_4bit_send_char_data(&Lcd_Obj,u8KeyPadPressed);//Display The Character in The LCD
            _TimerDelay_ms(CHARACTER_REVIEW_TIME);//Halt The System For Half Second
            set_cursor_4bit_mode(&Lcd_Obj,2,12+PassCounter);//Set The Cursor On The Second Row And The Column in The Password Digit is In
            lcd_4bit_send_char_data(&Lcd_Obj,PASSWORD_SYMBOL);//Display The Character '*' in The LCD
            _TimerDelay_ms(50);//Halt The System Here for Given Time in Milli Seconds
            PassCounter++;//Increment The Pass Offset In LOOP
        }
        vWriteBlockOnEEPROM(ADMIN_PASS_ADDRESS_START,Pass,PASS_SIZE);//Store The Pass Array in EEPROM
        eeprom_WriteDataByte(ADMIN_PASS_ADDRESS_STATUS,PASS_SET);//Store in The Address That The Password Is Set (Indication)
        lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
        lcd_4bit_send_string_data(&Lcd_Obj,"Pass Saved");//Display Message To User That The Password Is Saved
        _TimerDelay_ms(1000);//Halt The System Here for Given Time in Milli Seconds
        
        /*******************Set The Guest Password*************************************/
        PassCounter=0;//Reset The PassCounter To Get The Guest Password
        lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
        lcd_4bit_send_string_data(&Lcd_Obj,"Set Guest Pass ");//Display Message To Set The Admin Pass
        set_cursor_4bit_mode(&Lcd_Obj,2,1);//Set The Cursor On The Second Row And The First Column 
        lcd_4bit_send_string_data(&Lcd_Obj,"Guest Pass:");//Display Message To Set The Admin Pass
        while (PassCounter < PASS_SIZE){//Loop TO Get The Password From the User For Guest Mode
            u8KeyPadPressed=NOT_PRESSED;
            while(u8KeyPadPressed == NOT_PRESSED){//Loop TO Make The user To Enter The Specific Password
                keypad_get_value(&Keypad_Obj,&u8KeyPadPressed);//Get The Value And Sore it In u8KeyPadPressed
            }
            Pass[PassCounter]=u8KeyPadPressed;//Store The Password In The Array of The Password
            lcd_4bit_send_char_data(&Lcd_Obj,u8KeyPadPressed);//Display The Character in The LCD
            _TimerDelay_ms(CHARACTER_REVIEW_TIME);//Halt The System For Half Second
            set_cursor_4bit_mode(&Lcd_Obj,2,12+PassCounter);//Set The Cursor On The Second Row And The Column in The Password Digit is In
            lcd_4bit_send_char_data(&Lcd_Obj,PASSWORD_SYMBOL);//Display The Character '*' in The LCD
            _TimerDelay_ms(50);//Halt The System Here for Given Time in Milli Seconds
            PassCounter++;//Increment The Pass Offset In LOOP
        }
        vWriteBlockOnEEPROM(GEUST_PASS_ADDRESS_START,Pass,PASS_SIZE);//Store The Pass of The Guest Array in EEPROM
        eeprom_WriteDataByte(GEUST_PASS_ADDRESS_STATUS,PASS_SET);//Store in The Address That The Password Is Set (Indication)
        lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
        lcd_4bit_send_string_data(&Lcd_Obj,"Pass Saved");//Display Message To User That The Password Is Saved
        _TimerDelay_ms(1000);//Halt The System Here for Given Time in Milli Seconds
        lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
        eeprom_WriteDataByte(BLOCK_PASS_ADDRESS_STATUS,FALSE);//Put The Status Of The Block Mode That is Not BLocked
    }
    else {//Run Only When The Pass Is Set And Saved in The EEPROM
        BlockModeFlag=u8PassIsSetOrNotSet(BLOCK_PASS_ADDRESS_STATUS);//Read The Status of The Block Mode in The EEPROM
    }
    while(1){
        u8KeyPadPressed=NOT_PRESSED;
        uint8 PassTriesCount=0;//The Tries of Entering The Password
        if(FlagTimeOut==TRUE){//The Session Time is Finished This Will Be Execute
                Timer1_DeInit(&Timer1Obj);//Stop Timer1 Module
                SessionCounter=0;//Reset The Session Counter Of The Timer
                u8LoginMode=NO_MODE;//Return To The Main Menu of Login mode
                FlagTimeOut=FALSE;//Put TimeoutFlag To 0 To Login Again
                u8KeyPadPressed=NOT_PRESSED;//Put Keypad NOT_PRESSED
                led_turn_off(&ADMIN_Led);//Turn off The Guest Led
                led_turn_off(&GUEST_Led);//Turn off The Admin Led 
                lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
                lcd_4bit_send_string_data(&Lcd_Obj,"Session Timeout");//Display Message To User That The Session Timeout is Exceeded 
                _TimerDelay_ms(1000);//Halt The System Here for Given Time in Milli Seconds
            }
        while(u8LoginMode==NO_MODE){//This While Will Break if The LoginMode Not Equal No_Mode
            if(BlockModeFlag==TRUE){//if User Enter The Password Wrong upper The Allowed Tries
                lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
                lcd_4bit_send_string_data(&Lcd_Obj,"Login Blocked");//Display Message To User That The login Blocked
                set_cursor_4bit_mode(&Lcd_Obj,2,1);//Set The Cursor On The Second Row And The First Column
                lcd_4bit_send_string_data(&Lcd_Obj,"Wait 20 Seconds");//Display Message To User That Waiting 20 Seconds
                led_turn_on(&BLOCK_Led);//Turn on The Blocked Led
                _TimerDelay_ms(BLOCK_MODE_TIME);//Halt The System Here for Given Time in Milli Seconds
                PassTriesCount=0;//Reset The Pass Tries
                BlockModeFlag=FALSE;//Return The Block Flag To Zero
                u8KeyPadPressed=NOT_PRESSED;//Put Keypad NOT_PRESSED
                led_turn_off(&BLOCK_Led);//Turn on The Blocked Led
                eeprom_WriteDataByte(BLOCK_PASS_ADDRESS_STATUS,FALSE);//Put The Status Of The Block Mode That is Not BLocked
            }
            lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
            lcd_4bit_send_string_data(&Lcd_Obj,"Select Mode :");//Display The Mode Option To User
            set_cursor_4bit_mode(&Lcd_Obj,2,1);//Set The Cursor On The Second Row And The First Column
            lcd_4bit_send_string_data(&Lcd_Obj,"0:Admin  1:Guest");//Display The Mode Option To User
            u8KeyPadPressed=NOT_PRESSED;
            while(u8KeyPadPressed == NOT_PRESSED){//Loop TO Make The user To Enter The Specific Password
                keypad_get_value(&Keypad_Obj,&u8KeyPadPressed);//Get The Value And Sore it In u8KeyPadPressed
            }
            if((u8KeyPadPressed != CHECK_ADMIN_MODE) && (u8KeyPadPressed!=CHECK_GUEST_MODE) ){//Check if User Input Wrong Input
                lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
                lcd_4bit_send_string_data(&Lcd_Obj,"Wrong Input");//Display The Mode Option To User
                _TimerDelay_ms(1000);//Halt The System Here for Given Time in Milli Seconds
                continue;//Return To The main Loop of LoginMode=NO_MODE 
            }
            uint8 PassCounter=0;//Iterator To use The Pass Array
            uint8 Pass[PASS_SIZE]={NOT_STORD,NOT_STORD,NOT_STORD,NOT_STORD};//The Array That Stored The Password That The User will Enter
            uint8 PassStored[PASS_SIZE]={NOT_STORD,NOT_STORD,NOT_STORD,NOT_STORD};//Array That Store The Pass Entered To check it with the Pass in EEPROM
/***************************************Admin Check Input To User *************************************************/            
            switch(u8KeyPadPressed){//Checker The User Is Admin Or Guest
                case CHECK_ADMIN_MODE://if The User Is Admin Mode 
                while(u8LoginMode != ADMIN_MODE){//To Loop In Case The Wrong Password 
                    u8KeyPadPressed=NOT_PRESSED;//Return The Variable To Get Input
                    lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
                    lcd_4bit_send_string_data(&Lcd_Obj,"Admin Mode");//Display The Admin Mode To User
                    set_cursor_4bit_mode(&Lcd_Obj,2,1);//Set The Cursor On The Second Row And The First Column
                    lcd_4bit_send_string_data(&Lcd_Obj,"Admin Pass:");//Display The Admin Mode To User
                    _TimerDelay_ms(200);//Halt The System Here for Given Time in Milli Seconds
                    while(PassCounter < PASS_SIZE){
                        u8KeyPadPressed=NOT_PRESSED;
                        while(u8KeyPadPressed == NOT_PRESSED){//Loop TO Make The user To Enter The Specific Password
                           keypad_get_value(&Keypad_Obj,&u8KeyPadPressed);//Get The Value And Sore it In u8KeyPadPressed
                        }
                        Pass[PassCounter]=u8KeyPadPressed;//Store The Pass In The Array To Check it Correct or Not
                        lcd_4bit_send_char_data(&Lcd_Obj,u8KeyPadPressed);//Display The Character in The LCD
                        _TimerDelay_ms(CHARACTER_REVIEW_TIME);//Halt The System For Half Second
                        set_cursor_4bit_mode(&Lcd_Obj,2,12+PassCounter);//Set The Cursor On The Second Row And The Column in The Password Digit is In
                        lcd_4bit_send_char_data(&Lcd_Obj,PASSWORD_SYMBOL);//Display The Character '*' in The LCD
                        _TimerDelay_ms(100);//Halt The System Here for Given Time in Milli Seconds
                        PassCounter++;//Increment The Pass Offset In LOOP
                    }
                    PassCounter=0;//Iterator To use The Pass Array
                    vReadBlockOnEEPROM(ADMIN_PASS_ADDRESS_START,PassStored,PASS_SIZE);
                    /*Check The Pass Is Correct Or Not */
                    if(TRUE==u8ComparePass(PassStored,Pass,PASS_SIZE)){
                        u8LoginMode=ADMIN_MODE;//Set LoginMode As Admin Mode 
                        PassTriesCount=0;//Clear The Pass Tries If The User May Enter Wrong Pass
                        lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
                        lcd_4bit_send_string_data(&Lcd_Obj,"Correct Pass");//Display The Admin Mode To User
                        set_cursor_4bit_mode(&Lcd_Obj,2,1);//Set The Cursor In The Row2 And Column 1
                        lcd_4bit_send_string_data(&Lcd_Obj,"Admin Mode");//Display The Admin Mode To User
                        _TimerDelay_ms(1000);//Halt The System Here for Given Time in Milli Seconds
                        led_turn_on(&ADMIN_Led);//Set Led of The Admin On
                        Timer1_Init(&Timer1Obj);//Initialize Timer1 Module
                        lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
                    }
                    else if (FALSE==u8ComparePass(PassStored,Pass,PASS_SIZE)){
                        PassTriesCount++;//Increment PassTries By on 
                        u8LoginMode=NO_MODE;//Set LoginMode As NO_Mode
                        lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
                        lcd_4bit_send_string_data(&Lcd_Obj,"Wrong Pass");//Display Wrong Pass of The Admin Mode To User
                        set_cursor_4bit_mode(&Lcd_Obj,2,1);//Set The Cursor In The Row2 And Column 1
                        lcd_4bit_send_string_data(&Lcd_Obj,"Tries Left");//Display Wrong Pass of The Admin Mode To User
                        lcd_4bit_send_char_data(&Lcd_Obj,TRIES_ALLOWD-PassTriesCount+ASSCI_ZERO);//Display The Tries Left
                        _TimerDelay_ms(1000);//Halt The System Here for Given Time in Milli Seconds
                        lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
                        if(PassTriesCount >= TRIES_ALLOWD){
                            eeprom_WriteDataByte(BLOCK_PASS_ADDRESS_STATUS,TRUE);//Put The Status Of The Block Mode That is Not BLocked
                            BlockModeFlag=TRUE;//Set The Block Mode To User
                            break;//Break The Loop of Loop Of LoginMode!=AdminMode   
                        }
                    }
                 }
                   break;//break from The Switch Case
/********************************Guest Check Input To User*********************************************************/                    
                case CHECK_GUEST_MODE://if The User Is Guest Mode 
                    while(u8LoginMode != GUEST_MODE){//To Loop In Case The Wrong Password 
                    u8KeyPadPressed=NOT_PRESSED;//Return The Variable To Get Input
                    lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
                    lcd_4bit_send_string_data(&Lcd_Obj,"Guest Mode");//Display The Guest Mode To User
                    set_cursor_4bit_mode(&Lcd_Obj,2,1);//Set The Cursor On The Second Row And The First Column
                    lcd_4bit_send_string_data(&Lcd_Obj,"Guest Pass:");//Display The Guest Mode To User
                    _TimerDelay_ms(200);//Halt The System Here for Given Time in Milli Seconds
                    while(PassCounter <PASS_SIZE){
                        u8KeyPadPressed=NOT_PRESSED;
                        while(u8KeyPadPressed == NOT_PRESSED){//Loop TO Make The user To Enter The Specific Password
                           keypad_get_value(&Keypad_Obj,&u8KeyPadPressed);//Get The Value And Sore it In u8KeyPadPressed
                        }
                        Pass[PassCounter]=u8KeyPadPressed;//Store The Pass In The Array To Check it Correct or Not
                        lcd_4bit_send_char_data(&Lcd_Obj,u8KeyPadPressed);//Display The Character in The LCD
                        _TimerDelay_ms(CHARACTER_REVIEW_TIME);//Halt The System For Half Second
                        set_cursor_4bit_mode(&Lcd_Obj,2,12+PassCounter);//Set The Cursor On The Second Row And The Column in The Password Digit is In
                        lcd_4bit_send_char_data(&Lcd_Obj,PASSWORD_SYMBOL);//Display The Character '*' in The LCD
                        _TimerDelay_ms(100);//Halt The System Here for Given Time in Milli Seconds
                        PassCounter++;//Increment The Pass Offset In LOOP
                    }
                    PassCounter=0;//Iterator To use The Pass Array
                    vReadBlockOnEEPROM(GEUST_PASS_ADDRESS_START,PassStored,PASS_SIZE);
                    /*Check The Pass Is Correct Or Not */
                    if(TRUE==u8ComparePass(PassStored,Pass,PASS_SIZE)){
                        u8LoginMode=GUEST_MODE;//Set LoginMode As Guest Mode 
                        PassTriesCount=0;//Clear The Pass Tries If The User May Enter Wrong Pass
                        lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
                        lcd_4bit_send_string_data(&Lcd_Obj,"Correct Pass");//Display The Guest Mode To User
                        set_cursor_4bit_mode(&Lcd_Obj,2,1);//Set The Cursor In The Row2 And Column 1
                        lcd_4bit_send_string_data(&Lcd_Obj,"Guest Mode");//Display The Guest Mode To User
                        _TimerDelay_ms(1000);//Halt The System Here for Given Time in Milli Seconds
                        led_turn_on(&GUEST_Led);//Set Led of The Admin On
                        Timer1_Init(&Timer1Obj);//Initialize Timer1 Module
                        lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
                    }
                    else {
                        PassTriesCount++;//Increment PassTries By on 
                        u8LoginMode=NO_MODE;//Set LoginMode As NO_Mode
                        lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
                        lcd_4bit_send_string_data(&Lcd_Obj,"Wrong Pass");//Display Wrong Pass of The Guest Mode To User
                        set_cursor_4bit_mode(&Lcd_Obj,2,1);//Set The Cursor In The Row2 And Column 1
                        lcd_4bit_send_string_data(&Lcd_Obj,"Tries Left");//Display Wrong Pass of The Admin Mode To User
                        lcd_4bit_send_char_data(&Lcd_Obj,TRIES_ALLOWD-PassTriesCount+ASSCI_ZERO);//Display The Tries Left
                        _TimerDelay_ms(1000);//Halt The System Here for Given Time in Milli Seconds
                        lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
                        if(PassTriesCount >= TRIES_ALLOWD){
                            eeprom_WriteDataByte(BLOCK_PASS_ADDRESS_STATUS,TRUE);//Put The Status Of The Block Mode That is Not BLocked
                            BlockModeFlag=TRUE;//Set The Block Mode To User
                            break;//Break The Loop of Loop Of LoginMode!=AdminMode   
                        }
                    }
                 }
                    break;//break from The Switch Case
           }
        }
        /*****************************Started of The Show System of Rooms And Of The TV And Air Conditioning*/
        uint8 ShowMenu=MAIN_MENU;//Put The Show Menu As The Main Menu
        
        while(FlagTimeOut != TRUE){//Loop Until The Time Is Not Out 
            u8KeyPadPressed=NOT_PRESSED;//Set The KeyPad By Its Default Value 
            switch(ShowMenu){
                case MAIN_MENU:
                    do{
                        /*Print The Main Menu*/
                        lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
                        lcd_4bit_send_string_data(&Lcd_Obj,"1:Room1 2:Room2");//Display The Guest Mode To User
                        set_cursor_4bit_mode(&Lcd_Obj,2,1);//Set The Cursor In The Row2 And Column 1
                        if(u8LoginMode==ADMIN_MODE){
                            lcd_4bit_send_string_data(&Lcd_Obj,"3:Room3 4:More");//Display The Admin Mode To User
                        }
                        else if (u8LoginMode==GUEST_MODE){
                            lcd_4bit_send_string_data(&Lcd_Obj,"3:Room3 4:Room4");//Display The Guest Mode To User
                        }
                        u8KeyPadPressed=u8GetPressedValue(u8LoginMode);
                        _TimerDelay_ms(100);//Halt The System Here for Given Time in Milli Seconds
                        if(u8KeyPadPressed==SELECT_ROOM1){//If User Select Room1 To Display it 
                            ShowMenu=ROOM1_MENU;//Put The Display menu Room1
                        }
                        else if(u8KeyPadPressed==SELECT_ROOM2){//If User Select Room2 To Display it 
                            ShowMenu=ROOM2_MENU;//Put The Display menu Room2
                        }
                        else if(u8KeyPadPressed==SELECT_ROOM3){//If User Select Room3 To Display it 
                            ShowMenu=ROOM3_MENU;//Put The Display menu Room3
                        }
                        else if((u8KeyPadPressed==ADMIN_MORE_OPTION) && (u8LoginMode==ADMIN_MODE)){//if The user Enter 4 And User id Admin
                            ShowMenu=MORE_MENU;//Put The Display menu more
                        }
                        else if((u8KeyPadPressed==SELECT_ROOM4) && (u8LoginMode==GUEST_MODE)){//if The user Enter 4 And User id Guest
                             ShowMenu=ROOM4_MENU;//Put The Display Room4 menu 
                        }
                        else if(u8KeyPadPressed != NOT_PRESSED){//if The User Enter Wrong Input
                            lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
                            lcd_4bit_send_string_data(&Lcd_Obj,"Wrong Input");//Display Wrong Input To User
                            _TimerDelay_ms(500);//Halt The System Here for Given Time in Milli Seconds
                        }
                    }while(((u8KeyPadPressed < '1') || (u8KeyPadPressed > '4')) && (FlagTimeOut == FALSE));//Loop until the User Enter Valid Key  
                    break;//Break The Switch case The End Of The Main Case 
                case MORE_MENU://in Case Show More Window in Admin Mode
                    do{
                    lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
                    lcd_4bit_send_string_data(&Lcd_Obj,"1:Room4 2:TV");//Display Wrong Input To User
                    set_cursor_4bit_mode(&Lcd_Obj,2,1);//Set The Cursor In The Row2 And Column 1
                    lcd_4bit_send_string_data(&Lcd_Obj,"3:AirCond 4:RET");//Display Wrong Input To User
                    u8KeyPadPressed=u8GetPressedValue(u8LoginMode);
                    _TimerDelay_ms(100);//Halt The System Here for Given Time in Milli Seconds
                    if(u8KeyPadPressed==SELECT_ROOM4_ADMIN){//If User Select Room4 To Display it 
                            ShowMenu=ROOM4_MENU;//Put The Display menu Room4
                        }
                        else if(u8KeyPadPressed==SELECT_TV){//If User Select TV To Display it 
                            ShowMenu=TV_MENU;//Put The Display menu Tv
                        }
                        else if(u8KeyPadPressed==SELECT_AIR_CONDTIONING){//If User Select Air Conditioning To Display it 
                            ShowMenu=AIR_COND_MENU;//Put The Display menu Air Conditioning
                        }
                        else if(u8KeyPadPressed==ADMIN_RET_OPTION){//if The user Enter 4 And User id Admin
                            ShowMenu=MAIN_MENU;//Put The Display menu more
                        }
                        else if(u8KeyPadPressed != NOT_PRESSED){//if The User Enter Wrong Input
                            lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
                            lcd_4bit_send_string_data(&Lcd_Obj,"Wrong Input");//Display Wrong Input To User
                            _TimerDelay_ms(500);//Halt The System Here for Given Time in Milli Seconds
                        }
                    }while(((u8KeyPadPressed < '1') || (u8KeyPadPressed > '4')) && (FlagTimeOut == FALSE));//Loop until the User Enter Valid Key
                    break;//Break The Switch Case
                case AIR_COND_MENU:
                    do{
                    lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
                    lcd_4bit_send_string_data(&Lcd_Obj,"1:Set Temperature");//DisplayT Inputs To User
                    set_cursor_4bit_mode(&Lcd_Obj,2,1);//Set The Cursor In The Row2 And Column 1
                    lcd_4bit_send_string_data(&Lcd_Obj,"2:Control 0:RET");//Display Inputs To User
                    u8KeyPadPressed=u8GetPressedValue(u8LoginMode);
                    _TimerDelay_ms(100);//Halt The System Here for Given Time in Milli Seconds
                    if(u8KeyPadPressed==SELECT_SET_TEMPERATURE){//If User Select Set Temperature To Display it 
                            ShowMenu=TEMP_MENU;//Put The Display menu Te,p menu 
                        }
                        else if(u8KeyPadPressed==SELECT_AIR_COND_CTRL){//If User Select Air Control To Display it 
                            ShowMenu=AIR_CONTROL_MENU;//Put The Display menu Air Control
                        }
                        else if(u8KeyPadPressed==SELECT_AIR_COND_RET){//If User Select Air Conditioning Return  To Display it 
                            ShowMenu=MORE_MENU;//Put The Display menu Air Conditioning
                        }
                        else if(u8KeyPadPressed != NOT_PRESSED){//if The User Enter Wrong Input
                            lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
                            lcd_4bit_send_string_data(&Lcd_Obj,"Wrong Input");//Display Wrong Input To User
                            _TimerDelay_ms(500);//Halt The System Here for Given Time in Milli Seconds
                        }
                    }while(((u8KeyPadPressed < '0') || (u8KeyPadPressed > '2')) && (FlagTimeOut == FALSE));
                    break;//Break The Switch case End The Air Conditioning Case
                case ROOM1_MENU://In The Case Of Room 1 Menu 
                    vMenuOption(ROOM1_MENU,u8LoginMode);//Display Room 1 Display
                    ShowMenu=MAIN_MENU;//Return The Main Menu
                    break;
                case ROOM2_MENU://In The Case Of Room 1 Menu 
                    vMenuOption(ROOM2_MENU,u8LoginMode);//Display Room 1 Display
                    ShowMenu=MAIN_MENU;//Return The Main Menu
                    break;
                case ROOM3_MENU://In The Case Of Room 1 Menu 
                    vMenuOption(ROOM3_MENU,u8LoginMode);//Display Room 1 Display
                    ShowMenu=MAIN_MENU;//Return The Main Menu
                    break;
                case ROOM4_MENU://In The Case Of Room 1 Menu 
                    vMenuOption(ROOM4_MENU,u8LoginMode);//Display Room 1 Display
                    if(u8LoginMode==GUEST_MODE){//in case in The Guest Mode
                        ShowMenu=MAIN_MENU;//Return The Main Menu
                    }
                    else if(u8LoginMode==ADMIN_MODE){//In case In The Admin Mode
                    ShowMenu=MORE_MENU;//Return The MORE Menu
                    }
                    break;
                case TV_MENU://In The Case Of Room 1 Menu 
                    vMenuOption(TV_MENU,u8LoginMode);//Display Room 1 Display
                    ShowMenu=MORE_MENU;//Return The Main Menu
                    break;
                case AIR_CONTROL_MENU://In The Case Of Room 1 Menu 
                    vMenuOption(AIR_CONTROL_MENU,u8LoginMode);//Display Room 1 Display
                    ShowMenu=AIR_COND_MENU;//Return The Main Menu
                    break;
                case TEMP_MENU://Case of The Temperature Menu
                    Temperature=0;//Clear The Value Of The Temperature
                    while(Temperature==0 && FlagTimeOut==FALSE){//Loop Until Enter The Temperature From The User
                        lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
                        lcd_4bit_send_string_data(&Lcd_Obj,"Set Temp.:__");//Display Set Temp To Enter The Temp
                        lcd_4bit_send_char_data(&Lcd_Obj,EDGRESS_SYMBOL);//Display The Degrees Symbol 
                        lcd_4bit_send_char_data(&Lcd_Obj,'C');//Display The Degrees Symbol
                        set_cursor_4bit_mode(&Lcd_Obj,1,11);//Set The Cursor In The Row2 And Column 11
                        _TimerDelay_ms(200);//Halt The System Here for Given Time in Milli Seconds
                        /*********************************************************************************/
                        /***********************This For The First Character Of the Temp*************/
                        u8KeyPadPressed=u8GetPressedValue(u8LoginMode);
                        _TimerDelay_ms(300);//Halt The System Here for Given Time in Milli Seconds
                        if(FlagTimeOut==TRUE){//Check if The Session Is ended
                            break;//break The While Loop 
                        }
                        if(u8KeyPadPressed < '0' || u8KeyPadPressed > '9'){//In Case Enter Wrong Input
                            lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
                            lcd_4bit_send_string_data(&Lcd_Obj,"Wrong Input");//Display Set Temp To Enter The Temp
                            _TimerDelay_ms(500);//Halt The System Here for Given Time in Milli Seconds
                            continue;//Return To The Loop To Enter Input Again
                        }
                        else {//The Input If Valid
                            lcd_4bit_send_char_data(&Lcd_Obj,u8KeyPadPressed);//Display The The First Number of The Temp
                            TempTens=u8KeyPadPressed-ASSCI_ZERO;//Store The Temp Tens In This Variables
                            u8KeyPadPressed=NOT_PRESSED;//Set The Key Pad Value To The Default Value 
                        }
                        /***********************This For The Seconds Character Of the Temp*************/
                        u8KeyPadPressed=u8GetPressedValue(u8LoginMode);
                        _TimerDelay_ms(300);//Halt The System Here for Given Time in Milli Seconds
                        if(FlagTimeOut==TRUE){//Check if The Session Is ended
                            break;//break The While Loop 
                        }
                        if(u8KeyPadPressed < '0' || u8KeyPadPressed > '9'){//In Case Enter Wrong Input
                            lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
                            lcd_4bit_send_string_data(&Lcd_Obj,"Wrong Input");//Display Wrong  To The User
                            _TimerDelay_ms(500);//Halt The System Here for Given Time in Milli Seconds
                            continue;//Return To The Loop To Enter Input Again
                        }
                        else {//The Input If Valid
                            lcd_4bit_send_char_data(&Lcd_Obj,u8KeyPadPressed);//Display The The First Number of The Temp
                            TempOnes=u8KeyPadPressed-ASSCI_ZERO;//Store The Temp Tens In This Variables
                            u8KeyPadPressed=NOT_PRESSED;//Set The Key Pad Value To The Default Value 
                        }
                        Temperature=TempTens*10+TempOnes;//Set The Temperature With The Tempons And TempTens
                        u8SpiTranmitRecieve(SET_TEMPERATURE);//Master asks The Status of The Specific Device
                        _TimerDelay_ms(200);//Halt The System Here for Given Time in Milli Seconds
                        u8SpiTranmitRecieve(Temperature);//Send The Temperature To The Slave To Set The Air Conditioning
                        _TimerDelay_ms(200);//Halt The System Here for Given Time in Milli Seconds
                         lcd_4bit_send_command(&Lcd_Obj,DISPLAY_CLEAR);//Clear The Screen And Set Cursor Home
                         lcd_4bit_send_string_data(&Lcd_Obj,"Temperature Sent");//Display Wrong  To The User
                         _TimerDelay_ms(500);//Halt The System Here for Given Time in Milli Seconds
                    }
                    ShowMenu=AIR_COND_MENU;//Return To The Air Conditioning Menu
                    break;//Break The Temp Menu
            }//End Of The Switch Case
        }//End Of The Show Menu Loop
                
    }//End of While(1) Loop Or  The Main Funtion 
    return (EXIT_SUCCESS);
}

/**Call Back Function In The Timer1 Call it if The Interrupt Happen*/
void Timer1Handler(void){
    SessionCounter++;//Increment The Session Counter Every Timer Interrupt
}

/********Helper Function TO Check if The Pass of the Guest And The Admin Is Set Or Not**********/
uint8 u8PassIsSetOrNotSet(uint16 Address){
    uint8 u8LocDataAdd;
    eeprom_ReadDataByte(Address,&u8LocDataAdd);
    return u8LocDataAdd;
}
/********Helper Function For Write Bytes in The EEPROM*************/
void vWriteBlockOnEEPROM(uint16 u16TheStartAddress,uint8 *Pass,uint8 Size){
    int LOC_Iterator=0;
    for(LOC_Iterator=0;LOC_Iterator<Size;LOC_Iterator++){
        eeprom_WriteDataByte((u16TheStartAddress +LOC_Iterator),*(Pass+LOC_Iterator));
    }
}
/********Helper Function For Read Bytes in The EEPROM*************/
void vReadBlockOnEEPROM(uint16 u16TheStartAddress,uint8 *Pass,uint8 Size){
    int LOC_Iterator=0;
    for(LOC_Iterator=0;LOC_Iterator<Size;LOC_Iterator++){
        eeprom_ReadDataByte((u16TheStartAddress +LOC_Iterator),(Pass+LOC_Iterator));
    }
}