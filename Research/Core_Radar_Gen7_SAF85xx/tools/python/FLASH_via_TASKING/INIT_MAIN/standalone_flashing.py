"""
This is standalone tasking debugger flash file helps CICD Flashing.
"""
import isystem.connect as ic
import os
import argparse

ROOT_PATH = "../../../../"
WINIDEA_EXE = "C:/winIDEA/winIDEA.exe"
WINIDEA_CFG = "GEN7_V2.xjrf"  # Relative to script path
glob_close_winidea = False


class CFlashConfig:
    """
    This Class has the config related to flash files.
    """

    erase_first: bool = False
    symbol_files: dict = {}
    flashing_files: list = []

    def add_symbol_file(self, path: str, target_core: str):
        """
        Helps in adding the symbol file.
        """
        if target_core not in self.symbol_files:
            self.symbol_files[target_core] = [path]
        else:
            self.symbol_files[target_core].append(path)

    def remove_last_symbol_file(self, target_core: str):
        """
        Helps in removing the last symbol file.
        """
        if len(self.symbol_files[target_core]) > 0:
            del self.symbol_files[target_core][-1]

    def add_flashing_file(self, path: str):
        """
        Helps in adding the Flash File.
        """
        self.flashing_files.append(path)


def parse_flash_cfg(dir_path: str) -> CFlashConfig:
    """
    Helps in Reading ini related file from output folder.
    """
    RAW_CFG_CMD_IDX = 0
    RAW_CFG_PARAM_IDX = 1
    flash_cfg_path = os.path.join(dir_path, "flash_session.ini")
    file_handle = None
    flash_cfg_raw: list = []
    try:
        file_handle = open(flash_cfg_path, "r")
        flash_cfg_raw = file_handle.readlines()
    except Exception:
        raise
    finally:
        if file_handle is not None:
            file_handle.close()
    flash_cfg: CFlashConfig = CFlashConfig()
    file_path = None
    for raw_cfg in flash_cfg_raw:
        raw_cfg_tokenized = raw_cfg.split(" ")
        cmd = raw_cfg_tokenized[RAW_CFG_CMD_IDX].lower()
        param = None
        if len(raw_cfg_tokenized) > RAW_CFG_PARAM_IDX:
            param = raw_cfg_tokenized[RAW_CFG_PARAM_IDX].strip()
        if "pre_erase" in cmd:
            flash_cfg.erase_first = param.lower() == "on"
        if "lfile" in cmd:
            if param is not None:
                file_path = os.path.join(dir_path, param)
                file_path = os.path.normpath(file_path)
            else:
                file_path = None
        if ("lcode" in cmd) and (file_path is not None) and (param is not None):
            if param.lower() == "on":
                flash_cfg.add_flashing_file(file_path)
        if ("lsym" in cmd) and (file_path is not None) and (param is not None):
            if param.lower() == "on":
                core_idx: int = int(cmd[4])
                if core_idx == 0:
                    flash_cfg.add_symbol_file(file_path, "M7_0")
                elif core_idx == 1:
                    flash_cfg.add_symbol_file(file_path, "A53")
                elif core_idx == 2:
                    flash_cfg.add_symbol_file(file_path, "BBE32EP")
    return flash_cfg


def get_variant_bin_dir(variant: str) -> str:
    """
    Helps in fetching the Variant.
    """
    script_path = os.path.abspath(__file__)
    scirpt_dir = os.path.dirname(script_path)
    variant_dir = os.path.join(scirpt_dir, f"{ROOT_PATH}/bazel-bin/outputs/{variant.lower()}")
    variant_dir = os.path.normpath(variant_dir)
    return variant_dir


def get_flashing_dir(variant: str | None, path: str | None) -> str:
    """
    Helps in fetching the flash ini Dir.
    """
    if path is not None:
        return path
    elif variant is not None:
        return get_variant_bin_dir(variant)
    else:
        raise Exception("At least variant or flash cfg path must be provided through arguments!")


