/*===================================================================================*\
* FILE: f360_math_func.cpp
*====================================================================================
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*------------------------------------------------------------------------------------
* %full_filespec: AIT-69%
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains  c version of matlab built-in functions.
*
* ABBREVIATIONS:
*   NONE
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/


/******************************
* Includes
*******************************/
#include <cstring>
#include <algorithm>
#include "f360_math.h"
#include "f360_math_func.h"
#include "f360_norm_heading_angle.h"

namespace f360_variant_A
{

   /*===========================================================================*\
   * FUNCTION: F360_Get_Hypotenuse()
   *===========================================================================
   * RETURN VALUE:
   * float32_t hypot - Hypotenuse
   *
   * PARAMETERS:
   * const float32_t a
   * const float32_t b
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function calculates and returns the hypoteneus of a and b.
   *
   \*===========================================================================*/
   float32_t F360_Get_Hypotenuse(
      const float32_t a,
      const float32_t b)
   {
      return F360_Sqrtf((a * a) + (b * b));
   }

   /*===========================================================================*\
   * FUNCTION: F360_Get_Hypotenuse_Squared()
   *===========================================================================
   * RETURN VALUE:
   * float32_t hypot_squared - Hypotenuse squared
   *
   * PARAMETERS:
   * const float32_t a
   * const float32_t b
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function calculates and returns the squared hypotenuse of a and b.
   *
   \*===========================================================================*/
   float32_t F360_Get_Hypotenuse_Squared(
      const float32_t a,
      const float32_t b)
   {
      return (a * a) + (b * b);
   }

   /*===========================================================================*\
   * FUNCTION: F360_Saturate()
   *===========================================================================
   * RETURN VALUE:
   * float32_t max
   *
   * PARAMETERS:
   * const float32_t input
   * const float32_t min_value
   * const float32_t max_value
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * OBSOLETE - to be replaced in DFU-898 by Clamp
   * Saturate value between Min and Max
   *
   \*===========================================================================*/
   float32_t F360_Saturate(
      const float32_t input,
      const float32_t min_value,
      const float32_t max_value)
   {
      float32_t output = std::max(input, min_value);
      output = std::min(output, max_value);
      return output;
   }

   /*===========================================================================*\
   * FUNCTION: F360_matinv_2x2()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t (&X)[2][2] - Matrix to invert
   * float32_t (&result)[2][2] - Inverse of X
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function inverts a 2x2 matrix.
   *
   \*===========================================================================*/
   bool F360_matinv_2x2(
      const float32_t(&X)[2][2],
      float32_t(&result)[2][2])
   {
      bool ret;
      const float32_t det = X[0][0] * X[1][1] - X[0][1] * X[1][0];
      if (det > F360_EPSILON)
      {
         result[0][0] = X[1][1] / det;
         result[0][1] = -X[0][1] / det;
         result[1][0] = -X[1][0] / det;
         result[1][1] = X[0][0] / det;
         ret = true;
      }
      else
      {
         (void)memset(result, 0, sizeof(result));
         ret = false;
      }
      return ret;
   }

   /*===========================================================================*\
   * FUNCTION: F360_matinv_3x3()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t (&X)[3][3] - Matrix to invert
   * float32_t (&result)[3][3] - Inverse of X
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function inverts a 3x3 matrix.
   *
   \*===========================================================================*/
   bool F360_matinv_3x3(
      const float32_t(&X)[3][3],
      float32_t(&result)[3][3])
   {
      bool ret;
      const float32_t den = X[0][0] * X[1][1] * X[2][2] - X[0][0] * X[1][2] * X[2][1] - X[0][1] * X[1][0] * X[2][2] + X[0][1] * X[1][2] * X[2][0] + X[0][2] * X[1][0] * X[2][1] - X[0][2] * X[1][1] * X[2][0];
      if (den > F360_EPSILON)
      {
         result[0][0] = (X[1][1] * X[2][2] - X[1][2] * X[2][1]) / den;
         result[0][1] = -(X[0][1] * X[2][2] - X[0][2] * X[2][1]) / den;
         result[0][2] = (X[0][1] * X[1][2] - X[0][2] * X[1][1]) / den;
         result[1][0] = -(X[1][0] * X[2][2] - X[1][2] * X[2][0]) / den;
         result[1][1] = (X[0][0] * X[2][2] - X[0][2] * X[2][0]) / den;
         result[1][2] = -(X[0][0] * X[1][2] - X[0][2] * X[1][0]) / den;
         result[2][0] = (X[1][0] * X[2][1] - X[1][1] * X[2][0]) / den;
         result[2][1] = -(X[0][0] * X[2][1] - X[0][1] * X[2][0]) / den;
         result[2][2] = (X[0][0] * X[1][1] - X[0][1] * X[1][0]) / den;
         ret = true;
      }
      else
      {
         (void)memset(result, 0, sizeof(result));
         ret = false;
      }
      return ret;
   }

