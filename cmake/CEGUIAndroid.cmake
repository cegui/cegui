#/*******************************************************************************
#    Filename: CEGUIAndroid.cmake 
#    Created:  28/9/2014
#    Author:   David Reepmeyer <djreep81@gmail.com>
#*******************************************************************************/
#/***************************************************************************
# *   Copyright (C) 2004 - 2013 Paul D Turner & The CEGUI Development Team
# *
# *   Permission is hereby granted, free of charge, to any person obtaining
# *   a copy of this software and associated documentation files (the
# *   "Software"), to deal in the Software without restriction, including
# *   without limitation the rights to use, copy, modify, merge, publish,
# *   distribute, sublicense, and/or sell copies of the Software, and to
# *   permit persons to whom the Software is furnished to do so, subject to
# *   the following conditions:
# *
# *   The above copyright notice and this permission notice shall be
# *   included in all copies or substantial portions of the Software.
# *
# *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# *   OTHER DEALINGS IN THE SOFTWARE.
# ***************************************************************************/

macro(create_android_proj ANDROID_PROJECT_TARGET) 
    if (ANDROID)
        if(APPLE OR WIN32)
            SET(ANDROID_EXECUTABLE "android")
            SET(NDK_BUILD_EXECUTABLE "ndk-build")
        else()
            SET(ANDROID_EXECUTABLE "$ENV{ANDROID_SDK}/tools/android")
            SET(NDK_BUILD_EXECUTABLE "$ENV{ANDROID_NDK}/ndk-build")
        endif()

        SET(ANT_EXECUTABLE "ant")
        if(${ANDROID_NATIVE_API_LEVEL} LESS 14)
            MATH(EXPR ANDROID_SDK_API_LEVEL "${ANDROID_NATIVE_API_LEVEL}+1")
        else()
            SET(ANDROID_SDK_API_LEVEL "${ANDROID_NATIVE_API_LEVEL}")
            SET(SCREEN_SIZE "|screenSize")
        endif()

        SET(ANDROID_TARGET "android-${ANDROID_SDK_API_LEVEL}")

        SET(NDKOUT "${CMAKE_BINARY_DIR}/SampleBrowserNDK")
        file(MAKE_DIRECTORY "${NDKOUT}")
        file(MAKE_DIRECTORY "${NDKOUT}/jni")
        file(MAKE_DIRECTORY "${NDKOUT}/assets")
        file(MAKE_DIRECTORY "${NDKOUT}/res")
        file(WRITE "${NDKOUT}/default.properties" "target=${ANDROID_TARGET}")
        file(WRITE "${NDKOUT}/jni/Application.mk" 
            "APP_ABI := ${ANDROID_ABI}\nAPP_STL := gnustl_static\nAPP_CPPFLAGS := -fexceptions -frtti \nNDK_TOOLCHAIN_VERSION := ${ANDROID_COMPILER_VERSION}") 
        SET(ANDROID_MOD_NAME "${CEGUI_TARGET_NAME}")
        SET(JNI_PATH "${CMAKE_SOURCE_DIR}/samples/browser")
        foreach(COMBINED_SOURCE_FILE ${COMBINED_SOURCE_FILES})
            set(COMBINED_SOURCE_FILES_STRING "${COMBINED_SOURCE_FILES_STRING} ${COMBINED_SOURCE_FILE}")
        endforeach()
        SET(JNI_SRC_FILES "${COMBINED_SOURCE_FILES_STRING}")
        SET(PKG_NAME "cegui.org.uk.browser")
        SET(ANDROID_SHARED_LIB "${CEGUI_TARGET_NAME}")
        file(COPY "${CMAKE_SOURCE_DIR}/datafiles/imagesets/ic_launcher.png" DESTINATION "${NDKOUT}/res/drawable")
        file(COPY "${CMAKE_SOURCE_DIR}/datafiles" DESTINATION "${NDKOUT}/assets")
        if (CEGUI_SAMPLES_RENDERER_OGRE_ACTIVE)
            SET(ANDROID_GLES_VERSION "0x00020000")
            configure_file("${OGRE_TEMPLATES_DIR}/Android.OgreGLES2.mk.in" "${NDKOUT}/jni/Android.mk" @ONLY)
        elseif (CEGUI_SAMPLES_RENDERER_OPENGLES2_ACTIVE)
            if (CEGUI_BUILD_RENDERER_OPENGLES2_WITH_GLES3_SUPPORT)
                SET(ANDROID_GLES_VERSION "0x00030000")
                configure_file("${OGRE_TEMPLATES_DIR}/Android.GLES3.mk.in" "${NDKOUT}/jni/Android.mk" @ONLY)
            else()
                SET(ANDROID_GLES_VERSION "0x00020000")
                configure_file("${OGRE_TEMPLATES_DIR}/Android.GLES2.mk.in" "${NDKOUT}/jni/Android.mk" @ONLY)
            endif()
        endif()
        configure_file("${OGRE_TEMPLATES_DIR}/AndroidManifest.xml.in" "${NDKOUT}/AndroidManifest.xml" @ONLY)

        add_custom_command(
            TARGET ${ANDROID_PROJECT_TARGET} POST_BUILD COMMAND ${ANDROID_EXECUTABLE} 
            update project --name ${ANDROID_PROJECT_TARGET} --subprojects --target ${ANDROID_TARGET} --path "${NDKOUT}" WORKING_DIRECTORY ${NDKOUT})

        if(DEBUG)
            add_custom_command( TARGET ${ANDROID_PROJECT_TARGET} POST_BUILD COMMAND 
                ${NDK_BUILD_EXECUTABLE} all -j2 V=1 NDK_DEBUG=1 WORKING_DIRECTORY ${NDKOUT})
        else()
            add_custom_command( TARGET ${ANDROID_PROJECT_TARGET} POST_BUILD COMMAND 
                ${NDK_BUILD_EXECUTABLE} all -j2 V=1 WORKING_DIRECTORY ${NDKOUT})
        endif()

        add_custom_command( TARGET ${ANDROID_PROJECT_TARGET} POST_BUILD 
            COMMAND ${ANT_EXECUTABLE} debug WORKING_DIRECTORY ${NDKOUT})
    endif()
endmacro(create_android_proj)


