#ifndef OLP_IFACE_H
#define OLP_IFACE_H

/**************************************************************************************************\
* Preprocessor Constants
\**************************************************************************************************/
#define OLP_SW_MAJOR_VERSION 5u
#define OLP_SW_MINOR_VERSION 1u
#define OLP_SW_PATCH_VERSION 1u

#ifndef OLP_NUMBER_OF_OBJECTS
#define OLP_NUMBER_OF_OBJECTS (250)
#endif

#ifndef OLP_OBJ_SOURCE_F360
#define OLP_OBJ_SOURCE_F360
#endif

/**************************************************************************************************\
* enums
\**************************************************************************************************/
typedef enum
{
   OLP_OBJ_STATUS_INVALID             = (0), /**< track not existing */
   OLP_OBJ_STATUS_NEW                 = (1), /**< track created this cycle */
   OLP_OBJ_STATUS_MATURE              = (2), /**< track was confirmed by succeeding measurements of detections */
   OLP_OBJ_STATUS_COASTED             = (3), /**< track was MATURE, but is currently no longer supported by measured detections. */
   OLP_OBJ_STATUS_COASTED_IMPLAUSIBLE = (4)  /**< track was MATURE, is no longer supported by detections but also not in sensor
                                                      field of view. */
} Olp_Obj_Status_T;

typedef enum
{
   OLP_OBJ_CLASS_UNKNOWN    = (0), /**< unknown object class */
   OLP_OBJ_CLASS_PEDESTRIAN = (1), /**< pedestrians */
   OLP_OBJ_CLASS_2WHEEL     = (2), /**< bicycles, motorbikes, etc */
   OLP_OBJ_CLASS_CAR        = (3), /**< cars */
   OLP_OBJ_CLASS_TRUCK      = (4)  /**< trucks */
} Olp_Obj_Class_T;

typedef enum
{
   OLP_OBJ_CURVI_COORDINATES_UNKNOWN                  = (0), /**< unknown */
   OLP_OBJ_CURVI_COORDINATES_BASED_ON_VCS             = (1), /**< based on VCS */
   OLP_OBJ_CURVI_COORDINATES_SNAIL_TRAIL              = (2), /**< based on snail trail */
   OLP_OBJ_CURVI_COORDINATES_DISTANCE_BASED_CURVATURE = (3)  /**< distance based curvature */
} Olp_Obj_Curvi_Calc_Method_T;

/**************************************************************************************************\
   * typedefs
   \**************************************************************************************************/
/* Input Types */
typedef struct Vehicle_Info_Tag
{
   float comp_yaw_rate_filtered; /* [rad/s] Filtered and bias compensated yaw rate of host vehicle */
   float k_dist_rear_axle_to_vcs; /* [m] Longitudinal distance of host vehicle rear axle from VCS origin*/
   float filt_veh_speed_over_ground; /* [m/s] Filtered Veh Speed of host vehicle */
   float vcs_sideslip; /* [rad] Sideslip of host at VCS origin (center of the front bumper) */
   float sideslip_rear_axle; /* [rad] Sideslip of host at the rear axle */
   float vcs_long_velocity; /* [m/s] Host vehicle Longitudinal velocity component at the VCS origin */
   float vcs_lat_velocity; /* [m/s] Host vehicle Lateral velocity component at the VCS origin */
} Vehicle_Info_T;

/* Output Types */
typedef struct Olp_Vector_2d_Tag
{
   float x; /**< First component of a vector */
   float y; /**< Second component of a vector */
} Olp_Vector_2d_T;

