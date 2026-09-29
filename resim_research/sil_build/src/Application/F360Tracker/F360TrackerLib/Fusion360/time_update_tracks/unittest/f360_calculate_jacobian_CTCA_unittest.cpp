/** \file
   File with unit tests for calculation of CTCA model jacobian.
*/

#include "f360_calculate_jacobian_CTCA.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>
#include <cmath>

/** \defgroup  f360_calculate_jacobian_CTCA
 *  @{
 */

/** \brief
 *  Add brief description of test group
 */
using namespace f360_variant_A;
TEST_GROUP(f360_calculate_jacobian_CTCA)
{
   /** \setup
   * Nothing to setup in this test group
   */
   TEST_SETUP()
   {
   }

   /** \teardown
    * Nothing to teardown in this test group
    */
   TEST_TEARDOWN()
   {
      //mock.clear();
   }

};

/**
*\purpose  Check calculation correctnes
*\req    NA
*/
TEST(f360_calculate_jacobian_CTCA, Calc_1__tcs_vec_from_center_rear_to_ref_pnt_Is_Zero)
{
    // Data is generated from MATLAB

    /** \precond */
    float32_t psi = 0.2511f;
    float32_t c = 0.616f;
    float32_t v = 0.4733f;
    float32_t a = 0.3517f;
    float32_t tcs_vec_from_center_rear_to_ref_pnt[2] = {0.0F, 0.0F};

    float32_t T = 0.05f;

    /** \action */
    float32_t output[6][6] = { 0.0F };
    F360_Calculate_Jacobian_CTCA(cosf(psi), sinf(psi), v, c, a, T, tcs_vec_from_center_rear_to_ref_pnt, output);

    /** \result */
    float32_t expected_output[6][6] = {
         {1.0f, 0.0f, -0.0062f, -0.0001f, 0.0482f, 0.0012f},
         {0.0f, 1.0f,  0.0233f,  0.0003f, 0.0131f, 0.0003f},
         {0.0f, 0.0f,  1.0f,     0.0241f, 0.0308f, 0.0008f},
         {0.0f, 0.0f,  0.0f,     1.0f,    0.0f,    0.0f  },
         {0.0f, 0.0f,  0.0f,     0.0f,    1.0f,    0.05f },
         {0.0f, 0.0f,  0.0f,     0.0f,    0.0f,    1.0f  },
    };

    for (int row = 0; row < 6; row++)
    {
        for (int col = 0; col < 6; col++)
        {
            DOUBLES_EQUAL(expected_output[row][col], output[row][col], 1e-4);
        }
    }
}

/**
*\purpose  Check calculation correctnes. Same example as Calc_1 test but now the input tcs_vec_from_center_rear_to_ref_pnt is non-zero
*\req    NA
*/
TEST(f360_calculate_jacobian_CTCA, Calc_1__tcs_vec_from_center_rear_to_ref_pnt_Is_Nonzero)
{
    // Data is generated from MATLAB

    /** \precond */
    float32_t psi = 0.2511f;
    float32_t c = 0.616f;
    float32_t v = 0.4733f;
    float32_t a = 0.3517f;
    float32_t tcs_vec_from_center_rear_to_ref_pnt[2] = {15.0F, 1.5F};

    float32_t T = 0.05f;

    /** \action */
    float32_t output[6][6] = { 0.0F };
    F360_Calculate_Jacobian_CTCA(cosf(psi), sinf(psi), v, c, a, T, tcs_vec_from_center_rear_to_ref_pnt, output);

    /** \result */
    float32_t expected_output[6][6] = {
         {1.0f, 0.0f, -0.2157f, -0.1300f, -0.1178f, -0.0029f},
         {0.0f, 1.0f, -0.0552f,  0.3397f, 0.4468f, 0.0112f},
         {0.0f, 0.0f,  1.0f,     0.0241f, 0.0308f, 0.0008f},
         {0.0f, 0.0f,  0.0f,     1.0f,    0.0f,    0.0f  },
         {0.0f, 0.0f,  0.0f,     0.0f,    1.0f,    0.05f },
         {0.0f, 0.0f,  0.0f,     0.0f,    0.0f,    1.0f  },
    };

    for (int row = 0; row < 6; row++)
    {
        for (int col = 0; col < 6; col++)
        {
            DOUBLES_EQUAL(expected_output[row][col], output[row][col], 1e-4);
        }
    }
}

