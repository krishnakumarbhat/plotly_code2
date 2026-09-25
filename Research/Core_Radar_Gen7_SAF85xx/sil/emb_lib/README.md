# Bazel version
7.1.1, see `ADVRADAR_AWR294X/software/app/emb_lib/.bazeliskrc`
# Flags
 - `.bazelrc` has the following:
   + `--@spbb//common/test/ti/c6xsim:use_c6x_simulator=True`
   + `--@spbb//common:build_pcresim=True`
   + `--@appl_inclusion_dep//:build_pcresim=True`

 - `build/build.bat` has `--@appl_inclusion_dep//:variant=srr7p`

# Dependencies
## Local
 - `gen7`: `../../..` (root of the git repo)
 - `calib_cfg`: `../common/calibrations/config` (`software/app/common/calibrations/config` from the root of the git repo)
 - `building_block` is copied from `@gen7_sil/gen7_sil_wrapper/building_block` from the *same* version of `@gen7_sil` used as a dependency
## External
 - `bazel_platform`: https://jfrog.asux.aptiv.com/artifactory/bazel_tools_internal-generic-local/bazel_platform/commit/bp_a2f58879178f93b38e0bd8e524a7fa6cd55195ce.zip
 ```starlark
 http_archive(
     name = "bazel_platform",
     sha256 = "d782b3aa54e3dab7dc50eade086bf995da1108d32c51a3db9e94097149c83b77",
     url = "https://jfrog.asux.aptiv.com/artifactory/bazel_tools_internal-generic-local/bazel_platform/commit/bp_a2f58879178f93b38e0bd8e524a7fa6cd55195ce.zip",
 )
 load("@bazel_platform//:dependencies.bzl", "bazel_platform_dependencies", "bazel_platform_toolchains")
 bazel_platform_dependencies(
     name = "@bazel_platform",
     repo_url = "https://jfrog.asux.aptiv.com/artifactory/bazel_tools_external-generic-local/",
 )
 bazel_platform_toolchains("@bazel_platform")
 ```
 - `bazel_skylib`: https://jfrog.asux.aptiv.com/artifactory/bazel_tools_external-generic-local/bazel_skylib/bazel-skylib-1.2.1.tar.gz
 - mingw: https://jfrog.asux.aptiv.com/artifactory/bazel_tools_external-generic-local/mingw/windows/x86_64-14.2.0-release-mcf-seh-ucrt-rt_v12-rev0.tar.zst
 - gcc-glibc (for linux): https://jfrog.asux.aptiv.com/artifactory/bazel_tools_internal-generic-local/gcc/gcc-11.4.0-glibc-2.31-ubuntu20.tar.zst
 ```starlark
http_archive(
    name = "mingw",
    build_file = "@bazel_platform//:toolchains/mingw/archive.BUILD",
    sha256 = "ed2097a9546b7a5c935f63ee3d969900a52a6a491c35e9708cbec1fcc54a0442",
    strip_prefix = "mingw64",
    url = "https://jfrog.asux.aptiv.com/artifactory/bazel_tools_external-generic-local/mingw/windows/x86_64-14.2.0-release-mcf-seh-ucrt-rt_v12-rev0.tar.zst",
)

http_archive(
    name = "gcc-glibc",
    build_file = "@bazel_platform//toolchains/gcc:toolchain.BUILD",
    sha256 = "11d72c3724a5187a13973e7d2bf90d0a8f5133a067871c85cf1150b1c7cac172",
    strip_prefix = "x86_64-unknown-linux-gnu",
    url = "https://jfrog.asux.aptiv.com/artifactory/bazel_tools_internal-generic-local/gcc/gcc-11.4.0-glibc-2.31-ubuntu20.tar.zst",
)

register_toolchains(
    "//toolchains/mingw:mingw_windows_toolchain",
    "//toolchains/gcc:gcc_glibc_toolchain",
)
 ```
 - `spbb`: https://jfrog.asux.aptiv.com/artifactory/gen7-aptiv-advradar-building_blocks-local/spbb/releases/2.19/gen7_spbb_2.19.00.zip
 - `afbb`: https://jfrog.asux.aptiv.com/artifactory/gen7-aptiv-advradar-building_blocks-local/afbb/awr294x/releases/1.4/gen7_awr294x_afbb_1.4.10.zip
 - `ti_common_package`: https://gitgerrit.asux.aptiv.com/a/plugins/gitiles/ADVRADAR_AWR2944_TI_SDK/+archive/refs/tags/ti_sdk_es2_aptiv_oct7_2022.tar
 - `Calibration_Handler`: https://jfrog.asux.aptiv.com/artifactory/core_radar-aptiv-00000000-building_blocks-local/Calibration-Handler/Calibration_Handler_v01/BB_Calibration_Handler_v01_05.zip
 - `pugixml`: https://github.com/zeux/pugixml/archive/refs/tags/v1.14.zip
 - `gen7_sil`: https://gitgerrit.asux.aptiv.com/a/plugins/gitiles/ADVRADAR_GEN7_RSP_SIL/+archive/refs/tags/R9.0_Release.tar
 - `smc_srr7p`: https://jfrog.asux.aptiv.com/artifactory/core_radar-aptiv-00000000-calibrations-local/SMC/v44/SRR7P/SRR7P_Standalone_SMC_44_72_14_2_2024-09-05_09-57-17.zip
 - `smc_srr7hd`: https://jfrog.asux.aptiv.com/artifactory/core_radar-aptiv-00000000-calibrations-local/SMC/v41/SRR7HD/SRR7HD_Satellite_SMC_41_75_0_0_2024-05-28_10-26-41.zip
 - `smc_flr7`: https://jfrog.asux.aptiv.com/artifactory/core_radar-aptiv-00000000-calibrations-local/SMC/v45/FLR7/FLR7_Satellite_SMC_45_73_0_3_2024-08-09_01-56-14.zip
 - `usc_srr7p`: https://jfrog.asux.aptiv.com/artifactory/core_radar-aptiv-00000000-calibrations-local/USC/SRR7P/v3/SRR7P_2314601862_USC_3_72_0_2_2024-06-26_07-49-20.zip
 - `usc_srr7hd`: https://jfrog.asux.aptiv.com/artifactory/gen7-aptiv-advradar-calibrations-local/USC/SRR7HD/v4/SRR7HD_u00000001_USC_4_75_0_3_2023-08-07_03-07-50.zip
 - `usc_flr7`: https://jfrog.asux.aptiv.com/artifactory/core_radar-aptiv-00000000-calibrations-local/USC/FLR7/v4/FLR7_2324900059_USC_4_73_0_3_2024-07-29_09-59-25.zip

