#ifndef SENSOR_FLAG_T_H
#define SENSOR_FLAG_T_H

#include "reuse.h"

typedef struct
{
   uint8_t front_left : 1;
   uint8_t front_center : 1;
   uint8_t front_right : 1;
   uint8_t right_center : 1;
   uint8_t rear_right : 1;
   uint8_t rear_center : 1;
   uint8_t rear_left : 1;
   uint8_t left_center : 1;
}SENSOR_FLAG_T;

#endif
