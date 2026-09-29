#include "olp.h"
#include "olp_calibration.h"

extern Olp_Calibration_T Olp_Cals;
extern void Olp_Update_Cal_Default_Value(void);

namespace olp
{

   void ObjectListPostprocessor::init()
   {
      (void)Olp_Update_Cal_Default_Value();
   }

   void ObjectListPostprocessor::fill_curvi_data_info(Olp_InOut_Object_Data_T &olp_output, const Curvi_Data_T obj_curvi_out, const Vehicle_Info_T &vehicle_info)
   {
      olp_output.curvi_pos.x = obj_curvi_out.curvi_pos.x;
      olp_output.curvi_pos.y = obj_curvi_out.curvi_pos.y;
      olp_output.curvi_vel.x = obj_curvi_out.curvi_vel.x;
      olp_output.curvi_vel.y = obj_curvi_out.curvi_vel.y;
      olp_output.curvi_vel_rel.x = obj_curvi_out.curvi_vel.x - vehicle_info.vcs_long_velocity;
      olp_output.curvi_vel_rel.y = obj_curvi_out.curvi_vel.y - vehicle_info.vcs_lat_velocity;
      olp_output.curvi_heading = obj_curvi_out.curvi_heading;
      olp_output.curvi_coordinates_calc_method = obj_curvi_out.curvi_coordinates_calc_method;
   }

   void ObjectListPostprocessor::process(Olp_InOut_Object_Data_T &olp_output, const Vehicle_Info_T &vehicle_info, const Olp_Extended_Object_Data_T &object_extnd_data)
   {
      static Snail_Trail_T snail_data;
      static Curvature_Info_T curvature_input;
      Curvi_Data_T obj_curvi;
      snail_trail.snail_trail_time_update(snail_data, vehicle_info, Olp_Cals);
      distance_based_curvature::distance_based_curvature_computation(vehicle_info, curvature_input, Olp_Cals);
      obj_curvi = comp_curvi.Compute_Obj_Curvi_Data(olp_output,  snail_data, vehicle_info, curvature_input, Olp_Cals, object_extnd_data);
      fill_curvi_data_info(olp_output, obj_curvi, vehicle_info);
   }
   
   void olp_core::OLP_Init_Hook(void)
   {
      
   }

   void olp_core::OLP_Main_Hook(Olp_Data_T &olp_data_ref)
   {
      if(OLP_NUMBER_OF_OBJECTS < olp_data_ref.no_of_valid_objects)
      {
         return;
      }
      for (unsigned int index = 0u; index < olp_data_ref.no_of_valid_objects; index++)
      {
         if (0u != olp_data_ref.olp_inout_obj_data[index].id )
         {
            #ifdef OLP_OBJ_SOURCE_F360
            Cog_Dynamics_T cog_out = { 0 };
            Cog_Input_T cog_input = { 0 };
            comp_cog.fill_cog_input(cog_input, olp_data_ref.olp_inout_obj_data[index], olp_data_ref.olp_extnd_obj_data[index]);
            comp_cog.Compute_Obj_COG(cog_out, cog_input);
            comp_cog.fill_cog_output(olp_data_ref.olp_inout_obj_data[index], cog_out, olp_data_ref.veh_info_data);
            #endif
            
            Olp_ObjListPostProc.process(olp_data_ref.olp_inout_obj_data[index], olp_data_ref.veh_info_data, olp_data_ref.olp_extnd_obj_data[index]);
         }
      }
   }
}
