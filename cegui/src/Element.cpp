/***********************************************************************
    created:    18/8/2011
    author:     Martin Preisler

    purpose:    Implements the Element class
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
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include "CEGUI/Element.h"
#include "CEGUI/CoordConverter.h"
#include "CEGUI/System.h"
#include "CEGUI/Logger.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4355) // 'this' is used to init unclipped rects
#endif

// Start of CEGUI namespace section
namespace CEGUI
{

const String Element::EventNamespace("Element");

const String Element::EventSized("Sized");
const String Element::EventMoved("Moved");
const String Element::EventHorizontalAlignmentChanged("HorizontalAlignmentChanged");
const String Element::EventVerticalAlignmentChanged("VerticalAlignmentChanged");
const String Element::EventRotated("Rotated");
const String Element::EventChildAdded("ChildAdded");
const String Element::EventChildRemoved("ChildRemoved");
const String Element::EventChildOrderChanged("ChildOrderChanged");
const String Element::EventZOrderChanged("ZOrderChanged");
const String Element::EventNonClientChanged("NonClientChanged");
const String Element::EventIsSizeAdjustedToContentChanged("IsSizeAdjustedToContentChanged");
const String Element::EventDefaultParagraphDirectionChanged("DefaultParagraphDirectionChanged");

//----------------------------------------------------------------------------//
Element::Element():
    d_parent(nullptr),

    d_nonClient(false),
    d_isWidthAdjustedToContent(false),
    d_isHeightAdjustedToContent(false),

    d_area(cegui_reldim(0), cegui_reldim(0), cegui_reldim(0), cegui_reldim(0)),
    d_horizontalAlignment(HorizontalAlignment::Left),
    d_verticalAlignment(VerticalAlignment::Top),
    d_minSize(cegui_reldim(0), cegui_reldim(0)),
    d_maxSize(cegui_reldim(0), cegui_reldim(0)),
    d_aspectMode(AspectMode::Ignore),
    d_aspectRatio(1.0 / 1.0),
    d_pixelAligned(true),
    d_pixelSize(0.0f, 0.0f),
    d_rotation(1.f, 0.f, 0.f, 0.f), // <-- IDENTITY
    d_pivot(UVector3(cegui_reldim(1./2), cegui_reldim(1./2), cegui_reldim(1./2))),

    d_unclippedOuterRect(this, &Element::getUnclippedOuterRect_impl),
    d_unclippedInnerRect(this, &Element::getUnclippedInnerRect_impl)
{
    addElementProperties();
}

//----------------------------------------------------------------------------//
Element::~Element() = default;

//----------------------------------------------------------------------------//
void Element::setArea(const UVector2& pos, const USize& size, bool adjust_size_to_content)
{
    // TODO: early exit if equal? or return bool from notifyScreenAreaChanged?

    d_area.setPositionAndSize(pos, size);
    notifyScreenAreaChanged(adjust_size_to_content);
}

//----------------------------------------------------------------------------//
void Element::notifyScreenAreaChanged(bool adjust_size_to_content, bool forceLayoutChildren)
{
    // Update pixel size and detect resizing
    const Sizef oldSize = d_pixelSize;
    d_pixelSize = calculatePixelSize();
    const bool sized = (d_pixelSize != oldSize);

    // Update outer rect to detect moving
    // NB: pixel size must be already updated
    const glm::vec2 oldPos = getUnclippedOuterRect().getCurrent().getPosition();
    d_unclippedOuterRect.invalidateCache();
    const bool moved = (getUnclippedOuterRect().get().getPosition() != oldPos);

    // Handle outer rect changes and check if child content rects changed
    const uint8_t flags = handleAreaChanges(moved, sized);

    if (!d_children.empty())
    {
        const bool needClientLayout = forceLayoutChildren || (flags & ClientSized);
        const bool needNonClientLayout = forceLayoutChildren || (flags & NonClientSized);
        if (needClientLayout || needNonClientLayout)
        {
            // We need full layouting when child area size changed or when explicitly requested
            performChildLayout(needClientLayout, needNonClientLayout); //???propagate adjust_size_to_content?
        }
        else if (flags & (ClientMoved | NonClientMoved))
        {
            // When moved only, recursively invalidate rects and geometry settings without full layouting
            for (Element* child : d_children)
                child->handlePositionChangeRecursively(flags & ClientMoved, flags & NonClientMoved);
        }
    }

    if (moved)
    {
        ElementEventArgs eventArgs(this);
        onMoved(eventArgs);
    }

    if (sized)
    {
        ElementEventArgs eventArgs(this);
        onSized(eventArgs);

        if (adjust_size_to_content)
            adjustSizeToContent();
    }
}

//----------------------------------------------------------------------------//
uint8_t Element::handleAreaChanges(bool moved, bool sized)
{
    // Element has inner == outer, so all children are affected by outer rect changes
    const uint8_t flags =
        (moved ? (NonClientMoved | ClientMoved) : 0) |
        (sized ? (NonClientSized | ClientSized) : 0);

    if (flags)
        d_unclippedInnerRect.invalidateCache();

    return flags;
}

//----------------------------------------------------------------------------//
void Element::handlePositionChangeRecursively(bool client, bool nonClient)
{
    d_unclippedOuterRect.invalidateCache();

    const uint8_t flags = handleAreaChanges(true, false);

    if (client || nonClient)
        for (Element* child : d_children)
            if (child->isNonClient() ? nonClient : client)
                child->handlePositionChangeRecursively(flags & ClientMoved, flags & NonClientMoved);
}

//----------------------------------------------------------------------------//
void Element::performChildLayout(bool client, bool nonClient)
{
    if (client || nonClient)
        for (Element* child : d_children)
            if (child->isNonClient() ? nonClient : client)
                child->notifyScreenAreaChanged(true);
}

//----------------------------------------------------------------------------//
void Element::setHorizontalAlignment(const HorizontalAlignment alignment)
{
    if (d_horizontalAlignment == alignment)
        return;

    d_horizontalAlignment = alignment;

    ElementEventArgs args(this);
    onHorizontalAlignmentChanged(args);
}

//----------------------------------------------------------------------------//
void Element::setVerticalAlignment(const VerticalAlignment alignment)
{
    if (d_verticalAlignment == alignment)
        return;

    d_verticalAlignment = alignment;

    ElementEventArgs args(this);
    onVerticalAlignmentChanged(args);
}

//----------------------------------------------------------------------------//
void Element::setMinSize(const USize& size)
{
    d_minSize = size;
    notifyScreenAreaChanged(true);
}

//----------------------------------------------------------------------------//
void Element::setMaxSize(const USize& size)
{
    d_maxSize = size;
    notifyScreenAreaChanged(true);
}

//----------------------------------------------------------------------------//
void Element::setAspectMode(AspectMode mode)
{
    if (d_aspectMode == mode)
        return;

    d_aspectMode = mode;

    // TODO: We want an Event and more smart rect update handling

    // Ensure the area is calculated with the new aspect mode
    // TODO: This potentially wastes effort, we should just mark it as dirty
    //       and postpone the calculation for as long as possible
    notifyScreenAreaChanged(true);
}

//----------------------------------------------------------------------------//
void Element::setAspectRatio(float ratio)
{
    if (d_aspectRatio == ratio)
        return;

    d_aspectRatio = ratio;

    // TODO: We want an Event and more smart rect update handling

    // Ensure the area is calculated with the new aspect ratio
    // TODO: This potentially wastes effort, we should just mark it as dirty
    //       and postpone the calculation for as long as possible
    notifyScreenAreaChanged(true);
}

//----------------------------------------------------------------------------//
void Element::setPixelAligned(const bool setting)
{
    if (d_pixelAligned == setting)
        return;

    d_pixelAligned = setting;

    // TODO: We want an Event and more smart rect update handling

    // Ensure the area is calculated with the new pixel aligned setting
    // TODO: This potentially wastes effort, we should just mark it as dirty
    //       and postpone the calculation for as long as possible
    notifyScreenAreaChanged(true);
}

//----------------------------------------------------------------------------//
Sizef Element::calculatePixelSize(bool skipAllPixelAlignment) const
{
    // calculate pixel sizes for everything, so we have a common format for
    // comparisons.
    const Sizef rootSize = getRootContainerSize();
    Sizef absMin(CoordConverter::asAbsolute(d_minSize, rootSize, false));
    const Sizef absMax(CoordConverter::asAbsolute(d_maxSize, rootSize, false));

    Sizef ret = CoordConverter::asAbsolute(getSize(), getBasePixelSize(skipAllPixelAlignment), false);

    // in case absMin components are larger than absMax ones,
    // max size takes precedence
    if (absMax.d_width != 0.0f && absMin.d_width > absMax.d_width)
    {
        absMin.d_width = absMax.d_width;
        CEGUI_LOGINSANE("MinSize resulted in an absolute pixel size with "
                        "width larger than what MaxSize resulted in");
    }

    if (absMax.d_height != 0.0f && absMin.d_height > absMax.d_height)
    {
        absMin.d_height = absMax.d_height;
        CEGUI_LOGINSANE("MinSize resulted in an absolute pixel size with "
                        "height larger than what MaxSize resulted in");
    }

    // limit new pixel size to: minSize <= newSize <= maxSize
    if (ret.d_width < absMin.d_width)
        ret.d_width = absMin.d_width;
    else if (absMax.d_width != 0.0f && ret.d_width > absMax.d_width)
        ret.d_width = absMax.d_width;

    if (ret.d_height < absMin.d_height)
        ret.d_height = absMin.d_height;
    else if (absMax.d_height != 0.0f && ret.d_height > absMax.d_height)
        ret.d_height = absMax.d_height;

    if (d_aspectMode != AspectMode::Ignore)
    {
        // make sure we respect current aspect mode and ratio
        ret.scaleToAspect(d_aspectMode, d_aspectRatio);

        /* Make sure we haven't blown any of the hard limits. Still maintain the
           aspect when we do this.

           NOTE: When the hard min max limits are unsatisfiable with the aspect
                 lock mode, the result won't be limited by both limits! */
        if (ret.d_width < absMin.d_width)
        {
            ret.d_height *= absMin.d_width / ret.d_width;
            ret.d_width = absMin.d_width;
        }
        else if (ret.d_height < absMin.d_height)
        {
            ret.d_width *= absMin.d_height / ret.d_height;
            ret.d_height = absMin.d_height;
        }
        else if (absMax.d_width != 0.f  &&  ret.d_width > absMax.d_width)
        {
            ret.d_height *= absMax.d_width / ret.d_width;
            ret.d_width = absMax.d_width;
        }
        else if (absMax.d_height != 0.f  &&  ret.d_height > absMax.d_height)
        {
            ret.d_width *= absMax.d_height / ret.d_height;
            ret.d_height = absMax.d_height;
        }
    }

    if (d_pixelAligned)
    {
        ret.d_width = CoordConverter::alignToPixels(ret.d_width);
        ret.d_height = CoordConverter::alignToPixels(ret.d_height);
    }

    return ret;
}

