/******************************************************************************
 * Copyright 2025 Aptiv, All Rights Reserved.
 * Aptiv Confidential
 ******************************************************************************/
#include "f360_math_func.h"
#include "f360_cvt_saturated_odometer.h"

namespace f360_variant_A
{
   /*===========================================================================
    * FUNCTION: Update_Saturated_Odometer
    *===========================================================================
    * DESCRIPTION:
    * Update the odometer that tracks the saturated distance traveled since
    * the last reversing switch, to trigger reversing countermeasures.
    * 1. The accumulated distance "cvt_state.saturated_odometer_reversing_countermeasures"
    * increases when the host is moving forward, and decreases when the host is reversing.
    * The odometer is saturated between -2.5 m and 50.0 m.
    * 2. Each time the odometer is saturated, it will trigger the reversing countermeasures
    * to be turned on (at negative border) or off (at positive border)
    * the switch "cvt_state.f_reversing_countermeasures_active".
    * 3. When the host changes moving direction (from forward to reverse or vice versa),
    * the odometer is reset to zero. The purpose is to keep the distance threshold for
    * triggering countermeasures constant.
    *
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const F360_CVT_Input_Data_T& cvt_input,
    * F360_CVT_State_T& cvt_state
    *
    * EXTERNAL REFERENCES:
    * None.
    *=========================================================================*/
   void Update_Saturated_Odometer(
      const F360_Calibrations_T& calibrations,
      const F360_CVT_Input_Data_T& cvt_input,
      F360_CVT_State_T& cvt_state)
   {
      const bool f_host_moving = fabsf(cvt_input.host_speed) > calibrations.k_cvt_slow_speed_threshold;
      if (f_host_moving)
      {
         const bool f_host_now_reversing = cvt_input.host_speed < -calibrations.k_cvt_slow_speed_threshold;
         const bool f_host_now_moving_forward = cvt_input.host_speed > calibrations.k_cvt_slow_speed_threshold;
         const bool f_host_previously_reversing = cvt_state.saturated_odometer_reversing_countermeasures < 0.0F;
         const bool f_host_previously_moving_forward = cvt_state.saturated_odometer_reversing_countermeasures > 0.0F;
         // The odometer has different signs from the current movement direction, reset it to make the countermeasures threshold constant
         const bool f_reset_odometer = (f_host_previously_reversing && f_host_now_moving_forward) ||
                                       (f_host_previously_moving_forward && f_host_now_reversing);
         const float32_t higher_bound_travel_dist = 50.0F;
         const float32_t lower_bound_travel_dist = -2.5F;

         // Assuming execution at 20 Hz, so 0.05s per update when moving forward
         const float32_t time_elapsed = 0.05F;
         const float32_t travel_dist_diff = cvt_input.host_speed * time_elapsed;
         const float32_t new_dist_traveled = cvt_state.saturated_odometer_reversing_countermeasures + travel_dist_diff;

         if ((!cvt_state.f_reversing_countermeasures_active) &&
            (new_dist_traveled < lower_bound_travel_dist))
         {
            // turn on the countermeasures, since the host has reversed enough distance ( > 2.5 m backward)
            cvt_state.f_reversing_countermeasures_active = true;
         }
         else if ((cvt_state.f_reversing_countermeasures_active) &&
                  (new_dist_traveled > higher_bound_travel_dist))
         {
            // turn off the countermeasures, since the host has moved forward enough distance ( > 50.0 m forward)
            cvt_state.f_reversing_countermeasures_active = false;
         }
         else
         {
            // MISRA, keep the current countermeasures state
         }

         if (f_reset_odometer)  // reset the odometer when changing moving direction
         {
            cvt_state.saturated_odometer_reversing_countermeasures = travel_dist_diff;  // reset the odometer to 0.0F and update with the current travel distance
         }
         else
         {
            cvt_state.saturated_odometer_reversing_countermeasures = fminf(higher_bound_travel_dist, fmaxf(lower_bound_travel_dist, new_dist_traveled));         
         }
      }
   }
}
