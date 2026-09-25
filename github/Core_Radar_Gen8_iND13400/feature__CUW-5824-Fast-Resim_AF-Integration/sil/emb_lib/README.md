# Gen8 emb_lib

This folder contains the Gen8 SIL embedded library flow: build scripts, emb_lib configuration, stream wrappers, and the shared library that is loaded by the SIL engine during replay or debugging.

## Relevant directories

- `build_bin/`: interactive build script and post-build copy scripts.
- `sil_source/`: main emb_lib implementation and test targets.
- `rsp_emb_lib/`, `tracker_emb_lib/`, `udp/`, `can/`, `streams/`: runtime integration layers used by the SIL build.
- `Emb_Lib_Config.xml`: emb_lib sensor configuration used by the SIL engine.
- `example.launch.json`: sample VS Code debug configuration. This is only a template and must be copied into a real `launch.json` by each user.

## Prerequisites

1. Run `python repo_init.py` once from the repository root so the repo dependencies and hooks are initialized.
2. Use the Bazel version configured by the repository through `bazelisk`.
3. Install the VS Code `C/C++` extension (`cpptools`) if you want to debug from VS Code.
4. Have a local SIL engine package available, for example under:
   - Windows: `C:/Git/10028634_01_SRR_RESIM_Release/RESIM_SIL/GEN8_SIL_ENGINE`
   - Linux: adjust to your local SIL engine path before running or debugging.

## Build configuration used by emb_lib

The local `sil/emb_lib/.bazelrc` adds the Gen8 emb_lib-specific Bazel settings, including:

- `--@afbb//module/_common:sil_config_enable=True`
- `--@afbb//module/_common:use_bbe_cstub_simulator=False`
- `--@Gen8_iND13400//software/bbe32/src:enable_cdc=True`
- `--@appl_inclusion_dep//:enable_psp_sil=True`
- `--@emb_tracker_wrapper//:enable_pc_resim_tracker=True`

The interactive build script also prompts for the main runtime selections:

- Variant: `srr8p` or `flr8`
- SIL mode: `CDC`, `RDD`, `AF`, `DETECT`
- Feature function: `true` or `false`
- Tracker variant
- Customer: `gpo` or `al`
- Output target: shared library, static library, or test executable
- Build config: `emblib_debug` or `emblib_release`

## Building emb_lib

Run the build from `sil/emb_lib/build_bin`:

```powershell
cd sil\emb_lib\build_bin
python build.py
```

The script prints the final Bazel command before invoking it. For a normal debug build, the most common output target is the dynamic library target `//sil/emb_lib/sil_source:emb_lib_sharedlib`.

Useful commands:

```powershell
cd sil\emb_lib\build_bin
python build.py --all
python build.py clean
python build.py clean --expunge
```

## Copying build output to the SIL engine

After a successful build, `build.py` tries to copy the generated shared library to the default SIL engine folder automatically.

- On Windows it uses one of the batch files in `build_bin/`, such as `copy_position_dll_srr.bat` or `copy_position_dll_flr.bat`.
- On Linux it uses the corresponding `.sh` scripts.

If the default engine folder does not exist on your machine, the build still succeeds, but the post-build copy step is skipped. In that case run the copy script manually and pass your engine directory.

Example on Windows:

```powershell
cd sil\emb_lib\build_bin
copy_position_dll_srr.bat C:\Git\10028634_01_SRR_RESIM_Release\RESIM_SIL\GEN8_SIL_ENGINE
```

## Running the SIL engine with Gen8 emb_lib

1. Get the SIL engine package that matches your intended replay setup.
2. Copy the decoder library into the engine folder:
   - Windows: copy `radar_stream_decoder.dll`
   - Linux: copy `libradar_stream_decoder.so`
   - Source folder: `10028634_01_SRR_RESIM_Release/RESIM_SIL/GEN7_DECODER_DLL`
   - Destination folder: `10028634_01_SRR_RESIM_Release/RESIM_SIL/GEN8_SIL_ENGINE`
3. Update the engine-side `SIL_Engine_Config.xml`.
4. Set `SENSOR_CONFIG` to this repository file:
   - `sil/emb_lib/Emb_Lib_Config.xml`
5. Review and update the engine configuration values that depend on your setup:
   - `RESIM_OUTPUT_PATH.Output_Path_Options`
   - `RESIM_OUTPUT_FILE_FORMAT`
   - `SENSOR_STATUS`
   - `SIL_INJECTION_MODE`
   - `SIL_Entrypoint`
   - `CALIBRATION_SOURCE`
6. Update the engine-side `SIL_Input_Logs.txt` as needed for the replay log list.
7. Build emb_lib and copy the generated libraries into the engine folder.
8. Start the engine from the engine directory.

Example on Windows:

```powershell
cd C:\Git\10028634_01_SRR_RESIM_Release\RESIM_SIL\GEN8_SIL_ENGINE
.\APT_SRR_RESIM.exe SIL_Engine_Config.xml SIL_Input_Logs.txt C:\out
```

## Debugging from VS Code
Install the "C/C++" extension in VS Code (search for "cpptools" in extensions).
`example.launch.json` is a dummy template only. It is not the active VS Code launch file. Each user must create a real local `launch.json` from it at the workspace root.

### Create the real launch.json file

1. Open the repository root workspace folder in VS Code.
2. Create a local folder named `.vscode` in the repository root if it does not already exist.
3. Inside that folder, create a file named `launch.json`.
4. Open `sil/emb_lib/example.launch.json`.
5. Copy the entire contents of `example.launch.json`.
6. Paste those contents into `.vscode/launch.json` at the repository root.
7. Save the file.

The result should be this local file path on your machine:

```text
.vscode/launch.json
```

Debugging must be started from the workspace root, so keep the real `launch.json` only at the repository root. `sil/emb_lib/example.launch.json` remains the template source file.

This file is intentionally local-only. The `.vscode` folder is ignored in this project, so each user can keep machine-specific paths without committing them.

### Update the copied launch.json for your machine

After copying the template, edit the path fields in the root `.vscode/launch.json` before starting the debugger.

Check at least these fields:

1. `program`: set it to your local SIL engine executable.
2. `cwd`: set it to your local SIL engine directory.
3. `args`: update the config file path, input log path, and output directory if your local setup differs from the sample.
4. `environment`: for Linux, update `LD_LIBRARY_PATH` to the engine directory on your machine.
5. `miDebuggerPath`: verify the MinGW `gdb.exe` path on Windows if your Bazel output base or workspace location differs.
6. `setupCommands`: update any hard-coded dependency or source directories if you are using a different local layout.

Important notes:

- The sample file still contains machine-specific placeholder paths.
- The Linux sample entries still need local path cleanup before use.
- The Windows sample points to a Gen8 engine location, which is a good starting point, but you still need to confirm the exact path on your machine.

### Start debugging

1. Build emb_lib in debug mode using `python build.py` and choose `emblib_debug`.
2. Make sure the generated DLL or SO files have been copied into the SIL engine directory.
3. Open the repository root workspace in VS Code and then open the Run and Debug view.
4. Select the configuration you want to use from the root `.vscode/launch.json`.
5. Start the debugger.


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

# Debugging calibration print code

If you need to debug calibration printing logic, remove the `copts = ["-Os", "-g0"]` setting from the relevant targets under `sil_source/src/event_logger/calib_print/BUILD` so debug information is generated for that code.

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
