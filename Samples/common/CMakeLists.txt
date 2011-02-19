set( CEGUI_TARGET_NAME ${CEGUI_SAMPLEHELPER_LIBNAME} )

set( CORE_SOURCE_FILES
    ./src/CEGuiBaseApplication.cpp
    ./src/CEGuiRendererSelector.cpp
    ./src/CEGuiSample.cpp
)

if (CEGUI_SAMPLES_USE_OPENGL)
    list (APPEND CORE_SOURCE_FILES
        ./src/CEGuiOpenGLBaseApplication.cpp
    )
endif()

if (CEGUI_SAMPLES_USE_OGRE)
    list (APPEND CORE_SOURCE_FILES
        ./src/CEGuiOgreBaseApplication.cpp
    )
endif()

if (CEGUI_SAMPLES_USE_IRRLICHT)
    list (APPEND CORE_SOURCE_FILES
        ./src/CEGuiIrrlichtBaseApplication.cpp
    )
endif()

if (WIN32)
    list (APPEND CORE_SOURCE_FILES
        ./src/Win32AppHelper.cpp
        ./src/Win32CEGuiRendererSelector.cpp
    )
elseif (APPLE)
    list (APPEND CORE_SOURCE_FILES
        ./src/MacCEGuiRendererSelector.cpp
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

if (CEGUI_SAMPLES_USE_DIRECT3D9)
    list (APPEND CORE_SOURCE_FILES
        ./src/CEGuiD3D9BaseApplication.cpp
        ./src/DeviceReset_Direct3D9.cpp
    )
endif()

if (CEGUI_SAMPLES_USE_DIRECT3D9)
    list (APPEND CORE_SOURCE_FILES
        ./src/CEGuiD3D10BaseApplication.cpp
        ./src/DeviceReset_Direct3D10.cpp
    )
endif()

if (CEGUI_SAMPLES_USE_DIRECTFB)
    list (APPEND CORE_SOURCE_FILES
        ./src/CEGuiDirectFBBaseApplication.cpp
    )
endif()

include_directories(${CMAKE_BINARY_DIR}/Samples/common/include
                    ${CMAKE_SOURCE_DIR}/Samples/common/include
                    ${CMAKE_SOURCE_DIR}/cegui/include)

if (CEGUI_SAMPLES_USE_DIRECT3D9 OR CEGUI_SAMPLES_USE_DIRECT3D10)
    include_directories(${DIRECTXSDK_INCLUDE_DIR})
    link_directories(${DIRECTXSDK_LIBRARY_DIR})
endif()

cegui_add_library(TRUE FALSE)

if (CEGUI_SAMPLES_USE_OPENGL)
    cegui_add_dependency(OPENGL)
    cegui_add_dependency(GLUT)
    target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_OPENGL_RENDERER_LIBNAME})

    # This works around an issue in the prebuilt deps and can be removed once those are fixed :)
    if (WIN32)
        set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES LINK_FLAGS_DEBUG /NODEFAULTLIB:freeglut.lib)
    endif()
endif()

if (CEGUI_SAMPLES_USE_OGRE)
    cegui_add_dependency(OGRE)
    cegui_add_dependency(OIS)
    target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_OGRE_RENDERER_LIBNAME})
endif()

if (CEGUI_SAMPLES_USE_IRRLICHT)
    cegui_add_dependency(IRRLICHT)
    target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_IRRLICHT_RENDERER_LIBNAME})
endif()

if (CEGUI_SAMPLES_USE_DIRECT3D9 OR CEGUI_SAMPLES_USE_DIRECT3D10)
    target_link_libraries(${CEGUI_TARGET_NAME} dxerr)

    if (CEGUI_SAMPLES_USE_DIRECT3D9)
        target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_DIRECT3D9_RENDERER_LIBNAME} d3d9)
    endif()

    if (CEGUI_SAMPLES_USE_DIRECT3D10)
        target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_DIRECT3D10_RENDERER_LIBNAME} d3d10)
    endif()

endif()

if (CEGUI_SAMPLES_USE_GTK2)
    cegui_add_dependency(GTK2)
endif()

if (APPLE)
    target_link_libraries(${CEGUI_TARGET_NAME} ${CARBON_FWK})
    set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES
        FRAMEWORK TRUE
        MACOSX_FRAMEWORK_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/CEGUISampleHelper-Info.plist"
    )
endif()


target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME})

