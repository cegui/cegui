set(CEGUI_TARGET_NAME ${CEGUI_SAMPLEFRAMEWORK_EXENAME})

set(CORE_SOURCE_FILES
    ./src/SampleBrowserBase.cpp
    ./src/SampleBrowser.cpp
    ./src/CEGuiBaseApplication.cpp
    ./src/CEGuiRendererSelector.cpp
    ./src/SampleHandler.cpp
    ./src/MetaDataWindowManager.cpp
    ./src/SampleBrowserManager.cpp
)

set(CORE_HEADER_FILES
    ./include/SampleBrowserBase.h
    ./include/SampleBrowser.h
    ./include/CEGuiBaseApplication.h
    ./include/CEGuiRendererSelector.h
    ./include/SampleHandler.h
    ./include/MetaDataWindowManager.h
    ./include/SampleBrowserManager.h
)


# Getting all header and source files from the common files folder
file(GLOB COMMON_HEADER_FILES "${CMAKE_SOURCE_DIR}/samples/common/*.h")
file(GLOB COMMON_SOURCE_FILES "${CMAKE_SOURCE_DIR}/samples/common/*.cpp")


include_directories(${CMAKE_SOURCE_DIR}/samples/browser/include
                    ${CMAKE_BINARY_DIR}/samples/common/include
                    ${CMAKE_SOURCE_DIR}/samples/common/include
                    ${CMAKE_SOURCE_DIR}/cegui/include
                    ${CMAKE_SOURCE_DIR}/samples)

if(WIN32)
    list(APPEND CORE_SOURCE_FILES
        ./src/Win32AppHelper.cpp
        ./src/Win32CEGuiRendererSelector.cpp
    )
elseif(APPLE)
    list(APPEND CORE_SOURCE_FILES
        ./src/MacCEGuiRendererSelector.mm
    )
elseif(CEGUI_SAMPLES_USE_GTK2)
    list(APPEND CORE_SOURCE_FILES
        ./src/GTK2CEGuiRendererSelector.cpp
    )
else()
    list(APPEND CORE_SOURCE_FILES
        ./src/CLICEGuiRendererSelector.cpp
    )
endif()

if (ANDROID)
    list (APPEND CORE_SOURCE_FILES
        ./src/SamplesFrameworkAndroid.cpp
    )
endif ()

if(CEGUI_SAMPLES_RENDERER_OPENGL_ACTIVE)
    if (NOT ANDROID)
    
        list (APPEND CORE_SOURCE_FILES
            ./src/CEGuiGLFWSharedBase.cpp
        )
        if (CEGUI_USE_GLFW_VER STREQUAL "3")
            list (APPEND CORE_SOURCE_FILES
                ./src/CEGuiGLFW3SharedBase.cpp
            )
        else() # CEGUI_USE_GLFW_VER STREQUAL "2"
            list (APPEND CORE_SOURCE_FILES
                ./src/CEGuiGLFW2SharedBase.cpp
            )
        endif()

        if (CEGUI_BUILD_RENDERER_OPENGL)
            list (APPEND CORE_SOURCE_FILES
                ./src/CEGuiOpenGLBaseApplication.cpp
            )
        endif()

        if (CEGUI_BUILD_RENDERER_OPENGL3)
            list (APPEND CORE_SOURCE_FILES
                ./src/CEGuiOpenGL3BaseApplication.cpp
            )
        endif()
  
    endif ()

    if (CEGUI_SAMPLES_SUPPORT_RENDERER_OPENGLES2)
        if (ANDROID)
            list (APPEND CORE_SOURCE_FILES
                ./src/CEGuiAndroidOpenglEs2BaseApplication.cpp
            )
        else ()
            list (APPEND CORE_SOURCE_FILES
                ./src/CEGuiOpenGLES2BaseApplication.cpp
            )
        endif ()
    endif()

endif()

if(CEGUI_SAMPLES_RENDERER_OPENGLES2_ALTERNATE_ACTIVE)
    list(APPEND CORE_SOURCE_FILES
        ./src/CEGuiEGLBaseApplication.cpp
    )
    if (ANDROID)
        list(APPEND CORE_SOURCE_FILES
            ./src/AndroidAppHelper.cpp
        )
    endif()
