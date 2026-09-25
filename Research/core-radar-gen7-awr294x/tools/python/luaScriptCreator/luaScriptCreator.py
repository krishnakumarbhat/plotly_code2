"""
Parse a streamdef folder, select the newest version, and create a lua disector.

This Python module performs the following:
- parses the stream def folder and picks the newest version of each stream
- Creates a wiresharek dissector that supports all of these new streams

Inputs:
- Path to the stream def files
- Path to the version.c file
- Path to save the output lua script

Notes:
- Arrays in stream are not represented in the lua scripts
"""
import os
from os.path import exists
from dataclasses import dataclass
import datetime
import argparse
import re
from functools import reduce

# Used to flip the reading order of the stream def files, big endian vs little endian.
variableOrder = 1

# Destination Ports, 5555 for most streams, 5556 for stream 6 (cdc)
DestinationPorts = [5555, 5556, 5558]


@dataclass
class _streamVar:
    _type: str
    _name: str
    _longName: str = ""
    _unit: str = ""
    _size: int = 0
    _offset: int = 0
    _arraySize: int = 1


class Lua_Script_Creator:
    """Read in all the stream defs and create a dissector for Wiresharks."""

    def __init__(self, streamDefPath, versionFile, outputLuaScriptPath, bigEndian=False):
        """
        Create a lua script creator object and setup the standard type sizes.

        Args:
            streamDefPath: Path to the streamdef file
            versionFile: Path to the version information file
            outputLuaScriptPath: Path to save the output Lua script
            bigEndian: Default: False (Little Endian) - Set to True for Big Endian
        """
        self.streamDefFiles = streamDefPath
        self.streamDefPath = os.path.dirname(streamDefPath[0])

        self.versionFile = versionFile
        self.bigEndian = bigEndian
        self.sourceIDs = {
            204: "srr7p",
            205: "srr7hd",
            206: "flr7",
            72: "srr7p",
            73: "flr7",
            75: "srr7hd",
        }
        self.variant = ""
        self.sourceID = ""

        self.outputLuaScriptPath = outputLuaScriptPath
        self.streams = []

        if self.outputLuaScriptPath == "":
            self.outputLuaScriptPath = self.streamDefPath

        self.chunkSizes = {"A1": 1444, "A5": 1444}
        self.skipArrays = True

        # These streams are ignored and not added to the lua scripts
        self.ignoreStreamList = [14]

        # this is a lookup table for variable types.
        # If new types are added in the source code, they will also need to be added here.
        self.variableTypes = {
            "uint32_t": ["uint32", 4],
            "uint16_t": ["uint16", 2],
            "uint8_t": ["uint8", 1],
            "u8p0_t": ["uint8", 1],
            "u1p7_t": ["uint8", 1],
            "u2p6_t": ["uint8", 1],
            "u0p8_t": ["uint8", 1],
            "sint32_t": ["sint32", 4],
            "sint16_t": ["int16", 2],
            "sint8_t": ["int8", 1],
            "int16_t": ["int16", 2],
            "u16p0_t": ["uint16", 2],
            "int32_t": ["int32", 4],
            "u9p7_t": ["uint16", 2],
            "s7p8_t": ["int16", 2],
            "s2p13_t": ["int16", 2],
            "s7p0_t": ["int8", 1],
            "s2p5_t": ["int8", 1],
            "u16p16_t": ["uint32", 4],
            "s10p21_t": ["int32", 4],
            "s11p20_t": ["int32", 4],
            "s4p27_t": ["int32", 4],
            "s8p23_t": ["int32", 4],
            "s15p0_t": ["int16", 2],
            "uint64_t": ["uint64", 8],
            "boolean_t": ["bool", 1],
            "bitfield32_T": ["uint32", 4],
            "u1p15_t": ["uint16", 2],
            "u32p0_t": ["uint32", 4],
            "s8p7_t": ["int16", 2],
            "u20p12_t": ["uint32", 4],
            "u25p7_t": ["uint32", 4],
            "u0p16_t": ["uint16", 2],
            "u7p1_t": ["uint8", 1],
            "u8p24_t": ["uint32", 4],
            "s31p0_t": ["int32", 4],
            "s0p31_t": ["int32", 4],
            "u12p20_t": ["uint32", 4],
            "int8_t": ["int8", 1],
            "s0p15_t": ["int16", 2],
            "s18p13_t": ["int32", 4],
            "u21p11_t": ["uint32", 4],
            "u12p4_t": ["uint16", 2],
            "u14p2_t": ["uint16", 2],
            "u3p13_t": ["uint16", 2],
            "u4p12_t": ["uint16", 2],
            "u6p10_t": ["uint16", 2],
            "float64_t": ["float", 8],
            "float32_t": ["float", 4],
            "u6p2_t": ["uint8", 1],
            "u8p8_t": ["uint16", 2],
            "s15p16_t": ["int32", 4],
            "u4p4_t": ["uint8", 1],
            "s1p14_t": ["int16", 2],
            "u7p9_t": ["uint16", 2],
            "s6p9_t": ["int16", 2],
            "s3p4_t": ["int8", 1],
            "u2p14_t": ["uint16", 2],
            "s20p11_t": ["int32", 4],
            "s9p6_t": ["int16", 2],
            "s10p5_t": ["int16", 2],
            "s5p2_t": ["int8", 1],
            "u15p1_t": ["uint16", 2],
            "s6p1_t": ["int8", 1],
        }

        # These variables are ignored and not aded to the lua scripts
        self.ignoreVariableList = []
        # Verify stream def folder exists

        if not os.path.isdir(self.streamDefPath):
            print("ERROR - Path does not exist:\n    {0}".format(self.streamDefPath))
            exit(1)

        self.stream_file_list = self.streamDefFiles
        # set to uint or le_unit
        if self.bigEndian is True:
            self.ENDIAN_FORMAT = "uint"
        else:
            self.ENDIAN_FORMAT = "le_uint"

        self.__readStreamFolder(self.stream_file_list)

    def __createLuaScript(self, streams, variant, luaScript):
        """Crate the lua script after pull all the correct streams and info from the stream files."""
        with open(luaScript, "w") as f:

            now = datetime.datetime.now()

            f.write(
                "------------------------------------------------------------------------------\n"
            )
            f.write("-- File       : wiresharkDissector.lua\n")
            f.write("-- Authors    : Fred Grandlienard\n")
            f.write("-- Company    : Aptiv\n")
            f.write("-- Date       : {0}-{1}-{2}\n".format(now.month, now.day, now.year))
            f.write(
                "------------------------------------------------------------------------------\n"
            )
            f.write("-- Description:\n")
            f.write("--\n")
            f.write("-- Dissector for Aptiv's UDP protocol header and data\n")
            f.write("--\n")
            f.write("--    VARIANT: {0}\n".format(variant))
            f.write(
                "-- SW VERSION: {0}_v{1}\n".format(variant, self.__getSWVersion(self.versionFile))
            )
            f.write("--\n")

            template = "-- {0:^10}|{1:^8}|{2:^9}\n"  # column widths: 8, 10, 15
            f.write(template.format("SOURCE ID", "STREAM", "VERSION"))  # header

            # Create a list of (key, value) tuples from the streams dictionary and sort by the 'stream' key
            sorted_streams = sorted(streams.items(), key=lambda kv: kv[1]["stream"])

            for _key, value in sorted_streams:
                f.write(template.format(value["source"], value["stream"], value["version"]))

            f.write(
                "------------------------------------------------------------------------------\n"
            )
            f.write("\n")

            f.write("do\n")
            f.write("\t---------------------------------------\n")
            f.write("\t-- Declare the protocol ---------------\n")
            f.write("\t---------------------------------------\n")
            f.write('\taptivudp = Proto("aptivudp", "Aptiv UDP Stream")\n')
            f.write("\n")
            f.write("\t---------------------------------------\n")
            f.write("\t-- Declare header fields --------------\n")
            f.write("\t---------------------------------------\n")
            f.write(
                '\tfieldSourceInfo      = ProtoField.uint8("aptivudp.sourceInfo", "sourceInfo")\n'
            )
            f.write(
                '\tfieldSourceTxCnt     = ProtoField.uint16("aptivudp.sourceTxCnt", "sourceTxCnt")\n'
            )
            f.write(
                '\tfieldSourceTxTime    = ProtoField.uint32("aptivudp.sourceTxTime", "sourceTxTime")\n'
            )
            f.write(
                '\tfieldStreamNumber    = ProtoField.uint8("aptivudp.streamNumber", "streamNumber")\n'
            )
            f.write(
                '\tfieldStreamVersion   = ProtoField.uint8("aptivudp.streamVersion", "streamVersion")\n'
            )
            f.write(
                '\tfieldStreamTxCnt     = ProtoField.uint8("aptivudp.streamTxCnt", "streamTxCnt")\n'
            )
            f.write(
                '\tfieldStreamRefIndex  = ProtoField.uint32("aptivudp.streamRefIndex", "streamRefIndex")\n'
            )
            f.write(
                '\tfieldStreamChunks    = ProtoField.uint16("aptivudp.streamChunks", "streamChunks")\n'
            )
            f.write(
                '\tfieldStreamChunkIdx  = ProtoField.uint16("aptivudp.streamChunkIdx", "streamChunkIdx")\n'
            )
            f.write(
                '\tfieldStreamChunksPerCycle    = ProtoField.uint8("aptivudp.streamChunksPerCycle", "streamChunksPerCycle")\n'
            )
            f.write('\tfieldSensorId        = ProtoField.uint8("aptivudp.sensorId", "sensorId")\n')
            f.write(
                '\tfieldCustomerId      = ProtoField.uint8("aptivudp.customerId", "customerId")\n'
            )
            f.write(
                '\tfieldSourceInfo1     = ProtoField.uint8("aptivudp.sourceInfo1", "sourceInfo1")\n'
            )
            f.write(
                '\tfieldSourceInfo2     = ProtoField.uint8("aptivudp.sourceInfo2", "sourceInfo2")\n'
            )
            f.write(
                '\tfieldSourceInfo3     = ProtoField.uint8("aptivudp.sourceInfo3", "sourceInfo3")\n'
            )
            f.write("\n")

            # Create the field variables for each stream+variables
            for key, value in sorted_streams:
                f.write("\t---------------------------------------\n")
                f.write("\t-- Stream {0} ---------------------------\n".format(key))
                f.write("\t---------------------------------------\n")

                for variable in value["variables"][::variableOrder]:
                    if variable._name not in self.ignoreVariableList:

                        varStruct = self.__return_class_variables(variable)
                        _name = varStruct["_name"]
                        _type = varStruct["_type"]
                        f.write(
                            '\tfields{0}_{1}      = ProtoField.{2}("aptivudp.s{3}_{4}", "s{5}_{6}")\n'.format(
                                key, _name.replace(".", "_"), _type, key, _name, key, _name
                            )
                        )

            # Create field structure
            f.write("\t---------------------------------------\n")
            f.write("\t-- Fields Structure -------------------\n")
            f.write("\t---------------------------------------\n")

            f.write("\taptivudp.fields = {\n")
            f.write("\t\t-- header -- \n")
            f.write(
                "\t\tfieldSourceInfo, fieldSourceTxCnt, fieldSourceTxTime, fieldStreamNumber, \n"
            )
            f.write(
                "\t\tfieldStreamVersion, fieldStreamTxCnt, fieldStreamRefIndex, fieldSensorId, fieldCustomerId,\n"
            )
            f.write("\t\tfieldStreamChunkIdx,fieldStreamChunks,fieldStreamChunksPerCycle,\n")

            for index, (key, value) in enumerate(sorted_streams):
                f.write("\t\t-- Stream {0} --	\n".format(key))
                cnt = 0
                lineStr = "\t\t"

                for variable in value["variables"][::variableOrder]:
                    if variable._name not in self.ignoreVariableList:

                        varStruct = self.__return_class_variables(variable)
                        _name = varStruct["_name"]
                        _type = varStruct["_type"]
                        _field = "fields{0}_{1}".format(key, _name.replace(".", "_"))
                        lineStr += _field + ", "
                        cnt += 1
                        if cnt >= 10:
                            lineStr += "\n\t\t"
                            cnt = 0
                if cnt < 10:
                    lineStr += "\n"

                # Check if this is the last item in sorted_streams
                if index == len(sorted_streams) - 1:
                    lineStr = lineStr.rstrip(", \n") + "}\n"
                f.write(lineStr)

            f.write(
                """\n
        -- Define the dissector.
        function aptivudp.dissector(tvbuffer, pinfo, treeitem)

            -- Determine the endianness and header version.
            local temp1, temp2, isBigEndian, headerVersion, headerSize, streamNumber, streamChunkIdx
            temp1 = tvbuffer(0,1):uint()
            temp2 = tvbuffer(1,1):uint()

            if (temp1 >= 0xA1) and (temp1 <= 0xAF) then -- Big endian data.
                isBigEndian = 1
                headerVersion = temp1
                headerSize = temp2
            elseif (temp2 >= 0xA1) and (temp2 <= 0xAF) then -- Little endian data.
                isBigEndian = 0
                headerSize = tvbuffer(0,1):uint()
                headerVersion = tvbuffer(1,1):uint()
            else -- Invalid Aptiv header.
                headerVersion = 0
            end


            if headerVersion ~= 0 then -- Valid Aptiv header.

                streamNumber   = tvbuffer(19,1):le_uint()
                \n"""
            )
            first = 0
            for key, _value in sorted_streams:
                if first == 0:
                    f.write(
                        '\t\t\tif  streamNumber == {0} then pinfo.cols.protocol = "Aptiv UDP Stream {1}"\n'.format(
                            key, key
                        )
                    )
                    first = 1
                else:
                    f.write(
                        '\t\t\telseif  streamNumber == {0} then pinfo.cols.protocol = "Aptiv UDP Stream {1}"\n'.format(
                            key, key
                        )
                    )

            f.write("\t\t\tend\n")

            f.write(
                """\n\t\t\t-- Calculate signals with size > 1.
                local sourceTxCnt, sourceTxTime, streamRefIndex, streamDataLength
                if isBigEndian ~= 0 then
                    sourceTxCnt = tvbuffer(2,2):uint()
                    sourceTxTime = tvbuffer(4,4):uint()
                    streamRefIndex = tvbuffer(12,4):uint()
                    streamDataLength = tvbuffer(16,2):uint()
                else
                    sourceTxCnt = tvbuffer(2,2):le_uint()
                    sourceTxTime = tvbuffer(4,4):le_uint()
                    streamRefIndex = tvbuffer(12,4):le_uint()
                    streamDataLength = tvbuffer(16,2):le_uint()
                end

                -- Calculate remaining signals with size == 1.
                local sourceInfo, streamTxCnt, streamVersion, streamChunks, sensorId, customerId


                sourceInfo     = tvbuffer(8,1):le_uint()
                streamTxCnt    = tvbuffer(18,1):le_uint()
                streamVersion  = tvbuffer(20,1):le_uint()
                streamChunks   = tvbuffer(21,1):le_uint()
                streamChunkIdx = tvbuffer(22,1):le_uint()

                if headerVersion == 0xA5 then
                    streamChunksPerCycle = tvbuffer(21,1):le_uint()

                    if isBigEndian ~= 0 then
                        streamChunks = tvbuffer(22,2):uint()
                        streamChunkIdx = tvbuffer(24,2):uint()
                        sensorId = tvbuffer(27,1):uint()
                    else
                        streamChunks = tvbuffer(22,2):le_uint()
                        streamChunkIdx = tvbuffer(24,2):le_uint()
                        sensorId = tvbuffer(27,1):le_uint()
                    end
                    customerId = 22
                elseif headerVersion >= 0xA2 then
                    streamChunksPerCycle = 0
                    sensorId = tvbuffer(9,1):le_uint()
                    customerId = tvbuffer(23,1):le_uint()
                    streamChunks   = tvbuffer(21,1):le_uint()
                    streamChunkIdx = tvbuffer(22,1):le_uint()
                else -- headerVersion == 0xA1
                    streamChunksPerCycle = 0
                    sensorId = tvbuffer(23,1):le_uint()
                    customerId = 0
                end\n"""
            )
            f.write("\t\t\t---------------------------------------\n")
            f.write("\t\t\t-- Populate Variables -----------------\n")
            f.write("\t\t\t---------------------------------------\n")

            headercntr = 0
            for header in self.chunkSizes:

                _chunkSize = self.chunkSizes[header]

                if headercntr == 0:
                    f.write("\t\tif headerVersion == 0x{0} then\n".format(header))
                    headercntr = 1
                else:
                    f.write("\t\telseif headerVersion == 0x{0} then\n".format(header))
                elseIfFlag = 0
                for key, value in sorted_streams:
                    if elseIfFlag == 0:
                        f.write(
                            "\t\t\tif streamNumber == {0} and streamChunkIdx == 0 then\n".format(
                                key
                            )
                        )
                        elseIfFlag = 1
                    else:
                        f.write(
                            "\n\t\t\telseif streamNumber == {0} and streamChunkIdx == 0 then\n".format(
                                key
                            )
                        )

                    first = 1
                    sizeCntr = 0
                    chunkoffsetCnt = 0
                    chunkCntr = 0

                    for variable in value["variables"][::variableOrder]:
                        if variable._name not in self.ignoreVariableList:
                            varStruct = self.__return_class_variables(variable)

                            _type = varStruct["_type"]
                            _name = varStruct["_name"]
                            _unit = varStruct["_unit"]
                            _size = varStruct["_size"]
                            _arraySize = varStruct["_arraySize"]

                            if (chunkoffsetCnt + _size) > _chunkSize:
                                chunkCntr += 1

                                chunkoffsetCnt = 0
                                f.write(
                                    "\n\t\t\telseif streamNumber == {0} and streamChunkIdx == {1} then\n".format(
                                        key, chunkCntr
                                    )
                                )

                            if _arraySize == 1:
                                if _unit == "float32_t":
                                    f.write(
                                        "\t\t\t\ts{0}_{1} = hex2float(tvbuffer(headerSize+{2},{3}):{4}())\n".format(
                                            key, _name, chunkoffsetCnt, _size, self.ENDIAN_FORMAT
                                        )
                                    )
                                else:
                                    if (
                                        _size >= 8
                                    ):  # added 64 to the endian format, the le_uint64() is for 64bit numbers
                                        f.write(
                                            "\t\t\t\ts{0}_{1} = tvbuffer(headerSize+{2},{3}):{4}64()\n".format(
                                                key,
                                                _name,
                                                chunkoffsetCnt,
                                                _size,
                                                self.ENDIAN_FORMAT,
                                            )
                                        )
                                    else:
                                        f.write(
                                            "\t\t\t\ts{0}_{1} = tvbuffer(headerSize+{2},{3}):{4}()\n".format(
                                                key,
                                                _name,
                                                chunkoffsetCnt,
                                                _size,
                                                self.ENDIAN_FORMAT,
                                            )
                                        )
                                chunkoffsetCnt += _size
                                sizeCntr += _size
                            else:
                                for index in range(0, _arraySize):

                                    if (chunkoffsetCnt + _size) > _chunkSize:
                                        chunkCntr += 1
                                        f.write(
                                            "\n\t\t\telseif streamNumber == {0} and streamChunkIdx == {1} then\n".format(
                                                key, chunkCntr
                                            )
                                        )

                                        chunkoffsetCnt = 0
                                        if self.skipArrays is False:
                                            if _unit == "float32_t":
                                                f.write(
                                                    "\t\t\t\ts{0}_{1}[{2}] = hex2float(tvbuffer(headerSize+{3},{4}):{5}())\n".format(
                                                        key,
                                                        _name,
                                                        index,
                                                        chunkoffsetCnt,
                                                        _size,
                                                        self.ENDIAN_FORMAT,
                                                    )
                                                )
                                            else:
                                                f.write(
                                                    "\t\t\t\ts{0}_{1}[{2}] = tvbuffer(headerSize+{3},{4}):{5}()\n".format(
                                                        key,
                                                        _name,
                                                        index,
                                                        chunkoffsetCnt,
                                                        _size,
                                                        self.ENDIAN_FORMAT,
                                                    )
                                                )
                                        chunkoffsetCnt += _size
                                        sizeCntr += _size
                                    else:
                                        if self.skipArrays is False:
                                            if _unit == "float32_t":
                                                f.write(
                                                    "\t\t\t\ts{0}_{1}[{2}] = hex2float(tvbuffer(headerSize+{3},{4}):{5}())\n".format(
                                                        key,
                                                        _name,
                                                        index,
                                                        chunkoffsetCnt,
                                                        _size,
                                                        self.ENDIAN_FORMAT,
                                                    )
                                                )
                                            else:
                                                f.write(
                                                    "\t\t\t\ts{0}_{1}[{2}] = tvbuffer(headerSize+{3},{4}):{5}()\n".format(
                                                        key,
                                                        _name,
                                                        index,
                                                        chunkoffsetCnt,
                                                        _size,
                                                        self.ENDIAN_FORMAT,
                                                    )
                                                )
                                        chunkoffsetCnt += _size
                                        sizeCntr += _size

                f.write("\n\t\t\tend\n")

            f.write("\n\t\tend")
            f.write("\n\n")
            f.write(
                """\n
            -- Generate the summary information.
            chunkSummary = ""
            summary = "Src: " .. getVariantString(sourceInfo) .. " (Cnt: " .. sourceTxCnt .. ")"
            if sensorId > 0 then
                summary = summary .. ", Sens: " .. sensorId
            end
            summary = summary .. ", Str: " .. streamNumber .. " (Cnt: " .. streamTxCnt .. "), Ver: " .. streamVersion
            if streamChunks > 1 then
                chunkSummary = " (Chunk " .. streamChunkIdx+1 .. " of " .. streamChunks .. ")"
                summary = summary .. chunkSummary
            end\n\n"""
            )

            f.write("\t\t----------------------------------------\n")
            f.write("\t\t-- Add Subtree info for wireshark GUI --\n")
            f.write("\t\t----------------------------------------\n")

            f.write(
                """\n
            -- Add UDP Source Information
            subtree = treeitem:add(aptivudp, tvbuffer(0,31), "Aptiv UDP Header, " .. summary)
            subtree:add(tvbuffer(0,2), "endianness: " .. endiannessString(isBigEndian))
            subtree:add(tvbuffer(0,2), "versionInfo: " .. "Version: 0x" .. num2hex(headerVersion) .. ", Size: " .. headerSize)
            subtree:add(fieldSourceTxCnt, tvbuffer(2,2), sourceTxCnt)
            subtree:add(fieldSourceTxTime, tvbuffer(4,4), sourceTxTime)
            subtree:add(fieldSourceInfo, tvbuffer(8,1), sourceInfo)

            if headerVersion == 0xA5 then
                subtree:add(fieldSensorId, tvbuffer(27,1), sensorId)
                subtree:add(tvbuffer(9,3), "reserved: " .. tvbuffer(9,1):uint() .. " " ..
                tvbuffer(10,1):uint() .. " " .. tvbuffer(11,1):uint())
            elseif headerVersion >= 0xA2 then
                subtree:add(fieldSensorId, tvbuffer(9,1), sensorId)
                subtree:add(tvbuffer(10,2),"reserved: " .. tvbuffer(10,1):uint() .. " " ..
                tvbuffer(11,1):uint())
            else -- headerVersion <= 0xA1
                subtree:add(tvbuffer(9,3), "reserved: " .. tvbuffer(9,1):uint() .. " " ..
                tvbuffer(10,1):uint() .. " " .. tvbuffer(11,1):uint())
            end

            -- Add UDP Stream Information


            subtree:add(fieldStreamRefIndex, tvbuffer(12,4), streamRefIndex)
            subtree:add(tvbuffer(16,2), "streamDataLength: " .. streamDataLength)
            subtree:add(fieldStreamTxCnt,    tvbuffer(18,1), streamTxCnt)
            subtree:add(fieldStreamNumber,   tvbuffer(19,1), streamNumber)
            subtree:add(fieldStreamVersion,  tvbuffer(20,1), streamVersion)


            if headerVersion >= 0xA5 then
                subtree:add(fieldStreamChunksPerCycle,   tvbuffer(21,1), streamChunksPerCycle)
                subtree:add(fieldStreamChunks,   tvbuffer(22,2), streamChunks)
                subtree:add(fieldStreamChunkIdx, tvbuffer(24,2), streamChunkIdx)
                subtree:add(fieldSensorId, tvbuffer(27,1), sensorId)
            elseif headerVersion >= 0xA2 then
                subtree:add(fieldStreamChunks,   tvbuffer(21,1), streamChunks)
                subtree:add(fieldStreamChunkIdx, tvbuffer(22,1), streamChunkIdx)

                subtree:add(fieldCustomerId, tvbuffer(23,1), customerId, "customerId: " ..
                getCustomerString(sourceInfo,customerId) .. " (" .. customerId .. ")")
            else -- headerVersion <= 0xA1
                subtree:add(fieldStreamChunks,   tvbuffer(21,1), streamChunks)
                subtree:add(fieldStreamChunkIdx, tvbuffer(22,1), streamChunkIdx)

                subtree:add(fieldSensorId, tvbuffer(23,1), sensorId)
            end\n"""
            )

            f.write('\n\t\tsubtree:add(tvbuffer(0,2), "-------Aptiv Data--------------------")	\n')

            f.write("\t\t---------------------------------------\n")
            f.write("\t\t-- Print Stream Data ------------------\n")
            f.write("\t\t---------------------------------------\n")

            headercntr = 0
            for header in self.chunkSizes:
                _chunkSize = self.chunkSizes[header]
                if headercntr == 0:
                    f.write("\t\tif headerVersion == 0x{0} then\n".format(header))
                    headercntr = 1
                else:
                    f.write("\t\telseif headerVersion == 0x{0} then\n".format(header))
                elseIfFlag = 0

                for key, value in sorted_streams:
                    if elseIfFlag == 0:
                        f.write(
                            "\t\t\tif streamNumber == {0} and streamChunkIdx == 0 then\n".format(
                                key
                            )
                        )
                        elseIfFlag = 1
                    else:
                        f.write(
                            "\n\t\t\telseif streamNumber == {0} and streamChunkIdx == 0 then\n".format(
                                key
                            )
                        )

                    first = 1
                    sizeCntr = 0
                    chunkoffsetCnt = 0
                    chunkCntr = 0

                    for variable in value["variables"][::variableOrder]:
                        if variable._name not in self.ignoreVariableList:
                            varStruct = self.__return_class_variables(variable)
                            _type = varStruct["_type"]
                            _name = varStruct["_name"]
                            _size = varStruct["_size"]
                            _arraySize = varStruct["_arraySize"]

                            # These items are added to the "Info" column and are used in the testing script
                            if (
                                "stream_hdr_scan_index" not in _name
                                and "ML_ScanIndex" not in _name
                                and "debug_scanindex" not in _name
                            ) and ("scanindex" in _name or "scan_index" in _name):
                                f.write(
                                    '\t\t\t\tsummary = summary .. ", ScanIndex: " .. s{0}_{1}\n'.format(
                                        key, _name
                                    )
                                )
                            elif "stream_hdr_scan_index" in _name:
                                f.write(
                                    '\t\t\t\tsummary = summary .. ", ScanIndexHdr: " .. s{0}_{1}\n'.format(
                                        key, _name
                                    )
                                )

                            if "lookindex" in _name or "look_index" in _name:
                                f.write(
                                    '\t\t\t\tsummary = summary .. ", LookIndex: " .. s{0}_{1}\n'.format(
                                        key, _name
                                    )
                                )
                            if "rbin_res" in _name:
                                f.write(
                                    '\t\t\t\tsummary = summary .. ", rbinRes: " .. s{0}_{1}\n'.format(
                                        key, _name
                                    )
                                )
                            if "dbin_res" in _name:
                                f.write(
                                    '\t\t\t\tsummary = summary .. ", dbinRes: " .. s{0}_{1}\n'.format(
                                        key, _name
                                    )
                                )
                            if (
                                "targetcount" in _name
                                or "target_count" in _name
                                or "num_af_det" in _name
                            ):
                                f.write(
                                    '\t\t\t\tsummary = summary .. ", TargetCnt: " .. s{0}_{1}\n'.format(
                                        key, _name
                                    )
                                )
                            if "sensortimestampsec" in _name or "sensor_timestamp_sec" in _name:
                                f.write(
                                    '\t\t\t\tsummary = summary .. ", TSsec: " .. s{0}_{1}\n'.format(
                                        key, _name
                                    )
                                )
                            if "sensortimestampns" in _name or "sensor_timestamp_ns" in _name:
                                f.write(
                                    '\t\t\t\tsummary = summary .. ", TSnsec: " .. s{0}_{1}\n'.format(
                                        key, _name
                                    )
                                )
                            if (
                                ("look_id" in _name and "look_id_err_cnt" not in _name)
                                or "lookid" in _name
                                or "look_type" in _name
                            ):
                                f.write(
                                    '\t\t\t\tsummary = summary .. ", LookId: " .. s{0}_{1}\n'.format(
                                        key, _name
                                    )
                                )
                            if "num_fp_detections" in _name:
                                f.write(
                                    '\t\t\t\tsummary = summary .. ", num_fp_detections: " .. s{0}_{1}\n'.format(
                                        key, _name
                                    )
                                )
                            if "num_sp_detections" in _name:
                                f.write(
                                    '\t\t\t\tsummary = summary .. ", num_sp_detections: " .. s{0}_{1}\n'.format(
                                        key, _name
                                    )
                                )

                            if "radar_post_daq_count" in _name:
                                f.write(
                                    '\t\t\t\tsummary = summary .. ", RadarPostDaqCnt: " .. s{0}_{1}\n'.format(
                                        key, _name
                                    )
                                )
                            if "mode_trig_count" in _name:
                                f.write(
                                    '\t\t\t\tsummary = summary .. ", ModeTrigCnt: " .. s{0}_{1}\n'.format(
                                        key, _name
                                    )
                                )
                            if "radar_post_rdd_count" in _name:
                                f.write(
                                    '\t\t\t\tsummary = summary .. ", RadarPostRDDCnt: " .. s{0}_{1}\n'.format(
                                        key, _name
                                    )
                                )

                            if (chunkoffsetCnt + _size) > _chunkSize:
                                chunkCntr += 1

                                chunkoffsetCnt = 0
                                f.write(
                                    "\n\t\t\telseif streamNumber == {0} and streamChunkIdx == {1} then\n".format(
                                        key, chunkCntr
                                    )
                                )

                            if _arraySize == 1:
                                f.write(
                                    "\t\t\t\tsubtree:add(fields{0}_{1},   tvbuffer(headerSize+{2},{3}), s{4}_{5})\n".format(
                                        key,
                                        _name.replace(".", "_"),
                                        chunkoffsetCnt,
                                        _size,
                                        key,
                                        _name,
                                    )
                                )

                                chunkoffsetCnt += _size
                                sizeCntr += _size
                            else:
                                for _index in range(0, _arraySize):

                                    if (chunkoffsetCnt + _size) > _chunkSize:
                                        chunkCntr += 1
                                        f.write(
                                            "\n\t\t\telseif streamNumber == {0} and streamChunkIdx == {1} then\n".format(
                                                key, chunkCntr
                                            )
                                        )

                                        chunkoffsetCnt = 0
                                        if self.skipArrays is False:
                                            f.write(
                                                "\t\t\t\tsubtree:add(fields{0}_{1},   tvbuffer(headerSize+{2},{3}), s{4}_{5})\n".format(
                                                    key,
                                                    _name.replace(".", "_"),
                                                    chunkoffsetCnt,
                                                    _size,
                                                    key,
                                                    _name,
                                                )
                                            )
                                        chunkoffsetCnt += _size
                                        sizeCntr += _size
                                    else:
                                        if self.skipArrays is False:
                                            f.write(
                                                "\t\t\t\tsubtree:add(fields{0}_{1},   tvbuffer(headerSize+{2},{3}), s{4}_{5})\n".format(
                                                    key,
                                                    _name.replace(".", "_"),
                                                    chunkoffsetCnt,
                                                    _size,
                                                    key,
                                                    _name,
                                                )
                                            )
                                        sizeCntr += _size
                                        chunkoffsetCnt += _size

                f.write("\t\t\tend\n")
                f.write("\t\t\tpinfo.cols.info = summary\n")
            f.write("\t\tend")
            f.write("\t\tend")
            f.write("\n\tend\n\n")
            f.write("\t---------------------------------------\n")
            f.write("\t-- Functions --------------------------\n")
            f.write("\t---------------------------------------\n")
            f.write("\n")
            f.write("\tfunction num2hex(num)\n")
            f.write("\t\tlocal hexstr = '0123456789abcdef'\n")
            f.write("\t\ts = ''\n")
            f.write("\t\twhile num > 0 do\n")
            f.write("\t\t\tmod = math.fmod(num, 16)\n")
            f.write("\t\t\ts = string.sub(hexstr, mod+1, mod+1) .. s\n")
            f.write("\t\t\tnum = math.floor(num / 16)\n")
            f.write("\t\tend\n")
            f.write("\t\tif s == '' then s = '0' end\n")
            f.write("\t\treturn s\n")
            f.write("\tend\n\n")

            f.write("\tfunction endiannessString(isBigEndian)\n")
            f.write("\t\tlocal s\n")
            f.write("\t\tif isBigEndian ~= 0 then\n")
            f.write('\t\t\ts = "Big Endian"\n')
            f.write("\t\telse -- isBigEndian == 0\n")
            f.write('\t\t\ts = "Little Endian"\n')
            f.write("\t\tend\n")
            f.write("\t\treturn s\n")
            f.write("\tend\n\n")

            f.write("\tfunction getCustomerString(source, customer)\n")
            f.write("\t\tlocal s\n")
            f.write("\t\tif source == 41 then\n")
            f.write('\t\t\tif     customer == 0x01 then s = "Volvo"\n')
            f.write('\t\t\telseif customer == 0x02 then s = "BMW"\n')
            f.write('\t\t\telseif customer == 0x03 then s = "GM"\n')
            f.write('\t\t\telseif customer == 0x04 then s = "HKMC"\n')
            f.write('\t\t\telseif customer == 0x05 then s = "Ford"\n')
            f.write('\t\t\telseif customer == 0x06 then s = "SGM"\n')
            f.write('\t\t\telseif customer == 0x07 then s = "VGTT"\n')
            f.write('\t\t\telseif customer == 0x08 then s = "Audi"\n')
            f.write('\t\t\telseif customer == 0x09 then s = "JLR"\n')
            f.write('\t\t\telseif customer == 0x0A then s = "Changan"\n')
            f.write('\t\t\telseif customer == 0x0B then s = "Scania/MAN"\n')
            f.write('\t\t\telseif customer == 0x0C then s = "Geely"\n')
            f.write('\t\t\telseif customer == 0x0D then s = "HKMC"\n')
            f.write('\t\t\telseif customer == 0x0E then s = "Renault"\n')
            f.write('\t\t\telseif customer == 0x0F then s = "ADV"\n')
            f.write('\t\t\telseif customer == 0x10 then s = "GWM"\n')
            f.write('\t\t\telse s = "Unknown"\n')
            f.write("\t\t\tend\n")
            f.write("\t\telse\n")
            f.write('\t\t\ts = ""\n')
            f.write("\t\tend\n")
            f.write("\t\treturn s\n")
            f.write("\tend\n\n")

            f.write("\tfunction hex2float (c)\n")
            f.write("\t\tif c == 0 then return 0.0 end\n")
            f.write(
                '\t\tc = string.gsub(string.format("%X", c), "(..)",function (x) return string.char(tonumber(x, 16)) end)\n'
            )
            f.write("\t\tlocal b1,b2,b3,b4 = string.byte(c, 1, 4)\n")
            f.write("\t\tlocal sign = b1 > 0x7F\n")
            f.write("\t\tlocal expo = (b1 % 0x80) * 0x2 + math.floor(b2 / 0x80)\n")
            f.write("\t\tlocal mant = ((b2 % 0x80) * 0x100 + b3) * 0x100 + b4\n")
            f.write("\t\tif sign then\n")
            f.write("\t\t\tsign = -1\n")
            f.write("\t\telse\n")
            f.write("\t\t\tsign = 1\n")
            f.write("\t\tend\n")
            f.write("\t\tlocal n\n")
            f.write("\t\tif mant == 0 and expo == 0 then\n")
            f.write("\t\t\tn = sign * 0.0\n")
            f.write("\t\telseif expo == 0xFF then\n")
            f.write("\t\t\tif mant == 0 then\n")
            f.write("\t\t\t\tn = sign * math.huge\n")
            f.write("\t\t\telse\n")
            f.write("\t\t\t\tn = 0.0/0.0\n")
            f.write("\t\t\tend\n")
            f.write("\t\telse\n")
            f.write("\t\t\tn = sign * math.ldexp(1.0 + mant / 0x800000, expo - 0x7F)\n")
            f.write("\t\tend\n")
            f.write("\t\treturn n\n")
            f.write("\tend\n\n")

            f.write("\tfunction getVariantString(source)\n")
            f.write("\t\tlocal v\n")
            f.write('\t\tif source == 200 then v = "FLR4"\n')
            f.write('\t\telseif source == 201 then v = "SRR6"\n')
            f.write('\t\telseif source == 202 then v = "SRR6p"\n')
            f.write('\t\telseif source == 203 then v = "FLR4p"\n')
            f.write('\t\telseif source == 204 or source == 72 then v = "SRR7p"\n')
            f.write('\t\telseif source == 205 or source == 75 then v = "SRR7hd"\n')
            f.write('\t\telseif source == 206 or source == 73 then v = "FLR7"\n')
            f.write('\t\telse v = "Unknown"\n')
            f.write("\t\tend\n")
            f.write("\t\treturn v\n")
            f.write("\tend\n\n")

            f.write('\tlocal udp_table = DissectorTable.get("udp.port")\n')

            f.write("\t-- Register Aptiv UDP protocol to handle messages on expected UDP ports.\n")
            locaUdpPorts = "\tlocal udp_ports = {"
            first = True

            for port in DestinationPorts:
                if first:
                    locaUdpPorts += '"{0}"'.format(port)
                    first = False
                else:
                    locaUdpPorts += ',"{0}"'.format(port)

            locaUdpPorts += "}\n"

            f.write(locaUdpPorts)

            f.write("\tfor i,port in ipairs(udp_ports) do\n")
            f.write("\t\tudp_table:add(port, aptivudp)\n")
            f.write("\tend\n")
            f.write("end\n")

    def __readStreamDef(self, filePath):
        arrayType = 0
        var = None
        varList = []
        arraySize = 1
        structSize = 0
        arrayStack = []  # Stack to manage array nesting and their offsets

        for line in open(filePath):
            if line != "":

                if structSize == 0 and "END_REPEAT" not in line:
                    structSize = int(line)

                if (len(line.split())) == 2:
                    _type = line.split()[0]
                    _name = line.split()[1].replace(".", "_")

                if line.startswith("REPEAT"):
                    arraySize = int(line.replace("REPEAT", ""))
                    arrayType = 1
                    arrayStack.append(arraySize)

                elif arrayType == 1 and "END_REPEAT" not in line:
                    var = self.__getVariableInfo(_type, _name)

                    if arrayStack:
                        lastArraySize = reduce(lambda x, y: x * y, arrayStack)
                        var._arraySize = lastArraySize
                    else:
                        var._arraySize = arraySize
                    structSize = structSize - (var._size * var._arraySize)
                    var._offset = structSize
                    varList.append(var)

                elif "END_REPEAT" in line:
                    arrayStack.pop()
                    if len(arrayStack) == 0:
                        arrayType = 0
                        arraySize = 1
                elif len(line.split()) == 2:
                    var = self.__getVariableInfo(_type, _name)
                    structSize = structSize - var._size
                    var._offset = structSize
                    varList.append(var)

        return varList

    def __return_class_variables(self, A):
        return A.__dict__

    def __getVariableInfo(self, _type, _name):
        _type = _type.strip().lower()
        _newType = _type.strip().lower()
        _size = None

        if _type.strip().lower() in self.variableTypes:
            _newType = self.variableTypes[_type][0]
            _size = self.variableTypes[_type][1]
        else:
            print("Unknown Variable Type: %s" % (_newType))

        v = _streamVar(_newType, _name, "", _type.strip().lower(), _size)

        return v

    def __addNewStream(self, _source, _stream, _version, _variables):
        newSource = {}
        _sum = 0

        for var in _variables:
            _sum += var._arraySize * var._size

        if self.sourceID == "":
            self.variant = self.sourceIDs[int(_source)]
            self.sourceID = int(_source)

        newSource = {
            "source": _source,
            "stream": _stream,
            "version": _version,
            "variables": _variables,
            "size": _sum,
        }

        self.streams.append(newSource)

    def __getSWVersion(self, streamFile):
        """Pull the sw version from the history file."""
        sw_version = ""

        if exists(streamFile):
            with open(streamFile) as file_:
                for line in file_:
                    if "#define MAJOR_RELEASE_REVISION" in line:
                        versionLine = line.split("(uint8_t)")
                        sw_version += versionLine[1].replace(")", "").strip() + "."
                    if "#define MINOR_RELEASE_REVISION" in line:
                        versionLine = line.split("(uint8_t)")
                        sw_version += versionLine[1].replace(")", "").strip() + "."
                    if "#define PATCH_RELEASE_REVISION" in line:
                        versionLine = line.split("(uint8_t)")
                        sw_version += versionLine[1].replace(")", "").strip() + "."
                    if "#define PRE_RELEASE_REVISION" in line:
                        versionLine = line.split("(uint8_t)")
                        sw_version += versionLine[1].replace(")", "").strip()
                        # print(sw_version)
                        break

        return (
            sw_version.replace(";", "")
            .replace("/", "")
            .replace("*", "")
            .replace("U", "")
            .replace(",", "")
        )

    def __readStreamFolder(self, file_list):
        """Read all the stream def files from the given array of stream def files."""
        for sd in file_list:
            sd = os.path.basename(sd)
            _sd = os.path.basename(sd.replace(".txt", ""))
            if "strdef" in _sd or "streamdef" in _sd:
                _src = int(re.search(r"_src[0-9]{3}", sd).group().replace("_src", ""))
                _str = int(re.search(r"_str[0-9]{3}", sd).group().replace("_str", ""))
                _ver = int(re.search(r"_ver[0-9]{3}", sd).group().replace("_ver", ""))
                _vars = self.__readStreamDef(os.path.join(self.streamDefPath, sd))

                self.__addNewStream(_src, _str, _ver, _vars)

        # ---------------------------------------------------------- #
        # Read all the stream def files from the given folder        #
        # ---------------------------------------------------------- #
        luaScript = self.outputLuaScriptPath

        desiredStream = []
        latestVersions = {}
        workingStreams = {}

        for stream in self.streams:

            if (
                self.sourceID == int(stream["source"])
                and stream["stream"] not in self.ignoreStreamList
            ):
                latestVersions["source"] = int(stream["source"])
                if stream["stream"] in latestVersions.keys():
                    if int(latestVersions[stream["stream"]]) < int(stream["version"]):
                        latestVersions[stream["stream"]] = stream["version"]
                        workingStreams[stream["stream"]] = stream
                        desiredStream.append(stream)
                else:
                    latestVersions[stream["stream"]] = stream["version"]
                    workingStreams[stream["stream"]] = stream
                    desiredStream.append(stream)

        self.__createLuaScript(workingStreams, self.variant, luaScript)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="APTIV Gen7 Lua Script Creator", epilog="APTIV Ltd, copyright 2022"
    )
    parser.add_argument(
        "-s",
        "--streamDefs",
        type=str,
        nargs="+",
        help="The stream defs used to generate the dissector file",
    )
    parser.add_argument("-o", "--outLuaScript", type=str, help="Output Lua Script File")
    parser.add_argument("-c", "--versionFile", type=str, help="Version .c file")

    args = parser.parse_args()

    luaCreator = Lua_Script_Creator(args.streamDefs, args.versionFile, args.outLuaScript)
