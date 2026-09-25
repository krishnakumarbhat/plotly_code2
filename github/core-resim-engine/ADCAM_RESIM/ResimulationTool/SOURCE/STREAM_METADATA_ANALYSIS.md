# Stream Metadata Tracking Analysis: Per-File vs Global Tracking

## Current Implementation Overview

### Data Flow
```
Input UDP Files (file1.bin, file2.bin, file3.bin, ...)
    ↓
adcam_latch_detection_mode() [line 3966]
    ↓
TrackStreamMetadata(streamNo, CurrScan) [line 16125]
    ↓
m_global_stream_metadata (std::map<uint8_t, StreamMetadata_T>)
    ↓
Application Cleanup [line 2145]
    ↓
ExportStreamMetadataToXML() → stream_loss_report.xml
```

### Problem Analysis

**Current Behavior** (AGGREGATED):
```
FILE1: Stream[0] Scans=[0-100], Losses=[2]   ─┐
FILE2: Stream[0] Scans=[0-99],  Losses=[1]   ├─→ m_global_stream_metadata
FILE3: Stream[0] Scans=[0-101], Losses=[3]   ─┘

m_global_stream_metadata[0]:
  occurrence_count: 300 (all files combined)
  scan_loss_count:  6   (all files combined)
  first_scan:       0   (FILE1 first)
  last_scan:        101 (FILE3 last)
```

**Expected Behavior** (PER-FILE):
```
stream_loss_report.xml
├── File: FILE1.bin
│   └── Stream[0]: scans [0-100], losses=[2]
├── File: FILE2.bin
│   └── Stream[0]: scans [0-99], losses=[1]
└── File: FILE3.bin
    └── Stream[0]: scans [0-101], losses=[3]
```

---

## Root Cause Analysis

