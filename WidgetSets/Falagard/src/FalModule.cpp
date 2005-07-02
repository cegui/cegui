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
#include "CEGUIExceptions.h"
#include "CEGUIWindowFactoryManager.h"
// includes for types we create
#include "FalButton.h"
#include "FalCheckbox.h"
#include "FalEditbox.h"
#include "FalFrameWindow.h"
#include "FalRadioButton.h"
#include "FalTitlebar.h"

/*************************************************************************
    Static factory objects
*************************************************************************/
static CEGUI::FalagardButtonFactory             s_ButtonFactory;
static CEGUI::FalagardCheckboxFactory           s_CheckboxFactory;
static CEGUI::FalagardEditboxFactory            s_EditboxFactory;
static CEGUI::FalagardFrameWindowFactory        s_FrameWindowFactory;
static CEGUI::FalagardRadioButtonFactory        s_RadioButtonFactory;
static CEGUI::FalagardTitlebarFactory           s_TitlebarFactory;


using namespace CEGUI;

struct mapEntry
{
    const CEGUI::utf8* d_name;
    CEGUI::WindowFactory* d_factory;
};

mapEntry factoriesMap[] =
{
    {FalagardButton::WidgetTypeName, &s_ButtonFactory},
    {FalagardCheckbox::WidgetTypeName, &s_CheckboxFactory},
    {FalagardEditbox::WidgetTypeName, &s_EditboxFactory},
    {FalagardFrameWindow::WidgetTypeName, &s_FrameWindowFactory},
    {FalagardRadioButton::WidgetTypeName, &s_RadioButtonFactory},
    {FalagardTitlebar::WidgetTypeName, &s_TitlebarFactory},
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
