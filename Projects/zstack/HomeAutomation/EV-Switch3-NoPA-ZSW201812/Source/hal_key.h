/**************************************************************************************************
  Filename:       hal_key.h
  Revised:        Date: 2016-01-01 10:42:24
  Revision:       Revision: 001

  Description:    This file contains the interface to the LED Service.
  Copyright 2016-2020 Giturbo Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Giturbo protocol device ,
  which is integrated into your product.  Other than for the foregoing purpose, 
  you may not use, reproduce, copy, prepare derivative works of, modify, 
  distribute, perform, display or sell this Software and/or its documentation 
  for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, 
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, 
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. 

  Should you have any questions regarding your right to use this Software,
  contact Giturbo Incorporated at www.Giturbo.com. 
**************************************************************************************************/


#ifndef HAL_KEY_H
#define HAL_KEY_H

#ifdef __cplusplus
extern "C"
{
#endif

/**************************************************************************************************
 *                                             INCLUDES
 **************************************************************************************************/
#include "hal_board.h"
  
/**************************************************************************************************
 * MACROS
 **************************************************************************************************/

/**************************************************************************************************
 *                                            CONSTANTS
 **************************************************************************************************/
typedef struct
{
  uint8 state:4;
  uint8 shift:4;
  uint8 key;
  uint16 runtime;
}HalKey_t;

#define KEY0_VALUE              0x01
#define KEY1_VALUE              0x02
#define KEY2_VALUE              0x04
#define KEY3_VALUE              0x08
#define KEY4_VALUE              0x10
#define KEY5_VALUE              0x20

#define KEY_SHIFT_INVALID       0x00
#define KEY_SHIFT_DOWN          0x01
#define KEY_SHIFT_KEEP          0x02
#define KEY_SHIFT_UP            0x03

/**************************************************************************************************
 * @fn      HalKeyInit
 * @brief   初始化按键
* @return  
 **************************************************************************************************/
void  HalKeyInit( void );
/**************************************************************************************************
 * @fn      HalKeyRead
 * @brief   Read the current value of a key
* @param   *key:接收按键值，titcks:调用时间间隔，
* @return  返回按键状态:0x00无效，01按下，02保持，03弹起
 **************************************************************************************************/
uint8 HalKeyRead (uint8 *key,uint8 ticks);
/**************************************************************************************************
 * @fn      HalKeyRead
 * @brief   读按键保持状态下的时间，单位ms
* @return  返回按下时间
 **************************************************************************************************/
uint16 HalKeyGetRuntim(void);

void  HalKeyEnterSleep ( void );
uint8 HalKeyExitSleep (uint8 *key, uint8 ticks);

#ifdef __cplusplus
}
#endif

#endif
