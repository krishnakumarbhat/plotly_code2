/** \file
   Give a detailed description of what  this unit-test file contain.
 */

#include "f360_linear_solver_data_types.h"
#include "f360_matrix_vector_Init_real32_T.h"
#include "f360_LinearSolvers.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

#include "f360_math_func.h"
//sneak in mocked functions
//Declaration of stubbed/mock functions

//Implementation of stubbed interfaces

/** \defgroup  f360_LinearSolvers
*  @{
*/

using namespace f360_variant_A;
/** \brief
 * Test the validity of the function qr_modified_gram_schimdt() and F360_Fit_Second_Degree_Polynomial().
 **/
TEST_GROUP(f360_LinearSolvers)
{
};


/**
 *\purpose  Verify that QR factorization of matrix A works as intended
 *          Expected relation is A = Q*R
 *\req    NA
 */
TEST(f360_LinearSolvers, qr_modified_gram_schimdt_QR_Factorization)
{
    /** \precond
     * Declare matrix A
     * Initialize matrix Q and R
     * Define test pass threshold
     **/
   F360_matrix_real32_LSC_T A;
   A.m_size[0] = 5;
   A.m_size[1] = 3;
   A.numDimensions = 3;
   A.data[0][0] = 1.0F;
   A.data[0][1] = 2.0F;
   A.data[0][2] = 3.0F;
   A.data[1][0] = 2.0F;
   A.data[1][1] = 3.0F;
   A.data[1][2] = 1.0F;
   A.data[2][0] = 1.0F;
   A.data[2][1] = 4.0F;
   A.data[2][2] = 3.0F;
   A.data[3][0] = 1.0F;
   A.data[3][1] = 5.0F;
   A.data[3][2] = 3.0F;
   A.data[4][0] = 2.0F;
   A.data[4][1] = 3.0F;
   A.data[4][2] = 1.0F;

   F360_matrix_real32_LSC_T Q;
   F360_matrix_real32_LSC_T R;

   float32_t test_pass_thres = 1.0e-4F;

   /** \action
    * Call function and reconstruct A matrix from result
    * Also map Q and R matrix to matrix of smaller size so
    * we can use function F360_Matmul_MxN_NxP() to reconstruct
    * expected result Q*R = A 
    **/

   qr_modified_gram_schimdt(Q, R, A);

   float32_t Q_reconstruct[5][3];
   for (uint32_t row = 0U; row < 5U; row++)
   {
      for (uint32_t col = 0U; col < 3U; col++)
      {
         Q_reconstruct[row][col] = Q.data[row][col];
      }
   }

   float32_t R_reconstruct[3][3];
   for (uint32_t row = 0U; row < 3U; row++)
   {
      for (uint32_t col = 0U; col < 3U; col++)
      {
         R_reconstruct[row][col] = R.data[row][col];
      }
   }

   float32_t A_reconstruct[5][3];
   F360_Matmul_MxN_NxP(
      Q_reconstruct,
      R_reconstruct,
      A_reconstruct,
      5U,
      3U,
      3U);

   /** \result
    * Verify that A = Q * R
    **/
   for (uint32_t row = 0U; row < 5U; row++)
   {
      for (uint32_t col = 0U; col < 3U; col++)
      {
         DOUBLES_EQUAL(A.data[row][col], A_reconstruct[row][col], test_pass_thres);
      }
   }
}

/**
 *\purpose  Verify polynomial fit returns false when input to function doesn't
 *          match preconditions
 *          Number of rows in A don't match rows in B
 *\req    NA
 */
TEST(f360_LinearSolvers, F360_Fit_Second_Degree_Polynomial_Matrix_Row_Size_Dont_Match)
{
   /** \precond
    * Declare matrix A and B
    **/
   F360_matrix_real32_LSC_T A;
   A.m_size[0] = 5;
   A.m_size[1] = 3;
   A.numDimensions = 3;
   
   F360_vector_real32_LSC_T B;
   B.m_size = 4;

   float32_t a;
   float32_t b;
   float32_t c;

   /** \action
    * Call function
    **/
   bool f_poly_fit_ok = F360_Fit_Second_Degree_Polynomial(A, B, a, b, c);

   /** \result
    * Verify that function have returned false
    **/
   CHECK_FALSE(f_poly_fit_ok); 
}

/**
 *\purpose  Verify polynomial fit returns false when input to function doesn't
 *          match preconditions
 *          Number of columns in A differs from 3
 *\req    NA
 */
