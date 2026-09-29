/* modify any of the following in case of interface changes */
#define input_init()    init_host_T( host);\
   init_core_T(core_info); \
   init_sensors_T(sensors);\
   init_sensor_calib_T(sensors);\
   init_globals_T(globals);\
   init_host_props_T(host_props);\
   init_det_props_T(detections);\
   init_det_hist_T(det_hist);\
   init_timing_info_T(timing_info);\
   init_clusters_T(clusters);\
   Initialize_Tracker_Calibrations(calibrations);\
   init_raw_detect_T(raw_detection_list);\
   init_sensor_props_T(sensor_props);\
   raw_detection_list.number_of_valid_detections =85;\
   init_object_tracks_T(object_tracks);\
   init_tracker_info_T(tracker_info,clusters,object_tracks);\
   init_static_env_class_T(static_env_class);\
   init_trailer_detector_core(trailer_detector_core);\
   init_occupancy_grid_T(occupancy_grid);\
   //init_underdrivability_T(underdrivability);
   