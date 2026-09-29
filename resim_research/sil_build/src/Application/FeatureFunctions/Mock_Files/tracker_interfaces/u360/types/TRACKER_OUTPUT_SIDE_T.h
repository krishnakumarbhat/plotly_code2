#ifndef TRACKER_OUTPUT_SIDE_T_H
#define TRACKER_OUTPUT_SIDE_T_H


//#include "racam_enums.h"
//#include "symbolic_constants.h"

/* Define custom types */
#include "reuse.h"
#include "racam_enums.h"
#define NUMBER_OF_OBJECTS (96)
typedef float32_T float_tracker_T;

typedef uint16_t uint16_T;
typedef uint8_t uint8_T;

/**
 This structure holds the output data from the tracker
*/

typedef struct TRACKER_OUTPUT_SIDE
{

   float_tracker_T vcs_long_posn[NUMBER_OF_OBJECTS];      //!< [m] from RSDS_OBJECT_FLT_T
   float_tracker_T vcs_long_vel[NUMBER_OF_OBJECTS];       //!< [m/s] from RSDS_OBJECT_FLT_T
   float_tracker_T vcs_long_accel[NUMBER_OF_OBJECTS];     //!< [m/s^2] from RSDS_OBJECT_FLT_T
   float_tracker_T vcs_lat_posn[NUMBER_OF_OBJECTS];       //!< [m] from RSDS_OBJECT_FLT_T
   float_tracker_T vcs_lat_vel[NUMBER_OF_OBJECTS];        //!< [m/s] from RSDS_OBJECT_FLT_T
   float_tracker_T vcs_lat_accel[NUMBER_OF_OBJECTS];      //!< [m/s^2] from RSDS_OBJECT_FLT_T
   float_tracker_T speed[NUMBER_OF_OBJECTS];              //!< [m/s] from RSDS_OBJECT_FLT_T
   float_tracker_T heading[NUMBER_OF_OBJECTS];            //!< [rad] from RSDS_OBJECT_FLT_T
   float_tracker_T heading_rate[NUMBER_OF_OBJECTS];       //!< [rad/s] from RSDS_OBJECT_FLT_T
   float_tracker_T length[NUMBER_OF_OBJECTS];             //!< [m] from RSDS_OBJECT_FLT_T
   float_tracker_T width[NUMBER_OF_OBJECTS];              //!< [m] from RSDS_OBJECT_FLT_T
   float_tracker_T curvi_long_posn[NUMBER_OF_OBJECTS];    //!< [m] from RSDS_OBJECT_FLT_T
   float_tracker_T curvi_long_vel[NUMBER_OF_OBJECTS];     //!< [m/s] from RSDS_OBJECT_FLT_T
   float_tracker_T curvi_long_vel_rel[NUMBER_OF_OBJECTS]; //!< [m/s] from RSDS_OBJECT_FLT_T
   float_tracker_T curvi_long_accel[NUMBER_OF_OBJECTS];   //!< [m/s^2] from RSDS_OBJECT_FLT_T
   float_tracker_T curvi_lat_posn[NUMBER_OF_OBJECTS];     //!< [m] from RSDS_OBJECT_FLT_T
   float_tracker_T curvi_lat_vel[NUMBER_OF_OBJECTS];      //!< [m/s] from RSDS_OBJECT_FLT_T
   float_tracker_T curvi_lat_vel_rel[NUMBER_OF_OBJECTS];  //!< [m/s] from RSDS_OBJECT_FLT_T
   float_tracker_T curvi_lat_accel[NUMBER_OF_OBJECTS];    //!< [m/s^2] from RSDS_OBJECT_FLT_T
   float_tracker_T curvi_heading[NUMBER_OF_OBJECTS];      //!< [rad] from RSDS_OBJECT_FLT_T
   uint16_T stage_age[NUMBER_OF_OBJECTS];                 //!< from RSDS_OBJECT_FLT_T
   uint16_T age[NUMBER_OF_OBJECTS];                       //!< from RSDS_OBJECT_FLT_T
   OBJECT_CLASS object_class[NUMBER_OF_OBJECTS];          //!< from RSDS_OBJECT_FLT_T
   uint8_T f_moveable[NUMBER_OF_OBJECTS];                 //!< from RSDS_OBJECT_FLT_T
   uint8_T f_stationary[NUMBER_OF_OBJECTS];               //!< from RSDS_OBJECT_FLT_T
   ST4 status[NUMBER_OF_OBJECTS];                         //!< from RSDS_OBJECT_FLT_T
   uint8_T id[NUMBER_OF_OBJECTS];                         //!< from RSDS_OBJECT_FLT_T
   uint8_T radar_fus_sources[NUMBER_OF_OBJECTS];          /*Defines unique fused object radar sources*/
                                                          /*0x0) = NONE,  (0x1) = FLR,   (0x2)  = SODFL,*/
                                                          /*(0x4) = SODFR, (0x8) = SODRL, (0x10) = SODRR*/
   uint8_T conf_crossing_traffic[NUMBER_OF_OBJECTS];	  
   uint8_T MOISConfidence[NUMBER_OF_OBJECTS];             /**< Confidence of Object's relevance for MOIS,
                                                            0 = NONE, 1 = LOW, 2, = MEDIUM and 3 = HIGH */

} TRACKER_OUTPUT_SIDE_T;

#endif
