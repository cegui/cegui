-- tolua: module class
-- Written by Waldemar Celes
-- TeCGraf/PUC-Rio
-- Jul 1998
-- $Id$

-- This code is free software; you can redistribute it and/or modify it.
-- The software provided hereunder is on an "as is" basis, and
-- the author has no obligation to provide maintenance, support, updates,
-- enhancements, or modifications.



-- Module class
-- Represents module.
-- The following fields are stored:
--    {i} = list of objects in the module.
classModule = {
 classtype = 'module'
}
classModule.__index = classModule
setmetatable(classModule,classContainer)

-- register module
function classModule:register (pre)
 pre = pre or ''
 push(self)
 output(pre..'tolua_module(tolua_S,"'..self.name..'",',self:hasvar(),');')
 output(pre..'tolua_beginmodule(tolua_S,"'..self.name..'");')
 local i=1
 while self[i] do
  self[i]:register(pre..' ')
  i = i+1
 end
 output(pre..'tolua_endmodule(tolua_S);')
	pop()
end


--- 
-- LuaDoc Patch
-- outputs an empty(without documentation) LuaDoc interface 
-- by klapeto (http://cegui.org.uk/forum/viewtopic.php?f=7&t=6784)
function classModule:output_luadoc()

	output('---------------------------------------------------------------------------------\n')
	if (self.parent.name and self.parent.name~='') then
		output('-- @field [parent=#'..cleanseType(self.parent.name)..'] #'..cleanseType(self.name)..' '..cleanseType(self.name)..'\n')
	else
		local name = cleanseType(self.name)
		output('---------------------------------------------------------------------------------\n')
		output('-- @module '..name..'\n')
		output('\n')
		output('---\n')
		output('-- @field [parent=#global] #'..name..' '..name..'\n')
	end
	
	output('\n')
	
	output('---------------------------------------------------------------------------------\n')
	output('-- '..self.name..'\n')
	output('-- @type '..cleanseType(self.name)..'\n')
	
	output('\n')
	
	local i=1
	while self[i] do
		self[i]:output_luadoc()
		i = i+1
	end

end

-- Print method
function classModule:print (ident,close)
 print(ident.."Module{")
 print(ident.." name = '"..self.name.."';")
 local i=1
 while self[i] do
  self[i]:print(ident.." ",",")
  i = i+1
 end
 print(ident.."}"..close)
end

-- Internal constructor
function _Module (t)
 setmetatable(t,classModule)
 append(t)
 return t
end

-- Constructor
-- Expects two string representing the module name and body.
function Module (n,b)
 local t = _Module(_Container{name=n})
 push(t)
 t:parse(strsub(b,2,strlen(b)-1)) -- eliminate braces
 pop()
 return t
end


