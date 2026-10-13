#ifndef F360_OBJECT_LOG_H
#define F360_OBJECT_LOG_H
/*===================================================================================*\
 * FILE:  f360_object_log.h
 *====================================================================================
 * Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
 */

// Add pragmas to throw error if struct is padded
#if defined _MSC_VER
   #pragma warning(push)
   #pragma warning(error:4820)
#elif 0
   #pragma GCC diagnostic push
   #pragma GCC diagnostic error "-Wpadded"
#endif

#include "f360_reuse.h"

static const int32_t F360_Objects_Log_Stream_Num       = 70;
static const int32_t F360_Objects_Log_Stream_Ver       = 12;
static const int32_t F360_Objects_Log_Max_Payload_Size = 32000;

typedef struct F360_Object_Log_Tag
{
   uint32_t unique_id; // [0 - 2^32-1] unique ID of object track
   uint16_t trkID;     // ID of object tracks, zero-ID represents an inactive track.
   uint16_t reducedID; // ID of downselected object tracks, zero-ID means current track is not downselected.

   float xposn;  // [m] x-position of object track's reference point.
   float yposn;  // [m] y-position of object track's reference point.
   float xvel;   // [m/s] over-the-ground velocity of object track.
   float yvel;   // [m/s] over-the-ground velocity of object track.
   float xaccel; // [m/s^2] over-the-ground acceleration of object track.
   float yaccel; // [m/s^2] over-the-ground acceleration of object track.

   float xvel_relative;   // [m/s]   velocity of object track relative to host
   float yvel_relative;   // [m/s]   velocity of object track relative to host
   float xaccel_relative; // [m/s^2] acceleration of object track relative to host
   float yaccel_relative; // [m/s^2] acceleration of object track relative to host

   float heading;    // [rad] heading angle of object track.
   float curvature;  // [rad/m] trajectory curvature of object track.
   float speed;      // [m/s] signed speed of object track. Negative value indicates reversing.
   float tang_accel; // [m/s^2] over-the-ground tangential acceleration of object.

   float xposn_var;   // estimated state error variance
   float yposn_var;   // estimated state error variance
   float xy_posn_cov; // estimated state error covariance

   float xvel_var;     // estimated state error variance
   float yvel_var;     // estimated state error variance
   float xaccel_var;   // estimated state error variance
   float yaccel_var;   // estimated state error variance
   float xy_vel_cov;   // estimated state error covariance
   float xy_accel_cov; // estimated state error covariance

   float heading_var;         // estimated state error variance
   float curvature_var;       // estimated state error variance
   float speed_var;           // estimated state error variance
   float accel_var;           // estimated state error variance
   float speed_heading_cov;   // estimated state error covariance
   float accel_curvature_cov; // estimated state error covariance

   float orientation;          // [rad] orientation of track's bounding box.
   float orientation_rate;     // [rad/s] orientation rate of track's bounding box.
   float orientation_var;      // estimated state error variance
   float orientation_rate_var; // estimated state error variance

   float length;     // [m] length of bounding box.
   float width;      // [m] width of bounding box.
   float length_var; // estimated state error variance
   float width_var;  // estimated state error variance

   float time_since_initialization; // [seconds] time since the creation of current track.
   float time_since_downselected;   // [seconds] time since downselection of current track.
   float time_since_stage_start;    // [seconds] time since start of current track status.

   float otg_height;            // [m] Object over the ground height, accumulated over the objects lifetime
   float confidence_level;      // [0-1] confidence level of current track.
   float existence_probability; // [0-1] Probability that the target is an actual target
   float radar_cross_section;   // [dB/m^2] object rcs value. It is the low-pass filtered RCS value based on average rcs of
                                // associated detections

   uint16_t sensor_src; // bit1-front foward, bit2-front left, bit3-left side, bit4-rear left, bit5-rear center, bit6-rear right,
                        // bit7-right side, bit8-front right

   uint8_t ndets;              // number of sensor detections associated to current object track.
   uint8_t num_rr_inlier_dets; // number of associated detections which range-rate closely match the predicted range-rate for the
                               // object track.
   uint8_t trk_fltr_type;      // track filter type, 0-INVALID, 1-CCV, 2-CTCA, 3-CCA.
   uint8_t reference_point;    // [0-8] (enum F360_Reference_Point_T) Specifies where on the object the (x,y)-posn is located. 0 =
                            // center, 1 = front left, 2 = front mid, 3 = front right, 4 = right mid, 5 = rear right, 6 = rear mid,
                            // 7 = rear left, 8 = left mid.
   uint8_t status; // 0-INVALID, 1-NEW, 2-UPDATED, 3-COASTED.

   uint8_t object_class; // [0-10] Determines the type of Object detected. 0=Object Undetermined, 1=Car, 2=MotorCycle,  3=Truck ,
                         // 4=Pedestrain, 5=Pole, 6=Tree, 7=Animal, 8=General on-road Object Detected, 9=Bicycle, 10=Vehicle
                         // Unidentified.
   uint8_t probability_pedestrian; // [0-100] class probability
   uint8_t probability_car;        // [0-100] class probability
   uint8_t probability_motorcycle; // [0-100] class probability
   uint8_t probability_bicycle;    // [0-100] class probability
   uint8_t probability_truck;      // [0-100] class probability
   uint8_t probability_undet;      // [0-100] class probability

   uint8_t f_moving;      // [bool] indicates if current track is moving at the current time.
   uint8_t f_moveable;    // [bool] indicates if current track has ever been seen to move.
   uint8_t f_oncoming;    // [bool] indicates if current track is on-coming with respect to host vehicle.
   uint8_t f_onguardrail; // [bool] indicates if the current track is on guard rail.

   uint8_t underdrivable_status_ocg; // [0-3] (enum Underdrivable_Status_T) track underdrivability status, 0 - host can not pass
                                     // under track, 1 - host is likely to pass under track, 2 - host can pass under track, 3 -
                                     // track is not in area of interest.
   uint8_t probability_underdrivable_ocg; // [0-100] probability that the object is underdrivable [0.0F - 1.0]
   uint8_t drivable_status_sg;            // [0-3] (enum)  UNCLASSIFIED = 0, OVERDRIVABLE = 1, NONDRIVABLE = 2, UNDERDRIVABLE = 3,
   uint8_t drivable_confidence_sg;        // [0-100] Confidence of the assigned SG drivability status in percentage

   uint8_t obstacle_prob;      // [0-100] probability that object is an obstacle
   uint8_t movable_prob;       // [0-100] probability indicating how likely an object is movable
   uint8_t mirror_probability; // [0-100]
   uint8_t occlusion_status;   // 0 = undefined, 1 = occluded, 2 = on_edge, 3 = visible
   uint8_t aeb_confidence;     // Radar-only AEB confidence signal
   uint8_t unused;
} F360_Object_Log_T;

// Restore MSVC and GCC warning settings
#if defined _MSC_VER
   #pragma warning(pop)
#elif 0
   #pragma GCC diagnostic pop
#endif

#endif
