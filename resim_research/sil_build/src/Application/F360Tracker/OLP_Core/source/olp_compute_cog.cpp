#include "olp_compute_cog.h"
#include "olp_ml_math.h"

namespace olp
{
#ifdef OLP_OBJ_SOURCE_F360
   compute_cog::compute_cog() {}

   void compute_cog::Init_Obj_Cog(Cog_Dynamics_T &cog)
   {
      cog.cog_long_pos_vcs = 0.0F;
      cog.cog_lat_pos_vcs = 0.0F;
      cog.cog_long_vel_vcs = 0.0F;
      cog.cog_lat_vel_vcs = 0.0F;
      cog.cog_speed = 0.0F;
      cog.cog_heading = 0.0F;
      cog.cog_long_acc_vcs = 0.0F;
      cog.cog_lat_acc_vcs = 0.0F;
   }

   void compute_cog::fill_cog_input(Cog_Input_T &cog_input, const Olp_InOut_Object_Data_T &object_data, const Olp_Extended_Object_Data_T &object_extnd_data)
   {
      cog_input.speed = object_data.speed;
      cog_input.vcs_xposn = object_data.vcs_pos.x;
      cog_input.vcs_yposn = object_data.vcs_pos.y;
      cog_input.vcs_xvel = object_data.vcs_vel.x;
      cog_input.vcs_yvel = object_data.vcs_vel.y;
      cog_input.vcs_xaccel = object_data.vcs_accel.x;
      cog_input.vcs_yaccel = object_data.vcs_accel.y;

      cog_input.curvature = object_extnd_data.curvature;
      cog_input.tang_accel = object_extnd_data.tang_accel;
      cog_input.vcs_pointing = object_extnd_data.vcs_pointing;
      cog_input.len1 = object_extnd_data.len1;
      cog_input.len2 = object_extnd_data.len2;
      cog_input.wid1 = object_extnd_data.wid1;
      cog_input.wid2 = object_extnd_data.wid2;
   }

   void compute_cog::fill_cog_output(Olp_InOut_Object_Data_T &Olp_Object_Data, const Cog_Dynamics_T &cog_out, const Vehicle_Info_T &vehicle_info_input)
   {
      Olp_Object_Data.speed = cog_out.cog_speed;
      Olp_Object_Data.vcs_pos = { cog_out.cog_long_pos_vcs, cog_out.cog_lat_pos_vcs };
      Olp_Object_Data.vcs_vel = { cog_out.cog_long_vel_vcs, cog_out.cog_lat_vel_vcs };
      Olp_Object_Data.vcs_vel_rel = { cog_out.cog_long_vel_vcs - vehicle_info_input.vcs_long_velocity, cog_out.cog_lat_vel_vcs - vehicle_info_input.vcs_lat_velocity };
      Olp_Object_Data.vcs_accel = { cog_out.cog_long_acc_vcs, cog_out.cog_lat_acc_vcs };
      Olp_Object_Data.vcs_heading = cog_out.cog_heading;
      
   }

   void compute_cog::Rotate_2D_Vector(const float prev_x, const float prev_y, const float cos_angle, const float sin_angle, float &next_x, float &next_y)
   {
      next_x = prev_x * cos_angle - prev_y * sin_angle;
      next_y = prev_x * sin_angle + prev_y * cos_angle;
   }

   void compute_cog::Compute_Obj_COG(Cog_Dynamics_T &cog_out, const Cog_Input_T &obj_in)
   {
      // Compute center position of object
      const float delta_x_tcs = 0.5F * (obj_in.len2 - obj_in.len1);
      const float delta_y_tcs = 0.5F * (obj_in.wid2 - obj_in.wid1);
      float delta_long_pos_vcs;
      float delta_lat_pos_vcs;

      Init_Obj_Cog(cog_out);

      Rotate_2D_Vector(delta_x_tcs, delta_y_tcs, olp_ml_wrapper::Olp_Fast_Cos(obj_in.vcs_pointing), olp_ml_wrapper::Olp_Fast_Sin(obj_in.vcs_pointing), // If we rebase on a later dev then obj.vcs_cos_pointing and obj.vcs_sin_pointing exist and we do not have to re-compute it here
      delta_long_pos_vcs, delta_lat_pos_vcs);

      cog_out.cog_long_pos_vcs = obj_in.vcs_xposn + delta_long_pos_vcs; // I think that if you rebase on a later dev the obj.vcs_xposn might have a different name
      cog_out.cog_lat_pos_vcs = obj_in.vcs_yposn + delta_lat_pos_vcs;  // I think that if you rebase on a later dev the obj.vcs_yposn might have a different name

      // Precompute som information that is needed to compute velocity and acceleration in object center
      float obj_yaw_rate;
      float obj_yaw_acc; // Derivative of object yaw rate

      obj_yaw_rate = obj_in.curvature * obj_in.speed;
      obj_yaw_acc = obj_in.tang_accel * obj_in.curvature; // yaw_rate = speed * curv => derivative of yaw rate w.r.t. time is yaw_acc = tang_acc * curvature under the assumption that curvature derivative is 0. This is the assumption which is used by KF CTCA filter so we use it here as well


      // Compute velocity of center of object (using rigid body mechanics: v_B = v_A + w x dr)
      cog_out.cog_long_vel_vcs = obj_in.vcs_xvel - obj_yaw_rate * delta_lat_pos_vcs;
      cog_out.cog_lat_vel_vcs = obj_in.vcs_yvel + obj_yaw_rate * delta_long_pos_vcs;

      // Compute speed and heading of object_center
      cog_out.cog_speed = olp_ml_wrapper::Olp_Fast_Sqrt(cog_out.cog_long_vel_vcs * cog_out.cog_long_vel_vcs + cog_out.cog_lat_vel_vcs * cog_out.cog_lat_vel_vcs);
      cog_out.cog_heading = olp_ml_wrapper::Olp_Fast_Atan2(cog_out.cog_lat_vel_vcs, cog_out.cog_long_vel_vcs); // TODO: Do we need to handle special case when both cog_lat_vel_vcs and cog_long_vel_vcs is zero/close to zero?

      // Compute acceleration of center of object (using rigid body mechanics: a_B = a_A + w_dot x dr + w x w x dr)
      cog_out.cog_long_acc_vcs = obj_in.vcs_xaccel - obj_yaw_acc * delta_lat_pos_vcs - obj_yaw_rate * obj_yaw_rate * delta_long_pos_vcs;
      cog_out.cog_lat_acc_vcs = obj_in.vcs_yaccel + obj_yaw_acc * delta_long_pos_vcs - obj_yaw_rate * obj_yaw_rate * delta_lat_pos_vcs;
   }
#endif
}