endif()

if(CEGUI_SAMPLES_RENDERER_OGRE_ACTIVE)
    # this prevents the boost auto-link dis-feature
    add_definitions(-DBOOST_ALL_NO_LIB)

    list(APPEND CORE_SOURCE_FILES
        ./src/CEGuiOgreBaseApplication.cpp
    )
    if (ANDROID)
        list(APPEND CORE_SOURCE_FILES
            ./src/AndroidAppHelper.cpp
        )
    endif()
endif()

if(CEGUI_SAMPLES_RENDERER_IRRLICHT_ACTIVE)
    list(APPEND CORE_SOURCE_FILES
        ./src/CEGuiIrrlichtBaseApplication.cpp
    )
endif()

if(CEGUI_SAMPLES_RENDERER_DIRECT3D11_ACTIVE)
    list(APPEND CORE_SOURCE_FILES
        ./src/CEGuiD3D11BaseApplication.cpp
        ./src/DeviceReset_Direct3D11.cpp
    )
endif()

if(CEGUI_SAMPLES_RENDERER_DIRECTFB_ACTIVE)
    list(APPEND CORE_SOURCE_FILES
        ./src/CEGuiDirectFBBaseApplication.cpp
    )
endif()

if(CEGUI_SAMPLES_RENDERER_DIRECT3D11_ACTIVE)
    include_directories(${DIRECTXSDK_INCLUDE_DIR})
    link_directories(${DIRECTXSDK_LIBRARY_DIR})
endif()

if(CEGUI_SAMPLES_ENABLED)
    list (APPEND CORE_HEADER_FILES
        ${CMAKE_CURRENT_BINARY_DIR}/../common/include/samples.inc
        ${CMAKE_CURRENT_BINARY_DIR}/../common/include/CEGUISamplesConfig.h
        ${CMAKE_CURRENT_BINARY_DIR}/../common/include/samples.h
        )
endif()


# Combine the sample framework/browser's sources and headers with the common files of the samples
set(COMBINED_HEADER_FILES ${CORE_HEADER_FILES} ${COMMON_HEADER_FILES})
set(COMBINED_SOURCE_FILES ${CORE_SOURCE_FILES} ${COMMON_SOURCE_FILES})

# We iterate through all added Samples and add the sources and headers to the combined headers and sources
foreach(CUR_SAMPLE_DIR_NAME ${SAMPLES_LIST})
    # We generate the Sample's variable names for the list of headers and list of sources, based on the directory name of the sample
    
    set(SAMPLE_HEADER_LIST_NAME "${CUR_SAMPLE_DIR_NAME}_SAMPLE_HEADERS")
    set(SAMPLE_SOURCE_LIST_NAME "${CUR_SAMPLE_DIR_NAME}_SAMPLE_SOURCES")

    set(COMBINED_SOURCE_FILES ${COMBINED_SOURCE_FILES} ${${SAMPLE_SOURCE_LIST_NAME}})
    set(COMBINED_HEADER_FILES ${COMBINED_HEADER_FILES} ${${SAMPLE_HEADER_LIST_NAME}})
endforeach()

# add additional include directories; provided by sample
include_directories(${ADDITIONAL_INCLUDE_DIRS})

