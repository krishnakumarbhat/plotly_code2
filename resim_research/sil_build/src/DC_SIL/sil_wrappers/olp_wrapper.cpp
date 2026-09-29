#include <cstring>
#include "f360_variant_definition.h"
#include "f360_log_types.h"
#include "f360_host.h"
#include "VSE_Master_Model_L2_types.h"
#include "olp_wrapper.h"
#include "olp_calibration.h"
#include "olp_core_types.h"
#include "olp_iface.h"
#include "sfl_wrapper.h"

/* OLP Inputs */
static Olp_Data_T Olp_Data;
static Olp_Data_T *Olp_Data_GPtr = &Olp_Data;

/* SFL Input */
SFL_Vehicle_Output_T sfl_vehicle_data = {};
SFL_Olp_Objects_Log_T sfl_object_log  = {};

void Olp_Update_Cal_Default_Value(void);

void OLPUpdateVehicleData(Olp_Data_T& olp_data_ref, VSE_OUT* vse_info)
{
	olp_data_ref.veh_info_data.comp_yaw_rate_filtered = vse_info->VsVSE_rps_CompYawRateFilt;
	 // during curve scenario orcas o/p is not proper, lcda didnt give warning if not negated
	olp_data_ref.veh_info_data.k_dist_rear_axle_to_vcs = vse_info->KsVSE_m_DistRearAxleToVCS;
	olp_data_ref.veh_info_data.filt_veh_speed_over_ground = vse_info->VsVSE_mps_VehFiltSpdOverGround;
	olp_data_ref.veh_info_data.vcs_sideslip = vse_info->VsVSE_rad_VCSSideslip;
	olp_data_ref.veh_info_data.sideslip_rear_axle = vse_info->VsVSE_rad_SideslipRearAxle;
	olp_data_ref.veh_info_data.vcs_long_velocity = vse_info->VsVSE_mps_VCSLongVel;
	olp_data_ref.veh_info_data.vcs_lat_velocity = vse_info->VsVSE_mps_VCSLatVel;
}
unsigned char getAge(float time_since_created) {
   int NO_OF_CYCLES_PER_SEC = 20;
   float age                = (time_since_created * NO_OF_CYCLES_PER_SEC);
   age                      = ((age > 255.0F) ? 255.0F : age);

   return (unsigned char)age;
}
Olp_Obj_Status_T F360MapObjectStatus(uint8_t Status) {
   Olp_Obj_Status_T OLP_Status;
   switch (Status) {
   case (unsigned char)0:
      OLP_Status = OLP_OBJ_STATUS_INVALID;
      break;
   case (unsigned char)1:
      OLP_Status = OLP_OBJ_STATUS_NEW;
      break;
   case (unsigned char)2:
      OLP_Status = OLP_OBJ_STATUS_COASTED;
      break;
   case (unsigned char)3:
      OLP_Status = OLP_OBJ_STATUS_MATURE;
      break;
   case (unsigned char)4:
      OLP_Status = OLP_OBJ_STATUS_MATURE;
      break;
   case (unsigned char)5:
      OLP_Status = OLP_OBJ_STATUS_COASTED;
      break;
   default:
      OLP_Status = OLP_OBJ_STATUS_INVALID;
      break;
   }
   return OLP_Status;
}

Olp_Obj_Class_T F360MapObjectClass(uint8_t Class) {
   Olp_Obj_Class_T OLP_Object_Class;
   switch (Class) {
   case (unsigned char)0:
      OLP_Object_Class = OLP_OBJ_CLASS_UNKNOWN;
      break;
   case (unsigned char)1:
      OLP_Object_Class = OLP_OBJ_CLASS_CAR;
      break;
   case (unsigned char)2:
      OLP_Object_Class = OLP_OBJ_CLASS_2WHEEL;
      break;
   case (unsigned char)3:
      OLP_Object_Class = OLP_OBJ_CLASS_TRUCK;
      break;
   case (unsigned char)4:
      OLP_Object_Class = OLP_OBJ_CLASS_PEDESTRIAN;
      break;
   case (unsigned char)5:
      OLP_Object_Class = OLP_OBJ_CLASS_UNKNOWN;
      break;
   case (unsigned char)6:
      OLP_Object_Class = OLP_OBJ_CLASS_UNKNOWN;
      break;
   case (unsigned char)7:
      OLP_Object_Class = OLP_OBJ_CLASS_UNKNOWN;
      break;
   case (unsigned char)8:
      OLP_Object_Class = OLP_OBJ_CLASS_UNKNOWN;
      break;
   case (unsigned char)9:
      OLP_Object_Class = OLP_OBJ_CLASS_2WHEEL;
      break;
   case (unsigned char)10:
      OLP_Object_Class = OLP_OBJ_CLASS_UNKNOWN;
      break;
   default:
      OLP_Object_Class = OLP_OBJ_CLASS_UNKNOWN;
      break;
   }
   return OLP_Object_Class;
}

