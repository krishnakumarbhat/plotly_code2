/** \file
 *  This file contains unit tests that verifies the content of f360_rot_object_output.cpp
 */

#include <CppUTest/TestHarness.h>
#include "f360_rot_object_output.h"
#include "f360_mounting_location.h"
#include "f360_constants.h"
#include "f360_functional_safety_faults_log.h"

using namespace f360_variant_A;

/** \defgroup  f360_rot_object_output
 *  @{
 */

/** \brief
 * Test group for testing if Set_ROT_Object_Output fill in the output object expectely.
 */
TEST_GROUP(f360_rot_object_output)
{
    ROT_Object_Output_T rot_obj_output[NUMBER_OF_REDUCED_OBJECT_TRACKS] = {};
    F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
    F360_Host_T host_info = {};
    rspp_variant_A::RSPP_Detection_List_T det_list = {};
    F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
    uint16_t front_mid_sensor = 0x0U;
    uint16_t front_left_sensor = 0x0U;
    uint16_t left_side_sensor = 0x0U;
    uint16_t rear_left_sensor = 0x0U;
    uint16_t rear_mid_sensor = 0x0U;
    uint16_t rear_right_sensor = 0x0U;
    uint16_t right_side_sensor = 0x0U;
    uint16_t front_right_sensor = 0x0U;
    TEST_SETUP()
    {
        front_mid_sensor = 0x0001U;
        front_left_sensor = 0x0002U;
        left_side_sensor = 0x0004U;
        rear_left_sensor = 0x0008U;
        rear_mid_sensor = 0x0010U;
        rear_right_sensor = 0x0020U;
        right_side_sensor = 0x0040U;
        front_right_sensor = 0x0080U;
        object_tracks[0].reduced_id = 1;
        object_tracks[1].reduced_id = 2;
        object_tracks[2].reduced_id = 3;
        object_tracks[3].reduced_id = 4;
        object_tracks[4].reduced_id = 5;
        object_tracks[5].reduced_id = 6;
        object_tracks[6].reduced_id = 7;
        object_tracks[7].reduced_id = 8;
        object_tracks[8].reduced_id = 9;
        object_tracks[9].reduced_id = 10;
        object_tracks[10].reduced_id = 11;
        sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD;
        sensors[1].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;
        sensors[2].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_SIDE1;
        sensors[3].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;
        sensors[4].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_REAR;
        sensors[5].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;
        sensors[6].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_SIDE1;
        sensors[7].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;
    }
};

/** \purpose
 * Check if rot_object_output convertes object to ISO coordiante correctly .
 * \req
 * NA.
 */
TEST(f360_rot_object_output, object_track_convert_to_rot_interface)
{
    /** \precond
     * define two objects in object_tracks needed properties for this test group.
     */
    object_tracks[0].reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
    object_tracks[0].vcs_position.Set_Position(1.0F,1.0F);
    object_tracks[0].vcs_velocity.lateral = 2.0F;
    object_tracks[0].vcs_velocity.longitudinal = 2.0F;
    object_tracks[0].vcs_accel.lateral = 0.5F;
    object_tracks[0].vcs_accel.longitudinal = 0.5F;
    object_tracks[0].bbox.Set_Length(6.0F);
    object_tracks[0].length_processed = 6.0F;
    object_tracks[0].length_uncertainty = 0.5F;
    object_tracks[0].bbox.Set_Width(1.8F);
    object_tracks[0].width_processed = 1.8F;
    object_tracks[0].bbox.Set_Orientation(1.0F);
    object_tracks[0].width_uncertainty = 0.3F;
    object_tracks[0].id = (int32_t)1;
    object_tracks[0].unique_id = (uint32_t)2000;
    object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
    object_tracks[0].detids[0] = 5;
    const int16_t det_id = object_tracks[0].detids[0];
    object_tracks[0].ndets = 1;
    object_tracks[0].time_since_initialization = 5.0F;
    object_tracks[0].time_since_downselected = 2.03F;
    object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
    object_tracks[0].exist_prob = 1.0;
    object_tracks[0].f_moveable = true;
    object_tracks[0].movable_prob = 1.0F;
    object_tracks[0].f_moving = true;
    object_tracks[0].curvature = 0.0F;
    object_tracks[0].heading_rate = 0.0F;
    object_tracks[0].tang_accel = 0.0F;
    object_tracks[0].average_rcs = 2.0F; // 2db
    object_tracks[0].underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
    object_tracks[0].drivable_status_sg = sg::SG_Drivability_Class_T::NONDRIVABLE;
    det_list.detections[det_id - 1].raw.sensor_id = 1U;

    object_tracks[1].reduced_id=0U; //set index 1 object invalid

    object_tracks[2].reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
    object_tracks[2].vcs_position.Set_Position(2.0F,2.0F);
    object_tracks[2].vcs_velocity.lateral = 3.0F;
    object_tracks[2].vcs_velocity.longitudinal = 3.0F;
    object_tracks[2].vcs_accel.lateral = 1.5F;
    object_tracks[2].vcs_accel.longitudinal = 1.5F;
    object_tracks[2].bbox.Set_Length(4.8F);
    object_tracks[2].length_processed = 4.8F;
    object_tracks[2].bbox.Set_Width(1.5F);
    object_tracks[2].width_processed = 1.5F;
    object_tracks[2].id = (int32_t)120;
    object_tracks[2].unique_id = (uint32_t)3000;
    object_tracks[2].trk_fltr_type = F360_TRACKER_TRKFLTR_CCV;
    object_tracks[2].detids[0] = 7;
    object_tracks[2].detids[1] = 8;
    const int16_t det_id_2 = object_tracks[2].detids[0];
    const int16_t det_id_3 = object_tracks[2].detids[1];
    object_tracks[2].ndets = 2;
    object_tracks[2].time_since_initialization = 6.0F;
    object_tracks[2].time_since_downselected = 5.03F;
    object_tracks[2].status = F360_OBJECT_STATUS_UPDATED;
    object_tracks[2].exist_prob = 0.5F;
    object_tracks[2].movable_prob = 1.0F;
    object_tracks[2].f_moveable = true;
    object_tracks[2].f_moving = false;
    object_tracks[2].curvature = 0.0F;
    object_tracks[2].heading_rate = 0.0F;
    object_tracks[2].tang_accel = 0.0F;
    object_tracks[2].underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER;
    object_tracks[2].drivable_status_sg = sg::SG_Drivability_Class_T::OVERDRIVABLE;
    det_list.detections[det_id_2 - 1].raw.sensor_id = 1U;
    det_list.detections[det_id_3 - 1].raw.sensor_id = 2U;
    host_info.dist_rear_axle_to_vcs_m = 3.5F;

    object_tracks[3].movable_prob=0.0;
    object_tracks[3].f_moving=false; 
    object_tracks[3].underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
    object_tracks[3].drivable_status_sg = sg::SG_Drivability_Class_T::UNDERDRIVABLE;

    for (int i = 0; i < STATE_DIMENSION; ++i)
    {
        for (int j = 0; j < STATE_DIMENSION; ++j)
        {
            object_tracks[0].errcov[i][j] = 0.1F;
            object_tracks[2].errcov[i][j] = 0.1F;
        }
    }

    /** \action
     * Call Set_ROT_Object_Output to fill ROT output
     **/
    Set_ROT_Object_Output(host_info, sensors, det_list, object_tracks, rot_obj_output);
    bool obj1_has_front_sensor = (rot_obj_output[0].sensor_src & front_mid_sensor) > 0U;
    bool obj3_has_front_sensor = (rot_obj_output[2].sensor_src & front_mid_sensor) > 0U;
    bool obj3_has_front_left_sensor = (rot_obj_output[2].sensor_src & front_left_sensor) > 0U;

    /** \result
     * Ensure that position and speed is correctly transformed, and object size, sensor source is set correctly
     **/
    CHECK_TRUE(obj1_has_front_sensor);
    CHECK_TRUE(obj3_has_front_sensor);
    CHECK_TRUE(obj3_has_front_left_sensor);

    DOUBLES_EQUAL(rot_obj_output[0].iso_x_posn, 4.5F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].iso_y_posn, -1.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].iso_x_vel, 2.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].iso_y_vel, -2.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].iso_x_acc, 0.5F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].iso_y_acc, -0.5F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].width, 1.8F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].length, 6.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].width_var, 0.3F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].length_var, 0.5F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].iso_orientation, -1.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].time_since_published, 2.03F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].time_since_created, 5.00F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].existence_probability, 1.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].radar_cross_section, 2.0F, 1e-4F);
    CHECK_EQUAL(rot_obj_output[0].id, 1);
    CHECK_EQUAL(rot_obj_output[0].unique_id, 2000);
    CHECK_EQUAL(rot_obj_output[0].movement_status, 3);
 

    DOUBLES_EQUAL(rot_obj_output[1].iso_x_posn, 0.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[1].iso_y_posn, 0.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[1].iso_x_vel, 0.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[1].iso_y_vel, 0.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[1].iso_x_acc, 0.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[1].iso_y_acc, 0.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[1].width, 0.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[1].length, 0.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[1].time_since_published, 0.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[1].time_since_created, 0.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[1].existence_probability, 0.0F, 1e-4F);
    CHECK_EQUAL(rot_obj_output[1].id, 0);
    CHECK_EQUAL(rot_obj_output[1].unique_id, 0);
    CHECK_EQUAL(rot_obj_output[1].movement_status, 0);

    DOUBLES_EQUAL(rot_obj_output[2].iso_x_posn, 5.5F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[2].iso_y_posn, -2.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[2].iso_x_vel, 3.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[2].iso_y_vel, -3.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[2].iso_x_acc, 1.5F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[2].iso_y_acc, -1.5F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[2].width, 1.5F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[2].length, 4.8F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[2].time_since_published, 5.03F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[2].time_since_created, 6.00F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[2].existence_probability, 0.5F, 1e-4F);
    CHECK_EQUAL(rot_obj_output[2].id, 120);
    CHECK_EQUAL(rot_obj_output[2].unique_id, 3000);
    CHECK_EQUAL(rot_obj_output[2].movement_status, 2);
    CHECK_EQUAL(rot_obj_output[3].movement_status, 1);
}

