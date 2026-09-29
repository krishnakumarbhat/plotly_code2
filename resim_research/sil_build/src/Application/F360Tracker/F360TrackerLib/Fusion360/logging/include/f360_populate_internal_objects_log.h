#ifndef F360_POPULATE_INTERNAL_OBJECTS_LOG_H
#define F360_POPULATE_INTERNAL_OBJECTS_LOG_H
/*===================================================================================*\
* FILE:  f360_populate_internal_objects_log.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*------------------------------------------------------------------------------------
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*/
#include "f360_internal_object.h"
#include "f360_object_track.h"
#include "T360_Types.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   void Populate_Internal_Objects_Data(F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Calibrations_T& calibrations,
      const F360_Internal_Object_T(&internal_objects_log)[NUMBER_OF_OBJECT_TRACKS]);

   void Populate_Internal_Objects_Log_Data(F360_Internal_Object_T(&internal_objects_log)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS], const int32_t num_active_objects,
      const int32_t(&active_obj_ids)[NUMBER_OF_OBJECT_TRACKS]);
}

#endif 
