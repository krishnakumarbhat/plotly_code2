#ifndef VEHICLE_CONFIG_FLT_T_H
#define VEHICLE_CONFIG_FLT_T_H

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

typedef struct VEHICLE_CONFIG_FLT
{
   float_tracker_T host_vehicle_width;
   float_tracker_T host_vehicle_length;

} VEHICLE_CONFIG_FLT_T;

#ifdef PCRESIM
#pragma pack(pop, save_pack)
#endif

#endif /* VEHICLE_CONFIG_FLT_T_H */
