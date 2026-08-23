from InteractivePlot.c_data_storage.data_model_storage import DataModelStorage
# If RegexStorage is used and needs testing, import it here
# from InteractivePlot.c_data_storage.config_strage import RegexStorage

# Tests for DataModelStorage
def test_data_model_storage_initialization():
    storage = DataModelStorage()
    assert storage is not None
    # Add more assertions based on how DataModelStorage is initialized
    # For example, if it initializes with empty structures:
    assert storage.data == {}
    assert storage.metadata == {}

def test_data_model_storage_add_data():
    storage = DataModelStorage()
    sample_data = {"key1": "value1"}
    sample_metadata = {"source": "test"}
    storage.add_data("test_id", sample_data, sample_metadata)
    assert "test_id" in storage.data
    assert storage.data["test_id"] == sample_data
    assert "test_id" in storage.metadata
    assert storage.metadata["test_id"] == sample_metadata

def test_data_model_storage_get_data():
    storage = DataModelStorage()
    sample_data = {"key2": "value2"}
    storage.add_data("test_id_2", sample_data, {})
    retrieved_data = storage.get_data("test_id_2")
    assert retrieved_data == sample_data

def test_data_model_storage_get_metadata():
    storage = DataModelStorage()
    sample_metadata = {"source": "test_meta"}
    storage.add_data("test_id_3", {}, sample_metadata)
    retrieved_metadata = storage.get_metadata("test_id_3")
    assert retrieved_metadata == sample_metadata

def test_data_model_storage_get_nonexistent_data():
    storage = DataModelStorage()
    assert storage.get_data("nonexistent_id") is None
    assert storage.get_metadata("nonexistent_id") is None

# Add tests for RegexStorage if it's implemented and used
# Example (if RegexStorage has a method to add and match regex):
# def test_config_storage_add_and_match():
#     config_storage = RegexStorage()
#     config_storage.add_regex("test_pattern", "^test$")
#     assert config_storage.match_string("test_pattern", "test") is True
#     assert config_storage.match_string("test_pattern", "testing") is False