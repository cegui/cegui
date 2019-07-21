/***********************************************************************
    created:    29/7/2010
    author:     Martin Preisler

    purpose:    Defines abstract base class for layout containers
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
#ifndef _CEGUILayoutContainer_h_
#define _CEGUILayoutContainer_h_

#include "../Window.h"

#include <map>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
    An abstract base class providing common functionality and specifying the
    required interface for derived classes.

    Layout Container provide means for automatic positioning based on sizes of
    it's child Windows. This is useful for dynamic UIs.
*/
class CEGUIEXPORT LayoutContainer : public Window
{
public:
    /*************************************************************************
        Event name constants
    *************************************************************************/
    //! Namespace for global events
    static const String EventNamespace;

    //! fired when child windows get rearranged
    static const String EventChildOrderChanged;

    /*!
    \brief
        Constructor for Window base class

    \param type
        String object holding Window type (usually provided by WindowFactory).

    \param name
        String object holding unique name for the Window.
    */
    LayoutContainer(const String& type, const String& name);

    /*!
    \brief
        Destructor for Window base class
    */
    virtual ~LayoutContainer(void);

    /*!
    \brief
        marks this layout container for relayouting before drawing
    */
    void markNeedsLayouting();

    /*!
    \brief
        returns true if this layout container will be relayouted before drawing
    */
    bool needsLayouting() const;

    /*!
    \brief
        (re)layouts all windows inside this layout container immediately
    */
    virtual void layout() = 0;

    /*!
    \brief
        (re)layouts all windows inside this layout container if it was marked
        necessary
    */
    void layoutIfNecessary();

    /*!
    \brief
        Returns an actual layout child count not including auxiliary items
    */
    virtual size_t getActualChildCount() const;

    /*!
    \brief
        Gets the position of given child window
    */
    size_t getChildIndexByName(const String& wnd) const;

    /*!
    \brief
        Swaps positions of given windows
    */
    void swapChildren(Window* wnd1, Window* wnd2);

    /*!
    \brief
        Swaps positions of given windows
    */
    void swapChildren(const String& wnd1, Window* wnd2);

    /*!
    \brief
        Swaps positions of given windows
    */
    void swapChildren(Window* wnd1, const String& wnd2);

    /*!
    \brief
        Swaps positions of given windows
    */
    void swapChildren(const String& wnd1, const String& wnd2);

    /*!
    \brief
        Swaps child windows at given positions
    */
    void swapChildren(size_t index1, size_t index2);

    /*!
    \brief
        Moves a window that is already a child of the layout container
        to given position (if the window is currently in a position
        that is smaller than given position, given position is
        automatically decremented
    */
    void moveChildToIndex(size_t indexFrom, size_t indexTo);

    /*!
    \brief
        Moves a window that is already a child of the layout container
        to given position (if the window is currently in a position
        that is smaller than given position, given position is
        automatically decremented
    */
    void moveChildToIndex(Window* wnd, size_t index);

    /*!
    \brief
        Moves a window that is already a child of the layout container
        to given position (if the window is currently in a position
        that is smaller than given position, given position is
        automatically decremented
    */
    void moveChildToIndex(const String& wnd, size_t index);

    /*!
    \brief
        Moves a window forward or backward, depending on delta
        (-1 moves it backward one step, 1 moves it forward one step)

    \param delta
        The amount of steps the window will be moved
        (old position + delta = new position)
    */
    void moveChild(Window* window, int delta = 1);

    /*!
    \brief
        Adds a window to given position
    */
    void addChildToIndex(Window* window, size_t position);

    /*!
    \brief
        Removes a window from given position
    */
    void removeChildFromIndex(size_t position);

    /// @copydoc Window::update
    void update(float elapsed) override;

    const CachedRectf& getClientChildContentArea() const override;

    void notifyScreenAreaChanged(bool recursive) override;

protected:
    /// @copydoc Window::getUnclippedInnerRect_impl
    Rectf getUnclippedInnerRect_impl(bool skipAllPixelAlignment) const override;
    
    Rectf getClientChildContentArea_impl(bool skipAllPixelAlignment) const;

    /// @copydoc Window::addChild_impl
    void addChild_impl(Element* element) override;
    /// @copydoc Window::removeChild_impl
    void removeChild_impl(Element* element) override;

    /*************************************************************************
        Event trigger methods
    *************************************************************************/
    /*!
    \brief
        Handler called when child window gets sized

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the
        window that triggered the event.  For this event the trigger window is
        the one that was sized.
    */
    virtual bool handleChildSized(const EventArgs& e);

    /*!
    \brief
        Handler called when child window changes margin(s)

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the
        window that triggered the event.  For this event the trigger window is
        the one that has had it's margin(s) changed.
    */
    virtual bool handleChildMarginChanged(const EventArgs& e);

    /*!
    \brief
        Handler called when child window gets added

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the
        window that triggered the event.  For this event the trigger window is
        the one that was added.
    */
    virtual bool handleChildAdded(const EventArgs& e);

    /*!
    \brief
        Handler called when child window gets removed

    \param e
        WindowEventArgs object whose 'window' pointer field is set to the
        window that triggered the event.  For this event the trigger window is
        the one that was removed.
    */
    virtual bool handleChildRemoved(const EventArgs& e);

    /*!
    \brief
        returns margin offset for given window
    */
    virtual UVector2 getOffsetForWindow(Window* window) const;

    /*!
    \brief
        returns bounding size for window, including margins
    */
    virtual UVector2 getBoundingSizeForWindow(Window* window) const;

    // overridden from parent class
    void onParentSized(ElementEventArgs& e) override;

    /*!
    \brief
        Handler called when children of this window gets rearranged in any way

    \param e
        WindowEventArgs object whose 'window' field is set this layout
        container.
    */
    virtual void onChildOrderChanged(WindowEventArgs& e);

    /*************************************************************************
        Implementation Data
    *************************************************************************/
    // if true, we will relayout before rendering of this window starts
    bool d_needsLayouting;

    typedef std::multimap<Window*, Event::Connection>  ConnectionTracker;
    //! Tracks event connections we make.
    ConnectionTracker d_eventConnections;
    
    CachedRectf d_clientChildContentArea;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUILayoutContainer_h_

