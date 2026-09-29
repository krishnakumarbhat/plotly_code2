/**
 * @file fbk_field_of_interest_factory_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for field of interest implementation functions
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42225}
 */

#include "fbk_field_of_interest_factory_test.hpp"
#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_field_of_interest.h"
#include "fbk_field_of_interest_factory.c"
#include "fbk_macros.h"
#include "fbk_ref_point.h"
#include "ml_float_range_t.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
}

/**
 * Check whether field of interest is created correctly. Here some example points are set up as input and the mapping of those
 * shall be verified. \uts{CSCSA-42297} \sdd{SF-4104} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Create_Field_Of_Interest__Initialize_Valid_Field_Of_View)
{
   /** \arrange set up some example field of views */
   float32_T long_comps[FBK_MAX_SIZE_OF_FOI] = {-5.0f, -1.0f, -3.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f};
   float32_T lat_comps[FBK_MAX_SIZE_OF_FOI]  = {-1.0f, -2.0f, -3.0f, -4.0f, 0.0f, 0.0f, 0.0f, 0.0f};

   field_of_interest_size = 4;

   /** \action call fov constructor */
   Fbk_Create_Field_Of_Interest(&res_foi, long_comps, lat_comps, field_of_interest_size);

   /** \assert check whether points are mapped correctly */
   for (uint8_t i = FBK_ZERO_UINT; i < field_of_interest_size; i++)
   {
      EXPECT_FLOAT_EQ(res_foi.points[i].x, long_comps[i]);
      EXPECT_FLOAT_EQ(res_foi.points[i].y, lat_comps[i]);
   }
   EXPECT_EQ(res_foi.size, field_of_interest_size);
}


#ifndef NDEBUG
/**
 * Check whether initialization routine is throwing an exception when too many points are set as input.
 * \uts{CSCSA-42298} \sdd{SF-4104} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Create_Field_Of_Interest__Initialize_Foi_With_Too_Many_Points)
{
   EXPECT_DEATH(
      {
         /** \arrange set up too many points */
         float32_T long_comps[FBK_MAX_SIZE_OF_FOI] = {0};
         float32_T lat_comps[FBK_MAX_SIZE_OF_FOI]  = {0};

         field_of_interest_size = FBK_MAX_SIZE_OF_FOI + 1;

         /** \action call fov creation */
         Fbk_Create_Field_Of_Interest(&res_foi, long_comps, lat_comps, field_of_interest_size);

         /** \assert check that assertion is thrown */
      },
      ".*field_of_interest_size.*");
}


/**
 * Check whether initialization routine is throwing an exception when longitudinal point array is NULL.
 * \uts{CSCSA-42299} \sdd{SF-4104} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Create_Field_Of_Interest__Long_Comp_Are_Null)
{
   EXPECT_DEATH(
      {
         /** \arrange set longitudinal components to NULL */
         float32_T lat_comps[FBK_MAX_SIZE_OF_FOI] = {0};

         field_of_interest_size = FBK_MAX_SIZE_OF_FOI - 1;

         /** \action call fov creation */
         Fbk_Create_Field_Of_Interest(&res_foi, NULL, lat_comps, field_of_interest_size);

         /** \assert check that assertion is thrown */
      },
      ".*p_x.*");
}

/**
 * Check whether initialization routine is throwing an exception when lateral point array is NULL.
 * \uts{CSCSA-42300} \sdd{SF-4104} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Create_Field_Of_Interest__Lat_Comp_Are_Null)
{
   EXPECT_DEATH(
      {
         /** \arrange set lateral components to NULL */
         float32_T long_comps[FBK_MAX_SIZE_OF_FOI] = {0};

         field_of_interest_size = FBK_MAX_SIZE_OF_FOI - 1;

         /** \action call fov creation */
         Fbk_Create_Field_Of_Interest(&res_foi, long_comps, NULL, field_of_interest_size);

         /** \assert check that assertion is thrown */
      },
      ".*p_y.*");
}

/**
 * Check whether initialization routine is throwing an exception when field of view pointer is NULL.
 * \uts{CSCSA-42301} \sdd{SF-4104} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Create_Field_Of_Interest__Field_Of_View_Ptr_Is_Null)
{
   EXPECT_DEATH(
      {
         /** \arrange set up field of view pointer is null */
         float32_T long_comps[FBK_MAX_SIZE_OF_FOI] = {0};
         float32_T lat_comps[FBK_MAX_SIZE_OF_FOI]  = {0};

         field_of_interest_size = FBK_MAX_SIZE_OF_FOI - 1;

         /** \action call fov creation */
         Fbk_Create_Field_Of_Interest(NULL, long_comps, lat_comps, field_of_interest_size);

         /** \assert check that assertion is thrown */
      },
      ".*p_res_foi.*");
}
#endif

/**
 * Check whether field of interest is reset correctly when called.
 * \uts{CSCSA-42302} \sdd{SF-4105} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Reset_Field_Of_Interest__resets_field_of_interest_properly)
{
   /** \arrange set up a non default field of interest */
   float32_T long_comps[FBK_MAX_SIZE_OF_FOI] = {-5.0f, -1.0f, -3.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f};
   float32_T lat_comps[FBK_MAX_SIZE_OF_FOI]  = {-1.0f, -2.0f, -3.0f, -4.0f, 0.0f, 0.0f, 0.0f, 0.0f};

   field_of_interest_size = 4;
   Fbk_Create_Field_Of_Interest(&res_foi, long_comps, lat_comps, field_of_interest_size);

   /** \action call reset routine */
   Fbk_Reset_Field_Of_Interest(&res_foi);

   /** \assert expect that field of view is containing default values */
   for (uint8_t i = FBK_ZERO_UINT; i < field_of_interest_size; i++)
   {
      EXPECT_FLOAT_EQ(res_foi.points[i].x, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(res_foi.points[i].y, FBK_ZERO_F);
   }
   EXPECT_EQ(res_foi.size, FBK_ZERO_UINT);
}


#ifndef NDEBUG

/**
 * Check whether initialization routine is throwing an exception when input is NULL.
 * \uts{CSCSA-42303} \sdd{SF-4105} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Reset_Field_Of_Interest__input_array_is_null)
{
   EXPECT_DEATH(
      {
         /** \arrange set up NULL pointer */
         /** \action call fov reset */
         Fbk_Reset_Field_Of_Interest(NULL);

         /** \assert check that assertion is thrown */
      },
      ".*p_res_foi.*");
}
#endif

/**
 * Verify that area calculation of a field of interest is zero, when only zeros are provided as an input.
 * \uts{CSCSA-42304} \sdd{SF-4106} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Area_Field_Of_Interest__return_zero_area_value)
{
   /** \arrange set up default component arrays */
   float32_T long_comps[FBK_MAX_SIZE_OF_FOI] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
   float32_T lat_comps[FBK_MAX_SIZE_OF_FOI]  = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

   field_of_interest_size = 4;
   Fbk_Create_Field_Of_Interest(&res_foi, long_comps, lat_comps, field_of_interest_size);

   /** \action call area calculation routine of fov */
   float32_T returned_area = Fbk_Get_Area_Field_Of_Interest(&res_foi);

   /** \assert expect 0 */
   EXPECT_FLOAT_EQ(returned_area, FBK_ZERO_F);
}


/**
 * Verify that area calculation of a field of interest is zero, when a zone is given which is not enclosing any area.
 * \uts{CSCSA-42305} \sdd{SF-4106} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Area_Field_Of_Interest__return_zero_since_zone_consists_if_two_points)
{
   /** \arrange set up default component arrays */
   float32_T long_comps[FBK_MAX_SIZE_OF_FOI] = {5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
   float32_T lat_comps[FBK_MAX_SIZE_OF_FOI]  = {1.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

   field_of_interest_size = 2;
   Fbk_Create_Field_Of_Interest(&res_foi, long_comps, lat_comps, field_of_interest_size);

   /** \action call area calculation routine of fov */
   float32_T returned_area = Fbk_Get_Area_Field_Of_Interest(&res_foi);

   /** \assert expect 0 */
   EXPECT_FLOAT_EQ(returned_area, FBK_ZERO_F);
}


/**
 * Verify that area calculation of a field of interest is greater than zero when rectangle in the positive right half plane is
 * provided as input. \uts{CSCSA-42306} \sdd{SF-4106} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Area_Field_Of_Interest__return_correct_positive_area_value_rectangle)
{
   /** \arrange set up components in the positive right half plane */
   float32_T long_comps[FBK_MAX_SIZE_OF_FOI] = {5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
   float32_T lat_comps[FBK_MAX_SIZE_OF_FOI]  = {0.0f, 3.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

   field_of_interest_size = 4;
   Fbk_Create_Field_Of_Interest(&res_foi, long_comps, lat_comps, field_of_interest_size);

   /** \action call area calculation routine of fov */
   float32_T returned_area = Fbk_Get_Area_Field_Of_Interest(&res_foi);

   /** \assert Expect returned_area to be 5 3 = 15 */
   EXPECT_FLOAT_EQ(returned_area, 15.0f);
}

/**
 * Verify that area calculation of a field of interest is greater than zero when tetragon in the positive right half plane is
 * provided as input. \uts{CSCSA-42307} \sdd{SF-4106} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Area_Field_Of_Interest__return_correct_positive_area_value_tetragon)
{
   /** \arrange set up components in the positive right half plane */
   float32_T long_comps[FBK_MAX_SIZE_OF_FOI] = {0.0f, 4.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f};
   float32_T lat_comps[FBK_MAX_SIZE_OF_FOI]  = {0.0f, 0.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f};

   field_of_interest_size = 4;
   Fbk_Create_Field_Of_Interest(&res_foi, long_comps, lat_comps, field_of_interest_size);

   /** \action call area calculation routine of fov */
   float32_T returned_area = Fbk_Get_Area_Field_Of_Interest(&res_foi);

   /** \assert Expect returned_area to be 5 */
   EXPECT_FLOAT_EQ(returned_area, 5.0f);
}

/**
 * Verify that area calculation of a field of interest is greater than zero when rectangle in the negative left half plane is
 * provided as input. \uts{CSCSA-42308} \sdd{SF-4106} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Area_Field_Of_Interest__return_correct_negative_area_value_rectangle)
{
   /** \arrange set up components in the negative left half plane */
   float32_T long_comps[FBK_MAX_SIZE_OF_FOI] = {-5.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
   float32_T lat_comps[FBK_MAX_SIZE_OF_FOI]  = {0.0f, -3.0f, -3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

   field_of_interest_size = 4;
   Fbk_Create_Field_Of_Interest(&res_foi, long_comps, lat_comps, field_of_interest_size);

   /** \action call area calculation routine of fov */
   float32_T returned_area = Fbk_Get_Area_Field_Of_Interest(&res_foi);

   /** \assert Expect returned_area to be 5 3 = 15 */
   EXPECT_FLOAT_EQ(returned_area, 15.0f);
}

/**
 * Verify that area calculation of a field of interest is greater than zero when triangle in the positive right half plane is
 * provided as input. \uts{CSCSA-42309} \sdd{SF-4106} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Area_Field_Of_Interest__return_correct_positive_area_value_triangle)
{
   /** \arrange set up triangle in the positive right half plane */
   float32_T long_comps[FBK_MAX_SIZE_OF_FOI] = {5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
   float32_T lat_comps[FBK_MAX_SIZE_OF_FOI]  = {0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

   field_of_interest_size = 3;
   Fbk_Create_Field_Of_Interest(&res_foi, long_comps, lat_comps, field_of_interest_size);

   /** \action call area calculation routine of fov */
   float32_T returned_area = Fbk_Get_Area_Field_Of_Interest(&res_foi);

   /** \assert Expect returned_area to be half of 5 3 = 15, thus 7.5. */
   EXPECT_FLOAT_EQ(returned_area, 7.5f);
}

/**
 * Verify that area calculation of a field of interest is greater than zero when triangle in the negative left half plane is
 * provided as input. \uts{CSCSA-42310} \sdd{SF-4106} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Area_Field_Of_Interest__return_correct_negative_area_value_triangle)
{
   /** \arrange set up triangle in the negative left half plane */
   float32_T long_comps[FBK_MAX_SIZE_OF_FOI] = {-5.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
   float32_T lat_comps[FBK_MAX_SIZE_OF_FOI]  = {0.0f, -3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

   field_of_interest_size = 3;
   Fbk_Create_Field_Of_Interest(&res_foi, long_comps, lat_comps, field_of_interest_size);

   /** \action call area calculation routine of fov */
   float32_T returned_area = Fbk_Get_Area_Field_Of_Interest(&res_foi);

   /** \assert Expect returned_area to be half of 5 3 = 15, thus 7.5 */
   EXPECT_FLOAT_EQ(returned_area, 7.5f);
}


#ifndef NDEBUG

/**
 * Check whether area calculation routine is throwing an exception when input is NULL.
 * \uts{CSCSA-42311} \sdd{SF-4106} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Area_Field_Of_Interest__input_array_is_null)
{
   EXPECT_DEATH(
      {
         /** \arrange set up NULL pointer */
         /** \action call area calculation method */
         Fbk_Get_Area_Field_Of_Interest(NULL);

         /** \assert check that assertion is thrown */
      },
      ".*p_field_of_interest.*");
}
#endif

/**
 * Verify that the correct field of interest is returned for the given object data.
 * \uts{CSCSA-42312} \sdd{SF-4178} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Create_Field_Of_Interest_From_Object_Data__return_correct_foi_for_object)
{
   /** \arrange set up object data */
   Fbk_Field_Of_Interest_T object_zone;
   Vector_2d_T object_position{};
   float32_T object_length  = 4.0f;
   float32_T object_width   = 2.0f;
   float32_T object_heading = 0.0f;

   /** \action call FBK function */
   Fbk_Create_Field_Of_Interest_From_Object_Data(&object_zone, object_position, object_length, object_width, object_heading);

   /** \assert Expect correct dimensions of object field of interest. */
   EXPECT_EQ(object_zone.size, 4u);
   EXPECT_FLOAT_EQ(object_zone.points[0].x, 2.0f);
   EXPECT_FLOAT_EQ(object_zone.points[0].y, -1.0f);
   EXPECT_FLOAT_EQ(object_zone.points[1].x, 2.0f);
   EXPECT_FLOAT_EQ(object_zone.points[1].y, 1.0f);
   EXPECT_FLOAT_EQ(object_zone.points[2].x, -2.0f);
   EXPECT_FLOAT_EQ(object_zone.points[2].y, 1.0f);
   EXPECT_FLOAT_EQ(object_zone.points[3].x, -2.0f);
   EXPECT_FLOAT_EQ(object_zone.points[3].y, -1.0f);
}

