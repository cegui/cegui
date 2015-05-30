/***********************************************************************
created:    18/4/2015
author:     Lukas E Meindl
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2015 Paul D Turner & The CEGUI Development Team
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
#ifndef _LookNFeelOverview_Demo_
#define _LookNFeelOverview_Demo_

#include "SampleBase.h"
#include "CEGUI/ForwardRefs.h"

// Sample class
class LookNFeelOverviewDemo : public Sample
{
public:
    // method to initialse the samples windows and events.
    virtual bool initialise(CEGUI::GUIContext* guiContext);

    // method to perform any required cleanup operations.
    virtual void deinitialise();

protected:
    bool handleSkinSelectionAccepted(const CEGUI::EventArgs& args);

    // We store some items for comparison
    CEGUI::ListboxTextItem* d_taharezLookListboxItem;
    CEGUI::ListboxTextItem* d_vanillaLookListboxItem;

    // We want to change the default font later so we store some fonts in variables
    CEGUI::Font* d_fontForTaharez;
    CEGUI::Window* d_taharezOverviewLayout;
    // We want to change the default font later so we store some fonts in variables
    CEGUI::Window* d_vanillaOverviewLayout;

    CEGUI::Window* d_loadedLayoutContainer;
    CEGUI::GUIContext* d_guiContext;
};

#endif
