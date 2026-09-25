# Import python base installed modules here
from datetime import datetime
import pythoncom
import time

# Method, Class, and Other definitions here (bulk of code)


def com_sleep(time_s: float) -> None:
    """
    Utility function to sleep while pumping messages.

    Args:
        time_s: The time to sleep for.

    NOTE:
        The minimum time resolution of this function is about 0.1 s,
        so the sleep will be the smallest multiple of 0.1 greater
        than time_s.
    """
    start = time.time()
    while time.time() - start < time_s:  # While time elapsed is less than the time specified
        pythoncom.PumpWaitingMessages()
        time.sleep(0.1)


def str_now():
    return datetime.now().strftime('%m-%d-%Y %H:%M:%S.%f')


def file_time():
    return datetime.now().strftime('%b_%d_%Y_%H-%M-%S')


def check_symb_val(t32_session, var_name, val_check, check_name=""):
    check_val = t32_session.read_from_symbol(var_name)
    if check_val is False:
        result = False
        assert result, f"Unable to Read {var_name} Value"
    elif check_val == val_check:
        if check_name == "":
            print(f"{var_name} ({check_val}) == {val_check}")
        else:
            print(f"{var_name} ({check_val}) == {check_name} ({val_check})")
    else:
        if check_name == "":
            result = False
            print(f"{var_name} ({check_val}) != {val_check}")
            assert result, f"{var_name} ({check_val}) != {val_check}"
        else:
            result = False
            print(f"{var_name} ({check_val}) != {check_name} ({val_check})")
            assert result, f"{var_name} ({check_val}) != {check_name} ({val_check})"
