--- Event Handlers ---

---------------------------------------------------------
---	Event to handle when "Return" is hit in the edit box
--- Input Window: Console/Frame/Input
---------------------------------------------------------
function handleEntryAccepted(args)
	-- Get a pointer to the input editbox
	local input = this:getWindow() 
	-- Get a pointer to the output MLEditbox
	local output = this:getWindow("Console/Frame/ConsoleText")
	
	-- Retrieve the Text from the edit box
	local instr = input:getText()
		
	-- Set the Editbox text to an empty string to clear out the entry
	input:setText("")
	
	-- set the text in the output box to its current text + the new text.
	output:setText(string.format("%s%s",output:getText(),instr))
	
	--Done!
end

function update(args)
   --Yatta! -- this is called every frame -- Put somthing interesting here!
end

--- End Event Handlers ---

--- Entry Point ---
local winMgr = CEGUI.WindowManager:getSingleton()

--- Load the xml for the instanced window we want, specifying true
--- indicates that we are making an instanced window and the system will
--- autogenerate a prefix for us.
local root = winMgr:loadWindowLayout("Console.wnd",true)

--- Since we are just now creating this window, there is no 'this' pointer
--- for us to use. Instead since now we want to set the events we will use
--- the getChild function which will recursivly search from the given node down
--- until it find the window... it will throw an exception.
root:getChild("Console/Frame/Input"):subscribeEvent("TextAccepted",handleEntryAccepted)

--- This event is the window update event, it is called every frame
root:subscribeEvent("WindowUpdate",update)

-- Add it to the default GUI sheet
CEGUI.System:getSingleton():getGUISheet():addChildWindow(root)
	