   void F360_Matmul_6x6_6x6(
      const float32_t(&mat1)[6][6],
      const float32_t(&mat2)[6][6],
      float32_t(&result_mat)[6][6])
   {
      // Combine outer loops to enable software pipelining
      uint32_t row = 0U;
      uint32_t col = 0U;
      for (uint32_t i = 0U; i < 36U; i++)
      {
         result_mat[row][col] =
            (mat1[row][0] * mat2[0][col]) +
            (mat1[row][1] * mat2[1][col]) +
            (mat1[row][2] * mat2[2][col]) +
            (mat1[row][3] * mat2[3][col]) +
            (mat1[row][4] * mat2[4][col]) +
            (mat1[row][5] * mat2[5][col]);

         // simple conditionals won't break pipeline
         col++;
         const bool f_row_done = (col == 6U);
         row = (f_row_done) ? (row + 1U) : row;
         col = (f_row_done) ? 0U : col;
      }
   }

   void F360_Matmul_6x6_6x6_Transpose(
      const float32_t(&mat1)[6][6],
      const float32_t(&mat2)[6][6],
      float32_t(&result_mat)[6][6])
   {
      // Combine outer loops to enable software pipelining
      uint32_t row = 0U;
      uint32_t col = 0U;
      for (uint32_t i = 0U; i < 36U; i++)
      {
         result_mat[row][col] =
            (mat1[row][0] * mat2[col][0]) +
            (mat1[row][1] * mat2[col][1]) +
            (mat1[row][2] * mat2[col][2]) +
            (mat1[row][3] * mat2[col][3]) +
            (mat1[row][4] * mat2[col][4]) +
            (mat1[row][5] * mat2[col][5]);

         // simple conditionals won't break pipeline
         col++;
         const bool f_row_done = (col == 6U);
         row = (f_row_done) ? (row + 1U) : row;
         col = (f_row_done) ? 0U : col;
      }
   }

   void F360_Matmul_6x6_6x6_Transpose_Symmetric(
      const float32_t(&mat1)[6][6],
      const float32_t(&mat2)[6][6],
      float32_t(&result_mat)[6][6])
   {
      // Combine outer loops to enable software pipelining
      uint32_t row = 0U;
      uint32_t col = 0U;
      const uint32_t n_loops = 21U;

      for (uint32_t i = 0U; i < n_loops; i++)
      {
         const float32_t sum =
            (mat1[row][0] * mat2[col][0]) +
            (mat1[row][1] * mat2[col][1]) +
            (mat1[row][2] * mat2[col][2]) +
            (mat1[row][3] * mat2[col][3]) +
            (mat1[row][4] * mat2[col][4]) +
            (mat1[row][5] * mat2[col][5]);

         result_mat[row][col] = sum;
         result_mat[col][row] = (col == row) ? result_mat[col][row] : sum;

         // simple conditionals won't break pipeline
         col++;
         const bool f_row_done = (col == 6U);
         row = (f_row_done) ? (row + 1U) : row;
         col = (f_row_done) ? row : col;
      }
   }

   void F360_Matadd_6x6_6x6(
      const float32_t(&mat1)[6][6],
      const float32_t(&mat2)[6][6],
      float32_t(&result_mat)[6][6])
   {
      for (size_t i = 0U; i < 6U; i++)
      {
         // unroll inner loop
         result_mat[i][0] = (mat1[i][0] + mat2[i][0]);
         result_mat[i][1] = (mat1[i][1] + mat2[i][1]);
         result_mat[i][2] = (mat1[i][2] + mat2[i][2]);
         result_mat[i][3] = (mat1[i][3] + mat2[i][3]);
         result_mat[i][4] = (mat1[i][4] + mat2[i][4]);
         result_mat[i][5] = (mat1[i][5] + mat2[i][5]);
      }
   }

