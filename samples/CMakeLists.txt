add_subdirectory(common)
add_subdirectory(Demo6)
add_subdirectory(Demo7)
add_subdirectory(DragDropDemo)
add_subdirectory(FalagardDemo1)
add_subdirectory(FirstWindow)
add_subdirectory(FontDemo)
add_subdirectory(InventoryDemo)
add_subdirectory(Minesweeper)
add_subdirectory(ScrollablePaneDemo)
add_subdirectory(TabControlDemo)
add_subdirectory(TextDemo)
add_subdirectory(TreeDemo)

add_subdirectory(CommonDialogsDemo)

if (CEGUI_BUILD_LUA_MODULE)
    add_subdirectory(Demo8)
endif()

# Simple test for GLES renderer.
#
# Commented since it's does not use the samples framework and is merely included
# such that recreation from scratch is not required in the future.  Aka, it's
# one of CrazyEddie's hacks that he wants to keep around :-p
#if (CEGUI_BUILD_RENDERER_OPENGLES)
#    add_subdirectory(GLESTest)
#endif()
