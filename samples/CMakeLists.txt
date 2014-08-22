# Lists all subdirectories of a given directory
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

# initialize variables
set (SAMPLES_SOURCES "")
set (SAMPLES_HEADERS "")
set (SAMPLES_SUBDIRS "")

# gather sample dirs
SUBDIRLIST(SAMPLES_SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

# loop through samples and exclude those that should not be build 
foreach (dir ${SAMPLES_SUBDIRS})
    if ("${dir}" MATCHES "^GLESDemo$") # exclude the hacky GLES demo
    elseif ("${dir}" MATCHES "^common$") # exclude common folder; samples include files from there directly when necessary
    else()
        add_subdirectory (${dir})
    endif ()
endforeach ()

# additional include directories have to go here
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/common/include)
include_directories(${CMAKE_BINARY_DIR}/samples/common/include)

cegui_add_library_impl("CEGUISamples" FALSE SAMPLES_SOURCES SAMPLES_HEADERS FALSE FALSE)