#include "olp_compute_curvi_data.h"
#include "olp_ml_math.h"

namespace olp
{

   void compute_curvi_data::Initialize_Object_State(Object_Vcs_T &obj)
   {
      const Olp_Vector_2d_T zero_vect = { 0.0F, 0.0F };

      obj.speed             = 0.0F;
      obj.heading           = olp_ml_wrapper::Olp_Create_Angle(0.0F);
      obj.center_position   = zero_vect;
      obj.velocity          = zero_vect;
      obj.relative_velocity = zero_vect;
      obj.f_updated         = false;
   }

   Object_Vcs_T compute_curvi_data::Compute_Obj_Vcs(const Olp_InOut_Object_Data_T obj_in, const Olp_Extended_Object_Data_T &object_extnd_data)
   {
      Object_Vcs_T obj_vcs;
      Initialize_Object_State(obj_vcs);
      obj_vcs.center_position.x = obj_in.vcs_pos.x;
      obj_vcs.center_position.y = obj_in.vcs_pos.y;
      obj_vcs.velocity.x = obj_in.vcs_vel.x;
      obj_vcs.velocity.y = obj_in.vcs_vel.y;
      obj_vcs.speed = obj_in.speed;
      obj_vcs.heading = olp_ml_wrapper::Olp_Create_Angle(obj_in.vcs_heading);
      obj_vcs.curvature = object_extnd_data.curvature;

      return obj_vcs;
   }

   void compute_curvi_data::Initialize_Curvi_Data(Curvi_Data_T &crv_dat)
   {
      crv_dat.curvi_coordinates_calc_method = OLP_OBJ_CURVI_COORDINATES_UNKNOWN;
      crv_dat.curvi_pos                     = { 0.0F, 0.0F };
      crv_dat.curvi_vel                     = { 0.0F, 0.0F };
      crv_dat.curvi_vel_rel                 = { 0.0F, 0.0F };
      crv_dat.curvi_heading                 = 0.0F;
   }

   Snail_Trail_Match_T compute_curvi_data::initSnailTrailMatch(const Snail_Trail_T &p_snail_trail)
   {
      Snail_Trail_Match_T snail_trail_match_return;
      snail_trail_match_return.f_best_match_found                     = OLP_FALSE;
      snail_trail_match_return.f_second_best_match_found              = OLP_FALSE;
      snail_trail_match_return.best_match_index                       = (short int)-1;
      snail_trail_match_return.second_best_match_index                = (short int)-1;
      snail_trail_match_return.best_match_interpolation_factor        = OLP_AS_TOOLBOX_INFINITY;
      snail_trail_match_return.second_best_match_interpolation_factor = OLP_AS_TOOLBOX_INFINITY;
      snail_trail_match_return.best_match_dist2                       = OLP_AS_TOOLBOX_INFINITY;
      snail_trail_match_return.second_best_match_dist2                = OLP_AS_TOOLBOX_INFINITY;

      snail_trail_match_return.p_snail_trail = &p_snail_trail;

      return snail_trail_match_return;
   }

   Olp_Vector_2d_T compute_curvi_data::Transform_Point_Vcs_to_Wcs(const Olp_Vector_2d_T &p_vcs_point, const Olp_Vector_2d_T &host_real_axle_pos_vcs, const Olp_Angle_T &snail_heading_angle, const Olp_Vector_2d_T &snail_host_pos)
   {
      Olp_Vector_2d_T tmp_result;
      Olp_Vector_2d_T point_wcs;

      /* point in VCS is expressed in VCS-orientated coordinate system located in the middle of rear axle */
      tmp_result = olp_ml_wrapper::Olp_Vector_2d_Alg_Diff(p_vcs_point, host_real_axle_pos_vcs);

      /* point is expressed in world-orientation coordinate system located in the middle
   * of rear axle by rotating it with host heading in world coordinates
   */
      tmp_result = olp_ml_wrapper::Olp_Vector_2d_Alg_Rotate(snail_heading_angle, tmp_result);

      /* point is expressed in world-orientation coordinate system. Therefore the interim result
   * has to be shifted by a vector reflecting the shift of host since sensor initializationIS_TRUE
   */
      point_wcs = olp_ml_wrapper::Olp_Vector_2d_Alg_Add(snail_host_pos, tmp_result);

      return point_wcs;
   }

