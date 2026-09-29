#ifndef F360_DETECTION_DROP_COUNTERMEASURE_H
#define F360_DETECTION_DROP_COUNTERMEASURE_H

#include "f360_object_track.h"
#include "f360_tracker_info.h"
#include "f360_static_env_poly_types.h"

namespace f360_variant_A
{
   void Flag_Object_Suspectable_For_Detection_Drop_Variant_K(
      const F360_Tracker_Info_T & tracker_info,
      const float32_t host_speed,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]);
   bool Is_Object_Within_Detection_Drop_Zone(
      const F360_Object_Track_T& obj_trk);
}
#endif
