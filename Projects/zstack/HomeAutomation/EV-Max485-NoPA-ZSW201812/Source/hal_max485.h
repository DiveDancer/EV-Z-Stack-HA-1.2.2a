
#ifndef __HAL_MAX485_H__
#define __HAL_MAX485_H__

void hal_max485_init(void);
void hal_max485_receive(uint8 *pdata, int len);
void hal_max485_send(uint8 *pdata, int len);

#endif

