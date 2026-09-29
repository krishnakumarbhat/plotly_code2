/* -*- C++ -*-
 * Delphi Delco Electronics Proprietary
 *
 * Project page is http://pep.usinkok.northamerica.delphiauto.net/projectdb/public/?page=project-tasks&pid=2733.5882
 * Jira Page is http://jiraprod1.delphiauto.net:8080/projects/AUM
 *
 *
 * Copyright 2016 by Delphi Corporation. All Rights Reserved.
 * This file contains Delphi Delco Electonics Proprietary information.
 * It may not be reproduced or distributed without permission.
 * Contact: Uri Iurgel
 *
 * @file
 * Library for writing binary files fror ASGUI
 * AS = Active Safety
 * Based on binary_writer.cpp from TCK, Audi SRR3 PDP project
 */

#include "binary_writer_lib.h"
#include <assert.h>

#ifdef _MSC_VER
#include <io.h>     // _access_s
#include <direct.h> // _mkdir

#elif __GNUC__
#include <unistd.h>   // access()
#include <sys/stat.h> //  mkdir()
#include <dirent.h>   // DIR, opendir(), closedir()
#endif

#ifdef _MSC_VER
#define MSVS_DISABLE_WARNING(wrn) __pragma(warning(disable : wrn))
#define MSVS_ENABLE_WARNING(wrn) \
   __pragma(warning(default : wrn))
#else
#define MSVS_DISABLE_WARNING(warning)
#define MSVS_ENABLE_WARNING(warning)
#endif

/* As long as some feature still use INFTY we need to know it definition in the bin writer. */
#ifndef INFTY
#define INFTY ((float)1E+36)
#endif

#ifdef WIN32
std::string delimiter = "\\";
#else
std::string delimiter = "/";
#endif

// definition of static member
std::string AS_bin_writer_lib::resim_id_str = "";

AS_bin_writer_lib::AS_bin_writer_lib(void) {
   init();
}

AS_bin_writer_lib::AS_bin_writer_lib(const std::string &_data_type) {
   init();
   set_data_type(_data_type);
}

void AS_bin_writer_lib::init() {
   first_run       = true;
   is_active_state = true;
   file_ptr        = NULL;
   is_file_open    = false;
   file_path.clear();
   bin_dir.clear();
   log_fname.clear();
   data_type.clear();
   data_type.clear();

   memset(&debug_store, 0, sizeof(DEBUG_FLT_T) * AS_BIN_WRITER_MAX_FUSED_SENSORS);
   num_used_sensors = 1; // yes, 1 because 1 is the minimum number
}

AS_bin_writer_lib::~AS_bin_writer_lib(void) {
   close_file();
}

#ifdef __GNUC__
void splitpath_gcc(
    std::string log_file_str, /**< Path that shall be split */
    std::string delimiter,    /**< Delimiter between files and folders */
    std::string *path,        /**< [out] path to log_file_str */
    std::string *file_name    /**< File name of log_file_str */
) {
   std::string bin_dir = "";
   size_t pos          = 0;
   std::string token;
   while ((pos = log_file_str.find(delimiter)) != std::string::npos) {
      token = log_file_str.substr(0, pos);
      log_file_str.erase(0, pos + delimiter.length());
      bin_dir += token + delimiter;
   }
   *path      = bin_dir;
   *file_name = log_file_str;
}

#endif

void AS_bin_writer_lib::set_log_and_subdir_and_id_str(
    char const *log_file_str,
    char const *subdir,
    char const *resim_id) {
   resim_id_str = resim_id;
#ifdef _MSC_VER
   char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
   _splitpath_s(log_file_str,
                drive, _MAX_DRIVE,
                dir, _MAX_DIR,
                fname, _MAX_FNAME,
                ext, _MAX_EXT);
   log_fname = fname;
   bin_dir   = drive;
   bin_dir += dir;
#elif __GNUC__
   splitpath_gcc(log_file_str, delimiter, &bin_dir, &log_fname);
#endif

   if (resim_id_str != "") {
      bin_dir += resim_id_str + "_";
   }
   bin_dir += std::string(subdir);
}