/** \purpose
 * Check if rot_object_output is filled from F360_Object_Track_T for CTCA object when object is not reversing (speed is positive).
 * \req
 * NA.
 */
TEST(f360_rot_object_output, rot_object_filled_from_F360_Object_Track_T__CTCA_Object__Pos_Speed)
{
    /** \precond
     * define one object in object_tracks
     */
    object_tracks[0].reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
    object_tracks[0].vcs_position.Set_Position(1.0F,1.0F);
    object_tracks[0].vcs_velocity.lateral = 2.0F;
    object_tracks[0].vcs_velocity.longitudinal = 2.0F;
    object_tracks[0].vcs_accel.lateral = 0.5F;
    object_tracks[0].vcs_accel.longitudinal = 0.5F;
    object_tracks[0].bbox.Set_Length(6.0F);
    object_tracks[0].length_uncertainty = 0.5F;
    object_tracks[0].bbox.Set_Width(1.8F);
    object_tracks[0].bbox.Set_Orientation(1.0F);
    object_tracks[0].width_uncertainty = 0.3F;
    object_tracks[0].id = (int32_t)1;
    object_tracks[0].unique_id = (uint32_t)2000;
    object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
    object_tracks[0].detids[0] = 5;
    const int16_t det_id = object_tracks[0].detids[0];
    object_tracks[0].ndets = 1;
    object_tracks[0].num_dets_used_in_rr_msmt_update = 1;
    object_tracks[0].time_since_initialization = 5.0F;
    object_tracks[0].time_since_downselected = 2.03F;
    object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
    object_tracks[0].exist_prob = 1.0;
    object_tracks[0].movable_prob = 1.0F;
    object_tracks[0].f_moving = true;
    det_list.detections[det_id - 1].raw.sensor_id = 1U;

    for (int i = 0; i < STATE_DIMENSION; ++i)
    {
        for (int j = 0; j < STATE_DIMENSION; ++j)
        {
            object_tracks[0].errcov[i][j] = 0.1F;

        }
    }
    object_tracks[0].vcs_heading = Angle(0.2F);
    object_tracks[0].curvature = 0.5F;
    object_tracks[0].speed = 5.0F;
    object_tracks[0].heading_rate = object_tracks[0].speed * object_tracks[0].curvature;
    object_tracks[0].tang_accel = 2.0F ;
    object_tracks[0].confidenceLevel = 0.9;
    object_tracks[0].time_since_stage_start = 5;
    object_tracks[0].probability_pedestrian= 0.01;
    object_tracks[0].probability_car= 0.9;
    object_tracks[0].probability_bicycle= 0.01;
    object_tracks[0].probability_motorcycle= 0.02;
    object_tracks[0].probability_truck= 0.07;
    object_tracks[0].probability_undet= 0.0F;
    object_tracks[0].on_sep_id = 0;

    /** \action
     * Call Set_ROT_Object_Output to fill ROT output
     **/
    Set_ROT_Object_Output(host_info, sensors, det_list, object_tracks, rot_obj_output);


    /** \result
     * Ensure that rot object output is filled from the F360_Object_Track_T
     **/

    DOUBLES_EQUAL(rot_obj_output[0].vcs_x_posn, 1.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_y_posn, 1.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_x_vel, -9.40638F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_y_vel, 11.99784F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_x_acc, -29.05716F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_y_acc, -24.01682F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_heading, 2.23571F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_pointing, 1.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_curvature, 0.16398F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].speed, 15.24559F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].tang_accel, -0.97256F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].confidence_level, 0.9F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].probability_pedestrian, 0.01F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].probability_car, 0.9F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].probability_bicycle, 0.01F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].probability_motorcycle, 0.02F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].probability_truck, 0.07F, 1e-4F);
    CHECK_EQUAL(rot_obj_output[0].ndets, 1);
    CHECK_EQUAL(rot_obj_output[0].num_dets_used_in_rr_msmt_update, 1);
    CHECK_EQUAL(rot_obj_output[0].trk_fltr_type, F360_TRACKER_TRKFLTR_CTCA);
    CHECK_EQUAL(rot_obj_output[0].f_onguardrail, 0);

    for (int i = 0; i < STATE_DIMENSION; ++i)
    {

    DOUBLES_EQUAL(rot_obj_output[0].vcs_state_variance[i] , 0.1F, 1e-4F); 

    }
    for (int i = 0; i < 3; ++i)
    {

    DOUBLES_EQUAL(rot_obj_output[0].vcs_supplemental_state_covariance[i] , 0.1F, 1e-4F); 

    }
}


/** \purpose
 * Check if rot_object_output is filled from F360_Object_Track_T for CTCA object when object is reversing (speed is negative).
 * \req
 * NA.
 */
