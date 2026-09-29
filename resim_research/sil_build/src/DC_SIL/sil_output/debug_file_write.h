#pragma once
#include <string>
#include <string_view>

enum class Func_Call_LogFile_T {
   NONE,
   HDF_FILE_LOG,
   BIN_FILE_LOG,
   STATISTIC_FILE_LOG,
   DGPS_FILE_LOG,
};

void InitDebugFiles(Func_Call_LogFile_T LogFile);
void WriteDebugFiles(Func_Call_LogFile_T LogFile);
void ResetDebugFiles(Func_Call_LogFile_T LogFile, void *pLogFolder = nullptr, bool endFile = false);
void SetPathDebugFiles(Func_Call_LogFile_T LogFile, const char *OpFilepath);
void writeF360Bin();
