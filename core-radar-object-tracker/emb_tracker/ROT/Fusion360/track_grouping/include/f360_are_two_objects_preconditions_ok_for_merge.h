/*===================================================================================*\
* FILE: f360_are_two_objects_preconditions_ok_for_merge.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declaration of Are_Two_Objects_Preconditions_OK_For_Merge function.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/
#ifndef F360_ARE_TWO_OBJECTS_PRECONDITIONS_OK_FOR_MERGE_H
#define F360_ARE_TWO_OBJECTS_PRECONDITIONS_OK_FOR_MERGE_H

#include "f360_calibrations.h"
#include "f360_detection_props.h"
#include "f360_globals.h"
#include "f360_object_track.h"

namespace f360_variant_A
{
bool Are_Two_Objects_Preconditions_OK_For_Merge(const F360_Calibrations_T &calib, const F360_Object_Track_T &first_object,
                                                const F360_Object_Track_T &second_object,
                                                const F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
                                                float32_t &coarse_gate_score);

bool Are_Merging_Coarse_Gate_Conditions_Met(const F360_Object_Track_T &first_object, const F360_Object_Track_T &second_object,
                                            const F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
                                            const float32_t heading_gate, const float32_t speed_gate,
                                            const float32_t curvature_gate, const float32_t coarse_score_gate,
                                            float32_t &coarse_gate_score);

bool Merge_Metal_To_Metal_Test(const F360_Object_Track_T &first_object, const F360_Object_Track_T &second_object,
                               const F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
                               const F360_Calibrations_T &calib);

void Determine_Min_Max_Det_POS_in_Object_TCS(const Point &object1_center_vcs_position, const Angle &object1_vcs_pointing,
                                             const uint32_t obj_ndets, const uint32_t (&obj_detids)[MAX_DETS_IN_OBJ_TRK],
                                             const F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
                                             float32_t &min_para, float32_t &max_para, float32_t &min_orth, float32_t &max_orth,
                                             uint32_t &min_para_det_idx, uint32_t &max_para_det_idx);

bool Check_If_Detections_Support_Merge(const F360_Object_Track_T &obj1, const F360_Object_Track_T &obj2,
                                       const F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
                                       const float32_t k_max_allowed_para_gap_to_len_ratio,
                                       const float32_t k_max_allowed_orth_gap_to_width_ratio,
                                       const bool f_check_rr_straight_motion);

bool Are_RRs_Consistent_With_Low_Curvature(const F360_Object_Track_T &obj1, const F360_Object_Track_T &obj2,
                                           const F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
                                           const uint32_t min1_para_det_idx, const uint32_t max1_para_det_idx,
                                           const uint32_t min2_para_det_idx, const uint32_t max2_para_det_idx,
                                           const float32_t min1_para, const float32_t max1_para, const float32_t min2_para,
                                           const float32_t max2_para);
} // namespace f360_variant_A
#endif
