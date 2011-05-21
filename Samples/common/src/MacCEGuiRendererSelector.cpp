/***********************************************************************
    filename:   MacCEGuiRendererSelector.cpp
    created:    Sat May 21 2011
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#include <Carbon/Carbon.h>

//----------------------------------------------------------------------------//
// Implementation of the Carbon based dialog
class RendererSelectorDialog
{
    WindowRef d_dialog;
    ControlRef d_cancel;
    ControlRef d_okay;
    ControlRef d_rendererList;
    MenuRef d_rendererListMenu;
    bool d_cancelled;

    //------------------------------------------------------------------------//
    OSStatus handleCommandID(UInt32 commandID)
    {
        switch (commandID)
        {
        case 1:
            d_cancelled = true;
            QuitAppModalLoopForWindow(d_dialog);
            break;

        case 2:
            QuitAppModalLoopForWindow(d_dialog);
            break;
        }
        return noErr;
    }
    
    //------------------------------------------------------------------------//
    static OSStatus eventDispatcher(EventHandlerCallRef, EventRef event, void* ob)
    {
        HICommand command;
        GetEventParameter(event, kEventParamDirectObject, typeHICommand, 
                          0, sizeof(command), 0, &command); 
        
        return reinterpret_cast<RendererSelectorDialog*>(ob)->
            handleCommandID(command.commandID);
    }

public:
    //------------------------------------------------------------------------//
    RendererSelectorDialog() :
        d_cancelled(false)
    {
        Rect dialogBounds = {100, 100, 200, 391};
        CreateNewWindow(kMovableModalWindowClass,
                        kWindowStandardHandlerAttribute|kWindowCompositingAttribute,
                        &dialogBounds, &d_dialog);

        SetWindowTitleWithCFString(d_dialog, CFSTR("CEGUI Sample Application"));

        Rect cancelBounds = {69, 14, 89, 131};
        CFStringRef cancelTitle = CFSTR("Cancel");
        CreatePushButtonControl(d_dialog, &cancelBounds, cancelTitle, &d_cancel);
        SetControlCommandID(d_cancel, 1);
        
        Rect okayBounds = {69, 160, 89, 277};
        CFStringRef okayTitle = CFSTR("OK");
        CreatePushButtonControl(d_dialog, &okayBounds, okayTitle, &d_okay);
        SetControlCommandID(d_okay, 2);
        
        CreateNewMenu(0, 0, &d_rendererListMenu);

        Rect rendererListBounds = {18, 17, 38, 274};
        CFStringRef rendererListTitle = CFSTR("Renderer:");

        CreatePopupButtonControl(d_dialog, &rendererListBounds, rendererListTitle,
                                 -12345, false, 64, teFlushRight, 0, &d_rendererList);

        SetControlPopupMenuHandle(d_rendererList, d_rendererListMenu);

        SetWindowCancelButton(d_dialog, d_cancel);
        SetWindowDefaultButton(d_dialog, d_okay);
    
        EventTypeSpec cmdEvt;	
        cmdEvt.eventClass = kEventClassCommand;
        cmdEvt.eventKind = kEventCommandProcess;
        
        InstallEventHandler(GetWindowEventTarget(d_dialog),
                            NewEventHandlerUPP(RendererSelectorDialog::eventDispatcher),
                            1, &cmdEvt, this, 0); 
    }
    
    //------------------------------------------------------------------------//
    ~RendererSelectorDialog()
    {
        DisposeWindow(d_dialog);
    }

    //------------------------------------------------------------------------//
    void addRendererNameToMenu(CFStringRef rendererName)
    {
        AppendMenuItemTextWithCFString(d_rendererListMenu, rendererName, 0, 0, 0);

        UInt16 count = CountMenuItems(d_rendererListMenu);
        SetControlMaximum(d_rendererList, count);
        
        if (count == 1)
            SetControlValue(d_rendererList, 1);
    }

    //------------------------------------------------------------------------//
    // show dialog modally, returning selected item index or -1 if cancelled
    int run()
    {
        ShowWindow(d_dialog);
        SetUserFocusWindow(d_dialog);
        ActivateControl(d_okay);
        RunAppModalLoopForWindow(d_dialog);
        HideWindow(d_dialog);

        return d_cancelled ? -1 : static_cast<int>(GetControlValue(d_rendererList)) - 1;
    }

    //------------------------------------------------------------------------//
};

//----------------------------------------------------------------------------//
bool MacCEGuiRendererSelector::invokeDialog()
{
    RendererSelectorDialog dialog;
    int rendererCount = populateRendererMenu(dialog);

    // 'cancel' if there are no renderers available
    if (rendererCount == 0)
        return false;

    // only show the dialog if there is a choice ;)
    int idx = (rendererCount > 1) ? dialog.run() : 0;

    // bail out if user cancelled dialog
    if (idx == -1)
        return false;

    // set the last selected renderer - i.e. the one we want to use.
    d_lastSelected = d_rendererTypes[idx];

    return true;
}

//----------------------------------------------------------------------------//
int MacCEGuiRendererSelector::populateRendererMenu(RendererSelectorDialog& dialog)
{
    int idx = 0;
    // Put items in the combobox for enabled renderers.
    if (d_rendererAvailability[OpenGLGuiRendererType])
    {
        d_rendererTypes[idx++] = OpenGLGuiRendererType;
        dialog.addRendererNameToMenu(CFSTR("OpenGL Renderer"));
    }
    if (d_rendererAvailability[IrrlichtGuiRendererType])
    {
        d_rendererTypes[idx++] = IrrlichtGuiRendererType;
        dialog.addRendererNameToMenu(CFSTR("Irrlicht Renderer"));
    }
    if (d_rendererAvailability[OgreGuiRendererType])
    {
        d_rendererTypes[idx++] = OgreGuiRendererType;
        dialog.addRendererNameToMenu(CFSTR("Ogre Renderer"));
    }
    if (d_rendererAvailability[DirectFBGuiRendererType])
    {
        d_rendererTypes[idx++] = DirectFBGuiRendererType;
        dialog.addRendererNameToMenu(CFSTR("DirectFB Renderer"));
    }

    return idx;
}

//----------------------------------------------------------------------------//
