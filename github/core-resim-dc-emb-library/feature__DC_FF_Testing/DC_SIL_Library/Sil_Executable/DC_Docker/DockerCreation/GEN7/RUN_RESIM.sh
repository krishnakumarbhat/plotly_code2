#!/bin/sh


#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS AND INPUT PARAMETER INSIDE CONTAINER-------


ch_k=0
ch_f=0


if [ -f "$1" -a -d "$2" ]
then
echo " "
else
       ch_k=1
fi


if [ ! -f "$3" ]
then
       ch_f=1
fi


echo $ch_k
exitcode=1
#----------VALIDATING INPUT PARAMETERS-------------------------
if [ $ch_k -eq 1 ]
then
       echo INCORRECT FILE PATH 
       exit $exitcode
fi


#--------------VALIDATING WHICH PARAMETER IS FILE LIST AND RUNNING LOGS WITH INPUT PARAMETER-------------------- 


cd /OUTPUT/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:./
if [ $ch_f -eq 1 ]
then
        ./APT_SRR_RESIM /./sil_engine_config.xml $1 $2
else
        ./APT_SRR_RESIM $3 $1 $2
fi
exitcode=$?


if [ $exitcode -eq 0 ]
then
        echo Application Executed Successfully
        exit 0
else
        echo Application Executed With Error
        exit $exitcode
fi


#!/bin/sh


#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS AND INPUT PARAMETER INSIDE CONTAINER-------


ch_k=0
ch_f=0


if [ -f "$1" -a -d "$2" ]
then
echo " "
else
       ch_k=1
fi


if [ ! -f "$3" ]
then
       ch_f=1
fi


echo $ch_k
exitcode=1
#----------VALIDATING INPUT PARAMETERS-------------------------
if [ $ch_k -eq 1 ]
then
       echo INCORRECT FILE PATH 
       exit $exitcode
fi


#--------------VALIDATING WHICH PARAMETER IS FILE LIST AND RUNNING LOGS WITH INPUT PARAMETER-------------------- 


cd /OUTPUT/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:./
chmod +777 BMW_SRR5_Input.json DC_Lib_Control.xml Dockerfile Emb_Lib_Config.xml FW_Release GEN_LOG.sh INPUT_LOGS New Text Document.txt OUTPUT RUN_RESIM.sh SIL_Engine_Config.xml SIL_Input.txt SM_Config createImage_GEN7.sh create_base_image_GEN7.sh dc_031024.simg dc_031024.tar dc_base.tar rna_base.tar version.txt
if [ $ch_f -eq 1 ]
then
        ./APT_SRR_RESIM /./sil_engine_config.xml $1 $2
else
        ./APT_SRR_RESIM $3 $1 $2
fi
exitcode=$?


if [ $exitcode -eq 0 ]
then
        echo Application Executed Successfully
        exit 0
else
        echo Application Executed With Error
        exit $exitcode
fi


#!/bin/sh


#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS AND INPUT PARAMETER INSIDE CONTAINER-------


ch_k=0
ch_f=0


if [ -f "$1" -a -d "$2" ]
then
echo " "
else
       ch_k=1
fi


if [ ! -f "$3" ]
then
       ch_f=1
fi


echo $ch_k
exitcode=1
#----------VALIDATING INPUT PARAMETERS-------------------------
if [ $ch_k -eq 1 ]
then
       echo INCORRECT FILE PATH 
       exit $exitcode
fi


#--------------VALIDATING WHICH PARAMETER IS FILE LIST AND RUNNING LOGS WITH INPUT PARAMETER-------------------- 


cd /OUTPUT/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:./
if [ $ch_f -eq 1 ]
then
        ./APT_SRR_RESIM /./sil_engine_config.xml /./BMW_SRR5_Input.json /./OUTPUT $1 $2
else
        ./APT_SRR_RESIM $3 $1 $2
fi
exitcode=$?


if [ $exitcode -eq 0 ]
then
        echo Application Executed Successfully
        exit 0
else
        echo Application Executed With Error
        exit $exitcode
fi


#!/bin/sh


#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS AND INPUT PARAMETER INSIDE CONTAINER-------


ch_k=0
ch_f=0


if [ -f "$1" -a -d "$2" ]
then
echo " "
else
       ch_k=1
fi


if [ ! -f "$3" ]
then
       ch_f=1
fi