//----------------------------------------------------------------------------//
Sizef Element::getParentPixelSize(bool skipAllPixelAlignment) const
{
    if (d_parent)
    {
        return skipAllPixelAlignment ?
            d_parent->calculatePixelSize(true) : d_parent->getPixelSize();
    }
    else
    {
        return getRootContainerSize();
    }
}

//----------------------------------------------------------------------------//
Sizef Element::getBasePixelSize(bool skipAllPixelAlignment) const
{
    if (skipAllPixelAlignment)
    {
        return Sizef((d_parent && !d_nonClient) ?
            d_parent->getUnclippedInnerRect().getFresh(true).getSize() :
            getParentPixelSize(true));
    }
    else
    {
        return Sizef((d_parent && !d_nonClient) ?
            d_parent->getUnclippedInnerRect().get().getSize() :
            getParentPixelSize());
    }
}

//----------------------------------------------------------------------------//
Sizef Element::getRootContainerSize() const
{
    return System::getSingleton().getRenderer()->getDisplaySize();
}

//----------------------------------------------------------------------------//
void Element::adjustSizeToContent()
{
    adjustSizeToContent_direct();
}

//----------------------------------------------------------------------------//
float Element::getContentWidth() const
{
    throw InvalidRequestException("This function isn't implemented for this type of element.");
}