## Using local dependencies
 - Change `http_archive` to `local_repository`
 - Remove the `urls` argument
 - Add a `path` argument which is a string with a path to the dependency. The path can be either absolute or relative to the main repository's `WORKSPACE` file.
 For example,
 ```starlark
 http_archive(
    name = "dependency_1",
    urls = ["https://jfrog.asux.aptiv.com/dependency_1.tgz"],
 )
 ```
 becomes
```starlark
 local_repository(
    name = "dependency_1",
    path = "/path/to/dependency_1",
 )
```
Note that the local dependency must be a folder, not a zip/tarball. See https://bazel.build/reference/be/workspace#local_repository.
## Things to check when updating `@gen7_sil`
 - In the `WORKSPACE` file, update links to the following repositories
     - smc
     - usc
     - afbb
     - spbb

 - Update `building_block` from `gen7_sil/gen7_sil_wrapper/building_block`

# Building
Run `python build.py` from the `build` folder.
# Running SIL Engine
- Get SIL Engine from the `10028634_01_SRR_RESIM_Release` Plastic repository
- Copy the radar_stream_decoder.dll in windows or libradar_stream_decoder.so in linux from `10028634_01_SRR_RESIM_Release/RESIM_SIL/DECODER_DLL` to `10028634_01_SRR_RESIM_Release/RESIM_SIL/GEN7_SIL_ENGINE`
- Update `SIL_Engine_Config.xml` in the SIL engine directory (`10028634_01_SRR_RESIM_Release/RESIM_SIL/GEN7_SIL_ENGINE`)
  + Change `RESIM_OUTPUT_PATH.Output_Path_Options` to `SAME_AS_INPUT`
  + Change `SENSOR_CONFIG` to the `<git_repo>/software/app/emb_lib/Emb_Lib_Config.xml`
  + Change `RESIM_OUTPUT_FILE_FORMAT`
  + Enable the required sensors in `SENSOR_STATUS`
  + Change `SIL_INJECTION_MODE`
  + Change `SIL_Entrypoint`
  + Change `CALIBRATION_SOURCE` to `LOAD_UDP_CAL`
