/** \file
   Give a detailed description of what  this unit-test file contain.
*/

#include "f360_get_unique_rdot_interval_ids.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

using namespace f360_variant_A;
//#include "headerfile_needed.h"

//sneak in mocked functions
//Declaration of stubbed/mock functions

//Implementation of stubbed interfaces

/** \defgroup  f360_get_unique_rdot_interval_ids
 *  @{
 */

/** \brief
*  Add brief description of test group
**/
TEST_GROUP(f360_get_unique_rdot_interval_ids)
{
    F360_Tracker_Info_T tracker_info = { };
    int32_t ndets = 0;
    int32_t num_unique_rdot_intervals = 0;
    float32_t det_rdot_width[MAX_DETS_IN_OBJ_TRK * 2U];
    int32_t rdot_interval_ids[MAX_DETS_IN_OBJ_TRK * 2U];
    int32_t unique_rdot_interval_ids[MAX_NUM_UNIQUE_RDOT_INTERVAL];
    float32_t rdot_interval_width_1 = 69.8359375;
    float32_t rdot_interval_width_2 = 59.6132813;
    float32_t tolerance = 1e-6;

   /** \setup
   * Nothing to setup in this test group
   **/
   TEST_SETUP()
   {
      tracker_info.num_unique_rdot_interval_widths = 2;
      tracker_info.unique_rdot_interval_widths[0] = rdot_interval_width_1;
      tracker_info.unique_rdot_interval_widths[1] = rdot_interval_width_2;
   }

   /** \teardown
   * Nothing to teardown in this test group
   **/
   TEST_TEARDOWN()
   {
      //mock.clear();
   }

};

/**
*\purpose  Describe purpose of test
*\req    put in requirement tag if any otherwise set to NA
*/
TEST(f360_get_unique_rdot_interval_ids, Get_Unique_Rdot_Interval_Ids_no_unique_intervals)
{
   /** \step{1}
    *describe test step
    **/
    unique_rdot_interval_ids[0] = 1;
    unique_rdot_interval_ids[1] = 2;

    rdot_interval_ids[0] = 1;
    rdot_interval_ids[1] = 2;
    det_rdot_width[0] = rdot_interval_width_1 * 2.0F;
    det_rdot_width[1] = rdot_interval_width_2 * 2.0F;
    ndets = 2;

   /** \action
   *describe actions
   **/
   Get_Unique_Rdot_Interval_Ids(tracker_info.unique_rdot_interval_widths, tracker_info.num_unique_rdot_interval_widths, ndets, det_rdot_width, rdot_interval_ids, unique_rdot_interval_ids, num_unique_rdot_intervals);

   /** \result
   *describe test result
   **/
   CHECK_EQUAL(0, num_unique_rdot_intervals);
}

/** @}*/