   void compute_curvi_data::calcSnailTrailMatchFactors(Snail_Trail_Match_T &snail_trail_match, const Snail_Trail_T &p_snail_trail, const Olp_Calibration_T &cal)
   {
      if (OLP_IS_TRUE(snail_trail_match.f_second_best_match_found))
      {
         /* given point is between the two snail points match and second_best_match*/
         /* lets interpolate */
         unsigned char f_best_match_younger = p_snail_trail.snail_trail_states[snail_trail_match.best_match_index].distance_traveled > p_snail_trail.snail_trail_states[snail_trail_match.second_best_match_index].distance_traveled;

         float snail_point_dist;
         float best_dist = olp_ml_wrapper::Olp_Fast_Sqrt(snail_trail_match.best_match_dist2);
         float second_best_dist = olp_ml_wrapper::Olp_Fast_Sqrt(snail_trail_match.second_best_match_dist2);
         float gamma = 0.0f;
         float second_best_weight;

         if (OLP_IS_TRUE(f_best_match_younger))
         {
            snail_point_dist = p_snail_trail.snail_trail_states[snail_trail_match.best_match_index].dist_between_points;
         }
         else
         {
            snail_point_dist = p_snail_trail.snail_trail_states[snail_trail_match.second_best_match_index].dist_between_points;
         }

         if ((OLP_TRACKER_THRESHOLD_IS_ZERO < snail_point_dist) && 
            olp_ml_wrapper::Olp_Is_AcceptableTriangle(snail_point_dist, best_dist, second_best_dist, OLP_EPSILON))
         {
            if ((second_best_dist + best_dist) > snail_point_dist)
            {
               gamma = olp_ml_wrapper::Olp_Triangle_Gamma_From_Abc(snail_point_dist, best_dist, second_best_dist);
               second_best_weight = (best_dist * olp_ml_wrapper::Olp_Fast_Cos(gamma)) / snail_point_dist;
            }
            else
            {
               second_best_weight = best_dist / snail_point_dist;    /* Point is on line between snail points */
            }

            /* Gamma is the angle opposite to the second_best_dist. */
            /* This angle must be smaller than 90 deg for a snail point to be within the snail trail. */
            if ((gamma < (PI / 2.0f))  && (second_best_weight >= 0.0f && second_best_weight <= 1.0f))
            {
               snail_trail_match.best_match_interpolation_factor = 1.0f - second_best_weight;
               snail_trail_match.second_best_match_interpolation_factor = second_best_weight;
            }
            else
            {
               snail_trail_match.best_match_interpolation_factor = 1.0f;
               snail_trail_match.second_best_match_interpolation_factor = 0.0f;
               snail_trail_match.second_best_match_dist2 = OLP_AS_TOOLBOX_INFINITY;
               snail_trail_match.f_second_best_match_found = OLP_FALSE;
               snail_trail_match.second_best_match_index = 0;
            }

         }
         else
         {
            snail_trail_match = initSnailTrailMatch(p_snail_trail);
         }
      }
      else
      {
         /* given point close to snail point match*/
         snail_trail_match.second_best_match_index = 0;
         snail_trail_match.best_match_interpolation_factor = 1.0f;
         snail_trail_match.second_best_match_interpolation_factor = 0.0f;
      }

      /*Handle oldest (far away) snail trail point different.
   * check if the oldest snail trail point is chosen.
   * As we don't have snail trail information beyond this point choose a smaller gate to be sure that the object matches the snail trail.
   * If the object isn't inside the smaller gate do not correct with the snail trail.*/
      if ((snail_trail_match.best_match_index == p_snail_trail.oldest_snail_trail_index) &&
            (snail_trail_match.best_match_dist2 > cal.k_vp_snail_trail_max_dist2_oldest_snail_point))     /*for this far away snail trail point use smaller gate */
      {
         if ((snail_trail_match.best_match_index == 0) && (p_snail_trail.f_snail_full_buffer == 0) && (p_snail_trail.snail_index != 1))
         {

         }
         else
         {
            snail_trail_match = initSnailTrailMatch(p_snail_trail);
         }
      }


      
   }

