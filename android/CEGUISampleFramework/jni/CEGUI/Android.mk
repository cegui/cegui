LOCAL_PATH := $(call my-dir)

################################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := CEGUIBase
LOCAL_SRC_FILES := $(ceguiInstallDir)/lib/libCEGUIBase-$(ceguiVerMajor).so
LOCAL_EXPORT_C_INCLUDES := $(ceguiInstallDir)/include/cegui-$(ceguiVerMajor)
include $(PREBUILT_SHARED_LIBRARY)

################################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := CEGUICommonDialogs
LOCAL_SRC_FILES := $(ceguiInstallDir)/lib/libCEGUICommonDialogs-$(ceguiVerMajor).so
include $(PREBUILT_SHARED_LIBRARY)

################################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := CEGUICoreWindowRendererSet
LOCAL_SRC_FILES := $(ceguiInstallDir)/lib/cegui-$(ceguiVerMajorMinor)/libCEGUICoreWindowRendererSet.so
include $(PREBUILT_SHARED_LIBRARY)

################################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := CEGUIOpenGLRenderer
LOCAL_SRC_FILES := $(ceguiInstallDir)/lib/libCEGUIOpenGLRenderer-$(ceguiVerMajor).so
include $(PREBUILT_SHARED_LIBRARY)

################################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := CEGUISampleFramework
LOCAL_SRC_FILES := $(ceguiInstallDir)/lib/libCEGUISampleFramework-$(ceguiVerMajorMinor).so
include $(PREBUILT_SHARED_LIBRARY)

################################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := CEGUIFreeImageImageCodec
LOCAL_SRC_FILES := $(ceguiInstallDir)/lib/cegui-$(ceguiVerMajorMinor)/libCEGUIFreeImageImageCodec.so
include $(PREBUILT_SHARED_LIBRARY)

################################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := CEGUITinyXMLParser
LOCAL_SRC_FILES := $(ceguiInstallDir)/lib/cegui-$(ceguiVerMajorMinor)/libCEGUITinyXMLParser.so
include $(PREBUILT_SHARED_LIBRARY)
