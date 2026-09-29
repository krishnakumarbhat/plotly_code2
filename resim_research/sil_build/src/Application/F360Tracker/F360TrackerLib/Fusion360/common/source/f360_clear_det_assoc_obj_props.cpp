/*===========================================================================*\
* FILE: f360_clear_det_assoc_obj_props.cpp
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Clear_Det_Assoc_Obj_Props()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#include "f360_clear_det_assoc_obj_props.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Clear_Det_Assoc_Obj_Props()
   *===========================================================================
   * RETURN VALUE: None
   *
   * PARAMETERS:
   * const F360_Object_Track_T & obj,
   * F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * The function clears object related properties of all detections that are associated
   * to the specified input object.
   *
   \*===========================================================================*/
   void Clear_Det_Assoc_Obj_Props(
      const F360_Object_Track_T & obj,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      for (uint32_t i = 0U; i < obj.ndets; i++)
      {
         F360_Detection_Props_T & det_prop = det_props[obj.detids[i] - 1U];
         det_prop.object_track_id = 0;
         det_prop.f_ok_to_use = false;
         det_prop.f_rr_inlier = false;
         det_prop.range_rate_predicted = 0.0F;
         det_prop.f_inside_gate = false;
         det_prop.f_inside_mov_gate = false;
      }
   }
}
