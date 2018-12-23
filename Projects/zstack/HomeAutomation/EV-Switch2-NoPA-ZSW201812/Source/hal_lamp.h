
#ifndef __HAL_LAMP__
#define __HAL_LAMP__

#define HAL_LAMP_DEFAULT_MAX_LAMPS      3
#define HAL_LAMP_OFF     0x00
#define HAL_LAMP_ON      0x01

void  HalLampInit (void);
void  HalLampSet(uint8 lamp, uint8 val);
void  HalLampToggle(uint8 lamp);
uint8 HalLampGet(uint8 lamp);

#endif

