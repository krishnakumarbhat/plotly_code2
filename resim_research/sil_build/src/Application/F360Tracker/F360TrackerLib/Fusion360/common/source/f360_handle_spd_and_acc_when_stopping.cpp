/*===================================================================================*\
* FILE: f360_handle_spd_and_acc_when_stopping.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Handle_Spd_And_Acc_When_Stopping_CTCA() and Handle_Spd_And_Acc_When_Stopping_CCA().
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN, "APTIV C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

/******************************
* Includes
*******************************/

#include "f360_handle_spd_and_acc_when_stopping.h"
#include "f360_math_func.h"

namespace f360_variant_A
{
   /*===========================================================================*\
     * FUNCTION: Handle_Spd_And_Acc_When_Stopping_CTCA()
     *===========================================================================
     * RETURN VALUE:
     * None
     *
     * PARAMETERS:
     * const float32_t previous_speed,
     * const F360_Calibrations_T& calibs,
     * F360_Object_Track_T& obj
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
     * This function sets object tangential acceleration and object speed to zero if
     * tangential acceleration is largely negative and the sign of speed has changed 
     * from positive to negative. Function is intended to be used on CTCA objects only.
     *
     * PRECONDITIONS:
     *
     * POSTCONDITIONS:
     * None
     \*===========================================================================*/
   void Handle_Spd_And_Acc_When_Stopping_CTCA(
         const float32_t previous_speed,
         const F360_Calibrations_T& calibs,
         F360_Object_Track_T& obj)
   {
      const bool f_object_is_braking_and_speed_changes_sign_to_negative = (obj.tang_accel < -calibs.k_abs_acc_threshold_for_breaking) && (F360_Sign(obj.speed) == -1) && (F360_Sign(previous_speed) > -1);
      if (f_object_is_braking_and_speed_changes_sign_to_negative)
      {
         obj.speed = 0.0F;
         obj.tang_accel = 0.0F;
      }
   }


   /*===========================================================================*\
   * FUNCTION: Handle_Spd_And_Acc_When_Stopping_CCA()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_VCS_Velocity_T& previous_velocity,
   * const F360_Calibrations_T& calibs,
   * F360_Object_Track_T& obj
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
   * This function sets object acceleration and velocity to zero if
   * the magnitude of the acceleration is large and the projection 
   * of the velocity vector onto the acceleration vector changes sign
   * from negative to positive. This function is intended to be used for
   * CCA objects only.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   void Handle_Spd_And_Acc_When_Stopping_CCA(
      const F360_VCS_Velocity_T& previous_velocity,
      const F360_Calibrations_T& calibs,
      F360_Object_Track_T& obj)
   {
      const float32_t acceleration_magnitude = F360_Get_Hypotenuse(obj.vcs_accel.longitudinal, obj.vcs_accel.lateral);
      if ((acceleration_magnitude > calibs.k_abs_acc_threshold_for_breaking) && (std::abs(obj.speed) < calibs.k_abs_speed_threshold_for_stopping))
      {
         const float32_t acceleration_magnitude_inv = 1.0F / acceleration_magnitude;
         const float32_t unit_vector_in_acceleration_direction[2] = { obj.vcs_accel.longitudinal * acceleration_magnitude_inv, obj.vcs_accel.lateral * acceleration_magnitude_inv };
         const float32_t prev_projected_velocity = previous_velocity.longitudinal * unit_vector_in_acceleration_direction[0] + previous_velocity.lateral * unit_vector_in_acceleration_direction[1];
         const float32_t curr_projected_velocity = obj.vcs_velocity.longitudinal * unit_vector_in_acceleration_direction[0] + obj.vcs_velocity.lateral * unit_vector_in_acceleration_direction[1];
         if ((F360_Sign(prev_projected_velocity) == -1) && (F360_Sign(curr_projected_velocity) == 1))
         {
            obj.vcs_accel.longitudinal = 0.0F;
            obj.vcs_accel.lateral = 0.0F;
            obj.tang_accel = 0.0F;
            obj.vcs_velocity.longitudinal = 0.0F;
            obj.vcs_velocity.lateral = 0.0F;
            obj.speed = 0.0F;
         }
      }
   }


}
