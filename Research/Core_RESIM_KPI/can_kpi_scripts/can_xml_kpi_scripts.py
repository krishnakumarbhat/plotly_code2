#!/usr/bin/env python3
"""Usage:
    python CAN_xml_kpi_scripts.py <input_xml_folder> <output_xml_folder> <csv_output_folder> --delete=<comma_separated_substrings_to_delete>
"""

from pathlib import Path
from statistics import mean
from typing import Dict, List, Any, Tuple
import plotly.graph_objects as go
from plotly.subplots import make_subplots
import sys
import os
import csv
import re
import argparse
import xml.etree.ElementTree as ET
from datetime import datetime
from typing import List, Dict, Optional, Tuple
from pathlib import Path
from collections import defaultdict
from pathlib import Path
import sys
import tempfile
import shutil

#customer specific strings, this needs to be taken from xml file name 
CUSTOMER_STR = ["MCIP","CEER"]
RADAR_POS_LIST = ["FLR", "SRR_FL", "SRR_FR", "SRR_RL", "SRR_RR"]
TARGET_LINE = '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>'
DEBUG_MODE = False
RADAR_POS = "FLR"
DETECTION_MARKER = RADAR_POS + "_DETECTION_001_004"
CANID_KEYS = ("canid", "canID", "CANID")
TS_KEYS = ("timestamp", "time", "ts", "timeStamp", "Timestamp")
# The ANSI escape code for green text is \033[32m
GREEN = "\033[32m"
BLUE = "\033[34m"
# The ANSI escape code to reset formatting is \033[0m
RESET = "\033[0m"

# Global maps to hold detection data per scanindex
fileA_scanindex_det_map = {}
fileB_scanindex_det_map = {}
fileA_scanindex_num_of_det_map = {}
fileB_scanindex_num_of_det_map = {}
terminal_print_file_handler = None

# [START]================= Helper functions for removing redundant XML header lines ================

def normalize(line: str) -> str:
    if line.startswith("\ufeff"):
        line = line.lstrip("\ufeff")
    return line.strip()


def process_file(p: Path, terminal_file) -> bool:
    try:
        text = p.read_text(encoding="utf-8")
    except Exception:
        try:
            text = p.read_text(encoding="utf-8-sig")
        except Exception as e:
            print(f"Failed to read {p}: {e}", file=sys.stderr)
            print(f"Failed to read {p}: {e}", file=terminal_file)
            return False

    lines = text.splitlines(keepends=True)
    seen = False
    out_lines = []
    changed = False

    for ln in lines:
        if normalize(ln) == TARGET_LINE:
            if not seen:
                out_lines.append(ln)
                seen = True
            else:
                changed = True
                continue
        else:
            out_lines.append(ln)

    if changed:
        try:
            with tempfile.NamedTemporaryFile("w", delete=False, encoding="utf-8", newline="") as tf:
                tf.writelines(out_lines)
                tmp = tf.name
            shutil.move(tmp, str(p))
            print(f"Cleaned: {p}", file=terminal_file)
            return True
        except Exception as e:
            print(f"Failed to write {p}: {e}", file=sys.stderr)
            print(f"Failed to write {p}: {e}", file=terminal_file)
            return False
    else:
        print(f"No change: {p}",file=terminal_file)
        return True


def remove_redundant_header(input_path:str, terminal_file):
    root = Path(input_path)
    if not root.exists():
        print(f"Path not found: {root}", file=sys.stderr)
        print(f"Path not found: {root}", file=terminal_file)
        sys.exit(2)

    files = []
    if root.is_file() and root.suffix.lower() == ".xml":
        files = [root]
    elif root.is_dir():
        files = sorted([p for p in root.glob("*.xml") if p.is_file()])
    else:
        print("No .xml files to process.", file=sys.stderr)
        print("No .xml files to process.", file=terminal_file)
        sys.exit(2)

    for f in files:
        process_file(f, terminal_file)

# [END]================= Helper functions for removing redundant XML header lines ==================


# [START]================= Helper functions for HTML Plots ================

def create_table_html(datasets: Dict[str,List[Dict[str, list[float]]]], set_names: List[str]) -> str:
    """
    Create HTML for scrollable tables showing statistics for each set.
    """

    
    tables_html = '<div style="margin-top: 30px;">'
    
    #for set_idx, (data, set_name) in enumerate(zip(datasets, set_names)):
    for set_idx, (radpos, data) in enumerate(datasets.items()):

        tables_html += f'''
        <div style="margin-bottom: 30px; border: 2px solid #ddd; border-radius: 8px; padding: 15px;">
            <h3 style="font-family: Arial, sans-serif; color: #333; margin-bottom: 15px;">
                {radpos} - Statistics Summary
            </h3>
            <div style="overflow-x: auto; max-height: 300px; overflow-y: auto;">
                <table style="width: 100%; border-collapse: collapse; font-family: Arial, sans-serif;">
                    <thead style="position: sticky; top: 0; background-color: #4CAF50; color: white;">
                        <tr>
                            <th style="padding: 12px; text-align: left; border: 1px solid #ddd;">Filename</th>
                            <th style="padding: 12px; text-align: right; border: 1px solid #ddd;">decoded_scanindex_input </th>
                            <th style="padding: 12px; text-align: right; border: 1px solid #ddd;">decoded_scanindex_output </th>
                            <th style="padding: 12px; text-align: right; border: 1px solid #ddd;">scanindex_used_for_comparison </th>
                            <th style="padding: 12px; text-align: right; border: 1px solid #ddd;">timestamp-scanindex match (%)</th>
                            <th style="padding: 12px; text-align: right; border: 1px solid #ddd;">dets parameter match (%)</th>
                        </tr>
                    </thead>
                    <tbody>
        '''
        for dict_value in data:
            for key, value in dict_value.items():
        
                # Alternate row colors
                row_color = "#f9f9f9" 
                
                tables_html += f'''
                            <tr style="background-color: {row_color};">
                                <td style="padding: 10px; border: 1px solid #ddd; font-weight: bold; text-align: left;">{key}</td>
                                <td style="padding: 10px; border: 1px solid #ddd; text-align: right;">{value[0]:.0f}</td>
                                <td style="padding: 10px; border: 1px solid #ddd; text-align: right;">{value[1]:.0f}</td>
                                <td style="padding: 10px; border: 1px solid #ddd; text-align: right;">{value[2]:.0f}</td>
                                <td style="padding: 10px; border: 1px solid #ddd; text-align: right;">{value[3]:.2f}</td>
                                <td style="padding: 10px; border: 1px solid #ddd; text-align: right;">{value[4]:.2f}</td>
                            </tr>
                '''
        
        tables_html += '''
                    </tbody>
                </table>
            </div>
        </div>
        '''
    
    tables_html += '</div>'
    return tables_html


