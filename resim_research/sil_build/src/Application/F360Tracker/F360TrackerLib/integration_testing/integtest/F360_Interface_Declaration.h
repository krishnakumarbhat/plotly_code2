/* for any change in interface name please change it in here and it will reflect in all test files */
#ifndef F360_INTERFACE_DECLARATION_H
#define F360_INTERFACE_DECLARATION_H

   F360_Host_Tag host = {};
   F360_Core_Info_T core_info = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals ={};
   F360_Host_Props_T host_props={};
   F360_Detection_Props_T detections[MAX_NUMBER_OF_DETECTIONS]={};
   F360_Detection_Hist_T det_hist={};
   F360_TRKR_TIMING_INFO_T    timing_info={};
   F360_Cluster_T     clusters[NUMBER_OF_CLUSTERS]={};
   F360_Calibrations_T    calibrations={};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list={};
   F360_Tracker_Info_T tracker_info = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS]= {};
   F360_Radar_Sensor_Props_T sensor_props[MAX_NUMBER_OF_SENSORS] = {};
   Static_Env_T static_env_class;
   Trailer_Detector_Core trailer_detector_core;
   ocg::OCG_Outputs_T occupancy_grid;
   
#endif