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
#include "CEGUI/USize.h"
#include "CEGUI/Sizef.h"
#include "CEGUI/URect.h"
#include "CEGUI/DefaultParagraphDirection.h"

#include <algorithm>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4355)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{

const String Element::EventNamespace("Element");

const String Element::EventSized("Sized");
const String Element::EventParentSized("ParentSized");
const String Element::EventMoved("Moved");
const String Element::EventHorizontalAlignmentChanged("HorizontalAlignmentChanged");
const String Element::EventVerticalAlignmentChanged("VerticalAlignmentChanged");
const String Element::EventRotated("Rotated");
const String Element::EventChildAdded("ChildAdded");
const String Element::EventChildRemoved("ChildRemoved");
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
    d_rotation(1, 0, 0, 0), // <-- IDENTITY
    d_pivot(UVector3(cegui_reldim(1./2), cegui_reldim(1./2), cegui_reldim(1./2))),

    d_unclippedOuterRect(this, &Element::getUnclippedOuterRect_impl),
    d_unclippedInnerRect(this, &Element::getUnclippedInnerRect_impl)
{
    addElementProperties();
}

//----------------------------------------------------------------------------//
Element::~Element()
{}

//----------------------------------------------------------------------------//
Element::Element(const Element& other):
    EventSet(other),
    d_unclippedOuterRect(this, &Element::getUnclippedOuterRect_impl),
    d_unclippedInnerRect(this, &Element::getUnclippedInnerRect_impl)
{}

//----------------------------------------------------------------------------//
void Element::setArea(const UVector2& pos, const USize& size, bool adjust_size_to_content)
{
    setArea_impl(pos, size, false, true, adjust_size_to_content);
}

//----------------------------------------------------------------------------//
void Element::notifyScreenAreaChanged(bool recursive /* = true */)
{
    d_unclippedOuterRect.invalidateCache();
    d_unclippedInnerRect.invalidateCache();

    // inform children that their screen area must be updated
    if (recursive)
    {
        const size_t child_count = getChildCount();
        for (size_t i = 0; i < child_count; ++i)
            d_children[i]->notifyScreenAreaChanged();
    }
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

    // TODO: Perhaps we could be more selective and skip this if min size won't
    //       affect the size
    setSize(getSize());
}

//----------------------------------------------------------------------------//
void Element::setMaxSize(const USize& size)
{
    d_maxSize = size;

    // TODO: Perhaps we could be more selective and skip this if min size won't
    //       affect the size
    setSize(getSize());
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
    setArea(getArea());
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
    setArea(getArea());
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
    setArea(getArea());
}