- Update `Emb_Lib_Config.xml` and `SIL_Input_Logs.txt` in the SIL engine directory
- Copy the dynamic libraries from `bazel-bin/sil_source` to the SIL engine directory
  + There is a script for this - run `copy_position_dll_srr.bat <SIL_ENGINE_DIRECTORY>` or `copy_position_dll_flr.bat <SIL_ENGINE_DIRECTORY>` based on the build from the `build` folder
- In the SIL engine directory, run `APT_SRR_RESIM SIL_Engine_Config.xml SIL_Input_Logs.txt`
# Debugging
Install the "C/C++" extension in VS Code (search for "cpptools" in extensions). `launch.json` is already configured, but it assumes that SIL Engine is in `C:/Plastic/10028634_01_SRR_RESIM_Release/RESIM_SIL/GEN7_SIL_ENGINE`.

NOTE: to debug calibration printing functions, comment out the `copts = ["-Os", "-g0"]` arguments in `sil_source/src/event_logger/calib_print/BUILD` (see the [hacks section](#debug-info-for-writing-calibration))

# Extracting CSV from output logs
 - Go to `10028634_01_SRR_RESIM_Release/RESIM_Toolset/MUDP_LogData_Extractertool`
 - In `MUDP_DATA_Extracter_config.xml`:
   + Change `GENERATION_SELECTION` to `GEN7`
   + Enable the required sensors in `SENSOR_STATUS`
   + Enable the output stream in `Radar_Stream_Option`
   + Change `Output_Path_Options` to `SAME_AS_INPUT`
   + Enable `CSV_MODE` in `Data_Extraction_Mode`
   + Enable `Vector_Library` in `LIBRARY_INTEGRATED`
 - Change `f_list.txt`
 - For satellite logs, copy the libraries from `GEN7_Satellite_Decoder/`

# Saving objects to csv/xml
To save objects to csv/xml, use the functions in `sil_source/inc/serialize_object.h` (bazel target `//sil_source/inc:serialize_object`).
## Singleton objects
For singleton objects (like calibrations), use:
 - `print_singleton_csv`: call it with a file stream and the object to print. For example:
 ```cpp
 USC_Cal_T cal;
 std::ofstream stream{"file.csv"};
 print_singleton_csv(stream, cal);
 ```
 - `save_singleton_xml`: call it with an `xml_node` to append to, and the object to print. For example:
 ```cpp
   USC_Cal_T cal;
   pugi::xml_document xml{};
   xml_node top_level               = xml.append_child("USC_Cal");
   save_singleton_xml(top_level, cal);
   xml.save_file("file.xml");
 ```

## Non-singleton objects
For non-singleton objects that have to be printed every cycle (like detections), use `print_csv_header` and `print_csv_data`: call `print_csv_header` only the first time, and call `print_csv_data` every time. Both of these functions have the same arguments as `print_singleton_csv`, i.e. file stream and the object to print. For example:
```cpp
static std::ofstream stream;

void print_detections() { // called every cycle
  Detection_Stream_T detections;
  if (!stream.is_open()) {
    stream = std::ofstream{"file.csv"};
    print_csv_header(stream, detections);
  }
  print_csv_data(stream, detections);
}
```
# Branching and tagging
## Creating a branch from `dev`
 - Ensure you're on the tip of `dev`:
 ```
  git checkout dev
  git pull
 ```
 - Create the branch, check it out, and push it:
 ```
 git checkout -b <new_branch>
 git push --set-upstream origin <new_branch>
 ```
 Note that normal users can only create branches starting with `feature/`
## Merging from `dev` to the new branch
 - Ensure you're on the latest commit of both `dev` and `<new_branch>` (do it in the below order so that you're on the new branch before you merge)
 ```
 git checkout dev
 git pull
 git checkout <new_branch>
 git pull
 git merge dev
 ```
## Tagging
Ensure you're on the commit you want to tag, then `git tag <tag_name>` (optionally `git tag tag_name -m "<tag_message>"`)
## Merging from the new branch to `dev`
Same as [merging from `dev` to the new branch](#merging-from-dev-to-the-new-branch), just swap `dev` and `<new_branch>`

# Hacks
## Sandbox disabled
Sandbox has been disabled in order to be able to build, ideally we should ask the [signal processing repo](https://gitgerrit.asux.aptiv.com/admin/repos/ADVRADAR_Gen7_Signal_Processing) to fix this:
```diff
diff --git a/common/BUILD b/common/BUILD
index 6f4acd9..c474c2f 100644
--- a/common/BUILD
+++ b/common/BUILD
@@ -315,6 +315,7 @@ cc_library(
         ":ti_helpers_pcresim_h",
         ":timing_helpers_pcresim_lib",
         "@spbb//common/test/ti/hwa-c-model/hwam:hwa_sim_lib",
+        "@ti_common_package//mcu_plus_sdk_awr294x_08_03_00_01/source/drivers/hwa/v0:hwa_h",
     ],
 )
```
## Duplicate typedefs

We have duplicated typedefs in `streams/` because using `rdd_stream.h` from `//software/common:UDPLoggingStreamHeaders` causes struct redefinition errors.

- This has led us to duplicate files in `rc_emb_lib` - namely `static_align_wrapper` and `dyn_align_wrapper`, because the pre-existing code in `@gen7` uses the old types

## Zero checksum

We pass the checksum test when the checksum in the log is zero, this should be fixed when the logs are correct

## Debug info for writing calibration
We use template functions for writing calibrations as there are nested structs, which means that these functions are monomorphized multiple times, generating a lot of debug information. To reduce output size in debug mode, we have disabled debuginfo for these - see `linkopts` in `sil_source/src/event_logger/calib_print/BUILD`.

To debug these functions, comment out the `copts = ["-Os", "-g0"]` argument in these targets.

## Windows command to add prefix to all the files in a folder
```
FOR /r "." %a in (*.*) DO REN "%~a" "prefix%~nxa"
```
Replace `prefix` with your desired name.

## Git config in a common PC (SM2)
```
git remote set-url origin <Your_URL>
git config --global user.email "your_email@example.com"
git config --global user.name "Your Name"
```

## Steps to follow for every SW integration

- `SRR7_SiL_GetVersion()`: version.Release_Revision/Promote_Revision/Field_Revision/strm_version
- Add new streams for version check in `SRR7_SiL_GetVersion`
- Add new streams inside `populate_stream_data()` function
- Update all stream population to be in sync with SAF.
  - Compare `udp` folder with `stream_handler` folder from SAF.
  - Add new streams to `stream_handler_cfg.h`.
  - Add `Stream_Config_Init()` for new streams in `Config_All_Stream()`.
  - Visualize in both Orcas live mode and by opening the output mf4 file.
- Re-check `populate_ipc_misc_data()` function
- Update Tracker wrapper files (`f360_wrapper` folder)
- Update `rsp_emb_lib`
  - Update `rsp_sil_wrapper.h`
  - Check `populate_rsp_input()` and `populate_rsp_output()`.

## Temporary fix to remove the BUILD files in sfl zip file
Temporarily remove the BUILD files from the zip file to address the issue using `patch_cmds_win` in `software/app/emb_lib/WORKSPACE`.
patch_cmds_win = ["rm (Get-ChildItem -Filter BUILD -Recurse -ErrorAction SilentlyContinue -Force).fullname"]
