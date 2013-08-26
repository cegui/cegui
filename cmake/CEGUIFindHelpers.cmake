################################################################################
# CEGUIFindHelpers
# 
# Macros and things that simplify the custom cmake Find modules distributed with
# CEGUI.
################################################################################
macro(_find_dep_lib _PREFIX _FALLBACKLIBS _PATH_SUFFIXES) # _LIBS_VAR_EXT _STATIC
    set(_LIBS_VAR_EXT ${ARGV3})

    set(_OLD_CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})

    if ("${ARGV4}")
        set(_STATIC "_STATIC")

        if(UNIX)
            set(CMAKE_FIND_LIBRARY_SUFFIXES ".a;.so;.dynlib")
        endif()

    else()
        set(_STATIC)
    endif()

    # decide which libs to look for, prefer list we get from pkg-config rather
    # than making idiotic assumptions or guesses (which we resort to if
    # pkg-config did not tell us anything).
    if (PC_${_PREFIX}${_STATIC}_LIBRARIES)
        set(_FINDLIBS ${PC_${_PREFIX}${_STATIC}_LIBRARIES})
    else()
        set(_FINDLIBS ${_FALLBACKLIBS})
    endif()

    # Find all the libraries listed. Use pkg-config provided paths as hints for
    # where we can find them.
    set(${_PREFIX}_FOUND_ALL_LIBS${_LIBS_VAR_EXT} TRUE)
    foreach(_L ${_FINDLIBS})
        string(TOUPPER "${_L}" _UL)
        find_library(${_UL}_LIBRARY${_LIBS_VAR_EXT} NAMES ${_L} HINTS ${PC_${_PREFIX}_LIBDIR} ${PC_${_PREFIX}${_STATIC}_LIBRARY_DIRS} PATH_SUFFIXES ${_PATH_SUFFIXES})
        if(${_UL}_LIBRARY${_LIBS_VAR_EXT})
            list(APPEND ${_PREFIX}_LIBRARIES${_LIBS_VAR_EXT} ${${_UL}_LIBRARY${_LIBS_VAR_EXT}})
        else()
            set(${_PREFIX}_FOUND_ALL_LIBS${_LIBS_VAR_EXT} FALSE)
        endif()
        mark_as_advanced(${_UL}_LIBRARY${_LIBS_VAR_EXT})
    endforeach()

    if (NOT ${_PREFIX}_FOUND_ALL_LIBS${_LIBS_VAR_EXT})
        set(${_PREFIX}_LIBRARIES${_LIBS_VAR_EXT})
    endif()

    set(CMAKE_FIND_LIBRARY_SUFFIXES ${_OLD_CMAKE_FIND_LIBRARY_SUFFIXES})

endmacro()

macro(cegui_find_package _PREFIX _LIB _H _H_SUFFIXES _PC_MOD)
    # Do pkg-config lookup
    if (NOT _PC_MOD STREQUAL "")
        find_package(PkgConfig)
        pkg_check_modules(PC_${_PREFIX} ${_PC_MOD})
    endif()

    # Do the check for the header
    find_path(${_PREFIX}_H_PATH
        NAMES ${_H}
        HINTS ${PC_${_PREFIX}_INCLUDEDIR} ${PC_${_PREFIX}_INCLUDE_DIRS}
        PATH_SUFFIXES ${_H_SUFFIXES}
    )
    mark_as_advanced(${_PREFIX}_H_PATH)

    # set up the list of include dirs
    list(APPEND ${_PREFIX}_INCLUDE_DIR ${${_PREFIX}_H_PATH} ${PC_${_PREFIX}_INCLUDEDIR} ${PC_${_PREFIX}_INCLUDE_DIRS})
    list(REMOVE_DUPLICATES ${_PREFIX}_INCLUDE_DIR)

    _find_dep_lib(${_PREFIX} ${_LIB} dynamic)
    _find_dep_lib(${_PREFIX} ${_LIB}_d dynamic _DBG)
    _find_dep_lib(${_PREFIX} ${_LIB} static _STATIC TRUE)
    _find_dep_lib(${_PREFIX} ${_LIB}_d static _STATIC_DBG TRUE)

    cegui_find_package_handle_standard_args(${_PREFIX} ${_PREFIX}_FOUND_ALL_LIBS ${_PREFIX}_INCLUDE_DIR)

    # clear output vars if not found
    if (NOT ${_PREFIX}_FOUND)
        set (${_PREFIX}_INCLUDE_DIR)
        set (${_PREFIX}_LIBRARIES)
        set (${_PREFIX}_LIBRARIES_DBG)
        set (${_PREFIX}_LIBRARIES_STATIC)
        set (${_PREFIX}_LIBRARIES_STATIC_DBG)
    endif()

endmacro()
