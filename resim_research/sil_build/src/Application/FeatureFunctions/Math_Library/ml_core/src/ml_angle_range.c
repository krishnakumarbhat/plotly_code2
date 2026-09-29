/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "reuse.h"
#include <assert.h>
#include "ml_angle.h"
#include "ml_bool.h"
#include "ml_angle_range.h"
#include "ml_macros.h"
#include "ml_math.h"
#include "ml_math_infinity_silent.h"

/**
* Macro to ensure that the given value is inside \f$+-\pi\f$
* \return TRUE is the value is within the range of \f$-pi\f$ and \f$pi\f$
* \ingroup angle_range
*/
#define Is_Abs_Value_Le_Pi(value) (((value) >= -PI) && ((value) <= PI)) /* PRQA S 3453 */ /* macro used in assertions only */

/**
* Possible ways two angle ranges can overlap
* \ingroup angle_range
*/
typedef enum Overlap_Cases_Tag
{
   NO_OVERLAP,
   OVERLAP_A_START_IN_B,
   OVERLAP_B_START_IN_A,
   FULL_OVERLAP
} Overlap_Cases_T;

/**
 * How are the two given angle ranges overlapping?
 * \return the overlapping case of the two given angle ranges
 * \ingroup angle_range
 */
static Overlap_Cases_T Determine_Overlap_Case(
   const Angle_Range_T *const p_range_a, /**< [in] range to be tested */
   const Angle_Range_T *const p_range_b  /**< [in] range to be tested */
   );

/**
* Compute the overlapping angle range for the case OVERLAP_A_START_IN_B
* \return the overlapping case of the two given angle ranges
* \ingroup angle_range
*/
static void Handle_Overlap_A_Start_In_B(
   Overlapping_Angle_Range_T *p_overlapping_range, /**< [out] Resulting angle range */
   const Angle_Range_T *const p_range_a, /**< [in] range to be tested */
   const Angle_Range_T *const p_range_b  /**< [in] range to be tested */
   );

/**
* Compute the overlapping angle range for the case FULL_OVERLAP
* \return the overlapping case of the two given angle ranges
* \ingroup angle_range
*/
static void Handle_Overlap_Full_Overlap( /**< [out] Resulting angle range */
   Overlapping_Angle_Range_T *p_overlapping_range,
   const Angle_Range_T *const p_range_a, /**< [in] range to be tested */
   const Angle_Range_T *const p_range_b  /**< [in] range to be tested */
   );

boolean_T Is_Angle_Contained_In_Angle_Range(
   float32_T                  angle,
   const Angle_Range_T *const p_angle_range)
{
   boolean_T f_contained = FALSE;

   assert(NULL != p_angle_range);

   /* Ensure all angles are normalized */
   assert(Is_Abs_Value_Le_Pi(p_angle_range->start));
   assert(Is_Abs_Value_Le_Pi(p_angle_range->end));
   assert(Is_Abs_Value_Le_Pi(angle));

   if (p_angle_range->start > p_angle_range->end)
   {
      if ((angle >= p_angle_range->start) || (angle <= p_angle_range->end))
      {
         f_contained = TRUE;
      }
   }
   else
   {
      if ((angle >= p_angle_range->start) && (angle <= p_angle_range->end))
      {
         f_contained = TRUE;
      }
   }

   return f_contained;
}


void Create_Angle_Range(
   Angle_Range_T *p_angle_range,
   const Angle_T       *p_start,
   const Angle_T       *p_end)
{
   assert(NULL != p_angle_range);
   assert(NULL != p_start);
   assert(NULL != p_end);

   Create_Angle_Range_From_Float(
      p_angle_range,
      p_start->angle,
      p_end->angle
      );
}

void Create_Angle_Range_From_Float(
   Angle_Range_T *p_angle_range,
   const float    start,
   const float    end
   )
{
   assert(NULL != p_angle_range);

   p_angle_range->start = Normalize_Angle(start, 0.f);
   p_angle_range->end = Normalize_Angle(end, 0.0f);

   assert(Is_Abs_Value_Le_Pi (p_angle_range->start));
   assert(Is_Abs_Value_Le_Pi (p_angle_range->end));
}

