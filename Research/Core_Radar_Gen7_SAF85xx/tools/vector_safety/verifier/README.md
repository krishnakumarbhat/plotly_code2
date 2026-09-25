# WdgM Verifier Tool

This tool verifies the Watchdog Manager (WdgM) configuration for automotive functional safety compliance.

## Prerequisites

### 1. GCC MinGW Compiler Installation

1. **Download MinGW-w64**:
   - Visit: https://www.mingw-w64.org/downloads/
   - Download the installer for Windows

2. **Install MinGW**:
   - Install to: `C:\MinGW` (recommended)
    -Install mingw32-base
![alt text](image.png)
![alt text](image-1.png)


3. **Add to PATH Environment Variable**:
   - Open Edit the system environment variables Properties → Advanced → Environment Variables
   - Edit the `PATH` variable
   - Add: `C:\MinGW\bin`
   - Click OK to save
   ![alt text](image-2.png)

4. **Verify Installation**:
   ```cmd
   gcc --version
   ```
   Should display GCC version information.
![alt text](image-3.png)

### 2. Project Structure

Ensure the following directory structure exists:
```
C:\Gen7_V2\Core_Radar_Gen7_SAF85xx\
├── software\m7\autosar\sip\Components\WdgM\Verifier\
│   ├── Verifier_Tests\
│   │   ├── src\
│   │   │   ├── wdgm_verifier.c
│   │   │   ├── utilities.c
│   │   │   ├── edf_utilities.c
│   │   │   ├── block_a_tests.c
│   │   │   ├── block_b_tests.c
│   │   │   └── block_c_tests.c
│   │   └── inc\
│   ├── verify_wdgm_source.xsl
│   └── verify_wdgm_header.xsl
├── software\m7\autosar\sip\Components\WdgM\Verifier_Tools\xsltproc\
│   └── xsltproc.exe
├── software\m7\autosar\config\Config\ECUC\
│   └── Core_Radar_Gen7_SAF85xx_WdgM_WdgM_ecuc.arxml
├── software\m7\autosar\config\Appl\GenData\
│   └── WdgM_Cfg.c
└── tools\vector_safety\verifier\
    ├── run_wdgm_verifier.bat
    ├── wdgm_verifier_dependencies\
    └── README.md (this file)
```

## Usage

### Running the Verifier

1. **Navigate to the verifier directory**:
   ```cmd
   cd C:\Gen7_V2\Core_Radar_Gen7_SAF85xx\tools\vector_safety\verifier
   ```

2. **Execute the batch script**:
   ```cmd
   run_wdgm_verifier.bat
   ```
![alt text]({0CD2DE05-73AF-44D8-935D-1DE37EF5A1BC}.png)



### Input Files

The verifier uses the following input files (automatically detected):

- **ECUC Configuration**:
  - `C:\Gen7_V2\Core_Radar_Gen7_SAF85xx\software\m7\autosar\config\Config\ECUC\Core_Radar_Gen7_SAF85xx_WdgM_WdgM_ecuc.arxml`

- **Generated Configuration**:
  - `C:\Gen7_V2\Core_Radar_Gen7_SAF85xx\software\m7\autosar\config\Appl\GenData\WdgM_Cfg.c`

- **Verifier Test Sources**:
  - Located in: `C:\Gen7_V2\Core_Radar_Gen7_SAF85xx\software\m7\autosar\sip\Components\WdgM\Verifier\Verifier_Tests\src\`

### Output Files

The verifier generates the following outputs in: `C:\Gen7_V2\Core_Radar_Gen7_SAF85xx\bazel-bin\outputs\WdgM_Verifier\`

- **Verifier Report**: `Verifier_Report_Gen7_SAF.txt`
- **Compiled Executable**: `Verifier.exe`
- **Generated Sources**:
  - `wdgm_verifier_info.c`
  - `wdgm_verifier_info.h`
![alt text](image-4.png)
