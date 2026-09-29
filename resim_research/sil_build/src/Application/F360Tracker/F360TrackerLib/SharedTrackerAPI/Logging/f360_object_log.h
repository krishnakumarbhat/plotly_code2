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
#pragma warning(error : 4820)
#elif 0
#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wpadded"
#endif

#include <f360_reuse.h>

static const int32_t F360_Objects_Log_Stream_Num = 70;
static const int32_t F360_Objects_Log_Stream_Ver = 11;
static const int32_t F360_Objects_Log_Max_Payload_Size = 32000;

typedef struct F360_Object_Log_Tag
{
   uint32_t unique_id;           //!< [0 - 2^32-1] unique ID of object track
   uint16_t trkID;               //!< ID of object tracks, zero-ID represents an inactive track.
   uint16_t reducedID;            //!< ID of downselected object tracks, zero-ID means current track is not downselected.

   float vcs_xposn;              //!< [m] x-position of object track's reference point in VCS coordinate. 
   float vcs_yposn;              //!< [m] y-position of object track's reference point in VCS coordinate. 
   float vcs_xvel;               //!< [m/s] over-the-ground velocity of object track along x direction (in VCS coordinate). For CTCA defined in the rear center, for CCA defined in reference point.
   float vcs_yvel;               //!< [m/s] over-the-ground velocity of object track along y direction (in VCS coordinate). For CTCA defined in the rear center, for CCA defined in reference point.
   float vcs_xaccel;             //!< [m/s^2] over-the-ground track acceleration along x direction (in VCS coordinate). For CTCA defined in the rear center, for CCA defined in reference point.
   float vcs_yaccel;             //!< [m/s^2] over-the-ground track acceleration along y direction (in VCS coordinate). For CTCA defined in the rear center, for CCA defined in reference point.
   float vcs_heading;            //!< [rad] heading angle of object track in VCS coordinate. For CTCA defined in the rear center, for CCA defined in reference point.
   float vcs_pointing;           //!< [rad] orientation of track's bounding box in VCS coordinate.

   float speed;                  //!< [m/s, (-10)-100] signed speed of object track. Negative value indicates track backing up. For CTCA defined in the rear center, for CCA defined in reference point.
   float curvature;              //!< [rad/m] trajectory curvature of object track VCS coordinate. For CTCA defined in the rear center, for CCA defined in reference point.
   float tang_accel;             //!< [m/s^2] over-the-ground tangential acceleration of object. For CTCA defined in the rear center, for CCA defined in reference point.
   float existence_probability;  //!< [0-1] Probability that the target is an actual target
   float otg_height;             //!< [m] Object over the ground height, accumulated over the objects lifetime

   float state_variance[6];      //!< variance of the state vector consisting of [vcs_xposn, vcs_yposn, vcs_heading, curvature, speed and tangential accelaration] when track filter is CTCA
                                 //!< and [vcs_xposn, vcs_xvel, vcs_yposn, vcs_yvel, vcs_xacc, vcs_yacc] when tracker filter is CCA

   float supplemental_state_covariance[3];  //!< covariance of [vcs_xposn & vcs_yposn, speed & vcs_heading, tangential acceleration & curvature] when track filter is CTCA 
                                            //!< and [vcs_xposn & vcs_yposn, vcs_xvel & vcs_yvel, vcs_xacc & vcs_yacc] when track filter is CCA
   float confidenceLevel;        //!< [0-1] confidence level of current track. 

   float time_since_measurement;     //!< [seconds] time since the current track was updated (it can be time of measurement or in case of coasting the time increased by cumulated time of time-update step). 
   float time_since_cluster_created; //!< [seconds] time since the creation of current track. 
   float time_since_track_updated;   //!< [seconds] time since most recent detections were associated to current track. 
   float time_since_stage_start;     //!< [seconds] time since start of current track status. 

   float len1;                   //!< [m] longitudinal distance from reference point to rear edge of bounding box. 
   float len2;                   //!< [m] longitudinal distance from reference point to front edge of bounding box. 
   float wid1;                   //!< [m] lateral distance from reference point to left edge of bounding box. 
   float wid2;                   //!< [m] lateral distance from reference point to right edge of bounding box. 
   float accuracy_length;
   float accuracy_width;

   /* Probability vectors */
   float probability_pedestrian;
   float probability_car;
   float probability_motorcycle;
   float probability_bicycle;
   float probability_truck;
   float probability_undet;
   float probability_underdrivable_ocg; // probability that the object is underdrivable [0.0F - 1.0F]
   float movable_prob;              // probability indicating how likely an object is movable [0.0, 1.0]

   float radar_cross_section;       // object rcs value. It is the low-pass filtered RCS value based on average rcs of associated detections [dB/m^2]

   uint16_t ndets;                           //!< [0-200] number of sensor detections associated to current object track.
   uint16_t num_rr_inlier_dets;              //!< [0-200] number of associated detections which range-rate closely match the predicted range-rate for the object track.
   uint16_t num_dets_used_in_rr_msmt_update; //!< [0-200] number of rr inliers used in the measurement range-rate update.

   uint8_t status;               //!< 0-INVALID, 1-NEW, 2-NEW UPDATED, 3-NEW COASTED, 4-UPDATED, 5-COASTED.
   uint8_t reference_point;      //!< [0-8] (enum F360_Reference_Point_T) Specifies where on the object the (x,y)-posn is located. 0 = center, 1 = front left, 2 = front mid, 3 = front right, 4 = right mid, 5 = rear right, 6 = rear mid, 7 = rear left, 8 = rear left.
   uint8_t reducedStatus;        //!< 0-not downselected, 1-newly downselected, 3-5 current track downselected, same meaning as variable status. 
   int8_t init_scheme;           //!< [(-1)-3] enum F360_TRK_INIT_TYPE, (-1)-invalid, 0-first generation of cloud algorithm, 1-second generation of cloud algorithm, 2-radial motion, 3-longitudinal motion. 
   uint8_t object_class;         //!< [0-10] Determines the type of Object detected. 0=Object Undetermined, 1=Car, 2=MotorCycle,  3=Truck , 4=Pedestrain,
                                 //!         5=Pole, 6=Tree, 7=Animal, 8=General on-road Object Detected, 9=Bicycle, 10=Vehicle Unidentified. 

   uint8_t f_crossing;           //!< [bool] indicates if current track has cross radial motion with respect to the sensor illuminating this object. 
   uint8_t f_moving;             //!< [bool] indicates if current track is moving at the current time.  
   uint8_t f_moveable;           //!< [bool] indicates if current track has ever been seen to move. 
   uint8_t f_oncoming;           //!< [bool] indicates if current track is on-coming with respect to host vehicle. 

   uint8_t f_vehicular_trk;      //!< [bool] indicates if current track is a vehicle.
   uint8_t f_onguardrail;        //!< [bool] indicates if the current track is on guard rail.
   uint8_t f_fast_moving;        //!< [bool] indicates if the current track's speed is above 3 m/s
   uint8_t underdrivable_status_ocg; //!< [0-3] (enum Underdrivable_Status_T) track underdrivability status, 0 - host can not pass under track, 1 - host is likely to pass under track,
                                 //!< 2 - host can pass under track, 3 - track is not in area of interest.
   uint8_t drivable_status_sg; //!<[0-3] (enum)  UNCLASSIFIED = 0, OVERDRIVABLE = 1, // the host can drive over an obstacle
                                   //!< NONDRIVABLE = 2,// the host cannot drive through UNDERDRIVABLE = 3,     // the host can drive under an obstacle
   uint8_t drivable_confidence_sg; // [0-100] Confidence of the assigned SG drivability status in percentage
   uint8_t current_msmt_type;    //!< [0-1] (enum F360_CURRENT_MSMT_TYPE) current measurement type, intermediate unification of interfaces, to be removed after DEX-1436.
   uint8_t trk_fltr_type;        //!< track filter type, 0-INVALID, 1-CCV, 2-CTCA, 3-CCA.
   uint8_t obstacle_prob;        //!< probability that object is an obstacle [0 - 100]

   uint8_t aeb_confidence;       //!< Radar-only AEB confidence signal, [0-4], 0-INVALID, 1-LOW, 2-MEDIUM, 3-MEDIUM_HIGH, 4-HIGH.
   uint8_t padding[3];

} F360_Object_Log_T;

// Restore MSVC and GCC warning settings
#if defined _MSC_VER
#pragma warning(pop)
#elif 0
#pragma GCC diagnostic pop
#endif

#endif
