/** \file
 * This file contains unit tests for content of f360_iterator.h file
 */
 // Unit testing guidelines: http://confluenceprod1.delphiauto.net:8090/display/F360Core/Unit+testing+guidelines


#include "f360_iterator.h"
#include <CppUTest/TestHarness.h>
#include <vector>
#include <cstdio>

using namespace f360_variant_A;

/** \defgroup  cmn_begin
 *  @{
 */

/** \brief
* This test group is testing cmn::begin function from f360_iterator.h file
 */
TEST_GROUP(cmn_begin){};

/** \purpose  
 * Test cmn::begin with one dimensional array of ints
 * \req
 * NA.
 */
TEST(cmn_begin, shouldReturnAnAdressOfTheFirstElementOfAnArrayOfInts)
{
   /** \precond
    * One Dimensional array of ints is passed as parameter
    */
   int array[1] = {};
   /** \action
    * cmn::begin is called
    */
   int* result = cmn::begin(array);
   /** \result
    * cmn::begin should return adress of the first element of the array.
    */	
   int* expected = &array[0];
   POINTERS_EQUAL(expected, result);
}

/** \purpose
 * Test cmn::begin with one dimensional array of floats
 * \req
 * NA.
 */
TEST(cmn_begin, shouldReturnAnAdressOfTheFirstElementOfAnArrayOfFloats)
{
   /** \precond
    * One Dimensional array of floats is passed as parameter
    */
   float array[17] = {};
   /** \action
    * cmn::begin is called
    */
   float* result = cmn::begin(array);
   /** \result
    * cmn::begin should return adress of the first element of the array.
    */
   float* expected = &array[0];
   POINTERS_EQUAL(expected, result);
}

/** \purpose
 * Test cmn::begin with two dimensional array of chars
 * \req
 * NA.
 */
TEST(cmn_begin, shouldReturnAnAdressOfTheFirstElementOfTwoDimensionalArrayOfChars)
{
   /** \precond
    * Two dimensional array of chars is passed as parameter
    */
   char array[1][1] = {};
   /** \action
    * cmn::begin is called
    */
   char* result = cmn::begin(array);
   /** \result
    * cmn::begin should return adress of the first element of the array.
    */
   char* expected = &array[0][0];
   POINTERS_EQUAL(expected, result);
}

/** \purpose
 * Test cmn::begin with one dimensional array of pointers
 * \req
 * NA.
 */
TEST(cmn_begin, shouldReturnAnAdressOfTheFirstElementOfAnArrayOfpointers)
{
   /** \precond
    * One Dimensional array of pointers is passed as parameter
    */
   void* array[17] = {};
   /** \action
    * cmn::begin is called
    */
   void** result = cmn::begin(array);
   /** \result
    * cmn::begin should return adress of the first element of the array.
    */
   void** expected = &array[0];
   POINTERS_EQUAL(expected, result);
}

/** \purpose
 * Test cmn::begin with three dimensional array of pointers to pointers to pointers
 * \req
 * NA.
 */
TEST(cmn_begin, shouldReturnAnAdressOfTheFirstElementOfAnArrayOfpointersToPointersToPointers)
{
   /** \precond
    * Three Dimensional array of pointers to pointers to pointers is passed as parameter
    */
   void*** array[3][4][1] = {};
   /** \action
    * cmn::begin is called
    */
   void**** result = cmn::begin(array);
   /** \result
    * cmn::begin should return adress of the first element of the array.
    */
   void**** expected = &array[0][0][0];
   POINTERS_EQUAL(expected, result);
}


/** @}*/

/** \defgroup  cmn_end
 *  @{
 */

 /** \brief
 * This test group is testing cmn::end function from f360_iterator.h file
  */
TEST_GROUP(cmn_end){};


/** \purpose
 * Test cmn::end with one dimensional array of ints
 * \req
 * NA.
 */
TEST(cmn_end, shouldReturnIteratorToTheEndOfOneDimensionalArrayOfInts)
{
   /** \precond
    * One Dimensional array of ints is passed as parameter
    */
   const std::size_t array_size = 1;
   int array[array_size] = {};
   /** \action
    * cmn::end is called
    */
   int* result = cmn::end(array);
   /** \result
    * cmn::end should return adress after the last element of the array.
    */
   int* expected = &array[0] + array_size;
   POINTERS_EQUAL(expected, result);
}

/** \purpose
 * Test cmn::end with two dimensional array of floats
 * \req
 * NA.
 */
TEST(cmn_end, shouldReturnIteratorToTheEndOfTwoDimensionalArrayOfFloats)
{
   /** \precond
    *  Two dimensional array of floats is passed as parameter
    */
   const std::size_t first_dimension_size = 7;
   const std::size_t second_dimension_size = 13;
   float array[first_dimension_size][second_dimension_size] = {};
   const std::size_t array_size = first_dimension_size * second_dimension_size;
   /** \action
    * cmn::end is called
    */
   float* result = cmn::end(array);
   /** \result
    * cmn::end should return adress after the last element of the array.
    */
   float* expected = &array[0][0] + array_size;
   POINTERS_EQUAL(expected, result);
}

