/*===========================================================================*\
* Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <gtest/gtest.h>
#include "ml_matrix.hpp"

#include "st_bool.h"
#include "ml_vector_2d.hpp"
#include "ml_vector_3d.hpp"
#include "st_compiler_warning.h"

/**
* Test that multiplying a matrix with an X-unit vector returns the first column vector
*/
TEST(HppMatrix_vector_mul_Matrix_2x2, Matrix_2x2_times_x_unit_vector_yields_first_column)
{
   /** \arrange fill matrix with arbitrary numbers, each entry different from all others */
   Matrix_2X2_T mat = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
   }};

   Vector_2d_T v = ml::vector2d::X();

      /** \action call FUT */
   Vector_2d_T ret = mat * v;

   /** \assert result matches first column vector */
   EXPECT_EQ(ret.x, Column(mat, 0).x);
   EXPECT_EQ(ret.y, Column(mat, 0).y);
}

/**
* Test that multiplying a matrix with an Y-unit vector returns the second column vector
*/
TEST(HppMatrix_vector_mul_Matrix_2x2, Matrix_2x2_times_y_unit_vector_yields_second_column)
{
   /** \arrange fill matrix with arbitrary numbers, each entry different from all others */
   Matrix_2X2_T mat = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
      }};

   Vector_2d_T v = ml::vector2d::Y();

   /** \action call FUT */
   Vector_2d_T ret = mat * v;

   /** \assert result matches second column vector */
   EXPECT_EQ(ret.x, Column(mat, 1).x);
   EXPECT_EQ(ret.y, Column(mat, 1).y);
}

/**
* Test that multiplying a X-unit vector with a matrix returns the first row vector
*/
TEST(HppMatrix_vector_mul_Matrix_2x2, x_unit_vector_times_Matrix_2x2_yields_first_row)
{
   /** \arrange fill matrix with arbitrary numbers, each entry different from all others */
   Matrix_2X2_T mat = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
   }};

   Vector_2d_T v = ml::vector2d::X();

   /** \action call FUT */
   Vector_2d_T ret = v * mat;

   /** \assert result matches first row vector */
   EXPECT_EQ(ret.x, Row(mat, 0).x);
   EXPECT_EQ(ret.y, Row(mat, 0).y);
}

/**
* Test that multiplying a Y-unit vector with a matrix returns the second row vector
*/
TEST(HppMatrix_vector_mul_Matrix_2x2, y_unit_vector_times_Matrix_2x2_yields_second_row)
{
   /** \arrange fill matrix with arbitrary numbers, each entry different from all others */
   Matrix_2X2_T mat = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
      }};

   Vector_2d_T v = ml::vector2d::Y();

   /** \action call FUT */
   Vector_2d_T ret = v * mat;

   /** \assert result matches second row vector */
   EXPECT_EQ(ret.x, Row(mat, 1).x);
   EXPECT_EQ(ret.y, Row(mat, 1).y);
}

/**
 * Ensure that the result is an identity matrix.
* \sdd{WI-13902}
*/
TEST(HppMatrix_2x2_Create_Identity_Matrix, 2x2_Create_Identity_Matrix_fills_correctly)
{
   /** \action call FUT */
   Matrix_2X2_T mat = ml::matrix_2x2::Identity();

   /** \assert result matches expectation */
   EXPECT_EQ(mat.elements[0][0], 1.0f);
   EXPECT_EQ(mat.elements[0][1], 0.0f);
   EXPECT_EQ(mat.elements[1][0], 0.0f);
   EXPECT_EQ(mat.elements[1][1], 1.0f);
}

/**
* Ensure that the result is a diagonal matrix.
*/
TEST(HppMatrix_2x2_Create_Diagonal_Matrix, 2x2_Create_Diagonal_Matrix_from_vector_fills_correctly)
{
   Vector_2d_T v = { 1.0f, 2.0f };
   /** \action call FUT */
   Matrix_2X2_T mat = ml::matrix_2x2::Diagonal(v);

   /** \assert result matches expectation */
   EXPECT_EQ(mat.elements[0][0], v.x);
   EXPECT_EQ(mat.elements[0][1], 0.0f);
   EXPECT_EQ(mat.elements[1][0], 0.0f);
   EXPECT_EQ(mat.elements[1][1], v.y);
}

/**
* Ensure that the result is a diagonal matrix.
*/
TEST(HppMatrix_2x2_Create_Diagonal_Matrix, 2x2_Create_Diagonal_Matrix_from_floats_fills_correctly)
{
   /** \action call FUT */
   Matrix_2X2_T mat = ml::matrix_2x2::Diagonal(1.0f, 2.0f);

   /** \assert result matches expectation */
   EXPECT_EQ(mat.elements[0][0], 1.0f);
   EXPECT_EQ(mat.elements[0][1], 0.0f);
   EXPECT_EQ(mat.elements[1][0], 0.0f);
   EXPECT_EQ(mat.elements[1][1], 2.0f);
}

/**
* Test that multiplying a X-unit vector with a matrix returns the first row vector
*/
TEST(HppMatrix_vector_mul_Matrix_3x3, x_unit_vector_times_Matrix_3x3_yields_first_row)
{
   /** \arrange fill matrix with arbitrary numbers, each entry different from all others */
   Matrix_3X3_T mat = {{
      {1.0f, 4.0f,  0.0f},
      {4.0f, 2.0f,  0.0f},
      {1.0f, 5.0f, -2.0f}
       }};

   Vector_3d_T v = ml::vector3d::X();

   /** \action call FUT */
   Vector_3d_T ret = v * mat;

   /** \assert result matches first row vector */
   EXPECT_EQ(ret.x, Row(mat, 0).x);
   EXPECT_EQ(ret.y, Row(mat, 0).y);
   EXPECT_EQ(ret.z, Row(mat, 0).z);
}

