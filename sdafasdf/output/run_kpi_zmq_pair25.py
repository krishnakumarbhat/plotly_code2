import sys, os, json, time, subprocess, signal, socket, re
sys.path.insert(0, r'C:\Users\ouymc2\Desktop\sdafasdf\output\KPI')
sys.path.insert(0, r'C:\Users\ouymc2\Desktop\sdafasdf\output\KPI\intplot_kpi')

os.environ.setdefault('PROTOCOL_BUFFERS_PYTHON_IMPLEMENTATION', 'python')
import zmq
from UDP_KPI.runtime_utils import ensure_dir, normalize_fs_path
from InteractivePlot.kpi_client.hdf_add_pb2 import RequestMessage, ReplyMessage, PingMessage, PongMessage

base = r'C:\Users\ouymc2\Desktop\sdafasdf\output'
hdf_path = os.path.join(base, 'IFV7XX_WBATR95060NC89209_20250919_080721_0000_b05.h5')
out_dir = ensure_dir(os.path.join(base, 'pair25_zmq'))
base_name = 'pair25_ZMQ_IFV7XX_b05'
port = 5555

sensors = ['FC','FL','FR','RL','RR']

server_proc = None
try:
    # 1) Start ZMQ server in background
    log_path = os.path.join(base, 'zmq_server.log')
    print('Starting KPI ZMQ server on port {}...'.format(port))
    server_proc = subprocess.Popen(
        [sys.executable, '-m', 'UDP_KPI.kpi_server', 'zmq', str(port)],
        cwd=os.path.join(base, 'KPI'),
        stdout=open(log_path, 'w'), stderr=subprocess.STDOUT,
        creationflags=subprocess.CREATE_NEW_PROCESS_GROUP if hasattr(subprocess, 'CREATENEWPROCESSGROUP') else 0,
    )

    # Wait for server to start
    context = zmq.Context()
    sock = context.socket(zmq.REQ)
    sock.setsockopt(zmq.LINGER, 0)
    sock.connect('tcp://127.0.0.1:{}'.format(port))

    for attempt in range(20):
        try:
            ping = PingMessage(message_type='ping')
            sock.send(ping.SerializeToString())
            if sock.poll(1000):
                resp_bytes = sock.recv()
                pong = PongMessage()
                pong.ParseFromString(resp_bytes)
                if pong.status == 'pong':
                    print('Server responded after {}s'.format(attempt * 0.5))
                    break
        except Exception:
            sock.close(linger=0)
            sock = context.socket(zmq.REQ)
            sock.setsockopt(zmq.LINGER, 0)
            sock.connect('tcp://127.0.0.1:{}'.format(port))
        time.sleep(0.5)
    else:
        print('ERROR: Server did not start')
        raise SystemExit(1)
    sock.close()

    # 2) Send KPI requests for each sensor
    results = []
    for sensor in sensors:
        print('\n--- Sensor {} ---'.format(sensor))
        sock = context.socket(zmq.REQ)
        sock.setsockopt(zmq.LINGER, 0)
        sock.connect('tcp://127.0.0.1:{}'.format(port))
        try:
            req = RequestMessage(
                sensor_id=sensor,
                hdf_file_path=normalize_fs_path(hdf_path),
                output_dir=normalize_fs_path(out_dir),
                base_name=base_name,
                kpi_subdir='KPI',
                output_hdf_path=normalize_fs_path(hdf_path),
                input_file=normalize_fs_path(hdf_path),
                sensor=sensor,
                server_port=port,
            )
            print('  Sending request...')
            sock.send(req.SerializeToString())
            if sock.poll(60000) == 0:
                print('  TIMEOUT after 60s')
                results.append({'sensor': sensor, 'status': 'timeout'})
                continue
            resp_bytes = sock.recv()
            reply = ReplyMessage()
            reply.ParseFromString(resp_bytes)
            html_path = reply.html_file_path
            status = reply.status
            msg = reply.message
            print('  Status: {}'.format(status))
            print('  HTML:   {}'.format(html_path))
            if msg:
                print('  Msg:    {}'.format(msg))

            # Read HTML to confirm content
            if html_path and os.path.exists(html_path):
                content = open(html_path, 'r', encoding='utf-8').read()
                m = re.search(r'Accuracy[:\s]*([\d.]+)%', content)
                accuracy = float(m.group(1)) if m else None
                print('  Accuracy: {}%'.format(accuracy if accuracy is not None else 'N/A'))
                results.append({'sensor': sensor, 'status': status, 'html': html_path, 'accuracy': accuracy})
            else:
                results.append({'sensor': sensor, 'status': status, 'html': html_path, 'accuracy': None})
        except Exception as e:
            print('  ERROR: {}'.format(e))
            results.append({'sensor': sensor, 'status': 'error', 'error': str(e)})
        finally:
            sock.close(linger=0)
        time.sleep(0.5)

    # 3) Print summary
    print('\n' + '=' * 70)
    print('ZMQ KPI Results for Pair25 (IFV7XX_b05 self-pair)')
    print('=' * 70)
    for r in results:
        acc = r.get('accuracy')
        acc_str = '{:.2f}%'.format(acc) if acc is not None else 'N/A'
        print('{:<5s} -> status={:<10s} accuracy={}'.format(r['sensor'], r['status'], acc_str))

finally:
    if server_proc:
        print('\nStopping ZMQ server...')
        server_proc.terminate()
        server_proc.wait(timeout=5)
        print('Server stopped.')
