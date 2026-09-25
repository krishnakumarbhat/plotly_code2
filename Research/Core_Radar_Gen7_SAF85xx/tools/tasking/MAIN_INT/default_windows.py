"""
This py script helps in initiating default windows.
"""
import isystem.connect as ic
import shutil
import os
import argparse

WINIDEA_EXE = "C:/winIDEA/winIDEA.exe"
MAX_WINIDEA_CONFIGS: int = 100
TEMP_DIR = "../TEMP"  # Path relative to script file


def create_copied_config() -> str:
    """
    Helps in creating the Duplicate config.
    """
    BASE_WINIDEA_CONFIG: str = "MAIN_GEN7_V2.xjrf"
    script_dir = os.path.dirname(__file__)
    script_dir = os.path.abspath(script_dir)
    base_cfg_abs_path = os.path.join(script_dir, BASE_WINIDEA_CONFIG)
    base_cfg_abs_path = os.path.normpath(base_cfg_abs_path)

    # Create TEMP_DIR if it does not exist yet
    tmp_dir_abs_path = os.path.join(script_dir, TEMP_DIR)
    tmp_dir_abs_path = os.path.normpath(tmp_dir_abs_path)
    if not os.path.exists(tmp_dir_abs_path):
        os.makedirs(tmp_dir_abs_path)

    # find next free configuration name:
    new_cfg_abs_path = None
    for config_idx in range(MAX_WINIDEA_CONFIGS):
        new_cfg_candidate = f"{config_idx}_{BASE_WINIDEA_CONFIG}"
        new_cfg_abs_path = os.path.join(tmp_dir_abs_path, new_cfg_candidate)
        new_cfg_abs_path = os.path.normpath(new_cfg_abs_path)
        if not os.path.exists(new_cfg_abs_path):
            # found valid configuration name that is not yet used
            break
    else:
        raise Exception(
            f"Limit check for maximum number of existing winIDEA configs ({MAX_WINIDEA_CONFIGS}) reached!"
        )
    shutil.copy(base_cfg_abs_path, new_cfg_abs_path)
    return new_cfg_abs_path


def start_winidea(exe_path: str, cfg_path: str) -> ic.ConnectionMgr:
    """
    Helps in starting the winidea.
    """
    conn_mgr: ic.ConnectionMgr = ic.ConnectionMgr()
    conn_cfg: ic.CConnectionConfig = ic.CConnectionConfig()
    exe_dir = os.path.dirname(exe_path)
    conn_cfg.exe_dir(exe_dir)
    conn_cfg.workspace(cfg_path)
    conn_mgr.connect(conn_cfg)
    return conn_mgr


def load_default_view_layout(conn_mgr: ic.ConnectionMgr) -> None:
    """
    Helps in Loading the default view layout.
    """
    LAYOUT_FILE: str = "APTIV_default_layout.jwst"  # path relative to script file
    script_dir = os.path.dirname(__file__)
    script_dir = os.path.abspath(script_dir)
    layout_abs_path = os.path.join(script_dir, LAYOUT_FILE)
    layout_abs_path = os.path.normpath(layout_abs_path)
    ide_ctrl: ic.CIDEController = ic.CIDEController(conn_mgr)
    ide_ctrl.importViewLayout(layout_abs_path)


def add_watch_variable(conn_mgr: ic.ConnectionMgr, expression: str) -> None:
    """
    Helps in adding the watch variable.
    """
    ws_ctrl = ic.CWorkspaceController(conn_mgr)
    ws_ctrl.invoke("/IDE/IDE", {"Operation": "AddWatch", "Watch.Expression": expression})


def clear_watches(conn_mgr: ic.ConnectionMgr) -> None:
    """
    Helps in clearing the watches.
    """
    ws_ctrl = ic.CWorkspaceController(conn_mgr)
    ws_ctrl.invoke("/IDE/IDE", {"Operation": "DeleteWatch"})


def add_default_watch_variables(conn_mgr: ic.ConnectionMgr) -> None:
    """
    Helps in adding the default watch variables for GEN7_V2.
    """
    add_watch_variable(conn_mgr, "\\\\m7App\\versions\\Git_Status")
    add_watch_variable(conn_mgr, "\\\\m7App\\versions\\Git_Commit_Hash")
    add_watch_variable(conn_mgr, "Application_Version")
    add_watch_variable(conn_mgr, "Radar_Ctl_Data")
    add_watch_variable(conn_mgr, "spt_evt_count")
    add_watch_variable(conn_mgr, "dsp_err_count")
    add_watch_variable(conn_mgr, "dsp_isr_count")
    add_watch_variable(conn_mgr, "Radar_50ms_Counter")


def save_workspace(conn_mgr: ic.ConnectionMgr) -> None:
    """
    Helps in saving the workspace.
    """
    ws_ctrl = ic.CWorkspaceController(conn_mgr)
    ws_ctrl.save()


def open_secondary_core_worksace(conn_mgr: ic.ConnectionMgr, core_name: str) -> None:
    """
    Helps in opening the other cores.
    """
    sess_ctrl: ic.CSessionCtrl = ic.CSessionCtrl(conn_mgr)
    sess_ctrl.get_topology().SoCs()[0].cores()[2].name()  # temporary patch
    sess_ctrl.get_topology().SoCs()[0].cores()[4].name()  # temporary patch
    conn_mgr_secondary: ic.ConnectionMgr = sess_ctrl.instance_attach(core_name)
    load_default_view_layout(conn_mgr_secondary)
    save_workspace(conn_mgr_secondary)


def parse_args():
    """
    Helps in parsing the CMD line Arguments.
    """
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-start_new_winidea",
        help="Create a new winIDEA configuration and start new winIDEA.",
        action="store_true",
    )
    parser.add_argument(
        "-bbe32", help="Start also winIDEA instance for BBE32EP core.", action="store_true"
    )
    parser.add_argument(
        "-a53", help="Start also winIDEA instance for A53 core.", action="store_true"
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


def run():
    """
    Helps in running the main py code.
    """
    args = parse_args()
    if args.start_new_winidea:
        new_cfg: str = create_copied_config()
        conn_mgr: ic.ConnectionMgr = start_winidea(WINIDEA_EXE, new_cfg)
        delete_all_symbol_files("M7_0", conn_mgr)
        delete_all_symbol_files("BBE32EP", conn_mgr)
        delete_all_symbol_files("A53", conn_mgr)
    else:
        conn_mgr: ic.ConnectionMgr = ic.ConnectionMgr()
        conn_mgr.connect()
    clear_watches(conn_mgr)
    load_default_view_layout(conn_mgr)
    add_default_watch_variables(conn_mgr)
    sess_ctrl: ic.CSessionCtrl = ic.CSessionCtrl(conn_mgr)
    sess_ctrl.begin_reset()
    save_workspace(conn_mgr)

    if args.bbe32:
        open_secondary_core_worksace(conn_mgr, "BBE32EP")
    if args.a53:
        open_secondary_core_worksace(conn_mgr, "A53")


if __name__ == "__main__":
    run()
    print()
