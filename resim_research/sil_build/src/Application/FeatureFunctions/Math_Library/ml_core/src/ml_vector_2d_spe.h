#ifndef ML_VECTOR_2D_SPE_H
#define ML_VECTOR_2D_SPE_H
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "ml_math.h"

/* Below codes are related with inline assembly code syntax, Be careful when you modify the codes.*/
/* If you want to modify this code, refer "Wind River Diab Compiler for PowerPC User's Guide 5,9,4" on page 114 */


/**
 * \defgroup Vector_2d_SPE2 2D Vector Algebra (SPE2)
 * \brief Vector operations by using SPE2 Assembly instead of C
 *
 * Vector 2D operations can be accelerated by using SPE2 in NXP's PowerPC Core MCUs
 *
 * \section subSecCMakeSPE2 1. How to use it - Setup in CMake
 * Step1. Check "Shared_Toolbox_USE_SPE2" in CMake options. It will add SPE2 directory path and definition in makefile.
 *
 * \section subSecMakeSPE2 2. How to use it - Setup in project
 * Step2. Add preprocessor directive(ST_ENABLE_SPE2_VEC) in project makefile. Also if project makefile doesn't include "/inc_spe2" folder, you have to add that folder.
 *
 * \section subSecFuncList 3.  List of functions that implemented by SPE2
 * - Vector_2d_Alg_Rotate
 * - Vector_2d_Alg_Multiply_Scalar
 * - Vector_2d_Alg_Add
 * - Vector_2d_Alg_Scalar_Product
 * - Vector_2d_Alg_Abs
 * - Vector_2d_Alg_Abs_Squared
 * - Vector_2d_Alg_Rotate_Negative
 * - Vector_2d_Alg_Diff
 * - Vector_2d_Alg_Abs_Component_Wise
 * - Vector_2d_Alg_Sqrt_Component_Wise
 * - Vector_2d_Alg_Scalar_Product_With_Angle
 * - Vector_2d_Alg_Limit_Vector
 *
 * \section subSecPerformance 4. Performance improvement of SPE2
 *
 * These function performance are measured by using internal timer function via TRACE32. (STM_Timer_Get_Value, STM_Timer_Get_us)
 *
 * <table>
* <tr><th>Function Name <th>C <th>SPE2  <th>Reduced Time <th> Performance
* <tr><td>	Vector_2d_Alg_Rotate <td>0.316 us<td>0.200 us<td>0.116 us<td>36.7% Up
* <tr><td>	Vector_2d_Alg_Multiply_Scalar	<td>	0.300 us	<td>	0.183 us	<td>	0.117 us	<td>	39.0% Up
* <tr><td>	Vector_2d_Alg_Add	<td>	0.267 us	<td>	0.183 us	<td>	0.084 us	<td>	31.4%  Up
* <tr><td>	Vector_2d_Alg_Scalar_Product	<td>	0.250 us	<td>	0.166 us	<td>	0.084 us	<td>	33.6% Up
* <tr><td>	Vector_2d_Alg_Abs	<td>	2.413 us	<td>	0.300 us	<td>	2.113 us	<td>	87.5% Up
* <tr><td>	Vector_2d_Alg_Abs_Squared	<td>	1.366 us	<td>	0.233 us	<td>	1.133 us	<td>	82.9% Up
* <tr><td>	Vector_2d_Alg_Rotate_Negative	<td>	0.333 us	<td>	0.166 us	<td>	0.167 us	<td>	50.1% Up
* <tr><td>	Vector_2d_Alg_Diff	<td>	0.266 us	<td>	0.183 us	<td>	0.83 us	<td>	31.2% Up
* <tr><td>	Vector_2d_Alg_Abs_Component_Wise	<td>	0.25 us	<td>	0.25 us	<td>	-	<td>	None Change
* <tr><td>	Vector_2d_Alg_Sqrt_Component_Wise	<td>	9.03 us	<td>	0.300 us	<td>	8.73 us	<td>	96.6% Up
* <tr><td>	Vector_2d_Alg_Scalar_Product_With_Angle	<td>	0.283 us	<td>	0.167 us	<td>	0.116 us	<td>	40.9% Up
* <tr><td>	Vector_2d_Alg_Limit_Vector	<td>	0.316 us	<td>	0.283 us	<td>	0.033 us	<td>	10.4% Up
* </table>
 *
 *
 * \section subSecDifference 5. Output value difference between SPE2 and C
 * There are no difference in 8 functions, 4 functions have differences.
 * Their maximum differences are 10^-6 level (0.000001)
 * <table>
 * <tr><th>Function Name <th>Maximum Difference  between SPE2 and C
 * <tr><td>	Vector_2d_Alg_Rotate					<td> 0.48 x 10^-6
* <tr><td>	Vector_2d_Alg_Multiply_Scalar			<td> 0
* <tr><td>	Vector_2d_Alg_Add						<td> 0
* <tr><td>	Vector_2d_Alg_Scalar_Product			<td> 1.90 x 10^-6
* <tr><td>	Vector_2d_Alg_Abs						<td> 0
* <tr><td>	Vector_2d_Alg_Abs_Squared				<td> 0
* <tr><td>	Vector_2d_Alg_Rotate_Negative			<td> 0.48 x 10^-6
* <tr><td>	Vector_2d_Alg_Diff						<td> 0
* <tr><td>	Vector_2d_Alg_Abs_Component_Wise			<td> 0
* <tr><td>	Vector_2d_Alg_Sqrt_Component_Wise		<td> 0
* <tr><td>	Vector_2d_Alg_Scalar_Product_With_Angle	<td> 0.48 x 10^-6
* <tr><td>	Vector_2d_Alg_Limit_Vector				<td> 0
 * </table>
 */

