@echo off

set ADB=adb.exe

echo ndk-build testfuzzylite
call %NDKROOTr10d%/ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./test_ndk.mk APP_ABI=armeabi-v7a,x86 APP_PLATFORM=android-15 APP_STL=gnustl_shared APP_PIE=true

echo deploy file to device
call %ADB% push libs\armeabi-v7a\libfuzzylite.so /data/local/tmp
call %ADB% push libs\armeabi-v7a\libgnustl_shared.so /data/local/tmp
call %ADB% push libs\armeabi-v7a\testfuzzylite /data/local/tmp

echo run test on device
call %ADB% shell chmod 751 /data/local/tmp/testfuzzylite
call %ADB% shell LD_LIBRARY_PATH=/data/local/tmp /data/local/tmp/testfuzzylite

echo run test done!
pause
