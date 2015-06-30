LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := fuzzylite
LOCAL_SRC_FILES := $(LOCAL_PATH)/../libs/$(TARGET_ARCH_ABI)/libfuzzylite.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := testfuzzylite
LOCAL_SRC_FILES := test_ndk.cpp
LOCAL_SHARED_LIBRARIES := fuzzylite
LOCAL_CPP_FEATURES := rtti exceptions

include $(BUILD_EXECUTABLE)
