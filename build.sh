#!/bin/bash

export PATH=$PATH:"/home/michael/hardware_security/rasperrypi_3_optee/rpi/toolchains/aarch64/bin":"/home/michael/hardware_security/rasperrypi_3_optee/rpi/toolchains/aarch32/bin"

export TA_DEV_KIT_DIR="/home/michael/hardware_security/rasperrypi_3_optee/rpi/optee_os/out/arm/export-ta_arm64"

export TEEC_EXPORT="/home/michael/hardware_security/rasperrypi_3_optee/rpi/optee_client/out/export"

export HOST_CROSS_COMPILE=aarch64-linux-gnu-
export TA_CROSS_COMPILE=arm-linux-gnueabihf-
export LIBFUSE_HOST=aarch64-linux-gnu

mkdir -p out/

make O=./out HOST_CROSS_COMPILE=$HOST_CROSS_COMPILE TA_CROSS_COMPILE=$TA_CROSS_COMPILE LIBFUSE_HOST=$LIBFUSE_HOST $@
