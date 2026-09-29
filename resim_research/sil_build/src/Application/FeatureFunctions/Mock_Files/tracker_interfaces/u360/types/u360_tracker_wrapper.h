
#ifndef U360_TRACKER_WRAPPER_H
#define U360_TRACKER_WRAPPER_H

#include "VEHICLE_DATA_FLT_T.h"
#include "TRACKER_OUTPUT_SIDE_T.h"

/* to allow external linking with c++ wrappers, we need to define
   functions as extern "C". Add __cplusplus guards to avoid build
   issues with existing C code. */
#ifdef __cplusplus
extern "C"
{
#endif

   extern VEHICLE_DATA_FLT_T *Get_Tracker_Veh_Ptr(void);
   extern TRACKER_OUTPUT_SIDE_T *Get_Tracker_Out_Ptr(void);
   extern VEHICLE_CONFIG_FLT_T *Get_Vehicle_Config_Ptr(void);
#ifdef __cplusplus
}
#endif

#endif /* U360_TRACKER_WRAPPER_H */
