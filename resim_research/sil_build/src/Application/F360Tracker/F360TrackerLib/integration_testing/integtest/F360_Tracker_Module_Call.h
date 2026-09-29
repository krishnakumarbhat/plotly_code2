    /* for a cumulative module call */
#define Up_To_Inputs_Preprocessing_Module_call    1
#define Up_To_Update_TimeStamp_Module_call        2
#define Up_To_TimeUpdate_Module_call              3
#define Up_To_StaticEnvironment_LSC_Module_call   4
#define Up_To_Occlusion_Module_call               5
#define Up_To_Internal_Preprocessing_Module_call  6
#define Up_To_Trailer_Detector_Core_Module_call   7
#define Up_To_PreAssociation_Module_call          8
#define Up_To_Association_Module_call             9
#define Up_To_Clustering_Module_call              10
#define Up_To_Cluster_Grouping_Module_call        11
#define Up_To_TrackGrouping_Module_call           12
#define Up_To_MsmtUpdate_Module_call              13
#define Up_To_NewObjInit_Module_call              14
#define Up_To_PostUpdateTrack_Module_call         15
#define Up_To_TrackClassification_Module_call     16
#define Up_To_TrackValidity_Module_call           17
#define Up_To_TrackDownSelection_Module_call      18
#define Up_To_StaticEnvironment_CWD_Module_call   19
#define Up_To_PostProcessor_Module_call           20

#define Number_Of_Module_Call                     20

    /* for single module call */
#define Inputs_Preprocessing_Module_single_call   100
#define Update_Time_Module_single_call            101
#define TimeUpdate_Module_single_call             102
#define StaticEnvironment_LSC_Module_single_call  103
#define Occlusion_Module_single_call              104
#define Internal_Preprocessing_Module_single_call 105
#define Trailer_Detector_Core_Module_single_call  106
#define PreAssociation_Module_single_call         107
#define Association_Module_single_call            108
#define Clustering_Module_single_call             109
#define Cluster_Grouping_Module_single_call       110
#define TrackGrouping_Module_single_call          111
#define MsmtUpdate_Module_single_call             112
#define NewObjInit_Module_single_call             113
#define PostUpdate_Module_single_call             114
#define TrackClassification_Module_single_call    115
#define TrackValidity_Module_single_call          116
#define TrackDownSelection_Module_single_call     117
#define StaticEnvironment_CWD_Module_single_call  118
#define PostProcessor_Module_single_call          119

#include "f360_occlusion.h"
#include "F360_Interface_Declaration.h"