/**
* Test that multiplying a matrix with an Y-unit vector returns the second column vector
*/
TEST(HppMatrix_vector_mul_Matrix_3x3, Matrix_3x3_times_y_unit_vector_yields_second_column)
{
   /** \arrange fill matrix with arbitrary numbers, each entry different from all others */
   Matrix_3X3_T mat = {{
      {1.0f, 4.0f,  0.0f},
      {4.0f, 2.0f,  0.0f},
      {1.0f, 5.0f, -2.0f}
       }};

   Vector_3d_T v = ml::vector3d::Y();

   /** \action call FUT */
   Vector_3d_T ret = mat * v;

   /** \assert result matches second column vector */
   EXPECT_EQ(ret.x, Column(mat, 1).x);
   EXPECT_EQ(ret.y, Column(mat, 1).y);
   EXPECT_EQ(ret.z, Column(mat, 1).z);
}

/**
* Ensure that the result is a diagonal matrix.
*/
TEST(HppMatrix_3x3_Create_Diagonal_Matrix, 3x3_Create_Diagonal_Matrix_from_floats_fills_correctly)
{
   /** \action call FUT */
   Matrix_3X3_T mat = ml::matrix_3x3::Diagonal(1.0f, 2.0f, 3.0f);

   /** \assert result matches expectation */
   EXPECT_EQ(mat.elements[0][0], 1.0f);
   EXPECT_EQ(mat.elements[0][1], 0.0f);
   EXPECT_EQ(mat.elements[0][2], 0.0f);

   EXPECT_EQ(mat.elements[1][0], 0.0f);
   EXPECT_EQ(mat.elements[1][1], 2.0f);
   EXPECT_EQ(mat.elements[1][2], 0.0f);

   EXPECT_EQ(mat.elements[2][0], 0.0f);
   EXPECT_EQ(mat.elements[2][1], 0.0f);
   EXPECT_EQ(mat.elements[2][2], 3.0f);
}

/** ensure its possible to create a zero matrix
* \sdd{WI-13901}
*/
TEST(HppMatrix_2x2_Create_Zero_Matrix, Create_Zero_Matrix_fills_correctly)
{
   /** \action call FUT */
   Matrix_2X2_T mat = ml::matrix_2x2::Zero();

   /** \assert result matches expectation */
   EXPECT_EQ(mat.elements[0][0], 0.0f);
   EXPECT_EQ(mat.elements[0][1], 0.0f);
   EXPECT_EQ(mat.elements[1][0], 0.0f);
   EXPECT_EQ(mat.elements[1][1], 0.0f);
}

/** Ensure multiplication of two matrices works
* \sdd{WI-13900}
*/
TEST(HppMatrix_Matrix_2x2_Mul_Matrix_2x2, Matrix_2x2_multiplication)
{
   /** \arrange fill two matrices with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
   }};
   Matrix_2X2_T mat_b = {{
      {5.0f, 6.0f},
      {7.0f, 8.0f}
   }};
   Matrix_2X2_T mat;

   /** \action call FUT */
   mat = mat_a * mat_b;
   /** \assert result matches expectation */
   EXPECT_EQ(mat.elements[0][0], 19);
   EXPECT_EQ(mat.elements[0][1], 22);
   EXPECT_EQ(mat.elements[1][0], 43);
   EXPECT_EQ(mat.elements[1][1], 50);
}

/** Multiplication with a zero matrix
* \sdd{WI-13900}
*/
TEST(HppMatrix_Matrix_2x2_Mul_Matrix_2x2, Matrix_2x2_multiplication_with_zero)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
   }};
   /** \arrange create zero matrix */
   Matrix_2X2_T mat_b = {{
      {.0f , .0f},
      {.0f , .0f}
   }};
   Matrix_2X2_T mat;

   /** \action call FUT */
   mat = mat_a * mat_b;
   /** \assert result matches expectation */
   EXPECT_EQ(mat.elements[0][0], 0.0f);
   EXPECT_EQ(mat.elements[0][1], 0.0f);
   EXPECT_EQ(mat.elements[1][0], 0.0f);
   EXPECT_EQ(mat.elements[1][1], 0.0f);
}

/** Try out multiplication with identity matrix
* \sdd{WI-13900}
*/
TEST(HppMatrix_Matrix_2x2_Mul_Matrix_2x2, Matrix_2x2_multiplication_with_identity_matrix)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
   }};
   /** \arrange create identity matrix */
   Matrix_2X2_T mat_b = {{
      {1.0f, .0f},
      {.0f, 1.0f}
   }};
   Matrix_2X2_T mat;

   /** \action call FUT */
   mat = mat_a * mat_b;
   /** \assert result matches expectation */
   EXPECT_EQ(mat.elements[0][0], mat_a.elements[0][0]);
   EXPECT_EQ(mat.elements[0][1], mat_a.elements[0][1]);
   EXPECT_EQ(mat.elements[1][0], mat_a.elements[1][0]);
   EXPECT_EQ(mat.elements[1][1], mat_a.elements[1][1]);
}

