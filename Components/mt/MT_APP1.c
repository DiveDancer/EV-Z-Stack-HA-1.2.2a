/**************************************************************************************************
  Filename:       MT_APP1.h
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
#include "ZComDef.h"
#include "MT.h"        /* This is here because all the SPI_CMD_XXX are defined in this header */
#include "MT_APP.h"
#include "MT_AF.h"     /* This is here because this MT_APP makes some routine call to AF */
#include "MT_RPC.h"

#include "zcl.h"
#include "zcl_ss.h"
#include "zcl_ha.h"
#include "zcl_general.h"
#include "zcl_closures.h"
#include "zcl_lighting.h"

#if defined( APP_TP )
 #include "TestProfile.h"
#endif
#if defined( APP_TP2 )
 #include "TestProfile2.h"
 #include "nwk_util.h"
#endif
#include "MT_APP1.h"

/***************************************************************************************************
 * LOCAL FUNCTIONS
 ***************************************************************************************************/
#if defined (MT_APP_FUNC)
static void MT_AppMsg(uint8 *pBuf);
static void MT_AppUserCmd(byte *pData);
#if defined ( MT_APP_PB_ZCL_FUNC )
static void MT_AppPB_ZCLMsg(byte *pData);
static void MT_AppPB_ZCLCfg(byte *pData);
#endif // MT_APP_PB_ZCL_FUNC
#endif // MT_APP_FUNC




typedef struct
{
  uint8 addr[2];
  uint8 point;
  uint8 seq;
  uint8 disableRsp;
  uint8 cID[2];
  uint8 len;
  uint8 pdata[0];
}APkg_t;

typedef struct
{
  uint8 cmd;
  uint8 specific;
  uint8 direction;
  uint8 manuCode[2];
  uint8 cmdFormatLen;
  uint8 cmdFormat[0];
}SCommand_t;

static uint8 appTransID;

