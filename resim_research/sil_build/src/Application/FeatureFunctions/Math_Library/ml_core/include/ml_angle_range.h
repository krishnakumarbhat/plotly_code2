#ifndef ML_ANGLE_RANGE_H
#define ML_ANGLE_RANGE_H
#ifdef __cplusplus
extern "C"
{
#endif

/*===================================================================*\
* Copyright 2021, Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential.
*--------------------------------------------------------------------*/

/**
* \defgroup angle_range Angle Range
* \brief An angle range is a section on a circle defined by a start and an end angle.
*
* The direction of rotation is mathematically positive. This means that the 'active'
* range is defined from the start angle in positive direction to the end angle.
* Example:
* \code
*                          0                  PI/3                            2PI/3              2PI
*  start: 0,    end: 2PI   |----------------------------------------------------------------------|
*  start: PI/3  end: 2PI/3                      |-------------------------------|
*  start: 2PI/3 end: PI/3  ---------------------|                               |------------------
* \endcode
*/

#include "reuse.h"
#include "ml_angle.h"
#include "ml_angle_range_fuse_state_t.h"
#include "ml_angle_range_t.h"
#include "ml_overlapping_angle_range_t.h"


/**
 * Tests if the given angle is within the given p_angle_range.
 * \return TRUE if the given angle is within the given p_angle_range
 * \ingroup angle_range
 * \sdd{WI-13817}
 */
boolean_T Is_Angle_Contained_In_Angle_Range(
   float32_T                  angle,        /**< [in] value to be tested */
   const Angle_Range_T *const p_angle_range /**< [in] angle_range to be tested */
   );

/**
 * Creates an Angle_Range_T based on given p_start and given p_end
 * \ingroup angle_range
 * \sdd{WI-13824}
 */
void Create_Angle_Range(
   Angle_Range_T *p_angle_range, /**< [out] an Angle_Range_T set to given start and end */
   const Angle_T       *p_start, /**< [in]  start angle of angle range */
   const Angle_T       *p_end    /**< [in]  end angle of angle range */
   );

/**
* Creates an Angle_Range_T based on given p_start and given p_end
* \ingroup angle_range
* \sdd{WI-13823}
*/
void Create_Angle_Range_From_Float(
   Angle_Range_T *p_angle_range, /**< [out] an Angle_Range_T set to given start and end */
   const float    start,         /**< [in]  start angle of angle range */
   const float    end            /**< [in]  end angle of angle range */
   );

/**
* Creates an Angle_Range_T based on given p_start and given p_end.
* If necessary p_start and p_end are swapped so that p_reference is inside the resulting angle range.
* \ingroup angle_range
* \sdd{WI-13821}
*/
void Create_Angle_Range_Reference_Inside(
   Angle_Range_T *p_angle_range,    /**< [out] an Angle_Range_T set to given start and end */
   const Angle_T       *p_start,    /**< [in]  start angle of angle range */
   const Angle_T       *p_end,      /**< [in]  end angle of angle range */
   const Angle_T       *p_reference /**< [in]  reference angle inside angle range */
   );

/**
* Creates an Angle_Range_T based on given p_start and given p_end.
* If necessary p_start and p_end are swapped so that p_reference is outside the resulting angle range.
* \ingroup angle_range
* \sdd{WI-13819}
*/
void Create_Angle_Range_Reference_Outside(
   Angle_Range_T *p_angle_range,    /**< [out] an Angle_Range_T set to given start and end */
   const Angle_T       *p_start,    /**< [in] start angle of angle range */
   const Angle_T       *p_end,      /**< [in] end angle of angle range */
   const Angle_T       *p_reference /**< [in] reference angle inside angle range */
   );


/**
* Computes the fused angle range from given Angle_Range_T's
* \return Angle_Range_Fuse_State_T telling if fusion was possible and what values from the two angles where used
* \code
* ...|-----|......  A
* .......|----|...  B
* ...|--------|...  Result
*\endcode
* \ingroup angle_range
* \sdd{WI-13826}
*/
Angle_Range_Fuse_State_T Angle_Range_Fuse(
   Angle_Range_T *p_angle_range,         /**< [out] an Angle_Range_T that spans the ranges p_range_a and p_range_b if these overlap. Undefined when called with non overlapping ranges */
   const Angle_Range_T *const p_range_a, /**< [in] range to be fused */
   const Angle_Range_T *const p_range_b  /**< [in] range to be fused */
   );

/**
 * Tests if the given Angle_Range_T structures overlap.
 * \return TRUE if the given Angle_Range_T structures overlap.
 * \ingroup angle_range
 * \sdd{WI-13827}
 */
boolean_T Does_Angle_Range_Overlap_Angle_Range(
   const Angle_Range_T *const p_range_a, /**< [in] range to be tested */
   const Angle_Range_T *const p_range_b  /**< [in] range to be tested */
   );

/**
 * initializes the given p_overlapping_range
 * \ingroup angle_range
 */
void Initialize_Overlapping_Angle_Range(
   Overlapping_Angle_Range_T *p_overlapping_range /**< Pointer to Overlapping_Angle_Range_T to be initialized */
   );

/**
 * Computes Overlapping_Angle_Range_T from given Angle_Range_T's
 * \code
 * ...|-----|......  A
 * .......|----|...  B
 * .......|-|......  Result
 *\endcode

 * There are two possibilities here:
 * \code
 * -----|.....|----  A aliased
 * ...|---------|    B
 *\endcode
 * or:
 * \code
 * ..|---------------|.. A not aliased
 * ........|---|........ B
 * \endcode
 * Therefore the \ref Overlapping_Angle_Range_T structure can hold two angle ranges and the information of how many angle ranges were found.
 * \ingroup angle_range
 * \sdd{WI-13830}
 */
void Get_Overlapping_Angle_Range(
   Overlapping_Angle_Range_T *p_overlapping_range, /**< [out] resulting \ref Overlapping_Angle_Range_T*/
   const Angle_Range_T *const p_range_a, /**< [in] range to be tested */
   const Angle_Range_T *const p_range_b  /**< [in] range to be tested */
   );

/**
 * Computes the width of the given Angle_Range_T
 * \return width of the given Angle_Range_T as a float in radian
 * \ingroup angle_range
 * \sdd{WI-13832}
 */
float32_T Get_Angle_Range_Width_Float(
   const Angle_Range_T *const p_angle_range /**< [in] Angle_Range_T to compute the width of */
   );

/**
* Computes the width of the given Angle_Range_T
* \return width of the given Angle_Range_T as an Angle_T in radian
* \ingroup angle_range
* \sdd{WI-13831}
*/
Angle_T Get_Angle_Range_Width_Angle(
   const Angle_Range_T *const p_angle_range /**< [in] Angle_Range_T to compute the width of */
   );

/**
 * Swaps the start and the end angle in the given Angle_Range_T. The resulting angle range is the complement of the original angle range.
 * \code
 * ...|-----|......  Before
 * ---|.....|------  Swapped
 *\endcode
 * \ingroup angle_range
 * \sdd{WI-13833}
 */
void Swap_Angle_Range_Start_End(
   Angle_Range_T *p_angle_range /**< [in, out] Angle_Range_T to swap start and end in */
   );

/**
* Returns the start angle of the given Angle_Range_T
* \return the start angle of the given Angle_Range_T as an Angle_T structure
* \ingroup angle_range
* \sdd{WI-13829}
*/
Angle_T Get_Angle_Range_Start_Angle(
   const Angle_Range_T *p_angle_range /**< [in] Angle_Range_T to return the start of */
   );

/**
* Returns the end angle of the given Angle_Range_T
* \return the end angle of the given Angle_Range_T as an Angle_T structure
* \ingroup angle_range
* \sdd{WI-13820}
*/
Angle_T Get_Angle_Range_End_Angle(
   const Angle_Range_T *p_angle_range /**< [in] Angle_Range_T to return the end of */
   );

/**
* Returns the start angle of the given Angle_Range_T
* \return the start angle of the given Angle_Range_T as a float
* \ingroup angle_range
* \sdd{WI-13822}
*/
float32_T Get_Angle_Range_Start(
   const Angle_Range_T *p_angle_range /**< [in] Angle_Range_T to return the start of */
   );

/**
* Returns the end angle of the given Angle_Range_T
* \return the end angle of the given Angle_Range_T as a float
* \ingroup angle_range
* \sdd{WI-13816}
*/
float32_T Get_Angle_Range_End(
   const Angle_Range_T *p_angle_range /**< [in] Angle_Range_T to return the end of */
   );

/**
* Returns the center angle of the given Angle_Range_T
* \return the center angle of the given Angle_Range_T as a float
* \ingroup angle_range
* \sdd{WI-13818}
*/
float32_T Get_Angle_Range_Center(
   const Angle_Range_T *p_angle_range /**< [in] Angle_Range_T to return the end of */
   );

#ifdef __cplusplus
}
#endif
#endif
