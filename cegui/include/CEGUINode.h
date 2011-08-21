/***********************************************************************
    filename:   CEGUINode.h
    created:    18/8/2011
    author:     Martin Preisler

    purpose:    Defines a class representing an item in a graph
                (deals with relative positions, relative dimensions, ...)
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUINode_h_
#define _CEGUINode_h_

#include "CEGUIBase.h"
#include "CEGUIPropertySet.h"
#include "CEGUIEventSet.h"
#include "CEGUIEventArgs.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
    Enumerated type used when specifying horizontal alignments.
 */
enum HorizontalAlignment
{
    /**
     * Window's position specifies an offset of it's left edge from the left
     * edge of it's parent.
     */
    HA_LEFT,
    /**
     * Window's position specifies an offset of it's horizontal centre from the
     * horizontal centre of it's parent.
     */
    HA_CENTRE,
    /**
     * Window's position specifies an offset of it's right edge from the right
     * edge of it's parent.
     */
    HA_RIGHT
};

template<>
class PropertyHelper<HorizontalAlignment>
{
public:
    typedef HorizontalAlignment return_type;
    typedef return_type safe_method_return_type;
    typedef HorizontalAlignment pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName()
    {
        static String type("HorizontalAlignment");

        return type;
    }

    static return_type fromString(const String& str)
    {
    
        if (str == "Centre")
        {
            return HA_CENTRE;
        }
        else if (str == "Right")
        {
            return HA_RIGHT;
        }
        else
        {
            return HA_LEFT;
        }
    }

    static string_return_type toString(pass_type val)
    {
        if (val == HA_CENTRE)
        {
            return "Centre";
        }
        else if (val == HA_RIGHT)
        {
            return "Right";
        }
        else if (val == HA_LEFT)
        {
            return "Left";
        }
        else
        {
            assert(false && "Invalid horizontal alignment");
            return "Centre";
        }
    }
};

/*!
\brief
    Enumerated type used when specifying vertical alignments.
 */
enum VerticalAlignment
{
    /**
     * Window's position specifies an offset of it's top edge from the top edge
     * of it's parent.
     */
    VA_TOP,
    /**
     * Window's position specifies an offset of it's vertical centre from the
     * vertical centre of it's parent.
     */
    VA_CENTRE,
    /**
     * Window's position specifies an offset of it's bottom edge from the
     * bottom edge of it's parent.
     */
    VA_BOTTOM
};

template<>
class PropertyHelper<CEGUI::VerticalAlignment>
{
public:
    typedef VerticalAlignment return_type;
    typedef return_type safe_method_return_type;
    typedef VerticalAlignment pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName()
    {
        static String type("VerticalAlignment");

        return type;
    }

    static return_type fromString(const String& str)
    {
    
      if (str == "Centre")
      {
          return VA_CENTRE;
      }
      else if (str == "Bottom")
      {
          return VA_BOTTOM;
      }
      else
      {
          return VA_TOP;
      }
    }

    static string_return_type toString(pass_type val)
    {
        if (val == VA_CENTRE)
        {
            return "Centre";
        }
        else if (val == VA_BOTTOM)
        {
            return "Bottom";
        }
        else if (val == VA_TOP)
        {
            return "Top";
        }
        else
        {
            assert(false && "Invalid vertical alignment");
            return "Centre";
        }
    }
};

/*!
\brief
    EventArgs based class that is used for objects passed to handlers triggered for events
    concerning some Node object.
*/
class CEGUIEXPORT NodeEventArgs : public EventArgs
{
public:
    NodeEventArgs(Node* node):
        node(node)
    {}

    Node* node;     //!< pointer to a Node object of relevance to the event.
};

