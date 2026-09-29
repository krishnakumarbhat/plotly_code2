/** \file
*   This file aims to test the calling order of tracker modules.
*/
#include <float.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cfloat>
#include "f360_tracker.h"
#include "f360_update_relative_timestamps.h"
#include "f360_time_update_tracks.h"
#include "f360_pre_association_track_management.h"
#include "f360_detection_to_track_association.h"
#include "f360_clustering.h"
#include "f360_cluster_grouping.h"
#include "f360_internal_preprocessing.h"
#include "f360_inputs_preprocessing.h"
#include "f360_track_grouping.h"
#include "f360_initialize_tracks.h"
#include "f360_measurement_update_tracks.h"
#include "f360_post_update_track_adjustments.h"
#include "f360_track_classification.h"
#include "f360_track_validity.h"
#include "f360_track_downselection.h"
#include "f360_e2e_protection.h"
#include "f360_static_environment_class.h"
#include "f360_trailer_detector_core.h"
#include "f360_sanity_check.h"
#include "f360_sensor_postprocessing.h"
#include "f360_get_wall_time.h"
#include "f360_occlusion.h"
#include "f360_static_env_poly_types.h"
#include "f360_occupancy_grid_types.h"
#include "init_tracker_info_T.h"
#include "init_object_tracks_T.h"
#include "init_clusters_T.h"
#include "init_core_T.h"
#include "init_det_hist_T.h"

#include "init_det_props_T.h"
#include "init_globals_T.h"
#include "init_host_props_T.h"
#include "init_host_T.h"
#include "init_raw_detect_T.h"
#include "init_sensor_calib_T.h"
#include "init_sensor_props_T.h"
#include "init_sensors_T.h"
#include "init_timing_info_T.h"
#include "init_static_env_class_T.h"
#include "init_trailer_detector_core.h"
#include "init_occupancy_grid_T.h"
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#define Incorrect_Calling_Order                  0
#define Up_To_Inputs_Preprocessing_Module_call   1
#define Up_To_Update_TimeStamp_Module_call       2
#define Up_To_TimeUpdate_Module_call             3
#define Up_To_StaticEnvironment_LSC_Module_call  4
#define Up_To_Occlusion_Module_call              5
#define Up_To_Internal_Preprocessing_Module_call 6
#define Up_To_Trailer_Detector_Core_Module_call  7
#define Up_To_PreAssociation_Module_call         8
#define Up_To_Association_Module_call            9
#define Up_To_Clustering_Module_call             10
#define Up_To_Cluster_Grouping_Module_call       11
#define Up_To_TrackGrouping_Module_call          12
#define Up_To_MsmtUpdate_Module_call             13
#define Up_To_NewObjInit_Module_call             14
#define Up_To_PostUpdateTrack_Module_call        15
#define Up_To_TrackClassification_Module_call    16
#define Up_To_TrackValidity_Module_call          17
#define Up_To_TrackDownSelection_Module_call     18
#define Up_To_StaticEnvironment_CWD_Module_call  19
#define Up_To_PostProcessor_Module_call          20

namespace f360_variant_A
{
   /*Wrapping the required module functions with a simple code to make sure that the calling order is correct */
   unsigned int calling_order = 0;

   void Inputs_Preprocessing(
      const F360_Core_Info_T& core_info,
      const F360_Host_T& host,
      F360_Host_Props_T& host_props,
      F360_TRKR_TIMING_INFO_T& timing_info)
   {
      if (calling_order == 0)
      {
         calling_order = Up_To_Inputs_Preprocessing_Module_call;
      }
      else
      {
         calling_order = Incorrect_Calling_Order;
      }
   }


   void Update_Relative_Timestamps(
      const float32_t elapsed_time_s,
      const F360_Tracker_Info_T & p_tracker_info,
      F360_Object_Track_T(&obj_trks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      F360_Detection_Hist_T & p_det_hist)
   {
      if (calling_order == Up_To_Inputs_Preprocessing_Module_call)
      {
         calling_order = Up_To_Update_TimeStamp_Module_call;
      }
      else
      {
         calling_order = Incorrect_Calling_Order;
      }
   }

   void Time_Update_Tracks(
      const F360_Calibrations_T & calibrations,
      const float32_t elapsed_time_s,
      const F360_Host_T & host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Host_Props_T & host_props,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T & tracker_info,
      F360_TRKR_TIMING_INFO_T &timing_info)
   {
      if (calling_order == Up_To_Update_TimeStamp_Module_call)
      {
         calling_order = Up_To_TimeUpdate_Module_call;
      }
      else
      {
         calling_order = Incorrect_Calling_Order;
      }

   }

   void Static_Env_T::Run_Longi_Stat_Curves(
         const F360_Tracker_Info_T& tracker_info,
         const F360_Calibrations_T& calibs,
         F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS],
         F360_TRKR_TIMING_INFO_T& timing_info)
   {
      if (calling_order == Up_To_TimeUpdate_Module_call)
      {
         calling_order = Up_To_StaticEnvironment_LSC_Module_call;
      }
      else
      {
         calling_order = Incorrect_Calling_Order;
      }
   }

