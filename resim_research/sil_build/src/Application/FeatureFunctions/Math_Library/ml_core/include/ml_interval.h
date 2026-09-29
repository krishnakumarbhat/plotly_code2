#ifndef ML_INTERVAL_H
#define ML_INTERVAL_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "reuse.h"
#include "ml_float_range_t.h"
#include "ml_int_range_t.h"

/** \defgroup interval Intervals
* An interval has a minimum and a maximum value.
*
* \section interval_contained contained
* A value x which is \f$min <= x <= max\f$ is considered to be
* contained in the interval.
*
* \section interval_overlap overlap
* Two intervals overlap if at least its min or its max value is \ref interval_contained the other interval.
*/

/**
* Creates a Float_Range_T by sorting x and y
* \return resulting interval
* \ingroup interval
* \sdd{WI-13856}
*/
Float_Range_T Create_Float_Range(
   const float32_T x, /**< [in] either end of the interval, min or max */
   const float32_T y  /**< [in] either end of the interval, min or max */
);

/**
* Creates a Int_Range_T by sorting x and y
* \return resulting interval
* \ingroup interval
* \sdd{WI-13855}
*/
Int_Range_T Create_Int_Range(
   const int32_t x, /**< [in] either end of the interval, min or max */
   const int32_t y  /**< [in] either end of the interval, min or max */
);

/**
* Creates a Float_Range_T with min set to AS_TOOLBOX_INFINITY and max set to -AS_TOOLBOX_INFINITY. This can be used to collect an actual Float_Range_T from values using the extend range function.
* \return           initialized Float_Range_T
* \ingroup interval
* \sdd{WI-13858}
*/
Float_Range_T Init_Float_Range(void);

/**
* Creates a Int_Range_T with min set to INT32_MAX and max set to INT32_MIN. This can be used to collect an actual Int_Range_T from values using the extend range function.
* \return           initialized Int_Range_T
* \ingroup interval
* \sdd{WI-13857}
*/
Int_Range_T Init_Int_Range(void);

/**
* Returns TRUE if the given number is \ref interval_contained in the given range
* \return TRUE if the given number is \ref interval_contained in the given range
* \ingroup interval
* \sdd{WI-13847}
*/
boolean_T Is_Float_Contained_In_Float_Range(
   const float32_T      n, /**< [in] value to be tested */
   const Float_Range_T * const p_range /**< [in] range to be tested */
);



/**
* Returns TRUE if the given number is \ref interval_contained in the given range
* \return TRUE if the given number is \ref interval_contained in the given range
* \ingroup interval interval
* \sdd{WI-13846}
*/
boolean_T Is_Int_Contained_In_Float_Range(
   const int32_t        n, /**< [in] value to be tested */
   const Float_Range_T * const p_range/**< [in] range to be tested */
);

/**
* Returns TRUE if the given number is \ref interval_contained in the given range
* \return TRUE if the given number is \ref interval_contained in the given range
* \ingroup interval interval
* \sdd{WI-13845}
*/
boolean_T Is_Float_Contained_In_Int_Range(
   const float32_T    n, /**< [in] value to be tested */
   const Int_Range_T * const p_range/**< [in] range to be tested */
);

/*
* Returns TRUE if the given number is \ref interval_contained in the given range
* \return TRUE if the given number is \ref interval_contained in the given range
* \ingroup interval
* \sdd{WI-13844}
*/
boolean_T Is_Int_Contained_In_Int_Range(
   const int32_t      n, /**< [in] value to be tested */
   const Int_Range_T * const p_range/**< [in] range to be tested */
);


/**
* Returns TRUE if the given two ranges \ref interval_overlap
* \return TRUE if the given two ranges \ref interval_overlap
* \ingroup interval
* \sdd{WI-13850}
*/
boolean_T Does_Int_Range_Overlap_Int_Range(
   const Int_Range_T * const p_range_a,/**< [in] range to be tested */
   const Int_Range_T * const p_range_b/**< [in] range to be tested */
);