void Create_Angle_Range_Reference_Inside(
   Angle_Range_T *p_angle_range,
   const Angle_T       *p_start,
   const Angle_T       *p_end,
   const Angle_T       *p_reference
   )
{
   boolean_T f_inside;

   assert(NULL != p_start);
   assert(NULL != p_end);
   assert(NULL != p_reference);

   assert(Is_Abs_Value_Le_Pi (p_start->angle));
   assert(Is_Abs_Value_Le_Pi (p_end->angle));
   assert(Is_Abs_Value_Le_Pi (p_reference->angle));

   Create_Angle_Range(
      p_angle_range,
      p_start,
      p_end);

   f_inside = Is_Angle_Contained_In_Angle_Range(p_reference->angle, p_angle_range);
   if (Is_False(f_inside))
   {
      Swap_Angle_Range_Start_End(p_angle_range);
   }
}

void Create_Angle_Range_Reference_Outside(
   Angle_Range_T *p_angle_range,
   const Angle_T       *p_start,
   const Angle_T       *p_end,
   const Angle_T       *p_reference
   )
{
   boolean_T f_inside;

   assert(NULL != p_start);
   assert(NULL != p_end);
   assert(NULL != p_reference);

   assert(Is_Abs_Value_Le_Pi (p_start->angle));
   assert(Is_Abs_Value_Le_Pi (p_end->angle));
   assert(Is_Abs_Value_Le_Pi (p_reference->angle));

   Create_Angle_Range(p_angle_range, p_start, p_end);

   f_inside = Is_Angle_Contained_In_Angle_Range(p_reference->angle, p_angle_range);
   if (Is_True(f_inside))
   {
      Swap_Angle_Range_Start_End(p_angle_range);
   }
}

