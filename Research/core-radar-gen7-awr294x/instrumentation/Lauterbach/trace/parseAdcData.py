"""
Convert a raw trace log from Lauterbach to parsed ADC data.

This module was developed to work with the ADP Gen 7 Radar embedded code.
It makes assumptions about how the data comes out in the trace log, and assumes
that only ADC data is in the Trace log.

This module converts data into a python object by default, which could be used by other scripts
to perform additional manipulations.

It also can output the data into a csv format, which could be imported by other scripts.
"""

import sys
import csv
import time

from os import mkdir, path
from os.path import isfile, split, splitext, isdir
from shutil import rmtree
import colorama
from colorama import Fore, Style


class elapsedTime:
    """
    Class to get timing metrics from when the class was created.
    """

    def __init__(self, noColor=False):
        """
        Start an elapsed time measurement.

        Args:
            noColor: Default - False: Set to true if you do not want colored outputs
        """
        self.startTime = time.time()
        self.noColor = noColor

    def printElapsedTime(self):
        """
        Print the elapsed time since the class was created.
        """
        if not self.noColor:
            elapsedTimeMsg = (
                Fore.CYAN
                + Style.BRIGHT
                + "Time Elapsed: "
                + Style.RESET_ALL
                + str(round(time.time() - self.startTime))
                + " Seconds"
            )
        else:
            elapsedTimeMsg = (
                "Time Elapsed: " + str(round(time.time() - self.startTime)) + " Seconds"
            )
        print(elapsedTimeMsg)


class log:
    """
    Store a collection of frames in the log class for a provided ADC log.

    The addFrame method should be used on each line inside a log to add data.
    """

    def __init__(self, recordColumnIdx, dataColumnIdx, cycleColumnIdx) -> None:
        """
        Create the log by letting the log know where to find specific data from the raw data format.

        Args:
            recordColumnIdx: column index for the record number
            dataColumnIdx: column index for the raw data
            cycleColumnIdx: column index for the cycle item
        """
        self.recordColumnIdx = recordColumnIdx
        self.dataColumnIdx = dataColumnIdx
        self.cycleColumnIdx = cycleColumnIdx
        self.frames = []
        self.numFrames = 0

    def addFrame(self, frameIdx):
        """
        Add a frame of data to the log class.

        This data must be added sequentially, and the data is assumed to be time formatted when it is added

        Args:
            frameIdx: Frame index for the added frame
        """
        self.frames.append(frame(frameIdx))
        self.numFrames = self.numFrames + 1
        return self.frames[self.numFrames - 1]


class frame:
    """
    Store a collection of chirps into a single frame using the frame class.
    """

    def __init__(self, frameIdx) -> None:
        """
        Create a new frame object and provide the frameIdx.

        Args:
            frameIdx: Frame index for the created frame
        """
        self.frameIdx = frameIdx
        self.chirps = []
        self.chirpLength = None
        self.numChirps = 0

    def addChirp(self):
        """
        Add a chirp to the frame class.
        """
        self.chirps.append(chirp())
        self.numChirps = self.numChirps + 1
        return self.chirps[self.numChirps - 1]

    def removeLastChirp(self):
        """
        Remove the last chirp from this frame.

        This method is used to patch a bug where a chirp is duplicated on the raw ADC buffer
        """
        self.chirps.pop()
        self.numChirps = self.numChirps - 1


class chirp:
    """
    Create a chirp object to store the raw chirp data in.
    """

    def __init__(self) -> None:
        """
        Create a new chirp object.
        """
        self.chirpData = []
        self.numSamples = 0

    def addRawDataStream(self, rawDataStream):
        """
        Add a section of the raw data stream to this chirp.

        Format the raw data into parsed data by taking the twos complement of the raw data

        Args:
            rawDataStream: The raw data that should be added
        """
        numBits = 16
        charsPerSample = int(numBits / 2 / 2)  # Bits / 2 chars per byte / 2 bytes per Sample
        idx = len(rawDataStream)
        while idx - charsPerSample >= 0:
            val = twos_complement(rawDataStream[idx - charsPerSample : idx], numBits)
            self.chirpData.append(val)
            idx = idx - charsPerSample
            self.numSamples = self.numSamples + 1


def findNextValidData(row, adcReader, cycleColumnIdx):
    """
    Find the next valid raw data stream from a raw trace log.

    The ADC trace log has many debugging and error messages contained along with the ADC data that is of interest.
    This method determines if a message is from the debugging and error messages or from a valid ADC data message
    and returns the row with the next valid data.

    Args:
        row: current row that should be analyzed
        adcReader: Full log of trace data, converted via csv.reader
        cycleColumnIdx: The column index of the cycle parameter
    """
    while True:
        row = next(adcReader)
        if row[cycleColumnIdx] == "d64":
            break
    return row


def twos_complement(hexstr, bits):
    """
    Take the twos complement of an input.

    Args:
        hexstr: The raw hex string to perform the twos complement on
        bits: The number of bits to run the twos complement on
    """
    value = int(hexstr, 16)
    if value & (1 << (bits - 1)):
        value -= 1 << bits
    return value


