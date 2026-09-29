/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <gtest/gtest.h>
#include "ml_bool.h"
#include "ml_math.h"
#include "ml_matrix_2x2_t.h"
#include "ml_matrix_3x3_t.h"
#include "ml_vector_2d_t.h"
#include "ml_matrix.h"


/**
 * Ensure that the result is an identity matrix.
* \sdd{WI-13902}
*/
TEST(Matrix_2x2_Create_Identity_Matrix, WI_16113_Matrix_2x2_Create_Identity_Matrix_fills_correctly)
{
   Matrix_2X2_T mat;
   Matrix_2x2_Create_Identity_Matrix(&mat);

   EXPECT_EQ(mat.elements[0][0], 1.0f);
   EXPECT_EQ(mat.elements[0][1], 0.0f);
   EXPECT_EQ(mat.elements[1][0], 0.0f);
   EXPECT_EQ(mat.elements[1][1], 1.0f);
}

/** ensure its possible to create a zero matrix
* \sdd{WI-13901}
*/
TEST(Matrix_2x2_Create_Zero_Matrix, WI_16114_Matrix_2x2_Create_Zero_Matrix_fills_correctly)
{
   Matrix_2X2_T mat;
   /** \action call FUT */
   Matrix_2x2_Create_Zero_Matrix(&mat);

   /** \assert result matches expectation */
   EXPECT_EQ(mat.elements[0][0], 0.0f);
   EXPECT_EQ(mat.elements[0][1], 0.0f);
   EXPECT_EQ(mat.elements[1][0], 0.0f);
   EXPECT_EQ(mat.elements[1][1], 0.0f);
}

/** Ensure multiplication of two matrices works
* \sdd{WI-13900}
*/
TEST(matrix_Matrix_2x2_Mul_Matrix_2x2, WI_16115_Matrix_2x2_multiplication)
{
   /** \arrange fill two matrices with arbitrary values */
   Matrix_2X2_T mat_a{{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
      }};
   Matrix_2X2_T mat_b{{
      {5.0f, 6.0f},
      {7.0f, 8.0f}
      }};
   Matrix_2X2_T mat;

   /** \action call FUT */
   Matrix_2x2_Mul_Matrix_2x2(&mat, &mat_a, &mat_b);
   /** \assert result matches expectation */
   EXPECT_EQ(mat.elements[0][0], 19);
   EXPECT_EQ(mat.elements[0][1], 22);
   EXPECT_EQ(mat.elements[1][0], 43);
   EXPECT_EQ(mat.elements[1][1], 50);
}

/** Multiplication with a zero matrix
* \sdd{WI-13900}
*/
TEST(matrix_Matrix_2x2_Mul_Matrix_2x2, WI_16116_Matrix_2x2_multiplication_with_zero)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a{{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
      }};
   /** \arrange create zero matrix */
   Matrix_2X2_T mat_b{{
      {.0f, .0f},
      {.0f, .0f}
      }};
   Matrix_2X2_T mat;

   /** \action call FUT */
   Matrix_2x2_Mul_Matrix_2x2(&mat, &mat_a, &mat_b);
   /** \assert result matches expectation */
   EXPECT_EQ(mat.elements[0][0], 0.0f);
   EXPECT_EQ(mat.elements[0][1], 0.0f);
   EXPECT_EQ(mat.elements[1][0], 0.0f);
   EXPECT_EQ(mat.elements[1][1], 0.0f);
}

