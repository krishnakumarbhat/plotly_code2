/*===================================================================================*\
* FILE: f360_overall_confidence_helpers.cpp
*====================================================================================
*Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* Contains function definitions for functions related to the overall confidence helpers.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_overall_confidence_helpers.h"
#include "f360_math.h"

namespace f360_variant_A
{
   static void Decay_Confidence(CONF9_T& confidence);
   static void Counter_Decrease_Rollover_Safe(const uint8_t value, uint8_t& counter);

   /*===========================================================================*\
   * FUNCTION: Overall_Confidence_Decay
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * F360_Object_Track_T(&object_track) - reference to objects tracks struct
   * const F360_Calibrations_T& calib - reference to calibrations struct
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
   * Attempt to decay the internal confidence states for a track.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Overall_Confidence_Decay(F360_Object_Track_T& object_track, const float32_t timeout_to_start_decay_th)
   {
      if ((F360_OBJECT_STATUS_COASTED == object_track.status) && (object_track.time_since_stage_start > timeout_to_start_decay_th))
      {
         Decay_Confidence(object_track.conf_longitudinal_position);
         Decay_Confidence(object_track.conf_lateral_position);
         Decay_Confidence(object_track.conf_speed);
      }
   }

   /*===========================================================================*\
   * FUNCTION: Overall_Confidence_Update
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * F360_Object_Track_T(&object_track) - reference to objects track struct
   * const F360_Calibrations_T& calib - reference to calibrations struct
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
   * Run the state machine for internal confdence states.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Overall_Confidence_Update(F360_Object_Track_T& object_track, const F360_Calibrations_T& calib)
   {
      const float32_t difference_vcs_long_posn = std::abs(object_track.vcs_position.x - object_track.predicted_vcs_position.x);
      const float32_t difference_vcs_lat_posn = std::abs(object_track.vcs_position.y - object_track.predicted_vcs_position.y);

      Confidence_State_Machine(object_track.conf_longitudinal_position, difference_vcs_long_posn, calib.k_conf_overall_difference_thresh_long_posn_h, calib.k_conf_overall_difference_thresh_long_posn_m, calib.k_conf_overall_difference_thresh_long_posn_l, calib.k_conf_overall_difference_thresh_long_posn_vl);
      Confidence_State_Machine(object_track.conf_lateral_position, difference_vcs_lat_posn, calib.k_conf_overall_difference_thresh_lat_posn_h, calib.k_conf_overall_difference_thresh_lat_posn_m, calib.k_conf_overall_difference_thresh_lat_posn_l, calib.k_conf_overall_difference_thresh_lat_posn_vl);

      const float32_t difference_speed = std::abs(object_track.speed - object_track.predicted_speed); 
      if (F360_TRACKER_TRKFLTR_CCA == object_track.trk_fltr_type)
      {   
         Confidence_State_Machine(object_track.conf_speed, difference_speed, calib.k_conf_cca_overall_difference_thresh_speed_h, calib.k_conf_cca_overall_difference_thresh_speed_m, calib.k_conf_cca_overall_difference_thresh_speed_l, calib.k_conf_cca_overall_difference_thresh_speed_vl);
      }
      else
      {
         Confidence_State_Machine(object_track.conf_speed, difference_speed, calib.k_conf_ctca_overall_difference_thresh_speed_h, calib.k_conf_ctca_overall_difference_thresh_speed_m, calib.k_conf_ctca_overall_difference_thresh_speed_l, calib.k_conf_ctca_overall_difference_thresh_speed_vl);
      }
   }

   /*===========================================================================*\
   * FUNCTION: Confidence_State_Machine
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * CONF9_T& current_conf - Current state confidence
   * const float32_t difference - Difference between the predicted and updated state 
   * const float32_t thresh_easy - The easiest threshold to pass 
   * const float32_t thresh_medium - The medium threshold to pass 
   * const float32_t thresh_hard - The hard threshold to pass
   * const float32_t thresh_very_hard - The very hard threshold to pass
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
   * State machine definition for internal state confidences.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Confidence_State_Machine(CONF9_T& current_conf, const float32_t difference, const float32_t thresh_easy, const float32_t thresh_medium, const float32_t thresh_hard, const float32_t thresh_very_hard)
   {
      uint8_t new_confidence;
      if ((difference > thresh_easy) || (current_conf == CONF9_NONE))
      {
         // If the difference is larger than the easiest threshold or if it's a new track, set the confidence to the lowest level
         new_confidence = CONF9_LOW1;
      }
      else if (difference < thresh_very_hard)
      {
         // We pass the very hard threshold, jump two steps in the 9 stage Confidence scale (saturated to CONF9_HIGH)
         if (current_conf < CONF9_MED3)
         {
            new_confidence = static_cast<uint8_t>(current_conf) + 2U;
         }
         else
         {
            new_confidence = CONF9_HIGH;
         }
      }
      else
      {
         if (current_conf <= CONF9_LOW4)
         {
            // We're in the LOW1 - LOW4 interval. Increase confidence level when we pass the medium threshold, else leave it unchanged
            if (difference < thresh_medium)
            {
               new_confidence = static_cast<uint8_t>(current_conf) + 1U;
            }
            else
            {
               new_confidence = static_cast<uint8_t>(current_conf);
            }
         }
         else
         {
            // We're in the MED1 - HIGH interval
            if (difference < thresh_hard)
            {
               // We pass the hard threshold, increase the confidence up to max
               if (current_conf < CONF9_HIGH)
               {
                  new_confidence = static_cast<uint8_t>(current_conf) + 1U;
               }
               else
               {
                  new_confidence = static_cast<uint8_t>(current_conf);
               }
            }
            else if (difference < thresh_medium)
            {
               // We pass the medium threshold, leave the confidence unchanged
               new_confidence = static_cast<uint8_t>(current_conf);
            }
            else
            {
               // We only pass the easy threshold, reset the confidence to MED1
               new_confidence = CONF9_MED1;
            }
         }
      }
      current_conf = static_cast<CONF9_T>(new_confidence);
   }

   /*===========================================================================*\
   * FUNCTION: CTCA_Obj_Confidence_Less_Than_Or_Equal_To
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * F360_Object_Track_T(&object_track) - reference to objects track struct
   * const CONF9_T threshold - Confidence level threshold
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
   * Run the state machine for internal confdence states.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To(const F360_Object_Track_T& object_track, const CONF9_T threshold)
   {
      const bool result = (object_track.conf_longitudinal_position <= threshold) ||
         (object_track.conf_lateral_position <= threshold) ||
         (object_track.conf_speed <= threshold);

      return result;
   }

   /*===========================================================================*\
   * FUNCTION: Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * F360_Object_Track_T(&object_track) - reference to objects track struct
   * const CONF9_T threshold - Confidence level threshold
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
   * Run the state machine for internal confdence states.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To(const F360_Object_Track_T& object_track, const CONF9_T threshold)
   {
      const bool result = (object_track.conf_longitudinal_position <= threshold) ||
         (object_track.conf_lateral_position <= threshold) ||
         (object_track.conf_speed <= threshold);

      return result;
   }

   /*===========================================================================*\
   * FUNCTION: Decay_Confidence
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * CONF9_T& confidence
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
   * Reduce confidence
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static void Decay_Confidence(CONF9_T& confidence)
   {
      if (confidence > CONF9_LOW1)
      {
         confidence = static_cast<CONF9_T>(static_cast<uint8_t>(confidence) - 1U);
      }
   }

   /*===========================================================================*\
   * FUNCTION: Update_Low_RCS_Dets_Counter_And_Block_Confidence
   *===========================================================================
   * RETURN VALUE:
   * bool f_block_confidence
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list - Raw detection list 
   * const F360_Calibrations_T& calib - Tracker calibrations
   * F360_Object_Track_T& object_track - Object to increment/decrement conf block counter for
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * When there is only one associated detection and that detection has low RCS,
   * increase a counter. When there are multiple detections or when detection does 
   * not have low RCS, decrease the counter.
   * While the counter is nonzero, prevent the overall confidence from increasing. The logic 
   * has a latch functionality and is only relevevant for newly created tracks.
   * This function is primarily designed to catch ghost objects created on tram
   * tracks near host.
   * 
   \*===========================================================================*/
   bool Update_Low_RCS_Dets_Counter_And_Block_Confidence(
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Calibrations_T& calib,
      F360_Object_Track_T& object_track)
   {
      // First determine if the counter should be updated
      if ((1U == object_track.ndets) &&
         (object_track.low_rcs_dets_cnt > 0U) &&
         (F360_OBJECT_STATUS_UPDATED == object_track.status))
      {
         const rspp_variant_A::RSPP_Detection_T& detection = raw_detect_list.detections[object_track.detids[0] - 1U];

         // Evaluate range, RCS and range-rate
         if ((detection.raw.range < calib.k_ocb_max_range) && 
            (detection.raw.rcs < calib.k_ocb_rcs_thresh_midlow_rcs) && 
            (std::abs(detection.raw.range_rate) < calib.k_ocb_max_range_rate))
         {
            // Increase the counter with different values depending on the RCS of the detection
            if (detection.raw.rcs < calib.k_ocb_rcs_thresh_low_rcs)
            {
               object_track.low_rcs_dets_cnt += calib.k_ocb_cnt_delta_low_rcs_or_mult_dets;
            }
            else
            {
               object_track.low_rcs_dets_cnt += calib.k_ocb_cnt_delta_midlow_rcs;
            }

            // Ensure the counter does not increase over the maximum value
            if (object_track.low_rcs_dets_cnt > calib.k_ocb_cnt_max)
            {
               object_track.low_rcs_dets_cnt = calib.k_ocb_cnt_max;
            }
         }
         else
         {
            // Decrease the counter with different values depending on the RCS
            if (detection.raw.rcs > calib.k_ocb_rcs_thresh_hi_rcs)
            {
               Counter_Decrease_Rollover_Safe(calib.k_ocb_cnt_delta_low_rcs_or_mult_dets, object_track.low_rcs_dets_cnt);
            }
            else
            {
               Counter_Decrease_Rollover_Safe(calib.k_ocb_cnt_delta_midlow_rcs, object_track.low_rcs_dets_cnt);
            }
         }
      }
      else if (object_track.ndets > 1U)
      {
         // More than one detection associated; decrease the counter with the larger value.
         Counter_Decrease_Rollover_Safe(calib.k_ocb_cnt_delta_low_rcs_or_mult_dets, object_track.low_rcs_dets_cnt);
      }
      else
      {
         // Do nothing. No detections associated, or counter not active
      }

      const bool f_block_confidence = (object_track.low_rcs_dets_cnt > 0U);
      return f_block_confidence;
   }

   /*===========================================================================*\
  * FUNCTION: Counter_Decrease_Rollover_Safe
  *===========================================================================
  * RETURN VALUE:
  * None
  *
  * PARAMETERS:
  * const uint8_t value,
  * uint8_t& counter
  *
  * --------------------------------------------------------------------------
  * ABSTRACT:
  * --------------------------------------------------------------------------
  * Decrese counter with a value. Prevent rollover.
  *
  \*===========================================================================*/
   static void Counter_Decrease_Rollover_Safe(const uint8_t value, uint8_t& counter)
   {
      if (counter > value)
      {
         counter -= value;
      }
      else
      {
         counter = 0U;
      }
   }
}
