 execute_process(
      COMMAND
        ${PYTHON_EXECUTABLE} -c "import sys;print (str(sys.version_info[0]) + '.' + str(sys.version_info[1]) + '.' + str(sys.version_info[2]))"
        OUTPUT_VARIABLE PYTHON_VERSION_STRING
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE RUN_RESULT
 )
set( PYTHON_VERSION_FOUND FALSE)
string(COMPARE EQUAL ${RUN_RESULT} "0" RUN_SUCCESS)
if (RUN_SUCCESS)
   set( PYTHON_VERSION_FOUND TRUE)
   string(REPLACE "." " " _PY_LIST  ${PYTHON_VERSION_STRING} )
   list(LENGTH _PY_LIST _PY_LIST_LEN)
   if ( _PY_LIST_LEN GREATER 0)
      list(GET _PY_LIST 0 PYTHON_VERSION_STRING_MAJOR)
   endif()
   if ( _PY_LIST_LEN GREATER 1)
      list(GET _PY_LIST 0 PYTHON_VERSION_STRING_MINOR)
   endif()
   if ( _PY_LIST_LEN GREATER 2)
      list(GET _PY_LIST 0 PYTHON_VERSION_STRING_MICRO)
   endif()
endif()

