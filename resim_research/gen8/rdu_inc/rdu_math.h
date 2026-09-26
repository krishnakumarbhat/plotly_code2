#ifndef RDU_MATH_H
#define RDU_MATH_H

#include <math.h>

#include "api/bbe_helpers.h"
#include "radar_math.h"
#include <xtensa/tie/xt_bben_scalarfp.h>

static inline float rdu_finv(float x)
{
   float y = 0.0F;

   RADAR_RECIP(x, y);

   return y;
}

static inline float rdu_expf(float x)
{
   float y = 0.0F;

   RADAR_POW2(1.442695041e+00f * x, y);

   return y;
}

static inline float rdu_sqrtf(float x)
{
   return XT_SQRT_S(XT_ABS_S(x));
}

static inline float rdu_floorf(float x)
{
   return XT_FIFLOOR_S(x);
}

static inline float rdu_ceilf(float x)
{
   return XT_FICEIL_S(x);
}

static inline float rdu_roundf(float x)
{
   return XT_FIROUND_S(x);
}

static inline float rdu_absf(float x)
{
   return XT_ABS_S(x);
}

static inline float rdu_fmaxf(float a, float b)
{
   return XT_MAX_S(a, b);
}

static inline float rdu_acosf(float x)
{
   return rm_atan2f((float)XT_SQRT_S(XT_ABS_S(1.0f - x * x)), x);
}

#ifndef RADAR_PI
   #define RADAR_PI (3.141592653589793f)
#endif

#ifndef RADAR_PI_BY_2
   #define RADAR_PI_BY_2 (1.5707963267948966f) /* pi / 2 */
#endif

#define MAX_CONST_ATAN2F    (1.175494351e-38f)
#define POLY_ATAN2F_COEFF_1 (-7.704688187e-04f)
#define POLY_ATAN2F_COEFF_2 (3.941011655e-03f)
#define POLY_ATAN2F_COEFF_3 (-9.576779956e-03f)
#define POLY_ATAN2F_COEFF_4 (1.585144461e-02f)
#define POLY_ATAN2F_COEFF_5 (-2.233707356e-02f)
#define POLY_ATAN2F_COEFF_6 (3.178666856e-02f)
#define POLY_ATAN2F_COEFF_7 (-5.304973746e-02f)
#define POLY_ATAN2F_COEFF_8 (1.591549295e-01f)

#define CONVERT_P21_TO_FLOAT (0.000000476837158203125F)           /* 1/(2^21) */
#define CONVERT_P31_TO_FLOAT (0.0000000004656612873077392578125F) /* 1/(2^31) */