###########################################################################
#                     Statically Linked Executable
###########################################################################
if(CEGUI_BUILD_STATIC_CONFIGURATION)
    if (ANDROID)
        file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp "int x = 0;")
        add_library(${CEGUI_TARGET_NAME}_Static STATIC ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)
    else()
        add_executable(${CEGUI_TARGET_NAME}_Static ${COMBINED_SOURCE_FILES} ${COMBINED_HEADER_FILES})
    endif()
    set_property(TARGET ${CEGUI_TARGET_NAME}_Static APPEND PROPERTY COMPILE_DEFINITIONS CEGUI_STATIC)

    # append the _d (or whatever) for debug builds as needed.
    if(CEGUI_HAS_BUILD_SUFFIX AND CEGUI_BUILD_SUFFIX)
        set_target_properties(${CEGUI_TARGET_NAME}_Static PROPERTIES
            OUTPUT_NAME_DEBUG "${CEGUI_TARGET_NAME}_Static${CEGUI_BUILD_SUFFIX}"
        )
    endif()

    # This works around an issue in the prebuilt deps and can be removed once those are fixed :)
    if(WIN32)
        set_target_properties(${CEGUI_TARGET_NAME}_Static PROPERTIES LINK_FLAGS_DEBUG /NODEFAULTLIB:freeglut.lib)
    endif()

    if(APPLE)
        cegui_apple_app_setup(${CEGUI_TARGET_NAME}_Static TRUE)
    endif()

    if (ANDROID)
        include(CEGUIAndroid)
        set(OGRE_TEMPLATES_DIR "${CMAKE_SOURCE_DIR}/cmake/templates")
        file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/browserdummy.cpp "int x_dummy = 0;")
        SET(ANDROID_PROJECT_TARGET "CEGUISampleBrowserAndroid")
        ADD_LIBRARY(${ANDROID_PROJECT_TARGET} SHARED ${CMAKE_CURRENT_BINARY_DIR}/browserdummy.cpp)
        target_link_libraries(${ANDROID_PROJECT_TARGET} ${CEGUI_TARGET_NAME}_Static)
        #if cegui dependencies dir not supplied, then look in the provided prefix path
        if (NOT CEGUI_DEPENDENCIES_DIR) 
            set(CEGUI_DEPENDENCIES_DIR ${CMAKE_PREFIX_PATH})
        endif()
        create_android_proj(${ANDROID_PROJECT_TARGET})
    endif()
endif()

###########################################################################
#                   Dynamically Linked Executable
###########################################################################
if(CEGUI_BUILD_DYNAMIC_CONFIGURATION)
    if (ANDROID)
        add_library (${CEGUI_TARGET_NAME} SHARED ${CORE_SOURCE_FILES})
    else ()   
        add_executable(${CEGUI_TARGET_NAME} ${COMBINED_SOURCE_FILES} ${COMBINED_HEADER_FILES})
    endif ()

    # append the _d (or whatever) for debug builds as needed.
    if(CEGUI_HAS_BUILD_SUFFIX AND CEGUI_BUILD_SUFFIX)
        set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES
            OUTPUT_NAME_DEBUG "${CEGUI_TARGET_NAME}${CEGUI_BUILD_SUFFIX}"
        )
    endif()

    if(NOT APPLE AND CEGUI_INSTALL_WITH_RPATH)
        set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES
            INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CEGUI_LIB_INSTALL_DIR}"
        )
    endif()

    if(APPLE)
        # we add these to ensure the dynamically loaded modules we will be
        # using are built before the .app's post build step that copies
        # libs into the app bundle.
        add_dependencies(${CEGUI_TARGET_NAME} ${CEGUI_CORE_WR_LIBNAME}
                                              CEGUI${CEGUI_OPTION_DEFAULT_XMLPARSER}
                                              CEGUI${CEGUI_OPTION_DEFAULT_IMAGECODEC}
        )

        cegui_apple_app_setup(${CEGUI_TARGET_NAME} FALSE)
    endif()
endif(CEGUI_BUILD_DYNAMIC_CONFIGURATION)

###########################################################################
#                    Setting up the source groups
###########################################################################

source_group("Header Files\\browser" FILES ${CORE_HEADER_FILES})
source_group("Source Files\\browser" FILES ${CORE_SOURCE_FILES})

source_group("Header Files\\common" FILES ${COMMON_HEADER_FILES})
source_group("Source Files\\common" FILES ${COMMON_SOURCE_FILES})

# We iterate through all added Samples and add a source group for its sources and headers respectively
foreach(CUR_SAMPLE_DIR_NAME ${SAMPLES_LIST})
    # We generate the Sample's variable names for the list of headers and list of sources, based on the directory name of the sample
    set(SAMPLE_HEADER_LIST_NAME "${CUR_SAMPLE_DIR_NAME}_SAMPLE_HEADERS")
    set(SAMPLE_SOURCE_LIST_NAME "${CUR_SAMPLE_DIR_NAME}_SAMPLE_SOURCES")

    # We set the source group for the files
    source_group("Header Files\\${CUR_SAMPLE_DIR_NAME}" FILES ${${SAMPLE_HEADER_LIST_NAME}})
    source_group("Source Files\\${CUR_SAMPLE_DIR_NAME}" FILES ${${SAMPLE_SOURCE_LIST_NAME}})