/** Try out multiplication with identity matrix
* \sdd{WI-13900}
*/
TEST(matrix_Matrix_2x2_Mul_Matrix_2x2, WI_16117_Matrix_2x2_multiplication_with_identity_matrix)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a{{
      { 1.0f, 2.0f},
      {3.0f, 4.0f}
      }};
   /** \arrange create identity matrix */
   Matrix_2X2_T mat_b{{
      {1.0f, .0f},
      {.0f, 1.0f}
      }};
   Matrix_2X2_T mat;

   /** \action call FUT */
   Matrix_2x2_Mul_Matrix_2x2(&mat, &mat_a, &mat_b);
   /** \assert result matches expectation */
   EXPECT_EQ(mat.elements[0][0], mat_a.elements[0][0]);
   EXPECT_EQ(mat.elements[0][1], mat_a.elements[0][1]);
   EXPECT_EQ(mat.elements[1][0], mat_a.elements[1][0]);
   EXPECT_EQ(mat.elements[1][1], mat_a.elements[1][1]);
}

/** Try creating an identity matrix
* \sdd{WI-13909}
*/
TEST(Matrix_3x3_Create_Identity_Matrix, WI_16118_Matrix_3x3_Create_Identity_Matrix_fills_correctly)
{
   Matrix_3X3_T mat;
   /** \action call FUT */
   Matrix_3x3_Create_Identity_Matrix(&mat);
   /** \assert result matches expectation */
   EXPECT_EQ(mat.elements[0][0], 1.0f);
   EXPECT_EQ(mat.elements[0][1], 0.0f);
   EXPECT_EQ(mat.elements[0][2], 0.0f);

   EXPECT_EQ(mat.elements[1][0], 0.0f);
   EXPECT_EQ(mat.elements[1][1], 1.0f);
   EXPECT_EQ(mat.elements[1][2], 0.0f);

   EXPECT_EQ(mat.elements[2][0], 0.0f);
   EXPECT_EQ(mat.elements[2][1], 0.0f);
   EXPECT_EQ(mat.elements[2][2], 1.0f);
}


/** Try creating a zero matrix
* \sdd{WI-13897}
*/
TEST(Matrix_3x3_Create_Zero_Matrix, WI_16119_Matrix_3x3_Create_Zero_Matrix_fills_correctly)
{
   Matrix_3X3_T mat;
   /** \action call FUT */
   Matrix_3x3_Create_Zero_Matrix(&mat);
   /** \assert result matches expectation */
   EXPECT_EQ(mat.elements[0][0], 0.0f);
   EXPECT_EQ(mat.elements[0][1], 0.0f);
   EXPECT_EQ(mat.elements[0][2], 0.0f);

   EXPECT_EQ(mat.elements[1][0], 0.0f);
   EXPECT_EQ(mat.elements[1][1], 0.0f);
   EXPECT_EQ(mat.elements[1][2], 0.0f);

   EXPECT_EQ(mat.elements[2][0], 0.0f);
   EXPECT_EQ(mat.elements[2][1], 0.0f);
   EXPECT_EQ(mat.elements[2][2], 0.0f);
}



/** Try out matrix multiplication
* \sdd{WI-13899}
*/
TEST(matrix_matMul_3x3_3x3, WI_16120_Matrix_3x3_multiplication)
{
   /** \arrange fill two matrices with arbitrary values */
   Matrix_3X3_T mat_a{{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
      } };
   Matrix_3X3_T mat_b{{
      {10.0f, 20.0f, 30.0f},
      {40.0f, 50.0f, 60.0f},
      {70.0f, 80.0f, 90.0f}
      }};
   Matrix_3X3_T mat;

   /** \action call FUT */
   Matrix_3x3_Mul_Matrix_3x3(&mat, &mat_a, &mat_b);
   /** \assert result matches expectation */
   EXPECT_EQ(mat.elements[0][0], 300);
   EXPECT_EQ(mat.elements[0][1], 360);
   EXPECT_EQ(mat.elements[0][2], 420);
   EXPECT_EQ(mat.elements[1][0], 660);
   EXPECT_EQ(mat.elements[1][1], 810);
   EXPECT_EQ(mat.elements[1][2], 960);
   EXPECT_EQ(mat.elements[2][0], 1020);
   EXPECT_EQ(mat.elements[2][1], 1260);
   EXPECT_EQ(mat.elements[2][2], 1500);
}