//----------------------------------------------------------------------------//
float Element::getContentHeight() const
{
    throw InvalidRequestException("This function isn't implemented for this type of element.");
}

//----------------------------------------------------------------------------//
UDim Element::getWidthOfAreaReservedForContentLowerBoundAsFuncOfElementWidth() const
{
    throw InvalidRequestException("This function isn't implemented for this type of element.");
}

//----------------------------------------------------------------------------//
UDim Element::getHeightOfAreaReservedForContentLowerBoundAsFuncOfElementHeight() const
{
    throw InvalidRequestException("This function isn't implemented for this type of element.");
}

/*----------------------------------------------------------------------------//
    By definition of
    "getWidthOfAreaReservedForContentLowerBoundAsFuncOfElementWidth" (see its
    doc), if we let "t" be the width of the area of the element which is
    reserved for content and "n" be the element width, then the following holds
    true:

        t >= inverse.d_scale*m + inverse.d_offset                          (1)

    Therefore, for every non-negative number "c", if we want "t >= c" to be
    true, it's sufficient to require that:

        inverse.d_scale*m + inverse.d_offset >= c

    Assume "inverse.d_scale > 0". Then that's equivalent to:

        m >= (c - inverse.d_offset) / inverse.d_scale

    If we let "a = 1/inverse.d_scale" and
    "b = -inverse.d_offset / inverse.d_scale" then that's equivalent to:

        m >= a*c +b

    So we have the following: for every non-negative number "c", if
    "m >= a*c +b" then "t >= c". Therefore, by the definition of
    "getElementWidthLowerBoundAsFuncOfWidthOfAreaReservedForContent" (see its
    doc), we can return:

        UDim(a, b) = UDim(1.f /inverse.d_scale, -inverse.d_offset /inverse.d_scale)

    Now, if "inverse.d_scale = 0" obviously all the above doesn't work.f In this
    case we have, from (1):

        t >= inverse.d_scale*m + inverse.d_offset = inverse.d_offset

    Which means the width of the area of the element which is reserved for
    content that we can guarantee is constant and doesn't depend on the element
    width. Therefore, no matter what "a" and "b" we choose, we can't guarantee
    that for every non-negative number "c", if "m >= a*c +b" then "t >= c"
    (because all we know is t >= inverse.d_offset). Therefore in such a case we
    throw an exception.
------------------------------------------------------------------------------*/
UDim Element::getElementWidthLowerBoundAsFuncOfWidthOfAreaReservedForContent() const
{
    UDim inverse(getWidthOfAreaReservedForContentLowerBoundAsFuncOfElementWidth());
    if (inverse.d_scale == 0.f)
        throw InvalidRequestException("Content width doesn't depend on the element width.");
    return UDim(1.f /inverse.d_scale, -inverse.d_offset /inverse.d_scale);
}

