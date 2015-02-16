################################################################################
# Custom cmake module for CEGUI to find the glm header
################################################################################
include(FindPackageHandleStandardArgs)

find_path(GLM_H_PATH NAMES glm/glm.hpp glm.hpp)
mark_as_advanced(GLM_H_PATH)

find_package_handle_standard_args(GLM DEFAULT_MSG GLM_H_PATH)


# set up output vars
if (GLM_FOUND)
    set (GLM_INCLUDE_DIR ${GLM_H_PATH})
    find_path(OLDER_VERSION NAMES glm/gtx/constants.hpp gtx/constants.hpp)
    if (NOT OLDER_VERSION)
      set (CEGUI_GLM_HAS_0_9_6_API TRUE)
    endif()
else()
    set (GLM_INCLUDE_DIR)
endif()

