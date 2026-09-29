#ifndef F360_LINEAR_SOLVER_DATA_TYPES_H
#define F360_LINEAR_SOLVER_DATA_TYPES_H

#include "f360_reuse.h"
#include "f360_constants.h"

namespace f360_variant_A
{  
   typedef struct F360_matrix_real32_LSC_Tag
   {
      float32_t data[NUMBER_OF_OBJECT_TRACKS][LSC_NR_POLY_COEFF_SLOTS];
      int32_t m_size[2];
      int32_t numDimensions;
   }F360_matrix_real32_LSC_T;

   typedef struct F360_vector_real32_LSC_Tag
   {
      float32_t data[NUMBER_OF_OBJECT_TRACKS];
      int32_t m_size;
      int32_t numDimensions;
   }F360_vector_real32_LSC_T;
}


#endif
