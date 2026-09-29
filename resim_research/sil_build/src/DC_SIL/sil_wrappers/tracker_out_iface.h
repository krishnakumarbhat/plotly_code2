#ifndef TRACKER_OUTPUT_INTERFACE_DATA_H
#define TRACKER_OUTPUT_INTERFACE_DATA_H

#include <cinttypes>

#define TRACKER_OUTPUT_INTERFACE_DATA_VERSION (1)
#define NUMBER_OF_GUARDRAILS                  (2)
#define NUMBER_OF_OBJECTS                     (50)

#pragma pack(push, save_pack)
#pragma pack(push, 2)

typedef struct Vector_2d_Points {
   float x; /**< First component of a vector */
   float y; /**< Second component of a vector */
} Vector_2d_Points_T;

typedef enum track_status_values {
   TRACKER_STATUS_INVALID          = (0), /**< does not exist*/
   TRACKER_STATUS_NEW              = (1), /**< unconfirmed */
   TRACKER_STATUS_MATURE           = (2), /**< was confirmed by measurements in more than 1 cycle and a measurement update has been done in current cycle */
   TRACKER_STATUS_COASTED          = (3), /**< not confirmed by measurement in current cycle. Therefore available data is a prediction. */
   TRACKER_NUMBER_OF_OBJECT_STATUS = (4)
} track_status_values_T;

typedef struct SENSOR_FLAG_Tag_Values {
   uint8_t front_left : 1;
   uint8_t front_center : 1;
   uint8_t front_right : 1;
   uint8_t right_center : 1;
   uint8_t rear_right : 1;
   uint8_t rear_center : 1;
   uint8_t rear_left : 1;
   uint8_t left_center : 1;
} SENSOR_FLAG_Tag_Values_T;

typedef struct GUARDRAIL_OUTPUT_Tag_Values {
   float lateral_position;          /**< [m] guardrail lateralPosition */
   float existence_probability;     /**< [0...1] Measure for how likely the guardrail actually exists */
   track_status_values_T status;    /**< available values are INVALID, NEW, MATURE, and COASTED */
   uint8_t age;                     /**< Number of cycles the guardrail has been consecutively in a state other than TRACK_STATUS_INVALID. Saturates at UINT8_MAX. \ref GUARDRAIL_OUTPUT_Tag_Values_T::status */
   uint8_t f_active : 1;            /**< flag indicating that this guardrail is enabled */
   uint8_t f_guardrail_present : 1; /**< flag indicating that a guardrail exists near the sensor */
} GUARDRAIL_OUTPUT_Tag_Values_T;

typedef struct RSDS_OBJECT_ACCURACY_FLT_Tag_Values {
   Vector_2d_Points_T position;     /**< [m] position accuracy */
   Vector_2d_Points_T velocity;     /**< [m/s] velocity accuracy */
   Vector_2d_Points_T acceleration; /**< [m/s^2] acceleration accuracy */
   Vector_2d_Points_T size;         /**< [m] size accuracy */
   float heading;                   /**< [rad] heading accuracy */
   float heading_rate;              /**< [rad/s] heading rate accuracy */
} RSDS_OBJECT_ACCURACY_FLT_Tag_Values_T;

typedef struct OBJECT_COVARIANCE_Tag_Values {
   float velocities_filtered; /**< [m^2/s^2] covariance between longitudinal and lateral velocities, filtered */
   float positions_filtered;  /**< [m^2] covariance between longitudinal and lateral positions, filtered */
} OBJECT_COVARIANCE_Tag_Values_T;

typedef struct OBJECT_VARIANCE_Tag_Values {
   Vector_2d_Points_T velocity_filtered;       /**< [m^2/s^2] Velocity variance, filtered.
                                               Computes the associated detections variance according to the velocity vector of the object */
   Vector_2d_Points_T position_filtered;       /**< [m^2] Position variance, filtered.
                                               Variance of the detection closest to the reference point.*/
   Vector_2d_Points_T size_filtered;           /**< [m^2] Size variance, filtered.
                                               Derived using measured bounding box and predicted/assumed size as raw data, corrected by some factors */
   OBJECT_COVARIANCE_Tag_Values_T covariances; /**< Covariances structure, filtered */
   float heading;                              /**< [rad^2] Heading variance.
                                                   Derived using a variance propagation of the velocity variance and the objects updated velocity. */
} OBJECT_VARIANCE_Tag_Values_T;

