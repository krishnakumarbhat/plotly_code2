#if 1

#include "f360_xtrk_logging.h"

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <assert.h>
#include <float.h>
#include <fstream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm")

#elif defined(__linux__)
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdarg>
#include <cstddef>

typedef int errno_t;
static const int _MAX_PATH = 260;

template <typename T, size_t N>
constexpr size_t _countof(T (&)[N]) {
   return std::extent<T[N]>::value;
}

int _isnan(double x) {
   return std::isnan(x);
}
int _finite(double x) {
   return std::isfinite(x);
}

inline int sprintf_s(char *buffer, size_t sizeOfBuffer, const char *format, ...) {
   va_list ap;
   va_start(ap, format);
   int result = vsnprintf(buffer, sizeOfBuffer, format, ap);
   va_end(ap);
   return result;
}

template <size_t sizeOfBuffer>
inline int sprintf_s(char (&buffer)[sizeOfBuffer], const char *format, ...) {
   va_list ap;
   va_start(ap, format);
   int result = vsnprintf(buffer, sizeOfBuffer, format, ap);
   va_end(ap);
   return result;
}
static errno_t fopen_s(FILE **f, const char *name, const char *mode) {
   errno_t ret = 0;
   assert(f);
   *f = fopen(name, mode);
   if (!*f)
      ret = errno;
   return ret;
}

template <typename T>
T max(T a, T b) {
   return std::max(a, b);
}
#endif

