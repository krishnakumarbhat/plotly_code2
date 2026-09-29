/**
 * @file fbk_vehicle_validation_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for FBK vehicle validation.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-73098}
 */

#include "fbk_vehicle_validation_test.hpp"
#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "fbk_vehicle_data_t.h"
#include "fbk_vehicle_validation.c"
#include "ml_vector_2d_t.h"
#include "pa_context.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}


/**
 * Checks whether the vehicle data is correctly set.
 * \uts{CSCSA-73099} \sdd{SF-4191} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Vehicle_Validation_Test, Fbk_Fill_Vehicle_Information_general_test)
{
   /** \arrange set typical vehicle data */
   fbk_vehicle_data.host_length        = 2.0f;
   fbk_vehicle_data.host_width         = 1.0f;
   fbk_vehicle_data.rear_axle_position = -1.6f;
   fbk_vehicle_data.wheelbase          = 1.0f;
   fbk_vehicle_data.host_speed         = 5.0f;
   fbk_vehicle_data.steering_angle     = 0.0f;
   fbk_vehicle_data.yawrate            = 0.0f;
   fbk_vehicle_data.long_vel           = 4.0f;
   fbk_vehicle_data.long_acc           = 0.0f;
   fbk_vehicle_data.lat_acc            = 0.0f;
   fbk_vehicle_data.prndl              = PA_VEH_PRNDL_STATE_PARK;
   fbk_vehicle_data.lane_width         = 3.0f;
   fbk_vehicle_data.lane_center_offset = 0.0f;
   fbk_vehicle_data.turn_signal        = 1u;
   fbk_vehicle_data.curvature          = 0.0f;
   fbk_vehicle_data.f_reverse          = FBK_FALSE;
   /** \action executes function to test. */
   Fbk_Fill_Vehicle_Information(&fbk_vehicle_data, &context);

   /** \assert expect that true is returned. */
   EXPECT_FLOAT_EQ(fbk_vehicle_data.host_length, 2.0f);
   EXPECT_FLOAT_EQ(fbk_vehicle_data.host_width, 1.0f);
   EXPECT_FLOAT_EQ(fbk_vehicle_data.rear_axle_position, -1.6f);
   EXPECT_FLOAT_EQ(fbk_vehicle_data.wheelbase, 1.0f);
   EXPECT_FLOAT_EQ(fbk_vehicle_data.host_speed, 5.0f);
   EXPECT_FLOAT_EQ(fbk_vehicle_data.steering_angle, 0.0f);
   EXPECT_FLOAT_EQ(fbk_vehicle_data.yawrate, 0.0f);
   EXPECT_FLOAT_EQ(fbk_vehicle_data.long_vel, 4.0f);
   EXPECT_FLOAT_EQ(fbk_vehicle_data.long_acc, 0.0f);
   EXPECT_FLOAT_EQ(fbk_vehicle_data.lat_acc, 0.0f);
   EXPECT_EQ(fbk_vehicle_data.prndl, PA_VEH_PRNDL_STATE_PARK);
   EXPECT_FLOAT_EQ(fbk_vehicle_data.lane_width, 3.0f);
   EXPECT_FLOAT_EQ(fbk_vehicle_data.lane_center_offset, 0.0f);
   EXPECT_EQ(fbk_vehicle_data.turn_signal, 1u);
   EXPECT_FLOAT_EQ(fbk_vehicle_data.curvature, 0.0f);
   EXPECT_FALSE(fbk_vehicle_data.f_reverse);
}

