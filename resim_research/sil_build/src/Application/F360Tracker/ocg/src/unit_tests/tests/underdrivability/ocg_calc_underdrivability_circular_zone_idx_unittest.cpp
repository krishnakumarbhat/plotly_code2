/** \file
 * This file contains unit tests for content of ocg_calc_underdrivability_circular_zone_idx.cpp file
 */

#include "cmn_calc_circular_zone_idx.h"
#include <gtest/gtest.h>

#include "ocg_underdrivability_type.h"

using namespace ocg;

/** \defgroup  f360_calc_underdrivability_circular_zone_idx
 *  @{
 */

/** \brief
 * Test group of Calc_Circular_Zone_Idx function. Tests verify whether circular zone is properly calculated.
 */
class f360_calc_underdrivability_circular_zone_idx : public ::testing::Test
{
protected:
   uint16_t circular_buffer_idx {};
   uint32_t zone_idx {};
};

/** \purpose  
 * Purpose of this test is to verify whether circular zone index is properly compensated for circular buffer index.
 * \req
 * NA.
 */
TEST_F(f360_calc_underdrivability_circular_zone_idx, Calc_Circular_Zone_Idx__Compensated_For_Circular_Buffer_Index)
{
   /** \precond
    * Set up zone index.
    * Set up circular zone index.
    */
   zone_idx = 10U;
   circular_buffer_idx = 5U;
	
   /** \action
    * Call tested function.
    */
   const uint32_t circular_zone_idx = cmn::Calc_Circular_Zone_Idx(circular_buffer_idx, zone_idx, NUM_CELLS_X);

   /** \result
    * Check whether returned value is equal to zone_idx + circular_buffer_idx.
    */	
   const uint32_t expected_idx = 15U;
   EXPECT_EQ(expected_idx, circular_zone_idx);
}

/** \purpose
* Purpose of this test is to verify whether circular zone index is properly 
* compensated when it is greater when total number of zones
* \req
* NA.
*/
TEST_F(f360_calc_underdrivability_circular_zone_idx, Calc_Circular_Zone_Idx__Compensated_When_Greater_Than_Total_Number_Of_Zones)
{
   /** \precond
   * Set up zone index.
   * Set up circular zone index to NUM_CELLS_X - 2.
   */
   zone_idx = 10U;
   circular_buffer_idx = NUM_CELLS_X - 2U;

   /** \action
   * Call tested function.
   */
   const uint32_t circular_zone_idx = cmn::Calc_Circular_Zone_Idx(circular_buffer_idx, zone_idx, NUM_CELLS_X);
   /** \result
   * Check whether returned value is equal to zone_idx + circular_buffer_idx - NUM_CELLS_X.
   */
   const uint32_t expected_idx = 8U;
   EXPECT_EQ(expected_idx, circular_zone_idx);
}
/** @}*/
