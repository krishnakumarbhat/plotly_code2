import os
import h5py
import tempfile
import pytest
from KPI.a_persistence_layer.hdf_wrapper import KPIHDFWrapper, KPIProcessingConfig
from KPI.b_data_storage.kpi_data_model_storage import KPI_DataModelStorage

@pytest.fixture
def sample_hdf5_file():
    """Create a temporary HDF5 file with test data"""
    fd, path = tempfile.mkstemp(suffix='.h5')
    try:
        with h5py.File(path, 'w') as f:
            # Create test stream data
            for stream in ['stream1', 'stream2']:
                grp = f.create_group(f'sensor1/{stream}')
                hdr = grp.create_group('Stream_Hdr')
                hdr.create_dataset('scan_index', data=[1,2,3])
                grp.create_dataset('signal1', data=[10,20,30])
                grp.create_dataset('signal2', data=[40,50,60])
        yield path
    finally:
        os.remove(path)

def test_model_persistence_across_streams(sample_hdf5_file):
    """Test that model retains data across multiple stream parses"""
    config = KPIProcessingConfig(
        sensor_id='sensor1',
        input_file_path=sample_hdf5_file,
        output_file_path=sample_hdf5_file,  # Using same file for simplicity
        output_dir=tempfile.mkdtemp(),
        base_name='test'
    )
    
    wrapper = KPIHDFWrapper(config)
    
    # Verify models are empty initially
    assert len(wrapper.stream_input_model.get_data()) == 0
    assert len(wrapper.stream_output_model.get_data()) == 0
    
    # Parse the file
    results = wrapper.parse()
    
    # Check that both streams were processed
    assert results['streams_processed']['stream1']['input_available']
    assert results['streams_processed']['stream2']['input_available']
    
    # Verify models contain data from both streams
    input_data = wrapper.stream_input_model.get_data()
    assert 'stream1' in input_data
    assert 'stream2' in input_data
    assert 'signal1' in input_data['stream1']
    assert 'signal2' in input_data['stream2']