bool make_sure_directory_exists(std::string directory) {
#ifdef _MSC_VER
   bool dir_exists = false;
   // check whether directory exists, if not: create
   errno_t err = _access_s(directory.c_str(), 0);
   if (err) {
      // error
      switch (err) {
      case EACCES:
      case EINVAL:
         printf("Cannot create bin file. Access denied to directory %s.\n", directory.c_str());
         break;

      case ENOENT:
         // path not found create it.
         // returns the value 0 if the new directory was created
         if (!_mkdir(directory.c_str())) {
            dir_exists = true;
         }
         // on windows: alternative for creating full path: MakeSureDirectoryPathExists
         break;
      }
   } else {
      dir_exists = true;
   }

   return dir_exists;
#elif __GNUC__
   bool dir_exists = false;
   int err         = access(directory.c_str(), F_OK);
   DIR *dir        = opendir(directory.c_str());
   if (dir) {
      /* Directory exists. */
      dir_exists = true;
      closedir(dir);
   } else {
      /* Directory does not exist. */
#ifdef __linux__
      mkdir(directory.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#else
      mkdir(directory.c_str());
#endif
      /* check again if dir could be created, return value from mkdir differs between compilers */
      dir_exists = (NULL != opendir(directory.c_str()));
   }
   return dir_exists;
#endif
}

/**
 * A memory buffer of size  512k is used for the file
 * @return true if file could be opened or is already open
 */
bool AS_bin_writer_lib::open_file() {
   if (!is_file_open) {
      bool dir_exists = false;
      file_path       = bin_dir + delimiter + log_fname;
      file_path += "_" + data_type + "." + std::string(FILE_EXT);

      dir_exists = make_sure_directory_exists(bin_dir);

      if (dir_exists) {
#ifdef _MSC_VER
         fopen_s(&file_ptr, file_path.c_str(), "wb");
#else
         file_ptr = fopen(file_path.c_str(), "wb");
#endif
         if (file_ptr != NULL) {
            is_file_open = true;

            /* increase file buffer to have less writes
             * The default size of a stream buffer is 4K.
             */
            setvbuf(file_ptr, NULL, _IOFBF, 512 * 1024);
         } else {
            printf("\n\nERROR opening bin file %s.", file_path.c_str());
         }
      }
   }
   return is_file_open;
}

/**
 * @brief Create file from file name and path, without extension
 * @param file_name: path and file name without extension
 * @return true if file could be opened
 */
#if 0
bool AS_bin_writer_lib::create_file(std::string file_name)
{
   file_path = file_name + std::string(FILE_EXT);

   file_ptr = fopen(file_path.c_str(), "wb");
   if (file_ptr != NULL)
   {
      is_file_open = true;
   }
   else
   {
      printf("\n\nERROR opening file %s.", file_path.c_str());
      is_file_open = false;
   }

   return is_file_open;
}

#endif

bool AS_bin_writer_lib::is_open(void) const {
   return is_file_open;
}

void AS_bin_writer_lib::close_file(void) {
   if (is_file_open == true) {
      fclose(file_ptr);
      file_ptr     = NULL;
      is_file_open = false;
   }
}

void safe_string_copy(
    char *dest,
    size_t dest_size,
    const char *source) {
   size_t source_length;

   source_length = strlen(source);
   if (source_length >= dest_size) {
      printf("\nString '%s' is too long (%zu bytes, max is %zu). Shortening string.", dest, source_length, dest_size);
      memcpy(dest, source, dest_size);
      dest[dest_size - 1] = 0;
   } else {
      memset(dest, 0, sizeof(char) * dest_size);
      memcpy(dest, source, source_length);
   }
}

/************************************************/
/* Elementary write functions */
/************************************************/

bool AS_bin_writer_lib::ensure_elem_name(
    DEBUG_FLT_T *debug_struct,
    const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN],
    int &debug_index) {
   int i;
   bool found = false;
   char checked_var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN];

   assert(debug_index == -1);
   assert(debug_struct->number_of_debug_values < AS_BIN_WRITER_MAX_VARIABLES);
   assert(strlen(var_name) <= AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN);

   /* The pointer var_name is provided by a user. It thus is not guaranteed to
    * comply with the size limit. */
   safe_string_copy(checked_var_name, AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN, var_name);

   for (i = 0; i < debug_struct->number_of_debug_values; i++) {
      if (strcmp(checked_var_name, debug_struct->debug_values[i].name) == 0) {
         found = true;
         break;
      }
   }
   if (found) {
      debug_index = i;
   } else {
      if (debug_struct->locked) {
         printf("\nTrying to add variable %s although the .bin header was written already!", checked_var_name);
         debug_index = -1;
         return false;
      }
      debug_index = debug_struct->number_of_debug_values;
      debug_struct->number_of_debug_values++;
      assert(debug_struct->number_of_debug_values < AS_BIN_WRITER_MAX_VARIABLES);
      if (debug_index < AS_BIN_WRITER_MAX_VARIABLES) {
         memcpy(debug_struct->debug_values[debug_index].name, checked_var_name, AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN);
      }
   }
   return true;
}

