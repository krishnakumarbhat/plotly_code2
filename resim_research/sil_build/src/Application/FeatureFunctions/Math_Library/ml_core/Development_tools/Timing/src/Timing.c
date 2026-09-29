/*===================================================================*\
* Copyright 2018, Aptiv Services, Inc., All Rights Reserved.
* Aptiv Confidential.
\*===================================================================*/


#include "Timing.h"
#include <string.h>
#include <Windows.h>
#include <assert.h>

#include "reuse.h"
#include "ml_bool.h"
#include "ml_macros.h"
#include "ml_math.h"

/**
 * Number of timers simultaneously available.
 * \ingroup dev_tool_timing
 */
#define ST_NUMBER_OF_TIMERS    (64)

/**
*Index of a timer if its not set.
* \ingroup dev_tool_timing
*/
#define ST_TIMER_NOT_SET       (-1)

/**
* \brief This structure holds timing information.
*
* \note All integers within this structure must have the same size, otherwise the results are corrupted! The reason for this is unknown.
* \ingroup dev_tool_timing
*/
typedef struct TIMING_COUNTER_Tag
{
   uint64_t calls_per_cycle;  /**< number of calls in each cycle */
   uint64_t start_time;       /**< Time stamp when this timer was started last */
   uint64_t overall_run_time; /**< Overall summed up runtime of this timer */
   uint64_t exclusive_run_time; /**< time spent exclusively in this timer, not in timers started during runtime of the timer */
   uint64_t non_exclusive_run_time; /**< time spent in timers that have been started/stopped during the runtime of this timer */
   uint64_t max_run_time;     /**< The maximal time this timer did run */
   uint64_t min_run_time;     /**< The minimal time this timer did run */
   boolean_T f_is_running;    /**< TRUE if this timer is currently running  */
   boolean_T f_timer_exist;   /**< TRUE if this timer is used, FALSE if its free to be created using Timer_Create() */
} TIMING_COUNTER_T;

/**
* Used to store timing information
* \ingroup dev_tool_timing
*/
static TIMING_COUNTER_T timing_counters[ST_NUMBER_OF_TIMERS] = { 0 };

/**
*To be able to compute the exclusive runtime of a timer we need to know that timers run in parallel
** \ingroup dev_tool_timing
*/
static TIMING_COUNTER_T *p_timer_stack[ST_NUMBER_OF_TIMERS] = { 0 };

/**
 * timer_stack height
 * \ingroup dev_tool_timing
 */
static int8_t timer_stack_index = -1;

/**
* Returns the current performance counter as 64 bit integer.
*
* \return         uint64_t value of current performance counter
* \ingroup dev_tool_timing
* \sa dev_tool_timing
*
*/
static uint64_t GetCounter(void);

/**
* Returns the frequency [ticks/second] the host is running on.
*
* \return         frequency [ticks/second] the host is running on
* \ingroup dev_tool_timing
* \sa dev_tool_timing
*
*/
static double GetFreq(void);

/**
* Resets a single timing counter.
* \ingroup dev_tool_timing
* \sa dev_tool_timing
*
*/
static void Reset_Single_Timing_Counter(TIMING_COUNTER_T *p_timer /**< timer to be reset */ );

void Timer_Reset(TIMING_COUNTER_T *p_timer)
{
   Reset_Single_Timing_Counter(p_timer);
}

TIMING_COUNTER_T * Timer_Create(void)
{
   TIMING_COUNTER_T * p_return_timer = NULL;
   int16_t return_idx = ST_TIMER_NOT_SET;
   int16_t i_tmr      = 0;

   while ((i_tmr < ST_NUMBER_OF_TIMERS) && (ST_TIMER_NOT_SET == return_idx))
   {
      if (Is_False(timing_counters[i_tmr].f_timer_exist))
      {
         /* found first empty slot, set new timer */
         Reset_Single_Timing_Counter(&timing_counters[i_tmr]);
         return_idx = i_tmr;
      }
      i_tmr++;
   }
   if (ST_TIMER_NOT_SET != return_idx)
   {
      p_return_timer = &timing_counters[return_idx];
   }
   return p_return_timer;
}

