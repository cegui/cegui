/************************************************************************
	filename: 	TLModule.cpp
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Implements the System <-> GUI Module interface.
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
#include "TLModule.h"
#include "CEGUIExceptions.h"
#include "CEGUIWindowFactoryManager.h"
#include "TLFrameWindow.h"
#include "TLTitlebar.h"
#include "TLButton.h"
#include "TLCloseButton.h"
#include "TLCheckbox.h"
#include "TLRadioButton.h"
#include "TLSliderThumb.h"
#include "TLSlider.h"
#include "TLProgressBar.h"
#include "TLAlternateProgressBar.h"
#include "TLEditbox.h"
#include "TLVertScrollbarThumb.h"
#include "TLVertScrollbar.h"
#include "TLMiniVertScrollbarThumb.h"
#include "TLMiniVertScrollbar.h"
#include "TLMiniHorzScrollbarThumb.h"
#include "TLMiniHorzScrollbar.h"
#include "TLStatic.h"
#include "TLListbox.h"
#include "TLCombobox.h"
#include "TLComboEditbox.h"
#include "TLComboDropList.h"
#include "TLListHeader.h"
#include "TLListHeaderSegment.h"
#include "TLMultiColumnList.h"
#include "TLMultiLineEditbox.h"
#include "TLTabControl.h"
#include "TLTabButton.h"
#include "TLTabPane.h"


/*************************************************************************
	Static factory objects
*************************************************************************/
static CEGUI::TLFrameWindowFactory		s_FrameWindowFactory;
static CEGUI::TLTitlebarFactory			s_TitlebarFactory;
static CEGUI::TLCloseButtonFactory		s_CloseButtonFactory;
static CEGUI::TLButtonFactory			s_ButtonFactory;
static CEGUI::TLCheckboxFactory			s_CheckboxFactory;
static CEGUI::TLRadioButtonFactory		s_RadioButtonFactory;
static CEGUI::TLSliderThumbFactory		s_SliderThumbFactory;
static CEGUI::TLSliderFactory			s_SliderFactory;
static CEGUI::TLProgressBarFactory		s_ProgressBarFactory;
static CEGUI::TLAlternateProgressBarFactory s_AltProgressBarFactory;
static CEGUI::TLEditboxFactory			s_EditboxFactory;
static CEGUI::TLVertScrollbarFactory	s_VertScrollbarFactory;
static CEGUI::TLVertScrollbarThumbFactory	s_VertScrollbarThumbFactory;
static CEGUI::TLMiniVertScrollbarFactory	s_MiniVertScrollbarFactory;
static CEGUI::TLMiniVertScrollbarThumbFactory	s_MiniVertScrollbarThumbFactory;
static CEGUI::TLMiniHorzScrollbarFactory		s_MiniHorzScrollbarFactory;
static CEGUI::TLMiniHorzScrollbarThumbFactory	s_MiniHorzScrollbarThumbFactory;
static CEGUI::TLStaticImageFactory		s_StaticImageFactory;
static CEGUI::TLStaticTextFactory		s_StaticTextFactory;
static CEGUI::TLListboxFactory			s_ListboxFactory;
static CEGUI::TLComboboxFactory			s_ComboboxFactory;
static CEGUI::TLComboDropListFactory	s_ComboDropListFactory;
static CEGUI::TLComboEditboxFactory		s_ComboEditboxFactory;
static CEGUI::TLListHeaderSegmentFactory	s_ListHeaderSegmentFactory;
static CEGUI::TLListHeaderFactory		s_ListHeaderFactory;
static CEGUI::TLMultiColumnListFactory	s_MultiColumnListFactory;
static CEGUI::TLMultiLineEditboxFactory	s_MultiLineEditboxFactory;
static CEGUI::TLTabControlFactory	    s_TabControlFactory;
static CEGUI::TLTabButtonFactory	    s_TabButtonFactory;
static CEGUI::TLTabPaneFactory	        s_TabPaneFactory;


/*************************************************************************
	Plugin access interface
*************************************************************************/
extern "C" void registerFactory(const CEGUI::String& type_name)
{
	using namespace CEGUI;

	if (type_name == TLFrameWindow::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_FrameWindowFactory);
		return;
	}
	else if (type_name == TLTitlebar::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_TitlebarFactory);
		return;
	}
	else if (type_name == TLButton::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_ButtonFactory);
		return;
	}
	else if (type_name == TLCloseButton::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_CloseButtonFactory);
		return;
	}
	else if (type_name == TLCheckbox::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_CheckboxFactory);
		return;
	}
	else if (type_name == TLRadioButton::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_RadioButtonFactory);
		return;
	}
	else if (type_name == TLSliderThumb::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_SliderThumbFactory);
		return;
	}
	else if (type_name == TLSlider::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_SliderFactory);
		return;
	}
	else if (type_name == TLProgressBar::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_ProgressBarFactory);
		return;
	}
	else if (type_name == TLAlternateProgressBar::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_AltProgressBarFactory);
		return;
	}
	else if (type_name == TLEditbox::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_EditboxFactory);
		return;
	}
	else if (type_name == TLVertScrollbar::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_VertScrollbarFactory);
		return;
	}
	else if (type_name == TLVertScrollbarThumb::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_VertScrollbarThumbFactory);
		return;
	}
	else if (type_name == TLMiniVertScrollbar::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_MiniVertScrollbarFactory);
		return;
	}
	else if (type_name == TLMiniVertScrollbarThumb::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_MiniVertScrollbarThumbFactory);
		return;
	}
	else if (type_name == TLMiniHorzScrollbar::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_MiniHorzScrollbarFactory);
		return;
	}
	else if (type_name == TLMiniHorzScrollbarThumb::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_MiniHorzScrollbarThumbFactory);
		return;
	}
	else if (type_name == TLStaticImage::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_StaticImageFactory);
		return;
	}
	else if (type_name == TLStaticText::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_StaticTextFactory);
		return;
	}
	else if (type_name == TLListbox::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_ListboxFactory);
		return;
	}
	else if (type_name == TLCombobox::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_ComboboxFactory);
		return;
	}
	else if (type_name == TLComboDropList::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_ComboDropListFactory);
		return;
	}
	else if (type_name == TLComboEditbox::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_ComboEditboxFactory);
		return;
	}
	else if (type_name == TLListHeaderSegment::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_ListHeaderSegmentFactory);
		return;
	}
	else if (type_name == TLListHeader::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_ListHeaderFactory);
		return;
	}
	else if (type_name == TLMultiColumnList::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_MultiColumnListFactory);
		return;
	}
	else if (type_name == TLMultiLineEditbox::WidgetTypeName)
	{
		WindowFactoryManager::getSingleton().addFactory(&s_MultiLineEditboxFactory);
		return;
	}
	else if (type_name == TLTabControl::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_TabControlFactory);
        return;
    }
	else if (type_name == TLTabButton::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_TabButtonFactory);
        return;
    }
	else if (type_name == TLTabPane::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_TabPaneFactory);
        return;
    }

	throw UnknownObjectException((utf8*)"::registerFactory - The window factory for type '" + type_name + "' is not known in this module.");

	return;
}