/*----------------------------------------------------------------------------//
    The implementation of this method is equivalent to that of
    "getElementWidthLowerBoundAsFuncOfWidthOfAreaReservedForContent". See the
    comment before the definition of that method for more details.
------------------------------------------------------------------------------*/
UDim Element::getElementHeightLowerBoundAsFuncOfHeightOfAreaReservedForContent() const
{
    UDim inverse(getHeightOfAreaReservedForContentLowerBoundAsFuncOfElementHeight());
    if (inverse.d_scale == 0.f)
        throw InvalidRequestException("Content height doesn't depend on the element height.");
    return UDim(1.f /inverse.d_scale, -inverse.d_offset /inverse.d_scale);
}

//----------------------------------------------------------------------------//
void Element::adjustSizeToContent_direct()
{
    if (!isSizeAdjustedToContent())
        return;
    const float epsilon = adjustSizeToContent_getEpsilon();
    USize size_func(UDim(-1.f, -1.f), UDim(-1.f, -1.f));
    Sizef new_pixel_size(getPixelSize());
    if (isWidthAdjustedToContent())
    {
        size_func.d_width = getElementWidthLowerBoundAsFuncOfWidthOfAreaReservedForContent();
        new_pixel_size.d_width = std::ceil((getContentWidth()+epsilon)*size_func.d_width.d_scale  +
                                            size_func.d_width.d_offset);
    }
    if (isHeightAdjustedToContent())
    {
        size_func.d_height = getElementHeightLowerBoundAsFuncOfHeightOfAreaReservedForContent();
        new_pixel_size.d_height = std::ceil((getContentHeight()+epsilon)*size_func.d_height.d_scale  +
                                             size_func.d_height.d_offset);
    }
    if (getAspectMode() != AspectMode::Ignore)
    {
        if (isWidthAdjustedToContent())
        {
            if (isHeightAdjustedToContent())
                new_pixel_size.scaleToAspect(AspectMode::Expand, getAspectRatio());
            else
                new_pixel_size.scaleToAspect(AspectMode::AdjustHeight, getAspectRatio());
        }
        else
        {
            if (isHeightAdjustedToContent())
                new_pixel_size.scaleToAspect(AspectMode::AdjustWidth, getAspectRatio());
        }
    }
    USize new_size(getSize());
    if (isWidthAdjustedToContent()  ||  (getAspectMode() != AspectMode::Ignore))
        new_size.d_width = UDim(0.f, new_pixel_size.d_width);
    if (isHeightAdjustedToContent()  ||  (getAspectMode() != AspectMode::Ignore))
        new_size.d_height = UDim(0.f, new_pixel_size.d_height);
    setSize(new_size, false);
}

//----------------------------------------------------------------------------//
float Element::adjustSizeToContent_getEpsilon() const
{
    return 1.f / 64.f;
}