   /*===========================================================================*\
   * FUNCTION: F360_Linear_Equation
   *===========================================================================
   * RETURN VALUE:
   * float32_t
   *
   * PARAMETERS:
   *  const float32_t x,
   *  const float32_t x1,
   *  const float32_t x2,
   *  const float32_t y1,
   *  const float32_t y2,
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Linear function on two points
   *
   \*===========================================================================*/
   float32_t F360_Linear_Equation(
      const float32_t x,
      const float32_t x1,
      const float32_t x2,
      const float32_t y1,
      const float32_t y2)
   {
      float32_t y;

      const float32_t denominator = x2 - x1;
      if (std::abs(denominator) > F360_EPSILON)
      {
         // Linear equation for 2 points.
         y = y1 + (((y2 - y1) / denominator) * (x - x1));
      }
      else
      {
         // Signum function.
         if (((x < x1) && (x1 <= x2)) || ((x > x1) && (x1 > x2)))
         {
            y = y1;
         }
         else if (((x < x2) && (x2 < x1)) || ((x > x2) && (x2 >= x1)))
         {
            y = y2;
         }
         else
         {
            y = (y1 + y2) * 0.5F;
         }
      }

      return y;
   }

   /*===========================================================================*\
   * FUNCTION: F360_Linear_Equation_With_Saturation()
   *===========================================================================
   * RETURN VALUE:
   * float32_t
   *
   * PARAMETERS:
   *  const float32_t x,
   *  const float32_t x1,
   *  const float32_t x2,
   *  const float32_t y1,
   *  const float32_t y2,
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Linear function on two points with saturation
   *
   \*===========================================================================*/
   float32_t F360_Linear_Equation_With_Saturation(
      const float32_t x,
      const float32_t x1,
      const float32_t x2,
      const float32_t y1,
      const float32_t y2)
   {
      float32_t y = F360_Linear_Equation(x, x1, x2, y1, y2);

      const float32_t y_min = std::min(y1, y2);
      const float32_t y_max = std::max(y1, y2);

      y = F360_Saturate(y, y_min, y_max);

      return y;
   }

   /*===========================================================================*\
   * FUNCTION: F360_Low_Pass_Filter_First_Order
   *===========================================================================
   * RETURN VALUE:
   * float32_t newly filtered value
   *
   * PARAMETERS:
   *  const float32_t new_input - new input to filter
   *  const float32_t prev_filt - previously filtered value
   *  const float32_t filter_coef - filter constant denoted as alpha in Abstract formula. (1-filter_coef) is weight for new_input and filter_coef is weight for prev_filt
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Simple low pass filter with filter_coef/alpha filtering coefficient
   * y(t) = y(t-1) + (alpha * (x(t) - y(t-1))) = alpha*x(t) + (1-alpha)*y(t-1) = filter_coef*new_input + (1-filter_coef)*prev_filt
   *
   * PRECONDITIONS:
   * Filter constant should be in interval 0 < filter_coef < 1
   *
   \*===========================================================================*/
   float32_t F360_Low_Pass_Filter_First_Order(
      const float32_t new_input,
      const float32_t prev_filt,
      const float32_t filter_coef)
   {
      return (prev_filt + (filter_coef * (new_input - prev_filt)));
   }

   /*===========================================================================*\
   * FUNCTION: F360_Low_Pass_Filter_Angle_First_Order
   *===========================================================================
   * RETURN VALUE:
   * float32_t newly filtered value
   *
   * PARAMETERS:
   *  const float32_t new_input - new input to filter
   *  const float32_t prev_filt - previously filtered value
   *  const float32_t filter_coef - filter constant denoted as alpha in Abstract formula. (1-filter_coef) is weight for new_input and filter_coef is weight for prev_filt
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Simple low pass filter with filter_coef/alpha filtering coefficient
   * y(t) = y(t-1) + (alpha * (x(t) - y(t-1))) = alpha*x(t) + (1-alpha)*y(t-1) = filter_coef*new_input + (1-filter_coef)*prev_filt
   *
   * This function also handles the ambiguities when filtering an angle value.
   *
   * PRECONDITIONS:
   * The function only handles angles in the interval -pi <= new_input,prev_filt <= pi
   * Filter constant should be in interval 0 < filter_coef < 1
   *
   \*===========================================================================*/
   float32_t F360_Low_Pass_Filter_Angle_First_Order(
      const float32_t new_input,
      const float32_t prev_filt,
      const float32_t filter_coef)
   {
      const float32_t delta = new_input - prev_filt;

      float32_t temp_y;
      if (delta > F360_PI)
      {
         temp_y = prev_filt + F360_2PI;
      }
      else if (delta < -F360_PI)
      {
         temp_y = prev_filt - F360_2PI;
      }
      else
      {
         temp_y = prev_filt;
      }

      const float32_t temp_x = F360_Low_Pass_Filter_First_Order(new_input, temp_y, filter_coef);

      return Normalize_Heading_Angle(temp_x, 0.0F);
   }