namespace f360_variant_A
{

static const int VERSION_RAWTRACK  = 2;
static const int MAGIC_RAWTRACK    = 0x5277546b; // RwTk
static const int MAXIDENTIFIER     = 63;         // mod(MAXIDENTIFIER+1, 4) == 0
static const int MAXTYPEIDENTIFIER = 63;         // mod(MAXTYPEIDENTIFIER+1, 4) == 0

// XTRK types and macros
#pragma pack(push) // Byte align the header info
#pragma pack(1)

typedef struct HDR_RAWTRACK_TAG {
   int32_t magicNum;
   int32_t version;
   int32_t numSupBlocks; // # of Super Block
   int32_t szHeader;     // size in bytes of the file header, including all the block and super block information.
   int32_t szRecord;     // size in bytes of record. One record is a full set of superblock with each block repeated with the matching numRepeats
} HDR_RAWTRACK_T;

typedef struct BLOCK_TAG {
   int32_t szType;                      // size of type in bytes, only need this in the last block to determine pad, but simpler to dup across all the blocks in a supBlock
   char field[MAXIDENTIFIER + 1];       // name of structure field for MATLAB
   char typein[MAXTYPEIDENTIFIER + 1];  // data type in, e.g. int32_t, double, ...
   char typeout[MAXTYPEIDENTIFIER + 1]; // MATLAB data type out, e.g. int32, double, TRACK_STATUS, ...  Used to map integers to reals or integer to enumerated types
   int32_t offset;                      // offset in bytes for structure field
   int32_t numBytes;                    // # of bytes, size of field (block)
   int32_t rows;                        // The field (block) is interpreted asa 2D array: rows x cols
   int32_t cols;
} BLOCK_T;

typedef struct SUP_BLOCK_INFO_TAG {
   char name[MAXIDENTIFIER + 1];
   int32_t numRepeats;
   int32_t numBlocks;
} SUP_BLOCK_INFO_T;

typedef struct SUPER_BLOCK_TAG {
   BLOCK_T *block;
   SUP_BLOCK_INFO_T info;
} SUPER_BLOCK_T;

#pragma pack(pop)

#define SET_BLOCK(TypeName, Field, Typein, Typeout, Rows, Cols)                                                      {sizeof(TypeName), #Field, #Typein, #Typeout, offsetof(TypeName, Field), sizeof(Typein), Rows, Cols}
#define SET_BLOCK_PRIV_MEMBER(TypeName, Field, FieldPrivMember, FielddPrivMemberOffset, Typein, Typeout, Rows, Cols) {sizeof(TypeName), FieldPrivMember, #Typein, #Typeout, offsetof(TypeName, Field) + FielddPrivMemberOffset, sizeof(Typein), Rows, Cols}
#define SET_SUP_BLOCK(Block, Name, NumRepeats) \
   {                                           \
      Block, {                                 \
         #Name, NumRepeats, _countof(Block)    \
      }                                        \
   }

// Special handling of the detection hist struct
typedef struct F360_Detection_Hist_Props_Tag {
   bool f_idx_occupied[MAX_NUMBER_OF_HISTORIC_DETECTIONS];
   int32_t n_occupied;
   int32_t max_occupation;
} F360_Detection_Hist_Props_T;

#ifdef Default_variant_A_defined
#define XTRK_ASSERT(expn) typedef char __XTRK_ASSERT__[(expn) ? 1 : -1]
XTRK_ASSERT(sizeof(F360_Detection_Hist_T) == sizeof(F360_Detection_Hist_Props_T) + MAX_NUMBER_OF_HISTORIC_DETECTIONS * sizeof(F360_Detection_Hist_Data_T));
#endif
// Special handling of sensor props struct, logging internal reflection buffers separately
struct Internal_Reflection_Buffer_Xtrklog {
   float32_t range[INTERNAL_REFLECTIONS_BUFFER_SIZE];
   float32_t azimuth[INTERNAL_REFLECTIONS_BUFFER_SIZE];
   float32_t rcs[INTERNAL_REFLECTIONS_BUFFER_SIZE];
   uint32_t occurrence_count[INTERNAL_REFLECTIONS_BUFFER_SIZE];
   uint32_t age[INTERNAL_REFLECTIONS_BUFFER_SIZE];
   bool f_updated_this_cycle[INTERNAL_REFLECTIONS_BUFFER_SIZE];
   bool f_classified_as_internal_reflection[INTERNAL_REFLECTIONS_BUFFER_SIZE];
   uint8_t padding[2][INTERNAL_REFLECTIONS_BUFFER_SIZE];
};

// Special handling of tracker_version info
struct Tracker_Version_Xtrk_Log {
   uint8_t major;
   uint8_t minor;
   uint8_t patch;
   uint8_t padding;
};

static F360_Initialization_Debug_Data_T init_debug_data;
static F360_Initialization_Cluster_Debug_Data_T init_cluster_debug_data;

// Record Size define
static const int32_t MAX_XTRK_RECORDS                   = 30;
static int32_t XTRK_NUM_RECORDS                         = 0;
static size_t XTRK_RECORD_SIZE_CUMSUM[MAX_XTRK_RECORDS] = {0};
// END - XTRK types and macros

// OUTPUT Files
static FILE *fp_xtrk = NULL;

static int init_xtrk_log(void);

void close_xtrk_log(void) {
   if (NULL != fp_xtrk) {
      fflush(fp_xtrk);
      fclose(fp_xtrk);
   }
}

bool open_xtrk_log(const char *filename_base) {
   char filename[_MAX_PATH + 1] = "";

   // xtrk binary log file ================================================================
   sprintf_s(filename, sizeof(filename) - 1, "%s%s", filename_base, ".xtrk");
   errno_t err = fopen_s(&fp_xtrk, filename, "wb");
   if (err) {
      printf("\txtrk binary output file: %s\n", filename);
      return (false);
   }
   init_xtrk_log();

   return (true); // Success
}

int write_xtrk_descriptor(const SUPER_BLOCK_T SupBlocks[], int cntSupBlocks) {
   // Check if the XTRAK block sizes will fit inside the size check array
   XTRK_NUM_RECORDS = cntSupBlocks;
   if (MAX_XTRK_RECORDS < XTRK_NUM_RECORDS) {
      printf("XTRK: write_xtrk_descriptor: To many SupBlocks defined %d, max %d. Increase the size define MAX_XTRK_RECORDS\n", cntSupBlocks, MAX_XTRK_RECORDS);
   }

   int i;
   int j;
   HDR_RAWTRACK_T hdr = {MAGIC_RAWTRACK, VERSION_RAWTRACK, cntSupBlocks, sizeof(hdr), 0};
   size_t cnt         = 0;
   if (NULL != fp_xtrk) {
      for (i = 0; i < hdr.numSupBlocks; i++) {
         hdr.szHeader += sizeof(SUP_BLOCK_INFO_T) + SupBlocks[i].info.numBlocks * sizeof(BLOCK_T);
         hdr.szRecord += SupBlocks[i].info.numRepeats * SupBlocks[i].block[0].szType;

         // Add the super block size into the size check array
         XTRK_RECORD_SIZE_CUMSUM[i] = static_cast<size_t>(hdr.szRecord);
      }

      cnt += sizeof(hdr) * fwrite(&hdr, sizeof(hdr), 1, fp_xtrk);

      for (i = 0; i < hdr.numSupBlocks; i++) {
         cnt += sizeof(SUP_BLOCK_INFO_T) * fwrite(&SupBlocks[i].info, sizeof(SUP_BLOCK_INFO_T), 1, fp_xtrk);

         for (j = 0; j < SupBlocks[i].info.numBlocks; j++) {
            cnt += sizeof(BLOCK_T) * fwrite(&SupBlocks[i].block[j], sizeof(BLOCK_T), 1, fp_xtrk);
         }
      }
   }

   return (cnt == static_cast<size_t>(hdr.szHeader));
}

int init_xtrk_log(void) {
   BLOCK_T Sensor_Block[] = {
       SET_BLOCK(F360_Radar_Sensor_T, constant.internal_reflections.min_host_vel, float, single, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.internal_reflections.occurrence_lowerlimit, float, single, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.internal_reflections.occurrence_threshold, float, single, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.internal_reflections.rcs_tolerance, float, single, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.internal_reflections.azimuth_tolerance, float, single, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.internal_reflections.range_tolerance, float, single, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.internal_reflections.max_abs_range_rate, float, single, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.internal_reflections.rcs_max, float, single, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.internal_reflections.range_max, float, single, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.internal_reflections.age_threshold, uint16_t, uint16, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.internal_reflections.f_enable, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.mounting_position.vcs_position.longitudinal, float, single, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.mounting_position.vcs_position.lateral, float, single, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.mounting_position.vcs_position.height, float, single, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.mounting_position.vcs_boresight_azimuth_angle, float, single, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.mounting_position.vcs_boresight_elevation_angle, float, single, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.range_limits, float, single, 4, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.fov_min_az_rad, float, single, 4, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.fov_max_az_rad, float, single, 4, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.fov_min_el_rad, float, single, 4, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.fov_max_el_rad, float, single, 4, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.min_aliaised_range_rate, float, single, 4, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.v_wrapping, float, single, 4, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.r_wrapping, float, single, 4, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.id, uint32_t, uint32, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.polarity, int32_t, int32, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.mounting_location, int8_t, MOUNT_LOC, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, constant.sensor_type, int8_t, SENSOR, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, variable.timestamp_us, uint64_t, uint64, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, variable.vcs_velocity.longitudinal, float, single, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, variable.vcs_velocity.lateral, float, single, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, variable.vacs_boresight_az_estimated, float, single, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, variable.vacs_boresight_el_estimated, float, single, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, variable.number_of_valid_detections, uint32_t, uint32, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, variable.overall_rain_level, uint16_t, uint16, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, variable.look_index, uint16_t, uint16, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, variable.is_valid, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, variable.f_sensor_fault_detected, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, variable.look_id, int8_t, LOOK_ID, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_T, refined.interior_fov, float, single, 4, 1),
       SET_BLOCK(F360_Radar_Sensor_T, refined.left_fov_normal, float, single, 4, 1),
       SET_BLOCK(F360_Radar_Sensor_T, refined.right_fov_normal, float, single, 4, 1),
       SET_BLOCK(F360_Radar_Sensor_T, refined.time_since_measurement_s, float, single, 1, 1),
   };

   BLOCK_T Sensor_Props_Block[] = {
       SET_BLOCK(F360_Radar_Sensor_Props_T, f_object_track_next_to_sensor, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_Props_T, next_to_sensor_object_track_id, int32_t, int32, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_Props_T, next_to_sensor_object_track_min_long_pos, float, single, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_Props_T, next_to_sensor_object_track_max_long_pos, float, single, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_Props_T, next_to_sensor_object_track_min_lat_pos, float, single, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_Props_T, next_to_sensor_object_track_max_lat_pos, float, single, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_Props_T, low_power_clutter_severity, float, single, 1, 1),
       SET_BLOCK(F360_Radar_Sensor_Props_T, rain_level_filtered, float, single, 1, 1),
   };

   BLOCK_T Sensor_Props_Int_Ref_Buf_Block[] = {
       SET_BLOCK(Internal_Reflection_Buffer_Xtrklog, range, float, single, INTERNAL_REFLECTIONS_BUFFER_SIZE, 1),
       SET_BLOCK(Internal_Reflection_Buffer_Xtrklog, azimuth, float, single, INTERNAL_REFLECTIONS_BUFFER_SIZE, 1),
       SET_BLOCK(Internal_Reflection_Buffer_Xtrklog, rcs, float, single, INTERNAL_REFLECTIONS_BUFFER_SIZE, 1),
       SET_BLOCK(Internal_Reflection_Buffer_Xtrklog, occurrence_count, uint32_t, uint32, INTERNAL_REFLECTIONS_BUFFER_SIZE, 1),
       SET_BLOCK(Internal_Reflection_Buffer_Xtrklog, age, uint32_t, uint32, INTERNAL_REFLECTIONS_BUFFER_SIZE, 1),
       SET_BLOCK(Internal_Reflection_Buffer_Xtrklog, f_updated_this_cycle, uint8_t, logical, INTERNAL_REFLECTIONS_BUFFER_SIZE, 1),
       SET_BLOCK(Internal_Reflection_Buffer_Xtrklog, f_classified_as_internal_reflection, uint8_t, logical, INTERNAL_REFLECTIONS_BUFFER_SIZE, 1),
       SET_BLOCK(Internal_Reflection_Buffer_Xtrklog, padding, uint8_t, uint8, INTERNAL_REFLECTIONS_BUFFER_SIZE, 2),
   };

   BLOCK_T Static_Host_Block[] = {
       SET_BLOCK(F360_Host_Calib_T, dist_rear_axle_to_vcs_m, float, single, 1, 1),
       SET_BLOCK(F360_Host_Calib_T, rear_cornering_compliance, float, single, 1, 1),
       SET_BLOCK(F360_Host_Calib_T, steer_gear_ratio, float, single, 1, 1),
       SET_BLOCK(F360_Host_Calib_T, wheelbase_m, float, single, 1, 1),
       SET_BLOCK(F360_Host_Calib_T, understeer_coefficient, float, single, 1, 1),
       SET_BLOCK(F360_Host_Calib_T, vehicle_width_m, float, single, 1, 1),
       SET_BLOCK(F360_Host_Calib_T, vehicle_length_m, float, single, 1, 1),
       SET_BLOCK(F360_Host_Calib_T, cog_x, float, single, 1, 1),
       SET_BLOCK(F360_Host_Calib_T, cog_y, float, single, 1, 1),
       SET_BLOCK(F360_Host_Calib_T, front_wheel_radius_m, float, single, 1, 1),
       SET_BLOCK(F360_Host_Calib_T, front_track_width_m, float, single, 1, 1),
   };

   BLOCK_T Raw_Host_Block[] = {
       SET_BLOCK(F360_Host_Raw_T, global_time_sync_s, float, single, 1, 1),
       SET_BLOCK(F360_Host_Raw_T, timestamp_s, float, single, 1, 1),
       SET_BLOCK(F360_Host_Raw_T, raw_speed, float, single, 1, 1),
       SET_BLOCK(F360_Host_Raw_T, raw_yaw_rate_rad, float, single, 1, 1),
       SET_BLOCK(F360_Host_Raw_T, steering_wheel_angle_rad, float, single, 1, 1),
       SET_BLOCK(F360_Host_Raw_T, road_wheel_angle_rad, float, single, 1, 1),
       SET_BLOCK(F360_Host_Raw_T, lat_accel, float, single, 1, 1),
       SET_BLOCK(F360_Host_Raw_T, long_accel, float, single, 1, 1),
       SET_BLOCK(F360_Host_Raw_T, prndl, uint8_t, PRNDL_STATE, 1, 1),
       SET_BLOCK(F360_Host_Raw_T, reverse_gear, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Host_Raw_T, speed_qf, uint8_t, QUALITY_FACTOR, 1, 1),
       SET_BLOCK(F360_Host_Raw_T, yaw_rate_qf, uint8_t, QUALITY_FACTOR, 1, 1),
       SET_BLOCK(F360_Host_Raw_T, steering_wheel_angle_qf, uint8_t, QUALITY_FACTOR, 1, 1),
       SET_BLOCK(F360_Host_Raw_T, road_wheel_angle_qf, uint8_t, QUALITY_FACTOR, 1, 1),
       SET_BLOCK(F360_Host_Raw_T, lat_accel_qf, uint8_t, QUALITY_FACTOR, 1, 1),
       SET_BLOCK(F360_Host_Raw_T, long_accel_qf, uint8_t, QUALITY_FACTOR, 1, 1),
       SET_BLOCK(F360_Host_Raw_T, f_trailer_presence_hardware, uint8_t, logical, 1, 1),
   };

   BLOCK_T Host_Block[] = {
       SET_BLOCK(F360_Host_T, vehicle_index, uint32_t, uint32, 1, 1),
       SET_BLOCK(F360_Host_T, speed, float, single, 1, 1),
       SET_BLOCK(F360_Host_T, vcs_speed, float, single, 1, 1),
       SET_BLOCK(F360_Host_T, acceleration, float, single, 1, 1),
       SET_BLOCK(F360_Host_T, vcs_lat_acceleration, float, single, 1, 1),
       SET_BLOCK(F360_Host_T, vcs_long_acceleration, float, single, 1, 1),
       SET_BLOCK(F360_Host_T, yaw_rate_rad, float, single, 1, 1),
       SET_BLOCK(F360_Host_T, vcs_sideslip, float, single, 1, 1),
       SET_BLOCK(F360_Host_T, curvature_rear, float, single, 1, 1),
       SET_BLOCK(F360_Host_T, dist_rear_axle_to_vcs_m, float, single, 1, 1),
       SET_BLOCK(F360_Host_T, rear_cornering_compliance, float, single, 1, 1),
       SET_BLOCK(F360_Host_T, speed_correction_factor, float, single, 1, 1),
       SET_BLOCK(F360_Host_T, speed_qf, uint8_t, QUALITY_FACTOR, 1, 1),
       SET_BLOCK(F360_Host_T, yaw_rate_qf, uint8_t, QUALITY_FACTOR, 1, 1),
       SET_BLOCK(F360_Host_T, lat_accel_qf, uint8_t, QUALITY_FACTOR, 1, 1),
       SET_BLOCK(F360_Host_T, long_accel_qf, uint8_t, QUALITY_FACTOR, 1, 1),
       SET_BLOCK(F360_Host_T, f_trailer_presence_hardware, uint8_t, logical, 1, 1),
   };

   BLOCK_T Host_Props_Block[] = {
       SET_BLOCK(F360_Host_Props_T, position_x, float, single, 1, 1),
       SET_BLOCK(F360_Host_Props_T, position_y, float, single, 1, 1),
       SET_BLOCK(F360_Host_Props_T, heading_angle, float, single, 1, 1),
       SET_BLOCK(F360_Host_Props_T, cos_heading, float, single, 1, 1),
       SET_BLOCK(F360_Host_Props_T, sin_heading, float, single, 1, 1),
       SET_BLOCK(F360_Host_Props_T, delta_pointing, float, single, 1, 1),
       SET_BLOCK(F360_Host_Props_T, cos_delta_pointing, float, single, 1, 1),
       SET_BLOCK(F360_Host_Props_T, sin_delta_pointing, float, single, 1, 1),
       SET_BLOCK(F360_Host_Props_T, delta_position_x, float, single, 1, 1),
       SET_BLOCK(F360_Host_Props_T, delta_position_y, float, single, 1, 1),
   };

   BLOCK_T Raw_Detection_Block[] = {
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, processed.range_rate_compensated, float, single, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, processed.next_sorted_idx, int16_t, int16, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, processed.prev_sorted_idx, int16_t, int16, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, raw.sensor_id, int32_t, int32, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, raw.det_id, int32_t, int32, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, raw.range, float, single, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, raw.std_range, float, single, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, raw.range_rate, float, single, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, raw.std_range_rate, float, single, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, raw.azimuth, float, single, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, raw.std_azimuth, float, single, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, processed.vcs_az, float, single, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, processed.cos_vcs_az, float, single, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, processed.sin_vcs_az, float, single, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, raw.elevation, float, single, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, raw.std_elevation, float, single, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, raw.snr, float, single, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, raw.rcs, float, single, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, raw.confid_azimuth, int8_t, int8, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, raw.confid_elevation, int8_t, int8, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, raw.f_super_res, uint8_t, logical, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, raw.f_host_veh_clutter, uint8_t, logical, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, raw.f_nd_target, uint8_t, logical, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, raw.f_bistatic, uint8_t, logical, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, processed.f_ok_to_use, uint8_t, logical, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, processed.motion_status, int8_t, DET_MOTION, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, processed.vcs_position_x, float, single, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, processed.vcs_position_y, float, single, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, processed.vcs_position_z, float, single, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, processed.vcs_el, float, single, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, raw.f_ci_det, uint8_t, logical, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, raw.f_idm_det, uint8_t, logical, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, raw.f_below_rain_thold, uint8_t, logical, 1, 1),
       SET_BLOCK(rspp_variant_A::RSPP_Detection_T, raw.prob_1stazhypo, float, single, 1, 1),
   };

