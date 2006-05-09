/************************************************************************
    filename:   FalModule.cpp
    created:    Wed Jun 22 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
CEGUI_END_WR_FACTORY_MAP

// define the widget module for Falagard
CEGUI_DEFINE_WR_MODULE( Falagard )
