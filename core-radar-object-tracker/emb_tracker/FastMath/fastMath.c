#include "fastMath.h"
#include "Math_Selector.h"
#include "fastMathTables.h"
#include "reuse.h" /*TODO: remove this include as soon as fastMath include has been removed from all other files*/
// #include "Basic_Macros.h"
// #include "Assert_Macros.h"
#include <assert.h>

static const float32_t PI                = 3.14159265358979323846f;
static const float32_t THRESHOLD_IS_ZERO = 1e-6f;

static const float32_t HALF_PI =
   (((float32_t)PI) * 0.5f); /** HALF_PI is only needed in this module and not needed if fast math library is not used */
static const float32_t ONE_HALF_PI = (((float32_t)PI) * 1.5f);
static const float32_t DOUBLE_PI   = (((float32_t)PI) * 2.0f);

static float32_t force_nonzero(float32_t x);

float32_t fast_cos(float32_t x)
{
   float ret_val;
   int index;

   if (x != x)
   {
      /* x = nan*/
      return x;
   }

   /* compute index into full CS table */
   /* as if there was a full table containing all values from 0 to 2*PI (full 360 degrees) */
   if (x < 0.f)
   {
      index = ((int)((-x * CS_INV_PREC) + .5f)) & CS_FULL_TABLE_MASK;
   }
   else
   {
      index = ((int)((x * CS_INV_PREC) + .5f)) & CS_FULL_TABLE_MASK;
   }

   /* get value from quarter CS table */
   /* as the cosine is symmetric, we only need one quarter of the full 360 degrees table to save all look up values */
   /* hence we compute the index into the quarter table and read out the corresponding value */
   if (index < CS_QUARTER_TABLE_SIZE)
   {
      ret_val = CS_TABLE[index];
   }
   else if (index <= CS_HALF_TABLE_MAX_INDEX)
   {
      assert((CS_HALF_TABLE_MAX_INDEX - index) < CS_QUARTER_TABLE_SIZE);
      ret_val = -CS_TABLE[CS_HALF_TABLE_MAX_INDEX - index];
   }
   else if (index <= (CS_HALF_TABLE_MAX_INDEX + CS_QUARTER_TABLE_MAX_INDEX))
   {
      assert((index - CS_HALF_TABLE_MAX_INDEX) < CS_QUARTER_TABLE_SIZE);
      ret_val = -CS_TABLE[index - CS_HALF_TABLE_MAX_INDEX];
   }
   else
   {
      assert((CS_FULL_TABLE_SIZE - index) < CS_QUARTER_TABLE_SIZE);
      ret_val = CS_TABLE[CS_FULL_TABLE_SIZE - index];
   }
   if ((ret_val <= -0.02) || (0.02 <= ret_val))
   {
      return ret_val;
   }
   x = (x < 0) ? -x : x;
   while (x > DOUBLE_PI)
   {
      x -= DOUBLE_PI;
   }
   if (PI < x)
   {
      return (x - ONE_HALF_PI);
   }
   else
   {
      return (HALF_PI - x);
   }
}

float32_t fast_sin(float32_t x)
{
   return fast_cos(x - HALF_PI);
}

static float32_t force_nonzero(float32_t x)
{
   float32_t x_nonzero = x;

   if ((x_nonzero < THRESHOLD_IS_ZERO) && (x_nonzero >= 0.f))
   {
      x_nonzero = THRESHOLD_IS_ZERO;
   }
   else if ((x_nonzero > -THRESHOLD_IS_ZERO) && (x_nonzero < 0.f))
   {
      x_nonzero = -THRESHOLD_IS_ZERO;
   }
   else
   { /*do nothing */
   }

   return x_nonzero;
}

float32_t fast_tan(float32_t x)
{
   float32_t cos_nonzero;

   cos_nonzero = fast_cos(x);
   cos_nonzero = force_nonzero(cos_nonzero);

   return fast_sin(x) / cos_nonzero;
}

float32_t fast_atan(float32_t x)
{
   int index;

   if (x != x)
   {
      /* x = nan*/
      return x;
   }

   /* handle negative condition*/
   if (x < 0.f)
   {
      float32_t x2;
      x2 = -x;
      if (x2 <= 1.f)
      {
         /* magnitude less than 1, use table directly*/
         index = (int)(x2 * ATAN_INV_PREC);
         return -ATAN_TABLE[index];
      }
      else
      {
         /* magnitude greater than 1, use inverse into table*/
         index = (int)(ATAN_INV_PREC / x2);
         return ATAN_TABLE[index] - HALF_PI;
      }
   }
   else
   {
      if (x <= 1.f)
      {
         /* magnitude less than 1, use table directly*/
         index = (int)(x * ATAN_INV_PREC);
         return ATAN_TABLE[index];
      }
      else
      {
         /* magnitude greater than 1, use inverse into table*/
         index = (int)(ATAN_INV_PREC / x);
         return HALF_PI - ATAN_TABLE[index];
      }
   }
}

float32_t fast_atan2(float32_t y, float32_t x)
{
   if (x != x)
   {
      /* x is nan*/
      return x;
   }
   if (y != y)
   {
      /* y is nan*/
      return y;
   }

   if (x > 0.f)
   {
      return fast_atan(y / x);
   }
   else if ((x < 0.f) && (y >= 0.f))
   {
      return fast_atan(y / x) + ((float32_t)PI);
   }
   else if ((x < 0.f) && (y < 0.f))
   {
      return fast_atan(y / x) - ((float32_t)PI);
   }
   else if ((x == 0.f) && (y > 0.f))
   {
      return HALF_PI;
   }
   else if ((x == 0.f) && (y < 0.f))
   {
      return -HALF_PI;
   }
   else
   {
      /* undefined; just return x value*/
      return x;
   }
}

float32_t fast_asin(float32_t x)
{
   float32_t x_sq;

   x_sq = x * x;

   if (x_sq <= 1.f)
   {
      return 2.f * fast_atan(x / (1.f + FAST_SQRT(1.f - x_sq)));
   }
   else
   {
      /* undefined; just return x value*/
      return x;
   }
}

float32_t fast_acos(float32_t x)
{
   if ((x >= -1.f) && (x <= 1.f))
   {
      return HALF_PI - fast_asin(x);
   }
   else
   {
      /* undefined; just return x value*/
      return x;
   }
}

float32_t fast_exp(float32_t x)
{
   uint32_t index;
   int32_t temp_var;

   /*return exp(x);*/
   if (x != x)
   {
      /* x = nan*/
      return x;
   }

   if (x <= LOWEST_DOMAIN_FLOAT)
   {
      return EXP_TABLE[0];
   }

   if (x >= HIGHEST_DOMAIN_FLOAT)
   {
      return EXP_TABLE[EXP_TABLE_SIZE - 1];
   }

   /* compute index into EXP table*/
   temp_var = (int32_t)(x * EXP_INV_PREC);

   index = temp_var + MAG_LOWEST_DOMAIN;

   return EXP_TABLE[index];
}