typedef struct RSDS_OBJECT_INNOVATION_OUT_FLT_Tag_Values {
   Vector_2d_Points_T position;     /**< [m] position innovation */
   Vector_2d_Points_T velocity;     /**< [m/s] velocity innovation */
   Vector_2d_Points_T acceleration; /**< [m/s^2] acceleration innovation */
   Vector_2d_Points_T size;         /**< [m] size innovation */
   float heading;                   /**< [rad] heading innovation */
   float heading_rate;              /**< [rad/s] heading rate innovation */
} RSDS_OBJECT_INNOVATION_OUT_FLT_Tag_Values_T;

typedef enum curvi_coordinates_calc_method_Tag_Values {
   TRACKER_CURVI_COORDINATES_UNKNOWN                  = (0), /**< default value */
   TRACKER_CURVI_COORDINATES_BASED_ON_VCS             = (1), /**< neither snail train nor distance based curvature is used. Curvi coordinates are same as vcs coordinates*/
   TRACKER_CURVI_COORDINATES_SNAIL_TRAIL              = (2), /**< nearest snail trail point is used  */
   TRACKER_CURVI_COORDINATES_DISTANCE_BASED_CURVATURE = (3)  /**< distance based curvature is used*/
} curvi_coordinates_calc_method_Tag_Values_T;

typedef enum reference_position_Tag_Values {
   TRACKER_REFERENCE_POSITION_FRONT_LEFT  = (0), /**< 0*/
   TRACKER_REFERENCE_POSITION_FRONT       = (1), /**< 1*/
   TRACKER_REFERENCE_POSITION_FRONT_RIGHT = (2), /**< 2*/
   TRACKER_REFERENCE_POSITION_RIGHT       = (3), /**< 3*/
   TRACKER_REFERENCE_POSITION_REAR_RIGHT  = (4), /**< 4*/
   TRACKER_REFERENCE_POSITION_REAR        = (5), /**< 5*/
   TRACKER_REFERENCE_POSITION_REAR_LEFT   = (6), /**< 6*/
   TRACKER_REFERENCE_POSITION_LEFT        = (7), /**< 7*/
   TRACKER_REFERENCE_POSITION_INVALID     = (8)  /**< 8*/
} reference_position_Tag_Values_T;

typedef struct OBJECT_CLASS_PROBABILITY_Tag_Values {
   float probability_unknown;    //!< probability that the object class is unknown
   float probability_pedestrian; //!< probability that the object is a pedestrian
   float probability_2wheel;     //!< probability that the object is a 2wheel
   float probability_car;        //!< probability that the object is a car
   float probability_truck;      //!< probability that the object is a truck
} OBJECT_CLASS_PROBABILITY_Tag_Values_T;

typedef enum object_class_Tag_Values {
   TRACKER_OBJECT_CLASS_UNKNOWN    = (0), /**< 0*/
   TRACKER_OBJECT_CLASS_PEDESTRIAN = (1), /**< 1*/
   TRACKER_OBJECT_CLASS_2WHEEL     = (2), /**< 2*/
   TRACKER_OBJECT_CLASS_CAR        = (3), /**< 3*/
   TRACKER_OBJECT_CLASS_TRUCK      = (4)  /**< 4*/
} object_class_Tag_Values_T;

typedef struct Version_Number_Tag_Value {
   uint16_t year : 5; /**< Year of the release */ /* PRQA S 0635 */                                                            /* The structure has to have a size of 2 bytes. At the moment this is more important than being portable */
   uint16_t month : 4; /**< Month of the release */ /* PRQA S 0635 */                                                          /* The structure has to have a size of 2 bytes. At the moment this is more important than being portable */
   uint16_t day : 5; /**< Day of the release */ /* PRQA S 0635 */                                                              /* The structure has to have a size of 2 bytes. At the moment this is more important than being portable */
   uint16_t iteration : 2; /**< If several versions are released on the same day the iteration is changed */ /* PRQA S 0635 */ /* The structure has to have a size of 2 bytes. At the moment this is more important than being portable */
} Version_Number_Tag_Value_T;

typedef enum tracker_cal_type_Tag_Values {
   F360_TRACKER_CAL_TYPE_INVALID   = 255,
   F360_TRACKER_CAL_TYPE_PRIMARY   = 1,
   F360_TRACKER_CAL_TYPE_SECONDARY = 2
} tracker_cal_type_Tag_Values_T;