/**
 * Tests the Fbk_Verify_Vehicle_Data_Range function with valid input.
 * \uts{CSCSA-308173} \sdd{CSCSA-307401} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Vehicle_Validation_Test, Fbk_Verify_Vehicle_Data_Range__Valid_Input)
{
   /** \arrange set valid vehicle data */
   fbk_vehicle_data.host_length        = 2.0f;
   fbk_vehicle_data.host_width         = 1.0f;
   fbk_vehicle_data.rear_axle_position = -1.6f;
   fbk_vehicle_data.wheelbase          = 1.0f;
   fbk_vehicle_data.host_speed         = 5.0f;
   fbk_vehicle_data.steering_angle     = 0.0f;
   fbk_vehicle_data.yawrate            = 0.0f;
   fbk_vehicle_data.long_vel           = 4.0f;
   fbk_vehicle_data.long_acc           = 0.0f;
   fbk_vehicle_data.lat_acc            = 0.0f;
   fbk_vehicle_data.prndl              = PA_VEH_PRNDL_STATE_PARK;
   fbk_vehicle_data.lane_width         = 3.0f;
   fbk_vehicle_data.lane_center_offset = 0.0f;
   fbk_vehicle_data.turn_signal        = 1u;
   fbk_vehicle_data.curvature          = 0.0f;
   fbk_vehicle_data.f_reverse          = FBK_FALSE;
   /** \action execute function to test */
   boolean_T result = Fbk_Verify_Vehicle_Data_Range(&fbk_vehicle_data);

   /** \assert expect that true is returned */
   EXPECT_TRUE(result);
}

/**
 * Tests the Fbk_Verify_Vehicle_Data_Range function with invalid ranges.
 * \uts{CSCSA-308174} \sdd{CSCSA-307401} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Vehicle_Validation_Test, Fbk_Verify_Vehicle_Data_Range__Invalid_Ranges_Above)
{
   /** \arrange set invalid longitudinal velocity */
   fbk_vehicle_data.host_length        = FBK_HOST_LENGTH_MAX_VAL + EPSILON;
   fbk_vehicle_data.host_width         = FBK_HOST_WIDTH_MAX_VAL + EPSILON;
   fbk_vehicle_data.rear_axle_position = FBK_REAR_AXLE_POSITION_MAX_VAL + EPSILON;
   fbk_vehicle_data.wheelbase          = FBK_WHEELBASE_MAX_VAL + EPSILON;
   fbk_vehicle_data.host_speed         = FBK_HOST_SPEED_MAX_VAL + EPSILON;
   fbk_vehicle_data.steering_angle     = FBK_STEERING_ANGLE_MAX_VAL + EPSILON;
   fbk_vehicle_data.yawrate            = FBK_YAWRATE_MAX_VAL + EPSILON;
   fbk_vehicle_data.long_vel           = FBK_LONG_VEL_MAX_VAL + EPSILON;
   fbk_vehicle_data.long_acc           = FBK_LONG_ACC_MAX_VAL + EPSILON;
   fbk_vehicle_data.lat_acc            = FBK_LAT_ACC_MAX_VAL + EPSILON;
   fbk_vehicle_data.prndl              = Pa_Veh_Prndl_State_T(uint8_t(FBK_PRNDL_MAX_VAL) + 1u);
   fbk_vehicle_data.lane_width         = FBK_LANE_WIDTH_MAX_VAL + EPSILON;
   fbk_vehicle_data.lane_center_offset = FBK_LANE_CENTER_OFFSET_MAX_VAL + EPSILON;
   fbk_vehicle_data.curvature          = FBK_CURVATURE_MAX_VAL + EPSILON;
   fbk_vehicle_data.f_reverse          = FBK_TRUE; /* Valid */

   /** \action execute function to test */
   boolean_T result = Fbk_Verify_Vehicle_Data_Range(&fbk_vehicle_data);

   /** \assert expect that false is returned */
   EXPECT_FALSE(result);
}


/**
 * Tests the Fbk_Verify_Vehicle_Data_Range function with invalid host length.
 * \uts{CSCSA-308175} \sdd{CSCSA-307401} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Vehicle_Validation_Test, Fbk_Verify_Vehicle_Data_Range__Invalid_Host_Length)
{
   /** \arrange set invalid host length */
   fbk_vehicle_data.host_length = 0.0f;

   /** \action execute function to test */
   boolean_T result = Fbk_Verify_Vehicle_Data_Range(&fbk_vehicle_data);

   /** \assert expect that false is returned */
   EXPECT_FALSE(result);
}

/**
 * Tests the Fbk_Verify_Vehicle_Data_Range function with invalid host width.
 * \uts{CSCSA-308176} \sdd{CSCSA-307401} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Vehicle_Validation_Test, Fbk_Verify_Vehicle_Data_Range__Invalid_Host_Width)
{
   /** \arrange set invalid host width */
   fbk_vehicle_data.host_width = 0.0f;

   /** \action execute function to test */
   boolean_T result = Fbk_Verify_Vehicle_Data_Range(&fbk_vehicle_data);

   /** \assert expect that false is returned */
   EXPECT_FALSE(result);
}

