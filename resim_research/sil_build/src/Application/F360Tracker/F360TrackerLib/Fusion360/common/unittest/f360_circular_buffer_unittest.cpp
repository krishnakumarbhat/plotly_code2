/** \file
 * This file contains unit tests for content of f360_circular_buffer.h file
 */
 // Unit testing guidelines: http://confluenceprod1.delphiauto.net:8090/display/F360Core/Unit+testing+guidelines


#include "f360_circular_buffer.h"
#include <CppUTest/TestHarness.h>
using namespace f360_variant_A;

/** \defgroup  cmn_circular_buffer
 *  @{
 */

 /** \brief
 * This test group is testing cmn::Circular_Buffer class
  */
TEST_GROUP(cmn_circular_buffer)
{

};

/** \purpose
 * Test if size of Circular_Buffer is zero after creation
 * \req
 * NA.
 */
TEST(cmn_circular_buffer, SizeIsEqualToZeroAfterCreation)
{
   /** \precond
    * create circular buffer
    */
   cmn::Circular_Buffer<int, 3U> buffer;
   /** \action
    * call size() function
    */
   std::size_t size = buffer.size();
   /** \result
    * return value of empty() is true
    */
   CHECK_EQUAL(0U, size);
}

/** \purpose
 * Test if Circular_Buffer capacity() function returns accual capacity
 * \req
 * NA.
 */
TEST(cmn_circular_buffer, AfterCreationCapacitIsEqualToMaxSize)
{
   /** \precond
    * create circular buffer with capacity equal 3
    */
   constexpr std::size_t buffer_capacity = 3U;
   cmn::Circular_Buffer<int, buffer_capacity> buffer;

   /** \action
    * call capacity() function
    */
   std::size_t capacity = buffer.capacity();

   /** \result
    * return value of capacity() function is equal to 3
    */
   CHECK_EQUAL(buffer_capacity, capacity);
}

/** \purpose
 * Test if Circular_Buffer is not empty after pushing a value to it
 * \req
 * NA.
 */
TEST(cmn_circular_buffer, AfterPushingAValueSizeIsEqualToOne)
{
   /** \precond
    * create circular buffer and push one value
    */
   cmn::Circular_Buffer<int, 5U> buffer;
   buffer.push(int{});
   /** \action
    * call size() function
    */
   std::size_t size = buffer.size();
   /** \result
    * return value of size() is 1
    */
   CHECK_EQUAL(1U, size);
}

/** \purpose
 * test if after pushing element it can be read using operator[]
 * \req
 * NA.
 */
TEST(cmn_circular_buffer, ArrayAccesOperatoreShouldgiveAccesToPushedElement)
{
   /** \precond
    * create circular buffer and push one value
    */
   cmn::Circular_Buffer<int, 4U> buffer;
   const int value_to_push = 32;
   buffer.push(value_to_push);
   /** \action
   * retrive value with index 0 from buffer
   */
   const int returned_value = buffer[0];
   /** \result
   *
   */
   CHECK_EQUAL(value_to_push, returned_value);
}

/** \purpose
 * test if after pushing element it can be read using operator[]
 * \req
 * NA.
 */
TEST(cmn_circular_buffer, ArrayAccesOperatoreShouldgiveAccesToPushedElements)
{
   /** \precond
    * create circular buffer and push three values
    */
   cmn::Circular_Buffer<int, 4U> buffer;
   const int value_to_push_1 = 32;
   const int value_to_push_2 = 35;
   const int value_to_push_3 = 7;
   buffer.push(value_to_push_1);
   buffer.push(value_to_push_2);
   buffer.push(value_to_push_3);
   /** \action
   * retrive value with index 0 from buffer
   */
   const int returned_value = buffer[2];
   /** \result
   *
   */
   CHECK_EQUAL(value_to_push_3, returned_value);
}

/** \purpose
 * test if circular buffer overwrites oldest values when we reach end of capacity
 * \req
 * NA.
 */
