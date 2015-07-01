@echo off

set ADB=adb.exe
set ARCH=armeabi-v7a
set DEVICE=/data/local/tmp

echo ndk-build testfuzzylite
call %NDK_ROOT%/ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./test_android.mk APP_ABI=%ARCH% APP_PLATFORM=android-15 APP_STL=gnustl_shared APP_PIE=true

echo deploy file to device
call %ADB% push libs\%ARCH%\libfuzzylite.so %DEVICE%
call %ADB% push libs\%ARCH%\libgnustl_shared.so %DEVICE%
call %ADB% push libs\%ARCH%\testfuzzylite %DEVICE%

echo run test on device
call %ADB% shell chmod 751 %DEVICE%/testfuzzylite
call %ADB% shell LD_LIBRARY_PATH=%DEVICE% %DEVICE%/testfuzzylite

echo run test done!
pause
