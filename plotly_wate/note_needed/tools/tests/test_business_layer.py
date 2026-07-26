import pytest
import pandas as pd # Assuming pandas is used for data manipulation
import numpy as np
from unittest.mock import patch, MagicMock, ANY
import logging
import plotly.graph_objects as go
from InteractivePlot.d_business_layer.data_prep import DataPrep
from InteractivePlot.d_business_layer.data_cal import DataCalculations as DataCal
# Import other necessary modules or mock classes

# Fixtures for DataPrep and DataCal if needed
@pytest.fixture
def sample_raw_data():
    # Example: raw data that DataPrep might process
    return pd.DataFrame({
        'time': pd.to_datetime(['2023-01-01 00:00:00', '2023-01-01 00:00:01']),
        'value': [10, 20],
        'status': ['raw', 'raw']
    })

@pytest.fixture
def sample_prepared_data():
    # Example: data after processing by DataPrep, ready for DataCal
    return pd.DataFrame({
        'time_processed': pd.to_datetime(['2023-01-01 00:00:00', '2023-01-01 00:00:01']),
        'value_cleaned': [10.0, 20.0],
        'value_normalized': [0.5, 1.0] # Assuming normalization to [0,1] for simplicity
    })

# Tests for DataPrep

@pytest.fixture
def mock_data_storage():
    storage = MagicMock()
    # Example: storage.get_signal_data.return_value = (np.array([...]), np.array([...]))
    # Example: storage.get_signal_metadata.return_value = {'unit': 'm/s'}
    return storage

@pytest.fixture
def mock_data_calculator():
    calculator = MagicMock(spec=DataCal)
    # Example: calculator.scatter_plot.return_value = ('fig_id_1', MagicMock(spec=go.Figure))
    return calculator

@pytest.fixture
def data_preparer(mock_data_storage, mock_data_calculator):
    config = {
        'config_file': 'dummy_config.xml',
        'json_parser': MagicMock(), # or a more specific mock if needed
        'output_folder': 'test_output',
        'plot_config': {
            'kpi_config': {},
            'signal_aliases': {},
            'unit_conversion': {}
        }
    }
    logger = logging.getLogger(__name__)
    # Ensure DataPrep is initialized with mocks where its actual dependencies would be
    # This might require adjusting DataPrep's __init__ or providing a way to inject mocks
    # For this example, let's assume DataPrep can take these as arguments or they are set as attributes
    dp = DataPrep(config, logger)
    dp.data_storage = mock_data_storage # Assuming direct attribute assignment or a setter
    dp.data_calculator = mock_data_calculator
    dp.html_generator = MagicMock() # Mock HtmlGenerator as well
    return dp


@patch('InteractivePlot.d_business_layer.data_prep.DataModelStorage') # If DataPrep instantiates it
@patch('InteractivePlot.d_business_layer.data_prep.DataCal')        # If DataPrep instantiates it
@patch('InteractivePlot.d_business_layer.data_prep.HtmlGenerator')  # If DataPrep instantiates it
def test_data_prep_initialization(MockHtmlGenerator, MockDataCal, MockDataModelStorage):
    config = {
        'config_file': 'dummy_config.xml',
        'json_parser': MagicMock(),
        'output_folder': 'test_output',
        'plot_config': {
            'kpi_config': {},
            'signal_aliases': {},
            'unit_conversion': {}
        }
    }
    logger = logging.getLogger(__name__)
    
    dp = DataPrep(config, logger)
    
    assert dp.config == config
    assert dp.logger == logger
    assert isinstance(dp.data_storage, MockDataModelStorage)
    assert isinstance(dp.data_calculator, MockDataCal)
    assert isinstance(dp.html_generator, MockHtmlGenerator)
    MockDataModelStorage.assert_called_once_with(config, logger)
    MockDataCal.assert_called_once_with()
    MockHtmlGenerator.assert_called_once_with(config['output_folder'], logger)


