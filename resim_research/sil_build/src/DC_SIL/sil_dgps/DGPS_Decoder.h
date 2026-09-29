
#include <string>
#include <string_view>
#include "SMValidationInterfaceDefinition.h"
#include "DGPS_Msg_Data.h"

void initDgpsLibrary();
void updateInputConfig();
bool updateRunConfig();
void clearOutputFile();
void writeDetectionOfAllSensor();
DGPS_Data_T *GetDGPSDataPtr();
void Set_DGPS_Output_Path(const char *filename);
unsigned int GetZeroTimestampCountSRR();
unsigned int GetZeroTimestampCountMRR();