typedef struct TRACKER_ERRORS_Tag_Values {
   unsigned int configuration_is_nullpointer : 1;                           /**< pointer does not refer to a valid object */
   unsigned int tracker_output_is_nullpointer : 1;                          /**< pointer does not refer to a valid object */
   unsigned int radar_parameters_is_nullpointer : 1;                        /**< pointer does not refer to a valid object */
   unsigned int vehicle_data_is_nullpointer : 1;                            /**< pointer does not refer to a valid object */
   unsigned int detections_is_nullpointer : 1;                              /**< pointer does not refer to a valid object */
   unsigned int tracks_is_nullpointer : 1;                                  /**< pointer does not refer to a valid object */
   unsigned int objects_is_nullpointer : 1;                                 /**< pointer does not refer to a valid object */
   unsigned int calibration_is_nullpointer : 1;                             /**< pointer does not refer to a valid object */
   unsigned int radar_mounting_not_available : 1;                           /**< radar mounting information was not set */
   unsigned int tracker_internals_is_nullpointer : 1;                       /**< pointer does not refer to a valid object */
   unsigned int guardrail_is_nullpointer : 1;                               /**< pointer does not refer to a valid object */
   unsigned int incorrect_calibration_curvature_distance_constant : 1;      /**< k_vp_host_curvature_distance_constant_slow or k_vp_host_curvature_distance_constant_fast is zero -> causes division by zero*/
   unsigned int incorrect_object_class_estimation : 1;                      /**< The sum of an objects' class probabilities is not 1. This is an subsequent error happening if objects are looking strange. */
   unsigned int incorrect_calibration_object_classification : 1;            /**< the calibration for the object classification module is not set correctly */
   unsigned int incorrect_std_heading_estimation : 1;                       /**< the counter variable for standard heading estimation is corrupted */
   unsigned int v_un_is_zero : 1;                                           /**< the unambiguous range rate cannot be zero */
   unsigned int incorrect_object_heading_estimation : 1;                    /**< heading innovation covariance matrix is corrupted */
   unsigned int incorrect_calibration_cone_of_silence : 1;                  /**< k_pd_cone_of_silence_min_cos_az < 0, then check in classify motion status doesn't work */
   unsigned int init_wrong_mount_loc : 1;                                   /**< Initialization temp_cals->k_in_sensor_mount_loc != p_configuration->mount_loc */
   unsigned int init_wrong_trigger_mount_loc : 1;                           /**< Initialization temp_cals->c_gp_trigger_mount_loc != p_configuration->mount_loc */
   unsigned int det_iterator_init_out_of_bounds : 1;                        /**< the given sorted detections range for initializing the iterator is corrupted */
   unsigned int radar_parameters_az_pol_faulty : 1;                         /**< The azimuth polarity of a radar can ONLY be -1 or +1 */
   unsigned int vehicle_data_rear_axle_position_not_set : 1;                /**< the rear axle position needs to be set */
   unsigned int incorrect_calibration_in_vehicle_processing : 1;            /**< the calibration for the vehicle processing module is not set correctly */
   unsigned int scan_index_non_consecutive : 1;                             /**< The scan index is not the last one +1 for any sensor*/
   unsigned int radar_parameters_dwell_type_out_of_range : 1;               /**< The dwell type (look type) can only be 0 or 1 */
   unsigned int radar_parameters_alignment_not_set : 1;                     /**< The alignment value cannot equal its initialization value of INFINITY */
   unsigned int radar_parameters_v_un_not_set : 1;                          /**< The v_un value cannot equal its initialization value of -INFINITY */
   unsigned int incorrect_calibration_guardrail : 1;                        /**< guardrail detector is not calibrated correctly */
   unsigned int incorrect_calibration_in_rsds_tracker : 1;                  /**< the calibration for the RSDS Tracker module is not set correctly */
   unsigned int v_un_inconsistant_with_dets_rr : 1;                         /**< The difference of the max and the min range rate of all detections is bigger than the received v_un */
   unsigned int detections_azimuth_confidence_out_of_range : 1;             /**< The DETECTION_FLT_T::azimuth_confidence is bigger or equal to \ref DET_AZ_CONFIDENCE_NUMBER_OF_LEVELS */
   unsigned int max_range_current_look_inconsistant_with_dets_r : 1;        /**< The maximum range of all detections is bigger than the received max_range_current_look */
   unsigned int tracker_cal_type_not_set : 1;                               /**< c_tracker_cal_type is still set to TRACKER_CAL_TYPE_INVALID*/
   unsigned int trigonometric_function_called_with_non_triangle : 1;        /**< A function in ac_trigonometry has been called with data not suiting a triangle */
   unsigned int calculated_time_stamp_delta_out_of_range : 1;               /**< the calculated delta in time stamps is not within the expected region */
   unsigned int runtime_parameter_out_of_range : 1;                         /**< a runtime parameter was set with a value which is outside of the allowed range */
   unsigned int runtime_parameter_mandatory_was_not_set : 1;                /**< a mandatory runtime parameter was not set */
   unsigned int runtime_parameter_prohibited_was_set : 1;                   /**< a runtime parameter which is prohibited to set, was set */
   unsigned int incorrect_calibration_in_process_Detections : 1;            /**< the calibration for the process detections module is not set correctly */
   unsigned int tracker_can_not_run : 1;                                    /**< Error flags are set that prevent the tracker from running*/
   unsigned int matching_internal_detection_does_not_exist : 1;             /**< some module tried to get tracker internal detection data for a detection from the history buffer*/
   unsigned int tracker_configuration_not_set : 1;                          /* The structure CONFIGURATION_T which is passed to initialization() has faulty entries */
   unsigned int rolling_count_transform_detections_failed : 1;              /**< rolling_count_transformDetections in RADAR_PARAMS_FLT_T has not been increased. This indicates that transformDetections() has
                                                                             * not been run. */
   unsigned int rolling_count_process_detections_failed : 1;                /**< rolling_count_processDetections in RADAR_PARAMS_FLT_T has not been increased. This indicates that processDetections() has not
                                                                             * been run. */
   unsigned int rolling_count_tracklet_tracker_failed : 1;                  /**< rolling_count_trackletTracker in RADAR_PARAMS_FLT_T has not been increased. This indicates that trackletTracker() has not been
                                                                             * run. */
   unsigned int rolling_count_vehicle_processings_failed : 1;               /**< rolling_count_vehicle_data in VEHICLE_DATA_FLT_T has not been increased. This indicates that vehicleProcessing() has not been
                                                                             * run. */
   unsigned int vehicle_data_host_vehicle_length_not_set : 1;               /**< the host vehicle length needs to be set */
   unsigned int vehicle_data_host_vehicle_width_not_set : 1;                /**< the host vehicle width needs to be set */
   unsigned int vehicle_data_lane_width_external_not_set : 1;               /**< lane_width_external in vehicle data needs to be set */
   unsigned int vehicle_data_max_value_mastertime_not_set : 1;              /**< max_value_mastertime in vehicle data needs to be set */
   unsigned int detection_with_range_below_min_present : 1;                 /**< Found a detections with a range below a minimum */
   unsigned int inconsistent_rear_axle_position_vs_host_vehicle_length : 1; /**< absolute value of rear axle position exceeds host vehicle length */
   unsigned int v_un_inconsistent_with_min_max_rr_values : 1;               /**< the v_un value needs to be the difference of the maximum and minimum unambiguous range rate of the current look*/
   unsigned int inconsistent_min_max_range_rate_current_look : 1;           /**< the maximum and minimum unambiguous range rate of the current look are inconsistent, min>max */
} TRACKER_ERRORS_Tag_Values_T;

