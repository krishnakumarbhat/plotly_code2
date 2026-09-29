#ifndef FBK_OBJECT_DATA_T_H
#define FBK_OBJECT_DATA_T_H

/**
 * @file fbk_object_data_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for fbk object type definition.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Include
\*===========================================================================*/

#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"

/*===========================================================================*\
* Typedef
\*===========================================================================*/

/**
 * @brief Fbk_Object_Data_T structure
 *
 * Summarizes the consumed tracker data of the Sfl stack.
 *
 */
typedef struct
{
   uint8_t id;             /**< ID of this object assigned for its lifetime */
   uint32_t unique_id;     /**< unique ID of object track */
   uint8_t index;          /**< index of this object in the input object array (IDs can change indeces in array over time) */
   Pa_Obj_Status_T status; /**< available values are INVALID, NEW, MATURE, COASTED and COASTED_IMPLAUSIBLE */
   uint8_t age;            /**< number of scans this object has existed */
   uint8_t stage_age;      /**< number of consecutive scans this object has had the same status */
   uint8_t fbk_stage_age;  /**< FBK-derived stage age */
   float32_T existence_probability; /**< [0,1] existence probability of the object */
   float32_T speed;                 /**< [m/s] Over ground speed of the object */

   Vector_2d_T vcs_pos;     /**< [m] position of the object bounding box center in vcs coordinates (origin at host front center).*/
   Vector_2d_T vcs_vel;     /**< [m/s] velocity of the object in vcs coordinates.*/
   Vector_2d_T vcs_vel_rel; /**< [m/s] relative velocity of the object in vcs coordinates.*/
   Vector_2d_T vcs_accel;   /**< [m/s^2] acceleration of the object in vcs coordinates.*/

   float32_T vcs_heading;      /**< [rad] [-pi, pi] heading of the object in vcs coordinates. Clockwise positive.*/
   float32_T heading_rate;     /**< [rad/s] heading rate of the object in vcs coordinates.*/
   float32_T heading_variance; /**< Variance of the heading value in vcs coordinates. */
   float32_T accuracy_heading; /**< The accuracy shall describe how close the tracked object is to the real object. */

   float32_T eclipse_value; /**< [0,1] eclipse value of object (= number of reference points eclipsed divided by 8).
                             * A point is said to be eclipsed if it lies in the shadow of another object */

   float32_T length; /**< [m] length of the target vehicle */
   float32_T width;  /**< [m] width of the target vehicle */

   float32_T obj_distance;     /**< [m] distance from the object closest reference point to host vehicle side rear corner.
                                * When object passed the host rear bumper the obj_distance becomes only lateral distance between     them.*/
   float32_T obstruction_prob; /**< [0,1] Obstruction probability. Probability that this object is an obstacle rather than
                                  relevant moving object. */

   Pa_Obj_Class_T obj_class;        /**< classification of the object based on target size */
   float32_T class_prob_pedestrian; /**< [0,1] probability that the object is a pedestrian */
   float32_T class_prob_2wheel;     /**< [0,1] probability that the object is a two-wheel */
   float32_T class_prob_car;        /**< [0,1] probability that the object is a car */
   float32_T class_prob_truck;      /**< [0,1] probability that the object is a truck */

   uint8_t id_merged_obj;     /**< ID of merged object (object that is not exisitng any more, but became part of this object) */
   boolean_T f_merge_occured; /**< Flag indicating that another object has been merged to this object in current cycle */

   Pa_Obj_Curvi_Calc_Method_T curvi_coordinates_calc_method; /**< Curvi coordinate calculation method */
   Vector_2d_T curvi_pos;                                    /**< [m] position of the object in curvi coordinates.*/
   Vector_2d_T curvi_vel;                                    /**< [m/s] velocity of the object in curvi coordinates.*/
   Vector_2d_T curvi_vel_rel;                                /**< [m/s] relative velocity of the object in curvi coordinates.*/
   float32_T curvi_heading;                                  /**< [rad] [-pi,pi] heading of the object in curvi coordinates.*/

   boolean_T f_reflection;         /**< Flag indicating that object is most likely a reflection */
   boolean_T f_stationary;         /**< Flag indicating that object is most likely stationary */
   boolean_T f_moveable;           /**< Flag indicating that object has moved at one point of time */
   boolean_T f_stationary_clutter; /**< Confidence that this object is stationary clutter */

   boolean_T f_is_fl_origin_sensor; /**< Flag indicating that object is seen by Front Left sensor */
   boolean_T f_is_fr_origin_sensor; /**< Flag indicating that object is seen by Front Right sensor */
   boolean_T f_is_rl_origin_sensor; /**< Flag indicating that object is seen by Rear Left sensor */
   boolean_T f_is_rr_origin_sensor; /**< Flag indicating that object is seen by Rear Right sensor */

   boolean_T f_is_in_fl_sensor_fov; /**< Flag indicating that object is in FoV of Front Left sensor */
   boolean_T f_is_in_fr_sensor_fov; /**< Flag indicating that object is in FoV of Front Right sensor */
   boolean_T f_is_in_rl_sensor_fov; /**< Flag indicating that object is in FoV of Rear Left sensor */
   boolean_T f_is_in_rr_sensor_fov; /**< Flag indicating that object is in FoV of Rear Right sensor */

} Fbk_Object_Data_T;

#endif
