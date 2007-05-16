/***********************************************************************
filename:   Instanced.cpp
created:    05-13-07
author:     Jonathan Welch (Kokoro-Sama)

purpose:    Demonstrates the use of instanced windows with XML and lua
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
*
*   Permission is hereby granted, free of charge, to any person obtaining
*   a copy of this software and associated documentation files (the
*   "Software"), to deal in the Software without restriction, including
*   without limitation the rights to use, copy, modify, merge, publish,
*   distribute, sublicense, and/or sell copies of the Software, and to
*   permit persons to whom the Software is furnished to do so, subject to
*   the following conditions:
*
*   The above copyright notice and this permission notice shall be
*   included in all copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
*   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
*   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
*   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
*   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
*   OTHER DEALINGS IN THE SOFTWARE.
***************************************************************************/

#include "../include/Instanced.h"
#include "CEGUI.h"
#include "CEGUILua.h"

using namespace CEGUI;

int main(int argc, char**argv)
{

	DemoInstanced app;
	return app.run();
}

/*************************************************************************
	Sample specific initialisation goes here.
*************************************************************************/
bool DemoInstanced::initialiseSample()
{
	using namespace CEGUI;

	//Create a scripting module
	LuaScriptModule* script = new LuaScriptModule();

	//Setup the System to use this module
	System::getSingleton().setScriptingModule(script);

	//Execute the InstancedInit.lua script which will setup the environment
	System::getSingleton().executeScriptFile("InstancedInit.lua");

	//Now lets create two different windows using the same lua/xml combo
	for(int i = 0; i<2; i++)
		System::getSingleton().executeScriptFile("IConsole.lua");

	return true;
}

void DemoInstanced::cleanupSample()
{
	delete System::getSingleton().getScriptingModule();
}