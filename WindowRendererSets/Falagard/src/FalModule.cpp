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

// Define factories to create Falagard window types
CEGUI_DEFINE_WR_FACTORY( FalagardButton )
CEGUI_DEFINE_WR_FACTORY( FalagardDefault )
CEGUI_DEFINE_WR_FACTORY( FalagardEditbox )
CEGUI_DEFINE_WR_FACTORY( FalagardFrameWindow )
CEGUI_DEFINE_WR_FACTORY( FalagardItemEntry )
CEGUI_DEFINE_WR_FACTORY( FalagardListHeader )
CEGUI_DEFINE_WR_FACTORY( FalagardListHeaderSegment )
CEGUI_DEFINE_WR_FACTORY( FalagardListbox )
CEGUI_DEFINE_WR_FACTORY( FalagardMenubar )
CEGUI_DEFINE_WR_FACTORY( FalagardMenuItem )
CEGUI_DEFINE_WR_FACTORY( FalagardMultiColumnList )
CEGUI_DEFINE_WR_FACTORY( FalagardMultiLineEditbox )
CEGUI_DEFINE_WR_FACTORY( FalagardPopupMenu )
CEGUI_DEFINE_WR_FACTORY( FalagardProgressBar )
CEGUI_DEFINE_WR_FACTORY( FalagardScrollablePane )
CEGUI_DEFINE_WR_FACTORY( FalagardScrollbar )
CEGUI_DEFINE_WR_FACTORY( FalagardSlider )
CEGUI_DEFINE_WR_FACTORY( FalagardStatic )
CEGUI_DEFINE_WR_FACTORY( FalagardStaticImage )
CEGUI_DEFINE_WR_FACTORY( FalagardStaticText )
CEGUI_DEFINE_WR_FACTORY( FalagardSystemButton )
CEGUI_DEFINE_WR_FACTORY( FalagardTabButton )
CEGUI_DEFINE_WR_FACTORY( FalagardTabControl )
CEGUI_DEFINE_WR_FACTORY( FalagardTitlebar )
CEGUI_DEFINE_WR_FACTORY( FalagardToggleButton )
CEGUI_DEFINE_WR_FACTORY( FalagardTooltip )
CEGUI_DEFINE_WR_FACTORY( FalagardItemListbox )
CEGUI_DEFINE_WR_FACTORY( FalagardTree )

// Define the factory map so types can be registered
CEGUI_START_WR_FACTORY_MAP( Falagard )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardButton )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardDefault )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardEditbox )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardFrameWindow )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardItemEntry )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardListHeader )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardListHeaderSegment )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardListbox )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardMenubar )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardMenuItem )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardMultiColumnList )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardMultiLineEditbox )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardPopupMenu )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardProgressBar )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardScrollablePane )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardScrollbar )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardSlider )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardStatic )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardStaticImage )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardStaticText )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardSystemButton )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardTabButton )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardTabControl )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardTitlebar )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardToggleButton )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardTooltip )
    CEGUI_WR_FACTORY_MAP_ENTRY( FalagardItemListbox )
	CEGUI_WR_FACTORY_MAP_ENTRY( FalagardTree )
CEGUI_END_WR_FACTORY_MAP

// define the widget module for Falagard
CEGUI_DEFINE_WR_MODULE( Falagard )
