#ifndef FBK_HOST_LANE_H
#define FBK_HOST_LANE_H

/**
 * @file fbk_host_lane.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains exported routines for creation of trail information and tranformation of those into path information.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_core_calibration_t.h"
#include "fbk_iface_types.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Global Functions Declaration
\*===========================================================================*/


/**
 * @brief Builds the host lane trail, which is later used for path creation.
 *
 * @return void.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4166}
 * @verification{}
 */
void Fbk_Update_Host_Trail(Fbk_Host_Trail_T *p_host_trail /**< host trail structure of FBK*/,
                           const Pa_Data_T *p_pa_data /**< context data of path tracking*/,
                           const Fbk_Core_Calibration_T *p_cals /**< fbk calibration values*/);

/**
 * @brief Initializes the host trail structure with default values.
 *
 * @return void.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4165}
 * @verification{}
 */
void Fbk_Init_Host_Trail(Fbk_Host_Trail_T *p_host_trail /**< host trail structure of FBK*/);

/**
 * @brief Checks if the host trail is empty.
 *
 * @return True if the host trail is empty.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4167}
 * @verification{}
 */
boolean_T Fbk_Is_Host_Trail_Empty(const Fbk_Host_Trail_T *p_host_trail);

/**
 * @brief Transforms trail components from wcs to vcs based on the absolute host heading in wcs and position of the host in wcs.
 *
 * @return void.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4168}
 * @verification{}
 */
void Fbk_Init_Trail_Vcs(
   Vector_2d_T trail_vcs[FBK_NUM_HOST_TRAIL_POINTS] /**< structure in which the transformed host trail is stored*/,
   const Fbk_Host_Trail_T *p_host_trail /**< host trail structure of PT*/,
   const float32_T rear_axle_position /**< rear axle position in meter */);

/**
 * @brief Returns point indices of the trail points for comparison. Those are dependent on the structure of the buffer.
 * This means, that the index will be limited to the total amount of host path points and will be transformed to a value in this
 * range when overflowing.
 *
 * @return void.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4169}
 * @verification{}
 */
void Fbk_Get_Point_Indices(Fbk_Point_Pair_T *p_index_pair /**< host trail structure of PT*/,
                           const Fbk_Host_Lane_Interval_T *p_interval /**< interval for path transformation*/,
                           const uint8_t trail_point_ctr /**< host trail structure of PT*/);

#endif /* FBK_HOST_LANE_H */
