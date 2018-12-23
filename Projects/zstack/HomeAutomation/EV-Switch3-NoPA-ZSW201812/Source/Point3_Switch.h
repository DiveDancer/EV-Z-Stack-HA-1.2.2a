/**************************************************************************************************
  Filename:       Point3_Switch.h
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

#ifndef __POINT3_H__
#define __POINT3_H__

#include "AppCommon.h"

#define defEndPoint3                    3

#if ZCL_DISCOVER
extern CONST zclCommandRec_t            Point3_Cmds[];
extern CONST uint8                      zclPoint3CmdsArraySize;
#endif

extern uint8  Point3_OnOff;
extern CONST zclCommandRec_t            Point3_Cmds[];
extern CONST zclAttrRec_t               Point3_Attrs[];
extern CONST uint8                      Point3_NumAttributes;
extern SimpleDescriptionFormat_t        Point3_SimpleDesc;
extern zclGeneral_AppCallbacks_t        Point3_CmdCallbacks;

#endif