   Snail_Trail_Match_T compute_curvi_data::Get_Snail_Trail_Index(const Snail_Trail_T &p_snail_trail, const float &host_rear_axle_position, const Olp_Vector_2d_T &p_vcs_point, const Olp_Calibration_T &cal)
   {
      Olp_Vector_2d_T host_rear_axle_pos_vcs;
      Olp_Vector_2d_T point_wcs;
      Olp_Vector_2d_T dist_point_to_snail_trail;
      Snail_Trail_Match_T snail_trail_match_return;

      snail_trail_match_return = initSnailTrailMatch(p_snail_trail);

      if (OLP_IS_TRUE(p_snail_trail.f_snail_full_buffer) || (p_snail_trail.snail_index > 0))
      {
         short int stop_index;
         short int search_index;
         float dist_squared;
         /* compute world coordinate of point */
         host_rear_axle_pos_vcs = olp_ml_wrapper::Olp_Create_2d_Vector_Coordinates(host_rear_axle_position, 0.0f);
         point_wcs = Transform_Point_Vcs_to_Wcs(p_vcs_point, host_rear_axle_pos_vcs, p_snail_trail.snail_host_heading, p_snail_trail.snail_host_position);

         if (OLP_IS_TRUE(p_snail_trail.f_snail_full_buffer))
         {
            stop_index = NUMBER_OF_SNAIL_POINTS;
         }
         else
         {
            stop_index = p_snail_trail.snail_index;
         }
         for(search_index = 0; search_index < stop_index; search_index++)
         {
            /* compute distance */
            dist_point_to_snail_trail = olp_ml_wrapper::Olp_Vector_2d_Alg_Diff(point_wcs, p_snail_trail.snail_trail_states[search_index].point);
            dist_squared = olp_ml_wrapper::Olp_Vector_2d_Alg_Abs_squared(dist_point_to_snail_trail);
            /* discard all points outside the calibrated distance or more far away than the current best match */
            if ((OLP_EPSILON < (cal.k_vp_snail_trail_max_dist2 - dist_squared)) && (OLP_EPSILON < (snail_trail_match_return.best_match_dist2 - dist_squared)))
            {
               snail_trail_match_return.best_match_dist2 = dist_squared;
               snail_trail_match_return.best_match_index = search_index;
               snail_trail_match_return.f_best_match_found = OLP_TRUE;
               }
         }
         
         if (OLP_IS_TRUE(snail_trail_match_return.f_best_match_found))
         {
            /* check if previous snail point is second best match */
            if (snail_trail_match_return.best_match_index != p_snail_trail.oldest_snail_trail_index)
            {
               search_index = ((snail_trail_match_return.best_match_index + NUMBER_OF_SNAIL_POINTS) - 1) % NUMBER_OF_SNAIL_POINTS;
               dist_point_to_snail_trail = olp_ml_wrapper::Olp_Vector_2d_Alg_Diff(point_wcs, p_snail_trail.snail_trail_states[search_index].point);
               dist_squared = olp_ml_wrapper::Olp_Vector_2d_Alg_Abs_squared(dist_point_to_snail_trail);
               snail_trail_match_return.second_best_match_dist2   = dist_squared;
               snail_trail_match_return.second_best_match_index   = search_index;
               snail_trail_match_return.f_second_best_match_found = TRUE;
            }
            
            /* check if next snail point is second best match */
            search_index = (snail_trail_match_return.best_match_index + 1) % NUMBER_OF_SNAIL_POINTS;
         
            if (search_index != p_snail_trail.snail_index)
            {
               dist_point_to_snail_trail = olp_ml_wrapper::Olp_Vector_2d_Alg_Diff(point_wcs, p_snail_trail.snail_trail_states[search_index].point);
               dist_squared = olp_ml_wrapper::Olp_Vector_2d_Alg_Abs_squared(dist_point_to_snail_trail);
               if (dist_squared < snail_trail_match_return.second_best_match_dist2)
               {
                  snail_trail_match_return.second_best_match_dist2   = dist_squared;
                  snail_trail_match_return.second_best_match_index   = search_index;
                  snail_trail_match_return.f_second_best_match_found = TRUE;
               }
            }
         }
      }
      return snail_trail_match_return;
   }

