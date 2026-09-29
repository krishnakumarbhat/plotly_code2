/*===================================================================================*\
* FILE:  f360_push_reduced_id.cpp
*====================================================================================
* Copyright (C) 2017 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains implementation of f360_push_reduced_id().
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/

#include "f360_push_reduced_id.h"
#include "f360_math_func.h"
#include "f360_iterator.h"
#include <algorithm>

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Push_Reduced_Id()
   *===========================================================================
   * RETURN VALUE:
   * bool
   *
   * PARAMETERS:
   * const int32_t reduced_id
   * F360_Tracker_Info_T & tracker_info
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
   * This function pushes reduced ID of object to array with inactive objects IDs.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Push_Reduced_Id(
      const int32_t reduced_id,
      F360_Tracker_Info_T & tracker_info)
   {
      const int32_t num_reduced_obj_active = tracker_info.reduced_num_active_objs;
      const int32_t* const begin = cmn::begin(tracker_info.reduced_active_obj_ids);
      const int32_t* const end = cmn::end(tracker_info.reduced_active_obj_ids);

      const int32_t* const p_active_reduced_obj_id_found = std::find(begin, end, reduced_id);

      const bool f_found_obj_among_active_reduced_objs = p_active_reduced_obj_id_found != end;

      const uint32_t obj_idx_in_reduced_active_obj_array = static_cast<uint32_t>(std::distance(begin, p_active_reduced_obj_id_found));

      const bool f_valid_reduced_obj = ((tracker_info.variant.num_reduced_tracks > obj_idx_in_reduced_active_obj_array) && f_found_obj_among_active_reduced_objs);

      if (f_valid_reduced_obj)
      {
         for (uint32_t idx = obj_idx_in_reduced_active_obj_array; idx < (tracker_info.variant.num_reduced_tracks - 1U); idx++)
         {
            tracker_info.reduced_active_obj_ids[idx] = tracker_info.reduced_active_obj_ids[idx + 1U];
         }
         tracker_info.reduced_active_obj_ids[tracker_info.variant.num_reduced_tracks - 1U] = 0;
         tracker_info.reduced_inactive_obj_ids[tracker_info.variant.num_reduced_tracks - static_cast<uint32_t>(num_reduced_obj_active)] = reduced_id;
         tracker_info.reduced_num_active_objs = tracker_info.reduced_num_active_objs - 1;
      }
   }
}

