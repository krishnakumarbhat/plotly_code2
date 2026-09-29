#ifndef ML_MATRIX_TOOLS_HPP
#define ML_MATRIX_TOOLS_HPP
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
extern "C"
{
#include "reuse.h"
}
#include <algorithm>
#include "ml_matrix_2x2_t.h"
#include "ml_matrix_3x3_t.h"
namespace ml {
   /**
   *\return true if inversion of a matrix with given determinant is possible
   */
   inline bool Matrix_Inversion_Possible(
      const float32_T  det /**< [in] determinant of a matrix */
   );

   /**
    * Create a sub matrix by deleting given row and column from given matrix
    * [Submatrix](https://en.wikipedia.org/wiki/Matrix_(mathematics)#Submatrix)
    * \return The resulting submatrix
    */
   inline Matrix_2X2_T Submatrix(
      const Matrix_3X3_T &mat, /**< Matrix to derive the sub matrix from */
      const size_t row,       /**< Row to be deleted */
      const size_t column     /**< Column to be deleted */
   );

   inline bool Matrix_Inversion_Possible(
      const float32_T  det
   )
   {
      float32_T threshold_is_zero = ((float32_T)1E-10);
      return threshold_is_zero < std::abs(det);
   }

   inline Matrix_2X2_T Submatrix(
      const Matrix_3X3_T &mat,
      const size_t row,
      const size_t column
      )
   {
      Matrix_2X2_T ret;
      size_t row_index_out = 0;
      for(size_t row_index=0; row_index<3; row_index++)
      {
         if(row_index != row)
         {
            size_t column_index_out = 0;
            for(size_t column_index=0; column_index<3; column_index++)
            {
               if(column_index != column)
               {
                  ret.elements[row_index_out][column_index_out] = mat.elements[row_index][column_index];
                  column_index_out++;
               }
            }
            row_index_out++;
         }
      }
      return ret;
   }
}

#endif
