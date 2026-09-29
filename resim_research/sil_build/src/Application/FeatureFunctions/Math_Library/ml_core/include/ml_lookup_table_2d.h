#ifndef ML_LOOKUP_TABLE_2D_H
#define ML_LOOKUP_TABLE_2D_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

/**
* \defgroup lookup_table Lookup table
* \brief A lookup table is an array that replaces runtime computation with a simpler array indexing operation.
*
* See [lookup table](https:\\en.wikipedia.org/wiki/Lookup_table)
*/

#include "reuse.h"

/**
* Returns a value corresponding to x. X is searched for in table_x and the corresponding interpolated value from table_y is returned.
* \return interpolated lookup table value corresponding to given x
*\ingroup lookup_table
* \sdd{WI-13738}
*/
float32_T Get_Value_From_2d_Lookup_Table(
   const float32_T * const p_table_x, /**< [in] Pointer to array of x values */
   const float32_T * const p_table_y, /**< [in] Pointer to array of y values */
   const uint8_t size, /**< [in] size of arrays table_x and table_y*/
   const float32_T x /**< [in] x value to be looked up */
);

/**
* This function interpolates in the segments defined by the origin and the point (x_threshold, y_threshold).
* If the x value is below 0 the function saturates to 0
* If the x value is above the y_threshold the function saturates to y_threshold
* It is imported that x_threshold != 0 otherwise a division by 0 will happen,
*\return a value in the range [0,y_threshold]. This value is calculated using linear interpolation
*\ingroup lookup_table
* \sdd{WI-13740}
*/
float32_T Interpolate_To_Zero(
   const float32_T x, /**< [in] Value to be checked and interpolated */
   const float32_T x_threshold, /**< [in] x component of the threshold */
   const float32_T y_threshold /**< [in] y component of the threshold */
);



#ifdef __cplusplus
}
#endif
#endif