/**
 * Verify that the correct boundary box is returned for the given zone data.
 * \uts{CSCSA-42313} \sdd{SF-4179} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Field_Of_Interest_Bounding_Box__return_correct_bounding_box)
{
   /** \arrange set up zone data */
   Fbk_Field_Of_Interest_T test_zone;
   test_zone.size        = 4u;
   test_zone.points[0].x = 5.0f;
   test_zone.points[0].y = 0.0f;
   test_zone.points[1].x = 5.0f;
   test_zone.points[1].y = 2.0f;
   test_zone.points[2].x = 0.0f;
   test_zone.points[2].y = 2.0f;
   test_zone.points[3].x = 0.0f;
   test_zone.points[3].y = 0.0f;

   /** \action call FBK function */
   Fbk_Bounding_Box_T bbox = Fbk_Get_Field_Of_Interest_Bounding_Box(&test_zone);

   /** \assert Expect correct dimensions of the bounding box */
   EXPECT_FLOAT_EQ(bbox.x.min, 0.0f);
   EXPECT_FLOAT_EQ(bbox.x.max, 5.0f);
   EXPECT_FLOAT_EQ(bbox.y.min, 0.0f);
   EXPECT_FLOAT_EQ(bbox.y.max, 2.0f);
}


/**
 * Verify that the correct result is returned for overlapping bounding boxes.
 * \uts{CSCSA-42314} \sdd{SF-4180} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Are_Bounding_Boxes_Overlapping__bounding_boxes_are_overlapping)
{
   /** \arrange set up bounding box data */
   Fbk_Bounding_Box_T bbox_a;
   Fbk_Bounding_Box_T bbox_b;

   bbox_a.x.min = 5.0f;
   bbox_a.x.max = 10.0f;
   bbox_a.y.min = 5.0f;
   bbox_a.y.max = 10.0f;

   bbox_b = bbox_a;

   /** \action call FBK function */
   boolean_T result = Fbk_Are_Bounding_Boxes_Overlapping(&bbox_a, &bbox_b);

   /** \assert Expect correct result */
   EXPECT_TRUE(result);
}

/**
 * Verify that the correct result is returned for non-overlapping bounding boxes.
 * \uts{CSCSA-42315} \sdd{SF-4180} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Are_Bounding_Boxes_Overlapping__bounding_boxes_are_not_overlapping)
{
   /** \arrange set up bounding box data */
   Fbk_Bounding_Box_T bbox_a;
   Fbk_Bounding_Box_T bbox_b;

   bbox_a.x.min = 5.0f;
   bbox_a.x.max = 10.0f;
   bbox_a.y.min = 5.0f;
   bbox_a.y.max = 10.0f;

   bbox_b.x.min = 15.0f;
   bbox_b.x.max = 20.0f;
   bbox_b.y.min = 15.0f;
   bbox_b.y.max = 20.0f;

   /** \action call FBK function */
   boolean_T result = Fbk_Are_Bounding_Boxes_Overlapping(&bbox_a, &bbox_b);

   /** \assert Expect correct result */
   EXPECT_FALSE(result);
}

/**
 * Verify that the correct result is returned for non-overlapping bounding boxes in longitudinal direction.
 * \uts{CSCSA-186069} \sdd{SF-4180} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Are_Bounding_Boxes_Overlapping__bounding_boxes_are_not_overlapping_longitudinaly)
{
   /** \arrange set up bounding box data */
   Fbk_Bounding_Box_T bbox_a;
   Fbk_Bounding_Box_T bbox_b;

   bbox_a.x.min = 5.0f;
   bbox_a.x.max = 10.0f;
   bbox_a.y.min = 5.0f;
   bbox_a.y.max = 10.0f;

   bbox_b.x.min = 15.0f;
   bbox_b.x.max = 20.0f;
   bbox_b.y.min = 5.0f;
   bbox_b.y.max = 10.0f;

   /** \action call FBK function */
   boolean_T result = Fbk_Are_Bounding_Boxes_Overlapping(&bbox_a, &bbox_b);

   /** \assert Expect correct result */
   EXPECT_FALSE(result);
}

/**
 * Verify that the correct result is returned for non-overlapping bounding boxes in lateral direction.
 * \uts{CSCSA-186071} \sdd{SF-4180} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Are_Bounding_Boxes_Overlapping__bounding_boxes_are_not_overlapping_lateraly)
{
   /** \arrange set up bounding box data */
   Fbk_Bounding_Box_T bbox_a;
   Fbk_Bounding_Box_T bbox_b;

   bbox_a.x.min = 5.0f;
   bbox_a.x.max = 10.0f;
   bbox_a.y.min = 5.0f;
   bbox_a.y.max = 10.0f;

   bbox_b.x.min = 5.0f;
   bbox_b.x.max = 10.0f;
   bbox_b.y.min = 15.0f;
   bbox_b.y.max = 20.0f;

   /** \action call FBK function */
   boolean_T result = Fbk_Are_Bounding_Boxes_Overlapping(&bbox_a, &bbox_b);

   /** \assert Expect correct result */
   EXPECT_FALSE(result);
}

/**
 * Verify that the correct result is returned for overlapping fields of interest. FoI A center point is in FoI B.
 * \uts{CSCSA-42316} \sdd{SF-4181} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Are_Fields_Of_Interest_Overlapping__fields_are_overlapping_foi_a_center_in_foi_b)
{
   /** \arrange set up field of interest data */
   Fbk_Field_Of_Interest_T foi_a;
   Fbk_Field_Of_Interest_T foi_b;

   foi_a.size        = 4u;
   foi_a.points[0].x = 5.0f;
   foi_a.points[0].y = 0.0f;
   foi_a.points[1].x = 5.0f;
   foi_a.points[1].y = 2.0f;
   foi_a.points[2].x = 0.0f;
   foi_a.points[2].y = 2.0f;
   foi_a.points[3].x = 0.0f;
   foi_a.points[3].y = 0.0f;

   foi_b = foi_a;

   /** \action call FBK function */
   boolean_T result = Fbk_Are_Fields_Of_Interest_Overlapping(&foi_a, &foi_b);

   /** \assert Expect correct result */
   EXPECT_TRUE(result);
}

/**
 * Verify that the correct result is returned for overlapping fields of interest. FoI B center point is in FoI A.
 * \uts{CSCSA-42317} \sdd{SF-4181} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Are_Fields_Of_Interest_Overlapping__fields_are_overlapping_foi_b_center_in_foi_a)
{
   /** \arrange set up field of interest data */
   Fbk_Field_Of_Interest_T foi_a;
   Fbk_Field_Of_Interest_T foi_b;

   foi_a.size        = 4u;
   foi_a.points[0].x = 5.0f;
   foi_a.points[0].y = 0.0f;
   foi_a.points[1].x = 5.0f;
   foi_a.points[1].y = 2.0f;
   foi_a.points[2].x = 0.0f;
   foi_a.points[2].y = 2.0f;
   foi_a.points[3].x = 0.0f;
   foi_a.points[3].y = 0.0f;

   foi_b.size        = 4u;
   foi_b.points[0].x = 0.5f;
   foi_b.points[0].y = 0.0f;
   foi_b.points[1].x = 0.5f;
   foi_b.points[1].y = 0.2f;
   foi_b.points[2].x = 0.0f;
   foi_b.points[2].y = 0.2f;
   foi_b.points[3].x = 0.0f;
   foi_b.points[3].y = 0.0f;

   /** \action call FBK function */
   boolean_T result = Fbk_Are_Fields_Of_Interest_Overlapping(&foi_a, &foi_b);

   /** \assert Expect correct result */
   EXPECT_TRUE(result);
}

/**
 * Verify that the correct result is returned for overlapping fields of interest. FoI A overlaps Foi B with all corners and center
 * point outside of Foi B. \uts{CSCSA-186072} \sdd{SF-4181} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test,
       Fbk_Are_Fields_Of_Interest_Overlapping__fields_are_overlapping_foi_a_stick_out_on_both_sides_of_foi_b)
{
   /** \arrange set up field of interest data */
   Fbk_Field_Of_Interest_T foi_a;
   Fbk_Field_Of_Interest_T foi_b;

   foi_a.size        = 4u;
   foi_a.points[0].x = 1.0f;
   foi_a.points[0].y = 0.0f;
   foi_a.points[1].x = 5.0f;
   foi_a.points[1].y = 0.0f;
   foi_a.points[2].x = 5.0f;
   foi_a.points[2].y = 5.0f;
   foi_a.points[3].x = 1.0f;
   foi_a.points[3].y = 5.0f;

   foi_b.size        = 4u;
   foi_b.points[0].x = 0.0f;
   foi_b.points[0].y = 1.0f;
   foi_b.points[1].x = 12.0f;
   foi_b.points[1].y = 1.0f;
   foi_b.points[2].x = 12.0f;
   foi_b.points[2].y = 3.0f;
   foi_b.points[3].x = 0.0f;
   foi_b.points[3].y = 3.0f;

   /** \action call FBK function */
   boolean_T result = Fbk_Are_Fields_Of_Interest_Overlapping(&foi_a, &foi_b);

   /** \assert Expect correct result */
   EXPECT_TRUE(result);
}

