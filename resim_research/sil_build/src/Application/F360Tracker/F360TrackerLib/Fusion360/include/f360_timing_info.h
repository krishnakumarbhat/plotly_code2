/*===================================================================================*\
* FILE: f360_timing_info.h
*====================================================================================
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*-----------------------------------------------------------------------------------------
* %full_filespec: %
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains F360_TRKR_TIMING_INFO_T  structure declaration
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): defineFusion360Types.m
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_TIMING_INFO_H
#define F360_TIMING_INFO_H

#include "f360_reuse.h"
namespace f360_variant_A
{
   struct F360_TRKR_TIMING_INFO_T
   {
      // modules
      float32_t core_tracker;
      float32_t inputs_preprocessing;
      float32_t internal_preprocessing;
      float32_t time_update_tracks;
      float32_t trailer_detector;
      float32_t cv_trailer_estimator;
      float32_t pre_association_track_management;
      float32_t detection_to_track_association;
      float32_t clustering;
      float32_t cluster_grouping;
      float32_t track_grouping;
      float32_t measurement_update_tracks;
      float32_t initialize_tracks;
      float32_t post_update_track_adjustments;
      float32_t track_classification;
      float32_t track_validity;
      float32_t track_downselection;
      float32_t e2e_protection;
      float32_t lsc_module;
      float32_t concrete_wall_detector;
      float32_t sensor_postprocessing;

      // sensor preprocessing
      float32_t host_state_update;
      float32_t calc_obj_mov_stat_thresh;
      float32_t update_sensor_valid_info;
      float32_t configure_rdot_interval_compability;
      float32_t sensor_motion;
      float32_t update_det_hist;
      float32_t ego_motion_compensate_dets;
      float32_t detect_wheelspin;
      float32_t check_az_el_conf;
      float32_t mark_out_det_pairs;
      float32_t sensor_capability_module;
      float32_t double_bounce_detection_countermeasure;

      // time update tracks
      float32_t time_update_obj_trks_cca;
      float32_t time_update_obj_trks_ctca;

      // pre association track management
      float32_t split_tracks;
      float32_t kill_coasted_tracks;

      // association
      float32_t assoc_dets_with_obj_trks;
      float32_t assoc_countermeasure;
      float32_t store_det_data;
      float32_t det_downselect;
      float32_t cluster_moving_detections;
      float32_t cluster_leftover_detections;
      float32_t m_initialize_clusters;
      float32_t assoc_unconf_obj_trks;

      // existence probability 
      float32_t predict_existence_probability;
      float32_t update_existence_probability;

      // pseudo estimations
      float32_t pseudo_estimations;

      // track properties
      float32_t obj_trk_status_bookkeeping;
      float32_t adjust_fltr_type_dependent_params;
      float32_t obj_trk_properties;
      float32_t cancel_new_updated_trk_overlapping_confirmed_trks;
      float32_t adjust_overlapping_confirmed_trks;
      float32_t det_mean_var;
      float32_t object_underdrivability_classification;
      float32_t assign_underdrivability_status_to_tracks_ocg;
      float32_t determine_underdrivability_for_movable;

      // downselect


      // static environment
      float32_t mark_object_tracks_next_to_sensors;

      // common
      float32_t sanity_check;

      // occlusion
      float32_t occlusion;

      // detailed timers
      float32_t msmt_update_obj_trks_cca_moveable;
      float32_t msmt_update_obj_trks_cca_non_moveable;
      float32_t msmt_update_obj_trks_cca;
      float32_t msmt_update_obj_trks_ctca;

      float32_t assoc_unconf_obj_trks_sub_time_1;
      float32_t assoc_unconf_obj_trks_sub_time_2;
      float32_t assoc_unconf_obj_trks_sub_time_3;
      float32_t assoc_unconf_obj_trks_sub_time_4;
      float32_t assoc_unconf_obj_trks_sub_time_5;
      float32_t assoc_unconf_obj_trks_sub_time_6;

   };

   static_assert(280 == sizeof(F360_TRKR_TIMING_INFO_T), "sizeof(F360_TRKR_TIMING_INFO_T) not as expected. Remember to align padding if needed");
}
#endif
