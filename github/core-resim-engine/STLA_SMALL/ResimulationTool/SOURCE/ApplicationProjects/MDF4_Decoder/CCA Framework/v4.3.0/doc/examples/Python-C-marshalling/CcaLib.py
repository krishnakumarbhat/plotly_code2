# Example file for the CCA-Framework-MDF: Demonstrates how to create a wrapper using C-marshalling.
#
# @copyright            Copyright (c) 2016-2023 ViGEM GmbH. All rights reserved.
#
# @file                 CcaLib.py
#
# @date                 2023-07-27
# @version              4.3.0
# @license              This example code from ViGEM GmbH has been provided
# for example and illustration purposes only.
#
# NOTE: THIS CODE IS PROVIDED FROM VIGEM GMBH "AS IS", WITHOUT ANY EXPRESS OR
# IMPLIED WARRANTY. VIGEM GMBH SHALL ONLY BE LIABLE WITHOUT LIMITATION UNDER THE
# GERMAN PRODUCT ACT (PRODUKTHAFTUNGSGESETZ). IN NO EVENT SHALL VIGEM GMBH BE
# LIABLE FOR ANY CLAIMS, DAMAGES OR OTHER LIABILITIES ARISING OUT OF THE USE OF
# THIS CODE, UNLESS VIGEM GMBH IS RESPONSIBLE FOR SUCH CLAIMS, DAMAGES OR OTHER
# LIABILITIES DUE TO INTENT OR GROSS NEGLIGENCE ON ITS PART.
#
# The rights of use the code are based on the terms of:
#   - the contract between the user and ViGEM GmbH,
#   - the General Terms and Conditions for Software of ViGEM GmbH.
#     NOTE: SECTION 6.2 OF THE GENERAL TERMS AND CONDITIONS FOR SOFTWARE OF
#           VIGEM GMBH DOES NOT APPLY FOR CODES PROVIDED FROM VIGEM GMBH FOR
#           EXAMPLE AND ILLUSTRATION PURPOSES.
# In addition shall apply supplementary:
#   - the General Terms and Conditions of ViGEM GmbH
#
# These additional applicable provisions can be sent to you by postal mail on
# request and can be viewed as electronic documents on our homepage and our
# customer portal at www.ViGEM.de.


from ctypes import *
import platform


# String to c_char_p converter
class c_string(object):
    @classmethod
    def from_param(cls, value):
        if isinstance(value, c_char_p):
            return value
        else:
            return value.encode('utf-8')


# if you installed the CCA-Framework-MDF somewhere else or want the 32 Bit library you have to change the path.
if platform.system() == 'Windows':
    CCA_LIBRARY_NAME = R"C:\Program Files\ViGEM GmbH\CCA Framework MDF\v4.3.0\bin\ccalib_mdf_x64_4.3.0.dll"
else:
    CCA_LIBRARY_NAME = "../../../../../lib/libccalib_mdf_x64_4.3.0.so"
CCA_PY_VERSION = "4.3.0"


MAX_PRODUCT_FAMILY_SIZE = 128
MAX_PRODUCT_GENERATION_SIZE = 128
MAX_PRODUCT_VARIANT_SIZE = 128
MAX_SERIAL_SIZE = 128
MAX_CUSTOMER_SIZE = 128
MAX_HG_TAG_SIZE = 128
MAX_HG_HASH_SIZE = 128
MAX_BUILD_DATE_SIZE = 128
MAX_HASH_SIZE = 128
MAX_HOST_NAME = 255
MAX_TIME_STAMP = 128
CCA_DEVICE_NAME_MAX = 64
CCA_DEVICE_DESCRIPTION_MAX = 100

CCA_INVALID_MARKER_ID = -1
CCA_STRUCT_INIT_MN = 0x22334455
CCA_INVALID_TIME_VALUE = 0x7FFFFFFFFFFFFFFF

# CcaRenameMode
CCA_RN_USE_GLOBAL = 0
CCA_RN_NONE = 1
CCA_RN_USE_FOLDER_NAME = 2
CCA_RN_USE_FIRST_FILENAME = 3
CCA_RN_USE_OUTPUT_FILENAME = 4


ccalib = CDLL(CCA_LIBRARY_NAME)      

CcaObject = c_void_p
CcaDevice = CcaObject
CcaTime = CcaObject
CcaRenameMode = c_int
vgm_bool = c_bool

CCA_PROGRESS_CALLBACK = CFUNCTYPE(None, c_int, CcaObject, c_uint32, c_char_p)
CCA_PROGRESS_CALLBACK_NULL = cast(None, CCA_PROGRESS_CALLBACK)
CCA_INVALID_HANDLE = None
CCA_TRACE_WRITE_CALLBACK = CFUNCTYPE(None, c_int32, c_char_p, c_char_p, c_int32, c_char_p)

# Helpers


def CcaLib_init():
    ccalib.CcaLib_initInternal(CcaLib_getVersion())


def defineFunction(name, restype, args=[]):
    globals()[name] = ccalib[name]
    globals()[name].restype = restype
    globals()[name].argtypes = args


def defineCollection(name):
    globals()[name+'Collection'] = CcaObject
    defineFunction(name+'Collection_create', CcaObject)
    defineFunction(name+'Collection_destroy', vgm_bool, [CcaObject])
    defineFunction(name+'Collection_count', c_long, [CcaObject])
    defineFunction(name+'Collection_get', CcaObject, [CcaObject, c_long])
    defineFunction(name+'Collection_clone', CcaObject, [CcaObject, c_long])
    defineFunction(name+'_destroy', vgm_bool, [CcaObject])


