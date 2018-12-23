/*********************************************************************
 * INCLUDES
 */
#include "ZComDef.h"
#include "OSAL.h"
#include "AF.h"
#include "ZDConfig.h"

#include "zcl.h"
#include "zcl_general.h"
#include "zcl_ha.h"
#include "zcl_ezmode.h"
#include "zcl_poll_control.h"
#include "zcl_electrical_measurement.h"
#include "zcl_diagnostic.h"
#include "zcl_meter_identification.h"
#include "zcl_appliance_identification.h"
#include "zcl_appliance_events_alerts.h"
#include "zcl_power_profile.h"
#include "zcl_appliance_control.h"
#include "zcl_appliance_statistics.h"
#include "zcl_hvac.h"
#include "hal_led.h"
#include "hal_drivers.h"
#include "AppCommon.h"
#include "AppTask.h"
#include "Point1_Switch.h"
#include "Point2_Switch.h"
/*********************************************************************
 * TYPEDEFS
 */
uint16 AppCommon_IdentifyTime;
uint8  AppCommonSeqNum = 0;
/*********************************************************************
 * MACROS
 */
#ifdef ZCL_EZMODE
uint8  AppCommon_IdentifyCommissionState;
#endif

#pragma location = "D_LOCK_BITS_ADDRESS_SPACE_START" 
const uint8 halFlashLockCfg[16] = {0};
//__root const char arry[]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x3f};

/*********************************************************************
 * GLOBAL VARIABLES
 */
const uint8 AppCommon_ManufacturerName[] = { 16, 'J','i','f','a','n',' ','I','o','t',' ',' ',' ',' ',' ',' ',' ' };
const uint8 AppCommon_ModelId[] = { 16, 'C','C','2','5','3','0',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };
const uint8 AppCommon_DateCode[] = { 16, '2','0','1','8','1','0','0','1',' ',' ',' ',' ',' ',' ',' ',' ' };
const uint8 AppCommon_PowerSource = POWER_SOURCE_MAINS_1_PHASE;//POWER_SOURCE_MAINS_1_PHASE;

uint8 AppCommon_LocationDescription[] = { 16, ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };
uint8 AppCommon_PhysicalEnvironment = 0;
uint8 AppCommon_DeviceEnable = DEVICE_ENABLED;

void AppCommon_BasicResetCB( void )
{
  HalLedBlink(0,4,200,200);
  hal_factory_reset(3000);
}

void AppCommon_IdentifyCB( zclIdentify_t *pCmd )
{
  AppCommon_IdentifyTime = pCmd->identifyTime;
  AppCommon_ProcessIdentifyTimeChange();
}

void AppCommon_ProcessIdentifyTimeChange( void )
{
  if ( AppCommon_IdentifyTime > 0 )
  {
    osal_start_timerEx( App_GetTaskID(), APP_IDENTIFY_TIMEOUT_EVT, 1000 );
  }
  else
  {
    osal_stop_timerEx( App_GetTaskID(), APP_IDENTIFY_TIMEOUT_EVT );
  }
}
void AppCommon_IdentifyQueryRspCB(  zclIdentifyQueryRsp_t *pRsp )
{
  (void)pRsp;
#ifdef ZCL_EZMODE
  {
    zclEZMode_ActionData_t data;
    data.pIdentifyQueryRsp = pRsp;
    zcl_EZModeAction ( EZMODE_ACTION_IDENTIFY_QUERY_RSP, &data );
  }
#endif
}
uint8 AppCommon_GetSeqNum(void)
{
  return AppCommonSeqNum++;
}
float AppCommon_ReadVDD(void)
{
    float v;
    uint16 temp;
    HalAdcSetReference(HAL_ADC_REF_125V);
    temp = HalAdcRead(HAL_ADC_CHANNEL_VDD,HAL_ADC_RESOLUTION_8);//
    if(temp&0x80)//¸ºÖµ
      temp = 0x00;
    v = (temp/128.0)*1.15*3;
    return v;
}
void AppCommon_FactoryReset(void)
{
  uint8 exAddr[8];
  NLME_LeaveReq_t leaveReq;
  osal_memset( &leaveReq, 0, sizeof( NLME_LeaveReq_t ) );
  leaveReq.extAddr = exAddr;
  osal_memcpy(leaveReq.extAddr,NLME_GetExtAddr(),Z_EXTADDR_LEN);
  leaveReq.rejoin = FALSE;
  leaveReq.removeChildren=FALSE;
  if ( NLME_LeaveReq( &leaveReq ) != ZSuccess )
  {
    ZDApp_LeaveReset( FALSE );
  }
  HalLedBlink(0,0xFFFF,100,100);
  hal_factory_reset(2500);
}