bool AS_bin_writer_lib::ensure_elem_array_name(
    DEBUG_FLT_T *debug_struct,
    const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN],
    int &debug_index) {
   int i;
   bool found = false;

   assert(debug_index == -1);
   for (i = 0; i < debug_struct->number_of_debug_arrays; i++) {
      if (strcmp(var_name, debug_struct->debug_arrays[i].name) == 0) {
         found = true;
         break;
      }
   }
   if (found) {
      debug_index = i;
   } else {
      if (debug_struct->locked) {
         printf("\nTrying to add variable %s although the .bin header was written already!", var_name);
         debug_index = -1;
         return false;
      }
      debug_index = debug_struct->number_of_debug_arrays;
      if (debug_index < AS_BIN_WRITER_MAX_VARIABLES) {
         debug_struct->number_of_debug_arrays++;
         safe_string_copy(debug_struct->debug_arrays[debug_index].name, AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN, var_name);
      } else {
         printf("\nUnable to write array %s because only %i array can be written.", var_name, AS_BIN_WRITER_MAX_VARIABLES);
         debug_index = -1;
         return false;
      }
   }
   return true;
}

void AS_bin_writer_lib::error_message(AS_BIN_WRITER_ERROR_T error) {
   switch (error) {
   case AS_BIN_WRITER_ERROR_DISK_FULL:
      printf("\n\nCould not write bin file. Is disk full?\n");
      getchar();
      exit(EXIT_FAILURE);

   default:
      printf("\n\nUnknown error!\n");
      exit(EXIT_FAILURE);
   }
}

void AS_bin_writer_lib::equalize_Array_lengths() {
   DEBUG_FLT_T *debug_struct_first = &(debug_store[0]);

   std::vector<int> arr_pos(num_used_sensors); // debug_struct_first->number_of_debug_arrays);

   for (int arr_count = 0; arr_count < debug_struct_first->number_of_debug_arrays; arr_count++) {
      std::string curr_name = debug_struct_first->debug_arrays[arr_count].name;

      // get position of the specific array in the debug_struct
      for (int radar_idx = 0; radar_idx < num_used_sensors; radar_idx++) {
         DEBUG_FLT_T *debug_struct_hlp = &(debug_store[radar_idx]);
         for (int i = 0; i < debug_struct_hlp->number_of_debug_arrays; i++) {
            if (debug_struct_hlp->debug_arrays[i].name == curr_name) {
               arr_pos[radar_idx] = i;
            }
         }
      }
      // find largest array
      int length = debug_struct_first->debug_arrays[arr_count].array_last_entry;
      for (int radar_idx = 0; radar_idx < num_used_sensors; radar_idx++) {
         DEBUG_FLT_T *debug_struct_hlp = &(debug_store[radar_idx]);
         if (debug_struct_hlp->debug_arrays[arr_pos[radar_idx]].array_last_entry > length) {
            length = debug_struct_hlp->debug_arrays[arr_pos[radar_idx]].array_last_entry;
         }
      }

      // write length to all arrays
      for (int radar_idx = 0; radar_idx < num_used_sensors; radar_idx++) {
         DEBUG_FLT_T *debug_struct_hlp                                       = &(debug_store[radar_idx]);
         debug_struct_hlp->debug_arrays[arr_pos[radar_idx]].array_last_entry = length;
      }
   }
}

int AS_bin_writer_lib::store_debug_value(
    const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN],
    double value,
    int radar_index,
    unsigned char active,
    int debug_index) {
   if (!active) {
      return -1;
   }

   assert(radar_index < AS_BIN_WRITER_MAX_FUSED_SENSORS);
   if (radar_index >= AS_BIN_WRITER_MAX_FUSED_SENSORS) {
      return -1;
   }

   // did we already store the variable name in all arrays of debug_struct ?
   if (-1 == debug_index) {
      // no: do it
      for (int i = 0; i < AS_BIN_WRITER_MAX_FUSED_SENSORS; i++) {
         // tell array for sensor i to store var_name
         // (pass param. -1 as debug_index because for radar_index i, this header variable name is not known)
         debug_index = -1;
         ensure_elem_name(&(debug_store[i]), var_name, debug_index);
      }
   }

   if (!used_sensors_bookkeeping(radar_index)) {
      return -1;
   }

   return store_debug_value(&(debug_store[radar_index]), debug_index, var_name, value, active);
}