   BLOCK_T Det_Props_Block[] = {
       SET_BLOCK(F360_Detection_Props_T, vcs_position.x, float, single, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, vcs_position.y, float, single, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, cluster_id, int16_t, int16, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, object_track_id, int32_t, int32, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, wheel_spin_type, uint8_t, DET_WHEELSPIN_TYPE, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, range_rate_dealiased, float, single, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, range_dealiased, float, single, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, range_rate_compensated, float, single, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, range_rate_predicted, float, single, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, f_dealiased, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, f_double_bounce, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, f_FOV_edge, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, f_rr_inlier, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, f_used_in_rr_msmt_update, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, f_close_target, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, f_inside_gate, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, f_inside_mov_gate, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, f_ok_to_use, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, f_det_pair, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, f_potential_angle_jump, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, f_object_based_angle_jump, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, f_water_spray, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, f_trailer_related_det, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, f_stationary_bounce, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, f_azimuth_rdot_outlier, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, behind_sep_id, uint8_t, uint8, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, on_sep_id, uint8_t, uint8, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, f_low_az_conf_det, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, f_angle_amb, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, f_nd_target, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, f_unreliable_in_clutter, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Props_T, f_has_close_det_with_similar_rr, uint8_t, logical, 1, 1),
   };

   BLOCK_T Hist_Det_Block[] = {
       SET_BLOCK(F360_Detection_Hist_Data_T, vcs_position_x, float, single, 1, 1),
       SET_BLOCK(F360_Detection_Hist_Data_T, vcs_position_y, float, single, 1, 1),
       SET_BLOCK(F360_Detection_Hist_Data_T, rdot, float, single, 1, 1),
       SET_BLOCK(F360_Detection_Hist_Data_T, rdot_comp, float, single, 1, 1),
       SET_BLOCK(F360_Detection_Hist_Data_T, vcs_az, float, single, 1, 1),
       SET_BLOCK(F360_Detection_Hist_Data_T, time_since_meas, float, single, 1, 1),
       SET_BLOCK(F360_Detection_Hist_Data_T, f_dealiased, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Hist_Data_T, f_FOV_edge, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Hist_Data_T, f_selected, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Hist_Data_T, f_is_range_in_all_looks, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Hist_Data_T, f_potential_angle_jump, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Detection_Hist_Data_T, wheel_spin_type, uint8_t, uint8, 1, 1),
       SET_BLOCK(F360_Detection_Hist_Data_T, motion_status, int8_t, int8, 1, 1),
       SET_BLOCK(F360_Detection_Hist_Data_T, v_wrapping, float, single, 1, 1),
       SET_BLOCK(F360_Detection_Hist_Data_T, r_wrapping, float, single, 1, 1),
       SET_BLOCK(F360_Detection_Hist_Data_T, cluster_idx, int16_t, int16, 1, 1),
   };

   BLOCK_T Hist_Det_Props_Block[] = {
       SET_BLOCK(F360_Detection_Hist_Props_T, f_idx_occupied, uint8_t, logical, 1, MAX_NUMBER_OF_HISTORIC_DETECTIONS),
       SET_BLOCK(F360_Detection_Hist_Props_T, n_occupied, int32_t, int32, 1, 1),
       SET_BLOCK(F360_Detection_Hist_Props_T, max_occupation, int32_t, int32, 1, 1),
   };

   BLOCK_T Cluster_Block[] = {
       SET_BLOCK(F360_Cluster_T, vcs_position_x, float, single, 1, 1),
       SET_BLOCK(F360_Cluster_T, vcs_position_y, float, single, 1, 1),
       SET_BLOCK(F360_Cluster_T, rep_vcs_az, float, single, 1, 1),
       SET_BLOCK(F360_Cluster_T, cos_vcs_az, float, single, 1, 1),
       SET_BLOCK(F360_Cluster_T, sin_vcs_az, float, single, 1, 1),
       SET_BLOCK(F360_Cluster_T, rep_rdotcomp, float, single, 1, 1),
       SET_BLOCK(F360_Cluster_T, id, int16_t, int16, 1, 1),
       SET_BLOCK(F360_Cluster_T, ndets, int16_t, int16, 1, 1),
       SET_BLOCK(F360_Cluster_T, detids, int16_t, int16, 1, MAX_DETS_IN_OBJ_TRK),
       SET_BLOCK(F360_Cluster_T, num_old_dets, int16_t, int16, 1, 1),
       SET_BLOCK(F360_Cluster_T, old_det_idx, int16_t, int16, 1, MAX_HIST_DETS_IN_CLUSTER),
       SET_BLOCK(F360_Cluster_T, num_types_of_dets, int16_t, int16, 1, 2),
       SET_BLOCK(F360_Cluster_T, clutter_counter, int16_t, int16, 1, 1),
       SET_BLOCK(F360_Cluster_T, f_dealiased, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Cluster_T, f_to_be_killed, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Cluster_T, time_since_cluster_updated, float, single, 1, 1),
       SET_BLOCK(F360_Cluster_T, time_since_measurement, float, single, 1, 1),
       SET_BLOCK(F360_Cluster_T, low_rcs_dets_cnt, uint8_t, uint8, 1, 1),
       SET_BLOCK(F360_Cluster_T, stationary_cluster_cnt, uint8_t, uint8, 1, 1),
   };

