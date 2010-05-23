/***********************************************************************
    filename:   CEGUIWindowRenderer.h
    created:    Jan 11 2006
    author:     Tomas Lindquist Olsen

    purpose:    Defines interface for the WindowRendererManager class
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
#ifndef _CEGUIWindowRendererManager_h_
#define _CEGUIWindowRendererManager_h_

#include "CEGUISingleton.h"
#include "CEGUIWindowRenderer.h"
#include "CEGUILogger.h"
#include "CEGUIExceptions.h"
#include <map>
#include <vector>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
class CEGUIEXPORT WindowRendererManager : public Singleton<WindowRendererManager>
{
public:
    /*************************************************************************
        Contructor / Destructor
    *************************************************************************/
    WindowRendererManager();
    ~WindowRendererManager();

    /*************************************************************************
        Singleton
    *************************************************************************/
    static WindowRendererManager& getSingleton();
    static WindowRendererManager* getSingletonPtr();

    /*************************************************************************
        Accessors
    *************************************************************************/
    bool isFactoryPresent(const String& name) const;
    WindowRendererFactory* getFactory(const String& name) const;

    /*************************************************************************
        Manipulators
    *************************************************************************/
    /*!
    \brief
        Creates a WindowRendererFactory of the type \a T and adds it to the
        system for use.

        The created WindowRendererFactory will automatically be deleted when the
        factory is removed from the system (either directly or at system
        deletion time).

    \tparam T
        Specifies the type of WindowRendererFactory subclass to add a factory
        for.
    */
    template <typename T>
    static void addFactory();
    void addFactory(WindowRendererFactory* wr);
    void removeFactory(const String& name);

    /*************************************************************************
        Factory usage
    *************************************************************************/
    WindowRenderer* createWindowRenderer(const String& name);
    void destroyWindowRenderer(WindowRenderer* wr);

private:
    /*************************************************************************
        Private implementation
    *************************************************************************/

    /*************************************************************************
        Implementation data
    *************************************************************************/
    typedef std::map<String, WindowRendererFactory*, String::FastLessCompare> WR_Registry;
    WR_Registry d_wrReg;

    //! Container type to hold WindowRenderFacory objects that we created.
    typedef std::vector<WindowRendererFactory*> OwnedFactoryList;
    //! Container that tracks WindowFactory objects we created ourselves.
    static OwnedFactoryList d_ownedFactories;
};

//----------------------------------------------------------------------------//
template <typename T>
void WindowRendererManager::addFactory()
{
    // create the factory object
    WindowRendererFactory* factory = new T;

    // only do the actual add now if our singleton has already been created
    if (WindowRendererManager::getSingletonPtr())
    {
        Logger::getSingleton().logEvent("Created WindowRendererFactory for '" +
                                        factory->getName() +
                                        "' WindowRenderers.");
        // add the factory we just created
        CEGUI_TRY
        {
            WindowRendererManager::getSingleton().addFactory(factory);
        }
        CEGUI_CATCH (Exception&)
        {
            Logger::getSingleton().logEvent("Deleted WindowRendererFactory for "
                                            "'" + factory->getName() +
                                            "' WindowRenderers.");
            // delete the factory object
            delete factory;
            CEGUI_RETHROW;
        }
    }

    d_ownedFactories.push_back(factory);
}

//----------------------------------------------------------------------------//


} // End of CEGUI namespace

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif // _CEGUIWindowRendererManager_h_
