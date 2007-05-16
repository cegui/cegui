--- Init Script for Instanced Windows Demo ---
local guiSystem = CEGUI.System:getSingleton()
local schemeMgr = CEGUI.SchemeManager:getSingleton()
local winMgr = CEGUI.WindowManager:getSingleton()

-- Load a Scheme File
schemeMgr:loadScheme("TaharezLook.scheme")
-- Setup Mouse Pointer
guiSystem:setDefaultMouseCursor("TaharezLook", "MouseArrow")
-- Setup a DefaultSheet
local root = winMgr:createWindow("DefaultWindow","RootNode")
local xx = CEGUI.UDim(1,0)
local yy = CEGUI.UDim(1,0)
local zz = CEGUI.UVector2(xx,yy)
root:setSize(zz)
guiSystem:setGUISheet(root)
-- set the Tooltip type
guiSystem:setDefaultTooltip("TaharezLook/Tooltip")
-- Done!