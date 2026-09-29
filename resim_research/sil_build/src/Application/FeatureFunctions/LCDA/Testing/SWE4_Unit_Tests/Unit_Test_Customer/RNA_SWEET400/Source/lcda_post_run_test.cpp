/**
 * @file lcda_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for lcda_post_run.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-43116}
 */

#include "lcda_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "lcda_post_run.c"
#include "pa_reuse.h"
}

/*
 * Check if correct optimum is returned.
 * \uts{CSCSA-43117} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Max_Min__result_max)
{
   /** \arrange */
   float32_T min_a = 0.5f;
   float32_T min_b = 2.0f;
   float32_T max   = 10.0f;
   float32_T result;

   /** \action Call Lcda_Max_Min to compute the maximum of one input and the minimun of the other inputs */
   result = Lcda_Max_Min(min_a, min_b, max);

   /** \assert Check result */
   EXPECT_FLOAT_EQ(max, result);
}

/*
 * Check if correct optimum is returned.
 * \uts{CSCSA-43118} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Max_Min_result__min_a)
{
   /** \arrange */
   float32_T min_a = 15.0f;
   float32_T min_b = 20.0f;
   float32_T max   = 10.0f;
   float32_T result;

   /** \action Call Lcda_Max_Min to compute the maximum of one input and the minimun of the other inputs */
   result = Lcda_Max_Min(min_a, min_b, max);

   /** \assert Check result */
   EXPECT_FLOAT_EQ(min_a, result);
}

/*
 * Check if correct optimum is returned.
 * \uts{CSCSA-43119} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Max_Min_result__min_b)
{
   /** \arrange */
   float32_T min_a = 30.0f;
   float32_T min_b = 15.0f;
   float32_T max   = 10.0f;
   float32_T result;

   /** \action Call Lcda_Max_Min to compute the maximum of one input and the minimun of the other inputs */
   result = Lcda_Max_Min(min_a, min_b, max);

   /** \assert Check result */
   EXPECT_FLOAT_EQ(min_b, result);
}

/**
 * Tests that reference point is calculated correctly.
 * \uts{CSCSA-73304} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Calculate_Target_Reference_Point_test)
{
   /** \arrange create index and vector */
   uint8_t object_index = 0u;
   Vector_2d_T reference_point;
   object_data->curvi_pos.x   = -10.0f;
   object_data->curvi_pos.y   = -10.0f;
   object_data->curvi_heading = 0.0f;
   object_data->length        = 2.0f;
   object_data->width         = 1.0f;


   /** \action calculate ref point */
   reference_point = Lcda_Calculate_Target_Reference_Point(object_index, &data, FBK_SIDE_LEFT);

   /** \assert check if coordinates are correct */
   EXPECT_FLOAT_EQ(reference_point.x, -9.0f);
   EXPECT_FLOAT_EQ(reference_point.y, -9.5f);
}

/**
 * Tests that lka coordinates are transformed correctly.
 * \uts{CSCSA-73305} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Transform_Customer_Output_Coords_Rna_test)
{
   /** \arrange create lka objects */
   LKA_Object_T lka_obj;
   uint8_t object_index       = 0u;
   lka_obj.lka_curvi_pos_lat  = -5.0f;
   lka_obj.lka_curvi_pos_long = -10.0f;
   lka_obj.lka_curvi_vel_lat  = -2.0f;

   /** \action calculate new coordinates */
   Lcda_Transform_Customer_Output_Coords_Rna(&lka_obj, object_index, &data);

   /** \assert check if coordinates are correct */
   EXPECT_FLOAT_EQ(lka_obj.lka_curvi_pos_lat, 5.0f);
   EXPECT_FLOAT_EQ(lka_obj.lka_curvi_pos_long, -10.0f - p_vehicle_data->rear_axle_position);
   EXPECT_FLOAT_EQ(lka_obj.lka_curvi_vel_lat, 2.0f);
}