TEST(f360_rot_object_output, rot_object_filled_from_F360_Object_Track_T__CTCA_Object__Neg_Speed)
{
    /** \precond
     * define one object in object_tracks
     */
    object_tracks[0].reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
    object_tracks[0].vcs_position.Set_Position(1.0F,1.0F);
    object_tracks[0].vcs_velocity.lateral = 2.0F;
    object_tracks[0].vcs_velocity.longitudinal = 2.0F;
    object_tracks[0].vcs_accel.lateral = 0.5F;
    object_tracks[0].vcs_accel.longitudinal = 0.5F;
    object_tracks[0].bbox.Set_Length(6.0F);
    object_tracks[0].length_uncertainty = 0.5F;
    object_tracks[0].bbox.Set_Width(1.8F);
    object_tracks[0].bbox.Set_Orientation(1.0F);
    object_tracks[0].width_uncertainty = 0.3F;
    object_tracks[0].id = (int32_t)1;
    object_tracks[0].unique_id = (uint32_t)2000;
    object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
    object_tracks[0].detids[0] = 5;
    const int16_t det_id = object_tracks[0].detids[0];
    object_tracks[0].ndets = 1;
    object_tracks[0].num_dets_used_in_rr_msmt_update = 1;
    object_tracks[0].time_since_initialization = 5.0F;
    object_tracks[0].time_since_downselected = 2.03F;
    object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
    object_tracks[0].exist_prob = 1.0;
    object_tracks[0].movable_prob = 1.0F;
    object_tracks[0].f_moving = true;
    det_list.detections[det_id - 1].raw.sensor_id = 1U;

    for (int i = 0; i < STATE_DIMENSION; ++i)
    {
        for (int j = 0; j < STATE_DIMENSION; ++j)
        {
            object_tracks[0].errcov[i][j] = 0.1F;

        }
    }
    object_tracks[0].vcs_heading = Angle(0.2F + F360_PI);
    object_tracks[0].curvature = -0.5F;
    object_tracks[0].speed = -5.0F;
    object_tracks[0].heading_rate = object_tracks[0].speed * object_tracks[0].curvature;
    object_tracks[0].tang_accel = -2.0F ;
    object_tracks[0].confidenceLevel = 0.9;
    object_tracks[0].time_since_stage_start = 5;
    object_tracks[0].probability_pedestrian= 0.01;
    object_tracks[0].probability_car= 0.9;
    object_tracks[0].probability_bicycle= 0.01;
    object_tracks[0].probability_motorcycle= 0.02;
    object_tracks[0].probability_truck= 0.07;
    object_tracks[0].probability_undet= 0.0F;
    object_tracks[0].on_sep_id = 0;

    /** \action
     * Call Set_ROT_Object_Output to fill ROT output
     **/
    Set_ROT_Object_Output(host_info, sensors, det_list, object_tracks, rot_obj_output);


    /** \result
     * Ensure that rot object output is filled from the F360_Object_Track_T
     **/
    DOUBLES_EQUAL(rot_obj_output[0].vcs_x_posn, 1.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_y_posn, 1.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_x_vel, -9.40638F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_y_vel, 11.99784F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_x_acc, -29.05716F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_y_acc, -24.01682F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_heading, -0.90588F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_pointing, 1.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_curvature, -0.16398F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].speed, -15.24559F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].tang_accel, 0.97256F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].confidence_level, 0.9F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].probability_pedestrian, 0.01F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].probability_car, 0.9F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].probability_bicycle, 0.01F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].probability_motorcycle, 0.02F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].probability_truck, 0.07F, 1e-4F);
    CHECK_EQUAL(rot_obj_output[0].ndets, 1);
    CHECK_EQUAL(rot_obj_output[0].num_dets_used_in_rr_msmt_update, 1);
    CHECK_EQUAL(rot_obj_output[0].trk_fltr_type, F360_TRACKER_TRKFLTR_CTCA);
    CHECK_EQUAL(rot_obj_output[0].f_onguardrail, 0);

    for (int i = 0; i < STATE_DIMENSION; ++i)
    {

    DOUBLES_EQUAL(rot_obj_output[0].vcs_state_variance[i] , 0.1F, 1e-4F); 

    }
    for (int i = 0; i < 3; ++i)
    {

    DOUBLES_EQUAL(rot_obj_output[0].vcs_supplemental_state_covariance[i] , 0.1F, 1e-4F); 

    }
}


/** \purpose
 * Check if rot_object_output is filled from F360_Object_Track_T for CTCA object when object speed is zero.
 * \req
 * NA.
 */
TEST(f360_rot_object_output, rot_object_filled_from_F360_Object_Track_T__CTCA_Object__Zero_Speed)
{
       /** \precond
     * define one object in object_tracks
     */
    object_tracks[0].reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
    object_tracks[0].vcs_position.Set_Position(1.0F,1.0F);
    object_tracks[0].vcs_velocity.lateral = 0.0F;
    object_tracks[0].vcs_velocity.longitudinal = 0.0F;
    object_tracks[0].vcs_accel.lateral = 0.5F;
    object_tracks[0].vcs_accel.longitudinal = 0.5F;
    object_tracks[0].bbox.Set_Length(6.0F);
    object_tracks[0].length_uncertainty = 0.5F;
    object_tracks[0].bbox.Set_Width(1.8F);
    object_tracks[0].bbox.Set_Orientation(1.0F);
    object_tracks[0].width_uncertainty = 0.3F;
    object_tracks[0].id = (int32_t)1;
    object_tracks[0].unique_id = (uint32_t)2000;
    object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
    object_tracks[0].detids[0] = 5;
    const int16_t det_id = object_tracks[0].detids[0];
    object_tracks[0].ndets = 1;
    object_tracks[0].num_dets_used_in_rr_msmt_update = 1;
    object_tracks[0].time_since_initialization = 5.0F;
    object_tracks[0].time_since_downselected = 2.03F;
    object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
    object_tracks[0].exist_prob = 1.0;
    object_tracks[0].movable_prob = 1.0F;
    object_tracks[0].f_moving = true;
    det_list.detections[det_id - 1].raw.sensor_id = 1U;

    for (int i = 0; i < STATE_DIMENSION; ++i)
    {
        for (int j = 0; j < STATE_DIMENSION; ++j)
        {
            object_tracks[0].errcov[i][j] = 0.1F;

        }
    }
    object_tracks[0].vcs_heading = Angle(0.2F);
    object_tracks[0].curvature = 0.5F;
    object_tracks[0].speed = 0.0F;
    object_tracks[0].heading_rate = object_tracks[0].speed * object_tracks[0].curvature;
    object_tracks[0].tang_accel = 2.0F ;
    object_tracks[0].confidenceLevel = 0.9;
    object_tracks[0].time_since_stage_start = 5;
    object_tracks[0].probability_pedestrian= 0.01;
    object_tracks[0].probability_car= 0.9;
    object_tracks[0].probability_bicycle= 0.01;
    object_tracks[0].probability_motorcycle= 0.02;
    object_tracks[0].probability_truck= 0.07;
    object_tracks[0].probability_undet= 0.0F;
    object_tracks[0].on_sep_id = 0;

    /** \action
     * Call Set_ROT_Object_Output to fill ROT output
     **/
    Set_ROT_Object_Output(host_info, sensors, det_list, object_tracks, rot_obj_output);


    /** \result
     * Ensure that rot object output is filled from the F360_Object_Track_T
     **/

    DOUBLES_EQUAL(rot_obj_output[0].vcs_x_posn, 1.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_y_posn, 1.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_x_vel, 0.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_y_vel, 0.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_x_acc, -4.06255F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_y_acc, 4.49914F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_heading, 0.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_pointing, 1.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_curvature, 0.5F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].speed, 0.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].tang_accel, -4.06255F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].confidence_level, 0.9F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].probability_pedestrian, 0.01F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].probability_car, 0.9F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].probability_bicycle, 0.01F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].probability_motorcycle, 0.02F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].probability_truck, 0.07F, 1e-4F);
    CHECK_EQUAL(rot_obj_output[0].ndets, 1);
    CHECK_EQUAL(rot_obj_output[0].num_dets_used_in_rr_msmt_update, 1);
    CHECK_EQUAL(rot_obj_output[0].trk_fltr_type, F360_TRACKER_TRKFLTR_CTCA);
    CHECK_EQUAL(rot_obj_output[0].f_onguardrail, 0);

    for (int i = 0; i < STATE_DIMENSION; ++i)
    {

    DOUBLES_EQUAL(rot_obj_output[0].vcs_state_variance[i] , 0.1F, 1e-4F); 

    }
    for (int i = 0; i < 3; ++i)
    {

    DOUBLES_EQUAL(rot_obj_output[0].vcs_supplemental_state_covariance[i] , 0.1F, 1e-4F); 

    }
}