/** Multiply with zero matrix
* \sdd{WI-13899}
*/
TEST(matrix_matMul_3x3_3x3, WI_16121_Matrix_3x3_multiplication_with_zero)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a{{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
      }};
   /** \arrange create zero matrix */
   Matrix_3X3_T mat_b{{
      {.0f, .0f},
      {.0f, .0f}
   }};
   Matrix_3X3_T mat;

   /** \action call FUT */
   Matrix_3x3_Mul_Matrix_3x3(&mat, &mat_a, &mat_b);
   /** \assert result matches expectation */
   EXPECT_EQ(mat.elements[0][0], .0f);
   EXPECT_EQ(mat.elements[0][1], .0f);
   EXPECT_EQ(mat.elements[0][2], .0f);
   EXPECT_EQ(mat.elements[1][0], .0f);
   EXPECT_EQ(mat.elements[1][1], .0f);
   EXPECT_EQ(mat.elements[1][2], .0f);
   EXPECT_EQ(mat.elements[2][0], .0f);
   EXPECT_EQ(mat.elements[2][1], .0f);
   EXPECT_EQ(mat.elements[2][2], .0f);
}

/** Multiply with identity matrix
* \sdd{WI-13899}
*/
TEST(matrix_matMul_3x3_3x3, WI_16122_Matrix_3x3_multiplication_with_identity_matrix)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a{{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
      }};
   /** \arrange create identity matrix */
   Matrix_3X3_T mat_b = {{
      {1.0f, 0.0f, 0.0f},
      {0.0f, 1.0f, 0.0f},
      {0.0f, 0.0f, 1.0f}
      }};
   Matrix_3X3_T mat;

   /** \action call FUT */
   Matrix_3x3_Mul_Matrix_3x3(&mat, &mat_a, &mat_b);
   /** \assert result matches expectation */
   EXPECT_EQ(mat.elements[0][0], mat_a.elements[0][0]);
   EXPECT_EQ(mat.elements[0][1], mat_a.elements[0][1]);
   EXPECT_EQ(mat.elements[0][2], mat_a.elements[0][2]);
   EXPECT_EQ(mat.elements[1][0], mat_a.elements[1][0]);
   EXPECT_EQ(mat.elements[1][1], mat_a.elements[1][1]);
   EXPECT_EQ(mat.elements[1][2], mat_a.elements[1][2]);
   EXPECT_EQ(mat.elements[2][0], mat_a.elements[2][0]);
   EXPECT_EQ(mat.elements[2][1], mat_a.elements[2][1]);
   EXPECT_EQ(mat.elements[2][2], mat_a.elements[2][2]);
}


/** Try out matrix multiplication
* \sdd{WI-13904}
*/
TEST(matrix_Matrix_2x2_Mul_Vector_2d, WI_16123_Matrix_2x2_multiplication_with_vector)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a{{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
      }};
   /** \arrange fill vector with arbitrary values */
   Vector_2d_T  vector{ 100.0f, 200.0f };
   Vector_2d_T  res;

   /** \action call FUT */
   Matrix_2x2_Mul_Vector_2d(&res, &mat_a, &vector);
   /** \assert result matches expectation */
   EXPECT_EQ(res.x, 500);
   EXPECT_EQ(res.y, 1100);
}

/** Multiplication of matrix and zero vector
* \sdd{WI-13904}
*/
TEST(matrix_Matrix_2x2_Mul_Vector_2d, WI_16124_Matrix_2x2_multiplication_with_zero_vector)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a{{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
      }};
   /** \arrange fill vector with zeros */
   Vector_2d_T  vector{ .0f };
   Vector_2d_T  res;

   /** \action call FUT */
   Matrix_2x2_Mul_Vector_2d(&res, &mat_a, &vector);
   /** \assert result matches expectation */
   EXPECT_EQ(res.x, .0f);
   EXPECT_EQ(res.y, .0f);
}

