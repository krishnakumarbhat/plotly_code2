/**
 * @file fbk_object_validation.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Source file with functions for object validation.
 *
 * @copyright Copyright (C) 2025 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_object_validation.h"
#include "fbk_macros.h"
#include "ml_vector_2d_t.h"
#include "pa_obj_in.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include <assert.h>

/*===========================================================================*\
* Macros
\*===========================================================================*/

/* Status range */
#define FBK_STATUS_MAX_VAL (4U) /* Adjust based on Pa_Obj_Status_T enum */

/* Probability ranges */
#define FBK_EXISTENCE_PROBABILITY_MIN_VAL (0.0F)
#define FBK_EXISTENCE_PROBABILITY_MAX_VAL (1.0F)

/* Speed range */
#define FBK_SPEED_MIN_VAL (-200.0F) /* m/s */
#define FBK_SPEED_MAX_VAL (200.0F)  /* m/s */

/* VCS position ranges */
#define FBK_VCS_POS_X_MIN_VAL (-1000.0F) /* m */
#define FBK_VCS_POS_X_MAX_VAL (1000.0F)  /* m */
#define FBK_VCS_POS_Y_MIN_VAL (-1000.0F) /* m */
#define FBK_VCS_POS_Y_MAX_VAL (1000.0F)  /* m */

/* VCS velocity ranges */
#define FBK_VCS_VEL_X_MIN_VAL (-200.0F) /* m/s */
#define FBK_VCS_VEL_X_MAX_VAL (200.0F)  /* m/s */
#define FBK_VCS_VEL_Y_MIN_VAL (-200.0F) /* m/s */
#define FBK_VCS_VEL_Y_MAX_VAL (200.0F)  /* m/s */

/* VCS relative velocity ranges */
#define FBK_VCS_VEL_REL_X_MIN_VAL (-200.0F) /* m/s */
#define FBK_VCS_VEL_REL_X_MAX_VAL (200.0F)  /* m/s */
#define FBK_VCS_VEL_REL_Y_MIN_VAL (-200.0F) /* m/s */
#define FBK_VCS_VEL_REL_Y_MAX_VAL (200.0F)  /* m/s */

/* VCS acceleration ranges */
#define FBK_VCS_ACCEL_X_MIN_VAL (-50.0F) /* m/s^2 */
#define FBK_VCS_ACCEL_X_MAX_VAL (50.0F)  /* m/s^2 */
#define FBK_VCS_ACCEL_Y_MIN_VAL (-50.0F) /* m/s^2 */
#define FBK_VCS_ACCEL_Y_MAX_VAL (50.0F)  /* m/s^2 */

/* Heading related ranges */
#define FBK_VCS_HEADING_MIN_VAL (-10.0F)  /* rad */
#define FBK_VCS_HEADING_MAX_VAL (10.0F)   /* rad */
#define FBK_HEADING_RATE_MIN_VAL (-10.0F) /* rad/s */
#define FBK_HEADING_RATE_MAX_VAL (10.0F)  /* rad/s */
#define FBK_HEADING_VARIANCE_MIN_VAL (0.0F)
#define FBK_HEADING_VARIANCE_MAX_VAL (10.0F)
#define FBK_ACCURACY_HEADING_MIN_VAL (0.0F)
#define FBK_ACCURACY_HEADING_MAX_VAL (3.15F) /* rad */

/* Eclipse value range */
#define FBK_ECLIPSE_VALUE_MIN_VAL (0.0F)
#define FBK_ECLIPSE_VALUE_MAX_VAL (1.0F)

/* Size related ranges */
#define FBK_LENGTH_MIN_VAL (0.0F)  /* m */
#define FBK_LENGTH_MAX_VAL (40.0F) /* m */
#define FBK_WIDTH_MIN_VAL (0.0F)   /* m */
#define FBK_WIDTH_MAX_VAL (10.0F)  /* m */

/* Distance and obstruction ranges */
#define FBK_OBJ_DISTANCE_MIN_VAL (0.0F)    /* m */
#define FBK_OBJ_DISTANCE_MAX_VAL (1000.0F) /* m */
#define FBK_OBSTRUCTION_PROB_MIN_VAL (0.0F)
#define FBK_OBSTRUCTION_PROB_MAX_VAL (1.0F)

