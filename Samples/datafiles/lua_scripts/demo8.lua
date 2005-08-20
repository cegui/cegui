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

	scroller:setPosition(CEGUI.Relative, CEGUI.Point:new_local(0, scroller:getScrollPosition() / demoWnd:getRelativeHeight()))
	demoWnd:setPosition(CEGUI.Relative, CEGUI.Point:new_local(0, -scroller:getScrollPosition()))
end

-----------------------------------------
-- Handler to set tip window text
-----------------------------------------
function tipHandler(args)
	local tipWnd = CEGUI.WindowManager:getSingleton():getWindow("Demo8/Window2/Tips")
	local tipstring = Tips[CEGUI.toWindowEventArgs(args).window:getName()]

	tipWnd:setText(tipstring)
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

	CEGUI.toStaticImage(winMgr:getWindow("Demo8/Window1/Controls/ColourSample")):setImageColours(col)
end


-----------------------------------------
-- Handler to add an item to the box
-----------------------------------------
function addItemHandler(args)
	local winMgr = CEGUI.WindowManager:getSingleton()

	local text = winMgr:getWindow("Demo8/Window1/Controls/Editbox"):getText()
	local cols = CEGUI.toStaticImage(winMgr:getWindow("Demo8/Window1/Controls/ColourSample")):getImageColours()

	local newItem = CEGUI.createListboxTextItem(text, 0, nil, false, true)
	newItem:setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush")
	newItem:setSelectionColours(cols)

	CEGUI.toListbox(winMgr:getWindow("Demo8/Window1/Listbox")):addItem(newItem)
end

-----------------------------------------
-- Script Entry Point
-----------------------------------------
local guiSystem = CEGUI.System:getSingleton()
local winMgr = CEGUI.WindowManager:getSingleton()
local root = guiSystem:getGUISheet()

-- set default mouse cursor
guiSystem:setDefaultMouseCursor("TaharezLook", "MouseArrow")

-- init the table we'll use to keep the added items 'alive'
ListItems = {}
ListItems.next = 0

-- init table of tips
Tips = {}
Tips["Demo8/Window1/Controls/Red"] = "Slider to adjust the red colour component."
Tips["Demo8/Window1/Controls/Green"] = "Slider to adjust the green colour component."
Tips["Demo8/Window1/Controls/Blue"] = "Slider to adjust the blue component."
Tips["Demo8/Window1/Controls/Editbox"] = "Enter the text for a new list box item here."
Tips["Demo8/Window1/Controls/Add"] = "Button to add a new item to the list box."
Tips["Demo8/Window1/Controls/ColourSample"] = "Contains a sample of the selected colour."
Tips["Demo8/Window1/Controls/ins1"] = "Contains brief instructions for the list box panel."
Tips["Demo8/Window1"] = "Demo Window 1 - contains a simple Listbox demonstration."
Tips["Demo8/Window2"] = "Demo Window 2 - contains some information panels."
Tips["Demo8/Window2/Info"] = "Contains a description of this tips panel."
Tips["Demo8/Window2/Tips"] = "Tips panel - will show brief information about the demo controls."
Tips["Demo8/ViewScroll"] = "This scroll bar can be used to scroll the main view."
Tips["Demo8/Window1/Listbox"] = "Listbox widget containing the items you have added."

-- subscribe required events
winMgr:getWindow("Demo8/ViewScroll"):subscribeEvent("ScrollPosChanged", "panelSlideHandler")
winMgr:getWindow("Demo8/Window1/Controls/Blue"):subscribeEvent("ScrollPosChanged", "colourChangeHandler")
winMgr:getWindow("Demo8/Window1/Controls/Red"):subscribeEvent("ScrollPosChanged", "colourChangeHandler")
winMgr:getWindow("Demo8/Window1/Controls/Green"):subscribeEvent("ScrollPosChanged", "colourChangeHandler")
winMgr:getWindow("Demo8/Window1/Controls/Red"):subscribeEvent("MouseEnter", "tipHandler")
winMgr:getWindow("Demo8/Window1/Controls/Green"):subscribeEvent("MouseEnter", "tipHandler")
winMgr:getWindow("Demo8/Window1/Controls/Blue"):subscribeEvent("MouseEnter", "tipHandler")
winMgr:getWindow("Demo8/Window1/Controls/ColourSample"):subscribeEvent("MouseEnter", "tipHandler")
winMgr:getWindow("Demo8/Window1/Controls/ins1"):subscribeEvent("MouseEnter", "tipHandler")
winMgr:getWindow("Demo8/Window1/Controls/Editbox"):subscribeEvent("MouseEnter", "tipHandler")
winMgr:getWindow("Demo8/Window1/Controls/Add"):subscribeEvent("MouseEnter", "tipHandler")
winMgr:getWindow("Demo8/Window1"):subscribeEvent("MouseEnter", "tipHandler")
winMgr:getWindow("Demo8/Window2"):subscribeEvent("MouseEnter", "tipHandler")
winMgr:getWindow("Demo8/Window2/Info"):subscribeEvent("MouseEnter", "tipHandler")
winMgr:getWindow("Demo8/Window2/Tips"):subscribeEvent("MouseEnter", "tipHandler")
winMgr:getWindow("Demo8/ViewScroll"):subscribeEvent("MouseEnter", "tipHandler")
winMgr:getWindow("Demo8/Window1/Listbox"):subscribeEvent("MouseEnter", "tipHandler")
winMgr:getWindow("Demo8/Window1/Controls/Add"):subscribeEvent("Clicked", "addItemHandler")