/** read a row vector
* \sdd{WI-13904}
*/
TEST(matrix_Matrix_2x2_Mul_Vector_2d, WI_16125_Matrix_2x2_get_row_vector_0)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a{{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
      }};
   /** \arrange set a vector to read the first row */
   Vector_2d_T  vector = { 1.0f, .0f };
   Vector_2d_T  res;

   /** \action call FUT */
   Matrix_2x2_Mul_Vector_2d(&res, &mat_a, &vector);
   /** \assert result matches expectation */
   EXPECT_EQ(res.x, mat_a.elements[0][0]);
   EXPECT_EQ(res.y, mat_a.elements[1][0]);
}

/**
* \sdd{WI-13904}
*/
TEST(matrix_Matrix_2x2_Mul_Vector_2d, WI_16126_Matrix_2x2_get_row_vector_1)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a{{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
      }};
   /** \arrange set a vector to read the second row */
   Vector_2d_T  vector = { .0f, 1.0f };
   Vector_2d_T  res;

   /** \action call FUT */
   Matrix_2x2_Mul_Vector_2d(&res, &mat_a, &vector);
   /** \assert result matches expectation */
   EXPECT_EQ(res.x, mat_a.elements[0][1]);
   EXPECT_EQ(res.y, mat_a.elements[1][1]);
}



/** calculate determinant of 2x2 Matrix
* \sdd{WI-13898}
*/
TEST(matrix_Matrix_2x2_Determinant, WI_16127_Matrix_2x2_determinant)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a{{
      { 1.0f, 2.0f},
      {3.0f, 4.0f}
      }};
   /** \action call FUT */
   float res = Matrix_2x2_Determinant(&mat_a);

   /** \assert result matches expectation */
   EXPECT_EQ(res, -2.0f);
}

/** calculate determinant of 3x3 Matrix
* \sdd{WI-13905}
*/
TEST(matrix_Matrix_3x3_Determinant, WI_16128_Matrix_3x3_determinant)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a{{
      {1.0f, 20.0f, 3.0f},
      {40.0f, 5.0f, 60.0f},
      {7.0f, 80.0f, 9.0f}
      }};
   /** \action call FUT */
   float res = Matrix_3x3_Determinant(&mat_a);

   /** \assert result matches expectation */
   EXPECT_EQ(res, 5940);
}

/** Error case determinant too small
* \sdd{WI-13910}
* \sdd{WI-13903}
*/
TEST(matrix_Matrix_2x2_Inverse_Given_Determinant, WI_16129_Matrix_2x2_determinant_too_small)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a{{
      {0.0f, 0.0f},
      {3.0f, 4.0f}
      }};

   EXPECT_DEBUG_DEATH({
      /** \action call FUT with a zero determinant */
      Matrix_2x2_Inverse_Given_Determinant(&mat_a, 0);

      /** \assert the matching assertion is thrown */
   }, "THRESHOLD_IS_ZERO < Abs.det");
#ifdef NDEBUG
   bool f_ret = Is_True(Matrix_2x2_Inverse_Given_Determinant(&mat_a, 0));
   EXPECT_FALSE(f_ret);
#endif
}

/** calculate inverse using determinant
* \sdd{WI-13910}
* \sdd{WI-13903}
*/
TEST(matrix_Matrix_2x2_Inverse_Given_Determinant, WI_16130_Matrix_2x2_inverse)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a{{
      { 1.0f, 2.0f},
      {3.0f, 4.0f}
      }};
   /** \action call FUT */
   boolean_T ret = Matrix_2x2_Inverse_Given_Determinant(&mat_a, -2.0f);

   /** \assert result matches expectation */
   EXPECT_TRUE(ret);
   EXPECT_EQ(mat_a.elements[0][0], -2.0f);
   EXPECT_EQ(mat_a.elements[0][1], 1.0f);
   EXPECT_EQ(mat_a.elements[1][0], 1.5f);
   EXPECT_EQ(mat_a.elements[1][1], -0.5f);
}


