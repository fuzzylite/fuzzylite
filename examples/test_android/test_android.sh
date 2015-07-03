#!/bin/bash

ADB=adb
ARCH=armeabi-v7a
DEVICE=/data/local/tmp

echo ndk-build testfuzzylite
$(NDK_ROOT)/ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./test_android.mk APP_ABI=$(ARCH) APP_PLATFORM=android-15 APP_STL=gnustl_shared APP_PIE=true

echo deploy file to device
$(ADB) push libs/$(ARCH)/libfuzzylite.so $(DEVICE)
$(ADB) push libs/$(ARCH)/libgnustl_shared.so $(DEVICE)
$(ADB) push libs/$(ARCH)/testfuzzylite $(DEVICE)

echo run test on device
$(ADB) shell chmod 751 $(DEVICE)/testfuzzylite
$(ADB) shell LD_LIBRARY_PATH=$(DEVICE) $(DEVICE)/testfuzzylite

echo run test done!