Angle_Range_Fuse_State_T Angle_Range_Fuse(
   Angle_Range_T *p_angle_range,
   const Angle_Range_T *const p_range_a,
   const Angle_Range_T *const p_range_b
   )
{
   boolean_T f_in_range;
   Angle_Range_Fuse_State_T angle_range_fuse_state;

   assert(NULL != p_range_a);
   assert(NULL != p_range_b);

   assert(Is_Abs_Value_Le_Pi (p_range_a->start));
   assert(Is_Abs_Value_Le_Pi (p_range_a->end));
   assert(Is_Abs_Value_Le_Pi (p_range_b->start));
   assert(Is_Abs_Value_Le_Pi (p_range_b->end));

   angle_range_fuse_state.f_success = TRUE;

   /*
   1)
   .......|------|.
   ...|--------|...
   2)
   .......|--|.....
   ...|--------|...
   3)
   .|----------|-
   ---|......|---
   4)
   .|------|.........
   .....|--------|...
   5)
   ...|--------|...
   .......|--|.....
   6)
   ---|......|---
   .|----------|.
   7)
   .|--|.........
   .......|---|..
   */
   f_in_range = Is_Angle_Contained_In_Angle_Range(p_range_a->start, p_range_b);
   if (Is_True(f_in_range))
   {
      /*
      1)
      .......|------|.
      ...|--------|...
      2)
      .......|--|.....
      ...|--------|...
      3)
      .|----------|-
      ---|......|---
      6)
      ---|......|---
      .|----------|.
      */
      f_in_range = Is_Angle_Contained_In_Angle_Range(p_range_a->end, p_range_b);
      if (Is_True(f_in_range))
      {
         /*
         2)
         .......|--|.....
         ...|--------|...
         3)
         .|----------|-
         ---|......|---
         6)
         ---|......|---
         .|----------|.
         */
         f_in_range = Is_Angle_Contained_In_Angle_Range(p_range_b->start, p_range_a);
         if (Is_True(f_in_range))
         {
            /*
            3)
            .|----------|-
            ---|......|---
            6)
            ---|......|---
            .|----------|.
            Or identical ranges
            ..|----|...
            ..|----|...
            */

            boolean_T f_start_identical = (Is_Equal(p_range_a->start,p_range_b->start)); /* PRQA S 3341 */ /* check of float for equality is intensional */
            boolean_T f_end_identical = (Is_Equal(p_range_a->end,p_range_b->end)); /* PRQA S 3341 */       /* check of float for equality is intensional */
            if (Is_False(f_start_identical) && Is_False(f_end_identical))
            {
               p_angle_range->start = -PI;
               p_angle_range->end = PI;
               angle_range_fuse_state.f_two_pi = TRUE;
               angle_range_fuse_state.f_start_from_a = FALSE;
               angle_range_fuse_state.f_start_from_b = FALSE;
               angle_range_fuse_state.f_end_from_a = FALSE;
               angle_range_fuse_state.f_end_from_b = FALSE;
            }
            else
            {
               p_angle_range->start = p_range_b->start;
               p_angle_range->end = p_range_b->end;
               angle_range_fuse_state.f_two_pi = FALSE;
               angle_range_fuse_state.f_start_from_a = FALSE;
               angle_range_fuse_state.f_start_from_b = TRUE;
               angle_range_fuse_state.f_end_from_a = FALSE;
               angle_range_fuse_state.f_end_from_b = TRUE;
            }
         }
         else
         {
            /*
            2)
            .......|--|.....
            ...|--------|...
            */
            p_angle_range->start = p_range_b->start;
            p_angle_range->end   = p_range_b->end;
            angle_range_fuse_state.f_two_pi = FALSE;
            angle_range_fuse_state.f_start_from_a = FALSE;
            angle_range_fuse_state.f_start_from_b = TRUE;
            angle_range_fuse_state.f_end_from_a = FALSE;
            angle_range_fuse_state.f_end_from_b = TRUE;
         }
      }
      else
      {
         p_angle_range->start = p_range_b->start;
         p_angle_range->end   = p_range_a->end;
         angle_range_fuse_state.f_two_pi = FALSE;
         angle_range_fuse_state.f_start_from_a = FALSE;
         angle_range_fuse_state.f_start_from_b = TRUE;
         angle_range_fuse_state.f_end_from_a = TRUE;
         angle_range_fuse_state.f_end_from_b = FALSE;
      }
   }
   else
   {
      /*
      4)
      .|------|.........
      .....|--------|...
      5)
      ...|--------|...
      .......|--|.....
      7)
      .|--|.........
      .......|---|..
      */
      f_in_range = Is_Angle_Contained_In_Angle_Range(p_range_a->end, p_range_b);
      if (Is_True(f_in_range))
      {
         /*
         4)
         .|------|.........
         .....|--------|...
         */
         p_angle_range->start = p_range_a->start;
         p_angle_range->end   = p_range_b->end;
         angle_range_fuse_state.f_two_pi = FALSE;
         angle_range_fuse_state.f_start_from_a = TRUE;
         angle_range_fuse_state.f_start_from_b = FALSE;
         angle_range_fuse_state.f_end_from_a = FALSE;
         angle_range_fuse_state.f_end_from_b = TRUE;
      }
      else
      {
         /*
         5)
         ...|--------|...
         .......|--|.....
         7)
         .|--|.........
         .......|---|..
         */
         f_in_range = Is_Angle_Contained_In_Angle_Range(p_range_b->start, p_range_a);
         if (Is_True(f_in_range))
         {
            /*
            5)
            ...|--------|...
            .......|--|.....
            */
            p_angle_range->start = p_range_a->start;
            p_angle_range->end = p_range_a->end;
            angle_range_fuse_state.f_two_pi = FALSE;
            angle_range_fuse_state.f_start_from_a = TRUE;
            angle_range_fuse_state.f_start_from_b = FALSE;
            angle_range_fuse_state.f_end_from_a = TRUE;
            angle_range_fuse_state.f_end_from_b = FALSE;
#ifndef NDEBUG
            f_in_range = Is_Angle_Contained_In_Angle_Range(p_range_b->end, p_range_a);
            assert(f_in_range);
#endif
         }
         else
         {
            /*
            7)
            .|--|.........
            .......|---|..
            */
            p_angle_range->start = AS_TOOLBOX_INFINITY;
            p_angle_range->end = AS_TOOLBOX_INFINITY;
            angle_range_fuse_state.f_success = FALSE;
            angle_range_fuse_state.f_two_pi = FALSE;
            angle_range_fuse_state.f_start_from_a = FALSE;
            angle_range_fuse_state.f_start_from_b = FALSE;
            angle_range_fuse_state.f_end_from_a = FALSE;
            angle_range_fuse_state.f_end_from_b = FALSE;
         }
      }
   }

   return angle_range_fuse_state;
}