typedef struct TRACKER_STATUS_Tag_Values {
   float max_measured_range_current_look;      /**< [m] maximum measured range current look. In case detection saturation is detected the measured maximum range is shown, the sensors
                                                * range coverage (all looks) otherwise. */
   float time_stamp_delta;                     /**< [s] difference between the time stamp of the current and previous call of tracker */
   int32_t scan_index;                         /**< scan index of tracking sensor (also named 'master' or 'own)*/
   TRACKER_ERRORS_Tag_Values_T tracker_errors; /**< structure holding Tracker error flags */

   tracker_cal_type_Tag_Values_T active_tracker_cal_type; /**< The tracker can be started using primary or secondary cals. 1=primary, 2=secondary, 255=invalid. This way the features can check if
                                                           * the expected cal type is active.*/

   Version_Number_Tag_Value_T tracker_version; /**< this tracker SW release version (date and iteration)*/

   uint8_t rolling_count_fusionTracker; /**< rolling count for the tracker. Will be increased by one each time the tracker is called. Used for debugging.*/

   uint8_t f_reset : 1;                              /**< flag indicating that the tracker was reset in the current cycle*/
   uint8_t f_updated : 1;                            /**< flag indicating the full tracker output has been updated. This includes vehicle data and objects as well as detections.*/
   uint8_t f_tracking_behind_guardrails_enabled : 1; /**< flag indicating that the tracker may produce objects located behind guardrails.*/
} TRACKER_STATUS_Tag_Values_T;