void Timer_Destroy(TIMING_COUNTER_T * p_timer)
{
   TIMING_COUNTER_T local_timer = { 0 };
   if (NULL != p_timer)
   {
      *p_timer = local_timer;
   }
}

boolean_T Timer_Start(TIMING_COUNTER_T *p_timer)
{
   boolean_T f_ret_val = FALSE;

   if (NULL != p_timer)
   {
      p_timer->calls_per_cycle++;
      p_timer->start_time = GetCounter();
      p_timer->f_is_running = TRUE;
      f_ret_val = TRUE;
      timer_stack_index++;
      p_timer_stack[timer_stack_index] = p_timer;
   }
   return f_ret_val;
}

uint64_t Timer_Stop(TIMING_COUNTER_T *p_timer)
{
   uint64_t runtime = 0;
   if (NULL != p_timer)
   {
      runtime = GetCounter() - p_timer->start_time;
      p_timer->overall_run_time += runtime;
      p_timer->f_is_running = FALSE;
      if (1 == p_timer->calls_per_cycle)
      {
         p_timer->max_run_time = runtime;
         p_timer->min_run_time = runtime;
      }
      else
      {
         p_timer->max_run_time = Max(p_timer->max_run_time, runtime);
         p_timer->min_run_time = Min(p_timer->min_run_time, runtime);
      }
      p_timer->exclusive_run_time = p_timer->overall_run_time - p_timer->non_exclusive_run_time;
      timer_stack_index--;
      if (timer_stack_index >= 0)
      {
         p_timer_stack[timer_stack_index]->non_exclusive_run_time += runtime;
      }
   }
   return (runtime);
}

double Timer_Get_Average(const TIMING_COUNTER_T *p_timer)
{
   double ret = 0;

   if (NULL != p_timer)
   {
      double pc_frequ;
      pc_frequ = GetFreq();
      if ((p_timer->calls_per_cycle > 0) && (pc_frequ > 0.0))
      {
         ret = p_timer->overall_run_time / p_timer->calls_per_cycle / pc_frequ;
      }
   }
   return ret;
}

double Timer_Get_Min(const TIMING_COUNTER_T *p_timer)
{
   double ret = 0;

   if (NULL != p_timer)
   {
      double pc_frequ;
      pc_frequ = GetFreq();
      if (pc_frequ > 0.0)
      {
         ret = p_timer->min_run_time / pc_frequ;
      }
   }
   return ret;
}

double Timer_Get_Max(const TIMING_COUNTER_T *p_timer)
{
   double ret = 0;

   if (NULL != p_timer)
   {
      double pc_frequ;
      pc_frequ = GetFreq();
      if (pc_frequ > 0.0)
      {
         ret = p_timer->max_run_time / pc_frequ;
      }
   }
   return ret;
}

uint64_t Timer_Get_Number_Of_Calls(const TIMING_COUNTER_T *p_timer)
{
   uint64_t ret = 0;

   if (NULL != p_timer)
   {
      ret = p_timer->calls_per_cycle;
   }
   return ret;
}

double Timer_Get_Total_Runtime(const TIMING_COUNTER_T *p_timer)
{
   double ret = 0;

   if (NULL != p_timer)
   {
      double pc_frequ;
      pc_frequ = GetFreq();
      ret = p_timer->overall_run_time / pc_frequ;
   }
   return ret;
}

double Timer_Get_Exclusive_Runtime(const TIMING_COUNTER_T *p_timer)
{
   double ret = 0;

   if (NULL != p_timer)
   {
      double pc_frequ;
      pc_frequ = GetFreq();
      ret = p_timer->exclusive_run_time / pc_frequ;
   }
   return ret;
}

static uint64_t GetCounter(void)
{
   LARGE_INTEGER li;

   QueryPerformanceCounter(&li);
   return (uint64_t)li.QuadPart;
}

static double GetFreq(void)
{
   LARGE_INTEGER li;

   QueryPerformanceFrequency(&li);
   return (double)li.QuadPart;
}


static void Reset_Single_Timing_Counter(TIMING_COUNTER_T *p_timer)
{
   TIMING_COUNTER_T local_timer = { 0 };
   if (NULL != p_timer)
   {
      local_timer.f_timer_exist = TRUE;
      *p_timer = local_timer;
   }
}
