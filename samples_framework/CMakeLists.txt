set( CEGUI_TARGET_NAME ${CEGUI_SAMPLEFRAMEWORK_EXENAME} )

set( CORE_SOURCE_FILES
    ./src/SamplesFrameworkBase.cpp
    ./src/SamplesFramework.cpp
    ./src/CEGuiBaseApplication.cpp
    ./src/CEGuiRendererSelector.cpp
    ./src/Samples_xmlHandler.cpp
    ./src/SampleData.cpp
    ./src/MetaDataWindowManager.cpp
    ./src/SamplesBrowserManager.cpp
)

set( CORE_HEADER_FILES
    ../samples/common/include/Sample.h
    ./include/SamplesFrameworkBase.h
    ./include/SamplesFramework.h
    ./include/CEGuiBaseApplication.h
    ./include/CEGuiRendererSelector.h
    ./include/Samples_xmlHandler.h
    ./include/SampleData.h
    ./include/MetaDataWindowManager.h
    ./include/SamplesBrowserManager.h
)


include_directories(${CMAKE_SOURCE_DIR}/samples_framework/include
                    ${CMAKE_BINARY_DIR}/samples/common/include
                    ${CMAKE_SOURCE_DIR}/samples/common/include
                    ${CMAKE_SOURCE_DIR}/cegui/include)

if (WIN32)
    list (APPEND CORE_SOURCE_FILES
        ./src/Win32AppHelper.cpp
        ./src/Win32CEGuiRendererSelector.cpp
    )
elseif (APPLE)
    list (APPEND CORE_SOURCE_FILES
        ./src/MacCEGuiRendererSelector.mm
    )
elseif (CEGUI_SAMPLES_USE_GTK2)
    list (APPEND CORE_SOURCE_FILES
        ./src/GTK2CEGuiRendererSelector.cpp
    )
else()
    list (APPEND CORE_SOURCE_FILES
        ./src/CLICEGuiRendererSelector.cpp
    )
endif()

if (ANDROID)
    list (APPEND CORE_SOURCE_FILES
        ./src/SamplesFrameworkAndroid.cpp
    )
endif ()

if (CEGUI_SAMPLES_RENDERER_OPENGL_ACTIVE)
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

if (CEGUI_SAMPLES_RENDERER_OGRE_ACTIVE)
    # this prevents the boost auto-link dis-feature
    add_definitions(-DBOOST_ALL_NO_LIB)

    list (APPEND CORE_SOURCE_FILES
        ./src/CEGuiOgreBaseApplication.cpp
    )
endif()

if (CEGUI_SAMPLES_RENDERER_IRRLICHT_ACTIVE)
    list (APPEND CORE_SOURCE_FILES
        ./src/CEGuiIrrlichtBaseApplication.cpp
    )
endif()

if (CEGUI_SAMPLES_RENDERER_DIRECT3D9_ACTIVE)
    list (APPEND CORE_SOURCE_FILES
        ./src/CEGuiD3D9BaseApplication.cpp
        ./src/DeviceReset_Direct3D9.cpp
    )
endif()

if (CEGUI_SAMPLES_RENDERER_DIRECT3D10_ACTIVE)
    list (APPEND CORE_SOURCE_FILES
        ./src/CEGuiD3D10BaseApplication.cpp
        ./src/DeviceReset_Direct3D10.cpp
    )
endif()

if (CEGUI_SAMPLES_RENDERER_DIRECT3D11_ACTIVE)
    list (APPEND CORE_SOURCE_FILES
        ./src/CEGuiD3D11BaseApplication.cpp
        ./src/DeviceReset_Direct3D11.cpp
    )
endif()

if (CEGUI_SAMPLES_RENDERER_DIRECTFB_ACTIVE)
    list (APPEND CORE_SOURCE_FILES
        ./src/CEGuiDirectFBBaseApplication.cpp
    )
endif()

if (CEGUI_SAMPLES_RENDERER_DIRECT3D9_ACTIVE OR CEGUI_SAMPLES_RENDERER_DIRECT3D10_ACTIVE OR CEGUI_SAMPLES_RENDERER_DIRECT3D11_ACTIVE)
    include_directories(${DIRECTXSDK_INCLUDE_DIR})
    link_directories(${DIRECTXSDK_LIBRARY_DIR})
endif()

