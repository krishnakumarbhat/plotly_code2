from IPS.EventMan.ievent_mediator import IEventMediator
from IPS.DataStore.idatastore import IDataStore
from IPS.Sig_Prep.sig_filter import SignalFilter
from IPS.DataCollect.icollectdata import IDataCollect

class CANPlotDataPreparation:
    def __init__(self, mediator: IEventMediator, radar_ds: IDataStore, plot_ds: IDataStore, json_ds: IDataStore, sig_fil: SignalFilter, can_collector: IDataCollect):
        self._mediator = mediator
        self._plot_ds = plot_ds
        self._radar_ds = radar_ds
        self._sig_fil = sig_fil

    def clear_data(self): pass

    def consume_event(self, sensor, event):
        if event != "CAN_DATA_COLLECTION_DONE": return
        signals = ["DET_RANGE", "DET_RCS", "DET_AZIMUTH", "DET_ELEVATION", "DET_RANGE_VELOCITY", "DET_SNR"]
        for sig in signals:
            try:
                for src in ('in', 'out'):
                    ts = self._radar_ds.get_data(sensor, "timestamp_data", src)
                    data = self._radar_ds.get_data(sensor, sig, src)
                    if not ts or not data: continue
                    flat_ts, flat_sig = [], []
                    for stream, vals in ts.items():
                        sig_data = [v for sub in data for v in sub]
                        total = len(sig_data)
                        if vals and total:
                            rep, rem = total // len(vals), total % len(vals)
                            scaled = vals * rep + vals[:rem]
                            flat_ts.extend(scaled)
                            flat_sig.extend(sig_data)
                    self._plot_ds.update_data("flattened_timestamp", flat_ts, src, sensor)
                    self._plot_ds.update_data(f"flattened_{sig.lower()}", flat_sig, src, sensor)
                self._mediator.notify_event(sensor, "CAN_DATA_PREP_UPDATED")
            except Exception as e:
                print(f"Error {sig} for {sensor}: {e}")
