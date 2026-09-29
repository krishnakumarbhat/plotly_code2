
#ifndef SFL_WRAPPER_H
#define SFL_WRAPPER_H

#include <stdint.h>
#include "olp_iface.h"

#include "cta_output_t.h"
#include "ta_output_t.h"
#include "scw_output_t.h"
#include "recw_output_t.h"
#include "ltb_output_t.h"
#include "lcda_output_t.h"
#include "esa_output_t.h"
#include "ced_output_t.h"
#include "pt_output_t.h"

#include "lcda_instance.h"
#include "cta_instance.h"
#include "ced_instance.h"
#include "recw_instance.h"
#include "scw_instance_t.h"
#include "ta_instance_t.h"
#include "ltb_instance.h"
#include "esa_instance_t.h"
#include "pt_instance.h"

#define SFL_OBJ_NUMBER_OF_OBJECTS OLP_NUMBER_OF_OBJECTS

typedef enum {
   SFL_OBJ_STATUS_INVALID             = (0), /**< track not existing */
   SFL_OBJ_STATUS_NEW                 = (1), /**< track created this cycle */
   SFL_OBJ_STATUS_MATURE              = (2), /**< track was confirmed by succeeding measurements of detections */
   SFL_OBJ_STATUS_COASTED             = (3), /**< track was MATURE, but is currently no longer supported by measured detections. */
   SFL_OBJ_STATUS_COASTED_IMPLAUSIBLE = (4)  /**< track was MATURE, is no longer supported by detections but also not in sensor
                             field of view. */
} SFL_Obj_Status_T;

typedef enum {
   SFL_OBJ_CLASS_UNKNOWN    = (0), /**< unknown object class */
   SFL_OBJ_CLASS_PEDESTRIAN = (1), /**< pedestrians */
   SFL_OBJ_CLASS_2WHEEL     = (2), /**< bicycles, motorbikes, etc */
   SFL_OBJ_CLASS_CAR        = (3), /**< cars */
   SFL_OBJ_CLASS_TRUCK      = (4)  /**< trucks */
} SFL_Obj_Class_T;

typedef enum {
   SFL_VEH_PRNDL_STATE_PARK    = (0), /**< park state */
   SFL_VEH_PRNDL_STATE_REVERSE = (1), /**< reverse state */
   SFL_VEH_PRNDL_STATE_NEUTRAL = (2), /**< neutral state */
   SFL_VEH_PRNDL_STATE_DRIVE   = (3), /**< drive state */
   SFL_VEH_PRNDL_STATE_LOW     = (4)  /**< low state */
} SFL_Veh_Prndl_State_t;

typedef enum {
   SFL_OBJ_CURVI_COORDINATES_UNKNOWN                  = (0), /**< unknown */
   SFL_OBJ_CURVI_COORDINATES_BASED_ON_VCS             = (1), /**< based on VCS */
   SFL_OBJ_CURVI_COORDINATES_SNAIL_TRAIL              = (2), /**< based on snail trail */
   SFL_OBJ_CURVI_COORDINATES_DISTANCE_BASED_CURVATURE = (3)  /**< distance based curvature */
} SFL_Obj_Curvi_Calc_Method_T;

typedef struct SFL_Olp_Vector_2d_Tag {
   float x; /**< First component of a vector */
   float y; /**< Second component of a vector */
} SFL_Olp_Vector_2d_T;

typedef struct SFL_Olp_Extended_Objects_Tag {
   uint8_t id;                  /**< ID of this object */
   uint32_t unique_id;          /**< unique ID of object track */
   uint8_t index;               /**< index of this object */
   SFL_Obj_Status_T status;     /**< available values are INVALID, NEW, MATURE, COASTED and COASTED_IMPLAUSIBLE */
   uint8_t age;                 /**< number of scans this object has existed */
   uint8_t stage_age;           /**< number of consecutive scans this object has had the same status */
   uint8_t fbk_stage_age;       /**< FBK-derived stage age */
   float existence_probability; /**< [%] existence probability of object */
   float speed;                 /**< [m/s] Over ground speed of the object */

   SFL_Olp_Vector_2d_T vcs_pos;     /**< [m] position of the object in vcs coordinates.*/
   SFL_Olp_Vector_2d_T vcs_vel;     /**< [m/s] velocity of the object in vcs coordinates.*/
   SFL_Olp_Vector_2d_T vcs_vel_rel; /**< [m/s] relative velocity of the object in vcs coordinates.*/
   SFL_Olp_Vector_2d_T vcs_accel;   /**< [m/s^2] acceleration of the object in vcs coordinates.*/

   float vcs_heading;      /**< [rad] heading of the object in vcs coordinates.*/
   float heading_rate;     /**< [rad/s] heading rate of the object in vcs coordinates.*/
   float heading_variance; /**< Variance of the heading value in vcs coordinates. */
   float accuracy_heading; /**< The accuracy shall describe how close the tracked object is to the real object. */

   float eclipse_value; /**< [%] eclipse value of object (= number of reference points eclipsed divided by 8).
                         * A point is said to be eclipsed if it lies in the shadow of another object */

   float length; /**< [m] length of the target vehicle */
   float width;  /**< [m] width of the target vehicle */

   float obj_distance;     /**< [m] distance to host vehicle */
   float obstruction_prob; /**< [%] Obstruction probability */

   SFL_Obj_Class_T obj_class;   /**< classification of the object based on target size */
   float class_prob_pedestrian; /**< [%] probability that the object is a pedestrian */
   float class_prob_2wheel;     /**< [%] probability that the object is a two-wheel */
   float class_prob_car;        /**< [%] probability that the object is a car */
   float class_prob_truck;      /**< [%] probability that the object is a truck */

   uint8_t id_merged_obj;   /**< ID of merged object */
   uint8_t f_merge_occured; /**< Flag indicating that this object has been merged in current cycle */

   SFL_Obj_Curvi_Calc_Method_T curvi_coordinates_calc_method; /**< Curvi coordinate calculation method. Set to 2 by default */
   SFL_Olp_Vector_2d_T curvi_pos;                             /**< [m] position of the object in curvi coordinates.*/
   SFL_Olp_Vector_2d_T curvi_vel;                             /**< [m/s] velocity of the object in curvi coordinates.*/
   SFL_Olp_Vector_2d_T curvi_vel_rel;                         /**< [m/s] relative velocity of the object in curvi coordinates.*/
   float curvi_heading;                                       /**< [rad] heading of the object in curvi coordinates.*/

   uint8_t f_reflection;         /**< Flag indicating that object is most likely a reflection */
   uint8_t f_stationary;         /**< Flag indicating that object is most likely stationary */
   uint8_t f_moveable;           /**< Flag indicating that object has moved at one point of time */
   uint8_t f_stationary_clutter; /**< Confidence that this object is stationary clutter */

   uint8_t f_is_fl_origin_sensor; /**< Flag indicating that object is seen by Front Left sensor */
   uint8_t f_is_fr_origin_sensor; /**< Flag indicating that object is seen by Front Right sensor */
   uint8_t f_is_rl_origin_sensor; /**< Flag indicating that object is seen by Rear Left sensor */
   uint8_t f_is_rr_origin_sensor; /**< Flag indicating that object is seen by Rear Right sensor */

   uint8_t f_is_in_fl_sensor_fov; /**< Flag indicating that object is in FoV of Front Left sensor */
   uint8_t f_is_in_fr_sensor_fov; /**< Flag indicating that object is in FoV of Front Right sensor */
   uint8_t f_is_in_rl_sensor_fov; /**< Flag indicating that object is in FoV of Rear Left sensor */
   uint8_t f_is_in_rr_sensor_fov; /**< Flag indicating that object is in FoV of Rear Right sensor */
} SFL_Olp_Extended_Objects_T;

