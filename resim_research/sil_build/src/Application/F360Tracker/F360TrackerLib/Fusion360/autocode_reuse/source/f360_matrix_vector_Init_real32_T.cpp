#include <algorithm>

#include "f360_math.h"
#include"f360_iterator.h"
#include <cstring>
#include"f360_linear_solver_data_types.h"
#include"f360_matrix_vector_Init_real32_T.h"

namespace f360_variant_A
{
   /*********************************************************
   // name: F360_matrix_Init_real32_T
   // what: To initialize all the elements in a matrix to zeros
   // in : Reference to Matrix, number of rows, number of cols
   // out: None
   **********************************************************/
   void F360_matrix_Init_real32_T(F360_matrix_real32_LSC_T & b_X_data, const int num_rows, const int num_cols)
   {
      b_X_data.m_size[0] = num_rows; //initialize size with the number of rows and cols passed in the function 
      b_X_data.m_size[1] = num_cols;

      b_X_data.numDimensions = num_cols;

      std::fill(cmn::begin(b_X_data.data), cmn::end(b_X_data.data), 0.0F);
   }

   /*********************************************************
   // name: F360_matrix_Copy_real32_T
   // what: To copy all the elements from one matrix to another
   // in : References to two Matrices
   // out: None
   **********************************************************/
   void F360_matrix_Copy_real32_T(
      F360_matrix_real32_LSC_T & X,
      const F360_matrix_real32_LSC_T & Y)
   {
      const int m = Y.m_size[0];
      const int n = Y.m_size[1];

      for (int row = 0; row < m; row++)
      {
         for (int col = 0; col < n; col++)
         {
            X.data[row][col] = Y.data[row][col];
         }
      }
   }
}
