@ECHO off

ECHO *******************************************************************************
REM DO NOT MODIFY THIS BATCH FILE
ECHO BINWRITER CMake build management bat	
ECHO This batch creates a compilable single project for BINWRITER. 	
ECHO. 
ECHO REQUIRED ENVIRONMENT VARIABLES:
ECHO  1) CUSTOMER: Name of the customer to consider. E.g. 'BMW, AUDI, JLR,...'	
ECHO  2) CMAKE_PATH: location of CMake executable. E.g.: C:\Tools\cmake-3.8.1\bin
ECHO                 INFO: is only required if cmake is not in 'system path' 	 
ECHO OPTIONAL ENVIRONMENT VARIABLES:
ECHO  3) OPTIONAL VARIABLES:
ECHO     a) MSVC_GENERATOR_NAME: MSVC compiler version to be used
ECHO        E.g.: - 'Visual Studio 12'       for visual studio 2013 x32 bit
ECHO              - 'Visual Studio 12 Win64' for visual studio 2013 x64 bit
ECHO              - 'Visual Studio 11'       for visual studio 2012 x32 bit
ECHO              - 'Visual Studio 11 Win64' for visual studio 2012 x64 bit
ECHO     b) CMAKE_BUILD_TYPE: Build configuration (Debug/Release) 'Default is Debug' 
ECHO     c) CMAKE_ENV: additional CMAKE variable in form '-DVARIABLE="VALUE"'
ECHO *******************************************************************************
ECHO.

REM Define source path
SET CURRENT_PATH=%CD%

REM Location to store the build
SET BUILD_PATH=%CURRENT_PATH%\build

REM Default compiler name
SET MSVC_GENERATOR_NAME=Visual Studio 12 Win64

REM Customer
IF [%CUSTOMER%] EQU [] (
	SET /p CUSTOMER=Enter a customer name:
)

REM check CMAKE Path
IF NOT EXIST "%CMAKE_PATH%" (
	for /f "tokens=*" %%a in ('where cmake') do set CMAKE_PATH=%%a
)
:CMAKE_PARAM
IF NOT EXIST "%CMAKE_PATH%" (
	SET /p CMAKE_PATH=Enter the CMake executable filepath 'e.g.: C:\Tools\cmake-3.8.1\bin\cmake.exe':
)
IF NOT EXIST "%CMAKE_PATH%" (
	ECHO Invalid cMake File
	GOTO CMAKE_PARAM
)

REM Set build type
IF "%CMAKE_BUILD_TYPE%"=="" (
	SET CMAKE_BUILD_TYPE=Debug
)
SET CMAKE_ENV=-DCUSTOMER="%CUSTOMER%"

REM Create build directory
IF EXIST %BUILD_PATH% (
	RMDIR /s /q %BUILD_PATH%
)
MKDIR %BUILD_PATH%
CD /d %BUILD_PATH%

ECHO.
ECHO CUSTOMER     : %CUSTOMER%
ECHO CONFIGURATION: %CMAKE_BUILD_TYPE%
ECHO PLATFORM     : %MSVC_GENERATOR_NAME%
ECHO.
ECHO - GENERATING BINWRITER BUILD
CALL "%CMAKE_PATH%" %CURRENT_PATH% -G"%MSVC_GENERATOR_NAME%" %CMAKE_ENV% || ( GOTO FAILED1 )

REM build generated project and solution
ECHO.
ECHO - COMPILING BINWRITER - START
%CMAKE_PATH% --build %CD% --config %CMAKE_BUILD_TYPE% --target %BUILD_PATH%\AS-bin-writer-lib || ( GOTO FAILED2 )
ECHO - COMPILING BINWRITER - END
ECHO.
GOTO END

:FAILED1
ECHO CMAKE got an error while generating the solution!
SET ERRORLEVEL=1

:FAILED2
ECHO CMAKE got an error while compiling!
SET ERRORLEVEL=1

:END
CD /d %CURRENT_PATH%
PAUSE