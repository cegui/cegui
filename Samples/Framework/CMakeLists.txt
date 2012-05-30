set( CEGUI_TARGET_NAME ${CEGUI_SAMPLEFRAMEWORK_EXENAME} )

set( CORE_SOURCE_FILES
    ./src/SamplesFrameworkBase.cpp
	./src/SamplesFramework.cpp
	./src/CEGuiBaseApplication.cpp
    ./src/CEGuiRendererSelector.cpp
)

set( CORE_HEADER_FILES
	../common/include/Sample.h
	./include/SamplesFrameworkBase.h
	./include/SamplesFramework.h
	./include/CEGuiBaseApplication.h
    ./include/CEGuiRendererSelector.h

)


include_directories(${CMAKE_BINARY_DIR}/samples/common/include
					${CMAKE_SOURCE_DIR}/samples/common/include
					${CMAKE_SOURCE_DIR}/samples/Framework/include
                    ${CMAKE_SOURCE_DIR}/cegui/include)

					
if (CEGUI_SAMPLES_USE_OPENGL OR CEGUI_SAMPLES_USE_OPENGL3)
    list (APPEND CORE_SOURCE_FILES
        ./src/CEGuiGLFWSharedBase.cpp
    )
endif()

if (CEGUI_SAMPLES_USE_OPENGL)
    list (APPEND CORE_SOURCE_FILES
        ./src/CEGuiOpenGLBaseApplication.cpp
    )
endif()

if (CEGUI_SAMPLES_USE_OPENGL3)
    list (APPEND CORE_SOURCE_FILES
        ./src/CEGuiOpenGL3BaseApplication.cpp
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




if (CEGUI_SAMPLES_USE_DIRECT3D9 OR CEGUI_SAMPLES_USE_DIRECT3D10)
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
    add_executable(${CEGUI_TARGET_NAME} ${CORE_SOURCE_FILES} ${CORE_HEADER_FILES})

    # append the _d (or whatever) for debug builds as needed.
    if (CEGUI_HAS_BUILD_SUFFIX AND CEGUI_BUILD_SUFFIX)
        set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES
            OUTPUT_NAME_DEBUG "${CEGUI_TARGET_NAME}${CEGUI_BUILD_SUFFIX}"
        )
    endif()

    if (NOT APPLE)
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
    #                      LIBRARY LINK SETUP
    ###########################################################################
    cegui_target_link_libraries(${CEGUI_TARGET_NAME}
        ${CEGUI_BASE_LIBNAME}
    )

    if (CEGUI_BUILD_STATIC_CONFIGURATION)
        target_link_libraries(${CEGUI_TARGET_NAME}_Static
            ${CEGUI_STATIC_XMLPARSER_MODULE}_Static
            ${CEGUI_STATIC_IMAGECODEC_MODULE}_Static
            ${CEGUI_CORE_WR_LIBNAME}_Static
        )
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








if (CEGUI_SAMPLES_USE_OPENGL OR CEGUI_SAMPLES_USE_OPENGL3)
    cegui_add_dependency(${CEGUI_TARGET_NAME} OPENGL)
    cegui_add_dependency(${CEGUI_TARGET_NAME} GLFW)
endif()

if (CEGUI_SAMPLES_USE_OPENGL)
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_OPENGL_RENDERER_LIBNAME})
endif()

if (CEGUI_SAMPLES_USE_OPENGL3)
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_OPENGL3_RENDERER_LIBNAME})
endif()

if (CEGUI_SAMPLES_USE_OGRE)
    cegui_add_dependency(${CEGUI_TARGET_NAME} OGRE)
    cegui_add_dependency(${CEGUI_TARGET_NAME} OIS)
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_OGRE_RENDERER_LIBNAME})
endif()

if (CEGUI_SAMPLES_USE_IRRLICHT)
    cegui_add_dependency(${CEGUI_TARGET_NAME} IRRLICHT)
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_IRRLICHT_RENDERER_LIBNAME})
endif()

if (CEGUI_SAMPLES_USE_DIRECT3D9 OR CEGUI_SAMPLES_USE_DIRECT3D10)
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} dxerr)

    if (CEGUI_SAMPLES_USE_DIRECT3D9)
        cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_DIRECT3D9_RENDERER_LIBNAME} d3d9)
    endif()

    if (CEGUI_SAMPLES_USE_DIRECT3D10)
        cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_DIRECT3D10_RENDERER_LIBNAME} d3d10)
    endif()

endif()

if (CEGUI_SAMPLES_USE_DIRECTFB)
    cegui_add_dependency(${CEGUI_TARGET_NAME} DIRECTFB)
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_DIRECTFB_RENDERER_LIBNAME})
endif()

if (CEGUI_SAMPLES_USE_GTK2)
    cegui_add_dependency(${CEGUI_TARGET_NAME} GTK2)
endif()

if (APPLE)
    set (CEGUI_EXTRA_FRAMEWORKS "-framework Cocoa")

    if (CEGUI_SAMPLES_USE_OGRE)
        set (CEGUI_EXTRA_FRAMEWORKS "${CEGUI_EXTRA_FRAMEWORKS} -framework IOKit -framework Carbon")
    endif()

    set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES
        LINK_FLAGS ${CEGUI_EXTRA_FRAMEWORKS}
    )
endif()

cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME})