/**
 * Tests that x coordinates are transformed correctly.
 * \uts{CSCSA-73306} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Transform_Coord_Lka_X_test)
{
   /** \arrange create coordinate */
   float32_T coord = 5.0f;

   /** \action calculate new coordinates */

   coord = Lcda_Transform_Coord_Lka_X(coord, &data);

   /** \assert check if coordinates are correct */
   EXPECT_FLOAT_EQ(coord, 5.0f - p_vehicle_data->rear_axle_position);
}

/**
 * Tests that y coordinates are transformed correctly.
 * \uts{CSCSA-73307} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Transform_Coord_Lka_Y_test)
{
   /** \arrange create coordinate */
   float32_T coord = -5.0f;

   /** \action calculate new coordinates */
   coord = Lcda_Transform_Coord_Lka_Y(coord);

   /** \assert check if coordinates are correct */
   EXPECT_FLOAT_EQ(coord, 5.0f);
}

/**
 * Tests that lka signals are saturated correctly
 * \uts{CSCSA-73308} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Saturate_Signals_Lka_test)
{
   /** \arrange create simple ka object */
   LKA_Object_T lka_obj;
   uint8_t object_index       = 0u;
   lka_obj.lka_curvi_pos_lat  = -5.0f;
   lka_obj.lka_curvi_pos_long = -10.0f;
   lka_obj.lka_curvi_vel_long = -3.0f;
   lka_obj.lka_curvi_vel_lat  = -2.0f;
   lka_obj.lka_ttc            = 5.5f;

   /** \action saturate signals */
   Lcda_Saturate_Signals_Lka(&lka_obj, object_index);

   /** \assert check if signals are not changed */
   EXPECT_FLOAT_EQ(lka_obj.lka_curvi_pos_long, -10.0f);
   EXPECT_FLOAT_EQ(lka_obj.lka_curvi_pos_lat, -5.0f);
   EXPECT_FLOAT_EQ(lka_obj.lka_curvi_vel_long, -3.0f);
   EXPECT_FLOAT_EQ(lka_obj.lka_curvi_vel_lat, -2.0f);
   EXPECT_FLOAT_EQ(lka_obj.lka_ttc, 5.5f);
}

/**
 * Tests that lka ttc is calculated correctly
 * \uts{CSCSA-73309} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Calculate_Ttc_Lka_test)
{
   /** \arrange create simple object data */
   uint8_t object_index   = 0u;
   Vector_2d_T target_ref = Create_2d_Vector_Coordinates(-10.0f, 0.0f);
   float32_T ttc;
   object_data->curvi_vel_rel.x       = 1.0f;
   p_vehicle_data->rear_axle_position = -1.0f;
   float32_T expected_ttc =
      (-(target_ref.x - p_vehicle_data->rear_axle_position + cals.k_cvw_ttc_long_calculation_offset) / object_data->curvi_vel_rel.x);

   /** \action calculate ttc */
   ttc = Lcda_Calculate_Ttc_Lka(&data, &cals, object_index, target_ref);

   /** \assert check if ttc is correct */
   EXPECT_FLOAT_EQ(expected_ttc, ttc);
}

/**
 * Tests that alert condition is returnet correctly
 * \uts{CSCSA-73310} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Get_Alert_Condition_Lka_test)
{
   /** \arrange create simple output data */
   float32_T curvi_long_vel_rel = 10.0f;
   uint8_t tracker_id           = 2u;
   uint8_t side                 = FBK_SIDE_LEFT;
   RENAULT_LSS_ALERT_CONDITION_T result;

   lcda_output.core_output.cvw_id_left = tracker_id;


   /** \action calculate alert */
   result = Lcda_Get_Alert_Condition_Lka(&lcda_output, curvi_long_vel_rel, tracker_id, side);

   /** \assert check if RENAULT_LSS_ALERT_CONDITION_TOS is returned */
   EXPECT_EQ(result, RENAULT_LSS_ALERT_CONDITION_TOS);
}

