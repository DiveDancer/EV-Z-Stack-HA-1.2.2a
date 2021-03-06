/**************************************************************************************************
  Filename:       AppTask.c
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
#include "MT_UART.h"

#include "nwk_util.h"

#include "zcl.h"
#include "zcl_general.h"
#include "zcl_ha.h"
#include "zcl_ezmode.h"
#include "zcl_diagnostic.h"

#include "AppTask.h"

#include "onboard.h"

/* HAL */
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_drivers.h"
#include "hal_max485.h"

#include "Point1_Switch.h"

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

/*********************************************************************
 * GLOBAL VARIABLES
 */
static byte App_TaskID;
/*********************************************************************
 * LOCAL VARIABLES
 */
static void App_ProcessIncomingMsg( zclIncomingMsg_t *pInMsg );
static void App_ProcessInDefaultRspCmd( zclIncomingMsg_t *pInMsg );
static void App_HandleKeys( byte shift, byte keys );

uint16 bindingInClusters[] =
{
  ZCL_CLUSTER_ID_GEN_ON_OFF
};
#define ZCLSAMPLELIGHT_BINDINGLIST (sizeof(bindingInClusters) / sizeof(bindingInClusters[0]))
/*********************************************************************
 * @fn          App_Init
 *
 * @brief       Initialization function for the zclGeneral layer.
 *
 * @param       none
 *
 * @return      none
 */
void App_TaskInit( byte task_id )
{
  App_TaskID = task_id;
  // This app is part of the Home Automation Profile
  zclHA_Init( &Point1_SimpleDesc );
  // Register the ZCL General Cluster Library callback functions
  zclGeneral_RegisterCmdCallbacks( defEndPoint1, &Point1_CmdCallbacks );
  // Register the application's attribute list
  zcl_registerAttrList( defEndPoint1, Point1_NumAttributes, Point1_Attrs );
  // Register the Application to receive the unprocessed Foundation command/response messages
  zcl_registerForMsg( App_TaskID );

#ifdef ZCL_DISCOVER
  // Register the application's command list
  zcl_registerCmdList( defEndPoint1, zclPoint1CmdsArraySize, zclPoint1_Cmds );
#endif
#ifdef ZGP_AUTO_TT
  zgpTranslationTable_RegisterEP ( &App_SimpleDesc );
#endif
  osal_start_reload_timer(App_TaskID,APP_KEYSCAN_EVT,500);
  int8 signed_dBm_of_TxPower_requeseted = 20;//4.5dbm
  MAC_MlmeSetReq(MAC_PHY_TRANSMIT_POWER_SIGNED, &signed_dBm_of_TxPower_requeseted);
  NLME_PermitJoiningRequest(0x00);
  
  MT_UartInit();
  MT_UartRegisterTaskID(App_TaskID);//register receive message task
  hal_max485_init();
}
uint8 App_GetTaskID(void)
{
  return App_TaskID;
}
/*********************************************************************
 * @fn          zclSample_event_loop
 *
 * @brief       Event Loop Processor for zclGeneral.
 *
 * @param       none
 *
 * @return      none
 */
uint16 App_Task( uint8 task_id, uint16 events )
{
  afIncomingMSGPacket_t *MSGpkt;
  osal_event_hdr_t      *pUart_msg;
  devStates_t NwkState;
  uint8 shift,key;
  (void)task_id;  // Intentionally unreferenced parameter
  if(events & APP_KEYSCAN_EVT)
  {
    shift = HalKeyRead(&key,20);
    if(shift)
    {
      App_HandleKeys(shift,key);
    }
    events ^= APP_KEYSCAN_EVT;
  }
  if(events & APP_UART_RECEIVE_EVT)
  {
    hal_max485_receive(NULL,0);
  }
  if ( events & SYS_EVENT_MSG )
  {
    while ( (MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( App_TaskID )) )
    {
      switch ( MSGpkt->hdr.event )
      {
        case ZCL_INCOMING_MSG:
          // Incoming ZCL Foundation command/response messages
          App_ProcessIncomingMsg( (zclIncomingMsg_t *)MSGpkt );
          break;
        case ZDO_STATE_CHANGE:
          NwkState = (devStates_t)(MSGpkt->hdr.status);
          switch(NwkState)
          {
            case DEV_END_DEVICE:                          // Started as device after authentication
            case DEV_ROUTER:                          // Device joined, authenticated and is a router
            case DEV_COORD_STARTING:                   // Started as Zigbee Coordinator
              NwkLedBlink(1,100,100);
              HalLedSet(0,HAL_LED_MODE_OFF);
            break;
            default:
              NwkLedBlink(0xFFFF,100,100);
              break;
          }
          break;
      case SPI_INCOMING_ZAPP_DATA://uart data receive event
        pUart_msg = (osal_event_hdr_t*)MSGpkt;
        hal_max485_receive((uint8*)(pUart_msg+1),pUart_msg->status);
        osal_start_timerEx(App_TaskID,APP_UART_RECEIVE_EVT,100);
        break;
        default:
          break;
      }
      // Release the memory
      osal_msg_deallocate( (uint8 *)MSGpkt );
    }
    // return unprocessed events
    return (events ^ SYS_EVENT_MSG);
  }
  if ( events & APP_IDENTIFY_TIMEOUT_EVT )
  {
    if ( AppCommon_IdentifyTime > 0 )
      AppCommon_IdentifyTime--;
    AppCommon_ProcessIdentifyTimeChange();
    return ( events ^ APP_IDENTIFY_TIMEOUT_EVT );
  }
  // Discard unknown events
  return 0;
}