/** \purpose
 * Check if rot_object_output is filled with the last reduced_id
 * \req
 * NA.
 */
TEST(f360_rot_object_output, rot_object_filled_with_last_reduced_id)
{
    /** \precond
     * define one object in object_tracks with the reduced_id = NUMBER_OF_REDUCED_OBJECT_TRACKS
     */
    object_tracks[0].reduced_id = NUMBER_OF_REDUCED_OBJECT_TRACKS;
    object_tracks[0].reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
    object_tracks[0].vcs_position.Set_Position(1.0F,1.0F);
    object_tracks[0].vcs_velocity.lateral = 2.0F;
    object_tracks[0].vcs_velocity.longitudinal = 2.0F;
    object_tracks[0].vcs_accel.lateral = 0.5F;
    object_tracks[0].vcs_accel.longitudinal = 0.5F;
    object_tracks[0].bbox.Set_Length(6.0F);
    object_tracks[0].length_uncertainty = 0.5F;
    object_tracks[0].bbox.Set_Width(1.8F);
    object_tracks[0].bbox.Set_Orientation(1.0F);
    object_tracks[0].width_uncertainty = 0.3F;
    object_tracks[0].id = (int32_t)1;
    object_tracks[0].unique_id = (uint32_t)2000;
    object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
    object_tracks[0].detids[0] = 5;
    const int16_t det_id = object_tracks[0].detids[0];
    object_tracks[0].ndets = 1;
    object_tracks[0].num_dets_used_in_rr_msmt_update = 1;
    object_tracks[0].time_since_initialization = 5.0F;
    object_tracks[0].time_since_downselected = 2.03F;
    object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
    object_tracks[0].exist_prob = 1.0;
    object_tracks[0].movable_prob = 1.0F;
    object_tracks[0].f_moving = true;
    det_list.detections[det_id - 1].raw.sensor_id = 1U;
   
    for (int i = 0; i < STATE_DIMENSION; ++i)
    {
        for (int j = 0; j < STATE_DIMENSION; ++j)
        {
            object_tracks[0].errcov[i][j] = 0.1F;

        }
    }
    object_tracks[0].vcs_heading = Angle(0.2F);
    object_tracks[0].curvature = 0.5F;
    object_tracks[0].speed = 5.0F;
    object_tracks[0].heading_rate = object_tracks[0].speed * object_tracks[0].curvature;
    object_tracks[0].tang_accel = 2.0F ;
    object_tracks[0].confidenceLevel = 0.9;
    object_tracks[0].time_since_stage_start = 5;
    object_tracks[0].probability_pedestrian= 0.01;
    object_tracks[0].probability_car= 0.9;
    object_tracks[0].probability_bicycle= 0.01;
    object_tracks[0].probability_motorcycle= 0.02;
    object_tracks[0].probability_truck= 0.07;
    object_tracks[0].probability_undet= 0.0F;
    object_tracks[0].on_sep_id = 0;
    object_tracks[0].obstacle_prob = 0.5F;
    object_tracks[0].aeb_confidence = AEB_CONF_MEDIUM_HIGH;

    /** \action
     * Call Set_ROT_Object_Output to fill ROT output
     **/
    Set_ROT_Object_Output(host_info, sensors, det_list, object_tracks, rot_obj_output);


    /** \result
     * Ensure that rot object output is filled from the F360_Object_Track_T at the index (NUMBER_OF_REDUCED_OBJECT_TRACKS - 1U)
     **/

    int rot_obj_idx = NUMBER_OF_REDUCED_OBJECT_TRACKS - 1U;

    DOUBLES_EQUAL(rot_obj_output[rot_obj_idx].vcs_x_posn, 1.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[rot_obj_idx].vcs_y_posn, 1.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[rot_obj_idx].vcs_x_vel, 2.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[rot_obj_idx].vcs_y_vel, 2.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[rot_obj_idx].vcs_x_acc, 0.5F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[rot_obj_idx].vcs_y_acc, 0.5F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[rot_obj_idx].vcs_heading, 0.2F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[rot_obj_idx].vcs_pointing, 1.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[rot_obj_idx].vcs_curvature, 0.5F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[rot_obj_idx].speed, 5.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[rot_obj_idx].tang_accel, 2.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[rot_obj_idx].confidence_level, 0.9F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[rot_obj_idx].probability_pedestrian, 0.01F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[rot_obj_idx].probability_car, 0.9F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[rot_obj_idx].probability_bicycle, 0.01F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[rot_obj_idx].probability_motorcycle, 0.02F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[rot_obj_idx].probability_truck, 0.07F, 1e-4F);
    CHECK_EQUAL(rot_obj_output[rot_obj_idx].ndets, 1);
    CHECK_EQUAL(rot_obj_output[rot_obj_idx].num_dets_used_in_rr_msmt_update, 1);
    CHECK_EQUAL(rot_obj_output[rot_obj_idx].trk_fltr_type, F360_TRACKER_TRKFLTR_CCA);
    CHECK_EQUAL(rot_obj_output[rot_obj_idx].trk_fltr_type, F360_TRACKER_TRKFLTR_CCA);
    CHECK_EQUAL(rot_obj_output[rot_obj_idx].f_onguardrail, 0);
    CHECK_EQUAL(rot_obj_output[rot_obj_idx].obstacle_prob, 50);
    CHECK_EQUAL(rot_obj_output[rot_obj_idx].aeb_confidence, AEB_CONF_MEDIUM_HIGH);

    for (int i = 0; i < STATE_DIMENSION; ++i)
    {

    DOUBLES_EQUAL(rot_obj_output[rot_obj_idx].vcs_state_variance[i] , 0.1F, 1e-4F); 

    }
    for (int i = 0; i < 3; ++i)
    {

    DOUBLES_EQUAL(rot_obj_output[rot_obj_idx].vcs_supplemental_state_covariance[i] , 0.1F, 1e-4F); 

    }
}



/** \purpose
 * Check if rot_object_output is filled from F360_Object_Track_T for CCA object.
 * \req
 * NA.
 */
