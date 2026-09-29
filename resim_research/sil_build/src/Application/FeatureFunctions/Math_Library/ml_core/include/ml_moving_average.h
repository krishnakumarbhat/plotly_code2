#ifndef ML_MOVING_AVERAGE_H
#define ML_MOVING_AVERAGE_H
#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/


/**
 * \defgroup moving_average Moving Average Filter
 * \brief A moving average filter is implemented as described in https:\\en.wikipedia.org/wiki/Moving_average
 *
 * The one exception is that if less than the defined size of values are given only those are used.
 * An extreme example would be if only one sample is given for a window of n>1 values.
 * Then the result will be this value.
 *
 * The internal status is defined in the structure \ref Moving_Average_Filter_Instance_T. It is expected that
 * the Moving_Average_Init() function is called before the filter is used the first time and that a
 * pointer to the ring buffer is available. In order to calculate the average call the function Moving_Average_Run()
 * \section Demonstration
 * The following code snippet can be found (and run) in the file Moving_Average_Test.cpp as
 * \code
 * TEST_F(Test_Moving_Average, demo)
 * \endcode
 * \snippet st_moving_average_test.cpp Moving_Average
 */

#include "reuse.h"
#include "ml_moving_average_filter_instance_t.h"

/**
 * The moving average internal status is initialized.
 * The struct is used to store the internal status of the filter.
 * Multiple parallel instances of the filter are possible due to this.
 * The user needs to ensure that p_ringbuffer_array points to a memory location of size interval_to_average that is available to all functions
 * that get the returned Moving_Average_Filter_Instance_T passed into.
 * NOTE: Moving average assumes that the size of ringbuffer_array is equal to interval_to_average. Hence declare ringbuffer_array accordingly!!
 * \ingroup moving_average
 * \sdd{WI-13805}
 */
Moving_Average_Filter_Instance_T Moving_Average_Init(
   const uint8_t    interval_to_average, /**< [in] size of the given ringbuffer_array */
   float32_T *const p_ringbuffer_array   /**< [in] pointer to ringbuffer_array */
   );

/**
 * Internal filter status of the moving average filter is reset
 * The definition of the constants can be found in the code.
 * The size of the ring buffer is not reset since it is directly connected to the buffer itself.
 * In the current implementation the buffer is also reset by calling Moving_Average_Reset_Ringbuffer()
 * \ingroup moving_average
 * \sdd{WI-13804}
 */
void Moving_Average_Reset(
   Moving_Average_Filter_Instance_T *const p_internal_filter_status /**< [in] Moving average ring buffer filter to be reset */
   );

/**
 * Only the ring buffer of the internal filter status is reset to zeros.
 * \ingroup moving_average
 * \sdd{WI-13803}
 */
void Moving_Average_Reset_Ringbuffer(
   Moving_Average_Filter_Instance_T *const p_internal_filter_status /**< [in] Moving average ring buffer filter to reset the buffer in */
   );

/**
 * Runs the moving average filter and returns the calculated average value. A new value is stored in the buffer and the average is calculated
 * If the ring buffer is not completely full only the filled values are used to average.
 * NOTE 1: Moving_Average_Init() should be called at least once to set up the moving average filter instance before calling this function
 * that returns the moving average.
 * NOTE 2: The user has to ensure that the average is stored for further use. Each time this function is called, the input value provided is added
 * to the ring buffer.
 * \return current moving average of given Moving_Average_Filter_Instance_T updated with given value
 * \ingroup moving_average
 * \sdd{WI-13802}
 */
float32_T Moving_Average_Run(
   Moving_Average_Filter_Instance_T *const p_internal_filter_status, /**< [in, out] Moving average filter structure to be updated */
   const float32_T                         value                     /**< [in] Value to update given Moving_Average_Filter_Instance_T with */
   );


#ifdef __cplusplus
}
#endif
#endif

