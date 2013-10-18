# ----------------------------------------------------------------------------------------------------------------------
# This file is part of the CMake build system for Summoning Wars (http://sumwars.org/)
#
# The contents of this file are placed in the public domain. Feel free to make use of it in any way you like.
# ----------------------------------------------------------------------------------------------------------------------
# 
# The purpose of this script is to determine the library that is used to provide threading support in OGRE.
# This script provides a set of functions to access the configured OGRE installation and read the OgreBuildSettings file.
# 
# E.g.: To store the ogre thread provider into the variable "OGRE_THREAD_PROVIDER", use the function call:
#   DETERMINE_OGRE_THREAD_PROVIDER(OGRE_THREAD_PROVIDER ${MY_OGRE_FOLDER}/OgreBuildSettings.h)
#
# Author: Augustin Preda.
#     Based on the "DetermineVersion.cmake" file by  Reto Grieder.
# Revision history:
# version 1, 2012.01.13: First commit of file. Single function: DETERMINE_OGRE_THREAD_PROVIDER
# 
# ----------------------------------------------------------------------------------------------------------------------


# ----------------------------------------------------------------------------------------------------------------------
# Functions:
# - DETERMINE_OGRE_THREAD_PROVIDER
# 
# ----------------------------------------------------------------------------------------------------------------------


# ----------------------------------------------------------------------------------------------------------------------
# Determine the OGRE Thread provider configured in the header file.
# Parameters:
#   - _tp_var: Output parameter. The variable in which to store the OGRE_THREAD_PROVIDER read from the OGRE config.
#   - _file: The name and path of the file to search into. This should typically be the "OgreBuildSettings.h" file.
#
# At the time of the writing of this function, the following possible values are defined in OGRE and can be stored in 
#_tp_var:
#   1 = boost
#   2 = poco
#   3 = tbb
# ----------------------------------------------------------------------------------------------------------------------
FUNCTION(DETERMINE_OGRE_THREAD_PROVIDER _tp_var _file)
  IF(EXISTS ${_file})
    FILE(READ ${_file} _file_content)
    SET(_parts ${ARGN})
    LIST(LENGTH _parts _parts_length)
    IF(NOT ${_parts_length} EQUAL 3)
      SET(_parts MAJOR MINOR PATCH)
    ENDIF()

    FOREACH(_part ${_parts})
      STRING(REGEX MATCH "#define OGRE_THREAD_PROVIDER +([0-9]+)" _match ${_file_content})
      IF(_match)
        SET(${_tp_var} ${CMAKE_MATCH_1})
        SET(${_tp_var} ${CMAKE_MATCH_1} PARENT_SCOPE)
      ELSE()
        SET(${_tp_var} "0")
        SET(${_tp_var} "0" PARENT_SCOPE)
      ENDIF()
    ENDFOREACH(_part)
    SET(${_tp_var}_VERSION "${${_tp_var}_VERSION}" PARENT_SCOPE)
  ENDIF(EXISTS ${_file})
ENDFUNCTION(DETERMINE_OGRE_THREAD_PROVIDER)