static inline void vec_atan2f(float *y, float *x, float *out)
{
   xb_vecN_2xf32 phi, u, v;
   phi                  = (xb_vecN_2xf32)(0.5F);
   xb_vecN_2xf32 *p_x   = (xb_vecN_2xf32 *)x;
   xb_vecN_2xf32 *p_y   = (xb_vecN_2xf32 *)y;
   xb_vecN_2xf32 *p_out = (xb_vecN_2xf32 *)out;
   xb_vecN_2xf32 v_x, v_y;
   xb_vecN_2xf32 v_acc, v_atan;
   vboolN_2 v_temp1;
   v_x     = BBE_LVN_2XF32_I(p_x, 0U);
   v_y     = BBE_LVN_2XF32_I(p_y, 0U);
   u       = BBE_ABSN_2XF32(v_x);
   v       = BBE_ABSN_2XF32(v_y);
   v_temp1 = v > u;
   u       = BBE_MOVN_2XF32T(v_x, u, v_temp1);
   v_x     = BBE_MOVN_2XF32T(v_y, v_x, v_temp1);
   v_y     = BBE_MOVN_2XF32T(-u, v_y, v_temp1);
   phi     = BBE_MOVN_2XF32T(phi + (xb_vecN_2xf32)(0.25F), phi, v_temp1);

   v_temp1 = v_x < (xb_vecN_2xf32)(0.0F);
   v_y     = BBE_MOVN_2XF32T(-v_y, v_y, v_temp1);
   phi     = BBE_MOVN_2XF32T(phi + (xb_vecN_2xf32)(0.5F), phi, v_temp1);

   v_x = BBE_ABSN_2XF32(v_x);
   v_x = BBE_MAXN_2XF32(v_x, MAX_CONST_ATAN2F);
   v   = BBE_RECIPN_2XF32(v_x);
   v   = v * v_y;
   u   = v * v;

   v_acc  = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_1);
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_2);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   v_acc  = v_atan;
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_3);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   v_acc  = v_atan;
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_4);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   v_acc  = v_atan;
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_5);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   v_acc  = v_atan;
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_6);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   v_acc  = v_atan;
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_7);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   v_acc  = v_atan;
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_8);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   BBE_MULAN_2XF32(phi, v, v_atan);

   v_acc  = BBE_FIFLOORN_2XF32(phi);
   v_acc  = phi - v_acc + (xb_vecN_2xf32)(-0.5F);
   v_atan = (xb_vecN_2xf32)(2.0F) * (xb_vecN_2xf32)(RADAR_PI)*v_acc;

   v_temp1 = v_atan <= (xb_vecN_2xf32)(-RADAR_PI);
   v_atan  = BBE_MOVN_2XF32T(v_atan + (xb_vecN_2xf32)(2.0f * RADAR_PI), v_atan, v_temp1);

   BBE_SVN_2XF32_I(v_atan, p_out, 0U);
}

static inline void vec_acosf(float *x, float *out)
{
   xb_vecN_2xf32 phi, u, v;
   phi                  = (xb_vecN_2xf32)(0.5F);
   xb_vecN_2xf32 *p_x   = (xb_vecN_2xf32 *)x;
   xb_vecN_2xf32 *p_out = (xb_vecN_2xf32 *)out;
   xb_vecN_2xf32 v_x, v_y;
   xb_vecN_2xf32 v_acc, v_atan;

   vboolN_2 v_temp1;
   v_x = BBE_LVN_2XF32_I(p_x, 0U);
   v_y = (xb_vecN_2xf32)(1.0F) - (BBE_MULN_2XF32(v_x, v_x));
   v_y = BBE_SQRTN_2XF32(v_y);

   /*
    reusing atan2f(y,x) function approximation
    since acos(x) = atan2(sqrt(1-x^2),x);
    */

   u       = BBE_ABSN_2XF32(v_x);
   v       = BBE_ABSN_2XF32(v_y);
   v_temp1 = v > u;
   u       = BBE_MOVN_2XF32T(v_x, u, v_temp1);
   v_x     = BBE_MOVN_2XF32T(v_y, v_x, v_temp1);
   v_y     = BBE_MOVN_2XF32T(-u, v_y, v_temp1);
   phi     = BBE_MOVN_2XF32T(phi + (xb_vecN_2xf32)(0.25F), phi, v_temp1);

   v_temp1 = v_x < (xb_vecN_2xf32)(0.0F);
   v_y     = BBE_MOVN_2XF32T(-v_y, v_y, v_temp1);
   phi     = BBE_MOVN_2XF32T(phi + (xb_vecN_2xf32)(0.5F), phi, v_temp1);

   v_x = BBE_ABSN_2XF32(v_x);
   v_x = BBE_MAXN_2XF32(v_x, MAX_CONST_ATAN2F);
   v   = BBE_RECIPN_2XF32(v_x);
   v   = v * v_y;
   u   = v * v;

   v_acc  = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_1);
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_2);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   v_acc  = v_atan;
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_3);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   v_acc  = v_atan;
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_4);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   v_acc  = v_atan;
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_5);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   v_acc  = v_atan;
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_6);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   v_acc  = v_atan;
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_7);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   v_acc  = v_atan;
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_8);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   BBE_MULAN_2XF32(phi, v, v_atan);

   v_acc  = BBE_FIFLOORN_2XF32(phi);
   v_acc  = phi - v_acc + (xb_vecN_2xf32)(-0.5F);
   v_atan = (xb_vecN_2xf32)(2.0F) * (xb_vecN_2xf32)(RADAR_PI)*v_acc;

   v_temp1 = v_atan <= (xb_vecN_2xf32)(-RADAR_PI);
   v_atan  = BBE_MOVN_2XF32T(v_atan + (xb_vecN_2xf32)(2.0f * RADAR_PI), v_atan, v_temp1);

   BBE_SVN_2XF32_I(v_atan, p_out, 0U);
}