/**
 * Tests the Fbk_Verify_Vehicle_Data_Range function with invalid rear axle position.
 * \uts{CSCSA-308177} \sdd{CSCSA-307401} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Vehicle_Validation_Test, Fbk_Verify_Vehicle_Data_Range__Invalid_Rear_Axle_Position)
{
   /** \arrange set invalid rear axle position */
   fbk_vehicle_data.rear_axle_position = -100.0f;

   /** \action execute function to test */
   boolean_T result = Fbk_Verify_Vehicle_Data_Range(&fbk_vehicle_data);

   /** \assert expect that false is returned */
   EXPECT_FALSE(result);
}

/**
 * Tests the Fbk_Verify_Vehicle_Data_Range function with invalid wheelbase.
 * \uts{CSCSA-308178} \sdd{CSCSA-307401} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Vehicle_Validation_Test, Fbk_Verify_Vehicle_Data_Range__Invalid_Wheelbase)
{
   /** \arrange set invalid wheelbase */
   fbk_vehicle_data.wheelbase = -0.65f;

   /** \action execute function to test */
   boolean_T result = Fbk_Verify_Vehicle_Data_Range(&fbk_vehicle_data);

   /** \assert expect that false is returned */
   EXPECT_FALSE(result);
}

/**
 * Tests the Fbk_Verify_Vehicle_Data_Range function with invalid host speed.
 * \uts{CSCSA-308179} \sdd{CSCSA-307401} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Vehicle_Validation_Test, Fbk_Verify_Vehicle_Data_Range__Invalid_Host_Speed)
{
   /** \arrange set invalid host speed */
   fbk_vehicle_data.host_speed = -100.0f;

   /** \action execute function to test */
   boolean_T result = Fbk_Verify_Vehicle_Data_Range(&fbk_vehicle_data);

   /** \assert expect that false is returned */
   EXPECT_FALSE(result);
}

/**
 * Tests the Fbk_Verify_Vehicle_Data_Range function with invalid steering angle.
 * \uts{CSCSA-308180} \sdd{CSCSA-307401} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Vehicle_Validation_Test, Fbk_Verify_Vehicle_Data_Range__Invalid_Steering_Angle)
{
   /** \arrange set invalid steering angle */
   fbk_vehicle_data.steering_angle = -91.0f;

   /** \action execute function to test */
   boolean_T result = Fbk_Verify_Vehicle_Data_Range(&fbk_vehicle_data);

   /** \assert expect that false is returned */
   EXPECT_FALSE(result);
}

/**
 * Tests the Fbk_Verify_Vehicle_Data_Range function with invalid yaw rate.
 * \uts{CSCSA-308181} \sdd{CSCSA-307401} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Vehicle_Validation_Test, Fbk_Verify_Vehicle_Data_Range__Invalid_Yaw_Rate)
{
   /** \arrange set invalid yaw rate */
   fbk_vehicle_data.yawrate = -58.1f;

   /** \action execute function to test */
   boolean_T result = Fbk_Verify_Vehicle_Data_Range(&fbk_vehicle_data);

   /** \assert expect that false is returned */
   EXPECT_FALSE(result);
}

/**
 * Tests the Fbk_Verify_Vehicle_Data_Range function with invalid longitudinal velocity.
 * \uts{CSCSA-308182} \sdd{CSCSA-307401} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Vehicle_Validation_Test, Fbk_Verify_Vehicle_Data_Range__Invalid_Long_Vel)
{
   /** \arrange set invalid longitudinal velocity */
   fbk_vehicle_data.long_vel = -100.0f;

   /** \action execute function to test */
   boolean_T result = Fbk_Verify_Vehicle_Data_Range(&fbk_vehicle_data);

   /** \assert expect that false is returned */
   EXPECT_FALSE(result);
}

