"""
KPI HDF Parser
Reuses InteractivePlot parsing functionality but focuses on KPI-specific data requirements
"""

import h5py
import numpy as np
import pandas as pd
import logging
from typing import Dict, List, Any, Optional, Tuple
from pathlib import Path
import sys
import os

# from KPI.a_config_layer.config_manager import ConfigManager
# from KPI.b_persistence_layer.hdf_parser import HDFParser
# from KPI.a_persistence_layer.kpi_hdf_parser import KPIHDFParser
import h5py
import gc
from typing import List, Set
from KPI.b_data_storage.kpi_data_model_storage import KPI_DataModelStorage

from KPI.b_data_storage.kpi_config_storage import KPI_ALIGNMENT_CONFIG ,KPI_TRACKER_CONFIG,KPI_DETECTION_CONFIG



class KPIHDFParser:
    """
    A parser for HDF5 files that implements a depth-first traversal strategy to extract
    and organize hierarchical scientific data into a structured format.
    """

    def __init__(
        self, group: h5py.Group, storage: KPI_DataModelStorage
    ):
        """
        Initialize the HDF5 Parser.

        Args:
            group: The HDF5 group to parse
            storage: Storage instance to save parsed data

        """
        self.root_group = group
        self.storage = storage
        # Use sets instead of lists for faster lookups and less memory overhead
        self.processed_groups: Set[str] = set()  # Track already processed groups
        self.grp_list: List[h5py.Group] = []  # Stack for DFS traversal
        self.datasets: List[tuple] = []  # Store datasets found in this group

    @classmethod
    def parse(
        cls,
        group: h5py.Group,
        storage: KPI_DataModelStorage,
        header_names,
    ) -> KPI_DataModelStorage:
        """
        Parse an HDF5 file structure using depth-first traversal.

        Args:
            group: The HDF5 group to parse
            storage: Storage instance to save parsed data

            header_names: Names of header groups to exclude from processing

        Returns:
            KPI_DataModelStorage: The storage instance with parsed data
        """
        parser = cls(group, storage)
        parser.parse_grp(parser.root_group, header_names)

        # Clean up memory after parsing
        parser.cleanup()

        return storage

    def cleanup(self):
        """
        Clean up memory after parsing is complete.
        This releases references to heavy objects like HDF5 Groups and Datasets.
        """
        self.grp_list.clear()
        self.datasets.clear()
        self.processed_groups.clear()
        self.root_group = None
        # Force garbage collection to release memory
        gc.collect()

    def parse_grp(self, group: h5py.Group, header_names) -> None:
        """
        Recursively parse an HDF5 group, process its datasets, and store data.
        Memory-optimized version with batch processing and early cleanup.

        Args:
            group: The HDF5 group to parse
            header_names: Names of header groups to exclude from processing
        """
        # Skip if we've already processed this group (prevents redundant processing)
        group_id = str(group.id)
        if group_id in self.processed_groups:
            return
        self.processed_groups.add(group_id)

        current_group_name = group.name.split("/")[-1]
        current_depth = (
            len(group.name.split("/")) - 1
        )  # Root is depth 0, first level is 1, etc.
        all_keys = set(KPI_ALIGNMENT_CONFIG.keys()).union(
            KPI_DETECTION_CONFIG.keys(),
            KPI_TRACKER_CONFIG.keys()
        )

        is_second_layer = current_depth == 2
        if is_second_layer and current_group_name not in all_keys:
            return  # Skip this group and all its children

        # Process datasets immediately to avoid storing them all in memory
        # This processes items in a single pass rather than storing and then processing
        datasets_to_process = []
        child_groups = []
        current_stream = group.name.split("/")[2] if len(group.name.split("/")) > 1 else ""

        # First collect all items to process with minimal memory retention
        for item_name, item in group.items():
            if isinstance(item, h5py.Dataset) and current_stream in all_keys:
                if item_name in KPI_ALIGNMENT_CONFIG.get(current_stream, {}) or \
                item_name in KPI_DETECTION_CONFIG.get(current_stream, {}) or \
                item_name in KPI_TRACKER_CONFIG.get(current_stream, {}):
                    datasets_to_process.append((item_name, item))
            elif isinstance(item, h5py.Group) and item_name not in header_names:
                child_groups.append(item)

        # Process datasets immediately
        for dataset_name, dataset in datasets_to_process:
            # Process each row in the dataset
            self.storage.set_value(
                dataset, signal_name=dataset_name, grp_name=current_group_name
            )
            # Clear reference to help with memory management
            dataset = None

        # Clear dataset references to release memory
        datasets_to_process.clear()

        # Process child groups recursively
        for current_group in child_groups:
            self.parse_grp(current_group, header_names)
            # Clear reference after processing
            current_group = None

        # Clear references to release memory
        child_groups.clear()
