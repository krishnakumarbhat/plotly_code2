#ifndef OLP_MAIN_H
#define OLP_MAIN_H

#include "olp_iface.h"
#include "olp_core_types.h"
#include "olp_compute_cog.h"
#include "olp_snail_trail_computation.h"
#include "olp_compute_curvi_data.h"
#include "olp_distance_based_curvature.h"

namespace olp
{
   class ObjectListPostprocessor
   {
   public:
      void init(void);
      void process(Olp_InOut_Object_Data_T &olp_output, const Vehicle_Info_T &vehicle_info, const Olp_Extended_Object_Data_T &object_extnd_data);
      void fill_curvi_data_info(Olp_InOut_Object_Data_T &olp_output, const Curvi_Data_T obj_curvi_out, const Vehicle_Info_T &vehicle_info);

   private:
      snail_trail_calculation snail_trail;
      compute_curvi_data comp_curvi;
   };

   class olp_core
   {

   public:
      #ifdef OLP_OBJ_SOURCE_F360
      compute_cog comp_cog;
      #endif
      ObjectListPostprocessor Olp_ObjListPostProc;
      
      void OLP_Init_Hook(void);
      void OLP_Main_Hook(Olp_Data_T &olp_data_ref);
   };

}

#endif /* OLP_MAIN_H */
