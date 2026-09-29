#ifndef ABSTRACT_VECTOR_2D_TEST_HPP
#define ABSTRACT_VECTOR_2D_TEST_HPP
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "ml_vector_2d_t.h"

class Basic_Vectors
{
public:
	static const Vector_2d_T vec_arbitrary1 /**< This is an arbitrary vector*/;
	static const Vector_2d_T vec_arbitrary2 /**< This is an arbitrary vector*/;

	static const Vector_2d_T vec_x_normal; /**< x_normal */
	static const Vector_2d_T vec_y_normal; /**< y_normal */
	static const Vector_2d_T vec_neg_x_normal; /**< negative x_normal */
	static const Vector_2d_T vec_neg_y_normal; /**< negative y_normal */
	static const Vector_2d_T vec_first_bisectrix; /**< vector bisecting the first 2d sector */
	static const Vector_2d_T vec_second_bisectrix; /**< vector bisecting the 2th 2d sector */
	static const Vector_2d_T vec_third_bisectrix; /**< vector bisecting the 3th 2d sector */
	static const Vector_2d_T vec_fourth_bisectrix; /**< vector bisecting the 4th 2d sector */

private:
	Basic_Vectors() {}; /**< this class should not be instantiated! */
};


#endif