   Snail_Trail_Match_T compute_curvi_data::getSnailTrailMatch(const Snail_Trail_T &snail_in, const Vehicle_Info_T &vehicle_info, const Olp_Vector_2d_T &obj_center_pos, const Olp_Calibration_T &cal)
   {
      Snail_Trail_Match_T snail_trail_match_return;

      if (IS_FALSE(cal.k_vp_enable_snail_trail))
      {
         return initSnailTrailMatch(snail_in);
      }
      float rear_axle_position = -(OLP_FAST_ABS(vehicle_info.k_dist_rear_axle_to_vcs));
      snail_trail_match_return = Get_Snail_Trail_Index(snail_in, rear_axle_position, obj_center_pos, cal);

      if (OLP_IS_TRUE(snail_trail_match_return.f_best_match_found))
      {
         calcSnailTrailMatchFactors(snail_trail_match_return, snail_in, cal);
      }
      else
      {
         snail_trail_match_return = initSnailTrailMatch(snail_in);  /* No match found */
      }

      return snail_trail_match_return;
   }

   float compute_curvi_data::snailTrailInterpolate(const float first_val, const float second_val, const Snail_Trail_Match_T &snail_trail_match)
   {
      float ret_val;

      if (OLP_IS_TRUE(snail_trail_match.f_second_best_match_found))
      {
         ret_val = (snail_trail_match.best_match_interpolation_factor * first_val) + (snail_trail_match.second_best_match_interpolation_factor * second_val);
      }
      else
      {
         ret_val = first_val;
      }
      return ret_val;
   }


   float compute_curvi_data::getSnailTrailHeading(const Snail_Trail_Match_T &snail_trail_match)
   {
      float best_match_heading;
      float second_best_match_heading;

      best_match_heading = snail_trail_match.p_snail_trail->snail_trail_states[snail_trail_match.best_match_index].heading;
      second_best_match_heading = snail_trail_match.p_snail_trail->snail_trail_states[snail_trail_match.second_best_match_index].heading;
      best_match_heading = olp_ml_wrapper::Olp_NormalizeAngle(best_match_heading, second_best_match_heading);

      return snailTrailInterpolate(
      best_match_heading,
      second_best_match_heading,
      snail_trail_match);
   }

   Olp_Angle_T compute_curvi_data::getSnailTrailHeading_angle(const Snail_Trail_Match_T &snail_trail_match)
   {
      float st_heading = getSnailTrailHeading(snail_trail_match);

      return olp_ml_wrapper::Olp_Create_Angle(st_heading);
   }

   Olp_Vector_2d_T compute_curvi_data::getMatchedSnailTrailPoint(const Snail_Trail_Match_T &snail_trail_match)
   {
      Olp_Vector_2d_T ret_point;

      ret_point.x = snailTrailInterpolate(
      snail_trail_match.p_snail_trail->snail_trail_states[snail_trail_match.best_match_index].point.x,
      snail_trail_match.p_snail_trail->snail_trail_states[snail_trail_match.second_best_match_index].point.x,
      snail_trail_match);
      ret_point.y = snailTrailInterpolate(
      snail_trail_match.p_snail_trail->snail_trail_states[snail_trail_match.best_match_index].point.y,
      snail_trail_match.p_snail_trail->snail_trail_states[snail_trail_match.second_best_match_index].point.y,
      snail_trail_match);
      return ret_point;
   }

