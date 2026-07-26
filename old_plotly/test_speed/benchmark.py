import os
import sys
import time
import socket
import urllib.parse
import subprocess
import json
import statistics
from typing import Dict, Any, List

def parse_url(url: str) -> tuple:
    parsed = urllib.parse.urlparse(url)
    scheme = parsed.scheme
    hostname = parsed.hostname
    port = parsed.port
    if not port:
        port = 80 if scheme == 'http' else 443
    path = parsed.path if parsed.path else '/'
    if parsed.query:
        path += '?' + parsed.query
    return scheme, hostname, port, path

def run_ping(hostname: str) -> Dict[str, Any]:
    print(f"Running ping to {hostname}...")
    # On Windows, ping -n 4. On Linux, ping -c 4.
    param = '-n' if os.name == 'nt' else '-c'
    cmd = ['ping', param, '4', hostname]
    try:
        start_time = time.time()
        res = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, timeout=10)
        elapsed = time.time() - start_time
        return {
            "success": res.returncode == 0,
            "stdout": res.stdout,
            "stderr": res.stderr,
            "elapsed_seconds": elapsed
        }
    except Exception as e:
        return {
            "success": False,
            "error": str(e)
        }

def run_traceroute(hostname: str) -> Dict[str, Any]:
    print(f"Running traceroute to {hostname}...")
    # On Windows, tracert. On Linux, traceroute.
    cmd = ['tracert', hostname] if os.name == 'nt' else ['traceroute', hostname]
    try:
        start_time = time.time()
        # Limit traceroute max hops to 15 to make it faster
        if os.name == 'nt':
            cmd = ['tracert', '-h', '15', hostname]
        else:
            cmd = ['traceroute', '-m', '15', hostname]
            
        res = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, timeout=20)
        elapsed = time.time() - start_time
        return {
            "success": res.returncode == 0,
            "stdout": res.stdout,
            "stderr": res.stderr,
            "elapsed_seconds": elapsed
        }
    except Exception as e:
        return {
            "success": False,
            "error": str(e)
        }

def measure_http_stages(url: str) -> Dict[str, Any]:
    scheme, hostname, port, path = parse_url(url)
    
    metrics = {
        "dns_resolution_ms": 0.0,
        "tcp_connection_ms": 0.0,
        "time_to_first_byte_ms": 0.0,
        "content_transfer_ms": 0.0,
        "total_time_ms": 0.0,
        "status_code": 0,
        "content_length": 0,
        "headers": {},
        "ip_address": "",
        "error": None
    }
    
    try:
        t0 = time.perf_counter()
        # 1. DNS Resolution
        ip = socket.gethostbyname(hostname)
        t1 = time.perf_counter()
        metrics["dns_resolution_ms"] = (t1 - t0) * 1000.0
        metrics["ip_address"] = ip
        
        # 2. TCP Connection
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(5.0) # 5 seconds connection timeout
        
        t2_start = time.perf_counter()
        s.connect((ip, port))
        t2_end = time.perf_counter()
        metrics["tcp_connection_ms"] = (t2_end - t2_start) * 1000.0
        
        # Send HTTP GET Request
        request_headers = (
            f"GET {path} HTTP/1.1\r\n"
            f"Host: {hostname}:{port}\r\n"
            f"User-Agent: SpeedTestBenchmarking/1.0\r\n"
            f"Accept: */*\r\n"
            f"Connection: close\r\n\r\n"
        )
        
        t3_start = time.perf_counter()
        s.sendall(request_headers.encode('utf-8'))
        
        # Read response headers (character by character or in small chunks until \r\n\r\n)
        header_buffer = b""
        ttfb_recorded = False
        t_ttfb = 0.0
        
        while True:
            chunk = s.recv(1)
            if not chunk:
                break
            if not ttfb_recorded:
                # Time to First Byte recorded on first chunk returned
                t_ttfb = time.perf_counter()
                ttfb_recorded = True
            header_buffer += chunk
            if b"\r\n\r\n" in header_buffer:
                break
                
        if not ttfb_recorded:
            raise Exception("No response received from server")
            
        metrics["time_to_first_byte_ms"] = (t_ttfb - t3_start) * 1000.0
        
        # Read body content
        content_buffer = b""
        while True:
            chunk = s.recv(4096)
            if not chunk:
                break
            content_buffer += chunk
            
        t4_end = time.perf_counter()
        metrics["content_transfer_ms"] = (t4_end - t_ttfb) * 1000.0
        metrics["total_time_ms"] = (t4_end - t0) * 1000.0
        
        s.close()
        
        # Parse headers and status line
        header_part = header_buffer.decode('utf-8', errors='ignore')
        header_lines = header_part.split("\r\n")
        
        # Status line: e.g. "HTTP/1.1 200 OK"
        status_line = header_lines[0]
        status_parts = status_line.split(" ", 2)
        if len(status_parts) >= 2:
            metrics["status_code"] = int(status_parts[1])
            
        parsed_headers = {}
        for line in header_lines[1:]:
            if ":" in line:
                key, val = line.split(":", 1)
                parsed_headers[key.strip().lower()] = val.strip()
                
        metrics["headers"] = parsed_headers
        metrics["content_length"] = len(content_buffer) + len(header_buffer)
        
    except Exception as e:
        metrics["error"] = str(e)
        
    return metrics

def run_load_test(url: str, count: int = 5) -> List[Dict[str, Any]]:
    print(f"Running benchmarking load test with {count} sequential requests to {url}...")
    results = []
    for i in range(count):
        print(f"  Request {i+1}/{count}...")
        res = measure_http_stages(url)
        results.append(res)
        time.sleep(0.2) # Small cooldown between requests
    return results

def main():
    urls = {
        "Fast Server KPI": "http://10.214.45.45:5002/html/kpi",
        "Slow Server General": "http://10.192.224.131:5002/html"
    }
    
    analysis = {}
    
    for label, url in urls.items():
        print(f"\n==================================================")
        print(f"ANALYZING: {label} ({url})")
        print(f"==================================================")
        
        scheme, hostname, port, path = parse_url(url)
        
        # 1. Run Ping
        ping_res = run_ping(hostname)
        
        # 2. Run Traceroute
        trace_res = run_traceroute(hostname)
        
        # 3. Single highly detailed latency measurement
        details = measure_http_stages(url)
        
        # 4. Multi-request Load Benchmarking
        load_details = run_load_test(url, count=5)
        
        analysis[label] = {
            "url": url,
            "hostname": hostname,
            "port": port,
            "path": path,
            "ping": ping_res,
            "traceroute": trace_res,
            "single_measurement": details,
            "load_test": load_details
        }
        
    # Write analysis as JSON
    os.makedirs("test_speed", exist_ok=True)
    with open("test_speed/raw_results.json", "w") as f:
        json.dump(analysis, f, indent=4)
        
    print("\nBenchmark completed. Results written to test_speed/raw_results.json")

if __name__ == "__main__":
    main()
