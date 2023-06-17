/* 
 * File:   STD_Messeges.h
 * Author: MOUSTAFA
 *
 * Created on June 9, 2023, 5:41 AM
 */

#ifndef STD_MESSEGES_H
#define	STD_MESSEGES_H

/**********************Macros Decelerations********************/
#define ROOM1_STATUS    0X11
#define ROOM2_STATUS    0X12
#define ROOM3_STATUS    0X13
#define ROOM4_STATUS    0X14
#define TV_STATUS       0X15
#define AIR_COND_STATUS 0X16

#define ROOM1_TURN_ON    0X21
#define ROOM2_TURN_ON    0X22
#define ROOM3_TURN_ON    0X23
#define ROOM4_TURN_ON    0X24
#define TV_TURN_ON       0X25
#define AIR_COND_TURN_ON 0X26

#define ROOM1_TURN_OFF    0X31
#define ROOM2_TURN_OFF    0X32
#define ROOM3_TURN_OFF    0X33
#define ROOM4_TURN_OFF    0X34
#define TV_TURN_OFF       0X35
#define AIR_COND_TURN_OFF 0X36

#define SET_TEMPERATURE 0X40

#define DEFAULT_ACK     0XFF
#define DEMAND_RESPONSE 0XFF

#define ON_STATUS   0X01
#define OFF_STATUS  0X00
#endif	/* STD_MESSEGES_H */