/* Object class range */
#define FBK_OBJ_CLASS_MAX_VAL (4U) /* Adjust based on Pa_Obj_Class_T enum */

/* Class probability ranges */
#define FBK_CLASS_PROB_PEDESTRIAN_MIN_VAL (0.0F)
#define FBK_CLASS_PROB_PEDESTRIAN_MAX_VAL (1.0F)
#define FBK_CLASS_PROB_2WHEEL_MIN_VAL (0.0F)
#define FBK_CLASS_PROB_2WHEEL_MAX_VAL (1.0F)
#define FBK_CLASS_PROB_CAR_MIN_VAL (0.0F)
#define FBK_CLASS_PROB_CAR_MAX_VAL (1.0F)
#define FBK_CLASS_PROB_TRUCK_MIN_VAL (0.0F)
#define FBK_CLASS_PROB_TRUCK_MAX_VAL (1.0F)

/* Curvilinear coordinates calculation method range */
#define FBK_CURVI_COORDINATES_CALC_METHOD_MAX_VAL (3U) /* Adjust based on Pa_Obj_Curvi_Calc_Method_T enum */

/* Curvilinear position ranges */
#define FBK_CURVI_POS_X_MIN_VAL (-1000.0F) /* m */
#define FBK_CURVI_POS_X_MAX_VAL (1000.0F)  /* m */
#define FBK_CURVI_POS_Y_MIN_VAL (-1000.0F) /* m */
#define FBK_CURVI_POS_Y_MAX_VAL (1000.0F)  /* m */

/* Curvilinear velocity ranges */
#define FBK_CURVI_VEL_X_MIN_VAL (-200.0F) /* m/s */
#define FBK_CURVI_VEL_X_MAX_VAL (200.0F)  /* m/s */
#define FBK_CURVI_VEL_Y_MIN_VAL (-200.0F) /* m/s */
#define FBK_CURVI_VEL_Y_MAX_VAL (200.0F)  /* m/s */

/* Curvilinear relative velocity ranges */
#define FBK_CURVI_VEL_REL_X_MIN_VAL (-200.0F) /* m/s */
#define FBK_CURVI_VEL_REL_X_MAX_VAL (200.0F)  /* m/s */
#define FBK_CURVI_VEL_REL_Y_MIN_VAL (-200.0F) /* m/s */
#define FBK_CURVI_VEL_REL_Y_MAX_VAL (200.0F)  /* m/s */

/* Curvilinear heading range */
#define FBK_CURVI_HEADING_MIN_VAL (-10.0F) /* rad */
#define FBK_CURVI_HEADING_MAX_VAL (10.0F)  /* rad */