def connect_to_winidea(
    start_session: bool, cfg_path: str, exe_path: str = None
) -> ic.ConnectionMgr:
    """
    Helps in connecting to the winidea.
    """
    # winIDEA configuration path is relative to script path
    script_path = os.path.abspath(__file__)
    scirpt_dir = os.path.dirname(script_path)
    cfg_abs_path = os.path.join(scirpt_dir, cfg_path)
    cfg_abs_path = os.path.normpath(cfg_abs_path)

    # Specify here any additonal parameters like winIDEA configuration file (.xjrf)
    conn_mgr: ic.ConnectionMgr = ic.ConnectionMgr()
    conn_cfg: ic.CConnectionConfig = ic.CConnectionConfig()
    conn_cfg.workspace(cfg_abs_path)
    if exe_path is not None:
        winidea_exe_dir = os.path.dirname(exe_path)
        conn_cfg.exe_dir(str(winidea_exe_dir))
    conn_mgr.connect(conn_cfg)

    if not start_session:
        return conn_mgr

    sess_ctrl: ic.CSessionCtrl = ic.CSessionCtrl(conn_mgr)
    sess_ctrl.begin_reset()

    dbg_ctrl: ic.CDebugFacade = ic.CDebugFacade(conn_mgr)
    stopped: bool = dbg_ctrl.waitUntilStopped(
        5000
    )  # if after 5 seconds core is not stopped, flashing cannot be done
    if not stopped:
        if glob_close_winidea:
            conn_mgr.disconnect_close(False)
        else:
            sess_ctrl: ic.CSessionCtrl = ic.CSessionCtrl(conn_mgr)
            sess_ctrl.end()
        raise Exception("M7 core is not stopped after reset. Flashing cannot be done.")

    return conn_mgr


def add_file_for_flashing(conn_mgr: ic.ConnectionMgr, file_path: str, spi_device_index: int = 0):
    """
    Helps in adding the flashing file.
    """
    files_opt_ctrl: ic.COptionController = ic.COptionController(
        conn_mgr, f"/iOPEN/Data.UMI.Devices.Devices[{spi_device_index}].DownloadFiles"
    )
    new_file_opt_ctr: ic.COptionController = files_opt_ctrl.add()
    new_file_opt_ctr.set_bool("Enabled", True)
    new_file_opt_ctr.set("Path", file_path)
    new_file_opt_ctr.set("Type", "Auto")


def remove_all_files_for_flashing(conn_mgr: ic.ConnectionMgr, spi_device_index: int = 0):
    """
    Helps in removing all the files for flashing.
    """
    files_opt_ctrl: ic.COptionController = ic.COptionController(
        conn_mgr, f"/iOPEN/Data.UMI.Devices.Devices[{spi_device_index}].DownloadFiles"
    )
    files_opt_ctrl.clear()


def program_SPI_flash(conn_mgr: ic.ConnectionMgr, spi_device_index: int = 0):
    """
    Helps in programming the SPI Flash.
    """
    dev_ctrl: ic.CStorageDeviceController = ic.CStorageDeviceFactory.makeDevice(
        conn_mgr, ic.EStorageDevice_SPIDevice, spi_device_index
    )
    dev_ctrl.write(ic.IConnectUMI.wProgDevice)
    sess_ctrl: ic.CSessionCtrl = ic.CSessionCtrl(conn_mgr)
    sess_ctrl.end()


def parse_args():
    """
    Helps in parsing the CMD line Args.
    """
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-variant",
        help="Variant of build files to be used (e.g. SRR7p, FLR7...). This option is overriden by -flash_cfg_dir.",
        type=str,
        required=False,
    )
    parser.add_argument(
        "-flash_cfg_dir",
        help="Specify path to flash configuration file (.ini). This option overrides -variant.",
        type=str,
        required=False,
    )
    parser.add_argument(
        "-close_winidea",
        help="Close winIDEA after flashing (by default it is left open to speed up CI process).",
        action="store_true",
    )
    args = parser.parse_args()
    return args


def add_watch_variable(conn_mgr: ic.ConnectionMgr, expression: str) -> None:
    """
    Helps in adding the Watch Varaibles.
    """
    ws_ctrl = ic.CWorkspaceController(conn_mgr)
    ws_ctrl.invoke("/IDE/IDE", {"Operation": "AddWatch", "Watch.Expression": expression})


