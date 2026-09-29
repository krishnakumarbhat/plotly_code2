from MUDP_FRAME import *
from asammdf import MDF, Signal
import sys
import os

def getEthFrames(m, label):
    eth_frames = None
    if label in m.channels_db:
        # Read channel IDs for ETH_Frame
        channel_id = [x[0] for x in m.channels_db[label]]
        # Get actual channel data for ETH_Frame with corresponding channel_ids
        eth_frames = m.select([[label, ch] for ch in channel_id])

    return eth_frames


def DecodeStreams(databytes):
    pass


def LatchDspaceStream(raw_udp_frame):
    pass


if __name__ == "__main__":
    if len(sys.argv)>1:
        for i in range(len(sys.argv)-1):
            file = sys.argv[i+1]
            filedir =os.path.dirname(os.path.realpath(file))
            filename = os.path.basename(os.path.realpath(file))
            filename = filename[:filename.rfind(".")]
            m = MDF(file)
            eth_frames = getEthFrames(m, "MF4Frame")
            if eth_frames is None:
                eth_frames = getEthFrames(m, "DelphiRawData")
                if eth_frames is None:
                    eth_frames = getEthFrames(m, "ETH_Frame")
                multi_raw_frames = [frame.samples for frame in eth_frames]
            else:
                multi_raw_frames = [frame.samples["MF4Frame.DataBytes"] for frame in eth_frames]
            ipn_frames = []
            mudp_frames = []

            for i, raw_frames in enumerate(multi_raw_frames):
                for j, single_raw_frame in enumerate(raw_frames[:]):
                    mudp_frames.append(single_raw_frame)

            fh = open(filedir+ "/" + filename + ".csv", "w")
            fh.write("versionInfo, sourceTxCnt, sourceTxTime, sourceInfo,sourceInfo1,sourceInfo2,"
                     "sourceInfo3,streamRefIndex,streamDataLen, streamTxCnt, streamNumber, streamVersion, "
                     "streamChunksPerCycle,streamChunks, streamChunkIdx, reserved, sensorId")
            fh.write("\n")
            dc_core0_stream_latched =[]
            dc_core1_stream_latched =[]
            dc_core2_stream_latched =[]
            dc_core3_stream_latched =[]

            dc_core0_stream_latch_complete_flag =False
            dc_core1_stream_latch_complete_flag =False
            dc_core2_stream_latch_complete_flag =False
            dc_core3_stream_latch_complete_flag =False

            for mudp in mudp_frames:
                msg = MUDP_FRAME.UDP_FRAME_T(bytearray(mudp))
                # print('total chunks={} chunkidx={}'.format(msg.streamChunks, msg.streamChunkIdx))
                if True: #(msg.mudp_frame_header.versionInfo == 0xa51c or msg.mudp_frame_header.versionInfo == 0x1ca5):
                    fh.write("{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}".format(hex(msg.mudp_frame_header.versionInfo),
                                                                                   hex(msg.mudp_frame_header.sourceTxCnt),
                                                                                   hex(msg.mudp_frame_header.sourceTxTime),
                                                                                   hex(msg.mudp_frame_header.sourceInfo),
                                                                                   hex(msg.mudp_frame_header.sourceInfo1),
                                                                                   hex(msg.mudp_frame_header.sourceInfo2),
                                                                                   hex(msg.mudp_frame_header.sourceInfo3),
                                                                                   (msg.mudp_frame_header.streamRefIndex),
                                                                                   hex(msg.mudp_frame_header.streamDataLen),
                                                                                   hex(msg.mudp_frame_header.streamTxCnt),
                                                                                   hex(msg.mudp_frame_header.streamNumber),
                                                                                   hex(msg.mudp_frame_header.streamVersion),
                                                                                   hex(msg.mudp_frame_header.streamChunksPerCycle),
                                                                                   hex(msg.mudp_frame_header.streamChunks),
                                                                                   hex(msg.mudp_frame_header.streamChunkIdx),
                                                                                   hex(msg.mudp_frame_header.reserved),
                                                                                   hex(msg.mudp_frame_header.sensorId)))
                    fh.write("\n")


            fh.close()
            latch_status = "Done"
            tracker_object = None
            vehicle_info = None
            latch_map = {}
            vehicle_info = None