def test_data_prep_process_stream_data(data_preparer, mock_data_storage, mock_data_calculator):
    stream_name = "TestStream"
    signals_to_process = ["SignalA", "SignalB"]
    
    # Mock return values for data_storage
    mock_data_storage.get_signal_data.side_effect = [
        (np.array([1,2]), np.array([10,20]), np.array([11,21])), # Data for SignalA
        (np.array([3,4]), np.array([30,40]), np.array([31,41]))  # Data for SignalB
    ]
    mock_data_storage.get_signal_metadata.return_value = {'unit': 'm'}
    
    # Mock return values for data_calculator plot methods
    mock_fig_a = MagicMock(spec=go.Figure)
    mock_fig_b = MagicMock(spec=go.Figure)
    mock_data_calculator.scatter_plot.side_effect = [
        ('fig_A', mock_fig_a),
        ('fig_B', mock_fig_b)
    ]
    
    processed_data = data_preparer.process_stream_data(stream_name, signals_to_process)
    
    assert stream_name in processed_data
    assert "SignalA" in processed_data[stream_name]
    assert "SignalB" in processed_data[stream_name]
    assert processed_data[stream_name]["SignalA"]['fig_id'] == 'fig_A'
    assert processed_data[stream_name]["SignalA"]['fig'] == mock_fig_a
    assert processed_data[stream_name]["SignalB"]['fig_id'] == 'fig_B'
    assert processed_data[stream_name]["SignalB"]['fig'] == mock_fig_b
    
    mock_data_storage.get_signal_data.assert_any_call(stream_name, "SignalA")
    mock_data_storage.get_signal_data.assert_any_call(stream_name, "SignalB")
    mock_data_calculator.scatter_plot.assert_any_call("SignalA", ANY, ANY, ANY) # ANY from unittest.mock
    mock_data_calculator.scatter_plot.assert_any_call("SignalB", ANY, ANY, ANY)
    data_preparer.html_generator.add_plot_to_report.assert_any_call(stream_name, "SignalA", 'fig_A', mock_fig_a, ANY)
    data_preparer.html_generator.add_plot_to_report.assert_any_call(stream_name, "SignalB", 'fig_B', mock_fig_b, ANY)


@patch('multiprocessing.Pool')
def test_data_prep_run_data_processing_multiprocess(MockPool, data_preparer):
    # This test focuses on the multiprocessing aspect
    mock_pool_instance = MockPool.return_value
    mock_pool_instance.starmap.return_value = [{'stream1_data': '...'}, {'stream2_data': '...'}]
    
    data_preparer.config['json_parser'].get_streams_and_signals.return_value = {
        'stream1': ['sigA'], 'stream2': ['sigB']
    }
    data_preparer.config['multiprocessing'] = True # Ensure multiprocessing is enabled
    
    final_report_data = data_preparer.run_data_processing()
    
    MockPool.assert_called_once()
    mock_pool_instance.starmap.assert_called_once()
    # Check that results from starmap are merged correctly
    assert 'stream1_data' in final_report_data
    assert 'stream2_data' in final_report_data
    data_preparer.html_generator.generate_html_report.assert_called_once()


def test_data_prep_run_data_processing_singleprocess(data_preparer):
    # This test focuses on the single processing aspect
    data_preparer.config['json_parser'].get_streams_and_signals.return_value = {
        'stream1': ['sigA']
    }
    data_preparer.config['multiprocessing'] = False # Ensure single processing
    
    # Mock process_stream_data to avoid re-testing its logic here
    data_preparer.process_stream_data = MagicMock(return_value={'stream1': {'sigA': 'plot_data_A'}})
    
    final_report_data = data_preparer.run_data_processing()
    
    data_preparer.process_stream_data.assert_called_once_with('stream1', ['sigA'])
    assert final_report_data == {'stream1': {'sigA': 'plot_data_A'}}
    data_preparer.html_generator.generate_html_report.assert_called_once()

# Add more tests for edge cases, error handling, KPI generation, unit conversion, signal aliasing etc.
# For example, test what happens if a signal is not found in data_storage.
# Test how unit conversions are applied if that logic is in DataPrep.



# Tests for DataCalculations

@pytest.fixture
def data_calculator():
    return DataCal()

@pytest.fixture
def sample_data_dict_for_plot():
    return {
        'SI': np.array([1, 2, 3]),
        'I': np.array([10, 12, 15]),
        'O': np.array([10, 11, 16]),
        'MI': ([], []), # Mismatch Input (indices, values)
        'MO': ([], [])  # Mismatch Output (indices, values) - Note: Original code uses MO[1] for values
    }

