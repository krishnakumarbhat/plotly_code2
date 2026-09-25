from CcaLib import *
import sys

@CCA_PROGRESS_CALLBACK
def cb(action, object, progress, desc):
    print("Action: " + str(action) + " Progress: " + str(progress) + " Descr: " + desc.decode('utf-8'))

if(len(sys.argv) == 3):
    CcaLib_init()
    CcaTrace_setDebugLevel(0)
    
    print('Connecting to: '+sys.argv[1])
    dev = CcaDevice_create(sys.argv[1])
    if dev == CCA_INVALID_HANDLE:
        sys.exit(1)
    
    if not CcaDevice_connect(dev):
        sys.exit(1)
    
    col = CcaSessionCollection_create()
    
    filt = CcaSessionFilter()
   
    if not CcaSession_scanDevice(dev, col, pointer(filt)):
        sys.exit(1)
    
    #if not CcaSessionCollection_downloadData(sys.argv[2], col, pointer(filt), False, True, False, cb, 0):
    #    sys.exit(1)
    
    if not CcaSessionCollection_executeDataSync(sys.argv[2], col, dev, pointer(filt), False, cb):
        sys.exit(1)
    
    CcaSessionCollection_destroy(col)
    CcaDevice_destroy(dev)
    
else:
    print("Unexpected argument count");
    print("Usage:");
    print("cca-download.py <logger_ip> <local_folder>");


    
    