TEST(f360_rot_object_output, rot_object_filled_from_F360_Object_Track_T__CCA_Object)
{
    /** \precond
     * define one object in object_tracks
     */
    object_tracks[0].reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
    object_tracks[0].vcs_position.Set_Position(1.0F,1.0F);
    object_tracks[0].vcs_velocity.lateral = 2.0F;
    object_tracks[0].vcs_velocity.longitudinal = 2.0F;
    object_tracks[0].vcs_accel.lateral = 0.5F;
    object_tracks[0].vcs_accel.longitudinal = 0.5F;
    object_tracks[0].bbox.Set_Length(6.0F);
    object_tracks[0].length_uncertainty = 0.5F;
    object_tracks[0].bbox.Set_Width(1.8F);
    object_tracks[0].bbox.Set_Orientation(1.0F);
    object_tracks[0].width_uncertainty = 0.3F;
    object_tracks[0].id = (int32_t)1;
    object_tracks[0].unique_id = (uint32_t)2000;
    object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
    object_tracks[0].detids[0] = 5;
    const int16_t det_id = object_tracks[0].detids[0];
    object_tracks[0].ndets = 1;
    object_tracks[0].num_dets_used_in_rr_msmt_update = 1;
    object_tracks[0].time_since_initialization = 5.0F;
    object_tracks[0].time_since_downselected = 2.03F;
    object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
    object_tracks[0].exist_prob = 1.0;
    object_tracks[0].movable_prob = 1.0F;
    object_tracks[0].f_moving = true;
    det_list.detections[det_id - 1].raw.sensor_id = 1U;
    for (int i = 0; i < STATE_DIMENSION; ++i)
    {
        for (int j = 0; j < STATE_DIMENSION; ++j)
        {
            object_tracks[0].errcov[i][j] = 0.1F;

        }
    }
    object_tracks[0].vcs_heading = Angle(0.2F);
    object_tracks[0].curvature = 0.5F;
    object_tracks[0].speed = 5.0F;
    object_tracks[0].heading_rate = object_tracks[0].speed * object_tracks[0].curvature;
    object_tracks[0].tang_accel = 2.0F ;
    object_tracks[0].confidenceLevel = 0.9;
    object_tracks[0].time_since_stage_start = 5;
    object_tracks[0].probability_pedestrian= 0.01;
    object_tracks[0].probability_car= 0.9;
    object_tracks[0].probability_bicycle= 0.01;
    object_tracks[0].probability_motorcycle= 0.02;
    object_tracks[0].probability_truck= 0.07;
    object_tracks[0].probability_undet= 0.0F;
    object_tracks[0].on_sep_id = 0;

    /** \action
     * Call Set_ROT_Object_Output to fill ROT output
     **/
    Set_ROT_Object_Output(host_info, sensors, det_list, object_tracks, rot_obj_output);


    /** \result
     * Ensure that rot object output is filled from the F360_Object_Track_T
     **/

    DOUBLES_EQUAL(rot_obj_output[0].vcs_x_posn, 1.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_y_posn, 1.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_x_vel, 2.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_y_vel, 2.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_x_acc, 0.5F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_y_acc, 0.5F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_heading, 0.2F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_pointing, 1.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].vcs_curvature, 0.5F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].speed, 5.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].tang_accel, 2.0F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].confidence_level, 0.9F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].probability_pedestrian, 0.01F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].probability_car, 0.9F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].probability_bicycle, 0.01F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].probability_motorcycle, 0.02F, 1e-4F);
    DOUBLES_EQUAL(rot_obj_output[0].probability_truck, 0.07F, 1e-4F);
    CHECK_EQUAL(rot_obj_output[0].ndets, 1);
    CHECK_EQUAL(rot_obj_output[0].num_dets_used_in_rr_msmt_update, 1);
    CHECK_EQUAL(rot_obj_output[0].trk_fltr_type, F360_TRACKER_TRKFLTR_CCA);
    CHECK_EQUAL(rot_obj_output[0].f_onguardrail, 0);

    for (int i = 0; i < STATE_DIMENSION; ++i)
    {

    DOUBLES_EQUAL(rot_obj_output[0].vcs_state_variance[i] , 0.1F, 1e-4F); 

    }
    for (int i = 0; i < 3; ++i)
    {

    DOUBLES_EQUAL(rot_obj_output[0].vcs_supplemental_state_covariance[i] , 0.1F, 1e-4F); 

    }
}



/** \brief
 * Test if object sensor source is set correctly
 */

TEST(f360_rot_object_output, object_track_radar_sensor_source)
{
    /** \precond
     * define objects with detectino from different sensor
     */
    int16_t det_id = 0;
    object_tracks[0].detids[0] = 5;
    det_id = object_tracks[0].detids[0];
    object_tracks[0].ndets = 1;
    object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
    det_list.detections[det_id - 1].raw.sensor_id = 3U;

    object_tracks[1].detids[0] = 6;
    det_id = object_tracks[1].detids[0];
    object_tracks[1].ndets = 1;
    object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;
    det_list.detections[det_id - 1].raw.sensor_id = 4U;

    object_tracks[2].detids[0] = 7;
    det_id = object_tracks[2].detids[0];
    object_tracks[2].ndets = 1;
    object_tracks[2].status = F360_OBJECT_STATUS_UPDATED;
    det_list.detections[det_id - 1].raw.sensor_id = 5U;

    object_tracks[3].detids[0] = 8;
    det_id = object_tracks[3].detids[0];
    object_tracks[3].ndets = 1;
    object_tracks[3].status = F360_OBJECT_STATUS_UPDATED;
    det_list.detections[det_id - 1].raw.sensor_id = 6U;

    object_tracks[4].detids[0] = 9;
    det_id = object_tracks[4].detids[0];
    object_tracks[4].ndets = 1;
    object_tracks[4].status = F360_OBJECT_STATUS_UPDATED;
    det_list.detections[det_id - 1].raw.sensor_id = 7U;

    object_tracks[5].detids[0] = 10;
    det_id = object_tracks[5].detids[0];
    object_tracks[5].ndets = 1;
    object_tracks[5].status = F360_OBJECT_STATUS_UPDATED;
    det_list.detections[det_id - 1].raw.sensor_id = 8U;
    /** \action
     * Call Set_ROT_Object_Output to fill ROT output
     **/
    Set_ROT_Object_Output(host_info, sensors, det_list, object_tracks, rot_obj_output);
    bool obj1_has_left_side_sensor = (rot_obj_output[0].sensor_src & left_side_sensor) > 0U;
    bool obj2_has_left_rear_sensor = (rot_obj_output[1].sensor_src & rear_left_sensor) > 0U;
    bool obj3_has_rear_mid_sensor = (rot_obj_output[2].sensor_src & rear_mid_sensor) > 0U;
    bool obj4_has_rear_right_sensor = (rot_obj_output[3].sensor_src & rear_right_sensor) > 0U;
    bool obj5_has_right_side_sensor = (rot_obj_output[4].sensor_src & right_side_sensor) > 0U;
    bool obj6_has_front_right_sensor = (rot_obj_output[5].sensor_src & front_right_sensor) > 0U;

    /** \result
     * Ensure that object sensor source is set correctly
     **/
    CHECK_TRUE(obj1_has_left_side_sensor);
    CHECK_TRUE(obj2_has_left_rear_sensor);
    CHECK_TRUE(obj3_has_rear_mid_sensor);
    CHECK_TRUE(obj4_has_rear_right_sensor);
    CHECK_TRUE(obj5_has_right_side_sensor);
    CHECK_TRUE(obj6_has_front_right_sensor);
}