###########################################################################
#                     Statically Linked Executable
###########################################################################
if (CEGUI_BUILD_STATIC_CONFIGURATION)
    add_executable(${CEGUI_TARGET_NAME}_Static ${CORE_SOURCE_FILES} ${CORE_HEADER_FILES})
    set_property(TARGET ${CEGUI_TARGET_NAME}_Static APPEND PROPERTY COMPILE_DEFINITIONS CEGUI_STATIC)

    # append the _d (or whatever) for debug builds as needed.
    if (CEGUI_HAS_BUILD_SUFFIX AND CEGUI_BUILD_SUFFIX)
        set_target_properties(${CEGUI_TARGET_NAME}_Static PROPERTIES
            OUTPUT_NAME_DEBUG "${CEGUI_TARGET_NAME}_Static${CEGUI_BUILD_SUFFIX}"
        )
    endif()

    # This works around an issue in the prebuilt deps and can be removed once those are fixed :)
    if (WIN32)
        set_target_properties(${CEGUI_TARGET_NAME}_Static PROPERTIES LINK_FLAGS_DEBUG /NODEFAULTLIB:freeglut.lib)
    endif()

    if (APPLE)
        cegui_apple_app_setup(${CEGUI_TARGET_NAME}_Static TRUE)
    endif()
endif()

###########################################################################
#                   Dynamically Linked Executable
###########################################################################
if (ANDROID)
    add_library (${CEGUI_TARGET_NAME} SHARED ${CORE_SOURCE_FILES})
else ()
    add_executable(${CEGUI_TARGET_NAME} ${CORE_SOURCE_FILES} ${CORE_HEADER_FILES})
endif ()

# append the _d (or whatever) for debug builds as needed.
if (CEGUI_HAS_BUILD_SUFFIX AND CEGUI_BUILD_SUFFIX)
    set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES
        OUTPUT_NAME_DEBUG "${CEGUI_TARGET_NAME}${CEGUI_BUILD_SUFFIX}"
    )
endif()

if (NOT APPLE AND CEGUI_INSTALL_WITH_RPATH)
    set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES
        INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CEGUI_LIB_INSTALL_DIR}"
    )
endif()

if (APPLE)
    # we add these to ensure the dynamically loaded modules we will be
    # using are built before the .app's post build step that copies
    # libs into the app bundle.
    add_dependencies(${CEGUI_TARGET_NAME} ${CEGUI_CORE_WR_LIBNAME}
                                          CEGUI${CEGUI_OPTION_DEFAULT_XMLPARSER}
                                          CEGUI${CEGUI_OPTION_DEFAULT_IMAGECODEC}
    )

    cegui_apple_app_setup(${CEGUI_TARGET_NAME} FALSE)
endif()

###########################################################################
#                    MSVC PROJ USER FILE TEMPLATES
###########################################################################
if(MSVC)
    # Visual Studio 2008 or less
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 16.0)
        configure_file(
            ${CMAKE_MODULE_PATH}/templates/VisualStudioUserFile.vcproj.user.in
            ${CMAKE_CURRENT_BINARY_DIR}/${CEGUI_TARGET_NAME}.vcproj.user
            @ONLY
        )
    else ()
    # Visual Studio 2010 or higher
        configure_file(
            ${CMAKE_MODULE_PATH}/templates/VisualStudioUserFile.vcxproj.user.in
            ${CMAKE_CURRENT_BINARY_DIR}/${CEGUI_TARGET_NAME}.vcxproj.user
            @ONLY
            )
    endif ()
endif()


###########################################################################
#                      LIBRARY LINK SETUP
###########################################################################
cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME})

if (CEGUI_BUILD_STATIC_CONFIGURATION)
    target_link_libraries(${CEGUI_TARGET_NAME}_Static
        ${CEGUI_STATIC_XMLPARSER_MODULE}_Static
        ${CEGUI_STATIC_IMAGECODEC_MODULE}_Static
        ${CEGUI_CORE_WR_LIBNAME}_Static
    )
endif()

###########################################################################
#  BUILD DEPENDENCIES FOR THE SHARED LIBRARIES USED BY THE SAMPLEBROWSER
###########################################################################

add_dependencies(${CEGUI_TARGET_NAME} ${CEGUI_CORE_WR_LIBNAME}
                                      CEGUI${CEGUI_OPTION_DEFAULT_XMLPARSER}
                                      CEGUI${CEGUI_OPTION_DEFAULT_IMAGECODEC})

if (CEGUI_SAMPLES_ENABLE_COMMON_DIALOGUES_DEMO)
    add_dependencies(${CEGUI_TARGET_NAME} ${CEGUI_COMMON_DIALOGS_LIBNAME})
endif()

