#ifndef F360_OBJECT_CLASS_PROBABILITY_TYPE_H
#define F360_OBJECT_CLASS_PROBABILITY_TYPE_H

#include "f360_reuse.h"

/**
 * This structure is used to hold the probabilities that an object belongs to a specific object class
 */
namespace f360_variant_A
{
   typedef struct OBJECT_CLASS_PROBABILITY_Tag
   {
      float32_t probability_unknown;    //!< probability that the object class is unknown
      float32_t probability_pedestrian; //!< probability that the object is a pedestrian
      float32_t probability_bicycle;    //!< probability that the object is a bicycle
      float32_t probability_motorcycle;  //!< probability that the object is a motorcycle
      float32_t probability_car;        //!< probability that the object is a car
      float32_t probability_truck;      //!< probability that the object is a truck
   } OBJECT_CLASS_PROBABILITY_T;

   static_assert(24 == sizeof(OBJECT_CLASS_PROBABILITY_T), "sizeof(OBJECT_CLASS_PROBABILITY_T) not as expected. Remember to align padding if needed");
}
#endif
