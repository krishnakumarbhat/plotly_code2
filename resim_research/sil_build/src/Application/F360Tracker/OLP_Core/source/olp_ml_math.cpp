#ifndef ML_DT_WRAPPER_CPP
#define ML_DT_WRAPPER_CPP

#include "olp_ml_math.h"
#include "ml_vector_2d.h"
#include "ml_angle.h"
#include "ml_math.h"
#include "ml_trigonometry.h"

namespace olp
{

   float olp_ml_wrapper::Olp_NormalizeAngle(const float a, const float b)
   {
      float res = static_cast<float>(Normalize_Angle(static_cast<float32_T>(a), static_cast<float32_T>(b)));
      return res;
   }

   float olp_ml_wrapper::Olp_Triangle_Gamma_From_Abc(const float a, const float b, const float c)
   {
      float res = Triangle_Gamma_From_Abc(a, b, c);
      return res;
   }

   float olp_ml_wrapper::Olp_Vector_2d_Alg_Abs_squared(const Olp_Vector_2d_T &v)
   {
      Vector_2d_T v2d = { static_cast<float32_T>(v.x), static_cast<float32_T>(v.y) };
      float res = static_cast<float>(Vector_2d_Alg_Abs_Squared(&v2d));
      return res;
   }

   Olp_Vector_2d_T olp_ml_wrapper::Olp_Create_2d_Vector_Coordinates(const float x, const float y)
   {
      Vector_2d_T res = Create_2d_Vector_Coordinates(static_cast<float32_T>(x), static_cast<float32_T>(y));
      Olp_Vector_2d_T ret = { static_cast<float>(res.x), static_cast<float>(res.y) };
      return ret;
   }

   Olp_Angle_T olp_ml_wrapper::Olp_Angle_diff(const Olp_Angle_T &angle1, const Olp_Angle_T &angle2)
   {
      Angle_T agl1 = { static_cast<float32_T>(angle1.angle), static_cast<float32_T>(angle1.sin), static_cast<float32_T>(angle1.cos) };
      Angle_T agl2 = { static_cast<float32_T>(angle2.angle), static_cast<float32_T>(angle2.sin), static_cast<float32_T>(angle2.cos) };
      Angle_T res = Angle_Diff(&agl1, &agl2);
      Olp_Angle_T ret = { static_cast<float>(res.angle), static_cast<float>(res.sin), static_cast<float>(res.cos) };
      return ret;
   }

   Olp_Vector_2d_T olp_ml_wrapper::Olp_Vector_2d_Alg_RotateNegative(const Olp_Angle_T &angle, const Olp_Vector_2d_T &vector)
   {
      Angle_T agl = { static_cast<float32_T>(angle.angle), static_cast<float32_T>(angle.sin), static_cast<float32_T>(angle.cos) };
      Vector_2d_T v2d = { static_cast<float32_T>(vector.x), static_cast<float32_T>(vector.y) };
      Vector_2d_T res = Vector_2d_Alg_Rotate_Negative(&agl, &v2d);
      Olp_Vector_2d_T ret = { static_cast<float>(res.x), static_cast<float>(res.y) };
      return ret;
   }

   Olp_Vector_2d_T olp_ml_wrapper::Olp_Vector_2d_Alg_Rotate(const Olp_Angle_T &angle, const Olp_Vector_2d_T &vector)
   {
      Angle_T agl = { static_cast<float32_T>(angle.angle), static_cast<float32_T>(angle.sin), static_cast<float32_T>(angle.cos) };
      Vector_2d_T v2d = { static_cast<float32_T>(vector.x), static_cast<float32_T>(vector.y) };
      Vector_2d_T res = Vector_2d_Alg_Rotate(&agl, &v2d);
      Olp_Vector_2d_T ret = { static_cast<float>(res.x), static_cast<float>(res.y) };
      return ret;
   }

   Olp_Vector_2d_T olp_ml_wrapper::Olp_Vector_2d_Alg_Add(const Olp_Vector_2d_T &vector_1, const Olp_Vector_2d_T &vector_2)
   {
      Vector_2d_T v1 = { static_cast<float32_T>(vector_1.x), static_cast<float32_T>(vector_1.y) };
      Vector_2d_T v2 = { static_cast<float32_T>(vector_2.x), static_cast<float32_T>(vector_2.y) };
      Vector_2d_T res = Vector_2d_Alg_Add(&v1, &v2);
      Olp_Vector_2d_T ret = { static_cast<float>(res.x), static_cast<float>(res.y) };
      return ret;
   }

   Olp_Vector_2d_T olp_ml_wrapper::Olp_Vector_2d_Alg_Diff(const Olp_Vector_2d_T &vector_1, const Olp_Vector_2d_T &vector_2)
   {
      Vector_2d_T v1 = { static_cast<float32_T>(vector_1.x), static_cast<float32_T>(vector_1.y) };
      Vector_2d_T v2 = { static_cast<float32_T>(vector_2.x), static_cast<float32_T>(vector_2.y) };
      Vector_2d_T res = Vector_2d_Alg_Diff(&v1, &v2);
      Olp_Vector_2d_T ret = { static_cast<float>(res.x), static_cast<float>(res.y) };
      return ret;
   }

   float olp_ml_wrapper::Olp_Vector_2d_Alg_Abs(const Olp_Vector_2d_T &vector)
   {
      Vector_2d_T v2d = { static_cast<float32_T>(vector.x), static_cast<float32_T>(vector.y) };
      return (static_cast<float>(Vector_2d_Alg_Abs(&v2d)));
   }

   Olp_Vector_2d_T olp_ml_wrapper::Olp_Create_2d_Vector_Origin()
   {
      Vector_2d_T res = Create_2d_Vector_Origin();
      Olp_Vector_2d_T ret = { static_cast<float>(res.x), static_cast<float>(res.y) };
      return ret;
   }

   Olp_Angle_T olp_ml_wrapper::Olp_Create_Angle(const float angle)
   {
      Angle_T res = Create_Angle(static_cast<float32_T>(angle));
      Olp_Angle_T ret = { static_cast<float>(res.angle), static_cast<float>(res.sin), static_cast<float>(res.cos) };
      return ret;
   }

   float olp_ml_wrapper::Olp_Fast_Cos(const float angle)
   {
      return static_cast<float>(Fast_Cos(static_cast<float32_T>(angle)));
   }

   float olp_ml_wrapper::Olp_Fast_Sin(const float angle)
   {
      return static_cast<float>(Fast_Sin(static_cast<float32_T>(angle)));
   }

   float olp_ml_wrapper::Olp_Fast_Sqrt(const float val)
   {
      return (Fast_Sqrt(val));
   }


   float olp_ml_wrapper::Olp_Fast_Atan2(const float val1, const float val2)
   {
      return static_cast<float>(Fast_Atan2(static_cast<float32_T>(val1), static_cast<float32_T>(val2)));
   }

   boolean_T olp_ml_wrapper::Olp_Is_AcceptableTriangle(const float side1, const float side2, const float side3, const float epsilon)
   {
      return (((side1 + side2) > (side3 - epsilon)) && ((side2 + side3) > (side1 - epsilon)) && ((side3 + side1) > (side2 - epsilon)));
   }

}

#endif