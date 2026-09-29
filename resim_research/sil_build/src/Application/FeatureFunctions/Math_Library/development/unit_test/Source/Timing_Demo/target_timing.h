#ifndef TARGET_TIMING_H
#define TARGET_TIMING_H
#ifdef __cplusplus
extern "C"
{
#endif
#include "reuse.h"
typedef enum Target_Timing_Tag
{
   TARGET_TIMING_TIMER_0,
   TARGET_TIMING_TIMER_1,
   TARGET_TIMING_TIMER_2,
   TARGET_TIMING_TIMER_3,
   TARGET_TIMING_NUMBER_OF
}Target_Timing_T;
void Target_Timing_Initialize(void);
void Target_Timing_Start(Target_Timing_T timer);
void Target_Timing_Stop(Target_Timing_T timer);
void Target_Timing_Store_Debug(void);
void Target_Timing_Destroy(void);
#ifdef __cplusplus
}
#endif
#endif