def clear_watches(conn_mgr: ic.ConnectionMgr) -> None:
    """
    Helps in Clearing the watches.
    """
    ws_ctrl = ic.CWorkspaceController(conn_mgr)
    ws_ctrl.invoke("/IDE/IDE", {"Operation": "DeleteWatch"})


def open_watch_window(conn_mgr: ic.ConnectionMgr) -> None:
    """
    Helps in opening the watch window.
    """
    ws_ctrl = ic.CWorkspaceController(conn_mgr)
    ws_ctrl.invoke("/IDE/IDE", {"Operation": "Window", "Window": "Watch"})


def add_default_watch_variables(conn_mgr: ic.ConnectionMgr) -> None:
    """
    Helps in adding the default variables for GEN7_V2.
    """
    add_watch_variable(conn_mgr, "\\\\m7App\\versions\\Git_Status")
    add_watch_variable(conn_mgr, "\\\\m7App\\versions\\Git_Commit_Hash")
    add_watch_variable(conn_mgr, "Application_Version")
    add_watch_variable(conn_mgr, "Radar_Ctl_Data")
    add_watch_variable(conn_mgr, "spt_evt_count")
    add_watch_variable(conn_mgr, "dsp_err_count")
    add_watch_variable(conn_mgr, "dsp_isr_count")
    add_watch_variable(conn_mgr, "Radar_50ms_Counter")


def config_flashing_files(conn_mgr: ic.ConnectionMgr, files: list) -> None:
    """
    Helps in making config for flashing files.
    """
    remove_all_files_for_flashing(conn_mgr)
    for file_path in files:
        add_file_for_flashing(conn_mgr, file_path)


def find_or_add_application_for_core(core_name: str, conn_mgr: ic.ConnectionMgr) -> str:
    """
    Helps in finding or adding the application for core.
    """
    mem_spaces_opt_ctrl: ic.COptionController = ic.COptionController(
        conn_mgr, "/IDE/System.Debug.SoCs[0].MemorySpaces"
    )
    mem_space_opt_ctrl: ic.COptionController = None
    try:
        mem_space_opt_ctrl: ic.COptionController = mem_spaces_opt_ctrl.find("Cores", core_name)
        if mem_space_opt_ctrl.get("Application") == "":
            mem_space_opt_ctrl.set("Application", f"App_{core_name}")
    except Exception:
        # Does not exist yet, create a new one
        mem_space_opt_ctrl: ic.COptionController = mem_spaces_opt_ctrl.add()
        mem_space_opt_ctrl.set("UserName", core_name)
        mem_space_opt_ctrl.set("Cores", core_name)
        mem_space_opt_ctrl.set("Application", f"App_{core_name}")

    mem_space_opt_ctrl.set_bool("Enabled", True)

    app_name: str = mem_space_opt_ctrl.get("Application")

    cfg_ctrl: ic.CConfigurationController = ic.CConfigurationController(conn_mgr)
    apps_opt_ctrl: ic.COptionController = cfg_ctrl.ide_apps()
    app_opt_ctrl: ic.COptionController = None
    try:
        app_opt_ctrl: ic.COptionController = apps_opt_ctrl.find("Name", app_name)
    except Exception:
        # Does not exist yet, create a new one
        app_opt_ctrl: ic.COptionController = apps_opt_ctrl.add()
        app_opt_ctrl.set("Name", app_name)

    return app_name


def config_symbol_files(conn_mgr: ic.ConnectionMgr, files: dict) -> None:
    """
    Helps in config part of symbol files and setup m7 file.
    """
    if "M7_0" in files:
        core_app_name: str = find_or_add_application_for_core("M7_0", conn_mgr)
        cfg_ctrl: ic.CConfigurationController = ic.CConfigurationController(conn_mgr)
        debug_files: ic.COptionFilesCfg = cfg_ctrl.ide_app_files(core_app_name)
        debug_files.clear()
        for file in files["M7_0"]:
            debug_files.add_file(file, "ELF", 0)


