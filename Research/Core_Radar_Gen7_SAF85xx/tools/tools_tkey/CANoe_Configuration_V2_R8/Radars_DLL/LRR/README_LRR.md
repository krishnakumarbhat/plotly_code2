**Release**    : 2

**Date**       : 19 Mar 2025

**Title**      : Seed and Key dll for **LRR ECU**

**Description**: **"SeedKey_LRR.dll"** is a single dll containing keys for 6 security levels.

**Security level 1**: 0x01 - General Diagnostics

**key**        : 3ce2ebb10b076d47ba2d357fae497e0203514fcf0b7f3112815d34eeaa557cb1

**Security level 2**: 0x03 - Programming

**key**        : 4b503a96ebd7c34580ce013fb943ad94033768d6b763aa33bb0b8b33d1979c1a

**Security level 3**: 0x5F - Safety System

**key**        : 28ea924dc97a40e7bf91cbf04d997ee9b8b765a877c5309f661ef9dcc7cd42b5

**Security level 4**: 0x07 - FOTA Dynamic Update

**key**        : 9979565efce6f3557f6ad5635d326a030c1694fcd4d08a496b20749484bbee17

**Security level 5**: 0x09 - EOL

**key**        : f40c2f2ed46d8abe63b3c21d3c336f73c1926baac841214f2f4f9575dc432c7f

**Security level 6**: 0x0B - Workshop Update

**key**        : 9a46478ffd46e6707a533a171ec0abe259c1922a9c488e0f5c27510363c03be9

(Please verify the keys with the keys used in ECU)

Supplier needs to pass a 32-byte seed and expect a 32-byte key as output.

This is a Win32 Debug build.

Use: store in relevant folder and give its path in vFlash

------------------------------------------------------------------------------------------------------------------------
