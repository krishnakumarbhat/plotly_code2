#ifndef ML_MATRIX_H
#define ML_MATRIX_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "reuse.h"
#include "ml_matrix_2x2_t.h"
#include "ml_matrix_3x3_t.h"
#include "ml_vector_2d_t.h"

/** \file
 * Functions to do matrix operations.
 * \defgroup Matrix Matrix
 * Matrix types and operations. Currently only \f$ \Re^{2 * 2 } \f$ and \f$ \Re^{3 * 3 } \f$ matrices are supported.
 * See [Wikipedia: Matrix_(mathematics) => Basic operations](https:\\en.wikipedia.org/wiki/Matrix_(mathematics)#Basic_operations)
 */

/**
* Create an identity \f$ \Re^{2 * 2 } \f$ matrix. See [Wikipedia: Identity Matrix](https:\\en.wikipedia.org/wiki/Identity_matrix)
*
* \ingroup Matrix
* \sdd{WI-13902}
*/
void Matrix_2x2_Create_Identity_Matrix(
   Matrix_2X2_T       *res_mat /**< [out] generated identity matrix */
);

/**
* Create a zero \f$ \Re^{2 * 2 } \f$ matrix. See [Wikipedia: Zero Matrix](https:\\en.wikipedia.org/wiki/Zero_matrix)
*
* \ingroup Matrix
* \sdd{WI-13901}
*/
void Matrix_2x2_Create_Zero_Matrix(
   Matrix_2X2_T       *res_mat /**< [out] generated zero matrix */
);


/**
 * Multiplication of two \f$ \Re^{2 * 2 } \f$ matrices: \f[ res\_mat = mat\_A * mat\_B \f] See [Wikipedia: Matrix_multiplication](https:\\en.wikipedia.org/wiki/Matrix_multiplication)
 *
 * \ingroup Matrix
 * \sdd{WI-13900}
 */
void Matrix_2x2_Mul_Matrix_2x2(
   Matrix_2X2_T       *res_mat, /**< [out] result of A*B */
   const Matrix_2X2_T *mat_a,   /**< [in] input matrix A */
   const Matrix_2X2_T *mat_b /**< [in] input matrix B */
);

/**
* Addition of two \f$ \Re^{2 * 2 } \f$ matrices: \f[ res\_mat = mat\_A + mat\_B \f] See [Wikipedia: Matrix_addition](https:\\en.wikipedia.org/wiki/Matrix_addition)
*
* \ingroup Matrix
* \sdd{WI-13896}
*/
void Matrix_2x2_Add_Matrix_2x2(
   Matrix_2X2_T       *res_mat, /**< [out] result of A+B */
   const Matrix_2X2_T *mat_a,   /**< [in] input matrix A */
   const Matrix_2X2_T *mat_b /**< [in] input matrix B */
);

/**
* Multiplication of a \f$ \Re^{2 * 2 } \f$ matrix with a scalar: \f[ res\_mat = mat * scalar \f] See [Wikipedia: Scalar_multiplication](https:\\en.wikipedia.org/wiki/Scalar_multiplication)
*
* \ingroup Matrix
* \sdd{WI-13894}
*/
void Matrix_2x2_Mul_Scalar(
   Matrix_2X2_T       *res_mat, /**< [out] result of multiplication of matrix with scalar */
   const Matrix_2X2_T *mat,   /**< [in] input matrix */
   const float32_T scalar /**< [in] scalar */
);

/**
* Create an identity \f$ \Re^{3 * 3 } \f$ matrix. See [Wikipedia: Identity Matrix](https:\\en.wikipedia.org/wiki/Identity_matrix)
*
* \ingroup Matrix
* \sdd{WI-13909}
*/
void Matrix_3x3_Create_Identity_Matrix(
   Matrix_3X3_T       *res_mat /**< [out] generated identity matrix */
);

/**
* Create a zero \f$ \Re^{3 * 3 } \f$ matrix. See [Wikipedia: Zero Matrix](https:\\en.wikipedia.org/wiki/Zero_matrix)
*
* \ingroup Matrix
* \sdd{WI-13897}
*/
void Matrix_3x3_Create_Zero_Matrix(
   Matrix_3X3_T       *res_mat /**< [out] generated zero matrix */
);

/**
 * Multiplication of two \f$ \Re^{3 * 3 } \f$ matrices: \f[ res\_mat = mat\_A * mat\_B \f] See [Wikipedia: Matrix_multiplication](https:\\en.wikipedia.org/wiki/Matrix_multiplication)
 *
 * \ingroup Matrix
 * \sdd{WI-13899}
 */
void Matrix_3x3_Mul_Matrix_3x3(
   Matrix_3X3_T       *res_mat, /**< [out] Result of multiplication*/
   const Matrix_3X3_T *mat_a,   /**< [in] input matrix A */
   const Matrix_3X3_T *mat_b /**< [in] input matrix B */
);

/**
* Multiplication of two \f$ \Re^{3 * 3 } \f$ matrices: \f[ res\_mat = mat\_A + mat\_B \f] See [Wikipedia: Matrix_addition](https:\\en.wikipedia.org/wiki/Matrix_addition)
*
* \ingroup Matrix
* \sdd{WI-13893}
*/
void Matrix_3x3_Add_Matrix_3x3(
   Matrix_3X3_T       *res_mat, /**< [out] Result of A + B*/
   const Matrix_3X3_T *mat_a,   /**< [in] input matrix A */
   const Matrix_3X3_T *mat_b /**< [in] input matrix B */
);

