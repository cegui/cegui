/************************************************************************
    filename:   CEGUIWindowRenderer.h
    created:    Jan 11 2006
    author:     Tomas Lindquist Olsen

    purpose:    Defines interface for the WindowRendererManager class
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2006 Paul D Turner (paul@cegui.org.uk)

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
#ifndef _CEGUIWindowRendererManager_h_
#define _CEGUIWindowRendererManager_h_

#include "CEGUIWindowRenderer.h"
#include "CEGUISingleton.h"
//#include "CEGUIIteratorBase.h"
#include <map>

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
};

} // End of CEGUI namespace

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif // _CEGUIWindowRendererManager_h_
