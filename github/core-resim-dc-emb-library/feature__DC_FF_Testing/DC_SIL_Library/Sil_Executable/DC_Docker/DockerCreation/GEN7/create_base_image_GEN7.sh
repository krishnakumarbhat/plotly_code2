#!/bin/sh

echo FROM        ubuntu   >>Dockerfile
echo MAINTAINER  aptiv  >>Dockerfile
echo ARG DEBIAN_FRONTEND=noninteractive >>Dockerfile
echo RUN apt-get update >>Dockerfile
echo RUN apt-get -yq install gdbserver >>Dockerfile
echo RUN apt-get -yq install python3 >>Dockerfile

docker build -t dc_base --network=host .

docker save -o dc_base.tar dc_base
