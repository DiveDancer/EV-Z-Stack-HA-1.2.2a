/**************************************************************************************************
  Filename:       Coordinator_util.h
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

#ifndef __COORDINATOR_H__
#define __COORDINATOR_H__

void zcl_BasicResetCB( void );
void zcl_IdentifyCB( zclIdentify_t *pCmd );
void zcl_IdentifyQueryRspCB( zclIdentifyQueryRsp_t *pRsp );
ZStatus_t zcl_ChangeNotificationCB(zclZoneChangeNotif_t *pCmd, afAddrType_t *srcAddr );
void zclSendWarningToAllWD(zclWDStartWarning_t *alarm);
ZStatus_t CoordinatorReceiveCB( zclIncoming_t *pMsg );
uint8 ProcessInDefaultRspCmd( zclIncomingMsg_t *pInMsg );
ZStatus_t zcl_EnrollRequestCB(zclZoneEnrollReq_t *pReq, uint8 endpoint );

#endif