/*----------------------------------------------------------------------------//
    Return the lowest power of 2 (with non-negative integer exponent) which is
    greater than or equal to "value".
------------------------------------------------------------------------------*/
static unsigned int powOf2Supremum(unsigned int value)
{
    unsigned int num_of_digits = 0;
    if (value != 0)
    {
        --value;
        while (value != 0)
        {
            ++num_of_digits;
            value >>= 1;
        }
    }
    return 1u << num_of_digits;
}

//----------------------------------------------------------------------------//
Sizef Element::getSizeAdjustedToContent_bisection(const USize& size_func, float domain_min, float domain_max) const
{
    int domain_min_int(static_cast<int>(std::floor(domain_min)));
    int domain_max_int(static_cast<int>(std::ceil(domain_max)));
    if (domain_min_int >= domain_max_int)
        throw InvalidRequestException("Length of domain is 0.");

    /* First, enlarge the domain so that it's a power of 2 (with non-negative
       integer exponent). This makes the bisection use only integer
       parameters. */
    int domain_size(domain_max_int - domain_min_int);
    int domain_size_pow_of_2(static_cast<int>(powOf2Supremum(domain_size)));
    domain_min_int -= domain_size_pow_of_2 - domain_size;

    Sizef element_size(0.f, 0.f);
    while (true)
    {
        int param((domain_min_int+domain_max_int+1) / 2);
        float param_float(static_cast<float>(param));
        element_size = Sizef(size_func.d_width.d_scale*param_float + size_func.d_width.d_offset,
                             size_func.d_height.d_scale*param_float + size_func.d_height.d_offset);
        if (domain_max_int <= domain_min_int+1)
            break;
        if (param_float <= domain_min)
            domain_min_int = param;
        else if (param_float >= domain_max    ||
                 ((element_size.d_width >= 0)  &&
                   (element_size.d_height >= 0)  &&
                   contentFitsForSpecifiedElementSize(element_size)))
            domain_max_int = param;
        else
            domain_min_int = param;
    }
    return element_size;
}

//----------------------------------------------------------------------------//
bool Element::contentFitsForSpecifiedElementSize(const Sizef& element_size) const
{
    return contentFitsForSpecifiedElementSize_tryByResizing(element_size);
}

//----------------------------------------------------------------------------//
bool Element::contentFitsForSpecifiedElementSize_tryByResizing(const Sizef& element_size) const
{
    const USize current_size(getSize());
    const_cast<Element*>(this)->setSize(
      USize(UDim(0.f, element_size.d_width), UDim(0.f, element_size.d_height)), false);
    const bool ret = contentFits();
    const_cast<Element*>(this)->setSize(current_size, false);
    return ret;
}

//----------------------------------------------------------------------------//
bool Element::contentFits() const
{
    throw InvalidRequestException("This function isn't implemented for this type of element.");
}

//----------------------------------------------------------------------------//
void Element::setRotation(const glm::quat& rotation)
{
    if (d_rotation == rotation)
        return;

    d_rotation = rotation;

    ElementEventArgs args(this);
    onRotated(args);
}

//----------------------------------------------------------------------------//
void Element::setPivot(const UVector3& pivot)
{
    if (d_pivot == pivot)
        return;

    d_pivot = pivot;

    ElementEventArgs args(this);
    onRotated(args);
}

//----------------------------------------------------------------------------//
void Element::addChild(Element* element)
{
    if (!element)
        throw InvalidRequestException("Can't add NULL to Element as a child!");

    if (element == this)
        throw InvalidRequestException("Can't make element its own child - "
                                      "this->addChild(this); is forbidden.");

    // if the element is already a child of this Element, this is a NOOP
    if (isChild(element))
        return;

    addChild_impl(element);

    // Update child rects after all class-specific child initialization
    element->notifyScreenAreaChanged(true, true);

    ElementEventArgs args(element);
    onChildAdded(args);
}

//----------------------------------------------------------------------------//
void Element::removeChild(Element* element)
{
    if (!element)
        throw InvalidRequestException("NULL can't be a child of any Element, "
                                      "it makes little sense to ask for its "
                                      "removal");

    removeChild_impl(element);
    ElementEventArgs args(element);
    onChildRemoved(args);
}

//----------------------------------------------------------------------------//
void Element::addChildAtIndex(Element* element, size_t index)
{
    addChild(element);
    moveChildToIndex(element, index);
}

//----------------------------------------------------------------------------//
void Element::removeChildAtIndex(size_t index)
{
    removeChild(getChildElementAtIndex(index));
}

