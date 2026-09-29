#ifndef DC_READ_CONFIG_H
#define DC_READ_CONFIG_H

#include <string>
#include <unordered_map>

typedef struct Input_Config_Tag {
   std::string DC_Library_XML_Version;
   std::string Customer_Name;
   std::string Sensor_Type;
   std::string UDP_Stream_Choice;
   std::string HDF_Files_Input;
   std::string HDF_Files_Output;
   std::string DC_IP_DQ_Chk_Print;
   std::string XTRK_Files;
   std::string BIN_Files;
   std::string DGPS_Decode_Status;
   std::string DGPS_SM_Config;
   int cust;
   int senstype;
} Input_Config_T;

void ReadConfigFromXML(const std::string &xmlpath);
Input_Config_T &getConfigParameters();
#endif