/************************************************************************
	filename: 	WLModule.cpp
	created:	8/8/2004
	author:		Paul D Turner
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
    Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

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


/*************************************************************************
	Plugin access interface
*************************************************************************/
extern "C" void registerFactory(const CEGUI::String& type_name)
{
	using namespace CEGUI;

	if (type_name == WLStaticText::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_StaticTextFactory);
		return;
	}
	else if (type_name == WLStaticImage::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_StaticImageFactory);
		return;
	}
	else if (type_name == WLButton::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_ButtonFactory);
		return;
	}
	else if (type_name == WLTitlebar::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_TitlebarFactory);
		return;
	}
	else if (type_name == WLCloseButton::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_CloseButtonFactory);
		return;
	}
	else if (type_name == WLFrameWindow::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_FrameWindowFactory);
		return;
	}
	else if (type_name == WLEditbox::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_EditboxFactory);
		return;
	}
	else if (type_name == WLCheckbox::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_CheckboxFactory);
		return;
	}
	else if (type_name == WLRadioButton::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_RadioButtonFactory);
		return;
	}
	else if (type_name == WLProgressBar::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_ProgressBarFactory);
		return;
	}
	else if (type_name == WLSliderThumb::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_SliderThumbFactory);
		return;
	}
	else if (type_name == WLSlider::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_SliderFactory);
		return;
	}
	else if (type_name == WLVertScrollbarThumb::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_VertScrollbarThumbFactory);
		return;
	}
	else if (type_name == WLVertScrollbar::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_VertScrollbarFactory);
		return;
	}
	else if (type_name == WLHorzScrollbarThumb::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_HorzScrollbarThumbFactory);
		return;
	}
	else if (type_name == WLHorzScrollbar::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_HorzScrollbarFactory);
		return;
	}
	else if (type_name == WLListbox::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_ListboxFactory);
		return;
	}
	else if (type_name == WLComboDropList::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_ComboDropListFactory);
		return;
	}
	else if (type_name == WLCombobox::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_ComboboxFactory);
		return;
	}
	else if (type_name == WLMultiLineEditbox::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_MultiLineEditboxFactory);
		return;
	}
	else if (type_name == WLListHeaderSegment::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_ListHeaderSegmentFactory);
		return;
	}
	else if (type_name == WLListHeader::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_ListHeaderFactory);
		return;
	}
	else if (type_name == WLMultiColumnList::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_MultiColumnListFactory);
		return;
	}
	else if (type_name == WLTabControl::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_TabControlFactory);
		return;
	}
	else if (type_name == WLTabButton::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_TabButtonFactory);
		return;
	}
	else if (type_name == WLTabPane::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_TabPaneFactory);
		return;
	}

	throw UnknownObjectException((utf8*)"::registerFactory - The window factory for type '" + type_name + "' is not known in this module.");

	return;
}