static inline xb_vecN_2xf32 bbe_vec_sinf(xb_vecN_2xf32 v_x)
{
   xb_vecN_2xf32 v_temp1, v_temp2, v_x_sq, v_acc, v_c, v_sin;
   xb_vecN_2xf32 v_int_2_float;
   xb_vecN_2x32v v_float_2_int;
   xb_vecN_2xf32 v_1_by_2pi = (xb_vecN_2xf32)(XT_DIV_S(1.0F, 2.0F * RADAR_PI));
   v_temp1                  = BBE_MULN_2XF32(v_x, v_1_by_2pi);                               /* x = x *(1/2*pi) */
   v_temp2                  = BBE_ABSN_2XF32(BBE_ADDN_2XF32(v_temp1, (xb_vecN_2xf32)0.25F)); /* x = abs(x + 0.25f) */
   v_float_2_int            = xb_vecN_2xf32_rtor_xb_vecN_2x32v(v_temp2);                     /* (int)x */
   v_int_2_float            = xb_vecN_2x32v_rtor_xb_vecN_2xf32(v_float_2_int);               /* (float)(int)x */
   v_temp1                  = BBE_SUBN_2XF32(v_temp2, (xb_vecN_2xf32)0.5f);                  /* x = -0.5f + x  */
   v_temp2                  = BBE_SUBN_2XF32(v_temp1, v_int_2_float);                        /* x = x - (float)(int)x */
   v_x_sq                   = BBE_MULN_2XF32(v_temp2, v_temp2);                              /* x_sq = x*x */
   /* p = c6 */
   v_acc = (xb_vecN_2xf32)(6.565280458e+00f);
   /* p = p*x + c5 */
   v_c = (xb_vecN_2xf32)(-2.599369394e+01f);
   BBE_MULAN_2XF32(v_c, v_acc, v_x_sq);
   v_acc = v_c;
   /* p = p*x + c4 */
   v_c = (xb_vecN_2xf32)(6.017455525e+01f);
   BBE_MULAN_2XF32(v_c, v_acc, v_x_sq);
   v_acc = v_c;
   /* p = p*x + c3 */
   v_c = (xb_vecN_2xf32)(-8.545097926e+01f);
   BBE_MULAN_2XF32(v_c, v_acc, v_x_sq);
   v_acc = v_c;
   /* p = p*x + c2 */
   v_c = (xb_vecN_2xf32)(6.493916301e+01f);
   BBE_MULAN_2XF32(v_c, v_acc, v_x_sq);
   v_acc = v_c;
   /* p = p*x + c1 */
   v_c = (xb_vecN_2xf32)(-1.973920539e+01f);
   BBE_MULAN_2XF32(v_c, v_acc, v_x_sq);
   v_acc = v_c;
   /* p = p*x + c0 */
   v_c = (xb_vecN_2xf32)(9.999999918e-01f);
   BBE_MULAN_2XF32(v_c, v_acc, v_x_sq);
   v_sin = v_c;

   return v_sin;
}

