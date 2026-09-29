#ifndef TRACKER_WRAPPER_H
#define TRACKER_WRAPPER_H

#include "DETECTION_FLT_T.h"
#include "RADAR_PARAMS_FLT_T.h"
#include "TRACKER_OUTPUT_T.h"
#include "VEHICLE_DATA_FLT_T.h"

/* to allow external linking with c++ wrappers, we need to define
   functions as extern "C". Add __cplusplus guards to avoid build
   issues with existing C code. */
#ifdef __cplusplus
extern "C"
{
#endif

   extern TRACKER_OUTPUT_T *Get_Tracker_Out_Ptr(void);
   extern VEHICLE_DATA_FLT_T *Get_Tracker_Veh_Ptr(void);
   extern RADAR_PARAMS_FLT_T *Get_Tracker_Par_Ptr(void);
   extern RADAR_PARAMS_FLT_T *Get_Tracker_DSP_Par_Ptr(void);
   extern DETECTION_FLT_T **Get_Tracker_Det_Ptr(void);

#ifdef __cplusplus
}
#endif

#endif /* TRACKER_WRAPPER_H */
