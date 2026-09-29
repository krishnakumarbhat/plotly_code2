/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef RADAR_PARAMS_FLT_T_H
#define RADAR_PARAMS_FLT_T_H

#include "reuse.h"
#include "tracker_constants_selector.h"
#include "mounting_location_T.h"
#include "DETECTION_FLT_T.h"
#include "DWELL_TYPE_T.h"
#include "VEHICLE_DATA_RADAR_PARAMETER_FLT_T.h"
#include "sort_det_T.h"
#include "FOV_LINES_T.h"
#include "ROLLING_COUNT_T.h"
#include "exclusion_zone_t.h"
#include "noise_degradation_level_t.h"

/**
 * Each RADAR_PARAMS_FLT_T stores a history of RADAR_PARAMETER_BUFFER_SIZE cycles.
 * This structure holds the data of this history buffer.
 *
 *
 */
typedef struct
{
   float32_T time_stamp_delta; /**< [s] difference between the timestamp of current and history data set */
   float32_T v_un; /**< unambiguous range rate in the history cycle */
   float32_T max_range;  /**< maximum measurable range in the history cycle */
} RADAR_PARAMS_HISTORY_FLT_T;

typedef enum
{
   align_quality_factor_tracker_T_UNKNOWN,
   align_quality_factor_tracker_T_ACCURATE,
   align_quality_factor_tracker_T_FAULTY,
   align_quality_factor_tracker_T_INACCURATE
} align_quality_factor_tracker_T;

typedef struct
{
   /** @name Persistent
   * This section contains all Tracker Inputs that have been set by calling \ref initialization. These have not to be set within each cycle.
   * \ref CONFIGURATION has been used by \ref initialization to set members of this group
   */
   /**@{*/

   float32_T                          azimuth_polarity;                 /**< +/- 1 (multiplied by detection azimuth produces true azimuth) */

   float32_T                          boresight_angle;                  /**< [rad] boresight angle in VCS-aligned coordinates centered at sensor */

   float32_T                          cos_boresight_angle;              /**< cosine of boresight angle */
   float32_T                          sin_boresight_angle;              /**< sine of boresight angle */

   float32_T                          long_posn;                        /**< [m] longitudinal position of the sensor in VCS coordinates */

   float32_T                          lat_posn;                         /**< [m] lateral position of the sensor in VCS coordinates */

   mounting_location_T                mount_loc;                        /**< sensor mounting location code from mounting_location_T */
   /**@}*/

   /** @name Inputs
   * This section contains all Tracker Inputs that have to be filled outside before the Tracker is being called.
   */
   /**@{*/

   float32_T                          v_un;                             /**< [m/s] unambiguous range rate (radial velocity) */

   float32_T                          max_range_current_look;           /**< [m] maximum nominal range of current look due to the used bandwidth */

   float32_T                          max_range_rate_current_look;      /**< [m/s] maximum unambiguous range rate of the current look */

   float32_T                          min_range_rate_current_look;      /**< [m/s] minimum unambiguous range rate of the current look */

   float32_T                          range_coverage;                   /**< [m] the maximum possible range of all bandwidthes and looks. This information is not known at configuration time, and thus
                                                                         * changes during the first cycles until the look with maximum range is found, i.e., range_coverage is the maximum of all max_range_rate_current_look */

   float32_T                          alignment;                        /**< [rad] correction (relative to boresight) which needs to be added to measured azimuth to obtain true azimuth */

   align_quality_factor_tracker_T     dynamic_alignment_quality_factor; /**< quality factor of alignment */

   uint16_t                           scan_index;                       /**< counter which increments each time there is a new look */

   int32_t                            look_id;                          /**< look id of detections */

   uint32_t                           time_stamp;                       /**< [ms] time stamp of current set of data */

   DWELL_TYPE_T                       dwell_type;                       /**< look type of detections */

   uint8_t                            f_detection_snr;                  /**< flag indicating that the detections detection_snr variable is filled for this sensor */

   uint8_t                            f_active;                         /**< flag indicating this sensor is active (some implementations do not have NUMBER_OF_SENSORS sensors) */

   Noise_Degradation_Level_T          noise_degradation_level;          /**< 2 bit signal indicating that the noise floor of the received radar signal increased above average */

   /**@}*/

   /** @name IO
    * This section contains all sub structs that are both Tracker Input and Output (see definition of that struct to determine inputs and outputs).
    */
   /**@{*/
   VEHICLE_DATA_RADAR_PARAMETER_FLT_T vehicle_data_synchronized;  /**< vehicle data at the time of radar measurement */
   /**@}*/

   /** @name Outputs
    * This section contains all Tracker Outputs that are being generated by the Tracker and may be used by other functions as well.
    */
   /**@{*/
   sort_det_T                         sort_det[NUMBER_OF_DETECTIONS];  /**< array for holding indices of detections sorted by range */

   DETECTION_FLT_T                   *detection_list;                  /**< pointer to list of detections gathered by this sensor*/

   float32_T                          long_vel;                        /**< [m/s] longitudinal velocity of the sensor in VCS coordinates */
   float32_T                          lat_vel;                         /**< [m/s] lateral velocity of the sensor in VCS coordinates */
   float32_T                          local_long_vel;                  /**< [m/s] longitudinal velocity of the sensor in local coordinates */
   float32_T                          local_lat_vel;                   /**< [m/s] lateral velocity of the sensor in local coordinates */

   float32_T                          time_stamp_delta;                /**< [s] difference between time stamp of the current and previous sets of data */

   float32_T                          max_measured_range_current_look; /**< [m] maximum measured range current look */

   uint32_t                           time_stamp_abs;                  /**< [ms] time stamp of the current set of data without overflow*/

   uint8_t                            idx;                             /**< zero-based unique id number. Equals the index into the radar parameters array. */
   /**@}*/

   /** @name Internals
    * This section contains all Tracker Internals that may be subject to changes at any time and shall not be used by any other function!
    */
   /**@{*/
   RADAR_PARAMS_HISTORY_FLT_T         radar_parameter_history[RADAR_PARAMETER_BUFFER_SIZE]; /**< historic radar parameter values from previous cycles */

   FOV_LINES_T                        single_sensor_fov_lines;                              /**< single sensor FOV lines */

   Exclusion_Zone_T                   single_sensor_exclusion_zones;                        /**< single sensor exclusion zone info */

   Vector_2d_T                        velocity_variance;                                    /**< [m2/s2] variance of the sensors' over the ground velocity*/

   float32_T                          v_un_last;                                            /**< [m/s] unambiguous range rate (velocity) of the last cycle */

   uint16_t                           scan_index_last;                                      /**< scan index of the last look */
   uint32_t                           time_stamp_previous;                                  /**< [ms] time stamp of the previous set of data */

   DWELL_TYPE_T                       dwell_type_last;                                      /**< look type of the last set of detections */

   ROLLING_COUNT_T                    rolling_count_transformDetections;                    /**< rolling count for transformDetections. Will be increased by one each time transformDetections is called.
                                                                                             * Used for debugging. */
   ROLLING_COUNT_T                    rolling_count_processDetections;                      /**< rolling count for processDetections. Will be increased by one each time processDetections is called.
                                                                                             * Used for debugging. */
   ROLLING_COUNT_T                    rolling_count_trackletTracker;                        /**< rolling count for trackletTracker. Will be increased by one each time trackletTracker is called.
                                                                                             * Used for debugging. */
   /**@}*/
} RADAR_PARAMS_FLT_T;

#endif
