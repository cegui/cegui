/***********************************************************************
    created:    1/3/2005
    author:     Paul D Turner
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
#ifndef _CEGUIScrolledContainer_h_
#define _CEGUIScrolledContainer_h_

#include "../Window.h"
#include "../WindowFactory.h"
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
    Helper container window class which is used in the implementation of the
    ScrollablePane widget class.
*/
class CEGUIEXPORT ScrolledContainer : public Window
{
public:
    //! Type name for ScrolledContainer.
    static const String WidgetTypeName;
    //! Namespace for global events
    static const String EventNamespace;

    //! Constructor for ScrolledContainer objects.
    ScrolledContainer(const String& type, const String& name);

    /*!
    \brief
        Return the current content pane area for the ScrolledContainer.

    \return
        Rect object that details the current pixel extents of the content
        pane represented by this ScrolledContainer.
    */
    Rectf getContentPixelRect() const;

    /*!
    \brief
        Return the current extents of the attached content.

    \return
        Rect object that describes the pixel extents of the attached
        child windows.  This is effectively the smallest bounding box
        that could contain all the attached windows.
    */
    Rectf getChildExtentsArea() const;

    virtual void adjustSizeToContent() override;
    
    const CachedRectf& getChildContentArea(const bool non_client = false) const override { (void)non_client; return d_childContentArea; }

protected:
    // Overridden from Window.
    Rectf getUnclippedInnerRect_impl(bool skipAllPixelAlignment) const override;
    
    Rectf getChildContentArea_impl(bool skipAllPixelAlignment) const;

    //! handles notifications about child windows being moved or sized.
    bool handleChildAreaChanged(const EventArgs& e);
    void subscribeOnChildAreaEvents(Window* child);
    void onIsSizeAdjustedToContentChanged(ElementEventArgs& e) override;

    // overridden from Window.
    void drawSelf(const RenderingContext&, std::uint32_t) override { /* do nothing; rendering is handled by children */ }
    Rectf getInnerRectClipper_impl() const override;
    Rectf getHitTestRect_impl() const override;
    void onChildAdded(ElementEventArgs& e) override;
    void onChildRemoved(ElementEventArgs& e) override;
    void cleanupChildren(void) override;
    uint8_t handleAreaChanges(bool movedOnScreen, bool movedInParent, bool sized) override;

    //! type definition for collection used to track event connections.
    typedef std::multimap<Window*, Event::Connection> ConnectionTracker;
    //! Tracks event connections we make.
    ConnectionTracker d_childAreaChangeConnections;

    // This offset helps to handle negative child coords in an auto-sized content area.
    // It is intentionally not exposed to user. Use positive coords when possible.
    glm::vec2 d_contentOffset;

    CachedRectf d_childContentArea;
};

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif    // end of guard _CEGUIScrolledContainer_h_