def cbuf(len):
    return create_string_buffer(len) 


# structs

# collections
defineCollection('CcaDevice')
defineCollection('CcaSession')


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


class Array(Structure):
    _fields_ = [
        ("Elements", c_void_p),
        ("Count", c_uint32),
    ]
    
    def __init__(self):
        self.Elements = None
        self.Count = 0

    def free(self):
        CcaLib_freeArray(self.Elements)
    
    def cast(self, target_type):
        return cast(self.Elements, POINTER(target_type))


class CcaSessionFilter(Structure):
    _fields_ = [
        ("begin", CcaTime),
        ("end", CcaTime),
        ("ccaName", c_char_p),
        ("labels", Array),
        ("markerId", c_int32),
        ("markerPreTime", c_uint32),
        ("markerPostTime", c_uint32),
        ("scanPartialFiles", vgm_bool),
        ("filterFilesInSessions", vgm_bool),
        ("combineContSessions", vgm_bool),
        ("init_specifier", c_uint32),
        ("scanAdditionalFiles", vgm_bool),
        ("outputFileName", c_char_p),
        ("renameMode", CcaRenameMode),
        ("fileFilterSettings", Array),
    ]
    
    def __init__(self):
        self.begin = CCA_INVALID_HANDLE
        self.end = CCA_INVALID_HANDLE
        self.ccaName = None
        self.labels = Array()
        self.markerId = CCA_INVALID_MARKER_ID
        self.markerPreTime = 0
        self.markerPostTime = 0
        self.scanPartialFiles = False
        self.filterFilesInSessions = False
        self.combineContSessions = False
        self.init_specifier = CCA_STRUCT_INIT_MN
        self.scanAdditionalFiles = True
        self.outputFileName = None
        self.renameMode = CCA_RN_NONE
        self.fileFilterSettings = Array()
            
    
class CcaCaptureInterfaceInfo(Structure):
    _fields_ = [
        ("enabled", vgm_bool),
        ("busId", c_uint32),
        ("cls", c_int),
        ("slot", c_int),
        ("subslot", c_int),
        ("init_specifier", c_uint32),
    ]
  

# Function definitions

# lib
defineFunction('CcaLib_isInit', vgm_bool)
defineFunction('CcaLib_getVersion', c_char_p)
defineFunction('CcaLib_getLastError', c_uint32)
defineFunction('CcaLib_freeArray', None, [c_void_p])

# device
defineFunction('CcaDevice_create', CcaDevice, [c_string])
defineFunction('CcaDevice_destroy', vgm_bool, [CcaDevice])
defineFunction('CcaDevice_disconnect', vgm_bool, [CcaDevice])
defineFunction('CcaDevice_connect', vgm_bool, [CcaDevice, vgm_bool])

defineFunction('CcaDevice_reboot', vgm_bool, [CcaDevice])
defineFunction('CcaDevice_shutdown', vgm_bool, [CcaDevice])

defineFunction('CcaDevice_startCapture', vgm_bool, [CcaDevice])
defineFunction('CcaDevice_stopCapture', vgm_bool, [CcaDevice])

defineFunction('CcaDevice_getIpAddress', vgm_bool, [CcaDevice, c_char_p, c_long])
defineFunction('CcaDevice_getDescription', vgm_bool, [CcaDevice, c_char_p, c_long])
defineFunction('CcaDevice_getMacAddress', vgm_bool, [CcaDevice, c_char_p, c_long])
defineFunction('CcaDevice_getName', vgm_bool, [CcaDevice, c_char_p, c_long])
defineFunction('CcaDevice_getSystemInfo', vgm_bool, [CcaDevice, POINTER(CcaSystemInfo)])
defineFunction('CcaDevice_getConfiguration', vgm_bool, [CcaDevice, c_string])

defineFunction('CcaSession_scanDevice', vgm_bool, [CcaDevice, c_void_p, POINTER(CcaSessionFilter)])
defineFunction('CcaSessionCollection_executeDataSync', vgm_bool,
               [c_string, CcaSessionCollection, CcaDevice, POINTER(CcaSessionFilter), vgm_bool, CCA_PROGRESS_CALLBACK])
defineFunction('CcaSessionCollection_downloadData', vgm_bool,
               [c_string, CcaSessionCollection, POINTER(CcaSessionFilter), vgm_bool, vgm_bool, vgm_bool,
                CCA_PROGRESS_CALLBACK, c_uint32])


# settings
defineFunction('CcaSettings_fromDevice', c_void_p, [c_void_p])
defineFunction('CcaSettings_fromFile', c_void_p, [c_string])
defineFunction('CcaSettings_destroy', vgm_bool, [c_void_p])
defineFunction('CcaSettings_listInterfaces', vgm_bool, [c_void_p, POINTER(Array)])
defineFunction('CcaDevice_addPreset', vgm_bool, [c_void_p, c_void_p, vgm_bool])
defineFunction('CcaSettings_setName', vgm_bool, [c_void_p, c_string])

# finder
defineFunction('CcaFinder_scan', c_void_p, [c_void_p])

# trace
defineFunction('CcaTrace_setDebugLevel', vgm_bool, [c_int])
defineFunction('CcaTrace_setWriteCallback', vgm_bool, [CCA_TRACE_WRITE_CALLBACK])


