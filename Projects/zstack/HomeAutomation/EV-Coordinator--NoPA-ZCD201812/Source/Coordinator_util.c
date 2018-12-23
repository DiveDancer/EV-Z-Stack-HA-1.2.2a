/**************************************************************************************************
  Filename:       Coordinator_util.c
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
/*********************************************************************
 * INCLUDES
 */
#include "ZComDef.h"
#include "OSAL.h"
#include "AF.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "MT_SYS.h"
#include "MT_APP.h"
#include "MT_APP1.h"
#include "nwk_util.h"
#include "zcl.h"
#include "zcl_ss.h"
#include "zcl_ha.h"
#include "zcl_ezmode.h"
#include "zcl_diagnostic.h"
#include "zcl_general.h"
#include "Coordinator.h"
#include "Coordinator_util.h"
#include "onboard.h"
/* HAL */
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"

#define SS_IAS_WD_ALARM_DEFAULT_DURATION    0x0014  


#if ( defined (ZGP_DEVICE_TARGET) || defined (ZGP_DEVICE_TARGETPLUS) \
      || defined (ZGP_DEVICE_COMBO) || defined (ZGP_DEVICE_COMBO_MIN) )
#include "zgp_translationtable.h"
  #if (SUPPORTED_S_FEATURE(SUPP_ZGP_FEATURE_TRANSLATION_TABLE))
    #define ZGP_AUTO_TT
  #endif
#endif

#if (defined HAL_BOARD_ZLIGHT) || (defined HAL_PWM)
#include "math.h"
#include "hal_timer.h"
#endif

#include "NLMEDE.h"


void zcl_BasicResetCB( void )
{

  if(ZG_BUILD_COORDINATOR_TYPE && ZG_DEVICE_COORDINATOR_TYPE)
  {
    // sending leave does nothing for ZC
  }
  else
  {
    // remove self from network if ZR or ZED
    NLME_LeaveReq_t req;
    req.extAddr = NULL;
    req.removeChildren = 0;
    req.rejoin = 0;
    req.silent = 0;
    NLME_LeaveReq(&req);
  }
}

void zcl_IdentifyCB( zclIdentify_t *pCmd )
{
 // zclSampleCIE_IdentifyTime = pCmd->identifyTime;
  //zclSampleCIE_ProcessIdentifyTimeChange();
}
void zcl_IdentifyQueryRspCB( zclIdentifyQueryRsp_t *pRsp )
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