/** error case inversion fails because determinant is too small
* \sdd{WI-13912}
* \sdd{WI-13903}
*/
TEST(matrix_Matrix_2x2_Inverse, WI_16131_Matrix_2x2_det_too_small)
{
   /** \arrange fill matrix in a way that leads to a small determinant */
   Matrix_2X2_T mat_a{{
      {0.0f, 0.0f},
      {3.0f, 4.0f}
      }};
   /** \action call FUT */
   boolean_T ret = Matrix_2x2_Inverse(&mat_a);

   /** \assert fail */
   EXPECT_FALSE(ret);
}

/** Try out inverting a matrix
* \sdd{WI-13912}
* \sdd{WI-13903}
*/
TEST(matrix_Matrix_2x2_Inverse, WI_16132_Matrix_2x2_det_ok)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a{{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
      }};
   /** \action call FUT */
   boolean_T ret = Matrix_2x2_Inverse(&mat_a);

   /** \assert success */
   EXPECT_TRUE(ret);

   /** \assert result matches expectation */
   EXPECT_EQ(mat_a.elements[0][0], -2.0f);
   EXPECT_EQ(mat_a.elements[0][1], 1.0f);
   EXPECT_EQ(mat_a.elements[1][0], 1.5f);
   EXPECT_EQ(mat_a.elements[1][1], -0.5f);
}


/** try transposing a 2x2 matrix
* \sdd{WI-13908}
*/
TEST(matrix_Matrix_2x2_Transpose, WI_16133_Matrix_2x2_transposed)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
      }};

   /** \action call FUT */
   Matrix_2x2_Transpose(&mat_a);
   /** \assert result matches expectation */
   EXPECT_EQ(mat_a.elements[0][0], 1.0f);
   EXPECT_EQ(mat_a.elements[0][1], 3.0f);
   EXPECT_EQ(mat_a.elements[1][0], 2.0f);
   EXPECT_EQ(mat_a.elements[1][1], 4.0f);
}


/** try transposing a 3x3 matrix
* \sdd{WI-13913}
*/
TEST(matrix_Matrix_3x3_Transpose, WI_16134_Matrix_3x3_transposed)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
      }};

   /** \action call FUT */
   Matrix_3x3_Transpose(&mat_a);
   /** \assert result matches expectation */
   EXPECT_EQ(mat_a.elements[0][0], 1);
   EXPECT_EQ(mat_a.elements[0][1], 4);
   EXPECT_EQ(mat_a.elements[0][2], 7);
   EXPECT_EQ(mat_a.elements[1][0], 2);
   EXPECT_EQ(mat_a.elements[1][1], 5);
   EXPECT_EQ(mat_a.elements[1][2], 8);
   EXPECT_EQ(mat_a.elements[2][0], 3);
   EXPECT_EQ(mat_a.elements[2][1], 6);
   EXPECT_EQ(mat_a.elements[2][2], 9);
}


/** Add a 3x3 matrix to itself
* \sdd{WI-13893}
*/
TEST(Matrix_3x3_Add_Matrix_3x3, WI_16135_Matrix_3x3_addition_with_self)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = { {
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
      }};
   Matrix_3X3_T mat_res;

   /** \action call FUT */
   Matrix_3x3_Add_Matrix_3x3(&mat_res, &mat_a, &mat_a);
   /** \assert result matches expectation */
   EXPECT_EQ(mat_res.elements[0][0], 2 * mat_a.elements[0][0]);
   EXPECT_EQ(mat_res.elements[0][1], 2 * mat_a.elements[0][1]);
   EXPECT_EQ(mat_res.elements[0][2], 2 * mat_a.elements[0][2]);
   EXPECT_EQ(mat_res.elements[1][0], 2 * mat_a.elements[1][0]);
   EXPECT_EQ(mat_res.elements[1][1], 2 * mat_a.elements[1][1]);
   EXPECT_EQ(mat_res.elements[1][2], 2 * mat_a.elements[1][2]);
   EXPECT_EQ(mat_res.elements[2][0], 2 * mat_a.elements[2][0]);
   EXPECT_EQ(mat_res.elements[2][1], 2 * mat_a.elements[2][1]);
   EXPECT_EQ(mat_res.elements[2][2], 2 * mat_a.elements[2][2]);
}

