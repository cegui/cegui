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


/*************************************************************************
	Plugin access interface
*************************************************************************/
extern "C" void registerFactory(const CEGUI::String& type_name)
{
	using namespace CEGUI;

	if (type_name == "Taharez Frame Window")
	{
		WindowFactoryManager::getSingleton().addFactory(new TLFrameWindowFactory());
		return;
	}
	else if (type_name == "Taharez Titlebar")
	{
		WindowFactoryManager::getSingleton().addFactory(new TLTitlebarFactory());
		return;
	}
	else if (type_name == "Taharez Button")
	{
		WindowFactoryManager::getSingleton().addFactory(new TLButtonFactory());
		return;
	}
	else if (type_name == "Taharez Close Button")
	{
		WindowFactoryManager::getSingleton().addFactory(new TLCloseButtonFactory());
		return;
	}
	else if (type_name == "Taharez Checkbox")
	{
		WindowFactoryManager::getSingleton().addFactory(new TLCheckboxFactory());
		return;
	}
	else if (type_name == "Taharez RadioButton")
	{
		WindowFactoryManager::getSingleton().addFactory(new TLRadioButtonFactory());
		return;
	}
	else if (type_name == "Taharez Slider Thumb")
	{
		WindowFactoryManager::getSingleton().addFactory(new TLSliderThumbFactory());
		return;
	}
	else if (type_name == "Taharez Slider")
	{
		WindowFactoryManager::getSingleton().addFactory(new TLSliderFactory());
		return;
	}
	else if (type_name == "Taharez ProgressBar")
	{
		WindowFactoryManager::getSingleton().addFactory(new TLProgressBarFactory());
		return;
	}
	else if (type_name == "Taharez AltProgressBar")
	{
		WindowFactoryManager::getSingleton().addFactory(new TLAlternateProgressBarFactory());
		return;
	}
	else if (type_name == "Taharez Editbox")
	{
		WindowFactoryManager::getSingleton().addFactory(new TLEditboxFactory());
		return;
	}
	else if (type_name == "Taharez VertScrollbar")
	{
		WindowFactoryManager::getSingleton().addFactory(new TLVertScrollbarFactory());
		return;
	}
	else if (type_name == "Taharez VertScrollbarThumb")
	{
		WindowFactoryManager::getSingleton().addFactory(new TLVertScrollbarThumbFactory());
		return;
	}
	else if (type_name == "Taharez MiniVertScrollbar")
	{
		WindowFactoryManager::getSingleton().addFactory(new TLMiniVertScrollbarFactory());
		return;
	}
	else if (type_name == "Taharez MiniVertScrollbarThumb")
	{
		WindowFactoryManager::getSingleton().addFactory(new TLMiniVertScrollbarThumbFactory());
		return;
	}
	else if (type_name == "Taharez MiniHorzScrollbar")
	{
		WindowFactoryManager::getSingleton().addFactory(new TLMiniHorzScrollbarFactory());
		return;
	}
	else if (type_name == "Taharez MiniHorzScrollbarThumb")
	{
		WindowFactoryManager::getSingleton().addFactory(new TLMiniHorzScrollbarThumbFactory());
		return;
	}

	throw UnknownObjectException((utf8*)"::registerFactory - The window factory for type '" + type_name + "' is not known in this module.");

	return;
}