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
#include "CEGUI/WindowRendererSets/Falagard/Module.h"
#include "CEGUI/TplWRFactoryRegisterer.h"

// includes for types we create
#include "CEGUI/WindowRendererSets/Falagard/Button.h"
#include "CEGUI/WindowRendererSets/Falagard/Default.h"
#include "CEGUI/WindowRendererSets/Falagard/Editbox.h"
#include "CEGUI/WindowRendererSets/Falagard/FrameWindow.h"
#include "CEGUI/WindowRendererSets/Falagard/ItemEntry.h"
#include "CEGUI/WindowRendererSets/Falagard/ListHeader.h"
#include "CEGUI/WindowRendererSets/Falagard/ListHeaderSegment.h"
#include "CEGUI/WindowRendererSets/Falagard/Listbox.h"
#include "CEGUI/WindowRendererSets/Falagard/Menubar.h"
#include "CEGUI/WindowRendererSets/Falagard/MenuItem.h"
#include "CEGUI/WindowRendererSets/Falagard/MultiColumnList.h"
#include "CEGUI/WindowRendererSets/Falagard/MultiLineEditbox.h"
#include "CEGUI/WindowRendererSets/Falagard/PopupMenu.h"
#include "CEGUI/WindowRendererSets/Falagard/ProgressBar.h"
#include "CEGUI/WindowRendererSets/Falagard/ScrollablePane.h"
#include "CEGUI/WindowRendererSets/Falagard/Scrollbar.h"
#include "CEGUI/WindowRendererSets/Falagard/Slider.h"
#include "CEGUI/WindowRendererSets/Falagard/Static.h"
#include "CEGUI/WindowRendererSets/Falagard/StaticImage.h"
#include "CEGUI/WindowRendererSets/Falagard/StaticText.h"
#include "CEGUI/WindowRendererSets/Falagard/SystemButton.h"
#include "CEGUI/WindowRendererSets/Falagard/TabButton.h"
#include "CEGUI/WindowRendererSets/Falagard/TabControl.h"
#include "CEGUI/WindowRendererSets/Falagard/Titlebar.h"
#include "CEGUI/WindowRendererSets/Falagard/ToggleButton.h"
#include "CEGUI/WindowRendererSets/Falagard/Tooltip.h"
#include "CEGUI/WindowRendererSets/Falagard/ItemListbox.h"
#include "CEGUI/WindowRendererSets/Falagard/Tree.h"

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
