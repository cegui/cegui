/***********************************************************************
    filename:   CEGUIWidgetModule.h
    created:    Sun Sep 25 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
#ifndef _CEGUIWidgetModule_h_
#define _CEGUIWidgetModule_h_

#include "CEGUIExceptions.h"
#include "CEGUIWindowFactoryManager.h"

#if (defined( __WIN32__ ) || defined( _WIN32 )) && !defined(CEGUI_STATIC)
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
