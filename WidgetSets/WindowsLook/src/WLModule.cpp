/************************************************************************
	filename: 	WLModule.cpp
	created:	8/8/2004
	author:		Paul D Turner
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
#include "WLModule.h"
#include "CEGUIExceptions.h"
#include "CEGUIWindowFactoryManager.h"
#include "WLStatic.h"
#include "WLButton.h"
#include "WLTitlebar.h"
#include "WLCloseButton.h"
#include "WLFrameWindow.h"
#include "WLEditbox.h"
#include "WLCheckbox.h"
#include "WLRadioButton.h"
#include "WLProgressBar.h"
#include "WLSliderThumb.h"
#include "WLSlider.h"
#include "WLVertScrollbarThumb.h"
#include "WLVertScrollbar.h"
#include "WLHorzScrollbarThumb.h"
#include "WLHorzScrollbar.h"
#include "WLListbox.h"
#include "WLComboDropList.h"
#include "WLCombobox.h"
#include "WLMultiLineEditbox.h"
#include "WLListHeaderSegment.h"
#include "WLListHeader.h"
#include "WLMultiColumnList.h"
#include "WLTabControl.h"
#include "WLTabButton.h"
#include "WLTabPane.h"
#include "WLSpinner.h"
#include "WLScrollablePane.h"
#include "WLTooltip.h"
#include "WLTextItem.h"
#include "WLMenuItem.h"
#include "WLMenubar.h"
#include "WLPopupMenu.h"

/*************************************************************************
	Static factory objects
*************************************************************************/
static CEGUI::WLButtonFactory				s_ButtonFactory;
static CEGUI::WLStaticTextFactory			s_StaticTextFactory;
static CEGUI::WLStaticImageFactory			s_StaticImageFactory;
static CEGUI::WLTitlebarFactory				s_TitlebarFactory;
static CEGUI::WLCloseButtonFactory			s_CloseButtonFactory;
static CEGUI::WLFrameWindowFactory			s_FrameWindowFactory;
static CEGUI::WLEditboxFactory				s_EditboxFactory;
static CEGUI::WLCheckboxFactory				s_CheckboxFactory;
static CEGUI::WLRadioButtonFactory			s_RadioButtonFactory;
static CEGUI::WLProgressBarFactory			s_ProgressBarFactory;
static CEGUI::WLSliderThumbFactory			s_SliderThumbFactory;
static CEGUI::WLSliderFactory				s_SliderFactory;
static CEGUI::WLVertScrollbarThumbFactory	s_VertScrollbarThumbFactory;
static CEGUI::WLVertScrollbarFactory		s_VertScrollbarFactory;
static CEGUI::WLHorzScrollbarThumbFactory	s_HorzScrollbarThumbFactory;
static CEGUI::WLHorzScrollbarFactory		s_HorzScrollbarFactory;
static CEGUI::WLListboxFactory				s_ListboxFactory;
static CEGUI::WLComboDropListFactory		s_ComboDropListFactory;
static CEGUI::WLComboboxFactory				s_ComboboxFactory;
static CEGUI::WLMultiLineEditboxFactory		s_MultiLineEditboxFactory;
static CEGUI::WLListHeaderSegmentFactory	s_ListHeaderSegmentFactory;
static CEGUI::WLListHeaderFactory			s_ListHeaderFactory;
static CEGUI::WLMultiColumnListFactory		s_MultiColumnListFactory;
static CEGUI::WLTabControlFactory		    s_TabControlFactory;
static CEGUI::WLTabButtonFactory		    s_TabButtonFactory;
static CEGUI::WLTabPaneFactory				s_TabPaneFactory;
static CEGUI::WLSpinnerFactory              s_SpinnerFactory;
static CEGUI::WLScrollablePaneFactory       s_ScrollablePaneFactory;
static CEGUI::WLTooltipFactory              s_TooltipFactory;
static CEGUI::WLTextItemFactory				s_TextItemFactory;
static CEGUI::WLMenubarItemFactory			s_MenubarItemFactory;
static CEGUI::WLPopupMenuItemFactory		s_PopupMenuItemFactory;
static CEGUI::WLMenubarFactory				s_MenubarFactory;
static CEGUI::WLPopupMenuFactory			s_PopupMenuFactory;

