import sys, os, time, subprocess
sys.path.insert(0, r'C:\Users\ouymc2\Desktop\sdafasdf\output\KPI')
sys.path.insert(0, r'C:\Users\ouymc2\Desktop\sdafasdf\output\KPI\intplot_kpi')
os.environ.setdefault('PROTOCOL_BUFFERS_PYTHON_IMPLEMENTATION', 'python')
os.environ['INTERACTIVE_PLOT_ENABLE_KPI'] = '1'

if __name__ == '__main__':
    base = r'C:\Users\ouymc2\Desktop\sdafasdf\output'
    hdf_path = os.path.join(base, 'IFV7XX_WBATR95060NC89209_20250919_080721_0000_b05.h5')
    out_dir = os.path.join(base, 'pair25_intplot_kpi')
    os.makedirs(out_dir, exist_ok=True)
    port = 5555

    # 1. Start ZMQ server first
    log_path = os.path.join(base, 'zmq_server.log')
    print('Starting KPI ZMQ server on port {}...'.format(port))
    server = subprocess.Popen(
        [sys.executable, '-m', 'UDP_KPI.kpi_server', 'zmq', str(port)],
        cwd=os.path.join(base, 'KPI'),
        stdout=open(log_path, 'w'), stderr=subprocess.STDOUT,
    )

    # Wait for it to be ready
    import zmq
    from InteractivePlot.kpi_client.hdf_add_pb2 import PingMessage, PongMessage
    context = zmq.Context()
    for i in range(20):
        try:
            s = context.socket(zmq.REQ)
            s.setsockopt(zmq.LINGER, 0)
            s.connect('tcp://127.0.0.1:{}'.format(port))
            s.send(PingMessage(message_type='ping').SerializeToString())
            if s.poll(1000):
                resp = PongMessage()
                resp.ParseFromString(s.recv())
                s.close()
                if resp.status == 'pong':
                    print('Server ready after {}s'.format(i * 0.5))
                    break
            s.close()
        except:
            pass
        time.sleep(0.5)
    else:
        print('Server not ready, continuing anyway...')

    # 2. Run InteractivePlot pipeline
    from InteractivePlot.b_persistence_layer.allsensor_hdf_parser import AllsensorHdfParser
    print('Running InteractivePlot AllsensorHdfParser...')
    parser = AllsensorHdfParser({hdf_path: hdf_path}, output_dir=out_dir)
    parser.parse()

    # 3. Stop server
    print('Stopping ZMQ server...')
    server.terminate()
    server.wait(timeout=5)

    # Show results
    result_dir = os.path.join(out_dir, 'IFV7XX_WBATR95060NC89209_20250919_080721_0000_b05')
    if os.path.exists(result_dir):
        for root, dirs, files in os.walk(result_dir):
            for f in files:
                if f.endswith('.html'):
                    print('  {}/{}'.format(os.path.relpath(root, result_dir), f))