boolean_T Does_Angle_Range_Overlap_Angle_Range(
   const Angle_Range_T *const p_range_a,
   const Angle_Range_T *const p_range_b)
{
   boolean_T result;

   assert(NULL != p_range_a);
   assert(NULL != p_range_b);

   assert(Is_Abs_Value_Le_Pi (p_range_a->start));
   assert(Is_Abs_Value_Le_Pi (p_range_a->end));
   assert(Is_Abs_Value_Le_Pi (p_range_b->start));
   assert(Is_Abs_Value_Le_Pi (p_range_b->end));

   result = Is_Angle_Contained_In_Angle_Range(p_range_a->start, p_range_b);
   if (Is_False(result))
   {
      result = Is_Angle_Contained_In_Angle_Range(p_range_a->end, p_range_b);
   }
   if (Is_False(result))
   {
      result = Is_Angle_Contained_In_Angle_Range(p_range_b->start, p_range_a);
   }
   if (Is_False(result))
   {
      result = Is_Angle_Contained_In_Angle_Range(p_range_b->end, p_range_a);
   }

   return result;
}


void Initialize_Overlapping_Angle_Range(Overlapping_Angle_Range_T *p_overlapping_range)
{
   int i;

   assert(NULL != p_overlapping_range);

   p_overlapping_range->number_of_ranges = 0;
   for (i = 0; i < MAX_OVERLAPPING_RANGES; i++)
   {
      /* Since HUGE_VAL is way outside +-PI a huge value is as good as infinity.
         Using AS_TOOLBOX_INFINITY is OK. */
      p_overlapping_range->overlapping_ranges[i].start = AS_TOOLBOX_INFINITY;
      p_overlapping_range->overlapping_ranges[i].end = AS_TOOLBOX_INFINITY;
   }
}


static Overlap_Cases_T Determine_Overlap_Case(
   const Angle_Range_T *const p_range_a,
   const Angle_Range_T *const p_range_b)
{
   Overlap_Cases_T overlap_case;
   boolean_T       f_b_start_in_a;
   boolean_T       f_b_end_in_a;

   assert(NULL != p_range_a);
   assert(NULL != p_range_b);

   f_b_start_in_a = Is_Angle_Contained_In_Angle_Range(p_range_b->start, p_range_a);
   f_b_end_in_a   = Is_Angle_Contained_In_Angle_Range(p_range_b->end, p_range_a);

   if (Is_True(f_b_start_in_a))
   {
      if (Is_True(f_b_end_in_a))
      {
         /* Two possibilities.
         * either:
         * -----|.....|----  A aliased
         * ...|---------|    B
         * or:
         * ..|---------------|.. A not aliased
         * ........|---|........ B
         */
         overlap_case = FULL_OVERLAP;
      }
      else
      {
         /* .|-----|..... A
         * .....|-----|. B */
         overlap_case = OVERLAP_B_START_IN_A;
      }
   }
   else
   {
      if (Is_True(f_b_end_in_a))
      {
         /* .....|-----|. A
         * .|-----|..... B */
         overlap_case = OVERLAP_A_START_IN_B;
      }
      else
      {
         /* .|-|........ A
         *  .......|-|.. B */
         overlap_case = NO_OVERLAP;
      }
   }

   return overlap_case;
}


