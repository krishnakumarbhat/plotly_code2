#ifndef ML_DT_WRAPPER_HPP
#define ML_DT_WRAPPER_HPP

#include "olp_core_types.h"
#include "Basic_Macros.h"
#include "Math_Selector.h"
#include "ml_math_infinity_silent.h"
#include "ml_trigonometry.h"

#define OLP_FAST_ABS(x) FAST_ABS(x)
#define OLP_FAST_EXP(x) FAST_EXP(x)
#define OLP_FAST_SQRT(x) FAST_SQRT(x)
#define OLP_FAST_ATAN(x) FAST_ATAN(x)
#define OLP_MAX(a,b) MAX(a,b)
#define OLP_MIN(a,b) MIN(a,b)
#define OLP_IS_TRUE(x) IS_TRUE(x)
#define OLP_IS_FALSE(x) IS_FALSE(x)
#define OLP_SIGN(number) SIGN(number)
#define OLP_AS_TOOLBOX_INFINITY AS_TOOLBOX_INFINITY
#define OLP_TRUE TRUE
#define OLP_FALSE FALSE
#define OLP_EPSILON EPSILON

namespace olp
{
   class olp_ml_wrapper
   {
   public:
      olp_ml_wrapper() = delete;
      static float Olp_NormalizeAngle(const float a, const float b);
      static float Olp_Triangle_Gamma_From_Abc(const float a, const float b, const float c);
      static float Olp_Vector_2d_Alg_Abs_squared(const Olp_Vector_2d_T &v);
      static Olp_Vector_2d_T Olp_Create_2d_Vector_Coordinates(const float32_T x, const float32_T y);
      static Olp_Angle_T Olp_Angle_diff(const Olp_Angle_T &angle1, const Olp_Angle_T &angle2);
      static Olp_Vector_2d_T Olp_Vector_2d_Alg_RotateNegative(const Olp_Angle_T &angle, const Olp_Vector_2d_T &vector);
      static Olp_Vector_2d_T Olp_Vector_2d_Alg_Rotate(const Olp_Angle_T &angle, const Olp_Vector_2d_T &vector);
      static Olp_Vector_2d_T Olp_Vector_2d_Alg_Add(const Olp_Vector_2d_T &v1, const Olp_Vector_2d_T &v2);
      static Olp_Vector_2d_T Olp_Vector_2d_Alg_Diff(const Olp_Vector_2d_T &v1, const Olp_Vector_2d_T &v2);
      static float Olp_Vector_2d_Alg_Abs(const Olp_Vector_2d_T &v);
      static Olp_Vector_2d_T Olp_Create_2d_Vector_Origin();
      static Olp_Angle_T Olp_Create_Angle(const float angle);
      static float Olp_Fast_Cos(const float angle);
      static float Olp_Fast_Sin(const float angle);
      static float Olp_Fast_Sqrt(const float val);
      static float Olp_Fast_Atan2(const float val1, const float val2);
      static boolean_T Olp_Is_AcceptableTriangle(const float side1, const float side2, const float side3,const float epsilon);
   };
}

#endif