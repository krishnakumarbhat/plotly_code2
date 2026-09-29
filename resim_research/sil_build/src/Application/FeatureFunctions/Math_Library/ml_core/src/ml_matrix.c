/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <assert.h>
#include <reuse.h>
#include "ml_bool.h"
#include "ml_macros.h"
#include "ml_math.h"
#include "ml_matrix.h"


void Matrix_2x2_Create_Identity_Matrix(
   Matrix_2X2_T       *res_mat)
{
   assert(NULL != res_mat);

   res_mat->elements[0][0] = 1.0f;
   res_mat->elements[0][1] = 0.0f;

   res_mat->elements[1][0] = 0.0f;
   res_mat->elements[1][1] = 1.0f;
}

void Matrix_2x2_Create_Zero_Matrix(
   Matrix_2X2_T       *res_mat)
{
   assert(NULL != res_mat);

   res_mat->elements[0][0] = 0.0f;
   res_mat->elements[0][1] = 0.0f;

   res_mat->elements[1][0] = 0.0f;
   res_mat->elements[1][1] = 0.0f;
}

void Matrix_2x2_Mul_Matrix_2x2(
   Matrix_2X2_T       *res_mat,
   const Matrix_2X2_T *mat_a,
   const Matrix_2X2_T *mat_b)
{
   uint8_t i;
   uint8_t j;

   assert(NULL != res_mat);
   assert(NULL != mat_a);
   assert(NULL != mat_b);

   for (i = 0; i < 2; i++)
   {
      for (j = 0; j < 2; j++)
      {
         uint8_t k;
         float32_T element = 0.0f;
         for (k = 0; k < 2; k++)
         {
            element += mat_a->elements[i][k] * mat_b->elements[k][j];
         }
         res_mat->elements[i][j] = element;
      }
   }
}

void Matrix_2x2_Add_Matrix_2x2(
   Matrix_2X2_T       *res_mat,
   const Matrix_2X2_T *mat_a,
   const Matrix_2X2_T *mat_b)
{
   uint8_t i;
   uint8_t j;

   assert(NULL != res_mat);
   assert(NULL != mat_a);
   assert(NULL != mat_b);

   for (i = 0; i < 2; i++)
   {
      for (j = 0; j < 2; j++)
      {
         res_mat->elements[i][j] = mat_a->elements[i][j] + mat_b->elements[i][j];
      }
   }
}

void Matrix_2x2_Mul_Scalar(
   Matrix_2X2_T       *res_mat,
   const Matrix_2X2_T *mat,
   const float32_T scalar )
{
   uint8_t i;
   uint8_t j;

   assert(NULL != res_mat);
   assert(NULL != mat);

   for (i = 0; i < 2; i++)
   {
      for (j = 0; j < 2; j++)
      {
         res_mat->elements[i][j] = mat->elements[i][j] * scalar;
      }
   }
}

void Matrix_3x3_Create_Identity_Matrix(
   Matrix_3X3_T       *res_mat)
{
   assert(NULL != res_mat);

   res_mat->elements[0][0] = 1.0f;
   res_mat->elements[0][1] = 0.0f;
   res_mat->elements[0][2] = 0.0f;

   res_mat->elements[1][0] = 0.0f;
   res_mat->elements[1][1] = 1.0f;
   res_mat->elements[1][2] = 0.0f;

   res_mat->elements[2][0] = 0.0f;
   res_mat->elements[2][1] = 0.0f;
   res_mat->elements[2][2] = 1.0f;
}

void Matrix_3x3_Create_Zero_Matrix(
   Matrix_3X3_T       *res_mat)
{
   assert(NULL != res_mat);

   res_mat->elements[0][0] = 0.0f;
   res_mat->elements[0][1] = 0.0f;
   res_mat->elements[0][2] = 0.0f;

   res_mat->elements[1][0] = 0.0f;
   res_mat->elements[1][1] = 0.0f;
   res_mat->elements[1][2] = 0.0f;

   res_mat->elements[2][0] = 0.0f;
   res_mat->elements[2][1] = 0.0f;
   res_mat->elements[2][2] = 0.0f;
}

void Matrix_3x3_Mul_Matrix_3x3(
   Matrix_3X3_T       *res_mat,
   const Matrix_3X3_T *mat_a,
   const Matrix_3X3_T *mat_b)
{
   uint8_t i;
   uint8_t j;

   assert(NULL != res_mat);
   assert(NULL != mat_a);
   assert(NULL != mat_b);

   for (i = 0; i < 3; i++)
   {
      for (j = 0; j < 3; j++)
      {
         uint8_t k;
         float32_T element = 0.0f;
         for (k = 0; k < 3; k++)
         {
            element += mat_a->elements[i][k] * mat_b->elements[k][j];
         }
         res_mat->elements[i][j] = element;
      }
   }
}