void OLPUpdateObjectData(Olp_Data_T &olp_data_ref, f360_variant_A::F360_Object_Log_Output_T *obj, ROT_Object_List_Info_T *rot_obj) {
   int reduced_count = 0;
   uint8_t obj_index = 0;
   for (unsigned int i = 0u; i < f360_variant_A::NUMBER_OF_OBJECT_TRACKS; i++) {
      if (obj->object[i].reducedID > 0 && obj_index < OLP_NUMBER_OF_OBJECTS) {
         olp_data_ref.olp_inout_obj_data[obj_index].id        = (unsigned char)obj->object[i].reducedID;
         olp_data_ref.olp_inout_obj_data[obj_index].unique_id = obj->object[i].trkID;
         olp_data_ref.olp_inout_obj_data[obj_index].index     = reduced_count;
         /*  map object status from f360 enum to olp enum in F360MapObjectStatus*/
         olp_data_ref.olp_inout_obj_data[obj_index].status = F360MapObjectStatus(obj->object[i].reducedStatus);
         /* If age is not available from f360 then it can be calculated from time using getAge function*/
         olp_data_ref.olp_inout_obj_data[obj_index].age                   = getAge(obj->object[i].time_since_cluster_created);
         olp_data_ref.olp_inout_obj_data[obj_index].stage_age             = getAge(obj->object[i].time_since_stage_start);
         olp_data_ref.olp_inout_obj_data[obj_index].existence_probability = obj->object[i].existence_probability;
         olp_data_ref.olp_inout_obj_data[obj_index].speed                 = obj->object[i].speed;
         olp_data_ref.olp_inout_obj_data[obj_index].vcs_pos               = {obj->object[i].vcs_xposn, obj->object[i].vcs_yposn};
         olp_data_ref.olp_inout_obj_data[obj_index].vcs_vel               = {obj->object[i].vcs_xvel, obj->object[i].vcs_yvel};
         olp_data_ref.olp_inout_obj_data[obj_index].vcs_vel_rel           = {
             obj->object[i].vcs_xvel - olp_data_ref.veh_info_data.vcs_long_velocity,
             obj->object[i].vcs_yvel - olp_data_ref.veh_info_data.vcs_lat_velocity};
         olp_data_ref.olp_inout_obj_data[obj_index].vcs_accel   = {obj->object[i].vcs_xaccel, obj->object[i].vcs_yaccel};
         olp_data_ref.olp_inout_obj_data[obj_index].vcs_heading = obj->object[i].vcs_heading;
         olp_data_ref.olp_inout_obj_data[obj_index].length      = obj->object[i].len1 + obj->object[i].len2;
         olp_data_ref.olp_inout_obj_data[obj_index].width       = obj->object[i].wid1 + obj->object[i].wid2;
         /* map object class from f360 enum to olp enum in F360MapObjectStatus*/
         olp_data_ref.olp_inout_obj_data[obj_index].obj_class             = F360MapObjectClass(obj->object[i].object_class);
         olp_data_ref.olp_inout_obj_data[obj_index].class_prob_pedestrian = obj->object[i].probability_pedestrian;
         olp_data_ref.olp_inout_obj_data[obj_index].class_prob_2wheel =
             obj->object[i].probability_motorcycle + obj->object[i].probability_bicycle;
         olp_data_ref.olp_inout_obj_data[obj_index].class_prob_car   = obj->object[i].probability_car;
         olp_data_ref.olp_inout_obj_data[obj_index].class_prob_truck = obj->object[i].probability_truck;
         olp_data_ref.olp_inout_obj_data[obj_index].f_moveable       = obj->object[i].f_moveable == 1 ? 1 : 0;

         /* Following signals are used in atleaseone of the feature function. So populate the acceptable value if not available
          * from F360*/
         olp_data_ref.olp_inout_obj_data[obj_index].eclipse_value         = 0;
         olp_data_ref.olp_inout_obj_data[obj_index].obstruction_prob      = 0;
         olp_data_ref.olp_inout_obj_data[obj_index].id_merged_obj         = 0;
         olp_data_ref.olp_inout_obj_data[obj_index].f_merge_occured       = 0;
         olp_data_ref.olp_inout_obj_data[obj_index].f_reflection          = 0;
         olp_data_ref.olp_inout_obj_data[obj_index].f_is_in_fl_sensor_fov = 0;
         olp_data_ref.olp_inout_obj_data[obj_index].f_is_in_fr_sensor_fov = 0;
         olp_data_ref.olp_inout_obj_data[obj_index].f_is_in_rl_sensor_fov = 0;
         olp_data_ref.olp_inout_obj_data[obj_index].f_is_in_rr_sensor_fov = 0;
         olp_data_ref.olp_inout_obj_data[obj_index].obj_distance          = 0;

         /*  Following signals are not used in feature function. So zero can be populated if not available from F360*/
         olp_data_ref.olp_inout_obj_data[obj_index].heading_rate          = 0.0f;
         olp_data_ref.olp_inout_obj_data[obj_index].heading_variance      = 0.0f;
         olp_data_ref.olp_inout_obj_data[obj_index].accuracy_heading      = 0.0f;
         olp_data_ref.olp_inout_obj_data[obj_index].f_stationary          = obj->object[i].f_moveable == 0 ? 1 : 0;
         olp_data_ref.olp_inout_obj_data[obj_index].f_stationary_clutter  = 0u;
         olp_data_ref.olp_inout_obj_data[obj_index].f_is_fl_origin_sensor = 1u;
         olp_data_ref.olp_inout_obj_data[obj_index].f_is_fr_origin_sensor = 1u;
         olp_data_ref.olp_inout_obj_data[obj_index].f_is_rl_origin_sensor = 1u;
         olp_data_ref.olp_inout_obj_data[obj_index].f_is_rr_origin_sensor = 1u;

         /* Integration Guideline --> Following signals extended data of object, which is required for OLP work as expected*/
         olp_data_ref.olp_extnd_obj_data[obj_index].curvature    = obj->object[i].curvature;
         olp_data_ref.olp_extnd_obj_data[obj_index].tang_accel   = obj->object[i].tang_accel;
         olp_data_ref.olp_extnd_obj_data[obj_index].vcs_pointing = obj->object[i].vcs_pointing;
         olp_data_ref.olp_extnd_obj_data[obj_index].len1         = obj->object[i].len1;
         olp_data_ref.olp_extnd_obj_data[obj_index].len2         = obj->object[i].len2;
         olp_data_ref.olp_extnd_obj_data[obj_index].wid1         = obj->object[i].wid1;
         olp_data_ref.olp_extnd_obj_data[obj_index].wid2         = obj->object[i].wid2;
         reduced_count++;
         obj_index++;
      }
   }
   olp_data_ref.no_of_valid_objects = reduced_count;
   // update FOV's
   for (int i = 0; i < f360_variant_A::NUMBER_OF_REDUCED_OBJECT_TRACKS; i++) {
      if (rot_obj->rot_object_list[i].id == olp_data_ref.olp_inout_obj_data[obj_index].id) {
         if (rot_obj->rot_object_list[i].sensor_src == 2) { // FL
            olp_data_ref.olp_inout_obj_data[i].f_is_fl_origin_sensor = 1;
         }
         if (rot_obj->rot_object_list[i].sensor_src == 128) { // FR
            olp_data_ref.olp_inout_obj_data[i].f_is_fr_origin_sensor = 1;
         }
         if (rot_obj->rot_object_list[i].sensor_src == 8) { // RL
            olp_data_ref.olp_inout_obj_data[i].f_is_rl_origin_sensor = 1;
         }
         if (rot_obj->rot_object_list[i].sensor_src == 32) { // RR
            olp_data_ref.olp_inout_obj_data[i].f_is_rr_origin_sensor = 1;
         }
      }
   }
}

