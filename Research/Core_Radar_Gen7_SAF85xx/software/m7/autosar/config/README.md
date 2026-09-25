# Information on Manual Changes :

### Manual Change 1
**File Name** : E2EXf_LCfg.h

**Changes** : DET related macros "E2EXF_DEV_ERROR_DETECT" and "E2EXF_DEV_ERROR_REPORT" shall be maually changed as STD_OFF

**Reason** : This could be because every time someone generates using a different version DaVinci, it could be generating differently.

### Manual Change 2
**File Name** : Eth_30_Wrapper_Cfg.h

**Changes** : The macro "EthConf_EthCtrlConfig_RADAR_EthConnector1_Controller_bbe781c0" shall be kept commented

**Reason** : Manual Change to avoid MACRO redefinition error. MACRO is defined in MCAL. This shall be fixed from MCAL Side.
