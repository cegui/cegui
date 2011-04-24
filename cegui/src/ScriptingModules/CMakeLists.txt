if (CEGUI_BUILD_EMBEDDED_TOLUAPP)
    add_subdirectory(LuaScriptModule/support/tolua++)
endif()

if (CEGUI_BUILD_LUA_GENERATOR)
    add_subdirectory(LuaScriptModule/support/tolua++bin)
endif()

if (CEGUI_BUILD_LUA_MODULE)
    add_subdirectory(LuaScriptModule)
endif()

if (CEGUI_BUILD_PYTHON_MODULES)
    add_subdirectory(PythonScriptModule/bindings)
endif()

