#ifndef PA_OBJ_IN_H
#define PA_OBJ_IN_H

/**
 * @file pa_obj_in.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file provides the PA tracker object macros for the F360 tracker
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/*============================================================================*\
 * Includes
\*============================================================================*/

#include "fbk_macros.h"
#include "ml_checked_rounding.h"
#include "ml_trigonometry.h"
#include "pa_const_macros.h"
#include "pa_context.h"
#include "pa_mock_functions.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "pa_vehicle_in.h"
#include "T360_Types.h"

/*============================================================================*\
* Mapping functions
\*============================================================================*/

#ifdef __GNUC__
/* coverity[misra_c_2012_rule_1_2_violation][Intentional use to avoid gcc compiler warning about unused function.] */
static inline Pa_Obj_Status_T Pa_F360_Map_Object_Status(uint8_t f360_object_status) __attribute__((unused));
/* coverity[misra_c_2012_rule_1_2_violation][Intentional use to avoid gcc compiler warning about unused function.] */
static inline Pa_Obj_Class_T Pa_F360_Map_Object_Class(uint8_t f360_object_class) __attribute__((unused));
/* coverity[misra_c_2012_rule_1_2_violation][Intentional use to avoid gcc compiler warning about unused function.] */
static inline uint8_t Pa_F360_Convert_Time_To_Cycles(float32_T time_passed, float32_T cycle_time) __attribute__((unused));
#endif


static inline Pa_Obj_Status_T Pa_F360_Map_Object_Status(uint8_t f360_object_status)
{
   Pa_Obj_Status_T pa_obj_status;

   switch (f360_object_status)
   {
      case (uint8_t) F360_OBJ_STATUS_INVALID:
         pa_obj_status = PA_OBJ_STATUS_INVALID;
         break;
      case (uint8_t) F360_OBJ_STATUS_NEW:
         pa_obj_status = PA_OBJ_STATUS_NEW;
         break;
      case (uint8_t) F360_OBJ_STATUS_NEW_UPDATED:
         pa_obj_status = PA_OBJ_STATUS_MATURE;
         break;
      case (uint8_t) F360_OBJ_STATUS_NEW_COASTED:
         pa_obj_status = PA_OBJ_STATUS_COASTED;
         break;
      case (uint8_t) F360_OBJ_STATUS_UPDATED:
         pa_obj_status = PA_OBJ_STATUS_MATURE;
         break;
      case (uint8_t) F360_OBJ_STATUS_COASTED:
         pa_obj_status = PA_OBJ_STATUS_COASTED;
         break;
      default:
         pa_obj_status = PA_OBJ_STATUS_INVALID;
         break;
   }

   return pa_obj_status;
}

static inline Pa_Obj_Class_T Pa_F360_Map_Object_Class(uint8_t f360_object_class)
{
   Pa_Obj_Class_T pa_obj_class;

   switch (f360_object_class)
   {
      case (uint8_t) F360_OBJECT_CLASS_UNDETERMINED:
         pa_obj_class = PA_OBJ_CLASS_UNKNOWN;
         break;
      case (uint8_t) F360_OBJECT_CLASS_CAR:
         pa_obj_class = PA_OBJ_CLASS_CAR;
         break;
      case (uint8_t) F360_OBJECT_CLASS_MOTORCYCLE:
         pa_obj_class = PA_OBJ_CLASS_2WHEEL;
         break;
      case (uint8_t) F360_OBJECT_CLASS_TRUCK:
         pa_obj_class = PA_OBJ_CLASS_TRUCK;
         break;
      case (uint8_t) F360_OBJECT_CLASS_PEDESTRIAN:
         pa_obj_class = PA_OBJ_CLASS_PEDESTRIAN;
         break;
      case (uint8_t) F360_OBJECT_CLASS_POLE:
         pa_obj_class = PA_OBJ_CLASS_UNKNOWN;
         break;
      case (uint8_t) F360_OBJECT_CLASS_TREE:
         pa_obj_class = PA_OBJ_CLASS_UNKNOWN;
         break;
      case (uint8_t) F360_OBJECT_CLASS_ANIMAL:
         pa_obj_class = PA_OBJ_CLASS_UNKNOWN;
         break;
      case (uint8_t) F360_OBJECT_CLASS_GOD:
         pa_obj_class = PA_OBJ_CLASS_UNKNOWN;
         break;
      case (uint8_t) F360_OBJECT_CLASS_BICYCLE:
         pa_obj_class = PA_OBJ_CLASS_2WHEEL;
         break;
      case (uint8_t) F360_OBJECT_CLASS_UNIDENTIFIED_VEHICLE:
         pa_obj_class = PA_OBJ_CLASS_UNKNOWN;
         break;
      default:
         pa_obj_class = PA_OBJ_CLASS_UNKNOWN;
         break;
   }

   return pa_obj_class;
}