/**
*\purpose  Check calculation correctnes
*\req    NA
*/
TEST(f360_calculate_jacobian_CTCA, Calc_2__tcs_vec_from_center_rear_to_ref_pnt_Is_Zero)
{
    // Data is generated from MATLAB

    /** \precond */
    float32_t psi = 5.6782f;
    float32_t c = 0.7585f;
    float32_t v = 0.5395f;
    float32_t a = 5.3080f;
    float32_t tcs_vec_from_center_rear_to_ref_pnt[2] = {0.0F, 0.0F};

    float32_t T = 0.05f;

    /** \action */
    float32_t output[6][6] = { 0.0F };
    F360_Calculate_Jacobian_CTCA(cosf(psi), sinf(psi), v, c, a, T, tcs_vec_from_center_rear_to_ref_pnt, output);

    /** \result */
    float32_t expected_output[6][6] = {
         {1.0f, 0.0f,  0.0188,  0.0003,  0.0419,  0.0010},
         {0.0f, 1.0f,  0.0279,  0.0005, -0.0274, -0.0007},
         {0.0f, 0.0f,  1.0f,    0.0336,  0.0379,  0.0009},
         {0.0f, 0.0f,  0.0f,    1.0f,    0.0f,   0.0f  },
         {0.0f, 0.0f,  0.0f,    0.0f,    1.0f,   0.05f },
         {0.0f, 0.0f,  0.0f,    0.0f,    0.0f,   1.0f  },
    };

    for (int row = 0; row < 6; row++)
    {
        for (int col = 0; col < 6; col++)
        {
            DOUBLES_EQUAL(expected_output[row][col], output[row][col], 1e-4);
        }
    }
}

/**
*\purpose  Check calculation correctnes. Same example as Calc_2 test but now the input tcs_vec_from_center_rear_to_ref_pnt is non-zero
*\req    NA
*/
TEST(f360_calculate_jacobian_CTCA, Calc_2_tcs_vec_from_center_rear_to_ref_pnt_Is_Nonzero)
{
    // Data is generated from MATLAB

    /** \precond */
    float32_t psi = 5.6782f;
    float32_t c = 0.7585f;
    float32_t v = 0.5395f;
    float32_t a = 5.3080f;
    float32_t tcs_vec_from_center_rear_to_ref_pnt[2] = {20.0F, -1.5F};

    float32_t T = 0.05f;

    /** \action */
    float32_t output[6][6] = { 0.0F };
    F360_Calculate_Jacobian_CTCA(cosf(psi), sinf(psi), v, c, a, T, tcs_vec_from_center_rear_to_ref_pnt, output);

    /** \result */
    float32_t expected_output[6][6] = {
         {1.0f, 0.0f, -0.3830f,  0.4107f,  0.5049f,  0.0126f},
         {0.0f, 1.0f,  0.3443f,  0.5355f, 0.5763f,  0.0144f},
         {0.0f, 0.0f,  1.0f,    0.0336f,  0.0379f,  0.0009f},
         {0.0f, 0.0f,  0.0f,    1.0f,    0.0f,   0.0f  },
         {0.0f, 0.0f,  0.0f,    0.0f,    1.0f,   0.05f },
         {0.0f, 0.0f,  0.0f,    0.0f,    0.0f,   1.0f  },
    };

    for (int row = 0; row < 6; row++)
    {
        for (int col = 0; col < 6; col++)
        {
            DOUBLES_EQUAL(expected_output[row][col], output[row][col], 1e-4);
        }
    }
}
/** @}*/
