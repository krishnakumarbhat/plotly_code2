# -*- coding: utf-8 -*-
"""
Created on Created on Wed Dec 10 08:38:57 2025

@author: d1cse7 (mandeep.singh1@aptiv.com)
"""
from smoketestdc_static import *

#***** function to import module dynamically *****
def import_module_dynamically(file_path):
    file_path = Path(file_path)
    module_name = file_path.stem
    spec = importlib.util.spec_from_file_location(module_name, file_path)
    #print(file_path, module_name,spec)
    if spec and spec.loader:
        module = importlib.util.module_from_spec(spec)
        sys.modules[module_name] = module
        try:
            spec.loader.exec_module(module)
            #print(f"✓ Module '{module_name}' imported successfully.")
            return module
        except Exception as e:
            print(f"[ERROR] :importing module: {e}")
            import traceback
            traceback.print_exc()
            return None
    else:
        print(f"[ERROR] : Could not create module spec for '{file_path}'")
        return None
    

def call_multiplex_api(module, sil_engine_config, output_dir): 
    if module is None:
        print("[ERROR]: Module is None, cannot call API.")
        return None
    if not hasattr(module, 'multiplex_silEngine_config'):
        print(f"[ERROR]: API 'multiplex_silEngine_config' not found in {module}.")
        #available = [attr for attr in dir(module) if not attr.startswith('_') and callable(getattr(module, attr))]
        #print(f"[M-INFO] : Available functions: {available}")
        sys.exit(1)
    try:
        result = module.multiplex_silEngine_config(sil_engine_config, output_dir)
        return result
    except Exception as e:
        print(f"[ERROR]: Calling API 'multiplex_silEngine_config' failed: {e}")
        import traceback
        traceback.print_exc()
        return None
    
def multiplex_silEngine(sil_engine_config, module_file, output_dir):
    module = import_module_dynamically(module_file)
    return call_multiplex_api(module, sil_engine_config, output_dir)

