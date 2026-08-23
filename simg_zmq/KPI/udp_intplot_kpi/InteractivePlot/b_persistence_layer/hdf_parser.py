import h5py
import gc
from typing import List, Set
from InteractivePlot.c_data_storage.data_model_storage import DataModelStorage
from InteractivePlot.c_data_storage.config_loader import get_plot_config


class HDF5Parser:
    """
    A parser for HDF5 files that implements a depth-first traversal strategy to extract
    and organize hierarchical scientific data into a structured format.
    """

    def __init__(
        self, group: h5py.Group, storage: DataModelStorage, scan_index: List[int]
    ):
        """
        Initialize the HDF5 Parser.

        Args:
            group: The HDF5 group to parse
            storage: Storage instance to save parsed data
            scan_index: List of scan indices for data organization
        """
        self.root_group = group
        self.storage = storage
        self.scan_index = scan_index
        # Use sets instead of lists for faster lookups and less memory overhead
        self.processed_groups: Set[str] = set()  # Track already processed groups
        self.grp_list: List[h5py.Group] = []  # Stack for DFS traversal
        self.datasets: List[tuple] = []  # Store datasets found in this group

    @classmethod
    def parse(
        cls,
        group: h5py.Group,
        storage: DataModelStorage,
        scan_index: List[int],
        header_names,
    ) -> DataModelStorage:
        """
        Parse an HDF5 file structure using depth-first traversal.

        Args:
            group: The HDF5 group to parse
            storage: Storage instance to save parsed data
            scan_index: List of scan indices for data organization
            header_names: Names of header groups to exclude from processing

        Returns:
            DataModelStorage: The storage instance with parsed data
        """
        parser = cls(group, storage, scan_index)
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
        plot_config = get_plot_config()

        is_second_layer = current_depth == 2
        if is_second_layer and current_group_name not in plot_config:
            return  # Skip this group and all its children

        # Process datasets immediately to avoid storing them all in memory
        # This processes items in a single pass rather than storing and then processing
        datasets_to_process = []
        child_groups = []
        current_stream = group.name.split("/")[2] if len(group.name.split("/")) > 1 else ""

        # First collect all items to process with minimal memory retention.
        # Resolve dataset names via aliases so we can parse HDFs whose datasets use different naming
        # (e.g., RDD_IDX / rdd_index) but still map them to canonical signal keys (e.g., rdd_idx).
        stream_config = plot_config.get(current_stream, {}) if current_stream in plot_config else {}

        for item_name, item in group.items():
            if isinstance(item, h5py.Dataset) and stream_config:
                canonical_signal_name = None

                # Direct hit (preferred)
                if item_name in stream_config:
                    canonical_signal_name = item_name
                else:
                    # Alias hit (fallback)
                    for signal_key, signal_cfg in stream_config.items():
                        if not isinstance(signal_cfg, dict):
                            continue
                        aliases = signal_cfg.get("aliases") or []
                        if item_name in aliases:
                            canonical_signal_name = signal_key
                            break

                if canonical_signal_name is not None:
                    datasets_to_process.append((canonical_signal_name, item))
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
