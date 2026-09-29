

#include "Basic_Vectors.hpp"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"

/**********************
* Init all static constants
*
* All Constants were moved here to get control of the initialization order.
*
* If you split up these to multiple compilation units, the order becomes undefined or
* relies on linker script order, which, for our purpose, means the same thing.
*/

const Vector_2d_T Basic_Vectors::vec_x_normal = Create_2d_Vector_X_Normal();
const Vector_2d_T Basic_Vectors::vec_y_normal = Create_2d_Vector_Y_Normal();
const Vector_2d_T Basic_Vectors::vec_neg_x_normal = Create_2d_Vector_Coordinates(-1., 0.);
const Vector_2d_T Basic_Vectors::vec_neg_y_normal = Create_2d_Vector_Coordinates(0., -1.);
const Vector_2d_T Basic_Vectors::vec_first_bisectrix = Create_2d_Vector_Coordinates(1., 1.);
const Vector_2d_T Basic_Vectors::vec_second_bisectrix = Create_2d_Vector_Coordinates(-1., 1.);
const Vector_2d_T Basic_Vectors::vec_third_bisectrix = Create_2d_Vector_Coordinates(-1., -1.);
const Vector_2d_T Basic_Vectors::vec_fourth_bisectrix = Create_2d_Vector_Coordinates(1., -1.);

const Vector_2d_T Basic_Vectors::vec_arbitrary1 = Create_2d_Vector_Coordinates(4275.82323234f, 4443.11f);
const Vector_2d_T Basic_Vectors::vec_arbitrary2 = Create_2d_Vector_Coordinates(4886.43332f, -1244.334f);