/* sfl related input mapping */
void SFLFillVehicleData(f360_variant_A::F360_Host_T *host, VSE_OUT *vse_info) {
   SFL_Vehicle_Output_T *sfl_vehicle_in = GetSFLVehiclePtr();
   sfl_vehicle_in->host_length          = host->vehicle_length;
   sfl_vehicle_in->host_width           = host->vehicle_width;
   sfl_vehicle_in->rear_axle_position   = -host->dist_rear_axle_to_vcs_m;
   // sfl_vehicle_in->wheelbase = host_calib.wheelbase_m;
   sfl_vehicle_in->host_speed         = host->vcs_speed;
   sfl_vehicle_in->yawrate            = host->yaw_rate_rad;
   sfl_vehicle_in->long_acc           = host->vcs_long_acceleration;
   sfl_vehicle_in->lat_acc            = host->vcs_lat_acceleration;
   sfl_vehicle_in->curvature          = host->curvature_rear;
	sfl_vehicle_in->steering_angle = vse_info->VsVSE_deg_RawSteeringAngle * 0.0174533F;
   sfl_vehicle_in->prndl              = host->vcs_speed > 0 ? SFL_VEH_PRNDL_STATE_DRIVE : ((host->vcs_speed == 0) ? SFL_VEH_PRNDL_STATE_PARK : SFL_VEH_PRNDL_STATE_REVERSE);
   sfl_vehicle_in->f_reverse          = host->vcs_speed < 0 ? 1 : 0;
   sfl_vehicle_in->lane_width         = 0.0F;
   sfl_vehicle_in->lane_center_offset = 0.0F;
   sfl_vehicle_in->turn_signal        = 0;
}
SFL_Obj_Status_T SFLMapObjectStatus(Olp_Obj_Status_T olp_status) {
   SFL_Obj_Status_T pa_status;

   switch (olp_status) {
   case OLP_OBJ_STATUS_INVALID:
      pa_status = SFL_OBJ_STATUS_INVALID;
      break;
   case OLP_OBJ_STATUS_NEW:
      pa_status = SFL_OBJ_STATUS_NEW;
      break;
   case OLP_OBJ_STATUS_MATURE:
      pa_status = SFL_OBJ_STATUS_MATURE;
      break;
   case OLP_OBJ_STATUS_COASTED:
      pa_status = SFL_OBJ_STATUS_COASTED;
      break;
   case OLP_OBJ_STATUS_COASTED_IMPLAUSIBLE:
      pa_status = SFL_OBJ_STATUS_COASTED_IMPLAUSIBLE;
      break;
   default:
      pa_status = SFL_OBJ_STATUS_INVALID;
      break;
   }
   return pa_status;
}

