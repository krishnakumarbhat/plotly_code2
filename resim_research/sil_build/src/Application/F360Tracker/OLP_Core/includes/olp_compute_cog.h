#ifndef COMPUTE_COG_H
#define COMPUTE_COG_H

#include "olp_core_types.h"
#include "olp_iface.h"

namespace olp
{
#ifdef OLP_OBJ_SOURCE_F360
   class compute_cog
   {
   public:
      compute_cog();
      void Compute_Obj_COG(Cog_Dynamics_T &cog_out, const Cog_Input_T &obj_in);
      void fill_cog_input(Cog_Input_T &cog_input, const Olp_InOut_Object_Data_T &object_data, const Olp_Extended_Object_Data_T &object_extnd_data);
      void fill_cog_output(Olp_InOut_Object_Data_T &Olp_Object_Data, const Cog_Dynamics_T &cog_out, const Vehicle_Info_T &vehicle_info_input);

   private:
      void Init_Obj_Cog(Cog_Dynamics_T &cog);
      void Rotate_2D_Vector(const float prev_x, const float prev_y, const float cos_angle, const float sin_angle, float &next_x, float &next_y);
   };
#endif
}

#endif