/**
 * Verify that the correct result is returned for overlapping fields of interest. FoI B overlaps Foi A with all corners and center
 * point outside of Foi A. \uts{CSCSA-186073} \sdd{SF-4181} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test,
       Fbk_Are_Fields_Of_Interest_Overlapping__fields_are_overlapping_foi_b_stick_out_on_both_sides_of_foi_a)
{
   /** \arrange set up field of interest data */
   Fbk_Field_Of_Interest_T foi_a;
   Fbk_Field_Of_Interest_T foi_b;

   foi_a.size        = 4u;
   foi_a.points[0].x = 1.0f;
   foi_a.points[0].y = 0.0f;
   foi_a.points[1].x = 5.0f;
   foi_a.points[1].y = 0.0f;
   foi_a.points[2].x = 5.0f;
   foi_a.points[2].y = 5.0f;
   foi_a.points[3].x = 1.0f;
   foi_a.points[3].y = 5.0f;

   foi_b.size        = 4u;
   foi_b.points[0].x = 0.0f;
   foi_b.points[0].y = 1.0f;
   foi_b.points[1].x = 12.0f;
   foi_b.points[1].y = 1.0f;
   foi_b.points[2].x = 12.0f;
   foi_b.points[2].y = 3.0f;
   foi_b.points[3].x = 0.0f;
   foi_b.points[3].y = 3.0f;

   /** \action call FBK function */
   boolean_T result = Fbk_Are_Fields_Of_Interest_Overlapping(&foi_b, &foi_a);

   /** \assert Expect correct result */
   EXPECT_TRUE(result);
}


/**
 * Verify that the correct result is returned for overlapping fields of interest. FoI A overlaps Foi B with all corners and center
 * point outside of Foi B and corner lateraly don't intersect. \uts{CSCSA-186074} \sdd{SF-4181} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Are_Fields_Of_Interest_Overlapping__long_foi_a_overlap_lateral_range_dont_intersect)
{
   /** \arrange set up field of interest data */
   Fbk_Field_Of_Interest_T foi_a;
   Fbk_Field_Of_Interest_T foi_b;

   foi_a.size        = 4u;
   foi_a.points[0].x = 0.0f;
   foi_a.points[0].y = -3.0f;
   foi_a.points[1].x = 1.0f;
   foi_a.points[1].y = -3.0f;
   foi_a.points[2].x = 1.0f;
   foi_a.points[2].y = 3.0f;
   foi_a.points[3].x = 0.0f;
   foi_a.points[3].y = 3.0f;

   foi_b.size        = 4u;
   foi_b.points[0].x = -1.0f;
   foi_b.points[0].y = 0.5f;
   foi_b.points[1].x = -1.0f;
   foi_b.points[1].y = 1.5f;
   foi_b.points[2].x = 10.0f;
   foi_b.points[2].y = 7.0f;
   foi_b.points[3].x = 10.0f;
   foi_b.points[3].y = 6.0f;

   /** \action call FBK function */
   boolean_T result = Fbk_Are_Fields_Of_Interest_Overlapping(&foi_a, &foi_b);

   /** \assert Expect correct result */
   EXPECT_FALSE(result);
}


/**
 * Verify that the correct result is returned for overlapping fields of interest. FoI B overlaps Foi A with all corners and center
 * point outside of Foi A and corner lateraly don't intersect. \uts{CSCSA-186075} \sdd{SF-4181} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Are_Fields_Of_Interest_Overlapping__long_foi_b_overlap_lateral_range_dont_intersect)
{
   /** \arrange set up field of interest data */
   Fbk_Field_Of_Interest_T foi_a;
   Fbk_Field_Of_Interest_T foi_b;

   foi_a.size        = 4u;
   foi_a.points[0].x = 0.0f;
   foi_a.points[0].y = -3.0f;
   foi_a.points[1].x = 1.0f;
   foi_a.points[1].y = -3.0f;
   foi_a.points[2].x = 1.0f;
   foi_a.points[2].y = 3.0f;
   foi_a.points[3].x = 0.0f;
   foi_a.points[3].y = 3.0f;

   foi_b.size        = 4u;
   foi_b.points[0].x = -1.0f;
   foi_b.points[0].y = 0.5f;
   foi_b.points[1].x = -1.0f;
   foi_b.points[1].y = 1.5f;
   foi_b.points[2].x = 10.0f;
   foi_b.points[2].y = 7.0f;
   foi_b.points[3].x = 10.0f;
   foi_b.points[3].y = 6.0f;

   /** \action call FBK function */
   boolean_T result = Fbk_Are_Fields_Of_Interest_Overlapping(&foi_b, &foi_a);

   /** \assert Expect correct result */
   EXPECT_FALSE(result);
}


/**
 * Verify that the correct result is returned for overlapping fields of interest. FoI A corner point is in FoI B.
 * \uts{CSCSA-42318} \sdd{SF-4181} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Are_Fields_Of_Interest_Overlapping__fields_are_overlapping_foi_a_corner_in_foi_b)
{
   /** \arrange set up field of interest data */
   Fbk_Field_Of_Interest_T foi_a;
   Fbk_Field_Of_Interest_T foi_b;

   foi_a.size        = 4u;
   foi_a.points[0].x = 5.0f;
   foi_a.points[0].y = 0.0f;
   foi_a.points[1].x = 5.0f;
   foi_a.points[1].y = 2.0f;
   foi_a.points[2].x = 0.0f;
   foi_a.points[2].y = 2.0f;
   foi_a.points[3].x = 0.0f;
   foi_a.points[3].y = 0.0f;

   foi_b.size        = 4u;
   foi_b.points[0].x = 10.0f;
   foi_b.points[0].y = 2.0f - EPSILON;
   foi_b.points[1].x = 10.0f;
   foi_b.points[1].y = 4.0f;
   foi_b.points[2].x = 5.0f - EPSILON;
   foi_b.points[2].y = 4.0f;
   foi_b.points[3].x = 5.0f - EPSILON;
   foi_b.points[3].y = 2.0f - EPSILON;

   /** \action call FBK function */
   boolean_T result = Fbk_Are_Fields_Of_Interest_Overlapping(&foi_a, &foi_b);

   /** \assert Expect correct result */
   EXPECT_TRUE(result);
}

/**
 * Verify that the correct result is returned for overlapping fields of interest. FoI B corner point is in FoI A.
 * \uts{CSCSA-42319} \sdd{SF-4181} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Are_Fields_Of_Interest_Overlapping__fields_are_overlapping_foi_b_corner_in_foi_a)
{
   /** \arrange set up field of interest data */
   Fbk_Field_Of_Interest_T foi_a;
   Fbk_Field_Of_Interest_T foi_b;

   foi_a.size        = 4u;
   foi_a.points[0].x = 5.0f;
   foi_a.points[0].y = 0.0f;
   foi_a.points[1].x = 5.0f;
   foi_a.points[1].y = 2.0f;
   foi_a.points[2].x = 0.0f;
   foi_a.points[2].y = 2.0f;
   foi_a.points[3].x = 0.0f;
   foi_a.points[3].y = 0.0f;

   foi_b.size        = 4u;
   foi_b.points[0].x = 4.0f;
   foi_b.points[0].y = 1.9f;
   foi_b.points[1].x = 4.0f;
   foi_b.points[1].y = 3.0f;
   foi_b.points[2].x = 1.0f;
   foi_b.points[2].y = 3.0f;
   foi_b.points[3].x = 1.0f;
   foi_b.points[3].y = 1.9f;

   /** \action call FBK function */
   boolean_T result = Fbk_Are_Fields_Of_Interest_Overlapping(&foi_a, &foi_b);

   /** \assert Expect correct result */
   EXPECT_TRUE(result);
}

/**
 * Verify that the correct result is returned for overlapping fields of interest. Here foi A is behind foi B.
 * \uts{CSCSA-186076} \sdd{SF-4181} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Are_Fields_Of_Interest_Overlapping__fields_are_not_overlapping_foi_a_behind_foi_b)
{
   /** \arrange set up field of interest data */
   Fbk_Field_Of_Interest_T foi_a;
   Fbk_Field_Of_Interest_T foi_b;

   foi_a.size        = 4u;
   foi_a.points[0].x = 0.0f;
   foi_a.points[0].y = 1.0f;
   foi_a.points[1].x = 2.0f;
   foi_a.points[1].y = 1.0f;
   foi_a.points[2].x = 2.0f;
   foi_a.points[2].y = 2.0f;
   foi_a.points[3].x = 0.0f;
   foi_a.points[3].y = 2.0f;

   foi_b.size        = 4u;
   foi_b.points[0].x = 3.0f;
   foi_b.points[0].y = 1.0f;
   foi_b.points[1].x = 5.0f;
   foi_b.points[1].y = 1.0f;
   foi_b.points[2].x = 5.0f;
   foi_b.points[2].y = 2.0f;
   foi_b.points[3].x = 3.0f;
   foi_b.points[3].y = 2.0f;

   /** \action call FBK function */
   boolean_T result = Fbk_Are_Fields_Of_Interest_Overlapping(&foi_a, &foi_b);

   /** \assert Expect correct result */
   EXPECT_FALSE(result);
}

/**
 * Verify that the correct result is returned for overlapping fields of interest. Here foi B is behind foi A.
 * \uts{CSCSA-186077} \sdd{SF-4181} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Are_Fields_Of_Interest_Overlapping__fields_are_not_overlapping_foi_b_behind_foi_a)
{
   /** \arrange set up field of interest data */
   Fbk_Field_Of_Interest_T foi_a;
   Fbk_Field_Of_Interest_T foi_b;

   foi_a.size        = 4u;
   foi_a.points[0].x = 0.0f;
   foi_a.points[0].y = 1.0f;
   foi_a.points[1].x = 2.0f;
   foi_a.points[1].y = 1.0f;
   foi_a.points[2].x = 2.0f;
   foi_a.points[2].y = 2.0f;
   foi_a.points[3].x = 0.0f;
   foi_a.points[3].y = 2.0f;

   foi_b.size        = 4u;
   foi_b.points[0].x = 3.0f;
   foi_b.points[0].y = 1.0f;
   foi_b.points[1].x = 5.0f;
   foi_b.points[1].y = 1.0f;
   foi_b.points[2].x = 5.0f;
   foi_b.points[2].y = 2.0f;
   foi_b.points[3].x = 3.0f;
   foi_b.points[3].y = 2.0f;

   /** \action call FBK function */
   boolean_T result = Fbk_Are_Fields_Of_Interest_Overlapping(&foi_b, &foi_a);

   /** \assert Expect correct result */
   EXPECT_FALSE(result);
}


