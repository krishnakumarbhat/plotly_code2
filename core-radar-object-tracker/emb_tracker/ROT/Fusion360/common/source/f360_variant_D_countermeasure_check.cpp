/*===========================================================================*\
 * FILE: f360_variant_D_countermeasure_check.cpp
 *============================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "f360_variant_D_countermeasure_check.h"

namespace f360_variant_A
{
bool Variant_D_Countermeasure_Check(const F360_Tracker_Variant_T tracker_variant, const F360_Object_Track_T &obj_trk)
{
   bool activate_counermeasure = false;
   if (tracker_variant == F360_VARIANT_TYPE_D)
   {
      // Force downselection of straight-moving objects after 3 tracker cycles
      const float32_t age_threshold     = 0.140F;              // > 2 tracker cycles
      const float32_t heading_threshold = F360_DEG2RAD(45.0F); // 45 degrees
      const float32_t lateral_threshold = 20.0F;

      const bool f_movable                 = (obj_trk.movable_prob > MOVABLE_PROB_THRESHOLD);
      const bool f_age_above_thr           = (obj_trk.time_since_initialization >= age_threshold);
      const bool f_similar_heading_to_host = (std::abs(obj_trk.vcs_heading.Value()) < heading_threshold);
      const bool f_in_lateral_window       = (std::abs(obj_trk.vcs_position.y) < lateral_threshold);

      activate_counermeasure = f_movable && f_age_above_thr && f_similar_heading_to_host && f_in_lateral_window;
   }
   return activate_counermeasure;
}
} // namespace f360_variant_A
