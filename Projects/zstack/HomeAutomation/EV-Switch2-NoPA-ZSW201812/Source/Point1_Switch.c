
/* INCLUDES*/
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

#include "AppCommon.h"
#include "Point1_Switch.h"
#include "AppTask.h"
#include "hal_lamp.h"

/*********************************************************************
 * CONSTANTS
 */
    
#define defPoint1Version                0
#define defPoint1Flag                   0

#define defHarwareVersion               1
#define defZCLVersion                   1

/*********************************************************************
 * MACROS
 */
static void Point1_OnOffCB( uint8 cmd );
/*********************************************************************
 * GLOBAL VARIABLES
 */

// Basic Cluster
const uint8 Point1_HWRevision = defPoint1Version;
const uint8 Point1_ZCLVersion = defZCLVersion;

uint8 Point1_PhysicalEnvironment = 0;
uint8 Point1_DeviceEnable = DEVICE_ENABLED;

// On/Off Cluster
uint8  Point1_OnOff = LIGHT_OFF;

#if ZCL_DISCOVER
CONST zclCommandRec_t Point1_Cmds[] =
{
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    COMMAND_BASIC_RESET_FACT_DEFAULT,
    CMD_DIR_SERVER_RECEIVED
  },
  {
    ZCL_CLUSTER_ID_GEN_ON_OFF,
    COMMAND_OFF,
    CMD_DIR_SERVER_RECEIVED
  },
  {
    ZCL_CLUSTER_ID_GEN_ON_OFF,
    COMMAND_ON,
    CMD_DIR_SERVER_RECEIVED
  },
  {
    ZCL_CLUSTER_ID_GEN_ON_OFF,
    COMMAND_TOGGLE,
    CMD_DIR_SERVER_RECEIVED
  }
};

CONST uint8 Point1CmdsArraySize = ( sizeof(Point1_Cmds) / sizeof(Point1_Cmds[0]) );
#endif // ZCL_DISCOVER

/*********************************************************************
 * ATTRIBUTE DEFINITIONS - Uses REAL cluster IDs
 */
CONST zclAttrRec_t Point1_Attrs[] =
{
  // *** General Basic Cluster Attributes ***
  {
    ZCL_CLUSTER_ID_GEN_BASIC,             // Cluster IDs - defined in the foundation (ie. zcl.h)
    {  // Attribute record
      ATTRID_BASIC_HW_VERSION,            // Attribute ID - Found in Cluster Library header (ie. zcl_general.h)
      ZCL_DATATYPE_UINT8,                 // Data Type - found in zcl.h
      ACCESS_CONTROL_READ,                // Variable access control - found in zcl.h
      (void *)&Point1_HWRevision  // Pointer to attribute variable
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_ZCL_VERSION,
      ZCL_DATATYPE_UINT8,
      ACCESS_CONTROL_READ,
      (void *)&Point1_ZCLVersion
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_MANUFACTURER_NAME,
      ZCL_DATATYPE_CHAR_STR,
      ACCESS_CONTROL_READ,
      (void *)AppCommon_ManufacturerName
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_MODEL_ID,
      ZCL_DATATYPE_CHAR_STR,
      ACCESS_CONTROL_READ,
      (void *)AppCommon_ModelId
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_DATE_CODE,
      ZCL_DATATYPE_CHAR_STR,
      ACCESS_CONTROL_READ,
      (void *)AppCommon_DateCode
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_POWER_SOURCE,
      ZCL_DATATYPE_UINT8,
      ACCESS_CONTROL_READ,
      (void *)&AppCommon_PowerSource
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_LOCATION_DESC,
      ZCL_DATATYPE_CHAR_STR,
      (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)AppCommon_LocationDescription
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_PHYSICAL_ENV,
      ZCL_DATATYPE_UINT8,
      (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)&AppCommon_PhysicalEnvironment
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_DEVICE_ENABLED,
      ZCL_DATATYPE_BOOLEAN,
      (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)&AppCommon_DeviceEnable
    }
  },
#ifdef ZCL_IDENTIFY
  // *** Identify Cluster Attribute ***
  {
    ZCL_CLUSTER_ID_GEN_IDENTIFY,
    { // Attribute record
      ATTRID_IDENTIFY_TIME,
      ZCL_DATATYPE_UINT16,
      (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)&AppCommon_IdentifyTime
    }
  },
#endif
  // *** On/Off Cluster Attributes ***
  {
    ZCL_CLUSTER_ID_GEN_ON_OFF,
    { // Attribute record
      ATTRID_ON_OFF,
      ZCL_DATATYPE_BOOLEAN,
      ACCESS_CONTROL_READ,
      (void *)&Point1_OnOff
    }
  }
};

CONST uint8 Point1_NumAttributes = ( sizeof(Point1_Attrs) / sizeof(Point1_Attrs[0]) );

