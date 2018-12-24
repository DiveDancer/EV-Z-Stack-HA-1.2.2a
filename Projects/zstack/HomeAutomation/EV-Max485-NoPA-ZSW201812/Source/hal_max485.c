
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
  //������봮�����ݷ���ռ��CPU�������淽ʽ��
  //����max485_active_receive()����_hal_uart_isr.c�����жϴ�������
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
  U1DBUF = ch;         //��Ҫ���͵�1�ֽ�����д��U0DBUF
  while(!UTX1IF);       //�ȴ�TX�жϱ�־�������ݷ������
  UTX1IF = 0;           //���TX�жϱ�־��׼����һ�η���
}
static void max485_active_receive(void)
{
  HAL_EN485_RESET();
}

static void max485_active_send(void)
{
  HAL_EN485_SET();
}