@patch('InteractivePlot.d_business_layer.data_cal.PlotlyCharts.scatter_plot')
def test_data_cal_scatter_plot(mock_scatter_plot, data_calculator, sample_data_dict_for_plot):
    mock_fig = MagicMock()
    mock_scatter_plot.return_value = mock_fig
    signal_name = "TestSignal"
    
    fig_id, fig = data_calculator.scatter_plot(signal_name, sample_data_dict_for_plot, None, None)
    
    assert fig_id == f"scatter_fig_{signal_name}"
    assert fig == mock_fig
    mock_scatter_plot.assert_called_once_with(
        sample_data_dict_for_plot['SI'],
        sample_data_dict_for_plot['I'],
        sample_data_dict_for_plot['O'],
        signal_name,
        "INPUT",
        "OUTPUT",
        "red",
        "blue",
        "IN/OUT",
    )

@patch('InteractivePlot.d_business_layer.data_cal.PlotlyCharts.scatter_plot')
def test_data_cal_scatter_plot_mstokmh(mock_scatter_plot, data_calculator, sample_data_dict_for_plot):
    mock_fig = MagicMock()
    mock_scatter_plot.return_value = mock_fig
    signal_name = "VelocitySignal"
    
    fig_id, fig = data_calculator.scatter_plot_mstokmh(signal_name, sample_data_dict_for_plot, None, None)
    
    expected_i_kmh = sample_data_dict_for_plot['I'] * 3.6
    expected_o_kmh = sample_data_dict_for_plot['O'] * 3.6
    
    assert fig_id == f"scatter_fig_{signal_name}"
    assert fig == mock_fig
    mock_scatter_plot.assert_called_once()
    call_args = mock_scatter_plot.call_args[0]
    np.testing.assert_array_almost_equal(call_args[1], expected_i_kmh)
    np.testing.assert_array_almost_equal(call_args[2], expected_o_kmh)
    assert call_args[0] is sample_data_dict_for_plot['SI'] # Direct pass through
    assert call_args[3] == signal_name

@patch('InteractivePlot.d_business_layer.data_cal.PlotlyCharts.scatter_plot')
def test_data_cal_scatter_with_mismatch_has_mismatch(mock_scatter_plot, data_calculator, sample_data_dict_for_plot):
    mock_fig = MagicMock()
    mock_scatter_plot.return_value = mock_fig
    signal_name = "MismatchSignal"
    
    # Modify data to have a mismatch
    data_with_mismatch = sample_data_dict_for_plot.copy()
    data_with_mismatch['O'] = np.array([10, 99, 16]) # 99 is a mismatch with 12
    # Correcting MI and MO structure based on usage in scatter_with_mismatch
    data_with_mismatch['MI'] = [[], []] 
    data_with_mismatch['MO'] = [[], []] # MO[0] is not used, MO[1] for values

    fig_id, fig = data_calculator.scatter_with_mismatch(signal_name, data_with_mismatch, None, None)
    
    assert fig_id == f"scatter_fig_{signal_name}" # No " (no mismatch)" suffix
    assert fig == mock_fig
    
    # Check that MI and MO were populated
    assert len(data_with_mismatch['MI'][0]) == 1 # One mismatch index (scan_idx = 2)
    assert data_with_mismatch['MI'][0][0] == 2 # Scan index of mismatch
    assert data_with_mismatch['MI'][1][0] == 12 # Input value at mismatch
    assert len(data_with_mismatch['MO'][1]) == 1 # One mismatch output value
    assert data_with_mismatch['MO'][1][0] == 99 # Output value at mismatch

    mock_scatter_plot.assert_called_once_with(
        data_with_mismatch['MI'][0], # x_vals = mismatch scan indices
        data_with_mismatch['MI'][1], # y_vals = mismatch input values
        data_with_mismatch['MO'][1], # y2_vals = mismatch output values
        signal_name,
        "MISMATCH",
        "output mismatch",
        "red",
        "blue",
        "MISMATCH",
    )

