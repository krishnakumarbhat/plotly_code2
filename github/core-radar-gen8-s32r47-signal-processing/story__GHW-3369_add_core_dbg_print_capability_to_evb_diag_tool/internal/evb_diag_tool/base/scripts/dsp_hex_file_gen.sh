#!/usr/bin/env bash

#
# @file dsp_hex_file_gen.sh
# @brief Converts BBE elf file to C data arrays.
#        Based on NXP's Diagnostics Tool code.
#
#------------------------------------------------------------------------------
#
# @copyright Copyright 2026 Aptiv, All Rights Reserved.
# @copyright Aptiv Confidential
#
#------------------------------------------------------------------------------
#
# @section DESC DESCRIPTION:
# NOTES: This is a direct copy of NXP's file, except the final extra copy to
#        another directory has been removed. Look for the keyword APTV.
#
#
# @section ABBR ABBREVIATIONS:
#
# @section TRACE TRACEABILITY INFO:
#   - Design Document(s):
#
#   - Requirements Document(s):
#
#   - Applicable Standards (in order of precedence: highest first):
#     - ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards"
#
# @section DFS DEVIATIONS FROM STANDARDS:
#   - None
#

ELF_NAME=$1
STB_DIR=$2
OUT_NAME=$3
PLATFORM=$4
OUT_DIR=$5
CORE=$6
INST_ID=$7
CUR_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

SROM0_BASE_ADDR=0x0
DRAM0_BASE_ADDR=0x0
DRAM1_BASE_ADDR=0x0
IRAM0_BASE_ADDR=0x0
SRAM_BASE_ADDR=0x0

DRAM_SIZE=0x0
IRAM_SIZE=0x0
SROM_SIZE=0x0
SRAM_SIZE=0x0

if [ -z "${ELF_NAME}" ] || [ -z "${STB_DIR}" ] || [ -z "${OUT_NAME}" ] || [ -z "${PLATFORM}" ]; then
    echo "Error: too few arguments."
    echo "Usage: dsp_hex_file_gen.sh ELF_NAME STB_DIR OUT_NAME PLATFORM [OUT_DIR] [CORE] [INST_ID]"
    exit 1
fi

if [ -z "${OUT_DIR}" ]; then
    OUT_DIR=.
fi

# For R47 and R43 CORE has to be provided by user. For all other platforms it is fixed.
if [ -z "${CORE}" ]; then
    if ([ $PLATFORM == "S32R47" ] || [ $PLATFORM == "S32R43" ]); then
        echo "Error: CORE has to be explicitly defined to BBE32 or KQ8."
        exit 1
    else
        CORE="BBE32"
    fi
fi

# For R47 INST_ID has to be provided by user. For all other platforms it is fixed.
if [ -z "${INST_ID}" ]; then
    if [ $PLATFORM == "S32R47" ]; then
        echo "Error: Instance ID has to be explicitly defined to 0 or 1."
        exit 1
    else
        INST_ID="0"
    fi
fi

if [ $PLATFORM == "S32R45" ]; then
    SROM0_BASE_ADDR=0x0
    DRAM0_BASE_ADDR=0x24160000
    DRAM1_BASE_ADDR=0x24140000
    IRAM0_BASE_ADDR=0x24180000
    SRAM_BASE_ADDR=0x34000000

    DRAM_SIZE=0x20000
    IRAM_SIZE=0x20000
    SROM_SIZE=0x1000000
    SRAM_SIZE=0x800000
elif [ $PLATFORM == "S32R41" ]; then
    SROM0_BASE_ADDR=0x0
    DRAM0_BASE_ADDR=0x24100000
    DRAM1_BASE_ADDR=0x24120000
    IRAM0_BASE_ADDR=0x24140000
    SRAM_BASE_ADDR=0x33c00000

    DRAM_SIZE=0x20000
    IRAM_SIZE=0x40000
    SROM_SIZE=0x1000000
    SRAM_SIZE=0x800000
