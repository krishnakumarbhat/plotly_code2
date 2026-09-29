#ifndef SNAIL_TRAIL_COMPUTATION_HPP
#define SNAIL_TRAIL_COMPUTATION_HPP

#include "olp_core_types.h"

namespace olp
{
   class snail_trail_calculation
   {
   public:
      void Init_snail_trail(Snail_Trail_T &p_snail_trail);
      void snail_trail_time_update(Snail_Trail_T &p_snail_trail, const Vehicle_Info_T &vehicle_ouput, const Olp_Calibration_T &pcals);

   private:
      Olp_Vector_2d_T Get_World_Time_Update(const Vehicle_Info_T &vehicle_ouput, const float &yaw_per_cycle);
   };
}

#endif