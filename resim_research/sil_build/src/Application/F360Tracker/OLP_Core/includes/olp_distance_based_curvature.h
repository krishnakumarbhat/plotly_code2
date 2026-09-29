#ifndef DISTANCE_BASED_CURVATURE_HPP
#define DISTANCE_BASED_CURVATURE_HPP

#include "olp_core_types.h"

namespace olp
{
   class distance_based_curvature
   {
   public:
      static void distance_based_curvature_computation(const Vehicle_Info_T &vehicle_ouput, Curvature_Info_T &curvature_input, const Olp_Calibration_T &pcals);
      static Olp_Vector_2d_T Transform_Point_With_Distance_Based_Curvature(const Vehicle_Info_T &vehicle_ouput, const Olp_Vector_2d_T &obj_center_pos, const Curvature_Info_T &curvature_input);
   };
}

#endif