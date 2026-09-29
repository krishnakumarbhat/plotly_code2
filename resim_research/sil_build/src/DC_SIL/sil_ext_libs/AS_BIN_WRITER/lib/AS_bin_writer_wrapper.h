#ifndef AS_BIN_WRITER_WRAPPER_H
#define AS_BIN_WRITER_WRAPPER_H

/*
Project page is http://pep.usinkok.northamerica.delphiauto.net/projectdb/public/?page=project-tasks&pid=2733.5882
Jira Page is http://jiraprod1.delphiauto.net:8080/projects/AUM
*/

#include "as-bin-writer-lib_export.h"
#include "binary_writer_lib_sizes.h"

typedef enum AS_BWW_MOUNTING_POSITIONS {
   AS_BWW_MOUNTING_UNKNOWN,
   AS_BWW_MOUNTING_ARTIFICIAL_LOGFILE,
   AS_BWW_MOUNTING_REAR_LEFT,
   AS_BWW_MOUNTING_REAR_RIGHT,
   AS_BWW_MOUNTING_FRONT_RIGHT,
   AS_BWW_MOUNTING_FRONT_LEFT,
   AS_BWW_MOUNTING_REAR_CENTER,
   AS_BWW_MOUNTING_FRONT_CENTER,
   AS_BWW_MOUNTING_LEFT_CENTER,
   AS_BWW_MOUNTING_RIGHT_CENTER,
   AS_BWW_MOUNTING_INVALID_POSITION,
   AS_BWW_MOUNTING_DOMAIN_CONTROLLER_UNIT,
   AS_BWW_MOUNTING_NUMBER_OF_MOUNTING_STRINGS
} AS_BWW_MOUNTING_POSITIONS_T;

#ifdef __cplusplus
extern "C"
{
#endif
   /**
    * Initializes the bin writer.
    * - The bin files will be written into a sub folder at in the folder where logPath is located.
    * - The bin files will have the same name but the extension .bin
    * - if logPath is a dvsu file and the file name ends with _71..._74 then these three bytes will be removed as well
    *   from the bin file name.
    */
   AS_BIN_WRITER_LIB_EXPORT void AS_bww_init(const char *logPath /**< path to log file to create a bin log for */);

   /**
    * Set the mounting position string of the fusing sensor. This will be used as folder name when writing the bin files.
    */
   AS_BIN_WRITER_LIB_EXPORT void AS_bww_SetMounting(const char *mntStr /**< Fusing sensor mounting position */);

   /**
    * Set the mounting position of the fusing sensor. This will be used as folder name when writing the bin files.
    */
   AS_BIN_WRITER_LIB_EXPORT void AS_bww_SetMounting_RP(AS_BWW_MOUNTING_POSITIONS_T RadarPosition /**< Fusing sensor mounting position */);

   /**
    * Set the time stamp to store in the bin files on next call to AS_bww_write_line() or AS_BWW_WRITE_LINE_MAC
    */
   AS_BIN_WRITER_LIB_EXPORT void AS_bww_SetTimeStamp(unsigned int time_stamp /**< Time stamp to be written */);

   /**
    * Set the scan index to store in the bin files on next call to AS_bww_write_line() or AS_BWW_WRITE_LINE_MAC
    */
   AS_BIN_WRITER_LIB_EXPORT void AS_bww_SetScanIndex(unsigned int scan_index /**< scan index to be written */);

   /**
    * Sets bin files not to be written.
    */
   AS_BIN_WRITER_LIB_EXPORT void AS_bww_SetListOfSuppressedBinFiles(
       const char **type,  /**< Array of pointers to strings of names to be suppressed */
       unsigned int length /**< length of type array */
   );

   /**
    * Closes the bin files currently open for given radarPosition
    */
   AS_BIN_WRITER_LIB_EXPORT void AS_bww_close_file(AS_BWW_MOUNTING_POSITIONS_T radarPosition /**< Fusing sensors mounting position */);

   /**
    * Writes currently collected data to bin files
    */
   AS_BIN_WRITER_LIB_EXPORT void AS_bww_write_line(
       const unsigned int timeStamp,             /**< Time stamp of currently collected data */
       const unsigned int ScanIndex,             /**< Scan index of currently collected data */
       AS_BWW_MOUNTING_POSITIONS_T radarPosition /**< Fusing sensors mounting position */
   );

   /**
    * Suppress writing of bin files
    */
   AS_BIN_WRITER_LIB_EXPORT void AS_bww_suppress_all_bin_files(
       unsigned char state /**< true to suppress writing all bin files, false to write to bin files */
   );

   /**
    * Collect a single value.
    */
   AS_BIN_WRITER_LIB_EXPORT int AS_bin_mgr_store_debug_value(
       const char *const type,                                   /**< [in] bin file type, e.g. 'detections'. Will be appended to file name of bin file. */
       const char *const mounting,                               /**< [in] determines the sub folder the bin file is stored to */
       const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN], /**< variable name to write to */
       double value,                                             /**< value to write */
       int radar_index,                                          /**< index in radar fusion setup the value belongs to */
       unsigned char active,                                     /**< Shall writing be performed */
       int debug_index,                                          /**< the index in the debug data to write to (if known. The index is returned by this function) */
       void **_ppwriter                                          /**< [in, out] bin writer instance or pointer to NULL. */
   );

   /**
    * Collect an array element.
    */
   AS_BIN_WRITER_LIB_EXPORT int AS_bin_mgr_store_debug_array_elem(
       const char *const type,                                   /**< [in] bin file type, e.g. 'detections'. Will be appended to file name of bin file. */
       const char *const mounting,                               /**< [in] determines the sub folder the bin file is stored to */
       const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN], /**< variable name to write to */
       double value,                                             /**< value to write */
       int arr_index,
       int radar_index,      /**< index in radar fusion setup the value belongs to */
       unsigned char active, /**< Shall writing be performed */
       int debug_index,      /**< the index in the debug data to write to (if known. The index is returned by this function) */
       void **_ppwriter      /**< [in, out] bin writer instance or pointer to NULL. */
   );

   /**
    * Collect a single value.
    * Version without direct specification of mounting string.
    * AS_bin_writer_manager::set_mounting()  must be used to  set the mounting position beforehand
    */
   AS_BIN_WRITER_LIB_EXPORT int AS_bin_mgr_store_debug_value_nomnt(
       const char *const type,                                   /**< [in] bin file type, e.g. 'detections'. Will be appended to file name of bin file. */
       const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN], /**< variable name to write to */
       double value,                                             /**< value to write */
       int radar_index,                                          /**< index in radar fusion setup the value belongs to */
       unsigned char active,                                     /**< Shall writing be performed */
       int debug_index,                                          /**< the index in the debug data to write to (if known. The index is returned by this function) */
       void **_ppwriter                                          /**< [in, out] bin writer instance or pointer to NULL. */
   );

   /**
    * Collect an array element.
    * Version without direct specification of mounting string.
    * AS_bin_writer_manager::set_mounting()  must be used to  set the mounting position beforehand
    */
   AS_BIN_WRITER_LIB_EXPORT int AS_bin_mgr_store_debug_array_elem_nomnt(
       const char *const type,                                   /**< [in] bin file type, e.g. 'detections'. Will be appended to file name of bin file. */
       const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN], /**< variable name to write to */
       double value,                                             /**< value to write */
       int arr_index,
       int radar_index,      /**< index in radar fusion setup the value belongs to */
       unsigned char active, /**< Shall writing be performed */
       int debug_index,      /**< the index in the debug data to write to (if known. The index is returned by this function) */
       void **_ppwriter      /**< [in, out] bin writer instance or pointer to NULL. */
   );

