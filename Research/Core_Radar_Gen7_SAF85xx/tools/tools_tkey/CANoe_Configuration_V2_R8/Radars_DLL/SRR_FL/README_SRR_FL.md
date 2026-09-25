**Release**    : 2

**Date**       : 19 Mar 2025

**Title**      : Seed and Key dll for **SRR_FL ECU**

**Description**: **"SeedKey_SRR_FL.dll"** is a single dll containing keys for 6 security levels.

**Security level 1**: 0x01 - General Diagnostics

**key**        : a1c5c33096caf206bc01a3779125831d72fcfe60553ba1ef0fd19d89860f52f0

**Security level 2**: 0x03 - Programming

**key**        : bbf9119523cdb0b81af218cbb36b2668c482067ad94870d0d0598893c3a10e9e

**Security level 3**: 0x5F - Safety System

**key**        : 65c04ead18dc0fffb659a9e0ef6b5846a71805545761a4448048a25d4fa71e5a

**Security level 4**: 0x07 - FOTA Dynamic Update

**key**        : df2022d986d0aa5f5ac1e023ba2f911c2e86c842531c1cd4b13fbb451a2bd769

**Security level 5**: 0x09 - EOL

**key**        : fbf6dbd1dbfb223e5c42b0d3425f95d768b2d9361f0f9cd377ba82b12b47077e

**Security level 6**: 0x0B - Workshop Update

**key**        : c6b33d28c503bc35d9c3dcef16fc39156a4a73e86e0b0dcfb8d0430c99dd6f70

(Please verify the keys with the keys used in ECU)

Supplier needs to pass a 32-byte seed and expect a 32-byte key as output.

This is a Win32 Debug build.

Use: store in relevant folder and give its path in vFlash

------------------------------------------------------------------------------------------------------------------------
