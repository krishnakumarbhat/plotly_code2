/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
#include "f360_cvt_determine_trailer_type.h"
#include "f360_math_func.h"

namespace f360_variant_A
{
   /*===========================================================================
    * TYPE: Para_Orth_Box_T
    *===========================================================================
    * DESCRIPTION:
    * Struct for holding the min/max dims in para-orth coordinates
    *=========================================================================*/
   typedef struct Para_Orth_Box_Tag
   {
      float32_t para_max;
      float32_t para_min;
      float32_t orth_max;
      float32_t orth_min;
   }Para_Orth_Box_T;

   /*===========================================================================
    * FUNCTION: Get_Trailer_Para_Orth_Limits
    *===========================================================================
    * DESCRIPTION:
    * Translate the trailer into para-orth coordinates and get the max/min dims
    *=========================================================================*/
   static Para_Orth_Box_T Get_Trailer_Para_Orth_Limits(
      const float32_t trailer_angle_sin,
      const float32_t trailer_angle_cos,
      const float32_t joint_longpos_vcs,
      const float32_t joint_latpos_vcs,
      const float32_t joint2center_distance,
      const float32_t trailer_length,
      const float32_t trailer_width)
   {
      const float32_t trailer_center_vcs_longpos = joint_longpos_vcs - trailer_angle_cos * joint2center_distance;
      const float32_t trailer_center_vcs_latpos = joint_latpos_vcs -trailer_angle_sin * joint2center_distance;

      Para_Orth_Box_T trailer;
      trailer.para_max = trailer_angle_cos * trailer_center_vcs_longpos + trailer_angle_sin * trailer_center_vcs_latpos + 0.5F * trailer_length;
      trailer.para_min = trailer_angle_cos * trailer_center_vcs_longpos + trailer_angle_sin * trailer_center_vcs_latpos - 0.5F * trailer_length;
      trailer.orth_max = -trailer_angle_sin * trailer_center_vcs_longpos + trailer_angle_cos * trailer_center_vcs_latpos + 0.5F * trailer_width;
      trailer.orth_min = -trailer_angle_sin * trailer_center_vcs_longpos + trailer_angle_cos * trailer_center_vcs_latpos - 0.5F * trailer_width;

      return trailer;
   }