/** Try creating an identity matrix
* \sdd{WI-13909}
*/
TEST(HppMatrix_3x3_Create_Identity_Matrix, Matrix_3x3_Create_Identity_Matrix_fills_correctly)
{
   /** \action call FUT */
   Matrix_3X3_T mat = ml::matrix_3x3::Identity();

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
TEST(HppMatrix_3x3_Create_Zero_Matrix, Matrix_3x3_Create_Zero_Matrix_fills_correctly)
{
   /** \action call FUT */
   Matrix_3X3_T mat = ml::matrix_3x3::Zero();

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
TEST(HppMatrix_matMul_3x3_3x3, Matrix_3x3_multiplication)
{
   /** \arrange fill two matrices with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
   }};
   Matrix_3X3_T mat_b = {{
      {10.0f, 20.0f, 30.0f},
      {40.0f, 50.0f, 60.0f},
      {70.0f, 80.0f, 90.0f}
   }};
   Matrix_3X3_T mat;

   /** \action call FUT */
   mat = mat_a * mat_b;
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
TEST(HppMatrix_matMul_3x3_3x3, Matrix_3x3_multiplication_with_zero)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
   }};
   /** \arrange create zero matrix */
   Matrix_3X3_T mat_b = {{
      {0.0f, 0.0f, 0.0f},
      {0.0f, 0.0f, 0.0f},
      {0.0f, 0.0f, 0.0f}
   }};
   Matrix_3X3_T mat;

   /** \action call FUT */
   mat = mat_a * mat_b;
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
TEST(HppMatrix_matMul_3x3_3x3, Matrix_3x3_multiplication_with_identity_matrix)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
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
   mat = mat_a * mat_b;
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
TEST(HppMatrix_Matrix_2x2_Mul_Vector_2d, Matrix_2x2_multiplication_with_vector)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a  = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
   }};
   /** \arrange fill vector with arbitrary values */
   Vector_2d_T  vector = { 100.0f, 200.0f };
   Vector_2d_T  res;

   /** \action call FUT */
   res = mat_a * vector;

   /** \assert result matches expectation */
   EXPECT_EQ(res.x, 500);
   EXPECT_EQ(res.y, 1100);
}

/** Multiplication of matrix and zero vector
* \sdd{WI-13904}
*/
TEST(HppMatrix_Matrix_2x2_Mul_Vector_2d, Matrix_2x2_multiplication_with_zero_vector)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
   }};
   /** \arrange fill vector with zeros */
   Vector_2d_T  vector = { .0f };
   Vector_2d_T  res;

   /** \action call FUT */
   res = mat_a * vector;
   /** \assert result matches expectation */
   EXPECT_EQ(res.x, .0f);
   EXPECT_EQ(res.y, .0f);
}

/** read a row vector
* \sdd{WI-13904}
*/
TEST(HppMatrix_Matrix_2x2_Mul_Vector_2d, Matrix_2x2_get_row_vector_0)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
      }};
   /** \arrange set a vector to read the first row */
   Vector_2d_T  vector = { 1.0f, .0f };
   Vector_2d_T  res;

   /** \action call FUT */
   res = mat_a * vector;
   /** \assert result matches expectation */
   EXPECT_EQ(res.x, mat_a.elements[0][0]);
   EXPECT_EQ(res.y, mat_a.elements[1][0]);
}

/** Create from row vectors
*/
TEST(HppMatrix_Matrix_2x2_Mul_Vector_2d, Create_from_Row)
{
   Vector_2d_T row0 = { 1.0f, 3.0f };
   Vector_2d_T row1 = { 2.0f, 4.0f };

   /** \action call FUT */
   Matrix_2X2_T mat_res = ml::matrix_2x2::Row(row0, row1);

   /** \assert result matches expectation */
   EXPECT_EQ(Row(mat_res, 0).x, row0.x);
   EXPECT_EQ(Row(mat_res, 0).y, row0.y);
   EXPECT_EQ(Row(mat_res, 1).x, row1.x);
   EXPECT_EQ(Row(mat_res, 1).y, row1.y);
}

/** read a row vector
*/
TEST(HppMatrix_Matrix_2x2_Mul_Vector_2d, Row_0)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
   }};

   /** \action call FUT */
   Vector_2d_T  res = Row(mat_a, 0);

   /** \assert result matches expectation */
   EXPECT_EQ(res.x, mat_a.elements[0][0]);
   EXPECT_EQ(res.y, mat_a.elements[0][1]);
}

/** read a row vector
*/
TEST(HppMatrix_Matrix_2x2_Mul_Vector_2d, Row_1)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
      }};

   /** \action call FUT */
   Vector_2d_T  res = Row(mat_a, 1);

   /** \assert result matches expectation */
   EXPECT_EQ(res.x, mat_a.elements[1][0]);
   EXPECT_EQ(res.y, mat_a.elements[1][1]);
}

/** read a column vector
*/
TEST(HppMatrix_Matrix_2x2_Mul_Vector_2d, Column_0)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
   }};

   /** \action call FUT */
   Vector_2d_T  res = Column(mat_a, 0);

   /** \assert result matches expectation */
   EXPECT_EQ(res.x, mat_a.elements[0][0]);
   EXPECT_EQ(res.y, mat_a.elements[1][0]);
}

/** read a column vector
*/
TEST(HppMatrix_Matrix_2x2_Mul_Vector_2d, Column_1)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
   }};

   /** \action call FUT */
   Vector_2d_T  res = Column(mat_a, 1);

   /** \assert result matches expectation */
   EXPECT_EQ(res.x, mat_a.elements[0][1]);
   EXPECT_EQ(res.y, mat_a.elements[1][1]);
}