elif [ $PLATFORM == "SAF85XX" ]; then
    SROM0_BASE_ADDR=0x0
    DRAM0_BASE_ADDR=0x24100000
    DRAM1_BASE_ADDR=0x24120000
    IRAM0_BASE_ADDR=0x24140000
    SRAM_BASE_ADDR=0x33c00000

    DRAM_SIZE=0x20000
    IRAM_SIZE=0x40000
    SROM_SIZE=0x1000000
    SRAM_SIZE=0x800000
elif [ $PLATFORM == "SAF86XX" ]; then
    SROM0_BASE_ADDR=0x0
    DRAM0_BASE_ADDR=0x24100000
    DRAM1_BASE_ADDR=0x24110000
    IRAM0_BASE_ADDR=0x24120000
    SRAM_BASE_ADDR=0x33E80000

    DRAM_SIZE=0x10000
    IRAM_SIZE=0x10000
    SROM_SIZE=0x1000000
    SRAM_SIZE=0x200000
elif [ $PLATFORM == "S32R47" ] || [ $PLATFORM == "S32R43" ]; then
    SROM0_BASE_ADDR=0x0
    SRAM_BASE_ADDR=0x33C00000

    SROM_SIZE=0x1000000
    SRAM_SIZE=0x800000

    if [ $CORE == "BBE32" ]; then
        if [ $INST_ID == "0" ]; then
            DRAM0_BASE_ADDR=0x21000000
            DRAM1_BASE_ADDR=0x21020000
            IRAM0_BASE_ADDR=0x21040000
        elif [ $INST_ID == "1" ]; then
            DRAM0_BASE_ADDR=0x21000000
            DRAM1_BASE_ADDR=0x21020000
            IRAM0_BASE_ADDR=0x21040000
        else
            echo "Wrong instance ID specified. Valid IDs: 0/1"
            exit 1
        fi
        DRAM_SIZE=0x20000
        IRAM_SIZE=0x40000

    elif [ $CORE == "KQ8" ]; then
        if [ $INST_ID == "0" ]; then
            DRAM0_BASE_ADDR=0x24000000
            DRAM1_BASE_ADDR=0x24040000
            IRAM0_BASE_ADDR=0x24080000
        elif [ $INST_ID == "1" ]; then
            DRAM0_BASE_ADDR=0x24100000
            DRAM1_BASE_ADDR=0x24140000
            IRAM0_BASE_ADDR=0x24180000
        else
            echo "Wrong instance ID specified. Valid IDs: 0/1"
            exit 1
        fi
        DRAM_SIZE=0x40000
        IRAM_SIZE=0x20000

    else
        echo "Wrong core specified. Valid cores: BBE32/KQ8"
        exit 1
    fi
else
    echo "Wrong platform specified. Valid platforms: S32R45/S32R41/SAF85XX/SAF86XX/S32R43/S32R47"
    exit 1
fi

OUTPUT_FILE_NAME=${OUT_DIR}/${OUT_NAME}

