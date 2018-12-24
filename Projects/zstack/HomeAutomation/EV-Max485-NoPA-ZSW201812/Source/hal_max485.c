
#include "ZComDef.h"
#include "OSAL.h"
#include "AF.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "MT_SYS.h"
#include "MT_UART.h"
#include "nwk_util.h"
#include "zcl.h"
#include "zcl_general.h"
#include "zcl_ha.h"
#include "zcl_ezmode.h"
#include "zcl_diagnostic.h"
#include "AppTask.h"
#include "onboard.h"
#include "hal_max485.h"

#define HAL_EN485_BV           BV(4)
#define HAL_EN485_PORT         P1
#define HAL_EN485_DIR          P1DIR
#define HAL_EN485_POLARITY     ACTIVE_HIGH
#define HAL_EN485_OUT()        (HAL_EN485_DIR |= HAL_EN485_BV)
#define HAL_EN485_SET()        (HAL_EN485_PORT |= HAL_EN485_BV)
#define HAL_EN485_RESET()      (HAL_EN485_PORT &= ~HAL_EN485_BV)
#define HAL_EN485_GET()        (!(HAL_EN485_PORT&HAL_EN485_BV))

static void max485_active_receive(void);
static void max485_active_send(void);
static void max485_write_byte(uint8 ch);

void hal_max485_init(void)
{
  HAL_EN485_OUT();
  max485_active_receive();
}
static uint8 rxbuf[255];
static uint8 rxcnt = 0;
void hal_max485_receive(uint8 *pdata, int len)
{
  if((pdata!=NULL)&&(len!=0))
  {
    osal_memcpy(&rxbuf[rxcnt],pdata,len);
    rxcnt += len;
  }
  else//time out and finished receive echo to pc
  {
    hal_max485_send(rxbuf,rxcnt);
    rxcnt = 0;
  }
}
void hal_max485_send(uint8 *pdata, int len)
{
  int i;
  max485_active_send();
  //如果不想串口数据发送占用CPU可用下面方式，
  //并把max485_active_receive()移至_hal_uart_isr.c发送中断处理函数中
  //HalUARTWrite(MT_UART_DEFAULT_PORT,pdata,len);
  for(i=0;i<len;i++)
  {
    max485_write_byte(pdata[i]);
  }
  max485_write_byte(0x00);
  max485_active_receive();
}

static void max485_write_byte(uint8 ch)
{
  U1DBUF = ch;         //将要发送的1字节数据写入U0DBUF
  while(!UTX1IF);       //等待TX中断标志，即数据发送完成
  UTX1IF = 0;           //清除TX中断标志，准备下一次发送
}
static void max485_active_receive(void)
{
  HAL_EN485_RESET();
}

static void max485_active_send(void)
{
  HAL_EN485_SET();
}