   Occlusion_T::Occlusion_T(
      const bool f_enable_occlusion,
      const F360_Calibrations_T& calib,
      const F360_Tracker_Info_T& tracker_info,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_TRKR_TIMING_INFO_T &timing_info,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS])
   {
      if (calling_order == Up_To_StaticEnvironment_LSC_Module_call)
      {
         calling_order = Up_To_Occlusion_Module_call;
      }
      else
      {
         calling_order = Incorrect_Calling_Order;
      }
   }

   void Internal_Preprocessing(
      const F360_Host_T &host,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detect_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T &calibrations,
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const Trailer_Detector_Flt_Fus_Output &trailer_detector_output,
      const F360_Host_Props_T& host_props,
      F360_Globals_T &globals,
      F360_Detection_Hist_T &det_hist,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Radar_Sensor_Props_T(&sensor_props)[MAX_NUMBER_OF_SENSORS],
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      F360_Tracker_Info_T &tracker_info,
      F360_TRKR_TIMING_INFO_T &timing_info)
   {
      if (calling_order == Up_To_Occlusion_Module_call)
      {
         calling_order = Up_To_Internal_Preprocessing_Module_call;
      }
      else
      {
         calling_order = Incorrect_Calling_Order;
      }

   }

   void Trailer_Detector_Core::Run_Trailer_Detector(
      const F360_Host_T &p_vehicle_data, 
      const rspp_variant_A::RSPP_Detection_List_T &raw_detect_list,
      const F360_Detection_Props_T (&all_detections)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const float32_t elapsed_time_s)
   {
      if (calling_order == Up_To_Internal_Preprocessing_Module_call)
      {
         calling_order = Up_To_Trailer_Detector_Core_Module_call;
      }
      else
      {
         calling_order = Incorrect_Calling_Order;
      }

   }

   void Pre_Association_Track_Management(
      const F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      const rspp_variant_A::RSPP_Detection_List_T &raw_detect_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Host_T& host,
      const F360_Calibrations_T &calibrations,
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_TRKR_TIMING_INFO_T & timing_info)
   {
      if (calling_order == Up_To_Trailer_Detector_Core_Module_call)
      {
         calling_order = Up_To_PreAssociation_Module_call;
      }
      else
      {
         calling_order = Incorrect_Calling_Order;
      }
   }
   void Detection_To_Track_Association(
      const F360_Host_T & host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Tracker_Info_T & tracker_info,
      const rspp_variant_A::RSPP_Detection_List_T & raw_detection_list,
      const F360_Calibrations_T & calibrations,
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T & timing_info)
   {
      if (calling_order == Up_To_PreAssociation_Module_call)
      {
         calling_order = Up_To_Association_Module_call;
      }
      else
      {
         calling_order = Incorrect_Calling_Order;
      }
   }
   void Clustering(
      const F360_Calibrations_T &calibrations,
      F360_Tracker_Info_T &tracker_info,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      F360_TRKR_TIMING_INFO_T &timing_info)
   {
      if (calling_order == Up_To_Association_Module_call)
      {
         calling_order = Up_To_Clustering_Module_call;
      }
      else
      {
         calling_order = Incorrect_Calling_Order;
      }
   }
   void Cluster_Grouping(
      const F360_Globals_T & globals,
      const F360_Calibrations_T &calib,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      F360_Tracker_Info_T & tracker_info,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Detection_Hist_T & det_hist,
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      F360_TRKR_TIMING_INFO_T & timing_info)
   {
      if (calling_order == Up_To_Clustering_Module_call)
      {
         calling_order = Up_To_Cluster_Grouping_Module_call;
      }
      else
      {
         calling_order = Incorrect_Calling_Order;
      }
   }
   void Track_Grouping(
      const F360_Calibrations_T & calib,
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Host_T& host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T & raw_detection_list,
      const F360_Globals_T& globals,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Tracker_Info_T& tracker_info,
      F360_TRKR_TIMING_INFO_T & timing_info)
   {
      if (calling_order == Up_To_Cluster_Grouping_Module_call)
      {
         calling_order = Up_To_TrackGrouping_Module_call;
      }
      else
      {
         calling_order = Incorrect_Calling_Order;
      }
   }
   void Measurement_Update_Tracks(
      const F360_Calibrations_T & calibrations,
      const F360_Host_T & host,
      const F360_Tracker_Info_T & tracker_info,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T & timing_info)
   {
      if (calling_order == Up_To_TrackGrouping_Module_call)
      {
         calling_order = Up_To_MsmtUpdate_Module_call;
      }
      else
      {
         calling_order = Incorrect_Calling_Order;
      }
   }
   void Initialize_Tracks(
      const F360_Host_T& host,
      const F360_Host_Props_T& host_props,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Detection_Hist_T& det_hist,
      const F360_Globals_T& globals,
      const F360_Calibrations_T& calibrations,
      const Static_Env_Poly_T(&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const Occlusion_T& occlusion,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T& tracker_info,
      F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_TRKR_TIMING_INFO_T& tracker_time_info)
   {
      if (calling_order == Up_To_MsmtUpdate_Module_call)
      {
         calling_order = Up_To_NewObjInit_Module_call;
      }
      else
      {
         calling_order = Incorrect_Calling_Order;
      }
   }
   void Post_Update_Track_Adjustments(
      const F360_Calibrations_T & calibrations,
      const F360_Globals_T & globals,
      const rspp_variant_A::RSPP_Detection_List_T & raw_detect_list,
      const F360_Host_T & host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Tracker_Info_T & tracker_info,
      F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T & timing_info)
   {
      if (calling_order == Up_To_NewObjInit_Module_call)
      {
         calling_order = Up_To_PostUpdateTrack_Module_call;
      }
      else
      {
         calling_order = Incorrect_Calling_Order;
      }
   }
   void Track_Classification(
      const F360_Host_T & host,
      const F360_Tracker_Info_T & tracker_info,
      const Occupancy_Grid_T & occupancy_grid, 
      const F360_Globals_T & globals,
      const rspp_variant_A::RSPP_Detection_List_T & dets_raw,
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T & calib,
      const Occlusion_T& occlusion,
      F360_TRKR_TIMING_INFO_T & timing_info,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS])
   {
      if (calling_order == Up_To_PostUpdateTrack_Module_call)
      {
         calling_order = Up_To_TrackClassification_Module_call;
      }
      else
      {
         calling_order = Incorrect_Calling_Order;
      }
   }
   void Track_Validity(
      const F360_Host_T& host,
      const F360_Tracker_Info_T& tracker_info,
      const F360_Calibrations_T& calibrations,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Radar_Sensor_Props_T(&sensor_props)[MAX_NUMBER_OF_SENSORS],
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T& timing_info)
   {
      if (calling_order == Up_To_TrackClassification_Module_call)
      {
         calling_order = Up_To_TrackValidity_Module_call;
      }
      else
      {
         calling_order = Incorrect_Calling_Order;
      }
   }

   void Track_Downselection(
      const F360_Host_T& host,
      const F360_Calibrations_T& calib,
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T& tracker_info,
      F360_TRKR_TIMING_INFO_T& timing_info)
   {
      if (calling_order == Up_To_TrackValidity_Module_call)
      {
         calling_order = Up_To_TrackDownSelection_Module_call;
      }
      else
      {
         calling_order = Incorrect_Calling_Order;
      }
   }
   void Static_Env_T::Run_Concrete_Wall_Detector(
         const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
         const rspp_variant_A::RSPP_Detection_List_T &raw_detect_list,
         const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
         const F360_Radar_Sensor_Props_T(&sensor_props)[MAX_NUMBER_OF_SENSORS],
         const F360_Calibrations_T &calib,
         const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
         const F360_Host_T& host,
         F360_TRKR_TIMING_INFO_T& timing_info)
   {
      if (calling_order == Up_To_TrackDownSelection_Module_call)
      {
         calling_order = Up_To_StaticEnvironment_CWD_Module_call;
      }
      else
      {
         calling_order = Incorrect_Calling_Order;
      }
   }

   void Sensor_Postprocessing(
      const F360_Tracker_Info_T& tracker_info,
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      F360_Detection_Hist_T& detection_hist,
      F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
      F360_TRKR_TIMING_INFO_T &timing_info)
   {
      if (calling_order == Up_To_StaticEnvironment_CWD_Module_call)
      {
         calling_order = Up_To_PostProcessor_Module_call;
      }
      else
      {
         calling_order = Incorrect_Calling_Order;
      }
   }

   /* wrapping the other required F360_Tracker functions */
   float get_wall_time()
   {
      return (float)0.0;
   }
   bool Sanity_Check(
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Detection_Hist_T& detection_hist,
      const F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Tracker_Info_T& tracker_info,
      F360_TRKR_TIMING_INFO_T& timing_info
   )
   {
      return false;
   }

   void F360_Sort(float32_t* arr, uint32_t num_elements, const bool f_ascend, uint32_t* perm)
   {}

   float32_t Calculate_Priority(
      const F360_Calibrations_T & calibs,
      const F360_Host_T & host_props,
      const bool f_movable,
      const float32_t confidence,
      const float32_t longitudal_pos,
      const float32_t lateral_pos) 
   {
       return float32_t{};
   }

   void Propagate_Uncertainty_For_Unified_Output_CTCA(
      const F360_Object_Track_T & obj_track,
      float32_t(&unified_errcov)[10][10])
   {}

   void E2E_Protection(
      F360_TRKR_TIMING_INFO_T & timing_info)
   {}

   Sensor_Occlusion_Info_T::Sensor_Occlusion_Info_T() {}

   Sensor_Occlusion_Info_T::~Sensor_Occlusion_Info_T() {}

   Occlusion_T::~Occlusion_T() {}

   Occlusion_Sector_T::Occlusion_Sector_T() {}

   Occlusion_Sector_T::~Occlusion_Sector_T() {}

   Sensor_FOV_T::Sensor_FOV_T() {}

   Sensor_FOV_T::~Sensor_FOV_T() {}

   F360_Concrete_Wall_Detector_T::F360_Concrete_Wall_Detector_T() {}
   F360_Concrete_Wall_Detector_T::~F360_Concrete_Wall_Detector_T() {}
   void F360_Concrete_Wall_Detector_T::Init_Long_Zones(const float32_t sensor_zone_half_length) {}
   void F360_Concrete_Wall_Detector_T::Init(const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS], const F360_Calibrations_T &calib) {}

   Concrete_Wall_Sensor_T::Concrete_Wall_Sensor_T() {}
   Concrete_Wall_Sensor_T::~Concrete_Wall_Sensor_T() {}

   Concrete_Wall_Measurements_T::Concrete_Wall_Measurements_T() {}
   void Concrete_Wall_Measurements_T::Push(const Concrete_Wall_Sample_T& sample) {}
   Concrete_Wall_Side_T::Concrete_Wall_Side_T() {}

   Static_Env_T::Static_Env_T() {}
   Static_Env_T::~Static_Env_T() {}
   void Static_Env_T::Initialize_Static_Env() {}
   void Static_Env_T::Set_Polynomials(Static_Env_Poly_T(&static_polys_in)[F360_NUM_OF_STATIC_ENV_POLYS]) {}

   Static_Env_T stat_class;
   const Static_Env_T::Static_Env_Polys_Array& Static_Env_T::Get_Polynomials() const { return stat_class.static_env_polys; }
   
   void Trailer_Detector_Core::Initialize() {}
   Trailer_Detector_Core td_class;
   Trailer_Detector_Flt_Fus_Output Trailer_Detector_Core::Get_Trailer_Detector_Output() const {return td_class.trailer_detector_output;}

}

