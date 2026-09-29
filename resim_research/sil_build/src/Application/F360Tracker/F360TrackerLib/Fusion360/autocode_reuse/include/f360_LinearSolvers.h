#ifndef F360_LINEARSOLVERS_H
#define F360_LINEARSOLVERS_H

#include "f360_reuse.h"
#include <cstddef>

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Forward_Substitution()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const T (&U)[MAX_NUM_OF_MSMT][MAX_NUM_STATES] - matrix, the result after performing a prior back substition step of the linear system of equations y*(L*D*L') = B'
     const T (&L)[MAX_NUM_OF_MSMT][MAX_NUM_OF_MSMT] - The L matrix in the LDL decompsition of A = L*D*L'
     const uint32_t num_measurements - leading size of L
     const uint32_t num_states - number of rows of the final result matrix X
     T (&X)[MAX_NUM_STATES][MAX_NUM_OF_MSMT] - the solution to the problem y*(L*D*L') = B'
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function performs the forward substitution step in order to solve the following system of linear equations
   * y*(L*D*L') = B'
   *
   * PRECONDITIONS:
   * L and d corresponds to an LDL decomposition of a  hermitian positive-definite matrix A = L*D*L'
   * A backwards substitution step has been run beforehand to generate the matrix U
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   template<std::size_t MAX_NUM_OF_MSMT, std::size_t MAX_NUM_STATES>
   void Forward_Substitution(
      const float32_t (&U)[MAX_NUM_OF_MSMT][MAX_NUM_STATES],
      const float32_t (&L)[MAX_NUM_OF_MSMT][MAX_NUM_OF_MSMT],
      const uint32_t num_measurements,
      const uint32_t num_states,
      float32_t (&X)[MAX_NUM_STATES][MAX_NUM_OF_MSMT])
   {
      const int32_t signed_num_measurements = static_cast<int32_t>(num_measurements);

      for (int32_t i = signed_num_measurements - 1; i >= 0; --i)
      {
         for (uint32_t j = 0U; j < num_states; ++j)
         {
            X[j][i] = U[i][j];
         }
         for (int32_t j = (i + 1); j < signed_num_measurements; ++j)
         {
            for (uint32_t k = 0U; k < num_states; ++k)
            {
               X[k][i] = X[k][i] - (L[j][i] * X[k][j]);
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Solve_Linear_System_Of_Equations()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const T (&d)[MAX_NUM_OF_MSMT] - array containing diaginal elements of the diagonal elements D in an LDL composition A = L*D*L'
   * const T (&L)[MAX_NUM_OF_MSMT][MAX_NUM_OF_MSMT] - the L matrix in an LDL decomposition A * L*D*L'
   * const T (&B)[MAX_NUM_OF_MSMT][MAX_NUM_STATES] - the matrix B in the problem y*(L*D*L') = B'
   * const uint32_t num_measurements - leading size of L and D
   * const uint32_t num_states - number of active columns of B
   * T (&X)[MAX_NUM_STATES][MAX_NUM_OF_MSMT] - matrix, the solution to y*(L*D*L') = B'
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function solves the following system of linear equations
   * y*(L*D*L') = B'
   *
   * PRECONDITIONS:
   * L and d corresponds to an LDL decomposition of a hermitian positive-definite matrix A = L*D*L'
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   template<std::size_t MAX_NUM_OF_MSMT, std::size_t MAX_NUM_STATES>
   void Solve_Linear_System_Of_Equations(
      const float32_t (&d)[MAX_NUM_OF_MSMT],
      const float32_t (&L)[MAX_NUM_OF_MSMT][MAX_NUM_OF_MSMT],
      const float32_t(&B)[MAX_NUM_OF_MSMT][MAX_NUM_STATES],
      const uint32_t num_measurements,
      const uint32_t num_states,
      float32_t(&X)[MAX_NUM_STATES][MAX_NUM_OF_MSMT])
   {
      // Start with back substitution
      float32_t V[MAX_NUM_OF_MSMT][MAX_NUM_STATES];

      for (uint32_t i = 0U; i < num_states; ++i)
      {
         V[0][i] = B[0][i];
      }

      for (uint32_t i = 1U; i < num_measurements; ++i)
      {
         for (uint32_t k = 0U; k < num_states; k++)
         {
            V[i][k] = B[i][k];
         }
         for (uint32_t indx = 0U; indx < i; ++indx)
         {
            for (uint32_t k = 0U; k < num_states; ++k)
            {
               V[i][k] = V[i][k] - (L[i][indx] * V[indx][k]);

            }
         }
      }

      for (uint32_t i = 0U; i < num_measurements; ++i)
      {
         const float32_t one_over_d_i = 1.0F / d[i];
         for (uint32_t j = 0U; j < num_states; ++j)
         {
            V[i][j] = V[i][j] * one_over_d_i;
         }
      }

      // Continue with forward substitiution
      Forward_Substitution(V, L, num_measurements, num_states, X);
   }

   /*===========================================================================*\
   * FUNCTION: LDL_Decomposition()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const T (&A)[MAX_NUM_OF_MSMT][MAX_NUM_OF_MSMT] - the matrix to decompose
   * const uint32_t leading_size - leading size of A
   * T (&d)[MAX_NUM_OF_MSMT] - array containing the diagonal elements of the decomposed D matrix
   * T (&L)[MAX_NUM_OF_MSMT][MAX_NUM_OF_MSMT] - the decomposed L matrix
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function computes the LDL decomposition of A:
   * A = L*D*L'
   * where L is a lower unit triangular matrix and D is a diagonal matrix
   *
   * PRECONDITIONS:
   * A is a hermitian positive-definite matrix
   *
   * POSTCONDITIONS:
   * None
   *
  \*===========================================================================*/
   template<std::size_t MAX_NUM_OF_MSMT>
   void Ldl_Decomposition(
      const float32_t(&A)[MAX_NUM_OF_MSMT][MAX_NUM_OF_MSMT],
      const uint32_t leading_size,
      float32_t(&d)[MAX_NUM_OF_MSMT],
      float32_t(&L)[MAX_NUM_OF_MSMT][MAX_NUM_OF_MSMT])
   {
      if (leading_size <= MAX_NUM_OF_MSMT)
      {
         for (uint32_t j = 0U; j < leading_size; ++j)
         {
            float32_t v[MAX_NUM_OF_MSMT];
            if (j > 0U)
            {
               float32_t total = 0.0F;
               for (uint32_t i = 0U; i < j; ++i)
               {
                  v[i] = L[j][i] * d[i];
                  total += v[i] * L[j][i];
               }

               v[j] = A[j][j] - total;
               d[j] = v[j];
               const float32_t one_over_v_j = 1.0F / v[j];
               for (uint32_t i = j + 1U; i < leading_size; ++i)
               {
                  total = 0.0F;
                  for (uint32_t k = 0U; k < j; ++k)
                  {
                     total += L[i][k] * v[k];
                  }
                  L[i][j] = (A[i][j] - total) * one_over_v_j;
               }
            }
            else
            {
               v[0] = A[0][0];
               d[0] = v[0];
               const float32_t one_over_v_0 = 1.0F / v[0];
               for (uint32_t i = 1U; i < leading_size; ++i)
               {
                  L[i][0] = A[i][0] * one_over_v_0;
               }
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Matrix_Division()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const T (&A)[MAX_NUM_OF_MSMT][MAX_NUM_OF_MSMT] - matrix (to do inverse on)
   *  const T (&B)[MAX_NUM_OF_MSMT][MAX_NUM_STATES] - matrix
   *  const uint32_t num_measurements - number of measurements (equal to number of active rows and columns in A and number of active rows in B)
   *  const uint32_t num_states - number of states (equal to number of active columns in B)
   *  T (&y)[MAX_NUM_STATES][MAX_NUM_OF_MSMT] - matrix, the result after taking matrix division
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function computes the solution y to the problem y = B'*inv(A) in an efficent
   * way by utilizes LDL decomposition of A in the equivalent problem formulation y*A = B'.
   *
   * PRECONDITIONS:
   * A is a hermitian positive-definite matrix
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   template<std::size_t MAX_NUM_OF_MSMT, std::size_t MAX_NUM_STATES>
   void Matrix_Division(
      const float32_t(&A)[MAX_NUM_OF_MSMT][MAX_NUM_OF_MSMT],
      const float32_t(&B)[MAX_NUM_OF_MSMT][MAX_NUM_STATES],
      const uint32_t num_measurements,
      const uint32_t num_states,
      float32_t(&y)[MAX_NUM_STATES][MAX_NUM_OF_MSMT])
   {
      float32_t d[MAX_NUM_OF_MSMT] = {};
      float32_t L[MAX_NUM_OF_MSMT][MAX_NUM_OF_MSMT] = {};

      Ldl_Decomposition(A, num_measurements, d, L); //computes ldl decomposition

      Solve_Linear_System_Of_Equations(d, L, B, num_measurements, num_states, y);
      return;
   }
}
#endif
