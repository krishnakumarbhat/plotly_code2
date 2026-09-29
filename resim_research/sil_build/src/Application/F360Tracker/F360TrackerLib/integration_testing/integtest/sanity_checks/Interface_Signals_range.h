

#define F360_PI       3.14159265358979323846F   /* pi */
/* range limits definition 
Note: the last part of the name represents the measuring unit
 */
#define max_float_range 3.40282e+38
#define min_float_range -3.40282e+38

#define Max_Host_Speed_M_Per_S 138.8888889F
#define Min_Host_Speed_M_Per_S -138.8888889F
#define Max_Host_Acceleration 30.0F
#define Min_Host_Acceleration -30.0F
#define Max_Host_Yaw_Rate_Rad_M_Per_S_2  1.7453F
#define Min_Host_Yaw_Rate_Rad_M_Per_S_2 -1.7453F
#define Max_Host_curvature_rear_1_Per_M  1.0F 
#define Min_Host_curvature_rear_1_Per_M -1.0F 
#define Max_Host_dist_rear_axle_to_vcs_m 10
#define Min_Host_dist_rear_axle_to_vcs_m 1
#define Max_Sensor_Calibs_ID 4
#define Min_Sensor_Calibs_ID 1
#define Max_Mounting_Position_long_M  2.0F
#define Min_Mounting_Position_long_M  -1.0F
#define Max_Mounting_Position_lat_M  1.5F
#define Min_Mounting_Position_lat_M  -1.5F
#define Max_Mounting_Position_height_M 2.0F
#define Min_Mounting_Position_height_M -2.0F
#define Max_Mounting_Position_azimuth    F360_PI   
#define Min_Mounting_Position_azimuth   -F360_PI   
#define Max_Mounting_Position_elevation  F360_PI
#define Min_Mounting_Position_elevation -F360_PI
#define Max_Sensor_Polarity 1.0F
#define Min_Sensor_Polarity -1.0F
#define Max_Sensor_Calibs_fov_min_az_rad   F360_PI     
#define Min_Sensor_Calibs_fov_min_az_rad  -F360_PI 
#define Max_Sensor_Calibs_fov_max_az_rad   F360_PI
#define Min_Sensor_Calibs_fov_max_az_rad  -F360_PI
#define Max_Sensor_Calibs_min_aliaised_range_rate_M_Per_S -5.0F
#define Min_Sensor_Calibs_min_aliaised_range_rate_M_Per_S -200.0F
#define Max_Sensor_Calibs_rng_rate_interval_widths_M_Per_S  200.0F
#define Min_Sensor_Calibs_rng_rate_interval_widths_M_Per_S  5.0F
#define max_allowed_sensor_vs_tracker_timestamp_diff_us 300000LL
#define min_allowed_sensor_vs_tracker_timestamp_diff_us 0LL
#define Max_Sensors_Number_Of_Valid_Detections 1000U
#define Min_Sensors_Number_Of_Valid_Detections 0U
#define Max_Sensors_Look_ID 4
#define Min_Sensors_Look_ID 0
#define Max_Sensors_Vcs_Velocity_longitudinal_M_Per_S  138.8888889F
#define Min_Sensors_Vcs_Velocity_longitudinal_M_Per_S -138.8888889F
#define Max_Sensors_Vcs_Velocity_lateral_M_Per_S  138.8888889F
#define Min_Sensors_Vcs_Velocity_lateral_M_Per_S -138.8888889F