void Matrix_3x3_Add_Matrix_3x3(
   Matrix_3X3_T       *res_mat,
   const Matrix_3X3_T *mat_a,
   const Matrix_3X3_T *mat_b)
{
   uint8_t i;
   uint8_t j;

   assert(NULL != res_mat);
   assert(NULL != mat_a);
   assert(NULL != mat_b);

   for (i = 0; i < 3; i++)
   {
      for (j = 0; j < 3; j++)
      {
         res_mat->elements[i][j] = mat_a->elements[i][j] + mat_b->elements[i][j];
      }
   }
}

void Matrix_3x3_Mul_Scalar(
   Matrix_3X3_T       *res_mat,
   const Matrix_3X3_T *mat,
   const float32_T scalar)
{
   uint8_t i;
   uint8_t j;

   assert(NULL != res_mat);
   assert(NULL != mat);

   for (i = 0; i < 3; i++)
   {
      for (j = 0; j < 3; j++)
      {
         res_mat->elements[i][j] = mat->elements[i][j] * scalar;
      }
   }
}


void Matrix_2x2_Mul_Vector_2d(
   Vector_2d_T        *vector_res,
   const Matrix_2X2_T *matrix,
   const Vector_2d_T  *vector)
{
   assert(NULL != vector_res);
   assert(NULL != matrix);
   assert(NULL != vector);

   vector_res->x = (matrix->elements[0][0] * vector->x) + (matrix->elements[0][1] * vector->y);
   vector_res->y = (matrix->elements[1][0] * vector->x) + (matrix->elements[1][1] * vector->y);
}


float32_T Matrix_2x2_Determinant(const Matrix_2X2_T *matrix)
{
   float32_T det;

   assert(NULL != matrix);

   /* Calculate determinant using diagonal method. */
   det = (matrix->elements[0][0] * matrix->elements[1][1]) -
         (matrix->elements[1][0] * matrix->elements[0][1]);

   return det;
}


float32_T Matrix_3x3_Determinant(const Matrix_3X3_T *matrix )
{
   float32_T det = 0;

   assert(NULL != matrix);

   /* Calculate determinant using diagonal method. */
   det = ((matrix->elements[0][0] * matrix->elements[1][1] * matrix->elements[2][2]) +
          (matrix->elements[0][1] * matrix->elements[1][2] * matrix->elements[2][0]) +
          (matrix->elements[0][2] * matrix->elements[1][0] * matrix->elements[2][1])) -
         ((matrix->elements[0][2] * matrix->elements[1][1] * matrix->elements[2][0]) +
          (matrix->elements[0][0] * matrix->elements[1][2] * matrix->elements[2][1]) +
          (matrix->elements[0][1] * matrix->elements[1][0] * matrix->elements[2][2]));

   return det;
}


boolean_T Matrix_2x2_Inverse_Given_Determinant(
   Matrix_2X2_T    *matrix,
   const float32_T  det)
{
   boolean_T f_inversion_possible = FALSE;

   assert(NULL != matrix);

   /* Find the inverse if determinant is non-zero, otherwise set the elements of inverse matrix to INFINITY
    * so that program can continue in RELEASE or embedded build */
   if (THRESHOLD_IS_ZERO < Abs(det))
   {
      Matrix_2X2_T mat_inv;
      float32_T det_inv;
      det_inv     = 1 / det;
      mat_inv.elements[0][0] = matrix->elements[1][1] * det_inv;
      mat_inv.elements[0][1] = -matrix->elements[0][1] * det_inv;
      mat_inv.elements[1][0] = -matrix->elements[1][0] * det_inv;
      mat_inv.elements[1][1] = matrix->elements[0][0] * det_inv;
      *matrix = mat_inv;
      f_inversion_possible = TRUE;
   }

   /* Set assert if absolute value of determinant is ~0
   * as matrix with zero determinant will not have an inverse*/
   /** \throws Assertion if given det is smaller than THRESHOLD_IS_ZERO */
   assert(THRESHOLD_IS_ZERO < Abs(det));
   return f_inversion_possible;
}


boolean_T Matrix_2x2_Inverse(Matrix_2X2_T *matrix)
{
   boolean_T f_inversion_possible = FALSE;
   float32_T det;

   assert(NULL != matrix);

   /* Find the determinant of the square matrix */
   det = Matrix_2x2_Determinant(matrix);

   /* Find the inverse if determinant is non-zero and set appropriate information */
   if (THRESHOLD_IS_ZERO < Abs(det))
   {
      f_inversion_possible = Matrix_2x2_Inverse_Given_Determinant(matrix, det);
   }

   return f_inversion_possible;
}


void Matrix_2x2_Transpose(Matrix_2X2_T *matrix)
{
   assert(NULL != matrix);

   Swap(matrix->elements[0][1], matrix->elements[1][0], float32_T);
}


void Matrix_3x3_Transpose(Matrix_3X3_T *matrix /**< [in/out] matrix to be transposed*/)
{
   assert(NULL != matrix);

   Swap(matrix->elements[0][1], matrix->elements[1][0], float32_T);
   Swap(matrix->elements[0][2], matrix->elements[2][0], float32_T);
   Swap(matrix->elements[1][2], matrix->elements[2][1], float32_T);
}

