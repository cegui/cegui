/***********************************************************************
    filename:   MacCEGuiRendererSelector.cpp
    created:    Tue Mar 17 2009
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
#include "MacCEGuiRendererSelector.h"

//----------------------------------------------------------------------------//
MacCEGuiRendererSelector::MacCEGuiRendererSelector() :
    d_dialog(0),
    d_rendererPopup(0),
    d_cancelled(false)
{
}

//----------------------------------------------------------------------------//
MacCEGuiRendererSelector::~MacCEGuiRendererSelector()
{
}

//----------------------------------------------------------------------------//
bool MacCEGuiRendererSelector::invokeDialog()
{
    loadDialogWindow();
    int rendererCount = populateRendererMenu();

    // 'cancel' if there are no renderers available
    if (rendererCount == 0)
        d_cancelled = true;
    // only bother with the dialog if there is a choice ;)
    else if (rendererCount > 1)
    {
        // set the event handling
        EventTypeSpec cmdEvt;	
        cmdEvt.eventClass = kEventClassCommand;
        cmdEvt.eventKind = kEventCommandProcess;
        
        InstallEventHandler(
            GetWindowEventTarget(d_dialog),
            NewEventHandlerUPP(MacCEGuiRendererSelector::eventDispatcher), 1,
            &cmdEvt, this, 0); 

        ShowWindow(d_dialog);
        RunApplicationEventLoop();
    }

    SInt32 idx = HIViewGetValue(d_rendererPopup);
    DisposeWindow(d_dialog);

    // bail out if user cancelled dialog or if selected index is 0
    if (d_cancelled || (idx == 0))
        return false;

    // set the last selected renderer - i.e. the one we want to use.
    d_lastSelected = d_rendererTypes[idx - 1];

    return true;
}

//----------------------------------------------------------------------------//
OSStatus MacCEGuiRendererSelector::commandHandler(UInt32 command)
{
	OSStatus status = noErr;

    switch (command)
    {
    case 'ok  ':
        QuitApplicationEventLoop();
        break;

    case 'not!':
        d_cancelled = true;
        QuitApplicationEventLoop();
        break;

    default:
        status = eventNotHandledErr;
        break;
    }

    return status;
}

//----------------------------------------------------------------------------//
void MacCEGuiRendererSelector::loadDialogWindow()
{
    // get our framework bundle from the app
    CFBundleRef helperFwk =
        CFBundleGetBundleWithIdentifier(
            CFSTR("uk.org.cegui.CEGUISampleHelper"));

    if (helperFwk)
    {
        IBNibRef nib;
        if (!CreateNibReferenceWithCFBundle(helperFwk,
                                            CFSTR("RendererSelector"), &nib))
        {
            CreateWindowFromNib(nib, CFSTR("RendererSelector"), &d_dialog);
            DisposeNibReference (nib);

            // find popup button in the window
            const HIViewID rendererPopupID = {'PBTN', 0};
            HIViewFindByID(HIViewGetRoot(d_dialog),
                           rendererPopupID,
                           &d_rendererPopup);
        }
    }    
}

//----------------------------------------------------------------------------//
int MacCEGuiRendererSelector::populateRendererMenu()
{
	// get the menu from the popup
	MenuRef menu;
	GetControlData(d_rendererPopup, 0, kControlPopupButtonMenuRefTag,
                   sizeof(menu), &menu, 0);

    int idx = 0;
    // Put items in the combobox for enabled renderers.
    if (d_rendererAvailability[OpenGLGuiRendererType])
    {
        d_rendererTypes[idx++] = OpenGLGuiRendererType;
        AppendMenuItemTextWithCFString(menu, CFSTR("OpenGL Renderer"),
                                       0, 0, 0);
    }
    if (d_rendererAvailability[IrrlichtGuiRendererType])
    {
        d_rendererTypes[idx++] = IrrlichtGuiRendererType;
        AppendMenuItemTextWithCFString(menu, CFSTR("Irrlicht Renderer"),
                                       0, 0, 0);
    }
    if (d_rendererAvailability[OgreGuiRendererType])
    {
        d_rendererTypes[idx++] = OgreGuiRendererType;
        AppendMenuItemTextWithCFString(menu, CFSTR("Ogre Renderer"),
                                       0, 0, 0);
    }
    if (d_rendererAvailability[DirectFBGuiRendererType])
    {
        d_rendererTypes[idx++] = DirectFBGuiRendererType;
        AppendMenuItemTextWithCFString(menu, CFSTR("DirectFB Renderer"),
                                       0, 0, 0);
    }

	HIViewSetMaximum(d_rendererPopup, CountMenuItems(menu));
    HIViewSetValue(d_rendererPopup, 1);
    return idx;
}

//----------------------------------------------------------------------------//
OSStatus MacCEGuiRendererSelector::eventDispatcher(EventHandlerCallRef,
                                                   EventRef event,
                                                   void* ob)
{
	// get the command
	HICommand command;
	GetEventParameter(event, kEventParamDirectObject, typeHICommand, 
				      0, sizeof(command), 0, &command); 

    return reinterpret_cast<MacCEGuiRendererSelector*>(ob)->
        commandHandler(command.commandID);
}

//----------------------------------------------------------------------------//
