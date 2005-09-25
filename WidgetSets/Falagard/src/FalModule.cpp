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
#include "FalCheckbox.h"
#include "FalComboDropList.h"
#include "FalCombobox.h"
#include "FalEditbox.h"
#include "FalFrameWindow.h"
#include "FalListHeader.h"
#include "FalListHeaderSegment.h"
#include "FalListbox.h"
#include "FalMenuBar.h"
#include "FalMenuItem.h"
#include "FalMultiColumnList.h"
#include "FalMultiLineEditbox.h"
#include "FalPopupMenu.h"
#include "FalProgressBar.h"
#include "FalRadioButton.h"
#include "FalScrollablePane.h"
#include "FalScrollbar.h"
#include "FalSlider.h"
#include "FalSpinner.h"
#include "FalStatic.h"
#include "FalStaticImage.h"
#include "FalStaticText.h"
#include "FalSystemButton.h"
#include "FalTabButton.h"
#include "FalTabControl.h"
#include "FalTabPane.h"
#include "FalThumb.h"
#include "FalTitlebar.h"
#include "FalTooltip.h"

// Define factories to create Falagard window types
CEGUI_DEFINE_FACTORY( FalagardButton )
CEGUI_DEFINE_FACTORY( FalagardCheckbox )
CEGUI_DEFINE_FACTORY( FalagardComboDropList )
CEGUI_DEFINE_FACTORY( FalagardCombobox )
CEGUI_DEFINE_FACTORY( FalagardEditbox )
CEGUI_DEFINE_FACTORY( FalagardFrameWindow )
CEGUI_DEFINE_FACTORY( FalagardListHeader )
CEGUI_DEFINE_FACTORY( FalagardListHeaderSegment )
CEGUI_DEFINE_FACTORY( FalagardListbox )
CEGUI_DEFINE_FACTORY( FalagardMenubar )
CEGUI_DEFINE_FACTORY( FalagardMenuItem )
CEGUI_DEFINE_FACTORY( FalagardMultiColumnList )
CEGUI_DEFINE_FACTORY( FalagardMultiLineEditbox )
CEGUI_DEFINE_FACTORY( FalagardPopupMenu )
CEGUI_DEFINE_FACTORY( FalagardProgressBar )
CEGUI_DEFINE_FACTORY( FalagardRadioButton )
CEGUI_DEFINE_FACTORY( FalagardScrollablePane )
CEGUI_DEFINE_FACTORY( FalagardScrollbar )
CEGUI_DEFINE_FACTORY( FalagardSlider )
CEGUI_DEFINE_FACTORY( FalagardSpinner )
CEGUI_DEFINE_FACTORY( FalagardStatic )
CEGUI_DEFINE_FACTORY( FalagardStaticImage )
CEGUI_DEFINE_FACTORY( FalagardStaticText )
CEGUI_DEFINE_FACTORY( FalagardSystemButton )
CEGUI_DEFINE_FACTORY( FalagardTabButton )
CEGUI_DEFINE_FACTORY( FalagardTabControl )
CEGUI_DEFINE_FACTORY( FalagardTabPane )
CEGUI_DEFINE_FACTORY( FalagardThumb )
CEGUI_DEFINE_FACTORY( FalagardTitlebar )
CEGUI_DEFINE_FACTORY( FalagardTooltip )

// Define the factory map so types can be registered
CEGUI_START_FACTORY_MAP( Falagard )
    CEGUI_FACTORY_MAP_ENTRY( FalagardButton )
    CEGUI_FACTORY_MAP_ENTRY( FalagardCheckbox )
    CEGUI_FACTORY_MAP_ENTRY( FalagardComboDropList )
    CEGUI_FACTORY_MAP_ENTRY( FalagardCombobox )
    CEGUI_FACTORY_MAP_ENTRY( FalagardEditbox )
    CEGUI_FACTORY_MAP_ENTRY( FalagardFrameWindow )
    CEGUI_FACTORY_MAP_ENTRY( FalagardListHeader )
    CEGUI_FACTORY_MAP_ENTRY( FalagardListHeaderSegment )
    CEGUI_FACTORY_MAP_ENTRY( FalagardListbox )
    CEGUI_FACTORY_MAP_ENTRY( FalagardMenubar )
    CEGUI_FACTORY_MAP_ENTRY( FalagardMenuItem )
    CEGUI_FACTORY_MAP_ENTRY( FalagardMultiColumnList )
    CEGUI_FACTORY_MAP_ENTRY( FalagardMultiLineEditbox )
    CEGUI_FACTORY_MAP_ENTRY( FalagardPopupMenu )
    CEGUI_FACTORY_MAP_ENTRY( FalagardProgressBar )
    CEGUI_FACTORY_MAP_ENTRY( FalagardRadioButton )
    CEGUI_FACTORY_MAP_ENTRY( FalagardScrollablePane )
    CEGUI_FACTORY_MAP_ENTRY( FalagardScrollbar )
    CEGUI_FACTORY_MAP_ENTRY( FalagardSlider )
    CEGUI_FACTORY_MAP_ENTRY( FalagardSpinner )
    CEGUI_FACTORY_MAP_ENTRY( FalagardStatic )
    CEGUI_FACTORY_MAP_ENTRY( FalagardStaticImage )
    CEGUI_FACTORY_MAP_ENTRY( FalagardStaticText )
    CEGUI_FACTORY_MAP_ENTRY( FalagardSystemButton )
    CEGUI_FACTORY_MAP_ENTRY( FalagardTabButton )
    CEGUI_FACTORY_MAP_ENTRY( FalagardTabControl )
    CEGUI_FACTORY_MAP_ENTRY( FalagardTabPane )
    CEGUI_FACTORY_MAP_ENTRY( FalagardThumb )
    CEGUI_FACTORY_MAP_ENTRY( FalagardTitlebar )
    CEGUI_FACTORY_MAP_ENTRY( FalagardTooltip )
CEGUI_END_FACTORY_MAP

// define the widget module for Falagard
CEGUI_DEFINE_WIDGET_MODULE( Falagard )
