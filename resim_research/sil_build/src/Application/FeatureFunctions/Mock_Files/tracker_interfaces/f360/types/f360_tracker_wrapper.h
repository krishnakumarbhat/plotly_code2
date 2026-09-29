
#ifndef F360_TRACKER_WRAPPER_H
#define F360_TRACKER_WRAPPER_H

#include "f360_object_log.h"
#include "pa_context.h"
#include "VehicleInfoLog.h"


/* to allow external linking with c++ wrappers, we need to define
   functions as extern "C". Add __cplusplus guards to avoid build
   issues with existing C code. */
#ifdef __cplusplus
extern "C"
{
#endif

   extern Vehicle_Info_Log_T *Get_Tracker_Veh_Ptr(void);
   extern F360_All_Objects_Log_T *Get_Tracker_Out_Ptr(void);

#ifdef __cplusplus
}
#endif

#endif /* F360_TRACKER_WRAPPER_H */