   float compute_curvi_data::getSnailTrailDistTraveled(const Snail_Trail_Match_T &snail_trail_match)
   {
      return (snailTrailInterpolate(
      snail_trail_match.p_snail_trail->snail_trail_states[snail_trail_match.best_match_index].distance_traveled,
      snail_trail_match.p_snail_trail->snail_trail_states[snail_trail_match.second_best_match_index].distance_traveled,
      snail_trail_match));
   }

   Olp_Vector_2d_T compute_curvi_data::Transform_Point_On_Snail_Trail(const Snail_Trail_Match_T &snail_match_in, const Olp_Vector_2d_T &obj_center_pos, const Vehicle_Info_T &vehicle_info, const Snail_Trail_T &p_snail_trail)
   {
      Olp_Vector_2d_T     point_curvi;
      Olp_Angle_T         matched_snail_trail_heading;
      Olp_Vector_2d_T     matched_snail_point;
      float matched_snail_trail_dist;

      Olp_Vector_2d_T host_rear_axle_pos_vcs;
      Olp_Vector_2d_T point_wcs;

      matched_snail_trail_heading = getSnailTrailHeading_angle(snail_match_in);
      matched_snail_point = getMatchedSnailTrailPoint(snail_match_in);
      matched_snail_trail_dist = getSnailTrailDistTraveled(snail_match_in);

      float rear_axle_pos = -(OLP_FAST_ABS(vehicle_info.k_dist_rear_axle_to_vcs));

      host_rear_axle_pos_vcs = olp_ml_wrapper::Olp_Create_2d_Vector_Coordinates(rear_axle_pos, 0.0f);
      point_wcs = Transform_Point_Vcs_to_Wcs(obj_center_pos, host_rear_axle_pos_vcs, p_snail_trail.snail_host_heading, p_snail_trail.snail_host_position);

      /* transform point from world coordinate system to curvi coordinate system using snail trail information */
      point_curvi = olp_ml_wrapper::Olp_Vector_2d_Alg_Diff(point_wcs, matched_snail_point);
      point_curvi = olp_ml_wrapper::Olp_Vector_2d_Alg_RotateNegative(matched_snail_trail_heading, point_curvi);
      point_curvi = olp_ml_wrapper::Olp_Vector_2d_Alg_Add(point_curvi, host_rear_axle_pos_vcs);
      point_curvi.x += (matched_snail_trail_dist - p_snail_trail.snail_host_dist);

      return point_curvi;
   }

   void compute_curvi_data::Calculate_Relative_Velocity(Curvi_Data_T &obj_curvi, const Vehicle_Info_T &vehicle_info)
   {
      obj_curvi.curvi_vel_rel.x = obj_curvi.curvi_vel.x - vehicle_info.vcs_long_velocity;
      obj_curvi.curvi_vel_rel.y = obj_curvi.curvi_vel.y - vehicle_info.vcs_lat_velocity;
   }