ELF_EXE_SPLIT=(${ELF_NAME//// })
ELF_EXE=${ELF_EXE_SPLIT[-1]}

${STB_DIR}/xt-dumpelf.exe --base=${SRAM_BASE_ADDR}  --offset=${SRAM_BASE_ADDR}  --xtsc --width=128 --linesize=64 --size=${SRAM_SIZE} ${ELF_NAME} > dsp_${OUT_NAME}_${ELF_EXE}_sram.data
${STB_DIR}/xt-dumpelf.exe --base=${SROM0_BASE_ADDR} --offset=${SROM0_BASE_ADDR} --xtsc --width=128 --linesize=64 --size=${SROM_SIZE} ${ELF_NAME} > dsp_${OUT_NAME}_${ELF_EXE}_srom.data
if [ $PLATFORM == "S32R45" ]; then
    ${STB_DIR}/xt-dumpelf.exe --base=${IRAM0_BASE_ADDR} --offset=${IRAM0_BASE_ADDR} --xtsc --width=128                   --size=${IRAM_SIZE} ${ELF_NAME} > dsp_${OUT_NAME}_${ELF_EXE}_iram0.data
    ${STB_DIR}/xt-dumpelf.exe --base=${DRAM0_BASE_ADDR} --offset=${DRAM0_BASE_ADDR} --xtsc --width=512                   --size=${DRAM_SIZE} ${ELF_NAME} > dsp_${OUT_NAME}_${ELF_EXE}_dram0.data
    ${STB_DIR}/xt-dumpelf.exe --base=${DRAM1_BASE_ADDR} --offset=${DRAM1_BASE_ADDR} --xtsc --width=512                   --size=${DRAM_SIZE} ${ELF_NAME} > dsp_${OUT_NAME}_${ELF_EXE}_dram1.data
elif [ $PLATFORM == "S32R41" ]; then
    ${STB_DIR}/xt-dumpelf.exe --base=${IRAM0_BASE_ADDR} --offset=${IRAM0_BASE_ADDR} --xtsc --width=128 --linesize=128 --size=${IRAM_SIZE} ${ELF_NAME} > dsp_${OUT_NAME}_${ELF_EXE}_iram0.data
    ${STB_DIR}/xt-dumpelf.exe --base=${DRAM0_BASE_ADDR} --offset=${DRAM0_BASE_ADDR} --xtsc --width=512 --linesize=128 --size=${DRAM_SIZE} ${ELF_NAME} > dsp_${OUT_NAME}_${ELF_EXE}_dram0.data
    ${STB_DIR}/xt-dumpelf.exe --base=${DRAM1_BASE_ADDR} --offset=${DRAM1_BASE_ADDR} --xtsc --width=512 --linesize=128 --size=${DRAM_SIZE} ${ELF_NAME} > dsp_${OUT_NAME}_${ELF_EXE}_dram1.data
elif [ $PLATFORM == "SAF85XX" ]; then
    ${STB_DIR}/xt-dumpelf.exe --base=${IRAM0_BASE_ADDR} --offset=${IRAM0_BASE_ADDR} --xtsc --width=128 --linesize=128 --size=${IRAM_SIZE} ${ELF_NAME} > dsp_${OUT_NAME}_${ELF_EXE}_iram0.data
    ${STB_DIR}/xt-dumpelf.exe --base=${DRAM0_BASE_ADDR} --offset=${DRAM0_BASE_ADDR} --xtsc --width=512 --linesize=128 --size=${DRAM_SIZE} ${ELF_NAME} > dsp_${OUT_NAME}_${ELF_EXE}_dram0.data
    ${STB_DIR}/xt-dumpelf.exe --base=${DRAM1_BASE_ADDR} --offset=${DRAM1_BASE_ADDR} --xtsc --width=512 --linesize=128 --size=${DRAM_SIZE} ${ELF_NAME} > dsp_${OUT_NAME}_${ELF_EXE}_dram1.data
elif [ $PLATFORM == "SAF86XX" ]; then
    ${STB_DIR}/xt-dumpelf.exe --base=${IRAM0_BASE_ADDR} --offset=${IRAM0_BASE_ADDR} --xtsc --width=128 --linesize=128 --size=${IRAM_SIZE} ${ELF_NAME} > dsp_${OUT_NAME}_${ELF_EXE}_iram0.data
    ${STB_DIR}/xt-dumpelf.exe --base=${DRAM0_BASE_ADDR} --offset=${DRAM0_BASE_ADDR} --xtsc --width=512 --linesize=128 --size=${DRAM_SIZE} ${ELF_NAME} > dsp_${OUT_NAME}_${ELF_EXE}_dram0.data
    ${STB_DIR}/xt-dumpelf.exe --base=${DRAM1_BASE_ADDR} --offset=${DRAM1_BASE_ADDR} --xtsc --width=512 --linesize=128 --size=${DRAM_SIZE} ${ELF_NAME} > dsp_${OUT_NAME}_${ELF_EXE}_dram1.data
elif [ $PLATFORM == "S32R47" ]; then
    if [ $CORE == "BBE32" ]; then
        ${STB_DIR}/xt-dumpelf.exe --base=${IRAM0_BASE_ADDR} --offset=${IRAM0_BASE_ADDR} --xtsc --width=128 --linesize=128 --size=${IRAM_SIZE} ${ELF_NAME} > dsp_${OUT_NAME}_${ELF_EXE}_iram0.data
        ${STB_DIR}/xt-dumpelf.exe --base=${DRAM0_BASE_ADDR} --offset=${DRAM0_BASE_ADDR} --xtsc --width=512 --linesize=128 --size=${DRAM_SIZE} ${ELF_NAME} > dsp_${OUT_NAME}_${ELF_EXE}_dram0.data
        ${STB_DIR}/xt-dumpelf.exe --base=${DRAM1_BASE_ADDR} --offset=${DRAM1_BASE_ADDR} --xtsc --width=512 --linesize=128 --size=${DRAM_SIZE} ${ELF_NAME} > dsp_${OUT_NAME}_${ELF_EXE}_dram1.data
    elif [ $CORE == "KQ8" ]; then
        ${STB_DIR}/xt-dumpelf.exe --base=${IRAM0_BASE_ADDR} --offset=${IRAM0_BASE_ADDR} --xtsc --width=128 --linesize=64  --size=${IRAM_SIZE} ${ELF_NAME} > dsp_${OUT_NAME}_${ELF_EXE}_iram0.data
        ${STB_DIR}/xt-dumpelf.exe --base=${DRAM0_BASE_ADDR} --offset=${DRAM0_BASE_ADDR} --xtsc --width=512 --linesize=128 --size=${DRAM_SIZE} ${ELF_NAME} > dsp_${OUT_NAME}_${ELF_EXE}_dram0.data
        ${STB_DIR}/xt-dumpelf.exe --base=${DRAM1_BASE_ADDR} --offset=${DRAM1_BASE_ADDR} --xtsc --width=512 --linesize=128 --size=${DRAM_SIZE} ${ELF_NAME} > dsp_${OUT_NAME}_${ELF_EXE}_dram1.data
    else
        echo "Wrong core specified. Valid cores: BBE32/KQ8"
        exit 1
    fi
else
    echo "Wrong platform specified. Valid platforms: S32R45/S32R41/SAF85XX/SAF86XX/S32R43/S32R47"
    exit 1
fi


cat dsp_${OUT_NAME}_${ELF_EXE}_*.data > dsp_${OUT_NAME}_${ELF_EXE}_data.data

{
  echo ' /*****************************************************************************'
  echo ' *'
  echo ' * Copyright 2025 NXP'
  echo ' * NXP Confidential. This software is owned or controlled by NXP and may only be used strictly in accordance with the'
  echo ' * applicable license terms.  By expressly accepting such terms or by downloading, installing, activating and/or otherwise'
  echo ' * using the software, you are agreeing that you have read, and that you agree to comply with and are bound by, such'
  echo ' * license terms.  If you do not agree to be bound by the applicable license terms, then you may not retain, install,'
  echo ' * activate or otherwise use the software.'
  echo ' *'
  echo ' * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR'
  echo ' * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES'
  echo ' * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.'
  echo ' * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,'
  echo ' * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES'
  echo ' * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR'
  echo ' * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)'
  echo ' * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,'
  echo ' * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING'
  echo ' * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF'
  echo ' * THE POSSIBILITY OF SUCH DAMAGE.'
  echo ' *'
  echo ' **************************************************************************************************/'
  echo ''
  echo '#include <stdint.h>'
  echo ''
  echo '#ifdef __cplusplus'
  echo 'extern "C" {'
  echo '#endif'
  echo ''

#postprocess vps.data to produce plain text c-style data arrays intended to be included directly in the main CPU's source code:
  idx=0
  segAddresses=''
  while read curentLine; do
    if [[ $curentLine = '@'* ]]; then

        if [ -z ${IS_NOT_FIRST+x} ]; then
          IS_NOT_FIRST=false
        else
          echo '};'
          echo ''
        fi

#        echo "uint8_t dsp_img_segment_${curentLine:1:10}[] __attribute__((section("\".dsp_img_segment_${curentLine:1:10}\""))) = {"
      segAddresses="${segAddresses}${curentLine:1:10}, "
        if [ $INST_ID == "0" ]; then
          echo "uint8_t dsp_0_img_segment_$idx[] = {"
        elif [ $INST_ID == "1" ]; then
          echo "uint8_t dsp_1_img_segment_$idx[] = {"
        fi
      ((idx++));
    else
        CleanedCurentLine=${curentLine//[$'\r\n']}
        echo ${CleanedCurentLine// /,},
    fi
  done <dsp_${OUT_NAME}_${ELF_EXE}_data.data

  echo '};'
  echo ''

  #echo "#define RSDK_NUM_DSP_IMG_SEGMENTS ($idx)"
  if [ $INST_ID == "0" ]; then
    echo "uint8_t gNumDsp0ImgSegments = $idx;"
  elif [ $INST_ID == "1" ]; then
    echo "uint8_t gNumDsp1ImgSegments = $idx;"
  fi
  echo ''

  #echo "uintptr_t dspImgSegmentRunaddr[RSDK_NUM_DSP_IMG_SEGMENTS] = {$segAddresses};"
  if [ $INST_ID == "0" ]; then
  echo "uintptr_t dsp0ImgSegmentRunaddr[] = {$segAddresses};"
  elif [ $INST_ID == "1" ]; then
  echo "uintptr_t dsp1ImgSegmentRunaddr[] = {$segAddresses};"
  fi
  echo ''

  ((idx--))
  #echo "uint32_t dspImgSegmentSizes[RSDK_NUM_DSP_IMG_SEGMENTS] = {"
  if [ $INST_ID == "0" ]; then
  echo "uint32_t dsp0ImgSegmentSizes[] = {"
  elif [ $INST_ID == "1" ]; then
  echo "uint32_t dsp1ImgSegmentSizes[] = {"
  fi
  for i in $(seq 0 $idx); do
  if [ $INST_ID == "0" ]; then
  echo "    sizeof(dsp_0_img_segment_$i)",
  elif [ $INST_ID == "1" ]; then
  echo "    sizeof(dsp_1_img_segment_$i)",
  fi
  done
  echo "};"
  echo ''

  #echo "uint8_t* dspImgSegmentLoadAddr[RSDK_NUM_DSP_IMG_SEGMENTS] = {"
  if [ $INST_ID == "0" ]; then
  echo "uint8_t* dsp0ImgSegmentLoadAddr[] = {"
  elif [ $INST_ID == "1" ]; then
  echo "uint8_t* dsp1ImgSegmentLoadAddr[] = {"
  fi
  for i in $(seq 0 $idx); do
  if [ $INST_ID == "0" ]; then
  echo "    dsp_0_img_segment_$i",
  elif [ $INST_ID == "1" ]; then
  echo "    dsp_1_img_segment_$i",
  fi
  done
  echo "};"
  echo ''

  echo '#ifdef __cplusplus'
  echo '}'
  echo '#endif'

} >${OUTPUT_FILE_NAME}

echo created file: ${OUTPUT_FILE_NAME}

# APTIV REMOVED
# cp ${OUTPUT_FILE_NAME} ../../Diag_Tool_M7/src/radar/bbe/src/
# echo
# echo copied to: ../../Diag_Tool_M7/src/radar/bbe/src/${OUT_NAME}

#remove temp files:
rm -f dsp_${OUT_NAME}_${ELF_EXE}_*.data
