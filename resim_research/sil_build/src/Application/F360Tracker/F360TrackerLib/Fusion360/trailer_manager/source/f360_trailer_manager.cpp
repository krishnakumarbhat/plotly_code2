/*===========================================================================*\
 * FILE: f360_trailer_manager.cpp
 *============================================================================
 * Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
 *-----------------------------------------------------------------------------------------
 * DESCRIPTION:
 *   This file defines Trailer Manager function to switch between trailer algos
 *   for commercial vehicles and passenger vehicles.
 *
 *   Applicable Standards (in order of precedence: highest first):
 *     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
 *     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
 *
\*==========================================================================================*/
#include "f360_trailer_manager.h"
#include "f360_identify_and_flag_internal_reflections.h"
#include "f360_get_wall_time.h"
#include "f360_mark_trailer_detections.h"
#include "f360_pvtrailer_main.h"

namespace f360_variant_A
{
   /*=========================================================================
    * Method         Trailer_Manager
    *
    * Description    Method that switches the algo between passenger vehicle trailer
    *                commercial vehicle trailer algos
    *
    * Parameter      const F360_Calibrations_T& calibrations,
    *                const F360_Host_T& host,
    *                const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
    *                const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
    *                const F360_Tracker_Info_T& tracker_info,
    *                F360_Detection_Props_T(&all_detections)[MAX_NUMBER_OF_DETECTIONS],
    *                F360_Radar_Sensor_Props_T(&sensor_props)[MAX_NUMBER_OF_SENSORS],
    *                F360_Passenger_Trailer_Estimator& passenger_trailer,
    *                F360_CV_Trailer_Estimator& cv_trailer,
    *                F360_Trailer_Estimator_Output_T& trailer_estimator_output,
    *                F360_TRKR_TIMING_INFO_T& timing_info
    *
    * Returns        None.
    *
    * Externals:     None.
    *
    * Precondition   None.
    *
    * Postcondition  None.
    *
    * Note           None.
    *========================================================================*/
   void Trailer_Manager(
      const F360_Calibrations_T& calibrations,
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Tracker_Info_T& tracker_info,
      F360_Detection_Props_T(&all_detections)[MAX_NUMBER_OF_DETECTIONS],
      F360_Radar_Sensor_Props_T(&sensor_props)[MAX_NUMBER_OF_SENSORS],
      F360_PVTrailer_Data_T& pvtrailer_data,
      F360_CVT_State_T& cvt_state,
      F360_Trailer_Estimator_Output_T& trailer_estimator_output,
      F360_TRKR_TIMING_INFO_T& timing_info)
   {
      const float32_t start_time = get_wall_time();
      const bool bike_carrier_attached = Is_Bike_Carrier_Attached(sensors, sensor_props);
      static bool f_internal_flag_for_trailer_presence = false;
      if (host.f_trailer_presence_hardware)
      {
         if (!bike_carrier_attached)
         {
            Run_Trailer_Manager_Estimators(calibrations, host, raw_detect_list, all_detections, sensors, tracker_info, pvtrailer_data, cvt_state, timing_info);
            Get_Trailer_Manager_Output(host, cvt_state, pvtrailer_data, trailer_estimator_output);
         }
         Run_Trailer_Related_Countermeasures(calibrations, host, raw_detect_list, sensors, trailer_estimator_output, bike_carrier_attached, sensor_props, all_detections);
         f_internal_flag_for_trailer_presence = true;
      }

      else if ((!host.f_trailer_presence_hardware) && f_internal_flag_for_trailer_presence)
      {
         Reset_Trailer_Manager_Estimators(host, cvt_state, pvtrailer_data, trailer_estimator_output);
         f_internal_flag_for_trailer_presence = false;
      }
      else
      {
         //misra
      }
      timing_info.trailer_detector = get_wall_time() - start_time;
   }

