#ifndef F360_MATRIX_VECTOR_INIT_REAL32_T_H
#define F360_MATRIX_VECTOR_INIT_REAL32_T_H

#include"f360_linear_solver_data_types.h"

namespace f360_variant_A
{
   void F360_matrix_Init_real32_T(
      F360_matrix_real32_LSC_T & b_X_data,
      const int num_rows,
      const int num_cols);

   void F360_matrix_Copy_real32_T(
      F360_matrix_real32_LSC_T & X,
      const F360_matrix_real32_LSC_T & Y);

   void qr_modified_gram_schimdt(
      F360_matrix_real32_LSC_T& Q,
      F360_matrix_real32_LSC_T& R,
      const F360_matrix_real32_LSC_T& A);

   void my_rdivide(
      F360_matrix_real32_LSC_T& Q,
      const F360_matrix_real32_LSC_T& VM,
      const F360_matrix_real32_LSC_T& R,
      const int num_rows,
      const int col);

   float euclid_norm(
      const F360_matrix_real32_LSC_T& VM,
      const int num_rows,
      const int col);

   void scalarProduct(
      F360_matrix_real32_LSC_T& R,
      const F360_matrix_real32_LSC_T& Q,
      const F360_matrix_real32_LSC_T& VM,
      const int col1,
      const int col2,
      const int num_rows);

   void updateVM(
      F360_matrix_real32_LSC_T& VM,
      const F360_matrix_real32_LSC_T& Q,
      const F360_matrix_real32_LSC_T& R,
      const int row,
      const int col,
      const int num_rows);

   bool F360_Fit_Second_Degree_Polynomial(
      const F360_matrix_real32_LSC_T& A,
      const F360_vector_real32_LSC_T& B,
      float32_t& p2,
      float32_t& p1,
      float32_t& p0);
}
#endif
