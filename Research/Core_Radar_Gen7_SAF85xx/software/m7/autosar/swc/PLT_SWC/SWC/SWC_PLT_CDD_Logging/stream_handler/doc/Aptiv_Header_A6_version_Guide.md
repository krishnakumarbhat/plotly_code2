# APTIV HEADER

[TOC]

-------------------------

## 1. Introduction

APTIV Header is introduced to identify what information is present in each frame such as valid total payload Data (chunkLength), particular module information (stream number), etc.

APTIV Header A6 version will be common usability for Radar, Camera, Lidar, Ultrasonics, OSPAS, Racam, etc.

-------------------------

## 2. APTIV Header version A6 Structure

<html>
<table>
    <thead>
        <tr>
            <th>Bit Position</th>
            <th width="70px">0-3</th>
            <th width="70px">4-7</th>
            <th width="70px">8-11</th>
            <th width="70px">12-15</th>
            <th width="70px">16-19</th>
            <th width="70px">20-23</th>
            <th width="70px">24-27</th>
            <th width="70px">28-31</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <th>Byte-0</th>
            <td colspan="4"><a href="#31-versioninfo" class="internal-link"><center>versionInfo</center></td>
            <td colspan="4"><a href="#32-sourcetxcnt" class="internal-link"><center>sourceTxCnt</center></td>
        </tr>
        <tr>
            <th>Byte-4</th>
            <td colspan="4"><a href="#33-chunklength" class="internal-link"><center>chunkLength</center></td>
            <td colspan="2"><a href="#34-chunkcount" class="internal-link"><center>chunkCount</center></td>
            <td colspan="2"><a href="#35-chunkindex" class="internal-link"><center>chunkIndex</center></td>
        </tr>
        <tr>
            <th>Byte-8</th>
            <td colspan="3"><a href="#36-domainid" class="internal-link"><center>domainId</center></td>
            <td colspan="1"><a href="#37-domaininst" class="internal-link"><center>domainInst</center></td>
            <td colspan="3"><a href="#38-componentid" class="internal-link"><center>componentId</center></td>
            <td colspan="1"><a href="#39-componentinst" class="internal-link"><center>componentInst</center></td>
        </tr>
        <tr>
            <th>Byte-12</th>
            <td colspan="8"><a href="#310-streamrefindex" class="internal-link"><center>streamRefIndex</center></td>
        </tr>
        <tr>
            <th>Byte-16</th>
            <td colspan="8"><a href="#311-streamtxtime" class="internal-link"><center>streamTxTime</center></td>
        </tr>
        <tr>
            <th>Byte-20</th>
            <td colspan="2"><a href="#312-streamtxcnt" class="internal-link"><center>streamTxCnt</center></td>
            <td colspan="2"><a href="#313-streamnumber" class="internal-link"><center>streamNumber</center></td>
            <td colspan="2"><a href="#314-streamversion" class="internal-link"><center>streamVersion</center></td>
            <td colspan="2"><a href="#315-serializationtype" class="internal-link"><center>serializationType</center></td>
        </tr>
    </tbody>
</table>
</html>