static inline xb_vecN_2xf32 bbe_vec_cosf(xb_vecN_2xf32 v_x)
{
   xb_vecN_2xf32 v_temp1, v_temp2, v_x_sq, v_acc, v_c, v_sin;
   xb_vecN_2xf32 v_int_2_float;
   xb_vecN_2x32v v_float_2_int;
   xb_vecN_2xf32 v_1_by_2pi = (xb_vecN_2xf32)(XT_DIV_S(1.0F, 2.0F * RADAR_PI));
   v_temp2                  = BBE_ADDN_2XF32(v_x, (xb_vecN_2xf32)RADAR_PI_BY_2);
   v_temp1                  = BBE_MULN_2XF32(v_temp2, v_1_by_2pi);                           /* x = x *(1/2*pi) */
   v_temp2                  = BBE_ABSN_2XF32(BBE_ADDN_2XF32(v_temp1, (xb_vecN_2xf32)0.25F)); /* x = abs(x + 0.25f) */
   v_float_2_int            = xb_vecN_2xf32_rtor_xb_vecN_2x32v(v_temp2);                     /* (int)x */
   v_int_2_float            = xb_vecN_2x32v_rtor_xb_vecN_2xf32(v_float_2_int);               /* (float)(int)x */
   v_temp1                  = BBE_SUBN_2XF32(v_temp2, (xb_vecN_2xf32)0.5f);                  /* x = -0.5f + x  */
   v_temp2                  = BBE_SUBN_2XF32(v_temp1, v_int_2_float);                        /* x = x - (float)(int)x */
   v_x_sq                   = BBE_MULN_2XF32(v_temp2, v_temp2);                              /* x_sq = x*x */
   /* p = c6 */
   v_acc = (xb_vecN_2xf32)(6.565280458e+00f);
   /* p = p*x + c5 */
   v_c = (xb_vecN_2xf32)(-2.599369394e+01f);
   BBE_MULAN_2XF32(v_c, v_acc, v_x_sq);
   v_acc = v_c;
   /* p = p*x + c4 */
   v_c = (xb_vecN_2xf32)(6.017455525e+01f);
   BBE_MULAN_2XF32(v_c, v_acc, v_x_sq);
   v_acc = v_c;
   /* p = p*x + c3 */
   v_c = (xb_vecN_2xf32)(-8.545097926e+01f);
   BBE_MULAN_2XF32(v_c, v_acc, v_x_sq);
   v_acc = v_c;
   /* p = p*x + c2 */
   v_c = (xb_vecN_2xf32)(6.493916301e+01f);
   BBE_MULAN_2XF32(v_c, v_acc, v_x_sq);
   v_acc = v_c;
   /* p = p*x + c1 */
   v_c = (xb_vecN_2xf32)(-1.973920539e+01f);
   BBE_MULAN_2XF32(v_c, v_acc, v_x_sq);
   v_acc = v_c;
   /* p = p*x + c0 */
   v_c = (xb_vecN_2xf32)(9.999999918e-01f);
   BBE_MULAN_2XF32(v_c, v_acc, v_x_sq);
   v_sin = v_c;

   return v_sin;
}

static inline xb_vecN_2xf32 bbe_vec_atan2f(xb_vecN_2xf32 v_y, xb_vecN_2xf32 v_x)
{
   xb_vecN_2xf32 phi, u, v;
   phi = (xb_vecN_2xf32)(0.5F);
   xb_vecN_2xf32 v_acc, v_atan;
   vboolN_2 v_temp1;
   u       = BBE_ABSN_2XF32(v_x);
   v       = BBE_ABSN_2XF32(v_y);
   v_temp1 = v > u;
   u       = BBE_MOVN_2XF32T(v_x, u, v_temp1);
   v_x     = BBE_MOVN_2XF32T(v_y, v_x, v_temp1);
   v_y     = BBE_MOVN_2XF32T(-u, v_y, v_temp1);
   phi     = BBE_MOVN_2XF32T(phi + (xb_vecN_2xf32)(0.25F), phi, v_temp1);

   v_temp1 = v_x < (xb_vecN_2xf32)(0.0F);
   v_y     = BBE_MOVN_2XF32T(-v_y, v_y, v_temp1);
   phi     = BBE_MOVN_2XF32T(phi + (xb_vecN_2xf32)(0.5F), phi, v_temp1);

   v_x = BBE_ABSN_2XF32(v_x);
   v_x = BBE_MAXN_2XF32(v_x, MAX_CONST_ATAN2F);
   v   = BBE_RECIPN_2XF32(v_x);
   v   = v * v_y;
   u   = v * v;

   v_acc  = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_1);
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_2);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   v_acc  = v_atan;
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_3);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   v_acc  = v_atan;
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_4);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   v_acc  = v_atan;
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_5);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   v_acc  = v_atan;
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_6);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   v_acc  = v_atan;
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_7);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   v_acc  = v_atan;
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_8);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   BBE_MULAN_2XF32(phi, v, v_atan);

   v_acc  = BBE_FIFLOORN_2XF32(phi);
   v_acc  = phi - v_acc + (xb_vecN_2xf32)(-0.5F);
   v_atan = (xb_vecN_2xf32)(2.0F) * (xb_vecN_2xf32)(RADAR_PI)*v_acc;

   v_temp1 = v_atan <= (xb_vecN_2xf32)(-RADAR_PI);
   v_atan  = BBE_MOVN_2XF32T(v_atan + (xb_vecN_2xf32)(2.0f * RADAR_PI), v_atan, v_temp1);

   return v_atan;
}

