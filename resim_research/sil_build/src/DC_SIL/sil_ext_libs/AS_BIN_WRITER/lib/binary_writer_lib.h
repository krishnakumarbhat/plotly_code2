/* -*- C++ -*-
 *  Project page is http://pep.usinkok.northamerica.delphiauto.net/projectdb/public/?page=project-tasks&pid=2733.5882
 *  Jira Page is http://jiraprod1.delphiauto.net:8080/projects/AUM
 * Delphi Delco Electronics Proprietary
 *
 * Copyright 2016 by Delphi Corporation. All Rights Reserved.
 * This file contains Delphi Delco Electonics Proprietary information.
 * It may not be reproduced or distributed without permission.
 * @uthor: Uri Iurgel
 *
 * @file
 * Library for writing binary files fror ASGUI
 * AS = Active Safety
 * Based on Kai Niederhagen's debug_additional code, and ideas from binary_writer.cpp from TCK, Audi SRR3 PDP project
 */

#pragma once
#ifndef BIN_OUTPUT_H
#define BIN_OUTPUT_H

#include <stdlib.h>
#include <stdio.h>
#include "binary_writer_lib_sizes.h"

#ifdef __cplusplus

#include <vector>
#include <cstring>

typedef enum {
   AS_BIN_WRITER_ERROR_NO_ERROR  = 0,
   AS_BIN_WRITER_ERROR_DISK_FULL = 1
} AS_BIN_WRITER_ERROR_T;

#include <string>
#include <unordered_map>

namespace AS_bin_writer
{
typedef enum {
   RSDS_MOUNTING_POSITION_REAR_LEFT   = 0,
   RSDS_MOUNTING_POSITION_REAR_RIGHT  = 1,
   RSDS_MOUNTING_POSITION_FRONT_RIGHT = 2,
   RSDS_MOUNTING_POSITION_FRONT_LEFT  = 3,
   RSDS_MOUNTING_POSITION_UNDEF       = 255
} RSDS_MOUNTING_POSITION_T;
} // namespace AS_bin_writer

#ifndef NAN
static const unsigned long __nan[2] = {0xffffffff, 0x7fffffff};
#define NAN (*(const float *)__nan)
#endif

/**
 * Copies max dest_size bytes from source to dest.
 * Issues a warning if source is too big.
 */
void safe_string_copy(
    char *dest,        /**< Destination to copy to */
    size_t dest_size,  /**< Size of dest buffer*/
    const char *source /**< Source to copy from */
);

class AS_bin_writer_lib {
 private:
   static const int _OUTPUT_PRECISION = 8;
#if _OUTPUT_PRECISION == (4)
#define FILE_EXT      ("bin32")
#define OUTPUT_FORMAT float
#else
#define FILE_EXT      ("bin")
#define OUTPUT_FORMAT double
#endif

   typedef OUTPUT_FORMAT output_format;
   /* structured data store map*/