TEST(f360_LinearSolvers, F360_Fit_Second_Degree_Polynomial_Matrix_Col_Size_Dont_Match)
{
   /** \precond
    * Declare matrix A and B
    * Set rows of A greater than rows of B
    **/
   F360_matrix_real32_LSC_T A;
   A.m_size[0] = 5;
   A.m_size[1] = 4;
   A.numDimensions = 4;

   F360_vector_real32_LSC_T B;
   B.m_size = 4;

   float32_t a;
   float32_t b;
   float32_t c;

   /** \action
    * Call function
    **/
   bool f_poly_fit_ok = F360_Fit_Second_Degree_Polynomial(A, B, a, b, c);

   /** \result
    * Verify that function have returned false
    **/
   CHECK_FALSE(f_poly_fit_ok);
}

/**
 *\purpose  Verify polynomial fit returns false when input to function doesn't
 *          match preconditions
 *          Number of rows in A is less than 3 which is minimum number of
 *          points to fit a second degree polynomial
 *\req    NA
 */
TEST(f360_LinearSolvers, F360_Fit_Second_Degree_Polynomial_Matrix_Row_Size_Too_Small)
{
   /** \precond
    * Declare matrix A and B
    * Set number of rows in A less than 3
    **/
   F360_matrix_real32_LSC_T A;
   A.m_size[0] = 2;
   A.m_size[1] = 3;
   A.numDimensions = 3;

   F360_vector_real32_LSC_T B;
   B.m_size = 2;

   float32_t a;
   float32_t b;
   float32_t c;

   /** \action
    * Call function
    **/
   bool f_poly_fit_ok = F360_Fit_Second_Degree_Polynomial(A, B, a, b, c);

   /** \result
    * Verify that function have returned false
    **/
   CHECK_FALSE(f_poly_fit_ok);
}

/**
 *\purpose  Verify polynomial fit returns true and correct coefficients are calculated
 *          when a-coefficient should be zero
 *\req    NA
 */
TEST(f360_LinearSolvers, F360_Fit_Second_Degree_Polynomial_Zero_A_Coeff)
{
   /** \precond
    * Declare matrix A and B
    * Declare test pass threshold
    **/
   F360_matrix_real32_LSC_T A;
   A.m_size[0] = 3;
   A.m_size[1] = 3;
   A.numDimensions = 3;
   A.data[0][0] = 4.0F;
   A.data[0][1] = -2.0F;
   A.data[0][2] = 1.0F;
   A.data[1][0] = 0.0F;
   A.data[1][1] = 0.0F;
   A.data[1][2] = 1.0F;
   A.data[2][0] = 4.0F;
   A.data[2][1] = 2.0F;
   A.data[2][2] = 1.0F;

   F360_vector_real32_LSC_T B;
   B.m_size = 3;
   B.numDimensions = 1;
   B.data[0] = 1.9F;
   B.data[1] = 2.0F;
   B.data[2] = 2.1F;

   float32_t test_pass_thres = 1.0e-4F;

   float32_t a;
   float32_t b;
   float32_t c;

   /** \action
    * Call function
    **/
   bool f_poly_fit_ok = F360_Fit_Second_Degree_Polynomial(A, B, a, b, c);

   /** \result
    * Verify that function have returned true and correct coefficients
    **/
   CHECK_TRUE(f_poly_fit_ok);
   DOUBLES_EQUAL(0.0F, a, test_pass_thres);
   DOUBLES_EQUAL(0.05F, b, test_pass_thres);
   DOUBLES_EQUAL(2.0F, c, test_pass_thres);
}

/**
 *\purpose  Verify polynomial fit returns true and correct coefficients are calculated
 *          when no coefficients are expected to be zero.
 *\req    NA
 */