/** create from column vector
*/
TEST(HppMatrix_Matrix_2x2_Mul_Vector_2d, Create_from_column)
{
   Vector_2d_T col0 = { 1.0f, 2.0f };
   Vector_2d_T col1 = { 3.0f, 4.0f };

   /** \action call FUT */
   Matrix_2X2_T mat_res = ml::matrix_2x2::Column(col0, col1);

   /** \assert result matches expectation */
   EXPECT_EQ(Column(mat_res, 0).x, col0.x);
   EXPECT_EQ(Column(mat_res, 0).y, col0.y);
   EXPECT_EQ(Column(mat_res, 1).x, col1.x);
   EXPECT_EQ(Column(mat_res, 1).y, col1.y);
}



/**
* \sdd{WI-13904}
*/
TEST(HppMatrix_Matrix_2x2_Mul_Vector_2d, Matrix_2x2_get_row_vector_1)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
   }};
   /** \arrange set a vector to read the second row */
   Vector_2d_T  vector = { .0f, 1.0f };
   Vector_2d_T  res;

   /** \action call FUT */
   res = mat_a * vector;
   /** \assert result matches expectation */
   EXPECT_EQ(res.x, mat_a.elements[0][1]);
   EXPECT_EQ(res.y, mat_a.elements[1][1]);
}



/** calculate determinant of 2x2 Matrix
* \sdd{WI-13898}
*/
TEST(HppMatrix_Matrix_2x2_Determinant, Matrix_2x2_determinant)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
   }};
   /** \action call FUT */
   float res = Determinant(mat_a);

   /** \assert result matches expectation */
   EXPECT_EQ(res, -2.0f);
}

/** calculate determinant of 3x3 Matrix
* \sdd{WI-13905}
*/
TEST(HppMatrix_Matrix_3x3_Determinant, Matrix_3x3_determinant)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 20.0f, 3.0f},
      {40.0f, 5.0f, 60.0f},
      {7.0f, 80.0f, 9.0f}
   }};
   /** \action call FUT */
   float res = Determinant(mat_a);

   /** \assert result matches expectation */
   EXPECT_EQ(res, 5940);
}

/** Error case: Determinant too small
* \sdd{WI-13910}
* \sdd{WI-13903}
*/
TEST(HppMatrix_Matrix_2x2_Inverse_Given_Determinant, Matrix_2x2_determinant_too_small)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {0.0f, 0.0f},
      {3.0f, 4.0f}
   }};

   EXPECT_DEBUG_DEATH({
      /** \action call FUT with a zero determinant */
      Inverse(mat_a, 0.0f);

      /** \assert the matching assertion is thrown */
   }, "false");
#ifdef NDEBUG
   /** \action call FUT */
   Msvs_Disable_Warning(4723)
   auto ret = Inverse(mat_a, 0.0f);
   Msvs_Enable_Warning(4723)
   EXPECT_EQ(ret.elements[0][0], 0.0f);
   EXPECT_EQ(ret.elements[0][1], 0.0f);

   EXPECT_EQ(ret.elements[1][0], 0.0f);
   EXPECT_EQ(ret.elements[1][1], 0.0f);
#endif
}

/** Error case Determinant too small
* \sdd{WI-13910}
* \sdd{WI-13903}
*/
TEST(HppMatrix_Matrix_3x3_Inverse_Given_Determinant, Matrix_3x3_determinant_too_small)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {0.0f, 0.0f, 0.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
   }};

   EXPECT_DEBUG_DEATH({
      /** \action call FUT with a zero determinant */
      Inverse(mat_a, 0.0f);

   /** \assert the matching assertion is thrown */
   }, "false");
#ifdef NDEBUG
   /** \action call FUT */
   Msvs_Disable_Warning(4723)
   auto ret = Inverse(mat_a, 0.0f);
   Msvs_Enable_Warning(4723)
   EXPECT_EQ(ret.elements[0][0], 0.0f);
   EXPECT_EQ(ret.elements[0][1], 0.0f);
   EXPECT_EQ(ret.elements[0][2], 0.0f);
   EXPECT_EQ(ret.elements[1][0], 0.0f);
   EXPECT_EQ(ret.elements[1][1], 0.0f);
   EXPECT_EQ(ret.elements[1][2], 0.0f);
   EXPECT_EQ(ret.elements[2][0], 0.0f);
   EXPECT_EQ(ret.elements[2][1], 0.0f);
   EXPECT_EQ(ret.elements[2][2], 0.0f);
#endif
}

/** calculate inverse using determinant
* \sdd{WI-13910}
* \sdd{WI-13903}
*/
TEST(HppMatrix_Matrix_2x2_Inverse_Given_Determinant, Matrix_2x2_inverse)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
   }};
   /** \action call FUT */
   auto ret = Inverse(mat_a, -2.0f);

   /** \assert result matches expectation */
   EXPECT_EQ(ret.elements[0][0], -2.0f);
   EXPECT_EQ(ret.elements[0][1], 1.0f);
   EXPECT_EQ(ret.elements[1][0], 1.5f);
   EXPECT_EQ(ret.elements[1][1], -0.5f);
}

TEST(HppMatrix_Matrix_3x3, Submatrix_0_0)
{
/** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
   }};

   Matrix_2X2_T sub = Submatrix(mat_a, 0u, 0u);

   /** \assert result matches expectation */
   EXPECT_EQ(sub.elements[0][0], 5.0f);
   EXPECT_EQ(sub.elements[0][1], 6.0f);
   EXPECT_EQ(sub.elements[1][0], 8.0f);
   EXPECT_EQ(sub.elements[1][1], 9.0f);
}

