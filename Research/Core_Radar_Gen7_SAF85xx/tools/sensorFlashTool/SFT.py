"""Import python base installed modules here."""
import os
import subprocess
import time
import argparse
import serial
from koradserial import KoradSerial

DEFAULT_VARIANT = "srr7e"


class SftWrapper:
    """SftWrapper class to flash different variants."""

    def __init__(self, executable):
        """Init function to flash different variants."""
        self.executable = executable
        self.workingDir = "."
        self.quitAfterFlashing = 1
        self.flashIterations = 1
        self.socketstoFlash = 1

    def flashVariant(self, config_xml, variant):
        """Flashing different variants."""
        command = f"{self.executable} -c {config_xml} -v {variant} -s {self.socketstoFlash} -n {self.flashIterations} -q {self.quitAfterFlashing}"
        results = subprocess.run(command, cwd=self.workingDir, stdout=subprocess.PIPE, text=True)
        return self.parserResults(results)

    def parserResults(self, result):
        """Read the results and return the value."""
        for line in str(result.stdout).split():
            if "FAILED" in line:
                return False
        return True


def power_up_test(comPort):
    """Power Up Test."""
    print("Running Power Up Test...")
    powerSupply = KoradSerial(comPort, False)

    # Turn it off and ensure correct voltage
    chn = powerSupply.channels[0]
    powerSupply.output.off()
    time.sleep(2)

    # Set voltage and current settings
    chn.voltage = 12.50
    chn.current = 3.00

    powerSupply.output.on()
    print("End of Power Up Test")


def power_session_ON(comPort):
    """Power ON."""
    powerSupply = KoradSerial(comPort, False)
    powerSupply.output.on()


def power_session_OFF(comPort):
    """Power OFF."""
    powerSupply = KoradSerial(comPort, False)
    powerSupply.output.off()


if __name__ == "__main__":

    parser = argparse.ArgumentParser(
        description="APTIV Gen7 Radar Power Cycle Test", epilog="APTIV Ltd, copyright 2022"
    )
    parser.add_argument(
        "-c",
        "--comport",
        metavar="COM Port for Power Supply",
        type=str,
        required=False,
        default="",
    )
    parser.add_argument(
        "-v",
        "--variant",
        metavar="Platform Variant",
        type=str,
        required=False,
        default=DEFAULT_VARIANT,
    )
    parser.add_argument(
        "-e",
        "--sft_folder",
        metavar="folder containing sensorFlashTool.exe and config files",
        type=str,
        required=False,
        default=".",
    )

    args = parser.parse_args()
    comPort = args.comport
    variant = args.variant
    sft_folder = args.sft_folder

    try:
        ser = serial.Serial(comPort)
        ser.close()
    except Exception as e:
        print(str(e))
        print(f"ERROR: Port {comPort} is not available. Check connection or port name.")
        exit(-1)

    # Call power supply test
    power_up_test(comPort)

    sftWrapper = SftWrapper(os.path.join(sft_folder, "SensorFlashTool.exe"))
    sftWrapper.workingDir = sft_folder
    result = sftWrapper.flashVariant(
        variant + "_main.xml", variant.replace("srr", "SRR").replace("flr", "FLR") + "_01"
    )
    if result:
        print(variant, "flashing successful")
    else:
        print(variant, " flashing failed")
    # Power cycle for successful flashing
    power_session_OFF(comPort)
    time.sleep(2)
    power_session_ON(comPort)
