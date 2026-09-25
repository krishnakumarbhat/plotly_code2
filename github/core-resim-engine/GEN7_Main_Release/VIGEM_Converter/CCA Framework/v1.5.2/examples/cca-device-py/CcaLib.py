from ctypes import *

CCA_LIBRARY_NAME = "ccalib_x86_1-5-2"
CCA_PY_VERSION = "1.5.2"


MAX_PRODUCT_FAMILY_SIZE      = 20
MAX_PRODUCT_GENERATION_SIZE  = 20
MAX_PRODUCT_VARIANT_SIZE     = 20
MAX_SERIAL_SIZE              = 20
MAX_CUSTOMER_SIZE            = 20
MAX_HG_TAG_SIZE              = 40
MAX_HG_HASH_SIZE             = 20
MAX_BUILD_DATE_SIZE          = 20
MAX_HASH_SIZE                = 20

ccalib = CDLL(CCA_LIBRARY_NAME)      

# Helpers

def CcaLib_init():
    ccalib.CcaLib_initInternal(CCA_PY_VERSION)

def defineFunction(name, restype, args=[]):
    globals()[name] = ccalib[name]
    globals()[name].restype = restype
    globals()[name].argtypes = args

def defineCollection(name):
    defineFunction(name+'Collection_create', c_void_p)
    defineFunction(name+'Collection_destroy', c_bool, [c_void_p])
    defineFunction(name+'Collection_count', c_long, [c_void_p])
    defineFunction(name+'Collection_get', c_void_p, [c_void_p, c_long])
    defineFunction(name+'Collection_clone', c_void_p, [c_void_p, c_long])
    defineFunction(name+'_destroy', c_bool, [c_void_p])

def cbuf(len):
    return create_string_buffer(len) 

#structs

class CcaVersionNumber(Structure):
    _fields_ = [
        ("major", c_long),
        ("minor", c_long),
        ("build", c_long),
        ]   

class CcaFirmwareImage(Structure):
    _fields_ = [
        ("version", CcaVersionNumber),
        ("hg_tag", c_char*MAX_HG_TAG_SIZE),
        ("hg_hash", c_char*MAX_HG_HASH_SIZE),
        ("build_date", c_char*MAX_BUILD_DATE_SIZE),
        ("hash", c_char*MAX_HASH_SIZE),
        ]   


class CcaSystemInfo(Structure):
    _fields_ = [
        ("product_family", c_char*MAX_PRODUCT_FAMILY_SIZE),
        ("product_generation", c_char*MAX_PRODUCT_GENERATION_SIZE),
        ("product_variant", c_char*MAX_PRODUCT_VARIANT_SIZE),
        ("serial", c_char*MAX_SERIAL_SIZE),
        ("customer", c_char*MAX_CUSTOMER_SIZE),
        ("image", CcaFirmwareImage),
        ]   
    
    
    
    
#Function definitions

# lib
defineFunction('CcaLib_isInit', c_bool)
defineFunction('CcaLib_getVersion', c_char_p)

# device
defineFunction('CcaDevice_create', c_void_p, [c_char_p])
defineFunction('CcaDevice_destroy', c_bool, [c_void_p])
defineFunction('CcaDevice_disconnect', c_bool, [c_void_p])
defineFunction('CcaDevice_connect', c_bool, [c_void_p])

defineFunction('CcaDevice_reboot', c_bool, [c_void_p])
defineFunction('CcaDevice_shutdown', c_bool, [c_void_p])

defineFunction('CcaDevice_startCapture', c_bool, [c_void_p])
defineFunction('CcaDevice_stopCapture', c_bool, [c_void_p])

defineFunction('CcaDevice_getIpAddress', c_bool, [c_void_p, c_char_p, c_long])
defineFunction('CcaDevice_getDescription', c_bool, [c_void_p, c_char_p, c_long])
defineFunction('CcaDevice_getMacAddress', c_bool, [c_void_p, c_char_p, c_long])
defineFunction('CcaDevice_getName', c_bool, [c_void_p, c_char_p, c_long])
defineFunction('CcaDevice_getSystemInfo', c_bool, [c_void_p, POINTER(CcaSystemInfo)])
defineFunction('CcaDevice_getConfiguration', c_bool, [c_void_p, c_char_p])
defineFunction('Cca_convertConfigToXML', c_bool, [c_char_p, c_char_p])



# finder
defineFunction('CcaFinder_scan', c_void_p, [c_void_p])

# collections
defineCollection('CcaDevice')