SFL_Obj_Class_T SFLMapObjectClass(Olp_Obj_Class_T olp_obj_class) {
   SFL_Obj_Class_T pa_obj_class;

   switch (olp_obj_class) {
   case OLP_OBJ_CLASS_UNKNOWN:
      pa_obj_class = SFL_OBJ_CLASS_UNKNOWN;
      break;
   case OLP_OBJ_CLASS_PEDESTRIAN:
      pa_obj_class = SFL_OBJ_CLASS_PEDESTRIAN;
      break;
   case OLP_OBJ_CLASS_2WHEEL:
      pa_obj_class = SFL_OBJ_CLASS_2WHEEL;
      break;
   case OLP_OBJ_CLASS_CAR:
      pa_obj_class = SFL_OBJ_CLASS_CAR;
      break;
   case OLP_OBJ_CLASS_TRUCK:
      pa_obj_class = SFL_OBJ_CLASS_TRUCK;
      break;
   default:
      pa_obj_class = SFL_OBJ_CLASS_UNKNOWN;
      break;
   }
   return pa_obj_class;
}

SFL_Obj_Curvi_Calc_Method_T SFLMapCurviCalcMethod(Olp_Obj_Curvi_Calc_Method_T olp_curvi_calc_method) {
   SFL_Obj_Curvi_Calc_Method_T pa_curvi_calc_method;

   switch (olp_curvi_calc_method) {
   case OLP_OBJ_CURVI_COORDINATES_UNKNOWN:
      pa_curvi_calc_method = SFL_OBJ_CURVI_COORDINATES_UNKNOWN;
      break;
   case OLP_OBJ_CURVI_COORDINATES_BASED_ON_VCS:
      pa_curvi_calc_method = SFL_OBJ_CURVI_COORDINATES_BASED_ON_VCS;
      break;
   case OLP_OBJ_CURVI_COORDINATES_SNAIL_TRAIL:
      pa_curvi_calc_method = SFL_OBJ_CURVI_COORDINATES_SNAIL_TRAIL;
      break;
   case OLP_OBJ_CURVI_COORDINATES_DISTANCE_BASED_CURVATURE:
      pa_curvi_calc_method = SFL_OBJ_CURVI_COORDINATES_DISTANCE_BASED_CURVATURE;
      break;
   default:
      pa_curvi_calc_method = SFL_OBJ_CURVI_COORDINATES_UNKNOWN;
      break;
   }
   return pa_curvi_calc_method;
}