def run_application(conn_mgr: ic.ConnectionMgr, flash_cfg: CFlashConfig) -> None:
    """
    Helps in running the application.
    """
    config_symbol_files(conn_mgr, flash_cfg.symbol_files)
    sess_ctrl: ic.CSessionCtrl = ic.CSessionCtrl(conn_mgr)
    sess_ctrl.begin_reset()
    dbg_ctrl: ic.CDebugFacade = ic.CDebugFacade(conn_mgr)
    dbg_ctrl.run()
    open_watch_window(conn_mgr)
    clear_watches(conn_mgr)
    add_default_watch_variables(conn_mgr)


def enable_cold_start(conn_mgr: ic.ConnectionMgr) -> dict:
    """
    Helps in Enable of cold start.
    """
    sess_ctrl: ic.CSessionCtrl = ic.CSessionCtrl(conn_mgr)
    sess_ctrl.end()

    opt_ctrl: ic.COptionController = ic.COptionController(conn_mgr, "/iOPEN/CallPoints")
    orig_settings: dict = {}
    orig_settings["Debug_ConnectToSoC.Scripts"] = opt_ctrl.get("Debug_ConnectToSoC.Scripts")
    orig_settings["Debug_ConnectToSoC.Execute"] = opt_ctrl.get("Debug_ConnectToSoC.Execute")
    orig_settings["Debug_ConnectToSoC_Same"] = opt_ctrl.get("Debug_ConnectToSoC_Same")

    opt_ctrl.set(
        "Debug_ConnectToSoC.Scripts",
        "$(SFR_FILE_DIR)/SAF85xx_ConnectToSoC.cpp;$(SFR_FILE_DIR)/SAF85xx_ColdStart.cpp",
    )
    opt_ctrl.set("Debug_ConnectToSoC.Execute", "Custom")
    opt_ctrl.set_bool("Debug_ConnectToSoC_Same", False)

    sess_ctrl.begin_reset()
    dbg_ctrl: ic.CDebugFacade = ic.CDebugFacade(conn_mgr)
    dbg_ctrl.waitUntilStopped(2000)
    return orig_settings


def restore_cold_start(conn_mgr: ic.ConnectionMgr, cfg: dict) -> None:
    """
    Helps in restore of cold start.
    """
    sess_ctrl: ic.CSessionCtrl = ic.CSessionCtrl(conn_mgr)
    sess_ctrl.end()

    opt_ctrl: ic.COptionController = ic.COptionController(conn_mgr, "/iOPEN/CallPoints")
    for opt_key in cfg:
        opt_ctrl.set(opt_key, cfg[opt_key])

    sess_ctrl.begin_reset()
    dbg_ctrl: ic.CDebugFacade = ic.CDebugFacade(conn_mgr)
    dbg_ctrl.waitUntilStopped(2000)


def main():
    """
    Helps in running the py script.
    """
    conn_mgr: ic.ConnectionMgr = None
    flashing_ok: bool = True

    try:
        cmd_line_args = parse_args()
        glob_close_winidea = cmd_line_args.close_winidea

        flasing_dir: str = get_flashing_dir(cmd_line_args.variant, cmd_line_args.flash_cfg_dir)
        flash_cfg: CFlashConfig = parse_flash_cfg(flasing_dir)

        conn_mgr: ic.ConnectionMgr = connect_to_winidea(False, WINIDEA_CFG, WINIDEA_EXE)

        config_flashing_files(conn_mgr, flash_cfg.flashing_files)
        config_symbol_files(conn_mgr, flash_cfg.symbol_files)

        conn_mgr: ic.ConnectionMgr = connect_to_winidea(True, WINIDEA_CFG, WINIDEA_EXE)

        # configure cold start script to be safe if QSPI is noy yet flashed
        orig_cfg: dict = enable_cold_start(conn_mgr)
        program_SPI_flash(conn_mgr)
        restore_cold_start(conn_mgr, orig_cfg)
    except Exception as ex:
        print(f"ERROR: flashing failed (Exception: {ex})")
        flashing_ok = False
    finally:
        if conn_mgr is not None:
            if glob_close_winidea:
                conn_mgr.disconnect_close(False)
            else:
                run_application(conn_mgr, flash_cfg)
    if not flashing_ok:
        exit(1)
    exit(0)


main()