/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
boolean_T Fbk_Is_Obj_Coasted_Status_Implausible(const Fbk_Object_Data_T *p_object_data)
{
   boolean_T f_obj_status_implausible = FBK_FALSE;
   boolean_T f_obj_not_in_fov         = (boolean_T) Fbk_Is_False(Fbk_Is_Obj_In_Any_Sensor_Fov(p_object_data));
   if (f_obj_not_in_fov && (PA_OBJ_STATUS_COASTED == p_object_data->status))
   {
      f_obj_status_implausible = FBK_TRUE;
   }

   return f_obj_status_implausible;
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
boolean_T Fbk_Is_Obj_In_Any_Sensor_Fov(const Fbk_Object_Data_T *p_object_data)
{
   boolean_T f_obj_in_any_fov = FBK_FALSE;

   assert(NULL != p_object_data);

   if (p_object_data->f_is_in_fl_sensor_fov || p_object_data->f_is_in_fr_sensor_fov || p_object_data->f_is_in_rl_sensor_fov
       || p_object_data->f_is_in_rr_sensor_fov)
   {
      f_obj_in_any_fov = FBK_TRUE;
   }

   return f_obj_in_any_fov;
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
void Fbk_Fill_Object_Information(Fbk_Object_Data_T *p_fbk_object_data, const Pa_Context_T *p_context, const uint8_t obj_index)
{
   /* Asserts */
   assert(NULL != p_fbk_object_data);
   assert(NULL != p_context);

   /* Fill tracker object information */
   p_fbk_object_data->index                         = obj_index;
   p_fbk_object_data->id                            = Pa_Get_Obj_Id(p_context, obj_index);
   p_fbk_object_data->unique_id                     = Pa_Get_Obj_Unique_Id(p_context, obj_index);
   p_fbk_object_data->status                        = Pa_Get_Obj_Status(p_context, obj_index);
   p_fbk_object_data->age                           = Pa_Get_Obj_Age(p_context, obj_index);
   p_fbk_object_data->stage_age                     = Pa_Get_Obj_Stage_Age(p_context, obj_index);
   p_fbk_object_data->existence_probability         = Pa_Get_Obj_Exist_Prob(p_context, obj_index);
   p_fbk_object_data->speed                         = Pa_Get_Obj_Speed(p_context, obj_index);
   p_fbk_object_data->vcs_pos.x                     = Pa_Get_Obj_Vcs_Long_Pos(p_context, obj_index);
   p_fbk_object_data->vcs_vel.x                     = Pa_Get_Obj_Vcs_Long_Vel(p_context, obj_index);
   p_fbk_object_data->vcs_vel_rel.x                 = Pa_Get_Obj_Vcs_Long_Vel_Rel(p_context, obj_index);
   p_fbk_object_data->vcs_accel.x                   = Pa_Get_Obj_Vcs_Long_Accel(p_context, obj_index);
   p_fbk_object_data->vcs_pos.y                     = Pa_Get_Obj_Vcs_Lat_Pos(p_context, obj_index);
   p_fbk_object_data->vcs_vel.y                     = Pa_Get_Obj_Vcs_Lat_Vel(p_context, obj_index);
   p_fbk_object_data->vcs_vel_rel.y                 = Pa_Get_Obj_Vcs_Lat_Vel_Rel(p_context, obj_index);
   p_fbk_object_data->vcs_accel.y                   = Pa_Get_Obj_Vcs_Lat_Accel(p_context, obj_index);
   p_fbk_object_data->vcs_heading                   = Pa_Get_Obj_Heading(p_context, obj_index);
   p_fbk_object_data->heading_rate                  = Pa_Get_Obj_Heading_Rate(p_context, obj_index);
   p_fbk_object_data->heading_variance              = Pa_Get_Obj_Heading_Variance(p_context, obj_index);
   p_fbk_object_data->accuracy_heading              = Pa_Get_Obj_Heading_Accuracy(p_context, obj_index);
   p_fbk_object_data->eclipse_value                 = Pa_Get_Obj_Eclipse_Value(p_context, obj_index);
   p_fbk_object_data->length                        = Pa_Get_Obj_Length(p_context, obj_index);
   p_fbk_object_data->width                         = Pa_Get_Obj_Width(p_context, obj_index);
   p_fbk_object_data->obj_distance                  = Pa_Get_Obj_Distance(p_context, obj_index);
   p_fbk_object_data->obstruction_prob              = Pa_Get_Obj_Obstruction_Prob(p_context, obj_index);
   p_fbk_object_data->obj_class                     = Pa_Get_Obj_Class(p_context, obj_index);
   p_fbk_object_data->class_prob_pedestrian         = Pa_Get_Obj_Class_Prob_Pedestrian(p_context, obj_index);
   p_fbk_object_data->class_prob_2wheel             = Pa_Get_Obj_Class_Prob_2wheel(p_context, obj_index);
   p_fbk_object_data->class_prob_car                = Pa_Get_Obj_Class_Prob_Car(p_context, obj_index);
   p_fbk_object_data->class_prob_truck              = Pa_Get_Obj_Class_Prob_Truck(p_context, obj_index);
   p_fbk_object_data->id_merged_obj                 = Pa_Get_Obj_Id_Merged_Obj(p_context, obj_index);
   p_fbk_object_data->f_merge_occured               = Pa_Get_Obj_F_Merged_Occured(p_context, obj_index);
   p_fbk_object_data->curvi_coordinates_calc_method = Pa_Get_Obj_Curvi_Coordinates_Calc_Method(p_context, obj_index);
   p_fbk_object_data->curvi_pos.x                   = Pa_Get_Obj_Curvi_Long_Posn(p_context, obj_index);
   p_fbk_object_data->curvi_vel.x                   = Pa_Get_Obj_Curvi_Long_Vel(p_context, obj_index);
   p_fbk_object_data->curvi_vel_rel.x               = Pa_Get_Obj_Curvi_Long_Vel_Rel(p_context, obj_index);
   p_fbk_object_data->curvi_pos.y                   = Pa_Get_Obj_Curvi_Lat_Posn(p_context, obj_index);
   p_fbk_object_data->curvi_vel.y                   = Pa_Get_Obj_Curvi_Lat_Vel(p_context, obj_index);
   p_fbk_object_data->curvi_vel_rel.y               = Pa_Get_Obj_Curvi_Lat_Vel_Rel(p_context, obj_index);
   p_fbk_object_data->curvi_heading                 = Pa_Get_Obj_Curvi_Heading(p_context, obj_index);
   p_fbk_object_data->f_reflection                  = Pa_Get_Obj_Reflect_Flag(p_context, obj_index);
   p_fbk_object_data->f_stationary                  = Pa_Get_Obj_F_Stationary(p_context, obj_index);
   p_fbk_object_data->f_moveable                    = Pa_Get_Obj_F_Moveable(p_context, obj_index);
   p_fbk_object_data->f_is_fl_origin_sensor         = Pa_Get_Obj_Is_Front_Left_Origin_Sensor(p_context, obj_index);
   p_fbk_object_data->f_is_fr_origin_sensor         = Pa_Get_Obj_Is_Front_Right_Origin_Sensor(p_context, obj_index);
   p_fbk_object_data->f_is_rl_origin_sensor         = Pa_Get_Obj_Is_Rear_Left_Origin_Sensor(p_context, obj_index);
   p_fbk_object_data->f_is_rr_origin_sensor         = Pa_Get_Obj_Is_Rear_Right_Origin_Sensor(p_context, obj_index);
   p_fbk_object_data->f_is_in_fl_sensor_fov         = Pa_Get_Obj_Is_In_Front_Left_Sensor_Fov(p_context, obj_index);
   p_fbk_object_data->f_is_in_fr_sensor_fov         = Pa_Get_Obj_Is_In_Front_Right_Sensor_Fov(p_context, obj_index);
   p_fbk_object_data->f_is_in_rl_sensor_fov         = Pa_Get_Obj_Is_In_Rear_Left_Sensor_Fov(p_context, obj_index);
   p_fbk_object_data->f_is_in_rr_sensor_fov         = Pa_Get_Obj_Is_In_Rear_Right_Sensor_Fov(p_context, obj_index);
   p_fbk_object_data->f_stationary_clutter          = Pa_Get_Obj_F_Stationary_Clutter(p_context, obj_index);
}

/* coverity[HIS_CCM][High CCM in this auto-generated function is expected] */
boolean_T Fbk_Verify_Object_Data_Range(const Fbk_Object_Data_T *p_object_data)
{
   boolean_T is_valid = FBK_TRUE;

   assert(NULL != p_object_data);

   /* Verify each field of Fbk_Object_Data_T. Don't check id, age, etc. because they use full range of their types, The C type
    * system already guarantees this constraint. TO DO: Verifies the integrity of a memory block using checksum validation */

   if ((uint8_t) p_object_data->status > FBK_STATUS_MAX_VAL)
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->existence_probability < FBK_EXISTENCE_PROBABILITY_MIN_VAL)
       || (p_object_data->existence_probability > FBK_EXISTENCE_PROBABILITY_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->speed < FBK_SPEED_MIN_VAL) || (p_object_data->speed > FBK_SPEED_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   /* Vector fields */
   if ((p_object_data->vcs_pos.x < FBK_VCS_POS_X_MIN_VAL) || (p_object_data->vcs_pos.x > FBK_VCS_POS_X_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->vcs_pos.y < FBK_VCS_POS_Y_MIN_VAL) || (p_object_data->vcs_pos.y > FBK_VCS_POS_Y_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->vcs_vel.x < FBK_VCS_VEL_X_MIN_VAL) || (p_object_data->vcs_vel.x > FBK_VCS_VEL_X_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->vcs_vel.y < FBK_VCS_VEL_Y_MIN_VAL) || (p_object_data->vcs_vel.y > FBK_VCS_VEL_Y_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->vcs_vel_rel.x < FBK_VCS_VEL_REL_X_MIN_VAL) || (p_object_data->vcs_vel_rel.x > FBK_VCS_VEL_REL_X_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->vcs_vel_rel.y < FBK_VCS_VEL_REL_Y_MIN_VAL) || (p_object_data->vcs_vel_rel.y > FBK_VCS_VEL_REL_Y_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->vcs_accel.x < FBK_VCS_ACCEL_X_MIN_VAL) || (p_object_data->vcs_accel.x > FBK_VCS_ACCEL_X_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->vcs_accel.y < FBK_VCS_ACCEL_Y_MIN_VAL) || (p_object_data->vcs_accel.y > FBK_VCS_ACCEL_Y_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   /* Heading related fields */
   if ((p_object_data->vcs_heading < FBK_VCS_HEADING_MIN_VAL) || (p_object_data->vcs_heading > FBK_VCS_HEADING_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->heading_rate < FBK_HEADING_RATE_MIN_VAL) || (p_object_data->heading_rate > FBK_HEADING_RATE_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->heading_variance < FBK_HEADING_VARIANCE_MIN_VAL)
       || (p_object_data->heading_variance > FBK_HEADING_VARIANCE_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->accuracy_heading < FBK_ACCURACY_HEADING_MIN_VAL)
       || (p_object_data->accuracy_heading > FBK_ACCURACY_HEADING_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->eclipse_value < FBK_ECLIPSE_VALUE_MIN_VAL) || (p_object_data->eclipse_value > FBK_ECLIPSE_VALUE_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   /* Size related fields */
   if ((p_object_data->length < FBK_LENGTH_MIN_VAL) || (p_object_data->length > FBK_LENGTH_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->width < FBK_WIDTH_MIN_VAL) || (p_object_data->width > FBK_WIDTH_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->obj_distance < FBK_OBJ_DISTANCE_MIN_VAL) || (p_object_data->obj_distance > FBK_OBJ_DISTANCE_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->obstruction_prob < FBK_OBSTRUCTION_PROB_MIN_VAL)
       || (p_object_data->obstruction_prob > FBK_OBSTRUCTION_PROB_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   /* Class related fields */
   if ((uint8_t) p_object_data->obj_class > FBK_OBJ_CLASS_MAX_VAL)
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->class_prob_pedestrian < FBK_CLASS_PROB_PEDESTRIAN_MIN_VAL)
       || (p_object_data->class_prob_pedestrian > FBK_CLASS_PROB_PEDESTRIAN_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->class_prob_2wheel < FBK_CLASS_PROB_2WHEEL_MIN_VAL)
       || (p_object_data->class_prob_2wheel > FBK_CLASS_PROB_2WHEEL_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->class_prob_car < FBK_CLASS_PROB_CAR_MIN_VAL) || (p_object_data->class_prob_car > FBK_CLASS_PROB_CAR_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->class_prob_truck < FBK_CLASS_PROB_TRUCK_MIN_VAL)
       || (p_object_data->class_prob_truck > FBK_CLASS_PROB_TRUCK_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->f_merge_occured != FBK_FALSE) && (p_object_data->f_merge_occured != FBK_TRUE))
   {
      is_valid = FBK_FALSE;
   }

   /* Curvilinear coordinates related fields */
   if ((uint8_t) p_object_data->curvi_coordinates_calc_method > FBK_CURVI_COORDINATES_CALC_METHOD_MAX_VAL)
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->curvi_pos.x < FBK_CURVI_POS_X_MIN_VAL) || (p_object_data->curvi_pos.x > FBK_CURVI_POS_X_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->curvi_pos.y < FBK_CURVI_POS_Y_MIN_VAL) || (p_object_data->curvi_pos.y > FBK_CURVI_POS_Y_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->curvi_vel.x < FBK_CURVI_VEL_X_MIN_VAL) || (p_object_data->curvi_vel.x > FBK_CURVI_VEL_X_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->curvi_vel.y < FBK_CURVI_VEL_Y_MIN_VAL) || (p_object_data->curvi_vel.y > FBK_CURVI_VEL_Y_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->curvi_vel_rel.x < FBK_CURVI_VEL_REL_X_MIN_VAL)
       || (p_object_data->curvi_vel_rel.x > FBK_CURVI_VEL_REL_X_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->curvi_vel_rel.y < FBK_CURVI_VEL_REL_Y_MIN_VAL)
       || (p_object_data->curvi_vel_rel.y > FBK_CURVI_VEL_REL_Y_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->curvi_heading < FBK_CURVI_HEADING_MIN_VAL) || (p_object_data->curvi_heading > FBK_CURVI_HEADING_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   /* Boolean fields */
   if ((p_object_data->f_reflection != FBK_FALSE) && (p_object_data->f_reflection != FBK_TRUE))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->f_stationary != FBK_FALSE) && (p_object_data->f_stationary != FBK_TRUE))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->f_moveable != FBK_FALSE) && (p_object_data->f_moveable != FBK_TRUE))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->f_stationary_clutter != FBK_FALSE) && (p_object_data->f_stationary_clutter != FBK_TRUE))
   {
      is_valid = FBK_FALSE;
   }

   /* Sensor origin flags */
   if ((p_object_data->f_is_fl_origin_sensor != FBK_FALSE) && (p_object_data->f_is_fl_origin_sensor != FBK_TRUE))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->f_is_fr_origin_sensor != FBK_FALSE) && (p_object_data->f_is_fr_origin_sensor != FBK_TRUE))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->f_is_rl_origin_sensor != FBK_FALSE) && (p_object_data->f_is_rl_origin_sensor != FBK_TRUE))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->f_is_rr_origin_sensor != FBK_FALSE) && (p_object_data->f_is_rr_origin_sensor != FBK_TRUE))
   {
      is_valid = FBK_FALSE;
   }

   /* Sensor FOV flags */
   if ((p_object_data->f_is_in_fl_sensor_fov != FBK_FALSE) && (p_object_data->f_is_in_fl_sensor_fov != FBK_TRUE))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->f_is_in_fr_sensor_fov != FBK_FALSE) && (p_object_data->f_is_in_fr_sensor_fov != FBK_TRUE))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->f_is_in_rl_sensor_fov != FBK_FALSE) && (p_object_data->f_is_in_rl_sensor_fov != FBK_TRUE))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_object_data->f_is_in_rr_sensor_fov != FBK_FALSE) && (p_object_data->f_is_in_rr_sensor_fov != FBK_TRUE))
   {
      is_valid = FBK_FALSE;
   }

   return is_valid;
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
void Fbk_Reset_Object_Data(Fbk_Object_Data_T *p_fbk_object_data)
{
   /* Assert */
   assert(NULL != p_fbk_object_data);

   p_fbk_object_data->index                         = PA_INVALID_OBJ_INDEX;
   p_fbk_object_data->id                            = PA_INVALID_OBJ_ID;
   p_fbk_object_data->status                        = PA_OBJ_STATUS_INVALID;
   p_fbk_object_data->age                           = FBK_ZERO_UINT;
   p_fbk_object_data->stage_age                     = FBK_ZERO_UINT;
   p_fbk_object_data->fbk_stage_age                 = FBK_ZERO_UINT;
   p_fbk_object_data->existence_probability         = FBK_ZERO_F;
   p_fbk_object_data->speed                         = FBK_ZERO_F;
   p_fbk_object_data->vcs_pos.x                     = FBK_ZERO_F;
   p_fbk_object_data->vcs_vel.x                     = FBK_ZERO_F;
   p_fbk_object_data->vcs_vel_rel.x                 = FBK_ZERO_F;
   p_fbk_object_data->vcs_accel.x                   = FBK_ZERO_F;
   p_fbk_object_data->vcs_pos.y                     = FBK_ZERO_F;
   p_fbk_object_data->vcs_vel.y                     = FBK_ZERO_F;
   p_fbk_object_data->vcs_vel_rel.y                 = FBK_ZERO_F;
   p_fbk_object_data->vcs_accel.y                   = FBK_ZERO_F;
   p_fbk_object_data->vcs_heading                   = FBK_ZERO_F;
   p_fbk_object_data->heading_rate                  = FBK_ZERO_F;
   p_fbk_object_data->heading_variance              = FBK_ZERO_F;
   p_fbk_object_data->accuracy_heading              = FBK_ZERO_F;
   p_fbk_object_data->eclipse_value                 = FBK_ZERO_F;
   p_fbk_object_data->length                        = FBK_ZERO_F;
   p_fbk_object_data->width                         = FBK_ZERO_F;
   p_fbk_object_data->obj_distance                  = FBK_ZERO_F;
   p_fbk_object_data->obstruction_prob              = FBK_ZERO_F;
   p_fbk_object_data->obj_class                     = PA_OBJ_CLASS_UNKNOWN;
   p_fbk_object_data->class_prob_pedestrian         = FBK_ZERO_F;
   p_fbk_object_data->class_prob_2wheel             = FBK_ZERO_F;
   p_fbk_object_data->class_prob_car                = FBK_ZERO_F;
   p_fbk_object_data->class_prob_truck              = FBK_ZERO_F;
   p_fbk_object_data->id_merged_obj                 = FBK_ZERO_INT;
   p_fbk_object_data->f_merge_occured               = FBK_FALSE;
   p_fbk_object_data->curvi_coordinates_calc_method = PA_OBJ_CURVI_COORDINATES_UNKNOWN;
   p_fbk_object_data->curvi_pos.x                   = FBK_ZERO_F;
   p_fbk_object_data->curvi_vel.x                   = FBK_ZERO_F;
   p_fbk_object_data->curvi_vel_rel.x               = FBK_ZERO_F;
   p_fbk_object_data->curvi_pos.y                   = FBK_ZERO_F;
   p_fbk_object_data->curvi_vel.y                   = FBK_ZERO_F;
   p_fbk_object_data->curvi_vel_rel.y               = FBK_ZERO_F;
   p_fbk_object_data->curvi_heading                 = FBK_ZERO_F;
   p_fbk_object_data->f_reflection                  = FBK_FALSE;
   p_fbk_object_data->f_stationary                  = FBK_FALSE;
   p_fbk_object_data->f_moveable                    = FBK_FALSE;
   p_fbk_object_data->f_is_fl_origin_sensor         = FBK_FALSE;
   p_fbk_object_data->f_is_fr_origin_sensor         = FBK_FALSE;
   p_fbk_object_data->f_is_rl_origin_sensor         = FBK_FALSE;
   p_fbk_object_data->f_is_rr_origin_sensor         = FBK_FALSE;
   p_fbk_object_data->f_is_in_fl_sensor_fov         = FBK_FALSE;
   p_fbk_object_data->f_is_in_fr_sensor_fov         = FBK_FALSE;
   p_fbk_object_data->f_is_in_rl_sensor_fov         = FBK_FALSE;
   p_fbk_object_data->f_is_in_rr_sensor_fov         = FBK_FALSE;
   p_fbk_object_data->f_stationary_clutter          = FBK_TRUE;
}


