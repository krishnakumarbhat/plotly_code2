#!/bin/bash
#-----------------------------------------------------------------------------
# This script takes in the parameters needed for the stream def tool
# to create the stream def txt file. It also check that the stream number
# and stream version in the logging file match the define number and version
# in the software\app\common\BUILD file.
#-----------------------------------------------------------------------------

POSITIONAL_ARGS=()

while [[ $# -gt 0 ]]; do
  case $1 in
    -STREAM_DEF_TOOL)
      STREAM_DEF_TOOL="$2"
      shift # past argument
      shift # past value
      ;;
    -LOGGING_FILE)
      LOGGING_FILE="$2"
      shift # past argument
      shift # past value
      ;;
    -SOURCE_NUM)
      SOURCE_NUM="$2"
      shift # past argument
      shift # past value
      ;;
    -STREAM_NUM)
      STREAM_NUM="$2"
      shift # past argument
      shift # past value
      ;;
    -STREAM_VER)
      STREAM_VER="$2"
      shift # past argument
      shift # past value
      ;;
    -STREAM_STRUCT)
      STREAM_STRUCT="$2"
      shift # past argument
      shift # past value
      ;;
    -MACRO_LABELS)
      MACRO_LABELS="$2"
      shift # past argument
      shift # past value
      ;;
    -SECTION_COMPATABILITY)
      SECTION_COMPATABILITY="$2"
      shift # past argument
      shift # past value
      ;;
    -OUT_DIR)
      OUT_DIR="$2"
      shift # past argument
      shift # past value
      ;;
    -OUT_FILE)
      OUT_FILE="$2"
      shift # past argument
      shift # past value
      ;;
    -*|--*)
      echo "Unknown option $1"
      exit 1
      ;;
    *)
      POSITIONAL_ARGS+=("$1") # save positional arg
      shift # past argument
      ;;
  esac
done

# Stream def tool options
if [ -z "$MACRO_LABELS" ]; then
  MACRO_LABELS='PCRESIM=true,PC_RESIM=true,DVTOOL_MODIFICATION=true,LITTLE_ENDIAN_STRUCTURE=true'
fi

DEFAULT_PACK_SIZE='4'
TASKING_STATUS='true'

if [ "$STREAM_NUM" = "14" ]; then
  # Read the logging file to get the Section Compatibility Version
  sec_comp_str="$(grep 'H2_SECTION_COMPATIBILITY' $LOGGING_FILE)"
  regex="H2_SECTION_COMPATIBILITY[[:space:]]+\(([[:digit:]]+)[uU]*\)"
  if [[ $sec_comp_str =~ $regex ]]; then
    sec_comp="${BASH_REMATCH[1]}"
  fi

  if [[ $sec_comp != $SECTION_COMPATABILITY ]]; then
    echo
    echo "ERROR: Actual section compatibility $sec_comp in $LOGGING_FILE does not match expected section compatibility $SECTION_COMPATABILITY in the BUILD file"
    echo
    exit 1
  fi

else
  # Read the logging file to get the Stream Number
  stream_number_str="$(grep 'STREAM_NUMBER' $LOGGING_FILE)"
  regex="STREAM_NUMBER[[:space:]]+\(([[:digit:]]+)[uU]*\)"
  if [[ $stream_number_str =~ $regex ]]; then
    stream_number="${BASH_REMATCH[1]}"
  fi

  # Read the logging file to get the Stream Version
  stream_version_str="$(grep 'STREAM_VERSION' $LOGGING_FILE)"
  regex="STREAM_VERSION[[:space:]]+\(([[:digit:]]+)[uU]*\)"
  if [[ $stream_version_str =~ $regex ]]; then
    stream_version="${BASH_REMATCH[1]}"
  fi

  # Check that the stream number provided matches the stream number in the file
  if [[ $stream_number != $STREAM_NUM ]]; then
    echo
    echo "ERROR: Actual stream number $stream_number in $LOGGING_FILE does not match expected stream number $STREAM_NUM in the BUILD file"
    echo
    exit 1
  fi

  # Check that the stream version provided matches the stream version in the file
  if [[ $stream_version != $STREAM_VER ]]; then
    echo
    echo "ERROR: Actual stream version $stream_version in $LOGGING_FILE does not match expected stream version $STREAM_VER in the BUILD file"
    echo
    exit 1
  fi
fi

# Read the logging file to get the Stream Structure name
regex="\<${STREAM_STRUCT}\>;"
stream_struct_str="$(grep ${regex} $LOGGING_FILE)"

if [ -z "$stream_struct_str" ]; then
  echo
  echo "ERROR: Stream structure type $STREAM_STRUCT provided in the BUILD file not found in $LOGGING_FILE"
  echo
  exit 1
fi

# Make the output directory if it doesn't exist
if [ ! -d $OUT_DIR ]; then
  mkdir $OUT_DIR
fi

# Run the streamdef generation tool
ARG_LIST="-f ${LOGGING_FILE} -s ${STREAM_STRUCT} -o ${OUT_DIR}/${OUT_FILE} -d ${MACRO_LABELS} -p ${DEFAULT_PACK_SIZE} --tasking ${TASKING_STATUS}"

mono ${STREAM_DEF_TOOL} ${ARG_LIST} > /dev/null
