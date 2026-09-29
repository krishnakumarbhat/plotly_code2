#ifndef TIMING_H
#define TIMING_H

/*===================================================================*\
* Copyright 2019, Aptiv Services, Inc., All Rights Reserved.
* Aptiv Confidential.
\*===================================================================*/

/**
 * \defgroup dev_tools Development tools
 * \brief The MathLibrary does provide some functions to support development.
 *
 * These functions are not meant to be used in productive code. They are only available if the
 * MathLibrary has been integrated using the CMakelists.txt provided by the MathLibrary and
 * if the CMake option Shared_Toolbox_SharedDevelopmentTools_ACTIVE is ON.
 *
 * \par Usage of MathLibrary development tools
 * In the CMakeLists.txt of the target wanting to use the development tools:
 * - Add an option to do timing measurement for TARGET
 * - Link to the target SharedDevelopmentTools if it exists:
 * \code{cmake}
 *   IF(TARGET SharedDevelopmentTools)
 *      OPTION(TARGET_TIMING_MEASUREMENT "Do timing measurement in TARGET" OFF)
 *      IF(TARGET_TIMING_MEASUREMENT)
 *         target_link_libraries(TARGET PRIVATE SharedDevelopmentTools)
 *      ENDIF()
 *   ENDIF()
 * \endcode
 *
 * \defgroup dev_tool_timing Timing functions
 * \brief Functions that can be used to measure the runtime.
 *
 * [Opaque pointers](https://en.wikipedia.org/wiki/Opaque_pointer#C) are used to handle timers.
 * - Call Timer_Create() to receive a pointer to a TIMING_COUNTER_T, store it somewhere.
 * - Use the TIMING_COUNTER_T pointer to call Timer_Reset() at the beginning of each cycle.
 * - Use the TIMING_COUNTER_T pointer to call Timer_Start() before the piece of code that you want to measure.
 * - Use the TIMING_COUNTER_T pointer to call Timer_Stop() after the piece of code that you want to measure.
 * You can call Timer_Start() and Timer_Stop() as often as you want for the same TIMING_COUNTER_T. The timer will
 * keep track of the minimal, maximal and average time spent on the function.
 * Call Timer_Get_Min(), Timer_Get_Max() and Timer_Get_Average() to get the measured values for a given TIMING_COUNTER_T.
 *
 * These timing functions are only available if the compile definition SHARED_DEVELOPMENT_TOOLS_TIMERS_AVAILABLE is set.
 * use ifdef SHARED_DEVELOPMENT_TOOLS_TIMERS_AVAILABLE to check.
 *
 * \startuml
 * actor Feature
 * boundary Timing.h
 * control Timer_Create
 * control Timer_Start
 * control Timer_Stop
 * control Timer_Destroy
 * control Timer_Get_Average
 * entity Timer
 * Feature -> Timer_Create : Request creation of a timer
 * Timer_Create -> Timer : Creates a timer object
 * Timer_Create -> Feature : Returns the timer
 * Feature -> Timer_Start : Start the timer
 * Timer_Start -> Timer : Start the timer
 * Feature -> Timer_Stop : Stop the timer
 * Timer_Stop -> Timer : Stops the timer
 * Feature -> Timer_Get_Average : Ask for the measurement
 * Timer_Get_Average -> Timer : Reads the measurement
 * Timer_Get_Average -> Feature : Returns the measurement
 * Feature -> Timer_Destroy : Request to destroy the timer
 * Timer_Destroy -> Timer : Destroys the timer
 *
 * \enduml

 * \startuml
 * start
 * :timer = Timer_Create();
 *
 * repeat
 * :Timer_Start(timer);
 * :Timer_Stop(timer);
 * repeat while (Feature running?)
 *
 * :Timer_Get_Average(timer);
 * : Do something useful with the average value;
 * :Timer_Destroy(timer);
 *
 * stop
 * \enduml
 *
 * \section dev_tool_timing_stacked_timers Stacked timers
 * Timers can be running while there is another timer already running.
 * This means that a timer can run exclusively (Ex) or non exclusively (NonEx):
 * \startuml
 * robust "Timers" as Ts
 * concise "Timer1" as T1timing
 * concise "Timer2" as T2timing
 * @0
 * Ts is Idle
 * T1timing is Idle
 * T2timing is Idle
 *
 * @100
 * Ts is Timer1
 * T1timing is Ex

 * @200
 * Ts is Timer2
 * T1timing is NonEx
 * T2timing is Ex
 *
 * @300
 * Ts is Timer1
 * T1timing is Ex
 * T2timing is Idle
 *
 * @400
 * Ts is Idle
 * T1timing is Idle
 *
 * \enduml
 *
 * See Timer_Get_Total_Runtime() also.
 *
 * \section dev_tool_timing_usage Usage of the timing functions
 * We'll call the target that wants to use the timing functions TARGET.
 *
 * The code samples can be found in the folder Shared_Toolbox_Managed_Unit_Tests/Source/Timing_Demo
 *
 * There is a unit test in the file Shared_Toolbox_Managed_Unit_Tests/Source/Timing_Test.cpp that runs
 * the provided usage demo.
 *
 * Create a 'convenience layer' in the TARGET. In a header file TARGET_Timing.h
 * \include target_timing.h
 *
 * In the corresponding source file TARGET_Timing.c:
 * \include target_timing.c
 * Here we assumed that the bin writer shall be used to write out timing information. The file appendix of the bin
 * file created is
 * \code
 * static char *AS_bww_Target_Timing
 * \endcode
 *
 * Remember that SHARED_DEVELOPMENT_TOOLS_TIMERS_AVAILABLE will only be set if the MathLibrary timing functions are available.
 *
 * This allows the following usage in TARGET:
 * - At the beginning of each cycle call Target_Timing_Initialize(). This will fill the array of timers timing_counters.
 * - During the run of TARGET call Target_Timing_Start() and Target_Timing_Stop() as often as needed with any of the [enums](https://en.wikipedia.org/wiki/Enumerated_type) as often as needed.
 * - At the end of each cycle call Target_Timing_Store_Debug() and Target_Timing_Destroy()
 *
 * To add another timer TARGET_TIMING_TIMER_4:
 * - Append another entry TARGET_TIMING_TIMER_3 in Target_Timing_T before TARGET_TIMING_NUMBER_OF
 * - In Target_Timing_Store_Debug() add another call to the macro GDSR_TRACKER_STORE_TIMING_ONE_DEBUG
 * - Call Target_Timing_Start(TARGET_TIMING_TIMER_3) and Target_Timing_Stop(TARGET_TIMING_TIMER_3) where needed
 *
 * \sa https://en.wikipedia.org/wiki/Opaque_pointer#C for an explanation of the concept of opaque pointers.
 * \sa dev_tools for how to activate the development tools offered by the MathLibrary.
 * \ingroup dev_tools
 */

