/**************************************************************************************************
  Filename:       AppCommon.h
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

#ifndef __APPCOMMON_H__
#define __APPCOMMON_H__

#include "ZComDef.h"
#include "OSAL.h"
#include "OSAL_PwrMgr.h"
#include "AF.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "MT_SYS.h"

#include "nwk_util.h"

#include "zcl.h"
#include "zcl_general.h"
#include "zcl_ha.h"
#include "zcl_ezmode.h"
#include "zcl_diagnostic.h"
#include "zcl_ss.h"

#include "AppCommon.h"
#include "AppTask.h"

#include "onboard.h"

/* HAL */
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_adc.h"
#include "hal_lamp.h"
#include "hal_drivers.h"
/*********************************************************************
 * CONSTANTS
 */
#define LIGHT_OFF                      0
#define LIGHT_ON                        1
   
#define DEVICE_VERSION                0
#define DEVICE_FLAG                   0

#define HARWARE_VERSION               1
#define ZCL_VERSION                   1
   
extern uint16           AppCommon_IdentifyTime;
extern const uint8      AppCommon_ManufacturerName[];
extern const uint8      AppCommon_ModelId[];
extern const uint8      AppCommon_DateCode[];
extern const uint8      AppCommon_PowerSource;
extern       uint8      AppCommon_LocationDescription[];
extern       uint8      AppCommon_PhysicalEnvironment;
extern       uint8      AppCommon_DeviceEnable;

void AppCommon_BasicResetCB(void);
void AppCommon_IdentifyCB(zclIdentify_t *pCmd);
void AppCommon_ProcessIdentifyTimeChange(void);
void AppCommon_IdentifyQueryRspCB(zclIdentifyQueryRsp_t *pRsp);
uint8 AppCommon_GetSeqNum(void);
float AppCommon_ReadVDD(void);
void AppCommon_FactoryReset(void);
void AppCommon_ReportSwtichState( uint8 ep );
void AppCommon_UpdateValue(void);
void AppCommon_ReportVoltage(uint8 check);
#endif

