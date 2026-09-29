#ifndef F360_DETECTION_LOG_H
#define F360_DETECTION_LOG_H
/*===================================================================================*\
* FILE:  f360_detection_log.h
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

static const int32_t F360_Detection_Log_Stream_Num = 71;
static const int32_t F360_Detection_Log_Stream_Ver = 3;
static const int32_t F360_Detection_Log_Max_Payload_Size = 32000;

typedef struct F360_Detection_Log_Tag
{
   uint16_t raw_det_id;        //!< ID of detection as reported by sensor.
   uint16_t objTrkID;         //!< [0-250] ID of track to which this detection is associated to.
   float vcs_x;               //!< [m] x-position of detection in VCS coordinate.    
   float vcs_y;               //!< [m] y-position of detection in VCS coordinate. 
   float rngrate_dealiased;   //!< [m/s] de-aliased range rate. 
   float rngrate_comp;        //!< [m/s] range rate compensated by sensor velocity along radial direction. 
   uint16_t clusterID;        //!< [0-250] ID of cluster owning this detection.
   uint8_t sensorID;          //!< ID of sensor which reported this detection. 
   int8_t motion_status;     //!< [(-1)-2], (enum F360_DET_MOTION) (-1)-invalid, 0-stationary, 1-moving, 2-ambigous.   
   uint8_t wheel_spin;        //!< [0-2] wheel-spin type, (enum F360_DET_WHEELSPIN_TYPE) (0)-not a WS, 0-detection pair WS, 1-object type WS.
   uint8_t f_dealiased;       //!< [bool] is the range rate of this detection de-aliased. 
   uint8_t f_double_bounce;   //!< INTERNAL USE
   uint8_t f_FOV_edge;        //!< [bool] is this detection on the edge of sensor FOV. 
   uint8_t f_rr_inlier;       //!< [bool] is this detection's range-rate considered an inlier to the associated track.
   uint8_t f_used_in_rr_msmt_update; //!< INTERNAL USE
   uint8_t f_close_target;    //!< INTERNAL USE
   uint8_t f_inside_gate;     //!< INTERNAL USE
   uint8_t f_inside_mov_gate;     //!< INTERNAL USE
   uint8_t f_ok_to_use;       //!< INTERNAL USE
   uint8_t f_trailer_related_det;       //!< INTERNAL USE
   uint8_t f_on_guardrail;    //!< [bool] is this detection on guardrail.
} F360_Detection_Log_T;

// Restore MSVC and GCC warning settings
#if defined _MSC_VER
#pragma warning(pop)
#elif 0
#pragma GCC diagnostic pop
#endif 

#endif