TEST(f360_LinearSolvers, F360_Fit_Second_Degree_Polynomial_Non_Zero_Coeffs)
{
   /** \precond
    * Declare matrix A and B
    * Declare test pass threshold
    **/
   F360_matrix_real32_LSC_T A;
   A.m_size[0] = 3;
   A.m_size[1] = 3;
   A.numDimensions = 3;
   A.data[0][0] = 4.0F;
   A.data[0][1] = -2.0F;
   A.data[0][2] = 1.0F;
   A.data[1][0] = 0.0F;
   A.data[1][1] = 0.0F;
   A.data[1][2] = 1.0F;
   A.data[2][0] = 4.0F;
   A.data[2][1] = 2.0F;
   A.data[2][2] = 1.0F;

   F360_vector_real32_LSC_T B;
   B.m_size = 3;
   B.numDimensions = 1;
   B.data[0] = 1.9F;
   B.data[1] = 2.0F;
   B.data[2] = 2.4F;

   float32_t test_pass_thres = 1.0e-4F;

   float32_t a;
   float32_t b;
   float32_t c;

   /** \action
    * Call function
    **/
   bool f_poly_fit_ok = F360_Fit_Second_Degree_Polynomial(A, B, a, b, c);

   /** \result
    * Verify that function have returned true and correct coefficients
    **/
   CHECK_TRUE(f_poly_fit_ok);
   DOUBLES_EQUAL(0.0375F, a, test_pass_thres);
   DOUBLES_EQUAL(0.125F, b, test_pass_thres);
   DOUBLES_EQUAL(2.0F, c, test_pass_thres);
}

/**
 *\purpose  Verify polynomial fit returns false when there exists no
 *          solution to the given A matrix.
 *\req    NA
 */
TEST(f360_LinearSolvers, F360_Fit_Second_Degree_Polynomial_No_Valid_Solution)
{
   /** \precond
    * Declare matrix A and B
    **/
   F360_matrix_real32_LSC_T A;
   A.m_size[0] = 3;
   A.m_size[1] = 3;
   A.numDimensions = 3;
   A.data[0][0] = 0.0F;
   A.data[0][1] = 0.0F;
   A.data[0][2] = 0.0F;
   A.data[1][0] = 0.0F;
   A.data[1][1] = 0.0F;
   A.data[1][2] = 0.0F;
   A.data[2][0] = 0.0F;
   A.data[2][1] = 0.0F;
   A.data[2][2] = 0.0F;

   F360_vector_real32_LSC_T B;
   B.m_size = 3;
   B.numDimensions = 1;
   B.data[0] = 1.9F;
   B.data[1] = 2.0F;
   B.data[2] = 2.4F;

   float32_t a;
   float32_t b;
   float32_t c;

   /** \action
    * Call function
    **/
   bool f_poly_fit_ok = F360_Fit_Second_Degree_Polynomial(A, B, a, b, c);

   /** \result
    * Verify that function have returned false
    **/
   CHECK_FALSE(f_poly_fit_ok);

}

/** \defgroup  f360_LinearSolvers_MatrixDivision
*  @{
*/

/** \brief
 *  Test the validity of the linear solver.
 *  The linear solver is essentially an algorithm to invert a matrix.
 *  The condition for the algorithm is that the matrix is positive-definite and symmetric.
 **/
TEST_GROUP(f360_LinearSolvers_MatrixDivision)
{
};

/**
 *\purpose  Invert a symmetric positive-definite matrix
 *\req  NA
 */
TEST(f360_LinearSolvers_MatrixDivision, MatrixInversion)
{
   /** \step{1}
    * Invert a symmetric positive-definite matrix
    **/

   /** \precond
    * Declare matrices and fill values.
    **/

   float32_t A[3][3] = {{1.5993F, 0.7491F, 0.8227F}, {0.7491F, 0.9506F, 0.1242F}, {0.8227F, 0.1242F, 0.6190F}};
   float32_t eye[3][3] = {{1.0F, 0.0F, 0.0F}, {0.0F, 1.0F, 0.0F}, {0.0F, 0.0F, 1.0F}};

   /** \action
    * Call linear solver
    **/
   float32_t A_inv[3][3] = {};
   Matrix_Division(A, eye, 3U, 3U, A_inv);

   /** \result
    * Check if the output is valid
    **/
   const float32_t expected_A_inv[3][3] = {{7.2784F, -4.5921F, -8.7522F}, {-4.5921F, 3.9775F,  5.3052F}, {-8.7522F, 5.3052F, 12.1834F}};
   for (int32_t i = 0; i < 3; i++)
   {
      for (int32_t j = 0; j < 3; j++)
      {
         DOUBLES_EQUAL(expected_A_inv[i][j], A_inv[i][j], 0.0001F);
      }
   }
}

/**
 *\purpose  Invert a symmetric positive-definite matrix when A and B matrices are not fully utilized (i.e. there are non-used elements in them)
 *\req  NA
 */