   BLOCK_T Obj_Trk_Block[] = {
       SET_BLOCK(F360_Object_Track_T, pseudo_vcs_position.x, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, pseudo_vcs_position.y, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, average_grid_search_tcs_position.x, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, average_grid_search_tcs_position.y, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, speed, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, hdg_ptng_disagmt, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, curvature, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, tang_accel, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, vcs_position.x, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, vcs_position.y, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, vcs_velocity.longitudinal, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, vcs_velocity.lateral, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, vcs_accel.longitudinal, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, vcs_accel.lateral, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, vcs_heading, float, single, 1, 1),
       SET_BLOCK_PRIV_MEMBER(F360_Object_Track_T, bbox, "vcs_pointing", 8, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, orientation_std, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, predicted_vcs_position.x, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, predicted_vcs_position.y, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, predicted_vcs_velocity.longitudinal, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, predicted_vcs_velocity.lateral, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, predicted_speed, float, single, 1, 1),
       SET_BLOCK_PRIV_MEMBER(F360_Object_Track_T, bbox, "length", 20, float, single, 1, 1),
       SET_BLOCK_PRIV_MEMBER(F360_Object_Track_T, bbox, "width", 24, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, status, uint8_t, OBJ_STATUS, 1, 1),
       SET_BLOCK(F360_Object_Track_T, occlusion_status, uint8_t, uint8, 1, 1),
       SET_BLOCK(F360_Object_Track_T, time_since_cluster_created, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, time_since_track_updated, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, time_since_split, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, errcov, float, single, STATE_DIMENSION, STATE_DIMENSION),
       SET_BLOCK(F360_Object_Track_T, init_scheme, int8_t, TRK_INIT_TYPE, 1, 1),
       SET_BLOCK(F360_Object_Track_T, ndets, int32_t, int32, 1, 1),
       SET_BLOCK(F360_Object_Track_T, detids, int32_t, int32, MAX_DETS_IN_OBJ_TRK, 1),
       SET_BLOCK(F360_Object_Track_T, num_rr_inlier_dets, int32_t, int32, 1, 1),
       SET_BLOCK(F360_Object_Track_T, num_dets_used_in_rr_msmt_update, int32_t, int32, 1, 1),
       SET_BLOCK(F360_Object_Track_T, slow_moving_cluster_id, int32_t, int32, 1, 1),
       SET_BLOCK(F360_Object_Track_T, num_members_in_slow_moving_obj_cluster, uint32_t, uint32, 1, 1),
       SET_BLOCK(F360_Object_Track_T, length_of_slow_moving_obj_cluster, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, f_moving, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Object_Track_T, f_shrink_fast, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Object_Track_T, f_moveable, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Object_Track_T, f_oncoming, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Object_Track_T, f_hide_occluded_track_behind_host, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Object_Track_T, f_vehicular_trk, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Object_Track_T, mirror_prob, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, filtered_combined_nosep_mirror_prob, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, historic_num_db_dets_with_forgetting_factor, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, num_db_dets, uint8_t, uint8, 1, 1),
       SET_BLOCK(F360_Object_Track_T, movable_prob, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, length_uncertainty, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, width_uncertainty, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, id, int32_t, int32, 1, 1),
       SET_BLOCK(F360_Object_Track_T, unique_id, uint32_t, uint32, 1, 1),
       SET_BLOCK(F360_Object_Track_T, reduced_id, int32_t, int32, 1, 1),
       SET_BLOCK(F360_Object_Track_T, reduced_status, uint8_t, OBJ_STATUS, 1, 1),
       SET_BLOCK(F360_Object_Track_T, cntConsecutiveAmbiguous, int32_t, int32, 1, 1),
       SET_BLOCK(F360_Object_Track_T, cntConsecutiveMoving, int32_t, int32, 1, 1),
       SET_BLOCK(F360_Object_Track_T, cntConsecutiveStopped, int32_t, int32, 1, 1),
       SET_BLOCK(F360_Object_Track_T, cntHostTurnForMirrorProb, int32_t, int32, 1, 1),
       SET_BLOCK(F360_Object_Track_T, raw_confidence_level, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, confidenceLevel, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, prev_avrg_conf_level, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, time_since_stage_start, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, num_types_of_dets, int32_t, int32, 2, 1),
       SET_BLOCK(F360_Object_Track_T, meascov, float, single, 2, 2),
       SET_BLOCK(F360_Object_Track_T, cnt_consecutive_visible_from_rear, int8_t, int8, 1, 1),
       SET_BLOCK(F360_Object_Track_T, long_buffer_zone_len1, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, long_buffer_zone_len2, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, lat_buffer_zone_wid1, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, lat_buffer_zone_wid2, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, f_prevent_orientation_std_decrease, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Object_Track_T, time_since_initialization, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, trk_fltr_type, uint8_t, TRKFLTR, 1, 1),
       SET_BLOCK(F360_Object_Track_T, time_since_last_stop, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, time_since_started_move, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, total_reduced_dets, int32_t, int32, 1, 1),
       SET_BLOCK(F360_Object_Track_T, filtered_dets, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, f_ghost_NU_2_C, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Object_Track_T, f_overlapping_with_object, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Object_Track_T, time_since_measurement, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, priority, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, p_higher_priority_track, int32_t, int32, 1, 1), // Pointer type. Not sure how to handle. Only logging pointer adress currently
       SET_BLOCK(F360_Object_Track_T, p_lower_priority_track, int32_t, int32, 1, 1),  // Pointer type. Not sure how to handle. Only logging pointer adress currently
       SET_BLOCK(F360_Object_Track_T, reference_point, uint8_t, REFERENCE_POINT, 1, 1),
       SET_BLOCK(F360_Object_Track_T, min_projection_reference_point, uint8_t, REFERENCE_POINT, 1, 1),
       SET_BLOCK(F360_Object_Track_T, object_class, uint8_t, uint8, 1, 1),
       SET_BLOCK(F360_Object_Track_T, dead_zone_status, uint8_t, DEAD_ZONE_STATUS, 1, 1),
       SET_BLOCK(F360_Object_Track_T, exist_prob, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, p_track_state, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, probability_pedestrian, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, probability_car, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, probability_bicycle, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, probability_motorcycle, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, probability_truck, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, probability_undet, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, time_since_obj_considered_veh_for_class_freeze, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, heading_rate, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, cca_pnt_filter_cov, float, single, 2, 2),
       SET_BLOCK(F360_Object_Track_T, num_updates_since_init, uint16_t, uint16, 1, 1),
       SET_BLOCK(F360_Object_Track_T, pseudo_pos_cov_outlier_count_orth, int32_t, int32, 1, 1),
       SET_BLOCK(F360_Object_Track_T, number_of_events_of_multipath_with_forgetting_factor, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, time_since_initialization_with_forgetting_factor, float, single, 1, 1),

       // Static Environment Polynomials
       SET_BLOCK(F360_Object_Track_T, behind_sep_id, uint8_t, uint8, 1, 1),
       SET_BLOCK(F360_Object_Track_T, on_sep_id, uint8_t, uint8, 1, 1),
       SET_BLOCK(F360_Object_Track_T, sep_intersection_point.x, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, sep_intersection_point.y, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, f_behind_sep_ambiguous, uint8_t, logical, 1, 1),

       SET_BLOCK(F360_Object_Track_T, conf_longitudinal_position, uint8_t, uint8, 1, 1),
       SET_BLOCK(F360_Object_Track_T, conf_lateral_position, uint8_t, uint8, 1, 1),
       SET_BLOCK(F360_Object_Track_T, conf_speed, uint8_t, uint8, 1, 1),
       SET_BLOCK(F360_Object_Track_T, conf_overall, uint8_t, uint8, 1, 1),
       SET_BLOCK(F360_Object_Track_T, low_rcs_dets_cnt, uint8_t, int32, 1, 1),

       SET_BLOCK(F360_Object_Track_T, orth_delta_filtered, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, orth_gap_filtered, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, filtered_pos_diff_heading, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, orth_range_rate_diff_filtered, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, filtered_mean_tcs_y_pos_of_lower_rr_err_bin, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, filtered_mean_tcs_y_pos_of_higher_rr_err_bin, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, filtered_rr_err_max_gap, float, single, 1, 1),

       SET_BLOCK(F360_Object_Track_T, average_rcs, float, single, 1, 1),

       SET_BLOCK(F360_Object_Track_T, filtered_hist_assoc_det_rr_err_mean, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, filtered_hist_assoc_det_rr_err_var, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, filtered_hist_assoc_n_dets, float, single, 1, 1),

       // Underdrivability
       SET_BLOCK(F360_Object_Track_T, underdrivable_status_ocg, uint8_t, Underdrivable_Status_T, 1, 1),
       SET_BLOCK(F360_Object_Track_T, drivable_status_sg, uint8_t, SG_Drivability_Class_T, 1, 1),
       SET_BLOCK(F360_Object_Track_T, drivable_confidence_sg, uint8_t, uint8, 1, 1),
       SET_BLOCK(F360_Object_Track_T, drivable_sg_dist_to_segment_sq, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, otg_height, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, ud_mov_historic_ndets, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, probability_underdrivable_ocg, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, ud_mov_cnt_underdrivable, int32_t, int32, 1, 1),

       SET_BLOCK(F360_Object_Track_T, cca_cross_moving_buffer, int8_t, int8, 20, 1),
       SET_BLOCK(F360_Object_Track_T, cca_cross_moving_buffer_index, uint8_t, uint8, 1, 1),
       SET_BLOCK(F360_Object_Track_T, prev_predicted_vcs_y_pos, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, pseudo_hdg_state_vec, float, single, 6, 1),
       SET_BLOCK(F360_Object_Track_T, pseudo_hdg, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, assoc_dets_pct_filtered, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, num_dets_in_ext_bbox, uint16_t, uint16, 1, 1),
       SET_BLOCK(F360_Object_Track_T, ud_overdrivable_det_pct, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, idm_det_fraction, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, bbox_center_otg_altitude, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, bbox_height, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, obstacle_prob, float, single, 1, 1),
       SET_BLOCK(F360_Object_Track_T, f_suspectable_for_det_drop, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Object_Track_T, aeb_confidence, uint8_t, uint8, 1, 1),

   };