/*********************************************************************
 * SIMPLE DESCRIPTOR
 */
// This is the Cluster ID List and should be filled with Application
// specific cluster IDs.
const cId_t Point1_InClusterList[] =
{
  ZCL_CLUSTER_ID_GEN_BASIC,
  ZCL_CLUSTER_ID_GEN_IDENTIFY,
  ZCL_CLUSTER_ID_GEN_GROUPS,
  ZCL_CLUSTER_ID_GEN_SCENES,
  ZCL_CLUSTER_ID_GEN_ON_OFF
};
// work-around for compiler bug... IAR can't calculate size of array with #if options.
#define POINT1_MAX_INCLUSTERS   5

const cId_t Point1_OutClusterList[] =
{
  ZCL_CLUSTER_ID_GEN_BASIC
};
#define POINT1_MAX_OUTCLUSTERS  (sizeof(Point1_OutClusterList) / sizeof(Point1_OutClusterList[0]))

SimpleDescriptionFormat_t Point1_SimpleDesc =
{
  defEndPoint1,                           //  int Endpoint;
  ZCL_HA_PROFILE_ID,                     //  uint16 AppProfId;
  ZCL_HA_DEVICEID_ON_OFF_LIGHT,          //  uint16 AppDeviceId;
  DEVICE_VERSION,                     //  int   AppDevVer:4;
  DEVICE_FLAG,                        //  int   AppFlags:4;
  POINT1_MAX_INCLUSTERS,         //  byte  AppNumInClusters;
  (cId_t *)Point1_InClusterList, //  byte *pAppInClusterList;
  POINT1_MAX_OUTCLUSTERS,        //  byte  AppNumInClusters;
  (cId_t *)Point1_OutClusterList //  byte *pAppInClusterList;
};


/*********************************************************************
 * ZCL General Profile Callback table
 */
zclGeneral_AppCallbacks_t Point1_CmdCallbacks =
{
  AppCommon_BasicResetCB,                       // Basic Cluster Reset command
  AppCommon_IdentifyCB,                         // Identify command
#ifdef ZCL_EZMODE
  NULL,                                   // Identify EZ-Mode Invoke command
  NULL,                                   // Identify Update Commission State command
#endif
  NULL,                                   // Identify Trigger Effect command
  AppCommon_IdentifyQueryRspCB,      // Identify Query Response command
  Point1_OnOffCB,                 // On/Off cluster commands
  NULL,                                   // On/Off cluster enhanced command Off with Effect
  NULL,                                   // On/Off cluster enhanced command On with Recall Global Scene
  NULL,                                   // On/Off cluster enhanced command On with Timed Off
#ifdef ZCL_LEVEL_CTRL
  zclSampleLight_LevelControlMoveToLevelCB, // Level Control Move to Level command
  zclSampleLight_LevelControlMoveCB,        // Level Control Move command
  zclSampleLight_LevelControlStepCB,        // Level Control Step command
  zclSampleLight_LevelControlStopCB,        // Level Control Stop command
#endif
#ifdef ZCL_GROUPS
  NULL,                                   // Group Response commands
#endif
#ifdef ZCL_SCENES
  NULL,                                  // Scene Store Request command
  NULL,                                  // Scene Recall Request command
  NULL,                                  // Scene Response command
#endif
#ifdef ZCL_ALARMS
  NULL,                                  // Alarm (Response) commands
#endif
#ifdef SE_UK_EXT
  NULL,                                  // Get Event Log command
  NULL,                                  // Publish Event Log command
#endif
  NULL,                                  // RSSI Location command
  NULL                                   // RSSI Location Response command
};
/****************************************************************************
****************************************************************************/

/*********************************************************************
 * @fn      zclSampleLight_OnOffCB
 *
 * @brief   Callback from the ZCL General Cluster Library when
 *          it received an On/Off Command for this application.
 *
 * @param   cmd - COMMAND_ON, COMMAND_OFF or COMMAND_TOGGLE
 *
 * @return  none
 */
static void Point1_OnOffCB( uint8 cmd )
{
 // afIncomingMSGPacket_t *pPtr = zcl_getRawAFMsg();
 // zclSampleLight_DstAddr.addr.shortAddr = pPtr->srcAddr.addr.shortAddr;
  // Turn on the light
  if ( cmd == COMMAND_ON )
  {
    HalLampSet(0,HAL_LAMP_ON);
  }
  // Turn off the light
  else if ( cmd == COMMAND_OFF )
  {
    HalLampSet(0,HAL_LAMP_OFF);
  }
  // Toggle the light
  else if ( cmd == COMMAND_TOGGLE )
  {
    HalLampToggle(0);
  }
  Point1_OnOff = HalLampGet(0);
  AppCommon_ReportSwtichState(defEndPoint1);
}


