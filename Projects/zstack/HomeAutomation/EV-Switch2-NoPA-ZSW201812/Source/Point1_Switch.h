
#ifndef __POINT1_H__
#define __POINT1_H__

#include "AppCommon.h"

#define defEndPoint1                    1

#if ZCL_DISCOVER
extern CONST zclCommandRec_t            Point1_Cmds[];
extern CONST uint8                      Point1CmdsArraySize;
#endif

extern uint8  Point1_OnOff;
extern CONST zclCommandRec_t            Point1_Cmds[];
extern CONST zclAttrRec_t               Point1_Attrs[];
extern CONST uint8                      Point1_NumAttributes;
extern SimpleDescriptionFormat_t        Point1_SimpleDesc;

extern zclGeneral_AppCallbacks_t        Point1_CmdCallbacks;

#endif