   BLOCK_T Trkr_Info_Block[] = {
       SET_BLOCK(F360_Tracker_Info_T, time_us, uint64_t, uint64, 1, 1),
       SET_BLOCK(F360_Tracker_Info_T, active_cluster_ids, int16_t, int16, 1, NUMBER_OF_CLUSTERS),
       SET_BLOCK(F360_Tracker_Info_T, inactive_cluster_ids, int16_t, int16, 1, NUMBER_OF_CLUSTERS),
       SET_BLOCK(F360_Tracker_Info_T, num_active_clusters, int16_t, int16, 1, 1),
       SET_BLOCK(F360_Tracker_Info_T, active_obj_ids, int32_t, int32, 1, NUMBER_OF_OBJECT_TRACKS),
       SET_BLOCK(F360_Tracker_Info_T, object_list_timestamp, uint64_t, uint64, 1, 1),
       SET_BLOCK(F360_Tracker_Info_T, inactive_obj_ids, int32_t, int32, 1, NUMBER_OF_OBJECT_TRACKS),
       SET_BLOCK(F360_Tracker_Info_T, num_active_objs, int32_t, int32, 1, 1),
       SET_BLOCK(F360_Tracker_Info_T, num_unique_objs, uint32_t, uint32, 1, 1),
       SET_BLOCK(F360_Tracker_Info_T, reduced_active_obj_ids, int32_t, int32, 1, NUMBER_OF_REDUCED_OBJECT_TRACKS),
       SET_BLOCK(F360_Tracker_Info_T, reduced_inactive_obj_ids, int32_t, int32, 1, NUMBER_OF_REDUCED_OBJECT_TRACKS),
       SET_BLOCK(F360_Tracker_Info_T, reduced_obj_ids, int32_t, int32, 1, NUMBER_OF_REDUCED_OBJECT_TRACKS),
       SET_BLOCK(F360_Tracker_Info_T, reduced_num_active_objs, int32_t, int32, 1, 1),
       SET_BLOCK(F360_Tracker_Info_T, cnt_loops, uint32_t, uint32, 1, 1),
       SET_BLOCK(F360_Tracker_Info_T, elapsed_time_s, float, single, 1, 1),
       SET_BLOCK(F360_Tracker_Info_T, unique_rdot_interval_widths, float, single, 1, MAX_NUM_UNIQUE_RDOT_INTERVAL),
       SET_BLOCK(F360_Tracker_Info_T, num_unique_rdot_interval_widths, int32_t, int32, 1, 1),
       SET_BLOCK(F360_Tracker_Info_T, rdot_interval_compatibility, uint8_t, logical, MAX_NUM_UNIQUE_RDOT_INTERVAL, MAX_NUM_UNIQUE_RDOT_INTERVAL),
       SET_BLOCK(F360_Tracker_Info_T, f_esr_sensor_valid, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Tracker_Info_T, f_srr2_sensor_valid, uint8_t, logical, 1, 1),
       SET_BLOCK(F360_Tracker_Info_T, vcslong_sorted_start, int32_t, int32, 1, 1),                            // Pointer type. Not sure how to handle. Only logging pointer adress currently
       SET_BLOCK(F360_Tracker_Info_T, vcslong_sorted_next_track, int32_t, int32, 1, NUMBER_OF_OBJECT_TRACKS), // Pointer type. Not sure how to handle. Only logging pointer adress currently
       SET_BLOCK(F360_Tracker_Info_T, vcslong_sorted_prev_track, int32_t, int32, 1, NUMBER_OF_OBJECT_TRACKS), // Pointer type. Not sure how to handle. Only logging pointer adress currently
       SET_BLOCK(F360_Tracker_Info_T, vcslong_sorted_cluster_list, int16_t, int16, 1, NUMBER_OF_CLUSTERS),
       SET_BLOCK(F360_Tracker_Info_T, vcslong_sorted_cluster_start, int32_t, int32, 1, 1),                 // Pointer type. Not sure how to handle. Only logging pointer adress currently
       SET_BLOCK(F360_Tracker_Info_T, vcslong_sorted_cluster_next, int32_t, int32, 1, NUMBER_OF_CLUSTERS), // Pointer type. Not sure how to handle. Only logging pointer adress currently
       SET_BLOCK(F360_Tracker_Info_T, vcslong_sorted_cluster_prev, int32_t, int32, 1, NUMBER_OF_CLUSTERS), // Pointer type. Not sure how to handle. Only logging pointer adress currently
       SET_BLOCK(F360_Tracker_Info_T, p_highest_priority_track, int32_t, int32, 1, 1),                     // Pointer type. Not sure how to handle. Only logging pointer adress currently
       SET_BLOCK(F360_Tracker_Info_T, p_lowest_priority_track, int32_t, int32, 1, 1),                      // Pointer type. Not sure how to handle. Only logging pointer adress currently
       SET_BLOCK(F360_Tracker_Info_T, rain_level_filtered, float, single, 1, 1)};

   // Static Environment Polynomials
   BLOCK_T Static_Env_Polys[] =
       {
           SET_BLOCK(Static_Env_Poly_T, status, uint8_t, Static_Env_Poly_Status, 1, 1),
           SET_BLOCK(Static_Env_Poly_T, poly_type, uint8_t, Static_Env_Poly_Type, 1, 1),
           SET_BLOCK(Static_Env_Poly_T, age, uint8_t, uint8, 1, 1),
           SET_BLOCK(Static_Env_Poly_T, confidence, float, single, 1, 1),
           SET_BLOCK(Static_Env_Poly_T, lower_limit, float, single, 1, 1),
           SET_BLOCK(Static_Env_Poly_T, upper_limit, float, single, 1, 1),
           SET_BLOCK(Static_Env_Poly_T, p0, float, single, 1, 1),
           SET_BLOCK(Static_Env_Poly_T, p1, float, single, 1, 1),
           SET_BLOCK(Static_Env_Poly_T, p2, float, single, 1, 1),
       };

   // Trailer Detector
   BLOCK_T Trailer_Detector_Output[] =
       {
           SET_BLOCK(F360_PVTrailer_Data_T, pvtrailer_length.trailer_length, float, single, 1, 1),
           SET_BLOCK(F360_PVTrailer_Data_T, pvtrailer_width.trailer_width, float, single, 1, 1),
           SET_BLOCK(F360_PVTrailer_Data_T, pvtrailer_angle.trailer_angle_rad, float, single, 1, 1),
           SET_BLOCK(F360_PVTrailer_Data_T, pvtrailer_angle.trailer_angle_rate_rad, float, single, 1, 1),
           SET_BLOCK(F360_PVTrailer_Data_T, pvtrailer_length.trailer_HV_gap, float, single, 1, 1),
       };

   // Tracker version
   BLOCK_T Tracker_Version_Block[] =
       {
           SET_BLOCK(Tracker_Version_Xtrk_Log, major, uint8_t, uint8, 1, 1),
           SET_BLOCK(Tracker_Version_Xtrk_Log, minor, uint8_t, uint8, 1, 1),
           SET_BLOCK(Tracker_Version_Xtrk_Log, patch, uint8_t, uint8, 1, 1),
       };

   // Occupancy Grid
   BLOCK_T OCG_Properties[] =
       {
           SET_BLOCK(ocg::OCG_Outputs_T, timestamp, double, double, 1, 1),
           SET_BLOCK(ocg::OCG_Outputs_T, iteration_index, uint32_t, uint32, 1, 1),
           SET_BLOCK(ocg::OCG_Outputs_T, grid_definition.num_cells_x_far, uint16_t, uint16, 1, 1),
           SET_BLOCK(ocg::OCG_Outputs_T, grid_definition.num_cells_x_mid, uint16_t, uint16, 1, 1),
           SET_BLOCK(ocg::OCG_Outputs_T, grid_definition.num_cells_x_close, uint16_t, uint16, 1, 1),
           SET_BLOCK(ocg::OCG_Outputs_T, grid_definition.num_cells_y, uint16_t, uint16, 1, 1),
           SET_BLOCK(ocg::OCG_Outputs_T, grid_definition.cell_length, float, single, 1, 1),
           SET_BLOCK(ocg::OCG_Outputs_T, grid_definition.cell_width, float, single, 1, 1),
           SET_BLOCK(ocg::OCG_Outputs_T, grid_definition.cell_width_extension_factor, float, single, 1, 1),
           SET_BLOCK(ocg::OCG_Outputs_T, underdrivability.grid_curvature, float, single, 1, 1),
           SET_BLOCK(ocg::OCG_Outputs_T, underdrivability.ogcs_host_rear_axle_position.x, float, single, 1, 1),
           SET_BLOCK(ocg::OCG_Outputs_T, underdrivability.ogcs_host_rear_axle_position.y, float, single, 1, 1),
           SET_BLOCK(ocg::OCG_Outputs_T, underdrivability.ogcs_host_rear_axle_position.z, float, single, 1, 1),
           SET_BLOCK(ocg::OCG_Outputs_T, underdrivability.ogcs_host_rear_axle_position.yaw, float, single, 1, 1),
       };

   // Occupancy Grid Underdrivability
   BLOCK_T OCG_Cell_Classification[] =
       {
           SET_BLOCK(ocg::OCG_Cell_Classification, underdrivability_status, int32_t, int32, 1, 1),
           SET_BLOCK(ocg::OCG_Cell_Classification, probs, float, single, ocg::UNDERDRIVABLE_STATUS_TOTAL, 1),
       };

   // Stationary geometries
   BLOCK_T SG_Properties[] =
       {
           SET_BLOCK(sg::SG_Output_T, execution_timestamp_us, uint64_t, uint64, 1, 1),
           SET_BLOCK(sg::SG_Output_T, cycle_index, uint32_t, uint32, 1, 1),
           SET_BLOCK(sg::SG_Output_T, num_contours, uint16_t, uint16, 1, 1),
           SET_BLOCK(sg::SG_Output_T, f_valid, uint8_t, logical, 1, 1)

       };

   // Stationary geometries Vertices
   BLOCK_T SG_Vertex_Out[] =
       {
           SET_BLOCK(sg::SG_Vertex_Out_T, position_x, float, single, 1, 1),
           SET_BLOCK(sg::SG_Vertex_Out_T, position_y, float, single, 1, 1),
           SET_BLOCK(sg::SG_Vertex_Out_T, position_variance_x, float, single, 1, 1),
           SET_BLOCK(sg::SG_Vertex_Out_T, position_variance_y, float, single, 1, 1),
           SET_BLOCK(sg::SG_Vertex_Out_T, position_covariance_xy, float, single, 1, 1),
           SET_BLOCK(sg::SG_Vertex_Out_T, drivability, uint8_t, SG_Drivability_Class_T, 1, 1),
           SET_BLOCK(sg::SG_Vertex_Out_T, drivability_confidence, uint8_t, uint8, 1, 1),
       };

