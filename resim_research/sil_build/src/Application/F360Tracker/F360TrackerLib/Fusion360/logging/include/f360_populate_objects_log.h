#ifndef F360_POPULATE_OBJECTS_LOG_H
#define F360_POPULATE_OBJECTS_LOG_H
/*===================================================================================*\
* FILE:  f360_populate_objects_log.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*------------------------------------------------------------------------------------
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*/

#include "f360_object_log.h"
#include "f360_object_track.h"
#include "f360_tracker_info.h"
#include "f360_calibrations.h"
#include "f360_host.h"

namespace f360_variant_A
{
   void Populate_Objects_Data(F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS], F360_Tracker_Info_T& tracker_info,
      const F360_Object_Log_T(&objects_log)[NUMBER_OF_OBJECT_TRACKS], const F360_Host_T& host);

   void Populate_Objects_Log_Data(F360_Object_Log_T(&objects_log)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS], const int32_t num_active_objects,
      const int32_t(&active_obj_ids)[NUMBER_OF_OBJECT_TRACKS]);
}

#endif 