   /*=========================================================================
    * Method         Parse_CV_Trailer_Output
    *
    * Description    Method parses the output from CV trailer to f360 tracker
    *                specific output, that is required for filtering of
    *                detections due to the trailer.
    *
    * Parameters     const trailer_estimator::jz_T& cv_trailer_output,
    *                Trailer_Detector_Flt_Fus_Output& trailer_output
    *
    * Returns        None.
    *
    * Externals:     None.
    *
    * Precondition   None.
    *
    * Postcondition  None.
    *
    * Note           None.
    *========================================================================*/
   void Parse_CV_Trailer_Output(
      const F360_CVT_State_T& cvt_state,
      F360_Trailer_Estimator_Output_T& trailer_estimator_output)
   {
      if (cvt_state.best_trailer_model == TRAILER_MODEL_TWO_LINK)
      {
         trailer_estimator_output.trailer_length[0] = cvt_state.two_link.trailer1_length;
         trailer_estimator_output.trailer_width[0] = cvt_state.two_link.trailer1_width;
         trailer_estimator_output.trailer_angle[0] = cvt_state.two_link.ekf_state[0];
         trailer_estimator_output.joint_position_vcs_long[0] = cvt_state.two_link.joint1_vcs_longpos;
         trailer_estimator_output.joint_position_vcs_lat[0] = cvt_state.two_link.joint1_vcs_latpos;
         trailer_estimator_output.joint2center[0] = cvt_state.two_link.joint1_dist_to_center;
         trailer_estimator_output.trailer_presence[0] = (cvt_state.two_link.filter_state != TRAILER_FILTER_STATE_NOT_STARTED) ? TRAILER_PRESENCE_STATE_DETECTED : TRAILER_PRESENCE_STATE_NOT_DETECTED;

         trailer_estimator_output.trailer_length[1] = cvt_state.two_link.trailer2_length;
         trailer_estimator_output.trailer_width[1] = cvt_state.two_link.trailer2_width;
         trailer_estimator_output.trailer_angle[1] = cvt_state.two_link.ekf_state[3];
         trailer_estimator_output.joint_position_vcs_long[1] = cvt_state.two_link.joint2_vcs_longpos;
         trailer_estimator_output.joint_position_vcs_lat[1] = cvt_state.two_link.joint2_vcs_latpos;
         trailer_estimator_output.joint2center[1] = cvt_state.two_link.joint2_dist_to_center;
         trailer_estimator_output.trailer_presence[1] = (cvt_state.two_link.filter_state != TRAILER_FILTER_STATE_NOT_STARTED) ? TRAILER_PRESENCE_STATE_DETECTED : TRAILER_PRESENCE_STATE_NOT_DETECTED;
      }
      else if (cvt_state.best_trailer_model == TRAILER_MODEL_ONE_LINK)
      {
         trailer_estimator_output.trailer_length[0] = cvt_state.one_link.trailer_length;
         trailer_estimator_output.trailer_width[0] = cvt_state.one_link.trailer_width;
         trailer_estimator_output.trailer_angle[0] = cvt_state.one_link.ekf_state[0];
         trailer_estimator_output.joint_position_vcs_long[0] = cvt_state.one_link.joint_vcs_longpos;
         trailer_estimator_output.joint_position_vcs_lat[0] = cvt_state.one_link.joint_vcs_latpos;
         trailer_estimator_output.joint2center[0] = cvt_state.one_link.joint_dist_to_center;
         trailer_estimator_output.trailer_presence[0] = (cvt_state.one_link.filter_state != TRAILER_FILTER_STATE_NOT_STARTED) ? TRAILER_PRESENCE_STATE_DETECTED : TRAILER_PRESENCE_STATE_NOT_DETECTED;

         trailer_estimator_output.trailer_length[1] = 0.0F;
         trailer_estimator_output.trailer_width[1] = 0.0F;
         trailer_estimator_output.trailer_angle[1] = 0.0F;
         trailer_estimator_output.joint_position_vcs_long[1] = 0.0F;
         trailer_estimator_output.joint_position_vcs_lat[1] = 0.0F;
         trailer_estimator_output.joint2center[1] = 0.0F;
         trailer_estimator_output.trailer_presence[1] = TRAILER_PRESENCE_STATE_NOT_DETECTED;
      }
      else
      {
         trailer_estimator_output.trailer_length[0] = 0.0F;
         trailer_estimator_output.trailer_width[0] = 0.0F;
         trailer_estimator_output.trailer_angle[0] = 0.0F;
         trailer_estimator_output.joint_position_vcs_long[0] = 0.0F;
         trailer_estimator_output.joint_position_vcs_lat[0] = 0.0F;
         trailer_estimator_output.joint2center[0] = 0.0F;
         trailer_estimator_output.trailer_presence[0] = TRAILER_PRESENCE_STATE_NOT_DETECTED;

         trailer_estimator_output.trailer_length[1] = 0.0F;
         trailer_estimator_output.trailer_width[1] = 0.0F;
         trailer_estimator_output.trailer_angle[1] = 0.0F;
         trailer_estimator_output.joint_position_vcs_long[1] = 0.0F;
         trailer_estimator_output.joint_position_vcs_lat[1] = 0.0F;
         trailer_estimator_output.joint2center[1] = 0.0F;
         trailer_estimator_output.trailer_presence[1] = TRAILER_PRESENCE_STATE_NOT_DETECTED;
      }

      trailer_estimator_output.f_reversing_countermeasures_active = cvt_state.f_reversing_countermeasures_active;
   }