TEST(HppMatrix_Matrix_3x3, Submatrix_0_1)
{
/** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
   }};

   Matrix_2X2_T sub = Submatrix(mat_a, 0u, 1u);

   /** \assert result matches expectation */
   EXPECT_EQ(sub.elements[0][0], 4.0f);
   EXPECT_EQ(sub.elements[0][1], 6.0f);
   EXPECT_EQ(sub.elements[1][0], 7.0f);
   EXPECT_EQ(sub.elements[1][1], 9.0f);
}

TEST(HppMatrix_Matrix_3x3, Submatrix_0_2)
{
/** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
   }};

   Matrix_2X2_T sub = Submatrix(mat_a, 0u, 2u);

   /** \assert result matches expectation */
   EXPECT_EQ(sub.elements[0][0], 4.0f);
   EXPECT_EQ(sub.elements[0][1], 5.0f);
   EXPECT_EQ(sub.elements[1][0], 7.0f);
   EXPECT_EQ(sub.elements[1][1], 8.0f);
}

TEST(HppMatrix_Matrix_3x3, Submatrix_1_0)
{
/** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
   }};

   Matrix_2X2_T sub = Submatrix(mat_a, 1u, 0u);

   /** \assert result matches expectation */
   EXPECT_EQ(sub.elements[0][0], 2.0f);
   EXPECT_EQ(sub.elements[0][1], 3.0f);
   EXPECT_EQ(sub.elements[1][0], 8.0f);
   EXPECT_EQ(sub.elements[1][1], 9.0f);
}

TEST(HppMatrix_Matrix_3x3, Submatrix_1_1)
{
/** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
   }};

   Matrix_2X2_T sub = Submatrix(mat_a, 1u, 1u);

   /** \assert result matches expectation */
   EXPECT_EQ(sub.elements[0][0], 1.0f);
   EXPECT_EQ(sub.elements[0][1], 3.0f);
   EXPECT_EQ(sub.elements[1][0], 7.0f);
   EXPECT_EQ(sub.elements[1][1], 9.0f);
}

TEST(HppMatrix_Matrix_3x3, Submatrix_1_2)
{
/** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
   }};

   Matrix_2X2_T sub = Submatrix(mat_a, 1u, 2u);

   /** \assert result matches expectation */
   EXPECT_EQ(sub.elements[0][0], 1.0f);
   EXPECT_EQ(sub.elements[0][1], 2.0f);
   EXPECT_EQ(sub.elements[1][0], 7.0f);
   EXPECT_EQ(sub.elements[1][1], 8.0f);
}

TEST(HppMatrix_Matrix_3x3, Submatrix_2_0)
{
/** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
   }};

   Matrix_2X2_T sub = Submatrix(mat_a, 2u, 0u);

   /** \assert result matches expectation */
   EXPECT_EQ(sub.elements[0][0], 2.0f);
   EXPECT_EQ(sub.elements[0][1], 3.0f);
   EXPECT_EQ(sub.elements[1][0], 5.0f);
   EXPECT_EQ(sub.elements[1][1], 6.0f);
}

TEST(HppMatrix_Matrix_3x3, Submatrix_2_1)
{
/** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
   }};

   Matrix_2X2_T sub = Submatrix(mat_a, 2u, 1u);

   /** \assert result matches expectation */
   EXPECT_EQ(sub.elements[0][0], 1.0f);
   EXPECT_EQ(sub.elements[0][1], 3.0f);
   EXPECT_EQ(sub.elements[1][0], 4.0f);
   EXPECT_EQ(sub.elements[1][1], 6.0f);
}

TEST(HppMatrix_Matrix_3x3, Submatrix_2_2)
{
/** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
   }};

   Matrix_2X2_T sub = Submatrix(mat_a, 2u, 2u);

   /** \assert result matches expectation */
   EXPECT_EQ(sub.elements[0][0], 1.0f);
   EXPECT_EQ(sub.elements[0][1], 2.0f);
   EXPECT_EQ(sub.elements[1][0], 4.0f);
   EXPECT_EQ(sub.elements[1][1], 5.0f);
}

/** calculate inverse using determinant
* \sdd{WI-13910}
* \sdd{WI-13903}
*/
TEST(HppMatrix_Matrix_3x3_Inverse_Given_Determinant, Matrix_3x3_inverse)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {0.0f, 1.0f, 4.0f},
      {5.0f, 6.0f, 0.0f}
   }};
   auto det = Determinant(mat_a);
   /** \action call FUT */
   auto ret = Inverse(mat_a, det);

   EXPECT_EQ(det, 1.f);

   /** \assert result matches expectation */
   EXPECT_EQ(ret.elements[0][0], -24.f);
   EXPECT_EQ(ret.elements[0][1],  18.f);
   EXPECT_EQ(ret.elements[0][2],   5.f);

   EXPECT_EQ(ret.elements[1][0],  20.f);
   EXPECT_EQ(ret.elements[1][1], -15.f);
   EXPECT_EQ(ret.elements[1][2],  -4.f);

   EXPECT_EQ(ret.elements[2][0], -5.f);
   EXPECT_EQ(ret.elements[2][1],  4.f);
   EXPECT_EQ(ret.elements[2][2],  1.f);
}