typedef struct Olp_InOut_Object_Data_Tag
{
   unsigned char id;             /**< ID of this object assigned for its lifetime */
   unsigned int unique_id;     /**< unique ID of object track */
   unsigned char index;          /**< index of this object in the input object array (IDs can change indeces in array over time) */
   Olp_Obj_Status_T status; /**< available values are INVALID, NEW, MATURE, COASTED and COASTED_IMPLAUSIBLE */
   unsigned char age;            /**< number of scans this object has existed */
   unsigned char stage_age;      /**< number of consecutive scans this object has had the same status */
   unsigned char fbk_stage_age;  /**< FBK-derived stage age */
   float existence_probability; /**< [0,1] existence probability of the object */
   float speed;                 /**< [m/s] Over ground speed of the object */

   Olp_Vector_2d_T vcs_pos;     /**< [m] position of the object bounding box center in vcs coordinates (origin at host front center).*/
   Olp_Vector_2d_T vcs_vel;     /**< [m/s] velocity of the object in vcs coordinates.*/
   Olp_Vector_2d_T vcs_vel_rel; /**< [m/s] relative velocity of the object in vcs coordinates.*/
   Olp_Vector_2d_T vcs_accel;   /**< [m/s^2] acceleration of the object in vcs coordinates.*/

   float vcs_heading;      /**< [rad] [-pi, pi] heading of the object in vcs coordinates. Clockwise positive.*/
   float heading_rate;     /**< [rad/s] heading rate of the object in vcs coordinates.*/
   float heading_variance; /**< Variance of the heading value in vcs coordinates. */
   float accuracy_heading; /**< The accuracy shall describe how close the tracked object is to the real object. */

   float eclipse_value; /**< [0,1] eclipse value of object (= number of reference points eclipsed divided by 8).
                           * A point is said to be eclipsed if it lies in the shadow of another object */

   float length; /**< [m] length of the target vehicle */
   float width;  /**< [m] width of the target vehicle */

   float obj_distance;     /**< [m] distance from the object closest reference point to host vehicle side rear corner.
                              * When object passed the host rear bumper the obj_distance becomes only lateral distance between     them.*/
   float obstruction_prob; /**< [0,1] Obstruction probability. Probability that this object is an obstacle rather than
                                 relevant moving object. */

   Olp_Obj_Class_T obj_class;        /**< classification of the object based on target size */
   float class_prob_pedestrian; /**< [0,1] probability that the object is a pedestrian */
   float class_prob_2wheel;     /**< [0,1] probability that the object is a two-wheel */
   float class_prob_car;        /**< [0,1] probability that the object is a car */
   float class_prob_truck;      /**< [0,1] probability that the object is a truck */

   unsigned char id_merged_obj;     /**< ID of merged object (object that is not exisitng any more, but became part of this object) */
   unsigned char f_merge_occured; /**< Flag indicating that another object has been merged to this object in current cycle */

   Olp_Obj_Curvi_Calc_Method_T curvi_coordinates_calc_method; /**< Curvi coordinate calculation method */
   Olp_Vector_2d_T curvi_pos;                                    /**< [m] position of the object in curvi coordinates.*/
   Olp_Vector_2d_T curvi_vel;                                    /**< [m/s] velocity of the object in curvi coordinates.*/
   Olp_Vector_2d_T curvi_vel_rel;                                /**< [m/s] relative velocity of the object in curvi coordinates.*/
   float curvi_heading;                                  /**< [rad] [-pi,pi] heading of the object in curvi coordinates.*/

   unsigned char f_reflection;         /**< Flag indicating that object is most likely a reflection */
   unsigned char f_stationary;         /**< Flag indicating that object is most likely stationary */
   unsigned char f_moveable;           /**< Flag indicating that object has moved at one point of time */
   unsigned char f_stationary_clutter; /**< Confidence that this object is stationary clutter */

   unsigned char f_is_fl_origin_sensor; /**< Flag indicating that object is seen by Front Left sensor */
   unsigned char f_is_fr_origin_sensor; /**< Flag indicating that object is seen by Front Right sensor */
   unsigned char f_is_rl_origin_sensor; /**< Flag indicating that object is seen by Rear Left sensor */
   unsigned char f_is_rr_origin_sensor; /**< Flag indicating that object is seen by Rear Right sensor */

   unsigned char f_is_in_fl_sensor_fov; /**< Flag indicating that object is in FoV of Front Left sensor */
   unsigned char f_is_in_fr_sensor_fov; /**< Flag indicating that object is in FoV of Front Right sensor */
   unsigned char f_is_in_rl_sensor_fov; /**< Flag indicating that object is in FoV of Rear Left sensor */
   unsigned char f_is_in_rr_sensor_fov; /**< Flag indicating that object is in FoV of Rear Right sensor */
} Olp_InOut_Object_Data_T;

typedef struct Olp_Extended_Object_Data_Tag
{
   float curvature;                        /* [rad/m] trajectory curvature of object track's center position in VCS coordinate. */
#ifdef OLP_OBJ_SOURCE_F360
   float len1;                         /*< [m] longitudinal distance from reference point to rear edge of bounding box. */
   float len2;                         /*< [m] longitudinal distance from reference point to front edge of bounding box. */
   float wid1;                         /*< [m] lateral distance from reference point to left edge of bounding box. */
   float wid2;                         /*< [m] lateral distance from reference point to right edge of bounding box. */
   float tang_accel;               /*< [m/s^2] over-the-ground tangential acceleration of object's reference point. */
   float vcs_pointing; //!< [rad] orientation of track's bounding box in VCS coordinate.
#endif
} Olp_Extended_Object_Data_T;

typedef struct Olp_Data_Tag
{
   unsigned int no_of_valid_objects;
   Vehicle_Info_T veh_info_data;
   Olp_InOut_Object_Data_T olp_inout_obj_data[OLP_NUMBER_OF_OBJECTS];
   Olp_Extended_Object_Data_T olp_extnd_obj_data[OLP_NUMBER_OF_OBJECTS];
} Olp_Data_T;

#ifdef __cplusplus
extern "C" void OLP_Init(void);
extern "C" void OLP_Main_Run_50ms(Olp_Data_T *olp_data_ref);
extern "C" unsigned char OLP_Get_Sw_Major_Version(void);
extern "C" unsigned char OLP_Get_Sw_Minor_Version(void);
extern "C" unsigned char OLP_Get_Sw_Patch_Version(void);
#else
void OLP_Init(void);
void OLP_Main_Run_50ms(Olp_Data_T &olp_data_ref);
unsigned char OLP_Get_Sw_Major_Version(void);
unsigned char OLP_Get_Sw_Minor_Version(void);
unsigned char OLP_Get_Sw_Patch_Version(void);
#endif

#endif /* OLP_IFACE_H*/

