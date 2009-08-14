/***********************************************************************
    filename:   CEGUIWindowRendererModule.h
    created:    Fri Jan 13 2006
    author:     Paul D Turner <paul@cegui.org.uk>
                Tomas Lindquist Olsen <tomas@famolsen.dk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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

#include "CEGUIBase.h"
#include <vector>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace CEGUI
{
//! Abstract interface for window renderer module objects.
class CEGUIEXPORT WindowRendererModule
{
public:
    //! Destructor.
    virtual ~WindowRendererModule();
    //! Register the factory for WindowRenderers of the specified type.
    void registerFactory(const String& type_name);
    //! Register factories for all WindowRenderer types in the module.
    uint registerAllFactories();
    //! Unregister the factory for WindowRenderers of the specified type.
    void unregisterFactory(const String& type_name);
    //! Unregister factories for all WindowRenderer types in the module.
    uint unregisterAllFactories();

protected:
    //! Collection type that holds pointers to the factory registerer objects.
    typedef std::vector<WRFactoryRegisterer*> FactoryRegistry;
    //! The collection of factorty registerer object pointers.
    FactoryRegistry d_registry;
};

}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _CEGUIWindowRendererModule_h_