uint16 osBtoU16(uint8 *pdata)
{
  uint16 temp;
  temp = pdata[1];
  temp<<=8;
  temp |= pdata[0];
  return temp;
}
void osU16toB(uint8 *pdata, uint16 val)
{
  pdata[0] = (uint8)val;
  pdata[1] = (uint8)(val>>8);
}
#include "hal_drivers.h"
void MT_App_Hander(uint8 *msg)
{
  endPointDesc_t        *epDesc;
  SCommand_t            *pcmd;
  APkg_t                *pkg;
  afAddrType_t          dst;
  uint16 addr,cID,temp;
  if(msg[MT_RPC_POS_CMD1]==MT_CmdAppFactoryReset)
  {
      hal_factory_reset(100);
  }
  else
  {
    pkg = (APkg_t*)&msg[MT_RPC_FRAME_HDR_SZ];
    addr = osBtoU16(pkg->addr);
    cID = osBtoU16(pkg->cID);
    epDesc = afFindEndPointDesc(SAMPLELIGHT_ENDPOINT);
    if(epDesc==NULL)return;
    dst.addr.shortAddr = addr;
    dst.endPoint = pkg->point;
    dst.addrMode = afAddr16Bit;
    dst.panId = 0x0000;
    switch(msg[MT_RPC_POS_CMD1])
    {
      case MT_CmdAppSendAfData://  = 0x05,
        AF_DataRequest(&dst,epDesc,cID,pkg->len,pkg->pdata,&appTransID,pkg->disableRsp,AF_DEFAULT_RADIUS);
        break;
      case MT_CmdAppSendCommand://  = 0x06,
        pcmd = (SCommand_t*)pkg->pdata;
        temp = osBtoU16(pcmd->manuCode);
       // ZStatus_t zcl_SendCommand( uint8 srcEP, afAddrType_t *destAddr,
                          //   uint16 clusterID, uint8 cmd, uint8 specific, uint8 direction,
                          //   uint8 disableDefaultRsp, uint16 manuCode, uint8 seqNum,
                           //  uint16 cmdFormatLen, uint8 *cmdFormat )
        //osLogI(DBG_APP,"Send data to %04x,%02x,%02x\r\n",dst.addr.shortAddr,dst.endPoint,pkg->seq);
        zcl_SendCommand(SAMPLELIGHT_ENDPOINT,&dst,cID,pcmd->cmd,pcmd->specific,pcmd->direction,pkg->disableRsp,temp,pkg->seq,pcmd->cmdFormatLen,pcmd->cmdFormat);
        break;
case MT_CmdAppSendCmdOff://  = 0x10,
      //ZStatus_t zclGeneral_SendOnOff_CmdOff( uint16 srcEP, afAddrType_t *dstAddr, uint8 disableDefaultRsp, uint8 seqNum );
      zclGeneral_SendOnOff_CmdOff(SAMPLELIGHT_ENDPOINT,&dst,pkg->disableRsp,pkg->seq);
      break;
    case MT_CmdAppSendCmdOn://  = 0x11,
      //ZStatus_t zclGeneral_SendOnOff_CmdOn( uint16 srcEP, afAddrType_t *dstAddr, uint8 disableDefaultRsp, uint8 seqNum );
      zclGeneral_SendOnOff_CmdOn(SAMPLELIGHT_ENDPOINT,&dst,pkg->disableRsp,pkg->seq);
      break;
    case MT_CmdAppSendCmdToggle://  = 0x12,
      //ZStatus_t zclGeneral_SendOnOff_CmdToggle( uint16 srcEP, afAddrType_t *dstAddr, uint8 disableDefaultRsp, uint8 seqNum );
      zclGeneral_SendOnOff_CmdToggle(SAMPLELIGHT_ENDPOINT,&dst,pkg->disableRsp,pkg->seq);
      break;
      /*
    case MT_CmdAppSendCmdUpOpen://= 0x13, ZStatus_t zclClosures_SendUpOpen( uint8 srcEP, afAddrType_t *dstAddr, uint8 disableDefaultRsp, uint8 seqNum )
      zclClosures_SendUpOpen(SAMPLELIGHT_ENDPOINT,&dst,pkg->disableRsp,pkg->seq);
      break;
    case MT_CmdAppSendCmdDonwClose:// = 0x14, ZStatus_t zclClosures_SendDownClose( uint8 srcEP, afAddrType_t *dstAddr, uint8 disableDefaultRsp, uint8 seqNum )
      zclClosures_SendDownClose(SAMPLELIGHT_ENDPOINT,&dst,pkg->disableRsp,pkg->seq);
      break;
    case MT_CmdAppSendCmdStop:// = 0x15, ZStatus_t zclClosures_SendStop( uint8 srcEP, afAddrType_t *dstAddr, uint8 disableDefaultRsp, uint8 seqNum )
      zclClosures_SendStop(SAMPLELIGHT_ENDPOINT,&dst,pkg->disableRsp,pkg->seq);
      break;
  
    case MT_CmdAppSendLevelControlMoveToLevel://  = 0x1A,
      //ZStatus_t zclGeneral_SendLevelControlMoveToLevel( uint16 srcEP, afAddrType_t *dstAddr, uint8 level, uint16 transTime, uint8 disableDefaultRsp, uint8 seqNum );
      temp = osBtoU16(&pkg->pdata[1]);
      zclGeneral_SendLevelControlMoveToLevel(SAMPLELIGHT_ENDPOINT,&dst,pkg->pdata[0],temp,pkg->disableRsp,pkg->seq);
      break;
    case MT_CmdAppSendLevelControlMove://  = 0x1B,
      //ZStatus_t zclGeneral_SendLevelControlMove( uint8 srcEP, afAddrType_t *dstAddr, uint8 moveMode, uint8 rate, uint8 disableDefaultRsp, uint8 seqNum )
      zclGeneral_SendLevelControlMove(SAMPLELIGHT_ENDPOINT,&dst,pkg->pdata[0],pkg->pdata[1],pkg->disableRsp,pkg->seq);
      break;
    case MT_CmdAppSendLevelControlStep://  = 0x1C,
      //ZStatus_t zclGeneral_SendLevelControlStep( uint8 srcEP, afAddrType_t *dstAddr, uint8 stepMode, uint8 stepSize, uint16 transTime, uint8 disableDefaultRsp, uint8 seqNum );
      temp = osBtoU16(&pkg->pdata[2]);
      zclGeneral_SendLevelControlStep(SAMPLELIGHT_ENDPOINT,&dst,pkg->pdata[0],pkg->pdata[1],temp,pkg->disableRsp,pkg->seq);
      break;
    case MT_CmdAppSendLevelControlStop://  = 0x1D,
      //ZStatus_t zclGeneral_SendLevelControlStop( uint8 srcEP, afAddrType_t *dstAddr, uint8 disableDefaultRsp, uint8 seqNum );
      zclGeneral_SendLevelControlStop(SAMPLELIGHT_ENDPOINT,&dst,pkg->disableRsp,pkg->seq);
      break;
    case MT_CmdAppSendLevelControlMoveToLevelWithOnOff://  = 0x1E,
      //ZStatus_t zclGeneral_SendLevelControlMoveToLevelWithOnOff( uint16 srcEP, afAddrType_t *dstAddr, uint8 level, uint16 transTime, uint8 disableDefaultRsp, uint8 seqNum );
      temp = osBtoU16(&pkg->pdata[1]);
      zclGeneral_SendLevelControlMoveToLevelWithOnOff(SAMPLELIGHT_ENDPOINT,&dst,pkg->pdata[0],temp,pkg->disableRsp,pkg->seq);
      break;
    case MT_CmdAppSendLevelControlMoveWithOnOff://  = 0x1F,
      //ZStatus_t zclGeneral_SendLevelControlMoveWithOnOff( uint8 srcEP, afAddrType_t *dstAddr, uint8 moveMode, uint8 rate, uint8 disableDefaultRsp, uint8 seqNum )
      zclGeneral_SendLevelControlMoveWithOnOff(SAMPLELIGHT_ENDPOINT,&dst,pkg->pdata[0],pkg->pdata[1],pkg->disableRsp,pkg->seq);
      break;
    case MT_CmdAppSendLevelControlStepWithOnOff://  = 0x20,
      //ZStatus_t zclGeneral_SendLevelControlStepWithOnOff( uint8 srcEP, afAddrType_t *dstAddr, uint8 stepMode, uint8 stepSize, uint16 transTime, uint8 disableDefaultRsp, uint8 seqNum );
      temp = osBtoU16(&pkg->pdata[2]);
      zclGeneral_SendLevelControlStepWithOnOff(SAMPLELIGHT_ENDPOINT,&dst,pkg->pdata[0],pkg->pdata[1],temp,pkg->disableRsp,pkg->seq);
      break;
    case MT_CmdAppSendLevelControlStopWithOnOff://  = 0x21,
      //ZStatus_t zclGeneral_SendLevelControlStopWithOnOff( uint8 srcEP, afAddrType_t *dstAddr, uint8 disableDefaultRsp, uint8 seqNum );
      zclGeneral_SendLevelControlStopWithOnOff(SAMPLELIGHT_ENDPOINT,&dst,pkg->disableRsp,pkg->seq);
      break;
    case MT_CmdAppSendAlarmResetAll://0x22
    //ZStatus_t zclGeneral_SendAlarmResetAll( uint16 srcEP, afAddrType_t *dstAddr, uint8 disableDefaultRsp, uint8 seqNum );
      zclGeneral_SendAlarmResetAll(SAMPLELIGHT_ENDPOINT,&dst,pkg->disableRsp,pkg->seq);
      break;
    case MT_CmdAppSendAlarmGet://                 = 0x23,
    //ZStatus_t zclGeneral_SendAlarmGet uint16 srcEP, afAddrType_t *dstAddr, uint8 disableDefaultRsp, uint8 seqNum );
      zclGeneral_SendAlarmGet(SAMPLELIGHT_ENDPOINT,&dst,pkg->disableRsp,pkg->seq);
      break;
    case MT_CmdAppSendAlarmResetLog://           = 0x24
    //ZStatus_t zclGeneral_SendAlarmResetLog( uint16 srcEP, afAddrType_t *dstAddr, uint8 disableDefaultRsp, uint8 seqNum );
      zclGeneral_SendAlarmResetLog(SAMPLELIGHT_ENDPOINT,&dst,pkg->disableRsp,pkg->seq);
      break;
    case MT_CmdAppSend_IAS_WD_StartWarningCmd://0x30 ZStatus_t zclSS_Send_IAS_WD_StartWarningCmd( uint8 srcEP, afAddrType_t *dstAddr,zclWDStartWarning_t *warning,uint8 disableDefaultRsp, uint8 seqNum );
      WDState.strobeLevel = pkg->pdata[0];
      WDState.strobeDutyCycle = pkg->pdata[1];
      WDState.warningDuration = osBtoU16(&pkg->pdata[2]);
      WDState.warningmessage.warningbyte = pkg->pdata[4];
      zclSS_Send_IAS_WD_StartWarningCmd(SAMPLELIGHT_ENDPOINT,&dst,&WDState,pkg->disableRsp,pkg->seq);
      break;
    case MT_CmdAppSend_IAS_WD_SquawkCmd://0x31 zclSS_Send_IAS_WD_SquawkCmd( uint8 srcEP, afAddrType_t *dstAddr,zclWDSquawk_t *squawk,uint8 disableDefaultRsp, uint8 seqNum );
      zclSS_Send_IAS_WD_SquawkCmd(SAMPLELIGHT_ENDPOINT,&dst,(zclWDSquawk_t*)&pkg->pdata[0],pkg->disableRsp,pkg->seq);
      break;*/
    }
  }
}

void MT_App_ReportStatue(uint8 cmd,uint16 src, uint8 spoint,uint8 seq,uint8 disableRsp,uint16 cID,uint8 *pdata, uint8 len)
{
  APkg_t *pmsg;
  uint8 slen = sizeof(APkg_t)+len;
  pmsg = (APkg_t*)osal_mem_alloc(slen);
  if(pmsg!=NULL)
  {
    osU16toB(pmsg->addr,src);
    pmsg->point = spoint;
    pmsg->seq = seq;
    pmsg->disableRsp = disableRsp;
    osU16toB(pmsg->cID,cID);
    pmsg->len = len;
    osal_memcpy(pmsg->pdata,pdata,len);
    MT_BuildAndSendZToolResponse(((uint8)MT_RPC_CMD_AREQ | (uint8)MT_RPC_SYS_APP), cmd, slen, (uint8*)pmsg);
    osal_mem_free(pmsg);
  }
}