/**
 * Verify that the correct result is returned for overlapping fields of interest. Here foi A is beside foi B.
 * \uts{CSCSA-186078} \sdd{SF-4181} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Are_Fields_Of_Interest_Overlapping__fields_are_not_overlapping_foi_a_beside_foi_b)
{
   /** \arrange set up field of interest data */
   Fbk_Field_Of_Interest_T foi_a;
   Fbk_Field_Of_Interest_T foi_b;

   foi_a.size        = 4u;
   foi_a.points[0].x = 2.0f;
   foi_a.points[0].y = 1.0f;
   foi_a.points[1].x = 4.0f;
   foi_a.points[1].y = 1.0f;
   foi_a.points[2].x = 4.0f;
   foi_a.points[2].y = 2.0f;
   foi_a.points[3].x = 2.0f;
   foi_a.points[3].y = 2.0f;

   foi_b.size        = 4u;
   foi_b.points[0].x = 1.0f;
   foi_b.points[0].y = 3.0f;
   foi_b.points[1].x = 5.0f;
   foi_b.points[1].y = 3.0f;
   foi_b.points[2].x = 5.0f;
   foi_b.points[2].y = 4.0f;
   foi_b.points[3].x = 1.0f;
   foi_b.points[3].y = 4.0f;

   /** \action call FBK function */
   boolean_T result = Fbk_Are_Fields_Of_Interest_Overlapping(&foi_a, &foi_b);

   /** \assert Expect correct result */
   EXPECT_FALSE(result);
}

/**
 * Verify that the correct result is returned for overlapping fields of interest. Here foi B is beside foi A.
 * \uts{CSCSA-186079} \sdd{SF-4181} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Are_Fields_Of_Interest_Overlapping__fields_are_not_overlapping_foi_b_beside_foi_a)
{
   /** \arrange set up field of interest data */
   Fbk_Field_Of_Interest_T foi_a;
   Fbk_Field_Of_Interest_T foi_b;

   foi_a.size        = 4u;
   foi_a.points[0].x = 2.0f;
   foi_a.points[0].y = 1.0f;
   foi_a.points[1].x = 4.0f;
   foi_a.points[1].y = 1.0f;
   foi_a.points[2].x = 4.0f;
   foi_a.points[2].y = 2.0f;
   foi_a.points[3].x = 2.0f;
   foi_a.points[3].y = 2.0f;

   foi_b.size        = 4u;
   foi_b.points[0].x = 1.0f;
   foi_b.points[0].y = 3.0f;
   foi_b.points[1].x = 5.0f;
   foi_b.points[1].y = 3.0f;
   foi_b.points[2].x = 5.0f;
   foi_b.points[2].y = 4.0f;
   foi_b.points[3].x = 1.0f;
   foi_b.points[3].y = 4.0f;

   /** \action call FBK function */
   boolean_T result = Fbk_Are_Fields_Of_Interest_Overlapping(&foi_b, &foi_a);

   /** \assert Expect correct result */
   EXPECT_FALSE(result);
}


/**
 * Verify that the correct result is returned for overlapping fields of interest. Here 2 lines are tested. False is expected.
 * \uts{CSCSA-42321} \sdd{SF-4181} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Are_Fields_Of_Interest_Overlapping__two_point_fields_are_not_overlapping)
{
   /** \arrange set up field of interest data */
   Fbk_Field_Of_Interest_T foi_a;
   Fbk_Field_Of_Interest_T foi_b;

   foi_a.size        = 2u;
   foi_a.points[0].x = 1.0f;
   foi_a.points[0].y = 1.0f;
   foi_a.points[1].x = 5.0f;
   foi_a.points[1].y = 5.0f;

   foi_b.size        = 2u;
   foi_b.points[0].x = 0.0f;
   foi_b.points[0].y = 2.0f;
   foi_b.points[1].x = 1.0f;
   foi_b.points[1].y = 0.0f;

   /** \action call FBK function */
   boolean_T result = Fbk_Are_Fields_Of_Interest_Overlapping(&foi_a, &foi_b);

   /** \assert Expect correct result */
   EXPECT_FALSE(result);
}


/**
 * Verify that the correct result is returned for a point located in the same direction as the given vector with positive
 * components. \uts{CSCSA-42322} \sdd{SF-4200} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Is_Field_Of_Interest_Point_In_Direction_Of_Vector__pointing_in_same_direction_positive)
{
   /** \arrange set up point and vector */
   Vector_2d_T foi_point        = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   Vector_2d_T vector_origin    = Create_2d_Vector_Coordinates(0.0f, 0.0f);
   Vector_2d_T vector_direction = Create_2d_Vector_Coordinates(0.5f, 0.5f);

   /** \action call FBK function */
   boolean_T result = Fbk_Is_Field_Of_Interest_Point_In_Direction_Of_Vector(&foi_point, &vector_origin, &vector_direction);

   /** \assert Expect correct result */
   EXPECT_TRUE(result);
}

/**
 * Verify that the correct result is returned for a point located in the same direction as the given vector with a negative
 * component. \uts{CSCSA-42323} \sdd{SF-4200} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Is_Field_Of_Interest_Point_In_Direction_Of_Vector__pointing_in_same_direction_negative)
{
   /** \arrange set up point and vector */
   Vector_2d_T foi_point        = Create_2d_Vector_Coordinates(1.0f, -1.0f);
   Vector_2d_T vector_origin    = Create_2d_Vector_Coordinates(0.0f, 0.0f);
   Vector_2d_T vector_direction = Create_2d_Vector_Coordinates(0.5f, -0.5f);

   /** \action call FBK function */
   boolean_T result = Fbk_Is_Field_Of_Interest_Point_In_Direction_Of_Vector(&foi_point, &vector_origin, &vector_direction);

   /** \assert Expect correct result */
   EXPECT_TRUE(result);
}

/**
 * Verify that the correct result is returned for a point not located in the same direction as the given vector.
 * \uts{CSCSA-42324} \sdd{SF-4200} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Is_Field_Of_Interest_Point_In_Direction_Of_Vector__not_pointing_in_same_direction)
{
   /** \arrange set up point and vector */
   Vector_2d_T foi_point        = Create_2d_Vector_Coordinates(-1.0f, -1.0f);
   Vector_2d_T vector_origin    = Create_2d_Vector_Coordinates(0.0f, 0.0f);
   Vector_2d_T vector_direction = Create_2d_Vector_Coordinates(0.5f, 0.5f);

   /** \action call FBK function */
   boolean_T result = Fbk_Is_Field_Of_Interest_Point_In_Direction_Of_Vector(&foi_point, &vector_origin, &vector_direction);

   /** \assert Expect correct result */
   EXPECT_FALSE(result);
}

/**
 * Verify that the correct result is returned for a point located in the same direction as the given vector with correct origin
 * compensation. \uts{CSCSA-42325} \sdd{SF-4200} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Is_Field_Of_Interest_Point_In_Direction_Of_Vector__move_origin_correctly)
{
   /** \arrange set up point and vector */
   Vector_2d_T foi_point        = Create_2d_Vector_Coordinates(2.0f, 2.0f);
   Vector_2d_T vector_origin    = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   Vector_2d_T vector_direction = Create_2d_Vector_Coordinates(0.5f, 0.5f);

   /** \action call FBK function */
   boolean_T result = Fbk_Is_Field_Of_Interest_Point_In_Direction_Of_Vector(&foi_point, &vector_origin, &vector_direction);

   /** \assert Expect correct result */
   EXPECT_TRUE(result);
}

/**
 * Verify that the correct result is returned for a point located in the same direction as the given vector with one coordinate
 * being zero. \uts{CSCSA-42326} \sdd{SF-4200} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Is_Field_Of_Interest_Point_In_Direction_Of_Vector__check_edge_case)
{
   /** \arrange set up point and vector */
   Vector_2d_T foi_point        = Create_2d_Vector_Coordinates(2.0f, 2.0f);
   Vector_2d_T vector_origin    = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   Vector_2d_T vector_direction = Create_2d_Vector_Coordinates(1.0f, 0.0f);

   /** \action call FBK function */
   boolean_T result = Fbk_Is_Field_Of_Interest_Point_In_Direction_Of_Vector(&foi_point, &vector_origin, &vector_direction);

   /** \assert Expect correct result */
   EXPECT_TRUE(result);
}

/**
 * Verify that the correct false is returned for NULL given as first argument.
 * \uts{CSCSA-42327} \sdd{SF-4199} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Are_Two_Lines_Intersecting__false_on_null_input)
{
   /** \arrange set up vectors */
   Vector_2d_T line1_start = Create_2d_Vector_Coordinates(0.0f, 0.0f);
   Vector_2d_T line1_end   = Create_2d_Vector_Coordinates(2.0f, 2.0f);
   Vector_2d_T line2_start = Create_2d_Vector_Coordinates(2.0f, 0.0f);
   Vector_2d_T line2_end   = Create_2d_Vector_Coordinates(0.0f, 2.0f);

   /** \action call FBK function */
   boolean_T result = Fbk_Are_Two_Lines_Intersecting(NULL, &line1_start, &line1_end, &line2_start, &line2_end);

   /** \assert Expect correct result */
   EXPECT_FALSE(result);
}

/**
 * Verify that the correct result is returned for two lines intersecting.
 * \uts{CSCSA-42328} \sdd{SF-4199} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Are_Two_Lines_Intersecting__lines_intersect)
{
   /** \arrange set up point and vector */
   Vector_2d_T intersection_point;
   Vector_2d_T line1_start = Create_2d_Vector_Coordinates(0.0f, 0.0f);
   Vector_2d_T line1_end   = Create_2d_Vector_Coordinates(2.0f, 2.0f);
   Vector_2d_T line2_start = Create_2d_Vector_Coordinates(2.0f, 0.0f);
   Vector_2d_T line2_end   = Create_2d_Vector_Coordinates(0.0f, 2.0f);

   /** \action call FBK function */
   boolean_T result = Fbk_Are_Two_Lines_Intersecting(&intersection_point, &line1_start, &line1_end, &line2_start, &line2_end);

   /** \assert Expect correct result */
   EXPECT_TRUE(result);
   EXPECT_FLOAT_EQ(intersection_point.x, 1.0f);
   EXPECT_FLOAT_EQ(intersection_point.y, 1.0f);
}

/**
 * Verify that the correct result is returned for two lines that are not intersecting.
 * \uts{CSCSA-42329} \sdd{SF-4199} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Are_Two_Lines_Intersecting__parallel_lines_no_intersection)
{
   /** \arrange set up point and vector */
   Vector_2d_T intersection_point;
   Vector_2d_T line1_start = Create_2d_Vector_Coordinates(0.0f, 0.0f);
   Vector_2d_T line1_end   = Create_2d_Vector_Coordinates(2.0f, 2.0f);
   Vector_2d_T line2_start = Create_2d_Vector_Coordinates(1.0f, 0.0f);
   Vector_2d_T line2_end   = Create_2d_Vector_Coordinates(3.0f, 2.0f);

   /** \action call FBK function */
   boolean_T result = Fbk_Are_Two_Lines_Intersecting(&intersection_point, &line1_start, &line1_end, &line2_start, &line2_end);

   /** \assert Expect correct result */
   EXPECT_FALSE(result);
   EXPECT_TRUE(isinf(intersection_point.x));
   EXPECT_TRUE(isinf(intersection_point.y));
}

/**
 * Verify that the correct result is returned for two lines that are not intersecting.
 * \uts{CSCSA-42330} \sdd{SF-4199} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Are_Two_Lines_Intersecting__both_lines_inf_slope)
{
   /** \arrange set up point and vector */
   Vector_2d_T intersection_point;
   Vector_2d_T line1_start = Create_2d_Vector_Coordinates(0.0f, 1.0f);
   Vector_2d_T line1_end   = Create_2d_Vector_Coordinates(0.0f, 2.0f);
   Vector_2d_T line2_start = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   Vector_2d_T line2_end   = Create_2d_Vector_Coordinates(1.0f, 2.0f);

   /** \action call FBK function */
   boolean_T result = Fbk_Are_Two_Lines_Intersecting(&intersection_point, &line1_start, &line1_end, &line2_start, &line2_end);

   /** \assert Expect correct result */
   EXPECT_FALSE(result);
   EXPECT_TRUE(isinf(intersection_point.x));
   EXPECT_TRUE(isinf(intersection_point.y));
}

