#ifndef TA_INTERSECTION_ANALYZER_H
#define TA_INTERSECTION_ANALYZER_H

/**
 * @file ta_intersection_analyzer.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module implements logic for determining if two objects have a high probability of collision
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_traj_predictor_t.h"
#include "pa_reuse.h"
#include "ta_core_calibration_t.h"

/*============================================================================*\
 * EXPORTED FUNCTIONS PROTOTYPES
\*============================================================================*/

/**
 * @brief Checks if given waypoints plus safety margin overlap.
 *
 * Checks whether or not Euclidean distance between ego circle centers at given timestep
 * and currently considered object waypoint is smaller than the predefined threshold,
 * meaning the object is in a circle neighborhood small enough for high collision probability.
 *
 * Assumes that data structure waypoints_ego as well as waypoint_curr_obj are filled
 * with valid and relevant data.
 *
 * @return         true if   there is a high collision probability meaning:
 *                           the Euclidean distance between any of the representing circles
 *                           is smaller than the threshold (min_safe_distance) and
 *                 false if  there is a low collision probability meaning:
 *                           the Euclidean distance between any of the representing circles
 *                           is >= than the threshold (min_safe_distance)
 *
 * @SRS{SF-2314,SF-2313,SF-2360}
 * @SAE{SF-3238}
 * @SDD{SF-8723}
 * @verification{Create tests in which the euclidian distance between circles is tested. Only when any of them is below a given
 * threshold, true shall be returned.}
 */
boolean_T Ta_Is_Critical_Approach(const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */,
                                  const Fbk_Waypoint_with_Circle_Centers_T *p_ego /**< Ego waypoints */,
                                  const Fbk_Waypoint_with_Circle_Centers_T *p_obj /**< Object waypoints */);

#endif /* TA_INTERSECTION_ANALYZER_H */
