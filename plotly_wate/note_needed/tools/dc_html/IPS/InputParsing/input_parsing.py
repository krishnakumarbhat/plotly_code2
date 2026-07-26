"""
Description:
 Singleton class that loads JSON config with 'inputhdffiles' and 'outputhdffiles',
ensures they are equal length, and stores paths in dicts for later use.
"""
import os, json
from threading import Lock

class HDFConfigSingleton:
    _instance = None
    _lock = Lock()

    def __new__(cls, json_path=None):
        with cls._lock:
            if cls._instance is None:
                if json_path is None:
                    raise ValueError("Initial creation requires a json_path")
                cls._instance = super().__new__(cls)
                cls._instance._initialized = False
            return cls._instance

    def __init__(self, json_path=None):
        if self._initialized:
            return
        if json_path is None:
            raise ValueError("json_path is required for first init")
        self.json_path = json_path
        self.inputs, self.outputs = {}, {}
        self._load()
        self._initialized = True

    def _load(self):
        if not os.path.exists(self.json_path):
            raise FileNotFoundError(f"Config file not found: {self.json_path}")
        with open(self.json_path, 'r', encoding='utf-8') as f:
            cfg = json.load(f)
        in_files, out_files = cfg.get('InputHDF'), cfg.get('OutputHDF')
        if not isinstance(in_files, list) or not isinstance(out_files, list):
            raise ValueError("'InputHDF' and 'OutputHDF' must be lists")
        if len(in_files) != len(out_files):
            raise ValueError("Input/output lists must be the same length")
        self.inputs = dict(enumerate(in_files))
        self.outputs = dict(enumerate(out_files))

    def get_input(self, idx): return self.inputs.get(idx)
    def get_output(self, idx): return self.outputs.get(idx)
    def all_pairs(self): return list(zip(self.inputs.values(), self.outputs.values()))