/**
 * Verify that the correct result is returned for two lines that are intersecting, where line 1 is parallel to axis.
 * \uts{CSCSA-42331} \sdd{SF-4199} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Are_Two_Lines_Intersecting__line_1_aligned_with_axis)
{
   /** \arrange set up point and vector */
   Vector_2d_T intersection_point;
   Vector_2d_T line1_start = Create_2d_Vector_Coordinates(0.0f, -1.0f);
   Vector_2d_T line1_end   = Create_2d_Vector_Coordinates(0.0f, 1.0f);
   Vector_2d_T line2_start = Create_2d_Vector_Coordinates(-1.0f, 0.0f);
   Vector_2d_T line2_end   = Create_2d_Vector_Coordinates(1.0f, 0.0f);

   /** \action call FBK function */
   boolean_T result = Fbk_Are_Two_Lines_Intersecting(&intersection_point, &line1_start, &line1_end, &line2_start, &line2_end);

   /** \assert Expect correct result */
   EXPECT_TRUE(result);
   EXPECT_FLOAT_EQ(intersection_point.x, 0.0f);
   EXPECT_FLOAT_EQ(intersection_point.y, 0.0f);
}

/**
 * Verify that the correct result is returned for two lines that are intersecting, where line 2 is parallel to axis.
 * \uts{CSCSA-42332} \sdd{SF-4199} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Are_Two_Lines_Intersecting__line_2_aligned_with_axis)
{
   /** \arrange set up point and vector */
   Vector_2d_T intersection_point;
   Vector_2d_T line1_start = Create_2d_Vector_Coordinates(-1.0f, 0.0f);
   Vector_2d_T line1_end   = Create_2d_Vector_Coordinates(1.0f, 0.0f);
   Vector_2d_T line2_start = Create_2d_Vector_Coordinates(0.0f, -1.0f);
   Vector_2d_T line2_end   = Create_2d_Vector_Coordinates(0.0f, 1.0f);

   /** \action call FBK function */
   boolean_T result = Fbk_Are_Two_Lines_Intersecting(&intersection_point, &line1_start, &line1_end, &line2_start, &line2_end);

   /** \assert Expect correct result */
   EXPECT_TRUE(result);
   EXPECT_FLOAT_EQ(intersection_point.x, 0.0f);
   EXPECT_FLOAT_EQ(intersection_point.y, 0.0f);
}


/**
 * Verify that the invalid time is returned for FoI too large and velocity vector.
 * \uts{CSCSA-42333} \sdd{SF-4202} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Time_To_Leave_Field_Of_Interest_Given_Point_And_Velocity_Vector__invalid_time)
{
   /** \arrange set up point and vector */
   Fbk_Field_Of_Interest_T foi_zone;
   Vector_2d_T vel_vector_origin = Create_2d_Vector_Coordinates(1.5f, 1.5f);
   Vector_2d_T vel_vector_dir    = Create_2d_Vector_Coordinates(1.0f, 0.0f);

   foi_zone.size      = 4u;
   foi_zone.points[0] = Create_2d_Vector_Coordinates(3000.0f, 0.0f);
   foi_zone.points[1] = Create_2d_Vector_Coordinates(3000.0f, 3000.0f);
   foi_zone.points[2] = Create_2d_Vector_Coordinates(0.0f, 3000.0f);
   foi_zone.points[3] = Create_2d_Vector_Coordinates(0.0f, 0.0f);

   /** \action call FBK function */
   float32_T time_to_leave =
      Fbk_Get_Time_To_Leave_Field_Of_Interest_Given_Point_And_Velocity_Vector(&foi_zone, &vel_vector_origin, &vel_vector_dir);

   /** \assert Expect correct result */
   EXPECT_FLOAT_EQ(time_to_leave, FBK_INVALID_TIME);
}

/**
 * Verify that the correct time is returned for given FoI point and velocity vector.
 * \uts{CSCSA-42334} \sdd{SF-4202} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test,
       Fbk_Get_Time_To_Leave_Field_Of_Interest_Given_Point_And_Velocity_Vector__calculate_correct_time)

{
   /** \arrange set up point and vector */
   Fbk_Field_Of_Interest_T foi_zone;
   Vector_2d_T vel_vector_origin = Create_2d_Vector_Coordinates(1.5f, 1.5f);
   Vector_2d_T vel_vector_dir    = Create_2d_Vector_Coordinates(1.0f, 0.0f);

   foi_zone.size      = 4u;
   foi_zone.points[0] = Create_2d_Vector_Coordinates(3.0f, 0.0f);
   foi_zone.points[1] = Create_2d_Vector_Coordinates(3.0f, 3.0f);
   foi_zone.points[2] = Create_2d_Vector_Coordinates(0.0f, 3.0f);
   foi_zone.points[3] = Create_2d_Vector_Coordinates(0.0f, 0.0f);

   /** \action call FBK function */
   float32_T time_to_leave =
      Fbk_Get_Time_To_Leave_Field_Of_Interest_Given_Point_And_Velocity_Vector(&foi_zone, &vel_vector_origin, &vel_vector_dir);

   /** \assert Expect correct result */
   EXPECT_FLOAT_EQ(time_to_leave, 1.5f);
}


/**
 * Verify that the correct time is returned for given FoI point and velocity vector.
 * \uts{CSCSA-42335} \sdd{SF-4201} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Time_For_Object_To_Leave_Field_Of_Interest__calculate_correct_time_for_object)
{
   /** \arrange set up point and vector */
   Fbk_Field_Of_Interest_T foi_zone;
   Fbk_Field_Of_Interest_T foi_object;
   Vector_2d_T vel_vector_origin = Create_2d_Vector_Coordinates(5.0f, 5.0f);
   Vector_2d_T vel_vector_dir    = Create_2d_Vector_Coordinates(1.0f, 0.0f);
   boolean_T f_allow_shift       = FBK_FALSE;

   foi_zone.size      = 4u;
   foi_zone.points[0] = Create_2d_Vector_Coordinates(10.0f, 0.0f);
   foi_zone.points[1] = Create_2d_Vector_Coordinates(10.0f, 10.0f);
   foi_zone.points[2] = Create_2d_Vector_Coordinates(0.0f, 10.0f);
   foi_zone.points[3] = Create_2d_Vector_Coordinates(0.0f, 0.0f);

   foi_object.size      = 4u;
   foi_object.points[0] = Create_2d_Vector_Coordinates(vel_vector_origin.x + 1.0f, vel_vector_origin.y - 1.0f);
   foi_object.points[1] = Create_2d_Vector_Coordinates(vel_vector_origin.x + 1.0f, vel_vector_origin.y + 1.0f);
   foi_object.points[2] = Create_2d_Vector_Coordinates(vel_vector_origin.x - 1.0f, vel_vector_origin.y - 1.0f);
   foi_object.points[3] = Create_2d_Vector_Coordinates(vel_vector_origin.x - 1.0f, vel_vector_origin.y + 1.0f);

   /** \action call FBK function */
   float32_T time_to_leave = Fbk_Get_Time_For_Object_To_Leave_Field_Of_Interest(&foi_zone, &foi_object, &vel_vector_origin,
                                                                                &vel_vector_dir, f_allow_shift);

   /** \assert Expect correct result */
   EXPECT_FLOAT_EQ(time_to_leave, 6.0f);
}

/**
 * Verify that the correct area is returned for Bounding Box. Bounding Box with correct dimension
 * \uts{CSCSA-42336} \sdd{CSCSA-27705} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Area_Bounding_Box__bbox_with_correct_dimensions)
{

   /** \arrange Bounding_Box */
   Fbk_Bounding_Box_T bbox;
   bbox.x.min = -5.0;
   bbox.x.max = -2.0;
   bbox.y.min = 2.0;
   bbox.y.max = 5.0;

   float32_T overlapped_area;

   /** \action call FBK function */
   overlapped_area = Fbk_Get_Area_Bounding_Box(&bbox);

   /** \assert Expect correct result */
   EXPECT_FLOAT_EQ(overlapped_area, 9.0);
}

/**
 * Verify that the correct area is returned for Bounding Box. Bounding Box with inccorect width dimension
 * \uts{CSCSA-42337} \sdd{CSCSA-27705} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Area_Bounding_Box__bbox_with_wrong_width)
{

   /** \arrange Bounding_Box */
   Fbk_Bounding_Box_T bbox;
   bbox.x.min = 1.0;
   bbox.x.max = 2.0;
   bbox.y.min = 5.0;
   bbox.y.max = 2.0;

   float32_T overlapped_area;

   /** \action call FBK function */
   overlapped_area = Fbk_Get_Area_Bounding_Box(&bbox);

   /** \assert Expect correct result */
   EXPECT_FLOAT_EQ(overlapped_area, FBK_ZERO_F);
}

/**
 * Verify that the correct area is returned for Bounding Box. Bounding Box with inccorect length dimension
 * \uts{CSCSA-42338} \sdd{CSCSA-27705} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Area_Bounding_Box__bbox_with_wrong_length)
{

   /** \arrange Bounding_Box */
   Fbk_Bounding_Box_T bbox;
   bbox.x.min = -2.0;
   bbox.x.max = -5.0;
   bbox.y.min = 2.0;
   bbox.y.max = 5.0;

   float32_T overlapped_area;

   /** \action call FBK function */
   overlapped_area = Fbk_Get_Area_Bounding_Box(&bbox);

   /** \assert Expect correct result */
   EXPECT_FLOAT_EQ(overlapped_area, FBK_ZERO_F);
}


/**
 * Verify that the correct area is returned for Bounding Box. Bounding Box has no dimensions
 * \uts{CSCSA-42339} \sdd{CSCSA-27705} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Area_Bounding_Box__bbox_with_no_dimension)
{

   /** \arrange Bounding_Box */
   Fbk_Bounding_Box_T bbox;
   bbox.x.min = FBK_ZERO_F;
   bbox.x.max = FBK_ZERO_F;
   bbox.y.min = FBK_ZERO_F;
   bbox.y.max = FBK_ZERO_F;

   float32_T overlapped_area;

   /** \action call FBK function */
   overlapped_area = Fbk_Get_Area_Bounding_Box(&bbox);

   /** \assert Expect correct result */
   EXPECT_FLOAT_EQ(overlapped_area, FBK_ZERO_F);
}

/**
 * Verify that the correct value of area overlapped by 2 Bonding Boxes. Bounding Boxes are partially overlapped.
 * \uts{CSCSA-42340} \sdd{CSCSA-27704} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Bounding_Boxes_Overlapped_Area__partial_overlap)
{
   /** \arrange Bounding Boxes */
   Fbk_Bounding_Box_T bbox_1;
   Fbk_Bounding_Box_T bbox_2;
   float32_T overlapped_area;

   bbox_1.x.min = -9.0f;
   bbox_1.x.max = -2.0f;
   bbox_1.y.min = -5.0f;
   bbox_1.y.max = -1.0f;

   bbox_2.x.min = -4.5f;
   bbox_2.x.max = -0.5f;
   bbox_2.y.min = -4.0f;
   bbox_2.y.max = -2.0f;

   /** \action call FBK function */
   overlapped_area = Fbk_Get_Bounding_Boxes_Overlapped_Area(&bbox_1, &bbox_2);

   /** \assert Expect correct result */
   EXPECT_FLOAT_EQ(overlapped_area, 5.0f);
}

