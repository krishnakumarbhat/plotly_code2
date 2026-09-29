/*===================================================================================*\
* FILE: f360_associate_detection_to_object.cpp
*====================================================================================
*Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
*  This file contains function definitions of Associate_Detection_To_Object(),
*  Is_Adding_New_Det_Possible() and Add_Det_To_Object()
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "f360_associate_detection_to_object.h"

namespace f360_variant_A
{
   static bool Is_Adding_New_Det_Possible(
      const F360_Tracker_Info_T& tracker_info,
      const uint32_t number_of_dets_in_objects,
      const int32_t object_id);
   
   static void Add_Det_To_Object(
      const uint32_t det_id,
      F360_Object_Track_T& object_track,
      F360_Detection_Props_T& detection_prop);

   /*===========================================================================*\
   * FUNCTION: Associate_Detection_To_Object()
   *===========================================================================
   * RETURN VALUE:
   * bool
   *
   * PARAMETERS:
   * const F360_Tracker_Info_T& tracker_info,
   * F360_Object_Track_T &object_track,
   * F360_Detection_Props_T &detection_prop,
   * const uint32_t det_id
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
   * Function check, if adding new detection to object is possible.
   * If it return false if it is not possible otherwise
   * function adds detection to object and return true.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Associate_Detection_To_Object(
      const F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T &object_track,
      F360_Detection_Props_T &detection_prop,
      const uint32_t det_id)
   {
      const bool f_is_adding_new_detection_possible = Is_Adding_New_Det_Possible(tracker_info, object_track.ndets, object_track.id);
      
      if (f_is_adding_new_detection_possible)
      {
         Add_Det_To_Object(det_id, object_track, detection_prop);
      }

      return f_is_adding_new_detection_possible;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Adding_New_Det_Possible()
   *===========================================================================
   * RETURN VALUE:
   * bool
   *
   * PARAMETERS:
   * const int32_t number_of_dets_in_objects,
   * const int32_t object_id
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
   * This function determines if there still is a place in object table for new
   * detection association and if object.id is valid id
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static bool Is_Adding_New_Det_Possible(
      const F360_Tracker_Info_T& tracker_info,
      const uint32_t number_of_dets_in_objects,
      const int32_t object_id)
   {
      return ((object_id > 0) && (tracker_info.variant.num_dets_in_track > number_of_dets_in_objects));
   }

   /*===========================================================================*\
   * FUNCTION: Add_Det_To_Object()
   *===========================================================================
   * RETURN VALUE:
   * None. 
   *
   * PARAMETERS:
   *  const uint32_t det_id,
   *  F360_Object_Track_T &object_track,
   *  F360_Detection_Props_T &detection_prop
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
   * This function updates object structure by new detection
   * and assign object id to this newly added detection.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static void Add_Det_To_Object(
      const uint32_t det_id,
      F360_Object_Track_T &object_track,
      F360_Detection_Props_T &detection_prop)
   {
      detection_prop.object_track_id = object_track.id;
      object_track.detids[object_track.ndets] = det_id;
      (object_track.ndets)++;

      if (rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING == detection_prop.motion_status)
      {
         (object_track.num_types_of_dets[0])++;
      }
      else
      {
         (object_track.num_types_of_dets[1])++;
      }
   }
}
