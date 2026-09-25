from pathlib import Path
import winreg
import os


def read_envvar(envvar: str, user_env=True):
    r"""function to read Current user environment variables

        Parameters
        ----------
        envvar : string
            name of the local environment variable

        Return
        ----------
        value : string
            value of the local environment variable
        """

    if user_env:
        reg = winreg.ConnectRegistry(None, winreg.HKEY_CURRENT_USER)
        with winreg.OpenKey(reg, r'Environment') as regkey:
            # print("System Environment variables are:")
            # print("#", "name", "value", "type")
            try:
                value = winreg.QueryValueEx(regkey, envvar)[0]
            except:
                value = ""
                pass
            # print("The value of " + envvar + "is: " + value)
            winreg.CloseKey(regkey)
        winreg.CloseKey(reg)
        return value

    else:
        reg = winreg.ConnectRegistry(None, winreg.HKEY_LOCAL_MACHINE)
        with winreg.OpenKey(reg, r'SYSTEM\CurrentControlSet\Control\Session Manager\Environment') as regkey:
            # print("System Environment variables are:")
            # print("#", "name", "value", "type")
            try:
                value = winreg.QueryValueEx(regkey, envvar)[0]
            except:
                value = ""
                pass
            # print("The value of " + envvar + "is: " + value)
            winreg.CloseKey(regkey)
        winreg.CloseKey(reg)
        return value


def write_envvar(envvar: str, value: str, user_env=True):
    r"""function to write a local environment variable to windows

        Parameters
        ----------
        envvar : string
            name of the local environment variable
        value : string
            value of the local environment variable
        """
    # write environment variable under current user
    #reg = winreg.ConnectRegistry(None, winreg.HKEY_LOCAL_MACHINE)
    if user_env:
        output = read_envvar(envvar)
        if value not in output:
            value = output + ";" + value
            reg = winreg.ConnectRegistry(None, winreg.HKEY_CURRENT_USER)
            with winreg.OpenKey(reg, r'Environment', 0,
                                winreg.KEY_SET_VALUE) as regkey:
                winreg.SetValueEx(regkey, envvar, 0, winreg.REG_EXPAND_SZ, value)
    else:
        output = read_envvar(envvar, False)
        if value not in output:
            value = output + ";" + value
            reg = winreg.ConnectRegistry(None, winreg.HKEY_LOCAL_MACHINE)
            with winreg.OpenKey(reg, r'SYSTEM\CurrentControlSet\Control\Session Manager\Environment', 0,
                                winreg.KEY_SET_VALUE) as regkey:
                winreg.SetValueEx(regkey, envvar, 0, winreg.REG_EXPAND_SZ, value)


# write environment variables
write_envvar("Path", os.path.abspath(r"fmuZIP\LogicalModel\binaries\win64"))
write_envvar("Path", os.path.abspath(r"fmuZIP\LogicalModel\binaries\win64"), False)
