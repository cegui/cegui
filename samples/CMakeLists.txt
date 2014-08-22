MACRO(SUBDIRLIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    SET(dirlist "")
    FOREACH(child ${children})
        IF(IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${child})
        ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
ENDMACRO()

set (SAMPLES_SOURCES "")
set (SAMPLES_HEADERS "")
set (SAMPLES_SUBDIRS "")

SUBDIRLIST(SAMPLES_SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

foreach (dir ${SAMPLES_SUBDIRS})
    if ("${dir}" MATCHES "^GLESDemo$") # exclude the hacky GLES demo
    elseif ("${dir}" MATCHES "^common$") # exclude common folder; samples include files from there when necessary
    else()
        add_subdirectory (${dir})
    endif ()
endforeach ()

cegui_add_library("CEGUISamples" SAMPLES_SOURCES SAMPLES_HEADERS)