//----------------------------------------------------------------------------//
void Element::moveChildToIndex(size_t indexFrom, size_t indexTo)
{
    indexTo = std::min(indexTo, d_children.size());

    if (indexFrom == indexTo || indexFrom >= d_children.size())
    {
        return;
    }

    // we get the iterator of the old position
    ChildList::iterator it = d_children.begin();
    std::advance(it, indexFrom);

    Element* child = *it;

    // erase the child from it's old position
    d_children.erase(it);

    // if the window comes before the point we want to insert to,
    // we have to decrement the position
    if (indexFrom < indexTo)
    {
        --indexTo;
    }

    // find iterator of the new position
    it = d_children.begin();
    std::advance(it, indexTo);
    // and insert the window there
    d_children.insert(it, child);

    ElementEventArgs args(this);
    onChildOrderChanged(args);
}

//----------------------------------------------------------------------------//
void Element::moveChildToIndex(Element* child, size_t index)
{
    moveChildToIndex(getChildIndex(child), index);
}

//----------------------------------------------------------------------------//
void Element::moveChildByDelta(Element* child, int delta)
{
    const size_t oldIndex = getChildIndex(child);
    const size_t newIndex =
        static_cast<size_t>(std::max(0, static_cast<int>(oldIndex) + delta));
    moveChildToIndex(oldIndex, newIndex);
}

//----------------------------------------------------------------------------//
void Element::swapChildren(size_t index1, size_t index2)
{
    if (index1 < d_children.size() &&
        index2 < d_children.size() &&
        index1 != index2)
    {
        std::swap(d_children[index1], d_children[index2]);

        ElementEventArgs args(this);
        onChildOrderChanged(args);
    }
}

//----------------------------------------------------------------------------//
void Element::swapChildren(Element* child1, Element* child2)
{
    if (child1 != child2)
        swapChildren(getChildIndex(child1), getChildIndex(child2));
}

//----------------------------------------------------------------------------//
size_t Element::getChildIndex(const Element* child) const
{
    const size_t child_count = getChildCount();

    for (size_t i = 0; i < child_count; ++i)
        if (d_children[i] == child)
            return i;

    // Any value >= getChildCount() must be treated as invalid
    return std::numeric_limits<size_t>().max();
}

//----------------------------------------------------------------------------//
bool Element::isChild(const Element* element) const
{
    return std::find(d_children.begin(), d_children.end(), element) != d_children.end();
}

//----------------------------------------------------------------------------//
void Element::setNonClient(const bool setting)
{
    if (setting == d_nonClient)
        return;

    d_nonClient = setting;

    ElementEventArgs args(this);
    onNonClientChanged(args);
}

//----------------------------------------------------------------------------//
void Element::setAdjustWidthToContent(bool value)
{
    if (d_isWidthAdjustedToContent == value)
        return;
    d_isWidthAdjustedToContent = value;
    ElementEventArgs args(this);
    onIsSizeAdjustedToContentChanged(args);
}

//----------------------------------------------------------------------------//
void Element::setAdjustHeightToContent(bool value)
{
    if (d_isHeightAdjustedToContent == value)
        return;
    d_isHeightAdjustedToContent = value;
    ElementEventArgs args(this);
    onIsSizeAdjustedToContentChanged(args);
}