/*Important note: if the module call isn't updated there will be a compilation error in this file with the title:  (line number):error: cannot convert x to y for example. so to solve this error: identify the line number from the error itself and then go to the function F360_Tracker::Execute in f360_tracker.cpp file and copy the corresponding updated module call in this file and then please modify both single and a cumulative module calls*/

 void  F360_Tracker_Modules_Call(int module_number)
{
    Occlusion_T occlusion(true, calibrations, tracker_info, sensors, timing_info, object_tracks);
    
    tracker_info.time_us = core_info.time_us;
    tracker_info.elapsed_time_s = core_info.elapsed_time_s;
    tracker_info.cnt_loops = core_info.cnt_loops;

   /* for a cumulative module call */
    if (module_number >= Up_To_Inputs_Preprocessing_Module_call && module_number <= Number_Of_Module_Call)
    {
      Inputs_Preprocessing(core_info, host, host_props, timing_info);
    }

   if(module_number >= Up_To_Update_TimeStamp_Module_call && module_number <= Number_Of_Module_Call)
   {
      Update_Relative_Timestamps(core_info.elapsed_time_s, tracker_info, object_tracks, clusters, det_hist);
   }
   if(module_number >= Up_To_TimeUpdate_Module_call && module_number <= Number_Of_Module_Call)
   {
      Time_Update_Tracks(calibrations, core_info.elapsed_time_s, host, sensors, host_props, object_tracks, tracker_info, timing_info);
   }
   if(module_number >= Up_To_StaticEnvironment_LSC_Module_call && module_number <= Number_Of_Module_Call)
   {
      static_env_class.Run_Longi_Stat_Curves(tracker_info, calibrations, object_tracks, timing_info);
   }
   if (module_number >= Up_To_Occlusion_Module_call && module_number <= Number_Of_Module_Call)
   {
      occlusion = Occlusion_T(true, calibrations, tracker_info, sensors, timing_info, object_tracks);
   }
   if(module_number >= Up_To_Internal_Preprocessing_Module_call && module_number <= Number_Of_Module_Call)
   {
      Internal_Preprocessing(host, raw_detection_list, sensors, calibrations, static_env_class.Get_Polynomials(), trailer_detector_core.Get_Trailer_Detector_Output(), host_props, globals, det_hist, object_tracks, detections, sensor_props, clusters, tracker_info, timing_info);
   }
   if(module_number >= Up_To_Trailer_Detector_Core_Module_call && module_number <= Number_Of_Module_Call)
   {
      trailer_detector_core.Run_Trailer_Detector(host, raw_detection_list, detections, sensors, tracker_info.elapsed_time_s);
   }
   if(module_number >= Up_To_PreAssociation_Module_call && module_number <= Number_Of_Module_Call)
   {
      Pre_Association_Track_Management(tracker_info, object_tracks, raw_detection_list, sensors, host, calibrations, static_env_class.Get_Polynomials(), detections, timing_info);
   }
   if(module_number >= Up_To_Association_Module_call && module_number <= Number_Of_Module_Call)
   {
      Detection_To_Track_Association(host, sensors, tracker_info, raw_detection_list, calibrations, static_env_class.Get_Polynomials(), detections, object_tracks, timing_info);
   }
   if(module_number >= Up_To_Clustering_Module_call && module_number <= Number_Of_Module_Call)
   {
      Clustering(calibrations, tracker_info, raw_detection_list, detections, clusters, timing_info);
   }
   if(module_number >= Up_To_Cluster_Grouping_Module_call && module_number <= Number_Of_Module_Call)
   {
      Cluster_Grouping(globals, calibrations, raw_detection_list, tracker_info, detections, det_hist, clusters, timing_info);
   }
   if(module_number >= Up_To_TrackGrouping_Module_call && module_number <= Number_Of_Module_Call)
   {
      Track_Grouping(calibrations, static_env_class.Get_Polynomials(), host, sensors, raw_detection_list, globals, object_tracks, detections, tracker_info, timing_info);
   }
   if(module_number >= Up_To_MsmtUpdate_Module_call && module_number <= Number_Of_Module_Call)
   {
      Measurement_Update_Tracks(calibrations, host, tracker_info, raw_detection_list, sensors, globals, detections, object_tracks, timing_info);
   }
   if(module_number >= Up_To_NewObjInit_Module_call && module_number <= Number_Of_Module_Call)
   {
      Initialize_Tracks(host, host_props, raw_detection_list, det_hist, globals, calibrations, static_env_class.Get_Polynomials(), occlusion, sensors, clusters, object_tracks, tracker_info, detections, timing_info);
   }
   if(module_number >= Up_To_PostUpdateTrack_Module_call && module_number <= Number_Of_Module_Call)
   {
      Post_Update_Track_Adjustments(calibrations, globals, raw_detection_list, host, sensors, tracker_info, detections, object_tracks, timing_info);
   }
   if(module_number >= Up_To_TrackClassification_Module_call && module_number <= Number_Of_Module_Call)
   {
      Track_Classification(host, host_props, tracker_info, &occupancy_grid, globals, raw_detection_list, detections, sensors, calibrations, occlusion, timing_info, object_tracks);
   }
   if(module_number >= Up_To_TrackValidity_Module_call && module_number <= Number_Of_Module_Call)
   {
      Track_Validity(host, tracker_info, calibrations, raw_detection_list, sensors, sensor_props, static_env_class.Get_Polynomials(), object_tracks, timing_info);
   }
   if(module_number >= Up_To_TrackDownSelection_Module_call && module_number <= Number_Of_Module_Call)
   {
      Track_Downselection(host, calibrations, static_env_class.Get_Polynomials(), object_tracks, tracker_info, timing_info);
   }
   if(module_number >= Up_To_StaticEnvironment_CWD_Module_call && module_number <= Number_Of_Module_Call)
   {
      static_env_class.Run_Concrete_Wall_Detector(detections, raw_detection_list, sensors, sensor_props, calibrations, object_tracks, host, timing_info);
   }
   if(module_number >= Up_To_PostProcessor_Module_call && module_number <= Number_Of_Module_Call)
   {
      Sensor_Postprocessing(tracker_info, detections, raw_detection_list, det_hist, clusters, timing_info);
   }

   /* for single module call */
   if (module_number == Inputs_Preprocessing_Module_single_call)
   {
      Inputs_Preprocessing(core_info, host, host_props, timing_info);
   }

   if(module_number == Update_Time_Module_single_call)
   {
      Update_Relative_Timestamps(core_info.elapsed_time_s, tracker_info, object_tracks, clusters, det_hist);
   }
   if(module_number == TimeUpdate_Module_single_call)
   {
      Time_Update_Tracks(calibrations, core_info.elapsed_time_s, host, sensors, host_props, object_tracks, tracker_info, timing_info);
   }
   if(module_number == StaticEnvironment_LSC_Module_single_call)
   {
      static_env_class.Run_Longi_Stat_Curves(tracker_info, calibrations, object_tracks, timing_info);
   }
   if (module_number == Occlusion_Module_single_call)
   {
      occlusion = Occlusion_T(true, calibrations, tracker_info, sensors, timing_info, object_tracks);
   }
   if(module_number == Internal_Preprocessing_Module_single_call)
   {
      Internal_Preprocessing(host, raw_detection_list, sensors, calibrations, static_env_class.Get_Polynomials(), trailer_detector_core.Get_Trailer_Detector_Output(), host_props, globals, det_hist, object_tracks, detections, sensor_props, clusters, tracker_info, timing_info);
   }
   if(module_number == Trailer_Detector_Core_Module_single_call)
   {
      trailer_detector_core.Run_Trailer_Detector(host, raw_detection_list, detections, sensors, tracker_info.elapsed_time_s);
   }
   if(module_number == PreAssociation_Module_single_call)
   {
      Pre_Association_Track_Management(tracker_info, object_tracks, raw_detection_list, sensors, host, calibrations, static_env_class.Get_Polynomials(), detections, timing_info);
   }
   if(module_number == Association_Module_single_call)
   {
      Detection_To_Track_Association(host, sensors, tracker_info, raw_detection_list, calibrations, static_env_class.Get_Polynomials(), detections, object_tracks, timing_info);
   }
   if(module_number == Clustering_Module_single_call)
   {
      Clustering(calibrations, tracker_info, raw_detection_list, detections, clusters, timing_info);
   }
   if(module_number == Cluster_Grouping_Module_single_call)
   {
       Cluster_Grouping(globals, calibrations, raw_detection_list, tracker_info, detections, det_hist, clusters, timing_info);
   }
   if(module_number == TrackGrouping_Module_single_call)
   {
      Track_Grouping(calibrations, static_env_class.Get_Polynomials(), host, sensors, raw_detection_list, globals, object_tracks, detections, tracker_info, timing_info);
   }
   if(module_number == MsmtUpdate_Module_single_call)
   {
      Measurement_Update_Tracks(calibrations, host, tracker_info, raw_detection_list, sensors, globals, detections, object_tracks, timing_info);
   }
   if(module_number == NewObjInit_Module_single_call)
   {
      Initialize_Tracks(host, host_props, raw_detection_list, det_hist, globals, calibrations, static_env_class.Get_Polynomials(), occlusion, sensors, clusters, object_tracks, tracker_info, detections, timing_info);
   }
   if(module_number == PostUpdate_Module_single_call)
   {
      Post_Update_Track_Adjustments(calibrations, globals, raw_detection_list, host, sensors, tracker_info, detections, object_tracks, timing_info);
   }
   if(module_number == TrackClassification_Module_single_call)
   {
      Track_Classification(host, host_props, tracker_info, &occupancy_grid, globals, raw_detection_list, detections, sensors, calibrations, occlusion, timing_info, object_tracks);
   }
   if(module_number == TrackValidity_Module_single_call)
   {
      Track_Validity(host, tracker_info, calibrations, raw_detection_list, sensors, sensor_props, static_env_class.Get_Polynomials(), object_tracks, timing_info);
   }
   if(module_number == TrackDownSelection_Module_single_call)
   {
      Track_Downselection(host, calibrations, static_env_class.Get_Polynomials(), object_tracks, tracker_info, timing_info);
   }
   if(module_number == StaticEnvironment_CWD_Module_single_call)
   {
      static_env_class.Run_Concrete_Wall_Detector(detections, raw_detection_list, sensors, sensor_props, calibrations, object_tracks, host, timing_info);
   }
   if(module_number == PostProcessor_Module_single_call)
   {
      Sensor_Postprocessing(tracker_info, detections, raw_detection_list, det_hist, clusters, timing_info);
   }
}