   // Stationary geometries Contours
   BLOCK_T SG_Contour_Out[] =
       {
           SET_BLOCK(sg::SG_Contour_Out_T, unique_id, uint32_t, uint32, 1, 1),
           SET_BLOCK(sg::SG_Contour_Out_T, num_vertices, uint16_t, uint16, 1, 1),
           SET_BLOCK(sg::SG_Contour_Out_T, type, uint8_t, SG_Contour_Type_T, 1, 1),
       };

#ifdef XTRKLOG_ADDITIONAL_INIT_DATA
   BLOCK_T Init_Debug_Block[] =
       {

           SET_BLOCK(F360_Initialization_Debug_Data_T, n_obj, int32_t, int32, 1, 1),
           SET_BLOCK(F360_Initialization_Debug_Data_T, trackID, int32_t, int32, 1, 20),
           SET_BLOCK(F360_Initialization_Debug_Data_T, longvel_by_cloud, float, single, 1, 20),
           SET_BLOCK(F360_Initialization_Debug_Data_T, latvel_by_cloud, float, single, 1, 20),
           SET_BLOCK(F360_Initialization_Debug_Data_T, longvel_by_posdiff, float, single, 1, 20),
           SET_BLOCK(F360_Initialization_Debug_Data_T, latvel_by_posdiff, float, single, 1, 20),
           SET_BLOCK(F360_Initialization_Debug_Data_T, n_det, int32_t, int32, 1, 20),
           SET_BLOCK(F360_Initialization_Debug_Data_T, n_old_det, int32_t, int32, 1, 20),
           SET_BLOCK(F360_Initialization_Debug_Data_T, det_id, int32_t, int32, MAX_DETS_IN_OBJ_TRK, 20),
           SET_BLOCK(F360_Initialization_Debug_Data_T, old_det_id, int32_t, int32, MAX_HIST_DETS_IN_CLUSTER, 20),
       };

   BLOCK_T Init_Cluster_Debug_Block[] =
       {
           SET_BLOCK(F360_Initialization_Cluster_Debug_Data_T, f_prioritized, uint8_t, logical, 1, NUMBER_OF_CLUSTERS),
           SET_BLOCK(F360_Initialization_Cluster_Debug_Data_T, occlusion_status, uint8_t, uint8, 1, NUMBER_OF_CLUSTERS),
           SET_BLOCK(F360_Initialization_Cluster_Debug_Data_T, cloud_confidence, uint8_t, uint8, 1, NUMBER_OF_CLUSTERS),
           SET_BLOCK(F360_Initialization_Cluster_Debug_Data_T, posdiff_confidence, uint8_t, uint8, 1, NUMBER_OF_CLUSTERS),
           SET_BLOCK(F360_Initialization_Cluster_Debug_Data_T, f_ambiguous_motion_in_clutter, uint8_t, logical, 1, NUMBER_OF_CLUSTERS),
           SET_BLOCK(F360_Initialization_Cluster_Debug_Data_T, init_type, int8_t, int8, 1, NUMBER_OF_CLUSTERS),
           SET_BLOCK(F360_Initialization_Cluster_Debug_Data_T, longvel_by_cloud, float, single, 1, NUMBER_OF_CLUSTERS),
           SET_BLOCK(F360_Initialization_Cluster_Debug_Data_T, latvel_by_cloud, float, single, 1, NUMBER_OF_CLUSTERS),
           SET_BLOCK(F360_Initialization_Cluster_Debug_Data_T, longvel_by_posdiff, float, single, 1, NUMBER_OF_CLUSTERS),
           SET_BLOCK(F360_Initialization_Cluster_Debug_Data_T, latvel_by_posdiff, float, single, 1, NUMBER_OF_CLUSTERS),
           SET_BLOCK(F360_Initialization_Cluster_Debug_Data_T, num_dets, int16_t, int16, 1, NUMBER_OF_CLUSTERS),
           SET_BLOCK(F360_Initialization_Cluster_Debug_Data_T, det_vcs_posx, float, single, MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER, NUMBER_OF_CLUSTERS),
           SET_BLOCK(F360_Initialization_Cluster_Debug_Data_T, det_vcs_posy, float, single, MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER, NUMBER_OF_CLUSTERS),
           SET_BLOCK(F360_Initialization_Cluster_Debug_Data_T, det_vcs_az, float, single, MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER, NUMBER_OF_CLUSTERS),
           SET_BLOCK(F360_Initialization_Cluster_Debug_Data_T, det_rr_comp, float, single, MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER, NUMBER_OF_CLUSTERS),
           SET_BLOCK(F360_Initialization_Cluster_Debug_Data_T, det_time_since_meas, float, single, MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER, NUMBER_OF_CLUSTERS),
       };
#endif

   // ====== SUPER BLOCK ===========
   SUPER_BLOCK_T SupBlocks[] = {
       SET_SUP_BLOCK(Sensor_Block, sensors, MAX_NUMBER_OF_SENSORS),
       SET_SUP_BLOCK(Sensor_Props_Block, sensor_props, MAX_NUMBER_OF_SENSORS),
       SET_SUP_BLOCK(Sensor_Props_Int_Ref_Buf_Block, sens_prop_int_ref_buf, MAX_NUMBER_OF_SENSORS),
       SET_SUP_BLOCK(Static_Host_Block, host_calib, 1),
       SET_SUP_BLOCK(Raw_Host_Block, host_raw, 1),
       SET_SUP_BLOCK(Host_Block, host, 1),
       SET_SUP_BLOCK(Host_Props_Block, host_props, 1),
       SET_SUP_BLOCK(Raw_Detection_Block, raw_dets, MAX_NUMBER_OF_DETECTIONS),
       SET_SUP_BLOCK(Det_Props_Block, det_props, MAX_NUMBER_OF_DETECTIONS),
       SET_SUP_BLOCK(Hist_Det_Block, hist_det, MAX_NUMBER_OF_HISTORIC_DETECTIONS),
       SET_SUP_BLOCK(Hist_Det_Props_Block, hist_det_props, 1),
       SET_SUP_BLOCK(Cluster_Block, clusters, NUMBER_OF_CLUSTERS),
       SET_SUP_BLOCK(Obj_Trk_Block, object_track, NUMBER_OF_OBJECT_TRACKS),
       SET_SUP_BLOCK(Trkr_Info_Block, trkrInfo, 1),
       SET_SUP_BLOCK(Static_Env_Polys, static_env_polys, F360_NUM_OF_STATIC_ENV_POLYS),
       SET_SUP_BLOCK(Trailer_Detector_Output, trailer_detector_output, 1),
       SET_SUP_BLOCK(Tracker_Version_Block, tracker_version, 1),
       SET_SUP_BLOCK(OCG_Properties, ocg_properties, 1),
       SET_SUP_BLOCK(OCG_Cell_Classification, ocg_cell_classification, ocg::NUM_CELLS_X * ocg::NUM_CELLS_Y),
       SET_SUP_BLOCK(SG_Properties, sg_properties, 1),
       SET_SUP_BLOCK(SG_Vertex_Out, sg_vertex_out, SG_MAX_NUM_OUTPUT_VERTICES),
       SET_SUP_BLOCK(SG_Contour_Out, sg_contour_out, SG_MAX_NUM_OUTPUT_CONTOURS),
#ifdef XTRKLOG_ADDITIONAL_INIT_DATA
       SET_SUP_BLOCK(Init_Debug_Block, initialization_debug, 1),
       SET_SUP_BLOCK(Init_Cluster_Debug_Block, initialization_cluster_debug, 1),
#endif
   };

   // Verify the SupBlocks by checking that the sizes are correct. Print an error message if they are not correct
   for (uint8_t iSup = 0; iSup < _countof(SupBlocks); iSup++) {
      int expectedSize = SupBlocks[iSup].block->szType;

      // Sensor_props' internal reflection buffer is logged through a separate structure
      if (std::string(SupBlocks[iSup].info.name) == "sensor_props") {
         expectedSize -= sizeof(Internal_Reflection_Buffer_Xtrklog);
      }

      int actualSize     = 0;
      int curAlign       = 0;
      int CUR_PACK_BYTES = 4; // The data will be aligned based on the biggest type. Start with 4 (normal case)
      for (int iBlk = 0; iBlk < SupBlocks[iSup].info.numBlocks; iBlk++) {
         // Update size with the data size
         const int curNumBytes = SupBlocks[iSup].block[iBlk].numBytes;
         const int curSize     = curNumBytes * SupBlocks[iSup].block[iBlk].cols * SupBlocks[iSup].block[iBlk].rows;
         actualSize += curSize;

         // Check if some padding needs to be added
         CUR_PACK_BYTES = max(CUR_PACK_BYTES, SupBlocks[iSup].block[iBlk].numBytes);

         // Add padding to the size dependent on the struct member types
         const bool f_initial_padding = (0 < curAlign) && (curAlign < curNumBytes);
         if (f_initial_padding) {
            if ((curNumBytes + curSize) > CUR_PACK_BYTES) {
               // Pad the struct to match the packing
               actualSize += (CUR_PACK_BYTES - curAlign);
               curAlign = 0;
            } else {
               // Pad struct size to match the current struct type
               actualSize += (curNumBytes - curAlign);
               curAlign = (curNumBytes % CUR_PACK_BYTES);
            }
         }

         if (curAlign + curSize < CUR_PACK_BYTES) {
            curAlign += curSize;
         } else if (curAlign + curSize == CUR_PACK_BYTES) {
            curAlign = 0;
         } else {
            curAlign = (curSize % CUR_PACK_BYTES);
         }
      }

      actualSize += (actualSize % CUR_PACK_BYTES) > 0 ? CUR_PACK_BYTES - (actualSize % CUR_PACK_BYTES) : 0;

      bool f_size_match = (expectedSize == actualSize);
      if (!f_size_match) {
         printf("xtrkLog: init_xtrk_log: xtrk data is not containing all fields in struct - %s - This may be intended...\n", SupBlocks[iSup].info.name);
      }
   }

   return write_xtrk_descriptor(SupBlocks, _countof(SupBlocks));
}

