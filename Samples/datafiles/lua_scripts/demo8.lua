--
-- ItemListbox test
--
local guiSystem = CEGUI.System:getSingleton()
local schemeMgr = CEGUI.SchemeManager:getSingleton()
local fontMgr = CEGUI.FontManager:getSingleton()
local winMgr = CEGUI.WindowManager:getSingleton()

schemeMgr:loadScheme("WindowsLookSkin.scheme")
fontMgr:createFont("Commonwealth-10.font")
local root = winMgr:loadWindowLayout("ItemTest.layout")
guiSystem:setGUISheet(root)
guiSystem:setDefaultMouseCursor("WindowsLook", "MouseArrow")
guiSystem:setDefaultTooltip("WindowsLook/Tooltip")

-- some shortcuts
local URect = CEGUI.URect
local UVector2 = CEGUI.UVector2
local UDim = CEGUI.UDim

local fw = winMgr:getWindow("FW")
local lb = winMgr:getWindow("List")
local function fw_resize(e)
    local ir = CEGUI.CoordConverter:screenToWindow(fw, fw:getUnclippedInnerRect())
    lb:setWindowArea(URect(UDim(0,ir.left), UDim(0,ir.top), UDim(0,ir.right), UDim(0,ir.bottom)))  
end
fw:subscribeEvent("Sized", fw_resize)
fw_resize()
