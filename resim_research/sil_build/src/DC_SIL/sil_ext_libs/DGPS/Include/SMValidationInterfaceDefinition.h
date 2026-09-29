#pragma once

#include <stdint.h>

#define INPUT_CONFIG_VERSION_MAJOR (1)
#define INPUT_CONFIG_VERSION_MINOR (0)
#define INPUT_CONFIG_VERSION_PATCH (0)
#define INPUT_CONFIG_SIZE (sizeof(SMValidationInputConfig_T))

#define OUTPUT_VERSION_MAJOR (1)
#define OUTPUT_VERSION_MINOR (0)
#define OUTPUT_VERSION_PATCH (0)
#define OUTPUT_SIZE (sizeof(SMValidationOutputConfigHeader_T))

/*SMValidation_MAX_NUMBER_OF_POSITION x 4(RT Range Target),
MAX SMValidation TARGET that RT Range can give is 4*/
#define SMVALIDATION_MAX_NUMBER_OF_TARGET (32)
#define SMVALIDATION_MAX_NUMBER_OF_DETECTIONS (100)

#define Equatorial_radius_of_Earth_in_meters (6378137u)
#define SMVALIDATION_MAX_PATH (4096)
/*Macro definition */
#define DGPS_TARGET_COUNT (1)

#pragma pack(push, 2)

/* Expose DGPS decoded data structures to external users via the public API headers. */
#include "DGPS_Msg_Data.h"
/*Position enum values are inline with the sm_config files*/
enum SMValidationSensorPosition {
	SMVALIDATION_FRONT_CENTER = 0,
	SMVALIDATION_CENTER_LEFT = 1,
	SMVALIDATION_FRONT_LEFT = 2,
	SMVALIDATION_REAR_LEFT = 3,
	SMVALIDATION_FRONT_RIGHT = 4,
	SMVALIDATION_REAR_RIGHT = 5,
	SMVALIDATION_CENTER_RIGHT = 6,
	SMVALIDATION_REAR_CENTER = 7,
	SMVALIDATION_MAX_NUMBER_OF_POSITION
};

enum DGPSBusType {
	DGPS_CAN = 0,
	DGPS_ETHERNET = 1
};

/*This has to be inline with customer in SM2*/
enum SMValidationCustomer {
	SMVALIDATION_CUSTOMER_BMW = 0,
	SMVALIDATION_CUSTOMER_FORD,
	SMVALIDATION_CUSTOMER_CHANGAN,
	SMVALIDATION_CUSTOMER_RNA,
	SMVALIDATION_CUSTOMER_SCANIA,
	SMVALIDATION_CUSTOMER_NISSAN,
	SMVALIDATION_CUSTOMER_HONDA,
	SMVALIDATION_CUSTOMER_HKMC,
	SMVALIDATION_CUSTOMER_TML,
	SMVALIDATION_CUSTOMER_STLA,
	SMVALIDATION_CUSTOMER_MTNL,
	SMVALIDATION_CUSTOMER_TRATON,
	SMVALIDATION_CUSTOMER_CEER,
	SMVALIDATION_CUSTOMER_GPO,
	SMVALIDATION_CUSTOMER_MAX
};

/*This has to be inline with customer in SM2*/
enum SMValidationSensorType {
	SMVALIDATION_SENSOR_TYPE_SRR5_PLUS = 0,
	SMVALIDATION_SENSOR_TYPE_MRR3,
	SMVALIDATION_SENSOR_TYPE_SRR5,
	SMVALIDATION_SENSOR_TYPE_SRR3,
	SMVALIDATION_SENSOR_TYPE_FLR4,
	SMVALIDATION_SENSOR_TYPE_SRR6_PLUS,
	SMVALIDATION_SENSOR_TYPE_SRR6,
	SMVALIDATION_SENSOR_TYPE_FLR4_PLUS,
	SMVALIDATION_SENSOR_TYPE_SRR7_PLUS,
	SMVALIDATION_SENSOR_TYPE_FLR7,
	SMVALIDATION_SENSOR_TYPE_SRR7_PLUS_UWB,
	SMVALIDATION_SENSOR_TYPE_FLR8,
	SMVALIDATION_SENSOR_TYPE_SRR8_PLUS,
	SMVALIDATION_SENSOR_TYPE_MAX
};

struct SMValidationMountingPose_T {
	double   osi_position_x;
	double   osi_position_y;
	double   osi_position_z;
	double   osi_orientation_roll;
	double   osi_orientation_pitch;
	double   osi_orientation_yaw;
};

struct SMValidationRadarAlignmentAngle_T {
	double vcs_boresight_az_align_angle;
	double vcs_boresight_el_align_angle;
};

