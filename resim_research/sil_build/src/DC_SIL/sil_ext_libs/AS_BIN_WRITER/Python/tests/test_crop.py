import unittest
import os
import pathlib

import numpy as np

import pybin.crop as pbc
import pybin.read as pbr

import pybin_test_files as ptf


class TestCrop(unittest.TestCase):
    crop_dir = os.path.join(ptf.bin_dir, "crop")

    bin_names = None
    bin_ext = None
    bin_paths = None

    @classmethod
    def setUpClass(cls):
        # arrange: Build list of bin files to read
        ptf.extract_test_files(mat_files=False)
        cls.bin_names, cls.bin_ext, cls.bin_paths = ptf.list_bin_files()

        # Create folder for temporary test files
        pathlib.Path(cls.crop_dir).mkdir(exist_ok=True)

    @classmethod
    def tearDownClass(cls):
        # Remove bin files created for the test
        ptf.remove_test_files()

    def test_crop(self):
        """Validate the output of the pybin crop against a manual cropping of the same bin file."""
        # arrange: Set the number of samples after cropping to 1
        new_num_samples = 1

        for bin_name, bin_ext, bin_path in zip(self.bin_names, self.bin_ext, self.bin_paths):
            # action Crop the bin file and save it
            test_file_path = os.path.join(self.crop_dir, bin_name + bin_ext)
            pbc.crop(bin_path, new_num_samples, test_file_path)

            # assert: Data read from the cropped bin file is an exact match of the data from a
            # manual cropping of the original bin file
            self.assert_crop(bin_path, test_file_path, bin_name, new_num_samples)

    def assert_crop(self, bin_path, cropped_path, name, new_num_samples):
        """Compares the output of the pybin crop with a manual cropping of the same bin file.

        Args:
            bin_path: Path to the original bin file
            cropped_path: Path to the cropped bin file
            name: Name of the bin file
            new_num_samples: The desired number of samples after cropping
        """
        data_full = pbr.read(bin_path)
        data_crop = pbr.read(cropped_path)

        for k in data_full.keys():
            with self.subTest(msg=f"Filename: {name} | Signal: {k}"):
                data_expect = data_full[k][:new_num_samples]
                data = data_crop[k]
                self.assertTrue(np.array_equal(data_expect, data))

    def test_crop_below_one_num_samples(self):
        """An error should be raised if the number of samples after cropping is below one."""
        # arrange: Select a bin file to crop
        bin_name = self.bin_names[0]
        bin_ext = self.bin_ext[0]
        bin_path = self.bin_paths[0]

        # action: Crop the bin file to 0 samples
        # assert: An error is raised
        with self.assertRaisesRegex(ValueError, "greater than 0"):
            test_file_path = os.path.join(self.crop_dir, bin_name + bin_ext)
            pbc.crop(bin_path, 0, test_file_path)

        # action: Crop the bin file to -2 samples
        # assert: An error is raised
        with self.assertRaisesRegex(ValueError, "greater than 0"):
            test_file_path = os.path.join(self.crop_dir, bin_name + bin_ext)
            pbc.crop(bin_path, -2, test_file_path)

    def test_crop_equal_to_num_samples(self):
        """An error should be raised if the number of samples after cropping is greater of equal
        to the number of samples in the original bin file."""
        # arrange: Select a bin file to crop
        bin_name = self.bin_names[0]
        bin_ext = self.bin_ext[0]
        bin_path = self.bin_paths[0]

        # action: Crop the bin file to the same number of samples as the original bin file
        # assert: An error is raised
        with self.assertRaisesRegex(ValueError, "less than"):
            test_file_path = os.path.join(self.crop_dir, bin_name + bin_ext)
            num_samples = pbc.get_num_samples(bin_path)
            pbc.crop(bin_path, num_samples, test_file_path)

        # action: Crop the bin file to a number of samples greater than the original bin file
        # assert: An error is raised
        with self.assertRaisesRegex(ValueError, "less than"):
            test_file_path = os.path.join(self.crop_dir, bin_name + bin_ext)
            num_samples = pbc.get_num_samples(bin_path)
            pbc.crop(bin_path, num_samples + 1, test_file_path)


class TestNumSamples(unittest.TestCase):
    bin_paths = None

    @classmethod
    def setUpClass(cls):
        # arrange: Build list of bin files to read
        ptf.extract_test_files(mat_files=False)
        _, _, cls.bin_paths = ptf.list_bin_files()

    @classmethod
    def tearDownClass(cls):
        # Remove bin files created for the test
        ptf.remove_test_files()

    def test_get_num_samples(self):
        """Test that 'get_num_samples' returns the correct number of samples in the bin file."""
        # arrange: Read a bin file and get the number of samples in the returned data
        bin_path = self.bin_paths[0]
        data = pbr.read(bin_path)

        # All values should have the same number of samples. We can just pop anything from the
        # dictionary
        _, v = data.popitem()
        expect_num_samples = v.shape[0]

        # action: Call FUT to get the number of samples in the bin file
        num_samples = pbc.get_num_samples(bin_path)

        # assert: Number of samples returned by the FUT is equal to the expected number of samples
        self.assertEqual(expect_num_samples, num_samples)


if __name__ == "__main__":
    unittest.main()