def create_multi_set_plot(datasets: Dict[str, List[Dict[str, list[float]]]], 
                          set_names: List[str],
                          output_file: Path,
                          title: str = ""):
    n_sets = len(set_names)
    
    # Create subplots (5 rows, 1 column)
    fig = make_subplots(
        rows=n_sets, 
        cols=1,
        subplot_titles=set_names,
        vertical_spacing=0.08,
        specs=[[{"secondary_y": False}] for _ in range(n_sets)]
    )
    
    colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd']
    

    for set_idx, (radpos, data) in enumerate(datasets.items()):
        y1_values = []
        y2_values = []
        for dict_value in data:
            for key, value in dict_value.items():
                y1_values.append(value[4])
                y2_values.append(value[3])

        y_values_list = [y1_values, y2_values]
        x_values = list(range(len(y1_values)))

        params = ["Detections matched (%)", " Timestamp-Scanindex match (%)"]
        
        for param_idx, (y_values, param) in enumerate(zip(y_values_list, params)):   
            fig.add_trace(
                go.Scatter(
                    x=x_values,
                    y=y_values,
                    mode='lines+markers',
                    name=param,
                    line=dict(color=colors[param_idx], width=2),
                    marker=dict(size=4),
                    legendgroup=param,
                    showlegend=(set_idx == 0),  # Only show legend for first set
                    hovertemplate=f'<b>{"Percentage of detections matched"}</b><br>File: %{{x}}<br>Match: %{{y:.2f}}%<extra></extra>'
                ),
                row=set_idx + 1,
                col=1
            )
        
        # Update y-axis for each subplot
        fig.update_yaxes(
            title_text="Match %",
            range=[0, 105],
            row=set_idx + 1,
            col=1
        )
        
        # Update x-axis
        fig.update_xaxes(
            title_text="number of files" if set_idx == n_sets - 1 else "",
            row=set_idx + 1,
            col=1
        )
    
    # Update overall layout
    fig.update_layout(
        title={
            'text': title,
            'x': 0.5,
            'xanchor': 'center',
            'font': {'size': 20}
        },
        height=400 * n_sets,
        showlegend=True,
        legend=dict(
            orientation="h",
            yanchor="bottom",
            y=1.02,
            xanchor="center",
            x=0.5
        ),
        hovermode='closest'
    )
    
    # Generate HTML with tables

    fig_html = fig.to_html(full_html=True, include_plotlyjs='cdn')
    
    # Create tables HTML
    tables_html = create_table_html(datasets, set_names)
    
    # Insert tables before closing body tag
    #lower_html = html_str.lower()
    #idx = lower_html.rfind("</body>")
    #if idx != -1:
    #    html_str = html_str[:idx] + tables_html + html_str[idx:]
    #else:
    #    html_str = html_str + tables_html
    
    html_str=f"""<!DOCTYPE html>
    <html>
    <head>
    <meta charset="utf-8"/>
    <title>PCAN Detection KPI</title>
    <style>
    body{{font-family:Arial, sans-serif; margin:16px;}}
    h1{{text-align:left;}}
    .tables-container{{display:flex;flex-wrap:wrap;gap:12px;}}
    .table-wrapper{{flex:1 1 300px; max-width:32%; border:1px solid #ccc; padding:6px; box-shadow:2px 2px 4px rgba(0,0,0,0.1);}}
    .scroll{{max-height:220px; overflow-y:auto;}}
    table{{border-collapse:collapse; width:100%; font-size:12px;}}
    th,td{{border:1px solid #999; padding:4px; text-align:right;}}
    th{{background:#5b9460; position:sticky; top:0;}}
    h3{{margin:4px 0 8px; font-size:14px; text-align:center;}}
    </style>
    </head>
    <body>
    <h1>PCAN Detection KPI</h1>
    {fig_html}
    <h2>Data Tables</h2>
    {tables_html}
    </body>
    </html>
    """
    # Write to file
    with open(str(output_file), "w", encoding="utf-8") as fh:
        fh.write(html_str)
    
    print(f"✓ Plot saved to: {output_file}")

# [END]================= Helper functions for HTML Plots= ==================


# [START]================= Helper functions for XML Files Comparison =====================