/** Add two 3x3 matrices
* \sdd{WI-13893}
*/
TEST(Matrix_3x3_Add_Matrix_3x3, WI_16136_Matrix_3x3_addition)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a{{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
      }};
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_b = {{
      {10.0f, 20.0f, 30.0f},
      {40.0f, 50.0f, 60.0f},
      {70.0f, 80.0f, 90.0f}
      }};
   Matrix_3X3_T mat_res;

   /** \action call FUT */
   Matrix_3x3_Add_Matrix_3x3(&mat_res, &mat_a, &mat_b);
   /** \assert result matches expectation */
   EXPECT_EQ(mat_res.elements[0][0], 11.0f);
   EXPECT_EQ(mat_res.elements[0][1], 22.0f);
   EXPECT_EQ(mat_res.elements[0][2], 33.0f);
   EXPECT_EQ(mat_res.elements[1][0], 44.0f);
   EXPECT_EQ(mat_res.elements[1][1], 55.0f);
   EXPECT_EQ(mat_res.elements[1][2], 66.0f);
   EXPECT_EQ(mat_res.elements[2][0], 77.0f);
   EXPECT_EQ(mat_res.elements[2][1], 88.0f);
   EXPECT_EQ(mat_res.elements[2][2], 99.0f);
}

/** Add two 3x3 matrices
* \sdd{WI-13893}
*/
TEST(Matrix_3x3_Add_Matrix_3x3, WI_16137_Matrix_3x3_addition_commutative)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
      }};
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_b = {{
      {10.0f, 20.0f, 30.0f},
      {40.0f, 50.0f, 60.0f},
      {70.0f, 80.0f, 90.0f}
      }};
   Matrix_3X3_T mat_res;

   /** \action call FUT */
   Matrix_3x3_Add_Matrix_3x3(&mat_res, &mat_b, &mat_a);
   /** \assert result matches expectation */
   EXPECT_EQ(mat_res.elements[0][0], 11.0f);
   EXPECT_EQ(mat_res.elements[0][1], 22.0f);
   EXPECT_EQ(mat_res.elements[0][2], 33.0f);
   EXPECT_EQ(mat_res.elements[1][0], 44.0f);
   EXPECT_EQ(mat_res.elements[1][1], 55.0f);
   EXPECT_EQ(mat_res.elements[1][2], 66.0f);
   EXPECT_EQ(mat_res.elements[2][0], 77.0f);
   EXPECT_EQ(mat_res.elements[2][1], 88.0f);
   EXPECT_EQ(mat_res.elements[2][2], 99.0f);
}

/** Add a 2x2 matrix to itself
* \sdd{WI-13896}
*/
TEST(Matrix_2x2_Add_Matrix_2x2, WI_16138_Matrix_2x2_addition_with_self)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
      }};
   Matrix_2X2_T mat_res;

   /** \action call FUT */
   Matrix_2x2_Add_Matrix_2x2(&mat_res, &mat_a, &mat_a);
   /** \assert result matches expectation */
   EXPECT_EQ(mat_res.elements[0][0], 2 * mat_a.elements[0][0]);
   EXPECT_EQ(mat_res.elements[0][1], 2 * mat_a.elements[0][1]);
   EXPECT_EQ(mat_res.elements[1][0], 2 * mat_a.elements[1][0]);
   EXPECT_EQ(mat_res.elements[1][1], 2 * mat_a.elements[1][1]);
}

