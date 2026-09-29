#ifndef VEHICLE_DATA_FLT_T_H
#define VEHICLE_DATA_FLT_T_H

/**
 This structure holds all of the vehicle related data such as speed, yawrate, and curvature.
*/

//#include "core_numeric_types.h"
//#include "CURV_STATE_T.h"

/* Define custom types */
#include "reuse.h"
typedef float32_T float_tracker_T;
typedef uint8_t uint8_T;

#ifndef PRNDL_STATE_T_H
#define PRNDL_STATE_T_H
/** \ingroup Enumerations */
typedef enum
{
   PRNDL_STATE_PARK    = (0), /**< 0*/
   PRNDL_STATE_REVERSE = (1), /**< 1*/
   PRNDL_STATE_NEUTRAL = (2), /**< 2*/
   PRNDL_STATE_DRIVE   = (3), /**< 3*/
   PRNDL_STATE_FOURTH  = (4), /**< 4*/
   PRNDL_STATE_THIRD   = (5), /**< 5*/
   PRNDL_STATE_LOW     = (6)  /**< 6*/
} PRNDL_state_T;
#endif


#ifndef SPEED_MODE_T_H
#define SPEED_MODE_T_H
/** \ingroup Enumerations */
typedef enum
{

   SPEED_MODE_LOW  = (0), /**< 0*/
   SPEED_MODE_HIGH = (1)  /**< 1*/

} speed_mode_T;
#endif


#ifndef TRACKER_MODE_T_H
#define TRACKER_MODE_T_H
/** \ingroup Enumerations */
typedef enum
{

   TRACKER_MODE_INVALID = (0), /**< 0*/
   TRACKER_MODE_FORWARD = (1), /**< 1*/
   TRACKER_MODE_REVERSE = (2)  /**< 2*/

} tracker_mode_T;
#endif


#ifdef PCRESIM
#pragma pack(push, save_pack, 2)
#endif

typedef struct VEHICLE_DATA_FLT
{

   float_tracker_T c_rear_axle_posn;

   float_tracker_T abs_speed; //!< [m/s] absolute speed of host vehicle
   float_tracker_T yawrate;   //!< [rad/s] yaw-rate of host vehicle, a turn to the right is positive

   float_tracker_T speed; //!< [m/s] signed speed of host vehicle

   float_tracker_T rear_axle_sideslip;  //!< [rad] sideslip angle at rear axle
   float_tracker_T vcs_long_vel;        //!< [m/s] longitudinal velocity of the vehicle at origin of VCS
   float_tracker_T vcs_lat_vel;         //!< [m/s] lateral velocity of the vehicle at origin of VCS
   float_tracker_T vcs_sideslip;        //!< [rad] sideslip angle at origin of VCS
   float_tracker_T cos_vcs_sideslip;    //!< cosine of vcs_sideslip
   float_tracker_T sin_vcs_sideslip;    //!< sine of vcs_sideslip
   float_tracker_T trailer_width;       //[m] Trailer Width
   float_tracker_T trailer_angle;       //[rad] Trailer Yaw angle
   float_tracker_T trailer_angle_rate;  //[rad/s] Trailer Yaw rate
   float_tracker_T trailer_hitch_vcs_x; //[m] Trailer Hitch location
   float_tracker_T cos_trailer_angle;   // cosine trailer angle
   float_tracker_T sin_trailer_angle;   // sine trailer angle
   float_tracker_T accel;

   float_tracker_T host_curvature_fast; //!< [1/m] curvature of the host path, fast filtering
   float_tracker_T host_curvature_slow; //!< [1/m] curvature of the host path, slow filtering
   // CURV_STATE_T tractor_curvature;      //!< structure holding data for kalman filtered curvature

   tracker_mode_T tracker_mode; //!< mode of the tracker (currently invalid, forward (LCMA), or reverse (CTRA))
   speed_mode_T speed_mode;     //!< speed mode (currently low or high)

   PRNDL_state_T prndl; //!< position of gear select

   uint8_T f_reverse_gear;    //!< flag indicating manual vehicle is in reverse gear
   uint8_T f_trailer_present; //!< flag indicating that a trailer is present
   uint8_T f_reverse;         //!< flag indicating vehicle is in reverse
   uint8_T isig_trailer;      // Indicates if trailer is present
   uint8_T isig_ATD_trailer;  // Indicates if trailer is present according to the ATD algorithm

} VEHICLE_DATA_FLT_T;

#ifdef PCRESIM
#pragma pack(pop, save_pack)
#endif

#endif