echo $ch_k
exitcode=1
#----------VALIDATING INPUT PARAMETERS-------------------------
if [ $ch_k -eq 1 ]
then
       echo INCORRECT FILE PATH 
       exit $exitcode
fi


#--------------VALIDATING WHICH PARAMETER IS FILE LIST AND RUNNING LOGS WITH INPUT PARAMETER-------------------- 


cd /OUTPUT/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:./
chmod +777 BMW_SRR5_Input.json DC_Lib_Control.xml Dockerfile Emb_Lib_Config.xml FW_Release GEN_LOG.sh INPUT_LOGS New Text Document.txt OUTPUT RUN_RESIM.sh SIL_Engine_Config.xml SIL_Input.txt SM_Config createImage_GEN7.sh create_base_image_GEN7.sh dc_031024.simg dc_031024.tar dc_base.tar rna_base.tar version.txt
if [ $ch_f -eq 1 ]
then
        ./APT_SRR_RESIM /./sil_engine_config.xml /./BMW_SRR5_Input.json /./OUTPUT $1 $2
else
        ./APT_SRR_RESIM $3 $1 $2
fi
exitcode=$?


if [ $exitcode -eq 0 ]
then
        echo Application Executed Successfully
        exit 0
else
        echo Application Executed With Error
        exit $exitcode
fi


#!/bin/sh


#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS AND INPUT PARAMETER INSIDE CONTAINER-------


ch_k=0
ch_f=0


if [ -f "$1" -a -d "$2" ]
then
echo " "
else
       ch_k=1
fi


if [ ! -f "$3" ]
then
       ch_f=1
fi


echo $ch_k
exitcode=1
#----------VALIDATING INPUT PARAMETERS-------------------------
if [ $ch_k -eq 1 ]
then
       echo INCORRECT FILE PATH 
       exit $exitcode
fi


#--------------VALIDATING WHICH PARAMETER IS FILE LIST AND RUNNING LOGS WITH INPUT PARAMETER-------------------- 


cd /OUTPUT/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:./
chmod +777 BMW_SRR5_Input.json DC_Lib_Control.xml Dockerfile Emb_Lib_Config.xml FW_Release GEN_LOG.sh INPUT_LOGS New Text Document.txt OUTPUT RUN_RESIM.sh SIL_Engine_Config.xml SIL_Input.txt SM_Config createImage_GEN7.sh create_base_image_GEN7.sh dc_031024.simg dc_031024.tar dc_base.tar rna_base.tar version.txt

if [ $ch_f -eq 1 ]
then
        ./APT_SRR_RESIM /./sil_engine_config.xml /./BMW_SRR5_Input.json /./OUTPUT $1 $2
else
        ./APT_SRR_RESIM $3 $1 $2
fi
exitcode=$?


if [ $exitcode -eq 0 ]
then
        echo Application Executed Successfully
        exit 0
else
        echo Application Executed With Error
        exit $exitcode
fi


#!/bin/sh


#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS AND INPUT PARAMETER INSIDE CONTAINER-------


ch_k=0
ch_f=0


if [ -f "$1" -a -d "$2" ]
then
echo " "
else
       ch_k=1
fi


if [ ! -f "$3" ]
then
       ch_f=1
fi


echo $ch_k
exitcode=1
#----------VALIDATING INPUT PARAMETERS-------------------------
if [ $ch_k -eq 1 ]
then
       echo INCORRECT FILE PATH 
       exit $exitcode
fi


#--------------VALIDATING WHICH PARAMETER IS FILE LIST AND RUNNING LOGS WITH INPUT PARAMETER-------------------- 


cd /OUTPUT/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:./
chmod +777 BMW_SRR5_Input.json DC_Lib_Control.xml Dockerfile Emb_Lib_Config.xml FW_Release GEN_LOG.sh INPUT_LOGS New Text Document.txt OUTPUT RUN_RESIM.sh SIL_Engine_Config.xml SIL_Input.txt SM_Config createImage_GEN7.sh create_base_image_GEN7.sh dc_031024.simg dc_031024.tar dc_base.tar rna_base.tar version.txt
if [ $ch_f -eq 1 ]
then
        ./APT_SRR_RESIM /./sil_engine_config.xml /./BMW_SRR5_Input.json /./OUTPUT $1 $2
else
        ./APT_SRR_RESIM $3 $1 $2
fi
exitcode=$?


if [ $exitcode -eq 0 ]
then
        echo Application Executed Successfully
        exit 0