/**
 * Tests the Fbk_Verify_Vehicle_Data_Range function with invalid longitudinal acceleration.
 * \uts{CSCSA-308183} \sdd{CSCSA-307401} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Vehicle_Validation_Test, Fbk_Verify_Vehicle_Data_Range__Invalid_Long_Acc)
{
   /** \arrange set invalid longitudinal acceleration */
   fbk_vehicle_data.long_acc = -55.0f;

   /** \action execute function to test */
   boolean_T result = Fbk_Verify_Vehicle_Data_Range(&fbk_vehicle_data);

   /** \assert expect that false is returned */
   EXPECT_FALSE(result);
}

/**
 * Tests the Fbk_Verify_Vehicle_Data_Range function with invalid lateral acceleration.
 * \uts{CSCSA-308184} \sdd{CSCSA-307401} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Vehicle_Validation_Test, Fbk_Verify_Vehicle_Data_Range__Invalid_Lat_Acc)
{
   /** \arrange set invalid lateral acceleration */
   fbk_vehicle_data.lat_acc = -40.0f;

   /** \action execute function to test */
   boolean_T result = Fbk_Verify_Vehicle_Data_Range(&fbk_vehicle_data);

   /** \assert expect that false is returned */
   EXPECT_FALSE(result);
}


/**
 * Tests the Fbk_Verify_Vehicle_Data_Range function with invalid lane width.
 * \uts{CSCSA-308185} \sdd{CSCSA-307401} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Vehicle_Validation_Test, Fbk_Verify_Vehicle_Data_Range__Invalid_Lane_Width)
{
   /** \arrange set invalid lane width */
   fbk_vehicle_data.lane_width = -1.0f;

   /** \action execute function to test */
   boolean_T result = Fbk_Verify_Vehicle_Data_Range(&fbk_vehicle_data);

   /** \assert expect that false is returned */
   EXPECT_FALSE(result);
}

/**
 * Tests the Fbk_Verify_Vehicle_Data_Range function with invalid lane center offset.
 * \uts{CSCSA-308186} \sdd{CSCSA-307401} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Vehicle_Validation_Test, Fbk_Verify_Vehicle_Data_Range__Invalid_Lane_Center_Offset)
{
   /** \arrange set invalid lane center offset */
   fbk_vehicle_data.lane_center_offset = -100.0f;

   /** \action execute function to test */
   boolean_T result = Fbk_Verify_Vehicle_Data_Range(&fbk_vehicle_data);

   /** \assert expect that false is returned */
   EXPECT_FALSE(result);
}

/**
 * Tests the Fbk_Verify_Vehicle_Data_Range function with invalid turn signal.
 * \uts{CSCSA-308187} \sdd{CSCSA-307401} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Vehicle_Validation_Test, Fbk_Verify_Vehicle_Data_Range__Invalid_Turn_Signal)
{
   /** \arrange set invalid turn signal */
   fbk_vehicle_data.turn_signal = 4u;

   /** \action execute function to test */
   boolean_T result = Fbk_Verify_Vehicle_Data_Range(&fbk_vehicle_data);

   /** \assert expect that false is returned */
   EXPECT_FALSE(result);
}

/**
 * Tests the Fbk_Verify_Vehicle_Data_Range function with invalid curvature.
 * \uts{CSCSA-308188} \sdd{CSCSA-307401} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Vehicle_Validation_Test, Fbk_Verify_Vehicle_Data_Range__Invalid_Curvature)
{
   /** \arrange set invalid curvature */
   fbk_vehicle_data.curvature = FBK_CURVATURE_MIN_VAL - EPSILON;

   /** \action execute function to test */
   boolean_T result = Fbk_Verify_Vehicle_Data_Range(&fbk_vehicle_data);

   /** \assert expect that false is returned */
   EXPECT_FALSE(result);
}

/**
 * Tests the Fbk_Verify_Vehicle_Data_Range function with invalid f_reverse.
 * \uts{CSCSA-308189} \sdd{CSCSA-307401} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Vehicle_Validation_Test, Fbk_Verify_Vehicle_Data_Range__Invalid_F_Reverse)
{
   /** \arrange set invalid f_reverse */
   fbk_vehicle_data.f_reverse = 2;

   /** \action execute function to test */
   boolean_T result = Fbk_Verify_Vehicle_Data_Range(&fbk_vehicle_data);

   /** \assert expect that false is returned */
   EXPECT_FALSE(result);
}
