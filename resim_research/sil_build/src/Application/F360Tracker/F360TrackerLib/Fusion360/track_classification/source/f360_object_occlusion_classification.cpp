/*===================================================================================*\
* FILE:  f360_object_occlusion_classification.cpp
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains the implementation of Object_Occlusion_Classification().
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
**/
#include "f360_object_occlusion_classification.h"
#include "f360_occlusion.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Object_Occlusion_Classification
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Occlusion_Data_T& occlusion_data - reference to occlusion data
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * const F360_Tracker_Info_T& tracker_info - reference to tracker info
   * F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS] - reference to objects tracks struct
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Main function to be called to determine object occlusion status.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
    void Object_Occlusion_Classification(
       const F360_Occlusion_Data_T(&occlusion_data)[MAX_NUMBER_OF_SENSORS],
       const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
       const F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS])
   {
      for (int32_t i_obj = 0; i_obj < tracker_info.num_active_objs; i_obj++)
      {
         const int32_t obj_idx = tracker_info.active_obj_ids[i_obj] - 1;
         F360_Object_Track_T& object = object_tracks[obj_idx];

         if (object.f_moving)
         {
            const F360_Occlusion_Status_T new_status = Get_Object_Occlusion_Status(object, sensors, occlusion_data);
            object.occlusion_status = new_status;
         }
         else
         {
            object.occlusion_status = OCCLUSION_STATUS_UNDEFINED;
         }
      }
   }
}
