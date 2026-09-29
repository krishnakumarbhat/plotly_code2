#ifndef FBK_CIRCULAR_SHAPE_CALCULATOR_H
#define FBK_CIRCULAR_SHAPE_CALCULATOR_H

/**
 * @file fbk_circular_shape_calculator.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements functions related to calculating the circles
 * around the objects coordinates.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_traj_predictor_t.h"
#include "pa_reuse.h"

/*============================================================================*\
 * EXPORTED FUNCTIONS PROTOTYPES
\*============================================================================*/

/**
 * @brief Calculates the radius of the object circles at the given time step of the given object.
 *
 * Circle Radius = Base Radius * Base Gain
 *
 * If the Grow Gain is other than 1.0 then the Circle Radius is multiplied by the
 * grow gain for each prediction step.
 *
 * @return circle radius for the given prediction step
 *
 * @SRS{SF-2307}
 * @SAE{SF-3238}
 * @SDD{SF-4249}
 * @verification{Create tests with the following variations: 1.) grow_gain equals to 1.0f so that base_radius * base_gain is
 * expected. 2.) grow_gain is unequal to 1.0f and thus base_radius * base_gain * grow_gain^(prediction_steps) is expected.}
 */
float32_T Fbk_Get_Circle_Radius(const uint8_t prediction_step /**< current prediction step */,
                                const float32_T base_radius /**< base radius */,
                                const float32_T base_gain /**< base radius gain */,
                                const float32_T grow_gain /**< grow radius gain */);

/**
 * @brief Calculates the circle center offsets for the ego vehicle.
 *
 * Calculates the offsets of the 3 circle center positions (x axis) based on
 * the length and width of the ego.
 *
 * It is assumed that the "reference position" for the ego is the origin of the VCS.
 *
 * @return Circle center offsets for ego
 *
 * @SRS{SF-303}
 * @SAE{SF-2552}
 * @SDD{SF-4244}
 * @verification{Based on host_length and host_width inputs the following shall be returned: offset_front_x = -0.5 * host_width +
 * ego_circle_offset, offset_middle_x = -0.5 * host_length * ego_circle_host_length_factor + ego_circle_offset,
 * offset_rear_x = -host_length * ego_circle_host_length_factor - offset_front_x + ego_circle_offset}
 */
Fbk_Circle_Center_Offset_T Fbk_Calculate_Ego_Circle_Center_Offsets(const float32_T host_length /**< Host vehicle length */,
                                                                   const float32_T host_width /**< Host vehicle width */,
                                                                   const Fbk_Ego_Predict_Data_T *p_fbk_ego_data /**< Host data*/);

/**
 * @brief Calculates the circle center offsets for the object.
 *
 * Calculates the offsets of the 3 circle center positions (x axis) based on
 * the length and width of the object.
 *
 * It is assumed that the "reference position" for the object is its center point.
 *
 * @return Circle center offsets for object
 *
 * @SRS{}
 * @SAE{SF-2552}
 * @SDD{SF-4247}
 * @verification{Based on obj_length and obj_width inputs the following shall be returned: offset_front_x = ((obj_width / (-2.0f))
 * + (obj_length / 2.0f)), offset_middle_x = 0.0f, offset_rear_x = (circle_center_offset_result.offset_front_x * (-1.0f))}
 */
Fbk_Circle_Center_Offset_T Fbk_Calculate_Obj_Circle_Center_Offsets(const float32_T obj_length /**< Object length */,
                                                                   const float32_T obj_width /**< Object width */);

/**
 * @brief Fills the circle center coordinates.
 *
 * Calculates the positions of the center points of the 3 circles that
 * approximate the shape of the ego or object in VCS coordinate system, respectively.
 * The object, which is represented, gets a heading angle by the given angle psi
 *
 * @return void
 *

   p_result->circle_center_middle.x = (p_result->waypoint_coordinates.x + (offset_x_middle * yaw_angle.cos));
   p_result->circle_center_middle.y = (p_result->waypoint_coordinates.y + (offset_x_middle * yaw_angle.sin));

 * @SRS{SF-303}
 * @SAE{SF-2552}
 * @SDD{SF-4248}
 * @verification{For the given inputs circle centers for the positions front, middle and rear shall be returned in the following
 manner: circle_center_pos.x = (waypoint_coordinates.x + (offset_x_pos * cos(yaw_angle))), circle_center_pos.y =
 (waypoint_coordinates.y + (offset_x_pos * sin(yaw_angle)))}
 */
void Fbk_Fill_Circle_Center_Coordinates(Fbk_Waypoint_with_Circle_Centers_T *p_result /**< Waypoint circle result */,
                                        const float32_T offset_x_front /**< Front offset */,
                                        const float32_T offset_x_middle /**< Middle offset */,
                                        const float32_T offset_x_rear /**< Rear offset */);

/**
 * @brief Init of Fbk_Circle_Center_Offset_T structure
 *
 * @return void
 *
 * @SRS{SF-303}
 * @SAE{SF-2552}
 * @SDD{SF-4251}
 * @verification{Create a test to check that circle center offset properties are reset to their default.}
 */
void Fbk_Init_Circle_Center_Offset_Structure(Fbk_Circle_Center_Offset_T *p_circle_center_offset_structure /**< Circle offsets */);

/**
 * @brief Init of Fbk_Waypoint_with_Circle_Centers structure
 *
 * @return void
 *
 * @SRS{SF-303}
 * @SAE{SF-2552}
 * @SDD{SF-4250}
 * @verification{Create a test to check that circle center properties are reset to their default.}
 */
void Fbk_Init_Waypoint_with_Circle_Centers_Structure(
   Fbk_Waypoint_with_Circle_Centers_T *p_waypoint_with_circle_centers_structure /**< Waypoint data */);

#endif /* FBK_CIRCULAR_SHAPE_CALCULATOR_H */
