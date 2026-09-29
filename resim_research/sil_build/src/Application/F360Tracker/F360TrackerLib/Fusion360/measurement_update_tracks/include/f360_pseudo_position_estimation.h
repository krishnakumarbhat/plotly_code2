/*===========================================================================*\
* FILE: f360_pseudo_position_estimation.h
*============================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Pseudo_Position_Estimation().
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/
#ifndef F360_PSEUDO_POSITION_ESTIMATION_H
#define F360_PSEUDO_POSITION_ESTIMATION_H

#include "f360_calibrations.h"
#include "f360_object_track.h"
#include "f360_object_sides.h"
#include "f360_radar_sensor.h"
#include "f360_globals.h"
#include "f360_host.h"
#include "f360_detection_props.h"
#include "f360_point.h"
#include "rspp_detection_list.h"
#include "f360_reuse.h"
#include "f360_bounding_box.h"
#include "f360_angle.h"

namespace f360_variant_A
{
   void Pseudo_Position_Estimation(
      const F360_Calibrations_T& calibrations,
      const F360_Host_T& host,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals,
      F360_Object_Track_T& obj);

   float32_t Calculate_Pseudo_Pos_Adjustment_For_Protruding_Corner(
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Host_T& host,
      const F360_Object_Track_T& obj);

   bool Get_Most_Critical_Front_Corner_Ypos(
      const F360_Object_Track_T& obj,
      const F360_Host_T& host,
      float32_t& critical_corner_y_pos);

   bool Is_Object_Not_In_Direct_Host_Path(
      const F360_Object_Track_T& obj,
      const float32_t obj_curvi_lat_pos);

   float32_t Calculate_Distance_Std_Dev_Of_Associated_Dets(
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Object_Track_T& obj);

   void Adjust_Pseudo_Cov_TCS(
      const F360_Calibrations_T& calibrations,
      const Point& pseudo_pos_vcs,
      F360_Object_Track_T& obj,
      float32_t(&pseudo_cov_tcs)[2][2]);

   int32_t Count_Pseudo_Pos_Outliers(
      const int32_t k_max_num_consistent_outliers,
      const float32_t k_pseudo_pos_dist_diff_thr,
      const float32_t diff_pos,
      int32_t pseudo_pos_cov_outlier_count);

   float32_t Compute_Pos_Cov_Inc_In_TCS(
      const float32_t k_pseudo_pos_dist_diff_thr,
      const float32_t k_pseudo_pos_dist_diff_gain,
      float32_t diff_pos);

   void Compute_Raw_Pseudo_Pos_Cov_In_TCS(
      const F360_Object_Track_T& obj,
      const F360_Host_T& host,
      float32_t(&pseudo_pos_cov_tcs)[2][2],
      const F360_Calibrations_T& calibrations);

   float32_t Calc_Weighted_Average_Pseudo_Pos(
      const uint32_t ndets,
      const float32_t(&assoc_dets_pos_average_dimension)[MAX_DETS_IN_OBJ_TRK],
      const float32_t(&assoc_dets_pos_reference_dimension)[MAX_DETS_IN_OBJ_TRK],
      const float32_t ref_pos,
      const float32_t k_huber_threshold);

   void Adjust_Pseudo_Cov_TCS_Wrt_Visibility(
      const F360_Object_Track_T& obj,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T& calibrations,
      const F360_Globals_T& globals,
      float32_t(&pseudo_cov_tcs)[2][2]);

   Point Compute_Pseudo_Pos_TCS_For_Extended_Object_Case_Min_Max_Weighted_Average(
      const F360_Object_Sides_T rear_front_side,
      const F360_Object_Sides_T right_left_side,
      const float32_t(&assoc_dets_tcs_x_pos)[MAX_DETS_IN_OBJ_TRK],
      const float32_t(&assoc_dets_tcs_y_pos)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t ndets);

   Point Compute_Pseudo_Pos_TCS_For_Extended_Object_Case_Grid_Search(
      const F360_Object_Track_T& obj,
      const F360_Object_Sides_T rear_front_side,
      const F360_Object_Sides_T right_left_side,
      const float32_t(&assoc_dets_tcs_x_pos)[MAX_DETS_IN_OBJ_TRK],
      const float32_t(&assoc_dets_tcs_y_pos)[MAX_DETS_IN_OBJ_TRK],
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]);

   Point Compute_Scores_And_Best_Pseudo_Pos_For_Given_Pointing(
      const uint32_t obj_ndets,
      const float32_t obj_length,
      const float32_t obj_width,
      const F360_Object_Sides_T rear_front_side,
      const F360_Object_Sides_T right_left_side,
      const float32_t(&assoc_dets_para_pos_tcs)[MAX_DETS_IN_OBJ_TRK],
      const float32_t(&assoc_dets_orth_pos_tcs)[MAX_DETS_IN_OBJ_TRK],
      float32_t& best_para_score,
      float32_t& best_orth_score);

   void Create_Position_Grid(
      const float32_t(&dets_pos)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t ndets,
      const F360_Object_Sides_T visible_side,
      const float32_t obj_dimension,
      float32_t(&pos_grid)[MAX_DETS_IN_OBJ_TRK],
      uint32_t& num_pos_points);

   float32_t Iterate_Over_Grid(
      const float32_t(&pos_grid)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t num_grid_points,
      const float32_t(&assoc_dets_pos_tcs)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t ndets,
      const F360_Object_Sides_T obj_visible_side,
      const float32_t obj_dimension,
      float32_t& best_score);

   float32_t Compute_Detection_Score(
      const float32_t det_mod_tcs_pos,
      const F360_Object_Sides_T visible_side,
      const float32_t obj_dimension);

   float32_t Compute_Bisquare_Score(
      const float32_t res,
      const float32_t k_bistatic);

   Point Compute_Pseudo_Pos_Mid_Point_Of_Detections(
      const F360_Object_Track_T& obj,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]);

   void Get_Vectors_And_Distances_To_Obj_Corners(
      const F360_Host_T& host,
      const F360_Object_Track_T& obj,
      float32_t(&vec_from_host_center_to_obj_corners)[4][2],
      float32_t(&dist_from_host_center_to_obj_corners)[4]);

   bool Is_Point_Object_Assumption_Valid(
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Object_Track_T& obj,
      const float32_t(&vec_from_host_center_to_obj_corners)[4][2],
      const float32_t(&dist_from_host_center_to_obj_corners)[4]);

   Point Compute_Pseudo_Pos_Object_Point_Assumption(
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Host_T& host,
      const F360_Object_Track_T& obj);

   Point Compute_Pseudo_Pos_Extended_Obj_Assumption(
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& obj);

   Point Transform_Pseudo_SCS_Pos_To_VCS(
      const Point& obj_center_pseudo_pos_in_pseudo_SCS,
      const Angle& angle_host_center_to_obj_center_vcs,
      const Point& host_center_vcs);

   float32_t Calculate_SCS_Min_Range(
      const F360_Object_Track_T& obj,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const Point& host_center_vcs);

   Angle Calculate_Mean_SCS_Azimuth(
      const F360_Object_Track_T& obj,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const Point& host_center_vcs,
      const Angle& angle_host_center_to_obj_center_vcs);

   void Compute_Combined_Weighted_Pseudo_Position(
      const F360_Object_Track_T& obj,
      const Point& pseudo_pos_tcs_min_max_avg,
      const Point& pseudo_pos_tcs_grid_search,
      Point& pseudo_pos_tcs);

   void Calculate_Pseudo_Pos_Weights(
      const Point& pseudo_pos_tcs,
      const Point& ref_pnt_tcs,
      float32_t(&weights)[2]);

   float32_t Transform_Object_Center_TCS_To_Reference_Point_TCS_In_Single_Dimension(
      const F360_Object_Sides_T& visible_side_in_dimension,
      const float32_t& obj_size_in_dimension,
      const float32_t& center_point_tcs_dimension);

   Point Get_Final_TCS_Pseudo_Pos(
      const bool f_valid_pseudo_pnt,
      const Point& obj_average_grid_search_tcs_position,
      const Point& unfiltered_pseudo_pos_tcs);

}
#endif