   /* string => value*/
   // next evolution: make value array, size dynamic (real_number_sensors), depending on data type
   typedef struct DEBUG_VALUE_FLT {
      char name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN];
      OUTPUT_FORMAT value;
   } DEBUG_VALUE_FLT_T;

   /* map: string => array (with size of array)*/
   // next evolution: add dimension to value array, size dynamic (real_number_sensors), depending on data type
   typedef struct DEBUG_ARRAY_FLT {
      int array_last_entry;
      char name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN];
      OUTPUT_FORMAT values[AS_BIN_WRITER_MAX_ARRAY_SIZE];
   } DEBUG_ARRAY_FLT_T;

   typedef struct DEBUG_DETECTION_FLT {
      DEBUG_VALUE_FLT_T debug_values[AS_BIN_WRITER_MAX_VARIABLES];
      DEBUG_ARRAY_FLT_T debug_arrays[AS_BIN_WRITER_MAX_VARIABLES];
      int number_of_debug_values;
      int number_of_debug_arrays;
      unsigned char locked;
   } DEBUG_FLT_T;

   DEBUG_FLT_T debug_store[AS_BIN_WRITER_MAX_FUSED_SENSORS]; // TODO: can be 1-D for some types, allocate dynamically

   int num_used_sensors; ///< how many sensors / sensor indices's did the user use to store values? Default: 1
   int padding;          /**< Unused: ensures 8 byte padding for private section below */
 protected:
   std::string log_name;
   std::string file_path;
   std::string bin_dir;
   std::string log_fname; ///< file name of log, without extension
   std::string data_type; ///< data type, e.g. detections, tracklets, etc. Used to generate bin file name
   FILE *file_ptr;
   bool first_run;
   bool is_file_open;
   bool is_active_state;
   bool open_file();

   void init();

   void write_header();

   void equalize_Array_lengths();

   void error_message(AS_BIN_WRITER_ERROR_T error);

   /*
    * elementary writing functions as in debug_additional come here
    */
   int store_debug_value(
       DEBUG_FLT_T *debug_struct,
       int debug_index,
       const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN],
       double value,
       unsigned char active);

   int store_debug_array_elem(
       DEBUG_FLT_T *debug_struct,
       int debug_index,
       const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN],
       double value,
       int arr_index,
       unsigned char active);

   bool ensure_elem_name(
       DEBUG_FLT_T *debug_struct,
       const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN],
       int &debug_index);

   bool ensure_elem_array_name(
       DEBUG_FLT_T *debug_struct,
       const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN],
       int &debug_index);

   bool used_sensors_bookkeeping(int radar_index);

   int get_number_of_header_vars();

   void reset_locked_state_if_possible(DEBUG_FLT_T *debug_struct) {
      if ((debug_struct->number_of_debug_values == 0) && (debug_struct->number_of_debug_arrays == 0)) {
         debug_struct->locked = false;
      }
   }

 public:
   /** ID string for this resimulation run,
    * public on purpose, can be changed from outside
    * Will be used as first part of the name of the final bin directory */
   static std::string resim_id_str;

   AS_bin_writer_lib(void);
   AS_bin_writer_lib(const std::string &_data_type);
   ~AS_bin_writer_lib(void);

   void close_file(void);

   void set_next_run(void) {
      first_run = true;
   }
   bool is_first_run(void) {
      return first_run;
   }
   bool is_active(void) {
      return is_active_state;
   }

   void set_data_type(const std::string &_data_type) {
      data_type = _data_type;
   }
   void set_active_state(bool state) {
      is_active_state = state;
   }

   bool is_open(void) const;

   void set_log_and_subdir_and_id_str(
       char const *log_file_str,
       char const *subdir,
       char const *resim_id);

   /// set number of (fused) sensors to be covered by this writer. Set before calling write_line(), and do not update with a higher value afterwards.
   void set_num_sensors(int num_sen) {
      used_sensors_bookkeeping(num_sen - 1);
   }
   int get_num_sensors() {
      return num_used_sensors;
   }

   /*
    * elementary writing functions as in debug_additional come here
    */
   int store_debug_value(
       const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN],
       double value,
       int radar_index,
       unsigned char active,
       int debug_index);

   int store_debug_value(
       const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN],
       double value,
       unsigned char active,
       int debug_index) {
      return store_debug_value(&(debug_store[0]), debug_index, var_name, value, active);
   }

   int store_debug_array_elem(
       const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN],
       double value,
       int arr_index,
       int radar_index,
       unsigned char active,
       int debug_index);

   int store_debug_array_elem(
       const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN],
       double value,
       int arr_index,
       unsigned char active,
       int debug_index) {
      return store_debug_array_elem(&(debug_store[0]), debug_index, var_name, value, arr_index, active);
   }

   int write_line();

   // helper functions
   static void bin_dir_from_log_dir(
       std::string &dir,
       AS_bin_writer::RSDS_MOUNTING_POSITION_T pos);

   static void bin_dir_from_log_dir(
       std::string &dir,
       std::string mounting_str);
};

class AS_bin_writer_manager {
 protected:
   typedef std::unordered_map<std::string, AS_bin_writer_lib> Bin_container_T;
   typedef std::unordered_map<std::string, int> Bin_suppressed_container_output_T;
   static Bin_container_T bin_files;
   static Bin_suppressed_container_output_T suppressed_bin_files;

   static bool suppress_all_bin_files;

   std::string log_file_str, resim_id_str;

   /**
    * There are two possible usages of the AS_bin_writer_manager:
    * - Specify the fusing sensor mounting position with each call to store_debug_array_elem and store_debug_value
    * - Specify the fusing sensor mounting position once by calling set_mounting()
    *   - In this case the mounting position stays the same for all calls to store_debug_array_elem and store_debug_value.
    *     Once set_mounting() is called the variants of store_debug_array_elem and store_debug_value that have a mounting
    *     parameter and the ones that don't behave the same. There is no possibility to mix both modes.
    */
   std::string fixed_mounting; /**< The fixed mounting position*/
   bool f_fixed_mounting;      /**< Flag indicating if AS_bin_writer_manager is run using the fixed mounting mode*/

 public:
   AS_bin_writer_manager() {
      f_fixed_mounting = false;
   }

   /** @brief get AS_bin_writer_lib object that matches the given type and mounting strings.
    * If no mounting string is specified, use fixed_mounting */
   AS_bin_writer_lib &get_obj(
       const char *const type,
       const char *mounting = 0) {
      std::string key = std::string(type) + "#";
      if (mounting && !f_fixed_mounting) {
         key += std::string(mounting);
      } else {
         key += "fixed_mounting"; /* the mounting position may change but in fixed_mounting mode the key must stay the same */
         mounting = fixed_mounting.c_str();
      }

      size_t c = bin_files.count(key);

      AS_bin_writer_lib *bin_obj = NULL;

      if (c == 0) {
         AS_bin_writer_lib *tmp = new AS_bin_writer_lib();
         bin_files.emplace(key, *tmp);
         delete tmp;

         bin_obj = &bin_files.at(key);

         bin_obj->set_data_type(type);
         bin_obj->set_log_and_subdir_and_id_str(log_file_str.c_str(), mounting, resim_id_str.c_str());

         c = suppressed_bin_files.count(type);

         if (c > 0) {
            bin_obj->set_active_state(false);
         }
         if (suppress_all_bin_files) {
            bin_obj->set_active_state(false);
         }

      } else {
         bin_obj = &bin_files.at(key);

         if (bin_obj->is_first_run()) {
            bin_obj->set_log_and_subdir_and_id_str(log_file_str.c_str(), mounting, resim_id_str.c_str());
         }
      }

      return *bin_obj;
   }