//----------------------------------------------------------------------------//
void Element::onIsSizeAdjustedToContentChanged(ElementEventArgs& e)
{
    adjustSizeToContent();
    fireEvent(EventIsSizeAdjustedToContentChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Element::addElementProperties()
{
    const String propertyOrigin("Element");

    CEGUI_DEFINE_PROPERTY(Element, URect,
        "Area", "Property to get/set the unified area rectangle. Value is a \"URect\".",
        &Element::setArea, &Element::getArea, URect(UDim(0, 0), UDim(0, 0), UDim(0, 0), UDim(0, 0))
    );

    CEGUI_DEFINE_PROPERTY_NO_XML(Element, UVector2,
        "Position", "Property to get/set the unified position. Value is a \"UVector2\".",
        &Element::setPosition, &Element::getPosition, UVector2(UDim(0, 0), UDim(0, 0))
    );

    CEGUI_DEFINE_PROPERTY(Element, VerticalAlignment,
        "VerticalAlignment", "Property to get/set the vertical alignment.  Value is one of \"Top\", \"Centre\" or \"Bottom\".",
        &Element::setVerticalAlignment, &Element::getVerticalAlignment, VerticalAlignment::Top
    );

    CEGUI_DEFINE_PROPERTY(Element, HorizontalAlignment,
        "HorizontalAlignment", "Property to get/set the horizontal alignment.  Value is one of \"Left\", \"Centre\" or \"Right\".",
        &Element::setHorizontalAlignment, &Element::getHorizontalAlignment, HorizontalAlignment::Left
    );

    CEGUI_DEFINE_PROPERTY_NO_XML(Element, USize,
        "Size", "Property to get/set the unified size. Value is a \"USize\".",
        &Element::setSize, &Element::getSize, USize(UDim(0, 0), UDim(0, 0))
    );

    CEGUI_DEFINE_PROPERTY(Element, USize,
        "MinSize", "Property to get/set the unified minimum size. Value is a \"USize\".",
        &Element::setMinSize, &Element::getMinSize, USize(UDim(0, 0), UDim(0, 0))
    );

    CEGUI_DEFINE_PROPERTY(Element, USize, "MaxSize",
        "Property to get/set the unified maximum size. Value is a \"USize\". "
        "Note that zero means no maximum size.",
        &Element::setMaxSize, &Element::getMaxSize, USize(UDim(0, 0), UDim(0, 0))
    );

    CEGUI_DEFINE_PROPERTY(Element, AspectMode,
        "AspectMode", "Property to get/set the 'aspect mode' setting. Value is either \"Ignore\", \"Shrink\", "
        "\"Expand\", \"AdjustHeight\" or \"AdjustWidth\".",
        &Element::setAspectMode, &Element::getAspectMode, AspectMode::Ignore
    );

    CEGUI_DEFINE_PROPERTY(Element, float,
        "AspectRatio", "Property to get/set the aspect ratio. Only applies when aspect mode is not \"Ignore\".",
        &Element::setAspectRatio, &Element::getAspectRatio, 1.0 / 1.0
    );

    CEGUI_DEFINE_PROPERTY(Element, bool,
        "PixelAligned", "Property to get/set whether the Element's size and position should be pixel aligned. "
        "Value is either \"true\" or \"false\".",
        &Element::setPixelAligned, &Element::isPixelAligned, true
    );

    CEGUI_DEFINE_PROPERTY(Element, glm::quat,
        "Rotation", "Property to get/set the Element's rotation. Value is a quaternion (glm::quat): "
        "\"w:[w_float] x:[x_float] y:[y_float] z:[z_float]\""
        "or \"x:[x_float] y:[y_float] z:[z_float]\" to convert from Euler angles (in degrees).",
        &Element::setRotation, &Element::getRotation, glm::quat(1.0, 0.0, 0.0, 0.0)
    );

    CEGUI_DEFINE_PROPERTY(Element, UVector3,
        "Pivot", "Property to get/set the Element's rotation's pivot point.",
        &Element::setPivot, &Element::getPivot,
        UVector3(cegui_reldim(1./2), cegui_reldim(1./2), cegui_reldim(1./2))
    );

    CEGUI_DEFINE_PROPERTY(Element, bool,
        "NonClient", "Property to get/set whether the Element is 'non-client'. "
        "Value is either \"true\" or \"false\".",
        &Element::setNonClient, &Element::isNonClient, false
    );

    CEGUI_DEFINE_PROPERTY(Element, bool, "AdjustWidthToContent",
        "Property to get/set whether to " "automatically adjust the element's " "width to the element's content.  "
        "Value is either \"true\" or \"false\".",
        &Element::setAdjustWidthToContent, &Element::isWidthAdjustedToContent, false
    );

    CEGUI_DEFINE_PROPERTY(Element, bool, "AdjustHeightToContent",
        "Property to get/set whether to " "automatically adjust the element's height to the element's content.  "
        "Value is either \"true\" or \"false\".",
        &Element::setAdjustHeightToContent, &Element::isHeightAdjustedToContent, false
    );

    CEGUI_DEFINE_PROPERTY(Element, DefaultParagraphDirection,
        "DefaultParagraphDirection", "Property to get/set the default paragraph direction. "
        "This is only in effect if raqm is linked and activate. It sets the default order of the "
        "words in a paragraph, which is relevant when having sentences in a RightToLeft language that "
        "may start with a word (or to be specific: first character of a word) from a LeftToRight language. "
        "Example: If the mode is set to Automatic and the first word of a paragraph in Hebrew is a German "
        "company name, written in German alphabet, the German word will end up left, whereas the rest of "
        "the Hebrew sentences starts from the righ, continuing towards the left. With the setting RightToLeft "
        "the sentence will start on the very right with the German word, as would be expected in a mainly "
        "RightToLeft written paragraph. If the language of the UI user is known, then either LeftToRight "
        "or RightToLeft should be chosen for the paragraphs. Default is LeftToRight."
        "Value is one of \"LeftToRight\", \"RightToLeft\" or \"Automatic\".",
        &Element::setDefaultParagraphDirection, &Element::getDefaultParagraphDirection,
        DefaultParagraphDirection::LeftToRight
    );

}

//----------------------------------------------------------------------------//
void Element::addChild_impl(Element* element)
{
    // if element is attached elsewhere, detach it first (will fire normal events)
    if (Element* const oldParent = element->d_parent)
        oldParent->removeChild(element);

    // add element to child list
    if (std::find(d_children.cbegin(), d_children.cend(), element) == d_children.cend())
        d_children.push_back(element);

    element->d_parent = this;
}

//----------------------------------------------------------------------------//
void Element::removeChild_impl(Element* element)
{
    // NB: it is intentionally valid to remove an element that is not in the list

    // find this element in the child list
    ChildList::iterator it = std::find(d_children.begin(), d_children.end(), element);

    // if the element was found in the child list, remove it from there
    if (it != d_children.end())
        d_children.erase(it);

    // reset element's parent so it's no longer this element
    if (element->d_parent == this)
        element->d_parent = nullptr;
}

//----------------------------------------------------------------------------//
Rectf Element::getUnclippedOuterRect_impl(bool skipAllPixelAlignment) const
{
    const Rectf parent_rect = (!d_parent) ?
        Rectf(glm::vec2(0, 0), getRootContainerSize()) :
        skipAllPixelAlignment ?
            d_parent->getChildContentArea(d_nonClient).getFresh(true) :
            d_parent->getChildContentArea(d_nonClient).get();

    const Sizef parent_size = parent_rect.getSize();
    const Sizef pixel_size = skipAllPixelAlignment ? calculatePixelSize(true) : getPixelSize();

    glm::vec2 offset = parent_rect.d_min + CoordConverter::asAbsolute(d_area.d_min, parent_size, false);

    if (d_horizontalAlignment == HorizontalAlignment::Centre)
        offset.x += (parent_size.d_width - pixel_size.d_width) * 0.5f;
    else if (d_horizontalAlignment == HorizontalAlignment::Right)
        offset.x += parent_size.d_width - pixel_size.d_width;

    if (d_verticalAlignment == VerticalAlignment::Centre)
        offset.y += (parent_size.d_height - pixel_size.d_height) * 0.5f;
    else if (d_verticalAlignment == VerticalAlignment::Bottom)
        offset.y += parent_size.d_height - pixel_size.d_height;

    if (d_pixelAligned && !skipAllPixelAlignment)
    {
        offset.x = CoordConverter::alignToPixels(offset.x);
        offset.y = CoordConverter::alignToPixels(offset.y);
    }

    return Rectf(offset, pixel_size);
}

//----------------------------------------------------------------------------//
Rectf Element::getUnclippedInnerRect_impl(bool skipAllPixelAlignment) const
{
    return skipAllPixelAlignment ? getUnclippedOuterRect().getFresh(true) : getUnclippedOuterRect().get();
}

//----------------------------------------------------------------------------//
void Element::onSized(ElementEventArgs& e)
{
    fireEvent(EventSized, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Element::onMoved(ElementEventArgs& e)
{
    fireEvent(EventMoved, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Element::onHorizontalAlignmentChanged(ElementEventArgs& e)
{
    notifyScreenAreaChanged(true);

    fireEvent(EventHorizontalAlignmentChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Element::onVerticalAlignmentChanged(ElementEventArgs& e)
{
    notifyScreenAreaChanged(true);

    fireEvent(EventVerticalAlignmentChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Element::onRotated(ElementEventArgs& e)
{
    fireEvent(EventRotated, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Element::onChildAdded(ElementEventArgs& e)
{
    fireEvent(EventChildAdded, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Element::onChildRemoved(ElementEventArgs& e)
{
    fireEvent(EventChildRemoved, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Element::onChildOrderChanged(ElementEventArgs& e)
{
    fireEvent(EventChildOrderChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Element::onNonClientChanged(ElementEventArgs& e)
{
    notifyScreenAreaChanged(true);

    fireEvent(EventNonClientChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Element::setDefaultParagraphDirection(DefaultParagraphDirection defaultParagraphDirection)
{
    if(defaultParagraphDirection != d_defaultParagraphDirection)
    {
        d_defaultParagraphDirection = defaultParagraphDirection;

        notifyScreenAreaChanged(true);

        ElementEventArgs eventArgs(this);
        fireEvent(EventDefaultParagraphDirectionChanged, eventArgs, EventNamespace);
    }
    
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

} // End of CEGUI namespace section