/**
* Multiplication of a \f$ \Re^{3 * 3 } \f$ matrix with a scalar: \f[ res\_mat = mat * scalar \f] See [Wikipedia: Scalar_multiplication](https:\\en.wikipedia.org/wiki/Scalar_multiplication)
*
* \ingroup Matrix
* \sdd{WI-13895}
*/
void Matrix_3x3_Mul_Scalar(
   Matrix_3X3_T       *res_mat, /**< [out] result of multiplication of matrix with scalar */
   const Matrix_3X3_T *mat,   /**< [in] input matrix */
   const float32_T scalar /**< [in] scalar */
);

/**
 * Multiplication of the given \f$ \Re^{2 * 2 } \f$ matrix and the given vector: \f[ vector\_res = matrix * \vec{vector} \f] See [Wikipedia: Matrix_multiplication](https:\\en.wikipedia.org/wiki/Matrix_multiplication)
 *
 * \ingroup Matrix
 * \sdd{WI-13904}
 */
void Matrix_2x2_Mul_Vector_2d(
   Vector_2d_T        *vector_res, /**< [out] result of A*v */
   const Matrix_2X2_T *matrix,     /**< [in] input matrix */
   const Vector_2d_T  *vector /**< [in] input vector */
);

/**
 * Returns the determinant of the given \f$ \Re^{2 * 2 } \f$  matrix: \f[ det = \left | matrix \right | \f] See [Wikipedia: Determinant](https:\\en.wikipedia.org/wiki/Determinant)
 *
 * \ingroup Matrix
 * \sdd{WI-13898}
 */
float32_T Matrix_2x2_Determinant(
   const Matrix_2X2_T *matrix /**< [in] input matrix */
);

/**
 * Returns the determinant of a \f$ \Re^{3 * 3 } \f$ matrix: \f[ det = \left | matrix \right | \f] See [Wikipedia: Determinant](https:\\en.wikipedia.org/wiki/Determinant)
 * \return Determinant of given matrix
 * \ingroup Matrix
 * \sdd{WI-13905}
 */
float32_T Matrix_3x3_Determinant(
   const Matrix_3X3_T *matrix /**< [in] input matrix */
);

/**
 * Calculates the inverse of the given \f$ \Re^{2 * 2 } \f$ matrix when determinate is given: \f[ matrix^{-1} =
 * \left [ \begin{array}{cc}
 *   a & b \\ c & d \\
 * \end{array}\right ] ^{-1} =
 * \frac{1}{\det matrix} \left [ \begin{array}{cc}
 *  \,\,\,d & \!\!-b \\ -c & \,a \\
 *  \end{array} \right ] \f]
 * See [Wikipedia: Inversion_of_2_x_2_matrices](https:\\en.wikipedia.org/wiki/Invertible_matrix#Inversion_of_2_%C3%97_2_matrices)
 * \return boolean_T TRUE if inversion was possible. matrix will be unchanged if FALSE is returned
 *
 * \ingroup Matrix
 * \attention        This function should not be called with zero determinant.
 *                 If determinant is ~0, all the elements of inverse matrix will be INFINITY
 * \sdd{WI-13910}
 * \sdd{WI-13903}
 */
boolean_T Matrix_2x2_Inverse_Given_Determinant(
   Matrix_2X2_T    *matrix, /**< [in, out] input matrix */
   const float32_T  det /**< [in] determinant of given matrix */
);

/**
 * Computes the inverse of given \f$ \Re^{2 * 2 } \f$ matrix: \f[ matrix = matrix^{-1} \f]. See [Wikipedia: Invertible_matrix](https:\\en.wikipedia.org/wiki/Invertible_matrix)
 * \return boolean_T TRUE if inversion was possible. matrix will be unchanged if FALSE is returned. Inversion is
 * possible only if the matrix determinant is bigger than THRESHOLD_IS_ZERO.
 *
 * \ingroup Matrix
 * \sdd{WI-13912}
 * \sdd{WI-13903}
 */
boolean_T Matrix_2x2_Inverse(
   Matrix_2X2_T *matrix /**< [in, out] input matrix */
);

/**
 * Transposes the given \f$ \Re^{2 * 2 } \f$  matrix: \f[ matrix^T \f] See [Wikipedia: Transpose](https:\\en.wikipedia.org/wiki/Transpose)
 *
 * \ingroup Matrix
 * \sdd{WI-13908}
 */
void Matrix_2x2_Transpose(
   Matrix_2X2_T *matrix /**< [in/out] matrix */
);

/**
 * Transposes the given \f$ \Re^{3 * 3 } \f$ matrix: \f[ matrix^T \f] See [Wikipedia: Transpose](https:\\en.wikipedia.org/wiki/Transpose)
 *
 * \ingroup Matrix
 * \sdd{WI-13913}
 */
void Matrix_3x3_Transpose(
   Matrix_3X3_T *matrix /**< [in/out] matrix to be transposed*/
);

#ifdef __cplusplus
}
#endif
#endif