/** error case: Inversion fails because determinant is too small
* \sdd{WI-13912}
* \sdd{WI-13903}
*/
TEST(HppMatrix_Matrix_2x2_Inverse, Matrix_2x2_det_too_small)
{
   /** \arrange fill matrix in a way that leads to a small determinant */
   Matrix_2X2_T mat_a = {{
      {0.0f, 0.0f},
      {3.0f, 4.0f}
      }};

   /** \action call FUT */
   EXPECT_DEBUG_DEATH({
   Inverse(mat_a, Determinant(mat_a));
   /** \assert the matching assertion is thrown */
   }, "false");
#ifdef NDEBUG
   /** \action call FUT */
   auto ret = Inverse(mat_a, Determinant(mat_a));
   /** \assert fail */
   EXPECT_EQ(ret.elements[0][0], 0.0f);
   EXPECT_EQ(ret.elements[0][1], 0.0f);

   EXPECT_EQ(ret.elements[1][0], 0.0f);
   EXPECT_EQ(ret.elements[1][1], 0.0f);
#endif
}

/** Try out inverting a matrix
* \sdd{WI-13912}
* \sdd{WI-13903}
*/
TEST(HppMatrix_Matrix_2x2_Inverse, Matrix_2x2_det_ok)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
   }};
   /** \action call FUT */
   auto ret = Inverse(mat_a, Determinant(mat_a));

   /** \assert result matches expectation */
   EXPECT_EQ(ret.elements[0][0], -2.0f);
   EXPECT_EQ(ret.elements[0][1], 1.0f);
   EXPECT_EQ(ret.elements[1][0], 1.5f);
   EXPECT_EQ(ret.elements[1][1], -0.5f);
}


/** try transposing a 2x2 matrix
* \sdd{WI-13908}
*/
TEST(HppMatrix_Matrix_2x2_Transpose, Matrix_2x2_transposed)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
   }};

   /** \action call FUT */
   mat_a = Transpose(mat_a);
   /** \assert result matches expectation */
   EXPECT_EQ(mat_a.elements[0][0], 1.0f);
   EXPECT_EQ(mat_a.elements[0][1], 3.0f);
   EXPECT_EQ(mat_a.elements[1][0], 2.0f);
   EXPECT_EQ(mat_a.elements[1][1], 4.0f);
}


/** try transposing a 3x3 matrix
* \sdd{WI-13913}
*/
TEST(HppMatrix_Matrix_3x3_Transpose, Matrix_3x3_transposed)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
   }};

   /** \action call FUT */
   mat_a = Transpose(mat_a);
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
TEST(HppMatrix_3x3_Add_Matrix_3x3, Matrix_3x3_addition_with_self)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
   }};
   Matrix_3X3_T mat_res;

   /** \action call FUT */
   mat_res = mat_a + mat_a;

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

/** Subtract a 3x3 matrix from itself
*/
TEST(HppMatrix_3x3_Add_Matrix_3x3, Matrix_3x3_subtraction_from_self)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
   }};
   Matrix_3X3_T mat_res;

   auto mat_b = mat_a;

   /** \action call FUT */
   mat_res = mat_a - mat_b;

   /** \assert result matches expectation */
   EXPECT_EQ(mat_res.elements[0][0], 0.0f);
   EXPECT_EQ(mat_res.elements[0][1], 0.0f);
   EXPECT_EQ(mat_res.elements[0][2], 0.0f);
   EXPECT_EQ(mat_res.elements[1][0], 0.0f);
   EXPECT_EQ(mat_res.elements[1][1], 0.0f);
   EXPECT_EQ(mat_res.elements[1][2], 0.0f);
   EXPECT_EQ(mat_res.elements[2][0], 0.0f);
   EXPECT_EQ(mat_res.elements[2][1], 0.0f);
   EXPECT_EQ(mat_res.elements[2][2], 0.0f);
}

/** Subtract a 2x2 matrix from itself
*/
TEST(HppMatrix_x2x_Add_Matrix_3x3, Matrix_x2x_subtraction_from_self)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
   }};
   Matrix_2X2_T mat_res;

   auto mat_b = mat_a;

   /** \action call FUT */
   mat_res = mat_a - mat_b;

   /** \assert result matches expectation */
   EXPECT_EQ(mat_res.elements[0][0], 0.0f);
   EXPECT_EQ(mat_res.elements[0][1], 0.0f);
   EXPECT_EQ(mat_res.elements[1][0], 0.0f);
   EXPECT_EQ(mat_res.elements[1][1], 0.0f);
}

/** Subtract a 3x3 matrix from itself
*/
TEST(HppMatrix_3x3_Add_Matrix_3x3, Matrix_3x3_subtraction_from_self_assigning)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
   }};

   auto mat_b = mat_a;

   /** \action call FUT */
   mat_a -= mat_b;

   /** \assert result matches expectation */
   EXPECT_EQ(mat_a.elements[0][0], 0.0f);
   EXPECT_EQ(mat_a.elements[0][1], 0.0f);
   EXPECT_EQ(mat_a.elements[0][2], 0.0f);
   EXPECT_EQ(mat_a.elements[1][0], 0.0f);
   EXPECT_EQ(mat_a.elements[1][1], 0.0f);
   EXPECT_EQ(mat_a.elements[1][2], 0.0f);
   EXPECT_EQ(mat_a.elements[2][0], 0.0f);
   EXPECT_EQ(mat_a.elements[2][1], 0.0f);
   EXPECT_EQ(mat_a.elements[2][2], 0.0f);
}

