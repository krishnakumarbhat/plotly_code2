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

#include "f360_reuse.h"
#include "f360_point.h"
#include "rspp_detection_motion_status.h"
#include "f360_detection_wheelspin_type.h"
#include "f360_look_type.h"
#include "f360_range_type.h"
#include "f360_matrix_dimension.h"

namespace f360_variant_A
{
   typedef struct F360_Detection_Props_Tag
   {
      // Processed part: Populated inside (by) F360 Tracker
      Point vcs_position;                 // position in Vehicle Coordinate System (VCS) [m]
      int32_t object_track_id;       // identifier of associated tracked object - equal to (index_in_objects_array + 1); 0 - unassigned [-]
      float32_t range_rate_dealiased;    // range rate after dealiasing - compensate measurement limitations (valid when f_dealiased flag is true) [m/s]
      float32_t range_dealiased;         // range after dealiasing - equal to raw detection range for FMCW but compensated for SFW (valid when f_dealiased flag is true) [m]
      float32_t range_rate_compensated;  // range rate after host motion compensation (like Over-The-Ground range rate) [m/s]
      float32_t range_rate_predicted;    // predicted range-rate based on associated target movement measured at the given range and azimuth [m/s]
      int16_t cluster_id;            // identifier of associated cluster - equal to (index_in_clusters_array + 1); 0 - unassigned [-]
      F360_Detection_Wheelspin_Type_T wheel_spin_type;  // type of wheel spin reflection (detected wheel spin should not be used for velocity estimation) [-]
      rspp_variant_A::RSPP_Detection_Motion_Status_T motion_status;
      bool f_dealiased;           // flag indicating that detection's range rate is dealiased [-]
      bool f_double_bounce;       // flag indicating that detection is multi-bounce (at the double range and range rate of other detection) (countermeasure) [-]
      bool f_FOV_edge;            // flag indicating that detection on radar's field of view edge (countermeasure) [-]
      bool f_rr_inlier;     // flag indicating that detection have been downselected to the associated object [-]
      bool f_used_in_rr_msmt_update; // flag indicating that detection was used in the range-rate measurement update [-]
      bool f_close_target;        // flag indicating that detection is close to an object but not associated to it (countermeasure) [-]
      bool f_inside_gate;         // flag indicating that detection in object solid and extended gates [-]
      bool f_inside_mov_gate;     // flag indicating that detection is inside moving gate [-]
      bool f_ok_to_use;           // flag indicating that detection pass plausibility checks [-]
      bool f_det_pair;            // flag for unassociated detections that were matched with another detection in same range and range rate bin [-]
      bool f_potential_angle_jump; // Flag indicating that detection is a potential angle jump from guardrail
      bool f_object_based_angle_jump; // Flag indicating that detection is a potential angle jump from object
      bool f_water_spray; // Flag indicating that detection potentially comes from water spray
      bool f_stationary_bounce; // Flag for detections which with high probablility comes from particular case of multpath, called stationary bounce.
      bool f_azimuth_rdot_outlier; // Flag indicating that this detection is close in azimuth to another detection that fits the objects linear velocity profile better
      bool f_trailer_related_det; // Flag indicating that this detection falls in the trailer bbox, is reflected by the trailer or is occluded by the trailer.
      bool f_low_az_conf_det;    // Flag indicating low azimuth confidence detection which is either suspected of angle jump from stationary environment for SRR7+ and FLR7 or it is near host in a specifed ROI
      bool f_unreliable_in_clutter; // Flag indicating that the sensor is observing significant clutter and should be careful about using unreliable detections.
      bool f_angle_amb;             // Flag indicating that this detection is suspicious to be angle ambiguous 
      bool f_rr_ambiguity;          // Flag is used when range rate of detection is close to sensor's wraping interval, so it can't be de-aliased with certainty. [-]
      bool f_nd_target;             // Flag indicating that this detection comes from near target algorithm with lower threshold, so it has more change to be noise [-].
      bool f_has_close_det_with_similar_rr; // Flag indicating that this detection has a close neighbor with similar range rate (used to prevent false wheel spin marking) [-]
      uint8_t behind_sep_id; // Id of highest prioritized Static Environment Polynomial that this detection is behind. Detections flagged as "on" will never be "behind"
      uint8_t on_sep_id; // Id of highest prioritized Static Environment Polynomial which this detection is "on". Detections flagged as "behind" will never be "on"
   } F360_Detection_Props_T;

}
#endif
