#ifndef SHARED_TOOLBOX_CONTENT_H
#define SHARED_TOOLBOX_CONTENT_H
#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

/*********** List of solved jira tickets ************************************/
#define AIG
#define AIG_1 /*There shall be a function calculating the distance between points*/
#define AIG_2 /*There shall be a function rotating vectors around the origin */
#define AIG_3 /*There shall be a function multiplying a vector with a scalar*/
#define AIG_4 /*There shall be a function  adding vectors*/
#define AIG_5 /*There shall be a function calculating the middle point between to points*/
#define AIG_6 /*There shall be a function calculating the length of a vector*/
#define AIG_9 /*There shall be a function calculating the perpendicular vector*/
#define AIG_10 /*There shall be a function normalizing a vector*/
#define AIG_15 /*There shall be a function calculating the scalar product between two vectors*/
#define AIG_29 /*Header to all Function*/
#define AIG_32 /*Set up DoxyGen to generate documentation for the Shared Toolbox*/
#define AIG_33 /*Create a small VS Unittest Project*/
#define AIG_35 /*Scalar product between two vectors*/
#define AIG_36 /*It should be possible to project a vector*/
#define AIG_37 /*Compute the length between two points*/
#define AIG_38 /*Create property pages */
#define AIG_39 /*It should be possible to create a norm vector from an angel*/
#define AIG_41 /*Create Base class for the test classes*/
#define AIG_43 /*Creating different project configurations for all customer*/
#define AIG_44 /*Integrate Shared Toolbox*/
#define AIG_45 /*'fast math tables: asin and acos'*/
#define AIG_46 /*Create functions to convert from Angle_T to Vector_2d_T*/
#define AIG_53 /*Provide version number structure definition*/
#define AIG_54 /*CMake: Do not ask for cmake path if detectable*/
#define AIG_60 /*Rename Vector_2d_Alg_Set_Length_to_1 => Vector_2d_Alg_Normalize_Vector*/
#define AIG_65 /*Move Delphi templates into Delphi sub folder*/
#define AIG_66 /*Create VS Template for tracker detection flag checking*/
#define AIG_71 /*Fix CMake build for AIG-53*/
#define AIG_72 /*'Use AS_Process_Tools for shared toolbox'*/
#define AIG_73 /*'Offer possibility to check for a minimum toolbox version'*/
#define AIG_74 /*'Create Unit Tests for Version Check module'*/
#define AIG_75 /*'make VERSION NUMBER more flexible to allow all modules to check their dependencies'*/
#define AIG_76 /*Use SHARED_TOOLBOX_COMPUTE_VERSION_INTEGER inside SHARED_TOOLBOX_VERSION_INSUFFICIENT_CHECK*/
#define AIG_78 /*'Create functions to extend ranges'*/
#define AIG_80 /*'Create function to init ranges'*/
#define AIG_82 /*'SWAP'*/
#define AIG_83 /*'Sieve'*/
#define AIG_87 /*Solve compiler warnings level 4*/
#define AIG_88 /*Does_Float_Range_Overlap_Int_Range calls wrong function*/
#define AIG_89 /*Additional sieve functions*/
#define AIG_90 /*Adjust name of Create_I_Range and Create_F_Range*/
#define AIG_91 /*Extend unit tests for min and max macro to test INFINITY*/
#define AIG_94 /*Create functions to round from float to int*/
#define AIG_95 /*Basic_Math_Factory: #include */
#define AIG_96 /*INFINITY definition check*/
#define AIG_97 /*Generate macro for FAST_ACOS*/
#define AIG_98 /*Do not use roundf()*/
#define AIG_100 /*Adjust embedded compiler warning for INFINITY undefined */
#define AIG_101 /*CMake: Default to MSVS 11*/
#define AIG_102 /*Only redefine Min and Max if they do not exist*/
#define AIG_103 /*Rename folders to make clearer what they contain*/
#define AIG_104 /*Make sure min Min and Max are defined*/
#define AIG_106 /*Create tool for runtime parameter handling*/
#define AIG_107 /*runtime parameter min max checks*/
#define AIG_110 /*Bring code coverage for runtime parameters to 100%*/
#define AIG_111 /*Offer bat file that produces unit test coverage report locally.*/
#define AIG_118 /*add FAST_SIN to fast math functions in Math_Selector.h*/
#define AIG_119 /*Rename Unit_Test project*/
#define AIG_120 /*Move getSlope to shared toolbox*/
#define AIG_121 /*Move testIntersect to shared toolbox*/
#define AIG_123 /*Create function Does_Float_Range_Overlap_Float_Range*/
#define AIG_126 /*create set_runtime_parameter functions*/
#define AIG_127 /*map cal properties for the legal state*/
#define AIG_128 /* PLATFORM_RACERUNNER*/
#define AIG_129 /*Offer Macros to suppress MSVS compiler warnings*/
#define AIG_130 /*Solve min and max compiler warnings*/
#define AIG_132 /*Provide mock calibration.h and Reuse.h files */
#define AIG_133 /*Move getCross to shared toolbox*/
#define AIG_135 /*Refactor function for checking if an interval A is a subset of interval B*/
#define AIG_137 /*Provide cmake functions to collect source and auxiliary files*/
#define AIG_138 /*Create functions to collect include directories*/
#define AIG_139 /*Clean up handling of Reuse.h and Calibration.h*/
#define AIG_145 /*[Unit Tests] Rename cpp header to hpp*/
#define AIG_150 /*Support stand alone build with CUSTOMER beeing set*/
#define AIG_151 /*Move definition of INFINITY into separate .h file*/
#define AIG_152 /*Solve compiler warning alignment of a member was sensitive to packing for RUNTIME_PARAMETER structures*/
#define AIG_154 /*Adapt implementation of NormalizeAngle*/
#define AHI_156 /*make customer folder for Post and Prerun */
#define AIG_164 /*Check for _MSC_VER before using microsoft specialties*/
#define AIG_167 /*Remove customer dependant MSVS settings*/
#define AIG_181 /*move handle_uint8 to shared toolbox*/
#define AIG_183 /*Move arithmetic saturation functions to own module*/
#define AIG_184 /*implement saturated math functions for all datatypes*/
#define AIG_193 /*Provide functionality to check if a version number is exactly equal a desired version number*/
#define AIG_194 /*AS_Unit_Test option to not make tracker api and bin writer*/
#define AIG_195 /*Use target_include_directories*/
#define AIG_196 /*AS_Unit_Test generate bat file for shared toolbox unit test coverage as well*/
#define AIG_198 /*Unit tests for SHARED_TOOLBOX_VERSION_EQUALS*/
#define AIG_200 /*Unit test Mock files*/
#define AIG_201 /*Move unit test folder outside shared_toolbox folder*/
#define AIG_202 /*Move Mock files into AS_Unit_Test*/
#define AIG_203 /*Add missing unit tests*/
#define AIG_204 /*Angle ranges*/
#define AIG_206 /*Insert Correct Typedef for int64_T/uint64_T in ReUse.h*/
#define AIG_207 /*Move trackers make_nonzero function into the toolbox*/
#define AIG_208 /*cmake functions for compiler warning management*/
#define AIG_209 /*Link PUBLIC to SRR_CORE_LIB and SIL_Library*/
#define AIG_210 /*AS_Unit_Test use AS-bin-writer-lib instead of BINWRITER*/
#define AIG_211 /*Create function to handle customer dependant differences*/
#define AIG_213 /*Add existing files to project tree*/
#define AIG_214 /*Add unit tests for expected type sizes*/
#define AIG_215 /*Missing include in Math_Infinity.h*/
#define AIG_217 /*Solve compiler warnings in unit tests for unused local variables*/
#define AIG_220 /*SDD generation using CMake*/
#define AIG_221 /*Usage of Shared_Toolbox_PATH*/
#define AIG_223 /*Option to suppress the warning for deprecated common include .cmake file*/
#define AIG_224 /*Provide macro for ceiled quotient*/
#define AIG_225 /*Create customer target for unit test coverage*/
#define AIG_227 /*Stick to typed defined in coding guideline*/
#define AIG_228 /*Create a customer independent configuration*/
#define AIG_229 /*GetValueFrom2dLookuptable function incorrectly returns INFINITY when input x value is equal to value in table*/
#define AIG_232 /*Get_Angle_Range_Width_Float() dont return zero if start == end point */
#define AIG_234 /*add cobertura export to AS_create_openCPPcoverage*/
#define AIG_236 /*Let AS_create_openCPPcoverage create an xml file containing the results*/
#define AIG_237 /*AS_unit_test reuse mock file needs types for new cal tool*/
#define AIG_238 /*Fix includes of Reuse.h => reuse.h*/
#define AIG_241 /*Use HUGE_VALF to avoid compiler warning for missing INFINITY constant*/
#define AIG_244 /*Give some kind of feedback if the AS_add_source and _header functions are called with non matching files*/
#define AIG_245 /*Add HUGE_VAL to the list of fallbacks for AS_TOOLBOX_INFINITY*/
#define AIG_249 /*AS_target_suppress_MSVC_warning uses comma as parameter separator*/
#define AIG_251 /*Sat_Add_Int32 does not saturate for two negative summand*/
#define AIG_252 /*Get_Overlapping_Angle_Range*/
#define AIG_257 /*Vector_2d_Alg_Add remove TOOLBOX_ASSERT_DIFF_BELOW_MACHINE_PREC*/
#define AIG_258 /*Add types needed by calibration to reuse.h*/
#define AIG_259 /*define FAST_MATH_TABLES within math_selector.h*/
#define AIG_260 /*Add Additional Stub Headers to AS_Unit_Test Framework and Modify reuse.h to account for new Cal-Tool */
#define AIG_262 /*Define FIXED_POINT_MATH within math_selector.h*/
#define AIG_265 /*reuse.h: Move types needed only in calibration tool into the AS_NON_CLEAN_TYPES section*/
#define AIG_267 /*Function to test if a point is in a polygon based on the Ray Casting Algorithm*/
#define AIG_278 /*Ensure that SRR_CORE_LIB_INCLUDE_DIRS and SIL_LIB_INCLUDE_DIRS are not empty before using them*/
#define AIG_280 /*Update QAC personality file*/
#define AIG_282 /*Solve QAC warnings level 7*/
#define AIG_283 /*AS_create_QAC_project create QAC folder if it doesn't exist*/
#define AIG_284 /*Solve QAC warnings*/
#define AIG_285 /*Create matrix module*/
#define AIG_288 /*Add a function that can create an include-what-you-use target*/
#define AIG_290 /*Provide macros to check if a boolean is TRUE/FALSE*/
#define AIG_294 /*[RNA_SRR5]: Asserts in Angle_Range.c due to Data corruption  */
#define AIG_295 /*SDD generation: get user principal name (UPN)*/
#define AIG_298 /*QAC project generation should not include header files*/
#define AIG_301 /*SIGN Macro in Basic_Macros.h raises QAC issues*/
#define AIG_302 /*Extend SDD generation to allow to include additional sources and api targets*/
#define AIG_307 /*Build Error in Shared_Toolbox*/
#define AIG_309 /*Create customer MAXUS in AS_create_customer_list*/
#define AIG_310 /*Cmake add option to choose if warnings shall be treated as errors*/
#define AIG_312 /*Introduce new customer BMW_SRR5 in common_functions.cmake*/
#define AIG_315 /*Timing functions*/
#define AIG_319 /*Offer functionality to generate a target for doxygen in AS_Unit_Test*/
#define AIG_320 /*AS_generate_SDD_generation_target issue a warning instead of a status message if docgen is not found*/
#define AIG_321 /*Add option to use plantuml in doxygen*/
#define AIG_323 /*Create timing demo files as part of a unit test*/
#define AIG_324 /*Extend timers to support stacked timers*/
#define AIG_325 /*Implement a moving average filter*/
#define AIG_329 /*Allow to add additional doxygen files to doxygen targets*/
#define AIG_330 /*Doxygen Add support for linking to external documentation*/
#define AIG_332 /*QAC warning for function like macro should be suppressed*/
#define AIG_334 /*Improve doxygen comments*/
#define AIG_335 /*Offer a way to use an infinity constant without getting a compiler warning*/
#define AIG_337 /*doxygen cmake script warnings*/
#define AIG_339 /*CMake scripts*/
#define AIG_340 /*Missing NULL pointer check in Timer_Stop*/
#define AIG_341 /*Allow callers of AS_generate_doxygen_target to set DOXYGEN options*/
#define AIG_342 /*Make clang-tidy readability-identifier-naming accessible in AS_Unit_Test*/
#define AIG_343 /*Removed attribute CUSTOMER_PROGRAM from Shared_Toolbox\Source\CMake\common_functions.cmake*/
#define AIG_344 /*Solve clang-tidy for local variables and static functions*/
#define AIG_350 /*Modernize CMakeLists.txt*/
#define AIG_351 /*Extend AS_Unit_Test documentation*/
#define AIG_354 /*Introduction of EXTRA_PACKAGES option for doxygen*/
#define AIG_355 /*Introduction of DOXYGEN_ALIASES option for doxygen*/
#define AIG_356 /*CMake Error cannot find   ${DEPENDENCIES_ROOT_PATH}/CMake/${CUSTOMER}_SRR_GlobalEnvironment.cmake="D:/jenkins/GDSR_Tracker_in_AUDI_SRR3/AUDI/RR_ADAS/CMake/../CMake/AUDI_SRR_GlobalEnvironment.cmake"*/
#define AIG_358 /*Implement Hesse line utilities*/
#define AIG_359 /*Unit tests for hesse line functions*/
#define AIG_360 /*add new customer BMW_SRR5_BPILLAR*/
#define AIG_361 /*Add get functions and a function to create a hesse line from hesse line parameters*/
#define AIG_362 /*Implement function for calculation of normalized mean angle */
#define AIG_363 /*Removed unnecessary DEPENDENCIES_ROOT_PATH from Source/CMakeLists.txt*/
#define AIG_364 /*QAC cannot check calibration tool generated files*/
#define AIG_365 /*QAC_generation folder should be in as_unit_test folder*/
#define AIG_366 /*Add ability to suppress checks to AS_create_QAC_project*/
#define AIG_370 /*Solve QAC warnings*/
#define AIG_372 /*Offer explanation for why there are no unit conversion macros*/
#define AIG_376 /*AS_Unit_Test use gtest gtest_force_shared_crt option*/
#define AIG_377 /*cmake: allow including project to set IDE folder*/
#define AIG_378 /*AS_generate_doxygen_target: Allow multiple IMAGE_PATHs*/
#define AIG_379 /*AS_generate_SDD_generation_target: Do not copy doc folder*/
#define AIG_381 /*target_sources should not use PUBLIC*/
#define AIG_382 /*Throughput optimization with SPE2*/
#define AIG_383 /*Apply changes from 20190820_SRR_Throughput_optimization_with_SPE2.pptx*/
#define AIG_386 /*SIGN macro misses brackets*/
#define AIG_387 /*AS_Unit_Test allow user to alter project name*/
#define AIG_388 /*Update lcf files*/
#define AIG_389 /*Create a generic shared toolbox make file*/
#define AIG_391 /*Support for ST_ENABLE_SPE2_VEC switch in cmake and make*/
#define AIG_396 /*Move testing code into ST repository*/
#define AIG_397 /*Extend documentation: Doxygen for SPE2 implementation*/
#define AIG_398 /*Shared_Toolbox.mak make variable access*/
#define AIG_399 /*rename function Force_None_Zero*/
#define AIG_400 /*Result of the function Is_Point_In_Polygon depends on indexing order of polygon corners*/
#define AIG_401 /*add empty mock definition of Reverse_Array in order to use functions provided by cal tool generated files*/
#define AIG_402 /*Solve QAC warnings*/
#define AIG_403 /*Give AS_create_QAC_project the ability to define additional macros*/
#define AIG_404 /*Give AS_create_QAC_project the ability to replace system headers*/
#define AIG_405 /*Let AS_create_QAC_project distinguish between system and target source files and include folders*/
#define AIG_406 /*Vector_2d_Alg_Angle_From_Vector*/
#define AIG_407 /*Fix Issues resulting of AIG-401*/
#define AIG_409 /*AS_generate_doxygen_target default for generation option should be off*/
#define AIG_410 /*QAC standard header math.h wrong function names*/
#define AIG_411 /*AIG-402 introduced issues with structure sizes*/
#define AIG_412 /*Provide Polygon_T type for a more generic zone definition in features*/
#define AIG_414 /*Does Range Overlap Range functions return FALSE for overlapping ranges*/
#define AIG_417 /*Extend AS_Unit_Test documentation*/
#define AIG_419 /*QAC cannot find user defined message file*/
#define AIG_420 /*QAC cmake script should generate a via file as well*/
#define AIG_421 /*Fix Release 2019-11-18 candidate*/
#define AIG_422 /*QAC script shall allow the user to define warnings output format*/
#define AIG_424 /*Additional angle range functions needed*/
#define AIG_425 /*AS_Unit_Test shall generate a Bullseye covselect file*/
#define AIG_426 /*Conflicts with c++ and the min max macros*/
#define AIG_427 /*Add RNA_SRR5 to AS_CUSTOMER_LIST*/
#define AIG_428 /*cmake missing include for CMakeDependentOption*/
#define AIG_430 /*Wrong capitalization of Feature_Building_Kit folder*/
#define AIG_434 /*Improve explanation of INFINITY macro*/
#define AIG_436 /*QAC Warning for cyclomatic complexity*/
#define AIG_437 /*AS_Unit_Test: Allow module under test to not have its CMakeLists file in a folder called cmake*/
#define AIG_438 /*case mismatch in CMakeLists.txt*/
#define AIG_440 /*Increase default threshold for cyclomatic complexity to 16*/
#define AIG_443 /*Use extern "C" for Tracker_Wrapper methods*/
#define AIG_444 /*AS_generate_doxygen_target should allow to define more than one DOXYGEN_PREDEFINED*/
#define AIG_445 /*AS_clang_tidy has an issue with multi configuration generators*/
#define AIG_446 /*Extend AS_Unit_Test to be able to specify the struct member packing*/
#define AIG_448 /*Fix unit tests for gcc*/
#define AIG_452 /*Move As_Unit_Test into its own repository*/
#define AIG_462 /*Move matrix types into their own header*/
#define AIG_469 /*Add switch to remove lookuptable for EXP function*/
#define AIG_471 /*Add unit test for Get_Angle_Range_Center*/
#define AIG_502 /*add module for checksum calculation*/
#define AIG_503 /*Add functionality to set the trigonometric tables by checksum*/
#define AIG_555 /*link unit tests to requirements*/
#define AIG_557 /*add missing test for requirements coverage*/
#define AIG_558 /*fix QAC warning*/
#define AIG_565 /*mak files are broken*/
#define AIG_578 /*Introduce new customer Nissan_SRR6 in common_functions.cmake*/
#define ABX_1007 /*Generate QAC project using CMake*/
#define ABX_1522 /*Add GWM and RNA to cmake AS_create_customer_list*/
/*********** End of list of solved jira tickets *****************************/

#ifdef __cplusplus
}
#endif
#endif