TEST(f360_LinearSolvers_MatrixDivision, MatrixInversion_matrices_not_fully_utilized)
{
   /** \step{1}
    * Invert a symmetric positive-definite matrix
    **/

   /** \precond
    * Declare matrices and fill values. A and B matrices are of size 5x5 but are only filled with 3x3 matrices, remaning elements are unused.
    **/
   float32_t A[5][5] = {{1.5993F, 0.7491F, 0.8227F, 0.0F, 0.0F}, {0.7491F, 0.9506F, 0.1242F, 0.0F, 0.0F}, {0.8227F, 0.1242F, 0.6190F, 0.0F, 0.0F}, {0.0F, 0.0F, 0.0F, 0.0F, 0.0F}, {0.0F, 0.0F, 0.0F, 0.0F, 0.0F}};
   float32_t eye[5][5] = {{1.0F, 0.0F, 0.0F, 0.0F, 0.0F}, {0.0F, 1.0F, 0.0F, 0.0F, 0.0F}, {0.0F, 0.0F, 1.0F, 0.0F, 0.0F}, {0.0F, 0.0F, 0.0F, 0.0F, 0.0F}, {0.0F, 0.0F, 0.0F, 0.0F, 0.0F}};

   /** \action
    * Call linear solver
    **/
   float32_t A_inv[5][5] = {};
   Matrix_Division(A, eye, 3U, 3U, A_inv);

   /** \result
    * Check if the output is valid
    **/
   const float32_t expected_A_inv[3][3] = {{7.2784F, -4.5921F, -8.7522F}, {-4.5921F, 3.9775F,  5.3052F}, {-8.7522F, 5.3052F, 12.1834F}};
   for (int32_t i = 0; i < 3; i++)
   {
      for (int32_t j = 0; j < 3; j++)
      {
         DOUBLES_EQUAL(expected_A_inv[i][j], A_inv[i][j], 0.0001F);
      }
   }
}


/**
 *\purpose  Perform matrix division
 *\req  NA
 */
TEST(f360_LinearSolvers_MatrixDivision, MatrixDivision)
{
   /** \step{1}
    * Perform matrix division using a symmetric positive-definite matrix which is diagonally dominant
    **/

   /** \precond
    * Declare matrices and fill values.
    **/
   float32_t A[7][7] = {{7.0823F,    0.9149F,    2.3631F,    2.0480F,    1.9661F,    1.0920F,    1.3417F},
         {0.9149F,    6.1159F,    1.2744F,    1.3463F,    1.6118F,    1.1559F,    0.9603F},
         {2.3631F,    1.2744F,    7.9816F,    2.5052F,    2.5874F,    1.5628F,    1.8560F},
         {2.0480F,    1.3463F,    2.5052F,    7.6023F,    2.6690F,    1.4709F,    1.5976F},
         {1.9661F,    1.6118F,    2.5874F,    2.6690F,    8.0359F,    1.9130F,    1.9370F},
         {1.0920F,    1.1559F,    1.5628F,    1.4709F,    1.9130F,    6.6736F,    1.3726F},
         {1.3417F,    0.9603F,    1.8560F,    1.5976F,    1.9370F,    1.3726F,    6.6702F}};
   float32_t B[7][3] = {{0.5313F,    0.2665F,    0.5181F},
         {0.3251F,    0.1537F,    0.9436F},
         {0.1056F,    0.2810F,    0.6377F},
         {0.6110F,    0.4401F,    0.9577F},
         {0.7788F,    0.5271F,    0.2407F},
         {0.4235F,    0.4574F,    0.6761F},
         {0.0908F,    0.8754F,    0.2891F}};


   /** \action
    * Call linear solver
    **/
    float32_t matrix_div[3][7] = {};
    Matrix_Division(A, B, 7U, 3U, matrix_div);

   /** \result
    * Check if the output is valid
    **/
    float32_t expected_mat_div[3][7] = {{ 0.0519F,    0.0217F,   -0.0464F,    0.0501F,    0.0767F,    0.0349F,   -0.0285F},
         { 0.0018F,   -0.0095F,   -0.0138F,    0.0227F,    0.0275F,    0.0365F,    0.1151F},
         { 0.0266F,    0.1271F,    0.0293F,    0.0957F,   -0.0567F,    0.0650F,   -0.0083F}};
    for (int32_t i = 0; i < 3; i++)
    {
       for (int32_t j = 0; j < 7; j++)
       {
          DOUBLES_EQUAL(expected_mat_div[i][j], matrix_div[i][j], 0.0001F);
       }
    }
}

