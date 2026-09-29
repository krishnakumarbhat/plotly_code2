/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef OBJECT_CLASS_PROBABILITY_T_H
#define OBJECT_CLASS_PROBABILITY_T_H

#include "reuse.h"

/**
 * This structure is used to hold the probabilities that an object belongs to a specific object class
 */
typedef struct
{
   float32_T probability_unknown;    //!< probability that the object class is unknown
   float32_T probability_pedestrian; //!< probability that the object is a pedestrian
   float32_T probability_2wheel;     //!< probability that the object is a 2wheel
   float32_T probability_car;        //!< probability that the object is a car
   float32_T probability_truck;      //!< probability that the object is a truck
} OBJECT_CLASS_PROBABILITY_T;

#endif

