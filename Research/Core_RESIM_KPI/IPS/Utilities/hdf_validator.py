"""
HDF File Validator Module
Description:
This module provides utilities to validate HDF5 files before processing.
Validates file integrity, structure, and accessibility to prevent processing errors.
"""

import os
import h5py
from typing import Tuple, Optional


class HDFValidator:
    """Validator class for HDF5 files"""
    
    HDF5_MAGIC_NUMBER = b'\x89HDF\r\n\x1a\n'
    MIN_FILE_SIZE = 1024  # Minimum 1KB for valid HDF5
    
    @staticmethod
    def validate_hdf_file(file_path: str) -> Tuple[bool, Optional[str]]:
        """
        Validate HDF5 file integrity and structure.
        
        Args:
            file_path (str): Path to the HDF5 file to validate
            
        Returns:
            Tuple[bool, Optional[str]]: (is_valid, error_message)
                - is_valid: True if file is valid, False otherwise
                - error_message: Description of validation error if any
        """
        
        # Check file existence
        if not os.path.exists(file_path):
            return False, f"File does not exist: {file_path}"
        
        # Check if it's a file (not directory)
        if not os.path.isfile(file_path):
            return False, f"Path is not a file: {file_path}"
        
        # Check file accessibility and size
        try:
            file_size = os.path.getsize(file_path)
            if file_size == 0:
                return False, f"File is empty: {file_path}"
            if file_size < HDFValidator.MIN_FILE_SIZE:
                return False, f"File size is too small ({file_size} bytes). Minimum required: {HDFValidator.MIN_FILE_SIZE} bytes"
        except OSError as e:
            return False, f"Cannot access file: {e}"
        
        # Check HDF5 magic number
        try:
            with open(file_path, 'rb') as f:
                magic_number = f.read(8)
                if magic_number != HDFValidator.HDF5_MAGIC_NUMBER:
                    return False, f"Invalid HDF5 magic number. File may be corrupted or not an HDF5 file."
        except IOError as e:
            return False, f"Cannot read file: {e}"
        
        # Try to open and validate structure
        try:
            with h5py.File(file_path, 'r') as f:
                # Check if file has any content
                if len(f) == 0:
                    # Empty HDF5 file - this might be acceptable in some cases
                    # but we log it as a warning
                    pass
                # Try to access root attributes and datasets to verify integrity
                _ = f.keys()
        except (OSError, ValueError, RuntimeError) as e:
            error_msg = str(e)
            if "bad object header version number" in error_msg:
                return False, f"Corrupted HDF5 file - bad object header: {file_path}"
            elif "truncated file" in error_msg:
                return False, f"Truncated HDF5 file: {file_path}"
            elif "unable to synchronously open file" in error_msg:
                return False, f"Cannot open HDF5 file - file is corrupted: {file_path}"
            else:
                return False, f"HDF5 file validation error: {error_msg}"
        except Exception as e:
            return False, f"Unexpected error validating HDF5 file: {e}"
        
        return True, None
    
    @staticmethod
    def validate_hdf_pair(input_file: str, output_file: str) -> Tuple[bool, Optional[str]]:
        """
        Validate both input and output HDF5 files.
        
        Args:
            input_file (str): Path to input HDF5 file
            output_file (str): Path to output HDF5 file
            
        Returns:
            Tuple[bool, Optional[str]]: (are_valid, error_message)
        """
        
        # Validate input file
        is_valid, error_msg = HDFValidator.validate_hdf_file(input_file)
        if not is_valid:
            return False, f"Input file validation failed: {error_msg}"
        
        # Validate output file
        is_valid, error_msg = HDFValidator.validate_hdf_file(output_file)
        if not is_valid:
            return False, f"Output file validation failed: {error_msg}"
        
        return True, None