/*!
\brief Represents a positioned and sized node in a tree graph (think of it as a widget graph)

\internal
    Methods retrieving Node (like getParentNode) have Node suffix so that deriving
    classes can easily make their getParent and return the proper type (Window* for example)
*/
class CEGUIEXPORT Node :
    public PropertySet,
    public EventSet,
    public AllocatedObject<Node>
{
public:
    //! Namespace for global events
    static const String EventNamespace;

    // generated internally by Node
    /** Event fired when the Node size has changed.
     * Handlers are passed a const NodeEventArgs reference with
     * NodeEventArgs::node set to the Node whose size was changed.
     */
    static const String EventSized;
    /** Event fired when the parent of this Window has been re-sized.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window pointing to the <em>parent window</em> that
     * was resized, not the window whose parent was resized.
     */
    static const String EventParentSized;
    /** Event fired when the Node position has changed.
     * Handlers are passed a const NodeEventArgs reference with
     * NodeEventArgs::node set to the Node whose position was changed.
     */
    static const String EventMoved;
    /** Event fired when the horizontal alignment for the window is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the window whose horizontal alignment
     * setting was changed.
     */
    static const String EventHorizontalAlignmentChanged;
    /** Event fired when the vertical alignment for the window is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the window whose vertical alignment
     * setting was changed.
     */
    static const String EventVerticalAlignmentChanged;
    
    /*!
    \brief Node caches many rectangles, this class is a tiny wrapper to hide at least some of the dirty work
    */
    class CachedRectf
    {
    public:
        //! the bool parameter - if true all will PixelAlignment settings be overridden and no pixel alignment will take place
        typedef Rectf (Node::*DataGenerator)(bool) const;
        
        CachedRectf(Node const* node, DataGenerator generator):
            d_node(node),
            d_generator(generator),
            // we don't have to initialise d_cachedData, it will get regenerated and reset anyways
            d_cacheValid(false)
        {}
        
        inline const Rectf& get() const
        {
            if (!d_cacheValid)
            {
                regenerateCache();
            }
            
            return d_cachedData;
        }
        
        /*!
        \brief skips all caching and calls the generator
        */
        inline Rectf getFresh(bool noPixelAlignment = false) const
        {
            // if the cache is not valid we will use this chance to regenerate it
            // of course this is only applicable if we are allowed to use pixel alignment where applicable
            if (!d_cacheValid && !noPixelAlignment)
            {
                return get();
            }
            
            return CEGUI_CALL_MEMBER_FN(*d_node, d_generator)(noPixelAlignment);
        }
        
        inline void invalidateCache() const
        {
            d_cacheValid = false;
        }
        
        inline bool isCachedValid() const
        {
            return d_cacheValid;
        }
        
        inline void regenerateCache() const
        {
            // false, since when we are caching we don't want to override anything
            d_cachedData = CEGUI_CALL_MEMBER_FN(*d_node, d_generator)(false);
            
            d_cacheValid = true;
        }
        
    private:
        Node const* d_node;
        DataGenerator d_generator;
        
        mutable Rectf d_cachedData;
        mutable bool  d_cacheValid;
    };
    
    /*!
    \brief Constructor
    */
    Node();

    /*!
    \brief Destructor
    */
    virtual ~Node();

    /*!
    \brief Retrieves parent of this node, 0 means that this Node is a root of a tree it represents
    */
    inline Node* getParentNode() const
    {
        return d_parent;
    }

    /*!
    \brief
        Set the window area.

        Sets the area occupied by this window.  The defined area is offset from
        the top-left corner of this windows parent window or from the top-left
        corner of the display if this window has no parent (i.e. it is the root
        window).

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \param pos
        UVector2 describing the new position (top-left corner) of the window
        area.

    \param size
        UVector2 describing the new size of the window area.
     */
    virtual void setArea(const UVector2& pos, const USize& size);

    /*!
    \brief
        Set the window area.

        Sets the area occupied by this window.  The defined area is offset from
        the top-left corner of this windows parent window or from the top-left
        corner of the display if this window has no parent (i.e. it is the root
        window).

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \param xpos
        UDim describing the new x co-ordinate (left edge) of the window area.

    \param ypos
        UDim describing the new y co-ordinate (top-edge) of the window area.

    \param width
        UDim describing the new width of the window area.

    \param height
        UDim describing the new height of the window area.
     */
    inline void setArea(const UDim& xpos, const UDim& ypos, const UDim& width, const UDim& height)
    {
        setArea(UVector2(xpos, ypos), USize(width, height));
    }
    
    /*!
    \brief
        Set the window area.

        Sets the area occupied by this window.  The defined area is offset from
        the top-left corner of this windows parent window or from the top-left
        corner of the display if this window has no parent (i.e. it is the root
        window).

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \param area
        URect describing the new area rectangle of the window area.
     */
    inline void setArea(const URect& area)
    {
        setArea(area.d_min, area.getSize());
    }

    /*!
    \brief
        Return the windows area.

        Returns the area occupied by this window.  The defined area is offset
        from the top-left corner of this windows parent window or from the
        top-left corner of the display if this window has no parent
        (i.e. it is the root window).

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \return
        URect describing the rectangle of the window area.
     */
    inline const URect& getArea() const
    {
        return d_area;
    }

    /*!
    \brief
        Set the window's position.

        Sets the position of the area occupied by this window.  The position is
        offset from the top-left corner of this windows parent window or from
        the top-left corner of the display if this window has no parent
        (i.e. it is the root window).

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \param pos
        UVector2 describing the new position (top-left corner) of the window
        area.
     */
    inline void setPosition(const UVector2& pos)
    {
        setArea_impl(pos, d_area.getSize());
    }

    /*!
    \brief
        Get the window's position.

        Gets the position of the area occupied by this window.  The position is
        offset from the top-left corner of this windows parent window or from
        the top-left corner of the display if this window has no parent
        (i.e. it is the root window).

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \return
        UVector2 describing the position (top-left corner) of the window area.
     */
    inline const UVector2& getPosition() const
    {
        return d_area.getPosition();
    }
    
    /*!
    \brief
        Set the horizontal alignment.

        Modifies the horizontal alignment for the window.  This setting affects
        how the windows position is interpreted relative to its parent.

    \param alignment
        One of the HorizontalAlignment enumerated values.

    \return
        Nothing.
     */
    virtual void setHorizontalAlignment(const HorizontalAlignment alignment);
    
    /*!
    \brief
        Get the horizontal alignment.

        Returns the horizontal alignment for the window.  This setting affects
        how the windows position is interpreted relative to its parent.

    \return
        One of the HorizontalAlignment enumerated values.
     */
    inline HorizontalAlignment getHorizontalAlignment() const
    {
        return d_horizontalAlignment;
    }

    /*!
    \brief
        Set the vertical alignment.

        Modifies the vertical alignment for the window.  This setting affects
        how the windows position is interpreted relative to its parent.

    \param alignment
        One of the VerticalAlignment enumerated values.

    \return
        Nothing.
     */
    virtual void setVerticalAlignment(const VerticalAlignment alignment);
    
    /*!
    \brief
        Get the vertical alignment.

        Returns the vertical alignment for the window.  This setting affects how
        the windows position is interpreted relative to its parent.

    \return
        One of the VerticalAlignment enumerated values.
     */
    inline VerticalAlignment getVerticalAlignment() const
    {
        return d_verticalAlignment;
    }

    /*!
    \brief
        Set the window's size.

        Sets the size of the area occupied by this window.

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \param size
        USize describing the new size of the window area.
     */
    inline void setSize(const USize& size)
    {
        setArea(d_area.getPosition(), size);
    }

    /*!
    \brief
        Get the window's size.

        Gets the size of the area occupied by this window.

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \return
        USize describing the size of the window area.
     */
    inline USize getSize() const
    {
        return d_area.getSize();
    }

    /*!
    \brief
        Set the window's minimum size.

        Sets the minimum size that this windows area may occupy (whether size
        changes occur by user interaction, general system operation, or by
        direct setting by client code).

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \param size
        USize describing the new minimum size of the window area.
     */
    void setMinSize(const USize& size);
    
    /*!
    \brief
        Get the window's minimum size.

        Gets the minimum size that this windows area may occupy (whether size
        changes occur by user interaction, general system operation, or by
        direct setting by client code).

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \return
        UVector2 describing the minimum size of the window area.
     */
    inline const USize& getMinSize() const
    {
        return d_minSize;
    }

    /*!
    \brief
        Set the window's maximum size.

        Sets the maximum size that this windows area may occupy (whether size
        changes occur by user interaction, general system operation, or by
        direct setting by client code).

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \param size
        USize describing the new maximum size of the window area.
     */
    void setMaxSize(const USize& size);

    /*!
    \brief
        Get the window's maximum size.

        Gets the maximum size that this windows area may occupy (whether size
        changes occur by user interaction, general system operation, or by
        direct setting by client code).

    \note
        This method makes use of "Unified Dimensions".  These contain both
        parent relative and absolute pixel components, which are used in
        determining the final value used.

    \return
        UVector2 describing the maximum size of the window area.
     */
    inline const USize& getMaxSize() const
    {
        return d_maxSize;
    }

    /*!
    \brief
        Sets current aspect mode and recalculates the area rect

    \param
        mode the new aspect mode to set
    */
    void setAspectMode(const AspectMode mode);

    /*!
    \brief
        Retrieves currently used aspect mode
    */
    inline AspectMode getAspectMode() const
    {
        return d_aspectMode;
    }

    /*!
    \brief
        Sets target aspect ratio

    This is ignored if AspectMode is AM_IGNORE.
    */
    void setAspectRatio(const float ratio);

    /*!
    \brief
        Retrieves target aspect ratio

    \see Window::setAspectRatio
    */
    inline float getAspectRatio() const
    {
        return d_aspectRatio;
    }
    
    /*!
    \brief
        Return the window size in pixels.

    \return
        Size object describing this windows size in pixels.
    */
    inline Sizef getPixelSize() const
    {
        return d_pixelSize;
    }

    /*!
    \brief
        Return the pixel size of the parent element.  This always returns a
        valid object.

    \return
        Size object that describes the pixel dimensions of this Node object's parent
    */
    Sizef getParentPixelSize(void) const;

    /*!
    \brief sets rotation of this widget

    \param rotation
        A Quaternion describing the rotation

    CEGUI used Euler angles previously. Whilst this is easy to use and seems
    intuitive, it causes Gimbal locks when animating and is just the worse
    solution than using Quaternions. You can still use Euler angles, see
    the \a Quaternion class for more info about that.
    */
    void setRotation(const Quaternion& rotation);

    /*!
    \brief retrieves rotation of this widget

    \see Window::setRotation
    */
    inline const Quaternion& getRotation() const
    {
        return d_rotation;
    }

    /*!
    \brief
        Add the specified Node as a child of this Node. If the Node
        \a node is already attached to a different Node, it is detached
        before being added to this Node.

    \param node
        Pointer to the Node object to be added.

    \exception InvalidRequestException
        thrown if Node \a node is an ancestor of this Node, to prevent
        cyclic Node structures.
    */
    void addChild(Node* node);

    /*!
    \brief
        Remove the Node referenced by the given name path from this Node's
        child list.
        
    \see
        Node::addChild
    */
    void removeChild(Node* node);
    
    inline size_t getChildCount() const
    {
        return d_children.size();
    }

    bool isChild(const Node* node) const;

    bool isAncestor(const Node* node) const;
    
    /*!
    \brief
        Set whether the Window is a non-client window.

        A non-client window is clipped, positioned and sized according to the
        parent window's full area as opposed to just the inner rect area used
        for normal client windows.

    \param setting
        - true if the window should be clipped, positioned and sized according
        to the full area rectangle of it's parent.
        - false if the window should be clipped, positioned and sized according
        to the inner rect area of it's parent.
    */
    void setNonClient(const bool setting);
    
    inline bool isNonClient() const
    {
        return d_nonClient;
    }
    
    /*!
    \brief
        Return a Rect that describes the unclipped outer rect area of the Window
        in screen pixels.
    */
    const CachedRectf& getUnclippedOuterRect() const;

    /*!
    \brief
        Return a Rect object that describes, unclipped, the inner rectangle for
        this window.  The inner rectangle is typically an area that excludes
        some frame or other rendering that should not be touched by subsequent
        rendering.

    \return
        Rect object that describes, in unclipped screen pixel co-ordinates, the
        window object's inner rect area.
    */
    const CachedRectf& getUnclippedInnerRect() const;

    /*!
    \brief
        Return a Rect that describes the unclipped area covered by the Window.

        This function can return either the inner or outer area dependant upon
        the boolean values passed in.

    \param inner
        - true if the inner rect area should be returned.
        - false if the outer rect area should be returned.
    */
    const Rectf& getUnclippedRect(const bool inner) const;

    /*!
    \brief
        Return a Rect that describes the rendering clipping rect based upon the
        outer rect area of the window.

    \note
        The area returned by this function gives you the correct clipping rect
        for rendering within the Window's outer rect area.  The area described
        may or may not correspond to the final visual clipping actually seen on
        the display; this is intentional and neccessary due to the way that
        imagery is cached under some configurations.
    */
    const Rectf& getOuterRectClipper() const;

    /*!
    \brief
        Return a Rect that describes the rendering clipping rect based upon the
        inner rect area of the window.

    \note
        The area returned by this function gives you the correct clipping rect
        for rendering within the Window's inner rect area.  The area described
        may or may not correspond to the final visual clipping actually seen on
        the display; this is intentional and neccessary due to the way that
        imagery is cached under some configurations.
    */
    const Rectf& getInnerRectClipper() const;

    /*!
    \brief
        Return a Rect that describes the rendering clipping rect for the Window.

        This function can return the clipping rect for either the inner or outer
        area dependant upon the boolean values passed in.

    \note
        The areas returned by this function gives you the correct clipping rects
        for rendering within the Window's areas.  The area described may or may
        not correspond to the final visual clipping actually seen on the
        display; this is intentional and neccessary due to the way that imagery
        is cached under some configurations.

    \param non_client
        - true to return the non-client clipping area (based on outer rect).
        - false to return the client clipping area (based on inner rect).
    */
    const Rectf& getClipRect(const bool non_client = false) const;

    /*!
    \brief
        Return the Rect that descibes the clipped screen area that is used for
        determining whether this window has been hit by a certain point.

        The area returned by this function may also be useful for certain
        calculations that require the clipped Window area as seen on the display
        as opposed to what is used for rendering (since the actual rendering
        clipper rects should not to be used if reliable results are desired).
    */
    const Rectf& getHitTestRect() const;

    /*!
    \brief
        Return a Rect that describes the area that is used to position
        and - for scale values - size child content attached to this Window.

        By and large the area returned here will be the same as the unclipped
        inner rect (for client content) or the unclipped outer rect (for non
        client content), although certain advanced uses will require
        alternative Rects to be returned.

    \note
        The behaviour of this function is modified by overriding the
        protected Window::getClientChildWindowContentArea_impl and/or
        Window::getNonClientChildWindowContentArea_impl functions.

    \param non_client
        - true to return the non-client child content area.
        - false to return the client child content area (default).
    */
    Rectf getChildContentArea(const bool non_client = false) const;
    
    /*!
    \brief
        check if the given pixel position would hit this window.

    \param position
        Vector2 object describing the position to check.  The position
        describes a pixel offset from the top-left corner of the display.

    \param allow_disabled
        - true specifies that the window may be 'hit' if it is disabled.
        - false specifies that the window may only be hit if it is enabled.

    \return
        - true if \a position hits this Window.
        - false if \a position does not hit this window.
    */
    virtual bool isHit(const Vector2f& position,
                       const bool allow_disabled = false) const;

    /*!
    \brief
        return the child Window that is hit by the given pixel position

    \param position
        Vector2 object describing the position to check.  The position
        describes a pixel offset from the top-left corner of the display.

    \return
        Pointer to the child Window that was hit according to the location
        \a position, or 0 if no child of this window was hit.
    */
    Node* getChildNodeAtPosition(const Vector2f& position) const;

    //! return Vector2 \a pos after being fully unprojected for this Window.
    Vector2f getUnprojectedPosition(const Vector2f& pos) const;
    
    /*!
    \brief
        Inform the node, and optionally all children, that screen area
        rectangles have changed.

    \param recursive
        - true to recursively call notifyScreenAreaChanged on attached child
          Node objects.
        - false to just process \e this Node.
    */
    virtual void notifyScreenAreaChanged(bool recursive = true);

protected:
    /*!
    \brief
        Set the parent window for this window object.

    \param parent
        Pointer to a Window object that is to be assigned as the parent to this
        Window.

    \return
        Nothing
    */
    void setParent(Node* parent);

    /*!
    \brief
        Add given window to child list at an appropriate position
    */
    virtual void addChild_impl(Node* node);

    /*!
    \brief
        Remove given window from child list
    */
    virtual void removeChild_impl(Node* node);

    /*!
    \brief
        Add standard CEGUI::Node properties.
    */
    void addNodeProperties(void);

    /*!
    \brief
        Implementation method to modify window area while correctly applying
        min / max size processing, and firing any appropriate events.

    /note
        This is the implementation function for setting size and position.
        In order to simplify area management, from this point on, all
        modifications to window size and position (area rect) should come
        through here.

    /param pos
        UVector2 object describing the new area position.

    /param size
        USize object describing the new area size.

    /param topLeftSizing
        - true to indicate the the operation is a sizing operation on the top
          and/or left edges of the area, and so window movement should be
          inhibited if size is at max or min.
        - false to indicate the operation is not a strict sizing operation on
          the top and/or left edges and that the window position may change as
          required

    /param fireEvents
        - true if events should be fired as normal.
        - false to inhibit firing of events (required, for example, if you need
          to call this from the onSize/onMove handlers).
     */
    void setArea_impl(const UVector2& pos, const USize& size,
                      bool topLeftSizing = false, bool fireEvents = true);

    //! helper to return whether the inner rect size has changed
    inline bool isInnerRectSizeChanged() const
    {
        const Sizef old_sz(d_unclippedInnerRect.get().getSize());
        d_unclippedInnerRect.invalidateCache();
        return old_sz != d_unclippedInnerRect.get().getSize();
    }
    
    //! Default implementation of function to return Window outer rect area.
    virtual Rectf getUnclippedOuterRect_impl(bool noPixelAlignment = false) const;
    //! Default implementation of function to return Window inner rect area.
    virtual Rectf getUnclippedInnerRect_impl(bool noPixelAlignment = false) const;
    //! Default implementation of function to return Window outer clipper area.
    virtual Rectf getOuterRectClipper_impl() const;
    //! Default implementation of function to return Window inner clipper area.
    virtual Rectf getInnerRectClipper_impl() const;
    //! Default implementation of function to return Window hit-test area.
    virtual Rectf getHitTestRect_impl() const;
    //! Default implementation of function to return non-client content area
    virtual Rectf getNonClientChildContentArea_impl() const;
    //! Default implementation of function to return client content area
    virtual Rectf getClientChildContentArea_impl() const;

    // constrain given USize to window's min size, return if size changed.
    bool constrainToMinSize(const Sizef& base_sz, USize& sz) const;
    // constrain given USize to window's max size, return if size changed.
    bool constrainToMaxSize(const Sizef& base_sz, USize& sz) const;
    
    /*************************************************************************
        Event trigger methods
    *************************************************************************/
    /*!
    \brief
        Handler called when the node's size changes.

    \param e
        NodeEventArgs object whose 'node' pointer field is set to the node
        that triggered the event.
    */
    virtual void onSized(NodeEventArgs& e);
    
    /*!
    \brief
        Handler called when this node's parent node has been resized.  If
        this node is the root / GUI Sheet window, this call will be made when
        the display size changes.

    \param e
        NodeEventArgs object whose 'node' pointer field is set the the
        node that caused the event; this is typically either this node's
        parent window, or NULL to indicate the screen size has changed.
    */
    virtual void onParentSized(NodeEventArgs& e);

    /*!
    \brief
        Handler called when the node's position changes.

    \param e
        WindowEventArgs object whose 'node' pointer field is set to the node
        that triggered the event.
    */
    virtual void onMoved(NodeEventArgs& e);

    /*!
    \brief
        Handler called when the horizontal alignment setting for the window is
        changed.

    \param e
        NodeEventArgs object initialised as follows:
        - node field is set to point to the Node object whos alignment has
          changed (typically 'this').
    */
    virtual void onHorizontalAlignmentChanged(NodeEventArgs& e);
    
    /*!
    \brief
        Handler called when the vertical alignment setting for the node is
        changed.

    \param e
        NodeEventArgs object initialised as follows:
        - node field is set to point to the Node object whos alignment has
          changed (typically 'this').
    */
    virtual void onVerticalAlignmentChanged(NodeEventArgs& e);
    
    /*************************************************************************
        Implementation Data
    *************************************************************************/
    //! definition of type used for the list of attached child windows.
    typedef std::vector<Node*
        CEGUI_VECTOR_ALLOC(Node*)> ChildList;

    //! The list of child Window objects attached to this.
    ChildList d_children;
    //! Holds pointer to the parent window.
    Node* d_parent;
    
    //! true if Window is in non-client (outside InnerRect) area of parent.
    bool d_nonClient;
    
    //! This Window objects area as defined by a URect.
    URect d_area;
    //! Specifies the base for horizontal alignment.
    HorizontalAlignment d_horizontalAlignment;
    //! Specifies the base for vertical alignment.
    VerticalAlignment d_verticalAlignment;
    //! current minimum size for the window.
    USize d_minSize;
    //! current maximum size for the window.
    USize d_maxSize;
    //! How to satisfy current aspect ratio
    AspectMode d_aspectMode;
    //! The target aspect ratio
    float d_aspectRatio;
    //! Current constrained pixel size of the window.
    Sizef d_pixelSize;
    //! Rotation of this window (relative to the parent)
    Quaternion d_rotation;
    
    //! outer area rect in screen pixels
    CachedRectf d_unclippedOuterRect;
    //! inner area rect in screen pixels
    CachedRectf d_unclippedInnerRect;
    
    //! outer area clipping rect in screen pixels
    mutable Rectf d_outerRectClipper;
    //! inner area clipping rect in screen pixels
    mutable Rectf d_innerRectClipper;
    //! area rect used for hit-testing against this window
    mutable Rectf d_hitTestRect;

    mutable bool d_outerRectClipperValid;
    mutable bool d_innerRectClipperValid;
    mutable bool d_hitTestRectValid;

private:
    /*************************************************************************
        May not copy or assign Node objects
    *************************************************************************/
    Node(const Node&);
    
    Node& operator=(const Node&) {return *this;}
};

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUINode_h_