/**
 * Tests that change status is returned correctly
 * \uts{CSCSA-73311} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Get_Change_Status_Lka_test)
{
   /** \arrange create simple output data */
   Pa_Obj_Status_T track_status = PA_OBJ_STATUS_NEW;
   RENAULT_LSS_CHANGE_STATUS_T result;

   /** \action calculate change status */
   result = Lcda_Get_Change_Status_Lka(track_status);

   /** \assert check if RENAULT_LSS_CHANGE_STATUS_CHANGE is returned */
   EXPECT_EQ(result, RENAULT_LSS_CHANGE_STATUS_CHANGE);
}

/**
 * Tests that object class is returned correctly
 * \uts{CSCSA-73312} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Get_Object_Class_Lka_test)
{
   /** \arrange create simple output data */
   Pa_Obj_Class_T obj_class = PA_OBJ_CLASS_CAR;
   RENAULT_LSS_OBJECT_CLASS_T result;

   /** \action calculate class */
   result = Lcda_Get_Object_Class_Lka(obj_class);

   /** \assert check if RENAULT_LSS_OBJECT_CLASS_CAR is returned */
   EXPECT_EQ(result, RENAULT_LSS_OBJECT_CLASS_CAR);
}

/**
 * Tests that motion class is returned correctly
 * \uts{CSCSA-73313} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Get_Motion_Class_Lka_test)
{
   /** \arrange create simple output data */
   Pa_Obj_Status_T obj_class = PA_OBJ_STATUS_MATURE;
   RENAULT_LSS_MOTION_CLASS_T result;

   /** \action calculate class */
   result = Lcda_Get_Motion_Class_Lka(obj_class);

   /** \assert check if RENAULT_LSS_MOTION_CLASS_MOVING_OBJECT is returned */
   EXPECT_EQ(result, RENAULT_LSS_MOTION_CLASS_MOVING_OBJECT);
}

/**
 * Tests that object id is returned correctly
 * \uts{CSCSA-73314} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Get_Obj_Id_Lka_test)
{
   /** \arrange create simple output data */
   uint8_t lka_obj_id = 10u;
   LKA_Object_T objects_from_prev_iteration[FBK_NUMBER_OF_SIDES][LCDA_RNA_NUMBER_OF_OBJECTS]{};
   boolean_T ids_in_use[FBK_NUMBER_OF_SIDES][LCDA_RNA_NUMBER_OF_USED_IDS]{};
   uint8_t tracker_id                                  = lka_obj_id + 1u;
   uint8_t side                                        = FBK_SIDE_LEFT;
   uint8_t lastly_freed_id                             = lka_obj_id + 2u;
   objects_from_prev_iteration[side][0].lka_tracker_id = tracker_id;
   objects_from_prev_iteration[side][0].lka_obj_id     = lka_obj_id;
   uint8_t result;

   /** \action calculate new id */
   result = Lcda_Get_Obj_Id_Lka(objects_from_prev_iteration, ids_in_use, tracker_id, side, lastly_freed_id);

   /** \assert check if value of lka_obj_id is returned */
   EXPECT_EQ(result, lka_obj_id);
}

/**
 * Tests that object in zone are detected
 * \uts{CSCSA-73356} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Check_If_Obj_In_Zone_Lka_test)
{
   /** \arrange create simple output data with object in zone */
   uint8_t tracker_index = 0u;
   uint8_t side          = FBK_SIDE_LEFT;
   boolean_T result;
   Lka_Zone.lat_end         = 10.0f;
   Lka_Zone.lat_start       = -10.0f;
   Lka_Zone.lon_end         = -10.0f;
   Lka_Zone.lon_start       = 10.0f;
   object_data->curvi_pos.x = 1.0f;
   object_data->curvi_pos.y = 1.0f;

   /** \action call function to test */
   result = Lcda_Check_If_Obj_In_Zone_Lka(&data, tracker_index, side);

   /** \assert expect that objet is in zone */
   EXPECT_TRUE(result);
}

/**
 * Tests that calibration is updated correctly.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Init_Output_test)
{
   /** \arrange declare variable for result and simple imput */
   Lcda_Output_T output;
   output.core_output.cvw_id_left = 1;
   /** \action call calibration update */
   Lcda_Init_Output(&output);
   /** \assert expect succes */
   EXPECT_EQ(output.core_output.cvw_id_left, 0);
}