/** \purpose
 * Test cmn::end with three dimensional array of of pointers to pointers to pointers
 * \req
 * NA.
 */
TEST(cmn_end, shouldReturnIteratorToTheEndOfThreDimensionalArrayOfPointersToPointersToPointers)
{
   /** \precond
    * three Dimensional array of pointers to pointers to pointers is passed as parameter
    */
   const std::size_t first_dimension_size = 9;
   const std::size_t second_dimension_size = 1;
   const std::size_t third_dimension_size = 2;
   void *** array[first_dimension_size][second_dimension_size][third_dimension_size] = {};
   const std::size_t array_size = first_dimension_size * second_dimension_size * third_dimension_size;
   /** \action
    * cmn::end is called
    */
   void **** result = cmn::end(array);
   /** \result
    * cmn::end should return adress after the last element of the array.
    */
   void **** expected = &array[0][0][0] + array_size;
   POINTERS_EQUAL(expected, result);
}

/** @}*/

/** \defgroup  cmn_begin
 *  @{
 */

 /** \brief
 * This test group is testing interaction between cmn::begin and cmn::end function from f360_iterator.h file
  */
TEST_GROUP(cmn_begin_and_end) {};


/** \purpose
 * Test if cmn::begin and cmn::end can be uset to iterate over one dimensional array
 * \req
 * NA.
 */
TEST(cmn_end, iteratingOverOneDimensionalArrayFromBeginToEndShouldGiveAccessToConsecutiveArrayElements)
{
   /** \precond
    * one dimensional array of ints is created
    */
   const std::size_t array_size = 5;
   int array[array_size]= {3,4,1,2,8};
   
   /** \action
    * iteration in the loop from cmn::begin to cmn::end
    */
   std::size_t index = 0;
   for (int* it = cmn::begin(array); it != cmn::end(array); ++it)
   {
    /** \result
    * result of dereferencing iterator is equal to consecutive elements of the array .
    */
      CHECK_EQUAL(array[index], *it);
      index++;
   }
}

/** \purpose
* Test if cmn::begin and cmn::end can be uset to iterate over one dimensional array
* \req
* NA.
*/
TEST(cmn_end, iteratingOverOneDimensionalArrayFromBeginToEndShouldCoverEntireArray)
{
   /** \precond
    * one dimensional array of ints is created
    */
   const std::size_t array_size = 13;
   int array[array_size] = {};

   /** \action
    * iteration in the loop from cmn::begin to cmn::end. 'count' variable is incremented on every iteration
    */
   std::size_t count = 0;
   for (int* it = cmn::begin(array); it != cmn::end(array); ++it)
   {
      count++;
   }
   /** \result
* 'count' variable is equal to array size witch means that loop iterated over whole array .
*/
   CHECK_EQUAL(array_size, count);
}

/** \purpose
 * Test if cmn::begin and cmn::end can be used to iterate over two dimensional array
 * \req
 * NA.
 */
TEST(cmn_end, iteratingOverTwoDimensionalArrayFromBeginToEndShouldCoverEntireArray)
{
   /** \precond
    * two Dimensional array of ints is created
    */
   const std::size_t first_dimension_size = 2;
   const std::size_t second_dimension_size = 3;
   int array[first_dimension_size][second_dimension_size] = { {7,2,1},
                                                             {3,4,5} };
   /** \action
    * iteration in the loop from cmn::begin to cmn::end
    */
   std::size_t index = 0;
   for (int* it = cmn::begin(array); it != cmn::end(array); ++it)
   {
    /** \result
    * result of dereferencing iterator is equal to consecutive elements of the array .
    */
      CHECK_EQUAL(*(&array[0][0] + index), *it);
      index++;
   }
}

/** \purpose
 * Test cmn::end and cmn::begin with std::copy replacing memmove.
 * \req
 * NA.
 */
TEST(cmn_end, memmove_iteratingOverOneDimensionalArrayFromBeginToEndMoveTailToHead)
{
   /** \precond
    * one dimensional array of ints is created
    */
   const std::size_t array_size = 5;
   int array[array_size]= {3,4,1,-2,8};
   int expectedArray[array_size]= {4,1,-2,8,8};

   /** \action
    * Move the n-1 elements to the beginning of array using std_copy.
    * Iteration in the loop from cmn::begin to cmn::end
    */
   std::copy(&array[1], cmn::end(array), cmn::begin(array));
   std::size_t index = 0;
   for (int* it = cmn::begin(array); it != cmn::end(array); ++it)
   {
    /** \result
    * Check that 4 last element have moved to the beginning.
    */
      CHECK_EQUAL(expectedArray[index], *it);
      index++;
   }
}
/** @}*/