void AppCommon_ReportSwtichState( uint8 ep )
{
  static uint8   state = 0x00;
  afAddrType_t   DstAddr;
  zclReportCmd_t *pCmd;
  AppCommon_UpdateValue();
  pCmd = (zclReportCmd_t*)osal_mem_alloc(sizeof(zclReportCmd_t)+sizeof(zclReport_t));
  if(pCmd!=NULL)
  {
    switch(ep)
    {
      case defEndPoint1:
        state = HalLampGet(0);
        break;
      case defEndPoint2:
        state = HalLampGet(1);
        break;
    }
    pCmd->numAttr = 0x01;
    pCmd->attrList[0].attrID = ATTRID_ON_OFF;
    pCmd->attrList[0].attrData = &state;
    pCmd->attrList[0].dataType = ZCL_DATATYPE_UINT8;
    DstAddr.addr.shortAddr = 0x0000;
    DstAddr.addrMode = afAddr16Bit;
    DstAddr.endPoint = 0x01;
    DstAddr.panId = 0x0000;
    zcl_SendReportCmd(ep,&DstAddr,ZCL_CLUSTER_ID_GEN_ON_OFF,pCmd,ZCL_FRAME_SERVER_CLIENT_DIR,FALSE,AppCommon_GetSeqNum());
    osal_mem_free(pCmd);
  }
}
void AppCommon_UpdateValue(void)
{
  Point1_OnOff = HalLampGet(0);
  Point2_OnOff = HalLampGet(1);
}
void AppCommon_ReportVoltage(uint8 check)
{
  afAddrType_t   DstAddr;
  uint8 voltage;
  uint8 alarm = 0x00;
  zclReportCmd_t *pCmd;
  voltage = (uint8)(AppCommon_ReadVDD()*10);
  if(((check)&&(voltage<=25))||(check==0))
  {
    uint8 slen = sizeof(zclReportCmd_t)+(sizeof(zclReport_t)*2);
    pCmd = (zclReportCmd_t*)osal_mem_alloc(slen);
    if(pCmd!=NULL)
    {
      pCmd->numAttr = 0x02;
      pCmd->attrList[0].attrID = ATTRID_POWER_CFG_BATTERY_VOLTAGE;//power voltage
      pCmd->attrList[0].attrData = &voltage;
      pCmd->attrList[0].dataType = ZCL_DATATYPE_UINT8;
      
      pCmd->attrList[1].attrID = ATTRID_POWER_CFG_BAT_ALARM_STATE;//battery alarm like this:BAT_ALARM_MASK_VOLT_2_LOW
      if(voltage<=26)
      {
        alarm = BAT_ALARM_MASK_VOLT_2_LOW;
      }
      pCmd->attrList[1].attrData = &alarm;
      pCmd->attrList[1].dataType = ZCL_DATATYPE_UINT8;
      
      DstAddr.addr.shortAddr = 0x0000;
      DstAddr.addrMode = afAddr16Bit;
      DstAddr.endPoint = 0xFF;
      DstAddr.panId = 0x0000;
      zcl_SendReportCmd(defEndPoint1,&DstAddr,ZCL_CLUSTER_ID_GEN_POWER_CFG,pCmd,ZCL_FRAME_SERVER_CLIENT_DIR,TRUE,AppCommon_GetSeqNum());
      osal_mem_free(pCmd);
    }
  }
}