/**
* Returns TRUE if the given two ranges \ref interval_overlap
* \return TRUE if the given two ranges \ref interval_overlap
* \ingroup interval interval
* \sdd{WI-13842}
*/
boolean_T Does_Float_Range_Overlap_Int_Range(
   const Float_Range_T * const p_float_range_a,/**< [in] range to be tested */
   const Int_Range_T   * const p_int_range_b/**< [in] range to be tested */
);

/**
* Returns TRUE if the given two ranges \ref interval_overlap
* \return TRUE if the given two ranges \ref interval_overlap
* \ingroup interval
* \sdd{WI-13848}
*/
boolean_T Does_Float_Range_Overlap_Float_Range(
   const Float_Range_T * const p_range_a,/**< [in] range to be tested */
   const Float_Range_T * const p_range_b/**< [in] range to be tested */
);



/**
* Returns TRUE if the first given interval is part of the second one
* \return TRUE if the first given interval is part of the second one
* \ingroup interval
* \sdd{WI-13849}
*/
boolean_T Is_Int_Interval_Subset_Of_Int_Interval(
   const Int_Range_T * const p_interval_a,/**< [in] interval to be tested */
   const Int_Range_T * const p_interval_b /**< [in] interval to be tested */
);


/**
* Returns TRUE if the first given interval is part of the second one
* \return TRUE if the first interval range is part of the second one
* \ingroup interval
* \sdd{WI-13843}
*/
boolean_T Is_Float_Interval_Subset_Of_Float_Interval(
   const Float_Range_T * const p_interval_a,/**< [in] interval to be tested */
   const Float_Range_T * const p_interval_b/**< [in] interval to be tested */
);


/**
* Extends the Float_Range_T range to include x
* \ingroup interval
* \sdd{WI-13841}
*/
void Extend_Float_Range(
   Float_Range_T  * const p_range,                 /**< [in] Range to be extended */
   const float32_T x /**< [in] Value to extend the range to*/
);




/**
* Extends the Int_Range_T range to include x
* \ingroup interval
* \sdd{WI-13840}
*/
void Extend_Int_Range(
   Int_Range_T  * const p_range,                 /**< [in] Range to be extended */
   const int32_t x /**< [in] Value to extend the range to*/
);


/**
* Returns value limited to be within min_value and max_value
* \return Clipped value
* \ingroup interval
* \sdd{WI-13852}
*/
float32_T Enforce_Range(
   const float32_T value, /**< [in] value to be clipped */
   const float32_T min_value, /**< [in] minimum clipping value */
   const float32_T max_value /**< [in] maximum clipping value */
);

/**
* \return value if value < -threshold
* \return value if value >  threshold
* \return -threshold if -threshold < value <= 0
* \return threshold if 0 < value < threshold
* \throws assertion if threshold < THRESHOLD_IS_ZERO
* \ingroup interval
* \sdd{WI-13851}
*/
float32_T Enforce_Nonzero(
   const float32_T value, /**< [in] value to be clipped */
   const float32_T threshold /**< [in] Threshold around zero to be used for clipping */
);


/**
* \return TRUE if (reference - tolerance) <= value <= (reference + tolerance)
* \throws assertion if threshold < THRESHOLD_IS_ZERO
* \ingroup interval
* \sdd{WI-13853}
*/
boolean_T Is_Float_Within_Tolerance(
   float32_T value, /**< [in] value to check*/
   float32_T reference, /**< [in] reference value to check against*/
   float32_T tolerance /**< [in] tolerance around reference to check if value is within */
);

/**
* \return TRUE if (reference - tolerance) <= value <= (reference + tolerance)
* \throws assertion if threshold < 1
* \ingroup interval
* \sdd{WI-13854}
*/
boolean_T Is_Int_Within_Tolerance(
   int32_t value, /**< [in] value to check*/
   int32_t reference, /**< [in] reference value to check against*/
   int32_t tolerance /**< [in] tolerance around reference to check if value is within */
);

#ifdef __cplusplus
}
#endif
#endif
