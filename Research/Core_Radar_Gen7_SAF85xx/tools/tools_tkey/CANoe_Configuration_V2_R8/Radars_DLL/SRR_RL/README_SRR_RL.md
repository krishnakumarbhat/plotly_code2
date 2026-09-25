**Release**    : 2

**Date**       : 19 Mar 2025

**Title**      : Seed and Key dll for **SRR_RL ECU**

**Description**: **"SeedKey_SRR_RL.dll"** is a single dll containing keys for 6 security levels.

**Security level 1**: 0x01 - General Diagnostics

**key**        : beb46600482545fb3a4240aa4f92a2c25918194bf1495cf349e46e9316311f9a

**Security level 2**: 0x03 - Programming

**key**        : beabc98c0d8fb3c1e4f9e530cefc647d3cf85fff18ac4dbaf8ed93b9476c8959

**Security level 3**: 0x5F - Safety System

**key**        : 3cc8769c70df68496c555a557d63d43be18bb53b3539d36dc928c879234897c1

**Security level 4**: 0x07 - FOTA Dynamic Update

**key**        : 38e0669a517b3a435fb0792fdcb97d353b7f53cbbdc9fb634d72399c42634927

**Security level 5**: 0x09 - EOL

**key**        : bd9b7ee4e8f0d83b84531eaccb18a129c9ef4d932d4c05adfa36731ea124e2ab

**Security level 6**: 0x0B - Workshop Update

**key**        : 00a058a5b5ed2df06be417ee6d2321211be4e4f34879df2b511c9099a55f7c49

(Please verify the keys with the keys used in ECU)

Supplier needs to pass a 32-byte seed and expect a 32-byte key as output.

This is a Win32 Debug build.

Use: store in relevant folder and give its path in vFlash

------------------------------------------------------------------------------------------------------------------------