#include "reuse.h"
#include "ml_math.h"

/**
 * \brief Count the runtime of a function.
 *
 * \sa dev_tool_timing
 * Opaque pointer type. See https://en.wikipedia.org/wiki/Opaque_pointer#C for details.
 */
typedef struct TIMING_COUNTER_Tag TIMING_COUNTER_T;

/**
* Sets up a new timing counter.
*
* \return An opaque TIMING_COUNTER_T pointer to the new timer
* \ingroup dev_tool_timing
* \sa dev_tool_timing
*
*/
TIMING_COUNTER_T *Timer_Create(void);

/**
* Frees up a given timing counter.
* \ingroup dev_tool_timing
* \sa dev_tool_timing
*
*/
void Timer_Destroy(TIMING_COUNTER_T * p_timer /**< The timer to be destroyed */);

/**
 * Resets the given timer.
 * Calling this function will reset the call counter to zero and will free all measured timings of this timer.
 * \ingroup dev_tool_timing
 * \sa dev_tool_timing
 */
void Timer_Reset(TIMING_COUNTER_T *p_timer);

/**
 * Starts a new or existing timer.
 *
 * \return         TRUE if timer could be started.
 * \ingroup dev_tool_timing
 * \sa dev_tool_timing
 *
 */
boolean_T Timer_Start(TIMING_COUNTER_T *p_timer /**< Timer to be started */);

/**
 * Stops a running timer.
 * When the timer that is being stopped has not run before, the results will be zero.
 *
 * \return         timing value
 * \pre   Timer has been started before.
 * \ingroup dev_tool_timing
 * \sa dev_tool_timing
 *
 */
uint64_t Timer_Stop(TIMING_COUNTER_T *p_timer /**< Timer to stop */);

/**
* Returns the average runtime of the timer in the current cycle (after Timer_Create average of all Timer_Start - Timer_Stop cycles)
*
* \return         Average runtime
* \pre   Timer has been started before.
* \ingroup dev_tool_timing
* \sa dev_tool_timing
*
*/
double Timer_Get_Average(const TIMING_COUNTER_T *p_timer /**< Timer to get average runtime for */);

/**
* Returns the minimal runtime of the timer in the current cycle (after Timer_Create average of all Timer_Start - Timer_Stop cycles)
*
* \return         minimal runtime
* \pre   Timer has been started before.
* \ingroup dev_tool_timing
* \sa dev_tool_timing
*
*/
double Timer_Get_Min(const TIMING_COUNTER_T *p_timer /**< Timer to get minimal runtime for */);

/**
* Returns the maximal runtime of the timer in the current cycle (after Timer_Create average of all Timer_Start - Timer_Stop cycles)
*
* \return         maximal runtime
* \pre   Timer has been started before.
* \ingroup dev_tool_timing
* \sa dev_tool_timing
*
*/
double Timer_Get_Max(const TIMING_COUNTER_T *p_timer /**< Timer to get max value of */);

/**
* Returns the number of calls of the timer in the current cycle (after Timer_Create average of all Timer_Start - Timer_Stop cycles)
*
* \return         maximal runtime
* \pre   Timer has been started before.
* \ingroup dev_tool_timing
* \sa dev_tool_timing
*
*/
uint64_t Timer_Get_Number_Of_Calls(const TIMING_COUNTER_T *p_timer /**< Timer to get max value of */);

/**
* Returns the overall runtime of the timer in the current cycle (after Timer_Create average of all Timer_Start - Timer_Stop cycles)
*
* This example results in a total runtime of 200 ms:
* \startuml
* robust "Timer" as T
* @0
* T is Idle
*
* @100
* T is Timer

* @200
* T is Idle
*
* @300
* T is Timer
*
* @400
* T is Idle
*
* \enduml
*
* \return         total runtime
* \pre   Timer has been started before.
* \ingroup dev_tool_timing
* \sa dev_tool_timing
*
*/
double Timer_Get_Total_Runtime(const TIMING_COUNTER_T *p_timer /**< Timer to get total of */);


/**
* Returns the time exclusively spend on this timer in the current cycle (after Timer_Create average of all Timer_Start - Timer_Stop cycles)
*
* \return         exclusive runtime
* \pre   Timer has been started before.
* \ingroup dev_tool_timing
* \sa dev_tool_timing
* \sa \ref dev_tool_timing_stacked_timers
*
*/
double Timer_Get_Exclusive_Runtime(const TIMING_COUNTER_T *p_timer  /**< Timer to get exclusive runtime of */);

#endif

