import sys
import json
from logger import logger

class Metadata:
   SUPPORTED_VERSION = "1.1"

   def __init__(self, data: dict):
      """
      Initialize Metadata instance from dictionary
      """
      self._data = {k: v for k, v in data.items() if k != "version"}
   
   @classmethod
   def from_file(cls, filename: str) -> 'Metadata':
      """
      Factory method to create Metadata instance from JSON file with version check
      """
      with open(filename, 'r') as file:
         data = json.load(file)
      
      file_version = data.get("version")
      if file_version != cls.SUPPORTED_VERSION:
         error_string = f"[ERROR]: Incompatible metadata version: {file_version}. Supported version is {cls.SUPPORTED_VERSION}."
         logger.custom_print(error_string)
         sys.exit(1)
      return cls(data)

   def to_dict(self) -> dict:
      """Return metadata as a dictionary"""
      return self._data.copy()

   def __repr__(self) -> str:
      return f"<Metadata: {self._data}>"

   def __getitem__(self, key):
      """Allow dictionary-style access: metadata['key']"""
      return self._data[key]

   def get(self, key, default=None):
      """Dictionary-style get with default"""
      return self._data.get(key, default)