/**
 * Verify that the correct value of area overlapped by 2 Bonding Boxes. Bounding Boxes are partially overlapped (2).
 * \uts{CSCSA-42341} \sdd{CSCSA-27704} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Bounding_Boxes_Overlapped_Area__partial_overlap_2)
{
   /** \arrange Bounding Boxes */
   Fbk_Bounding_Box_T bbox_1;
   Fbk_Bounding_Box_T bbox_2;
   float32_T overlapped_area;

   bbox_1.x.min = -9.0f;
   bbox_1.x.max = -2.0f;
   bbox_1.y.min = -5.0f;
   bbox_1.y.max = -1.0f;

   bbox_2.x.min = -4.5f;
   bbox_2.x.max = -0.5f;
   bbox_2.y.min = -6.0f;
   bbox_2.y.max = -4.0f;

   /** \action call FBK function */
   overlapped_area = Fbk_Get_Bounding_Boxes_Overlapped_Area(&bbox_1, &bbox_2);

   /** \assert Expect correct result */
   EXPECT_FLOAT_EQ(overlapped_area, 2.5f);
}

/**
 * Verify that the correct value of area overlapped by 2 Bonding Boxes. Bounding Boxes are partially overlapped (3).
 * \uts{CSCSA-42342} \sdd{CSCSA-27704} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Bounding_Boxes_Overlapped_Area__partial_overlap_3)
{
   /** \arrange Bounding_Box */
   Fbk_Bounding_Box_T bbox_1;
   Fbk_Bounding_Box_T bbox_2;
   float32_T overlapped_area;

   bbox_1.x.min = -9.0f;
   bbox_1.x.max = -2.0f;
   bbox_1.y.min = -5.0f;
   bbox_1.y.max = -1.0f;

   bbox_2.x.min = -11.0f;
   bbox_2.x.max = -7.0f;
   bbox_2.y.min = -2.5f;
   bbox_2.y.max = -0.5f;

   /** \action call FBK function */
   overlapped_area = Fbk_Get_Bounding_Boxes_Overlapped_Area(&bbox_1, &bbox_2);

   /** \assert Expect correct result */
   EXPECT_FLOAT_EQ(overlapped_area, 3.0f);
}

/**
 * Verify that the correct value of area overlapped by 2 Bonding Boxes. Bounding Box is fully overlapped overlapped by another one.
 * \uts{CSCSA-42343} \sdd{CSCSA-27704} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Bounding_Boxes_Overlapped_Area__full_overlap)
{
   /** \arrange Bounding_Box */
   Fbk_Bounding_Box_T bbox_1;
   Fbk_Bounding_Box_T bbox_2;
   float32_T overlapped_area;

   bbox_1.x.min = -9.0f;
   bbox_1.x.max = -2.0f;
   bbox_1.y.min = -5.0f;
   bbox_1.y.max = -1.0f;

   bbox_2.x.min = -8.0f;
   bbox_2.x.max = -5.0f;
   bbox_2.y.min = -4.0f;
   bbox_2.y.max = -2.0f;

   /** \action call FBK function */
   overlapped_area = Fbk_Get_Bounding_Boxes_Overlapped_Area(&bbox_1, &bbox_2);

   /** \assert Expect correct result */
   EXPECT_FLOAT_EQ(overlapped_area, 6.0f);
}

/**
 * Verify that the correct value of area overlapped by 2 Bonding Boxes. Bounding Boxes are overlapped.
 * \uts{CSCSA-42344} \sdd{CSCSA-27704} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Bounding_Boxes_Overlapped_Area__zone_overlapped_by_object)
{
   /** \arrange Bounding_Box */
   Fbk_Bounding_Box_T bbox_1;
   Fbk_Bounding_Box_T bbox_2;
   float32_T overlapped_area;

   bbox_1.x.min = -5.0f;
   bbox_1.x.max = -3.0f;
   bbox_1.y.min = -4.0f;
   bbox_1.y.max = -2.0f;

   bbox_2.x.min = -6.0f;
   bbox_2.x.max = -2.0f;
   bbox_2.y.min = -5.0f;
   bbox_2.y.max = -1.0f;

   /** \action call FBK function */
   overlapped_area = Fbk_Get_Bounding_Boxes_Overlapped_Area(&bbox_1, &bbox_2);

   /** \assert Expect correct result */
   EXPECT_FLOAT_EQ(overlapped_area, 4.0f);
}

/**
 * Verify that the correct value of area overlapped by 2 Bonding Boxes. Bounding Boxes are overlapped wrt. width dimension only.
 * \uts{CSCSA-42345} \sdd{CSCSA-27704} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Bounding_Boxes_Overlapped_Area__width_overlap)
{
   /** \arrange Bounding_Box */
   Fbk_Bounding_Box_T bbox_1;
   Fbk_Bounding_Box_T bbox_2;
   float32_T overlapped_area;

   bbox_1.x.min = -9.0f;
   bbox_1.x.max = -2.0f;
   bbox_1.y.min = -5.0f;
   bbox_1.y.max = -1.0f;

   bbox_2.x.min = -13.0f;
   bbox_2.x.max = -9.0f;
   bbox_2.y.min = -2.0f;
   bbox_2.y.max = 0.0f;


   /** \action call FBK function */
   overlapped_area = Fbk_Get_Bounding_Boxes_Overlapped_Area(&bbox_1, &bbox_2);

   /** \assert Expect correct result */
   EXPECT_FLOAT_EQ(overlapped_area, 0.0f);
}

/**
 * Verify that the correct value of area overlapped by 2 Bonding Boxes. Bounding Boxes are not overlapped.
 * \uts{CSCSA-42346} \sdd{CSCSA-27704} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Bounding_Boxes_Overlapped_Area__no_overlap)
{

   /** \arrange Bounding_Box */
   Fbk_Bounding_Box_T bbox_1;
   Fbk_Bounding_Box_T bbox_2;
   float32_T overlapped_area;

   bbox_1.x.min = -9.0f;
   bbox_1.x.max = -2.0f;
   bbox_1.y.min = -5.0f;
   bbox_1.y.max = -1.0f;

   bbox_2.x.min = -14.0f;
   bbox_2.x.max = -10.0f;
   bbox_2.y.min = -2.0f;
   bbox_2.y.max = 0.0f;


   /** \action call FBK function */
   overlapped_area = Fbk_Get_Bounding_Boxes_Overlapped_Area(&bbox_1, &bbox_2);

   /** \assert Expect correct result */
   EXPECT_FLOAT_EQ(overlapped_area, 0.0f);
}

/**
 * Verify that the correct value of area overlapped by 2 Bonding Boxes. Bounding Boxes are not overlapped (2).
 * \uts{CSCSA-42347} \sdd{CSCSA-27704} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Bounding_Boxes_Overlapped_Area__no_overlap_2)
{
   /** \arrange Bounding_Box */
   Fbk_Bounding_Box_T bbox_1;
   Fbk_Bounding_Box_T bbox_2;
   float32_T overlapped_area;

   bbox_1.x.min = -6.0f;
   bbox_1.x.max = -2.0f;
   bbox_1.y.min = -5.0f;
   bbox_1.y.max = -1.0f;

   bbox_2.x.min = -1.5f;
   bbox_2.x.max = -0.5f;
   bbox_2.y.min = -6.5f;
   bbox_2.y.max = -5.5f;


   /** \action call FBK function */
   overlapped_area = Fbk_Get_Bounding_Boxes_Overlapped_Area(&bbox_1, &bbox_2);

   /** \assert Expect correct result */
   EXPECT_FLOAT_EQ(overlapped_area, 0.0f);
}

/**
 * Verify that the correct value of area overlapped by 2 Bonding Boxes. Bounding Boxes are not overlapped (3).
 * \uts{CSCSA-42348} \sdd{CSCSA-27704} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Bounding_Boxes_Overlapped_Area__no_overlap_3)
{
   /** \arrange Bounding_Box */
   Fbk_Bounding_Box_T bbox_1;
   Fbk_Bounding_Box_T bbox_2;
   float32_T overlapped_area;

   bbox_1.x.min = -6.0f;
   bbox_1.x.max = -2.0f;
   bbox_1.y.min = -5.0f;
   bbox_1.y.max = -1.0f;

   bbox_2.x.min = -7.5f;
   bbox_2.x.max = -6.5f;
   bbox_2.y.min = -0.75f;
   bbox_2.y.max = -0.25f;


   /** \action call FBK function */
   overlapped_area = Fbk_Get_Bounding_Boxes_Overlapped_Area(&bbox_1, &bbox_2);

   /** \assert Expect correct result */
   EXPECT_FLOAT_EQ(overlapped_area, 0.0f);
}

/**
 * Verify that the correct value of area overlapped by 2 Bonding Boxes. Bounding Boxes are not overlapped (4).
 * \uts{CSCSA-42349} \sdd{CSCSA-27704} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Bounding_Boxes_Overlapped_Area__no_overlap_4)
{
   /** \arrange Bounding_Box */
   Fbk_Bounding_Box_T bbox_1;
   Fbk_Bounding_Box_T bbox_2;
   float32_T overlapped_area;

   bbox_1.x.min = -6.0f;
   bbox_1.x.max = -2.0f;
   bbox_1.y.min = -5.0f;
   bbox_1.y.max = -1.0f;

   bbox_2.x.min = -4.0f;
   bbox_2.x.max = -3.0f;
   bbox_2.y.min = -6.0f;
   bbox_2.y.max = -5.0f;


   /** \action call FBK function */
   overlapped_area = Fbk_Get_Bounding_Boxes_Overlapped_Area(&bbox_1, &bbox_2);

   /** \assert Expect correct result */
   EXPECT_FLOAT_EQ(overlapped_area, 0.0f);
}

/**
 * Verify that the correct value of area overlapped by 2 Bonding Boxes. Bounding Boxes with no dimension is located inside of
 * another bounding box. \uts{CSCSA-42350} \sdd{CSCSA-27704} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Bounding_Boxes_Overlapped_Area__obj_with_no_dimension_overlapped)
{
   /** \arrange Bounding_Box */
   Fbk_Bounding_Box_T bbox_1;
   Fbk_Bounding_Box_T bbox_2;
   float32_T overlapped_area;

   bbox_1.x.min = -9.0f;
   bbox_1.x.max = -2.0f;
   bbox_1.y.min = -5.0f;
   bbox_1.y.max = -1.0f;

   bbox_2.x.min = -5.0f;
   bbox_2.x.max = -5.0f;
   bbox_2.y.min = -3.0f;
   bbox_2.y.max = -3.0f;


   /** \action call FBK function */
   overlapped_area = Fbk_Get_Bounding_Boxes_Overlapped_Area(&bbox_1, &bbox_2);

   /** \assert Expect correct result */
   EXPECT_FLOAT_EQ(overlapped_area, 0.0f);
}

/**
 * Verify that the correct value of area overlapped by 2 Bonding Boxes. Bounding Box with no dimension is located out of another
 * bounding box. \uts{CSCSA-42351} \sdd{CSCSA-27704} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Bounding_Boxes_Overlapped_Area__obj_with_no_dimension_not_overlapped)
{
   /** \arrange Bounding_Box */
   Fbk_Bounding_Box_T bbox_1;
   Fbk_Bounding_Box_T bbox_2;
   float32_T overlapped_area;

   bbox_1.x.min = -9.0f;
   bbox_1.x.max = -2.0f;
   bbox_1.y.min = -5.0f;
   bbox_1.y.max = -1.0f;

   bbox_2.x.min = -5.0f;
   bbox_2.x.max = -5.0f;
   bbox_2.y.min = -6.0f;
   bbox_2.y.max = -6.0f;


   /** \action call FBK function */
   overlapped_area = Fbk_Get_Bounding_Boxes_Overlapped_Area(&bbox_1, &bbox_2);

   /** \assert Expect correct result */
   EXPECT_FLOAT_EQ(overlapped_area, 0.0f);
}

