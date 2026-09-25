#!/bin/bash
directory=$1
cd ../../../
cp -f bazel-bin/sil/emb_lib/sil_source/emb_lib_sharedlib.so $directory/libSRR8_SiL_FL.so
cp -f bazel-bin/sil/emb_lib/sil_source/emb_lib_sharedlib.so $directory/libSRR8_SiL_FR.so
cp -f bazel-bin/sil/emb_lib/sil_source/emb_lib_sharedlib.so $directory/libSRR8_SiL_RL.so
cp -f bazel-bin/sil/emb_lib/sil_source/emb_lib_sharedlib.so $directory/libSRR8_SiL_RR.so
cd sil/emb_lib/build_bin