   /*=========================================================================
   * Method        Run_Trailer_Manager_Estimators
   *
   * Description   Method runs trailer related estimators to determine the output
   *               parameters. Which estimator is to be run is determined based on
   *               the host_type. There are two possibilietes - estimate
   *               commercial trailer size, or estimate the passenger trailer type.
   *
   * Parameters    const F360_Calibrations_T& calibrations,
   *               const F360_Host_T& host,
   *               const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
   *               const F360_Detection_Props_T(&all_detections)[MAX_NUMBER_OF_DETECTIONS],
   *               const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   *               const F360_Tracker_Info_T& tracker_info,
   *               F360_Passenger_Trailer_Estimator& passenger_trailer,
   *               F360_CV_Trailer_Estimator& cv_trailer,
   *               F360_TRKR_TIMING_INFO_T& timing_info
   *
   * Returns        None.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   void Run_Trailer_Manager_Estimators(
      const F360_Calibrations_T& calibrations,
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Detection_Props_T(&all_detections)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Tracker_Info_T& tracker_info,
      F360_PVTrailer_Data_T& pvtrailer_data,
      F360_CVT_State_T& cvt_state,
      F360_TRKR_TIMING_INFO_T& timing_info)
   {
      switch (host.host_type)
      {
         case F360_HOST_TYPE_COMMERCIAL_VEHICLE:
            Run_CV_Trailer_Estimator(calibrations, host, raw_detect_list, all_detections, sensors, tracker_info, cvt_state, timing_info);
            break;

         case F360_HOST_TYPE_PASSENGER_VEHICLE:
            Run_PV_Trailer_Estimator(host, raw_detect_list, all_detections, sensors, tracker_info.elapsed_time_s, pvtrailer_data, timing_info);
            break;

         default:
            break;
      }
   }

   /*=========================================================================
   * Method         Reset_Trailer_Manager_Estimators
   *
   * Description    Method will clear the state of estimators and reset the
   *                trailer output.
   *
   * Parameters     const F360_Host_T& host,
   *                F360_CV_Trailer_Estimator& cv_trailer,
   *                F360_Passenger_Trailer_Estimator& passenger_trailer,
   *                F360_Trailer_Estimator_Output_T& trailer_estimator_output
   *
   * Returns        None.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   void Reset_Trailer_Manager_Estimators(
      const F360_Host_T& host,
      F360_CVT_State_T& cvt_state,
      F360_PVTrailer_Data_T& pvtrailer_data,
      F360_Trailer_Estimator_Output_T& trailer_estimator_output)
   {
      switch (host.host_type)
      {
         case F360_HOST_TYPE_COMMERCIAL_VEHICLE:
            CVT_Reset(cvt_state);
            break;

         case F360_HOST_TYPE_PASSENGER_VEHICLE:
            PVTrailer_Reset(pvtrailer_data);
            break;

         default:
            break;
      }
      Get_Trailer_Manager_Output(host, cvt_state, pvtrailer_data, trailer_estimator_output);
   }

   /*=========================================================================
   * Method         Get_Trailer_Manager_Output
   *
   * Description    Method will parse the trailer parameters based on the
   *                estimator outputs to the trailer output struct.
   *
   * Parameters     const F360_Host_T& host,
   *                const F360_CV_Trailer_Estimator& cv_trailer,
   *                const F360_Passenger_Trailer_Estimator& passenger_trailer,
   *                F360_Trailer_Estimator_Output_T& trailer_estimator_output
   *
   * Returns        None.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   void Get_Trailer_Manager_Output(
      const F360_Host_T& host,
      const F360_CVT_State_T& cvt_state,
      const F360_PVTrailer_Data_T& pvtrailer_data,
      F360_Trailer_Estimator_Output_T& trailer_estimator_output)
   {
      switch (host.host_type)
      {
         case F360_HOST_TYPE_COMMERCIAL_VEHICLE:
         {
            Parse_CV_Trailer_Output(cvt_state, trailer_estimator_output);
            break;
         }
         case F360_HOST_TYPE_PASSENGER_VEHICLE:
         {
            Parse_PV_Trailer_Output(pvtrailer_data, trailer_estimator_output);
            break;
         }
         default:
            break;
      }
   }

   /*=========================================================================
   * Method       Run_Trailer_Related_Countermeasures
   *
   * Description  Method contains set of countermeasures to prevent tracking
   *              deterioration with trailer attached. The countermeasures are
   *              dependant on the trailer output. The internal reflections
   *              countermeasure is also run in this module.
   *
   * Parameters   const F360_Calibrations_T& calibrations,
   *              const F360_Host_T& host,
   *              const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
   *              const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   *              const F360_Trailer_Estimator_Output_T& trailer_estimator_output,
   *              const bool bike_carrier_attached,
   *              F360_Radar_Sensor_Props_T(&sensor_props)[MAX_NUMBER_OF_SENSORS],
   *              F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
   *
   * Returns        None.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   void Run_Trailer_Related_Countermeasures(
      const F360_Calibrations_T& calibrations,
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Trailer_Estimator_Output_T& trailer_estimator_output,
      const bool bike_carrier_attached,
      F360_Radar_Sensor_Props_T(&sensor_props)[MAX_NUMBER_OF_SENSORS],
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      if (bike_carrier_attached)
      {
         const uint32_t number_of_valid_detections = raw_detection_list.number_of_valid_detections;
         for (uint32_t det_idx = 0U; det_idx < number_of_valid_detections; det_idx++)
         {
            const rspp_variant_A::RSPP_Detection_T& current_detection = raw_detection_list.detections[det_idx];
            F360_Detection_Props_T& current_detection_prop = detection_props[det_idx];
            const int32_t current_sensor_id = current_detection.raw.sensor_id;
            const F360_Radar_Sensor_T& current_sensor = sensors[current_sensor_id - 1];
            F360_Radar_Sensor_Props_T& current_sensor_props = sensor_props[current_sensor_id - 1];
            Identify_And_Mark_Internal_Reflections(current_detection, current_sensor, current_sensor_props, current_detection_prop);
         }
      }
      else
      {
         Mark_Trailer_Dets(calibrations, host, raw_detection_list, trailer_estimator_output, sensors, detection_props);
      }
   }
}