static void Handle_Overlap_A_Start_In_B(
   Overlapping_Angle_Range_T *p_overlapping_range,
   const Angle_Range_T *const p_range_a,
   const Angle_Range_T *const p_range_b)
{
   assert(NULL != p_overlapping_range);
   assert(NULL != p_range_a);
   assert(NULL != p_range_b);

   /* .....|------------|. A
    * .|-------|.......... B */
   p_overlapping_range->number_of_ranges            = 1;
   p_overlapping_range->overlapping_ranges[0].start = p_range_a->start;
   p_overlapping_range->overlapping_ranges[0].end   = p_range_b->end;
}


static void Handle_Overlap_Full_Overlap(
   Overlapping_Angle_Range_T *p_overlapping_range,
   const Angle_Range_T *const p_range_a,
   const Angle_Range_T *const p_range_b)
{
   Overlap_Cases_T overlap_case;

   assert(NULL != p_overlapping_range);
   assert(NULL != p_range_a);
   assert(NULL != p_range_b);

   /* Two possibilities, testing A overlaps B:
    * FULL_OVERLAP)
    * ----|      |---- A aliased
    * ..|---------|... B
    * NO_OVERLAP)
    * .|---------------|. A not aliased
    * .......|---|....... B
    * OVERLAP_B_START_IN_A)
    * .|---------------|. A end equals b end
    * .............|---|. B
    * OVERLAP_B_START_IN_A)
    * .|---------------|. A start equals b start
    * .|---|............. B
    * OVERLAP_A_START_IN_B)
    * .............|---|. A end equals b end
    * .|---------------|. B
    * OVERLAP_A_START_IN_B)
    * .|---|............. A start equals b start
    * .|---------------|. B
    */
   overlap_case = Determine_Overlap_Case(p_range_b, p_range_a);
   switch (overlap_case)
   {
   case FULL_OVERLAP:
   {
      /*a)*/
      /* Now check if the ranges are identical */
      if ((Abs(p_range_a->start - p_range_b->start) < THRESHOLD_IS_ZERO) &&
         (Abs(p_range_a->end - p_range_b->end) < THRESHOLD_IS_ZERO))
      {
         p_overlapping_range->number_of_ranges = 1;
         p_overlapping_range->overlapping_ranges[0].start = p_range_a->start;
         p_overlapping_range->overlapping_ranges[0].end = p_range_a->end;
      }
      else
      {
         p_overlapping_range->number_of_ranges = 2;
         p_overlapping_range->overlapping_ranges[0].start = p_range_b->start;
         p_overlapping_range->overlapping_ranges[0].end = p_range_a->end;
         p_overlapping_range->overlapping_ranges[1].start = p_range_a->start;
         p_overlapping_range->overlapping_ranges[1].end = p_range_b->end;
      }
   }
   break;
   case NO_OVERLAP:
      /*b)*/
      /* The first call to Determine_Overlap_Case()
         would have given OVERLAP_A_START_IN_B or
         OVERLAP_B_START_IN_A if range_A would be
         smaller than range_B */
   case OVERLAP_A_START_IN_B:
   case OVERLAP_B_START_IN_A:
   default:
      p_overlapping_range->number_of_ranges = 1;
      p_overlapping_range->overlapping_ranges[0].start = p_range_b->start;
      p_overlapping_range->overlapping_ranges[0].end = p_range_b->end;
      break;
   }
}


