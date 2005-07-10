/************************************************************************
	filename: 	TLModule.cpp
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Implements the System <-> GUI Module interface.
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
#include "TLVUMeter.h"
#include "TLSpinner.h"
#include "TLScrollablePane.h"
#include "TLTooltip.h"


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
static CEGUI::TLVUMeterFactory			s_VUMeterFactory;
static CEGUI::TLSpinnerFactory          s_SpinnerFactory;
static CEGUI::TLScrollablePaneFactory   s_ScrollablePaneFactory;
static CEGUI::TLTooltipFactory          s_TooltipFactory;

using namespace CEGUI;

struct mapEntry
{
    const CEGUI::utf8* d_name;
    CEGUI::WindowFactory* d_factory;
};

mapEntry factoriesMap[] =
{
    {TLAlternateProgressBar::WidgetTypeName, &s_AltProgressBarFactory},
    {TLButton::WidgetTypeName, &s_ButtonFactory},
    {TLCheckbox::WidgetTypeName, &s_CheckboxFactory},
    {TLCloseButton::WidgetTypeName, &s_CloseButtonFactory},
    {TLCombobox::WidgetTypeName, &s_ComboboxFactory},
    {TLComboDropList::WidgetTypeName, &s_ComboDropListFactory},
    {TLComboEditbox::WidgetTypeName, &s_ComboEditboxFactory},    
    {TLEditbox::WidgetTypeName, &s_EditboxFactory},
    {TLFrameWindow::WidgetTypeName, &s_FrameWindowFactory},
    {TLListbox::WidgetTypeName, &s_ListboxFactory},
    {TLListHeader::WidgetTypeName, &s_ListHeaderFactory},
    {TLListHeaderSegment::WidgetTypeName, &s_ListHeaderSegmentFactory},
    {TLMiniHorzScrollbar::WidgetTypeName, &s_MiniHorzScrollbarFactory},
    {TLMiniHorzScrollbarThumb::WidgetTypeName, &s_MiniHorzScrollbarThumbFactory},
    {TLMiniVertScrollbar::WidgetTypeName, &s_MiniVertScrollbarFactory},
    {TLMiniVertScrollbarThumb::WidgetTypeName, &s_MiniVertScrollbarThumbFactory},
    {TLMultiColumnList::WidgetTypeName, &s_MultiColumnListFactory},
    {TLMultiLineEditbox::WidgetTypeName, &s_MultiLineEditboxFactory},
    {TLProgressBar::WidgetTypeName, &s_ProgressBarFactory},
    {TLRadioButton::WidgetTypeName, &s_RadioButtonFactory},
    {TLScrollablePane::WidgetTypeName, &s_ScrollablePaneFactory},
    {TLSlider::WidgetTypeName, &s_SliderFactory},
    {TLSliderThumb::WidgetTypeName, &s_SliderThumbFactory},
    {TLSpinner::WidgetTypeName, &s_SpinnerFactory},
    {TLStaticImage::WidgetTypeName, &s_StaticImageFactory},
    {TLStaticText::WidgetTypeName, &s_StaticTextFactory},
    {TLTabButton::WidgetTypeName, &s_TabButtonFactory},
    {TLTabControl::WidgetTypeName, &s_TabControlFactory},
    {TLTabPane::WidgetTypeName, &s_TabPaneFactory},
    {TLTitlebar::WidgetTypeName, &s_TitlebarFactory},
    {TLTooltip::WidgetTypeName, &s_TooltipFactory},
    {TLVertScrollbar::WidgetTypeName, &s_VertScrollbarFactory},
    {TLVertScrollbarThumb::WidgetTypeName, &s_VertScrollbarThumbFactory},
    {TLVUMeter::WidgetTypeName, &s_VUMeterFactory},
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