def compute_match_stats(map1: Dict[str, List[List[float]]],
                        map2: Dict[str, List[List[float]]],
                        map3: Dict[str, List[List[float]]],
                        map4: Dict[str, List[List[float]]],
                        tol: float) -> Dict[str, Tuple[int, int, float]]:
    """
    Returns dict param -> (matched_pairs, total_pairs, percent)
    """
    det_match_across_scanindex = {}
    range_tolerance = 0.016
    range_rate_tolerance = 0.016
    azimuth_tolerance = 0.00873
    elevation_tolerance = 0.00873
    rcs_tolerance = 1

    for key in map1.keys():
        r1 = map1.get(key, [])
        r2 = map2.get(key, [])
        number_of_detections1 = map3.get(key, [])
        number_of_detections2 = map4.get(key, [])
        selected_flag = [False for _ in range(0, len(r2))]
        selected_flag_idexA = [-1 for _ in range(0, len(r1))]
        dets_matched = 0
        
        for idx_A, det_A in enumerate(r1):
            for idx_B, det_B in enumerate(r2):
                if (abs(float(det_A[0]) - float(det_B[0])) <= range_tolerance and abs(float(det_A[1]) - float(det_B[1])) <= range_rate_tolerance and abs(float(det_A[2]) - float(det_B[2])) <= elevation_tolerance and abs(float(det_A[3]) - float(det_B[3])) <= azimuth_tolerance) and not selected_flag[idx_B]:
                    selected_flag[idx_B] = True
                    selected_flag_idexA[idx_A] = idx_B
                    dets_matched += 1
                    break
        
        det_match_across_scanindex[key] = ((dets_matched / number_of_detections1) * 100)

    average_matches = list(det_match_across_scanindex.values())
    average_matches = sum(average_matches) / len(det_match_across_scanindex.keys())

    return det_match_across_scanindex, average_matches


def UpdataDetectionMarkerBasedOnRadarPosition(filename: str):
    global DETECTION_MARKER
    global RADAR_POS

    if "FLR" in filename:
        RADAR_POS = "FLR"
        DETECTION_MARKER = "FLR" + "_DETECTION_001_004"
    elif "FL" in filename:
        RADAR_POS = "SRR_FL"
        DETECTION_MARKER = "SRR_FL" + "_DETECTION_001_004"
    elif "FR" in filename:
        RADAR_POS = "SRR_FR"
        DETECTION_MARKER = "SRR_FR" + "_DETECTION_001_004"
    elif "RL" in filename:
        RADAR_POS = "SRR_RL"
        DETECTION_MARKER = "SRR_RL" + "_DETECTION_001_004"
    elif "RR" in filename:
        RADAR_POS = "SRR_RR"
        DETECTION_MARKER = "SRR_RR" + "_DETECTION_001_004"


def parse_delete_arg(arg: str):
    if not arg:
        return []
    parts = [x.strip() for x in arg.split(",") if x.strip()]
    return parts


def normalize_stem_by_deletion(stem: str, delete_list):
    s = stem.lower()
    for d in delete_list:
        if not d:
            continue
        s = s.replace(d.lower(), "")
    return s.strip()


def collect_files(dirpath: Path, delete_list):
    d = defaultdict(list)
    for p in dirpath.glob("*"):
        if p.is_file():
            key = normalize_stem_by_deletion(p.stem, delete_list) if sys.platform.startswith(
                "win") else normalize_stem_by_deletion(p.stem, delete_list)
            d[key].append(p)
    return d


def local_name(tag: Optional[str]) -> str:
    if not tag:
        return ""
    return tag.split("}", 1)[-1] if "}" in tag else tag


def load_root(path: str) -> ET.Element:
    try:
        tree = ET.parse(path)
        return tree.getroot()
    except Exception as e:
        print(f"Failed to parse {path}: {e}", file=sys.stderr)
        sys.exit(2)


def find_detection_positions(root: ET.Element, marker: str) -> Tuple[List[ET.Element], List[int]]:
    elems = list(root.iter())
    idxs = [i for i, e in enumerate(elems) if local_name(e.tag) == marker]
    return elems, idxs


def promote_data(parent: ET.Element) -> Tuple[Dict[str, str], List[ET.Element]]:
    """
    If parent has a <Data> child, return merged attributes (Data overrides parent) and combined children:
    parent's non-Data children + Data's children. If no Data child, return parent.attrib and list(parent).
    """
    merged = dict(parent.attrib)
    data_child = None
    for c in list(parent):
        if local_name(c.tag) == "Data":
            data_child = c
            break
    children = [c for c in list(parent) if c is not data_child]
    if data_child is not None:
        merged.update(data_child.attrib)
        children.extend(list(data_child))
    return merged, children


def extract_canid(merged_attrib: Dict[str, str], children: List[ET.Element]) -> Optional[str]:
    for k in CANID_KEYS:
        if k in merged_attrib and merged_attrib[k]:
            return merged_attrib[k]
    for c in children:
        if local_name(c.tag).lower() in ("canid", "can_id", "can"):
            if c.text and c.text.strip():
                return c.text.strip()
    return None


def extract_timestamp(merged_attrib: Dict[str, str], children: List[ET.Element]) -> Optional[str]:
    for k in TS_KEYS:
        if k in merged_attrib and merged_attrib[k]:
            return merged_attrib[k]
    # fallback: any attribute that looks like a timestamp
    for k, v in merged_attrib.items():
        if v and any(ch in v for ch in ("T", ":", "-", "Z", "+")):
            return v
    for c in children:
        if local_name(c.tag).lower() in ("timestamp", "time", "datetime", "ts"):
            if c.text and c.text.strip():
                return c.text.strip()
    return None


def parse_ts(text: Optional[str]) -> Optional[datetime]:
    if not text:
        return None
    s = text.strip()
    if not s:
        return None
    if s.endswith("Z"):
        s = s[:-1] + "+00:00"
    try:
        return datetime.fromisoformat(s)
    except Exception:
        # common fallback formats
        fmts = ("%Y-%m-%d %H:%M:%S.%f", "%Y-%m-%d %H:%M:%S", "%d/%m/%Y %H:%M:%S")
        for f in fmts:
            try:
                return datetime.strptime(s, f)
            except Exception:
                pass
    return None


