@ECHO OFF
SETLOCAL EnableExtensions DisableDelayedExpansion

:: ==== Build Type Selection ====
:buildtype_prompt
echo Select Build Type :
echo 1. Release
echo 2. Debug
set /p buildtype=Enter choice (1 or 2): 

if "%buildtype%"=="1" (
    set CMAKE_BUILD_TYPE=Release
) else if "%buildtype%"=="2" (
    set CMAKE_BUILD_TYPE=Debug
) else (
    echo Invalid choice. Please enter 1 or 2.
    goto buildtype_prompt
)

:: ==== DC to Build ====
:buildmode_prompt
echo Select DC type to build :
echo 1. srr_dc
echo 2. mrr_dc
set /p buildmode=Enter choice (1 or 2): 

if "%buildmode%"=="1" (
    set CMAKE_BUILD_MODE=srr_dc
) else if "%buildmode%"=="2" (
    set CMAKE_BUILD_MODE=mrr_dc
) else (
    echo Invalid choice. Please enter 1 or 2.
    goto buildmode_prompt
)

:: ==== Optional: Clean Build ====
echo Do you want to clean the build folder? (y/n)
set /p cleanbuild=Enter choice: 
if /i "%cleanbuild%"=="y" (
    if exist build (
        rmdir /s /q build
        echo Build folder cleaned.
    )
)

if not exist build mkdir build
cd build

cmake .. -DCMAKE_BUILD_TYPE=%CMAKE_BUILD_TYPE% -DBUILD_MODE=%CMAKE_BUILD_MODE% -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -Wno-deprecated
if %errorlevel% neq 0 (
    echo CMake Configuration failed.
    pause
    exit /b %errorlevel%
)

cmake --build . --config %CMAKE_BUILD_TYPE%
if %errorlevel% neq 0 (
    echo Build failed.
    pause
    exit /b %errorlevel%
)

echo.
echo Build Successful!
pause