int write_xtrk_log(
    const F360_Host_Calib_T *const host_calib,
    const F360_Host_Raw_T *const host_raw,
    const F360_Host_T *const host,
    const rspp_variant_A::RSPP_Detection_List_T *const raw_detect_list,
    const F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS], // );
    const F360_Radar_Sensor_Props_T (&sensor_props)[MAX_NUMBER_OF_SENSORS],
    const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
    const F360_Detection_Hist_T &det_hist,
    const F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
    const F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
    const F360_Tracker_Info_T &tracker_info,
    const F360_Host_Props_T &host_props,
    const Static_Env_Poly_T (&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
    const F360_PVTrailer_Data_T &pvtrailer_data,
    const ocg::OCG_Outputs_T *occupancy_grid,
    const sg::SG_Output_T *sg_output) {
   // Returns non-zero (true) if a write error occurs
   uint32_t error = 0;
   if (NULL != fp_xtrk) {
      static Internal_Reflection_Buffer_Xtrklog sens_prop_int_ref_buf[MAX_NUMBER_OF_SENSORS];

      // Special handling of sensor_props's internal reflection buffers
      for (uint32_t i_sens = 0; i_sens < MAX_NUMBER_OF_SENSORS; i_sens++) {
         for (uint32_t i_buf = 0; i_buf < INTERNAL_REFLECTIONS_BUFFER_SIZE; i_buf++) {
            sens_prop_int_ref_buf[i_sens].range[i_buf]                               = sensor_props[i_sens].internal_reflections_buffer[i_buf].range;
            sens_prop_int_ref_buf[i_sens].azimuth[i_buf]                             = sensor_props[i_sens].internal_reflections_buffer[i_buf].azimuth;
            sens_prop_int_ref_buf[i_sens].rcs[i_buf]                                 = sensor_props[i_sens].internal_reflections_buffer[i_buf].rcs;
            sens_prop_int_ref_buf[i_sens].occurrence_count[i_buf]                    = sensor_props[i_sens].internal_reflections_buffer[i_buf].occurrence_count;
            sens_prop_int_ref_buf[i_sens].age[i_buf]                                 = sensor_props[i_sens].internal_reflections_buffer[i_buf].age;
            sens_prop_int_ref_buf[i_sens].f_updated_this_cycle[i_buf]                = sensor_props[i_sens].internal_reflections_buffer[i_buf].f_updated_this_cycle;
            sens_prop_int_ref_buf[i_sens].f_classified_as_internal_reflection[i_buf] = sensor_props[i_sens].internal_reflections_buffer[i_buf].f_classified_as_internal_reflection;
         }
      }

      uint16_t k;
      uint16_t k1;
      size_t cnt      = 0;
      int numSupBlock = 0;

      // Sensor_Block
      for (k = 0; k < MAX_NUMBER_OF_SENSORS; k++) {
         cnt += sizeof(F360_Radar_Sensor_T) * fwrite(&sensors[k], sizeof(F360_Radar_Sensor_T), 1, fp_xtrk);
      }
      error += (XTRK_RECORD_SIZE_CUMSUM[numSupBlock++] == cnt) ? 0 : 1;

      // Sensor_Props_Block
      for (k = 0; k < MAX_NUMBER_OF_SENSORS; k++) {
         cnt += sizeof(F360_Radar_Sensor_Props_T) * fwrite(&sensor_props[k], sizeof(F360_Radar_Sensor_Props_T), 1, fp_xtrk);
      }
      error += (XTRK_RECORD_SIZE_CUMSUM[numSupBlock++] == cnt) ? 0 : 1;

      // Sensor_Props_Int_Ref_Buf_Block
      for (k = 0; k < MAX_NUMBER_OF_SENSORS; k++) {
         cnt += sizeof(Internal_Reflection_Buffer_Xtrklog) * fwrite(&sens_prop_int_ref_buf[k], sizeof(Internal_Reflection_Buffer_Xtrklog), 1, fp_xtrk);
      }
      error += (XTRK_RECORD_SIZE_CUMSUM[numSupBlock++] == cnt) ? 0 : 1;

      // Static_Host_Block
      cnt += sizeof(F360_Host_Calib_T) * fwrite(host_calib, sizeof(F360_Host_Calib_T), 1, fp_xtrk);
      error += (XTRK_RECORD_SIZE_CUMSUM[numSupBlock++] == cnt) ? 0 : 1;

      // Raw_Host_Block
      cnt += sizeof(F360_Host_Raw_T) * fwrite(host_raw, sizeof(F360_Host_Raw_T), 1, fp_xtrk);
      error += (XTRK_RECORD_SIZE_CUMSUM[numSupBlock++] == cnt) ? 0 : 1;

      // Host_Block
      cnt += sizeof(F360_Host_T) * fwrite(host, sizeof(F360_Host_T), 1, fp_xtrk);
      error += (XTRK_RECORD_SIZE_CUMSUM[numSupBlock++] == cnt) ? 0 : 1;

      // Host_Props_Block
      cnt += sizeof(F360_Host_Props_T) * fwrite(&host_props, sizeof(F360_Host_Props_T), 1, fp_xtrk);
      error += (XTRK_RECORD_SIZE_CUMSUM[numSupBlock++] == cnt) ? 0 : 1;

      // Raw_Detection_Block
      for (k = 0; k < MAX_NUMBER_OF_DETECTIONS; k++) {
         cnt += sizeof(rspp_variant_A::RSPP_Detection_T) * fwrite(&raw_detect_list->detections[k], sizeof(rspp_variant_A::RSPP_Detection_T), 1, fp_xtrk);
      }
      error += (XTRK_RECORD_SIZE_CUMSUM[numSupBlock++] == cnt) ? 0 : 1;

      // Det_Props_Block
      for (k = 0; k < MAX_NUMBER_OF_DETECTIONS; k++) {
         cnt += sizeof(F360_Detection_Props_T) * fwrite(&det_props[k], sizeof(F360_Detection_Props_T), 1, fp_xtrk);
      }
      error += (XTRK_RECORD_SIZE_CUMSUM[numSupBlock++] == cnt) ? 0 : 1;

      // Hist_Det_Block + Hist_Det_Props_Block
      numSupBlock++; // Increment two times since both Hist_Det_Block + Hist_Det_Props_Block is logged here
      cnt += sizeof(F360_Detection_Hist_T) * fwrite(&(det_hist), sizeof(F360_Detection_Hist_T), 1, fp_xtrk);
      error += (XTRK_RECORD_SIZE_CUMSUM[numSupBlock++] == cnt) ? 0 : 1;

      // Cluster_Block
      for (k = 0; k < NUMBER_OF_CLUSTERS; k++) {
         cnt += sizeof(F360_Cluster_T) * fwrite(&clusters[k], sizeof(F360_Cluster_T), 1, fp_xtrk);
      }
      error += (XTRK_RECORD_SIZE_CUMSUM[numSupBlock++] == cnt) ? 0 : 1;

      // Obj_Trk_Block
      for (k = 0; k < NUMBER_OF_OBJECT_TRACKS; k++) {
         cnt += sizeof(F360_Object_Track_T) * fwrite(&object_tracks[k], sizeof(F360_Object_Track_T), 1, fp_xtrk);
      }
      error += (XTRK_RECORD_SIZE_CUMSUM[numSupBlock++] == cnt) ? 0 : 1;

      // Trkr_Info_Block
      cnt += sizeof(F360_Tracker_Info_T) * fwrite(&tracker_info, sizeof(F360_Tracker_Info_T), 1, fp_xtrk);
      error += (XTRK_RECORD_SIZE_CUMSUM[numSupBlock++] == cnt) ? 0 : 1;

      // Longi_Stat_Curves_Block
      for (k = 0; k < F360_NUM_OF_STATIC_ENV_POLYS; k++) {
         cnt += sizeof(Static_Env_Poly_T) * fwrite(&static_env_polys[k], sizeof(Static_Env_Poly_T), 1, fp_xtrk);
      }
      error += (XTRK_RECORD_SIZE_CUMSUM[numSupBlock++] == cnt) ? 0 : 1;

      // Trailer_Detector_Block - DISABLE FOR NOW
      cnt += sizeof(F360_PVTrailer_Data_T) * fwrite(&pvtrailer_data, sizeof(F360_PVTrailer_Data_T), 1, fp_xtrk);
      error += (XTRK_RECORD_SIZE_CUMSUM[numSupBlock++] == cnt) ? 0 : 1;

      // Tracker_Version_Block
      Tracker_Version_Xtrk_Log tracker_version_xtrk{Tracker_Version_Major, Tracker_Version_Minor, Tracker_Version_Patch, 0U};
      cnt += sizeof(Tracker_Version_Xtrk_Log) * fwrite(&tracker_version_xtrk, sizeof(Tracker_Version_Xtrk_Log), 1, fp_xtrk);
      error += (XTRK_RECORD_SIZE_CUMSUM[numSupBlock++] == cnt) ? 0 : 1;

      // Occupancy_Grid_Block
      if (NULL != occupancy_grid) {
         cnt += sizeof(ocg::OCG_Outputs_T) * fwrite(occupancy_grid, sizeof(ocg::OCG_Outputs_T), 1, fp_xtrk);
      } else {
         const ocg::OCG_Outputs_T zero_ocg{};
         cnt += sizeof(ocg::OCG_Outputs_T) * fwrite(&zero_ocg, sizeof(ocg::OCG_Outputs_T), 1, fp_xtrk);
      }
      error += (XTRK_RECORD_SIZE_CUMSUM[numSupBlock++] == cnt) ? 0 : 1;

      // Occupancy_Grid_Underdrivability
      if (NULL != occupancy_grid) {
         for (k = 0; k < ocg::NUM_CELLS_X; k++)
            for (k1 = 0; k1 < ocg::NUM_CELLS_Y; k1++) {
               cnt += sizeof(ocg::OCG_Cell_Classification) * fwrite(&occupancy_grid->underdrivability.underdrivability_classification[k][k1], sizeof(ocg::OCG_Cell_Classification), 1, fp_xtrk);
            }
      } else {
         for (k = 0; k < ocg::NUM_CELLS_X; k++)
            for (k1 = 0; k1 < ocg::NUM_CELLS_Y; k1++) {
               const ocg::OCG_Cell_Classification zero_cell{};
               cnt += sizeof(ocg::OCG_Cell_Classification) * fwrite(&zero_cell, sizeof(ocg::OCG_Cell_Classification), 1, fp_xtrk);
            }
      }
      error += (XTRK_RECORD_SIZE_CUMSUM[numSupBlock++] == cnt) ? 0 : 1;

      // Sg_block
      if (NULL != sg_output) {
         cnt += sizeof(sg::SG_Output_T) * fwrite(sg_output, sizeof(sg::SG_Output_T), 1, fp_xtrk);
      } else {
         const sg::SG_Output_T zero_sg{};
         cnt += sizeof(sg::SG_Output_T) * fwrite(&zero_sg, sizeof(sg::SG_Output_T), 1, fp_xtrk);
      }
      error += (XTRK_RECORD_SIZE_CUMSUM[numSupBlock++] == cnt) ? 0 : 1;

      // sg vertices
      if (NULL != sg_output) {
         for (k = 0; k < SG_MAX_NUM_OUTPUT_VERTICES; k++) {
            cnt += sizeof(sg::SG_Vertex_Out_T) * fwrite(&sg_output->vertices[k], sizeof(sg::SG_Vertex_Out_T), 1, fp_xtrk);
         }
      } else {
         for (k = 0; k < SG_MAX_NUM_OUTPUT_VERTICES; k++) {
            const sg::SG_Vertex_Out_T zero_vertex{};
            cnt += sizeof(sg::SG_Vertex_Out_T) * fwrite(&zero_vertex, sizeof(sg::SG_Vertex_Out_T), 1, fp_xtrk);
         }
      }
      error += (XTRK_RECORD_SIZE_CUMSUM[numSupBlock++] == cnt) ? 0 : 1;

      // sg contours
      if (NULL != sg_output) {
         for (k = 0; k < SG_MAX_NUM_OUTPUT_CONTOURS; k++) {
            cnt += sizeof(sg::SG_Contour_Out_T) * fwrite(&sg_output->contours[k], sizeof(sg::SG_Contour_Out_T), 1, fp_xtrk);
         }
      } else {
         for (k = 0; k < SG_MAX_NUM_OUTPUT_CONTOURS; k++) {
            const sg::SG_Contour_Out_T zero_contour{};
            cnt += sizeof(sg::SG_Contour_Out_T) * fwrite(&zero_contour, sizeof(sg::SG_Contour_Out_T), 1, fp_xtrk);
         }
      }
      error += (XTRK_RECORD_SIZE_CUMSUM[numSupBlock++] == cnt) ? 0 : 1;

#ifdef XTRKLOG_ADDITIONAL_INIT_DATA
      cnt += sizeof(F360_Initialization_Debug_Data_T) * fwrite(&init_debug_data, sizeof(F360_Initialization_Debug_Data_T), 1, fp_xtrk);
      Clear_Init_Debug_Data();
      error += (XTRK_RECORD_SIZE_CUMSUM[numSupBlock++] == cnt) ? 0 : 1;

      cnt += sizeof(F360_Initialization_Cluster_Debug_Data_T) * fwrite(&init_cluster_debug_data, sizeof(F360_Initialization_Cluster_Debug_Data_T), 1, fp_xtrk);
      Clear_Init_Cluster_Debug_Data();
      error += (XTRK_RECORD_SIZE_CUMSUM[numSupBlock++] == cnt) ? 0 : 1;
#endif

      // Check if the number of records match
      error += (XTRK_NUM_RECORDS == numSupBlock) ? 0 : 1;

      if (0 < error) {
         printf("XTRK: write_xtrk_log: The xtrk record is not matching the descriptor. The data will be corrupt\n");
      }
   } else {
      error = 1;
   }

   return error;
}

