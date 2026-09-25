from CcaLib import *
import sys


@CCA_TRACE_WRITE_CALLBACK
def trace(lvl, file, function, line, format, va_list):
    print(format)

if(len(sys.argv) == 3):
    CcaLib_init()
    CcaTrace_setWriteCallback(trace)
    print('Connecting to: '+sys.argv[1])
    dev = CcaDevice_create(sys.argv[1])
    if dev == CCA_INVALID_HANDLE:
        sys.exit(1)
    
    if not CcaDevice_connect(dev):
        sys.exit(1)
    
    print('Loading settings from: '+sys.argv[2])
    set = CcaSettings_fromFile(sys.argv[2])
    if set == CCA_INVALID_HANDLE:
        sys.exit(1)
    
    
    arr = Array()
    if not CcaSettings_listInterfaces(set, arr):
        sys.exit(1)
    
    print('Settings have following BusIds activated:')
    sarr = arr.cast(CcaCaptureInterfaceInfo)
    for i in range(arr.Count):
        if sarr[i].enabled and sarr[i].busId != 0:
            print(' - '+str(sarr[i].busId))
    
    if(not CcaSettings_setName(set, 'test_settings')):
        sys.exit(1)
    
    print('Setting added as preset "test_settings" and loaded')
    if not CcaDevice_addPreset(dev, set, True):
        sys.exit(1)
    
    CcaSettings_destroy(set)
    CcaDevice_destroy(dev)
    
else:
    print("Unexpected argument count");
    print("Usage:");
    print("cca-config.py <logger_ip> <config_folder>");


    
    


