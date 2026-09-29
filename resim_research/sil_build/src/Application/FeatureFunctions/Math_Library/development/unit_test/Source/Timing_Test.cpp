/*===================================================================*\
* Copyright 2019, Aptiv Technologies, Inc., All Rights Reserved.
* Delphi Confidential.
\*===================================================================*/

#include <gtest/gtest.h>
#include <windows.h>

#include "Timing.h"
#include "Timing_Demo/target_timing.h"
#include "ml_bool.h"
#include "ml_macros.h"
#include "ml_math.h"

TEST(TimingTest, Timer_Create__Creates_Timer)
{
   // Arrange
   TIMING_COUNTER_T *p_timer;

   // Action
   p_timer = Timer_Create();

   // Assert
   EXPECT_TRUE(NULL != p_timer);
}

TEST(TimingTest, measures_avg)
{
   // Arrange
   TIMING_COUNTER_T *p_timer;

   // Action
   p_timer = Timer_Create();
   Timer_Start(p_timer);
   Sleep(100);
   (void)Timer_Stop(p_timer);
   double runtime_avg = Timer_Get_Average(p_timer);
   Timer_Destroy(p_timer);

   // Assert
   EXPECT_GE(runtime_avg, 0.09);
   EXPECT_LE(runtime_avg, 0.11);
}

TEST(TimingTest, measures_min)
{
   // Arrange
   TIMING_COUNTER_T *p_timer;

   // Action
   p_timer = Timer_Create();
   Timer_Start(p_timer);
   Sleep(100);
   (void)Timer_Stop(p_timer);
   double runtime_min = Timer_Get_Min(p_timer);
   Timer_Destroy(p_timer);

   // Assert
   EXPECT_GE(runtime_min, 0.09);
   EXPECT_LE(runtime_min, 0.11);
}

TEST(TimingTest, measures_max)
{
   // Arrange
   TIMING_COUNTER_T *p_timer;

   // Action
   p_timer = Timer_Create();
   Timer_Start(p_timer);
   Sleep(100);
   (void)Timer_Stop(p_timer);
   double runtime_max = Timer_Get_Max(p_timer);
   Timer_Destroy(p_timer);

   // Assert
   EXPECT_GE(runtime_max, 0.09);
   EXPECT_LE(runtime_max, 0.11);
}

TEST(TimingTest, measures_noc)
{
   // Arrange
   TIMING_COUNTER_T *p_timer;

   // Action
   p_timer = Timer_Create();
   Timer_Start(p_timer);
   Sleep(100);
   (void)Timer_Stop(p_timer);
   uint64_t runtime_noc = Timer_Get_Number_Of_Calls(p_timer);
   Timer_Destroy(p_timer);

   // Assert
   EXPECT_EQ(runtime_noc, 1);
}

TEST(TimingTest, counts)
{
   // Arrange
   TIMING_COUNTER_T *p_timer;

   // Action
   p_timer = Timer_Create();
   for (int i = 0; i < 10; i++)
   {
      Timer_Start(p_timer);
      (void)Timer_Stop(p_timer);
   }
   uint64_t runtime_noc = Timer_Get_Number_Of_Calls(p_timer);
   Timer_Destroy(p_timer);

   // Assert
   EXPECT_EQ(runtime_noc, 10);
}

TEST(TimingTest, too_many_timers)
{
   // Arrange
   TIMING_COUNTER_T *p_timer[65];

   // Action
   for (int i = 0; i < 65; i++)
   {
      p_timer[i] = Timer_Create();
   }

   // Assert
   EXPECT_TRUE(NULL == p_timer[64]);

   // Clean up
   for (int i = 0; i < 65; i++)
   {
      Timer_Destroy(p_timer[i]);
   }
}

TEST(TimingTest, resets_average)
{
   // Arrange
   TIMING_COUNTER_T *p_timer;

   // Action
   p_timer = Timer_Create();
   Timer_Start(p_timer);
   Sleep(100);
   (void)Timer_Stop(p_timer);
   Timer_Reset(p_timer);
   double runtime_avg = Timer_Get_Average(p_timer);
   Timer_Destroy(p_timer);

   // Assert
   EXPECT_EQ(runtime_avg, 0.0);
}

TEST(TimingTest, resets_max)
{
   // Arrange
   TIMING_COUNTER_T *p_timer;

   // Action
   p_timer = Timer_Create();
   Timer_Start(p_timer);
   Sleep(100);
   (void)Timer_Stop(p_timer);
   Timer_Reset(p_timer);
   double runtime_max = Timer_Get_Max(p_timer);
   Timer_Destroy(p_timer);
   // Assert
   EXPECT_EQ(runtime_max, 0.0);
}

TEST(TimingTest, resets_min)
{
   // Arrange
   TIMING_COUNTER_T *p_timer;

   // Action
   p_timer = Timer_Create();
   Timer_Start(p_timer);
   Sleep(100);
   (void)Timer_Stop(p_timer);
   Timer_Reset(p_timer);
   double runtime_min = Timer_Get_Min(p_timer);
   uint64_t runtime_noc = Timer_Get_Number_Of_Calls(p_timer);
   Timer_Destroy(p_timer);

   // Assert
   EXPECT_EQ(runtime_min, 0.0);
}

