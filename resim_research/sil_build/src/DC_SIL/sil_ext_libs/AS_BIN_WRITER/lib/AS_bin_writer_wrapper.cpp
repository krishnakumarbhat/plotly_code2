#include "AS_bin_writer_wrapper.h"
#include "binary_writer_lib.h"
#include <string>

/*
 * Project page is http://pep.usinkok.northamerica.delphiauto.net/projectdb/public/?page=project-tasks&pid=2733.5882
 * Jira Page is http://jiraprod1.delphiauto.net:8080/projects/AUM
 */

AS_bin_writer_manager bin_mgr;

static std::string AS_bww_MountingStrings[AS_BWW_MOUNTING_NUMBER_OF_MOUNTING_STRINGS] =
    {
        "UNKNOWN",
        "ARTIFICIAL_LOGFILE",
        "REAR_LEFT",
        "REAR_RIGHT",
        "FRONT_RIGHT",
        "FRONT_LEFT",
        "REAR_CENTER",
        "FRONT_CENTER",
        "LEFT_CENTER",
        "RIGHT_CENTER",
        "INVALID_POSITION",
        "DOMAIN_CONTROLLER_UNIT"};

static char AS_bww_IndexString[] = "index";

static char AS_bww_TimestampString[] = "log_timestamp";
static char AS_bww_ScanIndexString[] = "scan_index";

static char AS_bww_ID_String[] = "";

static unsigned int AS_bww_TimeStamp = 0;
static unsigned int AS_bww_ScanIndex = 0;

void AS_bww_init(const char *logPath) {
   std::string logPathCpp = std::string(logPath);
   std::size_t found      = logPathCpp.find_last_of("/\\");
   std::size_t found_dot  = logPathCpp.find_last_of(".");

   std::string logPathEnding = logPathCpp.substr(found_dot + 1, logPathCpp.length());
   std::size_t ExtensionDvsu = logPathEnding.compare("dvsu");

   std::string log_name;

   if (0 == ExtensionDvsu) {
      /* This is a dvsu file */
      /* Check if its a sensor specific file that has '_7#' appended (# stands for a second digit) */
      std::string logFileSensorSpecific        = logPathCpp.substr(found_dot - 3, 2);
      std::size_t logFileSensorSpecificCompare = logFileSensorSpecific.compare("_7");
      if (0 == logFileSensorSpecificCompare) {
         found_dot = found_dot - 3; /* Adjust found_dot to remove '_7#.dvsu' */
      }
   }

   log_name = logPathCpp.substr(0, found_dot); /* Remove the existing file extension */

   std::string log_name_with_extension = log_name + '.' + logPathEnding;
   std::string fileName                = logPathCpp.substr(found + 1, log_name.length());

   bin_mgr.set_log_and_id_str(log_name_with_extension.c_str(), AS_bww_ID_String);
   bin_mgr.new_logfile(log_name_with_extension.c_str());
}

void AS_bww_SetMounting(const char *mntStr) {
   bin_mgr.set_mounting(mntStr);
}

std::string Get_AS_bww_mointing_string(AS_BWW_MOUNTING_POSITIONS_T RadarPosition) {
   std::string result = AS_bww_MountingStrings[0];
   if ((int)RadarPosition < AS_BWW_MOUNTING_NUMBER_OF_MOUNTING_STRINGS) {
      result = AS_bww_MountingStrings[RadarPosition];
   }
   return result;
}

void AS_bww_SetMounting_RP(AS_BWW_MOUNTING_POSITIONS_T RadarPosition) {
   std::string result = Get_AS_bww_mointing_string(RadarPosition);
   AS_bww_SetMounting(result.c_str());
}

void AS_bww_SetTimeStamp(unsigned int time_stamp) {
   AS_bww_TimeStamp = time_stamp;
}

void AS_bww_SetScanIndex(unsigned int scan_index) {
   AS_bww_ScanIndex = scan_index;
}

void AS_bww_SetListOfSuppressedBinFiles(
    const char **type,
    unsigned int length) {
   unsigned int i = 0;

   for (i = 0; i < length; i++) {
      bin_mgr.set_SuppressedOutputFiles(type[i]);
   }
}

void AS_bww_close_file(AS_BWW_MOUNTING_POSITIONS_T radarPosition) {
   std::string position           = Get_AS_bww_mointing_string(radarPosition);
   std::vector<std::string> names = bin_mgr.get_OutputFilesAtPosition(position);

   for (size_t i = 0; i < names.size(); i++) {
      const char *OutputName = names[i].c_str();
      void *ppwriter         = &bin_mgr.get_obj(OutputName, position.c_str());

      AS_bin_writer_lib *pwriter = static_cast<AS_bin_writer_lib *>(ppwriter);
      if (pwriter == NULL) {
         printf("AS_bww_close_file(): No file available to close.");
         return;
      }

      if (pwriter->is_open()) {
         pwriter->close_file();
         pwriter->set_next_run();
      }
   }
}