TEST(f360_rot_object_output, object_track_status_conversion_check)
{
    /** \precond
     * define objects with detectino from different sensor
     */

    // object 1 updated but with range rate inlier detection
   object_tracks[0].ndets = 1;
   object_tracks[0].num_rr_inlier_dets = 1;
   object_tracks[0].detids[0] = 1;
   object_tracks[0].reduced_status = F360_OBJECT_STATUS_UPDATED;

   // Object 2 status is updated, with inlier detection 0
   object_tracks[1].num_rr_inlier_dets = 0;
   object_tracks[1].reduced_status = F360_OBJECT_STATUS_UPDATED;

   // Object 3 status is coasting, with range rate inlier detection 1
   object_tracks[2].num_rr_inlier_dets = 1;
   object_tracks[2].reduced_status = F360_OBJECT_STATUS_COASTED;

   //Object 4 status is new, with inlier detection 1
   object_tracks[3].num_rr_inlier_dets = 1;
   object_tracks[3].reduced_status = F360_OBJECT_STATUS_NEW;

   // Object 5 status is new, with inlier detection 0
   object_tracks[4].num_rr_inlier_dets = 0;
   object_tracks[4].reduced_status = F360_OBJECT_STATUS_NEW;

   // Object 6 status is newly updated, with inlier detection 1
   object_tracks[5].num_rr_inlier_dets = 1;
   object_tracks[5].reduced_status = F360_OBJECT_STATUS_NEW_UPDATED;

   // Object 7 status is newly updated, with inlier detection 0
   object_tracks[6].num_rr_inlier_dets = 0;
   object_tracks[6].reduced_status = F360_OBJECT_STATUS_NEW_UPDATED;

   // Object 8 status is coasting, with inlier detection 0
   object_tracks[7].num_rr_inlier_dets = 0U;
   object_tracks[7].reduced_status = F360_OBJECT_STATUS_COASTED;
   
   // Object 9 status is invalid
   object_tracks[8].reduced_status = F360_OBJECT_STATUS_INVALID;

    /** \action
     * Call Set_ROT_Object_Output to fill ROT output
     **/
    Set_ROT_Object_Output(host_info, sensors, det_list, object_tracks, rot_obj_output);

    /** \result
     * Ensure that object status is converted correctly
     **/
    bool all_obj_idx_0_1_2_is_updated = (rot_obj_output[0].object_status == 0U) && (rot_obj_output[1].object_status == 0U) &&  (rot_obj_output[2].object_status == 0U);
    bool all_obj_idx_3_4_5_6_is_new = (rot_obj_output[3].object_status == 1U) && (rot_obj_output[4].object_status == 1U) && (rot_obj_output[5].object_status == 1U) && (rot_obj_output[6].object_status == 1U);
    bool obj_idx_7_is_costing = (rot_obj_output[7].object_status == 2U);
    bool obj_idx_8_is_invalid = (rot_obj_output[8].object_status == 255U);

    CHECK_TRUE(all_obj_idx_0_1_2_is_updated);
    CHECK_TRUE(all_obj_idx_3_4_5_6_is_new);
    CHECK_TRUE(obj_idx_7_is_costing);
    CHECK_TRUE(obj_idx_8_is_invalid);
}

TEST(f360_rot_object_output, object_track_class_conversion_check)
{
    /** \precond
     * define objects with detectino from different sensor
     */
    object_tracks[0].object_class = F360_OBJ_CLASS_CAR;
    object_tracks[1].object_class = F360_OBJ_CLASS_MOTORCYCLE;
    object_tracks[2].object_class = F360_OBJ_CLASS_TRUCK;
    object_tracks[3].object_class = F360_OBJ_CLASS_PEDESTRIAN;
    object_tracks[4].object_class = F360_OBJ_CLASS_BICYCLE;
    object_tracks[5].object_class = F360_OBJ_CLASS_UNDETERMINED;
    object_tracks[6].object_class = F360_OBJ_CLASS_POLE; // should never be set in tracker core, only test for unit test coverage
    object_tracks[7].object_class = F360_OBJ_CLASS_TREE; // should never be set in tracker core, only test for unit test coverage
    object_tracks[8].object_class = F360_OBJ_CLASS_ANIMAL; // should never be set in tracker core, only test for unit test coverage
    object_tracks[9].object_class = F360_OBJ_CLASS_GOD; // should never be set in tracker core, only test for unit test coverage
    object_tracks[10].object_class = F360_OBJ_CLASS_UNIDENTIFIED_VEHICLE; // should never be set in tracker core, only test for unit test coverage

    /** \action
     * Call Set_ROT_Object_Output to fill ROT output
     **/
    Set_ROT_Object_Output(host_info, sensors, det_list, object_tracks, rot_obj_output);

     /** \result
     * Ensure that object classifcation is converted correctly
     **/
    CHECK_TRUE(rot_obj_output[0].object_class==1U);
    CHECK_TRUE(rot_obj_output[1].object_class==2U);
    CHECK_TRUE(rot_obj_output[2].object_class==3U);
    CHECK_TRUE(rot_obj_output[3].object_class==4U);
    CHECK_TRUE(rot_obj_output[4].object_class==5U);
    CHECK_TRUE(rot_obj_output[5].object_class==0U);
    CHECK_TRUE(rot_obj_output[6].object_class==0U);
    CHECK_TRUE(rot_obj_output[7].object_class==0U);
    CHECK_TRUE(rot_obj_output[8].object_class==0U);
    CHECK_TRUE(rot_obj_output[9].object_class==0U);
    CHECK_TRUE(rot_obj_output[10].object_class==0U);
}

TEST(f360_rot_object_output, object_refence_point_conversion_check)
{
    /** \precond
     * define objects with detectino from different sensor
     */
     object_tracks[0].reference_point = F360_REFERENCE_POINT_CENTER;
     object_tracks[1].reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
     object_tracks[2].reference_point = F360_REFERENCE_POINT_FRONT;
     object_tracks[3].reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;
     object_tracks[4].reference_point = F360_REFERENCE_POINT_RIGHT;
     object_tracks[5].reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
     object_tracks[6].reference_point = F360_REFERENCE_POINT_REAR;
     object_tracks[7].reference_point = F360_REFERENCE_POINT_REAR_LEFT;
     object_tracks[8].reference_point = F360_REFERENCE_POINT_LEFT;
    
    /** \action
     * Call Set_ROT_Object_Output to fill ROT output
     **/
    Set_ROT_Object_Output(host_info, sensors, det_list, object_tracks, rot_obj_output);

    /** \result
     * Ensure that object classifcation is converted correctly
     **/
    CHECK_TRUE(rot_obj_output[0].reference_point==0U);
    CHECK_TRUE(rot_obj_output[1].reference_point==1U);
    CHECK_TRUE(rot_obj_output[2].reference_point==2U);
    CHECK_TRUE(rot_obj_output[3].reference_point==3U);
    CHECK_TRUE(rot_obj_output[4].reference_point==4U);
    CHECK_TRUE(rot_obj_output[5].reference_point==5U);
    CHECK_TRUE(rot_obj_output[6].reference_point==6U);
    CHECK_TRUE(rot_obj_output[7].reference_point==7U);
    CHECK_TRUE(rot_obj_output[8].reference_point==8U);
}

/** \purpose
 * Check if orientation rates and orientation rate variance is calculated correctly for CCA objects.
 * \req
 * NA.
 */
TEST(f360_rot_object_output, Compute_Heading_Rate_Variance_CCA)
{
   /** \precond
    * Define heading rates for object tracks
    * Set tracker type to CCA to test the output
    * for that kind of objects
    */
   object_tracks[0].heading_rate = -0.7156F;
   object_tracks[1].heading_rate = -0.6807F;
   object_tracks[2].heading_rate = -0.3491F;
   object_tracks[3].heading_rate = -0.1745F;
   object_tracks[4].heading_rate = 0.0F;
   object_tracks[5].heading_rate = 0.1745F;
   object_tracks[6].heading_rate = 0.3491F;
   object_tracks[7].heading_rate = 0.6807F;
   object_tracks[8].heading_rate = 0.7156F;

   object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_tracks[1].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_tracks[2].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_tracks[3].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_tracks[4].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_tracks[5].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_tracks[6].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_tracks[7].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_tracks[8].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
    
   /** \action
    * Call Set_ROT_Object_Output to fill ROT output
    **/
   Set_ROT_Object_Output(host_info, sensors, det_list, object_tracks, rot_obj_output);

   /** \result
    * Ensure that the calculations are correct
    **/
   DOUBLES_EQUAL(0.043681F, rot_obj_output[0].iso_orientation_rate_var, 1e-4F);
   DOUBLES_EQUAL(0.042067F, rot_obj_output[1].iso_orientation_rate_var, 1e-4F);
   DOUBLES_EQUAL(0.017034F, rot_obj_output[2].iso_orientation_rate_var, 1e-4F);
   DOUBLES_EQUAL(0.008327F, rot_obj_output[3].iso_orientation_rate_var, 1e-4F);
   DOUBLES_EQUAL(0.002704F, rot_obj_output[4].iso_orientation_rate_var, 1e-4F);
   DOUBLES_EQUAL(0.008327F, rot_obj_output[5].iso_orientation_rate_var, 1e-4F);
   DOUBLES_EQUAL(0.017034F, rot_obj_output[6].iso_orientation_rate_var, 1e-4F);
   DOUBLES_EQUAL(0.042067F, rot_obj_output[7].iso_orientation_rate_var, 1e-4F);
   DOUBLES_EQUAL(0.043681F, rot_obj_output[8].iso_orientation_rate_var, 1e-4F);


   DOUBLES_EQUAL(0.7156F, rot_obj_output[0].iso_orientation_rate, 1e-4F);
   DOUBLES_EQUAL(0.6807F, rot_obj_output[1].iso_orientation_rate, 1e-4F);
   DOUBLES_EQUAL(0.3491F, rot_obj_output[2].iso_orientation_rate, 1e-4F);
   DOUBLES_EQUAL(0.1745F, rot_obj_output[3].iso_orientation_rate, 1e-4F);
   DOUBLES_EQUAL(0.0F, rot_obj_output[4].iso_orientation_rate, 1e-4F);
   DOUBLES_EQUAL(-0.1745F, rot_obj_output[5].iso_orientation_rate, 1e-4F);
   DOUBLES_EQUAL(-0.3491F, rot_obj_output[6].iso_orientation_rate, 1e-4F);
   DOUBLES_EQUAL(-0.6807F, rot_obj_output[7].iso_orientation_rate, 1e-4F);
   DOUBLES_EQUAL(-0.7156F, rot_obj_output[8].iso_orientation_rate, 1e-4F);
}

