/******************************************************************************
 * Copyright 2024 Aptiv, All Rights Reserved.
 * Aptiv Confidential
 ******************************************************************************/
#include <cstring>
#include "f360_cvt_estimator.h"
#include "f360_constants.h"
#include "f360_math.h"

#include "f360_cvt_generate_measurement.h"
#include "f360_cvt_one_link_ekf.h"
#include "f360_cvt_two_link_ekf.h"
#include "f360_cvt_discrete_derivative.h"
#include "f360_cvt_calc_trailer_speed.h"
#include "f360_cvt_estimate_trailer_length.h"
#include "f360_cvt_saturated_odometer.h"
#include "f360_cvt_determine_trailer_type.h"

namespace f360_variant_A
{
   /*===========================================================================
    * FUNCTION: CVT_Reset
    *===========================================================================
    * DESCRIPTION:
    * Reset internal CVT state data
    *=========================================================================*/
   void CVT_Reset(F360_CVT_State_T& cvt_state)
   {
      (void)memset(&cvt_state, 0, sizeof(cvt_state));
   }

   /*===========================================================================
    * FUNCTION: CVT_Initialize
    *===========================================================================
    * DESCRIPTION:
    * Initialization of internal CVT state data
    *=========================================================================*/
   void CVT_Initialize(
      const F360_Calibrations_T& calibrations,
      const F360_CVT_Initialization_Data_T& init_data,
      F360_CVT_State_T& cvt_state)
   {
      const float32_t k_init_trailer_length = 5.0F;
      const float32_t k_max_valid_vehicle_length = 60.0F;

      F360_CVT_State_T prev_cvt_state{};
      if (cvt_state.f_initiated_from_states)
      {
         // If we have initialized from states, make a copy of the previous cvt_state
         (void)std::memcpy(&prev_cvt_state, &cvt_state, sizeof(prev_cvt_state));
      }

      // reset all variable in cvt_state
      (void)std::memset(&cvt_state, 0, sizeof(cvt_state));

      // Set constraints
      cvt_state.state_constraints.min_x0 = -0.5F * F360_PI;
      cvt_state.state_constraints.max_x0 = 0.5F * F360_PI;
      cvt_state.state_constraints.min_x1 = -(init_data.joint1_vcs_longpos_max - init_data.host_rear_axle_vcs_longpos);
      cvt_state.state_constraints.max_x1 = -(init_data.joint1_vcs_longpos_min - init_data.host_rear_axle_vcs_longpos);
      cvt_state.state_constraints.min_x2 = init_data.link1_wheelbase_min;
      cvt_state.state_constraints.max_x2 = init_data.link1_wheelbase_max;
      cvt_state.state_constraints.min_x3 = -0.75F * F360_PI;
      cvt_state.state_constraints.max_x3 = 0.75F * F360_PI;
      cvt_state.state_constraints.min_x4 = init_data.link2_wheelbase_min;
      cvt_state.state_constraints.max_x4 = init_data.link2_wheelbase_max;

      const bool f_external_state_init = (prev_cvt_state.f_initiated_from_states &&
         (prev_cvt_state.one_link.n_updates >= 0) &&
         (prev_cvt_state.one_link.ekf_state_errcov[0][0] > 0.0F) &&
         (prev_cvt_state.one_link.ekf_state_errcov[1][1] > 0.0F) &&
         (prev_cvt_state.one_link.ekf_state_errcov[2][2] > 0.0F) &&
         (prev_cvt_state.one_link.full_vehicle_length > init_data.host_length) &&
         (prev_cvt_state.one_link.full_vehicle_length <= k_max_valid_vehicle_length) &&
         (prev_cvt_state.one_link.ekf_state[0] >= cvt_state.state_constraints.min_x0) &&
         (prev_cvt_state.one_link.ekf_state[0] <= cvt_state.state_constraints.max_x0) &&
         (prev_cvt_state.one_link.ekf_state[1] >= cvt_state.state_constraints.min_x1) &&
         (prev_cvt_state.one_link.ekf_state[1] <= cvt_state.state_constraints.max_x1) &&
         (prev_cvt_state.one_link.ekf_state[2] >= cvt_state.state_constraints.min_x2) &&
         (prev_cvt_state.one_link.ekf_state[2] <= cvt_state.state_constraints.max_x2) &&
         (prev_cvt_state.two_link.n_updates >= 0) &&
         (prev_cvt_state.two_link.ekf_state_errcov[0][0] > 0.0F) &&
         (prev_cvt_state.two_link.ekf_state_errcov[1][1] > 0.0F) &&
         (prev_cvt_state.two_link.ekf_state_errcov[2][2] > 0.0F) &&
         (prev_cvt_state.two_link.ekf_state_errcov[3][3] > 0.0F) &&
         (prev_cvt_state.two_link.ekf_state_errcov[4][4] > 0.0F) &&
         (prev_cvt_state.two_link.full_vehicle_length > init_data.host_length) &&
         (prev_cvt_state.two_link.full_vehicle_length <= k_max_valid_vehicle_length) &&
         (prev_cvt_state.two_link.ekf_state[0] >= cvt_state.state_constraints.min_x0) &&
         (prev_cvt_state.two_link.ekf_state[0] <= cvt_state.state_constraints.max_x0) &&
         (prev_cvt_state.two_link.ekf_state[1] >= cvt_state.state_constraints.min_x1) &&
         (prev_cvt_state.two_link.ekf_state[1] <= cvt_state.state_constraints.max_x1) &&
         (prev_cvt_state.two_link.ekf_state[2] >= cvt_state.state_constraints.min_x2) &&
         (prev_cvt_state.two_link.ekf_state[2] <= cvt_state.state_constraints.max_x2) &&
         (prev_cvt_state.two_link.ekf_state[3] >= cvt_state.state_constraints.min_x3) &&
         (prev_cvt_state.two_link.ekf_state[3] <= cvt_state.state_constraints.max_x3) &&
         (prev_cvt_state.two_link.ekf_state[4] >= cvt_state.state_constraints.min_x4) &&
         (prev_cvt_state.two_link.ekf_state[4] <= cvt_state.state_constraints.max_x4)
         );

      if (f_external_state_init)
      {
         // If initiated from state, we copy the previous one_link and two_link states into the reset cvt_state
         // Note that: only the essential states are preserved in cvt_state here. 
         // Hence, variables like "one_link.joint_vcs_longpos" will stay as zeros until the CVT_Execute() is called.
         (void)std::memcpy(&cvt_state.one_link.ekf_state, &prev_cvt_state.one_link.ekf_state, sizeof(cvt_state.one_link.ekf_state));
         (void)std::memcpy(&cvt_state.one_link.ekf_state_errcov, &prev_cvt_state.one_link.ekf_state_errcov, sizeof(cvt_state.one_link.ekf_state_errcov));
         cvt_state.one_link.n_updates = prev_cvt_state.one_link.n_updates;
         cvt_state.one_link.full_vehicle_length = prev_cvt_state.one_link.full_vehicle_length;

         (void)std::memcpy(&cvt_state.two_link.ekf_state, &prev_cvt_state.two_link.ekf_state, sizeof(cvt_state.two_link.ekf_state));
         (void)std::memcpy(&cvt_state.two_link.ekf_state_errcov, &prev_cvt_state.two_link.ekf_state_errcov, sizeof(cvt_state.two_link.ekf_state_errcov));
         cvt_state.two_link.n_updates = prev_cvt_state.two_link.n_updates;
         cvt_state.two_link.full_vehicle_length = prev_cvt_state.two_link.full_vehicle_length;

         cvt_state.best_trailer_model = prev_cvt_state.best_trailer_model;
      }
      else
      {
         // Set initial states
         cvt_state.one_link.ekf_state[0] = init_data.one_link_model.link1_angle;
         cvt_state.one_link.ekf_state[1] = -(init_data.one_link_model.joint1_vcs_longpos - init_data.host_rear_axle_vcs_longpos);
         cvt_state.one_link.ekf_state[2] = init_data.one_link_model.link1_wheelbase;

         cvt_state.two_link.ekf_state[0] = init_data.two_link_model.link1_angle;
         cvt_state.two_link.ekf_state[1] = -(init_data.two_link_model.joint1_vcs_longpos - init_data.host_rear_axle_vcs_longpos);
         cvt_state.two_link.ekf_state[2] = init_data.two_link_model.link1_wheelbase;
         cvt_state.two_link.ekf_state[3] = init_data.two_link_model.link2_angle;
         cvt_state.two_link.ekf_state[4] = init_data.two_link_model.link2_wheelbase;

         // initialize the EKF covariances
         (void)memset(cvt_state.one_link.ekf_state_errcov, 0, sizeof(cvt_state.one_link.ekf_state_errcov));
         cvt_state.one_link.ekf_state_errcov[0][0] = 0.5F;
         cvt_state.one_link.ekf_state_errcov[1][1] = 1.0F;
         cvt_state.one_link.ekf_state_errcov[2][2] = 1.0F;

         (void)memset(cvt_state.two_link.ekf_state_errcov, 0, sizeof(cvt_state.two_link.ekf_state_errcov));
         cvt_state.two_link.ekf_state_errcov[0][0] = 0.05F;
         cvt_state.two_link.ekf_state_errcov[1][1] = 0.2F;
         cvt_state.two_link.ekf_state_errcov[2][2] = 0.5F;
         cvt_state.two_link.ekf_state_errcov[3][3] = 0.05F;
         cvt_state.two_link.ekf_state_errcov[4][4] = 1.0F;

         // initialize length parameters
         cvt_state.one_link.n_updates = init_data.one_link_model.n_updates;
         cvt_state.one_link.full_vehicle_length = init_data.one_link_model.full_vehicle_length;

         cvt_state.two_link.n_updates = init_data.two_link_model.n_updates;
         cvt_state.two_link.full_vehicle_length = init_data.two_link_model.full_vehicle_length;

         // set initial model guess
         cvt_state.best_trailer_model = TRAILER_MODEL_ONE_LINK;
      }

      cvt_state.saturated_odometer_reversing_countermeasures = 0.0F;
      cvt_state.f_reversing_countermeasures_active = false;
      // Saturate initial states
      F360_CVT_State_Constraints_T& sc = cvt_state.state_constraints;

      cvt_state.one_link.ekf_state[0] = fminf(sc.max_x0, fmaxf(sc.min_x0, cvt_state.one_link.ekf_state[0]));
      cvt_state.one_link.ekf_state[1] = fminf(sc.max_x1, fmaxf(sc.min_x1, cvt_state.one_link.ekf_state[1]));
      cvt_state.one_link.ekf_state[2] = fminf(sc.max_x2, fmaxf(sc.min_x2, cvt_state.one_link.ekf_state[2]));

      cvt_state.two_link.ekf_state[0] = fminf(sc.max_x0, fmaxf(sc.min_x0, cvt_state.two_link.ekf_state[0]));
      cvt_state.two_link.ekf_state[1] = fminf(sc.max_x1, fmaxf(sc.min_x1, cvt_state.two_link.ekf_state[1]));
      cvt_state.two_link.ekf_state[2] = fminf(sc.max_x2, fmaxf(sc.min_x2, cvt_state.two_link.ekf_state[2]));
      cvt_state.two_link.ekf_state[3] = fminf(sc.max_x3, fmaxf(sc.min_x3, cvt_state.two_link.ekf_state[3]));
      cvt_state.two_link.ekf_state[4] = fminf(sc.max_x4, fmaxf(sc.min_x4, cvt_state.two_link.ekf_state[4]));

      cvt_state.best_model_change_counter = 0U;
      cvt_state.host_length = init_data.host_length;
      cvt_state.host_width = init_data.host_width;
      cvt_state.radar_id = init_data.radar_id;
      cvt_state.radar_vcs_longpos = init_data.radar_vcs_longpos;
      cvt_state.radar_vcs_latpos = init_data.radar_vcs_latpos;

      cvt_state.one_link.filter_state = TRAILER_FILTER_STATE_INIT;
      cvt_state.one_link.angle_rate = 0.0F;
      cvt_state.one_link.prev_trailer_angle = 0.0F;
      cvt_state.one_link.trailer_length = k_init_trailer_length;
      cvt_state.one_link.trailer_width = calibrations.k_cvt_trailer_width;

      cvt_state.two_link.filter_state = TRAILER_FILTER_STATE_INIT;
      cvt_state.two_link.angle_rate1 = 0.0F;
      cvt_state.two_link.angle_rate2 = 0.0F;
      cvt_state.two_link.prev_trailer1_angle = 0.0F;
      cvt_state.two_link.prev_trailer2_angle = 0.0F;
      cvt_state.two_link.trailer1_length = k_init_trailer_length;
      cvt_state.two_link.trailer2_length = k_init_trailer_length;
      cvt_state.two_link.trailer1_width = calibrations.k_cvt_trailer_width;
      cvt_state.two_link.trailer2_width = calibrations.k_cvt_trailer_width;


      cvt_state.f_init_complete =
         (cvt_state.one_link.full_vehicle_length > cvt_state.host_length) &&
         (cvt_state.one_link.full_vehicle_length <= k_max_valid_vehicle_length) &&
         (cvt_state.two_link.full_vehicle_length > cvt_state.host_length) &&
         (cvt_state.two_link.full_vehicle_length <= k_max_valid_vehicle_length) &&
         (cvt_state.host_width > 0.0F) &&
         (cvt_state.host_length > 0.0F) &&
         (cvt_state.radar_vcs_longpos < 0.0F) &&
         (cvt_state.radar_vcs_longpos > -cvt_state.host_length) &&
         (std::abs(cvt_state.radar_vcs_latpos) < (cvt_state.host_width * 0.5F + 0.5F));
   }