   std::vector<std::string> get_OutputFilesAtPosition(std::string position) {
      std::vector<std::string> output;
      std::string name;
      std::string sensorPos;
      std::string::size_type pos = 0;

      for (auto it = bin_files.begin(); it != bin_files.end(); ++it) {
         pos  = 0;
         name = it->first;
         pos  = name.find("#", pos);
         name = it->first.substr(0, pos);
         if (f_fixed_mounting) {
            output.push_back(name);
         } else {
            sensorPos = it->first.substr(pos + 1);
            if (std::strcmp(sensorPos.c_str(), position.c_str()) == 0) {
               output.push_back(name);
            }
         }
      }
      return output;
   }

   /* Fills a list of suppressed bin files, that shall not be generated. */
   void set_SuppressedOutputFiles(const char *type) {
      std::string key = std::string(type);

      size_t c = suppressed_bin_files.count(key);

      if (c == 0) {
         suppressed_bin_files.insert(std::make_pair(key, 1));
         int dummy = suppressed_bin_files[key]; // Temporary Fix to avoid crash in Linux Release mode when Bin files tag is disabled in DC_Lib_Control.xml
         (void)dummy;
      }
   }

   /** Suppress writing of all bin files */
   void SuppressAllBinFiles(
       bool suppress /**< true to suppress writing all bin files, false to write to bin files */
   ) {
      suppress_all_bin_files = suppress;
   }

   int store_debug_value(
       const char *const type,
       const char *const mounting,
       const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN],
       double value,
       int radar_index,
       unsigned char active,
       int debug_index) {
      get_obj(type, mounting).store_debug_value(var_name, value, radar_index, active, debug_index);
      return debug_index;
   }

   /** @brief Version that uses the mounting string set previously by set_mounting. */
   int store_debug_value(
       const char *const type,
       const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN],
       double value,
       int radar_index,
       unsigned char active,
       int debug_index) {
      get_obj(type).store_debug_value(var_name, value, radar_index, active, debug_index);
      return debug_index;
   }

   /** @brief version without specifying the radar index */
   int store_debug_value(
       const char *const type,
       const char *const mounting,
       const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN],
       double value,
       unsigned char active,
       int debug_index) {
      get_obj(type, mounting).store_debug_value(var_name, value, active, debug_index);
      return debug_index;
   }

   int store_debug_array_elem(
       const char *const type,
       const char *const mounting,
       const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN],
       double value,
       int arr_index,
       int radar_index,
       unsigned char active,
       int debug_index) {
      get_obj(type, mounting).store_debug_array_elem(var_name, value, arr_index, radar_index, active, debug_index);
      return debug_index;
   }

   /** @brief Version that uses the mounting string set previously by set_mounting. */
   int store_debug_array_elem(
       const char *const type,
       const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN],
       double value,
       int arr_index,
       int radar_index,
       unsigned char active,
       int debug_index) {
      get_obj(type).store_debug_array_elem(var_name, value, arr_index, radar_index, active, debug_index);
      return debug_index;
   }

   /** @brief version without specifying the radar index */
   int store_debug_array_elem(
       const char *const type,
       const char *const mounting,
       const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN],
       double value,
       int arr_index,
       unsigned char active,
       int debug_index) {
      get_obj(type, mounting).store_debug_array_elem(var_name, value, arr_index, active, debug_index);
      return debug_index;
   }

   void write_line() {
      for (auto it = bin_files.begin(); it != bin_files.end(); ++it) {
         it->second.write_line();
      }
   }

   void set_log_and_id_str(
       char const *_log_file_str,
       char const *_resim_id) {
      log_file_str = _log_file_str;
      resim_id_str = _resim_id;
   }

   /**
    * @brief Set string indicating the mounting for all subsequent calls to this manager. The string will determine the name of the directory
    * of the bin files.
    * Use when the mounting location is not known to the code that uses STORE_VAL_MGR  or STORE_ARRAY_ELEM_MGR.
    */
   void set_mounting(char const *mounting) {
      f_fixed_mounting = true;
      fixed_mounting   = mounting;
   }

   std::string get_mounting(void) {
      return fixed_mounting;
   }

   /**
    * @brief Set a new logfile name and set flag to update bin_mgr content.
    */
   void new_logfile(char const *_log_file_str) {
      log_file_str = _log_file_str;
   }
};

#endif

#endif
