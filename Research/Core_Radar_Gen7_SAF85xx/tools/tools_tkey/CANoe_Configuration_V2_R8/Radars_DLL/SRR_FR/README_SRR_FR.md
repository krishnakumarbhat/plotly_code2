**Release**    : 2

**Date**       : 19 Mar 2025

**Title**      : Seed and Key dll for **SRR_FR ECU**

**Description**: **"SeedKey_SRR_FR.dll"** is a single dll containing keys for 6 security levels.

**Security level 1**: 0x01 - General Diagnostics

**key**        : 769ed9a15f650c4976a794c271c005ae3621b203c1d93a5b2d1f11ead5b99bdb

**Security level 2**: 0x03 - Programming

**key**        : 5a9ff155b4f00121ed16d31c7dd31d3533acb00ecbae2b61b11dafde6e96d6e5

**Security level 3**: 0x5F - Safety System

**key**        : a3b5f9d561f449853ced14aca9b9e71c8d056fe85084c7ef51d95e1428bcb504

**Security level 4**: 0x07 - FOTA Dynamic Update

**key**        : 54fbb1086c780a129b8b0bafa20ed7763f8e6ba9728dfe6f2305701d75faf89d

**Security level 5**: 0x09 - EOL

**key**        : b482c6bb369ec8c95d1e8200f6102d94b94990315567bc697cb34c970e1bedc1

**Security level 6**: 0x0B - Workshop Update

**key**        : dfcc0357494493fb87683c8c469cab2afed113e71a4cc64940736e54f6eacb47

(Please verify the keys with the keys used in ECU)

Supplier needs to pass a 32-byte seed and expect a 32-byte key as output.

This is a Win32 Debug build.

Use: store in relevant folder and give its path in vFlash

------------------------------------------------------------------------------------------------------------------------
