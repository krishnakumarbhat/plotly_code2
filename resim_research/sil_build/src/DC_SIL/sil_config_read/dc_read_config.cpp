#include <iostream>
#include <cstring>
#include <filesystem>
#ifdef __GNUC__
#include <dlfcn.h>
#include <libgen.h>
#else
#include <Windows.h>
#include <libloaderapi.h>
#include <stdlib.h>
#endif
#include "dc_read_config.h"
#include "DCCustomer.h"
#include "DCSensorType.h"
#include "pugixml.hpp"

#define PATH_MAX (4096)
namespace fs = std::filesystem;
static Input_Config_T xml_config;
void DefaultConfigParameters(void);

std::unordered_map<std::string, DCCustomer> customer_map = {
    {"BMW", DC_CUSTOMER_BMW},
    {"FORD", DC_CUSTOMER_FORD},
    {"CHANGAN", DC_CUSTOMER_CHANGAN},
    {"RNA", DC_CUSTOMER_RNA},
    {"SCANIA", DC_CUSTOMER_SCANIA},
    {"NISSAN", DC_CUSTOMER_NISSAN},
    {"HONDA", DC_CUSTOMER_HONDA},
    {"HKMC", DC_CUSTOMER_HKMC},
    {"TML", DC_CUSTOMER_TML},
    {"STLA", DC_CUSTOMER_STLA},
    {"MTNL", DC_CUSTOMER_MTNL},
    {"TRATON", DC_CUSTOMER_TRATON},
    {"CEER", DC_CUSTOMER_CEER},
    {"GPO", DC_CUSTOMER_GPO}};
std::unordered_map<std::string, DCSensorType> sensor_map = {
    {"SRR5Plus", DC_SENSOR_TYPE_SRR5_PLUS},
    {"MRR3", DC_SENSOR_TYPE_MRR3},
    {"SRR5", DC_SENSOR_TYPE_SRR5},
    {"SRR3", DC_SENSOR_TYPE_SRR3},
    {"FLR4", DC_SENSOR_TYPE_FLR4},
    {"SRR6Plus", DC_SENSOR_TYPE_SRR6_PLUS},
    {"SRR6", DC_SENSOR_TYPE_SRR6},
    {"FLR4Plus", DC_SENSOR_TYPE_FLR4_PLUS},
    {"SRR7Plus", DC_SENSOR_TYPE_SRR7_PLUS},
    {"FLR7", DC_SENSOR_TYPE_FLR7},
    {"FLR8", DC_SENSOR_TYPE_FLR8},
    {"SRR8Plus", DC_SENSOR_TYPE_SRR8_PLUS},
    {"SRR7PlusUWB", DC_SENSOR_TYPE_SRR7_PLUS_UWB}};

Input_Config_T &getConfigParameters() {
   return xml_config;
}

void getDCLibraryInfo(char *pSharedObjectPath, char *pSharedObjectFileName, char *pSharedObjectName, char *pWorkingDir) {
#ifdef __GNUC__
   Dl_info dlInfo;
   dladdr(__builtin_extract_return_addr(__builtin_return_address(0)), &dlInfo);

   if (dlInfo.dli_sname != NULL && dlInfo.dli_saddr != NULL) {
      char path_copy[PATH_MAX]        = {'\0'};
      char sharedObjectPath[PATH_MAX] = {'\0'};
      strncpy(path_copy, dlInfo.dli_fname, sizeof(path_copy) - 1);
      strcpy(sharedObjectPath, dirname(path_copy));

      char path_copy2[PATH_MAX]           = {'\0'};
      char sharedObjectFileName[PATH_MAX] = {'\0'};
      strncpy(path_copy2, dlInfo.dli_fname, sizeof(path_copy) - 1);
      strcpy(sharedObjectFileName, basename(path_copy2));

      char *fileExtStringPointer;
      char sharedObjectName[PATH_MAX] = {'\0'};
      fileExtStringPointer            = strrchr(sharedObjectFileName, '.');
      strncpy(sharedObjectName, sharedObjectFileName, fileExtStringPointer - sharedObjectFileName);

      char workingDir[PATH_MAX] = {'\0'};
      strcpy(workingDir, getenv("PWD"));

      strcpy(pSharedObjectPath, sharedObjectPath);
      strcpy(pSharedObjectFileName, sharedObjectFileName);
      strcpy(pSharedObjectName, sharedObjectName);
      strcpy(pWorkingDir, workingDir);

   } else // if (dlInfo.dli_sname != NULL && dlInfo.dli_saddr != NULL)
   {
      pSharedObjectPath     = NULL;
      pSharedObjectFileName = NULL;
      pSharedObjectName     = NULL;
      pWorkingDir           = NULL;

   } // if (dlInfo.dli_sname != NULL && dlInfo.dli_saddr != NULL)
#else
   char filename[1024];
   std::string dll_name = DomainController; // this is defined in CMakeList
   dll_name += ".dll";
   if (GetModuleFileNameA(GetModuleHandle(dll_name.c_str()), filename, sizeof(filename))) {
      char path_copy[PATH_MAX]            = {'\0'};
      char psharedObjectPath[PATH_MAX]    = {'\0'};
      char sharedObjectPath[PATH_MAX]     = {'\0'};
      char path_copy2[PATH_MAX]           = {'\0'};
      char sharedObjectFileName[PATH_MAX] = {'\0'};
      char *fileExtStringPointer;
      char sharedObjectName[PATH_MAX] = {'\0'};
      char workingDir[PATH_MAX]       = {'\0'};
      // strcpy(workingDir, getenv("cd"));

      _splitpath_s(filename, path_copy, psharedObjectPath, sharedObjectFileName, path_copy2);
      strcpy_s(sharedObjectPath, path_copy);
      strcat_s(sharedObjectPath, psharedObjectPath);
      strcat_s(sharedObjectFileName, path_copy2);
      fileExtStringPointer = strrchr(sharedObjectFileName, '.');
      strncpy_s(sharedObjectName, sharedObjectFileName, fileExtStringPointer - sharedObjectFileName);

      strcpy_s(pSharedObjectPath, PATH_MAX, sharedObjectPath);
      strcpy_s(pSharedObjectFileName, PATH_MAX, sharedObjectFileName);
      strcpy_s(pSharedObjectName, PATH_MAX, sharedObjectName);
      // strcpy(pWorkingDir, workingDir);
   }
#endif
}