def collect_messages_between_markers(elems: List[ET.Element], start_idx: int, end_idx: int, radar_pos: str,
                                     filename: str, terminal_print) -> List[Dict]:
    """
    Scan elements between start_idx and end_idx (exclusive) and collect RADARPOS_DETECTION_* messages.
    Returns list of dicts: {tag, attrib, children, canid, raw_ts, ts}
    """
    seen = set()
    msgs = []
    temp_msg = []
    found_dets = False
    found_header = False
    for e in elems[start_idx:end_idx]:
        lname = local_name(e.tag)
        if lname not in seen:
            if lname.startswith(radar_pos + "_DETECTION_"):
                seen.add(lname)
                if found_header:
                    print(f"WARNING: detections was not followed by header for file ={filename}", file=terminal_print)
                    found_header = False
                merged_attrib, children = promote_data(e)
                raw_ts = extract_timestamp(merged_attrib, children)
                canid = extract_canid(merged_attrib, children)
                temp_msg.append({
                    "tag": lname,
                    "attrib": merged_attrib,
                    "children": children,
                    "scanindex": 0,
                    "number_of_detections": 0,
                    "canid": canid,
                    "raw_ts": raw_ts or "",
                    "ts": parse_ts(raw_ts) if raw_ts else None
                })
                found_dets = True
            elif lname.startswith(radar_pos + "_HEADER_") and found_dets:
                seen.add(lname)
                merged_attrib, children = promote_data(e)
                sindex = 0
                for c in children:
                    if c.attrib.get('HED_SCAN_INDEX') is None:
                        sindex +=1
                    else:
                        break

                numdetsindex = 0
                for c in children:
                    if c.attrib.get('HED_NUM_OF_VALID_DETECTIONS') is None:
                        numdetsindex +=1
                    else:
                        break
                for dict in temp_msg:
                    dict["scanindex"] = int(float(children[sindex].attrib['HED_SCAN_INDEX']))
                    dict["number_of_detections"] = int(float(children[numdetsindex].attrib['HED_NUM_OF_VALID_DETECTIONS']))
                    msgs.append(dict)
                temp_msg = []
                found_header = True
                found_dets = False
        else:
            for idx, msg in enumerate(temp_msg):
                if msg["tag"] == lname:
                    print(f"WARNING: same detections was repeated within one scanindex ={filename}", file=terminal_print)
                    merged_attrib, children = promote_data(e)
                    raw_ts = extract_timestamp(merged_attrib, children)
                    canid = extract_canid(merged_attrib, children)
                    repeated_message = {
                        "tag": lname,
                        "attrib": merged_attrib,
                        "children": children,
                        "scanindex": 0,
                        "number_of_detections": 0,
                        "canid": canid,
                        "raw_ts": raw_ts or "",
                        "ts": parse_ts(raw_ts) if raw_ts else None
                    }
                    temp_msg[idx] = repeated_message
                    break
    return msgs


def ensure_csv(path: str):
    header = ["cycle", "index",
              "fileA_tag", "fileA_canid", "fileA_raw_ts", "fileA_scanindex", "fileA_dets_list",
              "fileB_tag", "fileB_canid", "fileB_raw_ts", "fileB_scanindex", "fileB_dets_list",
              "delta_seconds", "scanindex_criteria"]
    if not path:
        return
    if not os.path.exists(path) or os.path.getsize(path) == 0:
        with open(path, "w", newline="", encoding="utf-8") as f:
            csv.writer(f).writerow(header)


def append_row(path: str, row: List):
    if not path:
        return
    with open(path, "a", newline="", encoding="utf-8") as f:
        csv.writer(f).writerow(row)


