@ECHO OFF
REM ================================================================================
REM BuildAll.bat - Windows batch script to build both SRR and MRR DC sequentially
REM Mirrors the functionality of BuildAll.sh for Linux
REM ================================================================================

SETLOCAL EnableExtensions DisableDelayedExpansion

set CMAKE_BUILD_TYPE=Release

echo.
echo ====================================================================
echo Building DC-SIL: SRR + MRR (Release)
echo ====================================================================
echo.

REM ================================================================================
REM Step 1: Clean and Build SRR_DC
REM ================================================================================
echo.
echo === Step 1: Building srr_dc ===
echo.

if exist build (
    echo Cleaning existing build folder...
    rmdir /s /q build
)

mkdir build
cd build

echo Running CMake for SRR...
cmake .. -DCMAKE_BUILD_TYPE=%CMAKE_BUILD_TYPE% -DBUILD_MODE=srr_dc -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -Wno-deprecated
if %errorlevel% neq 0 (
    echo ERROR: CMake configuration failed for srr_dc
    exit /b 1
)

echo Building SRR...
cmake --build . --config %CMAKE_BUILD_TYPE%
if %errorlevel% neq 0 (
    echo ERROR: Build failed for srr_dc
    exit /b 1
)

cd ..

REM Save SRR libraries temporarily
echo Saving SRR libraries...
if not exist temp_srr_libs (
    mkdir temp_srr_libs
)
if exist build\Release\SRR_DC_SIL_LIB.lib (
    copy /Y build\Release\SRR_DC_SIL_LIB.lib temp_srr_libs\
    echo Saved SRR_DC_SIL_LIB.lib
)
if exist build\Release\SRR_DC_SIL_LIB.dll (
    copy /Y build\Release\SRR_DC_SIL_LIB.dll temp_srr_libs\
    echo Saved SRR_DC_SIL_LIB.dll
)
if exist build\Release\SRR_DC_SIL_LIB.exp (
    copy /Y build\Release\SRR_DC_SIL_LIB.exp temp_srr_libs\
)

REM Wait a moment for file handles to release
timeout /t 1 /nobreak

echo Cleaning build folder for MRR...
rmdir /s /q build
if %errorlevel% neq 0 (
    echo ERROR: Could not clean build folder
    exit /b 1
)

REM ================================================================================
REM Step 2: Clean and Build MRR_DC
REM ================================================================================
echo.
echo === Step 2: Building mrr_dc ===
echo.

echo Creating fresh build folder for MRR...
mkdir build
cd build

echo Running CMake for MRR...
cmake .. -DCMAKE_BUILD_TYPE=%CMAKE_BUILD_TYPE% -DBUILD_MODE=mrr_dc -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -Wno-deprecated
if %errorlevel% neq 0 (
    echo ERROR: CMake configuration failed for mrr_dc
    cd ..
    exit /b 1
)

echo Building MRR...
cmake --build . --config %CMAKE_BUILD_TYPE%
if %errorlevel% neq 0 (
    echo ERROR: Build failed for mrr_dc
    cd ..
    exit /b 1
)

cd ..

REM ================================================================================
REM Step 3: Copy SRR libraries back to final build folder
REM ================================================================================
echo.
echo === Step 3: Combining SRR and MRR libraries in build folder ===
echo.

if exist temp_srr_libs (
    if not exist build\Release (
        mkdir build\Release
    )
    
    echo Copying SRR libraries to build\Release\
    if exist temp_srr_libs\SRR_DC_SIL_LIB.lib (
        copy /Y temp_srr_libs\SRR_DC_SIL_LIB.lib build\Release\
        echo   ✓ Copied SRR_DC_SIL_LIB.lib
    )
    if exist temp_srr_libs\SRR_DC_SIL_LIB.dll (
        copy /Y temp_srr_libs\SRR_DC_SIL_LIB.dll build\Release\
        echo   ✓ Copied SRR_DC_SIL_LIB.dll
    )
    if exist temp_srr_libs\SRR_DC_SIL_LIB.exp (
        copy /Y temp_srr_libs\SRR_DC_SIL_LIB.exp build\Release\
    )
    
    REM Clean up temporary folder
    rmdir /s /q temp_srr_libs
) else (
    echo WARNING: Could not find saved SRR libraries
)

REM ================================================================================
REM Build Complete
REM ================================================================================
echo.
echo ====================================================================
echo Build Complete!
echo ====================================================================
echo.
echo Final Build Folder: build\Release\
echo.
echo Generated Libraries:
if exist build\Release\SRR_DC_SIL_LIB.lib (
    echo   ✓ SRR_DC_SIL_LIB.lib
)
if exist build\Release\SRR_DC_SIL_LIB.dll (
    echo   ✓ SRR_DC_SIL_LIB.dll
)
if exist build\Release\MRR_DC_SIL_LIB.lib (
    echo   ✓ MRR_DC_SIL_LIB.lib
)
if exist build\Release\MRR_DC_SIL_LIB.dll (
    echo   ✓ MRR_DC_SIL_LIB.dll
)
echo.
echo Both SRR and MRR have been built and combined in: build\Release\
echo.

ENDLOCAL
exit /b 0