endforeach()

###########################################################################
#                    MSVC PROJ USER FILE TEMPLATES
###########################################################################
if(MSVC)
    # Placing vcxproj file to set debugging directory and other settings for the samplebrowser project right away 
    configure_file(
        ${CMAKE_MODULE_PATH}/templates/VisualStudioUserFile.vcxproj.user.in
        ${CMAKE_CURRENT_BINARY_DIR}/${CEGUI_TARGET_NAME}.vcxproj.user
        @ONLY
        )
endif()


###########################################################################
#                      LIBRARY LINK SETUP
###########################################################################
if (CEGUI_BUILD_DYNAMIC_CONFIGURATION)
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME})
endif()

if(CEGUI_BUILD_STATIC_CONFIGURATION)
    target_link_libraries(${CEGUI_TARGET_NAME}_Static
        ${CEGUI_STATIC_XMLPARSER_MODULE}_Static
        ${CEGUI_STATIC_IMAGECODEC_MODULE}_Static
        ${CEGUI_CORE_WR_LIBNAME}_Static
    )
endif()

###########################################################################
#  BUILD DEPENDENCIES FOR THE SHARED LIBRARIES USED BY THE SAMPLEBROWSER
###########################################################################
if (CEGUI_BUILD_DYNAMIC_CONFIGURATION)
    add_dependencies(${CEGUI_TARGET_NAME} ${CEGUI_CORE_WR_LIBNAME}
        CEGUI${CEGUI_OPTION_DEFAULT_XMLPARSER}
        CEGUI${CEGUI_OPTION_DEFAULT_IMAGECODEC})

    if (CEGUI_SAMPLES_ENABLE_COMMON_DIALOGUES_DEMO)
        add_dependencies(${CEGUI_TARGET_NAME} ${CEGUI_COMMON_DIALOGS_LIBNAME})
    endif()

endif()

###########################################################################
#                           INSTALLATION
###########################################################################
if (CEGUI_BUILD_DYNAMIC_CONFIGURATION)
    install(TARGETS ${CEGUI_TARGET_NAME}
        RUNTIME DESTINATION bin COMPONENT cegui_samples
        LIBRARY DESTINATION ${CEGUI_LIB_INSTALL_DIR} COMPONENT cegui_samples
        ARCHIVE DESTINATION ${CEGUI_LIB_INSTALL_DIR} COMPONENT cegui_samples
        BUNDLE DESTINATION ${CEGUI_LIB_INSTALL_DIR} COMPONENT cegui_samples
        )
endif()

if(CEGUI_BUILD_STATIC_CONFIGURATION)
    install(TARGETS ${CEGUI_TARGET_NAME}_Static
        RUNTIME DESTINATION bin COMPONENT cegui_samples
        LIBRARY DESTINATION ${CEGUI_LIB_INSTALL_DIR} COMPONENT cegui_samples
        ARCHIVE DESTINATION ${CEGUI_LIB_INSTALL_DIR} COMPONENT cegui_samples
        BUNDLE DESTINATION ${CEGUI_LIB_INSTALL_DIR} COMPONENT cegui_samples
)
endif()

if(CEGUI_BUILD_COMMON_DIALOGS)
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_COMMON_DIALOGS_LIBNAME})
endif()

if (UNIX AND NOT APPLE AND NOT ANDROID)
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${X11_LIBRARIES} ${X11_Xt_LIB} 
        ${XAW_LIBRARY} ${X11_Xrandr_LIB} pthread ${CMAKE_DL_LIBS})
endif()

if(CEGUI_SAMPLES_RENDERER_OPENGL_ACTIVE OR CEGUI_SAMPLES_RENDERER_OPENGLES2_ALTERNATE_ACTIVE)
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_OPENGL_RENDERER_LIBNAME})
endif()

