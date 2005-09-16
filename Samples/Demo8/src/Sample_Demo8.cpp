/************************************************************************
    filename:   Sample_Demo8.cpp
    created:    20/8/2005
    author:     Paul D Turner
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "Sample_Demo8.h"
#include "CEGUI.h"
#include "CEGUILua.h"

#if defined( __WIN32__ ) || defined( _WIN32 )
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,int nCmdShow)
#else
int main(int argc, char *argv[])
#endif
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

    // get a pointer to the renderer created by the base app object.
    Renderer* renderer = System::getSingleton().getRenderer();

    // delete the CEGUI::System created by the base app.
    //
    // NB:  You would not normally do this, we're doing this here
    // since we need to re-initialise everything in order to use the
    // scripting support.
    delete System::getSingletonPtr();

    // create a script module.
    LuaScriptModule* scriptmod = new LuaScriptModule();

    // now re-create the CEGUI::System passing in our scripting module
    // and basically 'bootstrapping' the demo via a config file.
    new System(renderer, scriptmod, (const utf8*)"../datafiles/configs/demo8.config");

    // success!
    return true;
}

/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void Demo8Sample::cleanupSample()
{
    // This is a bit dangerous!
    delete CEGUI::System::getSingleton().getScriptingModule();
}