void ReadConfigFromXML(const std::string &xmlpath) {
   std::string configPath;
   if (xmlpath == "NONE") {

      char SharedObjectPath[PATH_MAX];
      char SharedObjectFileName[PATH_MAX];
      char SharedObjectName[PATH_MAX];
      char WorkingDir[PATH_MAX];
#ifdef SRR_DC
      std::string filename = "SRR_DC_Lib_Control.xml";
#elif MRR_DC
      std::string filename = "MRR_DC_Lib_Control.xml";
#endif

      getDCLibraryInfo(SharedObjectPath, SharedObjectFileName, SharedObjectName, WorkingDir);
      std::string fullXMLpath = std::string(SharedObjectPath) + "/" + filename;
      if (fs::exists(fullXMLpath)) {
         configPath = fullXMLpath;

      }

      else {
         DefaultConfigParameters();
         std::cout << "[DC WARN]: Config XMl  not found, using the default values" << std::endl;
         return;
      }
   } else {
      configPath = xmlpath;
   }
   pugi::xml_document doc;
   pugi::xml_parse_result result = doc.load_file(configPath.c_str());
   pugi::xml_node root           = doc.child("Embedded_Library_Output_Control");

   xml_config.DC_Library_XML_Version = root.child("DC_Library_XML_Version").text().as_string();
   xml_config.Customer_Name          = root.child("Customer_Name").text().as_string();
   xml_config.Sensor_Type            = root.child("Sensor_Type").text().as_string();
   xml_config.UDP_Stream_Choice      = root.child("UDP_Stream_to_use").text().as_string();
   pugi::xml_node hdf_node           = root.child("HDF_FILES");
   xml_config.HDF_Files_Input        = hdf_node.child("INPUT").text().as_string();
   xml_config.HDF_Files_Output       = hdf_node.child("OUTPUT").text().as_string();

   xml_config.DC_IP_DQ_Chk_Print = root.child("DC_IP_DQ_Chk_Print").text().as_string();
   xml_config.XTRK_Files         = root.child("XTRK_FILES").text().as_string();
   xml_config.BIN_Files          = root.child("BIN_FILES").text().as_string();
   xml_config.DGPS_Decode_Status = root.child("DGPS_Decode_Status").text().as_string();
   xml_config.DGPS_SM_Config     = root.child("DGPS_SM_Config").text().as_string();
   xml_config.cust               = customer_map[xml_config.Customer_Name];
   xml_config.senstype           = sensor_map[xml_config.Sensor_Type];
}
void DefaultConfigParameters() {
   xml_config.Customer_Name = "BMW";
#ifdef SRR_DC
   xml_config.Sensor_Type = "SRR7Plus";
#elif MRR_DC
   xml_config.Sensor_Type = "FLR7";
#endif
   xml_config.XTRK_Files         = "DISABLE";
   xml_config.HDF_Files_Input    = "DISABLE";
   xml_config.HDF_Files_Output   = "DISABLE";
   xml_config.UDP_Stream_Choice  = "Gen7";
   xml_config.DC_IP_DQ_Chk_Print = "DISABLE";
   xml_config.BIN_Files          = "DISABLE";
   xml_config.DGPS_Decode_Status = "DISABLE";
   xml_config.cust               = customer_map[xml_config.Customer_Name];
   xml_config.senstype           = sensor_map[xml_config.Sensor_Type];
}