/*******************************************************************************
* @fn      static void zclSampleCIE_ChangeNotificationCB
*
* @brief   Process incoming Change Notification Command
*
* @param   Zone Status Change Notification command
*
* @return  none
*/
ZStatus_t zcl_ChangeNotificationCB(zclZoneChangeNotif_t *pCmd, afAddrType_t *srcAddr )
{
  
  //building the warning msg
  zclWDStartWarning_t alarm;
  alarm.warningDuration = SS_IAS_WD_ALARM_DEFAULT_DURATION;
  
  //displaying messages
  switch(pCmd->zoneStatus)
  {
    //Here only implementing fire alarm (1st bit), other bits not checked
  case 0://since zoneStatus is a bit mask
    {
      //setting params for the Warning Message
      alarm.warningmessage.warningbits.warnMode = SS_IAS_START_WARNING_WARNING_MODE_STOP;
      alarm.warningmessage.warningbits.warnStrobe = SS_IAS_START_WARNING_STROBE_NO_STROBE_WARNING;   
    }
    break;
    
  case SS_IAS_ZONE_STATUS_ALARM1_ALARMED:
    {
      //setting params for the Warning Message
      alarm.warningmessage.warningbits.warnMode = SS_IAS_START_WARNING_WARNING_MODE_FIRE;
      alarm.warningmessage.warningbits.warnStrobe = SS_IAS_START_WARNING_STROBE_USE_STPOBE_IN_PARALLEL_TO_WARNING;
    }
    break;
    
  default:
    break;
  }
  
  zclSendWarningToAllWD(&alarm);
  return ZSuccess ;
}
/*******************************************************************************

* @fn      static void zclSendWarningToAllWD
*
* @brief   sends warning message to all WD in WD table
*
* @param   uint16 network addr of alarm origininator
*
* @return  none
*/
void zclSendWarningToAllWD(zclWDStartWarning_t *alarm)
{
  // use the binding table as dstAddr
  afAddrType_t dstAddr;
  dstAddr.addrMode = (afAddrMode_t)AddrNotPresent;
  dstAddr.endPoint = 0;
  dstAddr.addr.shortAddr = 0;
  zclSS_Send_IAS_WD_StartWarningCmd(SAMPLELIGHT_ENDPOINT, &dstAddr, alarm, 1, 1);
}
ZStatus_t CoordinatorReceiveCB( zclIncoming_t *pMsg )
{
  uint8 *pbuf;
  uint8 dlen;
  dlen = pMsg->pDataLen;
  dlen += 6;
  pbuf = (uint8*)osal_mem_alloc(dlen);
  if(pbuf!=NULL)
  {
    //if(pMsg->hdr.commandID==ZCL_CMD_DEFAULT_RSP)
    //  osLogI(DBG_APP,"Receive Response From %04x,%02x,%02x\r\n",pMsg->msg->srcAddr.addr.shortAddr,pMsg->msg->srcAddr.endPoint,pMsg->hdr.transSeqNum);
    //else
    //  osLogI(DBG_APP,"Receive Data     From %04x,%02x,%02x\r\n",pMsg->msg->srcAddr.addr.shortAddr,pMsg->msg->srcAddr.endPoint,pMsg->hdr.transSeqNum);
    pbuf[0] = pMsg->hdr.commandID;
    pbuf[1] = pMsg->hdr.fc.type;
    pbuf[2] = pMsg->hdr.fc.direction;
    osU16toB(&pbuf[3],pMsg->hdr.manuCode);
    pbuf[5] = pMsg->pDataLen;
    osal_memcpy(&pbuf[6],pMsg->pData,pMsg->pDataLen);
    MT_App_ReportStatue(MT_CmdAppSendCommand,pMsg->msg->srcAddr.addr.shortAddr,pMsg->msg->srcAddr.endPoint,pMsg->hdr.transSeqNum,0x00,pMsg->msg->clusterId,pbuf,dlen);
    osal_mem_free(pbuf); 
  }
  //extern void MT_App_ReportStatue(uint8 cmd,uint16 src, uint8 spoint,uint8 seq,uint8 option,uint16 cID,uint8 *pdata, uint8 len);
  return 0;
}
/*********************************************************************
 * @fn      zclSampleLight_ProcessInDefaultRspCmd
 *
 * @brief   Process the "Profile" Default Response Command
 *
 * @param   pInMsg - incoming message to process
 *
 * @return  none
 */
uint8 ProcessInDefaultRspCmd( zclIncomingMsg_t *pInMsg )
{
  // zclDefaultRspCmd_t *defaultRspCmd = (zclDefaultRspCmd_t *)pInMsg->attrCmd;

  // Device is notified of the Default Response command.
  (void)pInMsg;

  return ( TRUE );
}
/*******************************************************************************
* @fn      static void zclSampleCIE_EnrollRequestCB
*
* @brief   Process incoming Zone Enroll Request Command
*
* @param   Zone Enroll Request command
*
* @return  none
*/
ZStatus_t zcl_EnrollRequestCB(zclZoneEnrollReq_t *pReq, uint8 endpoint )
{
  //create temporary placeholder for IEEE address
  uint8 extAddr[Z_EXTADDR_LEN];
  
  //copy the IEEE Address to the temp var
  if(APSME_LookupExtAddr(pReq->srcAddr->addr.shortAddr, extAddr))
  {    
    //fill in the enrolling Device IEEE Address in the zone record
    zclSS_UpdateZoneAddress( pReq->srcAddr->endPoint, pReq->zoneID, extAddr );
  }

  return ZSuccess ;
}




