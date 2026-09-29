/*===========================================================================*\
* FILE: f360_calc_obj_processed_size.cpp
*============================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Calc_Obj_Processed_Size()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/


#include "f360_calc_obj_processed_size.h"

namespace f360_variant_A
{

   /*===========================================================================*\
   * FUNCTION: Calc_Obj_Processed_Size()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Calibrations_T& calib
   * F360_Object_Track_T& object_track
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
   * In the tracker there are 2 object dimensions:
   *  - bbox length and width (bbox.Get_Length() and bbox.Get_Width)
   *  - processed length and width (processed.length and processed.width)
   * The bbox dimensions are used internally in the tracker for everything.
   * The processed dimensions are used as tracker output and don't have any impact
   * on internal tracker performance.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Calc_Obj_Processed_Size(
      const F360_Calibrations_T& calib,
      F360_Object_Track_T& object_track)
   {
      bool f_shrink_processed_size = false;
      object_track.length_processed = object_track.bbox.Get_Length();

      // For new objects and resim initialization
      if (object_track.width_processed < F360_EPSILON)
      {
         object_track.width_processed = object_track.bbox.Get_Width();
      }

      // Only shrink width for specific objects
      if ((object_track.bbox.Get_Length() > 15.0F) &&
         (std::abs(object_track.vcs_position.x) > 30.0F) &&
         (object_track.trk_fltr_type == F360_TRACKER_TRKFLTR_CTCA) &&
         ((object_track.reference_point == F360_REFERENCE_POINT_FRONT) || (object_track.reference_point == F360_REFERENCE_POINT_REAR)))
      {
         f_shrink_processed_size = true;
      }

      const float32_t alpha = 0.05F;
      const float32_t width = object_track.bbox.Get_Width();
      if (f_shrink_processed_size)
      {
         object_track.width_processed = alpha * (width / 2.0F) + (1.0F - alpha) * object_track.width_processed;
      }
      // When the difference in width between two iterations is small we don't want to introduce any delay to standard width calculations
      else if (std::abs(width - object_track.width_processed) < 0.3F)
      {
         object_track.width_processed = width;
      }
      else
      {
         object_track.width_processed = alpha * width + (1.0F - alpha) * object_track.width_processed;
      }

      // Make sure that dimensions are saturated
      if (object_track.trk_fltr_type == F360_TRACKER_TRKFLTR_CTCA)
      {
         object_track.width_processed = std::max(object_track.width_processed, calib.k_nonmoveable_target_diameter);
      }
   }
}