void Get_Overlapping_Angle_Range(
   Overlapping_Angle_Range_T *p_overlapping_range,
   const Angle_Range_T *const p_range_a,
   const Angle_Range_T *const p_range_b)
{
   const Angle_Range_T *p_range_a_internal;
   const Angle_Range_T *p_range_b_internal;
   Overlap_Cases_T      overlap_case;

   assert(NULL != p_overlapping_range);
   assert(NULL != p_range_a);
   assert(NULL != p_range_b);

   assert(Is_Abs_Value_Le_Pi (p_range_a->start));
   assert(Is_Abs_Value_Le_Pi (p_range_a->end));
   assert(Is_Abs_Value_Le_Pi (p_range_b->start));
   assert(Is_Abs_Value_Le_Pi (p_range_b->end));

   p_range_a_internal = p_range_a;
   p_range_b_internal = p_range_b;

   overlap_case = Determine_Overlap_Case(p_range_a_internal, p_range_b_internal);
   if (NO_OVERLAP == overlap_case)
   {
      /* no overlap, swap and try again */
      p_range_a_internal = p_range_b;
      p_range_b_internal = p_range_a;
      overlap_case       = Determine_Overlap_Case(p_range_a_internal, p_range_b_internal);
   }
   switch (overlap_case)
   {
   case OVERLAP_B_START_IN_A:
      Handle_Overlap_A_Start_In_B(p_overlapping_range, p_range_b_internal, p_range_a_internal); /* B start in A is the same case as A start in B just swapped parameters */
      break;
   case OVERLAP_A_START_IN_B:
      Handle_Overlap_A_Start_In_B(p_overlapping_range, p_range_a_internal, p_range_b_internal);
      break;
   case FULL_OVERLAP:
      Handle_Overlap_Full_Overlap(p_overlapping_range, p_range_a_internal, p_range_b_internal);
      break;
   case NO_OVERLAP:
   default:
      Initialize_Overlapping_Angle_Range(p_overlapping_range);
      break;
   }
}


float32_T Get_Angle_Range_Width_Float(const Angle_Range_T *const p_angle_range)
{
   float32_T result;

   assert(NULL != p_angle_range);

   assert(Is_Abs_Value_Le_Pi (p_angle_range->start));
   assert(Is_Abs_Value_Le_Pi (p_angle_range->end));

   if (p_angle_range->start <= p_angle_range->end)
   {
      result = p_angle_range->end - p_angle_range->start;
   }
   else
   {
      /* aliased */
      float32_T diff_of_angles;
      diff_of_angles = PI - p_angle_range->start;
      diff_of_angles += p_angle_range->end + PI;
      result      = diff_of_angles;
   }
   return result;
}


Angle_T Get_Angle_Range_Width_Angle(const Angle_Range_T *const p_angle_range)
{
   Angle_T result;

   assert(NULL != p_angle_range);

   result = Create_Angle(Get_Angle_Range_Width_Float(p_angle_range));

   return result;
}


void Swap_Angle_Range_Start_End(Angle_Range_T *p_angle_range)
{
   float32_T helper;

   assert(NULL != p_angle_range);

   helper = p_angle_range->start;
   p_angle_range->start = p_angle_range->end;
   p_angle_range->end   = helper;
}


Angle_T Get_Angle_Range_Start_Angle(const Angle_Range_T *p_angle_range)
{
   assert(NULL != p_angle_range);
   return Create_Angle(p_angle_range->start);
}


Angle_T Get_Angle_Range_End_Angle(const Angle_Range_T *p_angle_range)
{
   assert(NULL != p_angle_range);
   return Create_Angle(p_angle_range->end);
}


float32_T Get_Angle_Range_Start(const Angle_Range_T *p_angle_range)
{
   assert(NULL != p_angle_range);
   return p_angle_range->start;
}


float32_T Get_Angle_Range_End(const Angle_Range_T *p_angle_range)
{
   assert(NULL != p_angle_range);
   return p_angle_range->end;
}

float32_T Get_Angle_Range_Center(const Angle_Range_T *p_angle_range)
{
   float center;
   float width;

   assert(NULL != p_angle_range);

   width = Get_Angle_Range_Width_Float(p_angle_range);

   center = p_angle_range->start + (0.5f * width);

   center = Normalize_Angle(center, 0.0f);

   return center;
}

