#ifndef COMPUTE_CURVI_DATA_HPP
#define COMPUTE_CURVI_DATA_HPP

#include "olp_core_types.h"
#include "olp_distance_based_curvature.h"

namespace olp
{
   class compute_curvi_data
   {
   public:
      Curvi_Data_T Compute_Obj_Curvi_Data(const Olp_InOut_Object_Data_T &obj_in, const Snail_Trail_T &snail_in, const Vehicle_Info_T &vehicle_output, const Curvature_Info_T &curvature_input, const Olp_Calibration_T &pcals, const Olp_Extended_Object_Data_T &object_extnd_data);

   private:
      void Initialize_Curvi_Data(Curvi_Data_T &crv_dat);
      void Initialize_Object_State(Object_Vcs_T &obj);
      Object_Vcs_T Compute_Obj_Vcs(const Olp_InOut_Object_Data_T obj_in, const Olp_Extended_Object_Data_T &object_extnd_data);
      void convertVCSToCurvi_objectState(Curvi_Data_T &obj_curvi, const Object_Vcs_T &obj_vcs_in, const Vehicle_Info_T &vehicle_output, const Snail_Trail_T &p_snail_trail, const Curvature_Info_T &curvature_input, const Olp_Calibration_T &pcals);
      Snail_Trail_Match_T getSnailTrailMatch(const Snail_Trail_T &snail_in, const Vehicle_Info_T &vehicle_output, const Olp_Vector_2d_T &obj_center_pos, const Olp_Calibration_T &pcals);
      Olp_Vector_2d_T Transform_Point_On_Snail_Trail(const Snail_Trail_Match_T &snail_match_in, const Olp_Vector_2d_T &obj_center_pos, const Vehicle_Info_T &vehicle_otuput, const Snail_Trail_T &p_snail_trail);
      float getSnailTrailHeading(const Snail_Trail_Match_T &snail_trail_match);
      void Calculate_Relative_Velocity(Curvi_Data_T &obj_curvi, const Vehicle_Info_T &vehicle_output);
      Snail_Trail_Match_T initSnailTrailMatch(const Snail_Trail_T &p_snail_trail);
      Snail_Trail_Match_T Get_Snail_Trail_Index(const Snail_Trail_T &p_snail_trail, const float &host_rear_axle_position, const Olp_Vector_2d_T &p_vcs_point, const Olp_Calibration_T &pcals);
      void calcSnailTrailMatchFactors(Snail_Trail_Match_T &snail_trail_match, const Snail_Trail_T &p_snail_trail, const Olp_Calibration_T &pcals);
      Olp_Vector_2d_T Transform_Point_Vcs_to_Wcs(const Olp_Vector_2d_T &p_vcs_point, const Olp_Vector_2d_T &host_real_axle_pos_vcs, const Olp_Angle_T &snail_heading_angle, const Olp_Vector_2d_T &snail_host_pos);
      Olp_Angle_T getSnailTrailHeading_angle(const Snail_Trail_Match_T &snail_trail_match);
      Olp_Vector_2d_T getMatchedSnailTrailPoint(const Snail_Trail_Match_T &snail_trail_match);
      float getSnailTrailDistTraveled(const Snail_Trail_Match_T &snail_trail_match);
      float snailTrailInterpolate(const float first_val, const float second_val, const Snail_Trail_Match_T &snail_trail_match);

   };
}

#endif