   void compute_curvi_data::convertVCSToCurvi_objectState(Curvi_Data_T &obj_curvi, const Object_Vcs_T &obj_vcs_in, const Vehicle_Info_T &vehicle_info, const Snail_Trail_T &p_snail_trail, const Curvature_Info_T &curvature_input, const Olp_Calibration_T &cal)
   {
      if ((vehicle_info.filt_veh_speed_over_ground  > cal.k_ad_min_ego_speed_for_convert_to_curvi) && (obj_vcs_in.center_position.x < -(OLP_FAST_ABS(vehicle_info.k_dist_rear_axle_to_vcs))))
      {
         Olp_Angle_T             alpha;
         Snail_Trail_Match_T snail_trail_match = getSnailTrailMatch(p_snail_trail, vehicle_info, obj_vcs_in.center_position, cal);
         if (OLP_TRUE == snail_trail_match.f_best_match_found)
         {
            obj_curvi.curvi_coordinates_calc_method = OLP_OBJ_CURVI_COORDINATES_SNAIL_TRAIL;
            obj_curvi.curvi_pos = Transform_Point_On_Snail_Trail(snail_trail_match, obj_vcs_in.center_position, vehicle_info, p_snail_trail);
            alpha = olp_ml_wrapper::Olp_Create_Angle(getSnailTrailHeading(snail_trail_match) - p_snail_trail.snail_host_heading.angle);
            obj_curvi.curvi_vel = olp_ml_wrapper::Olp_Vector_2d_Alg_RotateNegative(alpha, obj_vcs_in.velocity);
            Calculate_Relative_Velocity(obj_curvi, vehicle_info);
            obj_curvi.curvi_heading = (olp_ml_wrapper::Olp_Angle_diff(obj_vcs_in.heading, alpha)).angle;
         }
         else
         {
            if((OLP_TRUE == curvature_input.f_host_curvature_calculated) && (OLP_FAST_ABS(curvature_input.host_curvature_slow - obj_vcs_in.curvature) < 0.0033f))
            {
               obj_curvi.curvi_pos = distance_based_curvature::Transform_Point_With_Distance_Based_Curvature(vehicle_info, obj_vcs_in.center_position, curvature_input);
               alpha = olp_ml_wrapper::Olp_Create_Angle(obj_curvi.curvi_pos.x * curvature_input.host_curvature_slow);
               obj_curvi.curvi_coordinates_calc_method = OLP_OBJ_CURVI_COORDINATES_DISTANCE_BASED_CURVATURE;
               obj_curvi.curvi_vel = olp_ml_wrapper::Olp_Vector_2d_Alg_RotateNegative(alpha, obj_vcs_in.velocity);
               Calculate_Relative_Velocity(obj_curvi, vehicle_info);
               obj_curvi.curvi_heading = (olp_ml_wrapper::Olp_Angle_diff(obj_vcs_in.heading, alpha)).angle;
            }
            else
            {
               obj_curvi.curvi_pos = obj_vcs_in.center_position;
               obj_curvi.curvi_vel = obj_vcs_in.velocity;
               Calculate_Relative_Velocity(obj_curvi, vehicle_info);
               obj_curvi.curvi_heading = obj_vcs_in.heading.angle;
               obj_curvi.curvi_coordinates_calc_method = OLP_OBJ_CURVI_COORDINATES_BASED_ON_VCS;
            }
         }
      }
      else
      {
         obj_curvi.curvi_pos = obj_vcs_in.center_position;
         obj_curvi.curvi_vel = obj_vcs_in.velocity;
         Calculate_Relative_Velocity(obj_curvi, vehicle_info);
         obj_curvi.curvi_heading = obj_vcs_in.heading.angle;
         obj_curvi.curvi_coordinates_calc_method = OLP_OBJ_CURVI_COORDINATES_BASED_ON_VCS;
      }
   }

   Curvi_Data_T compute_curvi_data::Compute_Obj_Curvi_Data(const Olp_InOut_Object_Data_T &obj_in, const Snail_Trail_T &snail_in, const Vehicle_Info_T &vehicle_info, const Curvature_Info_T &curvature_input, const Olp_Calibration_T &cal, const Olp_Extended_Object_Data_T &object_extnd_data)
   {
      Curvi_Data_T obj_curvi;
      Object_Vcs_T obj_vcs;

      Initialize_Curvi_Data(obj_curvi);
      obj_vcs = Compute_Obj_Vcs(obj_in, object_extnd_data);
      convertVCSToCurvi_objectState(obj_curvi, obj_vcs, vehicle_info, snail_in, curvature_input, cal);
      return obj_curvi;
   }
}

