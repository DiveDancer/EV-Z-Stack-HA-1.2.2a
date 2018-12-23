
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

#define HAL_LAMP1_BV           BV(4)
#define HAL_LAMP1_PORT         P1
#define HAL_LAMP1_DIR          P1DIR
#define HAL_LAMP1_POLARITY     ACTIVE_HIGH
#define HAL_LAMP1_OUT()        (HAL_LAMP1_DIR |= HAL_LAMP1_BV)
#define HAL_LAMP1_SET()        (HAL_LAMP1_PORT &= ~HAL_LAMP1_BV)
#define HAL_LAMP1_RESET()      (HAL_LAMP1_PORT |= HAL_LAMP1_BV)
#define HAL_LAMP1_GET()        (!(HAL_LAMP1_PORT&HAL_LAMP1_BV))

#define HAL_LAMP2_BV           BV(2)
#define HAL_LAMP2_PORT         P1
#define HAL_LAMP2_DIR          P1DIR
#define HAL_LAMP2_POLARITY     ACTIVE_HIGH
#define HAL_LAMP2_OUT()        (HAL_LAMP2_DIR |= HAL_LAMP2_BV)
#define HAL_LAMP2_SET()        (HAL_LAMP2_PORT &= ~HAL_LAMP2_BV)
#define HAL_LAMP2_RESET()      (HAL_LAMP2_PORT |= HAL_LAMP2_BV)
#define HAL_LAMP2_GET()        (HAL_LAMP2_PORT & HAL_LAMP2_BV)

void HalLampInit (void)
{
  HAL_LAMP0_OUT();
  HAL_LAMP0_RESET();
}
#include "hal_led.h"
void HalLampSet(uint8 lamp, uint8 val)
{
  HalLedSet(0,val);
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
      {
        HalLedSet(0,0);
        HAL_LAMP0_RESET();
      }
      else
      {
        HalLedSet(0,1);
        HAL_LAMP0_SET();    
      }
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
      if(!HAL_LAMP0_GET())
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