#!/bin/sh

# Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

USER=$(whoami)
OBJ=obj

BASE_PATH=${1:-/Users/$USER/}

#SDK_VERSION=3.0.1
SDK_VERSION=1.0.0

#BUILD_OS=macos
BUILD_OS=linux

set -e

#DEBUG=-ggdb
DEBUG=

$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/gcc-arm-none-eabi/9.2.1/$BUILD_OS/bin/arm-none-eabi-g++ \
-I$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/nuttx/include/libcxx \
-I$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/nuttx/include/newlib \
-I$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/nuttx/include \
-I$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/nuttx/arch \
-I$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/nuttx/arch/chip \
-I$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/sdk/include \
-I$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/sdk/modules/include \
-I$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/sdk/apps/include \
-I$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/sdk/system/include \
-I$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/sdk/externals/include \
-I$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/sdk/externals/include/cmsis \
-I$BASE_PATH/Library/Arduino15/packages/SPRESENSE/hardware/spresense/$SDK_VERSION/variants/spresense \
-I$BASE_PATH/Library/Arduino15/packages/SPRESENSE/hardware/spresense/$SDK_VERSION/libraries/SDHCI/src \
-I$BASE_PATH/Library/Arduino15/packages/SPRESENSE/hardware/spresense/$SDK_VERSION/libraries/Storage/src \
-I$BASE_PATH/Library/Arduino15/packages/SPRESENSE/hardware/spresense/$SDK_VERSION/libraries/File/src \
-I$BASE_PATH/Library/Arduino15/packages/SPRESENSE/hardware/spresense/$SDK_VERSION/libraries/Audio \
-I$BASE_PATH/Library/Arduino15/packages/SPRESENSE/hardware/spresense/$SDK_VERSION/libraries/MemoryUtil \
-DCONFIG_WCHAR_BUILTIN -DCONFIG_HAVE_DOUBLE -D__NuttX__ -c -w -MMD -std=gnu++11 -c -fno-builtin -mabi=aapcs -Wall -Os -fno-strict-aliasing -fno-strength-reduce -fomit-frame-pointer -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -pipe -ffunction-sections -fno-exceptions -fno-rtti $DEBUG -gdwarf-3 -ffunction-sections -fdata-sections -fpermissive \
-DF_CPU=156000000L -DARDUINO=10819 -DARDUINO_spresense_ast -DARDUINO_ARCH_SPRESENSE "-DARDUINO_BOARD=\"spresense_ast\"" \
-I$BASE_PATH/Library/Arduino15/packages/SPRESENSE/hardware/spresense/$SDK_VERSION/cores/spresense \
-I$BASE_PATH/Library/Arduino15/packages/SPRESENSE/hardware/spresense/$SDK_VERSION/variants/spresense \
-I$BASE_PATH/Library/Arduino15/packages/SPRESENSE/hardware/spresense/$SDK_VERSION/libraries/SPI \
-I$BASE_PATH/Documents/Arduino/libraries/Adafruit-GFX-Library \
-I$BASE_PATH/Documents/Arduino/libraries/Adafruit_ILI9341 \
-I../IchigoJam_BASIC/ -I./src/ ../IchigoJam_BASIC/main.c -o main.o

# $BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-tools/$SDK_VERSION/getspkinfo/$BUILD_OS/getspkinfo ichigojam-spr.spk
# info size check
# $BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/gcc-arm-none-eabi/9.2.1/$BUILD_OS/bin/arm-none-eabi-size -A main.elf

# write
# sh write.sh

$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/gcc-arm-none-eabi/9.2.1/$BUILD_OS/bin/arm-none-eabi-g++ \
$DEBUG -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Xlinker --entry=__start -nodefaultlibs -nostartfiles -Wl,--defsym,__reserved_ramsize=1572864-786432 \
-T$BASE_PATH/Library/Arduino15/packages/SPRESENSE/hardware/spresense/$SDK_VERSION/variants/spresense/ramconfig.ld -Wl,--gc-sections -Xlinker \
-Map=$OBJ/output.map -o main.elf -Wl,--start-group main.o \
$OBJ/SPI.cpp.o \
$OBJ/glcdfont.c.o \
$OBJ/Adafruit_GFX.cpp.o \
$OBJ/Adafruit_SPITFT.cpp.o \
$OBJ/Adafruit_ILI9341.cpp.o \
$OBJ/SDHCI.cpp.o \
$OBJ/Storage.cpp.o \
$OBJ/File.cpp.o \
$OBJ/FrontEnd.cpp.o \
$OBJ/Audio.cpp.o \
$OBJ/MediaPlayer.cpp.o \
$OBJ/OutputMixer.cpp.o \
$OBJ/MediaRecorder.cpp.o \
$OBJ/MemoryUtil.cpp.o \
-u spresense_main $OBJ/core.a $BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/nuttx/libs/libapps.a \
$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/nuttx/libs/libarch.a \
$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/nuttx/libs/libarm_cortexM4lf_math.a \
$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/nuttx/libs/libaudio.a \
$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/nuttx/libs/libbinfmt.a \
$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/nuttx/libs/libboard.a \
$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/nuttx/libs/libboards.a \
$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/nuttx/libs/libc.a \
$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/nuttx/libs/libcmsis_nn.a \
$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/nuttx/libs/libdrivers.a \
$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/nuttx/libs/libfs.a \
$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/nuttx/libs/libmm.a \
$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/nuttx/libs/libnet.a \
$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/nuttx/libs/libnnablart.a \
$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/nuttx/libs/libsched.a \
$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/nuttx/libs/libsslutils.a \
$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/nuttx/libs/libxx.a \
$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-sdk/$SDK_VERSION/spresense/release/nuttx/libs/libmossfw.a \
-lgcc -lm -lsupc++_nano -Wl,--end-group -L$OBJ

$BASE_PATH/Library/Arduino15/packages/SPRESENSE/tools/spresense-tools/$SDK_VERSION/mkspk/$BUILD_OS/mkspk -c 2 main.elf nuttx ichigojam-spr.spk

# info