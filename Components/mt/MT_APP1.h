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

#ifndef         MT_APP1_H__
#define         MT_APP1_H__

#define         SAMPLELIGHT_ENDPOINT 8
typedef enum
{
  MT_CmdAppSendAfData  = 0x05,
  MT_CmdAppSendCommand = 0x06,
  
  MT_CmdAppFactoryReset = 0x07,
  
  MT_CmdAppSendCmdOff = 0x10,
  MT_CmdAppSendCmdOn = 0x11,
  MT_CmdAppSendCmdToggle = 0x12,
  
  MT_CmdAppSendCmdUpOpen = 0x13,
  MT_CmdAppSendCmdDonwClose = 0x14,
  MT_CmdAppSendCmdStop = 0x15,

  MT_CmdAppSendLevelControlMoveToLevel = 0x1A,
  MT_CmdAppSendLevelControlMove = 0x1B,
  MT_CmdAppSendLevelControlStep = 0x1C,
  MT_CmdAppSendLevelControlStop = 0x1D,
  MT_CmdAppSendLevelControlMoveToLevelWithOnOff = 0x1E,
  MT_CmdAppSendLevelControlMoveWithOnOff = 0x1F,
  MT_CmdAppSendLevelControlStepWithOnOff = 0x20,
  MT_CmdAppSendLevelControlStopWithOnOff = 0x21,
  MT_CmdAppSendAlarmResetAll            = 0x22,
  MT_CmdAppSendAlarmGet                 = 0x23,
  MT_CmdAppSendAlarmResetLog            = 0x24,
  
  MT_CmdAppSend_IAS_WD_StartWarningCmd  = 0x30,
  MT_CmdAppSend_IAS_WD_SquawkCmd        = 0x31,
  
  MT_CmdAppSendDoorLockLockDoor         = 0x40,
  MT_CmdAppSendDoorLockUnlockDoor       = 0x41,
  MT_CmdAppSendDoorLockToggleDoor       = 0x42,
  MT_CmdAppSendDoorLockGetPINCode       = 0x43,
  MT_CmdAppSendDoorLockClearPINCode     = 0x44,
  MT_CmdAppSendDoorLockClearAllPINCodes = 0x45,
  MT_CmdAppSendDoorLockGetUserStatus    = 0x46,
  MT_CmdAppSendDoorLockGetWeekDaySchedule = 0x47,
  MT_CmdAppSendDoorLockClearWeekDaySchedule = 0x48,
  MT_CmdAppSendDoorLockGetYearDaySchedule = 0x49,
  MT_CmdAppSendDoorLockClearYearDaySchedule = 0x4A,
  MT_CmdAppSendDoorLockGetHolidaySchedule = 0x4B,
  MT_CmdAppSendDoorLockClearHolidaySchedule = 0x4C,
  MT_CmdAppSendDoorLockGetUserType      = 0x4D,
  MT_CmdAppSendDoorLockGetRFIDCode      = 0x4E,
  MT_CmdAppSendDoorLockClearRFIDCode    = 0x4F,
  MT_CmdAppSendDoorLockClearAllRFIDCodes = 0x50,

  MT_CmdAppSendGoToLiftValue            = 0x51,
  MT_CmdAppSendGoToLiftPercentage       = 0x52,
  MT_CmdAppSendGoToTiltValue            = 0x53,
  MT_CmdAppSendGoToTiltPercentage       = 0x54,
  
  MT_CmdAppColorControl_Send_MoveToHueCmd=0x60,
  MT_CmdAppColorControl_Send_MoveHueCmd=0x61,
  MT_CmdAppColorControl_Send_StepHueCmd=0x62,
  MT_CmdAppColorControl_Send_MoveToSaturationCmd=0x63,
  MT_CmdAppColorControl_Send_MoveSaturationCmd=0x64,
  MT_CmdAppColorControl_Send_StepSaturationCmd=0x65,
  MT_CmdAppColorControl_Send_MoveToHueAndSaturationCmd=0x66,
  MT_CmdAppColorControl_Send_MoveToColorCmd=0x67,
  MT_CmdAppColorControl_Send_MoveColorCmd=0x68,
  MT_CmdAppColorControl_Send_StepColorCmd=0x69,
  MT_CmdAppColorControl_Send_MoveToColorTemperatureCmd=0x6A,
  MT_CmdAppColorControl_Send_EnhancedMoveToHueCmd=0x6B,
  MT_CmdAppColorControl_Send_EnhancedMoveHueCmd=0x6C,
  MT_CmdAppColorControl_Send_EnhancedStepHueCmd=0x6D,
  MT_CmdAppColorControl_Send_EnhancedMoveToHueAndSaturationCmd=0x6E,
  MT_CmdAppColorControl_Send_StopMoveStepCmd=0x6F,
    
}MT_APP_CMD_ENUM;

uint16 osBtoU16(uint8 *pdata);
void osU16toB(uint8 *pdata, uint16 val);
void MT_App_Hander(uint8 *msg);
void MT_App_ReportStatue(uint8 cmd,uint16 src, uint8 spoint,uint8 seq,uint8 disableRsp,uint16 cID,uint8 *pdata, uint8 len);

#endif