static inline uint8_t Pa_F360_Convert_Time_To_Cycles(float32_T time_passed, float32_T cycle_time)
{
   uint8_t cycles = FBK_ZERO_UINT;

   /* Assert */
   assert(cycle_time > FBK_ZERO_F);

   if (time_passed > FBK_ZERO_F)
   {
      cycles = (uint8_t) (Fbk_Min(Roundf_Checked_Uint32(time_passed / cycle_time), 255u));
   }
   return cycles;
}

/*============================================================================*\
* Function like macro
\*============================================================================*/

/**
 * @brief Returns the heading of the object with given index in [rad]
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Heading(p_context, index) ((float32_T) (p_context)->p_tracker_output->obj[(uint8_t) index].vcs_heading)

/**
 * @brief Returns the heading rate of the object with given index in [rad/s]
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Heading_Rate(p_context, index) (Pa_Mock_Float(p_context, index, 0.0f))

/**
 * @brief Returns heading variance
 * Note: Provided by F360 under certain conditions, but appears to be not necessary -> need to check CTA Core code
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Heading_Variance(p_context, index) (Pa_Mock_Float(p_context, index, 0.0f))

/**
 * @brief Returns timestamp difference compared to last cycle.
 * TODO: Not provided by F360 -> needs to be checked, where value can be taken from
 *
 * @SDD{}
 */
#define Pa_Get_Time_Diff_To_Last_Cycle(p_context) (Pa_Mock_Float(p_context, 0u, 0.05f))

/**
 * @brief Returns the ID of the object with given index
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Id(p_context, index) ((uint8_t) (p_context)->p_tracker_output->obj[(uint8_t) index].reducedID)

/**
 * @brief Returns the unique ID of the object with given index
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Unique_Id(p_context, index) ((uint32_t) (p_context)->p_tracker_output->obj[(uint8_t) index].unique_id)

/**
 * @brief Returns the status of the object with given index
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Status(p_context, index) (Pa_F360_Map_Object_Status((p_context)->p_tracker_output->obj[(uint8_t) index].status))

/**
 * @brief Returns the age of the object with given index
 * TODO: Since the intermediate result might be greater than UINT8_T_MAX, a saturation is needed.
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Age(p_context, index)                                                                           \
   (Pa_F360_Convert_Time_To_Cycles((p_context)->p_tracker_output->obj[(uint8_t) index].time_since_cluster_created, \
                                   Pa_Get_Time_Diff_To_Last_Cycle(p_context)))

/**
 * @brief Returns the state age of the object with given index
 * TODO: Age is expected to be number of ticks instead of time in seconds.
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Stage_Age(p_context, index)                                                                 \
   (Pa_F360_Convert_Time_To_Cycles((p_context)->p_tracker_output->obj[(uint8_t) index].time_since_stage_start, \
                                   Pa_Get_Time_Diff_To_Last_Cycle(p_context)))

/**
 * @brief Returns the existence probability of the object with given index
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Exist_Prob(p_context, index) \
   ((float32_T) (p_context)->p_tracker_output->obj[(uint8_t) index].existence_probability)

/**
 * @brief Returns the absolute longitudinal position in vcs
 * of the object with given index in [m]. The longitudinal component of the centroid gets transformed on the center of the
 * bounding box here. @SDD{}
 */

