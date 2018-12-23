/**************************************************************************************************
  Filename:       hal_lamp.c
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

#include "hal_mcu.h"
#include "hal_defs.h"
#include "hal_types.h"
#include "hal_board.h"
#include "hal_drivers.h"
#include "hal_lamp.h"
#include "osal.h"

/* 1 - Green */
#define HAL_LAMP0_BV           BV(4)
#define HAL_LAMP0_PORT         P1
#define HAL_LAMP0_DIR          P1DIR
#define HAL_LAMP0_POLARITY     ACTIVE_HIGH
#define HAL_LAMP0_OUT()        (HAL_LAMP0_DIR |= HAL_LAMP0_BV)
#define HAL_LAMP0_SET()        (HAL_LAMP0_PORT &= ~HAL_LAMP0_BV)
#define HAL_LAMP0_RESET()      (HAL_LAMP0_PORT |= HAL_LAMP0_BV)
#define HAL_LAMP0_GET()        (!(HAL_LAMP0_PORT&HAL_LAMP0_BV))

#define HAL_LAMP1_BV           BV(5)
#define HAL_LAMP1_PORT         P1
#define HAL_LAMP1_DIR          P1DIR
#define HAL_LAMP1_POLARITY     ACTIVE_HIGH
#define HAL_LAMP1_OUT()        (HAL_LAMP1_DIR |= HAL_LAMP1_BV)
#define HAL_LAMP1_SET()        (HAL_LAMP1_PORT &= ~HAL_LAMP1_BV)
#define HAL_LAMP1_RESET()      (HAL_LAMP1_PORT |= HAL_LAMP1_BV)
#define HAL_LAMP1_GET()        (!(HAL_LAMP1_PORT&HAL_LAMP1_BV))

#define HAL_LAMP2_BV           BV(6)
#define HAL_LAMP2_PORT         P1
#define HAL_LAMP2_DIR          P1DIR
#define HAL_LAMP2_POLARITY     ACTIVE_HIGH
#define HAL_LAMP2_OUT()        (HAL_LAMP2_DIR |= HAL_LAMP2_BV)
#define HAL_LAMP2_SET()        (HAL_LAMP2_PORT &= ~HAL_LAMP2_BV)
#define HAL_LAMP2_RESET()      (HAL_LAMP2_PORT |= HAL_LAMP2_BV)
#define HAL_LAMP2_GET()        (!(HAL_LAMP2_PORT & HAL_LAMP2_BV))

void HalLampInit (void)
{
  HAL_LAMP0_OUT();
  HAL_LAMP1_OUT();
  HAL_LAMP2_OUT();
  HAL_LAMP0_RESET();
  HAL_LAMP1_RESET();
  HAL_LAMP2_RESET();
}
void HalLampSet(uint8 lamp, uint8 val)
{
  if(lamp<HAL_LAMP_DEFAULT_MAX_LAMPS)
  {
    switch(lamp)
    {
    case 0x00:
      if(val)
        HAL_LAMP0_SET();
      else
        HAL_LAMP0_RESET();
      break;
    case 0x01:
      if(val)
        HAL_LAMP1_SET();
      else
        HAL_LAMP1_RESET();
      break;
    case 0x02:
      if(val)
        HAL_LAMP2_SET();
      else
        HAL_LAMP2_RESET();
      break;
    }
  }
}
void HalLampToggle(uint8 lamp)
{
  if(lamp<HAL_LAMP_DEFAULT_MAX_LAMPS)
  {
    switch(lamp)
    {
    case 0x00:
      if(HAL_LAMP0_GET())
        HAL_LAMP0_RESET();
      else
        HAL_LAMP0_SET();        
      break;
    case 0x01:
      if(HAL_LAMP1_GET())
        HAL_LAMP1_RESET();
      else
        HAL_LAMP1_SET();
      break;
     case 0x02:
      if(HAL_LAMP2_GET())
        HAL_LAMP2_RESET();
      else
        HAL_LAMP2_SET();
      break;
    }
  }
}
uint8 HalLampGet(uint8 lamp)
{
  if(lamp<HAL_LAMP_DEFAULT_MAX_LAMPS)
  {
    switch(lamp)
    {
    case 0x00:
      if(HAL_LAMP0_GET())
        return HAL_LAMP_ON;
      else
        return HAL_LAMP_OFF;
    case 0x01:
      if(HAL_LAMP1_GET())
        return HAL_LAMP_ON;
      else
        return HAL_LAMP_OFF;
    case 0x02:
      if(HAL_LAMP2_GET())
        return HAL_LAMP_ON;
      else
        return HAL_LAMP_OFF;
    }
  }
  return HAL_LAMP_OFF;
}