/** add two 2x2 matrices
* \sdd{WI-13896}
*/
TEST(Matrix_2x2_Add_Matrix_2x2, WI_16139_Matrix_2x2_addition)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
      }};
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_b = {{
      {10.0f, 20.0f},
      {30.0f, 40.0f}
      }};
   Matrix_2X2_T mat_res;

   /** \action call FUT */
   Matrix_2x2_Add_Matrix_2x2(&mat_res, &mat_a, &mat_b);
   /** \assert result matches expectation */
   EXPECT_EQ(mat_res.elements[0][0], 11.0f);
   EXPECT_EQ(mat_res.elements[0][1], 22.0f);
   EXPECT_EQ(mat_res.elements[1][0], 33.0f);
   EXPECT_EQ(mat_res.elements[1][1], 44.0f);
}

/** add two 2x2 matrices
* \sdd{WI-13896}
*/
TEST(Matrix_2x2_Add_Matrix_2x2, WI_16140_Matrix_2x2_addition_commutative)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a{{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
      }};
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_b{{
      {10.0f, 20.0f},
      {30.0f, 40.0f}
      }};
   Matrix_2X2_T mat_res;

   /** \action call FUT */
   Matrix_2x2_Add_Matrix_2x2(&mat_res, &mat_b, &mat_a);
   /** \assert result matches expectation */
   EXPECT_EQ(mat_res.elements[0][0], 11.0f);
   EXPECT_EQ(mat_res.elements[0][1], 22.0f);
   EXPECT_EQ(mat_res.elements[1][0], 33.0f);
   EXPECT_EQ(mat_res.elements[1][1], 44.0f);
}

/** Multiply two matrices
* \sdd{WI-13894}
*/
TEST(Matrix_2x2_Mul_Scalar, WI_16141_Matrix_2x2_multiplication_with_scalar)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
      }};
   Matrix_2X2_T mat_res;

   /** \action call FUT */
   Matrix_2x2_Mul_Scalar(&mat_res, &mat_a, 2.0f);
   /** \assert result matches expectation */
   EXPECT_EQ(mat_res.elements[0][0], 2.0f * mat_a.elements[0][0]);
   EXPECT_EQ(mat_res.elements[0][1], 2.0f * mat_a.elements[0][1]);
   EXPECT_EQ(mat_res.elements[1][0], 2.0f * mat_a.elements[1][0]);
   EXPECT_EQ(mat_res.elements[1][1], 2.0f * mat_a.elements[1][1]);
}

/** Multiply two matrices
* \sdd{WI-13895}
*/
TEST(Matrix_3x3_Mul_Scalar, WI_16142_Matrix_3x3_multiplication_with_scalar)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
      }};
   Matrix_3X3_T mat_res;

   /** \action call FUT */
   Matrix_3x3_Mul_Scalar(&mat_res, &mat_a, 2.0f);
   /** \assert result matches expectation */
   EXPECT_EQ(mat_res.elements[0][0], 2.0f * mat_a.elements[0][0]);
   EXPECT_EQ(mat_res.elements[0][1], 2.0f * mat_a.elements[0][1]);
   EXPECT_EQ(mat_res.elements[0][2], 2.0f * mat_a.elements[0][2]);
   EXPECT_EQ(mat_res.elements[1][0], 2.0f * mat_a.elements[1][0]);
   EXPECT_EQ(mat_res.elements[1][1], 2.0f * mat_a.elements[1][1]);
   EXPECT_EQ(mat_res.elements[1][2], 2.0f * mat_a.elements[1][2]);
   EXPECT_EQ(mat_res.elements[2][0], 2.0f * mat_a.elements[2][0]);
   EXPECT_EQ(mat_res.elements[2][1], 2.0f * mat_a.elements[2][1]);
   EXPECT_EQ(mat_res.elements[2][2], 2.0f * mat_a.elements[2][2]);
}


