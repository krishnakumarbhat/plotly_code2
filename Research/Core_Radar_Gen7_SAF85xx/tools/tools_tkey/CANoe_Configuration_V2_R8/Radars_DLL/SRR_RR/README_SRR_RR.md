**Release**    : 2

**Date**       : 19 Mar 2025

**Title**      : Seed and Key dll for **SRR_RR ECU**

**Description**: **"SeedKey_SRR_RR.dll"** is a single dll containing keys for 6 security levels.

**Security level 1**: 0x01 - General Diagnostics

**key**        : 99dce0232cae272b64ecc56acd2c9d4a44fe5d9c78c858fceed55b66b5b92790

**Security level 2**: 0x03 - Programming

**key**        : d502714285b7b803d9ebede77431830d4ccf8b85556b68519a87cf7c9b7cd304

**Security level 3**: 0x5F - Safety System

**key**        : 8259aa03cf6f516fb9cc27221e58e8eb3e936f070665562911f838beb56ab0f5

**Security level 4**: 0x07 - FOTA Dynamic Update

**key**        : 2056d9c47c61aaa172073489ba0b991e76841b5991aa4f2d714ef88402e77e38

**Security level 5**: 0x09 - EOL

**key**        : 4af3b092c26b3137a0e4350c80b62a0aaac31ce5eaa5601e9dc91ede8a154a74

**Security level 6**: 0x0B - Workshop Update

**key**        : 1304ddb950da6e07736bf7678d6b111eedb31b8426d904440844f86764f00c49

(Please verify the keys with the keys used in ECU)

Supplier needs to pass a 32-byte seed and expect a 32-byte key as output.

This is a Win32 Debug build.

Use: store in relevant folder and give its path in vFlash

------------------------------------------------------------------------------------------------------------------------