def compare_cycles(cyclesA: List[List[Dict]], cyclesB: List[List[Dict]], terminal_file, csv_out: Optional[str] = None):
    # create lists of detection tags for comparison
    det_range_tag_list = [f"DET_RANGE_{num:03d}" for num in range(1, 500)]
    det_range_rate_tag_list = [f"DET_RANGE_VELOCITY_{num:03d}" for num in range(1, 500)]
    det_elevation_tag_list = [f"DET_ELEVATION_{num:03d}" for num in range(1, 500)]
    det_azimuth_tag_list = [f"DET_AZIMUTH_{num:03d}" for num in range(1, 500)]
    det_rcs_tag_list = [f"DET_RCS_{num:03d}" for num in range(1, 500)]

    scanindexA = -1
    scanindexB = -1
    file_timestamp_issue_or_eof = False
    mismatch_report = []
    scanindex_failed = 0
    skipped_bmsg = 0
    skipped_amsg = 0
    total_ts_scanindex_matched =0

    min_idx = min(len(cyclesA), len(cyclesB))
    max_idx = max(len(cyclesA), len(cyclesB))

    if csv_out:
        ensure_csv(csv_out)

    print(f"Note: fileA cycles={len(cyclesA)} fileB cycles={len(cyclesB)}; comparing {min_idx} cycles",
              file=sys.stderr)
    print(f"Note: fileA cycles={len(cyclesA)} fileB cycles={len(cyclesB)}; comparing {min_idx} cycles",
              file=terminal_file)
    
        
    # Lets loop through max_idx to identify sync issues between input and output files
    # and skip those cycles which are not matching in timestamp
    for ci in range(max_idx):

        # Loop till we find matching timestamp cycles between input and output files which is less than 40ms difference
        # if the difference is negative then the behavior is unexpected as output file timestamp cannot be older than input file timestamp
        while (True):

            # check if we have reached end of either cyclesA or cyclesB with skips
            if ci + skipped_amsg >= len(cyclesA) or ci + skipped_bmsg >= len(cyclesB):
                file_timestamp_issue_or_eof = True  
                break

            fila_a_timestamp = cyclesA[ci + skipped_amsg][0]["attrib"]["TIMESTAMP"]
            fila_b_timestamp = cyclesB[ci + skipped_bmsg][0]["attrib"]["TIMESTAMP"]
            
            junk_timestamp_found = False
            if fila_a_timestamp =='1.84467E+10':
                skipped_amsg += 1
                junk_timestamp_found = True
            elif fila_b_timestamp =='1.84467E+10':
                skipped_bmsg += 1
                junk_timestamp_found=True

            if not junk_timestamp_found:
                diff = float(fila_b_timestamp) - float(fila_a_timestamp)
                if diff > 0.040:  # output - input > 50ms then the input file has to start with next data
                    skipped_amsg += 1
                    print(f"\nWARNING: data start of the input file is not matching the start of output file \n "
                        f"selecting closest timestamp from input to match with start data of output file ",
                        file=terminal_file)
                elif diff < 0:  # output - input is negative then the output file has to start with next data
                    skipped_bmsg += 1
                    print(f"\nWARNING: This should not happen !!!\n"
                        f"assumption is that output start data matches atleast the start data of input\n"
                        f"output cannot have older timestamp data than the input file", file=terminal_file)
                else:
                    break

        if not file_timestamp_issue_or_eof:
            a_msgs = cyclesA[ci + skipped_amsg]
            b_msgs = cyclesB[ci + skipped_bmsg]

            # print(f"\n=== Cycle {ci+1} ===")
            # Build maps by canid (preserve lists for duplicate canids)
            amap: Dict[str, List[Tuple[int, Dict]]] = {}
            for i, m in enumerate(a_msgs):
                key = m["attrib"]["CAN_ID"] if m["attrib"]["CAN_ID"] is not None else f"__idxA_{i}"
                amap.setdefault(key, []).append((i, m))
            bmap: Dict[str, List[Tuple[int, Dict]]] = {}
            for i, m in enumerate(b_msgs):
                key = m["attrib"]["CAN_ID"] if m["attrib"]["CAN_ID"] is not None else f"__idxB_{i}"
                bmap.setdefault(key, []).append((i, m))

            # union keys
            keys = list(dict.fromkeys(list(amap.keys()) + list(bmap.keys())))
            scanindex_mismatch_check = ""
            dets_list_per_scanindex_fileA = []
            dets_list_per_scanindex_fileB = []
            dets_set = []

            scanindexA =-1
            scanindexB =-1
            # here each key represents one scanindex cycle data
            # keys are CAN IDs or unique identifiers for messages 
            for key in keys:
                alist = amap.get(key, [])
                blist = bmap.get(key, [])
                maxl = max(len(alist), len(blist))

                # loop through all entries for this key which _DETECTION_* messages only
                for k in range(maxl):
                    aentry = alist[k][1] if k < len(alist) else None
                    bentry = blist[k][1] if k < len(blist) else None
                    if scanindexA == -1:
                        scanindexA = aentry["scanindex"] if aentry else 0
                    
                    if scanindexB == -1:
                        scanindexB = bentry["scanindex"] if bentry else 0
                    number_of_detectionsA = aentry["number_of_detections"] if aentry else 0
                    number_of_detectionsB = bentry["number_of_detections"] if bentry else 0
                    
                    if aentry is not None:
                        # decode fileA det data from <data> tag children
                        for c in aentry["children"]:
                            for attribute in c.attrib:
                                if attribute in det_range_tag_list:
                                    if len(dets_set) == 5:
                                        dets_list_per_scanindex_fileA.append(dets_set)
                                    elif len(dets_set) == 0:
                                        # print(f"decoding scanindex = {scanindexA}")
                                        pass
                                    else:
                                        print("WARNING: Incomplete data decoded for det set",file=terminal_file)
                                    dets_set = []
                                    dets_set.append(c.attrib[attribute])
                                elif attribute in det_range_rate_tag_list:
                                    dets_set.append(c.attrib[attribute])
                                elif attribute in det_azimuth_tag_list:
                                    dets_set.append(c.attrib[attribute])
                                elif attribute in det_elevation_tag_list:
                                    dets_set.append(c.attrib[attribute])
                                elif attribute in det_rcs_tag_list:
                                    dets_set.append(c.attrib[attribute])
                                else:
                                    pass  # do nothing

                            # decode fileA det data

                        # add the last dets_set if it has 5 elements
                        if len(dets_set) == 5:
                            dets_list_per_scanindex_fileA.append(dets_set)
                            dets_set = []

                    if bentry is not None:
                        # decode fileB det data from <data> tag children
                        for c in bentry["children"]:
                            for attribute in c.attrib:
                                if attribute in det_range_tag_list:
                                    if len(dets_set) == 5:
                                        dets_list_per_scanindex_fileB.append(dets_set)
                                    elif len(dets_set) == 0:
                                        # print(f"decoding scanindex = {scanindexA}")
                                        pass
                                    else:
                                        print("WARNING: Incomplete data decoded for det set",file=terminal_file)
                                    dets_set = []
                                    dets_set.append(c.attrib[attribute])
                                elif attribute in det_range_rate_tag_list:
                                    dets_set.append(c.attrib[attribute])
                                elif attribute in det_azimuth_tag_list:
                                    dets_set.append(c.attrib[attribute])
                                elif attribute in det_elevation_tag_list:
                                    dets_set.append(c.attrib[attribute])
                                elif attribute in det_rcs_tag_list:
                                    dets_set.append(c.attrib[attribute])
                                else:
                                    pass  # do nothing

                        # add the last dets_set if it has 5 elements
                        if len(dets_set) == 5:
                            dets_list_per_scanindex_fileB.append(dets_set)
                            dets_set = []

                    # timestamp criteria check initialy checks if input and output timestamps are within 40ms
                    # here we also compute delta between input and output timestamps for reporting
                    ts_a = float(aentry["attrib"]["TIMESTAMP"]) if aentry else None
                    ts_b = float(bentry["attrib"]["TIMESTAMP"]) if bentry else None
                    if ts_a and ts_b:
                        delta = (ts_b - ts_a)
                        delta_out = f"{delta:.6f}"
                    else:
                        delta_out = ""

                    if bentry is not None and aentry is not None:
                        # check scanindex is same for matched timestamp entries
                        if bentry["scanindex"] - aentry["scanindex"] == 0:
                            scanindex_mismatch_check = "PASSED"
                            total_ts_scanindex_matched+=1/ len(keys)
                        else:
                            scanindex_mismatch_check = "FAILED"
                            scanindex_failed += (1 / len(keys))

                        if csv_out:
                            row = [
                                ci + 1,
                                key,
                                aentry["tag"] if aentry else "",
                                aentry["attrib"]["CAN_ID"] if aentry else "",
                                aentry["attrib"]["TIMESTAMP"] if aentry else "",
                                aentry["scanindex"],
                                dets_list_per_scanindex_fileA,
                                bentry["tag"] if bentry else "",
                                bentry["attrib"]["CAN_ID"] if bentry else "",
                                bentry["attrib"]["TIMESTAMP"] if bentry else "",
                                bentry["scanindex"],
                                dets_list_per_scanindex_fileB,
                                delta_out,
                                scanindex_mismatch_check
                            ]
                            append_row(csv_out, row)
                    else:
                        scanindex_mismatch_check = "FAILED"
                        scanindex_failed += (1 / len(keys))

            # after processing all messages for this scanindex cycle , 
            # if scanindex check passed, store dets list in global maps of scanidex: list of dets
            if scanindex_mismatch_check == "PASSED":
                if scanindexA > 0:
                    fileA_scanindex_det_map[scanindexA] = dets_list_per_scanindex_fileA
                    fileA_scanindex_num_of_det_map[scanindexA] = number_of_detectionsA

                if scanindexB > 0:
                    fileB_scanindex_det_map[scanindexB] = dets_list_per_scanindex_fileB
                    fileB_scanindex_num_of_det_map[scanindexB] = number_of_detectionsB  

            elif scanindex_mismatch_check == "FAILED":
                mismatch_report.append(scanindexB)

        else :
            break

    percentage = 0.0
    if min_idx != 0:
        percentage = (total_ts_scanindex_matched / max_idx) * 100
        print(
            f"\n{BLUE}[{RADAR_POS}] : {percentage}% match between input and output, {(skipped_bmsg + skipped_amsg)} scanindex was discarded due to sync issue b/w input and output!{RESET}")
        print(f"\n{BLUE}[{RADAR_POS}] : Discarded scanindexes in output file due to mismatch: {mismatch_report}{RESET}")

        print(
            f"\n{BLUE}[{RADAR_POS}] : {percentage}% match between input and output, {(skipped_bmsg + skipped_amsg)} scanindex was discarded due to sync issue b/w input and output!{RESET}",file=terminal_file)
        print(f"\n{BLUE}[{RADAR_POS}] : Discarded scanindexes in output file due to mismatch: {mismatch_report}{RESET}", file=terminal_file)

    else:
        print(
            f"\n [{RADAR_POS}] : Output or Input file has no cycles to compare!!")
        print(
            f"\n [{RADAR_POS}] : Output or Input file has no cycles to compare!!", file=terminal_file)

    return total_ts_scanindex_matched, scanindex_failed, (skipped_bmsg + skipped_amsg), percentage