/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
boolean_T Fbk_Is_Obj_State_Valid(Pa_Obj_Status_T obj_state)
{
   boolean_T f_obj_is_valid = FBK_FALSE;

   if ((PA_OBJ_STATUS_COASTED == obj_state) || (PA_OBJ_STATUS_MATURE == obj_state))
   {
      f_obj_is_valid = FBK_TRUE;
   }

   return f_obj_is_valid;
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
uint8_t Fbk_Get_Obj_Side(const float32_T position)
{
   uint8_t side = FBK_SIDE_RIGHT;

   if (position < FBK_ZERO_F)
   {
      side = FBK_SIDE_LEFT;
   }

   return side;
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
uint8_t Fbk_Get_Obj_Side_Coord_Sys(const Fbk_Object_Data_T *p_tracker_data, const boolean_T f_use_curvi_coordinates)
{
   uint8_t side;

   assert(NULL != p_tracker_data);

   /* Check if curvi coordinates are available. */
   if (f_use_curvi_coordinates)
   {
      /* Use curvi lateral position */
      side = Fbk_Get_Obj_Side(p_tracker_data->curvi_pos.y);
   }
   else
   {
      /* Use VCS lateral position */
      side = Fbk_Get_Obj_Side(p_tracker_data->vcs_pos.y);
   }
   /* Return on which side the object is located. */
   return side;
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
float32_T Fbk_Get_Obj_Side_Sign(const float32_T position)
{
   float32_T side_sign = 1.0f;

   if (position < FBK_ZERO_F)
   {
      side_sign = -1.0f;
   }

   return side_sign;
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
float32_T Fbk_Convert_Obj_Side_To_Sign(const uint8_t side)
{
   float32_T sign = 1.0f;

   if (side == FBK_SIDE_LEFT)
   {
      sign = -1.0f;
   }

   return sign;
}