/**
 *
 * This stores a value and its variable name in RAM. It then is written to a file when all
 *  the other values are written too
 * @param debug_index index of column name (var_name). -1 on first
 */
int AS_bin_writer_lib::store_debug_value(
    DEBUG_FLT_T *debug_struct,
    int debug_index,
    const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN],
    double value,
    unsigned char active) {
   if (!active) {
      return -1;
   }
   reset_locked_state_if_possible(debug_struct);

   /* find index of var_name. If not known, add it */
   if (debug_index < 0) {
      if (!ensure_elem_name(debug_struct, var_name, debug_index)) {
         return -1;
      }
   }
   assert(debug_index < AS_BIN_WRITER_MAX_VARIABLES);
   debug_struct->debug_values[debug_index].value = value;
   return debug_index;
}

int AS_bin_writer_lib::store_debug_array_elem(
    const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN],
    double value,
    int arr_index,
    int radar_index,
    unsigned char active,
    int debug_index) {
   if (!active) {
      return -1;
   }
   assert(radar_index < AS_BIN_WRITER_MAX_FUSED_SENSORS);
   if (radar_index >= AS_BIN_WRITER_MAX_FUSED_SENSORS) {
      return -1;
   }

   // did we already store the variable name in all arrays of debug_struct ?
   if (-1 == debug_index) {
      // no: do it
      for (int i = 0; i < AS_BIN_WRITER_MAX_FUSED_SENSORS; i++) {
         debug_index = -1;
         ensure_elem_array_name(&(debug_store[i]), var_name, debug_index);
         // tell array for sensor i to store var_name  (pass param. -1 as debug_index)
         // debug_index = store_debug_array_elem(&(debug_store[i]), -1, var_name, 0, arr_index, active);
      }
   }

   if (!used_sensors_bookkeeping(radar_index)) {
      return -1;
   }

   return store_debug_array_elem(&(debug_store[radar_index]), debug_index, var_name, value, arr_index, active);
}

/** This stores an array value and its variable name in RAM. It then is written to a file when all
 *  the other values are written too*/
int AS_bin_writer_lib::store_debug_array_elem(
    DEBUG_FLT_T *debug_struct,
    int debug_index,
    const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN],
    double value,
    int arr_index,
    unsigned char active) {
   if (!active) {
      return -1;
   }
   reset_locked_state_if_possible(debug_struct);

   /* find index of var_name, add if unknown */
   if (debug_index < 0) {
      if (!ensure_elem_array_name(debug_struct, var_name, debug_index)) {
         return -1;
      }
   }

   assert(arr_index < AS_BIN_WRITER_MAX_ARRAY_SIZE);
   if (arr_index < AS_BIN_WRITER_MAX_ARRAY_SIZE) {
      debug_struct->debug_arrays[debug_index].values[arr_index] = value;
      if (arr_index > debug_struct->debug_arrays[debug_index].array_last_entry) {
         debug_struct->debug_arrays[debug_index].array_last_entry = arr_index;
      }
   }
   return debug_index;
}

/**
 * Update num_used_sensors if necessary, based on passed radar_index
 * @return false if adding sensors after header / data has already been written.
 */
bool AS_bin_writer_lib::used_sensors_bookkeeping(int radar_index) {
   bool success = true;

   if (radar_index + 1 <= num_used_sensors) {
      return true;
   } else {
      if (!debug_store->locked) {
         // first "line" of data not yet written, still flexible to add more sensors
         num_used_sensors = radar_index + 1;
      } else {
         // adding more sensors after data (scans) have already been written? That would lead to inconsistent data
         if (radar_index + 1 > num_used_sensors) {
            printf("Bin writer: Not possible to add more sensors. Already wrote data for %d sensors.\n", num_used_sensors);
            printf("Bin file: %s\n", file_path.c_str());
            success = false;
            assert(0);
         }
      }
   }
   return success;
}

/** @brief Return number of variable name entries in header.
 * Cound includes "REPEAT" and "ENDREPEAT" strings. */
