/**************************************************************************************************
  Filename:       hal_led.c
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
/***************************************************************************************************
 *                                             INCLUDES
 ***************************************************************************************************/
#include "hal_mcu.h"
#include "hal_defs.h"
#include "hal_types.h"
#include "hal_drivers.h"
#include "hal_led.h"
#include "osal.h"
#include "hal_board.h"

/***************************************************************************************************
 *                                              TYPEDEFS
 ***************************************************************************************************/
/* LED control structure */
typedef struct {
  uint8 state;
  uint8 lastState;  
  uint16 blinkNum;
  uint16 onTime;      /* On cycle percentage */
  uint16 offTime;      /* On/off cycle time (msec) */
  uint16 runtime;
} HalLedControl_t;

static void HalLedOnOff (uint8 led, uint8 mode);
/***************************************************************************************************
 *                                           GLOBAL VARIABLES
 ***************************************************************************************************/

static HalLedControl_t HalLedControl[HAL_LED_DEFAULT_MAX_LEDS];
/***************************************************************************************************
 * @fn      HalLedInit
 *
 * @brief   Initialize LED Service
 *
 * @param   init - pointer to void that contains the initialized value
 *
 * @return  None
 ***************************************************************************************************/
void HalLedInit (void)
{
  HAL_LED0_OUT();
  HAL_LED0_RESET();
  osal_memset(&HalLedControl,0,sizeof(HalLedControl));
}
/***************************************************************************************************
 * @fn      HalLedSet
 *
 * @brief   Tun ON/OFF/TOGGLE given LEDs
 *
 * @param   led - bit mask value of leds to be turned ON/OFF/TOGGLE
 *          mode - BLINK, FLASH, TOGGLE, ON, OFF
 * @return  None
 ***************************************************************************************************/
uint8 HalLedSet (uint8 led, uint8 mode)
{
  if(led<HAL_LED_DEFAULT_MAX_LEDS)
  {
    HalLedControl[led].lastState = mode;
    HalLedOnOff(led,mode);
  }
  return mode;
}

/***************************************************************************************************
 * @fn      HalLedBlink
 *
 * @brief   Blink the leds
 *
 * @param   leds       - bit mask value of leds to be blinked
 *          numBlinks  - number of blinks
 *          percent    - the percentage in each period where the led
 *                       will be on
 *          period     - length of each cycle in milliseconds
 *
 * @return  None
 ***************************************************************************************************/
void HalLedBlink(uint8 led, uint16 numBlinks, uint16 ontime, uint16 offtime)
{
  if(led<HAL_LED_DEFAULT_MAX_LEDS)
  {
    if(HalLedControl[led].blinkNum==0)
      osal_set_event(Hal_TaskID, HAL_LED_BLINK_EVENT);
    HalLedControl[led].blinkNum = numBlinks;
    HalLedControl[led].onTime = ontime;
    HalLedControl[led].offTime = offtime;
  }
}
/***************************************************************************************************
 * @fn      HalLedUpdate
 *
 * @brief   Update leds to work with blink
 *
 * @param   none
 *
 * @return  none
 ***************************************************************************************************/
void HalLedUpdate (void)
{
    uint8 i;
    for(i=0;i<HAL_LED_DEFAULT_MAX_LEDS;i++)
    {
      if(HalLedControl[i].blinkNum)
      {
        if(HalLedControl[i].state==HAL_LED_MODE_ON)
        {
          HalLedControl[i].state = HAL_LED_MODE_OFF;
          HalLedOnOff(i,HAL_LED_MODE_OFF);
          osal_start_timerEx( Hal_TaskID, HAL_LED_BLINK_EVENT, HalLedControl[i].offTime );
          HalLedControl[i].blinkNum--;
          if(HalLedControl[i].blinkNum==0x00)
            HalLedOnOff(i,HalLedControl[i].lastState);
        }
        else
        {
            HalLedControl[i].state = HAL_LED_MODE_ON;
            HalLedOnOff(i,HAL_LED_MODE_ON);
            osal_start_timerEx( Hal_TaskID, HAL_LED_BLINK_EVENT, HalLedControl[i].onTime);
        }
      }
    }
}

/***************************************************************************************************
 * @fn      HalLedOnOff
 *
 * @brief   Turns specified LED ON or OFF
 *
 * @param   leds - LED bit mask
 *          mode - LED_ON,LED_OFF,
 *
 * @return  none
 ***************************************************************************************************/
static void HalLedOnOff (uint8 led, uint8 mode)
{
  switch(led)
  {
    case HAL_LED_0:
    if (mode == HAL_LED_MODE_ON)
    {
      HAL_LED0_SET();
    }
    else
    {
      HAL_LED0_RESET();
    }
    break;
  }
}
/***************************************************************************************************
 * @fn      HalGetLedState
 *
 * @brief   Dim LED2 - Dim (set level) of LED2
 *
 * @param   none
 *
 * @return  led state
 ***************************************************************************************************/
uint8 HalLedGetState (uint8 led)
{
  switch(led)
  {
  case HAL_LED_0:
    return HAL_LED0_GET();
  }
  return HAL_LED_MODE_OFF;
}

/***************************************************************************************************
 * @fn      HalLedEnterSleep
 *
 * @brief   Store current LEDs state before sleep
 *
 * @param   none
 *
 * @return  none
 ***************************************************************************************************/
void HalLedEnterSleep( void )
{
  HalLedBlink(HAL_LED_0,0,10,10);
  HalLedSet(HAL_LED_0,HAL_LED_MODE_OFF);
  //app_enter_sleep();
}

/***************************************************************************************************
 * @fn      HalLedExitSleep
 *
 * @brief   Restore current LEDs state after sleep
 *
 * @param   none
 *
 * @return  none
 ***************************************************************************************************/
void HalLedExitSleep( void )
{
  //app_exit_sleep();
}
void NwkLedBlink(uint16 num, uint16 htime, uint16 ltime)
{
    HalLedBlink(0,num,htime,ltime);
}

/***************************************************************************************************
***************************************************************************************************/




