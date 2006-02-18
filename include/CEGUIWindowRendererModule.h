/************************************************************************
    filename:   CEGUIWindowRendererModule.h
    created:    Fri Jan 13 2006
    author:     Paul D Turner <paul@cegui.org.uk>
                Tomas Lindquist Olsen <tomas@famolsen.dk>
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
#ifndef _CEGUIWindowRendererModule_h_
#define _CEGUIWindowRendererModule_h_

#include "CEGUIExceptions.h"
#include "CEGUIWindowRendererManager.h"
#include "CEGUILogger.h"

#if defined( __WIN32__ ) || defined( _WIN32 )
#   ifdef CEGUIWRMODULE_EXPORTS
#       define CEGUIWRMODULE_API __declspec(dllexport)
#   else
#       define CEGUIWRMODULE_API __declspec(dllimport)
#   endif
#else
#   define CEGUIWRMODULE_API 
#endif

// declare module
#define CEGUI_DECLARE_WR_MODULE( moduleName )\
\
class CEGUI::WindowRendererFactory;\
\
extern "C" CEGUIWRMODULE_API void registerFactory(const CEGUI::String& type_name);\
extern "C" CEGUIWRMODULE_API CEGUI::uint registerAllFactories(void);\
void doSafeFactoryRegistration(CEGUI::WindowRendererFactory* factory);

// define factory
#define CEGUI_DEFINE_WR_FACTORY( className )\
namespace CEGUI {\
class className ## WRFactory : public WindowRendererFactory\
{\
public:\
    className ## WRFactory(void) : WindowRendererFactory(className::TypeName) { }\
    WindowRenderer* create(void)\
    { return new className(className::TypeName); }\
    void destroy(WindowRenderer* wr)\
    { delete wr; }\
};\
}\
static CEGUI::className ## WRFactory  s_ ## className ## WRFactory;

// start factory map
#define CEGUI_START_WR_FACTORY_MAP( module )\
struct module ## WRMapEntry\
{\
    const CEGUI::utf8* d_name;\
    CEGUI::WindowRendererFactory* d_factory;\
};\
\
module ## WRMapEntry module ## WRFactoriesMap[] =\
{\

// end factory map
#define CEGUI_END_WR_FACTORY_MAP {0,0}};

// map entry
#define CEGUI_WR_FACTORY_MAP_ENTRY( class )\
{CEGUI::class::TypeName, &s_ ## class ## WRFactory},

// define module
#define CEGUI_DEFINE_WR_MODULE( module )\
extern "C" void registerFactory(const CEGUI::String& type_name)\
{\
    module ## WRMapEntry* entry = module ## WRFactoriesMap;\
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
    throw CEGUI::UnknownObjectException("::registerFactory - The window renderer factory for type '" + type_name + "' is not known in this module.");\
}\
\
extern "C" CEGUI::uint registerAllFactories(void)\
{\
    CEGUI::uint count = 0;\
    module ## WRMapEntry* entry = module ## WRFactoriesMap;\
    while (entry->d_name)\
    {\
        doSafeFactoryRegistration(entry->d_factory);\
        ++entry;\
        ++count;\
    }\
    return count;\
}\
\
void doSafeFactoryRegistration(CEGUI::WindowRendererFactory* factory)\
{\
    assert(factory != 0);\
\
    CEGUI::WindowRendererManager& wfm = CEGUI::WindowRendererManager::getSingleton();\
    if (wfm.isFactoryPresent(factory->getName()))\
    {\
        CEGUI::Logger::getSingleton().logEvent(\
            "WindowRenderer factory '" + factory->getName() + "' appears to be already registered, skipping.",\
            CEGUI::Informative);\
    }\
    else\
    {\
        wfm.addFactory(factory);\
    }\
}

#endif  // end of guard _CEGUIWindowRendererModule_h_
