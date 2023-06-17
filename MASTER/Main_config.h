/* 
 * File:   Main_config.h
 * Author: MOUSTAFA
 *
 * Created on June 9, 2023, 6:21 AM
 */

#ifndef MAIN_CONFIG_H
#define	MAIN_CONFIG_H

/* Section : Macros Declarations  */
#define NOT_STORD    0XFF
#define NOT_SELECTED 0XFF

#define BLOCK_MODE_TIME       (uint32)20000
#define CHARACTER_REVIEW_TIME (uint16)300
#define EDGRESS_SYMBOL        (uint8)0XDF

/***********************PIN Configuration****************************************/
#define PASS_SIZE  (uint8)4
#define TRIES_ALLOWD (uint8)3
#define PASSWORD_SYMBOL (uint8)'*'
#define ASSCI_ZERO (uint8)'0'

/***********************Configuration For The Password **************************/
#define PASS_NOT_SET  (uint8)0XFF
#define PASS_SET      (uint8)0X00

#define ADMIN_PASS_ADDRESS_STATUS  (uint16)0X0000
#define GEUST_PASS_ADDRESS_STATUS  (uint16)0X0001
#define BLOCK_PASS_ADDRESS_STATUS  (uint16)0X0002
#define ADMIN_PASS_ADDRESS_START   (uint16)0X0003
#define GEUST_PASS_ADDRESS_START   (uint16)0X000A

/*Configuration For Selection of The Inputs*/
#define CHECK_ADMIN_MODE (uint8)'0'
#define CHECK_GUEST_MODE (uint8)'1'

#define SELECT_ROOM1      (uint8)'1'
#define SELECT_ROOM2      (uint8)'2'
#define SELECT_ROOM3      (uint8)'3'
#define SELECT_ROOM4      (uint8)'4'
#define ADMIN_MORE_OPTION (uint8)'4'

#define SELECT_ROOM4_ADMIN      (uint8)'1'
#define SELECT_TV               (uint8)'2'
#define SELECT_AIR_CONDTIONING  (uint8)'3'
#define ADMIN_RET_OPTION        (uint8)'4'

#define SELECT_SET_TEMPERATURE  (uint8)'1'
#define SELECT_AIR_COND_CTRL    (uint8)'2'
#define SELECT_AIR_COND_RET     (uint8)'0'
/*******************Login Configuration**************/
#define NO_MODE      (uint8)0
#define ADMIN_MODE   (uint8)1
#define GUEST_MODE   (uint8)2

/***********Logic Configuration ****************/
#define FALSE (uint8)0
#define TRUE  (uint8)1

#define ADMIN_TIMEOUT (uint16)3000
#define GUEST_TIMEOUT (uint16)2000

/*****************Show Menu Configuraion ***************/
#define MAIN_MENU            (uint8)1
#define MORE_MENU            (uint8)2
#define ROOM1_MENU           (uint8)3
#define ROOM2_MENU           (uint8)4
#define ROOM3_MENU           (uint8)5
#define ROOM4_MENU           (uint8)6
#define TV_MENU              (uint8)7
#define AIR_COND_MENU        (uint8)8
#define AIR_CONTROL_MENU     (uint8)9
#define TEMP_MENU            (uint8)10


#define NOT_PRESSED 0XFF
#endif	/* MAIN_CONFIG_H */

