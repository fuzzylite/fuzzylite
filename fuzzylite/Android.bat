@echo off
call %NDK_ROOT%/ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk APP_ABI=armeabi-v7a APP_PLATFORM=android-15 APP_STL=gnustl_shared
pause
