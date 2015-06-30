@echo off

set ADB=adb.exe
set ARCH=armeabi-v7a

echo ndk-build testfuzzylite
call %NDK_ROOT%/ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./test_android.mk APP_ABI=%ARCH% APP_PLATFORM=android-15 APP_STL=gnustl_shared APP_PIE=true

echo deploy file to device
call %ADB% push libs\%ARCH%\libfuzzylite.so /data/local/tmp
call %ADB% push libs\%ARCH%\libgnustl_shared.so /data/local/tmp
call %ADB% push libs\%ARCH%\testfuzzylite /data/local/tmp

echo run test on device
call %ADB% shell chmod 751 /data/local/tmp/testfuzzylite
call %ADB% shell LD_LIBRARY_PATH=/data/local/tmp /data/local/tmp/testfuzzylite

echo run test done!
pause
