

/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
#include "hal_mcu.h"
#include "hal_defs.h"
#include "hal_types.h"
#include "hal_board.h"
#include "hal_drivers.h"
#include "hal_adc.h"
#include "hal_key.h"
#include "osal.h"

/* SW_6 is at P0.1 */
#define HAL_KEY0_PORT   P0
#define HAL_KEY0_BIT    BV(2)
#define HAL_KEY0_SEL    P0SEL
#define HAL_KEY0_DIR    P0DIR

#define HAL_KEY1_PORT   P0
#define HAL_KEY1_BIT    BV(2)
#define HAL_KEY1_SEL    P0SEL
#define HAL_KEY1_DIR    P0DIR

#define HAL_KEY2_PORT   P0
#define HAL_KEY2_BIT    BV(2)
#define HAL_KEY2_SEL    P0SEL
#define HAL_KEY2_DIR    P0DIR

#define HAL_KEY3_PORT   P0
#define HAL_KEY3_BIT    BV(2)
#define HAL_KEY3_SEL    P0SEL
#define HAL_KEY3_DIR    P0DIR


static HalKey_t  Key;

static void HalKeyPoll (uint8 ticks);
/**************************************************************************************************
 * @fn      HalKeyInit
 *
 * @brief   Initilize Key Service
 *
 * @param   none
 *
 * @return  None
 **************************************************************************************************/
void HalKeyInit( void )
{
  HAL_KEY0_SEL &= ~(HAL_KEY0_BIT);    /* Set pin function to GPIO */
  HAL_KEY0_DIR &= ~(HAL_KEY0_BIT);    /* Set pin direction to Input */
  osal_memset(&Key,0,sizeof(Key));
}
/**************************************************************************************************
 * @fn      HalKeyRead
 *
 * @brief   Read the current value of a key
 *
 * @param   None
 *
 * @return  keys - current keys status
 **************************************************************************************************/
uint8 HalKeyRead (uint8 *key,uint8 ticks)
{
  uint8 temp;
  HalKeyPoll(ticks);
  *key = Key.key;
  temp = Key.shift;
  Key.shift = 0x00;
  return temp;
}
/**************************************************************************************************
 * @fn      HalKeyPoll
 *
 * @brief   Called by hal_driver to poll the keys
 *
 * @param   None
 *
 * @return  None
 **************************************************************************************************/
static void HalKeyPoll (uint8 ticks)
{
  uint8 keys = 0;
  if (!(HAL_KEY0_PORT&HAL_KEY0_BIT))
  {
    keys |= KEY0_VALUE;
  }
  if(keys)
  {
    if(Key.state==0x00)
    {
      Key.state = 0x01;
      Key.shift = KEY_SHIFT_DOWN;
      Key.runtime = 0x0000;
    }
    else
      Key.shift = KEY_SHIFT_KEEP;
    Key.key = keys;
    Key.runtime+=ticks;
  }
  else if(Key.state)
  {
    Key.state = 0x00;
    Key.shift = KEY_SHIFT_UP;
  }
}
uint16 HalKeyGetRuntim(void)
{
  return Key.runtime;
}
/**************************************************************************************************
 * @fn      HalKeyEnterSleep
 *
 * @brief  - Get called to enter sleep mode
 *
 * @param
 *
 * @return
 **************************************************************************************************/
void HalKeyEnterSleep ( void )
{
}

/**************************************************************************************************
 * @fn      HalKeyExitSleep
 *
 * @brief   - Get called when sleep is over
 *
 * @param
 *
 * @return  - return saved keys
 **************************************************************************************************/
uint8 HalKeyExitSleep (uint8 *key, uint8 ticks)
{
  /* Wake up and read keys */
  return ( HalKeyRead (key,ticks) );
}

/**************************************************************************************************
**************************************************************************************************/



