/************************************************************************
    filename:   CEGUIWidgetModule.h
    created:    Sun Sep 25 2005
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
#ifndef _CEGUIWidgetModule_h_
#define _CEGUIWidgetModule_h_

#include "CEGUIExceptions.h"
#include "CEGUIWindowFactoryManager.h"

#if defined( __WIN32__ ) || defined( _WIN32 )
#   ifdef CEGUIWIDGETMODULE_EXPORTS
#       define CEGUIWIDGETMODULE_API __declspec(dllexport)
#   else
#       define CEGUIWIDGETMODULE_API __declspec(dllimport)
#   endif
#else
#   define CEGUIWIDGETMODULE_API 
#endif

#define CEGUI_DECLARE_WIDGET_MODULE( moduleName )\
\
class CEGUI::WindowFactory;\
\
extern "C" CEGUIWIDGETMODULE_API void registerFactory(const CEGUI::String& type_name);\
extern "C" CEGUIWIDGETMODULE_API CEGUI::uint registerAllFactories(void);\
void doSafeFactoryRegistration(CEGUI::WindowFactory* factory);


#define CEGUI_DEFINE_FACTORY( className )\
namespace CEGUI {\
class className ## Factory : public WindowFactory\
{\
public:\
    className ## Factory(void) : WindowFactory(className::WidgetTypeName) { }\
    Window* createWindow(const String& name)\
    { return new className(d_type, name); }\
    void destroyWindow(Window* window)\
    { delete window; }\
};\
}\
static CEGUI::className ## Factory  s_ ## className ## Factory;

#define CEGUI_START_FACTORY_MAP( module )\
struct module ## MapEntry\
{\
    const CEGUI::utf8* d_name;\
    CEGUI::WindowFactory* d_factory;\
};\
\
module ## MapEntry module ## FactoriesMap[] =\
{\

#define CEGUI_END_FACTORY_MAP {0,0}};

#define CEGUI_FACTORY_MAP_ENTRY( class )\
{CEGUI::class::WidgetTypeName, &s_ ## class ## Factory},

#define CEGUI_DEFINE_WIDGET_MODULE( module )\
extern "C" void registerFactory(const CEGUI::String& type_name)\
{\
    module ## MapEntry* entry = module ## FactoriesMap;\
    while (entry->d_name)\
    {\
        if (entry->d_name == type_name)\
        {\
            doSafeFactoryRegistration(entry->d_factory);\
            return;\
        }\
        ++entry;\
    }\
\
    throw CEGUI::UnknownObjectException("::registerFactory - The window factory for type '" + type_name + "' is not known in this module.");\
}\
\
extern "C" CEGUI::uint registerAllFactories(void)\
{\
    CEGUI::uint count = 0;\
    module ## MapEntry* entry = module ## FactoriesMap;\
    while (entry->d_name)\
    {\
        doSafeFactoryRegistration(entry->d_factory);\
        ++entry;\
        ++count;\
    }\
    return count;\
}\
\
void doSafeFactoryRegistration(CEGUI::WindowFactory* factory)\
{\
    assert(factory != 0);\
\
    CEGUI::WindowFactoryManager& wfm = CEGUI::WindowFactoryManager::getSingleton();\
    if (wfm.isFactoryPresent(factory->getTypeName()))\
    {\
        CEGUI::Logger::getSingleton().logEvent(\
            "Widget factory '" + factory->getTypeName() + "' appears to be already registered, skipping.",\
            CEGUI::Informative);\
    }\
    else\
    {\
        wfm.addFactory(factory);\
    }\
}

#endif  // end of guard _CEGUIWidgetModule_h_
