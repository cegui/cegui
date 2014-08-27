# Lists all subdirectories of a given directory
macro(subdirlist result curdir)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
    set(dirlist "")
    foreach(child ${children})
        if(IS_DIRECTORY ${curdir}/${child})
            list(APPEND dirlist ${child})
        endif()
    endforeach()
    set(${result} ${dirlist})
endmacro()

# initialize variables
set(ADDITIONAL_LIBS "")
set(ADDITIONAL_INCLUDE_DIRS "")
set(SAMPLES_SOURCES "")
set(SAMPLES_HEADERS "")
set(SAMPLES_LIST "")
set(SAMPLES_SUBDIRS "")

set(SAMPLE_LIST_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/common/include/samples.h)
set(SAMPLE_LIST_CODE ${CMAKE_CURRENT_SOURCE_DIR}/common/include/samples.inc)

# gather sample dirs
subdirlist(SAMPLES_SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

# loop through samples and exclude those that should not be build 
foreach(dir ${SAMPLES_SUBDIRS})
    if("${dir}" MATCHES "^GLESDemo$") # exclude the hacky GLES demo
    elseif("${dir}" MATCHES "^common$") # exclude common folder; samples include files from there directly when necessary
    elseif("${dir}" MATCHES "^samples$") # exclude this directory
    elseif("${dir}" MATCHES "^Demo8$") # this demo doesnt work at the moment
    elseif("${dir}" MATCHES "^SimpleGameMenu$") # the sample is broken somewhere
    elseif("${dir}" MATCHES "^browser$") # the directory has to be included at the end because it needs all samples' sources and headers
    else()
        add_subdirectory(${dir})
    endif()
endforeach()

# add all registered samples to samples.inc and samples.h file
file(WRITE ${SAMPLE_LIST_HEADER} "#ifndef _SamplesListHeader_h_\n#define _SamplesListHeader_h_\n\n#include \"Sample.h\"\n")
file(WRITE ${SAMPLE_LIST_CODE} "std::vector<Sample*> samples;")
foreach(sample ${SAMPLES_LIST})
    set(_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${sample}/${sample}.h")
    file(APPEND ${SAMPLE_LIST_HEADER} "\n#include \"${_PATH}\"")
    file(APPEND ${SAMPLE_LIST_CODE} "\nsamples.push_back(new ${sample}Sample());")
endforeach()
file(APPEND ${SAMPLE_LIST_HEADER} "\n#endif")

# additional include directories have to appear here
list(APPEND ADDITIONAL_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/common/include")
list(APPEND ADDITIONAL_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/samples/common/include")

add_subdirectory(browser/)