static inline xb_vecN_2xf32 bbe_vec_acosf(xb_vecN_2xf32 v_x)
{
   xb_vecN_2xf32 phi, u, v;
   phi = (xb_vecN_2xf32)(0.5F);
   xb_vecN_2xf32 v_y;
   xb_vecN_2xf32 v_acc, v_atan;

   vboolN_2 v_temp1;
   v_y = (xb_vecN_2xf32)(1.0F) - (BBE_MULN_2XF32(v_x, v_x));
   v_y = BBE_SQRTN_2XF32(v_y);

   /*
    reusing atan2f(y,x) function approximation
    since acos(x) = atan2(sqrt(1-x^2),x);
    */

   u       = BBE_ABSN_2XF32(v_x);
   v       = BBE_ABSN_2XF32(v_y);
   v_temp1 = v > u;
   u       = BBE_MOVN_2XF32T(v_x, u, v_temp1);
   v_x     = BBE_MOVN_2XF32T(v_y, v_x, v_temp1);
   v_y     = BBE_MOVN_2XF32T(-u, v_y, v_temp1);
   phi     = BBE_MOVN_2XF32T(phi + (xb_vecN_2xf32)(0.25F), phi, v_temp1);

   v_temp1 = v_x < (xb_vecN_2xf32)(0.0F);
   v_y     = BBE_MOVN_2XF32T(-v_y, v_y, v_temp1);
   phi     = BBE_MOVN_2XF32T(phi + (xb_vecN_2xf32)(0.5F), phi, v_temp1);

   v_x = BBE_ABSN_2XF32(v_x);
   v_x = BBE_MAXN_2XF32(v_x, MAX_CONST_ATAN2F);
   v   = BBE_RECIPN_2XF32(v_x);
   v   = v * v_y;
   u   = v * v;

   v_acc  = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_1);
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_2);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   v_acc  = v_atan;
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_3);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   v_acc  = v_atan;
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_4);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   v_acc  = v_atan;
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_5);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   v_acc  = v_atan;
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_6);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   v_acc  = v_atan;
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_7);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   v_acc  = v_atan;
   v_atan = (xb_vecN_2xf32)(POLY_ATAN2F_COEFF_8);
   BBE_MULAN_2XF32(v_atan, v_acc, u);
   BBE_MULAN_2XF32(phi, v, v_atan);

   v_acc  = BBE_FIFLOORN_2XF32(phi);
   v_acc  = phi - v_acc + (xb_vecN_2xf32)(-0.5F);
   v_atan = (xb_vecN_2xf32)(2.0F) * (xb_vecN_2xf32)(RADAR_PI)*v_acc;

   v_temp1 = v_atan <= (xb_vecN_2xf32)(-RADAR_PI);
   v_atan  = BBE_MOVN_2XF32T(v_atan + (xb_vecN_2xf32)(2.0f * RADAR_PI), v_atan, v_temp1);

   return v_atan;
}

