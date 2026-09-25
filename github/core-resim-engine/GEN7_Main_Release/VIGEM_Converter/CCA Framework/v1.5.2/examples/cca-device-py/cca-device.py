from CcaLib import *
import sys


ok = False
vect = []

if(len(sys.argv) == 3):
    print(CcaLib_getVersion() + ' - ' + CCA_PY_VERSION)
    CcaLib_init()
    devices = CcaDeviceCollection_create();
    ok = CcaFinder_scan(devices);
    for i in range(CcaDeviceCollection_count(devices)):
        device = CcaDeviceCollection_get(devices, i)
        ip = cbuf(20)
        CcaDevice_getIpAddress(device, ip, 20)
        if ip.value == sys.argv[1]:
            description = cbuf(100)
            mac = cbuf(30)
            name = cbuf(40)
            
            ok = CcaDevice_connect(device);
            ok = CcaDevice_getIpAddress(device, ip, 20);
            ok = CcaDevice_getDescription(device, description, 100);
            ok = CcaDevice_getMacAddress(device, mac, 30);
            ok = CcaDevice_getName(device, name, 40);
                        
            print('Name: '+ name.value)
            print('Ip: '+ ip.value)
            print('Mac: '+ mac.value)
            print('Desc: '+ description.value)


            print("")
            print("SysInfo:")
            sysinfo = CcaSystemInfo("","","","","",((0,0,0),"","","",""))
            CcaDevice_getSystemInfo(device, sysinfo)
            print("product_family: "+sysinfo.product_family)
            print("product_generation: "+sysinfo.product_generation)
            print("product_variant: "+sysinfo.product_variant)
            print("serial: "+sysinfo.serial)
            print("customer: "+sysinfo.customer)
            print("image.version.major: "+str(sysinfo.image.version.major))
            print("image.version.minor: "+str(sysinfo.image.version.minor))
            print("image.version.build: "+str(sysinfo.image.version.build))
            print("image.hg_tag: "+sysinfo.image.hg_tag)
            print("image.hg_hash: "+sysinfo.image.hg_hash)
            print("image.build_date: "+sysinfo.image.build_date)
            print("image.hash: "+sysinfo.image.hash)           
            print("")
            
            if(sys.argv[2] == "start"):
                print("Start logging ...")
                ok = CcaDevice_startCapture(device)
            else:
                print("Stop logging.")
                ok = CcaDevice_stopCapture(device)
            
            CcaDevice_disconnect(device)

    
    ok = CcaDeviceCollection_destroy(devices)
    
else:
    print("Unexpected argument count");
    print("Usage:");
    print("cca-device.py ip_address [start|stop]");


    
    


