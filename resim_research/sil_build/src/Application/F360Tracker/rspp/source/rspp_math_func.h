#ifndef RSPP_MATH_FUNC_H
#define RSPP_MATH_FUNC_H
/*===========================================================================*/
/**
 * @file rspp_math_func.h
 *
 * @brief RSPP Mathematical Utility Functions
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Mathematical utility functions for RSPP module including matrix operations,
 * sorting algorithms, and mathematical computations.
 *
 * @section ABBR ABBREVIATIONS:
 *   - RSPP: Radar Signal Pre-Processing
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
 *       wiki/53-SoftwareDetailedDesigns/AAU_SDD_RSPP
 *
 *   - Requirements Document(s):
 *     - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
 *       wiki/51-SoftwareRequirementsSpecifications/CMP_SRS_TrackerCore
 *
 *   - Applicable Standards (in order of precedence: highest first):
 *     - https://confluence.asux.aptiv.com/spaces/F360Core/pages/129995883/Coding+Guidelines
 *     - ESGW_4-2_PE-SWX_00-01-A01_EN - C++ Coding Standards [20190526]
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
  *   - None.
 *
 * @ updates to areas outside the scope of procedures:
 *   - Refer to module footer comment block.
 *
 * @defgroup rspp_math_func Math Functions
 * @{
 */
/*===========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/
#include "rspp_reuse.h"
#include <algorithm>
#include <cstring>

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "rspp_math.h"
#include "rspp_constants.h"
#include "rspp_iterator.h"
#include "rspp_sort_data_type.h"
#include "rspp_functional.h"

/*===========================================================================*
 * Forward Declarations
 *===========================================================================*/

/*===========================================================================*
 * Namespace Definition
 *===========================================================================*/
namespace rspp_variant_A
{
   /*===========================================================================*
    * Exported Enum Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Type Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Function Declarations
    *===========================================================================*/

