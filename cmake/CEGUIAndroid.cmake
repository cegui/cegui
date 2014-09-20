
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

        if(OGRE_CONFIG_ENABLE_GLES2_GLSL_OPTIMISER)
            SET(GLES_OPTIMISER "-lglsl_optimizer -lmesa -lglcpp-library")
        endif()

        SET(ANDROID_TARGET "android-${ANDROID_SDK_API_LEVEL}")

        SET(NDKOUT "${CMAKE_BINARY_DIR}/SampleBrowserNDK")
        file(MAKE_DIRECTORY "${NDKOUT}")
        file(MAKE_DIRECTORY "${NDKOUT}/jni")
        file(MAKE_DIRECTORY "${NDKOUT}/assets")
        file(MAKE_DIRECTORY "${NDKOUT}/res")
        file(WRITE "${NDKOUT}/default.properties" "target=${ANDROID_TARGET}")
        file(WRITE "${NDKOUT}/jni/Application.mk" 
            "APP_ABI := ${ANDROID_ABI}\nAPP_STL := gnustl_static\nAPP_CPPFLAGS := -fexceptions -frtti -std=c++11\nNDK_TOOLCHAIN_VERSION := ${ANDROID_COMPILER_VERSION}") 
        message( "checking for gles2 or gles" )
        if (OGRE_RenderSystem_GLES2_FOUND)
            message( "gles2 " )
            add_definitions(-DINCLUDE_RTSHADER_SYSTEM)
            SET(DEPENDENCIES OgreMain RenderSystem_GLES2)
        elseif (OGRE_RenderSystem_GLES_FOUND)
            message( "gles " )
            SET(DEPENDENCIES OgreMain RenderSystem_GLES)
        endif()
        SET(ANDROID_MOD_NAME "${CEGUI_TARGET_NAME}")
        SET(JNI_PATH "${CMAKE_SOURCE_DIR}/samples/browser")
        foreach(COMBINED_SOURCE_FILE ${COMBINED_SOURCE_FILES})
            set(COMBINED_SOURCE_FILES_STRING "${COMBINED_SOURCE_FILES_STRING} ${COMBINED_SOURCE_FILE}")
        endforeach()
        SET(JNI_SRC_FILES "${COMBINED_SOURCE_FILES_STRING}")
        SET(PKG_NAME "cegui.org.uk.browser")
        # Set this variable to false if no java code will be present (google android:hasCode for more info)
        SET(HAS_CODE "false")
        SET(HEADERS "")
        SET(SAMPLE_LDLIBS "")
        SET(ANDROID_SHARED_LIB "${CEGUI_TARGET_NAME}")
        file(COPY "${CMAKE_SOURCE_DIR}/datafiles/imagesets/ic_launcher.png" DESTINATION "${NDKOUT}/res/drawable")
        file(COPY "${CMAKE_SOURCE_DIR}/datafiles" DESTINATION "${NDKOUT}/assets")
        #file(COPY "${CMAKE_BINARY_DIR}/datafiles/samples/samples.xml" DESTINATION 
        #    "${NDKOUT}/assets/samples/samples.xml")
        configure_file("${OGRE_TEMPLATES_DIR}/AndroidManifest.xml.in" "${NDKOUT}/AndroidManifest.xml" @ONLY)
        configure_file("${OGRE_TEMPLATES_DIR}/Android_resources.cfg.in" "${NDKOUT}/assets/resources.cfg" @ONLY)
        if (OGRE_RenderSystem_GLES2_FOUND)
            configure_file("${OGRE_TEMPLATES_DIR}/Android.OgreGLES2.mk.in" "${NDKOUT}/jni/Android.mk" @ONLY)
        elseif (OGRE_RenderSystem_GLES_FOUND)
            configure_file("${OGRE_TEMPLATES_DIR}/Android.OgreGLES1.mk.in" "${NDKOUT}/jni/Android.mk" @ONLY)
        endif()

        add_custom_command(
            TARGET ${ANDROID_PROJECT_TARGET} POST_BUILD COMMAND ${ANDROID_EXECUTABLE} 
            update project --subprojects --target ${ANDROID_TARGET} --path "${NDKOUT}" WORKING_DIRECTORY ${NDKOUT})

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