/**
 * Verify that the given point exists in the array.
 * \uts{CSCSA-42352} \sdd{CSCSA-27798} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Is_Point_In_Array__point_in_considered_range)
{
   /** \arrange set up point and array of points */
   Vector_2d_T point = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   Vector_2d_T points[FBK_MAX_SIZE_OF_FOI];
   points[0]    = Create_2d_Vector_Coordinates(0.0f, 0.0f);
   points[1]    = Create_2d_Vector_Coordinates(1.0f, 0.0f);
   points[2]    = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   points[3]    = Create_2d_Vector_Coordinates(0.0f, 1.0f);
   uint8_t size = 3;

   boolean_T result;

   /** \action call FBK function */
   result = Fbk_Is_Point_In_Array(&point, points, size);

   /** \assert Expect correct result */
   EXPECT_TRUE(result);
}

/**
 * Verify that the given point exists in the array.
 * \uts{CSCSA-42353} \sdd{CSCSA-27798} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Is_Point_In_Array__point_out_of_considered_range)
{
   /** \arrange set up point and array of points */
   Vector_2d_T point = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   Vector_2d_T points[FBK_MAX_SIZE_OF_FOI];
   points[0]    = Create_2d_Vector_Coordinates(0.0f, 0.0f);
   points[1]    = Create_2d_Vector_Coordinates(1.0f, 0.0f);
   points[2]    = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   points[3]    = Create_2d_Vector_Coordinates(0.0f, 1.0f);
   uint8_t size = 2;

   boolean_T result;

   /** \action call FBK function */
   result = Fbk_Is_Point_In_Array(&point, points, size);

   /** \assert Expect correct result */
   EXPECT_FALSE(result);
}

/**
 * Verify that the given point exists in the array.
 * \uts{CSCSA-42354} \sdd{CSCSA-27798} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Is_Point_In_Array__point_does_not_exist)
{
   /** \arrange set up point and array of points */
   Vector_2d_T point = Create_2d_Vector_Coordinates(5.0f, 1.0f);
   Vector_2d_T points[FBK_MAX_SIZE_OF_FOI];
   points[0]    = Create_2d_Vector_Coordinates(0.0f, 0.0f);
   points[1]    = Create_2d_Vector_Coordinates(1.0f, 0.0f);
   points[2]    = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   points[3]    = Create_2d_Vector_Coordinates(0.0f, 1.0f);
   uint8_t size = 2;

   boolean_T result;

   /** \action call FBK function */
   result = Fbk_Is_Point_In_Array(&point, points, size);

   /** \assert Expect correct result */
   EXPECT_FALSE(result);
}

/**
 * Verify that intersection method return 1 point when 2 polygons intersect only in that point.
 * \uts{CSCSA-42355} \sdd{CSCSA-27799} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Intersection_Polygon__intersection_in_1_point)
{
   /** \arrange set up polygons */
   Fbk_Field_Of_Interest_T result, zone_polygon, object_polygon;
   zone_polygon.points[FBK_FOI_FRONT_LEFT_CORNER]  = Create_2d_Vector_Coordinates(0.0f, 0.0f);
   zone_polygon.points[FBK_FOI_FRONT_RIGHT_CORNER] = Create_2d_Vector_Coordinates(1.0f, 0.0f);
   zone_polygon.points[FBK_FOI_REAR_RIGHT_CORNER]  = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   zone_polygon.points[FBK_FOI_REAR_LEFT_CORNER]   = Create_2d_Vector_Coordinates(0.0f, 1.0f);
   zone_polygon.size                               = 4u;

   object_polygon.points[FBK_FOI_FRONT_LEFT_CORNER]  = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   object_polygon.points[FBK_FOI_FRONT_RIGHT_CORNER] = Create_2d_Vector_Coordinates(2.0f, 1.0f);
   object_polygon.points[FBK_FOI_REAR_RIGHT_CORNER]  = Create_2d_Vector_Coordinates(2.0f, 2.0f);
   object_polygon.points[FBK_FOI_REAR_LEFT_CORNER]   = Create_2d_Vector_Coordinates(1.0f, 2.0f);
   object_polygon.size                               = 4u;

   Vector_2d_T expected_point = {1, 1};

   /** \action call FBK function */
   Fbk_Get_Intersection_Polygon(&result, &zone_polygon, &object_polygon);

   /** \assert Expect correct result */
   EXPECT_EQ(result.size, 1u);
   EXPECT_TRUE(Fbk_Is_Point_In_Array(&(expected_point), result.points, result.size));
}

/**
 * Verify that intersection polygon calculated correct for 1 corner intersecting.
 * \uts{CSCSA-42356} \sdd{CSCSA-27799} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Intersection_Polygon__intersection_polygon_3_points)
{
   /** \arrange set up polygons */
   Fbk_Field_Of_Interest_T result, zone_polygon, object_polygon;
   zone_polygon.points[FBK_FOI_FRONT_LEFT_CORNER]  = Create_2d_Vector_Coordinates(1.0f, 0.0f);
   zone_polygon.points[FBK_FOI_FRONT_RIGHT_CORNER] = Create_2d_Vector_Coordinates(3.0f, 2.0f);
   zone_polygon.points[FBK_FOI_REAR_RIGHT_CORNER]  = Create_2d_Vector_Coordinates(2.0f, 3.0f);
   zone_polygon.points[FBK_FOI_REAR_LEFT_CORNER]   = Create_2d_Vector_Coordinates(0.0f, 1.0f);
   zone_polygon.size                               = 4u;

   object_polygon.points[FBK_FOI_FRONT_LEFT_CORNER]  = Create_2d_Vector_Coordinates(2.0f, 0.0f);
   object_polygon.points[FBK_FOI_FRONT_RIGHT_CORNER] = Create_2d_Vector_Coordinates(4.0f, 0.0f);
   object_polygon.points[FBK_FOI_REAR_RIGHT_CORNER]  = Create_2d_Vector_Coordinates(4.0f, 4.0f);
   object_polygon.points[FBK_FOI_REAR_LEFT_CORNER]   = Create_2d_Vector_Coordinates(2.0f, 4.0f);
   object_polygon.size                               = 4u;

   Vector_2d_T expected_points[3u] = {{2, 1}, {3, 2}, {2, 3}};

   /** \action call FBK function */
   Fbk_Get_Intersection_Polygon(&result, &zone_polygon, &object_polygon);

   /** \assert Expect correct result */
   EXPECT_EQ(result.size, 3u);
   EXPECT_TRUE(Fbk_Is_Point_In_Array(&(expected_points[0]), result.points, result.size));
   EXPECT_TRUE(Fbk_Is_Point_In_Array(&(expected_points[1]), result.points, result.size));
   EXPECT_TRUE(Fbk_Is_Point_In_Array(&(expected_points[2]), result.points, result.size));
}

/**
 * Verify that intersection polygon calculated correct for 2 corners intersecting.
 * \uts{CSCSA-42357} \sdd{CSCSA-27799} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Intersection_Polygon__intersection_polygon_4_points)
{
   /** \arrange set up polygons */
   Fbk_Field_Of_Interest_T result, zone_polygon, object_polygon;
   zone_polygon.points[FBK_FOI_FRONT_LEFT_CORNER]  = Create_2d_Vector_Coordinates(1.0f, 0.0f);
   zone_polygon.points[FBK_FOI_FRONT_RIGHT_CORNER] = Create_2d_Vector_Coordinates(2.0f, 1.0f);
   zone_polygon.points[FBK_FOI_REAR_RIGHT_CORNER]  = Create_2d_Vector_Coordinates(1.0f, 2.0f);
   zone_polygon.points[FBK_FOI_REAR_LEFT_CORNER]   = Create_2d_Vector_Coordinates(0.0f, 1.0f);
   zone_polygon.size                               = 4u;

   object_polygon.points[FBK_FOI_FRONT_LEFT_CORNER]  = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   object_polygon.points[FBK_FOI_FRONT_RIGHT_CORNER] = Create_2d_Vector_Coordinates(2.0f, 2.0f);
   object_polygon.points[FBK_FOI_REAR_RIGHT_CORNER]  = Create_2d_Vector_Coordinates(1.0f, 3.0f);
   object_polygon.points[FBK_FOI_REAR_LEFT_CORNER]   = Create_2d_Vector_Coordinates(0.0f, 2.0f);
   object_polygon.size                               = 4u;

   Vector_2d_T expected_points[4u] = {{1, 1}, {1.5, 1.5}, {1, 2}, {0.5, 1.5}};

   /** \action call FBK function */
   Fbk_Get_Intersection_Polygon(&result, &zone_polygon, &object_polygon);

   /** \assert Expect correct result */
   EXPECT_EQ(result.size, 4u);
   EXPECT_TRUE(Fbk_Is_Point_In_Array(&(expected_points[0]), result.points, result.size));
   EXPECT_TRUE(Fbk_Is_Point_In_Array(&(expected_points[1]), result.points, result.size));
   EXPECT_TRUE(Fbk_Is_Point_In_Array(&(expected_points[2]), result.points, result.size));
   EXPECT_TRUE(Fbk_Is_Point_In_Array(&(expected_points[3]), result.points, result.size));
}

/**
 * Verify that intersection polygon calculated correct for 3 corners intersecting.
 * \uts{CSCSA-42358} \sdd{CSCSA-27799} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Intersection_Polygon__intersection_polygon_5_points)
{
   /** \arrange set up polygons */
   Fbk_Field_Of_Interest_T result, zone_polygon, object_polygon;
   zone_polygon.points[FBK_FOI_FRONT_LEFT_CORNER]  = Create_2d_Vector_Coordinates(-1.0f, -1.0f);
   zone_polygon.points[FBK_FOI_FRONT_RIGHT_CORNER] = Create_2d_Vector_Coordinates(2.0f, -1.0f);
   zone_polygon.points[FBK_FOI_REAR_RIGHT_CORNER]  = Create_2d_Vector_Coordinates(2.0f, 2.0f);
   zone_polygon.points[FBK_FOI_REAR_LEFT_CORNER]   = Create_2d_Vector_Coordinates(-1.0f, 2.0f);
   zone_polygon.size                               = 4u;

   object_polygon.points[FBK_FOI_FRONT_LEFT_CORNER]  = Create_2d_Vector_Coordinates(-1.0f, -2.0f);
   object_polygon.points[FBK_FOI_FRONT_RIGHT_CORNER] = Create_2d_Vector_Coordinates(1.0f, 0.0f);
   object_polygon.points[FBK_FOI_REAR_RIGHT_CORNER]  = Create_2d_Vector_Coordinates(0.0f, 1.0f);
   object_polygon.points[FBK_FOI_REAR_LEFT_CORNER]   = Create_2d_Vector_Coordinates(-2.0f, -1.0f);
   object_polygon.size                               = 4u;

   Vector_2d_T expected_points[5u] = {{-1, -1}, {0, -1}, {1, 0}, {0, 1}, {-1, 0}};

   /** \action call FBK function */
   Fbk_Get_Intersection_Polygon(&result, &zone_polygon, &object_polygon);

   /** \assert Expect correct result */
   EXPECT_EQ(result.size, 5u);
   EXPECT_TRUE(Fbk_Is_Point_In_Array(&(expected_points[0]), result.points, result.size));
   EXPECT_TRUE(Fbk_Is_Point_In_Array(&(expected_points[1]), result.points, result.size));
   EXPECT_TRUE(Fbk_Is_Point_In_Array(&(expected_points[2]), result.points, result.size));
   EXPECT_TRUE(Fbk_Is_Point_In_Array(&(expected_points[3]), result.points, result.size));
   EXPECT_TRUE(Fbk_Is_Point_In_Array(&(expected_points[4]), result.points, result.size));
}

