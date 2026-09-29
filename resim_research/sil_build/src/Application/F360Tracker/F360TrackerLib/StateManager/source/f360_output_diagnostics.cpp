/*=========================================================================
*  FILE: f360_output_diagnostics.cpp
*=========================================================================
* Copyright (C) 2020 Aptiv. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains Output Diagnostics class definition.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
*========================================================================*/

/******************************
* Includes
*******************************/

#include "f360_output_diagnostics.h"
#include <cmath>

namespace f360_variant_A
{
   /*=========================================================================
   * Method         Output_Diagnostics::Output_Diagnostics
   *
   * Description    Constructor of Output_Diagnostics.
   *
   * Parameters     None.
   *
   * Returns        None.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   #if defined(__TMS320C6X__)
   #pragma FUNC_EXT_CALLED
   #endif
   Output_Diagnostics::Output_Diagnostics():
      calib{400.0F, 400.0F, 139.0F, -139.0F, 30.0F, -30.0F}
   {

   }

   /*=========================================================================
   * Method         Output_Diagnostics::~Output_Diagnostics
   *
   * Description    Destructor of Output_Diagnostics.
   *
   * Parameters     None.
   *
   * Returns        None.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   #if defined(__TMS320C6X__)
   #pragma FUNC_EXT_CALLED
   #endif
   Output_Diagnostics::~Output_Diagnostics()
   {

   }

   /*=========================================================================
   * Method         Output_Diagnostics::Execute
   *
   * Description    Main function of Output Diagnostics. Verifies object output
   *                parameters and tracker info, then sets output fault flags
   *                and stores unique ids of faulty objects.
   *
   * Parameters     const F360_Object_Log_Output_T& obj_log - object log output
   *                const Tracker_Info_Log_T& r_tracker_info - tracker diagnostics info
   *
   * Returns        Output_Faults_T - compressed information about detected output faults.
   *
   * Externals:     None.
   *
   * Precondition   Shall be called by SafetyLogic.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   #if defined(__TMS320C6X__)
   #pragma FUNC_EXT_CALLED
   #endif
   Output_Faults_T Output_Diagnostics::Execute(
       const F360_Object_Log_Output_T& obj_log,
       const Tracker_Info_Log_T& r_tracker_info
       ) const
   {
      Output_Faults_T output_faults {};
      uint32_t num_unique_faulty_obj = 0U;

      for (uint32_t obj_trk_idx = 0U; obj_trk_idx < obj_log.f360header.num_elements; obj_trk_idx++)
      {
         if (0U != obj_log.object[obj_trk_idx].status)
         {
            bool current_obj_faulty = false;

            if (Is_Track_Position_Incorrect(obj_log.object[obj_trk_idx].vcs_xposn, obj_log.object[obj_trk_idx].vcs_yposn))
            {
               output_faults.f_track_positions_faulty = true;
               current_obj_faulty = true;
            }
            if (Is_Track_Speed_Incorrect(obj_log.object[obj_trk_idx].speed))
            {
               output_faults.f_track_velocities_faulty = true;
               current_obj_faulty = true;
            }
            if (Is_Track_Acceleration_Incorrect(obj_log.object[obj_trk_idx].tang_accel))
            {
               output_faults.f_track_accelerations_faulty = true;
               current_obj_faulty = true;
            }

            if (current_obj_faulty)
            {
               num_unique_faulty_obj++;
            }
         }
      }

      if (num_unique_faulty_obj > 2U)
      {
         output_faults.f_enough_faulty_unique_objs = true;
      }

      if (r_tracker_info.f_severe_angle_jump_detected > 0U)
      {
          output_faults.f_severe_angle_jump_presence_fault = true;
      }
      return output_faults;
   }

   /*=========================================================================
   * Method         Output_Diagnostics::Is_Track_Position_Incorrect
   *
   * Description    Function verifies whether single track VCS position is incorrect.
   *
   * Parameters     const float32_t vcs_pos_long - track VCS longitudinal position
   *                const float32_t vcs_pos_lat - track VCS lateral position
   *
   * Returns        Flag indicating whether track position is faulty (not within limits).
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   bool Output_Diagnostics::Is_Track_Position_Incorrect(const float32_t vcs_pos_long, const float32_t vcs_pos_lat) const
   {
      return ((std::abs(vcs_pos_long) > calib.max_allowed_longitudinal_position) ||
               (std::abs(vcs_pos_lat) > calib.max_allowed_lateral_position));
   }

   /*=========================================================================
   * Method         Output_Diagnostics::Is_Track_Speed_Incorrect
   *
   * Description    Function verifies whether single track OTG velocity is incorrect.
   *
   * Parameters     const float32_t speed - track over-the-ground speed
   *
   * Returns        Flag indicating whether track speed is faulty (not within limits).
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   bool Output_Diagnostics::Is_Track_Speed_Incorrect(const float32_t speed) const
   {
      return ((speed < calib.min_allowed_speed) || (calib.max_allowed_speed < speed));
   }

   /*=========================================================================
   * Method         Output_Diagnostics::Is_Track_Acceleration_Incorrect
   *
   * Description    Function verifies whether single track tangential acceleration is incorrect.
   *
   * Parameters     const float32_t accel - track acceleration
   *
   * Returns        Flag indicating whether track tangential acceleration is faulty (not within limits).
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   bool Output_Diagnostics::Is_Track_Acceleration_Incorrect(const float32_t accel) const
   {
      return ((accel < calib.min_allowed_acceleration) || (calib.max_allowed_acceleration < accel));
   }
}
