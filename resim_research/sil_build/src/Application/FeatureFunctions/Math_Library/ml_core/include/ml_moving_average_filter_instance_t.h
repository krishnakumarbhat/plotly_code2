#ifndef ML_MOVING_AVERAGE_FILTER_INSTANCE_T_H
#define ML_MOVING_AVERAGE_FILTER_INSTANCE_T_H
#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "reuse.h"

/**
 * Structure used for moving average calculations using a ring buffer.
 * Before using any functionality use Moving_Average_Init() to initialize an instance of this structure.
 * \ingroup moving_average
 */
typedef struct Moving_Average_Filter_Instance_Tag
{
   float32_T *ringbuffer_array;                /**< Ring buffer to store the elements of the moving average filter*/
   float32_T  inverse_size;                    /**< inverse size . This will remove a division*/
   uint8_t    ringbuffer_size;                 /**< size of the ring buffer */
   uint8_t    first_empty_place_in_ringbuffer; /**< Current position in the ring buffer indicating where the new value
                                                *   can be placed, should only be used inside the moving average filter */
   uint8_t    averaged_cycles;                 /**< Averaged cycles. Used only if ring buffer is not completely full.
                                                *   Otherwise it will be just the size*/
} Moving_Average_Filter_Instance_T;

#ifdef __cplusplus
}
#endif
#endif

