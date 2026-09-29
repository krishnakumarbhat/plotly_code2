# ifndef FBK_PUBLIC_CALIBRATION_T_H
# define FBK_PUBLIC_CALIBRATION_T_H

/**
* @file fbk_public_calibration_t.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in fbk_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "ct_calibration_header_t.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Defines
\*===========================================================================*/

/* Macros for all calibrations */
/* Macros for array sizes for all array variables */

/* Macros for dimension size for all array variables */


/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define FBK_PUBLIC_CALIBRATION_SIZE (24u)

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

#ifdef CT_BIG_ENDIAN
typedef struct
{
   /* Definition of structure for big endian */
   float32_T k_fbk_host_trail_heading_separation; /**<If the ego vehicle heading changed more than the given distance, then create new host trail point.*/
   float32_T k_fbk_host_trail_dist_separation; /**<If the ego vehicle moved farther than the given distance, then create new host trail point.*/
   float32_T k_fbk_host_trail_max_recording_speed; /**<If host speed is above threshold, do no longer create host trail.*/
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
} Fbk_Public_Calibration_T;
#else
typedef struct
{
   /* Definition of structure for little endian */
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
   float32_T k_fbk_host_trail_max_recording_speed; /**<If host speed is above threshold, do no longer create host trail.*/
   float32_T k_fbk_host_trail_dist_separation; /**<If the ego vehicle moved farther than the given distance, then create new host trail point.*/
   float32_T k_fbk_host_trail_heading_separation; /**<If the ego vehicle heading changed more than the given distance, then create new host trail point.*/
} Fbk_Public_Calibration_T;
#endif /* CT_BIG_ENDIAN */
#endif /* FBK_PUBLIC_CALIBRATION_T_H */