else
        echo Application Executed With Error
        exit $exitcode
fi


#!/bin/sh


#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS AND INPUT PARAMETER INSIDE CONTAINER-------


ch_k=0
ch_f=0


if [ -f "$1" -a -d "$2" ]
then
echo " "
else
       ch_k=1
fi


if [ ! -f "$3" ]
then
       ch_f=1
fi


echo $ch_k
exitcode=1
#----------VALIDATING INPUT PARAMETERS-------------------------
if [ $ch_k -eq 1 ]
then
       echo INCORRECT FILE PATH 
       exit $exitcode
fi


#--------------VALIDATING WHICH PARAMETER IS FILE LIST AND RUNNING LOGS WITH INPUT PARAMETER-------------------- 


cd /OUTPUT/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:./
chmod +777 BMW_SRR5_Input.json DC_Lib_Control.xml Dockerfile Emb_Lib_Config.xml FW_Release GEN_LOG.sh INPUT_LOGS New Text Document.txt OUTPUT RUN_RESIM.sh SIL_Engine_Config.xml SIL_Input.txt SM_Config createImage_GEN7.sh create_base_image_GEN7.sh dc_031024.simg dc_031024.tar dc_base.tar rna_base.tar version.txt
if [ $ch_f -eq 1 ]
then
        ./APT_SRR_RESIM /./sil_engine_config.xml /./BMW_SRR5_Input.json /./OUTPUT $1 $2
else
        ./APT_SRR_RESIM $3 $1 $2
fi
exitcode=$?


if [ $exitcode -eq 0 ]
then
        echo Application Executed Successfully
        exit 0
else
        echo Application Executed With Error
        exit $exitcode
fi


#!/bin/sh


#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS AND INPUT PARAMETER INSIDE CONTAINER-------


ch_k=0
ch_f=0


if [ -f "$1" -a -d "$2" ]
then
echo " "
else
       ch_k=1
fi


if [ ! -f "$3" ]
then
       ch_f=1
fi


echo $ch_k
exitcode=1
#----------VALIDATING INPUT PARAMETERS-------------------------
if [ $ch_k -eq 1 ]
then
       echo INCORRECT FILE PATH 
       exit $exitcode
fi


#--------------VALIDATING WHICH PARAMETER IS FILE LIST AND RUNNING LOGS WITH INPUT PARAMETER-------------------- 


cd /OUTPUT/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:./
chmod +777 BMW_SRR5_Input.json DC_Lib_Control.xml Dockerfile Emb_Lib_Config.xml FW_Release GEN_LOG.sh INPUT_LOGS New Text Document.txt OUTPUT RUN_RESIM.sh SIL_Engine_Config.xml SIL_Input.txt SM_Config createImage_GEN7.sh create_base_image_GEN7.sh dc_031024.simg dc_031024.tar dc_base.tar rna_base.tar version.txt
if [ $ch_f -eq 1 ]
then
        ./APT_SRR_RESIM /./sil_engine_config.xml /./BMW_SRR5_Input.json /./OUTPUT $1 $2
else
        ./APT_SRR_RESIM $3 $1 $2
fi
exitcode=$?


if [ $exitcode -eq 0 ]
then
        echo Application Executed Successfully
        exit 0
else
        echo Application Executed With Error
        exit $exitcode
fi


#!/bin/sh


#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS AND INPUT PARAMETER INSIDE CONTAINER-------


ch_k=0
ch_f=0


if [ -f "$1" -a -d "$2" ]
then
echo " "
else
       ch_k=1
fi


if [ ! -f "$3" ]
then
       ch_f=1
fi


echo $ch_k
exitcode=1
#----------VALIDATING INPUT PARAMETERS-------------------------
if [ $ch_k -eq 1 ]
then
       echo INCORRECT FILE PATH 
       exit $exitcode
fi


#--------------VALIDATING WHICH PARAMETER IS FILE LIST AND RUNNING LOGS WITH INPUT PARAMETER-------------------- 


cd /OUTPUT/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:./
chmod +777 BMW_SRR5_Input.json DC_Lib_Control.xml Dockerfile Emb_Lib_Config.xml FW_Release GEN_LOG.sh INPUT_LOGS New Text Document.txt OUTPUT RUN_RESIM.sh SIL_Engine_Config.xml SIL_Input.txt SM_Config createImage_GEN7.sh create_base_image_GEN7.sh dc_031024.simg dc_031024.tar dc_base.tar rna_base.tar version.txt
if [ $ch_f -eq 1 ]
then
        ./APT_SRR_RESIM /./sil_engine_config.xml /./BMW_SRR5_Input.json /./OUTPUT $1 $2