class parseAdcData:
    """
    Parse a provided trace log and manipulate the data as required.
    """

    def __init__(self, inputFile) -> None:
        """
        Convert a trace log with ADC data into an object that can be used easily parse the data.

        Args:
            inputFile: Path to the raw trace data log
        """
        self.inputFile = inputFile
        currChirp = None
        currFrame = None
        prevChirp = None
        with open(self.inputFile) as adcDataFile:
            adcReader = csv.reader(adcDataFile, delimiter=",", quotechar='"')
            for row in adcReader:
                if adcReader.line_num == 2:
                    # Header row
                    # We have to adjust the index because Trace32 doesn't add a , if a field is empty
                    adjIdx = 0
                    for _idx, column in enumerate(row):
                        if column == "record":
                            recordColumnIdx = adjIdx
                        if column == "run" or column == "symbol":
                            adjIdx = adjIdx - 1
                        if column == "data":
                            dataColumnIdx = adjIdx
                        if column == "cycle":
                            cycleColumnIdx = adjIdx
                        adjIdx = adjIdx + 1
                    self.adcLog = log(recordColumnIdx, dataColumnIdx, cycleColumnIdx)
                elif adcReader.line_num > 2:
                    try:
                        # Skip over the random packets coming in that can't be decoded here
                        if row[self.adcLog.cycleColumnIdx] == "d64":
                            # Find the start of a Chirp Header Set
                            if row[self.adcLog.dataColumnIdx] == "ACD0000000000ADC":
                                row = findNextValidData(row, adcReader, cycleColumnIdx)
                                # Get the frame Index from the header
                                frameIdx = int(row[self.adcLog.dataColumnIdx], 16)
                                if self.adcLog.frames == [] or currFrame.frameIdx != frameIdx:
                                    print("Decoding Frame ", frameIdx)
                                    # Create a new frame and add a chirp to it
                                    currFrame = self.adcLog.addFrame(frameIdx)
                                    row = findNextValidData(row, adcReader, cycleColumnIdx)
                                    currFrame.chirpLength = int(row[self.adcLog.dataColumnIdx], 16)
                                    prevChirp = currChirp
                                    currChirp = currFrame.addChirp()
                                else:
                                    if currFrame.numChirps == 1:
                                        # Check the first chirp against the last chirp of the previous frame
                                        if currFrame is not None:
                                            # Check that this isn't the 2nd chirp we process
                                            if prevChirp is not None:
                                                # Compare the data in the chirps and verify 2 back
                                                if prevChirp.chirpData == currChirp.chirpData:
                                                    print(
                                                        Fore.YELLOW
                                                        + "    First chirp of frame %d duplicates the last chirp \
                                                          of frame %d.\n    Removing the chirp from frame %d!"
                                                        % (
                                                            currFrame.frameIdx,
                                                            currFrame.frameIdx - 1,
                                                            currFrame.frameIdx,
                                                        )
                                                        + Style.RESET_ALL
                                                    )
                                                    currFrame.removeLastChirp()
                                    # Add a chirp to the current frame
                                    prevChirp = currChirp
                                    currChirp = currFrame.addChirp()

                                # Move to the end of the header
                                while True:
                                    if row[self.adcLog.dataColumnIdx] != "0123456789ABCDEF":
                                        row = findNextValidData(row, adcReader, cycleColumnIdx)
                                    else:
                                        break
                                continue
                            if currChirp:
                                # Only get here if its a data row and we've already seen a frame header
                                currChirp.addRawDataStream(row[self.adcLog.dataColumnIdx])
                    except IndexError:
                        continue
            print("File Parsing Complete!\n")

    def printChirpsInFrames(self):
        """
        Print the number of chirps in every parsed frame.
        """
        for frame in self.adcLog.frames:
            print("Frame %d: %d Chirps Found" % (frame.frameIdx, frame.numChirps))

    def exportToParsedCsv(self):
        """
        Export the parsed data to a CSV file.

        The file will be created alongside the raw trace log
        """
        print("Exporting to CSV...")
        [dirPath, filename] = split(self.inputFile)
        outputDir = path.join(dirPath, splitext(filename)[0])
        if isdir(outputDir):
            rmtree(outputDir)
        mkdir(outputDir)
        for frame in self.adcLog.frames:
            csvExportFile = path.join(outputDir, "frame_%d.csv" % frame.frameIdx)
            with open(csvExportFile, "w", newline="") as f:
                writer = csv.writer(f)
                for chirp in frame.chirps:
                    writer.writerow(chirp.chirpData)
            print(
                "Frame %d written to %s with %d chirps"
                % (frame.frameIdx, csvExportFile, frame.numChirps)
            )


colorama.init(autoreset=True)
elapsedTimeObj = elapsedTime(False)

if len(sys.argv) > 2:
    print(
        "Too many arguments provided. Only 1 argument allowed that gives the path to the data csv file"
    )
elif len(sys.argv) < 2:
    print("Must provide the path to the csv data file as an argument")
elif not (isfile(sys.argv[1])):
    print("File is invalid. Provide a valid file path")
else:
    parseAdcObj = parseAdcData(sys.argv[1])
    parseAdcObj.exportToParsedCsv()

elapsedTimeObj.printElapsedTime()