/** Subtract a 2x2 matrix from itself
*/
TEST(HppMatrix_x2x_Add_Matrix_3x3, Matrix_x2x_subtraction_from_self_assigning)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
   }};

   auto mat_b = mat_a;

   /** \action call FUT */
   mat_a -= mat_b;

   /** \assert result matches expectation */
   EXPECT_EQ(mat_a.elements[0][0], 0.0f);
   EXPECT_EQ(mat_a.elements[0][1], 0.0f);
   EXPECT_EQ(mat_a.elements[1][0], 0.0f);
   EXPECT_EQ(mat_a.elements[1][1], 0.0f);
}

/** Add a 3x3 matrix to itself
* \sdd{WI-13893}
*/
TEST(HppMatrix_3x3_Add_Matrix_3x3, Matrix_3x3_addition_with_self_assigning)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
   }};
   Matrix_3X3_T mat_res = mat_a;

   /** \action call FUT */
   mat_res += mat_a;

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
TEST(HppMatrix_3x3_Add_Matrix_3x3, Matrix_3x3_addition)
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
   mat_res = mat_a + mat_b;
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
TEST(HppMatrix_3x3_Add_Matrix_3x3, Matrix_3x3_addition_commutative)
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
   mat_res = mat_b + mat_a;
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
TEST(HppMatrix_2x2_Add_Matrix_2x2, Matrix_2x2_addition_with_self)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {.0f, 2.0f},
      {3.0f, 4.0f}
   }};
   Matrix_2X2_T mat_res;

   /** \action call FUT */
   mat_res = mat_a + mat_a;
   /** \assert result matches expectation */
   EXPECT_EQ(mat_res.elements[0][0], 2 * mat_a.elements[0][0]);
   EXPECT_EQ(mat_res.elements[0][1], 2 * mat_a.elements[0][1]);
   EXPECT_EQ(mat_res.elements[1][0], 2 * mat_a.elements[1][0]);
   EXPECT_EQ(mat_res.elements[1][1], 2 * mat_a.elements[1][1]);
}

/** Add a 2x2 matrix to itself
* \sdd{WI-13896}
*/
TEST(HppMatrix_2x2_Add_Matrix_2x2, Matrix_2x2_addition_with_self_assigning)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
   }};
   Matrix_2X2_T mat_res = mat_a;

   /** \action call FUT */
   mat_res += mat_a;
   /** \assert result matches expectation */
   EXPECT_EQ(mat_res.elements[0][0], 2 * mat_a.elements[0][0]);
   EXPECT_EQ(mat_res.elements[0][1], 2 * mat_a.elements[0][1]);
   EXPECT_EQ(mat_res.elements[1][0], 2 * mat_a.elements[1][0]);
   EXPECT_EQ(mat_res.elements[1][1], 2 * mat_a.elements[1][1]);
}

/** add two 2x2 matrices
* \sdd{WI-13896}
*/
TEST(HppMatrix_2x2_Add_Matrix_2x2, Matrix_2x2_addition)
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
   mat_res = mat_a + mat_b;
   /** \assert result matches expectation */
   EXPECT_EQ(mat_res.elements[0][0], 11.0f);
   EXPECT_EQ(mat_res.elements[0][1], 22.0f);
   EXPECT_EQ(mat_res.elements[1][0], 33.0f);
   EXPECT_EQ(mat_res.elements[1][1], 44.0f);
}

/** add two 2x2 matrices
* \sdd{WI-13896}
*/
TEST(HppMatrix_2x2_Add_Matrix_2x2, Matrix_2x2_addition_commutative)
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
   mat_res = mat_b + mat_a;
   /** \assert result matches expectation */
   EXPECT_EQ(mat_res.elements[0][0], 11.0f);
   EXPECT_EQ(mat_res.elements[0][1], 22.0f);
   EXPECT_EQ(mat_res.elements[1][0], 33.0f);
   EXPECT_EQ(mat_res.elements[1][1], 44.0f);
}

/** Multiply matrix with scalar
* \sdd{WI-13894}
*/
TEST(HppMatrix_2x2_Mul_Scalar, Matrix_2x2_multiplication_with_scalar)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
   }};
   Matrix_2X2_T mat_res;

   /** \action call FUT */
   mat_res = mat_a * 2.0f;
   /** \assert result matches expectation */
   EXPECT_EQ(mat_res.elements[0][0], 2.0f * mat_a.elements[0][0]);
   EXPECT_EQ(mat_res.elements[0][1], 2.0f * mat_a.elements[0][1]);
   EXPECT_EQ(mat_res.elements[1][0], 2.0f * mat_a.elements[1][0]);
   EXPECT_EQ(mat_res.elements[1][1], 2.0f * mat_a.elements[1][1]);
}

/** Multiply matrix with scalar
* \sdd{WI-13894}
*/
TEST(HppMatrix_2x2_Mul_Scalar, Matrix_2x2_multiplication_with_scalar_assigning)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
   }};
   Matrix_2X2_T mat_res = mat_a;

   /** \action call FUT */
   mat_res *= 2.0f;
   /** \assert result matches expectation */
   EXPECT_EQ(mat_res.elements[0][0], 2.0f * mat_a.elements[0][0]);
   EXPECT_EQ(mat_res.elements[0][1], 2.0f * mat_a.elements[0][1]);
   EXPECT_EQ(mat_res.elements[1][0], 2.0f * mat_a.elements[1][0]);
   EXPECT_EQ(mat_res.elements[1][1], 2.0f * mat_a.elements[1][1]);
}