### 1. Single Global Metadata Container
**Location**: [dph_rr_adas_audi_sil.h:259](dph_rr_adas_audi_sil.h#L259)
```cpp
StreamMetadataMap_T m_global_stream_metadata;  // Accumulates ALL streams from ALL files
```

**Issue**: No file context is maintained. When `TrackStreamMetadata()` is called, only the stream number and scan index are passed. The function has **no way to know which input file the stream came from**.

### 2. Missing File Context in TrackStreamMetadata()
**Location**: [dph_rr_adas_audi_sil.cpp:16125](dph_rr_adas_audi_sil.cpp#L16125)
```cpp
void rr_adas_audiSIL::TrackStreamMetadata(uint8_t stream_no, uint32_t scan_index)
{
    // stream_no: "Stream 0" - but which file?
    // scan_index: "Scan 50" - but from which file?
    // NO FILE IDENTIFIER PASSED
}
```

### 3. Call Site Lacks File Information
**Location**: [dph_rr_adas_audi_sil.cpp:3966](dph_rr_adas_audi_sil.cpp#L3966)
```cpp
int streamNo = ADCAMMaptoStreamNumber(...);
TrackStreamMetadata(streamNo, CurrScan);
// Called during adcam_latch_detection_mode()
// No filename or file ID is passed
```

---

## Proposed Solution: Per-File Stream Tracking

### Architecture Change 1: Extend StreamMetadata_T Structure
**Current**:
```cpp
typedef struct StreamMetadata_TAG {
    uint8_t stream_number;
    uint32_t occurrence_count;
    int32_t last_scan_index;
    uint32_t scan_loss_count;
    // ... other fields ...
} StreamMetadata_T;
```

**Enhanced (in header file)**:
```cpp
typedef struct StreamMetadata_TAG {
    // Original fields
    uint8_t stream_number;
    uint32_t occurrence_count;
    int32_t last_scan_index;
    uint32_t scan_loss_count;
    int32_t last_valid_scan_index;
    uint32_t first_occurrence_scan;
    uint32_t last_occurrence_scan;
    bool is_active;
    uint64_t total_scans_processed;
    
    // NEW: File tracking fields
    std::string source_file_path;  // Input UDP file path
    uint32_t file_entry_scan_index; // First scan in THIS file
    uint32_t file_exit_scan_index;  // Last scan in THIS file
    uint32_t file_specific_loss_count; // Losses within THIS file only
    
    // Constructor
    StreamMetadata_TAG() : 
        stream_number(0), occurrence_count(0), last_scan_index(-1), 
        scan_loss_count(0), last_valid_scan_index(-1), 
        first_occurrence_scan(0), last_occurrence_scan(0), 
        is_active(false), total_scans_processed(0),
        source_file_path(""), file_entry_scan_index(0), 
        file_exit_scan_index(0), file_specific_loss_count(0) {}
} StreamMetadata_T;
```

### Architecture Change 2: Hierarchical Metadata Storage
**Current Structure** (flat):
```cpp
// In class rr_adas_audiSIL
StreamMetadataMap_T m_global_stream_metadata;  // Global map
```

**Proposed Structure** (hierarchical):
```cpp
// In class rr_adas_audiSIL

// Per-file tracking: map<filename, map<stream_no, StreamMetadata_T>>
typedef std::map<std::string, StreamMetadataMap_T> FileStreamMetadataMap_T;
FileStreamMetadataMap_T m_per_file_stream_metadata;

// Keep reference to current file being processed
std::string m_current_input_file;
```

### Architecture Change 3: Enhanced TrackStreamMetadata() Function
**Current Signature**:
```cpp
void TrackStreamMetadata(uint8_t stream_no, uint32_t scan_index);
```

**Enhanced Signature**:
```cpp
void TrackStreamMetadata(uint8_t stream_no, uint32_t scan_index, 
                        const char* input_file_path);
```

**Implementation Pattern**:
```cpp
void rr_adas_audiSIL::TrackStreamMetadata(uint8_t stream_no, uint32_t scan_index,
                                          const char* input_file_path)
{
    // Update m_current_input_file
    m_current_input_file = (input_file_path != nullptr) ? input_file_path : "UNKNOWN";
    
    // Get or create per-file metadata map
    if (m_per_file_stream_metadata.find(m_current_input_file) == 
        m_per_file_stream_metadata.end())
    {
        m_per_file_stream_metadata[m_current_input_file] = StreamMetadataMap_T();
    }
    
    StreamMetadataMap_T &file_metadata = m_per_file_stream_metadata[m_current_input_file];
    
    // Same logic as before, but on per-file map
    if (file_metadata.find(stream_no) == file_metadata.end())
    {
        // First occurrence in THIS file
        StreamMetadata_T new_stream;
        new_stream.stream_number = stream_no;
        new_stream.source_file_path = m_current_input_file;
        new_stream.file_entry_scan_index = scan_index;
        new_stream.occurrence_count = 1;
        new_stream.last_scan_index = scan_index;
        new_stream.first_occurrence_scan = scan_index;
        new_stream.last_occurrence_scan = scan_index;
        new_stream.is_active = true;
        new_stream.total_scans_processed = 0;
        new_stream.file_specific_loss_count = 0;
        
        file_metadata[stream_no] = new_stream;
    }
    else
    {
        StreamMetadata_T &metadata = file_metadata[stream_no];
        metadata.occurrence_count++;
        metadata.last_occurrence_scan = scan_index;
        metadata.file_exit_scan_index = scan_index;
        
        // Detect loss based on scan index gap within THIS file
        DetectStreamLoss(stream_no, scan_index, m_current_input_file);
        
        // Update total span for THIS file
        if (scan_index >= metadata.first_occurrence_scan)
        {
            metadata.total_scans_processed = scan_index - metadata.first_occurrence_scan + 1;
        }
        else
        {
            // Wraparound case
            metadata.total_scans_processed = (4294967295 - metadata.first_occurrence_scan) + scan_index + 2;
        }
        
        metadata.last_scan_index = scan_index;
    }
}
```

### Architecture Change 4: Enhanced DetectStreamLoss() Function
**Current Signature**:
```cpp
void DetectStreamLoss(uint8_t stream_no, uint32_t current_scan_index);
```

**Enhanced Signature**:
```cpp
void DetectStreamLoss(uint8_t stream_no, uint32_t current_scan_index,
                     const char* input_file_path);
```

**Key Change**: When loss is detected:
```cpp
if (gap_size > 0)
{
    // Update BOTH global and per-file counters
    metadata.scan_loss_count++;           // Total across all files
    metadata.file_specific_loss_count++;  // This file only
    
    // Log with file context
    LOGMSG(eWARN, "[STREAM_LOSS]: File=[%s], Stream[%d] - Gap detected: "
                  "Last=[%d], Current=[%d], GapSize=[%u]",
           input_file_path, stream_no, last_scan, 
           (int32_t)current_scan_index, gap_size);
}
```

---

## Updated ExportStreamMetadataToXML() Implementation

### New XML Structure (Per-File):
```xml
<?xml version="1.0" encoding="UTF-8"?>
<StreamMetadataReport>
  <ReportMetadata>
    <TotalFiles>3</TotalFiles>
    <TotalUniqueStreams>5</TotalUniqueStreams>
    <GeneratedTimestamp>1743168000</GeneratedTimestamp>
  </ReportMetadata>
  
  <Files>
    <File>
      <FilePath>/path/to/file1.bin</FilePath>
      <FileMetadata>
        <Processing>
          <StartTime>1743167900</StartTime>
          <EndTime>1743167950</EndTime>
        </Processing>
      </FileMetadata>
      <Streams>
        <Stream>
          <StreamNumber>0</StreamNumber>
          <StreamName>CAMERA_FRONT</StreamName>
          <Occurrences>145</Occurrences>
          <FileSpecificLosses>2</FileSpecificLosses>
          <ScanRange>
            <FirstScan>0</FirstScan>
            <LastScan>144</LastScan>
            <TotalDuration>145</TotalDuration>
          </ScanRange>
          <Statistics>
            <LossPercentage>1.38%</LossPercentage>
          </Statistics>
        </Stream>
      </Streams>
      <FileSummary>
        <TotalStreamCount>3</TotalStreamCount>
        <TotalLosses>5</TotalLosses>
        <OverallLossRate>1.23%</OverallLossRate>
      </FileSummary>
    </File>
    
    <File>
      <FilePath>/path/to/file2.bin</FilePath>
      <!-- Similar structure for FILE2 -->
    </File>
  </Files>
  
  <GlobalSummary>
    <TotalFiles>3</TotalFiles>
    <TotalStreams>5</TotalStreams>
    <TotalOccurrences>435</TotalOccurrences>
    <TotalScanLosses>12</TotalScanLosses>
    <GlobalLossRate>1.89%</GlobalLossRate>
  </GlobalSummary>
</StreamMetadataReport>
```

---

## Integration Points

### 1. Call Site Modification
**Location**: [dph_rr_adas_audi_sil.cpp:3966](dph_rr_adas_audi_sil.cpp#L3966)

**Current**:
```cpp
TrackStreamMetadata(streamNo, CurrScan);
```

**Updated**:
```cpp
// Need to pass the input file path - find where it's available
// Likely from m_pConfig or from the UDP frame header
TrackStreamMetadata(streamNo, CurrScan, m_current_input_file.c_str());
```

### 2. File Boundary Detection
**Need to add**: Function to detect when processing switches to a new input file
```cpp
void OnInputFileChanged(const char* new_file_path)
{
    m_current_input_file = new_file_path;
    // Could also reset per-file statistics here if desired
}
```

### 3. Export Trigger
**Location**: [dph_rr_adas_audi_sil.cpp:2145](dph_rr_adas_audi_sil.cpp#L2145)

**Current**:
```cpp
ExportStreamMetadataToXML("./stream_loss_report.xml", 14);
```

**Updated** (iterate through all files):
```cpp
// Generate per-file reports
for (auto &file_entry : m_per_file_stream_metadata)
{
    std::string report_path = GeneratePerFileReportPath(file_entry.first);
    ExportStreamMetadataToXMLPerFile(report_path.c_str(), 
                                    file_entry.first.c_str(),
                                    file_entry.second);
}

// Generate global report
ExportStreamMetadataToXMLGlobal("./stream_loss_report_global.xml");
```

---

## Summary of Changes Required

| Component | Change | Scope |
|-----------|--------|-------|
| `StreamMetadata_T` struct | Add file tracking fields | Header + Implementation |
| `m_global_stream_metadata` | Add hierarchical map | Header + Implementation |
| `TrackStreamMetadata()` | Add file parameter | Implementation |
| `DetectStreamLoss()` | Add file parameter | Implementation |
| `ExportStreamMetadataToXML()` | Iterate per-file maps | Implementation |
| Call sites (line 3966) | Pass file path | Implementation |
| File processing loop | Track current file | Implementation |

---

## Benefits of Per-File Tracking

✅ **Granular Analysis**: Identify which input files have stream losses
✅ **Quality Metrics**: Per-file stream quality assessment
✅ **Debugging**: Pinpoint problematic input files
✅ **Reporting**: Generate separate reports for each input file
✅ **Trend Analysis**: Track loss improvements across file versions
✅ **Root Cause**: Correlate file characteristics with loss patterns

---

## Backward Compatibility

- Keep `m_global_stream_metadata` for global statistics
- Extend functions with optional parameters (file_path defaults to NULL for backward compat)
- Existing export functions can call new per-file implementations internally

---

**Analysis Date**: 2026-03-26
**Status**: Architecture Definition Complete