namespace f360_variant_A
{
   /** \defgroup  Tracker_Calling_Order
    *  @{
    */

    /** \brief
    *  This test suit aims to test the calling order of tracker modules.
    **/
   TEST_GROUP(Tracker_Calling_Order)
   {
      F360_Host_Tag host = {};
      F360_Core_Info_T core_info = {};
      F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
      F360_Globals_T globals = {};
      F360_Host_Props_T host_props = {};
      F360_Detection_Props_T detections[MAX_NUMBER_OF_DETECTIONS] = {};
      F360_Detection_Hist_T det_hist = {};
      F360_TRKR_TIMING_INFO_T    timing_info = {};
      F360_Cluster_T     clusters[NUMBER_OF_CLUSTERS] = {};
      F360_Calibrations_T    calibrations = {};
      rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
      F360_Tracker_Info_T tracker_info = {};
      F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
      F360_Radar_Sensor_Props_T sensor_props[MAX_NUMBER_OF_SENSORS] = {};
      Static_Env_T static_env_class;
      Occupancy_Grid_T occupancy_grid = {};
      /** \setup
      **/
      TEST_SETUP()
      {
      }

      /** \teardown
      * Nothing to teardown in this test group
      **/
   };

   /** \purpose
    *  This test will check the correct calling order of F360_Tracker module functions.
    * \req NA
    */
   TEST(Tracker_Calling_Order, Tracker_Calling_Order_Test)
   {
      /** \precond
       *- Create F360_Tracker object
       **/
      F360_Tracker tracker;

      /** \action
       * Call tracker function
       **/
      tracker.Execute(core_info, host, occupancy_grid, raw_detection_list, sensors);

      /** \result
        *- check that calling order has the correct value of 20
       **/
      CHECK_EQUAL_TEXT(20, calling_order, "The calling order isn't correct, please check the F360_Tracker::Execute function calling order and modify  F360_Tracker_Module_Call.h file");
   }
   /** @}*/
}