#define POW2_POLY_COEFF_1 (1.897859271e-03f)
#define POW2_POLY_COEFF_2 (8.939715102e-03f)
#define POW2_POLY_COEFF_3 (5.586850271e-02f)
#define POW2_POLY_COEFF_4 (2.401391566e-01f)
#define POW2_POLY_COEFF_5 (6.931547523e-01f)
#define POW2_POLY_COEFF_6 (9.999999404e-01f)
#define LOG2_E            (1.442695041e+00f)

static inline void vec_pow2(float *x, float *y)
{
   uint32_t __attribute__((aligned(32))) bits_uint_arr[8]   = {0U};
   float32_t __attribute__((aligned(32))) bits_float_arr[8] = {0.0F};

   xb_vecN_2x32Uv bits;
   xb_vecN_2xf32 float_bits;
   xb_vecN_2xf32 rm_x127, rm_t, rm_p, rm_f;
   xb_vecN_2x32v rm_i;
   rm_p = BBE_ZERON_2XF32();
   xb_vecN_2xf32 v_x;
   xb_vecN_2xf32 *p_x = (xb_vecN_2xf32 *)x;
   xb_vecN_2xf32 *p_y = (xb_vecN_2xf32 *)y;
   v_x                = BBE_LVN_2XF32_I(p_x, 0U);
   xb_vecN_2xf32 v_acc, v_coeff;
   xb_vecN_2x32v *p_bits_uint_arr = (xb_vecN_2x32v *)(&bits_uint_arr[0]);

   rm_x127 = v_x + (xb_vecN_2xf32)(127.0F);

   // xb_vecN_2x32Uv_rtor_xb_vecN_2xf32(xb_vecN_2x32Uv b)
   rm_i = xb_vecN_2xf32_rtor_xb_vecN_2x32v(rm_x127);
   rm_t = rm_x127 - xb_vecN_2x32v_rtor_xb_vecN_2xf32(rm_i);

   bits       = xb_vecN_2x32Uv_rtor_xb_vecN_2x32v(rm_i);
   float_bits = xb_vecN_2xf32_rtor_xb_vecN_2x32Uv(bits);
   float_bits = BBE_MULN_2XF32(float_bits, (xb_vecN_2xf32)(1U << 23U));
   bits       = xb_vecN_2x32Uv_rtor_xb_vecN_2xf32(float_bits);
   BBE_SVN_2X32_I(bits, p_bits_uint_arr, 0U);
   vector_copy_aligned((void *)(&bits_float_arr[0]), (void *)(&bits_uint_arr[0]), 32U);
   rm_f = BBE_LVN_2XF32_I((xb_vecN_2xf32 *)(&bits_float_arr[0]), 0U);

   //	rm_f = xb_vecN_2x32Uv_rtor_xb_vecN_2xf32(bits);

   v_acc   = (xb_vecN_2xf32)(POW2_POLY_COEFF_1);
   v_coeff = (xb_vecN_2xf32)(POW2_POLY_COEFF_2);
   BBE_MULAN_2XF32(v_coeff, v_acc, rm_t);
   v_acc   = v_coeff;
   v_coeff = (xb_vecN_2xf32)(POW2_POLY_COEFF_3);
   BBE_MULAN_2XF32(v_coeff, v_acc, rm_t);
   v_acc   = v_coeff;
   v_coeff = (xb_vecN_2xf32)(POW2_POLY_COEFF_4);
   BBE_MULAN_2XF32(v_coeff, v_acc, rm_t);
   v_acc   = v_coeff;
   v_coeff = (xb_vecN_2xf32)(POW2_POLY_COEFF_5);
   BBE_MULAN_2XF32(v_coeff, v_acc, rm_t);
   v_acc   = v_coeff;
   v_coeff = (xb_vecN_2xf32)(POW2_POLY_COEFF_6);
   BBE_MULAN_2XF32(v_coeff, v_acc, rm_t);

   rm_p = BBE_MULN_2XF32(rm_f, v_coeff);

   rm_p = BBE_MOVN_2XF32T(rm_p, (xb_vecN_2xf32)(0.0F), rm_x127 >= (xb_vecN_2xf32)(0.0F));

   BBE_SVN_2XF32_I(rm_p, p_y, 0U);
   return;
}

