/***********************************************************************
    created:    12/08/2013
    author:     Timotei Dolean

    purpose:    Implements the ability to navigate in the GUI
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIWindowNavigator_h_
#define _CEGUIWindowNavigator_h_

#include <map>
#include "CEGUI/String.h"
#include "CEGUI/SemanticInputEvent.h"

#if defined (_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{

/*!
\brief
    Provides a strategy for navigating the GUI based on a specified mapping.

    For a brief tutorial on how to use the GUI navigation please refer
    to the @ref gui_navigation_tutorial
*/
class CEGUIEXPORT NavigationStrategy
{
public:
    virtual ~NavigationStrategy() {}

    /*!
    \brief
        Returns a window based on its neighbour and a certain payload

    \param neighbour
        The neighbour window relative to which the new window is requested

    \param payload
        A string payload value to help decide what window to return
    */
    virtual Window* getWindow(Window* neighbour, const String& payload) = 0;
};

/*!
\brief
    Provides a way of navigating the GUI by means of focusing windows

    For a brief tutorial on how to use the GUI navigation please refer
    to the @ref gui_navigation_tutorial
*/
class CEGUIEXPORT WindowNavigator
{
public:
    typedef std::map<SemanticValue, String> SemanticMappingsMap;
    ~WindowNavigator() {}

    /*!
    \brief
        Creates a new navigator with the specified event <-> payload mapping and
        the specified strategy

    \param mapping
        A mapping from semantic input events to certain strategy-specific payloads

    \param strategy
        The navigation strategy to be used
    */
    WindowNavigator(SemanticMappingsMap mappings, NavigationStrategy* strategy);

    /*!
    \brief
        Handles the specified semantic input event and generate a navigation if
        that is the case (a mapping matches)

    \param event
        The semantic input event
    */
    void handleSemanticEvent(const SemanticInputEvent& event);

    /*!
    \brief
        Sets a new current focused window

    \param window
        The window to be the new focused one
    */
    void setCurrentFocusedWindow(Window* window);

    /*!
    \brief
        Gets the current focused window

    \return
        An instance of Window
    */
    Window* getCurrentFocusedWindow();

private:
    SemanticMappingsMap d_mappings;
    NavigationStrategy* d_strategy;

    Window* d_currentFocusedWindow;
};

}

#if defined (_MSC_VER)
#   pragma warning(pop)
#endif

#endif    // end of guard _CEGUIWindowNavigator_h_
