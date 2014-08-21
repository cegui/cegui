/***********************************************************************
    created:    20/8/2005
    author:     Paul D Turner
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
#include "Sample_Demo8.h"
#include "CEGUI/CEGUI.h"
#include "CEGUI/ScriptModules/Lua/ScriptModule.h"

int main(int /*argc*/, char* /*argv*/[])
{
    // This is a basic start-up for the sample application which is
    // object orientated in nature, so we just need an instance of
    // the CEGuiSample based object and then tell that sample application
    // to run.  All of the samples will use code similar to this in the
    // main/WinMain function.
    Demo8Sample app;
    return app.run();
}


/*************************************************************************
    Sample specific initialisation goes here.
*************************************************************************/
bool Demo8Sample::initialiseSample()
{
    using namespace CEGUI;

    // create a script module.
    LuaScriptModule& scriptmod(LuaScriptModule::create());

    // tell CEGUI to use this scripting module
    System::getSingleton().setScriptingModule(&scriptmod);

    // execute the demo8 script which controls the rest of this demo
    System::getSingleton().executeScriptFile("demo8.lua");

    // success!
    return true;
}

/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void Demo8Sample::cleanupSample()
{
    using namespace CEGUI;

    LuaScriptModule* script_mod = static_cast<LuaScriptModule*>(
        System::getSingleton().getScriptingModule());

    // clear script module, since we're going to destroy it.
    System::getSingleton().setScriptingModule(0);

    LuaScriptModule::destroy(*script_mod);
}