   /******************************************************************************
    * Name:  RSPP_Get_Hypotenuse_Squared
    *   This function calculates the squared hypotenuse (a*a + b*b) without
    *   performing a square root operation, providing performance optimization.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   a - First value
    *   b - Second value
    *
    * Return Value:
    *   Squared hypotenuse (a*a + b*b)
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   float32_t RSPP_Get_Hypotenuse_Squared(
       const float32_t a,
       const float32_t b);

   /******************************************************************************
    * Name:  RSPP_Saturate
    *   This function clamps an input value within specified minimum and maximum
    *   bounds, ensuring the output is within [min_value, max_value].
    *
    * Shared Variables: None
    *
    * Parameters:
    *   input     - Value to be clamped
    *   min_value - Minimum allowable value
    *   max_value - Maximum allowable value
    *
    * Return Value:
    *   Clamped output value
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   float32_t RSPP_Saturate(
       const float32_t input,
       const float32_t min_value,
       const float32_t max_value);

   /******************************************************************************
    * Name:  RSPP_Matmul_MxN_NxP
    *   This template function performs matrix multiplication of an MxN matrix
    *   with an NxP matrix, supporting various numeric types with runtime
    *   dimension validation.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   mat1       - First matrix to be multiplied [M][N]
    *   mat2       - Second matrix to be multiplied [N][P]
    *   result_mat - Output matrix for result [M][P]
    *   row1       - Number of rows to multiply in first matrix (default: M)
    *   col1       - Number of columns to multiply in first matrix (default: N)
    *   col2       - Number of columns to multiply in second matrix (default: P)
    *
    * Return Value: None
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   template <typename T, std::size_t M, std::size_t N, std::size_t P>
   void RSPP_Matmul_MxN_NxP(
       const T (&mat1)[M][N],
       const T (&mat2)[N][P],
       T (&result_mat)[M][P],
       const uint32_t row1 = M,
       const uint32_t col1 = N,
       const uint32_t col2 = P)
   {
      for (uint32_t row_mat_1 = 0U; row_mat_1 < row1; row_mat_1++)
      {
         for (uint32_t col_mat_2 = 0U; col_mat_2 < col2; col_mat_2++)
         {
            T sum{};
            for (uint32_t col_mat_1 = 0U; col_mat_1 < col1; col_mat_1++)
            {
               sum = sum + (mat1[row_mat_1][col_mat_1] * mat2[col_mat_1][col_mat_2]);
            }
            result_mat[row_mat_1][col_mat_2] = sum;
         }
      }
   }

   /******************************************************************************
    * Name:  RSPP_Matmul_MxN_PxN_Transpose
    *   This template function performs matrix multiplication of an MxN matrix
    *   with the transpose of a PxN matrix (equivalent to mat1 * mat2^T),
    *   supporting various numeric types with runtime dimension validation.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   mat1       - First matrix to be multiplied [M][N]
    *   mat2       - Second matrix to be transposed and multiplied [P][N]
    *   result_mat - Output matrix for result [M][P]
    *   row1       - Number of rows to multiply in first matrix (default: M)
    *   col1       - Number of columns to multiply in first matrix (default: N)
    *   row2       - Number of rows to multiply in second matrix (default: P)
    *
    * Return Value: None
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   template <typename T, std::size_t M, std::size_t N, std::size_t P>
   void RSPP_Matmul_MxN_PxN_Transpose(
       const T (&mat1)[M][N],
       const T (&mat2)[P][N],
       T (&result_mat)[M][P],
       const uint32_t row1 = M,
       const uint32_t col1 = N,
       const uint32_t row2 = P)
   {
      for (uint32_t row_mat_1 = 0U; row_mat_1 < row1; row_mat_1++)
      {
         // The second matrix is not transposed, row_mat_2 is col_mat_2 in the transposed matrix
         for (uint32_t row_mat_2 = 0U; row_mat_2 < row2; row_mat_2++)
         {
            T sum{};
            for (uint32_t col_mat_1 = 0U; col_mat_1 < col1; col_mat_1++)
            {
               sum = sum + (mat1[row_mat_1][col_mat_1] * mat2[row_mat_2][col_mat_1]);
            }
            result_mat[row_mat_1][row_mat_2] = sum;
         }
      }
   }

   /******************************************************************************
    * Name:  RSPP_Merge_Sublists
    *   This template function merges two sorted sublists into a single sorted
    *   list, supporting custom comparison operators. Used internally by the
    *   mergesort algorithm.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   sublist1_start - Start index of first sublist
    *   sublist2_start - Start index of second sublist
    *   merged_list_end - End index of merged list
    *   in_list        - Input array containing sublists to merge
    *   out_list       - Output array for merged result
    *   comp           - Comparison function for sorting
    *
    * Return Value: None
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   template <typename T, typename Compare, size_t N>
   void RSPP_Merge_Sublists(
       const uint32_t sublist1_start,
       const uint32_t sublist2_start,
       const uint32_t merged_list_end,
       T (&in_list)[N],
       T (&out_list)[N],
       const Compare comp)
   {
      uint32_t sublist1_idx = sublist1_start;
      uint32_t sublist2_idx = sublist2_start;
      uint32_t outlist_idx = sublist1_start;

      // In the last iteration we could end up here with only one sublist
      if ((sublist2_idx < merged_list_end) && (sublist1_idx < sublist2_start))
      {
         bool f_continue = true;
         while (f_continue)
         {
            // Place one of the sublist elements in the outlist and update indices
            // Comparing 2 with 1, instead of 1 with 2, results is stability
            if (comp(in_list[sublist2_idx], in_list[sublist1_idx]))
            {
               out_list[outlist_idx] = in_list[sublist2_idx];
               sublist2_idx++;
               outlist_idx++;
               if (sublist2_idx >= merged_list_end)
               {
                  f_continue = false;
               }
            }
            else
            {
               out_list[outlist_idx] = in_list[sublist1_idx];
               sublist1_idx++;
               outlist_idx++;
               if (sublist1_idx >= sublist2_start)
               {
                  f_continue = false;
               }
            }
         }
      }

      // One sublist is empty, move everything from remaining sublist
      while ((sublist1_idx < sublist2_start) && (sublist1_idx < merged_list_end))
      {
         out_list[outlist_idx] = in_list[sublist1_idx];
         outlist_idx++;
         sublist1_idx++;
      }

      while (sublist2_idx < merged_list_end)
      {
         out_list[outlist_idx] = in_list[sublist2_idx];
         outlist_idx++;
         sublist2_idx++;
      }
   }

   /******************************************************************************
    * Name:  RSPP_Mergesort
    *   This template function performs merge sort on an array using a custom
    *   comparison function, with stable sort behavior. Uses an internal buffer
    *   for merge operations and is optimized for performance.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   array_to_sort - Array to be sorted (modified in place)
    *   count         - Number of elements to sort
    *   comp          - Comparison function for sorting order
    *
    * Return Value: None
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   template <typename T, typename Compare, size_t N>
   void RSPP_Mergesort(
       T (&array_to_sort)[N],
       const uint32_t count,
       const Compare comp)
   {
      T buffer[N];
      bool f_result_in_buffer = false;

      // First iteration. Sublist length 1 -> 2
      for (uint32_t i = 1U; i < count; i += 2U)
      {
         if (comp(array_to_sort[i], array_to_sort[i - 1U]))
         {
            const T temp = array_to_sort[i];
            array_to_sort[i] = array_to_sort[i - 1U];
            array_to_sort[i - 1U] = temp;
         }
      }

      // Subsequent iterations
      uint32_t sublist_length = 2U;
      while (sublist_length < count)
      {
         // Call RSPP_Merge_Sublists with either array_to_sort or buffer as input list
         if (f_result_in_buffer)
         {
            // Loop over all sublists
            for (uint32_t i = 0U; i < count; i += 2U * sublist_length)
            {
               // Make sure we don't go outside the size of the array to sort and call merge function
               const uint32_t merged_list_end = (count < (i + 2U * sublist_length)) ? count : (i + 2U * sublist_length);
               RSPP_Merge_Sublists(i, i + sublist_length, merged_list_end, buffer, array_to_sort, comp);
            }
         }
         else
         {
            for (uint32_t i = 0U; i < count; i += 2U * sublist_length)
            {
               const uint32_t merged_list_end = (count < (i + 2U * sublist_length)) ? count : (i + 2U * sublist_length);
               RSPP_Merge_Sublists(i, i + sublist_length, merged_list_end, array_to_sort, buffer, comp);
            }
         }

         // Keep track if the result is in array_to_sort or in buffer
         f_result_in_buffer = !f_result_in_buffer;
         sublist_length = 2U * sublist_length;
      }

      // If the merged list is currently in the buffer
      if (f_result_in_buffer)
      {
         const size_t list_size = static_cast<size_t>(count) * sizeof(T);
         (void)memcpy(array_to_sort, &buffer[0], list_size);
      }
   }

   /******************************************************************************
    * Name:  RSPP_Sort
    *   This template function sorts a float32_t array and provides permutation
    *   indices tracking the original positions of elements through the sort.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   arr          - Array to sort (modified in place)
    *   num_elements - Number of elements to sort
    *   f_ascending  - True for ascending order, false for descending
    *   perm         - Output array containing permutation indices
    *
    * Return Value:
    *   true - If sorting was successful
    *   false - If num_elements > N
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   template <std::size_t N>
   bool RSPP_Sort(
       float32_t (&arr)[N],
       const uint32_t num_elements,
       const bool f_ascending,
       uint32_t (&perm)[N])
   {
      bool sort_successful;
      if (num_elements == 0U)
      {
         // Nothing to sort
         sort_successful = true;
      }
      else if (num_elements <= N)
      {
         RSPP_Sort_Data_T work_data[N];
         for (uint32_t i = 0U; i < num_elements; i++)
         {
            work_data[i].index = i;
            work_data[i].data = arr[i];
         }

         if (f_ascending)
         {
            RSPP_Mergesort(work_data, num_elements, cmn::f360_less<RSPP_Sort_Data_T>());
         }
         else
         {
            RSPP_Mergesort(work_data, num_elements, cmn::f360_greater<RSPP_Sort_Data_T>());
         }

         for (uint32_t i = 0U; i < num_elements; i++)
         {
            perm[i] = work_data[i].index;
            arr[i] = work_data[i].data;
         }
         sort_successful = true;
      }
      else
      {
         sort_successful = false;
      }

      return sort_successful;
   }

   /******************************************************************************
    * Name:  RSPP_Piecewise_Linear_Equation
    *   This template function evaluates a piecewise linear function at a given
    *   point using linear interpolation between breakpoints. Values outside the
    *   breakpoint range are clamped to first/last Y values.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   x_question     - X coordinate to evaluate the function at
    *   breakpoints_x  - Array containing X breakpoints (must be increasing)
    *   breakpoints_y  - Array containing Y values at breakpoints
    *   num_breakpoints - Number of breakpoints to use (default: N)
    *
    * Return Value:
    *   Interpolated Y value at x_question
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   template <std::size_t N>
   float32_t RSPP_Piecewise_Linear_Equation(
       const float32_t x_question,
       const float32_t (&breakpoints_x)[N],
       const float32_t (&breakpoints_y)[N],
       const uint32_t num_breakpoints = N)
   {
      float32_t y_question;
      // Compute value of piecewise linear function
      if (x_question < breakpoints_x[0U])
      {
         // If x is less than the smallest breakpoint_x value then y
         // is choosen as the breakpoint_y value that corresponds to
         // the smallest breakpoint_x value
         y_question = breakpoints_y[0U];
      }
      else if (x_question > breakpoints_x[num_breakpoints - 1U])
      {
         // If x is larger than than the smallest breakpoint_x value
         // then y is choosen as the breakpoint_y value that corresponds
         // to the largest breakpoint_x value
         y_question = breakpoints_y[num_breakpoints - 1U];
      }
      else
      {
         // Find first value in sorted breakpoints_x that are smaller
         // than x
         uint32_t ind = 1U;
         while (x_question > breakpoints_x[ind])
         {
            ind++;
         }
         // We now know that:
         // sorted_breakpoints_x[ind-1]) <= x_question < sorted_breakpoints_x[ind]
         // Define a line y(x) = k*x + m through the points defined by
         // sorted_breakpoints_x[ind-1] and sorted_breakpoints_x[ind] and evaluate
         // y_question for the given input x_question.

         // Coordinates of two points on the line y(x) = k*x + m;
         const float32_t point1_x = breakpoints_x[ind - 1U];
         const float32_t point1_y = breakpoints_y[ind - 1U];
         const float32_t point2_x = breakpoints_x[ind];
         const float32_t point2_y = breakpoints_y[ind];

         const float32_t min_denominator = 1e-6F;
         if (std::abs(point2_x - point1_x) < min_denominator) // Protect from division with 0
         {
            // Take the mean of point2_y and point1_y
            y_question = 0.5F * (point2_y + point1_y);
         }
         else
         {
            const float32_t k = (point2_y - point1_y) / (point2_x - point1_x);
            const float32_t m = point1_y - k * point1_x;

            y_question = k * x_question + m;
         }
      }

      return y_question;
   }
}

/** @} doxygen end group */
#endif

/*============================================================================*\
 * AUTHOR(S) IDENTITY (AID)
 *-----------------------------------------------------------------------------
 *
 *  AID         NAME
 *  ---------------------------------------------------------------------------
 *  wzfkqj      Tobias Almroth
\*============================================================================*/

/*============================================================================*\
 * FILE REVISION HISTORY
 *-----------------------------------------------------------------------------
 *
 *  File history can be traced by URL:
 *  "https://gitgerrit.asux.aptiv.com/q/project:CORECOMP%252FALSW%252FOT_ObjectTracking"
\*============================================================================*/

/* END OF FILE -------------------------------------------------------------- */
