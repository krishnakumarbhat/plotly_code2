#ifndef PA_OBJ_IN_H
#define PA_OBJ_IN_H

/**
 * @file pa_obj_in.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file provides the PA tracker object macros for the GDSR tracker
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/*============================================================================*\
 * Includes
\*============================================================================*/

#include "curvi_coordinates_calc_method_T.h"
#include "fbk_macros.h"
#include "object_class_T.h"
#include "pa_const_macros.h"
#include "pa_context.h"
#include "pa_mock_functions.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "track_status_T.h"

/*============================================================================*\
* Mapping functions
\*============================================================================*/

#ifdef __GNUC__
/* coverity[misra_c_2012_rule_1_2_violation][Intentional use to avoid gcc compiler warning about unused function.] */
static Pa_Obj_Status_T Pa_Gdsr_Map_Object_Status(track_status_T gdsr_object_status) __attribute__((unused));
/* coverity[misra_c_2012_rule_1_2_violation][Intentional use to avoid gcc compiler warning about unused function.] */
static Pa_Obj_Class_T Pa_Gdsr_Map_Object_Class(object_class_T gdsr_object_class) __attribute__((unused));
/* coverity[misra_c_2012_rule_1_2_violation][Intentional use to avoid gcc compiler warning about unused function.] */
static Pa_Obj_Curvi_Calc_Method_T Pa_Gdsr_Map_Curvi_Calc_Method(curvi_coordinates_calc_method_T gdsr_curvi_calc_method)
   /* coverity[misra_c_2012_rule_1_2_violation][Intentional use to avoid gcc compiler warning about unused function.] */
   __attribute__((unused));
/* coverity[misra_c_2012_rule_1_2_violation][Intentional use to avoid gcc compiler warning about unused function.] */
static uint8_t Pa_Gdsr_Map_Merged_Obj_Id(int8_t gdsr_merged_obj_id) __attribute__((unused));
#endif


static Pa_Obj_Status_T Pa_Gdsr_Map_Object_Status(track_status_T gdsr_object_status)
{
   Pa_Obj_Status_T pa_obj_status;

   switch (gdsr_object_status)
   {
      case TRACK_STATUS_INVALID:
         pa_obj_status = PA_OBJ_STATUS_INVALID;
         break;
      case TRACK_STATUS_NEW:
         pa_obj_status = PA_OBJ_STATUS_NEW;
         break;
      case TRACK_STATUS_MATURE:
         pa_obj_status = PA_OBJ_STATUS_MATURE;
         break;
      case TRACK_STATUS_COASTED:
         pa_obj_status = PA_OBJ_STATUS_COASTED;
         break;
      default:
         pa_obj_status = PA_OBJ_STATUS_INVALID;
         break;
   }

   return pa_obj_status;
}

static Pa_Obj_Class_T Pa_Gdsr_Map_Object_Class(object_class_T gdsr_object_class)
{
   Pa_Obj_Class_T pa_obj_class;

   switch (gdsr_object_class)
   {
      case OBJECT_CLASS_UNKNOWN:
         pa_obj_class = PA_OBJ_CLASS_UNKNOWN;
         break;
      case OBJECT_CLASS_PEDESTRIAN:
         pa_obj_class = PA_OBJ_CLASS_PEDESTRIAN;
         break;
      case OBJECT_CLASS_2WHEEL:
         pa_obj_class = PA_OBJ_CLASS_2WHEEL;
         break;
      case OBJECT_CLASS_CAR:
         pa_obj_class = PA_OBJ_CLASS_CAR;
         break;
      case OBJECT_CLASS_TRUCK:
         pa_obj_class = PA_OBJ_CLASS_TRUCK;
         break;
      default:
         pa_obj_class = PA_OBJ_CLASS_UNKNOWN;
         break;
   }
   return pa_obj_class;
}

static Pa_Obj_Curvi_Calc_Method_T Pa_Gdsr_Map_Curvi_Calc_Method(curvi_coordinates_calc_method_T gdsr_curvi_calc_method)
{
   Pa_Obj_Curvi_Calc_Method_T pa_curvi_calc_method;

   switch (gdsr_curvi_calc_method)
   {
      case CURVI_COORDINATES_UNKNOWN:
         pa_curvi_calc_method = PA_OBJ_CURVI_COORDINATES_UNKNOWN;
         break;
      case CURVI_COORDINATES_BASED_ON_VCS:
         pa_curvi_calc_method = PA_OBJ_CURVI_COORDINATES_BASED_ON_VCS;
         break;
      case CURVI_COORDINATES_SNAIL_TRAIL:
         pa_curvi_calc_method = PA_OBJ_CURVI_COORDINATES_SNAIL_TRAIL;
         break;
      case CURVI_COORDINATES_DISTANCE_BASED_CURVATURE:
         pa_curvi_calc_method = PA_OBJ_CURVI_COORDINATES_DISTANCE_BASED_CURVATURE;
         break;
      default:
         pa_curvi_calc_method = PA_OBJ_CURVI_COORDINATES_UNKNOWN;
         break;
   }

   return pa_curvi_calc_method;
}