def find_first_xml(folder: Path):
    files = sorted([p for p in folder.glob("*.xml") if p.is_file()])
    return files if files else None


def extract_between_rr000(name: str):
    # case-insensitive search for '_rR000' then capture until next '_' (or end)
    m = re.search(r"_rR000([^_]*)_?", name, flags=re.IGNORECASE)
    return m.group(1) if m else None


def extract_between_r00(name: str):
    # case-insensitive search for '_rR000' then capture until next '_' (or end)
    m = re.search(r"_r00([^_]*)_?", name, flags=re.IGNORECASE)
    return m.group(1) if m else None

# [END]================= Helper functions for XML Files Comparison =====================


def main():
    global RADAR_POS
    global CUSTOMER_STR
    global DETECTION_MARKER
    global RADAR_POS_LIST
    global DEBUG_MODE
    global fileA_scanindex_det_map
    global fileB_scanindex_det_map
    global fileA_scanindex_num_of_det_map
    global fileB_scanindex_num_of_det_map
    global terminal_print_file_handler

    # The command needs to be given as:
    # compareXMLs.py --input_dir <input directory path> --output_dir <output directory path> 
    #                   --delete= <delete substring from output file for matching eg: _rR000601156>
    p = argparse.ArgumentParser(description="Compare timestamp and CAN ID between two XML files.")
    p.add_argument("--input_dir", help="input folder path")
    p.add_argument("--output_dir", help="output folder path")
    p.add_argument("--output_csv", help="output folder path")
    p.add_argument("--delete",
                   help="comma-separated substrings to delete from filenames when comparing (e.g. _v1,_final)")
    p.add_argument("positional", nargs="*", help="positional args: input output mode (in order)")

    # if the user provided positional args, use them to fill in missing named args
    # in order: input_dir, output_dir, output_csv, delete 
    # for example: compareXMLs.py ./input ./output ./reports/ 
    # positional args override named args only if named args are missing
    args = p.parse_args()
    pos = args.positional or []

    if args.input_dir is None and len(pos) > 0:
        args.input_dir = pos[0]
    if args.output_dir is None and len(pos) > 1:
        args.output_dir = pos[1]
    if args.output_csv is None and len(pos) > 2:
        args.output_csv = pos[2]
    if args.delete is None and len(pos) > 3:
        args.delete = pos[3]

    terminal_filename = "terminal_print.txt"
    terminal_print_idx = 0
    if os.path.exists(terminal_filename):
        while True:
            terminal_print_idx += 1
            terminal_filename = args.output_csv + f"/terminal_print_{terminal_print_idx}.txt"
            if not os.path.exists(terminal_filename):
                break

    terminal_print_file_handler = open(args.output_csv + f"/terminal_print_{terminal_print_idx}.txt", 'w')

    print(f"\nRemoving redundant header found in bordnet XMLs for both input and output")
    print(f"\nRemoving redundant header found in bordnet XMLs for both input and output", file=terminal_print_file_handler)

    # Remove redundant header line from input and output bordnet XMLs
    # header of type "<?xml version="1.0" encoding="UTF-8" standalone="yes"?>"
    print(f"\nInput path : {args.input_dir}")
    print(f"\nInput path : {args.input_dir}", file=terminal_print_file_handler)
    remove_redundant_header(args.input_dir,terminal_print_file_handler)

    print(f"\nOutput path : {args.output_dir}")
    print(f"\nOutput path : {args.output_dir}", file=terminal_print_file_handler)
    remove_redundant_header(args.output_dir,terminal_print_file_handler)
    print("", file=terminal_print_file_handler)

    # find customer string from xml files from input folder
    folder = Path(args.input_dir)
    first = find_first_xml(folder)
    customer_file_str = None
    customer = None
    for p in folder.glob("*.xml"):
        if p.is_file():
            for cust in CUSTOMER_STR:
                if cust in p.name:
                    customer_file_str = cust + "_PCAN"
                    customer= cust
                    break
            if customer_file_str:
                break  

    # determine delete arg values from first XML file in output folder if not provided
    # e.g. _rR000601156 extracted from filename like bordnet_rR000601156_v1.xml
    if args.delete is None:
        folder = Path(args.output_dir)
        if not folder.is_dir():
            print(f"Not a folder: {folder}", file=sys.stderr)
            print(f"Not a folder: {folder}", file=terminal_print_file_handler)
            sys.exit(2)

        first = find_first_xml(folder)
        user_in = None
        for i,file in enumerate(first):
            if not file:
                print("No .xml files found.", file=sys.stderr)
                print("No .xml files found.",file=terminal_print_file_handler)
                sys.exit(3)

            string_version_length = None
            if "rR000" in file.name:
                user_in = "_rR000" + extract_between_rr000(file.name)
                string_version_length = len("_rR000xxxxxx")
            else:
                user_in = "_r00" + extract_between_r00(file.name)
                string_version_length = len("_r00xxxxxx")
            #if extracted string is too long, skip this file and try next
            if string_version_length and len(str(user_in))>string_version_length:
                continue
            else:
                break

        if customer == "CEER":         
            user_in= f"{user_in}, _DEBUG, _BUS, _can"

        delete_list = parse_delete_arg(user_in)
    else:
        delete_list = parse_delete_arg(args.delete)


    
    in_dir = Path(args.input_dir)
    out_dir = Path(args.output_dir)
    if not in_dir.is_dir():
        print(f"Input folder not found: {in_dir}", file=sys.stderr)
        print(f"Input folder not found: {in_dir}", file=terminal_print_file_handler)
        sys.exit(2)
    if not out_dir.is_dir():
        print(f"Output folder not found: {out_dir}", file=sys.stderr)
        print(f"Output folder not found: {out_dir}", file=terminal_print_file_handler)
        sys.exit(2)

    # collect files in input and output folders, removes specified substrings from filenames for matching
    # and store in map of normalized stem -> list of Paths
    in_map = collect_files(in_dir, delete_list)
    out_map = collect_files(out_dir, delete_list)

    printed_stems = set()
    matched_any = False
    
    # need to determine first_file_name for consolidated report
    # this is done by finding first key in in_map that contains "mcip_pcan"
    # and extracting substring up to "mcip_pcan" for use as prefix
    first_file_name = None
    for key in in_map.keys():
        if customer_file_str.lower() in key:
            first_file_name = str(key)
            first_file_name = first_file_name[0:first_file_name.rfind(customer_file_str.lower())]
            break

    consolidated_csv_file_path = args.output_csv + first_file_name + "consolidated_report.csv"
    if os.path.exists(consolidated_csv_file_path):
        os.remove(consolidated_csv_file_path)
        print(f"File '{consolidated_csv_file_path}' deleted successfully.", file=terminal_print_file_handler)
    else:
        print(f"File '{consolidated_csv_file_path}' does not exist.", file=terminal_print_file_handler)

    header_consolidated = ["file", "RadarPos", "decoded_scanindex_input", "decoded_scanindex_output",
                           "scanindex_used_for_comparison", "% timestamp-scanindex match", "% dets parameter match"]
    with open(consolidated_csv_file_path, "w", newline="", encoding="utf-8") as f:
        csv.writer(f).writerow(header_consolidated)

    dataset = {"FLR": [], "SRR_FL": [], "SRR_FR": [], "SRR_RL": [], "SRR_RR": []}
    for key in sorted(set(in_map.keys()) & set(out_map.keys())):
        for inp in in_map[key]:
            for outp in out_map[key]:
                name_in = inp.name
                name_out = outp.name
                # require at least one filename contains MCIP_PCAN (case-insensitive)
                if (customer_file_str.upper() not in name_in.upper()) and (customer_file_str.upper() not in name_out.upper()):
                    continue
                # if either filename contains CANID_TS and its normalized stem was already printed, skip this match
                stem_in_norm = normalize_stem_by_deletion(inp.stem, delete_list)
                stem_out_norm = normalize_stem_by_deletion(outp.stem, delete_list)
                if ("CANID_TS" in name_in.upper()) or \
                        ("CANID_TS" in name_out.upper()):
                    continue
                # print match and record printed normalized stems
                print(f"Matched: {inp}  <->  {outp}")
                print(f"Matched: {inp}  <->  {outp}", file=terminal_print_file_handler)

                file_n = str(outp.stem)

                if DEBUG_MODE:
                    csv_file_path = args.output_csv + file_n + "_detailed_report.csv"  # Replace with the actual file path

                    if os.path.exists(csv_file_path):
                        os.remove(csv_file_path)
                        print(f"File '{csv_file_path}' deleted successfully.", file=terminal_print_file_handler)
                    else:
                        print(f"File '{csv_file_path}' does not exist.", file=terminal_print_file_handler)
                else:
                    csv_file_path = None

                # update DETECTION_MARKER based on radar position in filename
                # e.g. FLR will have FLR_DETECTION_001_004 as DETECTION_MARKER 
                UpdataDetectionMarkerBasedOnRadarPosition(outp.stem)

                # load XML roots of input and output xml files
                rootA = load_root(str(inp))
                rootB = load_root(str(outp))

                # find positions of DETECTION_MARKER in input and output xml files
                # these positions will be used to extract cycles
                # e.g. FLR_DETECTION_001_004 for FLR radar position and its positions in the xml file is stored in idxsA and idxsB
                # and elemsA and elemsB store all the elements of input and output xml files respectively
                elemsA, idxsA = find_detection_positions(rootA, DETECTION_MARKER)
                elemsB, idxsB = find_detection_positions(rootB, DETECTION_MARKER)

                # if fewer than 2 detection markers found in either file, no cycles to compare
                if len(idxsA) < 2 or len(idxsB) < 2:
                    print("WARNING: fewer than 2 detection markers in one of the files; no cycles to compare.",
                          file=sys.stderr)
                    print("WARNING: fewer than 2 detection markers in one of the files; no cycles to compare.",
                          file=terminal_print_file_handler)
                

                cyclesA = []

                # collect messages between detection markers for input xml file and store in cyclesA
                # if header is missed, retry by skipping next detection marker once 
                # this is to handle repeated detection markers within the same scanindex cycle
                skip_i = False
                for i in range(max(0, len(idxsA) - 1)):
                    if not skip_i:
                        run_once = 2
                        s, e = idxsA[i], idxsA[i + 1]
                        while run_once > 0:
                            collect_message_list = collect_messages_between_markers(elemsA, s, e, RADAR_POS, str(inp), terminal_print_file_handler)
                            if len(collect_message_list) != 0:
                                cyclesA.append(collect_message_list)
                                break
                            else:
                                print(
                                    f"fileA missed header decoding, Retrying again!! Maybe there is a repeated message of limiter DETECTION_MARKER",
                                    file=terminal_print_file_handler)

                                if i+2 < (len(idxsA)):
                                    s, e = idxsA[i], idxsA[i + 2]
                                    skip_i = True
                            run_once-=1
                    else:
                        skip_i = False

                # collect messages between detection markers for output xml file and store in cyclesB
                # if header is missed, retry by skipping next detection marker once 
                # this is to handle repeated detection markers within the same scanindex cycle
                cyclesB = []
                skip_i = False
                for i in range(max(0, len(idxsB) - 1)):
                    if not skip_i:
                        run_once = 2
                        s, e = idxsB[i], idxsB[i + 1]
                        while run_once > 0:
                            collect_message_list = collect_messages_between_markers(elemsB, s, e, RADAR_POS, str(outp),terminal_print_file_handler)
                            if len(collect_message_list) != 0:
                                cyclesB.append(collect_message_list)
                                break
                            else:
                                print(
                                    f"fileB missed header decoding, Retrying again!! Maybe there is a repeated message of limiter DETECTION_MARKER",
                                    file=terminal_print_file_handler)
                                
                                if i+2 < (len(idxsB)):
                                    s, e = idxsB[i], idxsB[i + 2]
                                    skip_i = True
                            run_once-=1
                    else:
                        skip_i = False

                # now cyclesA and cyclesB contain the extracted scanindex cycles from input and output xml files respectively
                # but they may have different number of cycles due to sync issues 
                # so we compare input and output cycles with same scanindex and timestamp and record the match results in csv file
                scanindex_compared, _, _, percentage_match = compare_cycles(cyclesA,cyclesB,csv_out=csv_file_path,terminal_file=terminal_print_file_handler)
                
                _, det_average_match = compute_match_stats(fileA_scanindex_det_map, fileB_scanindex_det_map, fileA_scanindex_num_of_det_map, fileB_scanindex_num_of_det_map,
                                                                      tol=0)

                row = [str(inp.name), RADAR_POS, len(cyclesA), len(cyclesB), scanindex_compared, percentage_match,
                       0 if det_average_match is None else det_average_match]

                print(
                    f"\n********************************************************************************************************", file=terminal_print_file_handler)
                print(f"\tRESULT SUMMARY FOR [{RADAR_POS}]: {row[2:],}"
                      f"\n\tScanindex that was successfully decoded from input xml = {row[2]}"
                      f"\n\tScanindex that was successfully decoded from output xml= {row[3]}"
                      f"\n\tScanindex that was used for comparison match between input and output = {row[4]}"
                      f"\n\t% of timestamp- scanindex match between input and output = {row[5]} %"
                      f"\n\t% of detections matched for timestamp- scanindex match between input and output={row[6]} % ",file=terminal_print_file_handler)
                print(
                    f"********************************************************************************************************\n", file=terminal_print_file_handler)

                dataset[RADAR_POS].append({str(inp.name): [row[2], row[3], row[4], row[5], row[6]]})
                fileA_scanindex_det_map = {}
                fileB_scanindex_det_map = {}
                fileA_scanindex_num_of_det_map = {}
                fileB_scanindex_num_of_det_map = {}
                printed_stems.add(stem_in_norm)
                printed_stems.add(stem_out_norm)
                matched_any = True

    html_file_path = consolidated_csv_file_path[:consolidated_csv_file_path.rfind(".")] + ".html"
    create_multi_set_plot( datasets=dataset,
            set_names=RADAR_POS_LIST,
            output_file=html_file_path,
            title=""
        )
    
    if not matched_any:
        print("No matches found.")
        print("No matches found.",file=terminal_print_file_handler)
    
    terminal_print_file_handler.close()


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print(
            "Usage: python compare_xml_cycles.py --input_dir <input directory path> --output_dir <output directory path> --delete= <delete substring from output file for matching eg: _rR000601156>",
            file=sys.stderr)
        sys.exit(1)

    main()
