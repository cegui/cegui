LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := unloadPreloadedConflictingNativeLibs
LOCAL_SRC_FILES := main.cpp
LOCAL_C_INCLUDES := $(ceguiInstallDir)/include/cegui-$(ceguiVerMajor)
include $(BUILD_SHARED_LIBRARY)