#ifdef __cplusplus
} /* end of extern "C"*/
#endif
/**
 * \defgroup AS_BWW_MAC Active Safety Bin Writer Wrapper Macros
 * If the cmake option BINWRITER_MOCK is ON then all these macros expand to NOOP.
 */

/** \copydoc AS_bww_init() \ingroup AS_BWW_MAC*/
#define AS_BWW_INIT_MAC(logPath) AS_bww_init(logPath)

/** \copydoc AS_bww_SetMounting_RP() \ingroup AS_BWW_MAC*/
#define AS_BWW_SET_MOUNTING_RP_MAC(radarPosition) AS_bww_SetMounting_RP(radarPosition)

/** \copydoc AS_bww_SetTimeStamp() \ingroup AS_BWW_MAC*/
#define AS_BWW_SET_TIME_STAMP_MAC(mntStr) AS_bww_SetTimeStamp(mntStr)

/** \copydoc AS_bww_SetScanIndex() \ingroup AS_BWW_MAC*/
#define AS_BWW_SET_SCAN_INDEX_MAC(timestamp) AS_bww_SetScanIndex(timestamp)

/** \copydoc AS_bww_SetListOfSuppressedBinFiles() \ingroup AS_BWW_MAC*/
#define AS_BWW_SET_LIST_OF_SUPPRESSED_BIN_FILES_MAC(type, length) AS_bww_SetListOfSuppressedBinFiles(type, length)

/** \copydoc AS_bww_close_file() \ingroup AS_BWW_MAC*/
#define AS_BWW_CLOSE_FILE_MAC(radarPosition) AS_bww_close_file(radarPosition)

/** \copydoc AS_bww_write_line() \ingroup AS_BWW_MAC*/
#define AS_BWW_WRITE_LINE_MAC(timeStamp, ScanIndex, radarPosition) AS_bww_write_line(timeStamp, ScanIndex, radarPosition)

