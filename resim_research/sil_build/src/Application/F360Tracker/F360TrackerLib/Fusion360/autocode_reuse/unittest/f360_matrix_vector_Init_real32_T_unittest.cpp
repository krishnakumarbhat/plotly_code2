/** \file
 * this files provides test to evaluate the matrix vector unit
 */

#include "f360_matrix_vector_Init_real32_T.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

 /** \defgroup  f360_matrix_vector_Init_real32_T
 *  @{
 */

using namespace f360_variant_A;
/** \brief
 * Test the validity of the matrix vector init.
 * The matrix vector validity initializes the given vector/matrix,
 * with the given parameters row and column numbers.
 * The functions do not check if the maximal allowed sized is exeeded.
 **/
TEST_GROUP(f360_matrix_vector_Init_real32_T)
{
};

/**
*\purpose  Evaluate the function to initialize a matrix of type floating point
*\req    put in requirement tag if any otherwise set to NA
*/
TEST(f360_matrix_vector_Init_real32_T, TestMatrixInitReal32)
{
    /** \precond
     * Resize the the given matrix and initialize it's value to zero
     * New size shall be:
     *   - rows = 6
     *   - columns 1
     * The initial matrix has a size
     *   - rows = 9
     *   - col = 2
     * and is initialized with 1.0F
     **/
    F360_matrix_real32_LSC_T mat;
    mat.m_size[0] = 9;
    mat.m_size[1] = 1;
    for (size_t n = 0; n < 9U; n++)
    {
        for (size_t m = 0; m < 1U; m++)
        {
            mat.data[n][m] = 1.0F;
        }
    }

    /** \action
     * Call init function
     **/
    F360_matrix_Init_real32_T(mat, 6, 2);

    /** \result
     * Check if the output is valid
     **/
    CHECK_EQUAL(6, mat.m_size[0]);
    CHECK_EQUAL(2, mat.m_size[1]);
    for (size_t n = 0; n < 9U; n++)
    {
        for (size_t m = 0; m < 2U; m++)
        {
            DOUBLES_EQUAL(0.0F, mat.data[n][m], 0.001F);
        }
    }
}

/**
*\purpose  Evaluate the function to copy the elements of a matrix of type
* floating point to another matrix of type floating point
*\req    put in requirement tag if any otherwise set to NA
*/
TEST(f360_matrix_vector_Init_real32_T, TestMatrixCopyReal32)
{
     /** \precond
      * Copy element from matrix A to Matrix B of the same size
      * the matrix has a size of rows=10, cols=2
      **/
    F360_matrix_real32_LSC_T matA;
    F360_matrix_real32_LSC_T matB;
    matA.m_size[0] = 10;
    matA.m_size[1] = 2;
    matB.m_size[0] = 10;
    matB.m_size[1] = 2;
    for (size_t n = 0U; n < 10U; n++)
    {
        for (size_t m = 0U; m < 2U; m++)
        {
            matA.data[n][m] = static_cast<float32_t>(n*m);
            matB.data[n][m] = 0.F;
        }
    }

    /** \action
     * Call init function
     **/
    F360_matrix_Copy_real32_T(matB, matA);

    /** \result
     * Check if the output is valid
     **/
    CHECK_EQUAL(10, matA.m_size[0]);
    CHECK_EQUAL(2, matA.m_size[1]);
    CHECK_EQUAL(10, matB.m_size[0]);
    CHECK_EQUAL(2, matB.m_size[1]);
    for (size_t n = 0U; n < 10U; n++)
    {
        for (size_t m = 0U; m < 2U; m++)
        {
            DOUBLES_EQUAL(matA.data[n][m], matB.data[n][m], 0.001F);
        }
    }
}
/** @}*/
