#ifndef __OS_PRINTF_H__
#define __OS_PRINTF_H__

#include "stdio.h"

#define DBG_ENABLE      0
#define DBG_UART        0

#define DBG_MAC         0
#define DBG_ZDO         0
#define DBG_NWK         0
#define DBG_APP         0
#define DBG_SYS         0

#define LOGD_ENABLE     0
#define LOGI_ENABLE     0
#define LOGE_ENABLE     0

extern void osUartInit(void);
extern void osUartDeinit(void);
void osPrint2b(char *s, uint8 *pdata, int len);

#define osPrintf(flag,...)   \
do{\
  if(flag)\
  printf(__VA_ARGS__);\
}while(0)

#define osPrintb(flag,...) \
do{\
  if(flag)\
    osPrint2b(__VA_ARGS__);\
}while(0)

#define osLogD(flag,...)\
do{\
  if(flag&&LOGD_ENABLE){\
    printf("Log -D, %s() line:%d,\t",__func__,__LINE__);\
    printf(__VA_ARGS__);\
  }\
}while(0)

#define osLogI(flag,...)\
do{\
  if(flag&&LOGI_ENABLE){\
    printf("Log -I, %s() line:%d,\t",__func__,__LINE__);\
    printf(__VA_ARGS__);\
  }\
}while(0)

#define osLogE(flag,...)\
do{\
  if(flag&&LOGE_ENABLE){\
    printf("Log -E, %s() line:%d,\t",__func__,__LINE__);\
    printf(__VA_ARGS__);\
  }\
}while(0)
 


#endif

