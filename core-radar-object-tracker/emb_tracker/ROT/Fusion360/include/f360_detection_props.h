/*===================================================================================*\
* FILE: f360_detection_props.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* %full_filespec: %
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains F360_Detection_Props_T structure declaration
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): defineFusion360Types.m
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [May 26, 2019]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_DETECTION_PROPS_H
#define F360_DETECTION_PROPS_H

#include "f360_detection_motion_status.h"
#include "f360_detection_wheelspin_type.h"
#include "f360_look_type.h"
#include "f360_matrix_dimension.h"
#include "f360_point.h"
#include "f360_range_type.h"
#include "f360_reuse.h"

namespace f360_variant_A
{
/******************************************************************************
 * Name:  F360_Detection_Props_T
 *   Structure containing detection properties along with sorted reference detection indices.
 *
 * Struct Members:
 *   object_track_id           - identifier of associated tracked object - equal to (index_in_objects_array + 1);
 *                               0 - unassigned [-]
 *   vcs_position              - 2D position in Vehicle Coordinate System (VCS) [m]
 *   vcs_position_z            - z coordinate in Vehicle Coordinate System (VCS) [m],
 *                               Note: The zero plane is defined at ground level and negative above ground
 *   vcs_az                    - [rad] azimuth aligned with VCS (corrected by boresight angle in VCS)
 *   vcs_el                    - NOTE: This signal is to be compensated by elevation misaligment in future releases.
 *                               For now it is populated by the raw elevation angle but compensated by polarity [rad]
 *   cos_vcs_az                - [-] cosine of vcs azimuth
 *   sin_vcs_az                - [-] sine of vcs azimuth
 *   range_rate_dealiased      - range rate after dealiasing - compensate measurement limitations
 *                               (valid when f_dealiased flag is true) [m/s]
 *   range_dealiased           - range after dealiasing - equal to raw detection range for FMCW but compensated for SFW
 *                               (valid when f_dealiased flag is true) [m]
 *   range_rate_compensated    - range rate after host motion compensation (like Over-The-Ground range rate) [m/s]
 *   range_rate_predicted      - predicted range-rate based on associated target movement measured at the given range and
 *                               azimuth [m/s]
 *   next_sorted_idx           - index of the next sorted detection (with higher VCS longitudinal position) [-]
 *   prev_sorted_idx           - index of the previous sorted detection (with lower VCS longitudinal position) [-]
 *   cluster_id                - identifier of associated cluster - equal to (index_in_clusters_array + 1); 0 - unassigned [-]
 *   wheel_spin_type           - type of wheel spin reflection
 *                               (detected wheel spin should not be used for velocity estimation) [-]
 *   motion_status             - motion status of the detection (stationary, moving, etc.) [-]
 *   f_dealiased               - flag indicating that detection's range rate is dealiased [-]
 *   f_double_bounce           - flag indicating that detection is multi-bounce
 *                               (at the double range and range rate of other detection) (countermeasure) [-]
 *   f_FOV_edge                - flag indicating that detection on radar's field of view edge (countermeasure) [-]
 *   f_rr_inlier               - flag indicating that detection have been downselected to the associated object [-]
 *   f_used_in_rr_msmt_update  - flag indicating that detection was used in the range-rate measurement update [-]
 *   f_close_target            - flag indicating that detection is close to an object but not associated to it
 *                               (countermeasure) [-]
 *   f_inside_gate             - flag indicating that detection in object solid and extended gates [-]
 *   f_inside_mov_gate         - flag indicating that detection is inside moving gate [-]
 *   f_ok_to_use               - flag indicating that detection pass plausibility checks [-]
 *   f_det_pair                - flag for unassociated detections that were matched with another detection in same range and
 *                               range rate bin [-]
 *   f_potential_angle_jump    - Flag indicating that detection is a potential angle jump from guardrail
 *   f_object_based_angle_jump - Flag indicating that detection is a potential angle jump from object
 *   f_water_spray             - Flag indicating that detection potentially comes from water spray
 *   f_stationary_bounce       - Flag for detections which with high probablility comes from particular case of multpath,
 *                               called stationary bounce.
 *   f_azimuth_rdot_outlier    - Flag indicating that this detection is close in azimuth to another detection that fits the
 *                               objects linear velocity profile better
 *   f_trailer_related_det     - Flag indicating that this detection falls in the trailer bbox, is reflected by the trailer or
 *                               is occluded by the trailer.
 *   f_low_az_conf_det         - Flag indicating low azimuth confidence detection which is either suspected of angle jump from
 *                               stationary environment for SRR7+ and FLR7 or it is near host in a specifed ROI
 *   f_unreliable_in_clutter   - Flag indicating that the sensor is observing significant clutter and should be careful about
 *                               using unreliable detections.
 *   f_angle_amb               - Flag indicating that this detection is suspicious to be angle ambiguous
 *   f_rr_ambiguity            - Flag is used when range rate of detection is close to sensor's wraping interval, so it can't
 *                               be de-aliased with certainty. [-]
 *   f_nd_target               - Flag indicating that this detection comes from near target algorithm with lower threshold,
 *                               so it has more change to be noise [-].
 *   f_has_close_det_with_similar_rr - Flag indicating that this detection has a close neighbor with similar range rate
 *                                     (used to prevent false wheel spin marking) [-]
 *   behind_sep_id             - Id of highest prioritized Static Environment Polynomial that this detection is behind.
 *                               Detections flagged as "on" will never be "behind"
 *   on_sep_id                 - Id of highest prioritized Static Environment Polynomial which this detection is "on".
 *                               Detections flagged as "behind" will never be "on"
 ******************************************************************************/
typedef struct F360_Detection_Props_Tag
{
   int32_t object_track_id;
   Point vcs_position;
   float32_t vcs_position_z;
   float32_t vcs_az;
   float32_t vcs_el;
   float32_t cos_vcs_az;
   float32_t sin_vcs_az;
   float32_t range_rate_dealiased;
   float32_t range_dealiased;
   float32_t range_rate_compensated;
   float32_t range_rate_predicted;
   int16_t next_sorted_idx;
   int16_t prev_sorted_idx;
   int16_t cluster_id;
   F360_Detection_Wheelspin_Type_T wheel_spin_type;
   F360_Detection_Motion_Status_T motion_status;
   bool f_dealiased;
   bool f_double_bounce;
   bool f_FOV_edge;
   bool f_rr_inlier;
   bool f_used_in_rr_msmt_update;
   bool f_close_target;
   bool f_inside_gate;
   bool f_inside_mov_gate;
   bool f_ok_to_use;
   bool f_det_pair;
   bool f_potential_angle_jump;
   bool f_object_based_angle_jump;
   bool f_water_spray;
   bool f_stationary_bounce;
   bool f_azimuth_rdot_outlier;
   bool f_trailer_related_det;
   bool f_low_az_conf_det;
   bool f_unreliable_in_clutter;
   bool f_angle_amb;
   bool f_rr_ambiguity;
   bool f_nd_target;
   bool f_has_close_det_with_similar_rr;
   uint8_t behind_sep_id;
   uint8_t on_sep_id;
} F360_Detection_Props_T;

} // namespace f360_variant_A

#endif
