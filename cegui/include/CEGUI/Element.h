/***********************************************************************
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

#ifndef _CEGUIElement_h_
#define _CEGUIElement_h_

#include "CEGUI/Base.h"
#include "CEGUI/PropertySet.h"
#include "CEGUI/EventSet.h"
#include "CEGUI/EventArgs.h"
#include "CEGUI/DefaultParagraphDirection.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{
/*!
\brief
    EventArgs based class that is used for objects passed to handlers triggered
    for events concerning some Element object.

\see CEGUI::Element
*/
class CEGUIEXPORT ElementEventArgs : public EventArgs
{
public:
    ElementEventArgs(Element* element):
        element(element)
    {}

    //! pointer to an Element object of relevance to the event.
    Element* element;
};

/*!
\brief A positioned and sized rectangular node in a tree graph

This class implements positioning, alignment, sizing including minimum and
maximum size constraining. In its bare essense it's an unnamed rectangular node
that may contain other unnamed rectangular nodes.

Unless you are implementing new CEGUI functionality you do NOT want to use this
class directly. You most likely want to use CEGUI::Window.

\see CEGUI::Window

\internal
    Currently only CEGUI::Window uses this but in the future Falagard might use
    it for all widget parts, this would unify much of currently repeated code.

\internal
    Methods retrieving Element (like getParentElement) have Element suffix so
    that deriving classes can easily make their getParent and return the proper
    casted type (Window* for example).
*/
class CEGUIEXPORT Element :
    public PropertySet,
    public EventSet
{
public:
    //! Namespace for global events
    static const String EventNamespace;

    /** Event fired when the Element size has changed.
     * Handlers are passed a const ElementEventArgs reference with
     * ElementEventArgs::element set to the Element whose size was changed.
     */
    static const String EventSized;
    /** Event fired when the parent of this Element has been re-sized.
     * Handlers are passed a const ElementEventArgs reference with
     * ElementEventArgs::element pointing to the <em>parent element</em> that
     * was resized, not the element whose parent was resized.
     */
    static const String EventParentSized;
    /** Event fired when the Element position has changed.
     * Handlers are passed a const ElementEventArgs reference with
     * ElementEventArgs::element set to the Element whose position was changed.
     */
    static const String EventMoved;
    /** Event fired when the horizontal alignment for the element is changed.
     * Handlers are passed a const ElementEventArgs reference with
     * ElementEventArgs::element set to the Element whose horizontal alignment
     * setting was changed.
     */
    static const String EventHorizontalAlignmentChanged;
    /** Event fired when the vertical alignment for the element is changed.
     * Handlers are passed a const ElementEventArgs reference with
     * ElementEventArgs::element set to the Element whose vertical alignment
     * setting was changed.
     */
    static const String EventVerticalAlignmentChanged;
    /** Event fired when the rotation factor(s) for the element are changed.
     * Handlers are passed a const ElementEventArgs reference with
     * ElementEventArgs::element set to the Element whose rotation was changed.
     */
    static const String EventRotated;
    /** Event fired when a child Element has been added.
     * Handlers are passed a const ElementEventArgs reference with
     * ElementEventArgs::element set to the child element that was added.
     */
    static const String EventChildAdded;
    /** Event fired when a child element has been removed.
     * Handlers are passed a const ElementEventArgs reference with
     * ElementEventArgs::element set to the child element that was removed.
     */
    static const String EventChildRemoved;
    /** Event fired when the z-order of the element has changed.
     * Handlers are passed a const ElementEventArgs reference with
     * ElementEventArgs::element set to the Element whose z order position has
     * changed.
     */
    static const String EventZOrderChanged;
    /** Event fired when the non-client setting for the Element is changed.
     * Handlers are passed a const ElementEventArgs reference with
     * ElementEventArgs::element set to the Element whose non-client setting was
     * changed.
     */
    static const String EventNonClientChanged;
    /*!
    \brief
        Fired when any of the properties "AdjustWidthToContent" and
        "AdjustHeightToContent" changes.
    */
    static const String EventIsSizeAdjustedToContentChanged;

    /*!
    \brief
        Fired when the "DefaultParagraphDirection" changes.
    */
    static const String EventDefaultParagraphDirectionChanged;

    /*!
    \brief A tiny wrapper to hide some of the dirty work of rect caching

    This is used internally by CEGUI::Element and other classes, it is passed
    to the user in several methods. In those circumstances you most likely
    want the result of either the "get()" or "getFresh(..)" methods.
    */
    class CachedRectf
    {
    public:
        /*!
        \brief Function to generate fresh data that might later be cached

        If the bool is true all PixelAlignment settings will be overridden
        and no pixel alignment will take place.
        */
        typedef Rectf (Element::*DataGenerator)(bool) const;

        CachedRectf(Element const* element, DataGenerator generator):
            d_element(element),
            d_generator(generator),
            // we don't have to initialise d_cachedData here, it will get
            // regenerated and reset anyways
            d_cacheValid(false)
        {}

        /*!
        \brief Retrieves cached Rectf or generated a fresh one and caches it
        */
        inline const Rectf& get() const
        {
            if (!d_cacheValid)
            {
                regenerateCache();
            }

            return d_cachedData;
        }

        /*!
        \brief Skips all caching and calls the generator

        This method will cache the result if cache is invalid and
        alignment is not being skipped.
        */
        inline Rectf getFresh(bool skipAllPixelAlignment = false) const
        {
            // if the cache is not valid we will use this chance to regenerate it
            // of course this is only applicable if we are allowed to use pixel alignment where applicable
            if (!d_cacheValid && !skipAllPixelAlignment)
            {
                return get();
            }

            return ((*d_element).*d_generator)(skipAllPixelAlignment);
        }

        /*!
        \brief Invalidates the cached Rectf causing it to be regenerated

        The regeneration will not happen immediately, it will happen when user
        requests the data.
        */
        inline void invalidateCache() const
        {
            d_cacheValid = false;
        }

        inline bool isCacheValid() const
        {
            return d_cacheValid;
        }

        inline void regenerateCache() const
        {
            // false, since when we are caching we don't want to skip anything, we want everything to act
            // exactly as it was setup
            d_cachedData = (*d_element.*d_generator)(false);

            d_cacheValid = true;
        }

    private:
        Element const* d_element;
        const DataGenerator d_generator;

        mutable Rectf d_cachedData;
        mutable bool  d_cacheValid;
    };

    /*!
    \brief Constructor
    */
    Element();

    /*!
    \brief Destructor
    */
    virtual ~Element();

    /*!
    \brief Retrieves parent of this element
   
    \returns
        pointer to parent or 0, 0 means that this Element is a root of
        the subtree it represents
    */
    inline Element* getParentElement() const
    {
        return d_parent;
    }

    /*!
    \brief
        Set the Element area.

        Sets the area occupied by this Element. The defined area is offset from
        one of the corners and edges of this Element's parent element (depending on alignments)
        or from the top-left corner of the display if this element has no parent
        (i.e. if it is the root element).

    \param pos
        UVector2 describing the new position of the element area. Meaning of
        position depends on currently set alignments. By default it is the
        offset from the top-left corner of widget's parent.

    \param size
        UVector2 describing the new size of the element area.

    \param adjust_size_to_content
        If the size actually changes, should we call "AdjustSizeToContent"?
        Normally, this should be true. However, if this function is called from
        inside "AdjustSizeToContent", you must set this to false to prevent
        infinite recursion.

    \see UDim
    */
    virtual void setArea(const UVector2& pos, const USize& size, bool adjust_size_to_content=true);

    //! \overload
    inline void setArea(const UDim& xpos, const UDim& ypos,
                        const UDim& width, const UDim& height)
    {
        setArea(UVector2(xpos, ypos), USize(width, height));
    }

    //! \overload
    inline void setArea(const URect& area)
    {
        setArea(area.d_min, area.getSize());
    }

    /*!
    \brief
        Return the element's area.

        Sets the area occupied by this Element. The defined area is offset from
        one of the corners and edges of this Element's parent element (depending on alignments)
        or from the top-left corner of the display if this element has no parent
        (i.e. it is the root element).

    \return
        URect describing the rectangle of the element area.

    \see UDim
    */
    inline const URect& getArea() const
    {
        return d_area;
    }

    /*!
    \brief
        Set the element's position.

        Sets the position of the area occupied by this element. The position is offset from
        one of the corners and edges of this Element's parent element (depending on alignments)
        or from the top-left corner of the display if this element has no parent
        (i.e. it is the root element).

    \param pos
        UVector2 describing the new position of the element area.

    \see UDim
    \see Element::setArea(const UVector2& pos, const USize& size)
    */
    inline void setPosition(const UVector2& pos)
    {
        setArea_impl(pos, getSize());
    }

    //! \overload
    inline void setXPosition(const UDim& pos)
    {
        setPosition(UVector2(pos, getYPosition()));
    }

    //! \overload
    inline void setYPosition(const UDim& pos)
    {
        setPosition(UVector2(getXPosition(), pos));
    }

    /*!
    \brief
        Get the element's position.

        Sets the position of the area occupied by this element. The position is offset from
        one of the corners of this Element's parent element (depending on alignments)
        or from the top-left corner of the display if this element has no parent
        (i.e. it is the root element).

    \return
        UVector2 describing the position of the element area.

    \see UDim
    */
    inline const UVector2& getPosition() const
    {
        return d_area.getPosition();
    }

    //! \overload
    inline const UDim& getXPosition() const
    {
        return getPosition().d_x;
    }

    //! \overload
    inline const UDim& getYPosition() const
    {
        return getPosition().d_y;
    }

    /*!
    \brief
        Set the horizontal alignment.

        Modifies the horizontal alignment for the element. This setting affects
        how the element's position is interpreted relative to its parent.

    \param alignment
        One of the HorizontalAlignment enumerated values.
     */
    virtual void setHorizontalAlignment(const HorizontalAlignment alignment);

    /*!
    \brief
        Get the horizontal alignment.

        Returns the horizontal alignment for the element. This setting affects
        how the element's position is interpreted relative to its parent.

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

        Modifies the vertical alignment for the element. This setting affects
        how the element's position is interpreted relative to its parent.

    \param alignment
        One of the VerticalAlignment enumerated values.
     */
    virtual void setVerticalAlignment(const VerticalAlignment alignment);

    /*!
    \brief
        Get the vertical alignment.

        Returns the vertical alignment for the element.  This setting affects how
        the element's position is interpreted relative to its parent.

    \return
        One of the VerticalAlignment enumerated values.
     */
    inline VerticalAlignment getVerticalAlignment() const
    {
        return d_verticalAlignment;
    }

    /*!
    \brief
        Set the element's size.

        Sets the size of the area occupied by this element.

    \param size
        USize describing the new size of the element's area.

    \see UDim
    */
    inline void setSize(const USize& size)
    {
        setSize(size, true);
    }

    /*!
    \brief
        Set the element's size.

        Sets the size of the area occupied by this element.

    \param size
        USize describing the new size of the element's area.

    \param adjust_size_to_content
        If the size actually changes, should we call "AdjustSizeToContent"?
        Normally, this should be true. However, if this function is called from
        inside "AdjustSizeToContent", you must set this to false to prevent
        infinite recursion.

    \see UDim
    */
    inline void setSize(const USize& size, bool adjust_size_to_content)
    {
        setArea(getPosition(), size, adjust_size_to_content);
    }

    //! \overload
    inline void setWidth(const UDim& width)
    {
        setSize(USize(width, getSize().d_height));
    }

    //! \overload
    inline void setHeight(const UDim& height)
    {
        setSize(USize(getSize().d_width, height));
    }

    /*!
    \brief
        Get the element's size.

        Gets the size of the area occupied by this element.

    \return
        USize describing the size of the element's area.

    \see UDim
    */
    inline USize getSize() const
    {
        return d_area.getSize();
    }

    //! \overload
    inline UDim getWidth() const
    {
        return getSize().d_width;
    }

    //! \overload
    inline UDim getHeight() const
    {
        return getSize().d_height;
    }

    /*!
    \brief
        Set the element's minimum size.

        Sets the minimum size that this element's area may occupy (whether size
        changes occur by user interaction, general system operation, or by
        direct setting by client code).

    \note
        The scale component of UDim takes display size as the base.
        It is not dependent on parent element's size!

    \param size
        USize describing the new minimum size of the element's area.

    \see Element::setSize
    */
    void setMinSize(const USize& size);

    /*!
    \brief
        Get the element's minimum size.

        Gets the minimum size that this element's area may occupy (whether size
        changes occur by user interaction, general system operation, or by
        direct setting by client code).

    \return
        UVector2 describing the minimum size of the element's area.

    \see Element::setMinSize
    */
    inline const USize& getMinSize() const
    {
        return d_minSize;
    }

    /*!
    \brief
        Set the element's maximum size.

        Sets the maximum size that this element area may occupy (whether size
        changes occur by user interaction, general system operation, or by
        direct setting by client code).

    \note
        The scale component of UDim takes display size as the base.
        It is not dependent on parent element's size!

    \param size
        USize describing the new maximum size of the element's area.  Note that
        zero is used to indicate that the Element's maximum area size will be
        unbounded.

    \see Element::setSize
    */
    void setMaxSize(const USize& size);

    /*!
    \brief
        Get the element's maximum size.

        Gets the maximum size that this element area may occupy (whether size
        changes occur by user interaction, general system operation, or by
        direct setting by client code).

    \return
        UVector2 describing the maximum size of the element's area.

    \see Element::setMaxSize
     */
    inline const USize& getMaxSize() const
    {
        return d_maxSize;
    }

    /*!
    \brief Sets current aspect mode and recalculates the area rect

    \param mode the new aspect mode to set

    \see CEGUI::AspectMode
    \see CEGUI::setAspectRatio
    */
    void setAspectMode(const AspectMode mode);

    /*!
    \brief
        Retrieves currently used aspect mode

    \see Element::setAspectMode
    */
    inline AspectMode getAspectMode() const
    {
        return d_aspectMode;
    }

    /*!
    \brief
        Sets target aspect ratio

    \param ratio
        The desired ratio as width / height. For example 4.0f / 3.0f,
        16.0f / 9.0.f, ...

    \note
        This is ignored if AspectMode is IGNORE.

    \see Element::setAspectMode
    */
    void setAspectRatio(const float ratio);

    /*!
    \brief
        Retrieves target aspect ratio

    \see Element::setAspectRatio
    */
    inline float getAspectRatio() const
    {
        return d_aspectRatio;
    }

    /*!
    \brief
        Sets whether this Element is pixel aligned (both position and size, basically the 4 "corners").
        
    \par Impact on the element tree
        Lets say we have Element A with child Element B, A is pixel aligned
        and it's position is 99.5, 99.5 px in screenspace. This gives us
        100, 100 px pixel aligned position.

        B's position is always relative to the pixel-aligned position of its
        parent. Say B isn't pixel-aligned and it's position is 0.5, 0.5 px.
        Its final position will be 100.5, 100.5 px in screenspace, not 100, 100 px!

        If it were pixel-aligned the final position would be 101, 101 px.

    \par Why you should pixel-align widgets
        Pixel aligning is enabled by default and for most widgets it makes
        a lot of sense and just looks better. Especially with text. However for
        HUD or decorative elements pixel aligning might make transitions less
        fluid. Feel free to experiment with the setting.
    */
    void setPixelAligned(const bool setting);

    /*!
    \brief
        Checks whether this Element is pixel aligned
        
    \see
        Element::setPixelAligned
    */
    inline bool isPixelAligned() const
    {
        return d_pixelAligned;
    }

    /*!
    \brief
        Return the element's absolute (or screen, depending on the type of the element) position in pixels.

    \return
        glm::vec2 object describing this element's absolute position in pixels.
    */
    inline glm::vec2 getPixelPosition() const
    {
        return getUnclippedOuterRect().get().d_min;
    }

    /*!
    \brief
        Return the element's size in pixels.

    \return
        Size object describing this element's size in pixels.
    */
    inline const Sizef& getPixelSize() const
    {
        return d_pixelSize;
    }

    /*!
    \brief Calculates this element's pixel size

    \param skipAllPixelAlignment 
        Should all pixel-alignment be skipped when calculating the pixel size?

    If you want to get the pixel size you most probably want to use the
    Element::getPixelSize method. This method skips caching and might
    impact performance!
    */
    Sizef calculatePixelSize(bool skipAllPixelAlignment = false) const;

    /*!
    \brief Return the pixel size of the parent element.

    If this element doesn't have any parent, the display size will be returned.
    This method returns a valid Sizef object in all cases.

    \return
        Size object that describes the pixel dimensions of this Element's parent
    */
    Sizef getParentPixelSize(bool skipAllPixelAlignment = false) const;

    /*!
    \brief sets rotation of this widget

    \param rotation
        A Quaternion describing the rotation

    \par Euler angles
        CEGUI used Euler angles previously. While these are easy to use and seem
        intuitive they cause Gimbal locks when animating and are overall the worse
        solution than using Quaternions. You can still use Euler angles, see
        the glm::quat class for more info about that.

    \see Element::getRotation
    */
    void setRotation(const glm::quat& rotation);

    /*!
    \brief retrieves rotation of this widget

    \see Element::setRotation
    */
    inline const glm::quat& getRotation() const
    {
        return d_rotation;
    }

    /*!
    \brief
        Retrieves the pivot point (the point around which the widget is rotated).

        The point is defined in 3D so you can also rotate around the Z axis.

        The point is defined with "UDim"-s, so you can use relative dimensions. They're relative to the size of the widget. The depth of the widget is currently always 0.

        The default is the widget's center.

    \see Element::setPivot
    \see Element::getRotation
    */
    UVector3 getPivot() const;

    /*!
    \brief
        Sets the pivot point (the point around which the widget is rotated).

    \see Element::getPivot
    \see Element::setRotation
    */
    void setPivot(const UVector3& pivot);

    /*!
    \brief
        Add the specified Element as a child of this Element.

    If the Element \a element is already attached to a different Element,
    it is detached before being added to this Element.

    \param element
        Pointer to the Element object to be added.

    \exception InvalidRequestException
        thrown if Element \a element is NULL.

    \exception InvalidRequestException
        thrown if Element \a element is "this" element

    \exception InvalidRequestException
        thrown if Element \a element is an ancestor of this Element, to prevent
        cyclic Element structures.
    */
    void addChild(Element* element);

    /*!
    \brief
        Remove the Element Element's child list.
        
    \exception InvalidRequestException
        thrown if Element \a element is NULL.

    \see
        Element::addChild
    */
    void removeChild(Element* element);

    /*!
    \brief
        return a pointer to the child element that is attached to 'this' at the
        given index.

    \param idx
        Index of the child element who's pointer should be returned.  This value
        is not bounds checked, client code should ensure that this is less than
        the value returned by getChildCount().

    \return
        Pointer to the child element currently attached at index position \a idx
    */
    inline Element* getChildElementAtIdx(size_t idx) const
    {
        return d_children[idx];
    }

    /*!
    \brief Returns number of child elements attached to this Element
    */
    inline size_t getChildCount() const
    {
        return d_children.size();
    }

    /*!
    \brief Checks whether given element is attached to this Element
    */
    bool isChild(const Element* element) const;

    /*!
    \brief Checks whether the specified Element is an ancestor of this Element

    \param element
        Pointer to the Element object to look for.

    This element itself is not its own ancestor!

    \return
        - true if \a element was found to be an ancestor (parent, or parent of
          parent, etc) of this Element.
        - false if \a element is not an ancestor of this element.
    */
    bool isAncestor(const Element* element) const;

    /*!
    \brief Set whether the Element is non-client.

    A non-client element is clipped, positioned and sized according to the
    parent element's full area as opposed to just the inner rect area used
    for normal client element.

    \param setting
        - true if the element should be clipped, positioned and sized according
        to the full area rectangle of it's parent.
        - false if the element should be clipped, positioned and sized according
        to the inner rect area of it's parent.
    */
    void setNonClient(const bool setting);

    /*!
    \brief Checks whether this element was set to be non client

    \see Element::setNonClient
    */
    inline bool isNonClient() const
    {
        return d_nonClient;
    }

    /*!
    \brief
        If set to "true", keep the width of the element at the minimal value in
        which the whole element content is visible without the
        need for a horizontal scrollbar (if possible), and while the content
        remains "intact" (if possible).
        
        See the documentation of "isWidthAdjustedToContent" for more details.

    \see isWidthAdjustedToContent
    \see setAdjustHeightToContent
    */
    void setAdjustWidthToContent(bool value);

    /*!
    \brief
        If set to "true", keep the height of the element at the minimal value in
        which the whole element content is visible without the
        need for a vertical scrollbar (if possible), and while the content
        remains "intact" (if possible).
        
        See the documentation of "isHeightAdjustedToContent" for more details.

    \see isHeightAdjustedToContent
    \see setAdjustWidthToContent
    */
    void setAdjustHeightToContent(bool value);

    /*!
    \brief
        If set to "true", keep the width of the element at the minimal value in
        which the whole element content is visible without the
        need for a horizontal scrollbar (if possible), and while the content
        remains "intact" (if possible).

        The meaning of "element content" and "intact" depends on the type of
        element. For example, for a "FalagardStaticText" widget, the content is
        the text, and "intact" means no single word is split between 2 or more
        lines. So setting this property to "true" would keep the width of the
        widget to the minimal value in which the whole text is visible without
        the need for a horizontal scrollbar (if possible), and without the need
        to split a word between 2 or more lines (if possible).

        Whenever the size of the widget should be (re)adjusted to its content,
        this is done by calling "adjustSizeToContent". This happens in the
        default implementations of "onIsSizeAdjustedToContentChanged" and
        "onSized".

    \see adjustSizeToContent
    \see isHeightAdjustedToContent
    \see setAdjustWidthToContent
    \see isSizeAdjustedToContent
    \see onIsSizeAdjustedToContentChanged
    \see onSized
    */
    bool isWidthAdjustedToContent() const;

    /*!
    \brief
        If set to "true", keep the height of the element at the minimal value in
        which the whole element content is visible without the
        need for a vertical scrollbar (if possible), and while the content
        remains "intact" (if possible).

        The meaning of "element content" and "intact" depends on the type of
        element. For example, for a "FalagardStaticText" widget, the content is
        the text, and "intact" means no single word is split between 2 or more
        lines. So setting this property to "true" would keep the height of the
        widget to the minimal value in which the whole text is visible without
        the need for a vertical scrollbar (if possible), and without the need
        to split a word between 2 or more lines (if possible).

        Whenever the size of the widget should be (re)adjusted to its content,
        this is done by calling "adjustSizeToContent". This happens in the
        default implementations of "onIsSizeAdjustedToContentChanged" and
        "onSized".

    \see adjustSizeToContent
    \see isWidthAdjustedToContent
    \see setAdjustHeightToContent
    \see isSizeAdjustedToContent
    \see onContentSizeChanged
    \see onIsSizeAdjustedToContentChanged
    \see onSized
    */
    bool isHeightAdjustedToContent() const;

    /*!
    \brief
        Return whether any of the properties "AdjustWidthToContent" and
        "AdjustHeightToContent" is set to true.

    \see isWidthAdjustedToContent
    \see isHeightAdjustedToContent
    */
    bool isSizeAdjustedToContent() const;

    /*!
    \brief Return a Rect that describes the unclipped outer rect area of the Element

    The unclipped outer rectangle is the entire area of the element, including
    frames and other outside decorations.

    \note
        Unclipped in this context means not limited by any ancestor Element's area.

    \note
        If you take position of the result rectangle it is the same as pixel
        position of the Element in screenspace.
    */
    inline const CachedRectf& getUnclippedOuterRect() const
    {
        return d_unclippedOuterRect;
    }

    /*!
    \brief Return a Rect that describes the unclipped inner rect area of the Element

    The inner rectangle is typically an area that excludes some frame or other decorations
    that should not be touched by rendering of client clipped child elements.

    \note
        Unclipped in this context means not limited by any ancestor Element's area.

    \return
        Rect object that describes, in unclipped screen pixel co-ordinates, the
        element object's inner rect area.
    */
    inline const CachedRectf& getUnclippedInnerRect() const
    {
        return d_unclippedInnerRect;
    }

    /*!
    \brief Return a Rect that describes the unclipped area covered by the Element.

    This function can return either the inner or outer area dependant upon
    the boolean values passed in.

    \param inner
        - true if the inner rect area should be returned.
        - false if the outer rect area should be returned.

    \see Element::getUnclippedOuterRect
    \see Element::getUnclippedInnerRect
    */
    inline const CachedRectf& getUnclippedRect(const bool inner) const
    {
        return inner ? getUnclippedInnerRect() : getUnclippedOuterRect();
    }

    /*!
    \brief Return a Rect that is used by client child elements as content area

    Client content area is used for relative sizing, positioning and clipping
    of child elements that are client (their NonClient property is "false").

    \see Element::getChildContentArea
    */
    virtual const CachedRectf& getClientChildContentArea() const;

    /*!
    \brief Return a Rect that is used by client child elements as content area

    Client content area is used for relative sizing, positioning and clipping
    of child elements that are non-client (their NonClient property is "true").

    \see Element::getChildContentArea
    */
    virtual const CachedRectf& getNonClientChildContentArea() const;

    /*!
    \brief Return a Rect that is used to position and size child elements

    It is used as the reference area for positioning and its size is used for
    the scale components of position and size.
 
    \note
        By and large the area returned here will be the same as the unclipped
        inner rect (for client content) or the unclipped outer rect (for non
        client content), although certain advanced uses will require
        alternative Rects to be returned.

    \note
        The behaviour of this function is modified by overriding the
        protected Element::getClientChildContentArea and/or
        Element::getNonClientChildContentArea functions.

    \param non_client
        - true to return the non-client child content area.
        - false to return the client child content area (default).
    */
    inline const CachedRectf& getChildContentArea(const bool non_client = false) const
    {
        return non_client ? getNonClientChildContentArea() : getClientChildContentArea();
    }

    /*!
    \brief Inform the element and (optionally) all children that screen area has changed

    \note
        This will cause recomputation and recaching of various rectangles used.
        Such an action, especially if applied recursively, will impact performance
        before everything is cached again.

    \param recursive
        - true to recursively call notifyScreenAreaChanged on attached child
          Element objects.
        - false to just process \e this Element.
    */
    virtual void notifyScreenAreaChanged(bool recursive = true);

    /*!
    \brief Return the size of the root container (such as screen size).

    This is size of the hypothetical parent of the root element that has no
    parent element. Display size is usually used.

    The value is significant and is used to size and position the root if
    it is using scale UDim component in position and/or size.
    */
    virtual const Sizef& getRootContainerSize() const;

    /*!
    \brief
        Set the size of the element to the minimal value in which the whole
        element content is visible without the need for scrollbars
        (if possible), and while the content remains "intact" (if possible).

        The meaning of "element content" and "intact" depends on the type of
        element. For example, for a "FalagardStaticText" widget, the content is
        the text, and "intact" means no single word is split between 2 or more
        lines. So setting this property to "true" would keep the width of the
        widget to the minimal value in which the whole text is visible without
        the need for scrollbars (if possible), and without the need to split a
        word between 2 or more lines (if possible).

        If the "AdjustWidthToContent" property is set to "true", the width is
        adjusted. If the "AdjustHeightToContent" property is set to "true", the
        height is adjusted. The exact behaviour of this method depends on the
        combination of "AdjustWidthToContent" and "AdjustHeightToContent", as
        well as on the type of widget, and possibly other properties. You might
        want to look at the documentation for
        "FalagardStaticText::adjustSizeToContent" for a detailed description on
        how this method behaves for a "FalagardStaticText" widget, which serves
        as a great example.
    
        There are 2 helper methods that you may want to use when implementing
        this method:
        1. "adjustSizeToContent_direct"
        2. "getSizeAdjustedToContent_bisection"

        The default implementations of "onIsSizeAdjustedToContentChanged" and
        "onSized" call this method. Make sure you call this method whenever the
        size should be re-adjusted to the content in any other case.
    
        The default implementation calls "adjustSizeToContent_direct".

    \see FalagardStaticText::adjustSizeToContent
    \see adjustSizeToContent_direct
    \see getSizeAdjustedToContent_bisection
    \see isWidthAdjustedToContent
    \see isHeightAdjustedToContent
    \see isSizeAdjustedToContent
    */
    virtual void adjustSizeToContent();

    /*!
    \brief
        Get the width of the content of the element.

        The meaning of "content" depends on the type of element. For instance,
        the content of a "FalagardStaticText" widget is its text.
        This method is used by "adjustSizeToContent_direct".
    */
    virtual float getContentWidth() const;

    /*!
    \brief
        Get the height of the content of the element.

        The meaning of "content" depends on the type of element. For instance,
        the content of a "FalagardStaticText" widget is its text.

        This method is used by "adjustSizeToContent_direct".
    */
    virtual float getContentHeight() const;
    
    /*!
    \brief
        Get a lower bound for the width of the area of the element which is
        reserved for content as an affine function of the element width.

        An affine function means: "t" is an affine function of "m" if, for some
        constants "a" and "b", it holds true that "t = a*m +b".

        The meaning of "content" depends on the type of element. For instance,
        the content of a "FalagardStaticText" widget is its text. The area
        reserved for the content would then be the area reserved for the text,
        which is defined by the Look'N'Feel, and usually equals the size of the
        widget minus the size of the frame and scrollbars.

        This method is used by "adjustSizeToContent".

    \return
        Let:
            - "ret" be the value returned by this method.
            - "t" be the width of the area of the element which is reserved for
               content.
            - "m" be the element width.
        Then the following applies:
            t >= ret.d_scale*m + ret.d_offset

    \see adjustSizeToContent
    \see getHeightOfAreaReservedForContentLowerBoundAsFuncOfElementHeight
    \see getContentWidth
    */
    virtual UDim getWidthOfAreaReservedForContentLowerBoundAsFuncOfElementWidth() const;
    
    /*!
    \brief
        Get a lower bound for the height of the area of the element which is
        reserved for content as an affine function of the element height.

        An affine function means: "t" is an affine function of "m" if, for some
        constants "a" and "b", it holds true that "t = a*m +b".

        The meaning of "content" depends on the type of element. For instance,
        the content of a "FalagardStaticText" widget is its text. The area
        reserved for the content would then be the area reserved for the text,
        which is defined by the Look'N'Feel, and usually equals the size of the
        widget minus the size of the frame and scrollbars.

        This method is used by "adjustSizeToContent".

    \return
        Let:
            - "ret" be the value returned by this method.
            - "t" be the height of the area of the element which is reserved for
              content.
            - "m" be the element height.
        Then the following applies:
            t >= ret.d_scale*m + ret.d_offset

    \see adjustSizeToContent
    \see getWidthOfAreaReservedForContentLowerBoundAsFuncOfElementWidth
    \see getContentHeight
    */
    virtual UDim getHeightOfAreaReservedForContentLowerBoundAsFuncOfElementHeight() const;

    /*!
    \brief
        Get a lower bound for the element width as an affine function of the
        width of the area of the element which is reserved for content.

        An affine function means: "m" is an affine function of "t" if, for some
        constants "a" and "b", it holds true that "m = a*t +b".

        The meaning of "content" depends on the type of element. For instance,
        the content of a "FalagardStaticText" widget is its text. The area
        reserved for the content would then be the area reserved for the text,
        which is defined by the Look'N'Feel, and usually equals the size of the
        widget minus the size of the frame and scrollbars.

        This method is used by "adjustSizeToContent".

        The default implementaiton inverts the function returned by
        "getWidthOfAreaReservedForContentLowerBoundAsFuncOfElementWidth". If the
        scale of the value returned by
        "getWidthOfAreaReservedForContentLowerBoundAsFuncOfElementWidth" is 0,
        there's no solution, so an exception is thrown.

    \return
        Let:
            - "ret" be the value returned by this method.
            - "m" be the element width.
            - "t" be the width of the area of the element which is reserved for
              content.
        Then the following applies: for every non-negative number "c", if
        "m >= ret.d_scale*c + ret.d_offset" then "t >= c".

    \see adjustSizeToContent
    \see getWidthOfAreaReservedForContentLowerBoundAsFuncOfElementWidth
    \see getElementHeightLowerBoundAsFuncOfHeightOfAreaReservedForContent
    \see getContentWidth
    */
    virtual UDim getElementWidthLowerBoundAsFuncOfWidthOfAreaReservedForContent() const;
      
    /*!
    \brief
        Get a lower bound for the element height as an affine function of the
        height of the area of the element which is reserved for content.

        An affine function means: "m" is an affine function of "t" if, for some
        constants "a" and "b", it holds true that "m = a*t +b".

        The meaning of "content" depends on the type of element. For instance,
        the content of a "FalagardStaticText" widget is its text. The area
        reserved for the content would then be the area reserved for the text,
        which is defined by the Look'N'Feel, and usually equals the size of the
        widget minus the size of the frame and scrollbars.

        This method is used by "adjustSizeToContent".

        The default implementaiton inverts the function returned by
        "getWidthOfAreaReservedForContentLowerBoundAsFuncOfElementWidth". If the
        scale of the value returned by
        "getWidthOfAreaReservedForContentLowerBoundAsFuncOfElementWidth" is 0,
        there's no solution, so an exception is thrown.

    \return
        Let:
            - "ret" be the value returned by this method.
            - "m" be the element height.
            - "t" be the height of the area of the element which is reserved for
              content.
        Then the following applies: for every non-negative number "c", if
        "m >= ret.d_scale*c + ret.d_offset" then "t >= c".

    \see adjustSizeToContent
    \see getHeightOfAreaReservedForContentLowerBoundAsFuncOfElementHeight
    \see getElementWidthLowerBoundAsFuncOfWidthOfAreaReservedForContent
    \see getContentHeight
    */
    virtual UDim getElementHeightLowerBoundAsFuncOfHeightOfAreaReservedForContent() const;
      
    /*!
    \brief
        An implementation of "adjustSizeToContent" that works for simple cases.

        Set the size of the element using the following logic:
        1) If the "AdjustWidthToContent" property is set to "true", do:
            A. Compute the content width by calling "getContentWidth".
            B. Compute a lower bound for the element width as a function of
               width of the area reserved for the element content by calling
               "getElementWidthLowerBoundAsFuncOfWidthOfAreaReservedForContent"
            C. Use the results from "A" and "B" to compute a suitable element
               width - that is, such as that the width of the area reserved for
               the content be at least the result from "A".
        2) Repeat the same steps of "1", replacing "width" with "height".

        Note that in this implementation the width and height are treated
        independently of each other.

        If "getAspectMode() != IGNORE", this method respects the aspect ratio
        using the following logic:
            - If only "isWidthAdjustedToContent()" is true, compute the height
              from the width and the aspect ratio.
            - If only "isHeightAdjustedToContent()" is true, compute the width
              from the height and the aspect ratio.
            - If both "isWidthAdjustedToContent()" and
              "isHeightAdjustedToContent()" are true, expand one of the width or
              the height to comply with the aspect ratio.

    \see adjustSizeToContent
    \see getElementWidthLowerBoundAsFuncOfWidthOfAreaReservedForContent
    \see getElementHeightLowerBoundAsFuncOfHeightOfAreaReservedForContent
    \see getSizeAdjustedToContent_bisection
    \see isWidthAdjustedToContent
    \see isHeightAdjustedToContent
    */
    void adjustSizeToContent_direct();

    /*!
    \brief
        Return a tiny number ("epsilon") serving as a "safety guard" for the
        computations of "adjustSizeToContent".

        The method "adjustSizeToContent" performs various computations to
        determine the required size so that the whole element content is visible
        without the need for scrollbars (if possible), and while the content
        remains "intact" (if possible). However, due to computational errors,
        it's possible that if we use the exact value obtained, we might not
        achieve this goal fully. Therefore, we should, at some points of the
        computation, add a tiny number ("epsilon") serving as a "safety guard".
        This method returns the epsilon to use.

        Normally there's no reason to override this method.

    \see adjustSizeToContent
    */
    virtual float adjustSizeToContent_getEpsilon() const;

    /*!
    \brief
        A helper method that can be used in the implementation of
        "adjustSizeToContent" for non-simple cases.

        This method tries to find a suitable size by try-and-error: It's given a
        (mathematical) function "f(n)" whose domain is the integers between
        "floor(domain_min)" and "ceil(domain_max)", and its codomain is R^2
        (representing possible element sizes - i.e. of type "Sizef"). This
        method finds the minimal "n" for which
        "contentFitsForSpecifiedElementSize(f(n))" returns "true" - that is, for
        which the whole element content is visible without the need for
        scrollbars (if possible), and while the content remains "intact" (if
        possible). The (mathematical) function "f(n)" is an affine function
        defined by:
            f(n) = (size_func.d_width.d_scale*n +size_func.d_width.d_offset,
                    size_func.d_height.d_scale*n +size_func.d_height.d_offset)

        Note: it's assumed that
        "contentFitsForSpecifiedElementSize(f(domain_min))" returns "false" and
        "contentFitsForSpecifiedElementSize(f(domain_max))" returns "true".

        This method uses bisection to find the solution.

        You might want to look at "FalagardStaticText::adjustSizeToContent" to
        see an example of how this method is used.

    \return
        f(n)
    
    \see contentFitsForSpecifiedElementSize
    \see adjustSizeToContent
    \see FalagardStaticText::adjustSizeToContent
    */
    Sizef getSizeAdjustedToContent_bisection(const USize& size_func, float domain_min, float domain_max) const;

    /*!
    \brief
        Return whether setting the element size to "element_size" would make the
        whole element content visible without the need for scrollbars (if
        possible), and while the content remains "intact" (if possible).

        The meaning of "element content" and "intact" depends on the type of
        element. For example, for a "FalagardStaticText" widget, the content is
        the text, and "intact" means no single word is split between 2 or more
        lines. So calling this method returns whether if we set the element size
        to "element_size" the whole text would be visible, without the need for
        scrollbars, and without having to split a single word between 2 or more
        lines.

        The default implementation calls
        "contentFitsForSpecifiedElementSize_tryByResizing".

        This method is used by "getSizeAdjustedToContent_bisection".

    \see contentFitsForSpecifiedElementSize_tryByResizing
    \see getSizeAdjustedToContent_bisection
    */
    virtual bool contentFitsForSpecifiedElementSize(const Sizef& element_size) const;
      
    /*!
    \brief
        A possible implementation of "contentFitsForSpecifiedElementSize".

        This implementation actually sets (temporarily) the element size to
        "element_size", and then calls "contentFits".

    \see contentFits
    \see contentFitsForSpecifiedElementSize
    */
    bool contentFitsForSpecifiedElementSize_tryByResizing(const Sizef& element_size) const;

    /*!
    \brief
        Return whether the whole element content is visible without the need for
        scrollbars (if possible), and while the content remains "intact" (if
        possible).

        The meaning of "element content" and "intact" depends on the type of
        element. For example, for a "FalagardStaticText" widget, the content is
        the text, and "intact" means no single word is split between 2 or more
        lines.

        This method is used by
        "contentFitsForSpecifiedElementSize_tryByResizing"

    \see contentFitsForSpecifiedElementSize_tryByResizing
    */
    virtual bool contentFits() const;

    //! Gets the default paragraph direction for the displayed text of this Element.
    DefaultParagraphDirection getDefaultParagraphDirection() const;

    //! Sets the default paragraph direction for the displayed text of this Element.
    void setDefaultParagraphDirection(DefaultParagraphDirection defaultParagraphDirection);

protected:
    /*!
    \brief
        Add standard CEGUI::Element properties.
    */
    void addElementProperties();

    /*!
    \brief
        Implementation method to modify element area while correctly applying
        min / max size processing, and firing any appropriate events.

    \note
        This is the implementation function for setting size and position.
        In order to simplify area management, from this point on, all
        modifications to element size and position (area rect) should come
        through here.

    \param pos
        UVector2 object describing the new area position.

    \param size
        USize object describing the new area size.

    \param topLeftSizing
        - true to indicate the operation is a sizing operation on the top
          and/or left edges of the area, and so element movement should be
          inhibited if size is at max or min.
        - false to indicate the operation is not a strict sizing operation on
          the top and/or left edges and that the element position may change as
          required

    \param fireEvents
        - true if events should be fired as normal.
        - false to inhibit firing of events (required, for example, if you need
          to call this from the onSize/onMove handlers).

    \param adjust_size_to_content
        If the size actually changes, should we call "AdjustSizeToContent"?
        Normally, this should be true. However, if this function is called from
        inside "AdjustSizeToContent", you must set this to false to prevent
        infinite recursion.
     */
    virtual void setArea_impl(const UVector2& pos, const USize& size, bool topLeftSizing=false, bool fireEvents=true,
                              bool adjust_size_to_content=true);

    //! helper to return whether the inner rect size has changed
    inline bool isInnerRectSizeChanged() const
    {
        const Sizef old_sz(d_unclippedInnerRect.get().getSize());
        d_unclippedInnerRect.invalidateCache();
        return old_sz != d_unclippedInnerRect.get().getSize();
    }

    /*!
    \brief
        Set the parent element for this element object.

    \param parent
        Pointer to a Element object that is to be assigned as the parent to this
        Element.

    \return
        Nothing
    */
    virtual void setParent(Element* parent);

    /*!
    \brief
        Add given element to child list at an appropriate position
    */
    virtual void addChild_impl(Element* element);

    /*!
    \brief
        Remove given element from child list
    */
    virtual void removeChild_impl(Element* element);

    //! Default implementation of function to return Element's outer rect area.
    virtual Rectf getUnclippedOuterRect_impl(bool skipAllPixelAlignment) const;
    //! Default implementation of function to return Element's inner rect area.
    virtual Rectf getUnclippedInnerRect_impl(bool skipAllPixelAlignment) const;

    /*!
    \brief
        Helper to fire events based on changes to area rect.
    
    \param adjust_size_to_content
        If the size actually changes, should we call "AdjustSizeToContent"?
        Normally, this should be true. However, if this function is called from
        inside "AdjustSizeToContent", you must set this to false to prevent
        infinite recursion.  
    */
    void fireAreaChangeEvents(const bool moved, const bool sized, bool adjust_size_to_content=true);

    void notifyChildrenOfSizeChange(const bool non_client,
                                    const bool client);

    /*************************************************************************
        Event trigger methods
    *************************************************************************/
    /*!
    \brief
        Handler called when the element's size changes.

    \param e
        ElementEventArgs object whose 'element' pointer field is set to the element
        that triggered the event.

    \param adjust_size_to_content
        If the size actually changes, should we call "AdjustSizeToContent"?
        Normally, this should be true. However, if this function is called from
        inside "AdjustSizeToContent", you must set this to false to prevent
        infinite recursion.  
    */
    virtual void onSized(ElementEventArgs& e, bool adjust_size_to_content=true);
    
    /*!
    \brief
        Handler called when the element's size changes.

    \param e
        ElementEventArgs object whose 'element' pointer field is set to the element
        that triggered the event.
    */
    virtual void onSized_impl(ElementEventArgs& e);

    /*!
    \brief
        Handler called when this element's parent element has been resized.  If
        this element is the root / GUI Sheet element, this call will be made when
        the display size changes.

    \param e
        ElementEventArgs object whose 'element' pointer field is set the the
        element that caused the event; this is typically either this element's
        parent element, or NULL to indicate the screen size has changed.
    */
    virtual void onParentSized(ElementEventArgs& e);

    /*!
    \brief
        Handler called when the element's position changes.

    \param e
        ElementEventArgs object whose 'element' pointer field is set to the element
        that triggered the event.
    */
    virtual void onMoved(ElementEventArgs& e);

    /*!
    \brief
        Handler called when the horizontal alignment setting for the element is
        changed.

    \param e
        ElementEventArgs object initialised as follows:
        - element field is set to point to the element object who's alignment has
          changed (typically 'this').
    */
    virtual void onHorizontalAlignmentChanged(ElementEventArgs& e);

    /*!
    \brief
        Handler called when the vertical alignment setting for the element is
        changed.

    \param e
        ElementEventArgs object initialised as follows:
        - element field is set to point to the element object who's alignment has
          changed (typically 'this').
    */
    virtual void onVerticalAlignmentChanged(ElementEventArgs& e);

    /*!
    \brief
        Handler called when the element's rotation is changed.

    \param e
        ElementEventArgs object whose 'element' pointer field is set to the element
        that triggered the event.
    */
    virtual void onRotated(ElementEventArgs& e);

    /*!
    \brief
        Handler called when a child element is added to this element.

    \param e
        ElementEventArgs object whose 'element' pointer field is set to the element
        that has been added.
    */
    virtual void onChildAdded(ElementEventArgs& e);

    /*!
    \brief
        Handler called when a child element is removed from this element.

    \param e
        ElementEventArgs object whose 'element' pointer field is set the element
        that has been removed.
    */
    virtual void onChildRemoved(ElementEventArgs& e);

    /*!
    \brief
        Handler called when the element's non-client setting, affecting it's
        position and size relative to it's parent is changed.

    \param e
        ElementEventArgs object whose 'element' pointer field is set to the element
        that triggered the event. For this event the trigger element is always
        'this'.
    */
    virtual void onNonClientChanged(ElementEventArgs& e);

    /*!
    \brief
        Called whenever any of the properties "AdjustWidthToContent" and
        "AdjustHeightToContent" change.
    */
    virtual void onIsSizeAdjustedToContentChanged(ElementEventArgs&);

    /*************************************************************************
        Implementation Data
    *************************************************************************/
    //! definition of type used for the list of attached child elements.
    typedef std::vector<Element*> ChildList;

    //! The list of child element objects attached to this.
    ChildList d_children;
    //! Holds pointer to the parent element.
    Element* d_parent;

    //! true if element is in non-client (outside InnerRect) area of parent.
    bool d_nonClient;

    /*!
    \brief
        If set to "true", keep the width of the element at the minimal value in
        which the whole element content is visible without the
        need for a horizontal scrollbar (if possible), and while the content
        remains "intact" (if possible).

        See the documentation for "isWidthAdjustedToContent" for a more detailed
        description.
    
    \see isWidthAdjustedToContent
    \see d_isHeightAdjustedToContent
    */
    bool d_isWidthAdjustedToContent;

    /*!
    \brief
        If set to "true", keep the height of the element at the minimal value in
        which the whole element content is visible without the
        need for a vertical scrollbar (if possible), and while the content
        remains "intact" (if possible).

        See the documentation for "isHeightAdjustedToContent" for a more
        detailed description.
    
    \see isHeightAdjustedToContent
    \see d_isWidthAdjustedToContent
    */
    bool d_isHeightAdjustedToContent;

    URect d_area;
    //! Specifies the base for horizontal alignment.
    HorizontalAlignment d_horizontalAlignment;
    //! Specifies the base for vertical alignment.
    VerticalAlignment d_verticalAlignment;
    //! current minimum size for the element.
    USize d_minSize;
    //! current maximum size for the element.
    USize d_maxSize;
    //! How to satisfy current aspect ratio
    AspectMode d_aspectMode;
    //! The target aspect ratio
    float d_aspectRatio;
    //! If true, the position and size are pixel aligned
    bool d_pixelAligned;
    //! Current constrained pixel size of the element.
    Sizef d_pixelSize;
    //! Rotation of this element (relative to the parent)
    glm::quat d_rotation;
    //! Pivot point (the point around which the widget is rotated).
    UVector3 d_pivot;

    //! outer area rect in screen pixels
    CachedRectf d_unclippedOuterRect;
    //! inner area rect in screen pixels
    CachedRectf d_unclippedInnerRect;

    
    /*!
    \brief
        Default direction of the paragraph, relevant for bidirectional text.
    \see DefaultParagraphDirection
    */
    DefaultParagraphDirection d_defaultParagraphDirection = DefaultParagraphDirection::LeftToRight;
private:
    /*************************************************************************
        May not copy or assign Element objects
    *************************************************************************/
    Element(const Element&);

    Element& operator=(const Element&) {return *this;}
};

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIElement_h_