/**
* Returns a Vector that is rotated around the origin by the provided angle
* \return           Vector that is rotated around the origin by the provided angle
* \ingroup Vector_2d_SPE2
* \sa Corresponding C function is Vector_2d_Alg_Rotate()
*/
__asm int Alg_Rotate_Asm(float* argf1, float* argf2, float* result)
{
% reg argf1 ; reg argf2 ; reg result;
! "r3", "r4", "r5" ,"r10", "r11"

  evldw r4, 0(argf1) 		/* p_vectorA {y,x} */
  evldw r5, 0(argf2) 		/* p_angle { cos, sin } */

  evfsmulx r11, r5, r4 		/* r11 = {x*cos, y*sin} retvalue.x */
  evfsmul r10, r4, r5 		/* r10 = {y*cos, x*sin} retvalue.y */

  evfsdiffsum r3, r11, r10 	/* see SPE2PIM manual, 1 is +- op, 1 is ++ op on each register */
  evstdw r3, 0(result)
}

/**
* Returns a Vector that is multiplied by the given factor
* \return           Vector that is multiplied by the given factor
* \ingroup Vector_2d_SPE2
* \sa Corresponding C function is Vector_2d_Alg_Multiply_Scalar()
*/
__asm void Alg_Multiply_Scalar_Asm(float* argf1, float* factor, float* result)
{
% reg argf1 ; reg factor ; reg result;
! "r3", "r4", "r5"

  evldw r4, 0(argf1) 		/* p_vectorA {y,x} */
  evldw r5, 0(factor) 		/* factor */

  evfsmule r3, r5, r4 		/* multiply_scalar */

  evstdw r3, 0(result)
}

/**
* Returns a Vector that is the addition of the provided vectors
* \return           Vector that is the addition of the provided vectors
* \ingroup Vector_2d_SPE2
* \sa Corresponding C function is Vector_2d_Alg_Add()
*/
__asm void Alg_Add_Asm(float* p_vector_A, float* p_vector_B, float* result)
{
% reg p_vector_A ; reg p_vector_B ; reg result;
! "r3", "r4", "r5"

  evldw r4, 0(p_vector_A) 	/* p_vectorA {y,x} */
  evldw r5, 0(p_vector_B) 	/* factor */

  evfsadd r3, r5, r4 		/* Add */

  evstdw r3, 0(result)
}


/**
* Computes the 2d scalar product between two vectors
* \return           2d scalar product between two vectors
* \sa Corresponding C function is Vector_2d_Alg_Scalar_Product(), http:\\mathworld.wolfram.com/DotProduct.html*
* \ingroup Vector_2d_SPE2
*
*/
__asm float Alg_Scalar_Product_Asm(float* p_vector_A, float* p_vector_B)
{
% reg p_vector_A ; reg p_vector_B ;
! "r3", "r4", "r5", "r6"

  evldw r4, 0(p_vector_A) 	/* p_vectorA {y,x} */
  evldw r5, 0(p_vector_B) 	/* p_vectorB {y,x} */

  evfsmul r6, r4, r5		/* A->y * B->y , A->x * B->x */
  evfssum r3, r6, r6 		/*  x^2 + y^2 */
}

/**
* Returns the absolute value (length) of a vector.
* \return the absolute value (length) of a vector.
* \sa Corresponding C function is Vector_2d_Alg_Abs, https:\\en.wikipedia.org/wiki/Absolute_value
* \ingroup Vector_2d_SPE2
*/
__asm float Alg_Abs_Asm(float* p_vector_A) // Same as Norm
{
% reg p_vector_A ;
! "r3", "r4", "r5", "r6"

  evldw r4, 0(p_vector_A) 	/* p_vectorA {y,x} */
  evldw r5, 0(p_vector_A) 	/* p_vectorB {y,x} */

  evfsmul r6, r4, r5 		/* A->y * B->y , A->x * B->x */
  evfssum r3, r6, r6 		/*  x^2 + y^2 */
  evfssqrt r3, r3
}

