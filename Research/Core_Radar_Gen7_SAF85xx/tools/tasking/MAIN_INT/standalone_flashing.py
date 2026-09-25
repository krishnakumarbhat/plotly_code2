"""
This is standalone tasking debugger flash file Helps with Flashing.
"""
import isystem.connect as ic
import os
import argparse

ROOT_PATH = "../../../"
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


def connect_to_winidea(start_session: bool) -> ic.ConnectionMgr:
    """
    Helps in connecting to the winidea.
    """
    # Specify here any additonal parameters like winIDEA configuration file (.xjrf)
    conn_mgr: ic.ConnectionMgr = ic.ConnectionMgr()
    conn_mgr.connect()  # connect to already opened winIDEA

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


def find_or_add_application_for_core(core_name: str, conn_mgr: ic.ConnectionMgr) -> str:
    """
    Helps in finding the core Application inforamtion.
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


def delete_all_symbol_files(core_name: str, conn_mgr: ic.ConnectionMgr) -> None:
    """
    Helps in deleting all the symbol files.
    """
    core_app_name: str = find_or_add_application_for_core(core_name, conn_mgr)
    cfg_ctrl: ic.CConfigurationController = ic.CConfigurationController(conn_mgr)
    debug_files: ic.COptionFilesCfg = cfg_ctrl.ide_app_files(core_app_name)
    debug_files.clear()


def load_symbol_file(file_path: str, core_name: str, conn_mgr: ic.ConnectionMgr) -> None:
    """
    Method to load a symbols for specific core.

    Args:
        file_path: The path to the symbols file.
    """
    core_app_name: str = find_or_add_application_for_core(core_name, conn_mgr)
    cfg_ctrl: ic.CConfigurationController = ic.CConfigurationController(conn_mgr)
    debug_files: ic.COptionFilesCfg = cfg_ctrl.ide_app_files(core_app_name)

    debug_files.add_file(file_path, "ELF", 0)


def set_default_symbol_file(core_name: str, file_idx: int, conn_mgr: ic.ConnectionMgr):
    """
    Helps in setting up the default symbol files.
    """
    core_app_name: str = find_or_add_application_for_core(core_name, conn_mgr)
    cfg_ctrl: ic.CConfigurationController = ic.CConfigurationController(conn_mgr)
    debug_files: ic.COptionFilesCfg = cfg_ctrl.ide_app(core_app_name)
    debug_files.set_int("SymbolFiles.DefaultFile", file_idx)


def setup_debug_files(flash_cfg: CFlashConfig, conn_mgr: ic.ConnectionMgr):
    """
    Helps in setting up the debug files.
    """
    # Delete all previous debug files from winIDEA
    delete_all_symbol_files("M7_0", conn_mgr)
    delete_all_symbol_files("BBE32EP", conn_mgr)
    delete_all_symbol_files("A53", conn_mgr)

    # Set correct application debug files
    if "M7_0" in flash_cfg.symbol_files:
        file_idx: int = 0
        for debug_file_path in flash_cfg.symbol_files["M7_0"]:
            load_symbol_file(debug_file_path, "M7_0", conn_mgr)
            if len(flash_cfg.symbol_files["M7_0"]) > 1:
                _, file_name = os.path.split(debug_file_path)
                if "m7app.elf" in file_name.lower():
                    set_default_symbol_file("M7_0", file_idx, conn_mgr)
            file_idx += 1

    if "BBE32EP" in flash_cfg.symbol_files:
        for debug_file_path in flash_cfg.symbol_files["BBE32EP"]:
            load_symbol_file(debug_file_path, "BBE32EP", conn_mgr)

    if "A53" in flash_cfg.symbol_files:
        for debug_file_path in flash_cfg.symbol_files["A53"]:
            load_symbol_file(debug_file_path, "A53", conn_mgr)


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

        conn_mgr: ic.ConnectionMgr = connect_to_winidea(False)

        remove_all_files_for_flashing(conn_mgr)
        for file_path in flash_cfg.flashing_files:
            add_file_for_flashing(conn_mgr, file_path)
        setup_debug_files(flash_cfg, conn_mgr)

        conn_mgr: ic.ConnectionMgr = connect_to_winidea(True)

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
                sess_ctrl: ic.CSessionCtrl = ic.CSessionCtrl(conn_mgr)
                sess_ctrl.begin_reset()
                dbg_ctrl: ic.CDebugFacade = ic.CDebugFacade(conn_mgr)
                dbg_ctrl.run()
    if not flashing_ok:
        exit(1)
    exit(0)


main()
