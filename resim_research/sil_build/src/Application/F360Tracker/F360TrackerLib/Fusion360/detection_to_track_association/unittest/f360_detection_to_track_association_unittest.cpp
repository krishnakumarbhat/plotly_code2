/** \file
 * This file contains unit tests for content of f360_detection_to_track_association.cpp file
 */

#include "f360_detection_to_track_association.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_detection_to_track_association
 *  @{
 */

/** \brief
* This test group includes test of the main function Detection_To_Track_Association() defined in
* f360_detection_to_track_association.cpp
*/
TEST_GROUP(f360_detection_to_track_association)
{
   // Declare common variables used within all tests in this test group.
   F360_Host_T host = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Tracker_Info_T tracker_info = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Calibrations_T calibrations = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS];

   /** \setup
   * Initialize calibrations.
   * Set up a scenario so it is possible to check if something has been modified inside one of the called functions.
   * For example: Detection_Association_Evaluation()
   * - num_active_objs = 1
   * - active_obj_ids[0] = 1
   * Note that there are no detections.
   */
   TEST_SETUP()
   {
      // Set up a default scenario for your tests. E.g. assign values to common variables declared above.
      Initialize_Tracker_Calibrations(calibrations);

      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = 1;
   }
};

/** \purpose
* Test that Detection_To_Track_Association() works as intended when no more detections are allowed to be associated to object with ID 1.
* \req
* NA
*/
TEST(f360_detection_to_track_association, DetectionToTrackAssociation)
{
   /** \precond
    * Set number of associated detections for objext with ID 1 to something non-zero (i.e. 40) 
    * so it can be checked that Detection_Association_Evaluation() has been called and ndets 
    * has been reset to 0 as intended.
    */
   object_tracks[0].ndets = 40;

   /** \action
    * Call Detection_To_Track_Association()
    */
   Detection_To_Track_Association(host, sensors, tracker_info, raw_detection_list, calibrations, sep, detection_props, object_tracks, timing_info);

   /** \result
    * Check that object properties ndets, num_types_of_dets and detids have not been modified. 
    */
   CHECK_EQUAL_TEXT(0, object_tracks[0].ndets, "Object property ndets was modified in an unexpected way.")
}
/** @}*/