else
        ./APT_SRR_RESIM $3 $1 $2
fi
exitcode=$?


if [ $exitcode -eq 0 ]
then
        echo Application Executed Successfully
        exit 0
else
        echo Application Executed With Error
        exit $exitcode
fi


#!/bin/sh


#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS AND INPUT PARAMETER INSIDE CONTAINER-------


ch_k=0
ch_f=0


if [ -f "$1" -a -d "$2" ]
then
echo " "
else
       ch_k=1
fi


if [ ! -f "$3" ]
then
       ch_f=1
fi


echo $ch_k
exitcode=1
#----------VALIDATING INPUT PARAMETERS-------------------------
if [ $ch_k -eq 1 ]
then
       echo INCORRECT FILE PATH 
       exit $exitcode
fi


#--------------VALIDATING WHICH PARAMETER IS FILE LIST AND RUNNING LOGS WITH INPUT PARAMETER-------------------- 


cd /OUTPUT/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:./
chmod +777 BMW_SRR5_Input.json DC_Lib_Control.xml Dockerfile Emb_Lib_Config.xml FW_Release GEN_LOG.sh INPUT_LOGS New Text Document.txt OUTPUT RUN_RESIM.sh SIL_Engine_Config.xml SIL_Input.txt SM_Config createImage_GEN7.sh create_base_image_GEN7.sh dc_031024.simg dc_031024.tar dc_base.tar rna_base.tar version.txt
if [ $ch_f -eq 1 ]
then
        ./APT_SRR_RESIM /./sil_engine_config.xml /./BMW_SRR5_Input.json /./OUTPUT $1 $2
else
        ./APT_SRR_RESIM $3 $1 $2
fi
exitcode=$?


if [ $exitcode -eq 0 ]
then
        echo Application Executed Successfully
        exit 0
else
        echo Application Executed With Error
        exit $exitcode
fi


#!/bin/sh


#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS AND INPUT PARAMETER INSIDE CONTAINER-------


ch_k=0
ch_f=0


if [ -f "$1" -a -d "$2" ]
then
echo " "
else
       ch_k=1
fi


if [ ! -f "$3" ]
then
       ch_f=1
fi


echo $ch_k
exitcode=1
#----------VALIDATING INPUT PARAMETERS-------------------------
if [ $ch_k -eq 1 ]
then
       echo INCORRECT FILE PATH 
       exit $exitcode
fi


#--------------VALIDATING WHICH PARAMETER IS FILE LIST AND RUNNING LOGS WITH INPUT PARAMETER-------------------- 


cd /OUTPUT/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:./
chmod +777 BMW_SRR5_Input.json DC_Lib_Control.xml Dockerfile Emb_Lib_Config.xml FW_Release GEN_LOG.sh INPUT_LOGS New Text Document.txt OUTPUT RUN_RESIM.sh SIL_Engine_Config.xml SIL_Input.txt SM_Config createImage_GEN7.sh create_base_image_GEN7.sh dc_031024.simg dc_031024.tar dc_base.tar rna_base.tar version.txt
if [ $ch_f -eq 1 ]
then
        ./APT_SRR_RESIM /./sil_engine_config.xml /./BMW_SRR5_Input.json /./OUTPUT $1 $2
else
        ./APT_SRR_RESIM $3 $1 $2
fi
exitcode=$?


if [ $exitcode -eq 0 ]
then
        echo Application Executed Successfully
        exit 0
else
        echo Application Executed With Error
        exit $exitcode
fi


#!/bin/sh


#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS AND INPUT PARAMETER INSIDE CONTAINER-------


ch_k=0
ch_f=0


if [ -f "$1" -a -d "$2" ]
then
echo " "
else
       ch_k=1
fi


if [ ! -f "$3" ]
then
       ch_f=1
fi


echo $ch_k
exitcode=1
#----------VALIDATING INPUT PARAMETERS-------------------------
if [ $ch_k -eq 1 ]
then
       echo INCORRECT FILE PATH 
       exit $exitcode
fi


#--------------VALIDATING WHICH PARAMETER IS FILE LIST AND RUNNING LOGS WITH INPUT PARAMETER-------------------- 


