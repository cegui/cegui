################################################################################
# Custom cmake module for CEGUI to see if iconv exists
################################################################################
include(FindPackageHandleStandardArgs)
include(CheckCXXSourceCompiles)

find_path(ICONV_H_PATH NAMES iconv.h)
find_library(ICONV_LIB NAMES iconv)

if (ICONV_H_PATH)
    check_cxx_source_compiles("
    #include <iconv.h>
    int main() {
        iconv(0, 0, 0, 0, 0);
        return 0;
    }"

    ICONV_LIB_GLIBC)

    if (NOT ICONV_LIB_GLIBC)
        find_package_handle_standard_args(Iconv DEFAULT_MSG ICONV_LIB ICONV_H_PATH)
    else()
        set(ICONV_FOUND TRUE)
        unset(ICONV_LIB)
    endif()

endif()

mark_as_advanced(ICONV_H_PATH ICONV_LIB ICONV_LIB_GLIBC)

# set up output vars
if (ICONV_FOUND)
    set (ICONV_INCLUDE_DIR ${ICONV_H_PATH})
    set (ICONV_LIBRARIES ${ICONV_LIB})
else()
    set (ICONV_INCLUDE_DIR)
    set (ICONV_LIBRARIES)
endif()