TEST(TimingTest, resets_noc)
{
   // Arrange
   TIMING_COUNTER_T *p_timer;

   // Action
   p_timer = Timer_Create();
   Timer_Start(p_timer);
   Sleep(100);
   (void)Timer_Stop(p_timer);
   Timer_Reset(p_timer);
   uint64_t runtime_noc = Timer_Get_Number_Of_Calls(p_timer);
   Timer_Destroy(p_timer);

   // Assert
   EXPECT_EQ(runtime_noc, 0);
}

TEST(TimingTest, Timer_Get_Average__call_NULL)
{
   // Arrange

   // Action
   double ret = Timer_Get_Average(NULL);

   // Assert
   EXPECT_EQ(ret, 0.0);
}

TEST(TimingTest, Timer_Get_Min__call_NULL)
{
   // Arrange

   // Action
   double ret = Timer_Get_Min(NULL);

   // Assert
   EXPECT_EQ(ret, 0.0);
}

TEST(TimingTest, Timer_Get_Max__call_NULL)
{
   // Arrange

   // Action
   double ret = Timer_Get_Max(NULL);

   // Assert
   EXPECT_EQ(ret, 0.0);
}

TEST(TimingTest, Timer_Get_Number_Of_Calls__call_NULL)
{
   // Arrange

   // Action
   uint64_t ret = Timer_Get_Number_Of_Calls(NULL);

   // Assert
   EXPECT_EQ(ret, 0.0);
}

TEST(TimingTest, stacked_timers1)
{
   // Arrange
   TIMING_COUNTER_T *p_timers[4];
   int itimer;
   // Action
   /* Start four timers.
     A '.' stands for non running time, a '-' stands for running, 100ms each:
     .-......
     ...-....
     .....-..
     --------
     This results in the following timers:
     timer 0: total runtime 800, exclusive runtime: 500
     timer 1: total runtime 100, exclusive runtime: 100
     timer 2: total runtime 100, exclusive runtime: 100
     timer 3: total runtime 100, exclusive runtime: 100*/
   for (itimer = 0; itimer < 4; itimer++)
   {
      p_timers[itimer] = Timer_Create();
   }
   Timer_Start(p_timers[0]);
   Sleep(100);
   for (itimer = 1; itimer < 4; itimer++)
   {
      Timer_Start(p_timers[itimer]);
      Sleep(100);
      (void)Timer_Stop(p_timers[itimer]);
      Sleep(100);
   }
   (void)Timer_Stop(p_timers[0]);
   double runtime_exc_total = 0;
   for (itimer = 0; itimer < 4; itimer++)
   {
      runtime_exc_total += Timer_Get_Exclusive_Runtime(p_timers[itimer]);
   }
   double runtime_tot_0 = Timer_Get_Total_Runtime(p_timers[0]);
   for (itimer = 0; itimer < 4; itimer++)
   {
      Timer_Destroy(p_timers[itimer]);
   }

   // Assert
   EXPECT_DOUBLE_EQ(runtime_exc_total, runtime_tot_0);
}

TEST(TimingTest, stacked_timers2)
{
   // Arrange
   TIMING_COUNTER_T *p_timers[4];
   int itimer;
   // Action
   /* Start four timers.
   A '.' stands for non running time, a '-' stands for running, 100ms each:
   ...--...
   ..----..
   .------.
   --------
   This results in the following timers:
   timer 0: total runtime 800, exclusive runtime: 200
   timer 1: total runtime 600, exclusive runtime: 200
   timer 2: total runtime 400, exclusive runtime: 200
   timer 3: total runtime 100, exclusive runtime: 200*/
   for (itimer = 0; itimer < 4; itimer++)
   {
      p_timers[itimer] = Timer_Create();
      Timer_Start(p_timers[itimer]);
      Sleep(100);
   }
   for (itimer = 3; itimer >= 0; itimer--)
   {
      Sleep(100);
      (void)Timer_Stop(p_timers[itimer]);
   }
   double runtime_exc_total = 0;
   for (itimer = 0; itimer < 4; itimer++)
   {
      runtime_exc_total += Timer_Get_Exclusive_Runtime(p_timers[itimer]);
   }
   double runtime_tot_0 = Timer_Get_Total_Runtime(p_timers[0]);
   for (itimer = 0; itimer < 4; itimer++)
   {
      Timer_Destroy(p_timers[itimer]);
   }

   // Assert
   EXPECT_DOUBLE_EQ(runtime_exc_total, runtime_tot_0);
}

