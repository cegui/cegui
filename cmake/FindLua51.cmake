################################################################################
# Custom cmake module for CEGUI to find lua (5.1)
################################################################################
include(FindPackageHandleStandardArgs)

find_path(LUA_H_PATH NAMES lua.h PATH_SUFFIXES lua5.1 lua51 lua-5.1 lua)
find_library(LUA_LIB NAMES lua5.1 lua51 lua-5.1 lua PATH_SUFFIXES dynamic)
find_library(LUA_LIB_DBG NAMES lua5.1_d lua51_d lua-5.1_d lua_d PATH_SUFFIXES dynamic)
mark_as_advanced(LUA_H_PATH LUA_LIB LUA_LIB_DBG)

if (WIN32 OR APPLE)
    find_library(LUA_LIB_STATIC NAMES lua5.1 lua51 lua-5.1 lua PATH_SUFFIXES static)
    find_library(LUA_LIB_STATIC_DBG NAMES lua5.1_d lua51_d lua-5.1_d lua_d PATH_SUFFIXES static)
    mark_as_advanced(LUA_LIB_STATIC LUA_LIB_STATIC_DBG)
endif()

cegui_find_package_handle_standard_args(LUA51 LUA_LIB LUA_H_PATH)


# set up output vars
if (LUA51_FOUND)
    set (LUA51_INCLUDE_DIR ${LUA_H_PATH})

    if (UNIX AND NOT APPLE)
        find_library(LUA_MATH_LIB m)
        set (LUA51_LIBRARIES ${LUA_LIB} ${LUA_MATH_LIB})
        mark_as_advanced(LUA_MATH_LIB)
    else()
        set (LUA51_LIBRARIES ${LUA_LIB})
    endif()
    if (LUA_LIB_DBG)
        set (LUA51_LIBRARIES_DBG ${LUA_LIB_DBG})
    endif()
    if (LUA_LIB_STATIC)
        set (LUA51_LIBRARIES_STATIC ${LUA_LIB_STATIC})
    endif()
    if (LUA_LIB_STATIC_DBG)
        set (LUA51_LIBRARIES_STATIC_DBG ${LUA_LIB_STATIC_DBG})
    endif()
else()
    set (LUA51_INCLUDE_DIR)
    set (LUA51_LIBRARIES)
    set (LUA51_LIBRARIES_DBG)
    set (LUA51_LIBRARIES_STATIC)
    set (LUA51_LIBRARIES_STATIC_DBG)
endif()