TEST(cmn_circular_buffer, OldestValuesShouldBeOverwrittenWhenReachingEndOfCapacity)
{
   /** \precond
    * create circular buffer with capacity of 3 and push five values
    */
   cmn::Circular_Buffer<int, 3U> buffer;
   const int value_to_push_1 = 99;
   const int value_to_push_2 = 14;
   const int value_to_push_3 = -15;
   const int value_to_push_4 = 20;
   const int value_to_push_5 = -7;
   buffer.push(value_to_push_1);
   buffer.push(value_to_push_2);
   buffer.push(value_to_push_3);
   buffer.push(value_to_push_4);
   buffer.push(value_to_push_5);
   /** \action
   * retrive all 3 values from buffer
   */

   const int returned_value_1 = buffer[0];
   const int returned_value_2 = buffer[1];
   const int returned_value_3 = buffer[2];
   /** \result
   * retrived values should be equal to 3 newest elements
   */
   CHECK_EQUAL(value_to_push_3, returned_value_1);
   CHECK_EQUAL(value_to_push_4, returned_value_2);
   CHECK_EQUAL(value_to_push_5, returned_value_3);
}

/** \purpose
 * test if circular buffer overwrites oldest values when we reach end of capacity
 * \req
 * NA.
 */
TEST(cmn_circular_buffer, InOneElementBufferLastPushedValueShouldBeOnIndexZero)
{
   /** \precond
    * create circular buffer with capacity of 1 and push four values
    */
   cmn::Circular_Buffer<int, 1U> buffer;
   const int value_to_push_1 = 5;
   const int value_to_push_2 = 76;
   const int value_to_push_3 = -322;
   const int value_to_push_4 = -8;
   buffer.push(value_to_push_1);
   buffer.push(value_to_push_2);
   buffer.push(value_to_push_3);
   buffer.push(value_to_push_4);
   /** \action
   * retrive value held under index 0
   */

   const int returned_value = buffer[0];

   /** \result
   * retrived value should be equal to last pushed value
   */
   CHECK_EQUAL(value_to_push_4, returned_value);

}

/** \purpose
 * test if after pushing number of values that is equal to capacity we can retrieve them all
 * \req
 * NA.
 */
TEST(cmn_circular_buffer, AfterPushingNumberOfValuesThatIsEqualToCapacityArrayAccessOperatoShouldRetieveThemAll)
{
   /** \precond
    * create circular buffer with capacity of 1 and push four values
    */
   cmn::Circular_Buffer<int, 4U> buffer;
   const int value_to_push_1 = 77;
   const int value_to_push_2 = 999;
   const int value_to_push_3 = -3;
   const int value_to_push_4 = -121;
   buffer.push(value_to_push_1);
   buffer.push(value_to_push_2);
   buffer.push(value_to_push_3);
   buffer.push(value_to_push_4);
   /** \action
   * retrive value held under index 0
   */

   const int returned_value_1 = buffer[0];
   const int returned_value_2 = buffer[1];
   const int returned_value_3 = buffer[2];
   const int returned_value_4 = buffer[3];

   /** \result
   * retrived value should be equal to last pushed value
   */
   CHECK_EQUAL(value_to_push_1, returned_value_1);
   CHECK_EQUAL(value_to_push_2, returned_value_2);
   CHECK_EQUAL(value_to_push_3, returned_value_3);
   CHECK_EQUAL(value_to_push_4, returned_value_4);

}

/** \purpose
 * test if after pushing number of values that is equal to 2 times capacity we can retrieve newest values
 * \req
 * NA.
 */
TEST(cmn_circular_buffer, AfterExactlyTwiceAsManyValuesAsCapacityArrayAccesOperatorShouldGiveAccessToNewestOnes)
{
   /** \precond
    * create circular buffer with capacity of 1 and push four values
    */
   cmn::Circular_Buffer<int, 3U> buffer;
   const int value_to_push_1 = 77;
   const int value_to_push_2 = 999;
   const int value_to_push_3 = -3;
   const int value_to_push_4 = -121;
   const int value_to_push_5 = 17;
   const int value_to_push_6 = -1111;
   buffer.push(value_to_push_1);
   buffer.push(value_to_push_2);
   buffer.push(value_to_push_3);
   buffer.push(value_to_push_4);
   buffer.push(value_to_push_5);
   buffer.push(value_to_push_6);
   /** \action
   * retrive all values
   */

   const int returned_value_1 = buffer[0];
   const int returned_value_2 = buffer[1];
   const int returned_value_3 = buffer[2];


   /** \result
   * retrived values should be equal to last pushed values
   */
   CHECK_EQUAL(value_to_push_4, returned_value_1);
   CHECK_EQUAL(value_to_push_5, returned_value_2);
   CHECK_EQUAL(value_to_push_6, returned_value_3);

}

/** @}*/

