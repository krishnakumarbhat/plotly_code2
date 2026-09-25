Configuration:
From the CMake build directory:
cmake -G "Visual Studio 14 Win64" ..


Open two terminals for the transmitter and receiver and export the below environment variables.

Terminal 1 - Transmitter
set VSOMEIP_APPLICATION_NAME=srr_master_tx
set VSOMEIP_CONFIGURATION=C:\Users\gjdwf9\wkspaces\10030156_03_Analysis_Framework_Suite\VSOMEIP\VSOMEIP_Gateway\Config\vsomeip_config_srr_master.json
srr_master_appl.exe

Terminal 2 - Receiver
set VSOMEIP_APPLICATION_NAME=srr_master_rx
set VSOMEIP_CONFIGURATION=C:\Users\gjdwf9\wkspaces\10030156_03_Analysis_Framework_Suite\VSOMEIP\VSOMEIP_Gateway\Config\vsomeip_config_srr_master.json
test_receiver.exe

For now, the environment variables are set in the code itself and can be modified there.
