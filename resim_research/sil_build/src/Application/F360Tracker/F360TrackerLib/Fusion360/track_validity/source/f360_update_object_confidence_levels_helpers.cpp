/*===================================================================================*\
* FILE: f360_update_object_confidence_levels_helpers.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function definition of supporting functions used in Update_Object_Confidence_Levels()
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#include "f360_math.h"
#include "f360_update_object_confidence_levels.h"
#include "f360_math_func.h"
#include "f360_update_object_confidence_levels_helpers.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Calc_Average_Confidence_Level
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t& elapsed_time - time since last tracker update
   * F360_Object_Track_T& object - analysed object
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
   * Function calculates average confidence level of an object.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Calc_Average_Confidence_Level(
      const float32_t& elapsed_time,
      F360_Object_Track_T& object)
   {
      if (object.time_since_initialization > 0.0F)
      {
         object.prev_avrg_conf_level = (object.prev_avrg_conf_level*(object.time_since_initialization - elapsed_time)
            + object.confidenceLevel*elapsed_time) / object.time_since_initialization;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Calculate_Conf_Tau_For_Coasted_Trks
   *===========================================================================
   * RETURN VALUE:
   * float32_t tau_updated
   *
   * PARAMETERS:
   * const F360_Object_Track_T& object - analysed object
   * const F360_Calibrations_T& calib - tracker calibrations
   *
   * EXTERNAL REFERENCES: confTrkConfidenceLevel.m
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function calculates confidence tau factor for coasted tracks
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Calculate_Conf_Tau_For_Coasted_Objects(
      const F360_Object_Track_T& object,
      const F360_Calibrations_T& calib)
   {
      float32_t tau;
      if ((calib.k_conf_coasted_max_long_posn_tructed_track < object.vcs_position.x) ||
         ((calib.k_conf_coasted_min_time_trusted_track < object.time_since_initialization) && (calib.k_conf_coasted_min_long_posn_tructed_track < std::abs(object.vcs_position.x))))
      {
         const float32_t large_tau_coasted = 1.0F;
         tau = large_tau_coasted;
      }
      else if ((calib.k_conf_coasted_min_time_since_init <= object.time_since_initialization) || (calib.k_conf_coasted_min_average_confidence_level <= object.prev_avrg_conf_level))
      {
         const float32_t medium_tau_coasted = 0.5F;
         tau = medium_tau_coasted;
      }
      else
      {
         const float32_t small_tau_coasted = 0.2F;
         tau = small_tau_coasted;
      }
      return tau;
   }

   /*===========================================================================*\
   * FUNCTION: Determine_Raw_Confidence_Level_Of_Coasted_Track
   *===========================================================================
   * RETURN VALUE:
   * float32_t raw_confidence_level - raw confidence level of object
   *
   * PARAMETERS:
   * const F360_Object_Track_T& object - analysed object
   * const float32_t time_since_coasted_track_updated_th - maximum time since coasted track was updated
   *
   * EXTERNAL REFERENCES: confTrkConfidenceLevel.m
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function calculates raw confidence level of coasted tracks.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Determine_Raw_Confidence_Level_Of_Coasted_Object(
      const F360_Object_Track_T& object,
      const float32_t time_since_coasted_track_updated_th)
   {
      float32_t raw_confidence_level;
      // if the track was updated within k_hyst_time_for_coasted_objects, then keep confidence level
      if (object.time_since_track_updated < time_since_coasted_track_updated_th)
      {
         raw_confidence_level = object.confidenceLevel;
      }
      else
      {
         raw_confidence_level = 0.0F;
      }
      return raw_confidence_level;
   }

   /*===========================================================================*\
   * FUNCTION: Determine_Raw_Confidence_Level_Of_Updated_Object
   *===========================================================================
   * RETURN VALUE:
   * float32_t raw_confidence_level - raw confidence level of updated track
   *
   * PARAMETERS:
   * const int32_t object_num_rr_inlier_dets - number of dets with rr close to predicted rr to analysed object;
   * const int32_t object_n_dets - number of all detections associated to analysed object;
   * const float32_t k_conf_raw_weight_dets - raw confidence detections weight;
   * const float32_t max_confidence_not_reduced_dets - max confidence level when object has only associated, but no reduced dets;
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
   * Function calculates raw confidence level of updated objects
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Determine_Raw_Confidence_Level_Of_Updated_Object(
      const uint32_t object_num_rr_inlier_dets,
      const uint32_t object_n_dets,
      const float32_t k_conf_raw_weight_dets,
      const float32_t max_confidence_not_reduced_dets)
   {
      float32_t raw_confidence_level;
      if (object_num_rr_inlier_dets > 0U)
      {
         raw_confidence_level = 1.0F - F360_Expf(k_conf_raw_weight_dets * static_cast<float32_t>(object_num_rr_inlier_dets));
      }
      else if (object_n_dets > 0U)
      {
         raw_confidence_level = max_confidence_not_reduced_dets - F360_Expf(k_conf_raw_weight_dets * static_cast<float32_t>(object_n_dets));
      }
      else
      {
         raw_confidence_level = 0.0F;
      }
      return raw_confidence_level;
   }

   /*===========================================================================*\
   * FUNCTION: Reduce_Confidence_Based_On_Object_Flags
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t k_conf_overlapping_reduction_factor - factor used to reduce object confidence level
   * F360_Object_Track_T& object - analysed object
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
   * Function reduces object confidence level basing on object specific flags.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Reduce_Confidence_Based_On_Object_Flags(
      const float32_t k_conf_overlapping_reduction_factor,
      F360_Object_Track_T& object)
   {
      if (object.f_overlapping_with_object)
      {
         object.confidenceLevel = object.confidenceLevel * k_conf_overlapping_reduction_factor;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Determine_Filter_Coef
   *===========================================================================
   * RETURN VALUE:
   * float32_t alpha - filter coefficient
   *
   * PARAMETERS:
   * const F360_Object_Track_T& object - analysed object
   * const float32_t elapsed_time - time since last tracker update
   * const F360_Calibrations_T& calib - tracker calibrations
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
   * Function calculates first order alpha coefficent basing on object status and calibrations.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Determine_Filter_Coef(
      const F360_Object_Track_T& object,
      const float32_t elapsed_time,
      const F360_Calibrations_T& calib)
   {
      float32_t alpha;
      if (((F360_OBJECT_STATUS_UPDATED == object.status)
         || (F360_OBJECT_STATUS_COASTED == object.status))
         && (object.time_since_track_updated < calib.k_hyst_time_for_coasted_objects))
      {
         alpha = F360_Expf(-elapsed_time / calib.k_conf_updated_tracks_filter_const);
      }
      else
      {
         const float32_t tau_coasted = Calculate_Conf_Tau_For_Coasted_Objects(object, calib);
         alpha = F360_Expf(-elapsed_time / tau_coasted);
      }

      return alpha;
   }

   /*===========================================================================*\
   * FUNCTION: Determine_Raw_Confidence_Level
   *===========================================================================
   * RETURN VALUE:
   * float32_t raw_confidence_level - object raw confidence level
   *
   * PARAMETERS:
   * const F360_Object_Track_T& object - analysed object
   * const F360_Calibrations_T& calib - tracker calibrations
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
   * Function calculates raw confidence level for updated and coasted tracks.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Determine_Raw_Confidence_Level(
      const F360_Object_Track_T& object,
      const F360_Calibrations_T& calib)
   {
      float32_t raw_confidence_level;
      if (F360_OBJECT_STATUS_COASTED == object.status)
      {
         raw_confidence_level = Determine_Raw_Confidence_Level_Of_Coasted_Object(object, calib.k_hyst_time_for_coasted_objects);
      }
      else
      {
         raw_confidence_level = Determine_Raw_Confidence_Level_Of_Updated_Object(object.num_rr_inlier_dets, 
                                                                                 object.ndets, 
                                                                                 calib.k_conf_raw_weight_dets,
                                                                                 calib.k_conf_raw_max_value_not_reduced_dets);
      }
      return raw_confidence_level;
   }
}

