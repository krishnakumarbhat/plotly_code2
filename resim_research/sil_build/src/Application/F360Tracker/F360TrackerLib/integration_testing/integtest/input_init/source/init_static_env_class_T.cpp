
#include "f360_reuse.h"
#include "f360_constants.h"
#include "f360_static_environment_class.h"

namespace f360_variant_A
{
void init_static_env_class_T(Static_Env_T & static_env_class)
{
   static_env_class.Initialize_Static_Env();

   Static_Env_Poly_T static_env_polys[F360_NUM_OF_STATIC_ENV_POLYS];

   static_env_polys[0].age=0U;
   static_env_polys[0].confidence=1.0F;
   static_env_polys[0].lower_limit=0.0F;
   static_env_polys[0].upper_limit=0.0F;
   static_env_polys[0].p0=-1.0F;
   static_env_polys[0].p1=0.0F;
   static_env_polys[0].p2=0.5F * (-0.000164843179F);
   static_env_polys[0].status=F360_STATIC_ENV_POLY_STATUS_INVALID;
   static_env_polys[0].poly_type=F360_STATIC_ENV_POLY_TYPE_LSC;

   static_env_polys[1].age=0U;
   static_env_polys[1].confidence=1.0F;
   static_env_polys[1].lower_limit=-16.0F;
   static_env_polys[1].upper_limit=16.0F;
   static_env_polys[1].p0=1.0F;
   static_env_polys[1].p1=0.0F;
   static_env_polys[1].p2=0.5F * (-0.000164843179F);
   static_env_polys[1].status=F360_STATIC_ENV_POLY_STATUS_INVALID;
   static_env_polys[1].poly_type=F360_STATIC_ENV_POLY_TYPE_CURVG;

   static_env_polys[2].age=0U;
   static_env_polys[2].confidence=0.0F;
   static_env_polys[2].lower_limit=0.0F;
   static_env_polys[2].upper_limit=0.0F;
   static_env_polys[2].p0=0.0F;
   static_env_polys[2].p1=0.0F;
   static_env_polys[2].p2=0.0F;
   static_env_polys[2].status=F360_STATIC_ENV_POLY_STATUS_INVALID;
   static_env_polys[2].poly_type=F360_STATIC_ENV_POLY_TYPE_INVALID;

   static_env_polys[3].age=0U;
   static_env_polys[3].confidence=0.0F;
   static_env_polys[3].lower_limit=0.0F;
   static_env_polys[3].upper_limit=0.0F;
   static_env_polys[3].p0=0.0F;
   static_env_polys[3].p1=0.0F;
   static_env_polys[3].p2=0.0F;
   static_env_polys[3].status=F360_STATIC_ENV_POLY_STATUS_INVALID;
   static_env_polys[3].poly_type=F360_STATIC_ENV_POLY_TYPE_INVALID;

   static_env_polys[4].age=0U;
   static_env_polys[4].confidence=0.0F;
   static_env_polys[4].lower_limit=0.0F;
   static_env_polys[4].upper_limit=0.0F;
   static_env_polys[4].p0=0.0F;
   static_env_polys[4].p1=0.0F;
   static_env_polys[4].p2=0.0F;
   static_env_polys[4].status=F360_STATIC_ENV_POLY_STATUS_INVALID;
   static_env_polys[4].poly_type=F360_STATIC_ENV_POLY_TYPE_CWD;

   static_env_polys[5].age=0U;
   static_env_polys[5].confidence=0.0F;
   static_env_polys[5].lower_limit=0.0F;
   static_env_polys[5].upper_limit=0.0F;
   static_env_polys[5].p0=0.0F;
   static_env_polys[5].p1=0.0F;
   static_env_polys[5].p2=0.0F;
   static_env_polys[5].status=F360_STATIC_ENV_POLY_STATUS_INVALID;
   static_env_polys[5].poly_type=F360_STATIC_ENV_POLY_TYPE_INVALID;

   static_env_class.Set_Polynomials(static_env_polys);
}
}
