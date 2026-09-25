#!/bin/bash

read -p "Enter CS version of RESIM Release Repo: " cs

cm xlink -e -rs ./AS_DSPACE /AS_dSPACE cs:$cs@10028634_01_SRR_RESIM_Release
cm xlink -e -rs ./FW_LIME /RESIM_SIL/LIME cs:$cs@10028634_01_SRR_RESIM_Release
cm xlink -e -rs ./RECU /RECU cs:$cs@10028634_01_SRR_RESIM_Release
cm xlink -e -rs ./RADAR_DLLS /RADAR_DLLS/BMW_SRR5 cs:$cs@10028634_01_SRR_RESIM_Release