if(CEGUI_SAMPLES_RENDERER_OPENGL_ACTIVE)
    if (CEGUI_USE_EPOXY)
        cegui_add_dependency(${CEGUI_TARGET_NAME} EPOXY)
    elseif (CEGUI_USE_GLEW)
        cegui_add_dependency(${CEGUI_TARGET_NAME} OPENGL)
    endif ()  
    if (CEGUI_USE_GLFW_VER STREQUAL "3")
        cegui_add_dependency(${CEGUI_TARGET_NAME} GLFW3)
        cegui_add_dependency(${CEGUI_TARGET_NAME} GLEW PRIVATE TRUE)
    else () # CEGUI_USE_GLFW_VER STREQUAL "2"
        cegui_add_dependency(${CEGUI_TARGET_NAME} GLFW)
        cegui_add_dependency(${CEGUI_TARGET_NAME} GLEW PRIVATE TRUE)
    endif ()
endif ()

if(CEGUI_SAMPLES_RENDERER_OPENGLES2_ALTERNATE_ACTIVE)
    if (NOT ANDROID) # LINUX GLES depends on GLEW 
        cegui_add_dependency(${CEGUI_TARGET_NAME} GLEW)
        cegui_target_link_libraries(${CEGUI_TARGET_NAME} EGL)
    endif()
    if (CEGUI_BUILD_RENDERER_OPENGLES2_WITH_GLES3_SUPPORT)
        cegui_add_dependency(${CEGUI_TARGET_NAME} OPENGLES3)
    else()
        cegui_add_dependency(${CEGUI_TARGET_NAME} OPENGLES2)
    endif()
endif()

if(CEGUI_SAMPLES_RENDERER_OGRE_ACTIVE)
    cegui_add_dependency(${CEGUI_TARGET_NAME} OIS)
    if (UNIX AND NOT APPLE AND NOT ANDROID)
        cegui_target_link_libraries(${CEGUI_TARGET_NAME}  GL )
    endif()
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_OGRE_RENDERER_LIBNAME})
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_OGRE_SAMPLES_EXTRALIBS} )
    if (OGRE_STATIC) #if ogre is static, then must include the static plugin libs
        set(CORE_OGRE_COMPONENTS RenderSystem_Direct3D9 RenderSystem_Direct3D11 RenderSystem_GLES RenderSystem_GLES2 RenderSystem_GL RenderSystem_GL3Plus)
        foreach (comp ${CORE_OGRE_COMPONENTS})
            if (${OGRE_${comp}_FOUND})
                include_directories(${OGRE_${comp}_INCLUDE_DIRS})
                cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${OGRE_${comp}_LIBRARIES})
            endif()
        endforeach (comp)
    endif()
endif()
    cegui_add_dependency(${CEGUI_TARGET_NAME} OGRE)

if(CEGUI_SAMPLES_RENDERER_IRRLICHT_ACTIVE)
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_IRRLICHT_RENDERER_LIBNAME})
    cegui_add_dependency(${CEGUI_TARGET_NAME} IRRLICHT)
endif()

if(CEGUI_SAMPLES_RENDERER_DIRECT3D11_ACTIVE)
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_DIRECT3D11_RENDERER_LIBNAME})
    if( MINGW )
        cegui_target_link_libraries(${CEGUI_TARGET_NAME} dinput8 )
    else()
        target_include_directories( ${CEGUI_TARGET_NAME} SYSTEM PRIVATE ${DIRECTXSDK_INCLUDE_DIR} )
    endif()
endif()

if(CEGUI_SAMPLES_RENDERER_DIRECTFB_ACTIVE)
    cegui_add_dependency(${CEGUI_TARGET_NAME} DIRECTFB)
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_DIRECTFB_RENDERER_LIBNAME})
endif()

if(CEGUI_SAMPLES_USE_GTK2)
    cegui_add_dependency(${CEGUI_TARGET_NAME} GTK2)
endif()

if(APPLE)
    set (CEGUI_EXTRA_FRAMEWORKS "-framework Cocoa")

    if(CEGUI_SAMPLES_RENDERER_OGRE_ACTIVE)
        set (CEGUI_EXTRA_FRAMEWORKS "${CEGUI_EXTRA_FRAMEWORKS} -framework IOKit -framework Carbon")
    endif()

    set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES
        LINK_FLAGS ${CEGUI_EXTRA_FRAMEWORKS}
    )
endif()

if ((UNIX AND NOT APPLE AND NOT ANDROID) OR MINGW)
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CMAKE_DL_LIBS})
endif()

cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME})