void Clear_Init_Debug_Data() {
   (void)memset(&init_debug_data, 0, sizeof(init_debug_data));
}

void Clear_Init_Cluster_Debug_Data() {
   (void)memset(&init_cluster_debug_data, 0, sizeof(init_cluster_debug_data));
}

void Prepare_Init_Debug_Data(
    const int32_t track_id,
    const float longvel_by_cloud,
    const float latvel_by_cloud,
    const float longvel_by_posdiff,
    const float latvel_by_posdiff,
    const F360_Cluster_T &cluster) {
   const int32_t n = init_debug_data.n_obj;
   if (n < 20) {
      init_debug_data.trackID[n]            = track_id;
      init_debug_data.longvel_by_cloud[n]   = longvel_by_cloud;
      init_debug_data.latvel_by_cloud[n]    = latvel_by_cloud;
      init_debug_data.longvel_by_posdiff[n] = longvel_by_posdiff;
      init_debug_data.latvel_by_posdiff[n]  = latvel_by_posdiff;
      init_debug_data.n_det[n]              = cluster.ndets;
      init_debug_data.n_old_det[n]          = cluster.num_old_dets;

      for (int32_t i = 0; i < cluster.ndets; i++) {
         init_debug_data.det_id[n][i] = cluster.detids[i];
      }
      for (int32_t i = 0; i < cluster.num_old_dets; i++) {
         init_debug_data.old_det_id[n][i] = static_cast<int32_t>(cluster.old_det_idx[i]) + 1;
      }
      init_debug_data.n_obj++;
   }
}

void Prepare_Init_Cluster_Debug_Data(
    const F360_Cluster_T &cluster,
    const rspp_variant_A::RSPP_Detection_List_T &raw_dets,
    const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
    const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
    const F360_Detection_Hist_T &det_hist,
    const F360_Occlusion_Status_T occlusion_status,
    const CONF3_T cloud_confidence,
    const CONF3_T posdiff_confidence,
    const bool f_ambiguous_motion_in_clutter,
    const F360_Track_Init_T init_type,
    const float32_t longvel_by_cloud,
    const float32_t latvel_by_cloud,
    const float32_t longvel_by_posdiff,
    const float32_t latvel_by_posdiff) {
   const int16_t cluster_idx                                          = cluster.id - 1;
   init_cluster_debug_data.f_prioritized[cluster_idx]                 = true;
   init_cluster_debug_data.occlusion_status[cluster_idx]              = static_cast<uint8_t>(occlusion_status);
   init_cluster_debug_data.cloud_confidence[cluster_idx]              = static_cast<uint8_t>(cloud_confidence);
   init_cluster_debug_data.posdiff_confidence[cluster_idx]            = static_cast<uint8_t>(posdiff_confidence);
   init_cluster_debug_data.f_ambiguous_motion_in_clutter[cluster_idx] = f_ambiguous_motion_in_clutter;
   init_cluster_debug_data.init_type[cluster_idx]                     = static_cast<int8_t>(init_type);
   init_cluster_debug_data.longvel_by_cloud[cluster_idx]              = longvel_by_cloud;
   init_cluster_debug_data.latvel_by_cloud[cluster_idx]               = latvel_by_cloud;
   init_cluster_debug_data.longvel_by_posdiff[cluster_idx]            = longvel_by_posdiff;
   init_cluster_debug_data.latvel_by_posdiff[cluster_idx]             = latvel_by_posdiff;
   init_cluster_debug_data.num_dets[cluster_idx]                      = cluster.ndets + cluster.num_old_dets;

   for (int16_t det_i = 0; det_i < cluster.ndets; det_i++) {
      const int16_t det_idx                                           = cluster.detids[det_i] - 1;
      init_cluster_debug_data.det_vcs_posx[cluster_idx][det_i]        = det_props[det_idx].vcs_position.x;
      init_cluster_debug_data.det_vcs_posy[cluster_idx][det_i]        = det_props[det_idx].vcs_position.y;
      init_cluster_debug_data.det_vcs_az[cluster_idx][det_i]          = raw_dets.detections[det_idx].processed.vcs_az;
      init_cluster_debug_data.det_rr_comp[cluster_idx][det_i]         = det_props[det_idx].range_rate_compensated;
      const int32_t sens_idx                                          = raw_dets.detections[det_idx].raw.sensor_id - 1;
      init_cluster_debug_data.det_time_since_meas[cluster_idx][det_i] = sensors[sens_idx].refined.time_since_measurement_s;
   }
   for (int16_t det_i = 0; det_i < cluster.num_old_dets; det_i++) {
      const int16_t det_idx                                               = cluster.old_det_idx[det_i];
      const int16_t det_arr_i                                             = det_i + cluster.ndets;
      init_cluster_debug_data.det_vcs_posx[cluster_idx][det_arr_i]        = det_hist.det_data[det_idx].vcs_position_x;
      init_cluster_debug_data.det_vcs_posy[cluster_idx][det_arr_i]        = det_hist.det_data[det_idx].vcs_position_y;
      init_cluster_debug_data.det_vcs_az[cluster_idx][det_arr_i]          = det_hist.det_data[det_idx].vcs_az;
      init_cluster_debug_data.det_rr_comp[cluster_idx][det_arr_i]         = det_hist.det_data[det_idx].rdot_comp;
      init_cluster_debug_data.det_time_since_meas[cluster_idx][det_arr_i] = det_hist.det_data[det_idx].time_since_meas;
   }
}

} // namespace f360_variant_A
#endif