#define Pa_Get_Obj_Vcs_Long_Pos(p_context, index)                                                                                  \
   ((float32_T) 0.5f                                                                                                               \
    * ((2.0f * (p_context)->p_tracker_output->obj[(uint8_t) index].vcs_xposn)                                                      \
       + (Fast_Cos(Pa_Get_Obj_Heading(p_context, index))                                                                           \
          * ((p_context)->p_tracker_output->obj[(uint8_t) index].len2 - (p_context)->p_tracker_output->obj[(uint8_t) index].len1)) \
       - (Fast_Sin(Pa_Get_Obj_Heading(p_context, index))                                                                           \
          * ((p_context)->p_tracker_output->obj[(uint8_t) index].wid2 - (p_context)->p_tracker_output->obj[(uint8_t) index].wid1))))

/**
 * @brief Returns the longitudinal velocity in vcs
 * of the object with given index in [m/s]
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Vcs_Long_Vel(p_context, index) ((float32_T) (p_context)->p_tracker_output->obj[(uint8_t) index].vcs_xvel)

/**
 * @brief Returns the relative longitudinal velocity in vcs
 * of the object with given index in [m/s]
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Vcs_Long_Vel_Rel(p_context, index) \
   (Pa_Get_Obj_Vcs_Long_Vel(p_context, index) - Pa_Veh_Get_Host_Speed(p_context))

/**
 * @brief Returns the longitudinal acceleration in vcs
 * of the object with given index in [m/s^2]
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Vcs_Long_Accel(p_context, index) ((float32_T) (p_context)->p_tracker_output->obj[(uint8_t) index].vcs_xaccel)


/**
 * @brief Returns the absolute lateral position in vcs
 * of the object with given index in [m]. The lateral component of the centroid gets transformed on the center of the bounding
 * box here. @SDD{}
 */
#define Pa_Get_Obj_Vcs_Lat_Pos(p_context, index)                                                                                   \
   ((float32_T) 0.5f                                                                                                               \
    * ((2.0f * (p_context)->p_tracker_output->obj[(uint8_t) index].vcs_yposn)                                                      \
       + (Fast_Sin(Pa_Get_Obj_Heading(p_context, index))                                                                           \
          * ((p_context)->p_tracker_output->obj[(uint8_t) index].len2 - (p_context)->p_tracker_output->obj[(uint8_t) index].len1)) \
       + (Fast_Cos(Pa_Get_Obj_Heading(p_context, index))                                                                           \
          * ((p_context)->p_tracker_output->obj[(uint8_t) index].wid2 - (p_context)->p_tracker_output->obj[(uint8_t) index].wid1))))

