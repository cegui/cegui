-----------------------------------------
-- Script Entry Point
-----------------------------------------
local guiSystem = CEGUI.System:getSingleton()
local schemeMgr = CEGUI.SchemeManager:getSingleton()
local winMgr = CEGUI.WindowManager:getSingleton()

-- load our demo8 scheme
schemeMgr:loadScheme("Demo8.scheme");
-- load our demo8 window layout
local root = winMgr:createWindow("DefaultWindow","Root")
-- set the layout as the root
guiSystem:setGUISheet(root)
-- set default mouse cursor
guiSystem:setDefaultMouseCursor("TaharezLook", "MouseArrow")
-- set the Tooltip type
guiSystem:setDefaultTooltip("TaharezLook/Tooltip")