/** \copydoc AS_bww_suppress_all_bin_files() \ingroup AS_BWW_MAC*/
#define AS_BWW_SUPPRESS_ALL_BIN_FILES(suppress) AS_bww_suppress_all_bin_files(suppress)

/** For the unconditional macros */
#define BINWRITER_TRUE (1)

/**
 * Conditionally stores a single value to the given bin file.
 * \param type bin file to write to (string)
 * \param var_name variable name to write to (string)
 * \param value to be written (double)
 * \param active flag indicating if writing shall be performed (boolean).
 */
#define STORE_VAL_MGR_WPR_CONDITIONAL(type, var_name, value, active)                                                                  \
   {                                                                                                                                  \
      static int debug_index = -1;                                                                                                    \
      static void *pwriter   = NULL;                                                                                                  \
      int radar_index        = 0;                                                                                                     \
      debug_index            = AS_bin_mgr_store_debug_value_nomnt(type, var_name, value, radar_index, active, debug_index, &pwriter); \
   }

/**
 * Stores a single value to the given bin file.
 * \param type bin file to write to (string)
 * \param var_name variable name to write to (string)
 * \param value to be written (double)
 */
#define STORE_VAL_MGR_WPR(type, var_name, value) STORE_VAL_MGR_WPR_CONDITIONAL(type, var_name, value, BINWRITER_TRUE)

/**
 * Conditionally stores a single indexed value to the given bin file.
 * \param type bin file to write to (string)
 * \param var_name variable name to write to (string)
 * \param value to be written (double)
 * \param index to write to
 * \param active flag indicating if writing shall be performed (boolean).
 */
#define STORE_VAL_MGR_WPR_INDEXED_CONDITIONAL(type, var_name, value, index, active)                                             \
   {                                                                                                                            \
      static int debug_index = -1;                                                                                              \
      static void *pwriter   = NULL;                                                                                            \
      debug_index            = AS_bin_mgr_store_debug_value_nomnt(type, var_name, value, index, active, debug_index, &pwriter); \
   }

/**
 * Stores a single indexed value to the given bin file.
 * \param type bin file to write to (string)
 * \param var_name variable name to write to (string)
 * \param value to be written (double)
 * \param index to write to
 */
#define STORE_VAL_MGR_WPR_INDEXED(type, var_name, value, index) STORE_VAL_MGR_WPR_INDEXED_CONDITIONAL(type, var_name, value, index, BINWRITER_TRUE)

/**
 * Conditionally stores a value to an index of an array to the given bin file.
 * \param type bin file to write to (string)
 * \param var_name variable name to write to (string)
 * \param value to be written (double)
 * \param arr_index to write to
 * \param active flag indicating if writing shall be performed (boolean).
 */
#define STORE_ARRAY_ELEM_MGR_WPR_CONDITIONAL(type, var_name, value, arr_index, active)                                                                \
   {                                                                                                                                                  \
      static int debug_index = -1;                                                                                                                    \
      static void *pwriter   = NULL;                                                                                                                  \
      int radar_index        = 0;                                                                                                                     \
      debug_index            = AS_bin_mgr_store_debug_array_elem_nomnt(type, var_name, value, arr_index, radar_index, active, debug_index, &pwriter); \
   }

/**
 * Stores a value to an index of an array to the given bin file.
 * \param type bin file to write to (string)
 * \param var_name variable name to write to (string)
 * \param value to be written (double)
 * \param arr_index to write to
 */
#define STORE_ARRAY_ELEM_MGR_WPR(type, var_name, value, arr_index) STORE_ARRAY_ELEM_MGR_WPR_CONDITIONAL(type, var_name, value, arr_index, BINWRITER_TRUE)

/**
 * Conditionally stores an indexed value to an index of an array to the given bin file.
 * \param type bin file to write to (string)
 * \param var_name variable name to write to (string)
 * \param value to be written (double)
 * \param arr_index to write to
 * \param index to write to
 * \param active flag indicating if writing shall be performed (boolean).
 */
#define STORE_ARRAY_ELEM_MGR_WPR_INDEXED_CONDITIONAL(type, var_name, value, arr_index, index, active)                                           \
   {                                                                                                                                            \
      static int debug_index = -1;                                                                                                              \
      static void *pwriter   = NULL;                                                                                                            \
      debug_index            = AS_bin_mgr_store_debug_array_elem_nomnt(type, var_name, value, arr_index, index, active, debug_index, &pwriter); \
   }

/**
 * Stores an indexed value to an index of an array to the given bin file.
 * \param type bin file to write to (string)
 * \param var_name variable name to write to (string)
 * \param value to be written (double)
 * \param arr_index to write to
 * \param index to write to
 */
#define STORE_ARRAY_ELEM_MGR_WPR_INDEXED(type, var_name, value, arr_index, index) STORE_ARRAY_ELEM_MGR_WPR_INDEXED_CONDITIONAL(type, var_name, value, arr_index, index, BINWRITER_TRUE)

#endif