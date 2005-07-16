/************************************************************************
    filename:   Sample_FalagardDemo1.cpp
    created:    19/6/2005
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
#include "Sample_FalagardDemo1.h"
#include "CEGUI.h"
#include "falagard/CEGUIFalWidgetLookManager.h"

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
    try
    {
        FalagardDemo1Sample app;
        return app.run();
    }
    catch(...)
    {
        // TODO: Print error message!

        return 0;
    }

}

/*************************************************************************
    Sample specific initialisation goes here.
*************************************************************************/
bool FalagardDemo1Sample::initialiseSample()
{
    using namespace CEGUI;

    // This is a temporary sample so Falagard can be tested.  I named the directory
    // something a bit more useful so we can reuse it after we're done with this
    // test app, lol :)

    // do usual simple setup stuff
    Logger::getSingleton().setLoggingLevel(Informative);

    // Load the TaharezLookSkin (which uses Falagard skinning system)
    SchemeManager::getSingleton().loadScheme("../datafiles/schemes/TaharezLookSkin.scheme");

    System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
    FontManager::getSingleton().createFont("../datafiles/fonts/Commonwealth-10.font");
    WindowManager& winMgr = WindowManager::getSingleton();

    // load layout and set as root
    System::getSingleton().setGUISheet(winMgr.loadWindowLayout("../datafiles/layouts/Demo7Windows.layout"));

    // success!
    return true;
}


/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void FalagardDemo1Sample::cleanupSample()
{
    // nothing to do here!
}
