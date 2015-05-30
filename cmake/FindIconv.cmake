# taken from:
# http://code.google.com/p/tinygettext/source/browse/trunk/FindICONV.cmake?spec=svn183&r=183

#
#  Copyright (c) 2006, Peter Kümmel, <syntheticpp@gmx.net>
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#  
#  1. Redistributions of source code must retain the copyright
#     notice, this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#  3. The name of the author may not be used to endorse or promote products 
#     derived from this software without specific prior written permission.
#  
#  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
#  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
#  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
#  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
#  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
#  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
#  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 

################################################################################
# Adapted for CEGUI
################################################################################

set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)
include(CheckFunctionExists)
if (ICONV_H_PATH)
  # Already in cache, be silent
  set(ICONV_FIND_QUIETLY TRUE)
endif()

find_path(ICONV_H_PATH iconv.h
 /usr/include
 /usr/local/include)

set(POTENTIAL_ICONV_LIBS iconv libiconv libiconv2)

find_library(ICONV_LIB NAMES ${POTENTIAL_ICONV_LIBS}
	PATHS /usr/lib /usr/local/lib)

if(WIN32)
	set(ICONV_DLL_NAMES iconv.dll  libiconv.dll libiconv2.dll)
	find_file(ICONV_DLL   
					NAMES ${ICONV_DLL_NAMES}
					PATHS ENV PATH
					NO_DEFAULT_PATH)
	find_file(ICONV_DLL_HELP   
					NAMES ${ICONV_DLL_NAMES}
					PATHS ENV PATH
					${ICONV_H_PATH}/../bin)
	if(ICONV_FIND_REQUIRED)
		if(NOT ICONV_DLL AND NOT ICONV_DLL_HELP)
			message(FATAL_ERROR "Could not find iconv.dll, please add correct your PATH environment variable")
		endif()
		if(NOT ICONV_DLL AND ICONV_DLL_HELP)
			get_filename_component(ICONV_DLL_HELP ${ICONV_DLL_HELP} PATH)
			message(STATUS)
			message(STATUS "Could not find iconv.dll in standard search path, please add ")
			message(STATUS "${ICONV_DLL_HELP}")
			message(STATUS "to your PATH environment variable.")
			message(STATUS)
			message(FATAL_ERROR "exit cmake")
		endif()
	endif()
	if(ICONV_H_PATH AND ICONV_LIB AND ICONV_DLL)
   		set(ICONV_FOUND TRUE)
	endif()
else()
	check_function_exists(iconv HAVE_ICONV_IN_LIBC)
	if(ICONV_H_PATH AND HAVE_ICONV_IN_LIBC)
   		set(ICONV_FOUND TRUE)
		set(ICONV_LIB  CACHE TYPE STRING FORCE)
	endif()
	if(ICONV_H_PATH AND ICONV_LIB)
   		set(ICONV_FOUND TRUE)
	endif()
endif()

mark_as_advanced(ICONV_H_PATH ICONV_LIB)

# set up output vars
if (ICONV_FOUND)
    set (ICONV_INCLUDE_DIR ${ICONV_H_PATH})
    set (ICONV_LIBRARIES ${ICONV_LIB})
else()
    set (ICONV_INCLUDE_DIR)
    set (ICONV_LIBRARIES)
endif()

