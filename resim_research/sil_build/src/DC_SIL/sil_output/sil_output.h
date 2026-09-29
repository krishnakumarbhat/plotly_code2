#ifndef SIL_OUTPUT_H
#define SIL_OUTPUT_H

#include "dc_output_data.h"
#include "dc_config.h"

SIL_DC_Output_Data_T *GetOutputDataPtr();
SIL_DC_Output_Data_T *SetOutputDataPtr(SIL_DC_Output_Data_T *output_ptr);
void SetRecuOutputHeaderdata(void);
void InitMUDPStreams(Run_Mode_T run_mode);
void TransmitMUDPStreams(void);
#endif