###########################################################################
#                           INSTALLATION
###########################################################################
install(TARGETS ${CEGUI_TARGET_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${CEGUI_LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${CEGUI_LIB_INSTALL_DIR}
    BUNDLE DESTINATION ${CEGUI_LIB_INSTALL_DIR}
)

if (CEGUI_BUILD_STATIC_CONFIGURATION)
    install(TARGETS ${CEGUI_TARGET_NAME}_Static
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${CEGUI_LIB_INSTALL_DIR}
        ARCHIVE DESTINATION ${CEGUI_LIB_INSTALL_DIR}
        BUNDLE DESTINATION ${CEGUI_LIB_INSTALL_DIR}
)
endif()

if (CEGUI_SAMPLES_RENDERER_OPENGL_ACTIVE)
    if (CEGUI_USE_EPOXY)
        cegui_add_dependency(${CEGUI_TARGET_NAME} EPOXY)
    elseif (CEGUI_USE_GLEW)
        cegui_add_dependency(${CEGUI_TARGET_NAME} OPENGL)
    endif ()
    if (CEGUI_USE_GLFW_VER STREQUAL "3")
        cegui_add_dependency(${CEGUI_TARGET_NAME} GLFW3)
    else () # CEGUI_USE_GLFW_VER STREQUAL "2"
        cegui_add_dependency(${CEGUI_TARGET_NAME} GLFW)
    endif ()
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_OPENGL_RENDERER_LIBNAME})
endif()

if (CEGUI_SAMPLES_RENDERER_OGRE_ACTIVE)
    cegui_add_dependency(${CEGUI_TARGET_NAME} OIS PRIVATE TRUE)
    if (CEGUI_SAMPLES_RENDERER_OGRE_ACTIVE)
        if (OGRE_STATIC) #if ogre is static, then must include the static plugin libs
            set(CORE_OGRE_COMPONENTS RenderSystem_Direct3D9 RenderSystem_Direct3D11 RenderSystem_GLES RenderSystem_GLES2 RenderSystem_GL RenderSystem_GL3Plus)
            foreach (comp ${CORE_OGRE_COMPONENTS})
                if (${OGRE_${comp}_FOUND})
                    include_directories(${OGRE_${comp}_INCLUDE_DIRS})
                    target_link_libraries(${CEGUI_TARGET_NAME} ${OGRE_${comp}_LIBRARIES})
                    if (CEGUI_BUILD_STATIC_CONFIGURATION)
                        target_link_libraries(${CEGUI_TARGET_NAME}_Static ${OGRE_${comp}_LIBRARIES})
                    endif()
                endif()
            endforeach (comp)
       endif()
    endif()
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_OGRE_RENDERER_LIBNAME} ${CEGUI_OGRE_SAMPLES_EXTRALIBS})
endif()

if (CEGUI_SAMPLES_RENDERER_IRRLICHT_ACTIVE)
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_IRRLICHT_RENDERER_LIBNAME})
endif()

if (CEGUI_SAMPLES_RENDERER_DIRECT3D9_ACTIVE OR CEGUI_SAMPLES_RENDERER_DIRECT3D10_ACTIVE OR CEGUI_SAMPLES_RENDERER_DIRECT3D11_ACTIVE)

    if (MINGW)
        cegui_target_link_libraries(${CEGUI_TARGET_NAME} dxerr9 dinput8 dxguid)
    else ()
        cegui_target_link_libraries(${CEGUI_TARGET_NAME} dxerr)
    endif()

    if (CEGUI_SAMPLES_RENDERER_DIRECT3D9_ACTIVE)
        cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_DIRECT3D9_RENDERER_LIBNAME} d3d9)
    endif()

    if (CEGUI_SAMPLES_RENDERER_DIRECT3D10_ACTIVE)
        cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_DIRECT3D10_RENDERER_LIBNAME})
        if (MINGW)
            cegui_target_link_libraries(${CEGUI_TARGET_NAME} d3dx10_43)
        else ()
            cegui_target_link_libraries(${CEGUI_TARGET_NAME} d3d10)
        endif()
    endif()

    if (CEGUI_SAMPLES_RENDERER_DIRECT3D11_ACTIVE)
        cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_DIRECT3D11_RENDERER_LIBNAME})
        if (MINGW)
            cegui_target_link_libraries(${CEGUI_TARGET_NAME} d3xd11_43)
        else ()
            cegui_target_link_libraries(${CEGUI_TARGET_NAME} d3d11)
        endif()
    endif()

endif()

if (CEGUI_SAMPLES_RENDERER_DIRECTFB_ACTIVE)
    cegui_add_dependency(${CEGUI_TARGET_NAME} DIRECTFB)
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_DIRECTFB_RENDERER_LIBNAME})
endif()

if (CEGUI_SAMPLES_USE_GTK2)
    cegui_add_dependency(${CEGUI_TARGET_NAME} GTK2)
endif()

if (APPLE)
    set (CEGUI_EXTRA_FRAMEWORKS "-framework Cocoa")

    if (CEGUI_SAMPLES_RENDERER_OGRE_ACTIVE)
        set (CEGUI_EXTRA_FRAMEWORKS "${CEGUI_EXTRA_FRAMEWORKS} -framework IOKit -framework Carbon")
    endif()

    set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES
        LINK_FLAGS ${CEGUI_EXTRA_FRAMEWORKS}
    )
endif()

cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME})