/** \purpose
 * Check if orientation rates and orientation rate variance is calculated correctly for CTCA objects.
 * \req
 * NA.
 */
TEST(f360_rot_object_output, Compute_Heading_Rate_Variance_CTCA)
{
   /** \precond
    * Define heading rates for object tracks
    * Set tracker type to CTCA to test the output
    * for that kind of objects
    */
   object_tracks[0].heading_rate = -0.7156F;
   object_tracks[1].heading_rate = -0.6807F;
   object_tracks[2].heading_rate = -0.3491F;
   object_tracks[3].heading_rate = -0.1745F;
   object_tracks[4].heading_rate = 0.0F;
   object_tracks[5].heading_rate = 0.1745F;
   object_tracks[6].heading_rate = 0.3491F;
   object_tracks[7].heading_rate = 0.6807F;
   object_tracks[8].heading_rate = 0.7156F;

   object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[1].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[2].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[3].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[4].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[5].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[6].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[7].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[8].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
    
   /** \action
    * Call Set_ROT_Object_Output to fill ROT output
    **/
   Set_ROT_Object_Output(host_info, sensors, det_list, object_tracks, rot_obj_output);

   /** \result
    * Ensure that the calculations are correct
    **/
   DOUBLES_EQUAL(0.043681F, rot_obj_output[0].iso_orientation_rate_var, 1e-4F);
   DOUBLES_EQUAL(0.042067F, rot_obj_output[1].iso_orientation_rate_var, 1e-4F);
   DOUBLES_EQUAL(0.017034F, rot_obj_output[2].iso_orientation_rate_var, 1e-4F);
   DOUBLES_EQUAL(0.008327F, rot_obj_output[3].iso_orientation_rate_var, 1e-4F);
   DOUBLES_EQUAL(0.002704F, rot_obj_output[4].iso_orientation_rate_var, 1e-4F);
   DOUBLES_EQUAL(0.008327F, rot_obj_output[5].iso_orientation_rate_var, 1e-4F);
   DOUBLES_EQUAL(0.017034F, rot_obj_output[6].iso_orientation_rate_var, 1e-4F);
   DOUBLES_EQUAL(0.042067F, rot_obj_output[7].iso_orientation_rate_var, 1e-4F);
   DOUBLES_EQUAL(0.043681F, rot_obj_output[8].iso_orientation_rate_var, 1e-4F);

   DOUBLES_EQUAL(0.7156F, rot_obj_output[0].iso_orientation_rate, 1e-4F);
   DOUBLES_EQUAL(0.6807F, rot_obj_output[1].iso_orientation_rate, 1e-4F);
   DOUBLES_EQUAL(0.3491F, rot_obj_output[2].iso_orientation_rate, 1e-4F);
   DOUBLES_EQUAL(0.1745F, rot_obj_output[3].iso_orientation_rate, 1e-4F);
   DOUBLES_EQUAL(0.0F, rot_obj_output[4].iso_orientation_rate, 1e-4F);
   DOUBLES_EQUAL(-0.1745F, rot_obj_output[5].iso_orientation_rate, 1e-4F);
   DOUBLES_EQUAL(-0.3491F, rot_obj_output[6].iso_orientation_rate, 1e-4F);
   DOUBLES_EQUAL(-0.6807F, rot_obj_output[7].iso_orientation_rate, 1e-4F);
   DOUBLES_EQUAL(-0.7156F, rot_obj_output[8].iso_orientation_rate, 1e-4F);
}


/** \purpose
 * Check if functional safety faults are populated to ROT output faults
 * \req
 * NA.
 */