static void App_HandleKeys( byte shift, byte keys )
{
  static uint8  clearFlag = 0x00;
  uint16 runtime;
  if(shift==KEY_SHIFT_DOWN)
  {
    clearFlag = 0x00;
      switch(keys)
      {
        case KEY1_VALUE:
          AppCommon_ReportSwtichState(defEndPoint1);
          break;
      }
  }
  else if(shift==KEY_SHIFT_KEEP)
  {
     if(keys==KEY0_VALUE)
     {
      runtime = HalKeyGetRuntim();
      if((runtime>4000)&&(clearFlag==0x00))
      {
        clearFlag = 0x01;
        AppCommon_FactoryReset();
      }
     }
  }
}

/******************************************************************************
 *
 *  Functions for processing ZCL Foundation incoming Command/Response messages
 *
 *****************************************************************************/

/*********************************************************************
 * @fn      App_ProcessIncomingMsg
 *
 * @brief   Process ZCL Foundation incoming message
 *
 * @param   pInMsg - pointer to the received message
 *
 * @return  none
 */
static void App_ProcessIncomingMsg( zclIncomingMsg_t *pInMsg )
{
  switch ( pInMsg->zclHdr.commandID )
  {
#ifdef ZCL_READ
    case ZCL_CMD_READ_RSP:
      //App_ProcessInReadRspCmd( pInMsg );
      break;
#endif
#ifdef ZCL_WRITE
    case ZCL_CMD_WRITE_RSP:
      //App_ProcessInWriteRspCmd( pInMsg );
      break;
#endif
#ifdef ZCL_REPORT
    // Attribute Reporting implementation should be added here
    case ZCL_CMD_CONFIG_REPORT:
      // App_ProcessInConfigReportCmd( pInMsg );
      break;

    case ZCL_CMD_CONFIG_REPORT_RSP:
      // App_ProcessInConfigReportRspCmd( pInMsg );
      break;

    case ZCL_CMD_READ_REPORT_CFG:
      // App_ProcessInReadReportCfgCmd( pInMsg );
      break;

    case ZCL_CMD_READ_REPORT_CFG_RSP:
      // App_ProcessInReadReportCfgRspCmd( pInMsg );
      break;

    case ZCL_CMD_REPORT:
      // App_ProcessInReportCmd( pInMsg );
      break;
#endif
    case ZCL_CMD_DEFAULT_RSP:
      App_ProcessInDefaultRspCmd( pInMsg );
      break;
#ifdef ZCL_DISCOVER
    case ZCL_CMD_DISCOVER_CMDS_RECEIVED_RSP:
      App_ProcessInDiscCmdsRspCmd( pInMsg );
      break;

    case ZCL_CMD_DISCOVER_CMDS_GEN_RSP:
      App_ProcessInDiscCmdsRspCmd( pInMsg );
      break;

    case ZCL_CMD_DISCOVER_ATTRS_RSP:
      App_ProcessInDiscAttrsRspCmd( pInMsg );
      break;

    case ZCL_CMD_DISCOVER_ATTRS_EXT_RSP:
      App_ProcessInDiscAttrsExtRspCmd( pInMsg );
      break;
#endif
    default:
      break;
  }

  if ( pInMsg->attrCmd )
    osal_mem_free( pInMsg->attrCmd );
}

static void App_ProcessInDefaultRspCmd( zclIncomingMsg_t *pInMsg )
{
  
}
/*********************************************************************
 * @fn      App_ProcessZDOMsgs
 *
 * @brief   Called when this node receives a ZDO/ZDP response.
 *
 * @param   none
 *
 * @return  status

static void App_ProcessZDOMsgs( zdoIncomingMsg_t *pMsg )
{
  zclEZMode_ActionData_t data;
  ZDO_MatchDescRsp_t *pMatchDescRsp;

  // Let EZ-Mode know of the Simple Descriptor Response
  if ( pMsg->clusterID == Match_Desc_rsp )
  {
    pMatchDescRsp = ZDO_ParseEPListRsp( pMsg );
    data.pMatchDescRsp = pMatchDescRsp;
    zcl_EZModeAction( EZMODE_ACTION_MATCH_DESC_RSP, &data );
    osal_mem_free( pMatchDescRsp );
  }
}
 */