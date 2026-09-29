 #include "target_timing.h"
 #ifdef SHARED_DEVELOPMENT_TOOLS_TIMERS_AVAILABLE

/* Writing to bin files is not done in demo */
/* #include "AS_bin_writer_wrapper.h" */
 #include "Timing.h"
 #include <string.h>
 #include <assert.h>
#include "ml_macros.h"
#include "ml_math.h"
static char *AS_bww_Target_Timing = "TargetTiming";
static TIMING_COUNTER_T *timing_counters[TARGET_TIMING_NUMBER_OF];
void Target_Timing_Initialize(void)
{
   int itime;
   for (itime = 0; itime < TARGET_TIMING_NUMBER_OF; itime++)
   {
      timing_counters[itime] = Timer_Create();
      assert(NULL != timing_counters[itime]);
   }
}
void Target_Timing_Start(Target_Timing_T timer)
{
   (void)Timer_Start(timing_counters[timer]);
}
void Target_Timing_Stop(Target_Timing_T timer)
{
   (void)Timer_Stop(timing_counters[timer]);
}
#define TARGET_STORE_TIMING_ONE_DEBUG(timer, name)                                        \
   min_val = Timer_Get_Min(timer);                                                        \
   max_val = Timer_Get_Max(timer);                                                        \
   avg_val = Timer_Get_Average(timer);                                                    \
   number_of_calls = Timer_Get_Number_Of_Calls(timer);                                    \
   str_len = strlen(name);                                                                \
   if (251 > str_len)                                                                     \
   {                                                                                      \
      /* strcpy(full_name, name); */                                                      \
      /* strcat(full_name, "_MIN"); */                                                    \
      /* Writing to bin files is not done in demo */                                      \
      /* STORE_VAL_MGR_WPR(AS_bww_Tracker_Timing, full_name, min_val); */                 \
      /* strcpy(full_name, name); */                                                      \
      /* strcat(full_name, "_MAX"); */                                                    \
      /* Writing to bin files is not done in demo */                                      \
      /* STORE_VAL_MGR_WPR(AS_bww_Tracker_Timing, full_name, max_val); */                 \
      /* strcpy(full_name, name); */                                                      \
      /* strcat(full_name, "_AVG"); */                                                    \
      /* Writing to bin files is not done in demo */                                      \
      /* STORE_VAL_MGR_WPR(AS_bww_Tracker_Timing, full_name, avg_val); */                 \
      /* strcpy(full_name, name); */                                                      \
      /* strcat(full_name, "_NUM"); */                                                    \
      /* Writing to bin files is not done in demo */                                      \
      /* STORE_VAL_MGR_WPR(AS_bww_Tracker_Timing, full_name, (double)number_of_calls); */ \
   }
void Target_Timing_Store_Debug(void)
{
   double   min_val;
   double   max_val;
   double   avg_val;
   uint64_t number_of_calls;
   size_t   str_len;
   TARGET_STORE_TIMING_ONE_DEBUG(timing_counters[TARGET_TIMING_TIMER_0], "TARGET_TIMING_TIMER_0");
   TARGET_STORE_TIMING_ONE_DEBUG(timing_counters[TARGET_TIMING_TIMER_1], "TARGET_TIMING_TIMER_1");
   TARGET_STORE_TIMING_ONE_DEBUG(timing_counters[TARGET_TIMING_TIMER_2], "TARGET_TIMING_TIMER_2");
   TARGET_STORE_TIMING_ONE_DEBUG(timing_counters[TARGET_TIMING_TIMER_3], "TARGET_TIMING_TIMER_3");
}
void Target_Timing_Destroy(void)
{
   int itime;
   for (itime = 0; itime < TARGET_TIMING_NUMBER_OF; itime++)
   {
      Timer_Destroy(timing_counters[itime]);
   }
}
#else
void Target_Timing_Initialize(void)
{}
void Target_Timing_Reset(void)
{}
void Target_Timing_Start(Target_Timing_T timer)
{}
void Target_Timing_Stop(Target_Timing_T timer)
{}
void Target_Timing_Store_Debug(void)
{}
void Target_Timing_Destroy(void)
{}
#endif