   /*===========================================================================*\
   * FUNCTION: F360_2d_Matrix_Determinant
   *===========================================================================
   * RETURN VALUE:
   * float32_t determinant of 2d matrix
   *
   * PARAMETERS:
   *
   * const float32_t (&matrix)[2][2]
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Determinant of 2d matrix
   *
   \*===========================================================================*/
   float32_t F360_2d_Matrix_Determinant(const float32_t(&matrix)[2][2])
   {
      return (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]);
   }

   /*===========================================================================*\
   * FUNCTION: F360_Safe_Log10f
   *===========================================================================
   * RETURN VALUE:
   * float32_t log10_value
   *
   * PARAMETERS:
   *
   * const float32_t value
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * safe version of F360_Log10f - in case of numerical instability
   * F360_Log10f(F360_EPSILON)
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t F360_Safe_Log10f(const float32_t value)
   {
      return (value > F360_EPSILON) ? F360_Log10f(value) : F360_Log10f(F360_EPSILON);
   }

   /*===========================================================================*\
   * FUNCTION: Rotate_2D_Covariance_Matrix_With_Precalc_Coeff()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t (&cov_mat)[2][2],
   * float32_t(&rotated_cov_mat)[2][2],
   * const float32_t sin_sq_angle,
   * const float32_t cos_sq_angle,
   * const float32_t sin_cos_angle
   * const float32_t cos_2_angle
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Based on calculated angle coefficents (sin^2, cos^2 and sin*cos, cos2)
   * functions rotates symetrical 2D Covariance Matrix.
   * When Rotation matrix R = [cos -sin; sin cos]
   * Function provides output from rotation equations (R*cov_mat*R').
   * --------------------------------------------------------------------------
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Rotate_2D_Covariance_Matrix_With_Precalc_Coeff(
      const float32_t(&cov_mat)[2][2],
      float32_t(&rotated_cov_mat)[2][2],
      const float32_t sin_sq_angle,
      const float32_t cos_sq_angle,
      const float32_t sin_cos_angle,
      const float32_t cos_2_angle)
   {
      const float32_t two_cov_sin_cos = 2.0F * cov_mat[0][1] * sin_cos_angle;
      rotated_cov_mat[0][0] = cov_mat[1][1] * sin_sq_angle + cov_mat[0][0] * cos_sq_angle - two_cov_sin_cos;
      rotated_cov_mat[0][1] = (cov_mat[0][0] - cov_mat[1][1]) * sin_cos_angle + cov_mat[0][1] * (cos_2_angle);
      rotated_cov_mat[1][0] = rotated_cov_mat[0][1];
      rotated_cov_mat[1][1] = cov_mat[0][0] * sin_sq_angle + cov_mat[1][1] * cos_sq_angle + two_cov_sin_cos;
   }

   /*===========================================================================*\
   * FUNCTION: Rotate_2D_Covariance_Matrix()
   *===========================================================================
   * RETURN VALUE:
   *
   * PARAMETERS:
   * const float32_t cos_rot_angle
   * const float32_t sin_rot_angle
   * const float32_t (&cov_mat)[2][2]
   * float32_t (&rotated_cov_mat)[2][2]
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Rotate covariance matrix, cov_rotated = R * cov_mat * R'.
   * Rotates a covariance matrix by an angle and saves it in rotated_cov_mat.
   * Function can be used for example when performing coordinate system
   * transformations. The input angle corresponds to the rotation angle needed
   * for the old coordinate system to coincide with the new coordinate system.
   * The rotation direction is positive when rotating from x-axis towards y-axis.
   *
   \*===========================================================================*/
   void Rotate_2D_Covariance_Matrix(
      const float32_t cos_rot_angle,
      const float32_t sin_rot_angle,
      const float32_t(&cov_mat)[2][2],
      float32_t(&rotated_cov_mat)[2][2])
   {
      const float32_t sin_sq_angle = sin_rot_angle * sin_rot_angle;
      const float32_t cos_sq_angle = cos_rot_angle * cos_rot_angle;
      const float32_t sin_cos_angle = sin_rot_angle * cos_rot_angle;
      // cos(2x) = cos^2-sin^2
      const float32_t cos_2_angle = cos_sq_angle - sin_sq_angle;

      Rotate_2D_Covariance_Matrix_With_Precalc_Coeff(cov_mat, rotated_cov_mat, sin_sq_angle, cos_sq_angle, sin_cos_angle, cos_2_angle);
   }

   /*===========================================================================*\
   * FUNCTION: F360_Translate_2D_Position()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t prev_x - Position in x direction before translation
   * const float32_t prev_y - Position in y direction before translation
   * const float32_t diff_x - How much the position should be translated in x direction
   * const float32_t diff_y - How much the position should be translated in y direction
   * float32_t* next_x - Position in x direction after translation
   * float32_t* next_y - Position in y direction after translation
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function translates a position (prev_x, prev_y) to a new position
   * (next_x, next_y) via the translation vector (diff_x, diff_y).
   *
   * PRECONDITIONS:
   * All pointer should point to valid structure
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void F360_Translate_2D_Position(
      const float32_t prev_x,
      const float32_t prev_y,
      const float32_t diff_x,
      const float32_t diff_y,
      float32_t& next_x,
      float32_t& next_y)
   {
      next_x = prev_x + diff_x;
      next_y = prev_y + diff_y;
   }

   /*===========================================================================*\
   * FUNCTION: F360_Rotate_2D_Vector()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t prev_x - Vector's x coordinate before rotation
   * const float32_t prev_y - Vector's y coordinate before rotation
   * const float32_t cos_angle - Cosine of rotation angle
   * const float32_t sin_angle - Sine of rotation angle
   * float32_t * const next_x - Vector's x coordinate after rotation
   * float32_t * const next_y - Vector's y coordinate after rotation
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function rotates a vector (prev_x, prev_y) to a new vector
   * (next_x, next_y) using the cosine and sine of a specified angle.
   *
   * PRECONDITIONS:
   * All pointer should point to valid structures.
   * The parameters cos_angle and sin_angle must fulfill basic trignometric properties,
   * i.e. cos^2(angle) + sin^2(angle) = 1.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void F360_Rotate_2D_Vector(
      const float32_t prev_x,
      const float32_t prev_y,
      const float32_t cos_angle,
      const float32_t sin_angle,
      float32_t& next_x,
      float32_t& next_y)
   {
      next_x = prev_x * cos_angle - prev_y * sin_angle;
      next_y = prev_x * sin_angle + prev_y * cos_angle;
   }

   /*===========================================================================*\
   * FUNCTION: F360_Huber_Weight()
   *===========================================================================
   * RETURN VALUE:
   * float32_t weight - Huber weight value
   *
   * PARAMETERS:
   * const float32_t residual,
   * const float32_t tuning_constant
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function returning value of Huber weight
   *
   *      /                1               for abs(residual) < tuning_constant
   * w = |
   *      \ tuning_constant/abs(residual)  for abs(residual) >= tuning_constant
   *
   * PRECONDITIONS:
   * Tuning constant has to be greater than zero, otherwise return value is zero.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t F360_Huber_Weight(const float32_t residual, const float32_t tuning_constant)
   {
      float32_t weight;

      if (F360_EPSILON < tuning_constant)
      {
         if (std::abs(residual) < tuning_constant)
         {
            weight = 1.0F;
         }
         else
         {
            // Zero division is guarded indirectly by two exisiting 'if' conditions, no need for redundant check
            weight = tuning_constant / std::abs(residual);
         }
      }
      else
      {
         weight = 0.0F;
      }

      return weight;
   }

   /*===========================================================================*\
   * FUNCTION: Compute_Dist_From_Point_To_Line_Squared()
   *===========================================================================
   * RETURN VALUE:
   * float32_t dist_to_line_sq
   *
   * PARAMETERS:
   * const float32_t x
   * const float32_t y
   * const float32_t k
   * const float32_t m
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function computes the shortest distance from point (x, y) to a line y(x) = k*x + m
   *
   * PRECONDITIONS:
   *  None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Compute_Dist_From_Point_To_Line_Squared(
      const float32_t x,
      const float32_t y,
      const float32_t k,
      const float32_t m)
   {

      // Compute closest distance from detection to a line y(x) = k*x + m
      float32_t proj_x;
      float32_t proj_y;
      Compute_Projection_Of_Point_On_Line(x, y, k, m, proj_x, proj_y);

      const float32_t diff_x = (x - proj_x);
      const float32_t diff_y = (y - proj_y);

      return (diff_x * diff_x + diff_y * diff_y);
   }

   /*===========================================================================*\
   * FUNCTION: Compute_Projection_Of_Point_On_Line()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t x
   * const float32_t y
   * const float32_t k
   * const float32_t m
   * float32_t & proj_x
   * float32_t & proj_y
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function computes the projection of a point (x,y) onto a line y(x) = k*x + m
   *
   * PRECONDITIONS:
   *  None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Compute_Projection_Of_Point_On_Line(
      const float32_t x,
      const float32_t y,
      const float32_t k,
      const float32_t m,
      float32_t& proj_x,
      float32_t& proj_y)
   {
      // Compute projection of a point [x,y] onto the line y(x) = k*x + m
      proj_x = (x - k * m + y * k) / (k * k + 1.0F);
      proj_y = k * (proj_x)+m;
   }

   /*===========================================================================*\
   * FUNCTION: Get_Vector_As_Linear_Equation()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const float32_t start_point_x
   *  const float32_t start_point_y
   *  const float32_t end_point_x
   *  const float32_t end_point_y
   *  float32_t & k
   *  float32_t & m
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function expresses a vector between two points as a linear equation
   * on the form y = kx + m
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Get_Vector_As_Linear_Equation(
      const float32_t start_point_x,
      const float32_t start_point_y,
      const float32_t end_point_x,
      const float32_t end_point_y,
      float32_t& k,
      float32_t& m)
   {
      float32_t denom = end_point_x - start_point_x;

      // Division by zero protection
      if (std::abs(denom) < F360_EPSILON)
      {
         if (denom < 0.0F)
         {
            denom = -F360_EPSILON;
         }
         else
         {
            denom = F360_EPSILON;
         }
      }

      // Express vector as linear equation
      k = (end_point_y - start_point_y) / denom;
      m = start_point_y - k * start_point_x;
   }

   /*===========================================================================*\
    * FUNCTION: Find_X_Intersect_Between_Two_Lines()
    * ===========================================================================
    * RETURN VALUE:
    * bool f_intersection_found
    *
    * PARAMETERS:
    *  const float32_t k1
    *  const float32_t m1
    *  const float32_t k2
    *  const float32_t m2
    *  float32_t &x_int
    *
    * EXTERNAL REFERENCES:
    * None.
    *
    * DEVIATIONS FROM STANDARDS:
    * None.
    *
    * --------------------------------------------------------------------------
    * ABSTRACT:
    * --------------------------------------------------------------------------
    * Find the intersections between two lines k1*x + m1 and k2*x + m2 if it exists.
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/
   bool Find_X_Intersect_Between_Two_Lines(
      const float32_t k1,
      const float32_t m1,
      const float32_t k2,
      const float32_t m2,
      float32_t& x_int)
   {

      const float32_t denom = k1 - k2;

      bool f_intersection_found;
      if (std::abs(denom) < F360_EPSILON)
      {
         // Lines are parallel
         x_int = INFTY;
         f_intersection_found = false;
      }
      else
      {
         x_int = (m2 - m1) / denom;
         f_intersection_found = true;
      }

      return f_intersection_found;
   }

   /*===========================================================================*\
   * FUNCTION: Find_X_Intersect_Between_Line_And_2_Deg_Poly()
   * ===========================================================================
   * RETURN VALUE:
   * bool f_intersection_found
   *
   * PARAMETERS:
   *  const float32_t a
   *  const float32_t b
   *  const float32_t c
   *  const float32_t k
   *  const float32_t m
   *  float32_t &x1
   *  float32_t &x2
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Find the x-intersections between the polynomial p2*x^2 + p1*x + p0 and the line
   * kx + m if they exist. x1 is the first solution and x2 is the second.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Find_X_Intersect_Between_Line_And_2_Deg_Poly(
      const float32_t p2,
      const float32_t p1,
      const float32_t p0,
      const float32_t k,
      const float32_t m,
      float32_t& x1,
      float32_t& x2)
   {
      // Solve for x using PQ-formula, Ax^2 + Bx + C = 0
      const float32_t A = p2;
      const float32_t B = p1 - k;
      const float32_t C = p0 - m;

      float32_t denom;
      if (std::abs(A) < F360_EPSILON)
      {
         if (A < 0.0F)
         {
            denom = -F360_EPSILON;
         }
         else
         {
            denom = F360_EPSILON;
         }
      }
      else
      {
         denom = A;
      }

      const float32_t one_over_A = 1.0F / denom;

      const float32_t B_over_2A = 0.5F * B * one_over_A;

      const float32_t term_to_sqrt = B_over_2A * B_over_2A - C * one_over_A;

      bool f_intersect_found;
      if (term_to_sqrt < 0.0F)
      {
         f_intersect_found = false;
      }
      else
      {
         const float32_t sqrt_term = F360_Sqrtf(term_to_sqrt);
         x1 = -B_over_2A - sqrt_term;
         x2 = -B_over_2A + sqrt_term;
         f_intersect_found = true;
      }

      return f_intersect_found;
   }

   /*===========================================================================*\
   * FUNCTION: F360_Hysteresis()
   *===========================================================================
   * RETURN VALUE:
   * bool f_new_state
   *
   * PARAMETERS:
   *  const float32_t input_value
   *  const bool f_current_state
   *  const float32_t activation_threshold
   *  const float32_t deactivation_threshold
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function implements a hysteresis check.
   * It uses an upper threshold for activation and a lower threshold for deactivation.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool F360_Hysteresis(
      const float32_t input_value,
      const bool f_current_state,
      const float32_t activation_threshold,
      const float32_t deactivation_threshold)
   {
      bool f_new_state = f_current_state;

      if (f_current_state)
      {
         if (input_value < deactivation_threshold)
         {
            f_new_state = false;
         }
      }
      else
      {
         if (input_value > activation_threshold)
         {
            f_new_state = true;
         }
      }
      return f_new_state;
   }
   
   /*===========================================================================*\
   * FUNCTION: f360_mergesort()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const uint32_t count, Number of elements to sort
   * const bool f_ascending, True if sorting should be done in ascending order
   * F360_Sort_Data_T(&array_to_sort)[SORT_ARRAY_SIZE], Array to sort
   * F360_Sort_Data_T(&array_buffer)[SORT_ARRAY_SIZE], Buffer to use as working data
   * bool& f_data_in_buffer, True if result is in the buffer
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Implements the list management for mergesort algorithm.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void f360_mergesort(
      const uint32_t count,
      const bool f_ascending,
      F360_Sort_Data_T(&array_to_sort)[SORT_ARRAY_SIZE],
      F360_Sort_Data_T(&array_buffer)[SORT_ARRAY_SIZE],
      bool& f_data_in_buffer)
   {
      if(count <= SORT_ARRAY_SIZE)
      {
         // First iteration. Sublist length 1 -> 2
         for (uint32_t i = 1U; i < count; i += 2U)
         {
            F360_Sort_Data_T* const prev = &array_to_sort[i - 1U];
            F360_Sort_Data_T* const curr = &array_to_sort[i];
            const bool swap = f_ascending ? (curr->data < prev->data) : (curr->data > prev->data);
            if (swap)
            {
               const F360_Sort_Data_T temp = *curr;
               *curr = *prev;
               *prev = temp;
            }
         }

         // Subsequent iterations
         uint32_t sublist_length = 2U;
         while (sublist_length < count)
         {
            // Call merge_sublists with either array_to_sort or buffer as input list
            if (f_data_in_buffer)
            {
               // Loop over all sublists
               for (uint32_t i = 0U; i < count; i += 2U * sublist_length)
               {
                  // Make sure we don't go outside the size of the array to sort and call merge function
                  const uint32_t merged_list_end = (count < (i + 2U * sublist_length)) ? count : (i + 2U * sublist_length);
                  merge_sublists(f_ascending, i, i + sublist_length, merged_list_end, array_buffer, array_to_sort);
               }
            }
            else
            {
               for (uint32_t i = 0U; i < count; i += 2U * sublist_length)
               {
                  const uint32_t merged_list_end = (count < (i + 2U * sublist_length)) ? count : (i + 2U * sublist_length);
                  merge_sublists(f_ascending, i, i + sublist_length, merged_list_end, array_to_sort, array_buffer);
               }
            }

            // Keep track if the result is in array_to_sort or in buffer
            f_data_in_buffer = !f_data_in_buffer;
            sublist_length = 2U * sublist_length;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: merge_sublists()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const bool f_ascending, True if sorting should be done in ascending order
   * const uint32_t sublist1_start, Start index of sublist 1
   * const uint32_t sublist2_start, Start index of sublist 2
   * const uint32_t merged_list_end, Ending index of merged lists
   * const F360_Sort_Data_T(&in_list)[SORT_ARRAY_SIZE], Array of input sublists
   * F360_Sort_Data_T(&out_list)[SORT_ARRAY_SIZE]), Array to populate with merged sublists
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Implements the merging of sorted sublists for mergesort algorithm.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void merge_sublists(
      const bool f_ascending,
      const uint32_t sublist1_start,
      const uint32_t sublist2_start,
      const uint32_t merged_list_end,
      const F360_Sort_Data_T(&in_list)[SORT_ARRAY_SIZE],
      F360_Sort_Data_T(&out_list)[SORT_ARRAY_SIZE])
   {
      uint32_t sublist1_idx = sublist1_start;
      uint32_t sublist2_idx = sublist2_start;
      uint32_t outlist_idx = sublist1_start;

      // In the last iteration we could end up here with only one sublist
      if ((sublist2_start < merged_list_end) && (sublist1_start < sublist2_start))
      {
         const F360_Sort_Data_T* sublist1_element = &in_list[sublist1_idx];
         const F360_Sort_Data_T* sublist2_element = &in_list[sublist2_idx];
         F360_Sort_Data_T* outlist_element = &out_list[outlist_idx];

         while (true)
         {
            // Place one of the sublist elements in the outlist and update indices
            // Comparing 2 with 1, instead of 1 with 2, results is stability
            bool f_break_condition = false;
            const bool use_sublist2 = f_ascending ? (sublist2_element->data < sublist1_element->data) : (sublist2_element->data > sublist1_element->data);

            if (use_sublist2)
            {
               *outlist_element = *sublist2_element;
               sublist2_idx++;
               sublist2_element = &in_list[sublist2_idx];
               if (sublist2_idx >= merged_list_end) { f_break_condition = true; }
            }
            else
            {
               *outlist_element = *sublist1_element;
               sublist1_idx++;
               sublist1_element = &in_list[sublist1_idx];
               if (sublist1_idx >= sublist2_start) { f_break_condition = true; }
            }
            outlist_idx++;
            outlist_element = &out_list[outlist_idx];

            if (f_break_condition)
            {
               break;
            }
         }
      }

      // Move remaining elements from sublist1, respecting the bounds of the lists
      while ((sublist1_idx < sublist2_start) && (sublist1_idx < merged_list_end))
      {
         out_list[outlist_idx] = in_list[sublist1_idx];
         sublist1_idx++;
         outlist_idx++;
      }

      // Move remaining elements from sublist2
      while (sublist2_idx < merged_list_end)
      {
         out_list[outlist_idx] = in_list[sublist2_idx];
         sublist2_idx++;
         outlist_idx++;
      }
   }

   /*===========================================================================*\
   * FUNCTION: get_sort_working_data()
   * ===========================================================================
   * RETURN VALUE:
   * F360_Sort_Working_Data_T* &sort_working_data
   *
   * PARAMETERS:
   * None.
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Helper function for accessing working data in mergesort algorithm.
   * Prevents having to allocate large amounts of data on stack.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   F360_Sort_Working_Data_T* get_sort_working_data()
   {
      static F360_Sort_Working_Data_T sort_working_data;
      return &sort_working_data;
   }
}

