/************************************************************************
    filename:   CEGUIWindowRenderer.h
    created:    Jan 11 2006
    author:     Tomas Lindquist Olsen

    purpose:    Defines interface for the WindowRenderer base class
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
#ifndef _CEGUIWindowRenderer_h_
#define _CEGUIWindowRenderer_h_

#include "CEGUIWindow.h"
#include "CEGUIProperty.h"
#include <vector>

// Start of CEGUI namespace section
namespace CEGUI
{

class WidgetLookFeel;

/*!
\brief
    Base-class for the assignable WindowRenderer object
*/
class CEGUIEXPORT WindowRenderer
{
public:
    /// Constructor
    WindowRenderer(const String& name, const String& class_name="Window");

    /// Destructor
    virtual ~WindowRenderer();

    /// Populate render cache
    virtual void render() = 0;

    /// Get name
    const String& getName() const {return d_name;}

    /// Get the window we are attached to
    Window* getWindow() const {return d_window;}

    /// Get the "minimum" Window class this renderer requires
    const String& getClass() const {return d_class;}

    /// Get the Look'N'Feel assigned to our window
    const WidgetLookFeel& getLookNFeel() const;

    /// Get unclipped inner rectangle
    virtual Rect getUnclippedInnerRect() const;

    /// Get actual pixel rectangle
    virtual Rect getPixelRect() const;

    /// Method called to perform extended laying out of the window's attached child windows.
    virtual void performChildWindowLayout() {}

protected:
    friend class Window;

    // PROTECTED METHODS
    void registerProperty(Property* property);

    // PROTECTED VIRTUAL METHODS
    virtual void onAttach();
    virtual void onDetach();
    virtual void onLookNFeelAssigned() {}
    virtual void onLookNFeelUnassigned() {}

    // PROTECTED IMPLEMENTATION DATA
    Window* d_window;
    const String d_name;
    const String d_class;

    typedef std::vector<Property*> PropertyList;
    PropertyList d_properties;
};

/*!
\brief
    Base-class for WindowRendererFactory
*/
class CEGUIEXPORT WindowRendererFactory
{
public:
    WindowRendererFactory(const String& name) : d_factoryName(name) {}
    const String& getName() const {return d_factoryName;}

    virtual WindowRenderer* create() = 0;
    virtual void destroy(WindowRenderer* wr) = 0;

protected:
    String d_factoryName;
};

} // End of CEGUI namespace

#endif // _CEGUIWindowRenderer_h_