TEST(f360_rot_object_output, Set_ROT_Scl_Faults)
{
   /** \precond
    * Fill in the functional_safety_faults
    */

   Functional_Safety_Faults_Log_T functional_safety_faults_log = {};
   All_SCL_Faults_T fault_status = {};

   functional_safety_faults_log.scl_output_faults.overall_fault_status = 60;

   functional_safety_faults_log.input_faults.core_info.cnt_loops_no_increase = 1;
   functional_safety_faults_log.input_faults.core_info.elapsed_time_above_upper_limit = 1 ;
   functional_safety_faults_log.input_faults.core_info.elapsed_time_below_lower_limit = 1;
   functional_safety_faults_log.input_faults.core_info.time_us_no_increase = 1;

   functional_safety_faults_log.input_faults.host_info.host_lateral_acceleration_invalid = 1;
   functional_safety_faults_log.input_faults.host_info.host_longitudinal_acceleration_invalid = 1;
   functional_safety_faults_log.input_faults.host_info.host_speed_invalid = 1;
   functional_safety_faults_log.input_faults.host_info.host_yawrate_invalid = 1;
   functional_safety_faults_log.input_faults.host_info.vehicle_index_no_increase = 1;

   functional_safety_faults_log.input_faults.raw_detection.range_is_invalid = 1;
   functional_safety_faults_log.input_faults.raw_detection.range_rate_is_invalid = 1;
   functional_safety_faults_log.input_faults.raw_detection.azimuth_is_invalid = 1;
   functional_safety_faults_log.input_faults.raw_detection.elevation_is_invalid = 1;

   functional_safety_faults_log.output_faults.f_track_accelerations_faulty = 1;
   functional_safety_faults_log.output_faults.f_track_positions_faulty = 1;
   functional_safety_faults_log.output_faults.f_track_velocities_faulty = 1;
   functional_safety_faults_log.output_faults.f_severe_angle_jump_presence_fault = 1;

   for (uint8_t i = 0; i < f360_variant_A::MAX_NUMBER_OF_SENSORS; i++)
   {
      functional_safety_faults_log.input_faults.sensors_calibs[i].boresight_angle_is_invalid = 1;
      functional_safety_faults_log.input_faults.sensors_calibs[i].mounting_pos_is_invalid = 1;
      functional_safety_faults_log.input_faults.sensors_calibs[i].polarity_is_invalid = 1;

      functional_safety_faults_log.input_faults.sensors[i].look_index_no_increase = 1;
      functional_safety_faults_log.input_faults.sensors[i].sensor_vs_tracker_timestamp_divergence = 1;
   }

   /** \action
    * Call Set_ROT_Scl_Faults to fill SCL fault statuses
    **/
   Set_ROT_Scl_Faults(functional_safety_faults_log, fault_status);

   /** \result
    * Ensure that the faults are populated correctly
    **/
   CHECK_EQUAL(functional_safety_faults_log.scl_output_faults.overall_fault_status, fault_status.overall_fault_status);

   CHECK_EQUAL(1, fault_status.core_info_faults.cnt_loops_no_increase);
   CHECK_EQUAL(1, fault_status.core_info_faults.elapsed_time_above_upper_limit);
   CHECK_EQUAL(1, fault_status.core_info_faults.elapsed_time_below_lower_limit);
   CHECK_EQUAL(1, fault_status.core_info_faults.time_us_no_increase);

   CHECK_EQUAL(1, fault_status.host_info_faults.host_lateral_acceleration_invalid);
   CHECK_EQUAL(1, fault_status.host_info_faults.host_longitudinal_acceleration_invalid);
   CHECK_EQUAL(1, fault_status.host_info_faults.host_speed_invalid);
   CHECK_EQUAL(1, fault_status.host_info_faults.host_yawrate_invalid);
   CHECK_EQUAL(1, fault_status.host_info_faults.vehicle_index_no_increase);

   CHECK_EQUAL(1, fault_status.object_faults.f_track_accelerations_faulty);
   CHECK_EQUAL(1, fault_status.object_faults.f_track_positions_faulty);
   CHECK_EQUAL(1, fault_status.object_faults.f_track_velocities_faulty);
   CHECK_EQUAL(1, fault_status.object_faults.f_severe_angle_jump_presence_fault);

   CHECK_EQUAL(1, fault_status.raw_detection_faults.range_is_invalid);
   CHECK_EQUAL(1, fault_status.raw_detection_faults.range_rate_is_invalid);
   CHECK_EQUAL(1, fault_status.raw_detection_faults.azimuth_is_invalid);
   CHECK_EQUAL(1, fault_status.raw_detection_faults.elevation_is_invalid);

   for (uint8_t i = 0; i < f360_variant_A::MAX_NUMBER_OF_SENSORS; i++)
   {
      CHECK_EQUAL(1, fault_status.radar_sensor_calib_faults[i].boresight_angle_is_invalid);
      CHECK_EQUAL(1, fault_status.radar_sensor_calib_faults[i].mounting_pos_is_invalid);
      CHECK_EQUAL(1, fault_status.radar_sensor_calib_faults[i].polarity_is_invalid);

      CHECK_EQUAL(1, fault_status.radar_sensor_faults[i].look_index_no_increase);
      CHECK_EQUAL(1, fault_status.radar_sensor_faults[i].sensor_vs_tracker_timestamp_divergence);
   }
}

/** \purpose
 * Check if the internal object occlusion status is mapped correctly to external occlusion status.
 * \req
 * NA.
 */
TEST(f360_rot_object_output, Check_Occlusion_Status_Mapping)
{
   /** \precond
    * Setup 4 objects' occlusion_status.at_vcs_position, each with a unique value of F360_Occlusion_Status_T
    */
   object_tracks[0].occlusion_status = OCCLUSION_STATUS_OCCLUDED;
   object_tracks[1].occlusion_status = OCCLUSION_STATUS_ON_EDGE;
   object_tracks[2].occlusion_status = OCCLUSION_STATUS_VISIBLE;
   object_tracks[3].occlusion_status = OCCLUSION_STATUS_UNDEFINED;
    
   /** \action
    * Call Set_ROT_Object_Output to fill ROT output
    **/
   Set_ROT_Object_Output(host_info, sensors, det_list, object_tracks, rot_obj_output);

   /** \result
    * Ensure that the calculations are correct
    **/
   CHECK_EQUAL(1U, rot_obj_output[0].occlusion_status);
   CHECK_EQUAL(2U, rot_obj_output[1].occlusion_status);
   CHECK_EQUAL(3U, rot_obj_output[2].occlusion_status);
   CHECK_EQUAL(0U, rot_obj_output[3].occlusion_status);
}

/** \purpose
 * Check the object undriable status with some abnormal conditions.
 * \req
 * NA.
 */
TEST(f360_rot_object_output, Check_Inregular_Object)
{
   //Modify some not normal configuration set up
   object_tracks[0].reference_point = (F360_Reference_Point_T)10U;
   object_tracks[0].ndets = 1;
   object_tracks[0].detids[0] = 5;
   det_list.detections[object_tracks[0].detids[0] - 1].raw.sensor_id = 1U;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_UNKNOWN;
   sensors[1].constant.mounting_location =F360_MOUNTING_LOCATION_CENTER2_FORWARD;
   sensors[2].constant.mounting_location =F360_MOUNTING_LOCATION_CENTER3_FORWARD;
   object_tracks[0].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[0].underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
   object_tracks[0].drivable_status_sg = sg::SG_Drivability_Class_T(10U);
   object_tracks[2].ndets = 2;
   object_tracks[2].detids[0] = 7;
   object_tracks[2].detids[1] = 8;
   det_list.detections[object_tracks[2].detids[0] - 1].raw.sensor_id = 1U;
   det_list.detections[object_tracks[2].detids[1] - 1].raw.sensor_id = 2U;
   object_tracks[2].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[3].reduced_id = 1000U;

   /** \action
    * Call Set_ROT_Object_Output to fill ROT output
    **/
   Set_ROT_Object_Output(host_info, sensors, det_list, object_tracks, rot_obj_output);

   /** \result
    * Verify the return data are as expected
    **/
  CHECK_EQUAL(3U, rot_obj_output[0].underdrivable_status_ocg);
  CHECK_EQUAL(0U, rot_obj_output[0].drivable_status_sg);
  CHECK_EQUAL(0U, rot_obj_output[0].reference_point);
}

/** \purpose
 * Check if the object relative velocity and acceleration is correctly to converted to iso coordinate.
 * \req
 * NA.
 */
TEST(f360_rot_object_output, Check_Object_Relative_Motion_Convert_To_ISO)
{
   /** \precond
    * Setup host motion status and objects[0] dynamic motion status
    */
    object_tracks[0].reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
    object_tracks[0].vcs_position.Set_Position(3.0F,3.0F);
    object_tracks[0].vcs_velocity.lateral = 2.0F;
    object_tracks[0].vcs_velocity.longitudinal = 15.0F;
    object_tracks[0].vcs_accel.lateral = 0.5F;
    object_tracks[0].vcs_accel.longitudinal = 0.5F;
    object_tracks[0].id = (int32_t)1;
    object_tracks[0].unique_id = (uint32_t)2000;
    object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
    host_info.dist_rear_axle_to_vcs_m = 3.8F;
    host_info.vcs_sideslip = -0.05F;
    host_info.vcs_speed = 10.0F;
    host_info.vcs_lat_acceleration = 1.5F;
    host_info.vcs_long_acceleration = 2.0F;
    host_info.yaw_rate_rad = -0.1745F;
    
   /** \action
    * Call Set_ROT_Object_Output to fill ROT output
    **/
   Set_ROT_Object_Output(host_info, sensors, det_list, object_tracks, rot_obj_output);

   /** \result
    * Ensure that the calculations are correct
    **/
   DOUBLES_EQUAL(4.489F, rot_obj_output[0].iso_relative_x_vel, 1e-4F);
   DOUBLES_EQUAL(-3.6863F, rot_obj_output[0].iso_relative_y_vel, 1e-4F);
   DOUBLES_EQUAL(-1.7071F, rot_obj_output[0].iso_relative_x_acc, 1e-4F);
   DOUBLES_EQUAL(1.0914F, rot_obj_output[0].iso_relative_y_acc, 1e-4F);

}


/** @}*/