static inline SFL_Olp_Vector_2d_T SFLMapVector2d(const Olp_Vector_2d_T olp_vector) {
   SFL_Olp_Vector_2d_T result = {olp_vector.x, olp_vector.y};
   return result;
}

void SFLFillObjectData() {
   SFL_Olp_Objects_Log_T *sfl_object_in = GetSFLObjectPtr();
   Olp_Data_T *olp_object_out           = GetOLPObjectsData();
   for (int i = 0; i < SFL_OBJ_NUMBER_OF_OBJECTS; i++) {
      sfl_object_in->obj[i].id                    = olp_object_out->olp_inout_obj_data[i].id;
      sfl_object_in->obj[i].unique_id             = olp_object_out->olp_inout_obj_data[i].unique_id;
      sfl_object_in->obj[i].index                 = olp_object_out->olp_inout_obj_data[i].index;
      sfl_object_in->obj[i].status                = SFLMapObjectStatus(olp_object_out->olp_inout_obj_data[i].status);
      sfl_object_in->obj[i].age                   = olp_object_out->olp_inout_obj_data[i].age;
      sfl_object_in->obj[i].stage_age             = olp_object_out->olp_inout_obj_data[i].stage_age;
      sfl_object_in->obj[i].fbk_stage_age         = olp_object_out->olp_inout_obj_data[i].stage_age;
      sfl_object_in->obj[i].existence_probability = olp_object_out->olp_inout_obj_data[i].existence_probability;
      sfl_object_in->obj[i].speed                 = olp_object_out->olp_inout_obj_data[i].speed;

      sfl_object_in->obj[i].vcs_pos               = SFLMapVector2d(olp_object_out->olp_inout_obj_data[i].vcs_pos);
      sfl_object_in->obj[i].vcs_vel               = SFLMapVector2d(olp_object_out->olp_inout_obj_data[i].vcs_vel);
      sfl_object_in->obj[i].vcs_vel_rel           = SFLMapVector2d(olp_object_out->olp_inout_obj_data[i].vcs_vel_rel);
      sfl_object_in->obj[i].vcs_accel             = SFLMapVector2d(olp_object_out->olp_inout_obj_data[i].vcs_accel);
      sfl_object_in->obj[i].vcs_heading           = olp_object_out->olp_inout_obj_data[i].curvi_heading;
      sfl_object_in->obj[i].heading_rate          = olp_object_out->olp_inout_obj_data[i].heading_rate;
      sfl_object_in->obj[i].heading_variance      = olp_object_out->olp_inout_obj_data[i].heading_variance;
      sfl_object_in->obj[i].accuracy_heading      = olp_object_out->olp_inout_obj_data[i].accuracy_heading;
      sfl_object_in->obj[i].eclipse_value         = olp_object_out->olp_inout_obj_data[i].eclipse_value;
      sfl_object_in->obj[i].length                = olp_object_out->olp_inout_obj_data[i].length;
      sfl_object_in->obj[i].width                 = olp_object_out->olp_inout_obj_data[i].width;
      sfl_object_in->obj[i].obj_distance          = olp_object_out->olp_inout_obj_data[i].obj_distance;
      sfl_object_in->obj[i].obstruction_prob      = olp_object_out->olp_inout_obj_data[i].obstruction_prob;
      sfl_object_in->obj[i].obj_class             = SFLMapObjectClass(olp_object_out->olp_inout_obj_data[i].obj_class);
      sfl_object_in->obj[i].class_prob_pedestrian = olp_object_out->olp_inout_obj_data[i].class_prob_pedestrian;
      sfl_object_in->obj[i].class_prob_2wheel     = olp_object_out->olp_inout_obj_data[i].class_prob_2wheel;
      sfl_object_in->obj[i].class_prob_car        = olp_object_out->olp_inout_obj_data[i].class_prob_car;
      sfl_object_in->obj[i].class_prob_truck      = olp_object_out->olp_inout_obj_data[i].class_prob_truck;

      sfl_object_in->obj[i].id_merged_obj                 = olp_object_out->olp_inout_obj_data[i].id_merged_obj;
      sfl_object_in->obj[i].f_merge_occured               = olp_object_out->olp_inout_obj_data[i].f_merge_occured;
      sfl_object_in->obj[i].curvi_coordinates_calc_method = SFLMapCurviCalcMethod(olp_object_out->olp_inout_obj_data[i].curvi_coordinates_calc_method);
      sfl_object_in->obj[i].curvi_pos                     = SFLMapVector2d(olp_object_out->olp_inout_obj_data[i].curvi_pos);
      sfl_object_in->obj[i].curvi_vel                     = SFLMapVector2d(olp_object_out->olp_inout_obj_data[i].curvi_vel);
      sfl_object_in->obj[i].curvi_vel_rel                 = SFLMapVector2d(olp_object_out->olp_inout_obj_data[i].curvi_vel_rel);
      sfl_object_in->obj[i].curvi_heading                 = olp_object_out->olp_inout_obj_data[i].curvi_heading;
      sfl_object_in->obj[i].f_reflection                  = olp_object_out->olp_inout_obj_data[i].f_reflection;
      sfl_object_in->obj[i].f_stationary                  = olp_object_out->olp_inout_obj_data[i].f_stationary;
      sfl_object_in->obj[i].f_moveable                    = olp_object_out->olp_inout_obj_data[i].f_moveable;
      sfl_object_in->obj[i].f_stationary_clutter          = olp_object_out->olp_inout_obj_data[i].f_stationary_clutter;

      sfl_object_in->obj[i].f_is_fl_origin_sensor = olp_object_out->olp_inout_obj_data[i].f_is_fl_origin_sensor;
      sfl_object_in->obj[i].f_is_fr_origin_sensor = olp_object_out->olp_inout_obj_data[i].f_is_fr_origin_sensor;
      sfl_object_in->obj[i].f_is_rl_origin_sensor = olp_object_out->olp_inout_obj_data[i].f_is_rl_origin_sensor;
      sfl_object_in->obj[i].f_is_rr_origin_sensor = olp_object_out->olp_inout_obj_data[i].f_is_rr_origin_sensor;
      sfl_object_in->obj[i].f_is_in_fl_sensor_fov = olp_object_out->olp_inout_obj_data[i].f_is_in_fl_sensor_fov;
      sfl_object_in->obj[i].f_is_in_fr_sensor_fov = olp_object_out->olp_inout_obj_data[i].f_is_in_fr_sensor_fov;
      sfl_object_in->obj[i].f_is_in_rl_sensor_fov = olp_object_out->olp_inout_obj_data[i].f_is_in_rl_sensor_fov;
      sfl_object_in->obj[i].f_is_in_rr_sensor_fov = olp_object_out->olp_inout_obj_data[i].f_is_in_rr_sensor_fov;
   }
   sfl_object_in->n_valid_objects = olp_object_out->no_of_valid_objects;
}
void InitOLP(void) {
   OLP_Init();
}
void RunOLP(f360_variant_A::F360_Object_Log_Output_T *obj, ROT_Object_List_Info_T *rot_obj, VSE_OUT *vse_info, f360_variant_A::F360_Host_T *host) {
   memset(Olp_Data_GPtr, 0, sizeof(Olp_Data_T));
   Olp_Update_Cal_Default_Value();
   OLPUpdateVehicleData(*Olp_Data_GPtr, vse_info);
   OLPUpdateObjectData(*Olp_Data_GPtr, obj, rot_obj);
   OLP_Main_Run_50ms(Olp_Data_GPtr);
   SFLFillVehicleData(host, vse_info);
   SFLFillObjectData();
}
Olp_Data_T *GetOLPObjectsData(void) {
   return (Olp_Data_GPtr);
}
SFL_Vehicle_Output_T *GetSFLVehiclePtr() {
   return &sfl_vehicle_data;
}

SFL_Olp_Objects_Log_T *GetSFLObjectPtr() {
   return &sfl_object_log;
}
