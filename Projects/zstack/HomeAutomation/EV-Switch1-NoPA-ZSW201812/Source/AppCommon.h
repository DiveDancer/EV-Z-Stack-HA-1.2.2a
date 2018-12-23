
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
extern       uint8      AppCommon_LocationDescription[17];
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

