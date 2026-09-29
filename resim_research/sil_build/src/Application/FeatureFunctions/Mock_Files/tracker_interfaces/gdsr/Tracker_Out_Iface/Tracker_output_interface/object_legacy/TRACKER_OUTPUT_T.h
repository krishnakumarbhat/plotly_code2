/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef TRACKER_OUTPUT_T_H
#define TRACKER_OUTPUT_T_H

#include "reuse.h"
#include "tracker_constants_selector.h"
#include "track_status_T.h"
#include "object_class_T.h"
#include "OBJECT_CLASS_PROBABILITY_T.h"
#include "reference_position_T.h"
#include "RSDS_OBJECT_ACCURACY_T.h"
#include "OBJECT_VARIANCE_T.h"
#include "curvi_coordinates_calc_method_T.h"
#include "GUARDRAIL_OUTPUT_T.h"
#include "tracker_errors_T.h"

#include "RSDS_OBJECT_INNOVATION_OUT_FLT_T.h"

#include "VERSION_NUMBER_T.h"

#include "tracker_cal_type_T.h"
#include "TRACKER_STATUS_T.h"

#include "SENSOR_FLAG_T.h"

/**
 * This structure holds the output data from the tracker
 */
#ifdef _MSC_VER
#pragma warning(disable: 4121)
#endif
typedef struct TRACKER_OUTPUT_Tag
{
   TRACKER_STATUS_T                 tracker_status;                                       /**< status of the tracker */

   uint8_t                          major_version;                                        /**< major version of the tracker */
   uint8_t                          minor_version;                                        /**< minor version of the tracker */

   float32_T                        max_measured_range_current_look;                      /**< [m] maximum measured range current look. In case detection saturation is detected the measured maximum
                                                                                            * range is shown, the sensors range coverage (all looks) otherwise. */
#ifndef TRACKER_SUPPRESS_ID
   int8_t                           id[NUMBER_OF_OBJECTS];                                /**< For compatibility with older features: id=index+1. the id field will be removed, do not use. */
#endif
   uint8_t                          distinct_id[NUMBER_OF_OBJECTS];                       /**< Distinctive id of this object. A distinctive id is not connected in any way to the index of an object.*/

   track_status_T                   status[NUMBER_OF_OBJECTS];                            /**< Status of each track */
   uint8_t                          age[NUMBER_OF_OBJECTS];                               /**< number of scans this object has existed */
   uint8_t                          stage_age[NUMBER_OF_OBJECTS];                         /**< number of consecutive scans this object has had the same status */
   float32_T                        existence_probability[NUMBER_OF_OBJECTS];             /**< existence probability of the object */
   float32_T                        eclipse_value[NUMBER_OF_OBJECTS];                     /**< eclipse value of the object */

   float32_T                        priority_value[NUMBER_OF_OBJECTS];                    /**< priority value of the object = lower value indicates higher priority */
   int8_t                           priority_number[NUMBER_OF_OBJECTS];                   /**< one-based index of rank of this object (1 is highest) according to priority value */

   float32_T                        vcs_long_posn[NUMBER_OF_OBJECTS];                     /**< [m] longitudinal position of each object in vcs coordinates */
   float32_T                        vcs_long_vel[NUMBER_OF_OBJECTS];                      /**< [m/s] longitudinal velocity of each object in vcs coordinates */
   float32_T                        vcs_long_accel[NUMBER_OF_OBJECTS];                    /**< [m/s^2] longitudinal acceleration of each object in vcs coordinates */
   float32_T                        vcs_lat_posn[NUMBER_OF_OBJECTS];                      /**< [m] lateral position of each object in vcs coordinates */
   float32_T                        vcs_lat_vel[NUMBER_OF_OBJECTS];                       /**< [m/s] lateral velocity of each object in vcs coordinates */
   float32_T                        vcs_lat_accel[NUMBER_OF_OBJECTS];                     /**< [m/s^2] lateral acceleration of each object in vcs coordinates */
   float32_T                        vcs_long_vel_rel[NUMBER_OF_OBJECTS];                  /**< [m/s] relative longitudinal velocity of each object in vcs coordinates */
   float32_T                        vcs_lat_vel_rel[NUMBER_OF_OBJECTS];                   /**< [m/s] relative lateral velocity of each object in vcs coordinates */

   uint8_t                          f_stationary[NUMBER_OF_OBJECTS];                      /**< flag indicating object is currently stationary */
   uint8_t                          f_moveable[NUMBER_OF_OBJECTS];                        /**< flag indicating object was non-stationary before */

   float32_T                        speed[NUMBER_OF_OBJECTS];                             /**< [m/s] speed of the object in vcs coordinates */
   float32_T                        tangential_accel[NUMBER_OF_OBJECTS];                  /**< [m/s^2] tangential acceleration of the object in vcs coordinates */
   float32_T                        heading[NUMBER_OF_OBJECTS];                           /**< [rad] orientation of the bounding box of the object in vcs coordinates */
   float32_T                        heading_rate[NUMBER_OF_OBJECTS];                      /**< [rad/s] heading rate of the object in vcs coordinates */

   float32_T                        object_distance[NUMBER_OF_OBJECTS];                   /**< [m] minimum distance from the border of the host vehicle to the border of each object */

   float32_T                        length[NUMBER_OF_OBJECTS];                            /**< [m] length of the target vehicle */
   float32_T                        width[NUMBER_OF_OBJECTS];                             /**< [m] width of the target vehicle */
   object_class_T                   object_class[NUMBER_OF_OBJECTS];                      /**< classification of the object based on target size */
   OBJECT_CLASS_PROBABILITY_T       object_class_probability[NUMBER_OF_OBJECTS];          /**< probability that the object is of a specific class */

   reference_position_T             ref_position[NUMBER_OF_OBJECTS];                      /**< enumeration indicating where the reference point is on the target vehicle */
   float32_T                        ref_long_posn[NUMBER_OF_OBJECTS];                     /**< [m] longitudinal position of the reference point in vcs coordinates */
   float32_T                        ref_long_vel[NUMBER_OF_OBJECTS];                      /**< [m/s] longitudinal velocity of the reference point in vcs coordinates */
   float32_T                        ref_long_accel[NUMBER_OF_OBJECTS];                    /**< [m/s^2] longitudinal acceleration of the reference point in vcs coordinates */
   float32_T                        ref_lat_posn[NUMBER_OF_OBJECTS];                      /**< [m] lateral position of the reference point in vcs coordinates */
   float32_T                        ref_lat_vel[NUMBER_OF_OBJECTS];                       /**< [m/s] lateral velocity of the reference point in vcs coordinates */
   float32_T                        ref_lat_accel[NUMBER_OF_OBJECTS];                     /**< [m/s^2] lateral acceleration of the reference point in vcs coordinates */
   uint8_t                          f_ref_updated[NUMBER_OF_OBJECTS];                     /**< flag indicating if reference point has been updated */

   float32_T                        curvi_long_posn[NUMBER_OF_OBJECTS];                   /**< [m] longitudinal position of the object in curvi coordinates */
   float32_T                        curvi_long_vel[NUMBER_OF_OBJECTS];                    /**< [m/s] longitudinal velocity of the object in curvi coordinates */
   float32_T                        curvi_long_accel[NUMBER_OF_OBJECTS];                  /**< [m/s^2] longitudinal acceleration of the object in curvi coordinates */
   float32_T                        curvi_lat_posn[NUMBER_OF_OBJECTS];                    /**< [m] lateral position of the object in curvi coordinates */
   float32_T                        curvi_lat_vel[NUMBER_OF_OBJECTS];                     /**< [m/s] lateral velocity of the object in curvi coordinates */
   float32_T                        curvi_lat_accel[NUMBER_OF_OBJECTS];                   /**< [m/s^2] lateral acceleration of the object in curvi coordinates */
   float32_T                        curvi_long_vel_rel[NUMBER_OF_OBJECTS];                /**< [m/s] relative longitudinal velocity of the object in curvi coordinates */
   float32_T                        curvi_lat_vel_rel[NUMBER_OF_OBJECTS];                 /**< [m/s] relative lateral velocity of the object in curvi coordinates */
   float32_T                        curvi_heading[NUMBER_OF_OBJECTS];                     /**< [rad] heading of the object in curvi coordinates */
   curvi_coordinates_calc_method_T  curvi_coordinates_calc_method[NUMBER_OF_OBJECTS];     /**< enum specifying which method was used to determine the curvi coordinates of the object */

   RSDS_OBJECT_INNOVATION_OUT_FLT_T innovation[NUMBER_OF_OBJECTS];                        /**< The filtered difference between the predicted and measured state of the object. It will get bigger if the
                                                                                            * object does not follow the prediction. */
   OBJECT_VARIANCE_T                variance[NUMBER_OF_OBJECTS];                          /**< Variance derived from the raw detections and the objects updated state */
   RSDS_OBJECT_ACCURACY_FLT_T       accuracy[NUMBER_OF_OBJECTS];                          /**< The accuracy shall describe how close the tracked object is to the real object. */

   uint8_t                          f_just_merged[NUMBER_OF_OBJECTS];                     /**< this object has been merged in current cycle */
   int8_t                           f_just_merged_with[NUMBER_OF_OBJECTS];                /**< id of the object this object was merged with. -1 if the object has not been merged in the current cycle or if the
                                                                                            * object it has been merged with has been created in the current cycle. In this case, the merged object never
                                                                                            * has been reported and the ID does not matter for features. It still may be of interest to know that a merge
                                                                                            * took place, therefore f_just_merged will be TRUE. */
   uint8_t                          f_just_redivided[NUMBER_OF_OBJECTS];                  /**< This object has been intentionally redivided */
   uint8_t                          f_behind_guardrail[NUMBER_OF_OBJECTS];                /**< This object is located behind a guardrail.
                                                                                            * The structure TRACKER_STATUS_T has a member f_tracking_behind_guardrails_enabled
                                                                                            * that indicates if the tracker is set up to produce objects that have this flag
                                                                                            * set. \sa TRACKER_STATUS_T::f_tracking_behind_guardrails_enabled */
   uint8_t                          f_intersects_guardrail[NUMBER_OF_OBJECTS];            /**< Flag indicating that this object intersects with a detected guardrail. \sa f_behind_guardrail*/

   SENSOR_FLAG_T                    origin_sensor[NUMBER_OF_OBJECTS];                     /**< information about which sensors' detections were used to create/update object */
   SENSOR_FLAG_T                    in_sensor_FOV[NUMBER_OF_OBJECTS];                     /**< information about which sensors field of view this object is in */

   float32_T                        obstruction_probability[NUMBER_OF_OBJECTS];           /**< probability that object is behind obstruction */
   float32_T                        rangeRegionObstructed_probability[NUMBER_OF_OBJECTS]; /**< This signal is not computed anymore. (ABX-1572) The value will always be 0.0f */

   float32_T                        filtered_amplitude[NUMBER_OF_OBJECTS];                /**< [dBsm] current filtered amplitude (max of tracklets) */

   uint8_t                          f_reflection[NUMBER_OF_OBJECTS];                      /**< This object is probably a reflection */

   GUARDRAIL_OUTPUT_T               guardrails[NUMBER_OF_GUARDRAILS];                     /**< Guardrail data to be used by functions. Index 0:left, index 1: right */
} TRACKER_OUTPUT_T;

typedef struct TRACKER_OUTPUT_Tag TRACKER_OUTPUT;

#endif
