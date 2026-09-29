/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <assert.h>
#include "ml_bool.h"
#include "ml_interval.h"
#include "ml_macros.h"
#include "ml_math.h"
#include "ml_math_infinity_silent.h"

#define Is_Not_Nan_Range(range) (Is_Not_Nan((range)->min) && Is_Not_Nan((range)->max)) /* PRQA S 3453 */ /* Macro is used for assertions */

Float_Range_T Create_Float_Range(
   const float32_T x,
   const float32_T y)
{
   Float_Range_T ret_value;

   assert(Is_Not_Nan(x));
   assert(Is_Not_Nan(y));
   /** \throws assertion if x or y are bigger than AS_TOOLBOX_INFINITY */
   assert(x <= AS_TOOLBOX_INFINITY);
   assert(y <= AS_TOOLBOX_INFINITY);
   assert(x >= -AS_TOOLBOX_INFINITY);
   assert(y >= -AS_TOOLBOX_INFINITY);

   if (x < y)
   {
      ret_value.min = x;
      ret_value.max = y;
   }
   else
   {
      ret_value.min = y;
      ret_value.max = x;
   }

   return ret_value;
}



Int_Range_T Create_Int_Range(
   const int32_t x,
   const int32_t y)
{
   Int_Range_T ret_value;

   if (x < y)
   {
      ret_value.min = x;
      ret_value.max = y;
   }
   else
   {
      ret_value.min = y;
      ret_value.max = x;
   }

   return ret_value;
}


Float_Range_T Init_Float_Range(void)
{
   Float_Range_T float_range;
   float_range.min = AS_TOOLBOX_INFINITY;
   float_range.max = -AS_TOOLBOX_INFINITY;

   return float_range;
}

Int_Range_T Init_Int_Range(void)
{
   Int_Range_T int_range;
   int_range.min = INT32_MAX;
   int_range.max = INT32_MIN;

   return int_range;
}

boolean_T Is_Float_Contained_In_Float_Range(
   const float32_T      n,
   const Float_Range_T * const p_range)
{
   assert(NULL != p_range);
   assert(Is_Not_Nan(n));
   assert(Is_Not_Nan_Range(p_range));

   return((n >= p_range->min) && (n <= p_range->max));
}

boolean_T Is_Int_Contained_In_Float_Range(
   const int32_t        n,
   const Float_Range_T * const p_range)
{
   assert(NULL != p_range);
   assert(Is_Not_Nan_Range(p_range));

   return(((float32_T)n >= p_range->min) && ((float32_T)n <= p_range->max));
}


boolean_T Is_Float_Contained_In_Int_Range(
   const float32_T    n,
   const Int_Range_T * const p_range)
{
   assert(NULL != p_range);
   assert(Is_Not_Nan(n));

   return((n >= (float32_T)p_range->min) && (n <= (float32_T)p_range->max));
}

boolean_T Is_Int_Contained_In_Int_Range(
   const int32_t      n,
   const Int_Range_T * const p_range)
{
   assert(NULL != p_range);

   return((n >= p_range->min) && (n <= p_range->max));
}

boolean_T Does_Int_Range_Overlap_Int_Range(
   const Int_Range_T * const p_range_a,
   const Int_Range_T * const p_range_b)
{
   boolean_T f_does_overlap;

   assert(NULL != p_range_a);
   assert(NULL != p_range_b);

   f_does_overlap = Is_Int_Contained_In_Int_Range(p_range_a->max, p_range_b);
   if (Is_False(f_does_overlap))
   {
      f_does_overlap = Is_Int_Contained_In_Int_Range(p_range_a->min, p_range_b);
   }
   if (Is_False(f_does_overlap))
   {
      f_does_overlap = Is_Int_Contained_In_Int_Range(p_range_b->min, p_range_a);
   }

   return f_does_overlap;
}


boolean_T Does_Float_Range_Overlap_Int_Range(
   const Float_Range_T * const p_float_range_a,
   const Int_Range_T   * const p_int_range_b)
{
   boolean_T does_a_overlap_b;
   boolean_T does_b_overlap_a;

   assert(NULL != p_float_range_a);
   assert(NULL != p_int_range_b);

   assert(Is_Not_Nan_Range(p_float_range_a));

   does_a_overlap_b = Is_Float_Contained_In_Int_Range(p_float_range_a->max, p_int_range_b);
   if (Is_False(does_a_overlap_b))
   {
      does_a_overlap_b = Is_Float_Contained_In_Int_Range(p_float_range_a->min, p_int_range_b);
   }
   does_b_overlap_a = Is_Int_Contained_In_Float_Range(p_int_range_b->max, p_float_range_a);
   if (Is_False(does_b_overlap_a))
   {
      does_b_overlap_a = Is_Int_Contained_In_Float_Range(p_int_range_b->min, p_float_range_a);
   }

   return(Is_True(does_a_overlap_b) || Is_True(does_b_overlap_a));
}