   /*===========================================================================
    * FUNCTION: CVT_Execute
    *===========================================================================
    * DESCRIPTION:
    * Top-level function for executing the CVTrailer Estimator
    *=========================================================================*/
   void CVT_Execute(
      const F360_Calibrations_T& calibrations,
      F360_CVT_Input_Data_T& cvt_input,
      F360_CVT_State_T& cvt_state)
   {
      Generate_Measurement(cvt_input, cvt_state);

      constexpr float32_t angle_gate_low_conf = F360_DEG2RAD(40.0F);
      constexpr float32_t angle_gate_hi_conf_1link = F360_DEG2RAD(10.0F);
      constexpr float32_t angle_gate_hi_conf_2link = F360_DEG2RAD(20.0F);
      const float32_t updates_for_hi_conf_1link = 100.0F;
      const float32_t updates_for_hi_conf_2link = 500.0F;

      const float32_t complete_percent1 = fminf(1.0F, static_cast<float32_t>(cvt_state.one_link.n_updates) / updates_for_hi_conf_1link);
      const bool f_host_yaw_rate_high = (fabsf(cvt_input.host_yawrate) > F360_DEG2RAD(15.0F));
      const bool f_1link_small_msmt_angle = (fabsf(cvt_state.primary_measurement.trailer_angle_vcs) < F360_DEG2RAD(20.0F));
      const float32_t angle_gate1_based_on_confidence = complete_percent1 * angle_gate_hi_conf_1link + (1.0F - complete_percent1) * angle_gate_low_conf;
      constexpr float32_t angle_gate1_if_small_msmt_angle = F360_DEG2RAD(20.0F);

      // use a larger angle gate to allow the trailer to catch up the measurement near the FoV border, when the host is turning aggressively and the measurement angle is small, 
      const bool f_use_larger_angle_gate_1link = (f_host_yaw_rate_high && 
                                                  f_1link_small_msmt_angle && 
                                                  (angle_gate1_based_on_confidence < angle_gate1_if_small_msmt_angle));

      const float32_t angle_gate1 = f_use_larger_angle_gate_1link ? angle_gate1_if_small_msmt_angle : angle_gate1_based_on_confidence;

      const float32_t complete_percent2 = fminf(1.0F, static_cast<float32_t>(cvt_state.two_link.n_updates) / updates_for_hi_conf_2link);
      const float32_t angle_gate2 = complete_percent2 * angle_gate_hi_conf_2link + (1.0F - complete_percent2) * angle_gate_low_conf;

      bool f_updated_1joint = false;
      One_Trailer_EKF(calibrations, cvt_input, angle_gate1, cvt_state, f_updated_1joint);
      
      cvt_state.one_link.joint_vcs_longpos = cvt_input.host_rear_axle_vcs_longpos - cvt_state.one_link.ekf_state[1];
      cvt_state.one_link.joint_vcs_latpos = 0.0F;
      cvt_state.one_link.joint_dist_to_wheels = cvt_state.one_link.ekf_state[2];

      discrete_time_derivative(cvt_state.one_link.ekf_state[0], cvt_state.one_link.prev_trailer_angle, cvt_state.one_link.angle_rate);

      const float32_t cos_sideslip = F360_Cosf(cvt_input.host_side_slip_vcs);
      const float32_t sin_sideslip = F360_Sinf(cvt_input.host_side_slip_vcs);
      const float32_t b1 = -cvt_input.host_rear_axle_vcs_longpos;

      // calc longvel
      calculate_trailer_longitudinal_velocity(
         F360_Cosf(cvt_state.one_link.ekf_state[0]),
         F360_Sinf(cvt_state.one_link.ekf_state[0]),
         cvt_state.one_link.ekf_state[1],
         cvt_input.host_speed,
         cos_sideslip,
         sin_sideslip,
         b1,
         cvt_state.one_link.longvel);

      bool f_updated_2joint = false;
      Two_Link_Trailer_EKF(calibrations, cvt_input, angle_gate2, cvt_state, f_updated_2joint);
      
      cvt_state.two_link.joint1_vcs_longpos = cvt_input.host_rear_axle_vcs_longpos - cvt_state.two_link.ekf_state[1];
      cvt_state.two_link.joint1_vcs_latpos = 0.0F;
      cvt_state.two_link.joint1_dist_to_wheels = cvt_state.two_link.ekf_state[2];

      cvt_state.two_link.joint2_vcs_longpos = cvt_state.two_link.joint1_vcs_longpos - F360_Cosf(cvt_state.two_link.ekf_state[0]) * cvt_state.two_link.ekf_state[2];
      cvt_state.two_link.joint2_vcs_latpos = F360_Sinf(-cvt_state.two_link.ekf_state[0]) * cvt_state.two_link.ekf_state[2];
      cvt_state.two_link.joint2_dist_to_wheels = cvt_state.two_link.ekf_state[4];

      discrete_time_derivative(cvt_state.two_link.ekf_state[0], cvt_state.two_link.prev_trailer1_angle, cvt_state.two_link.angle_rate1);
      discrete_time_derivative(cvt_state.two_link.ekf_state[3], cvt_state.two_link.prev_trailer2_angle, cvt_state.two_link.angle_rate2);
      
      calculate_trailer_longitudinal_velocity(
         F360_Cosf(cvt_state.two_link.ekf_state[0]),
         F360_Sinf(cvt_state.two_link.ekf_state[0]),         
         cvt_state.two_link.ekf_state[1],
         cvt_input.host_speed,
         cos_sideslip,
         sin_sideslip,
         b1,
         cvt_state.two_link.longvel1);
      
      // Only project the 1st trailer speed along the 2nd trailer longitudinally.
      const float32_t link1_wheelbase = cvt_state.two_link.ekf_state[2];
      const float32_t link2_angle_relative = cvt_state.two_link.ekf_state[3] - cvt_state.two_link.ekf_state[0];
      calculate_trailer_longitudinal_velocity(
         F360_Cosf(link2_angle_relative),
         F360_Sinf(link2_angle_relative),
         0.0F,
         cvt_state.two_link.longvel1,
         1.0F,
         0.0F,
         link1_wheelbase,
         cvt_state.two_link.longvel2);

      Run_Length_Filter(calibrations, f_updated_1joint, f_updated_2joint, cvt_state);

      Determine_Trailer_Type(cvt_input.host_speed, cvt_input.host_side_slip_vcs, cvt_input.detections, cvt_state);

      Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);

      const int32_t k_max_num_updates = 1000000;
      if (f_updated_1joint && (cvt_state.one_link.n_updates < k_max_num_updates))
      {
         cvt_state.one_link.n_updates++;
      }
      if (f_updated_2joint && (cvt_state.two_link.n_updates < k_max_num_updates))
      {
         cvt_state.two_link.n_updates++;
      }

      const int32_t k_cvt_min_n_updates = 100; // Number of updates needed to complete EKF initialization
      cvt_state.two_link.filter_state = (cvt_state.two_link.n_updates > k_cvt_min_n_updates) ? TRAILER_FILTER_STATE_ACTIVE : TRAILER_FILTER_STATE_INIT;
      cvt_state.one_link.filter_state = (cvt_state.one_link.n_updates > k_cvt_min_n_updates) ? TRAILER_FILTER_STATE_ACTIVE : TRAILER_FILTER_STATE_INIT;
   }
}