static inline xb_vecN_2xf32 bbe_vec_expf(xb_vecN_2xf32 v_x)
{
   uint32_t __attribute__((aligned(32))) bits_uint_arr[8]   = {0U};
   float32_t __attribute__((aligned(32))) bits_float_arr[8] = {0.0F};

   xb_vecN_2x32Uv bits;
   xb_vecN_2xf32 float_bits;
   xb_vecN_2xf32 rm_x127, rm_t, rm_p, rm_f;
   xb_vecN_2x32v rm_i;
   rm_p = BBE_ZERON_2XF32();
   xb_vecN_2xf32 v_acc, v_coeff;

   /* here computation is as below
    * e^x = 2^(x*log2(e)), fprintf('%16.9ef', log2(exp(1))) -> 1.442695041e+00f
    */

   /* multiplying the input with LOG2_E = 1.442695041e+00f */
   v_x = v_x * (xb_vecN_2xf32)(LOG2_E);

   /* this is approximate code to find exponent of 2 */
   rm_x127 = v_x + (xb_vecN_2xf32)(127.0F);

   rm_i = xb_vecN_2xf32_rtor_xb_vecN_2x32v(rm_x127);
   rm_t = rm_x127 - xb_vecN_2x32v_rtor_xb_vecN_2xf32(rm_i);

   bits       = xb_vecN_2x32Uv_rtor_xb_vecN_2x32v(rm_i);
   float_bits = xb_vecN_2xf32_rtor_xb_vecN_2x32Uv(bits);
   float_bits = BBE_MULN_2XF32(float_bits, (xb_vecN_2xf32)(1U << 23U));
   bits       = xb_vecN_2x32Uv_rtor_xb_vecN_2xf32(float_bits);
   BBE_SVN_2X32_I(bits, (xb_vecN_2x32v *)(&bits_uint_arr[0]), 0U);
   vector_copy_aligned((void *)(&bits_float_arr[0]), (void *)(&bits_uint_arr[0]), 32U);
   rm_f = BBE_LVN_2XF32_I((xb_vecN_2xf32 *)(&bits_float_arr[0]), 0U);

   v_acc   = (xb_vecN_2xf32)(POW2_POLY_COEFF_1);
   v_coeff = (xb_vecN_2xf32)(POW2_POLY_COEFF_2);
   BBE_MULAN_2XF32(v_coeff, v_acc, rm_t);
   v_acc   = v_coeff;
   v_coeff = (xb_vecN_2xf32)(POW2_POLY_COEFF_3);
   BBE_MULAN_2XF32(v_coeff, v_acc, rm_t);
   v_acc   = v_coeff;
   v_coeff = (xb_vecN_2xf32)(POW2_POLY_COEFF_4);
   BBE_MULAN_2XF32(v_coeff, v_acc, rm_t);
   v_acc   = v_coeff;
   v_coeff = (xb_vecN_2xf32)(POW2_POLY_COEFF_5);
   BBE_MULAN_2XF32(v_coeff, v_acc, rm_t);
   v_acc   = v_coeff;
   v_coeff = (xb_vecN_2xf32)(POW2_POLY_COEFF_6);
   BBE_MULAN_2XF32(v_coeff, v_acc, rm_t);

   rm_p = BBE_MULN_2XF32(rm_f, v_coeff);

   rm_p = BBE_MOVN_2XF32T(rm_p, (xb_vecN_2xf32)(0.0F), rm_x127 >= (xb_vecN_2xf32)(0.0F));

   return rm_p;
}

#endif /* RDU_MATH_H */
