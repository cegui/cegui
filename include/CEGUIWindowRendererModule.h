/***********************************************************************
    filename:   CEGUIWindowRendererModule.h
    created:    Fri Jan 13 2006
    author:     Paul D Turner <paul@cegui.org.uk>
                Tomas Lindquist Olsen <tomas@famolsen.dk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#ifndef _CEGUIWindowRendererModule_h_
#define _CEGUIWindowRendererModule_h_

#include "CEGUIExceptions.h"
#include "CEGUIWindowRendererManager.h"
#include "CEGUILogger.h"

#if (defined( __WIN32__ ) || defined( _WIN32 )) && !defined(CEGUI_STATIC)
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
extern "C" CEGUIWRMODULE_API void registerFactoryFunction(const CEGUI::String& type_name);\
extern "C" CEGUIWRMODULE_API CEGUI::uint registerAllFactoriesFunction(void);\
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
void registerFactoryFunction(const CEGUI::String& type_name)\
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
extern "C" CEGUI::uint registerAllFactoriesFunction(void)\
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
