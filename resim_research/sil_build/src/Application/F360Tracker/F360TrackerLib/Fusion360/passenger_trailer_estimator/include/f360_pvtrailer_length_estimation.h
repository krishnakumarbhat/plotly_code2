#ifndef F360_PVTRAILER_LENGTH_ESTIMATION_H
#define F360_PVTRAILER_LENGTH_ESTIMATION_H
/*===========================================================================*\
 * FILE: f360_pvtrailer_length_estimation.h
 *============================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "f360_host.h"
#include "rspp_detection_list.h"
#include "f360_detection_props.h"
#include "f360_radar_sensor.h"
#include "f360_pvtrailer_data.h"

namespace f360_variant_A
{
   static const int16_t PEAK_GROUP_SIZE = 20;

   struct TL_Peak
   {
      int32_t peak_pos;
      int32_t peak_val;
      int32_t peak_left_radius;
      int32_t peak_right_radius;
   };

   typedef struct TL_Area_Tag
   {
      int32_t starting_pos;
      int32_t ending_pos;
      int32_t ref_val;
      float32_t mean_val;
      float32_t max_val;
   } TL_Area_T;

   void PVTrailer_Estimate_Length(
      const F360_Host_T& vehicle_data,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Detection_Props_T(&all_detections)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_PVTrailer_Length_Data_T& pvtrailer_length);

   void Estimate_Trailer_Length_Peaks(
      F360_PVTrailer_Length_Data_T& pvtrailer_length,
      const TL_Peak(&first_peak_group)[PEAK_GROUP_SIZE],
      const int32_t& first_peak_cnt,
      const bool& first_peak_extension,
      const TL_Peak(&second_peak_group)[PEAK_GROUP_SIZE],
      const int32_t& second_peak_cnt,
      const TL_Peak(&third_peak_group)[PEAK_GROUP_SIZE],
      const int32_t& third_peak_cnt);

   void Adjust_Sample(F360_PVTrailer_Length_Data_T& pvtrailer_length);

   void Locate_Peaks(
      const F360_PVTrailer_Length_Data_T& pvtrailer_length,
      TL_Peak(&first_peak_group)[PEAK_GROUP_SIZE],
      int32_t& first_peak_cnt,
      bool& first_peak_extension,
      TL_Peak(&second_peak_group)[PEAK_GROUP_SIZE],
      int32_t& second_peak_cnt,
      TL_Peak(&third_peak_group)[PEAK_GROUP_SIZE],
      int32_t& third_peak_cnt);

   void Find_Peak_Group_Info(
      const F360_PVTrailer_Length_Data_T& pvtrailer_length,
      const int32_t(&temp_detection_row)[DETECTION_ROWS],
      const float32_t threshold_forward,
      const float32_t threshold_backward,
      const uint32_t peak_gap_max,
      const uint32_t peak_r_gap_max,
      const int32_t max_val,
      TL_Peak(&peak_list_point)[PEAK_GROUP_SIZE],
      int32_t& peak_cnt);

   bool Is_Peak(const int32_t(&sample)[DETECTION_ROWS], const uint32_t index);
   void Cal_Radius(const int32_t(&sample)[DETECTION_ROWS], const uint32_t index, uint32_t& r_left, uint32_t& r_right);
   int32_t Peak_Left_Edge(const TL_Peak(&peak_group)[PEAK_GROUP_SIZE], const int32_t pos);
   int32_t Peak_Right_Edge(const TL_Peak(&peak_group)[PEAK_GROUP_SIZE], const int32_t pos);
   void Get_Area_Info(const F360_PVTrailer_Length_Data_T& pvtrailer_length, TL_Area_T& area_info);

   void Shrink_Trailer_Length(
      const bool f_noise,
      const int32_t i_array_max,
      const int32_t j_array_max,
      float32_t& last_peak,
      bool& f_shrink,
      const F360_PVTrailer_Length_Data_T& pvtrailer_length,
      const TL_Peak(&first_peak_group)[PEAK_GROUP_SIZE],
      const int32_t& first_peak_cnt,
      const TL_Peak(&second_peak_group)[PEAK_GROUP_SIZE],
      const int32_t& second_peak_cnt,
      const TL_Peak(&third_peak_group)[PEAK_GROUP_SIZE],
      const int32_t& third_peak_cnt,
      const TL_Area_T& front_area,
      const TL_Area_T& middle_area);

   void Extend_Trailer_Length(
      const bool f_noise,
      const int32_t i_array_max,
      const int32_t j_array_max,
      const int32_t k_array_max,
      float32_t& last_peak,
      const F360_PVTrailer_Length_Data_T& pvtrailer_length,
      const TL_Peak(&first_peak_group)[PEAK_GROUP_SIZE],
      const int32_t& first_peak_cnt,
      const TL_Peak(&second_peak_group)[PEAK_GROUP_SIZE],
      const int32_t& second_peak_cnt,
      const TL_Peak(&third_peak_group)[PEAK_GROUP_SIZE],
      const int32_t& third_peak_cnt,
      const TL_Area_T& front_area);

   void Estimate_Trailer_Length_SVM(F360_PVTrailer_Length_Data_T& pvtrailer_length);
   void Norm_Detection_Row(const int32_t(&detection_row)[DETECTION_ROWS], float32_t(&norm_sample)[DETECTION_ROWS]);
   void SVM_Classification(const float32_t(&norm_sample)[DETECTION_ROWS], F360_PVTrailer_Length_Data_T& pvtrailer_length);
   void Post_Processing(F360_PVTrailer_Length_Data_T& pvtrailer_length, const TL_Peak(&first_peak_group)[PEAK_GROUP_SIZE]);
}

#endif