boolean_T Does_Float_Range_Overlap_Float_Range(
   const Float_Range_T * const p_range_a,
   const Float_Range_T * const p_range_b)
{
   boolean_T f_does_overlap;

   assert(NULL != p_range_a);
   assert(NULL != p_range_b);
   assert(Is_Not_Nan_Range(p_range_a));
   assert(Is_Not_Nan_Range(p_range_b));

   f_does_overlap = Is_Float_Contained_In_Float_Range(p_range_a->max, p_range_b);
   if (Is_False(f_does_overlap))
   {
      f_does_overlap = Is_Float_Contained_In_Float_Range(p_range_a->min, p_range_b);
   }
   if (Is_False(f_does_overlap))
   {
      f_does_overlap = Is_Float_Contained_In_Float_Range(p_range_b->min, p_range_a);
   }

   return f_does_overlap;
}

boolean_T Is_Int_Interval_Subset_Of_Int_Interval(
   const Int_Range_T * const p_interval_a,
   const Int_Range_T * const p_interval_b)
{
   boolean_T is_interval_a_subset_of_interval_b;

   assert(p_interval_a != NULL);
   assert(p_interval_b != NULL);


   is_interval_a_subset_of_interval_b = Is_Int_Contained_In_Int_Range(p_interval_a->max, p_interval_b);
   if (Is_True(is_interval_a_subset_of_interval_b))
   {
      is_interval_a_subset_of_interval_b = Is_Int_Contained_In_Int_Range(p_interval_a->min, p_interval_b);
   }

   return is_interval_a_subset_of_interval_b;
}

boolean_T Is_Float_Interval_Subset_Of_Float_Interval(
   const Float_Range_T * const p_interval_a,
   const Float_Range_T * const p_interval_b)
{
   boolean_T is_interval_a_subset_of_interval_b;

   assert(p_interval_a != NULL);
   assert(p_interval_b != NULL);

   assert(Is_Not_Nan_Range(p_interval_a));
   assert(Is_Not_Nan_Range(p_interval_b));

   is_interval_a_subset_of_interval_b = Is_Float_Contained_In_Float_Range(p_interval_a->max, p_interval_b);
   if (Is_True(is_interval_a_subset_of_interval_b))
   {
      is_interval_a_subset_of_interval_b = Is_Float_Contained_In_Float_Range(p_interval_a->min, p_interval_b);
   }

   return is_interval_a_subset_of_interval_b;
}

void Extend_Float_Range(
   Float_Range_T * const p_range,
   const float32_T x)
{
   assert(NULL != p_range);
   assert(Is_Not_Nan_Range(p_range));
   p_range->min = Min(x, p_range->min);
   p_range->max = Max(x, p_range->max);
}

void Extend_Int_Range(
   Int_Range_T * const p_range,
   const int32_t x)
{
   assert(NULL != p_range);
   p_range->min = Min(x, p_range->min);
   p_range->max = Max(x, p_range->max);
}

float32_T Enforce_Range(
   const float32_T value,
   const float32_T min_value,
   const float32_T max_value)
{
   float32_T value_in_range;

   assert(min_value <= max_value);

   value_in_range = Min(max_value, value);
   value_in_range = Max(min_value, value_in_range);

   return value_in_range;
}

float32_T Enforce_Nonzero(
   const float32_T value,
   const float32_T threshold
)
{
   float32_T ret_val;

   assert(threshold >= THRESHOLD_IS_ZERO);

   ret_val = value;
   if (Abs(value) < threshold)
   {
      if (value > 0)
      {
         ret_val = threshold;
      }
      else
      {
         ret_val = -threshold;
      }
   }

   return ret_val;
}

boolean_T Is_Float_Within_Tolerance(
   float32_T value,
   float32_T reference,
   float32_T tolerance)
{
   boolean_T f_return = FALSE;
   float32_T min_range;

   assert(tolerance >= 0.0f);

   min_range = reference - tolerance;
   if (value >= min_range)
   {
      float32_T max_range;

      max_range = reference + tolerance;
      f_return = (value <= max_range);
   }

   return f_return;
}

boolean_T Is_Int_Within_Tolerance(
   int32_t value,
   int32_t reference,
   int32_t tolerance)
{
   boolean_T f_return = FALSE;
   int32_t min_range;

   assert(tolerance > 0);

   min_range = reference - tolerance;
   if (value >= min_range)
   {
      int32_t max_range;

      max_range = reference + tolerance;
      f_return = (value <= max_range);
   }

   return f_return;
}
