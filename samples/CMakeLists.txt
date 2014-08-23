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
set (ADDITIONAL_LIBS "${CEGUI_BASE_LIBNAME}")
set (SAMPLES_SOURCES "")
set (SAMPLES_HEADERS "")
set (SAMPLES_LIST "")
set (SAMPLES_SUBDIRS "")

set (SAMPLE_LIST_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/common/include/samples.h)
set (SAMPLE_LIST_CODE ${CMAKE_CURRENT_SOURCE_DIR}/common/include/samples.inc)

# gather sample dirs
SUBDIRLIST(SAMPLES_SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

# loop through samples and exclude those that should not be build 
foreach (dir ${SAMPLES_SUBDIRS})
    if ("${dir}" MATCHES "^GLESDemo$") # exclude the hacky GLES demo
    elseif ("${dir}" MATCHES "^common$") # exclude common folder; samples include files from there directly when necessary
    elseif ("${dir}" MATCHES "^samples$") # exclude this directory
    elseif ("${dir}" MATCHES "^Demo8$") # this demo doesnt work at the moment
    elseif ("${dir}" MATCHES "^SimpleGameMenu$") # the sample is broken somewhere
    else()
        add_subdirectory (${dir})
    endif ()
endforeach ()

# add all registered samples to samples.inc and samples.h file
file (WRITE ${SAMPLE_LIST_HEADER} "#ifndef _SamplesListHeader_h_\n#define _SamplesListHeader_h_\n\n#include \"Sample.h\"\n")
file (WRITE ${SAMPLE_LIST_CODE} "std::vector<Sample*> samples;")
foreach (sample ${SAMPLES_LIST})
    set (_PATH "../../samples/${sample}/${sample}.h")
    file (APPEND ${SAMPLE_LIST_HEADER} "\n#include \"${_PATH}\"")
    file (APPEND ${SAMPLE_LIST_CODE} "\nsamples.push_back(new ${sample}Sample());")
endforeach ()
file (APPEND ${SAMPLE_LIST_HEADER} "\n#endif")

# additional include directories have to go here
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/common/include)
include_directories (${CMAKE_BINARY_DIR}/samples/common/include)

cegui_add_library_impl("CEGUISamples" FALSE SAMPLES_SOURCES SAMPLES_HEADERS FALSE FALSE)

# parse additional libs
foreach (lib ${ADDITIONAL_LIBS})
    cegui_target_link_libraries("CEGUISamples" ${lib})
endforeach ()