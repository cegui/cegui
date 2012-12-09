-----------------------------------------
-- Start of handler functions
-----------------------------------------
-----------------------------------------
-- Alpha slider handler (not used!)
-----------------------------------------
function sliderHandler(args)
    CEGUI.System:getSingleton():getGUISheet():setAlpha(CEGUI.toSlider(CEGUI.toWindowEventArgs(args).window):getCurrentValue())
end

-----------------------------------------
-- Handler to slide pane
--
-- Here we move the 'Demo8' sheet window
-- and re-position the scrollbar
-----------------------------------------
function panelSlideHandler(args)
    local scroller = CEGUI.toScrollbar(CEGUI.toWindowEventArgs(args).window)
    local demoWnd = CEGUI.WindowManager:getSingleton():getWindow("Demo8")

    local relHeight = demoWnd:getHeight():asRelative(demoWnd:getParentPixelHeight())

    scroller:setPosition(CEGUI.UVector2(CEGUI.UDim(0,0), CEGUI.UDim(scroller:getScrollPosition() / relHeight,0)))
    demoWnd:setPosition(CEGUI.UVector2(CEGUI.UDim(0,0), CEGUI.UDim(-scroller:getScrollPosition(),0)))
end

-----------------------------------------
-- Handler to set preview colour when
-- colour selector scrollers change
-----------------------------------------
function colourChangeHandler(args)
    local winMgr = CEGUI.WindowManager:getSingleton()
    
    local r = CEGUI.toScrollbar(winMgr:getWindow("Demo8/Window1/Controls/Red")):getScrollPosition()
    local g = CEGUI.toScrollbar(winMgr:getWindow("Demo8/Window1/Controls/Green")):getScrollPosition()
    local b = CEGUI.toScrollbar(winMgr:getWindow("Demo8/Window1/Controls/Blue")):getScrollPosition()
    local col = CEGUI.colour:new_local(r, g, b, 1)
    local crect = CEGUI.ColourRect(col)

    winMgr:getWindow("Demo8/Window1/Controls/ColourSample"):setProperty("ImageColours", CEGUI.PropertyHelper:colourRectToString(crect))
end


-----------------------------------------
-- Handler to add an item to the box
-----------------------------------------
function addItemHandler(args)
    local winMgr = CEGUI.WindowManager:getSingleton()

    local text = winMgr:getWindow("Demo8/Window1/Controls/Editbox"):getText()
    local cols = CEGUI.PropertyHelper:stringToColourRect(winMgr:getWindow("Demo8/Window1/Controls/ColourSample"):getProperty("ImageColours"))

    local newItem = CEGUI.createListboxTextItem(text, 0, nil, false, true)
    newItem:setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush")
    newItem:setSelectionColours(cols)

    CEGUI.toListbox(winMgr:getWindow("Demo8/Window1/Listbox")):addItem(newItem)
end

-----------------------------------------
-- Script Entry Point
-----------------------------------------
local guiSystem = CEGUI.System:getSingleton()
local schemeMgr = CEGUI.SchemeManager:getSingleton()
local winMgr = CEGUI.WindowManager:getSingleton()

-- load our demo8 scheme
schemeMgr:create("Demo8.scheme");
-- load our demo8 window layout
local root = winMgr:loadWindowLayout("Demo8.layout")
-- set the layout as the root
guiSystem:setGUISheet(root)
-- set default mouse cursor
guiSystem:setDefaultMouseCursor("TaharezLook", "MouseArrow")
-- set the Tooltip type
guiSystem:setDefaultTooltip("TaharezLook/Tooltip")

-- subscribe required events
winMgr:getWindow("Demo8/ViewScroll"):subscribeEvent("ScrollPosChanged", "panelSlideHandler")
winMgr:getWindow("Demo8/Window1/Controls/Blue"):subscribeEvent("ScrollPosChanged", "colourChangeHandler")
winMgr:getWindow("Demo8/Window1/Controls/Red"):subscribeEvent("ScrollPosChanged", "colourChangeHandler")
winMgr:getWindow("Demo8/Window1/Controls/Green"):subscribeEvent("ScrollPosChanged", "colourChangeHandler")
winMgr:getWindow("Demo8/Window1/Controls/Add"):subscribeEvent("Clicked", "addItemHandler")