cd /OUTPUT/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:./
if [ $ch_f -eq 1 ]
then
        ./APT_SRR_RESIM /./sil_engine_config.xml $1 $2
else
        ./APT_SRR_RESIM $3 $1 $2
fi
exitcode=$?


if [ $exitcode -eq 0 ]
then
        echo Application Executed Successfully
        exit 0
else
        echo Application Executed With Error
        exit $exitcode
fi


#!/bin/sh


#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS AND INPUT PARAMETER INSIDE CONTAINER-------


ch_k=0
ch_f=0


if [ -f "$1" -a -d "$2" ]
then
echo " "
else
       ch_k=1
fi


if [ ! -f "$3" ]
then
       ch_f=1
fi


echo $ch_k
exitcode=1
#----------VALIDATING INPUT PARAMETERS-------------------------
if [ $ch_k -eq 1 ]
then
       echo INCORRECT FILE PATH 
       exit $exitcode
fi


#--------------VALIDATING WHICH PARAMETER IS FILE LIST AND RUNNING LOGS WITH INPUT PARAMETER-------------------- 


cd /OUTPUT/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:./
if [ $ch_f -eq 1 ]
then
        ./APT_SRR_RESIM /./sil_engine_config.xml $1 $2
else
        ./APT_SRR_RESIM $3 $1 $2
fi
exitcode=$?


if [ $exitcode -eq 0 ]
then
        echo Application Executed Successfully
        exit 0
else
        echo Application Executed With Error
        exit $exitcode
fi


#!/bin/sh


#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS AND INPUT PARAMETER INSIDE CONTAINER-------


ch_k=0
ch_f=0


if [ -f "$1" -a -d "$2" ]
then
echo " "
else
       ch_k=1
fi


if [ ! -f "$3" ]
then
       ch_f=1
fi


echo $ch_k
exitcode=1
#----------VALIDATING INPUT PARAMETERS-------------------------
if [ $ch_k -eq 1 ]
then
       echo INCORRECT FILE PATH 
       exit $exitcode
fi


#--------------VALIDATING WHICH PARAMETER IS FILE LIST AND RUNNING LOGS WITH INPUT PARAMETER-------------------- 


cd /OUTPUT/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:./
if [ $ch_f -eq 1 ]
then
        ./APT_SRR_RESIM /./sil_engine_config.xml $1 $2
else
        ./APT_SRR_RESIM $3 $1 $2
fi
exitcode=$?


if [ $exitcode -eq 0 ]
then
        echo Application Executed Successfully
        exit 0
else
        echo Application Executed With Error
        exit $exitcode
fi


#!/bin/sh


#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS AND INPUT PARAMETER INSIDE CONTAINER-------


ch_k=0
ch_f=0


if [ -f "$1" -a -d "$2" ]
then
echo " "
else
       ch_k=1
fi


if [ ! -f "$3" ]
then
       ch_f=1
fi


echo $ch_k
exitcode=1
#----------VALIDATING INPUT PARAMETERS-------------------------
if [ $ch_k -eq 1 ]
then
       echo INCORRECT FILE PATH 
       exit $exitcode
fi


#--------------VALIDATING WHICH PARAMETER IS FILE LIST AND RUNNING LOGS WITH INPUT PARAMETER-------------------- 


cd /OUTPUT/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:./
if [ $ch_f -eq 1 ]
then
        ./APT_SRR_RESIM /./sil_engine_config.xml $1 $2
else
        ./APT_SRR_RESIM $3 $1 $2
fi
exitcode=$?


if [ $exitcode -eq 0 ]
then
        echo Application Executed Successfully
        exit 0
else
        echo Application Executed With Error
        exit $exitcode
fi


#!/bin/sh


#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS AND INPUT PARAMETER INSIDE CONTAINER-------


ch_k=0
ch_f=0


if [ -f "$1" -a -d "$2" ]
then
echo " "
else
       ch_k=1
fi


if [ ! -f "$3" ]
then
       ch_f=1
fi


echo $ch_k
exitcode=1
#----------VALIDATING INPUT PARAMETERS-------------------------
if [ $ch_k -eq 1 ]
then
       echo INCORRECT FILE PATH 
       exit $exitcode
fi


#--------------VALIDATING WHICH PARAMETER IS FILE LIST AND RUNNING LOGS WITH INPUT PARAMETER-------------------- 


