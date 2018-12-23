
#ifndef __POINT2_H__
#define __POINT2_H__

#include "AppCommon.h"

#define defEndPoint2                   2

#if ZCL_DISCOVER
extern CONST zclCommandRec_t            Point2_Cmds[];
extern CONST uint8                      Point2CmdsArraySize;
#endif

extern uint8  Point2_OnOff;
extern CONST zclCommandRec_t            Point2_Cmds[];
extern CONST zclAttrRec_t               Point2_Attrs[];
extern CONST uint8                      Point2_NumAttributes;
extern SimpleDescriptionFormat_t        Point2_SimpleDesc;
extern zclGeneral_AppCallbacks_t        Point2_CmdCallbacks;

#endif