/**
 * Check if the number of intersetion polygon points equal zero, when two polygons are not intersecting.
 * \uts{CSCSA-42359} \sdd{CSCSA-27799} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Intersection_Polygon__polygons_not_intersect)
{
   /** \arrange set up polygons */
   Fbk_Field_Of_Interest_T result, zone_polygon, object_polygon;
   zone_polygon.points[FBK_FOI_FRONT_LEFT_CORNER]  = Create_2d_Vector_Coordinates(0.0f, 0.0f);
   zone_polygon.points[FBK_FOI_FRONT_RIGHT_CORNER] = Create_2d_Vector_Coordinates(1.0f, 0.0f);
   zone_polygon.points[FBK_FOI_REAR_RIGHT_CORNER]  = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   zone_polygon.points[FBK_FOI_REAR_LEFT_CORNER]   = Create_2d_Vector_Coordinates(0.0f, 1.0f);
   zone_polygon.size                               = 4u;

   object_polygon.points[FBK_FOI_FRONT_LEFT_CORNER]  = Create_2d_Vector_Coordinates(-1.0f, -1.0f);
   object_polygon.points[FBK_FOI_FRONT_RIGHT_CORNER] = Create_2d_Vector_Coordinates(-1.0f, 0.0f);
   object_polygon.points[FBK_FOI_REAR_RIGHT_CORNER]  = Create_2d_Vector_Coordinates(-2.0f, 0.0f);
   object_polygon.points[FBK_FOI_REAR_LEFT_CORNER]   = Create_2d_Vector_Coordinates(-2.0f, -1.0f);
   object_polygon.size                               = 4u;

   /** \action call FBK function */
   Fbk_Get_Intersection_Polygon(&result, &zone_polygon, &object_polygon);

   /** \assert Expect correct result */
   EXPECT_EQ(result.size, FBK_ZERO_UINT);
}

/**
 * Check if points of poylgon are sorted counterclockwise.
 * \uts{CSCSA-42360} \sdd{CSCSA-27801} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Sort_Polygon_Points__sorting_5_points)
{
   /** \arrange set up polygons */
   Fbk_Field_Of_Interest_T polygon, sorted_polygon;
   polygon.points[0] = Create_2d_Vector_Coordinates(1.0f, 0.0f);
   polygon.points[1] = Create_2d_Vector_Coordinates(0.0f, 1.0f);
   polygon.points[2] = Create_2d_Vector_Coordinates(0.0f, 0.0f);
   polygon.points[3] = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   polygon.points[4] = Create_2d_Vector_Coordinates(0.0f, 2.0f);
   polygon.size      = 5u;

   sorted_polygon.points[0] = Create_2d_Vector_Coordinates(0.0f, 1.0f);
   sorted_polygon.points[1] = Create_2d_Vector_Coordinates(0.0f, 2.0f);
   sorted_polygon.points[2] = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   sorted_polygon.points[3] = Create_2d_Vector_Coordinates(1.0f, 0.0f);
   sorted_polygon.points[4] = Create_2d_Vector_Coordinates(0.0f, 0.0f);

   /** \action call FBK function */
   Fbk_Sort_Polygon_Points(&polygon);

   /** \assert Expect correct result */
   EXPECT_EQ(polygon.points[0].x, sorted_polygon.points[0].x);
   EXPECT_EQ(polygon.points[0].y, sorted_polygon.points[0].y);
   EXPECT_EQ(polygon.points[1].x, sorted_polygon.points[1].x);
   EXPECT_EQ(polygon.points[1].y, sorted_polygon.points[1].y);
   EXPECT_EQ(polygon.points[2].x, sorted_polygon.points[2].x);
   EXPECT_EQ(polygon.points[2].y, sorted_polygon.points[2].y);
   EXPECT_EQ(polygon.points[3].x, sorted_polygon.points[3].x);
   EXPECT_EQ(polygon.points[3].y, sorted_polygon.points[3].y);
   EXPECT_EQ(polygon.points[4].x, sorted_polygon.points[4].x);
   EXPECT_EQ(polygon.points[4].y, sorted_polygon.points[4].y);
}

/**
 * Verify that intersection point is in range of intersecting lines.
 * \uts{CSCSA-42361} \sdd{CSCSA-27803} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Is_Intersection_Point_In_Range__point_out_of_range)
{
   /** \arrange set up points */
   Vector_2d_T point             = Create_2d_Vector_Coordinates(2.0f, 2.0f);
   Vector_2d_T first_line_start  = Create_2d_Vector_Coordinates(0.0f, 0.0f);
   Vector_2d_T first_line_end    = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   Vector_2d_T second_line_start = Create_2d_Vector_Coordinates(0.0f, 2.0f);
   Vector_2d_T second_line_end   = Create_2d_Vector_Coordinates(1.0f, 2.0f);

   boolean_T result;

   /** \action call FBK function */
   result = Fbk_Is_Intersection_Point_In_Range(&point, &first_line_start, &first_line_end, &second_line_start, &second_line_end);

   /** \assert Expect correct result */
   EXPECT_FALSE(result);
}

/**
 * Verify that intersection point is in range of intersecting lines.
 * \uts{CSCSA-42362} \sdd{CSCSA-27803} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Is_Intersection_Point_In_Range__point_in_range)
{
   /** \arrange set up points */
   Vector_2d_T point             = Create_2d_Vector_Coordinates(2.0f, 2.0f);
   Vector_2d_T first_line_start  = Create_2d_Vector_Coordinates(0.0f, 0.0f);
   Vector_2d_T first_line_end    = Create_2d_Vector_Coordinates(3.0f, 3.0f);
   Vector_2d_T second_line_start = Create_2d_Vector_Coordinates(0.0f, 2.0f);
   Vector_2d_T second_line_end   = Create_2d_Vector_Coordinates(3.0f, 2.0f);

   boolean_T result;

   /** \action call FBK function */
   result = Fbk_Is_Intersection_Point_In_Range(&point, &first_line_start, &first_line_end, &second_line_start, &second_line_end);

   /** \assert Expect correct result */
   EXPECT_TRUE(result);
}

/**
 * Verify if intersection polygon points are calculated correct, when 1 corner of polygon1 is inside polygon2.
 * \uts{CSCSA-42363} \sdd{CSCSA-27800} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Intersection_Points_For_Point_In_Polygon__1_point_inside)
{
   /** \arrange set up polygons */
   Fbk_Field_Of_Interest_T result_polygon, first_polygon, second_polygon;

   result_polygon.size = FBK_ZERO_UINT;

   first_polygon.points[0] = Create_2d_Vector_Coordinates(1.0f, 0.0f);
   first_polygon.points[1] = Create_2d_Vector_Coordinates(2.0f, 1.0f);
   first_polygon.points[2] = Create_2d_Vector_Coordinates(1.0f, 2.0f);
   first_polygon.points[3] = Create_2d_Vector_Coordinates(0.0f, 1.0f);
   first_polygon.size      = 4u;

   second_polygon.points[0] = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   second_polygon.points[1] = Create_2d_Vector_Coordinates(2.0f, 2.0f);
   second_polygon.points[2] = Create_2d_Vector_Coordinates(1.0f, 3.0f);
   second_polygon.points[3] = Create_2d_Vector_Coordinates(0.0f, 2.0f);
   second_polygon.size      = 4u;

   Vector_2d_T expected_points[3u] = {{1.5, 1.5}, {1.0, 2.0}, {0.5, 1.5}};

   boolean_T result;

   /** \action call FBK function */
   result = Fbk_Get_Intersection_Points_For_Point_In_Polygon(&result_polygon, &first_polygon, &second_polygon);

   /** \assert Expect correct result */
   EXPECT_TRUE(result);
   EXPECT_EQ(result_polygon.size, 3u);
   EXPECT_TRUE(Fbk_Is_Point_In_Array(&(expected_points[0]), result_polygon.points, result_polygon.size));
   EXPECT_TRUE(Fbk_Is_Point_In_Array(&(expected_points[1]), result_polygon.points, result_polygon.size));
   EXPECT_TRUE(Fbk_Is_Point_In_Array(&(expected_points[2]), result_polygon.points, result_polygon.size));
}

/**
 * Verify if intersection polygon points are calculated correct, when 2 corners of polygon1 are inside polygon2.
 * \uts{CSCSA-42364} \sdd{CSCSA-27800} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Intersection_Points_For_Point_In_Polygon__2_points_inside)
{
   /** \arrange set up polygons */
   Fbk_Field_Of_Interest_T result_polygon, first_polygon, second_polygon;

   result_polygon.size = FBK_ZERO_UINT;

   first_polygon.points[0] = Create_2d_Vector_Coordinates(-1.0f, -1.0f);
   first_polygon.points[1] = Create_2d_Vector_Coordinates(2.0f, -1.0f);
   first_polygon.points[2] = Create_2d_Vector_Coordinates(2.0f, 2.0f);
   first_polygon.points[3] = Create_2d_Vector_Coordinates(-1.0f, 2.0f);
   first_polygon.size      = 4u;

   second_polygon.points[0] = Create_2d_Vector_Coordinates(-1.0f, -2.0f);
   second_polygon.points[1] = Create_2d_Vector_Coordinates(1.0f, 0.0f);
   second_polygon.points[2] = Create_2d_Vector_Coordinates(0.0f, 1.0f);
   second_polygon.points[3] = Create_2d_Vector_Coordinates(-2.0f, -1.0f);
   second_polygon.size      = 4u;

   Vector_2d_T expected_points[4u] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

   boolean_T result;

   /** \action call FBK function */
   result = Fbk_Get_Intersection_Points_For_Point_In_Polygon(&result_polygon, &second_polygon, &first_polygon);

   /** \assert Expect correct result */
   EXPECT_TRUE(result);
   EXPECT_EQ(result_polygon.size, 4u);
   EXPECT_TRUE(Fbk_Is_Point_In_Array(&(expected_points[0]), result_polygon.points, result_polygon.size));
   EXPECT_TRUE(Fbk_Is_Point_In_Array(&(expected_points[1]), result_polygon.points, result_polygon.size));
   EXPECT_TRUE(Fbk_Is_Point_In_Array(&(expected_points[2]), result_polygon.points, result_polygon.size));
   EXPECT_TRUE(Fbk_Is_Point_In_Array(&(expected_points[3]), result_polygon.points, result_polygon.size));
}

/**
 * Verify if intersection polygon points are calculated correct, when any corner of polygon1 is not inside polygon2.
 * \uts{CSCSA-42365} \sdd{CSCSA-27800} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Field_Of_Interest_Factory_Test, Fbk_Get_Intersection_Points_For_Point_In_Polygon__no_points_inside)
{
   /** \arrange set up polygons */
   Fbk_Field_Of_Interest_T result_polygon, first_polygon, second_polygon;

   result_polygon.size = FBK_ZERO_UINT;

   first_polygon.points[0] = Create_2d_Vector_Coordinates(0.0f, 0.0f);
   first_polygon.points[1] = Create_2d_Vector_Coordinates(1.0f, 0.0f);
   first_polygon.points[2] = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   first_polygon.points[3] = Create_2d_Vector_Coordinates(0.0f, 1.0f);
   first_polygon.size      = 4u;

   second_polygon.points[0] = Create_2d_Vector_Coordinates(-1.0f, -1.0f);
   second_polygon.points[1] = Create_2d_Vector_Coordinates(-1.0f, 0.0f);
   second_polygon.points[2] = Create_2d_Vector_Coordinates(-2.0f, 0.0f);
   second_polygon.points[3] = Create_2d_Vector_Coordinates(-2.0f, -1.0f);
   second_polygon.size      = 4u;

   boolean_T result;

   /** \action call FBK function */
   result = Fbk_Get_Intersection_Points_For_Point_In_Polygon(&result_polygon, &second_polygon, &first_polygon);

   /** \assert Expect correct result */
   EXPECT_FALSE(result);
   EXPECT_EQ(result_polygon.size, FBK_ZERO_UINT);
}