TEST(HppMatrix_2x2_Mul_Scalar, unary_minus)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
   }};
   Matrix_2X2_T mat_res;

   /** \action call FUT */
   mat_res = -mat_a;
   /** \assert result matches expectation */
   EXPECT_EQ(mat_res.elements[0][0], -1.0f * mat_a.elements[0][0]);
   EXPECT_EQ(mat_res.elements[0][1], -1.0f * mat_a.elements[0][1]);
   EXPECT_EQ(mat_res.elements[1][0], -1.0f * mat_a.elements[1][0]);
   EXPECT_EQ(mat_res.elements[1][1], -1.0f * mat_a.elements[1][1]);
}

TEST(HppMatrix_3x3_Mul_Scalar, unary_minus)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
   }};
   Matrix_3X3_T mat_res;

   /** \action call FUT */
   mat_res = -mat_a;
   /** \assert result matches expectation */
   EXPECT_EQ(mat_res.elements[0][0], -1.0f * mat_a.elements[0][0]);
   EXPECT_EQ(mat_res.elements[0][1], -1.0f * mat_a.elements[0][1]);
   EXPECT_EQ(mat_res.elements[0][2], -1.0f * mat_a.elements[0][2]);
   EXPECT_EQ(mat_res.elements[1][0], -1.0f * mat_a.elements[1][0]);
   EXPECT_EQ(mat_res.elements[1][1], -1.0f * mat_a.elements[1][1]);
   EXPECT_EQ(mat_res.elements[1][2], -1.0f * mat_a.elements[1][2]);
   EXPECT_EQ(mat_res.elements[2][0], -1.0f * mat_a.elements[2][0]);
   EXPECT_EQ(mat_res.elements[2][1], -1.0f * mat_a.elements[2][1]);
   EXPECT_EQ(mat_res.elements[2][2], -1.0f * mat_a.elements[2][2]);
}

TEST(HppMatrix_2x2_Mul_Scalar, unary_plus)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
   }};
   Matrix_2X2_T mat_res;

   /** \action call FUT */
   mat_res = +mat_a;
   /** \assert result matches expectation */
   EXPECT_EQ(mat_res.elements[0][0], mat_a.elements[0][0]);
   EXPECT_EQ(mat_res.elements[0][1], mat_a.elements[0][1]);
   EXPECT_EQ(mat_res.elements[1][0], mat_a.elements[1][0]);
   EXPECT_EQ(mat_res.elements[1][1], mat_a.elements[1][1]);
}

TEST(HppMatrix_3x3_Mul_Scalar, unary_plus)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
   }};
   Matrix_3X3_T mat_res;

   /** \action call FUT */
   mat_res = +mat_a;
   /** \assert result matches expectation */
   EXPECT_EQ(mat_res.elements[0][0], mat_a.elements[0][0]);
   EXPECT_EQ(mat_res.elements[0][1], mat_a.elements[0][1]);
   EXPECT_EQ(mat_res.elements[0][2], mat_a.elements[0][2]);
   EXPECT_EQ(mat_res.elements[1][0], mat_a.elements[1][0]);
   EXPECT_EQ(mat_res.elements[1][1], mat_a.elements[1][1]);
   EXPECT_EQ(mat_res.elements[1][2], mat_a.elements[1][2]);
   EXPECT_EQ(mat_res.elements[2][0], mat_a.elements[2][0]);
   EXPECT_EQ(mat_res.elements[2][1], mat_a.elements[2][1]);
   EXPECT_EQ(mat_res.elements[2][2], mat_a.elements[2][2]);
}

/** Multiply matrix with scalar
* \sdd{WI-13894}
*/
TEST(HppMatrix_2x2_Mul_Scalar, Matrix_2x2_multiplication_with_scalar_reverse)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_2X2_T mat_a = {{
      {1.0f, 2.0f},
      {3.0f, 4.0f}
   }};
   Matrix_2X2_T mat_res;

   /** \action call FUT */
   mat_res = 2.0f * mat_a;
   /** \assert result matches expectation */
   EXPECT_EQ(mat_res.elements[0][0], 2.0f * mat_a.elements[0][0]);
   EXPECT_EQ(mat_res.elements[0][1], 2.0f * mat_a.elements[0][1]);
   EXPECT_EQ(mat_res.elements[1][0], 2.0f * mat_a.elements[1][0]);
   EXPECT_EQ(mat_res.elements[1][1], 2.0f * mat_a.elements[1][1]);
}

/** Multiply matrix with scalar
* \sdd{WI-13895}
*/
TEST(HppMatrix_3x3_Mul_Scalar, Matrix_3x3_multiplication_with_scalar)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
   }};
   Matrix_3X3_T mat_res;

   /** \action call FUT */
   mat_res = mat_a * 2.0f;
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

/** Multiply matrix with scalar
* \sdd{WI-13895}
*/
TEST(HppMatrix_3x3_Mul_Scalar, Matrix_3x3_multiplication_with_scalar_assigning)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
   }};
   Matrix_3X3_T mat_res = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
   }};

   /** \action call FUT */
   mat_res *= 2.0f;
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

/** Multiply matrix with scalar
* \sdd{WI-13895}
*/
TEST(HppMatrix_3x3_Mul_Scalar, Matrix_3x3_multiplication_with_scalar_reverse)
{
   /** \arrange fill matrix with arbitrary values */
   Matrix_3X3_T mat_a = {{
      {1.0f, 2.0f, 3.0f},
      {4.0f, 5.0f, 6.0f},
      {7.0f, 8.0f, 9.0f}
   }};
   Matrix_3X3_T mat_res;

   /** \action call FUT */
   mat_res = 2.0f * mat_a;
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