cd /OUTPUT/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:./
chmod +777 BMW_SRR5_Input.json DC_Lib_Control.xml Dockerfile Emb_Lib_Config.xml FW_Release GEN_LOG.sh INPUT_LOGS New Text Document.txt OUTPUT RUN_RESIM.sh SIL_Engine_Config.xml SIL_Input.txt SM_Config createImage_GEN7.sh create_base_image_GEN7.sh dc_031024.simg dc_031024.tar dc_base.tar rna_base.tar version.txt
if [ $ch_f -eq 1 ]
then
        ./APT_SRR_RESIM /./sil_engine_config.xml $1 $2
else
        ./APT_SRR_RESIM $3 $1 $2
fi
exitcode=$?


if [ $exitcode -eq 0 ]
then
        echo Application Executed Successfully
        exit 0
else
        echo Application Executed With Error
        exit $exitcode
fi


#!/bin/sh


#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS AND INPUT PARAMETER INSIDE CONTAINER-------


ch_k=0
ch_f=0


if [ -f "$1" -a -d "$2" ]
then
echo " "
else
       ch_k=1
fi


if [ ! -f "$3" ]
then
       ch_f=1
fi


echo $ch_k
exitcode=1
#----------VALIDATING INPUT PARAMETERS-------------------------
if [ $ch_k -eq 1 ]
then
       echo INCORRECT FILE PATH 
       exit $exitcode
fi


#--------------VALIDATING WHICH PARAMETER IS FILE LIST AND RUNNING LOGS WITH INPUT PARAMETER-------------------- 


cd /OUTPUT/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:./
chmod +777 BMW_SRR5_Input.json DC_Lib_Control.xml Dockerfile Emb_Lib_Config.xml FW_Release GEN_LOG.sh INPUT_LOGS New Text Document.txt OUTPUT RUN_RESIM.sh SIL_Engine_Config.xml SIL_Input.txt SM_Config createImage_GEN7.sh create_base_image_GEN7.sh dc_031024.simg dc_031024.tar dc_base.tar rna_base.tar version.txt
if [ $ch_f -eq 1 ]
then
        ./APT_SRR_RESIM /./sil_engine_config.xml $1 $2
else
        ./APT_SRR_RESIM $3 $1 $2
fi
exitcode=$?


if [ $exitcode -eq 0 ]
then
        echo Application Executed Successfully
        exit 0
else
        echo Application Executed With Error
        exit $exitcode
fi


#!/bin/sh


#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS AND INPUT PARAMETER INSIDE CONTAINER-------


ch_k=0
ch_f=0


if [ -f "$1" -a -d "$2" ]
then
echo " "
else
       ch_k=1
fi


if [ ! -f "$3" ]
then
       ch_f=1
fi


echo $ch_k
exitcode=1
#----------VALIDATING INPUT PARAMETERS-------------------------
if [ $ch_k -eq 1 ]
then
       echo INCORRECT FILE PATH 
       exit $exitcode
fi


#--------------VALIDATING WHICH PARAMETER IS FILE LIST AND RUNNING LOGS WITH INPUT PARAMETER-------------------- 


cd /OUTPUT/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:./
chmod +777 BMW_SRR5_Input.json DC_Lib_Control.xml Dockerfile Emb_Lib_Config.xml FW_Release GEN_LOG.sh INPUT_LOGS New Text Document.txt OUTPUT RUN_RESIM.sh SIL_Engine_Config.xml SIL_Input.txt SM_Config createImage_GEN7.sh create_base_image_GEN7.sh dc_031024.simg dc_031024.tar dc_base.tar rna_base.tar version.txt
if [ $ch_f -eq 1 ]
then
        ./APT_SRR_RESIM /./sil_engine_config.xml $1 $2
else
        ./APT_SRR_RESIM $3 $1 $2
fi
exitcode=$?


if [ $exitcode -eq 0 ]
then
        echo Application Executed Successfully
        exit 0
else
        echo Application Executed With Error
        exit $exitcode
fi


#!/bin/sh


#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS AND INPUT PARAMETER INSIDE CONTAINER-------


ch_k=0
ch_f=0


if [ -f "$1" -a -d "$2" ]
then
echo " "
else
       ch_k=1
fi


if [ ! -f "$3" ]
then
       ch_f=1
fi