   /*===========================================================================
    * FUNCTION: Determine_Trailer_Type
    *===========================================================================
    * DESCRIPTION:
    * Determine if the detections better fit the one-trailer or two-trailer model
    *=========================================================================*/
   void Determine_Trailer_Type(
      const float32_t host_speed,
      const float32_t vcs_sideslip,
      const F360_CVT_Detection_Info_T(&detections)[CVT_MAX_NUMBER_OF_DETECTIONS],
      F360_CVT_State_T& cvt_state)
   {
      constexpr float32_t k_min_abs_trailer_angle = F360_DEG2RAD(10.0F);
      F360_CVT_Model_T estimated_model = cvt_state.best_trailer_model;
      F360_CVT_Model_T selected_model = cvt_state.best_trailer_model;
      const bool f_radar_left_side = cvt_state.radar_vcs_latpos < 0.0F;

      bool f_angle_ok;
      if (f_radar_left_side)
      {
         f_angle_ok = ((cvt_state.two_link.ekf_state[0] > 0.0F) &&  // 1st trailer should be visible -- The vcs angle of 1st trailer of 2-link model should be larger than 0
                       (cvt_state.two_link.ekf_state[3] > k_min_abs_trailer_angle) &&  // 2nd trailer should be visible -- The vcs angle of 2nd trailer of 2-link model should be larger than 10 degrees
                       (cvt_state.one_link.ekf_state[0] > k_min_abs_trailer_angle));  // 1st trailer should be visible -- The vcs angle of the trailer of 1-link model should be larger than 10 degrees
      }
      else
      {
         f_angle_ok = ((cvt_state.two_link.ekf_state[0] < 0.0F) &&  // 1st trailer should be visible -- The vcs angle of 1st trailer of 2-link model should be smaller than 0
                       (cvt_state.two_link.ekf_state[3] < -k_min_abs_trailer_angle) &&  // 2nd trailer should be visible -- The vcs angle of 2nd trailer of 2-link model should be smaller than -10 degrees
                       (cvt_state.one_link.ekf_state[0] < -k_min_abs_trailer_angle));  // 1st trailer should be visible -- The vcs angle of the trailer of 1-link model should be smaller than -10 degrees
      }

      if ((host_speed > 0.01F) && (cvt_state.n_valid_dets > 0) && f_angle_ok)
      {
         bool inside_one_joint[MAX_NUMBER_OF_VALID_DETECTIONS_CV_TRAILER] = {};
         bool inside_two_joint[MAX_NUMBER_OF_VALID_DETECTIONS_CV_TRAILER] = {};
         float32_t one_joint_fill_ratio = -1.0F;
         float32_t two_joint_fill_ratio = -1.0F;
         
         const bool f_run_one_joint = cvt_state.one_link.filter_state != TRAILER_FILTER_STATE_NOT_STARTED;
         const float32_t k_trailer_width_buffer_for_inlier_sorting = 0.5F;
         if (f_run_one_joint)
         {
            const float32_t trailer_angle_sin = F360_Sinf(cvt_state.one_link.ekf_state[0]);
            const float32_t trailer_angle_cos = F360_Cosf(cvt_state.one_link.ekf_state[0]);

            const Para_Orth_Box_T trailer = Get_Trailer_Para_Orth_Limits(trailer_angle_sin, trailer_angle_cos,
               cvt_state.one_link.joint_vcs_longpos, cvt_state.one_link.joint_vcs_latpos, cvt_state.one_link.joint_dist_to_center,
               cvt_state.one_link.trailer_length, cvt_state.one_link.trailer_width + k_trailer_width_buffer_for_inlier_sorting);

            float32_t max_para = -1000.0F;
            float32_t min_para = 1000.0F;
            for (int32_t i = 0; i < cvt_state.n_valid_dets; i++)
            {
               const int32_t det_idx = cvt_state.valid_det_idx[i];
               const float32_t det_para = trailer_angle_cos * detections[det_idx].vcs_longpos + trailer_angle_sin * detections[det_idx].vcs_latpos;
               const float32_t det_orth = -trailer_angle_sin * detections[det_idx].vcs_longpos + trailer_angle_cos * detections[det_idx].vcs_latpos;
               
               if ((det_para < trailer.para_max) && (det_para > trailer.para_min) && (det_orth < trailer.orth_max) && (det_orth > trailer.orth_min))
               {
                  inside_one_joint[i] = true;
                  max_para = fmaxf(max_para, det_para);
                  min_para = fminf(min_para, det_para);
               }
            }
            one_joint_fill_ratio = (max_para - min_para) / (trailer.para_max - trailer.para_min);
         }

         const bool f_run_two_joint = cvt_state.two_link.filter_state != TRAILER_FILTER_STATE_NOT_STARTED;
         if (f_run_two_joint)
         {
            const float32_t trailer1_angle_sin = F360_Sinf(cvt_state.two_link.ekf_state[0]);
            const float32_t trailer1_angle_cos = F360_Cosf(cvt_state.two_link.ekf_state[0]);
            const float32_t trailer2_angle_sin = F360_Sinf(cvt_state.two_link.ekf_state[3]);
            const float32_t trailer2_angle_cos = F360_Cosf(cvt_state.two_link.ekf_state[3]);

            const Para_Orth_Box_T trailer1 = Get_Trailer_Para_Orth_Limits(trailer1_angle_sin, trailer1_angle_cos,
               cvt_state.two_link.joint1_vcs_longpos, cvt_state.two_link.joint1_vcs_latpos, cvt_state.two_link.joint1_dist_to_center,
               cvt_state.two_link.trailer1_length, cvt_state.two_link.trailer1_width + k_trailer_width_buffer_for_inlier_sorting);

            const Para_Orth_Box_T trailer2 = Get_Trailer_Para_Orth_Limits(trailer2_angle_sin, trailer2_angle_cos,
               cvt_state.two_link.joint2_vcs_longpos, cvt_state.two_link.joint2_vcs_latpos, cvt_state.two_link.joint2_dist_to_center,
               cvt_state.two_link.trailer2_length, cvt_state.two_link.trailer2_width + k_trailer_width_buffer_for_inlier_sorting);

            float32_t max_para = -1000.0F;
            float32_t min_para = 1000.0F;
            for (int32_t i = 0; i < cvt_state.n_valid_dets; i++)
            {
               const int32_t det_idx = cvt_state.valid_det_idx[i];
               const float32_t det_para1 = trailer1_angle_cos * detections[det_idx].vcs_longpos + trailer1_angle_sin * detections[det_idx].vcs_latpos;
               const float32_t det_orth1 = -trailer1_angle_sin * detections[det_idx].vcs_longpos + trailer1_angle_cos * detections[det_idx].vcs_latpos;
               const float32_t det_para2 = trailer2_angle_cos * detections[det_idx].vcs_longpos + trailer2_angle_sin * detections[det_idx].vcs_latpos;
               const float32_t det_orth2 = -trailer2_angle_sin * detections[det_idx].vcs_longpos + trailer2_angle_cos * detections[det_idx].vcs_latpos;

               const bool inside_trailer1 = (det_para1 < trailer1.para_max) && (det_para1 > trailer1.para_min) && (det_orth1 < trailer1.orth_max) && (det_orth1 > trailer1.orth_min);
               const bool inside_trailer2 = (det_para2 < trailer2.para_max) && (det_para2 > trailer2.para_min) && (det_orth2 < trailer2.orth_max) && (det_orth2 > trailer2.orth_min);
               
               if (inside_trailer1)
               {
                  inside_two_joint[i] = true;
               }

               if (inside_trailer2)
               {
                  inside_two_joint[i] = true;
                  max_para = fmaxf(max_para, det_para2);
                  min_para = fminf(min_para, det_para2);
               }
            }
            two_joint_fill_ratio = (max_para - min_para) / (trailer2.para_max - trailer2.para_min);
         }

         // negative for one-link, positive for two-link
         int32_t selector_score = 0;
         for (int32_t i = 0; i < cvt_state.n_valid_dets; i++)
         {
            if (inside_one_joint[i] && (!inside_two_joint[i]))
            {
               selector_score--;
            }
            else if ((!inside_one_joint[i]) && inside_two_joint[i])
            {
               selector_score++;
            }
            else
            {
               // do nothing
            }
         }

         const float32_t one_joint_fill_ratio_error = fabsf(one_joint_fill_ratio - 1.0F);
         const float32_t two_joint_fill_ratio_error = fabsf(two_joint_fill_ratio - 1.0F);
         const float32_t ratio_error_thresh = 0.25F;
         const int32_t min_n_dets_model_switch = 4;
         if ((selector_score <= -min_n_dets_model_switch) || 
            ((two_joint_fill_ratio_error > ratio_error_thresh) && ((two_joint_fill_ratio_error - one_joint_fill_ratio_error) > ratio_error_thresh)))
         {
            selected_model = TRAILER_MODEL_ONE_LINK;   
         }
         else if ((selector_score >= min_n_dets_model_switch) || 
            ((one_joint_fill_ratio_error > ratio_error_thresh) && ((one_joint_fill_ratio_error - two_joint_fill_ratio_error) > ratio_error_thresh)))
         {
            selected_model = TRAILER_MODEL_TWO_LINK;
         }
         else
         {
            // do nothing
         }
         
         // Apply hysteresis for model change, dependent on the current sideslip angle
         const bool f_best_model_candidate_changed = (estimated_model != selected_model);
         const bool f_previous_best_model_valid = (estimated_model != TRAILER_MODEL_NOT_AVAILABLE);
         if (f_best_model_candidate_changed && f_previous_best_model_valid)
         {
            constexpr float32_t sideslip_threshold = F360_DEG2RAD(5.0F);
            const uint16_t model_change_counter_thres_turning = 20U;  // wait for 1 second consecutively
            const uint16_t model_change_counter_thres_straight = 50U;  // wait for 2.5 seconds 

            cvt_state.best_model_change_counter++;
            const bool f_high_sideslip = (std::abs(vcs_sideslip) > sideslip_threshold);
            const bool f_model_change_needed_for_turning_case = (f_high_sideslip && (cvt_state.best_model_change_counter > model_change_counter_thres_turning));
            const bool f_model_change_needed_for_going_straight_case = ((!f_high_sideslip) && (cvt_state.best_model_change_counter > model_change_counter_thres_straight));
            if (f_model_change_needed_for_turning_case || f_model_change_needed_for_going_straight_case)
            {
               // counter reached the threshold for changing model
               estimated_model = selected_model;
               cvt_state.best_model_change_counter = 0U;
            }
         }
         else
         {
            // The best model candidate is same as previous, or previously no model was selected
            cvt_state.best_model_change_counter = 0U;
            estimated_model = selected_model;
         }
      }

      cvt_state.best_trailer_model = estimated_model;
   }
}