/**
 *\purpose  Perform matrix division when A and B matrices are not fully utilized (i.e. there are non-used elements in them)
 *\req  NA
 */
TEST(f360_LinearSolvers_MatrixDivision, MatrixDivision_matrices_not_fully_utilized)
{
   /** \step{1}
    * Perform matrix division using a symmetric positive-definite matrix which is diagonally dominant. A and B matrices 
	* are of size 10x10 and 10x5 respectively but are only filled with 7x7 and 7x3 matrices, remaning elements are unused
    **/

   /** \precond
    * Declare matrices and fill values.
    **/
   float32_t A[10][10] = {{7.0823F,    0.9149F,    2.3631F,    2.0480F,    1.9661F,    1.0920F,    1.3417F,    0.0000F,    0.0000F,    0.0000F},
         {0.9149F,    6.1159F,    1.2744F,    1.3463F,    1.6118F,    1.1559F,    0.9603F,    0.0000F,    0.0000F,    0.0000F},
         {2.3631F,    1.2744F,    7.9816F,    2.5052F,    2.5874F,    1.5628F,    1.8560F,    0.0000F,    0.0000F,    0.0000F},
         {2.0480F,    1.3463F,    2.5052F,    7.6023F,    2.6690F,    1.4709F,    1.5976F,    0.0000F,    0.0000F,    0.0000F},
         {1.9661F,    1.6118F,    2.5874F,    2.6690F,    8.0359F,    1.9130F,    1.9370F,    0.0000F,    0.0000F,    0.0000F},
         {1.0920F,    1.1559F,    1.5628F,    1.4709F,    1.9130F,    6.6736F,    1.3726F,    0.0000F,    0.0000F,    0.0000F},
         {1.3417F,    0.9603F,    1.8560F,    1.5976F,    1.9370F,    1.3726F,    6.6702F,    0.0000F,    0.0000F,    0.0000F},
	     {0.0000F,    0.0000F,    0.0000F,    0.0000F,    0.0000F,    0.0000F,    0.0000F,    0.0000F,    0.0000F,    0.0000F},
		 {0.0000F,    0.0000F,    0.0000F,    0.0000F,    0.0000F,    0.0000F,    0.0000F,    0.0000F,    0.0000F,    0.0000F},
         {0.0000F,    0.0000F,    0.0000F,    0.0000F,    0.0000F,    0.0000F,    0.0000F,    0.0000F,    0.0000F,    0.0000F}};
   float32_t B[10][5] = {{0.5313F,    0.2665F,    0.5181F,    0.0000F,    0.0000F},
         {0.3251F,    0.1537F,    0.9436F,    0.0000F,    0.0000F},
         {0.1056F,    0.2810F,    0.6377F,    0.0000F,    0.0000F},
         {0.6110F,    0.4401F,    0.9577F,    0.0000F,    0.0000F},
         {0.7788F,    0.5271F,    0.2407F,    0.0000F,    0.0000F},
         {0.4235F,    0.4574F,    0.6761F,    0.0000F,    0.0000F},
         {0.0908F,    0.8754F,    0.2891F,    0.0000F,    0.0000F},
	     {0.0000F,    0.0000F,    0.0000F,    0.0000F,    0.0000F},
		 {0.0000F,    0.0000F,    0.0000F,    0.0000F,    0.0000F},
		 {0.0000F,    0.0000F,    0.0000F,    0.0000F,    0.0000F}};


   /** \action
    * Call linear solver
    **/
    float32_t matrix_div[5][10] = {};
    Matrix_Division(A, B, 7U, 3U, matrix_div);

   /** \result
    * Check if the output is valid
    **/
    float32_t expected_mat_div[3][7] = {{ 0.0519F,    0.0217F,   -0.0464F,    0.0501F,    0.0767F,    0.0349F,   -0.0285F},
         { 0.0018F,   -0.0095F,   -0.0138F,    0.0227F,    0.0275F,    0.0365F,    0.1151F},
         { 0.0266F,    0.1271F,    0.0293F,    0.0957F,   -0.0567F,    0.0650F,   -0.0083F}};
    for (int32_t i = 0; i < 3; i++)
    {
       for (int32_t j = 0; j < 7; j++)
       {
          DOUBLES_EQUAL(expected_mat_div[i][j], matrix_div[i][j], 0.0001F);
       }
    }
}
/** @}*/
