#ifndef F360_UPDATE_OBJECT_MAXIMUM_RCS_H
#define F360_UPDATE_OBJECT_MAXIMUM_RCS_H
/*===========================================================================*\
* FILE: f360_update_object_maximum_rcs.h
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*==========================================================================================*/

#include "f360_object_track.h"
#include "rspp_detection.h"

namespace f360_variant_A
{
   void Update_Object_Maximum_Rcs(
      const rspp_variant_A::RSPP_Detection_T(&raw_dets)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& object);
}

#endif
