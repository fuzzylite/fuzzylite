LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := fuzzylite

ifeq ($(HOST_OS), windows)
cat := type
else
cat := cat
endif

fl_sources := $(shell $(cat) FL_SOURCES)

LOCAL_SRC_FILES := $(fl_sources)

LOCAL_CPPFLAGS += -DFL_VERSION=\"6.0\" -DFL_DATE=\"2015\" -DFL_BACKTRACE_OFF -pedantic -Wall -Wextra -DFL_EXPORT_LIBRARY

# Notice: execinfo.h and backtrace() only work for glibc, unsupported by Android bionic
# TODO: support backtrace by http://stackoverflow.com/questions/8115192/android-ndk-getting-the-backtrace
# Add -DFL_USE_FLOAT if you need using "typedef float fl::scalar" instead of "typedef double fl::scalar"
# Add -std=c++11 -DFL_CPP11 if you need C++11 features
# TODO: Add -Wno-non-literal-null-conversion when using Clang Compiler
# Enable FL_IMPORT_LIBRARY when using for BUILD_EXECUTABLE
# Enable FL_EXPORT_LIBRARY when using for BUILD_SHARED_LIBRARY
# Disable FL_IMPORT_LIBRARY and FL_EXPORT_LIBRARY when using for BUILD_STATIC_LIBRARY

LOCAL_CPP_FEATURES := rtti exceptions

include $(BUILD_SHARED_LIBRARY)