@patch('InteractivePlot.d_business_layer.data_cal.PlotlyCharts.scatter_plot')
def test_data_cal_scatter_with_mismatch_no_mismatch(mock_scatter_plot, data_calculator, sample_data_dict_for_plot):
    mock_fig = MagicMock()
    mock_scatter_plot.return_value = mock_fig
    signal_name = "NoMismatchSignal"
    
    # Data already has no mismatch for I vs O
    data_no_mismatch = sample_data_dict_for_plot.copy()
    data_no_mismatch['I'] = np.array([10,11,16]) # Ensure I and O are identical
    data_no_mismatch['O'] = np.array([10,11,16])
    data_no_mismatch['MI'] = [[], []]
    data_no_mismatch['MO'] = [[], []]

    fig_id, fig = data_calculator.scatter_with_mismatch(signal_name, data_no_mismatch, None, None)
    
    expected_plot_signal_name = f"{signal_name} (no mismatch)"
    assert fig_id == f"scatter_fig_{expected_plot_signal_name.replace(' ', '_')}"
    assert fig == mock_fig
    
    assert len(data_no_mismatch['MI'][0]) == 0
    assert len(data_no_mismatch['MO'][1]) == 0

    mock_scatter_plot.assert_called_once_with(
        [], # x_vals = empty
        [], # y_vals = empty
        [], # y2_vals = empty
        expected_plot_signal_name,
        "NO MISMATCH",
        "output mismatch",
        "green",
        "blue",
        "NO MISMATCH",
    )

def test_data_cal_set_stream_name(data_calculator):
    stream_name = "TestStream"
    data_calculator.set_stream_name(stream_name)
    assert data_calculator.stream_name == stream_name

# Add more tests for other DataCal methods like scatter_plot_bs_si_sr, etc.
# For methods involving shared_data and lock, you might need to mock those as well if they are complex.
# Example for a method that uses shared_data (if it were simple):
# @patch('InteractivePlot.d_business_layer.data_cal.PlotlyCharts.some_other_plot')
# def test_data_cal_method_with_shared_data(mock_plot, data_calculator, sample_data_dict_for_plot):
#     mock_shared_data = MagicMock() # or a simple dict if that's what's expected
#     mock_lock = MagicMock()
#     # ... setup and call ...
#     # data_calculator.some_method_using_shared(..., mock_shared_data, mock_lock)
#     # ... assertions ...


# Placeholder for more specific tests once DataPrep and DataCal structures are known
# For example, if DataPrep involves filtering:
# def test_data_prep_filtering(sample_raw_data_with_outliers):
#     data_preparer = DataPrep(config={'filter_threshold': 100})
#     filtered_df = data_preparer.filter_data(sample_raw_data_with_outliers)
#     assert filtered_df['value'].max() <= 100

# If DataCal involves complex calculations:
# def test_data_cal_custom_metric(sample_prepared_data_for_metric):
#     data_calculator = DataCal(config={'metric_params': {...}})
#     metric_result = data_calculator.compute_custom_metric(sample_prepared_data_for_metric)
#     assert metric_result == expected_metric_value

# Note: The above tests are very generic. 
# Actual tests will need to be tailored to the specific methods and logic 
# within DataPrep.py and DataCal.py.
# You'll need to inspect those files to write meaningful tests.

# Example of a simple test if DataPrep has a simple transformation method
# class MockDataPrep(DataPrep):
#     def transform(self, data_frame, column_name):
#         # Simplified mock transformation
#         if column_name in data_frame:
#             data_frame[column_name + '_transformed'] = data_frame[column_name] * 2
#         return data_frame

# def test_mock_data_prep_transform():
#     preparer = MockDataPrep()
#     df = pd.DataFrame({'A': [1, 2, 3]})
#     transformed_df = preparer.transform(df.copy(), 'A')
#     assert 'A_transformed' in transformed_df
#     pd.testing.assert_series_equal(transformed_df['A_transformed'], pd.Series([2, 4, 6], name='A_transformed'))

# Example of a simple test if DataCal has a simple calculation method
# class MockDataCal(DataCal):
#     def sum_column(self, data_frame, column_name):
#         if column_name in data_frame:
#             return data_frame[column_name].sum()
#         return 0

# def test_mock_data_cal_sum():
#     calculator = MockDataCal()
#     df = pd.DataFrame({'B': [10, 20, 30]})
#     total_sum = calculator.sum_column(df, 'B')
#     assert total_sum == 60
#     assert calculator.sum_column(df, 'C') == 0 # Test non-existent column