typedef struct SFL_Olp_Objects_Log_Tag {
   SFL_Olp_Extended_Objects_T obj[SFL_OBJ_NUMBER_OF_OBJECTS];
   uint32_t n_valid_objects; // Number of valid objects in the list
} SFL_Olp_Objects_Log_T;

typedef struct SFL_Vehicle_Output_Tag {
   float host_length;           /**< [m] Length of host vehicle */
   float host_width;            /**< [m] Width of host vehicle */
   float rear_axle_position;    /**< [m] Rear axle position of host vehicle (to VCS origin at front center)*/
   float host_speed;            /**< [m/s] Over ground speed of host vehicle */
   float steering_angle;        /**< [rad] Steering angle of host vehicle (in VCS, clockwise positive) */
   float yawrate;               /**< [rad/s] Yawrate of host vehicle */
   float long_vel;              /**< [m/s] Longitudinal velocity of host vehicle */
   float long_acc;              /**< [m/s^2] Longitudinal acceleration of host vehicle */
   float lat_acc;               /**< [m/s^2] Lateral acceleration of host vehicle */
   SFL_Veh_Prndl_State_t prndl; /**< Park-Reverse-Neutral-Drive-Low setting (enum 0-1-2-3-4) */
   float lane_width;            /**< [m] Lane width of host vehicle lane */
   float lane_center_offset;    /**< [m] Lane center offset */
   uint8_t turn_signal;         /**< Turn signal: None-Left-Right (enum 0-1-2) */
   float curvature;             /**< [1/m] Host curvature */
   bool f_reverse;              /**< Flag indicating if host vehicle is in reverse gear */
   float wheelbase;             /**< [m] Wheelbase distance of host vehicle */
} SFL_Vehicle_Output_T;

/* function declarations */
void SFLFillVehicleData();
void SFLFillObjectData();
void InitFeatureFunction(void);
void RunFeatureFunction(void);

SFL_Vehicle_Output_T *GetSFLVehiclePtr();
SFL_Olp_Objects_Log_T *GetSFLObjectPtr();

extern "C" Cta_Output_T *Cta_Get_Output_Ptr(void);
extern "C" Ta_Output_T *Ta_Get_Output_Ptr(void);
extern "C" Scw_Output_T *Scw_Get_Output_Ptr(void);
extern "C" Recw_Output_T *Recw_Get_Output_Ptr(void);
extern "C" Ltb_Output_T *Ltb_Get_Output_Ptr(void);
extern "C" Lcda_Output_T *Lcda_Get_Output_Ptr(void);
extern "C" Esa_Output_T *Esa_Get_Output_Ptr(void);
extern "C" Ced_Output_T *Ced_Get_Output_Ptr(void);
extern "C" Pt_Output_T *Pt_Get_Output_Ptr(void);

extern "C" Lcda_Instance_T *Lcda_Get_Instance_Ptr(void);
extern "C" Cta_Instance_T *Cta_Get_Instance_Ptr(void);
extern "C" Ced_Instance_T *Ced_Get_Instance_Ptr(void);
extern "C" Esa_Instance_T *Esa_Get_Instance_Ptr(void);
extern "C" Ltb_Instance_T *Ltb_Get_Instance_Ptr(void);
extern "C" Pt_Instance_T *Pt_Get_Instance_Ptr(void);
extern "C" Recw_Instance_T *Recw_Get_Instance_Ptr(void);
extern "C" Scw_Instance_T *Scw_Get_Instance_Ptr(void);
extern "C" Ta_Instance_T *Ta_Get_Instance_Ptr(void);

#endif // SFL_ADAPTER_H