Refer **Rec_Hdr_T** in the file **stream_header.h** for more details with order of variables/members.
More info on [Aptiv Header](https://spo.aptiv.com/:x:/r/sites/0304-AdvEngSystems/AE_Software/Radar/NextGenRadar_WP_CI/SWE.2%20Software%20Architectural%20Design/GEN7/APTIV_Data_Header.xlsx?d=w4a9af0e9640646e29856b6e20e84fc98&csf=1&web=1&e=p0FYXN)

-----------------------------------------------------------

## 3. APTIV Header A6 Structure Description

### 3.1 versionInfo

It describes the version of this header, notifying the receiver how to parse the remainder of the header. It is made up of two bytes. The first byte is a header version identifier and has the format 0xAn, where n indicates the header format version (e.g. 0xA6 for header format version 6). The second byte contains the constant length of the header, in bytes.  The versionInfo value for this header is 0xA618 (version 6 is 24 bytes long).
By storing the versionInfo signal's two unique bytes as a short (16 bit word) of known/constant content, it can be used to determine if the data was transmitted in little-endian or in big-endian byte order.

### 3.2 sourceTxCnt

sourceTxCnt is the transmit counter for a given transmitter application.  Typically each ECU/sensor has a transmit thread that is handling data transmission from multiple components.  Each time a message is transmitted by a particular sender, this count shall be incremented by one, regardless of the stream type/originator or if it is a chunk of a multi-chunk stream.  This counter allows us to confirm that there were no missed messages between the transmit app and the receiver/logger. The streamTxCnt signal will help to identify any losses in the rest of the pipeline, from sending component to transmit app.

### 3.3 chunkLength

chunkLength defines the number of bytes in the current message’s payload, not including the APTIV header. This is not the reconstructed size of the de-chunked payload (which can be calculated as [(chunkLength*(chunkCount-1))+(chunkLength of last_chunk)] ).

One allowed exception will be for CDC data where, we set chunkCount=maxDataCubeCols and, if N chunks transmitted turns out to be less than chunkCount, send one more chunk with chunkLength=0, chunkIndex=N+1, and all other header values as for Nth chunk.

### 3.4 chunkCount

A Stream/message is segmented into number of chunks/messages and the chunkCount represents the total number of chunks/messages.


### 3.5 chunkIndex

chunkIndex is index which is informing the sequence of chunks of particular message/stream.

**The usage of combination of [chunkCount](#35-chunkcount) and [chunkIndex](#36-chunkindex):**
chunkCount and chunkIndex are used together to break a single large message into many smaller messages. This might be done for many reasons, including hardware limitations or bandwidth optimization.
If the entire payload is transmitted in a single message, chunkCount and chunkIndex shall both be set to zero. If the payload is broken into multiple messages, chunkCount shall be set to the number of individual messages, and chunkIndex shall specify the index (starting at zero) of the current message within the sequence. The size of each chunk is determined by chunkLength.

### 3.6 domainId

It is number that uniquely identifies the sending system type. This value corresponds to a sensor or ecu flavor, (e.g. Radar, camera, Lidar, OSPAS, etc.) and it contain which sub domain information for radar such as SRR(Short Range Radar), FLR(Forward Looking Radar),IMR(Imaging Radar),ICR(InCabin Radar) etc., similarly for camera/Lidar/OSPAS etc.

### 3.7 domainInst

It is used to identify the source of messages when there are multiple sensors of the same type transmitting the same messages.
For example, if there are four radars, the messages from one sensor would have domainInstance = 0, the second would have domainInstance = 1, etc.

### 3.8 componentId

Component has a unique identifier of a system component like VSE, ROT, Fusion, radar-ML, AngleFinding or 2nd-pass proc, etc. When one sensor/ecu has multiple copies of a Component running, those will all share one componentId.
For example, Alignment/DynamicAlignment/RadarCapability/ID streams are categorized under Radar_Capability module, so componentId will be considered as 0xF4(Radar_Capability) and Radar_Capability module owners have a freedom to decide/choose the streamNumber.

### 3.9 componentInst

If in same domain/ECU, two or more copy of same stream is running then componentInstance will change from 0 to respective copies of stream.
For example, if we divided RDD stream in two copies such RDD1 and RDD2 stream, then componentInstance=0 for RDD1 stream and componentInstance=1 for RDD2 stream.

### 3.10 streamRefIndex

streamRefIndex is a component-defined index used to associate produced data to a particular cycle that produced it. One component may have more than one streamRefIndex if it has more than one worker thread (or logical worker thread).
For example, if streamNumber A and streamNumber B contain data calculated from component cycle index 12345, streamRefIndex for both messages shall be set to 12345. However, if a component has a worker thread to slowly send out calibrations, then the streamRefIndex should increment once for each full set of transmissions (all N messages to send one set of cals will share one ref index and the chunk info used to define the chunks). The meaning of streamRefIndex is stream-specific (it may be a look index for a radar cdc and detections, a frame number for vision objects and lanes, or a cycle counter for other data). The key here is uniqueness and data association (e.g. cdc and detections are tied by look index, but cals are not specific to a look).

### 3.11 streamTxTime

streamTxTime is a millisecond timestamp assigned by the Component when the data is ready and trigger/passed to StreamHandler module to transmit.

### 3.12 streamTxCnt

streamTxCnt is the transmit counter for a given stream. Each time a message is transmitted for a given stream (or streamNumber), this could shall be incremented by one, regardless of the contents of the message payload. Each stream transmission source shall maintain a unique counter for this signal.

### 3.13 streamNumber

streamNumber identifies the contents of the message payload and shall be in the range [1..255].  These values are managed by the Component owner, with the exception of a few reserved values that shall be used by all Components to communicate certain required information.  Debug data should reside in separate steam numbers from mission-critical data used in ACF and RTE between components.  Ideally, variants do not generate new stream numbers.

### 3.14 streamVersion

streamVersion identifies the structure or format of the message payload for a given stream (or streamNumber) and shall be in the range [0..255].
For example, if a component is transmitting radar detections on streamNumber 15, then the first version would start at 0 and increment with every structural or content change.  Non-debug versions are to be managed by an interface control board.

### 3.15 serializationType

This is for future identification of deserialization/serialization method (0=none).

----------------------------------------

## 4. File Revision History

| Rev | Date | NetId | Name | SCR |
|---|---|---|---|---|
| 0.1 | 4-MAR-2025 | sjwpbv | Maulik | DNP-5914 |
