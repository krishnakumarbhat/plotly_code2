"""
File Name: radar_dataprep.py
Author: Bharanidharan Subramaniam
Email : Bharanidharan.s@aptiv.com
Description:
This module uses Radar Data Store(RDS) and prepare the data for
plotting. RDS have signal data in 2D format.
In this module convert 2D to 1D.
It also scales up scan index as per dimension of 2D data
Example : Range Signal 2D ( rows:200, column : 250)
Each row    ---> corresponds to Scan index
Each Column ---> corresponds to data

"""

from IPS.EventMan.ievent_mediator import IEventMediator
from IPS.DataStore.idatastore import IDataStore
from IPS.DataCollect.radar_hdf_datacollect import RadarHDFDataCollect
import numpy as np


class RadarDataPreparation:
    input_scan_index_arr = None
    output_scan_index_arr = None

    def __init__(self, event_mediator: IEventMediator,
                 radar_datastore: IDataStore,
                 plot_datastore: IDataStore, ):
        self._data_event_mediator = event_mediator
        self._radar_datastore = radar_datastore
        self._plot_datastore = plot_datastore

    def clear_data(self):

        RadarDataPreparation.input_scan_index_arr = None
        RadarDataPreparation.output_scan_index_arr = None

    def check_type(data):
        if isinstance(data, np.ndarray):
            return "NumPy array"
        elif isinstance(data, list):
            return "Python list"
        else:
            return "Other type"

    def consume_event(self, sensor, event):
        """
        Once HDF data collection is completed, an event RDS_UPDATED is notified.
        the event RDS_UPDATED is consumed here. Here we call flatten ( conversion of 2d to 1d)
        scan index and flatten ( conversion of 2d to 1d) radar data and store the flattened
        in plot data store (PDS).

        an event PDS_UPDATED is notified
        """

        if event == "RDS_UPDATED":

            signal_list = RadarHDFDataCollect.sensor_to_sig[sensor]

            target_signal_to_remove = 'scan_index'
            try:
                signal_list.remove(target_signal_to_remove)  # O(n)
                signal_list.insert(0, target_signal_to_remove)  # O(n)
            except ValueError:
                pass  # target not in list; do nothing or handle as needed

            for sig in signal_list:
                # print("signal", sig)
                if sig == 'scan_index':
                    self.flatten_scan_index(sensor, sig)

                if sig != 'scan_index':
                    self.flatten_radar_data(sensor, sig)
                    # self.compute_signal_difference(sig)
                    # self.compute_non_zero_sig_value(sig)

            self._data_event_mediator.notify_event(sensor, "PDS_UPDATED")  # PDS--> Plot Data store

    def flatten_scan_index(self, sensor, sig):
        """
           This function get scan index from radar data store and convert 2D scan index
           to 1D scan index using ravel() and update flatted scan index to plot data store
        """
        # print("flatten_scan_index", sig)
        input_scan_index_arr = self._radar_datastore.get_data(sensor, sig, "in")
        input_scan_index_arr = input_scan_index_arr.ravel()
        self._plot_datastore.update_data(sig, input_scan_index_arr, "in")

        output_scan_index_arr = self._radar_datastore.get_data(sensor, sig, "out")
        output_scan_index_arr = output_scan_index_arr.ravel()
        self._plot_datastore.update_data(sig, output_scan_index_arr, "out")

    def flatten_radar_data(self, sensor, sig):
        """
        This function gets radar data(2D) of signal from radar data store and takes only two decimal
        places and getting the dimension of signal and update signal dimension to plot data store.
        updated signal dimension is used as scaling factor for scan index duplication ( required for plotting)

        then convert 2D radar data to 1D data and update to plot data store.

        signals like phi and theta are converted from radian to degree and updated to plot data store.
        From plot data store we can retrieve to plot scatter plots

        """
        # print("flatten_radar_data", sig)
        sig_data_in = self._radar_datastore.get_data(sensor, sig, "in")
        # rounded_sig_data = (sig_data * 100).astype(int) / 100.0
        # input_scan_index_scaling_factor = rounded_sig_data.shape[1] if rounded_sig_data.ndim == 2 else "Not 2D array"

        input_scan_index_scaling_factor = sig_data_in.shape[1] if sig_data_in.ndim == 2 else "Not 2D array"

        if input_scan_index_scaling_factor != "Not 2D array":
            self._plot_datastore.update_data(str(sig) + "Dim", input_scan_index_scaling_factor, "in")
            scan_index_in = self._plot_datastore.get_data('scan_index', "in")
            scaled_scan_index_array_in = np.repeat(scan_index_in, input_scan_index_scaling_factor)
            self._plot_datastore.update_data(sig,
                                             scaled_scan_index_array_in,
                                             "in_scan_index", None, sensor)
        # rounded_sig_data = rounded_sig_data.ravel()  # ravel(convert 2d to 1d data)
        rounded_sig_data = sig_data_in.ravel()  # ravel(convert 2d to 1d data)

        if str(sig) == "theta" or str(sig) == "phi" or str(sig) == "std_theta_1" or str(
                sig) == "std_phi_1":  # converting rad to degree in place
            rounded_sig_data = rounded_sig_data.astype(np.float32)
            np.degrees(rounded_sig_data, out=rounded_sig_data)
        '''
        if str(sig) == "veh_speed":
            rounded_sig_data *= 3.6
        '''
        self._plot_datastore.update_data(sig, rounded_sig_data, "in")

        sig_data_out = self._radar_datastore.get_data(sensor, sig, "out")
        # rounded_sig_data = (sig_data * 100).astype(int) / 100.0
        # output_scan_index_scaling_factor = rounded_sig_data.shape[1] if rounded_sig_data.ndim == 2 else "Not 2D array"

        # rounded_sig_data = (sig_data * 100).astype(int) / 100.0
        output_scan_index_scaling_factor = sig_data_out.shape[1] if sig_data_out.ndim == 2 else "Not 2D array"

        if output_scan_index_scaling_factor != "Not 2D array":
            self._plot_datastore.update_data(sig + "Dim", output_scan_index_scaling_factor, "out")

            scan_index_out = self._plot_datastore.get_data('scan_index', "out")

            # Step 1: Find matches using np.isin
            mask = np.isin(scan_index_out, scan_index_in)

            # Step 2: Retain only matching rows from output_value_numpy2d
            matched_output_values_out = sig_data_out[mask]

            # Step 3: Also get corresponding scan indices if needed
            matched_scan_indices_out = scan_index_out[mask]

            scaled_scan_index_array_out = np.repeat(matched_scan_indices_out, output_scan_index_scaling_factor)

            self._plot_datastore.update_data(sig,
                                             scaled_scan_index_array_out,
                                             "out_scan_index", None, sensor)
        # rounded_sig_data = rounded_sig_data.ravel()
        rounded_sig_data = matched_output_values_out.ravel()

        if str(sig) == "theta" or str(sig) == "phi":  # converting rad to degree in place
            # rounded_sig_data = rounded_sig_data.astype(np.float32)
            # np.degrees(rounded_sig_data, out=rounded_sig_data)
            rounded_sig_data = rounded_sig_data.astype(np.float32)
            np.degrees(rounded_sig_data, out=rounded_sig_data)
        '''
        if str(sig) == "veh_speed":
            rounded_sig_data *= 3.6
        '''
        self._plot_datastore.update_data(sig, rounded_sig_data, "out")

    def compute_signal_difference(self, sig):

        if sig == "num_af_det":
            # print(f"compute_signal_difference for signal {sig}")
            input_signal_value = self._radar_datastore.get_data(sig, "in")
            # print("input_signal_value", input_signal_value)
            output_signal_value = self._radar_datastore.get_data(sig, "out")
            # print("output_signal_value", output_signal_value)

            differance_signal_value = input_signal_value - output_signal_value
            self._plot_datastore.update_data(sig + "_diff", differance_signal_value, "special")

    def compute_non_zero_sig_value(self, sig):

        if sig == "f_bistatic" or sig == "f_single_target" or sig == "f_superres_target":

            input_signal_value = self._radar_datastore.get_data(sig, "in")
            input_nonzero_counts_row_wise = np.sum(input_signal_value == 1, axis=1)

            self._plot_datastore.update_data(sig, input_nonzero_counts_row_wise, "in")
            output_signal_value = self._radar_datastore.get_data(sig, "out")
            output_nonzero_counts_row_wise = np.sum(output_signal_value == 1, axis=1)

            self._plot_datastore.update_data(sig, output_nonzero_counts_row_wise, "out")