using namespace CEGUI;

struct mapEntry
{
    const CEGUI::utf8* d_name;
    CEGUI::WindowFactory* d_factory;
};

mapEntry factoriesMap[] =
{
    {WLButton::WidgetTypeName, &s_ButtonFactory},
    {WLCheckbox::WidgetTypeName, &s_CheckboxFactory},
    {WLCloseButton::WidgetTypeName, &s_CloseButtonFactory},
    {WLCombobox::WidgetTypeName, &s_ComboboxFactory},
    {WLComboDropList::WidgetTypeName, &s_ComboDropListFactory},
    {WLEditbox::WidgetTypeName, &s_EditboxFactory},
    {WLFrameWindow::WidgetTypeName, &s_FrameWindowFactory},
    {WLListbox::WidgetTypeName, &s_ListboxFactory},
    {WLListHeader::WidgetTypeName, &s_ListHeaderFactory},
    {WLListHeaderSegment::WidgetTypeName, &s_ListHeaderSegmentFactory},
    {WLHorzScrollbar::WidgetTypeName, &s_HorzScrollbarFactory},
    {WLHorzScrollbarThumb::WidgetTypeName, &s_HorzScrollbarThumbFactory},
    {WLMultiColumnList::WidgetTypeName, &s_MultiColumnListFactory},
    {WLMultiLineEditbox::WidgetTypeName, &s_MultiLineEditboxFactory},
    {WLProgressBar::WidgetTypeName, &s_ProgressBarFactory},
    {WLRadioButton::WidgetTypeName, &s_RadioButtonFactory},
    {WLScrollablePane::WidgetTypeName, &s_ScrollablePaneFactory},
    {WLSlider::WidgetTypeName, &s_SliderFactory},
    {WLSliderThumb::WidgetTypeName, &s_SliderThumbFactory},
    {WLSpinner::WidgetTypeName, &s_SpinnerFactory},
    {WLStaticImage::WidgetTypeName, &s_StaticImageFactory},
    {WLStaticText::WidgetTypeName, &s_StaticTextFactory},
    {WLTabButton::WidgetTypeName, &s_TabButtonFactory},
    {WLTabControl::WidgetTypeName, &s_TabControlFactory},
    {WLTabPane::WidgetTypeName, &s_TabPaneFactory},
    {WLTitlebar::WidgetTypeName, &s_TitlebarFactory},
    {WLTooltip::WidgetTypeName, &s_TooltipFactory},
    {WLVertScrollbar::WidgetTypeName, &s_VertScrollbarFactory},
    {WLVertScrollbarThumb::WidgetTypeName, &s_VertScrollbarThumbFactory},
    {WLTextItem::WidgetTypeName, &s_TextItemFactory},
    {WLMenubarItem::WidgetTypeName, &s_MenubarItemFactory},
    {WLPopupMenuItem::WidgetTypeName, &s_PopupMenuItemFactory},
    {WLMenubar::WidgetTypeName, &s_MenubarFactory},
    {WLPopupMenu::WidgetTypeName, &s_PopupMenuFactory},
    {0,0}
};

/*************************************************************************
	Plugin access interface
*************************************************************************/
extern "C" void registerFactory(const CEGUI::String& type_name)
{
    mapEntry* entry = factoriesMap;

    while (entry->d_name)
    {
        if (entry->d_name == type_name)
        {
            WindowFactoryManager::getSingleton().addFactory(entry->d_factory);
            return;
        }

        ++entry;
    }

    throw UnknownObjectException((utf8*)"::registerFactory - The window factory for type '" + type_name + "' is not known in this module.");
}

extern "C" CEGUI::uint registerAllFactories(void)
{
    CEGUI::uint count = 0;
    mapEntry* entry = factoriesMap;

    while (entry->d_name)
    {
        WindowFactoryManager::getSingleton().addFactory(entry->d_factory);

        ++entry;
        ++count;
    }

    return count;
}