/**
 * @brief Returns the absolute latitude velocity in vcs
 * of the object with given index in [m/s]
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Vcs_Lat_Vel(p_context, index) ((float32_T) (p_context)->p_tracker_output->obj[(uint8_t) index].vcs_yvel)

/**
 * @brief Returns the relative longitude velocity in vcs
 * of the object with given index in [m/s]
 * Assumption: Host dependency is negligible to most cases (exception: donut trajectories),
 * use lateral vcs velocity of target instead.
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Vcs_Lat_Vel_Rel(p_context, index) (Pa_Get_Obj_Vcs_Lat_Vel(p_context, index))

/**
 * @brief Returns the lateral acceleration in vcs
 * of the object with given index in [m/s^2]
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Vcs_Lat_Accel(p_context, index) ((float32_T) (p_context)->p_tracker_output->obj[(uint8_t) index].vcs_yaccel)

/**
 * @brief Returns the speed of the object with given index in [m/s]
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Speed(p_context, index) ((float32_T) (p_context)->p_tracker_output->obj[(uint8_t) index].speed)

/**
 * @brief Returns the heading of the object with given index
 * TODO: Not provided by F360 -> needs to be checked, if required at all
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Eclipse_Value(p_context, index) (Pa_Mock_Float(p_context, index, 0.0f))

/**
 * @brief Returns the length of the object with given index in [m]
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Length(p_context, index) \
   ((float32_T) ((p_context)->p_tracker_output->obj[(uint8_t) index].len1 + (p_context)->p_tracker_output->obj[(uint8_t) index].len2))

/**
 * @brief Returns the width of the object with given index in [m]
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Width(p_context, index) \
   ((float32_T) ((p_context)->p_tracker_output->obj[(uint8_t) index].wid1 + (p_context)->p_tracker_output->obj[(uint8_t) index].wid2))

/**
 * @brief Returns the obstruction probability of the object with given index
   TODO: Not provided by F360 -> needs to be checked, if required at all
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Obstruction_Prob(p_context, index) (Pa_Mock_Float(p_context, index, 0.0f))

/**
 * @brief Returns flag indicating whether object with given index is a reflection
 * TODO: Not provided by F360 -> needs to be checked, if required at all
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Reflect_Flag(p_context, index) (Pa_Mock_Boolean(p_context, index, FBK_FALSE))


/**
 * @brief Returns object class of the tracker object
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Class(p_context, index) \
   (Pa_F360_Map_Object_Class((p_context)->p_tracker_output->obj[(uint8_t) index].object_class))

/**
 * @brief Returns object class probability pedestrian of the tracker object
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Class_Prob_Pedestrian(p_context, index) \
   ((float32_T) (p_context)->p_tracker_output->obj[(uint8_t) index].probability_pedestrian)

/**
 * @brief Returns object class probability 2wheel of the tracker object
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Class_Prob_2wheel(p_context, index)                                      \
   ((float32_T) ((p_context)->p_tracker_output->obj[(uint8_t) index].probability_motorcycle \
                 + (p_context)->p_tracker_output->obj[(uint8_t) index].probability_bicycle))

/**
 * @brief Returns object class probability car of the tracker object
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Class_Prob_Car(p_context, index) (Pa_Mock_Float(p_context, index, 0.0f))

/**
 * @brief Returns object class probability truck of the tracker object
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Class_Prob_Truck(p_context, index) (Pa_Mock_Float(p_context, index, 0.0f))

/**
 * @brief Returns id of object which was merged with object at index index
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Id_Merged_Obj(p_context, index) (Pa_Mock_Uint(p_context, index, (uint8_t) PA_INVALID_OBJ_ID))

/**
 * @brief Returns flag indicating whether a merge of objects has occured.
 *
 * @SDD{}
 */
#define Pa_Get_Obj_F_Merged_Occured(p_context, index) (Pa_Mock_Boolean(p_context, index, FBK_FALSE))