echo $ch_k
exitcode=1
#----------VALIDATING INPUT PARAMETERS-------------------------
if [ $ch_k -eq 1 ]
then
       echo INCORRECT FILE PATH 
       exit $exitcode
fi


#--------------VALIDATING WHICH PARAMETER IS FILE LIST AND RUNNING LOGS WITH INPUT PARAMETER-------------------- 


cd /OUTPUT/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:./
chmod +777 BMW_SRR5_Input.json DC_Lib_Control.xml Dockerfile Emb_Lib_Config.xml FW_Release GEN_LOG.sh INPUT_LOGS New Text Document.txt OUTPUT RUN_RESIM.sh SIL_Engine_Config.xml SIL_Input.txt SM_Config createImage_GEN7.sh create_base_image_GEN7.sh dc_031024.simg dc_031024.tar dc_base.tar rna_base.tar version.txt
if [ $ch_f -eq 1 ]
then
        ./APT_SRR_RESIM /./sil_engine_config.xml $1 $2
else
        ./APT_SRR_RESIM $3 $1 $2
fi
exitcode=$?


if [ $exitcode -eq 0 ]
then
        echo Application Executed Successfully
        exit 0
else
        echo Application Executed With Error
        exit $exitcode
fi


#!/bin/sh


#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS AND INPUT PARAMETER INSIDE CONTAINER-------


ch_k=0
ch_f=0


if [ -f "$1" -a -d "$2" ]
then
echo " "
else
       ch_k=1
fi


if [ ! -f "$3" ]
then
       ch_f=1
fi


echo $ch_k
exitcode=1
#----------VALIDATING INPUT PARAMETERS-------------------------
if [ $ch_k -eq 1 ]
then
       echo INCORRECT FILE PATH 
       exit $exitcode
fi


#--------------VALIDATING WHICH PARAMETER IS FILE LIST AND RUNNING LOGS WITH INPUT PARAMETER-------------------- 


cd /OUTPUT/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:./
chmod +777 BMW_SRR5_Input.json DC_Lib_Control.xml Dockerfile Emb_Lib_Config.xml FW_Release GEN_LOG.sh INPUT_LOGS New Text Document.txt OUTPUT RUN_RESIM.sh SIL_Engine_Config.xml SIL_Input.txt SM_Config createImage_GEN7.sh create_base_image_GEN7.sh dc_031024.simg dc_031024.tar dc_base.tar rna_base.tar version.txt
if [ $ch_f -eq 1 ]
then
        ./APT_SRR_RESIM /./sil_engine_config.xml $1 $2
else
        ./APT_SRR_RESIM $3 $1 $2
fi
exitcode=$?


if [ $exitcode -eq 0 ]
then
        echo Application Executed Successfully
        exit 0
else
        echo Application Executed With Error
        exit $exitcode
fi


#!/bin/sh


#------SCRIPT IS CALLED TO EXECUTE APPLICATION WITH LOGS AND INPUT PARAMETER INSIDE CONTAINER-------


ch_k=0
ch_f=0


if [ -f "$1" -a -d "$2" ]
then
echo " "
else
       ch_k=1
fi


if [ ! -f "$3" ]
then
       ch_f=1
fi


echo $ch_k
exitcode=1
#----------VALIDATING INPUT PARAMETERS-------------------------
if [ $ch_k -eq 1 ]
then
       echo INCORRECT FILE PATH 
       exit $exitcode
fi


#--------------VALIDATING WHICH PARAMETER IS FILE LIST AND RUNNING LOGS WITH INPUT PARAMETER-------------------- 


cd /OUTPUT/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:./
chmod +777 BMW_SRR5_Input.json DC_Lib_Control.xml Dockerfile Emb_Lib_Config.xml FW_Release GEN_LOG.sh INPUT_LOGS New Text Document.txt OUTPUT RUN_RESIM.sh SIL_Engine_Config.xml SIL_Input.txt SM_Config createImage_GEN7.sh create_base_image_GEN7.sh dc_031024.simg dc_031024.tar dc_base.tar rna_base.tar version.txt
if [ $ch_f -eq 1 ]
then
        ./APT_SRR_RESIM /./sil_engine_config.xml $1 $2
else
        ./APT_SRR_RESIM $3 $1 $2
fi
exitcode=$?


if [ $exitcode -eq 0 ]
then
        echo Application Executed Successfully
        exit 0
else
        echo Application Executed With Error
        exit $exitcode
fi


