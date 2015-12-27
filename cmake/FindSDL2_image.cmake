# Locate the SDL2_image library. This CMake module is a modified version
# of the original FindSDL_image.cmake file
# ###########################################################################
# Locate SDL_image library
# This module defines
# SDL2IMAGE_LIBRARY, the name of the library to link against
# SDLIMAGE_FOUND, if false, do not try to link to SDL
# SDL2IMAGE_INCLUDE_DIR, where to find SDL/SDL.h
#
# $SDLDIR is an environment variable that would
# correspond to the ./configure --prefix=$SDLDIR
# used in building SDL.
#
# Created by Eric Wing. This was influenced by the FindSDL.cmake 
# module, but with modifications to recognize OS X frameworks and 
# additional Unix paths (FreeBSD, etc).

#=============================================================================
# Copyright 2005-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distributed this file outside of CMake, substitute the full
#  License text for the above reference.)

FIND_PATH(SDL2IMAGE_H_PATH
  NAMES SDL_image.h
  HINTS
  $ENV{SDL2IMAGEDIR}
  $ENV{SDL2DIR}
  PATH_SUFFIXES include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include/SDL2
  /usr/include/SDL2
  /sw/include/SDL2 # Fink
  /opt/local/include/SDL2 # DarwinPorts
  /opt/csw/include/SDL2 # Blastwave
  /opt/include/SDL2
)

FIND_LIBRARY(SDL2IMAGE_LIB 
  NAMES SDL2_image
  HINTS
  $ENV{SDL2IMAGEDIR}
  $ENV{SDL2DIR}
  PATH_SUFFIXES lib64 lib
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)

FIND_LIBRARY(SDL2IMAGE_LIB_DBG
  NAMES SDL2_image_d
  HINTS
  $ENV{SDL2IMAGEDIR}
  $ENV{SDL2DIR}
  PATH_SUFFIXES lib64 lib
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)

mark_as_advanced(SDL2IMAGE_LIB SDL2IMAGE_LIB_DBG SDL2IMAGE_H_PATH)

cegui_find_package_handle_standard_args(SDL2IMAGE SDL2IMAGE_LIB SDL2IMAGE_H_PATH)

# set up output vars
if (SDL2IMAGE_FOUND)
    set (SDL2IMAGE_INCLUDE_DIR ${SDL2IMAGE_H_PATH})
    set (SDL2IMAGE_LIBRARIES ${SDL2IMAGE_LIB})
    if (SDL2IMAGE_LIB_DBG)
        set (SDL2IMAGE_LIBRARIES_DBG ${SDL2IMAGE_LIB_DBG})
    endif()
    if (SDL2IMAGE_LIB_STATIC)
        set (SDL2IMAGE_LIBRARIES_STATIC ${SDL2IMAGE_LIB_STATIC})
    endif()
    if (SDL2IMAGE_LIB_STATIC_DBG)
        set (SDL2IMAGE_LIBRARIES_STATIC_DBG ${SDL2IMAGE_LIB_STATIC_DBG})
    endif()
else()
    set (SDL2IMAGE_INCLUDE_DIR)
    set (SDL2IMAGE_LIBRARIES)
    set (SDL2IMAGE_LIBRARIES_DBG)
    set (SDL2IMAGE_LIBRARIES_STATIC)
    set (SDL2IMAGE_LIBRARIES_STATIC_DBG)
endif()