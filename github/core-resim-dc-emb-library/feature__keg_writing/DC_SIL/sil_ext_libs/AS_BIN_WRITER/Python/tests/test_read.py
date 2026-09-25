import unittest
import os
import scipy.io as sio
import numpy as np

import pybin.read as pbr

import pybin_test_files as ptf


class TestRead(unittest.TestCase):
    bin_names = None
    bin_paths = None

    @classmethod
    def setUpClass(cls):
        # arrange: Build list of bin files to read
        ptf.extract_test_files(mat_files=True)
        cls.bin_names, _, cls.bin_paths = ptf.list_bin_files()

    @classmethod
    def tearDownClass(cls):
        ptf.remove_test_files()

    def test_read_signal_indexing(self):
        """Validate pybin reader against MatLab bin reader with signal indexing."""
        for bin_name, bin_path in zip(self.bin_names, self.bin_paths):
            # action: Read the bin file with pybin with sample indexing disabled (performs signal
            # indexing)
            pbr_data = pbr.read(bin_path, sample_indexing=False)

            # assert: Build path to mat file, read it's contents and compare with output of pybin
            mat_path = os.path.join(ptf.mat_dir, "signal_indexing", bin_name + ".mat")
            mat_gt = sio.loadmat(mat_path)
            self.assert_signal_indexing(bin_name, pbr_data, mat_gt)

    def assert_signal_indexing(self, f_name, pbr_data, matlab_data):
        """Compares the output of the pybin reader against the MatLab ground-truth with signal
        indexing.

        Args:
            f_name: The filename where the data is read from.
            pbr_data: The output of the pybin reader.
            matlab_data: The output of the MatLab bin reader.
        """
        matlab_data = matlab_data["data"][0, 0]
        for signal in matlab_data.dtype.names:
            with self.subTest(msg=f"Filename: {f_name} | Signal: {signal}"):
                pbr_arr = pbr_data[signal]
                mat_arr = matlab_data[signal].squeeze()
                self.assertTrue(np.array_equal(pbr_arr, mat_arr))

    def test_read_sample_indexing(self):
        """Validate pybin reader against MatLab bin reader with sample indexing."""
        for bin_name, bin_path in zip(self.bin_names, self.bin_paths):
            # action: Read the bin file with pybin with sample indexing enabled
            pbr_data = pbr.read(bin_path, sample_indexing=True)

            # assert: Build path to mat file, read it's contents and compare with output of pybin
            mat_path = os.path.join(ptf.mat_dir, "sample_indexing", bin_name + ".mat")
            mat_gt = sio.loadmat(mat_path)
            self.assert_sample_indexing(bin_name, pbr_data, mat_gt)

    def assert_sample_indexing(self, f_name, pbr_data, matlab_data):
        """Compares the output of the pybin reader against the MatLab ground-truth with sample
        indexing.

        Args:
            f_name: The filename where the data is read from.
            pbr_data: The output of the pybin reader.
            matlab_data: The output of the MatLab bin reader.
        """
        matlab_data = matlab_data["data"].squeeze()
        for idx, sample in enumerate(matlab_data):
            for signal in sample.dtype.names:
                with self.subTest(msg=f"Filename: {f_name} | Sample: {idx} | Signal: {signal}"):
                    pbr_arr = pbr_data[idx][signal]
                    mat_arr = sample[signal].squeeze()
                    self.assertTrue(np.array_equal(pbr_arr, mat_arr))


if __name__ == "__main__":
    unittest.main()