TEST(TimingTest, stacked_timers_start_stop_start_stop)
{
   // Arrange
   TIMING_COUNTER_T *p_timer0;
   TIMING_COUNTER_T *p_timer1;
   // Action
   p_timer0 = Timer_Create();
   p_timer1 = Timer_Create();

   Timer_Start(p_timer0);

   Sleep(100);
   Timer_Start(p_timer1);
   Sleep(100);
   Timer_Stop(p_timer1);
   Sleep(100);
   Timer_Start(p_timer1);
   Sleep(100);
   Timer_Stop(p_timer1);
   Sleep(100);

   Timer_Stop(p_timer0);

   double runtime_exc_total = 0;
   runtime_exc_total += Timer_Get_Exclusive_Runtime(p_timer0);
   runtime_exc_total += Timer_Get_Exclusive_Runtime(p_timer1);

   double runtime_tot_0 = Timer_Get_Total_Runtime(p_timer0);
   Timer_Destroy(p_timer0);
   Timer_Destroy(p_timer1);


   // Assert
   EXPECT_DOUBLE_EQ(runtime_exc_total, runtime_tot_0);
}

TEST(TimingTest, stacked_timers_start_stop_start_stop_2_timers)
{
   // Arrange
   TIMING_COUNTER_T *p_timer0;
   TIMING_COUNTER_T *p_timer1;
   TIMING_COUNTER_T *p_timer2;
   // Action
   p_timer0 = Timer_Create();
   p_timer1 = Timer_Create();
   p_timer2 = Timer_Create();

   Timer_Start(p_timer0);

   Timer_Start(p_timer1);
   Sleep(100);
   Timer_Stop(p_timer1);

   Timer_Start(p_timer2);
   Sleep(100);
   Timer_Stop(p_timer2);

   Timer_Stop(p_timer0);

   double runtime_exc_total = 0;
   runtime_exc_total += Timer_Get_Exclusive_Runtime(p_timer0);
   runtime_exc_total += Timer_Get_Exclusive_Runtime(p_timer1);
   runtime_exc_total += Timer_Get_Exclusive_Runtime(p_timer2);

   double runtime_exc0 = Timer_Get_Exclusive_Runtime(p_timer0);

   double runtime_tot_0 = Timer_Get_Total_Runtime(p_timer0);
   Timer_Destroy(p_timer0);
   Timer_Destroy(p_timer1);
   Timer_Destroy(p_timer2);


   // Assert
   EXPECT_DOUBLE_EQ(runtime_exc_total, runtime_tot_0);
   EXPECT_LE(runtime_exc0, 2e-6);
}

TEST(TimingTest, stacked_timers_start_stop_start_stop_2_timers_and_base_timer)
{
   // Arrange
   TIMING_COUNTER_T *p_timerBase;
   TIMING_COUNTER_T *p_timer0;
   TIMING_COUNTER_T *p_timer1;
   TIMING_COUNTER_T *p_timer2;
   // Action
   p_timerBase = Timer_Create();
   p_timer0 = Timer_Create();
   p_timer1 = Timer_Create();
   p_timer2 = Timer_Create();

   Timer_Start(p_timerBase);
   Sleep(100);
   Timer_Start(p_timer0);

   Timer_Start(p_timer1);
   Sleep(100);
   Timer_Stop(p_timer1);

   Timer_Start(p_timer2);
   Sleep(100);
   Timer_Stop(p_timer2);

   Timer_Stop(p_timer0);
   Sleep(100);
   Timer_Stop(p_timerBase);

   double runtime_exc_total = 0;
   runtime_exc_total += Timer_Get_Exclusive_Runtime(p_timer0);
   runtime_exc_total += Timer_Get_Exclusive_Runtime(p_timer1);
   runtime_exc_total += Timer_Get_Exclusive_Runtime(p_timer2);

   double runtime_exc0 = Timer_Get_Exclusive_Runtime(p_timer0);

   double runtime_tot_0 = Timer_Get_Total_Runtime(p_timer0);
   Timer_Destroy(p_timerBase);
   Timer_Destroy(p_timer0);
   Timer_Destroy(p_timer1);
   Timer_Destroy(p_timer2);


   // Assert
   EXPECT_DOUBLE_EQ(runtime_exc_total, runtime_tot_0);
   EXPECT_LE(runtime_exc0, 2e-6);
}

TEST(TimingDemoTest, Run_Timing_Demo)
{
   ASSERT_NO_THROW(
   int i_repetition;
   /* Prepare target timers at the beginning of target */
   Target_Timing_Initialize();

   /* This represents the target being run */
   for (i_repetition = 0; i_repetition < 100; i_repetition++)
   {
      Target_Timing_Start(TARGET_TIMING_TIMER_0);
      Target_Timing_Stop(TARGET_TIMING_TIMER_0);
      Target_Timing_Start(TARGET_TIMING_TIMER_1);
      Target_Timing_Stop(TARGET_TIMING_TIMER_1);
      Target_Timing_Start(TARGET_TIMING_TIMER_2);
      Target_Timing_Stop(TARGET_TIMING_TIMER_2);
      Target_Timing_Start(TARGET_TIMING_TIMER_3);
      Target_Timing_Stop(TARGET_TIMING_TIMER_3);
   }
   /* Target calculations are done, store debug and destroy timers */
   Target_Timing_Store_Debug();
   Target_Timing_Destroy();
   );
}