int AS_bin_writer_lib::get_number_of_header_vars() {
   DEBUG_FLT_T *debug_struct = debug_store; /// 1st array enough, since all headers equal
   // count the number of header entrys for REPEAT in count_var
   int i_arr, count_var, i_var;
   unsigned char debug_array_written[AS_BIN_WRITER_MAX_VARIABLES];

   memset(debug_array_written, 0, sizeof(debug_array_written)); // set to false

   count_var = 0;
   for (i_var = 0; i_var < debug_struct->number_of_debug_arrays; i_var++) {
      if (!debug_array_written[i_var]) {
         int repeat_size = debug_struct->debug_arrays[i_var].array_last_entry + 1;
         count_var++; // REPEAT##
         for (i_arr = i_var; i_arr < debug_struct->number_of_debug_arrays; i_arr++) {
            if (repeat_size == debug_struct->debug_arrays[i_arr].array_last_entry + 1) {
               count_var++;
               debug_array_written[i_arr] = true;
            }
         }
         count_var++; // END_REPEAT
      }
   }
   return (count_var + debug_struct->number_of_debug_values);
}

void AS_bin_writer_lib::write_header() {
   int count_var = 0;
   int i_var, i_arr;
   unsigned char debug_array_written[AS_BIN_WRITER_MAX_VARIABLES];

   equalize_Array_lengths(); // set length of arrays in debug_store[radar_index] to correct number, where they are not filled

   FILE *&debug_file         = file_ptr; // just use other name
   DEBUG_FLT_T *debug_struct = debug_store;
   size_t written_bytes;
   // header is written, lock debug struct
   debug_struct->locked = true;
   memset(debug_array_written, 0, sizeof(debug_array_written)); // set to false

   // we need to write the debug header first
   fflush(debug_file); // why fflush?
   // write number of headers (columns)
   OUTPUT_FORMAT tempdbl = (OUTPUT_FORMAT)(get_number_of_header_vars());
   written_bytes         = fwrite(&tempdbl, sizeof(OUTPUT_FORMAT), 1, debug_file);
   if (written_bytes != 1) {
      error_message(AS_BIN_WRITER_ERROR_DISK_FULL);
   }
   // write the header for the arrays
   for (i_var = 0; i_var < debug_struct->number_of_debug_values; i_var++) {
      written_bytes = fwrite(&debug_struct->debug_values[i_var].name, sizeof(char), AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN, debug_file);
      if (written_bytes != AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN) {
         error_message(AS_BIN_WRITER_ERROR_DISK_FULL);
      }

      count_var++;
   }

   for (i_var = 0; i_var < debug_struct->number_of_debug_arrays; i_var++) {
      if (!debug_array_written[i_var]) {
         int repeat_size = debug_struct->debug_arrays[i_var].array_last_entry + 1;
         char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN];
         memset(&var_name[0], 0, sizeof(char) * AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN);
         MSVS_DISABLE_WARNING(4996)
         sprintf(&var_name[0], "REPEAT%d", repeat_size);
         MSVS_ENABLE_WARNING(4996)
         written_bytes = fwrite(&var_name[0], sizeof(char), AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN, debug_file);
         if (written_bytes != AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN) {
            error_message(AS_BIN_WRITER_ERROR_DISK_FULL);
         }
         for (i_arr = i_var; i_arr < debug_struct->number_of_debug_arrays; i_arr++) {
            if (repeat_size == debug_struct->debug_arrays[i_arr].array_last_entry + 1) {
               written_bytes = fwrite(&debug_struct->debug_arrays[i_arr].name, sizeof(char), AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN, debug_file);
               if (written_bytes != AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN) {
                  error_message(AS_BIN_WRITER_ERROR_DISK_FULL);
               }
               debug_array_written[i_arr] = true;
            }
         }
         safe_string_copy(var_name, AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN, "END_REPEAT");
         written_bytes = fwrite(&var_name[0], sizeof(char), AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN, debug_file);
         if (written_bytes != AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN) {
            error_message(AS_BIN_WRITER_ERROR_DISK_FULL);
         }
         count_var++;
      }
   }
}

/** write_line
 *   This writes the additional debug values ot the corresponding header into a given file
 *  Call once per cycle
 */
