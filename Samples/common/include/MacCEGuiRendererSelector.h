/***********************************************************************
    filename:   MacCEGuiRendererSelector.h
    created:    Tue Mar 17 2009
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#ifndef _MacCEGuiRendererSelector_h_
#define _MacCEGuiRendererSelector_h_

#include "CEGuiRendererSelector.h"
#include <string>

#ifdef __APPLE__
#   include <Carbon/Carbon.h>
#endif

//! RendererSelector for Apple Mac via Carbon
class MacCEGuiRendererSelector : public CEGuiRendererSelector
{
public:
    MacCEGuiRendererSelector();
    ~MacCEGuiRendererSelector();

    // Implement CEGuiRendererSelector interface.
    bool invokeDialog();

private:
    //! Load the dialog window from the nib file.
    void loadDialogWindow();
    //! Add entries for available renderers. returns the number of renderers.
    int populateRendererMenu();

    OSStatus commandHandler(UInt32 command);

    //! function that will dispatch events back into the object for handling.
    static OSStatus eventDispatcher(EventHandlerCallRef, EventRef, void*);

    //! array of CEGuiRendererTypes that map to entries in the pop up.
    CEGuiRendererType d_rendererTypes[4];
    //! the main dialog window.
    WindowRef d_dialog;
    //! The popup button holding the renderer choices.
	HIViewRef d_rendererPopup;
    //! true if user cancelled the dialog
    bool d_cancelled;
};

#endif  // end of guard _MacCEGuiRendererSelector_h_