typedef struct TRACKER_OUTPUT_Tag_Values {
   TRACKER_STATUS_Tag_Values_T tracker_status;      // < status of the tracker >
   uint8_t major_version;                           // < major version of the tracker >
   uint8_t minor_version;                           // < minor version of the tracker >
   float max_measured_range_current_look;           // [m] maximum measured range current look. In case detection saturation is detected the measured maximum																	* range is shown, the sensors range coverage (all looks) otherwise. */
   int8_t id[NUMBER_OF_OBJECTS];                    // For compatibility with older features: id=index+1. the id field will be removed, do not use. */
   track_status_values_T status[NUMBER_OF_OBJECTS]; // <Status of each track>
   uint8_t age[NUMBER_OF_OBJECTS];                  /**< number of scans this object has existed */
   uint8_t stage_age[NUMBER_OF_OBJECTS];            /**< number of consecutive scans this object has had the same status */
   float existence_probability[NUMBER_OF_OBJECTS];  /**< existence probability of the object */
   float eclipse_value[NUMBER_OF_OBJECTS];          /**< eclipse value of the object */

   float priority_value[NUMBER_OF_OBJECTS];   /**< priority value of the object = lower value indicates higher priority */
   int8_t priority_number[NUMBER_OF_OBJECTS]; /**< one-based index of rank of this object (1 is highest) according to priority value */

   float vcs_long_posn[NUMBER_OF_OBJECTS];    /**< [m] longitudinal position of each object in vcs coordinates */
   float vcs_long_vel[NUMBER_OF_OBJECTS];     /**< [m/s] longitudinal velocity of each object in vcs coordinates */
   float vcs_long_accel[NUMBER_OF_OBJECTS];   /**< [m/s^2] longitudinal acceleration of each object in vcs coordinates */
   float vcs_lat_posn[NUMBER_OF_OBJECTS];     /**< [m] lateral position of each object in vcs coordinates */
   float vcs_lat_vel[NUMBER_OF_OBJECTS];      /**< [m/s] lateral velocity of each object in vcs coordinates */
   float vcs_lat_accel[NUMBER_OF_OBJECTS];    /**< [m/s^2] lateral acceleration of each object in vcs coordinates */
   float vcs_long_vel_rel[NUMBER_OF_OBJECTS]; /**< [m/s] relative longitudinal velocity of each object in vcs coordinates */
   float vcs_lat_vel_rel[NUMBER_OF_OBJECTS];  /**< [m/s] relative lateral velocity of each object in vcs coordinates */
   uint8_t f_stationary[NUMBER_OF_OBJECTS];   /**< flag indicating object is currently stationary */
   uint8_t f_moveable[NUMBER_OF_OBJECTS];     /**< flag indicating object was non-stationary before */

   float speed[NUMBER_OF_OBJECTS];            /**< [m/s] speed of the object in vcs coordinates */
   float tangential_accel[NUMBER_OF_OBJECTS]; /**< [m/s^2] tangential acceleration of the object in vcs coordinates */
   float heading[NUMBER_OF_OBJECTS];          /**< [rad] orientation of the bounding box of the object in vcs coordinates */
   float heading_rate[NUMBER_OF_OBJECTS];     /**< [rad/s] heading rate of the object in vcs coordinates */

   float object_distance[NUMBER_OF_OBJECTS]; /**< [m] minimum distance from the border of the host vehicle to the border of each object */

   float length[NUMBER_OF_OBJECTS];                                                   /**< [m] length of the target vehicle */
   float width[NUMBER_OF_OBJECTS];                                                    /**< [m] width of the target vehicle */
   object_class_Tag_Values_T object_class[NUMBER_OF_OBJECTS];                         /**< classification of the object based on target size */
   OBJECT_CLASS_PROBABILITY_Tag_Values_T object_class_probability[NUMBER_OF_OBJECTS]; /**< probability that the object is of a specific class */
   reference_position_Tag_Values_T ref_position[NUMBER_OF_OBJECTS];                   /**< enumeration indicating where the reference point is on the target vehicle */
   float ref_long_posn[NUMBER_OF_OBJECTS];                                            /**< [m] longitudinal position of the reference point in vcs coordinates */
   float ref_long_vel[NUMBER_OF_OBJECTS];                                             /**< [m/s] longitudinal velocity of the reference point in vcs coordinates */
   float ref_long_accel[NUMBER_OF_OBJECTS];                                           /**< [m/s^2] longitudinal acceleration of the reference point in vcs coordinates */
   float ref_lat_posn[NUMBER_OF_OBJECTS];                                             /**< [m] lateral position of the reference point in vcs coordinates */
   float ref_lat_vel[NUMBER_OF_OBJECTS];                                              /**< [m/s] lateral velocity of the reference point in vcs coordinates */
   float ref_lat_accel[NUMBER_OF_OBJECTS];                                            /**< [m/s^2] lateral acceleration of the reference point in vcs coordinates */
   uint8_t f_ref_updated[NUMBER_OF_OBJECTS];                                          /**< flag indicating if reference point has been updated */

   float curvi_long_posn[NUMBER_OF_OBJECTS];                                                    /**< [m] longitudinal position of the object in curvi coordinates */
   float curvi_long_vel[NUMBER_OF_OBJECTS];                                                     /**< [m/s] longitudinal velocity of the object in curvi coordinates */
   float curvi_long_accel[NUMBER_OF_OBJECTS];                                                   /**< [m/s^2] longitudinal acceleration of the object in curvi coordinates */
   float curvi_lat_posn[NUMBER_OF_OBJECTS];                                                     /**< [m] lateral position of the object in curvi coordinates */
   float curvi_lat_vel[NUMBER_OF_OBJECTS];                                                      /**< [m/s] lateral velocity of the object in curvi coordinates */
   float curvi_lat_accel[NUMBER_OF_OBJECTS];                                                    /**< [m/s^2] lateral acceleration of the object in curvi coordinates */
   float curvi_long_vel_rel[NUMBER_OF_OBJECTS];                                                 /**< [m/s] relative longitudinal velocity of the object in curvi coordinates */
   float curvi_lat_vel_rel[NUMBER_OF_OBJECTS];                                                  /**< [m/s] relative lateral velocity of the object in curvi coordinates */
   float curvi_heading[NUMBER_OF_OBJECTS];                                                      /**< [rad] heading of the object in curvi coordinates */
   curvi_coordinates_calc_method_Tag_Values_T curvi_coordinates_calc_method[NUMBER_OF_OBJECTS]; /**< enum specifying which method was used to determine the curvi coordinates of the object */

   RSDS_OBJECT_INNOVATION_OUT_FLT_Tag_Values_T innovation[NUMBER_OF_OBJECTS]; /**< The filtered difference between the predicted and measured state of the object. It will get bigger if the
                                                                               * object does not follow the prediction. */
   OBJECT_VARIANCE_Tag_Values_T variance[NUMBER_OF_OBJECTS];                  /**< Variance derived from the raw detections and the objects updated state */
   RSDS_OBJECT_ACCURACY_FLT_Tag_Values_T accuracy[NUMBER_OF_OBJECTS];         /**< The accuracy shall describe how close the tracked object is to the real object. */

   uint8_t f_just_merged[NUMBER_OF_OBJECTS];                   /**< this object has been merged in current cycle */
   int8_t f_just_merged_with[NUMBER_OF_OBJECTS];               /**< id of the object this object was merged with. -1 if the object has not been merged in the current cycle or if the
                                                                * object it has been merged with has been created in the current cycle. In this case, the merged object never
                                                                * has been reported and the ID does not matter for features. It still may be of interest to know that a merge
                                                                * took place, therefore f_just_merged will be TRUE. */
   float object_distance_h[NUMBER_OF_OBJECTS];                 /**< [m] minimum distance from the border of the host vehicle to the border of each object */
   float obstruction_probability[NUMBER_OF_OBJECTS];           /**< probability that object is behind obstruction */
   float rangeRegionObstructed_probability[NUMBER_OF_OBJECTS]; /**< This signal is not computed anymore. (ABX-1572) The value will always be 0.0f */

   float filtered_amplitude[NUMBER_OF_OBJECTS]; /**< [dBsm] current filtered amplitude (max of tracklets) */

   uint8_t f_reflection[NUMBER_OF_OBJECTS];                        /**< This object is probably a reflection */
   SENSOR_FLAG_Tag_Values_T origin_sensor[NUMBER_OF_OBJECTS];      /**< information about which sensors' detections were used to create/update object */
   GUARDRAIL_OUTPUT_Tag_Values_T guardrails[NUMBER_OF_GUARDRAILS]; /* < Guardrail data to be used by functions. Index 0:left, index 1: right */
} TRACKER_OUTPUT_Tag_Values_T;
#pragma pack(pop, save_pack)
#endif