void AS_bww_suppress_all_bin_files(unsigned char state) {
   bin_mgr.SuppressAllBinFiles(!(0 == state));
}

void AS_bww_write_line(
    const unsigned int timeStamp,
    const unsigned int ScanIndex,
    AS_BWW_MOUNTING_POSITIONS_T radarPosition) {
   std::string position = Get_AS_bww_mointing_string(radarPosition);

   std::vector<std::string> names = bin_mgr.get_OutputFilesAtPosition(position);

   for (size_t i = 0; i < names.size(); i++) {
      const char *OutputName = names[i].c_str();
      void *ppwriter         = &bin_mgr.get_obj(OutputName, position.c_str());

      AS_bin_writer_lib *pwriter = static_cast<AS_bin_writer_lib *>(ppwriter);
      if (pwriter == NULL) {
         printf("AS_bww_write_line(): No file available to write to");
         return;
      }

      if (pwriter->is_active()) {
         int numSensors = pwriter->get_num_sensors();
         for (int j = 0; j < numSensors; j++) {
            pwriter->store_debug_value(AS_bww_TimestampString, timeStamp, j, true, -1);
            pwriter->store_debug_value(AS_bww_ScanIndexString, ScanIndex, j, true, -1);

            if (numSensors > 1) {
               pwriter->store_debug_value(AS_bww_IndexString, j, j, true, -1);
            }
         }
         pwriter->write_line();
      }
   }
}

/************************************************/
/* C interface */
/************************************************/

/** Versions with direct specification of mounting string */

int AS_bin_mgr_store_debug_value(
    const char *const type,
    const char *const mounting,
    const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN],
    double value,
    int radar_index,
    unsigned char active,
    int debug_index,
    void **_ppwriter) {
   AS_bin_writer_lib *pwriter = static_cast<AS_bin_writer_lib *>(*_ppwriter);

   if (pwriter == NULL) {
      pwriter    = &bin_mgr.get_obj(type, mounting);
      *_ppwriter = static_cast<void *>(pwriter);
   }
   if (pwriter->is_active()) {
      return pwriter->store_debug_value(var_name, value, radar_index, active, debug_index);
   } else {
      return -1;
   }
}

int AS_bin_mgr_store_debug_array_elem(
    const char *const type,
    const char *const mounting,
    const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN],
    double value,
    int arr_index,
    int radar_index,
    unsigned char active,
    int debug_index,
    void **_ppwriter) {
   AS_bin_writer_lib *pwriter = static_cast<AS_bin_writer_lib *>(*_ppwriter);

   if (pwriter == NULL) {
      pwriter    = &bin_mgr.get_obj(type, mounting);
      *_ppwriter = static_cast<void *>(pwriter);
   }
   if (pwriter->is_active()) {
      return pwriter->store_debug_array_elem(var_name, value, arr_index, radar_index, active, debug_index);
   } else {
      return -1;
   }
}

/** Versions without direct specification of mounting string.
 * AS_bin_writer_manager::set_mounting()  must be used to  set the mounting position beforehand */
int AS_bin_mgr_store_debug_value_nomnt(
    const char *const type,
    const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN],
    double value,
    int radar_index,
    unsigned char active,
    int debug_index,
    void **_ppwriter) {
   AS_bin_writer_lib *pwriter = static_cast<AS_bin_writer_lib *>(*_ppwriter);

   if (pwriter == NULL) {
      pwriter    = &bin_mgr.get_obj(type);
      *_ppwriter = static_cast<void *>(pwriter);
   }
   if (pwriter->is_active()) {
      return pwriter->store_debug_value(var_name, value, radar_index, active, debug_index);
   }
   return -1;
}

int AS_bin_mgr_store_debug_array_elem_nomnt(
    const char *const type,
    const char var_name[AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN],
    double value,
    int arr_index,
    int radar_index,
    unsigned char active,
    int debug_index,
    void **_ppwriter) {
   AS_bin_writer_lib *pwriter = static_cast<AS_bin_writer_lib *>(*_ppwriter);

   if (pwriter == NULL) {
      pwriter    = &bin_mgr.get_obj(type);
      *_ppwriter = static_cast<void *>(pwriter);
   }
   if (pwriter->is_active()) {
      return pwriter->store_debug_array_elem(var_name, value, arr_index, radar_index, active, debug_index);
   } else {
      return -1;
   }
}
