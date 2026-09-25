#!/bin/bash
directory=$1
cd ../../../
cp -f bazel-bin/sil/emb_lib/sil_source/emb_lib_sharedlib.so $directory/libFLR8_SIL_FC.so
cd sil/emb_lib/build_bin
