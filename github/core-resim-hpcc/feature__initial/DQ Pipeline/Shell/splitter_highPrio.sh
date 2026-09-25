#!/bin/bash

newarr[0]=$1
newarr[1]=$2
TEMP_PATH=$3
input_file=$4
CUST_NAME=$5

echo "$newarr[0]"
echo "$newarr[1]"
echo "$TEMP_PATH"
echo "$input_file"
echo "$CUST_NAME"



echo "[Splitter_Execution] : execution requested for - ${newarr[0]}, ${newarr[1]}"

if [[ $CUST_NAME == *'scale1'* ]]; then

INPUT_DIR=$(dirname "${newarr[0]}")

for file in "$INPUT_DIR"/*; do
   
  if [[ $file = *'bus'* ]]; then
  bus_files+=($file)
  elif [[ $file = *'deb'* ]]; then
  deb_files+=($file)

  fi
done

elif [[ $CUST_NAME == *'scale3'* ]]; then

IFS="/" read -r -a path <<< "${newarr[0]}" 
INPUT_DIR=${newarr[0]%%"/${path[-2]}"*}

for path in "$INPUT_DIR"/*; do

  if [[ $path = *'Resim_Radars_CAN_Corner_bus'* ]]; then
    for file in "$path"/*; do
      if [[ $file = *'.MF4'* ]]; then
          can_corner_bus_files+=($file)
      fi
    done

  elif [[ $path = *'Aptiv_FLR_Obj_Perc_for_mPAD'* ]]; then
    for file in "$path"/*; do
      if [[ $file = *'.MF4'* ]]; then
          mPAD_files+=($file)
      fi
    done

  elif [[ $path = *'Resim_Radars_deb'* ]]; then
    for file in "$path"/*; do
      if [[ $file = *'.MF4'* ]]; then
          deb_files+=($file)
      fi
    done

  elif [[ $path = *'Resim_Radars_CAN_Side_bus'* ]]; then
    for file in "$path"/*; do
      if [[ $file = *'.MF4'* ]]; then
          can_side_bus_files+=($file)
      fi
    done

  fi
done

elif [[ $CUST_NAME == *'traton'* ]]; then
IFS="/" read -r -a path <<< "${newarr[0]}" 
INPUT_DIR=${newarr[0]%%"/${path[-1]}"*}
for file in "$INPUT_DIR"/*; do
    if [[ $file = *'TRATON_SRR6P'* && $file = *'.mf4'* ]]; then
        deb_files+=($file)        
    fi 
done

elif [[ $CUST_NAME == *'rnasdv'* ]]; then

IFS="/" read -r -a path <<< "${newarr[0]}" 
INPUT_DIR=${newarr[0]%%"/${path[-1]}"*}
for file in "$INPUT_DIR"/*; do
    if [[ $file = *'SDV'* && $file != *'.aspx'* ]]; then
        deb_files+=($file)        
    fi 
done

elif [[ $CUST_NAME == *'honda'* ]]; then

IFS="/" read -r -a path <<< "${newarr[0]}" 
INPUT_DIR=${newarr[0]%%"/${path[-1]}"*}
for file in "$INPUT_DIR"/*; do
    if [[ $file == *'.mf4'* ]]; then
        deb_files+=($file)        
    fi 
done

elif [[ $CUST_NAME == *'gpo-gen7'* ]]; then

IFS="/" read -r -a path <<< "${newarr[0]}" 
INPUT_DIR=${newarr[0]%%"/${path[-1]}"*}
for file in "$INPUT_DIR"/*; do
    if [[ $file = *'_Aptiv.'* && $file != *'.aspx'* ]]; then
        deb_files+=($file)        
    fi 
done

elif [[ $CUST_NAME == *'dc'* ]]; then
 
    if [[ $CUST_NAME == *'ipnext_srr'* ]]; then
        IFS="/" read -r -a path <<< "${newarr[0]}" 
        INPUT_DIR=${newarr[0]%%"/${path[-2]}"*}
 
        for path in "$INPUT_DIR"/*; do
            if [[ $path = *'BN_RADETH'* ]]; then
                for file in "$path"/*; do
                    if [[ $file = *'KPI_SRR_DS'* && $file == *'.MF4'* ]]; then
                        deb_files+=($file)
                    fi
                done 
            fi           
        done
        if [[ $CUST_NAME == *'dgps'* ]]; then
            for path in "$INPUT_DIR"/*; do        
                if [[ $path = *'MT_RE'* ]]; then
                       for file in "$path"/*; do
                           if [[ $file = *'KPI_SRR_DS'* && $file == *'.MF4'* ]]; then
                               ref_files+=($file)
                           fi
                       done
                 fi
             done 
         fi
 
    elif [[ $CUST_NAME == *'ipnext_mrr'* ]]; then
        IFS="/" read -r -a path <<< "${newarr[0]}" 
        INPUT_DIR=${newarr[0]%%"/${path[-2]}"*}
 
        for path in "$INPUT_DIR"/*; do
            if [[ $path = *'BN_RADETH'* ]]; then
                for file in "$path"/*; do
                    if [[ $file = *'KPI_MRR_DS'* && $file == *'.MF4'* ]]; then
                        deb_files+=($file)
                    fi
                done
             fi   
        done
        if [[ $CUST_NAME == *'dgps'* ]]; then
            for path in "$INPUT_DIR"/*; do        
                if [[ $path = *'MT_RE'* ]]; then
                       for file in "$path"/*; do
                           if [[ $file = *'KPI_MRR_DS'* && $file == *'.MF4'* ]]; then
                               ref_files+=($file)
                           fi
                       done
                 fi
             done 
         fi
    fi

fi

log_name=${newarr[0]}


for i in "${!deb_files[@]}"; do
   if [[ "${deb_files[$i]}" = "${log_name}" ]]; then

       start_log=${i};
       end_log=$(( $start_log + ${newarr[1]} - 1 ));          
       break;

   fi

done

mkdir -p $TEMP_PATH/LOGS
#mkdir -p $TEMP_PATH/RESIM-RESULTS

echo "[Splitter_Execution] : start log is ${deb_files[$start_log]}"
echo "[Splitter_Execution] : end log is ${deb_files[$end_log]}"

for (( i=$start_log; i<=$end_log; i++ )); do

if [[ $CUST_NAME == *'scale1'* ]]; then

    cp ${deb_files[$i]} $TEMP_PATH/LOGS
    deb_logs+=($(basename "${deb_files[$i]}"))
    cp ${bus_files[$i]} $TEMP_PATH/LOGS
    bus_logs+=($(basename "${bus_files[$i]}"))

elif [[ $CUST_NAME == *'scale3'* ]]; then

    cp ${can_corner_bus_files[$i]} $TEMP_PATH/LOGS
    corner_bus_logs+=($(basename "${can_corner_bus_files[$i]}"))
    cp ${mPAD_files[$i]} $TEMP_PATH/LOGS
    mPAD_logs+=($(basename "${mPAD_files[$i]}"))
    cp ${deb_files[$i]} $TEMP_PATH/LOGS
    deb_logs+=($(basename "${deb_files[$i]}"))
    cp ${can_side_bus_files[$i]} $TEMP_PATH/LOGS
    side_bus_logs+=($(basename "${can_side_bus_files[$i]}"))

elif [[ $CUST_NAME == *'traton'* ]]; then

    cp ${deb_files[$i]} $TEMP_PATH/LOGS
    deb_logs+=($(basename "${deb_files[$i]}"))

elif [[ $CUST_NAME == *'honda'* ]]; then

    cp ${deb_files[$i]} $TEMP_PATH/LOGS
    deb_logs+=($(basename "${deb_files[$i]}"))

elif [[ $CUST_NAME == *'rnasdv'* ]]; then

    cp ${deb_files[$i]} $TEMP_PATH/LOGS
    deb_logs+=($(basename "${deb_files[$i]}"))

elif [[ $CUST_NAME == *'gpo-gen7'* ]]; then

    cp ${deb_files[$i]} $TEMP_PATH/LOGS
    deb_logs+=($(basename "${deb_files[$i]}"))

elif [[ $CUST_NAME == *'dc'* ]]; then
    cp ${deb_files[$i]} $TEMP_PATH/LOGS
    deb_logs+=($(basename "${deb_files[$i]}"))
        if [[ $CUST_NAME == *'dgps'* ]]; then
            cp ${ref_files[$i]} $TEMP_PATH/LOGS
            ref_logs+=($(basename "${ref_files[$i]}"))
        fi

fi
done


POPULATE_FILES(){

echo "		{"  >> $input_file
#echo "" >> $input_file
echo "  		\"key\": \"$1\"," >> $input_file
#echo "" >> $input_file
echo '			"files": [ ' >> $input_file
#echo "" >> $input_file


count=1	

for file in "${@:2}"; do

  if [[ "$count" -lt "${newarr[1]}" ]]; then
    echo "		                    \"$TEMP_PATH/LOGS/$file\"," >> $input_file    
  else
    echo "		                    \"$TEMP_PATH/LOGS/$file\" " >> $input_file    
   fi
    count=$((count + 1))
done

echo "			]" >> $input_file
if [[ $1 != "SRR_REFERENCE" ]]; then
	echo "		},"  >> $input_file
	#echo "" >> $input_file
else
	echo "		}"  >> $input_file
	#echo "" >> $input_file

fi

#POPULATE_FILES() function closes here
}

echo "{" > $input_file
echo '  "reprocessingInputFileStreams": ' >> $input_file
#echo "" >> $input_file
echo "	["  >> $input_file
#echo "" >> $input_file

if [[ $CUST_NAME == *'scale1'* ]]; then
    POPULATE_FILES "BN_CALIFR" "${bus_logs[@]}" 
    POPULATE_FILES "BN_FASETH" "${bus_logs[@]}" 
    POPULATE_FILES "SRR_DEBUG" "${deb_logs[@]}"
    POPULATE_FILES "SRR_REFERENCE"
elif [[ $CUST_NAME == *'scale3'* || $CUST_NAME == *'scale4'* ]]; then
    POPULATE_FILES "Resim_Radars_CAN_Corner_bus" "${corner_bus_logs[@]}" 
    POPULATE_FILES "Aptiv_FLR_Obj_Perc_for_mPAD" "${mPAD_logs[@]}" 
    POPULATE_FILES "Resim_Radars_deb" "${deb_logs[@]}"  
    POPULATE_FILES "Resim_Radars_CAN_Side_bus" "${side_bus_logs[@]}"  
    POPULATE_FILES "SRR_REFERENCE"
elif [[ $CUST_NAME == *'dc'* ]]; then
    POPULATE_FILES "BN_CALIFR" 
    POPULATE_FILES "BN_FASETH" "${deb_logs[@]}" 
    POPULATE_FILES "SRR_DEBUG"  
    if [[ $CUST_NAME == *'dgps'* ]]; then
        POPULATE_FILES "SRR_REFERENCE" "${ref_logs[@]}"
    else
        POPULATE_FILES "SRR_REFERENCE" "${deb_logs[@]}"
    fi 
else
    POPULATE_FILES "BN_CALIFR" "${deb_logs[@]}" 
    POPULATE_FILES "BN_FASETH" "${deb_logs[@]}" 
    POPULATE_FILES "SRR_DEBUG" "${deb_logs[@]}"  
    POPULATE_FILES "SRR_REFERENCE"
fi 

#echo "" >> $input_file
echo "	]"  >> $input_file
#echo "" >> $input_file
echo "}"  >> $input_file
#echo "" >> $input_file



 