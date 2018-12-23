/**************************************************************************************************
  Filename:       hal_led.h
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

#ifndef HAL_LED_H
#define HAL_LED_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "hal_board.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/* LEDS - The LED number is the same as the bit position */
#define HAL_LED_0     0x00
#define HAL_LED_1     0x01
#define HAL_LED_2     0x02
#define HAL_LED_3     0x03

/* Modes */
#define HAL_LED_MODE_OFF     0x00
#define HAL_LED_MODE_ON      0x01
#define HAL_LED_MODE_BLINK   0x02
#define HAL_LED_MODE_FLASH   0x04
#define HAL_LED_MODE_TOGGLE  0x08

/* Defaults */
#define HAL_LED_DEFAULT_MAX_LEDS      1
#define HAL_LED_TICKS           20

  /* 1 - Green */
#define HAL_LED0_BV           BV(7)
#define HAL_LED0_PORT         P1
#define HAL_LED0_DIR          P1DIR
#define HAL_LED0_POLARITY     ACTIVE_HIGH
#define HAL_LED0_OUT()        (HAL_LED0_DIR |= HAL_LED0_BV)
#define HAL_LED0_SET()        (HAL_LED0_PORT &= ~HAL_LED0_BV)//(HAL_LED0_PORT |= HAL_LED0_BV)
#define HAL_LED0_RESET()      (HAL_LED0_PORT |= HAL_LED0_BV)//
#define HAL_LED0_GET()        (HAL_LED0_PORT&HAL_LED0_BV)
  
/* 1 - Green 
#define HAL_LED0_BV           BV(4)
#define HAL_LED0_PORT         P0
#define HAL_LED0_DIR          P0DIR
#define HAL_LED0_POLARITY     ACTIVE_HIGH
#define HAL_LED0_OUT()        (HAL_LED0_DIR |= HAL_LED0_BV)
#define HAL_LED0_SET()        (HAL_LED0_PORT &= ~HAL_LED0_BV)//(HAL_LED0_PORT |= HAL_LED0_BV)
#define HAL_LED0_RESET()      (HAL_LED0_PORT |= HAL_LED0_BV)//(HAL_LED0_PORT &= ~HAL_LED0_BV)
#define HAL_LED0_GET()        (HAL_LED0_PORT&HAL_LED0_BV)
   */
/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * GLOBAL VARIABLES
 */

/*
 * Initialize LED Service.
 */
extern void HalLedInit( void );

/*
 * Set the LED ON/OFF/TOGGLE.
 */
extern uint8 HalLedSet( uint8 led, uint8 mode );

/*
 * Blink the LED.
 */
extern void HalLedBlink(uint8 led, uint16 numBlinks, uint16 ontime, uint16 offtime);

extern void HalLedUpdate (void);
/*
 * Put LEDs in sleep state - store current values
 */
extern void HalLedEnterSleep( void );

/*
 * Retore LEDs from sleep state
 */
extern void HalLedExitSleep( void );

/*
 * Return LED state
 */
extern uint8 HalLedGetState(uint8 led);

extern void NwkLedBlink(uint16 num, uint16 htime, uint16 ltime);
/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif
