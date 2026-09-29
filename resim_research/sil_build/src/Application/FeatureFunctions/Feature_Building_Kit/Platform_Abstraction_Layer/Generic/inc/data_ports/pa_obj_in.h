#ifndef PA_OBJ_IN_H
#define PA_OBJ_IN_H

/**
 * @file pa_obj_in.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file provides the PA tracker object macros for the generic interface
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*============================================================================*\
 * Includes
\*============================================================================*/

#include "pa_context.h"

/*============================================================================*\
* Function like macro
\*============================================================================*/

/**
 * @brief Returns the ID of the object with given index
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Id(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].id)

/**
 * @brief Returns the unique ID of the object with given index
 *
 * @SDD{}
 */
#define Pa_Get_Obj_Unique_Id(p_context, index) ((uint32_t) (p_context)->p_data->object_data[(uint8_t) (index)].unique_id)

/**
 * @brief Returns the status of the object with given index
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Status(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].status)

/**
 * @brief Returns the age of the object with given index
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Age(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].age)

/**
 * @brief Returns the state age of the object with given index
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Stage_Age(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].stage_age)

/**
 * @brief Returns the FBK state age of the object with given index. Mapped to be the regular stage age.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Fbk_Obj_Stage_Age(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].fbk_stage_age)

/**
 * @brief Returns the existence probability of the object with given index
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Exist_Prob(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].existence_probability)

/**
 * @brief Returns the absolute longitudinal position in vcs
 * of the object with given index in [m]
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Vcs_Long_Pos(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].vcs_pos.x)

/**
 * @brief Returns the longitudinal velocity in vcs
 * of the object with given index in [m/s]
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Vcs_Long_Vel(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].vcs_vel.x)

/**
 * @brief Returns the relative longitudinal velocity in vcs
 * of the object with given index in [m/s]
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Vcs_Long_Vel_Rel(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].vcs_vel_rel.x)

/**
 * @brief Returns the longitudinal acceleration in vcs
 * of the object with given index in [m/s^2]
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Vcs_Long_Accel(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].vcs_accel.x)

/**
 * @brief Returns the absolute lateral position in vcs
 * of the object with given index in [m]
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Vcs_Lat_Pos(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].vcs_pos.y)

/**
 * @brief Returns the absolute latitude velocity in vcs
 * of the object with given index in [m/s]
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Vcs_Lat_Vel(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].vcs_vel.y)

/**
 * @brief Returns the relative longitude velocity in vcs
 * of the object with given index in [m/s]
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Vcs_Lat_Vel_Rel(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].vcs_vel_rel.y)

/**
 * @brief Returns the lateral acceleration in vcs
 * of the object with given index in [m/s^2]
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Vcs_Lat_Accel(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].vcs_accel.y)

/**
 * @brief Returns the heading of the object with given index in [rad]
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Heading(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].vcs_heading)

/**
 * @brief Returns the heading rate of the object with given index in [rad/s]
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Heading_Rate(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].heading_rate)

/**
 * @brief Returns the heading variance of the object with given index in [rad^2]
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Heading_Variance(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].heading_variance)

/**
 * @brief Returns the speed of the object with given index in [m/s]
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Speed(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].speed)

/**
 * @brief Returns the heading of the object with given index
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Eclipse_Value(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].eclipse_value)

/**
 * @brief Returns the length of the object with given index in [m]
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Length(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].length)

/**
 * @brief Returns the length of the object with given index in [m]
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Width(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].width)

/**
 * @brief Returns the minimum distance from border of host vehicle to border of object in [m]
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Distance(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].obj_distance)

/**
 * @brief Returns the length of the object with given index
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Obstruction_Prob(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].obstruction_prob)

/**
 * @brief Returns flag indicating whether object with given index is a reflection
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Reflect_Flag(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].f_reflection)

/**
 * @brief Returns object class of the tracker object
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Class(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].obj_class)

/**
 * @brief Returns object class probability of the tracker object class pedestrian
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Class_Prob_Pedestrian(p_context, index) \
   ((p_context)->p_data->object_data[(uint8_t) (index)].class_prob_pedestrian)

/**
 * @brief Returns object class probability of the tracker object class 2wheel
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Class_Prob_2wheel(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].class_prob_2wheel)

/**
 * @brief Returns object class probability of the tracker object class car
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Class_Prob_Car(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].class_prob_car)

/**
 * @brief Returns object class probability of the tracker object class truck
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Class_Prob_Truck(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].class_prob_truck)

/**
 * @brief Returns id of object which was merged with object at index index
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Id_Merged_Obj(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].id_merged_obj)

/**
 * @brief Returns flag indicating whether a merge of objects has occured.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_F_Merged_Occured(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].f_merge_occured)

/**
 * @brief Returns timestamp difference in seconds compared to last cycle.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Time_Diff_To_Last_Cycle(p_context) ((p_context)->p_data->time_diff_to_last_cycle)

/**
 * @brief Returns which method was used to calculate the curvi coordinates.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Curvi_Coordinates_Calc_Method(p_context, index) \
   ((p_context)->p_data->object_data[(uint8_t) (index)].curvi_coordinates_calc_method)

/**
 * @brief Returns longitudinal curvi coordinates of object.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Curvi_Long_Posn(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].curvi_pos.x)

/**
 * @brief Returns lateral curvi coordinates of object.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Curvi_Lat_Posn(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].curvi_pos.y)

/**
 * @brief Returns longitudinal curvi relative velocity of object.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Curvi_Long_Vel_Rel(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].curvi_vel_rel.x)

/**
 * @brief Returns longitudinal curvi velocity of object.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Curvi_Long_Vel(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].curvi_vel.x)

/**
 * @brief Returns lateral curvi velocity of object.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Curvi_Lat_Vel(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].curvi_vel.y)

/**
 * @brief Returns lateral curvi relative velocity of object.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Curvi_Lat_Vel_Rel(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].curvi_vel_rel.y)

/**
 * @brief Returns the heading of the object with given index in [rad]
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Curvi_Heading(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].curvi_heading)

/**
 * @brief Returns heading accuracy.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Heading_Accuracy(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].accuracy_heading)

/**
 * @brief Returns whether front left sensor contributed detections to an object.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Is_Front_Left_Origin_Sensor(p_context, index) \
   ((p_context)->p_data->object_data[(uint8_t) (index)].f_is_fl_origin_sensor)

/**
 * @brief Returns whether the front right sensor contributed detections to an object.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Is_Front_Right_Origin_Sensor(p_context, index) \
   ((p_context)->p_data->object_data[(uint8_t) (index)].f_is_fr_origin_sensor)
/**
 * @brief Returns whether the rear left sensor contributed detections to an object.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Is_Rear_Left_Origin_Sensor(p_context, index) \
   ((p_context)->p_data->object_data[(uint8_t) (index)].f_is_rl_origin_sensor)
/**
 * @brief Returns whether the rear right sensor contributed detections to an object.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Is_Rear_Right_Origin_Sensor(p_context, index) \
   ((p_context)->p_data->object_data[(uint8_t) (index)].f_is_rr_origin_sensor)


/**
 * @brief Returns flag indicating whether the object is in the front left sensor fov, depending on look type.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Is_In_Front_Left_Sensor_Fov(p_context, index) \
   ((p_context)->p_data->object_data[(uint8_t) (index)].f_is_in_fl_sensor_fov)

/**
 * @brief Returns flag indicating whether the object is in the front right sensor fov, depending on look type.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Is_In_Front_Right_Sensor_Fov(p_context, index) \
   ((p_context)->p_data->object_data[(uint8_t) (index)].f_is_in_fr_sensor_fov)


/**
 * @brief Returns flag indicating whether the object is in the rear left sensor fov, depending on look type.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Is_In_Rear_Left_Sensor_Fov(p_context, index) \
   ((p_context)->p_data->object_data[(uint8_t) (index)].f_is_in_rl_sensor_fov)


/**
 * @brief Returns flag indicating whether the object is in the rear right sensor fov, depending on look type.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_Is_In_Rear_Right_Sensor_Fov(p_context, index) \
   ((p_context)->p_data->object_data[(uint8_t) (index)].f_is_in_rr_sensor_fov)

/**
 * @brief Returns flag indicating whether the object is stationary.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_F_Stationary(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].f_stationary)

/**
 * @brief Returns flag indicating whether the object is moveable.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_F_Moveable(p_context, index) ((p_context)->p_data->object_data[(uint8_t) (index)].f_moveable)

/**
 * @brief Returns flag indicating whether the object is stationary clutter.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Obj_F_Stationary_Clutter(p_context, index) \
   ((p_context)->p_data->object_data[(uint8_t) (index)].f_stationary_clutter)

#endif