static uint8_t Pa_Gdsr_Map_Merged_Obj_Id(int8_t gdsr_merged_obj_id)
{
   uint8_t pa_merged_obj_id = PA_INVALID_OBJ_ID;

   if (gdsr_merged_obj_id >= 0)
   {
      pa_merged_obj_id = (uint8_t) gdsr_merged_obj_id;
   }

   return pa_merged_obj_id;
}

/*============================================================================*\
* Function like macro
\*============================================================================*/

/**
 * @brief Returns the ID of the object with given index
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Id(p_context, index) \
   ((uint8_t) (((p_context)->p_tracker_output->id[(uint8_t) (index)] < 0) ? 0 : (p_context)->p_tracker_output->id[(uint8_t) (index)]))

/**
 * @brief Returns the unique ID of the object with given index
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Unique_Id(p_context, index) ((uint32_t) Pa_Mock_Uint(p_context, index, 0))

/**
 * @brief Returns the status of the object with given index
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Status(p_context, index) (Pa_Gdsr_Map_Object_Status((p_context)->p_tracker_output->status[(uint8_t) (index)]))

/**
 * @brief Returns the age of the object with given index
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Age(p_context, index) ((uint8_t) (p_context)->p_tracker_output->age[(uint8_t) (index)])

/**
 * @brief Returns the stage age of the object with given index
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Stage_Age(p_context, index) ((uint8_t) (p_context)->p_tracker_output->stage_age[(uint8_t) (index)])

/**
 * @brief Returns the alternative stage age of the object with given index computed by the Feature Building Kit.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Fbk_Obj_Stage_Age(p_context, index) ((uint8_t) (p_context)->p_fbk_obj_ageing->stage_age[(uint8_t) (index)])

/**
 * @brief Returns the existence probability of the object with given index
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Exist_Prob(p_context, index) \
   ((float32_T) (p_context)->p_tracker_output->existence_probability[(uint8_t) (index)])

/**
 * @brief Returns the absolute longitudinal position in vcs
 * of the object with given index in [m]
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Vcs_Long_Pos(p_context, index) ((float32_T) (p_context)->p_tracker_output->vcs_long_posn[(uint8_t) (index)])

/**
 * @brief Returns the longitudinal velocity in vcs
 * of the object with given index in [m/s]
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Vcs_Long_Vel(p_context, index) ((float32_T) (p_context)->p_tracker_output->vcs_long_vel[(uint8_t) (index)])

/**
 * @brief Returns the relative longitudinal velocity in vcs
 * of the object with given index in [m/s]
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Vcs_Long_Vel_Rel(p_context, index) \
   ((float32_T) (p_context)->p_tracker_output->vcs_long_vel_rel[(uint8_t) (index)])

/**
 * @brief Returns the longitudinal acceleration in vcs
 * of the object with given index in [m/s^2]
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Vcs_Long_Accel(p_context, index) ((float32_T) (p_context)->p_tracker_output->vcs_long_accel[(uint8_t) (index)])


/**
 * @brief Returns the absolute lateral position in vcs
 * of the object with given index in [m]
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Vcs_Lat_Pos(p_context, index) ((float32_T) (p_context)->p_tracker_output->vcs_lat_posn[(uint8_t) (index)])

/**
 * @brief Returns the absolute latitude velocity in vcs
 * of the object with given index in [m/s]
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Vcs_Lat_Vel(p_context, index) ((float32_T) (p_context)->p_tracker_output->vcs_lat_vel[(uint8_t) (index)])

/**
 * @brief Returns the relative longitude velocity in vcs
 * of the object with given index in [m/s]
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Vcs_Lat_Vel_Rel(p_context, index) \
   ((float32_T) (p_context)->p_tracker_output->vcs_lat_vel_rel[(uint8_t) (index)])

/**
 * @brief Returns the lateral acceleration in vcs
 * of the object with given index in [m/s^2]
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Vcs_Lat_Accel(p_context, index) ((float32_T) (p_context)->p_tracker_output->vcs_lat_accel[(uint8_t) (index)])

/**
 * @brief Returns the heading of the object with given index in [rad]
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Heading(p_context, index) ((float32_T) (p_context)->p_tracker_output->heading[(uint8_t) (index)])

/**
 * @brief Returns the heading rate of the object with given index in [rad/s]
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Heading_Rate(p_context, index) ((float32_T) (p_context)->p_tracker_output->heading_rate[(uint8_t) (index)])

/**
 * @brief Returns the heading variance of the object with given index in [rad^2]
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Heading_Variance(p_context, index) \
   ((float32_T) (p_context)->p_tracker_output->variance[(uint8_t) (index)].heading)

/**
 * @brief Returns the heading of the object with given index in [m/s]
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Speed(p_context, index) ((float32_T) (p_context)->p_tracker_output->speed[(uint8_t) (index)])

/**
 * @brief Returns the eclipse value of object with given index
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Eclipse_Value(p_context, index) ((float32_T) (p_context)->p_tracker_output->eclipse_value[(uint8_t) (index)])

/**
 * @brief Returns the length of the object with given index in [m]
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Length(p_context, index) ((float32_T) (p_context)->p_tracker_output->length[(uint8_t) (index)])


/**
 * @brief Returns the length of the object with given index in [m]
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Width(p_context, index) ((float32_T) (p_context)->p_tracker_output->width[(uint8_t) (index)])

/**
 * @brief Returns the minimum distance from border of host vehicle to border of object in [m]
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Distance(p_context, index) ((float32_T) (p_context)->p_tracker_output->object_distance[(uint8_t) (index)])

/**
 * @brief Returns the length of the object with given index
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Obstruction_Prob(p_context, index) \
   ((float32_T) (p_context)->p_tracker_output->obstruction_probability[(uint8_t) (index)])

/**
 * @brief Returns flag indicating whether object with given index is a reflection
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Reflect_Flag(p_context, index) \
   (boolean_T)(Fbk_Is_True((p_context)->p_tracker_output->f_reflection[(uint8_t) (index)]))

/**
 * @brief Returns object class of the tracker object
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Class(p_context, index) \
   (Pa_Gdsr_Map_Object_Class((p_context)->p_tracker_output->object_class[(uint8_t) (index)]))

/**
 * @brief Returns object class probability of the tracker object class pedestrian
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Class_Prob_Pedestrian(p_context, index) \
   ((float32_T) (p_context)->p_tracker_output->object_class_probability[(uint8_t) (index)].probability_pedestrian)

/**
 * @brief Returns object class probability of the tracker object class 2wheel
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Class_Prob_2wheel(p_context, index) \
   ((float32_T) (p_context)->p_tracker_output->object_class_probability[(uint8_t) (index)].probability_2wheel)

/**
 * @brief Returns object class probability of the tracker object class car
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Class_Prob_Car(p_context, index) \
   ((float32_T) (p_context)->p_tracker_output->object_class_probability[(uint8_t) (index)].probability_car)

/**
 * @brief Returns object class probability of the tracker object class truck
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Class_Prob_Truck(p_context, index) \
   ((float32_T) (p_context)->p_tracker_output->object_class_probability[(uint8_t) (index)].probability_truck)


/**
 * @brief Returns id of object which was merged with object at index index
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Id_Merged_Obj(p_context, index) \
   (Pa_Gdsr_Map_Merged_Obj_Id((p_context)->p_tracker_output->f_just_merged_with[(uint8_t) (index)]))

/**
 * @brief Returns flag indicating whether a merge of objects has occured.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_F_Merged_Occured(p_context, index) \
   (boolean_T)(Fbk_Is_True((p_context)->p_tracker_output->f_just_merged[(uint8_t) (index)]))

/**
 * @brief Returns whether front left sensor contributed detections to an object.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Is_Front_Left_Origin_Sensor(p_context, index) \
   (boolean_T)(Fbk_Is_True((p_context)->p_tracker_output->origin_sensor[(uint8_t) (index)].front_left))

/**
 * @brief Returns whether the front right sensor contributed detections to an object.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Is_Front_Right_Origin_Sensor(p_context, index) \
   (boolean_T)(Fbk_Is_True((p_context)->p_tracker_output->origin_sensor[(uint8_t) (index)].front_right))

/**
 * @brief Returns whether the rear left sensor contributed detections to an object.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Is_Rear_Left_Origin_Sensor(p_context, index) \
   (boolean_T)(Fbk_Is_True((p_context)->p_tracker_output->origin_sensor[(uint8_t) (index)].rear_left))

/**
 * @brief Returns whether the rear right sensor contributed detections to an object.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Is_Rear_Right_Origin_Sensor(p_context, index) \
   (boolean_T)(Fbk_Is_True((p_context)->p_tracker_output->origin_sensor[(uint8_t) (index)].rear_right))

/**
 * @brief Returns flag indicating whether the object is in the front left sensor fov, depending on look type.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Is_In_Front_Left_Sensor_Fov(p_context, index) \
   (boolean_T)(Fbk_Is_True((p_context)->p_tracker_output->in_sensor_FOV[(uint8_t) (index)].front_left))

/**
 * @brief Returns flag indicating whether the object is in the front right sensor fov, depending on look type.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Is_In_Front_Right_Sensor_Fov(p_context, index) \
   (boolean_T)(Fbk_Is_True((p_context)->p_tracker_output->in_sensor_FOV[(uint8_t) (index)].front_right))

/**
 * @brief Returns flag indicating whether the object is in the rear left sensor fov, depending on look type.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Is_In_Rear_Left_Sensor_Fov(p_context, index) \
   (boolean_T)(Fbk_Is_True((p_context)->p_tracker_output->in_sensor_FOV[(uint8_t) (index)].rear_left))

/**
 * @brief Returns flag indicating whether the object is in the rear right sensor fov, depending on look type.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Is_In_Rear_Right_Sensor_Fov(p_context, index) \
   (boolean_T)(Fbk_Is_True((p_context)->p_tracker_output->in_sensor_FOV[(uint8_t) (index)].rear_right))

/**
 * @brief Returns which method was used to calculate the curvi coordinates.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Curvi_Coordinates_Calc_Method(p_context, index) \
   (Pa_Gdsr_Map_Curvi_Calc_Method((p_context)->p_tracker_output->curvi_coordinates_calc_method[(uint8_t) (index)]))

/**
 * @brief Returns longitudinal curvi coordinates of object.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Curvi_Long_Posn(p_context, index) \
   ((float32_T) (p_context)->p_tracker_output->curvi_long_posn[(uint8_t) (index)])

/**
 * @brief Returns lateral curvi coordinates of object.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Curvi_Lat_Posn(p_context, index) ((float32_T) (p_context)->p_tracker_output->curvi_lat_posn[(uint8_t) (index)])

/**
 * @brief Returns longitudinal curvi velocity of object.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Curvi_Long_Vel(p_context, index) ((float32_T) (p_context)->p_tracker_output->curvi_long_vel[(uint8_t) (index)])

/**
 * @brief Returns longitudinal curvi relative velocity of object.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Curvi_Long_Vel_Rel(p_context, index) \
   ((float32_T) (p_context)->p_tracker_output->curvi_long_vel_rel[(uint8_t) (index)])

/**
 * @brief Returns lateral curvi velocity of object.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Curvi_Lat_Vel(p_context, index) ((float32_T) (p_context)->p_tracker_output->curvi_lat_vel[(uint8_t) (index)])

/**
 * @brief Returns lateral curvi relative velocity of object.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Curvi_Lat_Vel_Rel(p_context, index) \
   ((float32_T) (p_context)->p_tracker_output->curvi_lat_vel_rel[(uint8_t) (index)])

/**
 * @brief Returns the curvi heading of the object with given index in [rad]
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Curvi_Heading(p_context, index) ((float32_T) (p_context)->p_tracker_output->curvi_heading[(uint8_t) (index)])

/**
 * @brief Returns timestamp difference compared to last cycle.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Time_Diff_To_Last_Cycle(p_context) ((float32_T) (p_context)->p_tracker_output->tracker_status.time_stamp_delta)

/**
 * @brief Returns heading accuracy.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Heading_Accuracy(p_context, index) \
   ((float32_T) (p_context)->p_tracker_output->accuracy[(uint8_t) index].heading)

/**
 * @brief Returns flag indicating whether the object is stationary.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_F_Stationary(p_context, index) \
   (boolean_T)(Fbk_Is_True((p_context)->p_tracker_output->f_stationary[(uint8_t) (index)]))

/**
 * @brief Returns flag indicating whether object with given index is moveable
 *
 * @SDD{}
 */
#define Pa_Get_Obj_F_Moveable(p_context, index) \
   ((boolean_T) Fbk_Is_True((p_context)->p_tracker_output->f_moveable[(uint8_t) (index)]))

/**
 * @brief Returns flag indicating whether the object is stationary clutter, mocked to be false.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_F_Stationary_Clutter(p_context, index) (Pa_Mock_Boolean(p_context, index, FBK_FALSE))

#endif