/**
 * @brief Returns which method was used to calculate the curvi coordinates.
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Curvi_Coordinates_Calc_Method(p_context, index) \
   (Pa_Mock_Curvi_Calc_Method(p_context, index, PA_OBJ_CURVI_COORDINATES_UNKNOWN))

/**
 * @brief Returns longitudinal curvi coordinates of object.
 * Caveat: Currently using vcs coordinates until curvi-coordinate component is ready.
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Curvi_Long_Posn(p_context, index) (Pa_Get_Obj_Vcs_Long_Pos(p_context, index))

/**
 * @brief Returns lateral curvi coordinates of object.
 * Caveat: Currently using vcs coordinates until curvi-coordinate component is ready.
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Curvi_Lat_Posn(p_context, index) (Pa_Get_Obj_Vcs_Lat_Pos(p_context, index))

/**
 * @brief Returns longitudinal curvi relative velocity of object.
 * Caveat: Currently using vcs coordinates until curvi-coordinate component is ready.
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Curvi_Long_Vel_Rel(p_context, index) (Pa_Get_Obj_Vcs_Long_Vel_Rel(p_context, index))

/**
 * @brief Returns longitudinal curvi velocity of object.
 * Caveat: Currently using vcs coordinates until curvi-coordinate component is ready.
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Curvi_Long_Vel(p_context, index) (Pa_Get_Obj_Vcs_Long_Vel(p_context, index))

/**
 * @brief Returns lateral curvi velocity of object.
 * Caveat: Currently using vcs coordinates until curvi-coordinate component is ready.
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Curvi_Lat_Vel(p_context, index) (Pa_Get_Obj_Vcs_Lat_Vel(p_context, index))

/**
 * @brief Returns lateral curvi relative velocity of object.
 * Caveat: Currently using vcs coordinates until curvi-coordinate component is ready.
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Curvi_Lat_Vel_Rel(p_context, index) (Pa_Get_Obj_Vcs_Lat_Vel_Rel(p_context, index))

/**
 * @brief Returns the heading of the object with given index in [rad]
 * Caveat: Currently using vcs coordinates until curvi-coordinate component is ready.
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Curvi_Heading(p_context, index) (Pa_Get_Obj_Heading(p_context, index))

/**
 * @brief Returns heading accuracy.
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Heading_Accuracy(p_context, index) (Pa_Mock_Float(p_context, index, 0.0f))

/**
 * @brief Returns flag indicating whether the object is stationary.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_F_Stationary(p_context, index) (Pa_Mock_Boolean(p_context, index, FBK_FALSE))

/**
 * @brief Returns whether front left sensor contributed detections to an object.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Is_Front_Left_Origin_Sensor(p_context, index) (Pa_Mock_Boolean(p_context, index, FBK_FALSE))

/**
 * @brief Returns whether the front right sensor contributed detections to an object.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Is_Front_Right_Origin_Sensor(p_context, index) (Pa_Mock_Boolean(p_context, index, FBK_FALSE))

/**
 * @brief Returns whether the rear left sensor contributed detections to an object.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Is_Rear_Left_Origin_Sensor(p_context, index) (Pa_Mock_Boolean(p_context, index, FBK_FALSE))
/**
 * @brief Returns whether the rear right sensor contributed detections to an object.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Is_Rear_Right_Origin_Sensor(p_context, index) (Pa_Mock_Boolean(p_context, index, FBK_FALSE))

/**
 * @brief Returns flag indicating whether the object is in the front left sensor fov, depending on look type.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Is_In_Front_Left_Sensor_Fov(p_context, index) (Pa_Mock_Boolean(p_context, index, FBK_TRUE))

/**
 * @brief Returns flag indicating whether the object is in the front right sensor fov, depending on look type.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Is_In_Front_Right_Sensor_Fov(p_context, index) (Pa_Mock_Boolean(p_context, index, FBK_TRUE))

/**
 * @brief Returns flag indicating whether the object is in the rear left sensor fov, depending on look type.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Is_In_Rear_Left_Sensor_Fov(p_context, index) (Pa_Mock_Boolean(p_context, index, FBK_TRUE))

/**
 * @brief Returns flag indicating whether the object is in the rear right sensor fov, depending on look type.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Is_In_Rear_Right_Sensor_Fov(p_context, index) (Pa_Mock_Boolean(p_context, index, FBK_TRUE))

/**
 * @brief Returns the minimum distance from border of host vehicle to border of object in [m]
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Distance(p_context, index) (Pa_Mock_Float(p_context, index, 0.0f))

/**
 * @brief Returns the FBK state age of the object with given index. Mapped to be the regular stage age.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define Pa_Get_Fbk_Obj_Stage_Age(p_context, index) (Pa_Mock_Uint(p_context, index, (uint8_t) 255u))

/**
 * @brief Returns flag indicating whether object with given index is moveable
 *
 * @SDD{}
 */
#define Pa_Get_Obj_F_Moveable(p_context, index) \
   ((boolean_T) Fbk_Is_True((p_context)->p_tracker_output->obj[(uint8_t) (index)].f_moveable))

/**
 * @brief Returns flag indicating whether the object is stationary clutter, mocked to be false.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_F_Stationary_Clutter(p_context, index) (Pa_Mock_Boolean(p_context, index, FBK_FALSE))

#endif
