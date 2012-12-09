/***********************************************************************
    filename:   FalModule.cpp
    created:    Wed Jun 22 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
#include "FalModule.h"
#include "CEGUITplWRFactoryRegisterer.h"

// includes for types we create
#include "FalButton.h"
#include "FalDefault.h"
#include "FalEditbox.h"
#include "FalFrameWindow.h"
#include "FalItemEntry.h"
#include "FalListHeader.h"
#include "FalListHeaderSegment.h"
#include "FalListbox.h"
#include "FalMenubar.h"
#include "FalMenuItem.h"
#include "FalMultiColumnList.h"
#include "FalMultiLineEditbox.h"
#include "FalPopupMenu.h"
#include "FalProgressBar.h"
#include "FalScrollablePane.h"
#include "FalScrollbar.h"
#include "FalSlider.h"
#include "FalStatic.h"
#include "FalStaticImage.h"
#include "FalStaticText.h"
#include "FalSystemButton.h"
#include "FalTabButton.h"
#include "FalTabControl.h"
#include "FalTitlebar.h"
#include "FalToggleButton.h"
#include "FalTooltip.h"
#include "FalItemListbox.h"
#include "FalTree.h"

//----------------------------------------------------------------------------//
extern "C"
CEGUI::WindowRendererModule& getWindowRendererModule()
{
    static CEGUI::FalagardWRModule mod;
    return mod;
}

namespace CEGUI
{
//----------------------------------------------------------------------------//
FalagardWRModule::FalagardWRModule()
{
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardButton>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardDefault>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardEditbox>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardFrameWindow>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardItemEntry>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardListHeader>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardListHeaderSegment>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardListbox>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardMenubar>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardMenuItem>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardMultiColumnList>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardMultiLineEditbox>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardPopupMenu>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardProgressBar>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardScrollablePane>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardScrollbar>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardSlider>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardStatic>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardStaticImage>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardStaticText>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardSystemButton>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardTabButton>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardTabControl>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardTitlebar>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardToggleButton>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardTooltip>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardItemListbox>);
    d_registry.push_back(new TplWRFactoryRegisterer<FalagardTree>);
}

//----------------------------------------------------------------------------//
FalagardWRModule::~FalagardWRModule()
{
    FactoryRegistry::iterator i = d_registry.begin();
    for ( ; i != d_registry.end(); ++i)
        delete (*i);
}

//----------------------------------------------------------------------------//

}
