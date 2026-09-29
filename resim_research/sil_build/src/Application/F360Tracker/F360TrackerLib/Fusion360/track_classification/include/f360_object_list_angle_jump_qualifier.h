/*===================================================================================*\
* FILE:  f360_object_list_angle_jump_qualifier.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of Object_List_Angle_Jump_Qualifier() function and supportive functions.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
\*===================================================================================*/
#ifndef OBJECT_LIST_ANGLE_JUMP_QUALIFIER_H
#define OBJECT_LIST_ANGLE_JUMP_QUALIFIER_H

#include "f360_track_classification.h"
#include "f360_math.h"
#include "f360_math_func.h"
#include "f360_try_to_dealiase_range_rate.h"

namespace f360_variant_A
{

    void Object_List_Angle_Jump_Qualifier(
        const rspp_variant_A::RSPP_Detection_List_T& dets_raw,
        const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
        const F360_Host_T& host,
        const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
        F360_Tracker_Info_T& tracker_info);

    bool Find_Crossing_Objects(
        const F360_Tracker_Info_T& tracker_info,
        const uint16_t min_num_of_objects_for_clustering,
        uint16_t(&ids_of_crossing_objects)[NUMBER_OF_OBJECT_TRACKS],
        uint16_t& nr_crossing_objects);

    void Cluster_Crossing_Objects(
        const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
        const uint16_t min_num_of_objects_for_clustering,
        const uint16_t& nr_crossing_objects,
        const uint16_t(&ids_of_crossing_objects)[NUMBER_OF_OBJECT_TRACKS],
        uint16_t(&ids_of_clustered_crossing_objects)[NUMBER_OF_OBJECT_TRACKS],
        uint16_t& nr_clustered_crossing_objects);

    bool Satisfy_Cluster_Condition(
        const F360_Object_Track_T& last_object_in_cluster,
        const F360_Object_Track_T& current_object);

    void Count_Suspected_Stationary_Angle_Jumps_In_Objects(
        const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
        const rspp_variant_A::RSPP_Detection_List_T& dets_raw,
        const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
        const uint16_t(&ids_of_clustered_crossing_objects)[NUMBER_OF_OBJECT_TRACKS],
        const uint16_t nr_clustered_crossing_objects,
        uint16_t& nr_suspected_angle_jumps_in_clustered_objects);

    void Determine_Stationary_Angle_Jump_Signal(
        const uint16_t nr_suspected_angle_jumps_in_clustered_objects,
        F360_Tracker_Info_T& tracker_info);
}

#endif