/**
* Returns the squared absolute value (length^2) of a vector.
* \return           squared absolute value (length^2) of a vector.
* \sa Corresponding C function is Vector_2d_Alg_Abs_Squared()
* \ingroup Vector_2d_SPE2
*/
__asm float Alg_Abs_Sq_Asm(float* p_vector_A)
{
% reg p_vector_A ;
! "r3", "r4", "r5", "r6"

  evldw r4, 0(p_vector_A) 	/* p_vectorA {y,x} */
  evldw r5, 0(p_vector_A) 	/* p_vectorA {y,x} */

  evfsmul r6, r4, r5 		/* A->y * B->y , A->x * B->x  */
  evfssum r3, r6, r6 		/*  x^2 + y^2 */
}
/**
* Rotates a point around the coordinate systems origin by a inverted (negative) angle.
* \return   rotated point
* \sa Corresponding C function is Vector_2d_Alg_Rotate_Negative()
* \ingroup Vector_2d_SPE2
*/
__asm int Alg_RotateNegative_Asm(float* argf1, float* argf2, float* result)
{
% reg argf1 ; reg argf2 ; reg result;
! "r3", "r4", "r5" ,"r10", "r11"

  evldw r4, 0(argf1) 		/* p_vectorA {y,x} */
  evldw r5, 0(argf2) 		/* p_angle { cos, sin } */

  evfsmulx r11, r5, r4 		/* r11 = {x*cos, y*sin} retvalue.x */
  evfsmul r10, r5, r4 		/* r10 = {y*cos, x*sin} retvalue.y */
  evfsneg r10, r10

  evfssumdiff r3, r11, r10 	/* see SPE2PIM manual, 1 is +- op, 1 is ++ op on each register. */
  evstdw r3, 0(result)
}

/**
* Computes the subtraction of two vectors.
* \return   Difference between given vectors
* \sa Corresponding C function is Vector_2d_Alg_Diff(), http:\\mathworld.wolfram.com/VectorDifference.html
* \ingroup Vector_2d_SPE2
*/
__asm int Alg_Diff_Asm(float* argf1, float* argf2, float* result)
{
% reg argf1 ; reg argf2 ; reg result;
! "r3", "r4", "r5" ,"r10", "r11"

  evldw r4, 0(argf1) 		/* p_vectorA {y,x} */
  evldw r5, 0(argf2) 		/* p_vectorB {y,x} */

  evfssub r3, r4, r5
  evstdw r3, 0(result)
}

/**
* Computes the absolute value of each component in the input vector.
* \return   Given vector with each component in its absolute (positive) value.
* \sa Corresponding C function is Vector_2d_Alg_Abs_Component_Wise()
* \ingroup Vector_2d_SPE2
*/
__asm float Alg_Abs_CompWise_Asm(float* p_vector_A, float* result)
{
% reg p_vector_A ; reg result ;
! "r3", "r4", "r5", "r6"

  evldw r3, 0(p_vector_A) 	/* p_vectorA {y,x} */
  evfsabs r3, r3 			/* Do Absolute */
  evstdw r3, 0(result)
}

/**
* Computes the square root of each component of a vector
* \return   Given vector with each component being the root of the given component in the given vector
* \sa Corresponding C function is Vector_2d_Alg_Sqrt_Component_Wise()
* \ingroup Vector_2d_SPE2
*/
__asm float Alg_Sqrt_CompWise_Asm(float* p_vector_A, float* result)
{
% reg p_vector_A ; reg result ;
! "r3", "r4", "r5", "r6"

  evldw r3, 0(p_vector_A) 	/* p_vectorA {y,x} */
  evfssqrt r3, r3 			/* Do Sqrt */
  evstdw r3, 0(result)
}

/**
* Computes the scalar product of a vector with an angle
* \return   Scalar product of given vector and given angle
* \sa Corresponding C function is Vector_2d_Alg_Scalar_Product_With_Angle()
* \ingroup Vector_2d_SPE2
*/
__asm float Alg_Scalar_Product_with_angle_Asm(float* p_vector_A, float* p_angle)
{
% reg p_vector_A ; reg p_angle ;
! "r3", "r4", "r5", "r6"

  evldw r4, 0(p_vector_A) 	/* p_vectorA {y,x} */
  evldw r5, 0(p_angle) 		/* p_angle {cos,sin} */

  evfsmulx r6, r4, r5  		/* Cross Multiplcation */
  evfssum r3, r6, r6 		/* Sum */
}

/**
* Returns a vector that is limited to be within a square defined by p_limit_max_in and p_limit_min_in
* \return   vector that is limited to be within a square defined by p_limit_max_in and p_limit_min_in
* \sa Corresponding C function is Vector_2d_Alg_Limit_Vector()
* \ingroup Vector_2d_SPE2
*/
__asm void Alg_LimVector_Asm(float* p_vector_in, float* p_vector_max_in, float* p_vector_min_in, float* result)
{
% reg p_vector_in ; reg p_vector_max_in ; reg p_vector_min_in ; reg result ;
! "r3", "r4", "r5", "r6"

	evldw r3, 0(p_vector_in) 		/* p_vector_in {y,x} */
	evldw r4, 0(p_vector_max_in) 	/* p_vector_max_in {y,x} */
	evldw r5, 0(p_vector_min_in) 	/* p_vector_min_in {y,x} */

	evfsmin r3, r3, r4 				/* Value = Min(in, max) */
	evfsmax r3, r3, r5 				/* Max(Value, min) */

	evstdw r3, 0(result)
}

/**
 */

#endif