struct SMValidationSensorInfo_T {
	SMValidationMountingPose_T mounting_pose;
	SMValidationRadarAlignmentAngle_T radar_alignment_angle;
	SMValidationSensorPosition sensor_id;
	SMValidationSensorType sensortype;
};

struct SMValidationHostVehicleInfo_T {
	double   host_vehicle_width;
	double   host_vehicle_length;
	double   rear_axle_position;
	double   steering_angle;
	double   yaw_rate;
	double   abs_speed;
};

struct SMValidationTrackObject_T {
	double   width;
	double   length;
	double   heading_rate;
	double   heading;
	double   tangential_accel;
	double   speed;
	double   vcs_lat_vel_rel;
	double   vcs_long_vel_rel;
	double   vcs_lat_accel;
	double   vcs_lat_vel;
	double   vcs_lat_posn;
	double   vcs_long_accel;
	double   vcs_long_vel;
	double   vcs_long_posn;
	uint8_t   stage_age;
	uint8_t   age;
	uint8_t   status;
	uint32_t   id;
	uint8_t   object_class;
};

struct SMValidationDetection_T {
	uint8_t  bistatic;
	uint8_t  nd_target;
	uint8_t  valid;
	uint8_t  elevation_confidence;
	uint8_t  azimuth_confidence;
	uint8_t  objectid;
	double   existence_probability;
	double   multi_target_probability;
	double   std_rcs;
	double   std_range;
	double   std_range_rate;
	double   std_azimuth;
	double   std_elevation;
	double   snr;
	double   amplitude;
	double   range;
	double   range_rate;
	double   azimuth;
	double   elevation;
};

struct SMValidationDetectionsInfo_T
{
	uint8_t sensorid;
	uint8_t LookType;
	uint64_t LookID;
	uint32_t num_of_dets;
	SMValidationDetection_T   dets_float[SMVALIDATION_MAX_NUMBER_OF_DETECTIONS];
};

struct SMValidationFileTimestamp_T {
   uint64_t abs_timeStamp_ns;
   uint64_t rel_timeStamp_ns;
};

struct SMValidationInputConfigHeader_T {
	uint32_t version_major;
	uint32_t version_minor;
	uint32_t version_patch;
	uint32_t size;
};

struct SMValidationOutputHeader_T {
	uint32_t version_major;
	uint32_t version_minor;
	uint32_t version_patch;
	uint32_t size;
};

struct SMValidationTimestamp_T {
	uint64_t seconds;
	uint64_t fractional_seconds;
};

struct SMValidationSensorParameter_T {
	double max_range;
	double min_range_rate;
	double max_range_rate;
	double min_azimuth;
	double max_azimuth;
	double min_elevation;
	double max_elevation;
};

struct SMValidationInputConfig_T {
	SMValidationInputConfigHeader_T header;
	SMValidationSensorInfo_T sensor_info[SMVALIDATION_MAX_NUMBER_OF_POSITION];
	SMValidationCustomer customer;
	DGPSBusType bus_type;
	bool decode_ncom_packets_from_rcom; // applicable only for ethernet bus type, if set to true, it will disable the processing of NCOM packets.
	uint8_t number_of_sensors;
	char output_path[SMVALIDATION_MAX_PATH];
	char binary_path[SMVALIDATION_MAX_PATH];
	char sm_config_path[SMVALIDATION_MAX_PATH];
};



struct SMValidationOutput_T {
	SMValidationOutputHeader_T header;
	SMValidationTimestamp_T output_timestamp[SMVALIDATION_MAX_NUMBER_OF_POSITION];
	SMValidationHostVehicleInfo_T host_info;
	SMValidationTrackObject_T object[SMVALIDATION_MAX_NUMBER_OF_TARGET];
	SMValidationDetectionsInfo_T detections[SMVALIDATION_MAX_NUMBER_OF_POSITION];
	SMValidationSensorParameter_T sensor_params[SMVALIDATION_MAX_NUMBER_OF_POSITION];
};

struct SMValidationRunConfig_T {
	char current_mdffile[SMVALIDATION_MAX_PATH];
	char next_mdffile[SMVALIDATION_MAX_PATH];
	SMValidationTimestamp_T timestamp[SMVALIDATION_MAX_NUMBER_OF_POSITION];
	SMValidationFileTimestamp_T filetimestamp[SMVALIDATION_MAX_NUMBER_OF_POSITION];
	SMValidationHostVehicleInfo_T host_info;
	uint32_t look_id[SMVALIDATION_MAX_NUMBER_OF_POSITION];
};

#pragma pack(pop)