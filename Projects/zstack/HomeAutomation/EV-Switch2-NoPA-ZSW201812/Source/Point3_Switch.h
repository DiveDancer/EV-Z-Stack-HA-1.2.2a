
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

