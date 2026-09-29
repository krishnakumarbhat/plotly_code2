/** \file
 * This file contains unit tests for content of f360_update_object_reference_point_support_functions.cpp file
 */

#include "f360_update_object_reference_point_support_functions.h"
#include "f360_iterator.h"
#include <CppUTest/TestHarness.h>

//#include "headerfile_needed.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  Update_Object_KF_States_After_Reference_Point_Change_CCA
 *  @{
 */

/** \brief
 *  Testing function Update_Object_KF_States_After_Reference_Point_Change for CCA objects. A CCA object is set up with an initial reference point and corresponding KF states.
 *  Then the reference point is changed to all other possible options to make sure all updated KF states are correct.
 *  All test cases are generated with the help of the Matlab script plto_new_ref_point.m that can be found at https://jiraprod.aptiv.com/browse/DFD-1534.
 **/
TEST_GROUP(Update_Object_KF_States_After_Reference_Point_Change_CCA)
{
   // Common setup for test
   F360_Object_Track_T obj;
   float32_t yaw_rate;

   float32_t exp_x_pos;
   float32_t exp_y_pos;
   float32_t exp_x_vel;
   float32_t exp_y_vel;
   float32_t exp_heading;
   float32_t exp_pointing;
   float32_t exp_speed;
   float32_t exp_curvature;

   float32_t exp_pos_residual_x;
   float32_t exp_pos_residual_y;
   float32_t exp_vel_residual_x;
   float32_t exp_vel_residual_y;
   float32_t exp_heading_residual;
   float32_t exp_speed_residual;

   float32_t test_thresh = 0.001F;
   /** \setup
   * - Set up a movable CCA object with
   *     - Center of bbox at (5,7)
   *     - Orientation (pointing) -45 degrees
   *     - Length 6m
   *     - Width 2m
   * - Reference point set to LEFT.
   *     - With position (4.2929, 6.2929)
   *     - Velocity (2.8284, -2.8284) m/s
   *     - Heading -45 degrees
   *     - Speed 4 m/s
   *     - Curvature -0.03 (1/m)
   *     - Predicted position (4,6)
   *     - Predicted velocity (3,-3)
   *     - Predicted heading -43 degrees
   *     - Predicted speed is 4.2 m/s    
   **/
   TEST_SETUP()
   {      
      obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
      obj.movable_prob = 1.0F;
      obj.f_moving = true;
      obj.bbox.Set_Center({5.0F, 7.0F});
      obj.bbox.Set_Orientation(-0.7854F); //-45 degrees
      obj.bbox.Set_Length(6.0F);
      obj.bbox.Set_Width(2.0F);
      obj.vcs_heading = Angle(-0.7854F);
      obj.hdg_ptng_disagmt = obj.bbox.Get_Orientation().Value() - obj.vcs_heading.Value();
      obj.curvature = -0.03F;
      obj.speed = 4.0F;
      obj.reference_point = F360_REFERENCE_POINT_LEFT;
      obj.vcs_position.x = 4.2929F;
      obj.vcs_position.y = 6.2929F;
      obj.vcs_velocity.longitudinal = 2.8284F;
      obj.vcs_velocity.lateral = - 2.8284F;
      obj.predicted_vcs_position.x = 4.0F;
      obj.predicted_vcs_position.y = 6.0F;
      obj.predicted_vcs_velocity.longitudinal = 3.0F;
      obj.predicted_vcs_velocity.lateral = -3.0F;
      obj.predicted_speed = 4.2;

      yaw_rate = obj.curvature * obj.speed;

      // Expected residuals
      exp_pos_residual_x = obj.predicted_vcs_position.x - obj.vcs_position.x;
      exp_pos_residual_y = obj.predicted_vcs_position.y - obj.vcs_position.y;
      exp_vel_residual_x = obj.predicted_vcs_velocity.longitudinal - obj.vcs_velocity.longitudinal;
      exp_vel_residual_y = obj.predicted_vcs_velocity.lateral - obj.vcs_velocity.lateral;
      exp_speed_residual = obj.predicted_speed - obj.speed;
      exp_pointing = obj.bbox.Get_Orientation().Value(); //Pointing is expected to be constant 
   }
};

/**
*\purpose  Check that object's new position is correctly updated when reference point is changed to front left.
*\req    NA
*/
TEST(Update_Object_KF_States_After_Reference_Point_Change_CCA, New_Ref_Point_Front_Left)
{
   /** \precond
   * - An object with initial reference point LEFT is set up in TEST_GROUP
   * - Change object's reference point to FRONT LEFT
   * - Set expected data accordingly
   **/
   obj.reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
   exp_x_pos = 6.4142F;
   exp_y_pos = 4.1716F;   

   /** \action
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   Update_Object_KF_States_After_Reference_Point_Change(obj);

   // Calculate KF residuals after reference point change
   float32_t new_pos_residual_x = obj.predicted_vcs_position.x - obj.vcs_position.x;
   float32_t new_pos_residual_y = obj.predicted_vcs_position.y - obj.vcs_position.y;

   /** \result
   * Check that the object's position and predicted position are updated correctly.
   **/
   DOUBLES_EQUAL_TEXT(exp_x_pos, obj.vcs_position.x, test_thresh, "Object's xpos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_y_pos, obj.vcs_position.y, test_thresh, "Object's ypos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_x, new_pos_residual_x, test_thresh, "xpos residual has changed. It should be constant.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_y, new_pos_residual_y, test_thresh, "ypos residual has changed. It should be constant.");
}

/**
*\purpose  Check that object's new position is correctly updated when reference point is changed to FRONT.
*\req    NA
*/
TEST(Update_Object_KF_States_After_Reference_Point_Change_CCA, New_Ref_Point_Front)
{
   /** \precond
   * - An object with initial reference point LEFT is set up in TEST_GROUP
   * - Change object's reference point to FRONT
   * - Set expected data accordingly
   **/
   obj.reference_point = F360_REFERENCE_POINT_FRONT;
   exp_x_pos = 7.1213F;
   exp_y_pos = 4.8787F;
   

   /** \action
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   Update_Object_KF_States_After_Reference_Point_Change(obj);
   
   // Calculate KF residuals after reference point change
   float32_t new_pos_residual_x = obj.predicted_vcs_position.x - obj.vcs_position.x;
   float32_t new_pos_residual_y = obj.predicted_vcs_position.y - obj.vcs_position.y;
   
   /** \result
   * Check that the object's position and predicted position are updated correctly.
   **/
   DOUBLES_EQUAL_TEXT(exp_x_pos, obj.vcs_position.x, test_thresh, "Object's xpos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_y_pos, obj.vcs_position.y, test_thresh, "Object's ypos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_x, new_pos_residual_x, test_thresh, "xpos residual has changed. It should be constant.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_y, new_pos_residual_y, test_thresh, "ypos residual has changed. It should be constant.");
}

/**
*\purpose  Check that object's new position is correctly updated when reference point is changed to FRONT RIGHT.
*\req    NA
*/
TEST(Update_Object_KF_States_After_Reference_Point_Change_CCA, New_Ref_Point_Front_Right)
{
   /** \precond
   * - An object with initial reference point LEFT is set up in TEST_GROUP
   * - Change object's reference point to FRONT RIGHT
   * - Set expected data accordingly
   **/
   obj.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;
   exp_x_pos = 7.8284F;
   exp_y_pos = 5.5858F;
   

   /** \action
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   Update_Object_KF_States_After_Reference_Point_Change(obj);
   
   // Calculate KF residuals after reference point change
   float32_t new_pos_residual_x = obj.predicted_vcs_position.x - obj.vcs_position.x;
   float32_t new_pos_residual_y = obj.predicted_vcs_position.y - obj.vcs_position.y;

   /** \result
   * Check that the object's position and predicted position are updated correctly.
   **/
   DOUBLES_EQUAL_TEXT(exp_x_pos, obj.vcs_position.x, test_thresh, "Object's xpos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_y_pos, obj.vcs_position.y, test_thresh, "Object's ypos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_x, new_pos_residual_x, test_thresh, "xpos residual has changed. It should be constant.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_y, new_pos_residual_y, test_thresh, "ypos residual has changed. It should be constant.");
}

/**
*\purpose  Check that object's new position is correctly updated when reference point is changed to RIGHT.
*\req    NA
*/
TEST(Update_Object_KF_States_After_Reference_Point_Change_CCA, New_Ref_Point_Right)
{
   /** \precond
   * - An object with initial reference point LEFT is set up in TEST_GROUP
   * - Change object's reference point to RIGHT
   * - Set expected data accordingly
   **/
   obj.reference_point = F360_REFERENCE_POINT_RIGHT;
   exp_x_pos = 5.7071F;
   exp_y_pos = 7.7071F;
   

   /** \action
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   Update_Object_KF_States_After_Reference_Point_Change(obj);
   
   // Calculate KF residuals after reference point change
   float32_t new_pos_residual_x = obj.predicted_vcs_position.x - obj.vcs_position.x;
   float32_t new_pos_residual_y = obj.predicted_vcs_position.y - obj.vcs_position.y;

   /** \result
   * Check that the object's position and predicted position are updated correctly.
   **/
   DOUBLES_EQUAL_TEXT(exp_x_pos, obj.vcs_position.x, test_thresh, "Object's xpos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_y_pos, obj.vcs_position.y, test_thresh, "Object's ypos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_x, new_pos_residual_x, test_thresh, "xpos residual has changed. It should be constant.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_y, new_pos_residual_y, test_thresh, "ypos residual has changed. It should be constant.");
}

/**
*\purpose  Check that object's new position is correctly updated when reference point is changed to REAR RIGHT.
*\req    NA
*/
TEST(Update_Object_KF_States_After_Reference_Point_Change_CCA, New_Ref_Point_Rear_Right)
{
   /** \precond
   * - An object with initial reference point LEFT is set up in TEST_GROUP
   * - Change object's reference point to REAR RIGHT
   * - Set expected data accordingly
   **/
   obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
   exp_x_pos = 3.5858F;
   exp_y_pos = 9.8284F;
   

   /** \action
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   Update_Object_KF_States_After_Reference_Point_Change(obj);
   
   // Calculate KF residuals after reference point change
   float32_t new_pos_residual_x = obj.predicted_vcs_position.x - obj.vcs_position.x;
   float32_t new_pos_residual_y = obj.predicted_vcs_position.y - obj.vcs_position.y;

   /** \result
   * Check that the object's position and predicted position are updated correctly.
   **/
   DOUBLES_EQUAL_TEXT(exp_x_pos, obj.vcs_position.x, test_thresh, "Object's xpos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_y_pos, obj.vcs_position.y, test_thresh, "Object's ypos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_x, new_pos_residual_x, test_thresh, "xpos residual has changed. It should be constant.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_y, new_pos_residual_y, test_thresh, "ypos residual has changed. It should be constant.");
}

/**
*\purpose  Check that object's new position is correctly updated when reference point is changed to REAR.
*\req    NA
*/
TEST(Update_Object_KF_States_After_Reference_Point_Change_CCA, New_Ref_Point_Rear)
{
   /** \precond
   * - An object with initial reference point LEFT is set up in TEST_GROUP
   * - Change object's reference point to REAR
   * - Set expected data accordingly
   **/
   obj.reference_point = F360_REFERENCE_POINT_REAR;
   exp_x_pos = 2.8787F;
   exp_y_pos = 9.1213F;
   

   /** \action
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   Update_Object_KF_States_After_Reference_Point_Change(obj);
   
   // Calculate KF residuals after reference point change
   float32_t new_pos_residual_x = obj.predicted_vcs_position.x - obj.vcs_position.x;
   float32_t new_pos_residual_y = obj.predicted_vcs_position.y - obj.vcs_position.y;

   /** \result
   * Check that the object's position and predicted position are updated correctly.
   **/
   DOUBLES_EQUAL_TEXT(exp_x_pos, obj.vcs_position.x, test_thresh, "Object's xpos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_y_pos, obj.vcs_position.y, test_thresh, "Object's ypos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_x, new_pos_residual_x, test_thresh, "xpos residual has changed. It should be constant.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_y, new_pos_residual_y, test_thresh, "ypos residual has changed. It should be constant.");
}

/**
*\purpose  Check that object's new position is correctly updated when reference point is changed to REAR LEFT.
*\req    NA
*/
TEST(Update_Object_KF_States_After_Reference_Point_Change_CCA, New_Ref_Point_Rear_Left)
{
   /** \precond
   * - An object with initial reference point LEFT is set up in TEST_GROUP
   * - Change object's reference point to REAR LEFT
   * - Set expected data accordingly
   **/
   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   exp_x_pos = 2.1716F;
   exp_y_pos = 8.4142F;
   

   /** \action
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   Update_Object_KF_States_After_Reference_Point_Change(obj);
   
   // Calculate KF residuals after reference point change
   float32_t new_pos_residual_x = obj.predicted_vcs_position.x - obj.vcs_position.x;
   float32_t new_pos_residual_y = obj.predicted_vcs_position.y - obj.vcs_position.y;

   /** \result
   * Check that the object's position and predicted position are updated correctly.
   **/
   DOUBLES_EQUAL_TEXT(exp_x_pos, obj.vcs_position.x, test_thresh, "Object's xpos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_y_pos, obj.vcs_position.y, test_thresh, "Object's ypos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_x, new_pos_residual_x, test_thresh, "xpos residual has changed. It should be constant.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_y, new_pos_residual_y, test_thresh, "ypos residual has changed. It should be constant.");
}

/**
*\purpose  Check that object's new position is correctly updated when reference point is changed to CENTER.
*\req    NA
*/
TEST(Update_Object_KF_States_After_Reference_Point_Change_CCA, New_Ref_Point_Center)
{
   /** \precond
   * - An object with initial reference point LEFT is set up in TEST_GROUP
   * - Change object's reference point to CENTER
   * - Set expected data accordingly
   **/
   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   exp_x_pos = 5.0F;
   exp_y_pos = 7.0F;
   

   /** \action
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   Update_Object_KF_States_After_Reference_Point_Change(obj);
   
   // Calculate KF residuals after reference point change
   float32_t new_pos_residual_x = obj.predicted_vcs_position.x - obj.vcs_position.x;
   float32_t new_pos_residual_y = obj.predicted_vcs_position.y - obj.vcs_position.y;

   /** \result
   * Check that the object's position and predicted position are updated correctly.
   **/
   DOUBLES_EQUAL_TEXT(exp_x_pos, obj.vcs_position.x, test_thresh, "Object's xpos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_y_pos, obj.vcs_position.y, test_thresh, "Object's ypos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_x, new_pos_residual_x, test_thresh, "xpos residual has changed. It should be constant.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_y, new_pos_residual_y, test_thresh, "ypos residual has changed. It should be constant.");
}

/**
*\purpose  Check that object's new position is correctly updated when reference point is changed to LEFT from REAR LEFT.
*\req    NA
*/
TEST(Update_Object_KF_States_After_Reference_Point_Change_CCA, New_Ref_Point_Left)
{
   /** \precond
   * - Set up an object with
   *     - Center of bbox at (5,7)
   *     - Orientation (pointing) -45 degrees
   *     - Length 6m
   *     - Width 2m
   * - Reference point assume to be REAR LEFT.
   *     - With position (2.1716, 8.4142)
   *     - Velocity (2.8284, -2.8284) m/s
   *     - Heading -45 degrees
   *     - Speed 4 m/s
   *     - Curvature -0.03 (1/m)
   *     - Predicted position (4,6)
   *     - Predicted velocity (3,-3)
   *     - Predicted heading -43 degrees
   *     - Predicted speed is 4.2 m/s    
   * - Change object's reference point to LEFT
   * - Set expected data accordingly
   **/
   obj.vcs_position.x = 2.1716F;
   obj.vcs_position.y = 8.4142F;
   obj.vcs_velocity.longitudinal = 2.8284F;
   obj.vcs_velocity.lateral = -2.8284F;
   obj.predicted_vcs_position.x = 2.3F;
   obj.predicted_vcs_position.y = 8.5F;

   obj.reference_point = F360_REFERENCE_POINT_LEFT;
   exp_x_pos = 4.2929F;
   exp_y_pos = 6.2929F;

   // Expected residuals
   exp_pos_residual_x = obj.predicted_vcs_position.x - obj.vcs_position.x;
   exp_pos_residual_y = obj.predicted_vcs_position.y - obj.vcs_position.y;
   
   /** \action
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   Update_Object_KF_States_After_Reference_Point_Change(obj);
   
   // Calculate KF residuals after reference point change
   float32_t new_pos_residual_x = obj.predicted_vcs_position.x - obj.vcs_position.x;
   float32_t new_pos_residual_y = obj.predicted_vcs_position.y - obj.vcs_position.y;
   
   /** \result
   * Check that the object's position and predicted position are updated correctly.
   **/
   DOUBLES_EQUAL_TEXT(exp_x_pos, obj.vcs_position.x, test_thresh, "Object's xpos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_y_pos, obj.vcs_position.y, test_thresh, "Object's ypos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_x, new_pos_residual_x, test_thresh, "xpos residual has changed. It should be constant.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_y, new_pos_residual_y, test_thresh, "ypos residual has changed. It should be constant.");
}

/** @}*/

/** \defgroup  Update_Object_KF_States_After_Reference_Point_Change_CTCA
 *  @{
 */

/** \brief
 *  Testing function Update_Object_KF_States_After_Reference_Point_Change for CTCA objects. A CTCA object is set up with an initial reference point and corresponding KF states.
 *  Then the reference point is changed to all other possible options to make sure all updated KF states are correct.
 *  All test cases are generated with the help of the Matlab script plto_new_ref_point.m that can be found at https://jiraprod.aptiv.com/browse/DFD-1534.
 **/
TEST_GROUP(Update_Object_KF_States_After_Reference_Point_Change_CTCA)
{
   // Common setup for test
   F360_Object_Track_T obj;
   float32_t yaw_rate;

   float32_t exp_x_pos;
   float32_t exp_y_pos;
   float32_t exp_x_vel;
   float32_t exp_y_vel;
   float32_t exp_heading;
   float32_t exp_pointing;
   float32_t exp_speed;
   float32_t exp_curvature;

   float32_t exp_pos_residual_x;
   float32_t exp_pos_residual_y;
   float32_t exp_vel_residual_x;
   float32_t exp_vel_residual_y;
   float32_t exp_heading_residual;
   float32_t exp_speed_residual;

   float32_t test_thresh = 0.001F;
   /** \setup
   * - Set up a movable CCA object with
   *     - Center of bbox at (5,7)
   *     - Orientation (pointing) -45 degrees
   *     - Length 6m
   *     - Width 2m
   * - Reference point set to LEFT.
   *     - With position (4.2929, 6.2929)
   *     - Velocity (2.8284, -2.8284) m/s
   *     - Heading -45 degrees
   *     - Speed 4 m/s
   *     - Curvature -0.03 (1/m)
   *     - Predicted position (4,6)
   *     - Predicted velocity (3,-3)
   *     - Predicted heading -43 degrees
   *     - Predicted speed is 4.2 m/s    
   **/
   TEST_SETUP()
   {      
      obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      obj.movable_prob = 1.0F;
      obj.f_moving = true;
      obj.bbox.Set_Center({5.0F, 7.0F});
      obj.bbox.Set_Orientation(-0.7854F); //-45 degrees
      obj.bbox.Set_Length(6.0F);
      obj.bbox.Set_Width(2.0F);
      obj.vcs_heading = Angle(-0.7854F);
      obj.hdg_ptng_disagmt = obj.bbox.Get_Orientation().Value() - obj.vcs_heading.Value();
      obj.curvature = -0.03F;
      obj.speed = 4.0F;
      obj.reference_point = F360_REFERENCE_POINT_LEFT;
      obj.vcs_position.x = 4.2929F;
      obj.vcs_position.y = 6.2929F;
      obj.vcs_velocity.longitudinal = 2.8284F;
      obj.vcs_velocity.lateral = - 2.8284F;
      obj.predicted_vcs_position.x = 4.0F;
      obj.predicted_vcs_position.y = 6.0F;
      obj.predicted_vcs_velocity.longitudinal = 3.0F;
      obj.predicted_vcs_velocity.lateral = -3.0F;
      obj.predicted_speed = 4.2;

      yaw_rate = obj.curvature * obj.speed;

      // Expected residuals
      exp_pos_residual_x = obj.predicted_vcs_position.x - obj.vcs_position.x;
      exp_pos_residual_y = obj.predicted_vcs_position.y - obj.vcs_position.y;
      exp_vel_residual_x = obj.predicted_vcs_velocity.longitudinal - obj.vcs_velocity.longitudinal;
      exp_vel_residual_y = obj.predicted_vcs_velocity.lateral - obj.vcs_velocity.lateral;
      exp_speed_residual = obj.predicted_speed - obj.speed;
      exp_pointing = obj.bbox.Get_Orientation().Value(); //Pointing is expected to be constant 
   }
};

/**
*\purpose  Check that object's new position is correctly updated when reference point is changed to front left.
*\req    NA
*/
TEST(Update_Object_KF_States_After_Reference_Point_Change_CTCA, New_Ref_Point_Front_Left)
{
   /** \precond
   * - An object with initial reference point LEFT is set up in TEST_GROUP
   * - Change object's reference point to FRONT LEFT
   * - Set expected data accordingly
   **/
   obj.reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
   exp_x_pos = 6.4142F;
   exp_y_pos = 4.1716F;   

   /** \action
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   Update_Object_KF_States_After_Reference_Point_Change(obj);

   // Calculate KF residuals after reference point change
   float32_t new_pos_residual_x = obj.predicted_vcs_position.x - obj.vcs_position.x;
   float32_t new_pos_residual_y = obj.predicted_vcs_position.y - obj.vcs_position.y;

   /** \result
   * Check that the object's position and predicted position are updated correctly.
   **/
   DOUBLES_EQUAL_TEXT(exp_x_pos, obj.vcs_position.x, test_thresh, "Object's xpos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_y_pos, obj.vcs_position.y, test_thresh, "Object's ypos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_x, new_pos_residual_x, test_thresh, "xpos residual has changed. It should be constant.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_y, new_pos_residual_y, test_thresh, "ypos residual has changed. It should be constant.");
}

/**
*\purpose  Check that object's new position is correctly updated when reference point is changed to FRONT.
*\req    NA
*/
TEST(Update_Object_KF_States_After_Reference_Point_Change_CTCA, New_Ref_Point_Front)
{
   /** \precond
   * - An object with initial reference point LEFT is set up in TEST_GROUP
   * - Change object's reference point to FRONT
   * - Set expected data accordingly
   **/
   obj.reference_point = F360_REFERENCE_POINT_FRONT;
   exp_x_pos = 7.1213F;
   exp_y_pos = 4.8787F;
   

   /** \action
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   Update_Object_KF_States_After_Reference_Point_Change(obj);
   
   // Calculate KF residuals after reference point change
   float32_t new_pos_residual_x = obj.predicted_vcs_position.x - obj.vcs_position.x;
   float32_t new_pos_residual_y = obj.predicted_vcs_position.y - obj.vcs_position.y;
   
   /** \result
   * Check that the object's position and predicted position are updated correctly.
   **/
   DOUBLES_EQUAL_TEXT(exp_x_pos, obj.vcs_position.x, test_thresh, "Object's xpos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_y_pos, obj.vcs_position.y, test_thresh, "Object's ypos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_x, new_pos_residual_x, test_thresh, "xpos residual has changed. It should be constant.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_y, new_pos_residual_y, test_thresh, "ypos residual has changed. It should be constant.");
}

/**
*\purpose  Check that object's new position is correctly updated when reference point is changed to FRONT RIGHT.
*\req    NA
*/
TEST(Update_Object_KF_States_After_Reference_Point_Change_CTCA, New_Ref_Point_Front_Right)
{
   /** \precond
   * - An object with initial reference point LEFT is set up in TEST_GROUP
   * - Change object's reference point to FRONT RIGHT
   * - Set expected data accordingly
   **/
   obj.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;
   exp_x_pos = 7.8284F;
   exp_y_pos = 5.5858F;
   

   /** \action
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   Update_Object_KF_States_After_Reference_Point_Change(obj);
   
   // Calculate KF residuals after reference point change
   float32_t new_pos_residual_x = obj.predicted_vcs_position.x - obj.vcs_position.x;
   float32_t new_pos_residual_y = obj.predicted_vcs_position.y - obj.vcs_position.y;

   /** \result
   * Check that the object's position and predicted position are updated correctly.
   **/
   DOUBLES_EQUAL_TEXT(exp_x_pos, obj.vcs_position.x, test_thresh, "Object's xpos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_y_pos, obj.vcs_position.y, test_thresh, "Object's ypos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_x, new_pos_residual_x, test_thresh, "xpos residual has changed. It should be constant.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_y, new_pos_residual_y, test_thresh, "ypos residual has changed. It should be constant.");
}

/**
*\purpose  Check that object's new position is correctly updated when reference point is changed to RIGHT.
*\req    NA
*/
TEST(Update_Object_KF_States_After_Reference_Point_Change_CTCA, New_Ref_Point_Right)
{
   /** \precond
   * - An object with initial reference point LEFT is set up in TEST_GROUP
   * - Change object's reference point to RIGHT
   * - Set expected data accordingly
   **/
   obj.reference_point = F360_REFERENCE_POINT_RIGHT;
   exp_x_pos = 5.7071F;
   exp_y_pos = 7.7071F;
   

   /** \action
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   Update_Object_KF_States_After_Reference_Point_Change(obj);
   
   // Calculate KF residuals after reference point change
   float32_t new_pos_residual_x = obj.predicted_vcs_position.x - obj.vcs_position.x;
   float32_t new_pos_residual_y = obj.predicted_vcs_position.y - obj.vcs_position.y;

   /** \result
   * Check that the object's position and predicted position are updated correctly.
   **/
   DOUBLES_EQUAL_TEXT(exp_x_pos, obj.vcs_position.x, test_thresh, "Object's xpos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_y_pos, obj.vcs_position.y, test_thresh, "Object's ypos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_x, new_pos_residual_x, test_thresh, "xpos residual has changed. It should be constant.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_y, new_pos_residual_y, test_thresh, "ypos residual has changed. It should be constant.");
}

/**
*\purpose  Check that object's new position is correctly updated when reference point is changed to REAR RIGHT.
*\req    NA
*/
TEST(Update_Object_KF_States_After_Reference_Point_Change_CTCA, New_Ref_Point_Rear_Right)
{
   /** \precond
   * - An object with initial reference point LEFT is set up in TEST_GROUP
   * - Change object's reference point to REAR RIGHT
   * - Set expected data accordingly
   **/
   obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
   exp_x_pos = 3.5858F;
   exp_y_pos = 9.8284F;
   

   /** \action
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   Update_Object_KF_States_After_Reference_Point_Change(obj);
   
   // Calculate KF residuals after reference point change
   float32_t new_pos_residual_x = obj.predicted_vcs_position.x - obj.vcs_position.x;
   float32_t new_pos_residual_y = obj.predicted_vcs_position.y - obj.vcs_position.y;

   /** \result
   * Check that the object's position and predicted position are updated correctly.
   **/
   DOUBLES_EQUAL_TEXT(exp_x_pos, obj.vcs_position.x, test_thresh, "Object's xpos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_y_pos, obj.vcs_position.y, test_thresh, "Object's ypos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_x, new_pos_residual_x, test_thresh, "xpos residual has changed. It should be constant.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_y, new_pos_residual_y, test_thresh, "ypos residual has changed. It should be constant.");
}

/**
*\purpose  Check that object's new position is correctly updated when reference point is changed to REAR.
*\req    NA
*/
TEST(Update_Object_KF_States_After_Reference_Point_Change_CTCA, New_Ref_Point_Rear)
{
   /** \precond
   * - An object with initial reference point LEFT is set up in TEST_GROUP
   * - Change object's reference point to REAR
   * - Set expected data accordingly
   **/
   obj.reference_point = F360_REFERENCE_POINT_REAR;
   exp_x_pos = 2.8787F;
   exp_y_pos = 9.1213F;
   

   /** \action
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   Update_Object_KF_States_After_Reference_Point_Change(obj);
   
   // Calculate KF residuals after reference point change
   float32_t new_pos_residual_x = obj.predicted_vcs_position.x - obj.vcs_position.x;
   float32_t new_pos_residual_y = obj.predicted_vcs_position.y - obj.vcs_position.y;

   /** \result
   * Check that the object's position and predicted position are updated correctly.
   **/
   DOUBLES_EQUAL_TEXT(exp_x_pos, obj.vcs_position.x, test_thresh, "Object's xpos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_y_pos, obj.vcs_position.y, test_thresh, "Object's ypos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_x, new_pos_residual_x, test_thresh, "xpos residual has changed. It should be constant.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_y, new_pos_residual_y, test_thresh, "ypos residual has changed. It should be constant.");
}

/**
*\purpose  Check that object's new position is correctly updated when reference point is changed to REAR LEFT.
*\req    NA
*/
TEST(Update_Object_KF_States_After_Reference_Point_Change_CTCA, New_Ref_Point_Rear_Left)
{
   /** \precond
   * - An object with initial reference point LEFT is set up in TEST_GROUP
   * - Change object's reference point to REAR LEFT
   * - Set expected data accordingly
   **/
   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   exp_x_pos = 2.1716F;
   exp_y_pos = 8.4142F;
   

   /** \action
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   Update_Object_KF_States_After_Reference_Point_Change(obj);
   
   // Calculate KF residuals after reference point change
   float32_t new_pos_residual_x = obj.predicted_vcs_position.x - obj.vcs_position.x;
   float32_t new_pos_residual_y = obj.predicted_vcs_position.y - obj.vcs_position.y;

   /** \result
   * Check that the object's position and predicted position are updated correctly.
   **/
   DOUBLES_EQUAL_TEXT(exp_x_pos, obj.vcs_position.x, test_thresh, "Object's xpos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_y_pos, obj.vcs_position.y, test_thresh, "Object's ypos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_x, new_pos_residual_x, test_thresh, "xpos residual has changed. It should be constant.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_y, new_pos_residual_y, test_thresh, "ypos residual has changed. It should be constant.");
}

/**
*\purpose  Check that object's new position is correctly updated when reference point is changed to CENTER.
*\req    NA
*/
TEST(Update_Object_KF_States_After_Reference_Point_Change_CTCA, New_Ref_Point_Center)
{
   /** \precond
   * - An object with initial reference point LEFT is set up in TEST_GROUP
   * - Change object's reference point to CENTER
   * - Set expected data accordingly
   **/
   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   exp_x_pos = 5.0F;
   exp_y_pos = 7.0F;
   

   /** \action
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   Update_Object_KF_States_After_Reference_Point_Change(obj);
   
   // Calculate KF residuals after reference point change
   float32_t new_pos_residual_x = obj.predicted_vcs_position.x - obj.vcs_position.x;
   float32_t new_pos_residual_y = obj.predicted_vcs_position.y - obj.vcs_position.y;

   /** \result
   * Check that the object's position and predicted position are updated correctly.
   **/
   DOUBLES_EQUAL_TEXT(exp_x_pos, obj.vcs_position.x, test_thresh, "Object's xpos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_y_pos, obj.vcs_position.y, test_thresh, "Object's ypos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_x, new_pos_residual_x, test_thresh, "xpos residual has changed. It should be constant.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_y, new_pos_residual_y, test_thresh, "ypos residual has changed. It should be constant.");
}

/**
*\purpose  Check that object's new position is correctly updated when reference point is changed to LEFT from REAR LEFT.
*\req    NA
*/
TEST(Update_Object_KF_States_After_Reference_Point_Change_CTCA, New_Ref_Point_Left)
{
   /** \precond
   * - Set up an object with
   *     - Center of bbox at (5,7)
   *     - Orientation (pointing) -45 degrees
   *     - Length 6m
   *     - Width 2m
   * - Reference point assume to be REAR LEFT.
   *     - With position (2.1716, 8.4142)
   *     - Velocity (2.8284, -2.8284) m/s
   *     - Heading -45 degrees
   *     - Speed 4 m/s
   *     - Curvature -0.03 (1/m)
   *     - Predicted position (4,6)
   *     - Predicted velocity (3,-3)
   *     - Predicted heading -43 degrees
   *     - Predicted speed is 4.2 m/s    
   * - Change object's reference point to LEFT
   * - Set expected data accordingly
   **/
   obj.vcs_position.x = 2.1716F;
   obj.vcs_position.y = 8.4142F;
   obj.vcs_velocity.longitudinal = 2.8284F;
   obj.vcs_velocity.lateral = -2.8284F;
   obj.predicted_vcs_position.x = 2.3F;
   obj.predicted_vcs_position.y = 8.5F;

   obj.reference_point = F360_REFERENCE_POINT_LEFT;
   exp_x_pos = 4.2929F;
   exp_y_pos = 6.2929F;

   // Expected residuals
   exp_pos_residual_x = obj.predicted_vcs_position.x - obj.vcs_position.x;
   exp_pos_residual_y = obj.predicted_vcs_position.y - obj.vcs_position.y;
   
   /** \action
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   Update_Object_KF_States_After_Reference_Point_Change(obj);
   
   // Calculate KF residuals after reference point change
   float32_t new_pos_residual_x = obj.predicted_vcs_position.x - obj.vcs_position.x;
   float32_t new_pos_residual_y = obj.predicted_vcs_position.y - obj.vcs_position.y;
   
   /** \result
   * Check that the object's position and predicted position are updated correctly.
   **/
   DOUBLES_EQUAL_TEXT(exp_x_pos, obj.vcs_position.x, test_thresh, "Object's xpos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_y_pos, obj.vcs_position.y, test_thresh, "Object's ypos is incorrect.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_x, new_pos_residual_x, test_thresh, "xpos residual has changed. It should be constant.");
   DOUBLES_EQUAL_TEXT(exp_pos_residual_y, new_pos_residual_y, test_thresh, "ypos residual has changed. It should be constant.");
}

/** @}*/

/** \defgroup  Update_Object_KF_States_After_Reference_Point_Change_Reversing_Obj
 *  @{
 */

 /** \brief
  * Test group of Update_Object_KF_States_After_Reference_Point_Change function for reversing objects. 
  */
TEST_GROUP(Update_Object_KF_States_After_Reference_Point_Change_Reversing_Obj)
{
   /* Common setup for test.
    Create an arbitrary object. */
   F360_Object_Track_T obj = {};
   F360_Calibrations_T calibs;
   bool f_update_obj_pos_only;
   float32_t exp_speed;
   float32_t exp_heading;

   /** \setup
    * Set up a movable CCA object with
    *    - Orientation and heading 0
    *    - Speed -5 m/s, indicating that it's reversing
    *    - Set velocity accordingly
    *    - And curvature = 0
    **/
   TEST_SETUP()
   {  
      Initialize_Tracker_Calibrations(calibs);
      
      f_update_obj_pos_only = false;
      obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
      obj.f_moving = true;
      obj.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;
      obj.vcs_position = {8.0F,9.0F};
      obj.bbox.Set_Length(4.0F);
      obj.bbox.Set_Width(2.0F);
      obj.bbox.Set_Center(10.0F, 10.0F);
      obj.speed = -5.0F;
      obj.vcs_velocity.lateral = 0.0F;
      obj.vcs_velocity.longitudinal = obj.speed;
   }
};

/**
*\purpose  Verify that object properties for a reversing object are correctly set after reference point change.
*\req    NA
*/
TEST(Update_Object_KF_States_After_Reference_Point_Change_Reversing_Obj, New_Ref_Point_Front_Right)
{
   /** \precond
    * A reversing object has been set up.
    * Set expected heading and speed to same values as before function call.
    **/
   exp_speed = obj.speed;
   exp_heading = obj.vcs_heading.Value();

   /** \action
   * Call function Update_Object_KF_States_After_Reference_Point_Change
   **/
   Update_Object_KF_States_After_Reference_Point_Change(obj);

   /** \result
   * Expect that object heading and speed are correctly set, i.e. unchanged.
   **/
   DOUBLES_EQUAL_TEXT(exp_speed, obj.speed, F360_EPSILON, "Object speed is incorrect");
   DOUBLES_EQUAL_TEXT(exp_heading, obj.vcs_heading.Value(), F360_EPSILON, "Object heading is incorrect");
}

/** @}*/

/** \defgroup  Derive_Object_Reference_Point
 *  @{
 */

/** \brief
*  Testing function that determines the object's reference point
**/
TEST_GROUP(Derive_Object_Reference_Point)
{
   /* Common setup for test.
    Create an arbitrary object. In these specific tests the object position is moved around the host. To verify that the
    correct closest reference point is returned. */
   F360_Object_Track_T obj;
   F360_Reference_Point_T new_ref_point;
   float32_t host_dist_rear_axle_to_vcs_m;
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals = {};
   Reference_Point_Data ref_pnt_ignoring_visibility;
   Reference_Point_Data_Ext ref_pnt_considering_visibility;
   F360_Calibrations_T calibs;
   bool f_current_object_behind_another_object;

   /** \setup
    * Initialize common object to move around host to check returned ref points
    **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      f_current_object_behind_another_object = false;
      obj.vcs_position.x = -10.0F;
      obj.vcs_position.y = 10.0F;
      obj.Set_Bbox_Orientation(Angle{ 0.0F });
      obj.Update_Bbox_Size(6.0F, 2.0F);
      obj.reference_point = F360_REFERENCE_POINT_CENTER;
      obj.min_projection_reference_point = F360_REFERENCE_POINT_CENTER;
      obj.movable_prob = 1.0F;
      new_ref_point = F360_REFERENCE_POINT_CENTER;
      host_dist_rear_axle_to_vcs_m = 3.0F;

      globals.f_single_front_center_radar_only = false;

      sensors[0].variable.is_valid = true;

      globals.rotated_left_fov_normal[0][0] = 0.707106781186548F; // unit vector pointing 45 deg vcs
      globals.rotated_left_fov_normal[0][1] = 0.707106781186548F;
      
      globals.rotated_right_fov_normal[0][0] = 0.707106781186548F; // unit vector pointing -45 deg vcs
      globals.rotated_right_fov_normal[0][1] = -0.707106781186548F;
   }
};

/**
*\purpose  Verify that the reference point algorithm returns the RIGHT ref point, when prevous point center
*\req    NA
*/
TEST(Derive_Object_Reference_Point, Prev_Point_Center_RIGHT_Reference_Point_Seen)
{
   /** \precond
    * Place object in VCS, has pointing -3_pi_4 and is placed behind, to the right side
    * Visibility of reference points is not considered in this test, thus, min_projection ref pnt and ref pnt are the same
    **/
   obj.vcs_position.x = -10.0F - 0.6F * host_dist_rear_axle_to_vcs_m;
   obj.vcs_position.y = 10.0F;
   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_CENTER;
   obj.Set_Bbox_Orientation(Angle{ -F360_PI * 3.0F / 4.0F });

   /** \action
   * Call function Derive_Object_Reference_Point
   **/
   Derive_Object_Reference_Point(f_current_object_behind_another_object,
                                 calibs.k_far_away_object_dist_sq_thr,
                                 host_dist_rear_axle_to_vcs_m,
                                 sensors,
                                 globals,
                                 obj,
                                 ref_pnt_ignoring_visibility,
                                 ref_pnt_considering_visibility);

   /** \result
   * Expect the RIGHT reference point for both the considering visibility and ignoring visibility
   * Expect the previous rerference point data to be same as the input obj reference point and obj min projection ref pnt
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_RIGHT, ref_pnt_ignoring_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_CENTER, ref_pnt_ignoring_visibility.previous_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_RIGHT, ref_pnt_considering_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_CENTER, ref_pnt_considering_visibility.previous_point.reference_point);
}

/**
*\purpose  Verify that the reference point algorithm returns the RIGHT ref point, when the obj has pointing -3_pi_4 and is placed behind, to the right side of the host
*\req    NA
*/
TEST(Derive_Object_Reference_Point, RIGHT_Reference_Point_Seen)
{
   /** \precond
    * Place object in VCS, has pointing -3_pi_4 and is placed behind, to the right side
    * The previous reference points are set to be as the second best options
    * Visibility of reference points is not considered in this test, thus, min_projection ref pnt and ref pnt are the same
    **/
   obj.vcs_position.x = -10.0F - 0.6F * host_dist_rear_axle_to_vcs_m;
   obj.vcs_position.y = 10.0F;
   obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
   obj.Set_Bbox_Orientation(Angle{ -F360_PI * 3.0F / 4.0F });

   /** \action
   * Call function Derive_Object_Reference_Point
   **/
   Derive_Object_Reference_Point(f_current_object_behind_another_object,
                                 calibs.k_far_away_object_dist_sq_thr,
                                 host_dist_rear_axle_to_vcs_m,
                                 sensors,
                                 globals,
                                 obj,
                                 ref_pnt_ignoring_visibility,
                                 ref_pnt_considering_visibility);

   /** \result
   * Expect the RIGHT reference point for both the considering visibility and ignoring visibility
   * Expect the previous rerference point data to be same as the input obj reference point and obj min projection ref pnt
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_RIGHT, ref_pnt_ignoring_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_RIGHT, ref_pnt_ignoring_visibility.previous_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_RIGHT, ref_pnt_considering_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_RIGHT, ref_pnt_considering_visibility.previous_point.reference_point);
}

/**
*\purpose  Verify that the reference point algorithm returns the LEFT ref point, when the obj is placed to the right side and parallel to the host
*\req    NA
*/
TEST(Derive_Object_Reference_Point, LEFT_Reference_Point_Seen)
{
   /** \precond
    * Place object in VCS, on the right side of the host and point forward
    * The previous reference points are set to be as the second best options
    * Visibility of reference points is not considered in this test, thus, min_projection ref pnt and ref pnt are the same
    **/
   obj.vcs_position.x = 0.0F - 0.6F * host_dist_rear_axle_to_vcs_m;
   obj.vcs_position.y = 10.0F;
   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   obj.Set_Bbox_Orientation(Angle{ 0.0F });

   /** \action
   * Call function Derive_Object_Reference_Point
   **/
   Derive_Object_Reference_Point(f_current_object_behind_another_object,
                                 calibs.k_far_away_object_dist_sq_thr,
                                 host_dist_rear_axle_to_vcs_m,
                                 sensors,
                                 globals,
                                 obj,
                                 ref_pnt_ignoring_visibility,
                                 ref_pnt_considering_visibility);

   /** \result
   * Expect the LEFT reference point for both the considering visibility and ignoring visibility
   * Expect the previous rerference point data to be same as the input obj reference point and obj min projection ref pnt
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_LEFT, ref_pnt_ignoring_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_LEFT, ref_pnt_ignoring_visibility.previous_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_LEFT, ref_pnt_considering_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_LEFT, ref_pnt_considering_visibility.previous_point.reference_point);
}

/**
*\purpose  Verify that the reference point algorithm returns the FRONT ref point, when the obj is placed behind the host
*\req    NA
*/
TEST(Derive_Object_Reference_Point, FRONT_Reference_Point_Seen)
{
   /** \precond
    * Place object in VCS, behind the host and point forward
    * The previous reference points are set to be as the second best options
    * Visibility of reference points is not considered in this test, thus, min_projection ref pnt and ref pnt are the same
    **/
   obj.vcs_position.x = -10.0F - 0.6F * host_dist_rear_axle_to_vcs_m;
   obj.vcs_position.y = 0.0F;
   obj.reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
   obj.Set_Bbox_Orientation(Angle{ 0.0F });

   /** \action
   * Call function Derive_Object_Reference_Point
   **/
   Derive_Object_Reference_Point(f_current_object_behind_another_object,
                                 calibs.k_far_away_object_dist_sq_thr,
                                 host_dist_rear_axle_to_vcs_m,
                                 sensors,
                                 globals,
                                 obj,
                                 ref_pnt_ignoring_visibility,
                                 ref_pnt_considering_visibility);

   /** \result
   * Expect the FRONT reference point for both the considering visibility and ignoring visibility
   * Expect the previous rerference point data to be same as the input obj reference point and obj min projection ref pnt
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_FRONT, ref_pnt_ignoring_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_FRONT_LEFT, ref_pnt_ignoring_visibility.previous_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_FRONT, ref_pnt_considering_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_FRONT_LEFT, ref_pnt_considering_visibility.previous_point.reference_point);
}

/**
*\purpose  Verify that the reference point algorithm returns the REAR ref point, when the obj is placed in front and diagonally right to the host
*\req    NA
*/
TEST(Derive_Object_Reference_Point, REAR_Reference_Point_Seen)
{
   /** \precond
    * Place object in VCS, in front of the host and point forward
    * Visibility of reference points is not considered in this test, thus, min_projection ref pnt and ref pnt are the same
    **/
   obj.vcs_position.x = 8.0F - 0.6F * host_dist_rear_axle_to_vcs_m;
   obj.vcs_position.y = 10.0F;
   obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
   obj.Set_Bbox_Orientation(Angle{ F360_PI / 4.0F });

   /** \action
   * Call function Derive_Object_Reference_Point
   **/
   Derive_Object_Reference_Point(f_current_object_behind_another_object,
                                 calibs.k_far_away_object_dist_sq_thr,
                                 host_dist_rear_axle_to_vcs_m,
                                 sensors,
                                 globals,
                                 obj,
                                 ref_pnt_ignoring_visibility,
                                 ref_pnt_considering_visibility);

   /** \result
   * Expect the REAR reference point for both the considering visibility and ignoring visibility
   * Expect the previous rerference point data to be same as the input obj reference point and obj min projection ref pnt
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR, ref_pnt_ignoring_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_RIGHT, ref_pnt_ignoring_visibility.previous_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR, ref_pnt_considering_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_RIGHT, ref_pnt_considering_visibility.previous_point.reference_point);
}

/**
*\purpose  Verify that the reference point algorithm returns a corner reference point i.e the REAR_LEFT,instead of side refernece point i.e REAR
* when f_current_object_behind_another_object is true, implying that the object is occluded by another object
*\req    NA
*/
TEST(Derive_Object_Reference_Point, REAR_LEFT_Reference_Point_Selected_Due_To_f_current_object_behind_another_object)
{
   /** \precond
    * Place object in VCS, in front of the host and point forward
    * Visibility of reference points is not considered in this test, thus, min_projection ref pnt and ref pnt are the same
    * Set f_current_object_behind_another_object = true
    **/
   obj.vcs_position.x = 8.0F - 0.6F * host_dist_rear_axle_to_vcs_m;
   obj.vcs_position.y = 10.0F;
   obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
   obj.Set_Bbox_Orientation(Angle{ 0.0F });

   f_current_object_behind_another_object = true;

   /** \action
   * Call function Derive_Object_Reference_Point
   **/
   Derive_Object_Reference_Point(f_current_object_behind_another_object,
                                 calibs.k_far_away_object_dist_sq_thr,
                                 host_dist_rear_axle_to_vcs_m,
                                 sensors,
                                 globals,
                                 obj,
                                 ref_pnt_ignoring_visibility,
                                 ref_pnt_considering_visibility);

   /** \result
   * Expect the REAR_LEFT reference point for both the considering visibility and ignoring visibility
   * Expect the previous rerference point data to be same as the input obj reference point and obj min projection ref pnt
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_LEFT, ref_pnt_ignoring_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_RIGHT, ref_pnt_ignoring_visibility.previous_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_LEFT, ref_pnt_considering_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_RIGHT, ref_pnt_considering_visibility.previous_point.reference_point);
}

/**
*\purpose  Verify that the reference point algorithm returns the FRONT_LEFT ref point, when the obj is placed behind and diagonally right to the host 
*\req    NA
*/
TEST(Derive_Object_Reference_Point, FRONT_LEFT_Reference_Point_Seen)
{
   /** \precond
    * Place object in VCS, behind and diagonally right to the host
    * Visibility of reference points is not considered in this test, thus, min_projection ref pnt and ref pnt are the same
    **/
   obj.vcs_position.x = -10.0F - 0.6F * host_dist_rear_axle_to_vcs_m;
   obj.vcs_position.y = 10.0F;
   obj.reference_point = F360_REFERENCE_POINT_FRONT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_FRONT;
   obj.Set_Bbox_Orientation(Angle{ 0.0F });

   /** \action
   * Call function Derive_Object_Reference_Point
   **/
   Derive_Object_Reference_Point(f_current_object_behind_another_object,
                                 calibs.k_far_away_object_dist_sq_thr,
                                 host_dist_rear_axle_to_vcs_m,
                                 sensors,
                                 globals,
                                 obj,
                                 ref_pnt_ignoring_visibility,
                                 ref_pnt_considering_visibility);

   /** \result
   * Expect the FRONT LEFT reference point for both the considering visibility and ignoring visibility
   * Expect the previous rerference point data to be same as the input obj reference point and obj min projection ref pnt
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_FRONT_LEFT, ref_pnt_ignoring_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_FRONT, ref_pnt_ignoring_visibility.previous_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_FRONT_LEFT, ref_pnt_considering_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_FRONT, ref_pnt_considering_visibility.previous_point.reference_point);
}

/**
*\purpose  Verify that the reference point algorithm returns the FRONT_RIGHT ref point, when the obj is placed diagonally front and pointing 180 degree
*\req    NA
*/
TEST(Derive_Object_Reference_Point, FRONT_RIGHT_Reference_Point_Seen)
{
   /** \precond
    * Place object in VCS, diagonally front of host and pointing 180 degrees
    **/
   obj.vcs_position.x = 10.0F - 0.6F * host_dist_rear_axle_to_vcs_m;
   obj.vcs_position.y = 10.0F;
   obj.reference_point = F360_REFERENCE_POINT_FRONT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_FRONT;
   obj.Set_Bbox_Orientation(Angle{ F360_PI });

   /** \action
   * Call function Derive_Object_Reference_Point
   **/
   Derive_Object_Reference_Point(f_current_object_behind_another_object,
                                 calibs.k_far_away_object_dist_sq_thr,
                                 host_dist_rear_axle_to_vcs_m,
                                 sensors,
                                 globals,
                                 obj,
                                 ref_pnt_ignoring_visibility,
                                 ref_pnt_considering_visibility);

   /** \result
   * Expect the FRONT RIGHT reference point for both the considering visibility and ignoring visibility
   * Expect the previous rerference point data to be same as the input obj reference point and obj min projection ref pnt
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_FRONT_RIGHT, ref_pnt_ignoring_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_FRONT, ref_pnt_ignoring_visibility.previous_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_FRONT_RIGHT, ref_pnt_considering_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_FRONT, ref_pnt_considering_visibility.previous_point.reference_point);
}

/**
*\purpose  Verify that the reference point algorithm returns the REAR_LEFT ref point, when the obj is placed in front and diagonally right to the host 
*\req    NA
*/
TEST(Derive_Object_Reference_Point, REAR_LEFT_Reference_Point_Seen)
{
   /** \precond
    * Place object in VCS, in front and diagonally right to the host
    * Visibility of reference points is not considered in this test, thus, min_projection ref pnt and ref pnt are the same
    **/
   obj.vcs_position.x = 10.0F - 0.6F * host_dist_rear_axle_to_vcs_m;
   obj.vcs_position.y = 10.0F;
   obj.reference_point = F360_REFERENCE_POINT_REAR;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR;
   obj.Set_Bbox_Orientation(Angle{ 0.0F });

   /** \action
   * Call function Derive_Object_Reference_Point
   **/
   Derive_Object_Reference_Point(f_current_object_behind_another_object,
                                 calibs.k_far_away_object_dist_sq_thr,
                                 host_dist_rear_axle_to_vcs_m,
                                 sensors,
                                 globals,
                                 obj,
                                 ref_pnt_ignoring_visibility,
                                 ref_pnt_considering_visibility);

   /** \result
   * Expect the REAR LEFT reference point for both the considering visibility and ignoring visibility
   * Expect the previous rerference point data to be same as the input obj reference point and obj min projection ref pnt
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_LEFT, ref_pnt_ignoring_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR, ref_pnt_ignoring_visibility.previous_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_LEFT, ref_pnt_considering_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR, ref_pnt_considering_visibility.previous_point.reference_point);
}

/**
*\purpose  Verify that the reference point algorithm returns the REAR_RIGHT ref point, when the obj is placed in diagonally behind, right to the host and pointing 180 degrees
*\req    NA
*/
TEST(Derive_Object_Reference_Point, REAR_RIGHT_Reference_Point_Seen)
{
   /** \precond
    * Place object in VCS, diagonally behind, right to the host and facing the opposite way
    * Visibility of reference points is not considered in this test, thus, min_projection ref pnt and ref pnt are the same
    **/
   obj.vcs_position.x = -10.0F - 0.6F * host_dist_rear_axle_to_vcs_m;
   obj.vcs_position.y = 10.0F;
   obj.reference_point = F360_REFERENCE_POINT_REAR;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_REAR;
   obj.Set_Bbox_Orientation(Angle{ F360_PI });

   /** \action
   * Call function Derive_Object_Reference_Point
   **/
   Derive_Object_Reference_Point(f_current_object_behind_another_object,
                                 calibs.k_far_away_object_dist_sq_thr,
                                 host_dist_rear_axle_to_vcs_m,
                                 sensors,
                                 globals,
                                 obj,
                                 ref_pnt_ignoring_visibility,
                                 ref_pnt_considering_visibility);

   /** \result
   * Expect the REAR RIGHT reference point for both the considering visibility and ignoring visibility
   * Expect the previous rerference point data to be same as the input obj reference point and obj min projection ref pnt
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_RIGHT, ref_pnt_ignoring_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR, ref_pnt_ignoring_visibility.previous_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_RIGHT, ref_pnt_considering_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR, ref_pnt_considering_visibility.previous_point.reference_point);
}

/**
*\purpose
* Objects on long distances should only have sides (left/right/front/rear) as reference points, not any corner.
* This test validates that the function returns rear left as reference point when the object is on short distance.
*\req 
* NA
*/
TEST(Derive_Object_Reference_Point, REAR_LEFT_Reference_Point_Seen_At_Close_Distance)
{
   /** \precond
    * Place object in VCS, in front and diagonally right to the host
    * Visibility of reference points is not considered in this test, thus, min_projection ref pnt and ref pnt are the same
    * The object is placed at radial distance shorter than 80m such that rear left is the best reference point
    **/
   obj.vcs_position.x = 60.0F;
   obj.vcs_position.y = 52.0F;
   obj.reference_point = F360_REFERENCE_POINT_LEFT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_LEFT;
   obj.Set_Bbox_Orientation(Angle{ 0.0F });

   /** \action
   * Call function Derive_Object_Reference_Point
   **/
   Derive_Object_Reference_Point(f_current_object_behind_another_object,
                                 calibs.k_far_away_object_dist_sq_thr,
                                 host_dist_rear_axle_to_vcs_m,
                                 sensors,
                                 globals,
                                 obj,
                                 ref_pnt_ignoring_visibility,
                                 ref_pnt_considering_visibility);

   /** \result
   * Expect the REAR LEFT reference point for both the considering visibility and ignoring visibility
   * Expect the previous rerference point data to be same as the input obj reference point and obj min projection ref pnt
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_LEFT, ref_pnt_ignoring_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_LEFT, ref_pnt_ignoring_visibility.previous_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_LEFT, ref_pnt_considering_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_LEFT, ref_pnt_considering_visibility.previous_point.reference_point);
}

/**
*\purpose
* Objects on long distances should only have sides (left/right/front/rear) as reference points, not any corner.
* This test validates that, even if the best suited reference point is rear left, the function returns rear as
* the object is on long distance.
*\req 
* NA
*/
TEST(Derive_Object_Reference_Point, REAR_LEFT_Reference_Point_Seen_At_Long_Distance)
{
   /** \precond
    * Place object in VCS, in front and diagonally right to the host
    * Visibility of reference points is not considered in this test, thus, min_projection ref pnt and ref pnt are the same
    * The object is placed at distance greater than 80m
    * If the long distance condition is not applied, the alogirthm should return the rear_left ref_pnt (not intended)
    * The intended ref pnt is rear
    **/
   obj.vcs_position.x = 60.0F;
   obj.vcs_position.y = 53.0F;
   obj.reference_point = F360_REFERENCE_POINT_LEFT;
   obj.min_projection_reference_point = F360_REFERENCE_POINT_LEFT;
   obj.Set_Bbox_Orientation(Angle{ 0.0F });

   /** \action
   * Call function Derive_Object_Reference_Point
   **/
   Derive_Object_Reference_Point(f_current_object_behind_another_object,
                                 calibs.k_far_away_object_dist_sq_thr,
                                 host_dist_rear_axle_to_vcs_m,
                                 sensors,
                                 globals,
                                 obj,
                                 ref_pnt_ignoring_visibility,
                                 ref_pnt_considering_visibility);

   /** \result
   * Expect the REAR reference point for both the considering visibility and ignoring visibility
   * Expect the previous rerference point data to be same as the input obj reference point and obj min projection ref pnt
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR, ref_pnt_ignoring_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_LEFT, ref_pnt_ignoring_visibility.previous_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR, ref_pnt_considering_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_LEFT, ref_pnt_considering_visibility.previous_point.reference_point);
}

/**
*\purpose
* This test checks if for objects fullfilling specific conditions, like heading and position,
* we can select the second favourite reference point if the first one is either left or right.
* In this test we try the left side. The conditions are set up for a crossing object
*\req
* NA
*/
TEST(Derive_Object_Reference_Point, Select_Second_Favourite_Reference_Point)
{
    /** \precond
     * Place object in VCS, in front and diagonally right to the host
     * Visibility of reference points is not considered in this test, thus, min_projection ref pnt and ref pnt are the same
     * The object is placed at distance abs(x) > 5 and abs(y) > 4 and with a heading a -1.296 rad. More specifically x = 19.44, y = 10.202 and heading = 73.911
     * Also the width and the length of the object has been set to 1.322 and 3.497, respectively.
     * The previous reference point is fron left
     * The intended ref pnt is front left as well
     * The distance from the rear axle to center of host is set to 3.963.
     **/
    obj.vcs_position.x = 19.44F;
    obj.vcs_position.y = 10.202F;
    obj.reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
    obj.min_projection_reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
    obj.vcs_heading = Angle{ 73.911F }.Normalize();
    obj.bbox.Set_Width(1.322F);
    obj.bbox.Set_Length(3.497F);
    host_dist_rear_axle_to_vcs_m = 3.963F;

    /** \action
    * Call function Derive_Object_Reference_Point
    **/
    Derive_Object_Reference_Point(f_current_object_behind_another_object,
        calibs.k_far_away_object_dist_sq_thr,
        host_dist_rear_axle_to_vcs_m,
        sensors,
        globals,
        obj,
        ref_pnt_ignoring_visibility,
        ref_pnt_considering_visibility);

    /** \result
    * Expect the Front Left reference point for both the considering visibility and ignoring visibility
    * Expect the previous rerference point data to be same as the input obj reference point and obj min projection ref pnt
    **/
    CHECK_EQUAL(F360_REFERENCE_POINT_FRONT_LEFT, ref_pnt_ignoring_visibility.new_point.reference_point);
    CHECK_EQUAL(F360_REFERENCE_POINT_FRONT_LEFT, ref_pnt_ignoring_visibility.previous_point.reference_point);
    CHECK_EQUAL(F360_REFERENCE_POINT_FRONT_LEFT, ref_pnt_considering_visibility.new_point.reference_point);
    CHECK_EQUAL(F360_REFERENCE_POINT_FRONT_LEFT, ref_pnt_considering_visibility.previous_point.reference_point);
}
/** @}*/

/** \defgroup  Convert_Ref_Point_Enum_To_Ref_Point_Vector_Index
 *  @{
 */

/** \brief
*  Testing function that returns the index corresponding to the input reference point ENUM
*  It tests the following functions:
*  Convert_Ref_Point_Enum_To_Ref_Point_Vector_Index()
*  Convert_Ref_Point_Vector_Index_To_Ref_Point_Enum()
**/
TEST_GROUP(Convert_Ref_Point_Enum_To_Ref_Point_Vector_And_Reverse)
{
   /* Common setup for test */

   /** \setup
    * No Setup needed
    **/
   TEST_SETUP()
   {
      // No Setup needed
   }
};

/**
  *\purpose This tests Convert_Ref_Point_Enum_To_Ref_Point_Vector_Index()
  * The test checks if the correct index is retruned, given the reference point enum as input to the function
  *\req NA
  **/
TEST(Convert_Ref_Point_Enum_To_Ref_Point_Vector_And_Reverse, Check_if_correct_idx_is_returned_for_given_ref_point_enum)
{
   /** \precond
    * No precondition needed
    **/

   /** \action
   * Call function Convert_Ref_Point_Enum_To_Ref_Point_Vector_Index
   **/

   /** \result
   * Expect the correct index for the corresponding ref pnt enum
   **/
   CHECK_EQUAL(0U, Convert_Ref_Point_Enum_To_Ref_Point_Vector_Index(F360_REFERENCE_POINT_FRONT));
   CHECK_EQUAL(1U, Convert_Ref_Point_Enum_To_Ref_Point_Vector_Index(F360_REFERENCE_POINT_RIGHT));
   CHECK_EQUAL(2U, Convert_Ref_Point_Enum_To_Ref_Point_Vector_Index(F360_REFERENCE_POINT_REAR));
   CHECK_EQUAL(3U, Convert_Ref_Point_Enum_To_Ref_Point_Vector_Index(F360_REFERENCE_POINT_LEFT));
   CHECK_EQUAL(4U, Convert_Ref_Point_Enum_To_Ref_Point_Vector_Index(F360_REFERENCE_POINT_FRONT_LEFT));
   CHECK_EQUAL(5U, Convert_Ref_Point_Enum_To_Ref_Point_Vector_Index(F360_REFERENCE_POINT_FRONT_RIGHT));
   CHECK_EQUAL(6U, Convert_Ref_Point_Enum_To_Ref_Point_Vector_Index(F360_REFERENCE_POINT_REAR_RIGHT));
   CHECK_EQUAL(7U, Convert_Ref_Point_Enum_To_Ref_Point_Vector_Index(F360_REFERENCE_POINT_REAR_LEFT));
   // Default return value
   CHECK_EQUAL(2U, Convert_Ref_Point_Enum_To_Ref_Point_Vector_Index(F360_REFERENCE_POINT_CENTER));
}

/**
  *\purpose This tests Convert_Ref_Point_Vector_Index_To_Ref_Point_Enum()
  * The test checks if the correct enum is retruned, given the index as input to the function
  *\req NA
  **/
TEST(Convert_Ref_Point_Enum_To_Ref_Point_Vector_And_Reverse, Check_if_correct_ref_pnt_enum_is_returned_for_given_idx)
{
   /** \precond
    * No precondition needed
    **/

   /** \action
   * Call function Convert_Ref_Point_Vector_Index_To_Ref_Point_Enum
   **/

   /** \result
   * Expect the correct index for the corresponding ref pnt enum
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_FRONT, Convert_Ref_Point_Vector_Index_To_Ref_Point_Enum(0U));
   CHECK_EQUAL(F360_REFERENCE_POINT_RIGHT, Convert_Ref_Point_Vector_Index_To_Ref_Point_Enum(1U));
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR, Convert_Ref_Point_Vector_Index_To_Ref_Point_Enum(2U));
   CHECK_EQUAL(F360_REFERENCE_POINT_LEFT, Convert_Ref_Point_Vector_Index_To_Ref_Point_Enum(3U));
   CHECK_EQUAL(F360_REFERENCE_POINT_FRONT_LEFT, Convert_Ref_Point_Vector_Index_To_Ref_Point_Enum(4U));
   CHECK_EQUAL(F360_REFERENCE_POINT_FRONT_RIGHT, Convert_Ref_Point_Vector_Index_To_Ref_Point_Enum(5U));
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_RIGHT, Convert_Ref_Point_Vector_Index_To_Ref_Point_Enum(6U));
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_LEFT, Convert_Ref_Point_Vector_Index_To_Ref_Point_Enum(7U));
   // Default return value
   CHECK_EQUAL(F360_REFERENCE_POINT_CENTER, Convert_Ref_Point_Vector_Index_To_Ref_Point_Enum(8U));
}
/** @}*/

/** \defgroup  Derive_Object_Reference_Point_Single_Sensor
 *  @{
 */

/** \brief
*  Testing function that determines the object's reference point when considering a single sensor's field of view
**/
TEST_GROUP(Derive_Object_Reference_Point_Single_Sensor)
{
   /* Common setup for test.
    Create an arbitrary object. In these specific tests the object position is moved around the host. To verify that the
    correct closest reference point is returned when considering a single sensor's field of view. */
   F360_Object_Track_T obj;
   float32_t host_dist_rear_axle_to_vcs_m;
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals = {};
   Reference_Point_Data ref_pnt_ignoring_visibility;
   Reference_Point_Data_Ext ref_pnt_considering_visibility;
   F360_Calibrations_T calibs;
   bool f_current_object_behind_another_object;

   /** \setup
    * Initialize common object to move around host to check returned ref points
    **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      f_current_object_behind_another_object = false;
      obj.vcs_position.x = 0.0F;
      obj.vcs_position.y = 0.0F;
      obj.Set_Bbox_Orientation(Angle{ 0.0F });
      obj.Update_Bbox_Size(2.0F, 2.0F);
      obj.reference_point = F360_REFERENCE_POINT_CENTER;
      obj.min_projection_reference_point = F360_REFERENCE_POINT_CENTER;
      obj.movable_prob = 1.0F;
      
      host_dist_rear_axle_to_vcs_m = 0.0F;

      globals.f_single_front_center_radar_only = true;

      sensors[0].variable.is_valid = true;

      globals.rotated_left_fov_normal[0][0] = 0.707106781186548F; // unit vector pointing 45 deg vcs
      globals.rotated_left_fov_normal[0][1] = 0.707106781186548F;
      
      globals.rotated_right_fov_normal[0][0] = 0.707106781186548F; // unit vector pointing -45 deg vcs
      globals.rotated_right_fov_normal[0][1] = -0.707106781186548F;
   }
};

/**
  *\purpose Verify function returns rear left as both reference point and min projection reference point
  * when the reference point is inside of the fov
  *\req NA
  **/
TEST(Derive_Object_Reference_Point_Single_Sensor, Rear_Left_Reference_Point_Seen_And_Inside_FOV_No_Hysteresis)
{
   /** \precond
    * Place object such that the rear left corner is the best reference point and the rear left corner is inside of the FOV
    **/
   obj.vcs_position.x = 5.1F;
   obj.vcs_position.y = 5.0F;
   obj.bbox.Set_Center(Point{5.1F,5.0F});

   /** \action
   * Call function Derive_Object_Reference_Point
   **/
   Derive_Object_Reference_Point(f_current_object_behind_another_object,
                                 calibs.k_far_away_object_dist_sq_thr,
                                 host_dist_rear_axle_to_vcs_m,
                                 sensors,
                                 globals,
                                 obj,
                                 ref_pnt_ignoring_visibility,
                                 ref_pnt_considering_visibility);

   /** \result
   * Verify the reference point considering visibility is F360_REFERENCE_POINT_REAR_LEFT
   * Verify the reference point ignoring visibility is F360_REFERENCE_POINT_REAR_LEFT
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_LEFT, ref_pnt_considering_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_LEFT, ref_pnt_ignoring_visibility.new_point.reference_point);
}

/**
  *\purpose Verify function returns the expected reference point and min projection reference point when the best 
  * reference point is outside the FoV but the second best is inside
  *\req NA
  **/
TEST(Derive_Object_Reference_Point_Single_Sensor, Rear_Left_Reference_Point_Best_But_Outside_FOV_Pick_2nd_Best_No_Hysteresis)
{
   /** \precond
    * Place object such that the rear left corner is the best reference point but the rear left corner is outside of the FOV 
    * while the left reference point is inside the fov
    **/
   obj.vcs_position.x = 4.9F;
   obj.vcs_position.y = 5.0F;
   obj.bbox.Set_Center(Point{4.9F,5.0F});

   /** \action
   * Call function Derive_Object_Reference_Point
   **/
   Derive_Object_Reference_Point(f_current_object_behind_another_object,
                                 calibs.k_far_away_object_dist_sq_thr,
                                 host_dist_rear_axle_to_vcs_m,
                                 sensors,
                                 globals,
                                 obj,
                                 ref_pnt_ignoring_visibility,
                                 ref_pnt_considering_visibility);

   /** \result
   * Verify the reference point considering visibility is F360_REFERENCE_POINT_LEFT
   * Verify the reference point ignoring visibility is F360_REFERENCE_POINT_REAR_LEFT
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_LEFT, ref_pnt_considering_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_LEFT, ref_pnt_ignoring_visibility.new_point.reference_point);
}

/**
  *\purpose Verify function returns the expected reference point and min projection reference point when the best
  * and the second best reference point is outside the FoV but the third best is inside
  *\req NA
  **/
TEST(Derive_Object_Reference_Point_Single_Sensor, Rear_Left_Reference_Point_Best_But_Outside_FOV_Pick_3rd_Best_No_Hysteresis)
{
   /** \precond
    * Place object such that the rear left corner is the best reference point but the rear left and left reference point
    * is outside of the FOV but front left is inside the fov
    **/
   obj.vcs_position.x = 3.9F;
   obj.vcs_position.y = 5.0F;
   obj.bbox.Set_Center(Point{3.9F,5.0F});

   /** \action
   * Call function Derive_Object_Reference_Point
   **/
   Derive_Object_Reference_Point(f_current_object_behind_another_object,
                                 calibs.k_far_away_object_dist_sq_thr,
                                 host_dist_rear_axle_to_vcs_m,
                                 sensors,
                                 globals,
                                 obj,
                                 ref_pnt_ignoring_visibility,
                                 ref_pnt_considering_visibility);

   /** \result
   * Verify the reference point considering visibility is F360_REFERENCE_POINT_FRONT_LEFT
   * Verify the reference point ignoring visibility is F360_REFERENCE_POINT_REAR_LEFT
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_FRONT_LEFT, ref_pnt_considering_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_LEFT, ref_pnt_ignoring_visibility.new_point.reference_point);
}

/** \defgroup  Apply_Reference_Point_Hysteresis
 *  @{
 */

/** \brief
*  Testing function that determines the new reference point based on hystresis
**/
TEST_GROUP(Apply_Reference_Point_Hysteresis)
{
   /* Common setup for test.
    Create an arbitrary object. In these specific tests the object position is moved around the host. To verify that the
    correct closest reference point is returned when considering a single sensor's field of view. */
   F360_Object_Track_T obj;
   F360_Calibrations_T calibs = {};
   Reference_Point_Data ref_pnt_ignoring_visibility;
   Reference_Point_Data_Ext ref_pnt_considering_visibility;

   /** \setup
    * Initialize common object to move around host to check returned ref points
    **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      obj.Update_Bbox_Size(6.0F, 2.0F);
      obj.movable_prob = 1.0F;
      ref_pnt_considering_visibility.previous_point.reference_point = F360_REFERENCE_POINT_CENTER;
      ref_pnt_ignoring_visibility.previous_point.reference_point = F360_REFERENCE_POINT_CENTER;
   }
};

/**
  *\purpose The test checks if no hystersis is applied, when the previous reference point is center
  *\req NA
  **/
TEST(Apply_Reference_Point_Hysteresis, No_Hysteresis_due_to_prev_point_being_center)
{
   /** \precond
    * The previous referece point is set to be center for both cases of considering and ignoring visibility
    * Due to the previous point being the center, no hysteresis should be applied, thus, new ref point remains unchanged 
    **/
   ref_pnt_considering_visibility.f_all_new_ref_pnt_candidates_visible = false;
   ref_pnt_considering_visibility.previous_point.reference_point = F360_REFERENCE_POINT_CENTER;
   ref_pnt_ignoring_visibility.previous_point.reference_point = F360_REFERENCE_POINT_CENTER;

   ref_pnt_considering_visibility.new_point.reference_point = F360_REFERENCE_POINT_REAR;
   ref_pnt_ignoring_visibility.new_point.reference_point = F360_REFERENCE_POINT_REAR;

   /** \action
   * Call function Apply_Reference_Point_Hysteresis
   **/
   Apply_Reference_Point_Hysteresis(calibs, obj, ref_pnt_ignoring_visibility, ref_pnt_considering_visibility);

   /** \result
   * Verify the reference point considering visibility is F360_REFERENCE_POINT_REAR (i.e. unchanged from what the input was)
   * Verify the reference point ignoring visibility is F360_REFERENCE_POINT_REAR (i.e. unchanged from what the input was)
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR, ref_pnt_considering_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR, ref_pnt_ignoring_visibility.new_point.reference_point);
}

/**
  *\purpose Verify that the function returns the previous reference point (REAR) for both cases of considering visibility and ignoring visibility
  * since the switch to the best reference point (REAR_LEFT) is blocked by the hysteresis check with and without regard to visibility
  * It also verifies that when all points are visible, the ref pnt ignoring visibilty is set equal to ref pnt considering visiibility, regardless of any other condition
  *\req NA
  **/
TEST(Apply_Reference_Point_Hysteresis, Prev_Ref_Pnt_Inside_Fov_Best_Ref_Pnt_Blocked_By_Hysteresis)
{
   /** \precond
    * The rear left reference point is considered to be the best choice, but it is not chosen, because of hysteresis
    * The projection value of previous and new point is set such that, hyteresis check does pass
    * The previous reference point is inside FOV
    * All points are in the FOV
    **/
   ref_pnt_considering_visibility.f_all_new_ref_pnt_candidates_visible = true;

   ref_pnt_considering_visibility.previous_point.reference_point = F360_REFERENCE_POINT_REAR;
   ref_pnt_considering_visibility.previous_point.projection_value = -1.5F;
   ref_pnt_considering_visibility.previous_point.f_visible = true;

   ref_pnt_considering_visibility.new_point.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   ref_pnt_considering_visibility.new_point.projection_value = -1.0F;

   ref_pnt_ignoring_visibility.previous_point.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   ref_pnt_ignoring_visibility.previous_point.projection_value = -1.5F;
   ref_pnt_ignoring_visibility.previous_point.f_visible = true;

   ref_pnt_ignoring_visibility.new_point.reference_point =  F360_REFERENCE_POINT_REAR_LEFT;
   ref_pnt_ignoring_visibility.new_point.projection_value = -1.0F;

   /** \action
   * Call function Apply_Reference_Point_Hysteresis
   **/
   Apply_Reference_Point_Hysteresis(calibs, obj, ref_pnt_ignoring_visibility, ref_pnt_considering_visibility);

   /** \result
   * Verify the reference point ignoring and considering visibility is F360_REFERENCE_POINT_REAR (i.e. unchanged from what the input was, due to Hysteresis)
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR, ref_pnt_ignoring_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR, ref_pnt_considering_visibility.new_point.reference_point);
}

/**
  *\purpose Verify that the function returns the previous reference point (REAR) for both cases of considering visibility and ignoring visibility
  * since the switch to the best reference point (REAR_LEFT) is blocked by the hysteresis check with and without regard to visibility
  *\req NA
  **/
TEST(Apply_Reference_Point_Hysteresis, Prev_Ref_Pnt_Outside_Fov_Best_Ref_Pnt_Blocked_By_Hysteresis)
{
   /** \precond
    * The rear left reference point is considered to be the best choice
    * The projection value of previous and new point is set such that, hyteresis check does pass
    * The previous reference point is outside FOV
    * The previous ref pnt, considering_visibility = ignoring_visibility
    * Not all points are in the FOV
    **/
   ref_pnt_considering_visibility.f_all_new_ref_pnt_candidates_visible = false;
   ref_pnt_considering_visibility.previous_point.reference_point = F360_REFERENCE_POINT_REAR;
   ref_pnt_considering_visibility.previous_point.projection_value = -1.5F;
   ref_pnt_considering_visibility.previous_point.f_visible = false;

   ref_pnt_considering_visibility.new_point.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   ref_pnt_considering_visibility.new_point.projection_value = -1.0F;

   ref_pnt_ignoring_visibility = ref_pnt_considering_visibility;

   /** \action
   * Call function Apply_Reference_Point_Hysteresis
   **/
   Apply_Reference_Point_Hysteresis(calibs, obj, ref_pnt_ignoring_visibility, ref_pnt_considering_visibility);

   /** \result
   * Verify, considering visibility, ref_pnt is set to F360_REFERENCE_POINT_REAR_LEFT, i.e. bypassed the hysteresis check as the previous reference point is outside FoV
   * Verify, ignoring visibility, ref_pnt is set to F360_REFERENCE_POINT_REAR, i.e. was blocked by the hysteresis check
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR, ref_pnt_ignoring_visibility.new_point.reference_point);
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR_LEFT, ref_pnt_considering_visibility.new_point.reference_point);
}
/** @}*/


/** \defgroup  Increase_Object_Pos_Uncertainty_After_Reference_Point_Change
 *  @{
 */

/** \brief
 *  Purpose of test group is to test the function Increase_Object_Pos_Uncertainty_After_Reference_Point_Change().
 **/
TEST_GROUP(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change)
{
   // Common setup for test
   F360_Object_Track_T obj;
   F360_Reference_Point_T old_ref_pnt;
   F360_Calibrations_T calibs;

   const float32_t test_pass_th = 1e-6F;
   
   /** \setup
   * Setup an object with
   *    - previous reference point REAR_LEFT
   *    - current reference point REAR_RIGHT
   *    - bounding box dimensions 9 x 3m (length x width)
   *    - pointing of 0 degrees
   *    - unit matrix errcov
   *    - track filter type CTCA
   * Intialize calibration structure to default tracker calibrations
   **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      old_ref_pnt = F360_REFERENCE_POINT_REAR_LEFT;

      obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
      obj.bbox.Set_Length(9.0F);
      obj.bbox.Set_Width(3.0F);
      obj.bbox.Set_Orientation(0.0F);
      float32_t unit_mat[STATE_DIMENSION][STATE_DIMENSION] = {};
      for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
      {
         unit_mat[idx][idx] = 1.0F;
      }
      std::copy(cmn::begin(unit_mat), cmn::end(unit_mat), cmn::begin(obj.errcov));
      obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   }
};

/**
*\purpose  Check that object's position uncertainty is unchanged if previous object reference point was CENTER
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_No_Increse_When_Prev_Ref_Pnt_CENTER)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set previous object reference point to CENTER
   **/
   old_ref_pnt = F360_REFERENCE_POINT_CENTER;

   /** \action
   * Copy the object errcov before call to function such that we can later compare and make sure it is unchanged 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t errcov_before_call[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(errcov_before_call));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov is unchanged by the function call
   **/
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(errcov_before_call[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is unchanged if object track filter type is CCA
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_No_Increse_When_CCA)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set object track filter type to CCA
   **/
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;

   /** \action
   * Copy the object errcov before call to function such that we can later compare and make sure it is unchanged 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t errcov_before_call[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(errcov_before_call));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov is unchanged by the function call
   **/
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(errcov_before_call[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from REAR_LEFT to REAR_RIGHT
           and object pointing is 0 degrees.
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_REAR_LEFT_To_REAR_RIGHT_Zero_Pnt)
{
   /** \precond
   * Use default settings from test group
   **/

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[1][1] += 0.09F;
   expected_errcov[0][0] += 0.010961773830792F;
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from REAR_LEFT to REAR_RIGHT
           and object pointing is non-zero (for this test we choose -30 degrees).
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_REAR_LEFT_To_REAR_RIGHT_Nonzero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set object pointing to -30 degrees 
   **/
  obj.bbox.Set_Orientation(F360_DEG2RAD(-30.0F));

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[1][1] += 0.070240443457698F;
   expected_errcov[0][0] += 0.030721330373094F;
   expected_errcov[0][1] -= 0.034224555866297F;
   expected_errcov[1][0] = expected_errcov[0][1];

   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from FRONT_LEFT to FRONT_RIGHT
           and object pointing is 0 degrees.
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_FRONT_LEFT_To_FRONT_RIGHT_Zero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set previous object reference point to FRONT_LEFT
   *    - Set current object reference point to FRONT_RIGHT
   **/
  old_ref_pnt = F360_REFERENCE_POINT_FRONT_LEFT;
  obj.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[1][1] += 0.09F;
   expected_errcov[0][0] += 0.010961773830792F;
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from REAR_RIGHT to REAR_LEFT
           and object pointing is 0 degrees.
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_REAR_RIGHT_To_REAR_LEFT_Zero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set previous object reference point to REAR_RIGHT
   *    - Set current object reference point to REAR_LEFT
   **/
  old_ref_pnt = F360_REFERENCE_POINT_REAR_RIGHT;
  obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[1][1] += 0.09F;
   expected_errcov[0][0] += 0.010961773830792F;
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from FRONT_RIGHT to FRONT_LEFT
           and object pointing is 0 degrees.
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_FRONT_RIGHT_To_FRONT_LEFT_Zero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set previous object reference point to FRONT_RIGHT
   *    - Set current object reference point to FRONT_LEFT
   **/
  old_ref_pnt = F360_REFERENCE_POINT_FRONT_RIGHT;
  obj.reference_point = F360_REFERENCE_POINT_FRONT_LEFT;

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[1][1] += 0.09F;
   expected_errcov[0][0] += 0.010961773830792F;
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from REAR_LEFT to REAR
           and object pointing is 0 degrees.
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_REAR_LEFT_To_REAR_Zero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set current object reference point to REAR
   **/
  obj.reference_point = F360_REFERENCE_POINT_REAR;

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[1][1] += 0.022500000000000F;
   expected_errcov[0][0] += 0.002740443457698F;
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from FRONT_LEFT to FRONT
           and object pointing is 0 degrees.
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_FRONT_LEFT_To_FRONT_Zero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set previous object reference point to FRONT_LEFT
   *    - Set current object reference point to FRONT
   **/
  old_ref_pnt = F360_REFERENCE_POINT_FRONT_LEFT;
  obj.reference_point = F360_REFERENCE_POINT_FRONT;

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[1][1] += 0.022500000000000F;
   expected_errcov[0][0] += 0.002740443457698F;
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from REAR_RIGHT to REAR
           and object pointing is 0 degrees.
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_REAR_RIGHT_To_REAR_Zero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set previous object reference point to REAR_RIGHT
   *    - Set current object reference point to REAR
   **/
  old_ref_pnt = F360_REFERENCE_POINT_REAR_RIGHT;
  obj.reference_point = F360_REFERENCE_POINT_REAR;

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[1][1] += 0.022500000000000F;
   expected_errcov[0][0] += 0.002740443457698F;
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from FRONT_RIGHT to FRONT
           and object pointing is 0 degrees.
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_FRONT_RIGHT_To_FRONT_Zero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set previous object reference point to FRONT_RIGHT
   *    - Set current object reference point to FRONT
   **/
  old_ref_pnt = F360_REFERENCE_POINT_FRONT_RIGHT;
  obj.reference_point = F360_REFERENCE_POINT_FRONT;

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[1][1] += 0.022500000000000F;
   expected_errcov[0][0] += 0.002740443457698F;
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from REAR to REAR_LEFT
           and object pointing is 0 degrees.
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_REAR_To_REAR_LEFT_Zero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set previous object reference point to REAR
   *    - Set current object reference point to REAR_LEFT
   **/
  old_ref_pnt = F360_REFERENCE_POINT_REAR;
  obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[1][1] += 0.022500000000000F;
   expected_errcov[0][0] += 0.002740443457698F;
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from REAR to REAR_RIGHT
           and object pointing is 0 degrees.
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_REAR_To_REAR_RIGHT_Zero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set previous object reference point to REAR
   *    - Set current object reference point to REAR_RIGHT
   **/
  old_ref_pnt = F360_REFERENCE_POINT_REAR;
  obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[1][1] += 0.022500000000000F;
   expected_errcov[0][0] += 0.002740443457698F;
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from FRONT to FRONT_LEFT
           and object pointing is 0 degrees.
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_FRONT_To_FRONT_LEFT_Zero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set previous object reference point to FRONT
   *    - Set current object reference point to FRONT_LEFT
   **/
  old_ref_pnt = F360_REFERENCE_POINT_FRONT;
  obj.reference_point = F360_REFERENCE_POINT_FRONT_LEFT;

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[1][1] += 0.022500000000000F;
   expected_errcov[0][0] += 0.002740443457698F;
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from FRONT to FRONT_RIGHT
           and object pointing is 0 degrees.
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_FRONT_To_FRONT_RIGHT_Zero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set previous object reference point to FRONT
   *    - Set current object reference point to FRONT_RIGHT
   **/
  old_ref_pnt = F360_REFERENCE_POINT_FRONT;
  obj.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[1][1] += 0.022500000000000F;
   expected_errcov[0][0] += 0.002740443457698F;
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from REAR_LEFT to FRONT_LEFT
           and object pointing is 0 degrees.
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_REAR_LEFT_To_FRONT_LEFT_Zero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set current object reference point to FRONT_LEFT
   **/
  obj.reference_point = F360_REFERENCE_POINT_FRONT_LEFT;

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[0][0] += 0.81F;
   expected_errcov[1][1] += 0.098655964477128F;
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from REAR_LEFT to FRONT_LEFT
           and object pointing is non-zero (for this test we choose 30 degrees).
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_REAR_LEFT_To_FRONT_LEFT_Nonzero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set current object reference point to FRONT_LEFT
   *    - Set object pointing to 30 degrees 
   **/
  obj.reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
  obj.bbox.Set_Orientation(F360_DEG2RAD(30.0F));

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[0][0] += 0.632163991119282F;
   expected_errcov[1][1] += 0.276491973357846F;
   expected_errcov[0][1] += 0.308021002796674F;
   expected_errcov[1][0] = expected_errcov[0][1];

   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from REAR_RIGHT to FRONT_RIGHT
           and object pointing is 0 degrees.
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_REAR_RIGHT_To_FRONT_RIGHT_Zero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set previous object reference point to REAR_RIGHT
   *    - Set current object reference point to FRONT_RIGHT
   **/
  old_ref_pnt = F360_REFERENCE_POINT_REAR_RIGHT;
  obj.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[0][0] += 0.81F;
   expected_errcov[1][1] += 0.098655964477128F;
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from FRONT_LEFT to REAR_LEFT
           and object pointing is 0 degrees.
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_FRONT_LEFT_To_REAR_LEFT_Zero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set previous object reference point to FRONT_LEFT
   *    - Set current object reference point to REAR_LEFT
   **/
  old_ref_pnt = F360_REFERENCE_POINT_FRONT_LEFT;
  obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[0][0] += 0.81F;
   expected_errcov[1][1] += 0.098655964477128F;
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from FRONT_RIGHT to REAR_RIGHT
           and object pointing is 0 degrees.
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_FRONT_RIGHT_To_REAR_RIGHT_Zero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set previous object reference point to FRONT_RIGHT
   *    - Set current object reference point to REAR_RIGHT
   **/
  old_ref_pnt = F360_REFERENCE_POINT_FRONT_RIGHT;
  obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[0][0] += 0.81F;
   expected_errcov[1][1] += 0.098655964477128F;
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from REAR_LEFT to LEFT
           and object pointing is 0 degrees.
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_REAR_LEFT_To_LEFT_Zero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set current object reference point to LEFT
   **/
  obj.reference_point = F360_REFERENCE_POINT_LEFT;

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[0][0] += 0.2025F;
   expected_errcov[1][1] += 0.024663991119282F;
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from REAR_RIGHT  to RIGHT
           and object pointing is 0 degrees.
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_REAR_RIGHT_To_RIGHT_Zero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set previous object reference point to REAR_RIGHT
   *    - Set current object reference point to RIGHT
   **/
  old_ref_pnt = F360_REFERENCE_POINT_REAR_RIGHT;
  obj.reference_point = F360_REFERENCE_POINT_RIGHT;

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[0][0] += 0.2025F;
   expected_errcov[1][1] += 0.024663991119282F;
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from FRONT_LEFT to LEFT
           and object pointing is 0 degrees.
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_FRONT_LEFT_To_LEFT_Zero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set previous object reference point to FRONT_LEFT
   *    - Set current object reference point to LEFT
   **/
  old_ref_pnt = F360_REFERENCE_POINT_FRONT_LEFT;
  obj.reference_point = F360_REFERENCE_POINT_LEFT;

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[0][0] += 0.2025F;
   expected_errcov[1][1] += 0.024663991119282F;
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from FRONT_RIGHT to RIGHT
           and object pointing is 0 degrees.
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_FRONT_RIGHT_To_RIGHT_Zero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set previous object reference point to FRONT_RIGHT
   *    - Set current object reference point to RIGHT
   **/
  old_ref_pnt = F360_REFERENCE_POINT_FRONT_RIGHT;
  obj.reference_point = F360_REFERENCE_POINT_RIGHT;

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[0][0] += 0.2025F;
   expected_errcov[1][1] += 0.024663991119282F;
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from LEFT to REAR_LEFT
           and object pointing is 0 degrees.
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_LEFT_To_REAR_LEFT_Zero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set previous object reference point to LEFT
   *    - Set current object reference point to REAR_LEFT
   **/
  old_ref_pnt = F360_REFERENCE_POINT_LEFT;
  obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[0][0] += 0.2025F;
   expected_errcov[1][1] += 0.024663991119282F;
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from LEFT to FRONT_LEF
           and object pointing is 0 degrees.
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_LEFT_To_FRONT_LEFT_Zero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set previous object reference point to LEFT
   *    - Set current object reference point to FRONT_LEFT
   **/
  old_ref_pnt = F360_REFERENCE_POINT_LEFT;
  obj.reference_point = F360_REFERENCE_POINT_FRONT_LEFT;

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[0][0] += 0.2025F;
   expected_errcov[1][1] += 0.024663991119282F;
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from RIGHT to REAR_RIGHT
           and object pointing is 0 degrees.
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_RIGHT_To_REAR_RIGHT_Zero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set previous object reference point to RIGHT
   *    - Set current object reference point to REAR_RIGHT
   **/
  old_ref_pnt = F360_REFERENCE_POINT_RIGHT;
  obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[0][0] += 0.2025F;
   expected_errcov[1][1] += 0.024663991119282F;
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}

/**
*\purpose  Check that object's position uncertainty is correctly increased when the reference point of a CTCA object has changed from RIGHT to FRONT_RIGHT
           and object pointing is 0 degrees.
*\req    NA
*/
TEST(Increase_Object_Pos_Uncertainty_After_Reference_Point_Change, Check_Increse_When_CTCA_From_RIGHT_To_FRONT_RIGHT_Zero_Pnt)
{
   /** \precond
   * Use default settings from test group except for:
   *    - Set previous object reference point to RIGHT
   *    - Set current object reference point to FRONT_RIGHT
   **/
  old_ref_pnt = F360_REFERENCE_POINT_RIGHT;
  obj.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;

   /** \action
   * Copy the object errcov before call such that we can later compute the expected new errcov 
   * Call function Update_Object_Position_After_Reference_Point_Change
   **/
   float32_t expected_errcov[STATE_DIMENSION][STATE_DIMENSION];
   std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(expected_errcov));

   Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibs, obj);


   /** \result
   * Check that the object errcov has been correctly increased
   **/
   expected_errcov[0][0] += 0.2025F;
   expected_errcov[1][1] += 0.024663991119282F;
   for(uint8_t idx = 0U; idx < STATE_DIMENSION; idx++)
   {
      DOUBLES_EQUAL(expected_errcov[idx][idx], obj.errcov[idx][idx], test_pass_th);
   }
}
/** @}*/


/** \defgroup Get_Number_Of_Relevant_Reference_Points
 *  @{
 */

/** \brief
*  Test group to ensure Get_Number_Of_Relevant_Reference_Points returns the desired amount of reference points depending on object distance to host
**/
TEST_GROUP(Get_Number_Of_Relevant_Reference_Points)
{
   F360_Calibrations_T calibs;
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
   }
};

/**
  *\purpose 
   Check if the correct number of reference points are returned when object is close
  *\req NA
  **/
TEST(Get_Number_Of_Relevant_Reference_Points, Check_if_correct_num_reference_points_close_range)
{
   /** \precond
    * Setup 8 points in a circle around vcs origin on just below 80m range
    **/
   const Point pt1 = Point(79.9F, 0.0F);
   const Point pt2 = Point(-79.9F, 0.0F);
   const Point pt3 = Point(0.0F, 79.9F);
   const Point pt4 = Point(0.0F, -79.9F);
   const Point pt5 = Point(56.5F, 56.5F);
   const Point pt6 = Point(-56.5F, 56.5F);
   const Point pt7 = Point(56.5F, -56.5F);
   const Point pt8 = Point(-56.5F, -56.5F);

   /** \action
   * Call function Get_Number_Of_Relevant_Reference_Points
   **/
   const uint8_t res1 = Get_Number_Of_Relevant_Reference_Points(calibs.k_far_away_object_dist_sq_thr, pt1);
   const uint8_t res2 = Get_Number_Of_Relevant_Reference_Points(calibs.k_far_away_object_dist_sq_thr, pt2);
   const uint8_t res3 = Get_Number_Of_Relevant_Reference_Points(calibs.k_far_away_object_dist_sq_thr, pt3);
   const uint8_t res4 = Get_Number_Of_Relevant_Reference_Points(calibs.k_far_away_object_dist_sq_thr, pt4);
   const uint8_t res5 = Get_Number_Of_Relevant_Reference_Points(calibs.k_far_away_object_dist_sq_thr, pt5);
   const uint8_t res6 = Get_Number_Of_Relevant_Reference_Points(calibs.k_far_away_object_dist_sq_thr, pt6);
   const uint8_t res7 = Get_Number_Of_Relevant_Reference_Points(calibs.k_far_away_object_dist_sq_thr, pt7);
   const uint8_t res8 = Get_Number_Of_Relevant_Reference_Points(calibs.k_far_away_object_dist_sq_thr, pt8);

   /** \result
   * Expect the function to return all 8 reference points
   **/
   CHECK_EQUAL(8U, res1);
   CHECK_EQUAL(8U, res2);
   CHECK_EQUAL(8U, res3);
   CHECK_EQUAL(8U, res4);
   CHECK_EQUAL(8U, res5);
   CHECK_EQUAL(8U, res6);
   CHECK_EQUAL(8U, res7);
   CHECK_EQUAL(8U, res8);
}

/**
  *\purpose 
   Check if the correct number of reference points are returned when object is far
  *\req NA
  **/
TEST(Get_Number_Of_Relevant_Reference_Points, Check_if_correct_num_reference_points_far_range)
{
   /** \precond
    * Setup 8 points in a circle around vcs origin on just above 80m range
    **/
   const Point pt1 = Point(80.1F, 0.0F);
   const Point pt2 = Point(-80.1F, 0.0F);
   const Point pt3 = Point(0.0F, 80.1F);
   const Point pt4 = Point(0.0F, -80.1F);
   const Point pt5 = Point(57.0F, 57.0F);
   const Point pt6 = Point(-57.0F, 57.0F);
   const Point pt7 = Point(57.0F, -57.0F);
   const Point pt8 = Point(-57.0F, -57.0F);

   /** \action
   * Call function Get_Number_Of_Relevant_Reference_Points
   **/
   const uint8_t res1 = Get_Number_Of_Relevant_Reference_Points(calibs.k_far_away_object_dist_sq_thr, pt1);
   const uint8_t res2 = Get_Number_Of_Relevant_Reference_Points(calibs.k_far_away_object_dist_sq_thr, pt2);
   const uint8_t res3 = Get_Number_Of_Relevant_Reference_Points(calibs.k_far_away_object_dist_sq_thr, pt3);
   const uint8_t res4 = Get_Number_Of_Relevant_Reference_Points(calibs.k_far_away_object_dist_sq_thr, pt4);
   const uint8_t res5 = Get_Number_Of_Relevant_Reference_Points(calibs.k_far_away_object_dist_sq_thr, pt5);
   const uint8_t res6 = Get_Number_Of_Relevant_Reference_Points(calibs.k_far_away_object_dist_sq_thr, pt6);
   const uint8_t res7 = Get_Number_Of_Relevant_Reference_Points(calibs.k_far_away_object_dist_sq_thr, pt7);
   const uint8_t res8 = Get_Number_Of_Relevant_Reference_Points(calibs.k_far_away_object_dist_sq_thr, pt8);

   /** \result
   * Expect the function to return all 8 reference points
   **/
   CHECK_EQUAL(4U, res1);
   CHECK_EQUAL(4U, res2);
   CHECK_EQUAL(4U, res3);
   CHECK_EQUAL(4U, res4);
   CHECK_EQUAL(4U, res5);
   CHECK_EQUAL(4U, res6);
   CHECK_EQUAL(4U, res7);
   CHECK_EQUAL(4U, res8);
}
/** @}*/

/** \defgroup  Reference_Point_Blind_Spot_Countermeasure
 *  @{
 */

/** \brief
*  Testing function that implements blind spot countermeasure for reference point selection
**/
TEST_GROUP(Reference_Point_Blind_Spot_Countermeasure)
{
   /* Common setup for test.
    Create an arbitrary object to test the blind spot countermeasure functionality. */
   F360_Object_Track_T obj;
   Reference_Point_Data_Ext ref_pnt_considering_visibility;

   /** \setup
    * Initialize common object for blind spot countermeasure testing
    **/
   TEST_SETUP()
   {
      obj.vcs_position.x = 0.0F;
      obj.vcs_position.y = 0.0F;
      obj.Set_Bbox_Orientation(Angle{ 0.0F });
      obj.Update_Bbox_Size(4.0F, 2.0F);
      obj.bbox.Set_Center(Point{0.0F, 0.0F});
      obj.reference_point = F360_REFERENCE_POINT_CENTER;
      obj.dead_zone_status = F360_Dead_Zone_Status_T::OUTSIDE;
      
      ref_pnt_considering_visibility.new_point.reference_point = F360_REFERENCE_POINT_REAR;
      ref_pnt_considering_visibility.new_point.projection_value = -1.0F;
   }
};

/**
*\purpose  Verify that blind spot countermeasure forces LEFT reference point when object enters blind spot with LEFT visible orthogonal side
*\req    NA
*/
TEST(Reference_Point_Blind_Spot_Countermeasure, Force_Left_Reference_Point_When_Enters_Blind_Spot_Left_Side_Visible)
{
   /** \precond
    * Set up object in blind spot conditions:
    * - Dead zone status IN_REAR (entering blind spot from rear)
    * - Lateral position within blind spot threshold (< 3.0m)
    * - Current reference point has LEFT as visible orthogonal side
    * - Initial new reference point set to REAR
    **/
   obj.dead_zone_status = F360_Dead_Zone_Status_T::IN_REAR;
   obj.bbox.Set_Center(Point{-2.0F, 2.5F}); // Within 3.0m lateral threshold
   obj.reference_point = F360_REFERENCE_POINT_FRONT_LEFT; // LEFT orthogonal side visible
   ref_pnt_considering_visibility.new_point.reference_point = F360_REFERENCE_POINT_REAR;

   /** \action
   * Call function Reference_Point_Blind_Spot_Countermeasure
   **/
   Reference_Point_Blind_Spot_Countermeasure(obj, ref_pnt_considering_visibility);

   /** \result
   * Verify the reference point is forced to LEFT
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_LEFT, ref_pnt_considering_visibility.new_point.reference_point);
}

/**
*\purpose  Verify that blind spot countermeasure forces RIGHT reference point when object enters blind spot with RIGHT visible orthogonal side
*\req    NA
*/
TEST(Reference_Point_Blind_Spot_Countermeasure, Force_Right_Reference_Point_When_Enters_Blind_Spot_Right_Side_Visible)
{
   /** \precond
    * Set up object in blind spot conditions:
    * - Dead zone status IN_REAR (entering blind spot from rear)
    * - Lateral position within blind spot threshold (< 3.0m)
    * - Current reference point has RIGHT as visible orthogonal side
    * - Initial new reference point set to FRONT
    **/
   obj.dead_zone_status = F360_Dead_Zone_Status_T::IN_REAR;
   obj.bbox.Set_Center(Point{-1.5F, -2.8F}); // Within 3.0m lateral threshold
   obj.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT; // RIGHT orthogonal side visible
   ref_pnt_considering_visibility.new_point.reference_point = F360_REFERENCE_POINT_FRONT;

   /** \action
   * Call function Reference_Point_Blind_Spot_Countermeasure
   **/
   Reference_Point_Blind_Spot_Countermeasure(obj, ref_pnt_considering_visibility);

   /** \result
   * Verify the reference point is forced to RIGHT
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_RIGHT, ref_pnt_considering_visibility.new_point.reference_point);
}

/**
*\purpose  Verify that blind spot countermeasure forces LEFT reference point when object is in blind spot with LEFT visible orthogonal side
*\req    NA
*/
TEST(Reference_Point_Blind_Spot_Countermeasure, Force_Left_Reference_Point_When_In_Blind_Spot_Left_Side_Visible)
{
   /** \precond
    * Set up object in blind spot conditions:
    * - Dead zone status INSIDE (center point inside)
    * - Lateral position within blind spot threshold (< 3.0m)
    * - Current reference point has LEFT as visible orthogonal side
    * - Initial new reference point set to REAR
    **/
   obj.dead_zone_status = F360_Dead_Zone_Status_T::INSIDE;
   obj.bbox.Set_Center(Point{-2.0F, 2.5F}); // Within 3.0m lateral threshold
   obj.reference_point = F360_REFERENCE_POINT_FRONT_LEFT; // LEFT orthogonal side visible
   ref_pnt_considering_visibility.new_point.reference_point = F360_REFERENCE_POINT_REAR;

   /** \action
   * Call function Reference_Point_Blind_Spot_Countermeasure
   **/
   Reference_Point_Blind_Spot_Countermeasure(obj, ref_pnt_considering_visibility);

   /** \result
   * Verify the reference point is forced to LEFT
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_LEFT, ref_pnt_considering_visibility.new_point.reference_point);
}

/**
*\purpose  Verify that blind spot countermeasure forces RIGHT reference point when object is in blind spot with RIGHT visible orthogonal side
*\req    NA
*/
TEST(Reference_Point_Blind_Spot_Countermeasure, Force_Right_Reference_Point_When_In_Blind_Spot_Right_Side_Visible)
{
   /** \precond
    * Set up object in blind spot conditions:
    * - Dead zone status INSIDE (center point inside)
    * - Lateral position within blind spot threshold (< 3.0m)
    * - Current reference point has RIGHT as visible orthogonal side
    * - Initial new reference point set to FRONT
    **/
   obj.dead_zone_status = F360_Dead_Zone_Status_T::INSIDE;
   obj.bbox.Set_Center(Point{-1.5F, -2.8F}); // Within 3.0m lateral threshold
   obj.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT; // RIGHT orthogonal side visible
   ref_pnt_considering_visibility.new_point.reference_point = F360_REFERENCE_POINT_FRONT;

   /** \action
   * Call function Reference_Point_Blind_Spot_Countermeasure
   **/
   Reference_Point_Blind_Spot_Countermeasure(obj, ref_pnt_considering_visibility);

   /** \result
   * Verify the reference point is forced to RIGHT
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_RIGHT, ref_pnt_considering_visibility.new_point.reference_point);
}

/**
*\purpose  Verify that blind spot countermeasure does NOT apply when object is outside valid dead zone status
*\req    NA
*/
TEST(Reference_Point_Blind_Spot_Countermeasure, No_Countermeasure_When_Invalid_Dead_Zone_Status)
{
   /** \precond
    * Set up object with invalid dead zone status:
    * - Dead zone status OUTSIDE (not IN_REAR or INSIDE)
    * - Lateral position within blind spot threshold
    * - Current reference point has LEFT orthogonal side
    **/
   obj.dead_zone_status = F360_Dead_Zone_Status_T::OUTSIDE;
   obj.bbox.Set_Center(Point{-2.0F, 2.0F}); // Within 3.0m lateral threshold
   obj.reference_point = F360_REFERENCE_POINT_LEFT; // LEFT orthogonal side visible
   ref_pnt_considering_visibility.new_point.reference_point = F360_REFERENCE_POINT_REAR;

   /** \action
   * Call function Reference_Point_Blind_Spot_Countermeasure
   **/
   Reference_Point_Blind_Spot_Countermeasure(obj, ref_pnt_considering_visibility);

   /** \result
   * Verify the reference point remains unchanged (no countermeasure applied)
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR, ref_pnt_considering_visibility.new_point.reference_point);
}

/**
*\purpose  Verify that blind spot countermeasure does NOT apply when object lateral position exceeds threshold
*\req    NA
*/
TEST(Reference_Point_Blind_Spot_Countermeasure, No_Countermeasure_When_Beyond_Lateral_Threshold)
{
   /** \precond
    * Set up object beyond lateral threshold:
    * - Dead zone status IN_REAR (valid)
    * - Lateral position beyond blind spot threshold (>= 3.0m)
    * - Current reference point has LEFT orthogonal side
    **/
   obj.dead_zone_status = F360_Dead_Zone_Status_T::IN_REAR;
   obj.bbox.Set_Center(Point{-2.0F, 3.5F}); // Beyond 3.0m lateral threshold
   obj.reference_point = F360_REFERENCE_POINT_LEFT; // LEFT orthogonal side visible
   ref_pnt_considering_visibility.new_point.reference_point = F360_REFERENCE_POINT_REAR;

   /** \action
   * Call function Reference_Point_Blind_Spot_Countermeasure
   **/
   Reference_Point_Blind_Spot_Countermeasure(obj, ref_pnt_considering_visibility);

   /** \result
   * Verify the reference point remains unchanged (no countermeasure applied)
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR, ref_pnt_considering_visibility.new_point.reference_point);
}

/**
*\purpose  Verify that blind spot countermeasure does NOT apply when object has FRONT/REAR reference point (no orthogonal side)
*\req    NA
*/
TEST(Reference_Point_Blind_Spot_Countermeasure, No_Countermeasure_When_No_Orthogonal_Side)
{
   /** \precond
    * Set up object with front/rear reference point:
    * - Dead zone status IN_REAR (valid)
    * - Lateral position within blind spot threshold
    * - Current reference point FRONT (no orthogonal side)
    **/
   obj.dead_zone_status = F360_Dead_Zone_Status_T::IN_REAR;
   obj.bbox.Set_Center(Point{-2.0F, 2.0F}); // Within 3.0m lateral threshold
   obj.reference_point = F360_REFERENCE_POINT_FRONT; // No orthogonal side
   ref_pnt_considering_visibility.new_point.reference_point = F360_REFERENCE_POINT_REAR;

   /** \action
   * Call function Reference_Point_Blind_Spot_Countermeasure
   **/
   Reference_Point_Blind_Spot_Countermeasure(obj, ref_pnt_considering_visibility);

   /** \result
   * Verify the reference point remains unchanged (no countermeasure applied)
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_REAR, ref_pnt_considering_visibility.new_point.reference_point);
}

/**
*\purpose  Verify that blind spot countermeasure works with negative lateral position (left side of vehicle)
*\req    NA
*/
TEST(Reference_Point_Blind_Spot_Countermeasure, Countermeasure_Works_With_Negative_Lateral_Position)
{
   /** \precond
    * Set up object with negative lateral position:
    * - Dead zone status INSIDE (valid)
    * - Negative lateral position within blind spot threshold (< 3.0m absolute)
    * - Current reference point has RIGHT orthogonal side
    **/
   obj.dead_zone_status = F360_Dead_Zone_Status_T::INSIDE;
   obj.bbox.Set_Center(Point{-1.5F, -2.5F}); // Negative Y, within 3.0m absolute threshold
   obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT; // RIGHT orthogonal side visible
   ref_pnt_considering_visibility.new_point.reference_point = F360_REFERENCE_POINT_FRONT;

   /** \action
   * Call function Reference_Point_Blind_Spot_Countermeasure
   **/
   Reference_Point_Blind_Spot_Countermeasure(obj, ref_pnt_considering_visibility);

   /** \result
   * Verify the reference point is forced to RIGHT
   **/
   CHECK_EQUAL(F360_REFERENCE_POINT_RIGHT, ref_pnt_considering_visibility.new_point.reference_point);
}

/** @}*/