//----------------------------------------------------------------------------//
Sizef Element::calculatePixelSize(bool skipAllPixelAlignment) const
{
    // calculate pixel sizes for everything, so we have a common format for
    // comparisons.
    Sizef absMin(CoordConverter::asAbsolute(d_minSize,
        getRootContainerSize(), false));
    Sizef absMax(CoordConverter::asAbsolute(d_maxSize,
        getRootContainerSize(), false));

    Sizef base_size;
    if (skipAllPixelAlignment)
    {
        base_size = Sizef((d_parent && !d_nonClient) ?
                           d_parent->getUnclippedInnerRect().getFresh(true).getSize() :
                           getParentPixelSize(true));
    }
    else
    {
        base_size = Sizef((d_parent && !d_nonClient) ?
                           d_parent->getUnclippedInnerRect().get().getSize() :
                           getParentPixelSize());
    }

    Sizef ret = CoordConverter::asAbsolute(getSize(), base_size, false);

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
const Sizef& Element::getRootContainerSize() const
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
bool Element::contentFitsForSpecifiedElementSize(const Sizef& element_size)
  const
{
    return contentFitsForSpecifiedElementSize_tryByResizing(element_size);
}

//----------------------------------------------------------------------------//
bool Element::contentFitsForSpecifiedElementSize_tryByResizing(const Sizef& element_size) const
{
    const USize current_size(getSize());
    const_cast<Element*>(this)->setSize(
      USize(UDim(0.f, element_size.d_width), UDim(0.f, element_size.d_height)), false);
    bool ret(contentFits());
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
    d_rotation = rotation;

    ElementEventArgs args(this);
    onRotated(args);
}

//----------------------------------------------------------------------------//
UVector3 Element::getPivot() const
{
    return d_pivot;
}

//----------------------------------------------------------------------------//
void Element::setPivot(const UVector3& pivot)
{
    d_pivot = pivot;

    ElementEventArgs args(this);
    onRotated(args);
}

//----------------------------------------------------------------------------//
void Element::addChild(Element* element)
{
    if (!element)
        throw 
                InvalidRequestException("Can't add NULL to Element as a child!");

    if (element == this)
        throw InvalidRequestException("Can't make element its own child - "
                                       "this->addChild(this); is forbidden.");

    addChild_impl(element);
    ElementEventArgs args(element);
    onChildAdded(args);
}

//----------------------------------------------------------------------------//
void Element::removeChild(Element* element)
{
    if (!element)
        throw 
                InvalidRequestException("NULL can't be a child of any Element, "
                                        "it makes little sense to ask for its "
                                        "removal");

    removeChild_impl(element);
    ElementEventArgs args(element);
    onChildRemoved(args);
}

//----------------------------------------------------------------------------//
bool Element::isChild(const Element* element) const
{
    return std::find(d_children.begin(), d_children.end(), element) != d_children.end();
}

//----------------------------------------------------------------------------//
bool Element::isAncestor(const Element* element) const
{
    if (!d_parent)
    {
        // no parent, no ancestor, nothing can be our ancestor
        return false;
    }

    return d_parent == element || d_parent->isAncestor(element);
}

//----------------------------------------------------------------------------//
void Element::setNonClient(const bool setting)
{
    if (setting == d_nonClient)
    {
        return;
    }

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
bool Element::isWidthAdjustedToContent() const
{
    return d_isWidthAdjustedToContent;
}

//----------------------------------------------------------------------------//
bool Element::isHeightAdjustedToContent() const
{
    return d_isHeightAdjustedToContent;
}

//----------------------------------------------------------------------------//
bool Element::isSizeAdjustedToContent() const
{
    return isWidthAdjustedToContent() || isHeightAdjustedToContent();
}

//----------------------------------------------------------------------------//
const Element::CachedRectf& Element::getClientChildContentArea() const
{
    return getUnclippedInnerRect();
}

//----------------------------------------------------------------------------//
const Element::CachedRectf& Element::getNonClientChildContentArea() const
{
    return getUnclippedOuterRect();
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
void Element::setArea_impl(const UVector2& pos, const USize& size, bool topLeftSizing, bool fireEvents,
                           bool adjust_size_to_content)
{
    // we make sure the screen areas are recached when this is called as we need
    // it in most cases
    d_unclippedOuterRect.invalidateCache();
    d_unclippedInnerRect.invalidateCache();

    // save original size so we can work out how to behave later on
    const Sizef oldSize(d_pixelSize);

    d_area.setSize(size);
    d_pixelSize = calculatePixelSize();

    // have we resized the element?
    const bool sized = (d_pixelSize != oldSize);

    // If this is a top/left edge sizing op, only modify position if the size
    // actually changed.  If it is not a sizing op, then position may always
    // change.
    const bool moved = (!topLeftSizing || sized) && pos != d_area.d_min;

    if (moved)
        d_area.setPosition(pos);

    if (fireEvents)
        fireAreaChangeEvents(moved, sized, adjust_size_to_content);
}

//----------------------------------------------------------------------------//
void Element::fireAreaChangeEvents(const bool moved, const bool sized, bool adjust_size_to_content)
{
    if (moved)
    {
        ElementEventArgs args(this);
        onMoved(args);
    }

    if (sized)
    {
        ElementEventArgs args(this);
        onSized(args, adjust_size_to_content);
    }
}

//----------------------------------------------------------------------------//
void Element::setParent(Element* parent)
{
    d_parent = parent;
}

//----------------------------------------------------------------------------//
void Element::addChild_impl(Element* element)
{
    // if element is attached elsewhere, detach it first (will fire normal events)
    Element* const old_parent = element->getParentElement();
    if (old_parent)
        old_parent->removeChild(element);

    // add element to child list
    d_children.push_back(element);

    // set the parent element
    element->setParent(this);

    // update area rects and content for the added element
    element->notifyScreenAreaChanged(true);

    // correctly call parent sized notification if needed.
    if (!old_parent || old_parent->getPixelSize() != getPixelSize())
    {
        ElementEventArgs args(this);
        element->onParentSized(args);
    }
}

//----------------------------------------------------------------------------//
void Element::removeChild_impl(Element* element)
{
    // find this element in the child list
    ChildList::iterator it = std::find(d_children.begin(), d_children.end(), element);

    // if the element was found in the child list
    if (it != d_children.end())
    {
        // remove element from child list
        d_children.erase(it);
        // reset element's parent so it's no longer this element.
        element->setParent(nullptr);
    }
}

//----------------------------------------------------------------------------//
Rectf Element::getUnclippedOuterRect_impl(bool skipAllPixelAlignment) const
{
    const Sizef pixel_size = skipAllPixelAlignment ?
        calculatePixelSize(true) : getPixelSize();
    Rectf ret(glm::vec2(0, 0), pixel_size);

    const Element* parent = getParentElement();

    Rectf parent_rect;
    if (parent)
    {
        const CachedRectf& base = parent->getChildContentArea(isNonClient());
        parent_rect = skipAllPixelAlignment ? base.getFresh(true) : base.get();
    }
    else
    {
        parent_rect = Rectf(glm::vec2(0, 0), getRootContainerSize());
    }

    const Sizef parent_size = parent_rect.getSize();

    glm::vec2 offset = glm::vec2(parent_rect.d_min.x, parent_rect.d_min.y) + CoordConverter::asAbsolute(getArea().d_min, parent_size, false);

    switch (getHorizontalAlignment())
    {
        case HorizontalAlignment::Centre:
            offset.x += (parent_size.d_width - pixel_size.d_width) * 0.5f;
            break;
        case HorizontalAlignment::Right:
            offset.x += parent_size.d_width - pixel_size.d_width;
            break;
        default:
            break;
    }

    switch (getVerticalAlignment())
    {
        case VerticalAlignment::Centre:
            offset.y += (parent_size.d_height - pixel_size.d_height) * 0.5f;
            break;
        case VerticalAlignment::Bottom:
            offset.y += parent_size.d_height - pixel_size.d_height;
            break;
        default:
            break;
    }

    if (d_pixelAligned && !skipAllPixelAlignment)
    {
        offset = glm::vec2(CoordConverter::alignToPixels(offset.x),
                           CoordConverter::alignToPixels(offset.y));
    }

    ret.offset(offset);
    return ret;
}

//----------------------------------------------------------------------------//
Rectf Element::getUnclippedInnerRect_impl(bool skipAllPixelAlignment) const
{
    return skipAllPixelAlignment ? getUnclippedOuterRect().getFresh(true) : getUnclippedOuterRect().get();
}

//----------------------------------------------------------------------------//
void Element::onSized(ElementEventArgs& e, bool adjust_size_to_content)
{
    onSized_impl(e);

    if (adjust_size_to_content)
        adjustSizeToContent();
}

//----------------------------------------------------------------------------//
void Element::onSized_impl(ElementEventArgs& e)
{
    notifyScreenAreaChanged(false);
    notifyChildrenOfSizeChange(true, true);

    fireEvent(EventSized, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Element::notifyChildrenOfSizeChange(const bool non_client,
                                         const bool client)
{
    const size_t child_count = getChildCount();
    for (size_t i = 0; i < child_count; ++i)
    {
        Element * const child = d_children[i];

        if ((non_client && child->isNonClient()) ||
            (client && !child->isNonClient()))
        {
            ElementEventArgs args(this);
            d_children[i]->onParentSized(args);
        }
    }
}

//----------------------------------------------------------------------------//
void Element::onParentSized(ElementEventArgs& e)
{
    d_unclippedOuterRect.invalidateCache();
    d_unclippedInnerRect.invalidateCache();

    const Sizef oldSize(d_pixelSize);
    d_pixelSize = calculatePixelSize();
    const bool sized = (d_pixelSize != oldSize) || isInnerRectSizeChanged();

    const bool moved =
        ((d_area.d_min.d_x.d_scale != 0) || (d_area.d_min.d_y.d_scale != 0) ||
         (d_horizontalAlignment != HorizontalAlignment::Left) || (d_verticalAlignment != VerticalAlignment::Top));

    fireAreaChangeEvents(moved, sized);

    fireEvent(EventParentSized, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Element::onMoved(ElementEventArgs& e)
{
    notifyScreenAreaChanged();

    fireEvent(EventMoved, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Element::onHorizontalAlignmentChanged(ElementEventArgs& e)
{
    notifyScreenAreaChanged();

    fireEvent(EventHorizontalAlignmentChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Element::onVerticalAlignmentChanged(ElementEventArgs& e)
{
    notifyScreenAreaChanged();

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
void Element::onNonClientChanged(ElementEventArgs& e)
{
    // TODO: Be less wasteful with this update
    setArea(getArea());

    fireEvent(EventNonClientChanged, e, EventNamespace);
}

DefaultParagraphDirection Element::getDefaultParagraphDirection() const
{
    return d_defaultParagraphDirection;
}

void Element::setDefaultParagraphDirection(DefaultParagraphDirection defaultParagraphDirection)
{
    if(defaultParagraphDirection != d_defaultParagraphDirection)
    {
        d_defaultParagraphDirection = defaultParagraphDirection;

        notifyScreenAreaChanged();

        ElementEventArgs eventArgs(this);
        fireEvent(EventDefaultParagraphDirectionChanged, eventArgs, EventNamespace);
    }
    
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

} // End of CEGUI namespace section