int AS_bin_writer_lib::write_line() {
   int count_var = 0; // debug_struct->number_of_debug_arrays + debug_struct->number_of_debug_values;
   int i_var, i_arr, i_arr_entry;

   size_t written_bytes;
   OUTPUT_FORMAT value;

   if (first_run) {
      if (open_file()) {
         write_header();
      }
      first_run = false;
   }

   if (is_file_open) {
      FILE *&debug_file = file_ptr; // just use other name

      for (int radar_idx = 0; radar_idx < num_used_sensors; radar_idx++) {
         // write data of sensor radar_idx

         DEBUG_FLT_T *debug_struct = &(debug_store[radar_idx]);

         unsigned char debug_array_written[AS_BIN_WRITER_MAX_VARIABLES];
         memset(debug_array_written, 0, sizeof(debug_array_written)); // set to false

         count_var = 0;
         for (i_var = 0; i_var < debug_struct->number_of_debug_values; i_var++) {
            value = (OUTPUT_FORMAT)debug_struct->debug_values[i_var].value;
            if ((value == INFTY) || (value == -INFTY)) {
               value = NAN;
            }
            written_bytes = fwrite(&value, sizeof(OUTPUT_FORMAT), 1, debug_file);
            if (written_bytes != 1) {
               error_message(AS_BIN_WRITER_ERROR_DISK_FULL);
            }
#if 0
            if (strcmp(debug_struct->debug_values[i_var].name, "index") == 0)
            {
               printf("Writing ts %.4f\n", value);
            }
#endif
            count_var++;
         }

         for (i_var = 0; i_var < debug_struct->number_of_debug_arrays; i_var++) {
            if (!debug_array_written[i_var]) {
               int repeat_size = debug_struct->debug_arrays[i_var].array_last_entry + 1;
               for (i_arr_entry = 0; i_arr_entry < repeat_size; i_arr_entry++) {
                  for (i_arr = i_var; i_arr < debug_struct->number_of_debug_arrays; i_arr++) {
                     if (repeat_size == debug_struct->debug_arrays[i_arr].array_last_entry + 1) {
                        value = (OUTPUT_FORMAT)debug_struct->debug_arrays[i_arr].values[i_arr_entry];
                        if ((value == INFTY) || (value == -INFTY)) {
                           value = NAN;
                        }
                        written_bytes = fwrite(&value, sizeof(OUTPUT_FORMAT), 1, debug_file);
                        if (written_bytes != 1) {
                           error_message(AS_BIN_WRITER_ERROR_DISK_FULL);
                        }
                        count_var++;
                        debug_array_written[i_arr] = true;
                     }
                  }
               }
            }
         }
      }
   }
   return count_var;
}

/************************************************/
/* file name support functions */
/************************************************/

/*
 */

/**
 * @brief Append a suffix to the given directory string indicating the resim string and the mounting position
 * Path for storing bin files:
 * dir_to_log\<resim_string>_<mounting_position>
 * where resim_string can identify the Resimulation run, etc.
 * and mounting_poition is something like "_rear_left"
 *
 * @param[in,out] dir In: base directory name (e.g. from log file)
 * @param[in] pos: radar mounting position enum
 *
 * Also uses AS_bin_writer_lib::resim_id_str
 *
 */
void AS_bin_writer_lib::bin_dir_from_log_dir(
    std::string &dir,
    AS_bin_writer::RSDS_MOUNTING_POSITION_T pos) {
   dir += "\\" + resim_id_str;

   switch (pos) {
   case AS_bin_writer::RSDS_MOUNTING_POSITION_REAR_LEFT:
      dir += "_rear_left";
      break;

   case AS_bin_writer::RSDS_MOUNTING_POSITION_REAR_RIGHT:
      dir += "_rear_right";
      break;

   case AS_bin_writer::RSDS_MOUNTING_POSITION_FRONT_RIGHT:
      dir += "_front_right";
      break;

   case AS_bin_writer::RSDS_MOUNTING_POSITION_FRONT_LEFT:
      dir += "_front_left";
      break;

   default:
      assert(!"Unknown mounting position");
      break;
   }
}

void AS_bin_writer_lib::bin_dir_from_log_dir(
    std::string &dir,
    std::string mounting_str) {
   dir += "\\" + resim_id_str + mounting_str;
}

AS_bin_writer_manager::Bin_container_T AS_bin_writer_manager::bin_files                              = AS_bin_writer_manager::Bin_container_T();
AS_bin_writer_manager::Bin_suppressed_container_output_T AS_bin_writer_manager::suppressed_bin_files = AS_bin_writer_manager::Bin_suppressed_container_output_T();
bool AS_bin_writer_manager::suppress_all_bin_files                                                   = false;
