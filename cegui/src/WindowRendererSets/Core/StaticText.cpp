/***********************************************************************
    created:    Tue Jul 5 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/WindowRendererSets/Core/StaticText.h"
#include "CEGUI/falagard/WidgetLookManager.h"
#include "CEGUI/falagard/WidgetLookFeel.h"
#include "CEGUI/falagard/XMLEnumHelper.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/widgets/Scrollbar.h"
#include "CEGUI/Event.h"
#include "CEGUI/Font.h"
#include "CEGUI/LeftAlignedRenderedString.h"
#include "CEGUI/RightAlignedRenderedString.h"
#include "CEGUI/CentredRenderedString.h"
#include "CEGUI/JustifiedRenderedString.h"
#include "CEGUI/RenderedStringWordWrapper.h"
#include "CEGUI/TplWindowRendererProperty.h"
#include "CEGUI/CoordConverter.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const String FalagardStaticText::TypeName("Core/StaticText");

    /*************************************************************************
        Child Widget name constants
    *************************************************************************/
    const String FalagardStaticText::VertScrollbarName( "__auto_vscrollbar__" );
    const String FalagardStaticText::HorzScrollbarName( "__auto_hscrollbar__" );

    /************************************************************************
        Constructor
    *************************************************************************/
    FalagardStaticText::FalagardStaticText(const String& type) :
        FalagardStatic(type),
        d_horzFormatting(HorizontalTextFormatting::LeftAligned),
        d_actualHorzFormatting(HorizontalTextFormatting::LeftAligned),
        d_vertFormatting(VerticalTextFormatting::CentreAligned),
        d_actualVertFormatting(VerticalTextFormatting::CentreAligned),
        d_textCols(0xFFFFFFFF),
        d_enableVertScrollbar(false),
        d_enableHorzScrollbar(false),
        d_formattedRenderedString(nullptr),
        d_formatValid(false)
    {
        CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardStaticText, ColourRect,
            "TextColours", "Property to get/set the text colours for the FalagardStaticText widget."
            "  Value is \"tl:[aarrggbb] tr:[aarrggbb] bl:[aarrggbb] br:[aarrggbb]\".",
            &FalagardStaticText::setTextColours, &FalagardStaticText::getTextColours,
            ColourRect(0xFFFFFFFF));

        CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardStaticText, HorizontalTextFormatting,
            "HorzFormatting", "Property to get/set the horizontal formatting mode."
            "  Value is one of the HorzFormatting strings.",
            &FalagardStaticText::setHorizontalFormatting, &FalagardStaticText::getHorizontalFormatting,
            HorizontalTextFormatting::LeftAligned);

        CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardStaticText, VerticalTextFormatting,
            "VertFormatting", "Property to get/set the vertical formatting mode."
            "  Value is one of the VertFormatting strings.",
            &FalagardStaticText::setVerticalFormatting, &FalagardStaticText::getVerticalFormatting,
            VerticalTextFormatting::CentreAligned);

        CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardStaticText, bool,
            "VertScrollbar", "Property to get/set the setting for the vertical scroll bar."
            "  Value is either \"true\" or \"false\".",
            &FalagardStaticText::setVerticalScrollbarEnabled, &FalagardStaticText::isVerticalScrollbarEnabled,
            false);

        CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardStaticText, bool,
            "HorzScrollbar", "Property to get/set the setting for the horizontal scroll bar."
            "  Value is either \"true\" or \"false\".",
            &FalagardStaticText::setHorizontalScrollbarEnabled, &FalagardStaticText::isHorizontalScrollbarEnabled,
            false);

        CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardStaticText, float,
            "HorzExtent", "Property to get the current horizontal extent of the formatted text string."
            "  Value is a float indicating the pixel extent.",
            nullptr, &FalagardStaticText::getHorizontalTextExtent,
            0);

        CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardStaticText, float,
            "VertExtent", "Property to get the current vertical extent of the formatted text string."
            "  Value is a float indicating the pixel extent.",
            nullptr, &FalagardStaticText::getVerticalTextExtent,
            0);

        CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardStaticText, NumOfTextLinesToShow,
            "NumOfTextLinesToShow", "Property to get/set the number of text lines to use to compute the content height"
            "of the widget. Useful in conjunction with \"AdjustHeightToContent\".",
            &FalagardStaticText::setNumOfTextLinesToShow, &FalagardStaticText::getNumOfTextLinesToShow,
            FalagardStaticText::NumOfTextLinesToShow::auto_());
    }

//----------------------------------------------------------------------------//
    FalagardStaticText::~FalagardStaticText()
    {
        if (d_formattedRenderedString)
            delete d_formattedRenderedString;
    }

//----------------------------------------------------------------------------//
HorizontalTextFormatting FalagardStaticText::getActualHorizontalFormatting() const
{
    return d_actualHorzFormatting;
}

//----------------------------------------------------------------------------//
VerticalTextFormatting FalagardStaticText::getActualVerticalFormatting() const
{
    return d_actualVertFormatting;
}

//----------------------------------------------------------------------------//
FalagardStaticText::NumOfTextLinesToShow::NumOfTextLinesToShow()
{
    setAuto();
}

//----------------------------------------------------------------------------//
FalagardStaticText::NumOfTextLinesToShow::NumOfTextLinesToShow(float value)
{
    d_value = value;
}

//----------------------------------------------------------------------------//
bool FalagardStaticText::NumOfTextLinesToShow::isAuto() const
{
    // "-1.f" denotes "Auto".
    return d_value == -1.f;
}

//----------------------------------------------------------------------------//
void FalagardStaticText::NumOfTextLinesToShow::setAuto()
{
    // "-1.f" denotes "Auto".
    d_value = -1.f;
}

//----------------------------------------------------------------------------//
float FalagardStaticText::NumOfTextLinesToShow::get() const
{
    return d_value;
}

//----------------------------------------------------------------------------//
void FalagardStaticText::NumOfTextLinesToShow::set(float newValue)
{
    d_value = newValue;
}

//----------------------------------------------------------------------------//
FalagardStaticText::NumOfTextLinesToShow::operator float() const
{
    return get();
}

//----------------------------------------------------------------------------//
FalagardStaticText::NumOfTextLinesToShow FalagardStaticText::NumOfTextLinesToShow::auto_()
{
    return NumOfTextLinesToShow();
}

//----------------------------------------------------------------------------//
std::size_t FalagardStaticText::getNumOfOriginalTextLines() const
{
    return d_formattedRenderedString->getNumOfOriginalTextLines();
}

//----------------------------------------------------------------------------//
std::size_t FalagardStaticText::getNumOfFormattedTextLines() const
{
   return d_formattedRenderedString->getNumOfFormattedTextLines();
}

//----------------------------------------------------------------------------//
FalagardStaticText::NumOfTextLinesToShow FalagardStaticText::getNumOfTextLinesToShow() const
{
    return d_numOfTextLinesToShow;
}

void FalagardStaticText::createRenderGeometry()
{
    // base class rendering
    FalagardStatic::createRenderGeometry();

    addScrolledTextRenderGeometry();
}

//----------------------------------------------------------------------------//
void FalagardStaticText::onIsFrameEnabledChanged()
{
    FalagardStatic::onIsFrameEnabledChanged();
    invalidateFormatting();
    getWindow()->adjustSizeToContent();
}

//----------------------------------------------------------------------------//
bool FalagardStaticText::isWordWrapOn() const
{
    switch (getHorizontalFormatting())
    {
    case HorizontalTextFormatting::LeftAligned:
    case HorizontalTextFormatting::RightAligned:
    case HorizontalTextFormatting::CentreAligned:
    case HorizontalTextFormatting::Justified:
        return false;
    case HorizontalTextFormatting::WordWrapLeftAligned:
    case HorizontalTextFormatting::WordWrapRightAligned:
    case HorizontalTextFormatting::WordWrapCentreAligned:
    case HorizontalTextFormatting::WordWraperJustified:
        return true;
    default:
        throw InvalidRequestException("Invalid horizontal formatting.");
    }
}

//----------------------------------------------------------------------------//
float FalagardStaticText::getContentWidth() const
{
    return d_formattedRenderedString->getHorizontalExtent(getWindow());
}

//----------------------------------------------------------------------------//
float FalagardStaticText::getContentHeight() const
{
    if (getNumOfTextLinesToShow().isAuto())
        return d_formattedRenderedString->getVerticalExtent(getWindow()) + 1.f;
    if (getNumOfTextLinesToShow() <= 1.f)
        return getLineHeight() * getNumOfTextLinesToShow();
    return getLineHeight() + (getNumOfTextLinesToShow()-1.f)*getVerticalAdvance();
}

//----------------------------------------------------------------------------//
UDim FalagardStaticText::getWidthOfAreaReservedForContentLowerBoundAsFuncOfWindowWidth() const
{
    return getTextComponentArea().getWidthLowerBoundAsFuncOfWindowWidth(*getWindow());
}

//----------------------------------------------------------------------------//
UDim FalagardStaticText::getHeightOfAreaReservedForContentLowerBoundAsFuncOfWindowHeight() const
{
    return getTextComponentArea().getHeightLowerBoundAsFuncOfWindowHeight(*getWindow());
}

//----------------------------------------------------------------------------//
void FalagardStaticText::adjustSizeToContent()
{
    const float epsilon = getWindow()->adjustSizeToContent_getEpsilon();
    getHorzScrollbarWithoutUpdate()->hide();
    getVertScrollbarWithoutUpdate()->hide();
    if (isWordWrapOn())
    {
        LeftAlignedRenderedString orig_str(d_formattedRenderedString->getRenderedString());
        USize size_func(UDim(-1.f, -1.f), UDim(-1.f, -1.f));
        size_func.d_width = getWindow()->getElementWidthLowerBoundAsFuncOfWidthOfAreaReservedForContent();
        size_func.d_height = getWindow()->getElementHeightLowerBoundAsFuncOfHeightOfAreaReservedForContent();
        float content_max_width(orig_str.getHorizontalExtent(getWindow()));
        float window_max_width((content_max_width+epsilon)*size_func.d_width.d_scale + size_func.d_width.d_offset);
        if (isSizeAdjustedToContentKeepingAspectRatio())
        {
            adjustSizeToContent_wordWrap_keepingAspectRatio(
              orig_str, size_func, content_max_width, window_max_width, epsilon);
            return;
        }
        if (getWindow()->isWidthAdjustedToContent())
        {
            adjustSizeToContent_wordWrap_notKeepingAspectRatio(size_func, content_max_width, window_max_width, epsilon);
            return;
        }
    }
    adjustSizeToContent_direct();
}

//----------------------------------------------------------------------------//
bool FalagardStaticText::isSizeAdjustedToContentKeepingAspectRatio() const
{
    if (!(getWindow()->isSizeAdjustedToContent() && isWordWrapOn()))
        return false;
    if (getNumOfTextLinesToShow().isAuto())
        return (getWindow()->isWidthAdjustedToContent() &&
                  getWindow()->isHeightAdjustedToContent())  ||
               (getWindow()->getAspectMode() != AspectMode::Ignore);
    return getWindow()->isWidthAdjustedToContent()  &&
           !getWindow()->isHeightAdjustedToContent()  &&
           (getWindow()->getAspectMode() != AspectMode::Ignore);
}

//----------------------------------------------------------------------------//
bool FalagardStaticText::contentFitsForSpecifiedWindowSize(const Sizef& window_size) const
{
    return getWindow()->contentFitsForSpecifiedElementSize_tryByResizing(window_size);
}

//----------------------------------------------------------------------------//
bool FalagardStaticText::contentFits() const
{
    Sizef content_size(getDocumentSize());
    if (getHorzScrollbar()->isVisible()  ||
        getVertScrollbar()->isVisible())
        return false;
    Rectf area_reserved_for_content(getTextRenderArea());
    return
      !d_formattedRenderedString->wasWordSplit()  &&
      content_size.d_width <= area_reserved_for_content.getWidth()  &&
      content_size.d_height <= area_reserved_for_content.getHeight();
}

//------------------------------------------------------------------------//
void FalagardStaticText::invalidateFormatting()
{
    d_formatValid = false;
    d_window->invalidate();
}

void FalagardStaticText::addScrolledTextRenderGeometry()
{
    updateFormatting();

    // get destination area for the text.
    const Rectf clipper(getTextRenderArea());
    Rectf absarea(clipper);

    // see if we may need to adjust horizontal position
    const Scrollbar* const horzScrollbar = getHorzScrollbar();
    if (horzScrollbar->isEffectiveVisible())
    {
        const float range = horzScrollbar->getDocumentSize() -
                            horzScrollbar->getPageSize();

        switch(getActualHorizontalFormatting())
        {
        case HorizontalTextFormatting::LeftAligned:
        case HorizontalTextFormatting::WordWrapLeftAligned:
        case HorizontalTextFormatting::Justified:
        case HorizontalTextFormatting::WordWraperJustified:
            absarea.offset(glm::vec2(-horzScrollbar->getScrollPosition(), 0));
            break;

        case HorizontalTextFormatting::CentreAligned:
        case HorizontalTextFormatting::WordWrapCentreAligned:
            absarea.setWidth(horzScrollbar->getDocumentSize());
            absarea.offset(glm::vec2(range / 2 - horzScrollbar->getScrollPosition(), 0));
            break;

        case HorizontalTextFormatting::RightAligned:
        case HorizontalTextFormatting::WordWrapRightAligned:
            absarea.offset(glm::vec2(range - horzScrollbar->getScrollPosition(), 0));
            break;
        default:
                throw InvalidRequestException("Invalid actual horizontal text formatting.");
        }
    }

    // adjust y positioning according to formatting option
    float textHeight = d_formattedRenderedString->getVerticalExtent(d_window);
    const Scrollbar* const vertScrollbar = getVertScrollbar();
    const float vertScrollPosition = vertScrollbar->getScrollPosition();
    // if scroll bar is in use, position according to that.
    if (vertScrollbar->isEffectiveVisible())
        absarea.d_min.y -= vertScrollPosition;
    // no scrollbar, so adjust position according to formatting set.
    else
        switch(getActualVerticalFormatting())
        {
        case VerticalTextFormatting::CentreAligned:
            absarea.d_min.y += CoordConverter::alignToPixels((absarea.getHeight() - textHeight) * 0.5f);
            break;
        case VerticalTextFormatting::BottomAligned:
            absarea.d_min.y = absarea.d_max.y - textHeight;
            break;
        case VerticalTextFormatting::TopAligned:
            break;
        default:
                throw InvalidRequestException("Invalid actual vertical text formatting.");
        }

    // calculate final colours
    const ColourRect final_cols(d_textCols);
    // cache the text for rendering.
    std::vector<GeometryBuffer*> geomBuffers = d_formattedRenderedString->createRenderGeometry(
        d_window,
        absarea.getPosition(),
        &final_cols, &clipper);

    d_window->appendGeometryBuffers(geomBuffers);
}

    /************************************************************************
        Returns the vertical scrollbar component
    *************************************************************************/
    Scrollbar* FalagardStaticText::getVertScrollbar() const
    {
        updateFormatting();
        return getVertScrollbarWithoutUpdate();
    }

    /************************************************************************
        Returns the horizontal scrollbar component
    *************************************************************************/
    Scrollbar* FalagardStaticText::getHorzScrollbar() const
    {
        updateFormatting();
        return getHorzScrollbarWithoutUpdate();
    }

    /************************************************************************
        Gets the text rendering area
    *************************************************************************/
    Rectf FalagardStaticText::getTextRenderArea() const
    {
        updateFormatting();
        return getTextRenderAreaWithoutUpdate();
    }

//----------------------------------------------------------------------------//
const ComponentArea& FalagardStaticText::getTextComponentArea() const
{
    updateFormatting();
    return getTextComponentAreaWithoutUpdate();
}

    /************************************************************************
        Gets the pixel size of the document
    *************************************************************************/
    Sizef FalagardStaticText::getDocumentSize() const
    {
        updateFormatting();
        return getDocumentSizeWithoutUpdate();
    }

    /************************************************************************
        Gets the pixel size of the document
    *************************************************************************/
    Sizef FalagardStaticText::getDocumentSize(const Rectf& /*renderArea*/) const
    {
        return getDocumentSize();
    }

    /*************************************************************************
        Sets the colours to be applied when rendering the text.    
    *************************************************************************/
    void FalagardStaticText::setTextColours(const ColourRect& colours)
    {
        d_textCols = colours;
        d_window->invalidate();
    }

    /*************************************************************************
        Set the formatting required for the text.    
    *************************************************************************/
    void FalagardStaticText::setVerticalFormatting(VerticalTextFormatting v_fmt)
    {
        if (d_vertFormatting == v_fmt)
            return;
        d_vertFormatting = v_fmt;
        invalidateFormatting();
        getWindow()->adjustSizeToContent();
    }

//----------------------------------------------------------------------------//
void FalagardStaticText::setNumOfTextLinesToShow(NumOfTextLinesToShow newValue)
{
    if (d_numOfTextLinesToShow == newValue)
        return;
    d_numOfTextLinesToShow = newValue;
    invalidateFormatting();
    getWindow()->adjustSizeToContent();
}

    /*************************************************************************
        Set the formatting required for the text.    
    *************************************************************************/
    void FalagardStaticText::setHorizontalFormatting(HorizontalTextFormatting h_fmt)
    {
        if (h_fmt == d_horzFormatting)
            return;
        d_horzFormatting = h_fmt;
        invalidateFormatting();
        getWindow()->adjustSizeToContent();
    }

    /*************************************************************************
        Set whether the vertical scroll bar will be shown if needed.    
    *************************************************************************/
    void FalagardStaticText::setVerticalScrollbarEnabled(bool setting)
    {
        if (d_enableVertScrollbar == setting)
            return;
        d_enableVertScrollbar = setting;
        invalidateFormatting();
        getWindow()->adjustSizeToContent();
    }

    /*************************************************************************
        Set whether the horizontal scroll bar will be shown if needed.    
    *************************************************************************/
    void FalagardStaticText::setHorizontalScrollbarEnabled(bool setting)
    {
        if (d_enableHorzScrollbar == setting)
            return;
        d_enableHorzScrollbar = setting;
        invalidateFormatting();
        getWindow()->adjustSizeToContent();
    }

    /*************************************************************************
        Update string formatting and horizontal and vertical scrollbar
        visibility. This may require repeating a process several times because
        showing one of the scrollbars shrinks the area reserved for the text,
        and thus may require reformatting of the string, as well as cause the
        2nd scrollbar to also be required.
    *************************************************************************/
    void FalagardStaticText::configureScrollbars() const
    {
        Scrollbar* vertScrollbar = getVertScrollbarWithoutUpdate();
        Scrollbar* horzScrollbar = getHorzScrollbarWithoutUpdate();
        vertScrollbar->hide();
        horzScrollbar->hide();

        Rectf renderArea(getTextRenderAreaWithoutUpdate());
        Sizef renderAreaSize(renderArea.getSize());
        d_formattedRenderedString->format(getWindow(), renderAreaSize);
        Sizef documentSize(getDocumentSizeWithoutUpdate());
        bool showVert = (documentSize.d_height > renderAreaSize.d_height)  &&
                        d_enableVertScrollbar;
        bool showHorz = (documentSize.d_width > renderAreaSize.d_width)  &&
                        d_enableHorzScrollbar;
        vertScrollbar->setVisible(showVert);
        horzScrollbar->setVisible(showHorz);

        Rectf updatedRenderArea = getTextRenderAreaWithoutUpdate();
        if (renderArea != updatedRenderArea)
        {
            renderArea = updatedRenderArea;
            renderAreaSize = renderArea.getSize();
            d_formattedRenderedString->format(getWindow(), renderAreaSize);
            documentSize = getDocumentSizeWithoutUpdate();

            showVert = (documentSize.d_height > renderAreaSize.d_height)  &&
                       d_enableVertScrollbar;
            showHorz = (documentSize.d_width > renderAreaSize.d_width)  &&
                       d_enableHorzScrollbar;
            vertScrollbar->setVisible(showVert);
            horzScrollbar->setVisible(showHorz);

            updatedRenderArea = getTextRenderAreaWithoutUpdate();
            if (renderArea != updatedRenderArea)
            {
                renderArea = updatedRenderArea;
                renderAreaSize = renderArea.getSize();
                d_formattedRenderedString->format(getWindow(), renderAreaSize);
                documentSize = getDocumentSizeWithoutUpdate();
            }
        }

        getWindow()->performChildWindowLayout();

        vertScrollbar->setDocumentSize(documentSize.d_height);
        vertScrollbar->setPageSize(renderAreaSize.d_height);
        vertScrollbar->setStepSize(std::max(1.0f, renderAreaSize.d_height / 10.0f));
        horzScrollbar->setDocumentSize(documentSize.d_width);
        horzScrollbar->setPageSize(renderAreaSize.d_width);
        horzScrollbar->setStepSize(std::max(1.0f, renderAreaSize.d_width / 10.0f));
    }

    /*************************************************************************
        Handler called when text is changed.
    *************************************************************************/
    bool FalagardStaticText::onTextChanged(const EventArgs&)
    {
        invalidateFormatting();
        getWindow()->adjustSizeToContent();
        return true;
    }


    /*************************************************************************
        Handler called when size is changed
    *************************************************************************/
    bool FalagardStaticText::onSized(const EventArgs&)
    {
        invalidateFormatting();
        return true;
    }


    /*************************************************************************
        Handler called when font is changed.
    *************************************************************************/
    bool FalagardStaticText::onFontChanged(const EventArgs&)
    {
        invalidateFormatting();
        getWindow()->adjustSizeToContent();
        return true;
    }


    /*************************************************************************
        Handler for scroll actions
    *************************************************************************/
    bool FalagardStaticText::onScroll(const EventArgs& event)
    {
        const CursorInputEventArgs& e = static_cast<const CursorInputEventArgs&>(event);

        Scrollbar* vertScrollbar = getVertScrollbar();
        Scrollbar* horzScrollbar = getHorzScrollbar();

        const bool vertScrollbarVisible = vertScrollbar->isEffectiveVisible();
        const bool horzScrollbarVisible = horzScrollbar->isEffectiveVisible();

        if (vertScrollbarVisible && (vertScrollbar->getDocumentSize() > vertScrollbar->getPageSize()))
        {
            vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition() + vertScrollbar->getStepSize() * -e.scroll);
        }
        else if (horzScrollbarVisible && (horzScrollbar->getDocumentSize() > horzScrollbar->getPageSize()))
        {
            horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition() + horzScrollbar->getStepSize() * -e.scroll);
        }

        return vertScrollbarVisible || horzScrollbarVisible;
    }

//----------------------------------------------------------------------------//
bool FalagardStaticText::onIsSizeAdjustedToContentChanged(const EventArgs&)
{
    invalidateFormatting();
    return true;
}

    /*************************************************************************
        Handler called when the scroll bar positions change
    *************************************************************************/
    bool FalagardStaticText::handleScrollbarChange(const EventArgs&)
    {
        d_window->invalidate();
        return true;
    }

    /*************************************************************************
        Attach / Detach
    *************************************************************************/
    void FalagardStaticText::onLookNFeelAssigned()
    {
        // do initial scrollbar setup
        Scrollbar* vertScrollbar = getVertScrollbarWithoutUpdate();
        Scrollbar* horzScrollbar = getHorzScrollbarWithoutUpdate();

        vertScrollbar->hide();
        horzScrollbar->hide();

        d_window->performChildWindowLayout();

        // scrollbar events
        vertScrollbar->subscribeEvent(Scrollbar::EventScrollPositionChanged,
            Event::Subscriber(&FalagardStaticText::handleScrollbarChange, this));
        horzScrollbar->subscribeEvent(Scrollbar::EventScrollPositionChanged,
            Event::Subscriber(&FalagardStaticText::handleScrollbarChange, this));

        // events that scrollbars should react to
        d_connections.push_back(
            d_window->subscribeEvent(Window::EventTextChanged,
                Event::Subscriber(&FalagardStaticText::onTextChanged, this)));

        d_connections.push_back(
            d_window->subscribeEvent(Window::EventSized,
                Event::Subscriber(&FalagardStaticText::onSized, this)));

        d_connections.push_back(
            d_window->subscribeEvent(Window::EventFontChanged,
                Event::Subscriber(&FalagardStaticText::onFontChanged, this)));

        d_connections.push_back(
            d_window->subscribeEvent(Window::EventScroll,
                Event::Subscriber(&FalagardStaticText::onScroll, this)));

        d_connections.push_back(
            d_window->subscribeEvent(Window::EventIsSizeAdjustedToContentChanged,
                Event::Subscriber(&FalagardStaticText::onIsSizeAdjustedToContentChanged, this)));

        invalidateFormatting();
        getWindow()->adjustSizeToContent();
    }

    void FalagardStaticText::onLookNFeelUnassigned()
    {
        // clean up connections that rely on widgets created by the look and feel
        ConnectionList::iterator i = d_connections.begin();
        while (i != d_connections.end())
        {
            (*i)->disconnect();
            ++i;
        }
        d_connections.clear();
    }

//----------------------------------------------------------------------------//
    void FalagardStaticText::setupStringFormatter() const
    {
        // delete any existing formatter
        delete d_formattedRenderedString;
        d_formattedRenderedString = nullptr;

        // create new formatter of whichever type...
        switch(getActualHorizontalFormatting())
        {
        case HorizontalTextFormatting::LeftAligned:
            d_formattedRenderedString =
                new LeftAlignedRenderedString(d_window->getRenderedString());
            break;

        case HorizontalTextFormatting::RightAligned:
            d_formattedRenderedString =
                new RightAlignedRenderedString(d_window->getRenderedString());
            break;

        case HorizontalTextFormatting::CentreAligned:
            d_formattedRenderedString =
                new CentredRenderedString(d_window->getRenderedString());
            break;

        case HorizontalTextFormatting::Justified:
            d_formattedRenderedString =
                new JustifiedRenderedString(d_window->getRenderedString());
            break;

        case HorizontalTextFormatting::WordWrapLeftAligned:
            d_formattedRenderedString =
                new RenderedStringWordWrapper
                    <LeftAlignedRenderedString>(d_window->getRenderedString());
            break;

        case HorizontalTextFormatting::WordWrapRightAligned:
            d_formattedRenderedString =
                new RenderedStringWordWrapper
                    <RightAlignedRenderedString>(d_window->getRenderedString());
            break;

        case HorizontalTextFormatting::WordWrapCentreAligned:
            d_formattedRenderedString =
                new RenderedStringWordWrapper
                    <CentredRenderedString>(d_window->getRenderedString());
            break;

        case HorizontalTextFormatting::WordWraperJustified:
            d_formattedRenderedString =
                new RenderedStringWordWrapper
                    <JustifiedRenderedString>(d_window->getRenderedString());
            break;
        }
    }

//----------------------------------------------------------------------------//
float FalagardStaticText::getHorizontalTextExtent() const
{
    updateFormatting();
    return d_formattedRenderedString->getHorizontalExtent(d_window);
}

//----------------------------------------------------------------------------//
float FalagardStaticText::getVerticalTextExtent() const
{
    updateFormatting();
    return d_formattedRenderedString->getVerticalExtent(d_window);
}

//----------------------------------------------------------------------------//
float FalagardStaticText::getHorizontalScrollPosition() const
{
    return getHorzScrollbar()->getScrollPosition();
}

//----------------------------------------------------------------------------//
float FalagardStaticText::getVerticalScrollPosition() const
{
    return getVertScrollbar()->getScrollPosition();
}

//----------------------------------------------------------------------------//
float FalagardStaticText::getUnitIntervalHorizontalScrollPosition() const
{
    return getHorzScrollbar()->getUnitIntervalScrollPosition();
}

//----------------------------------------------------------------------------//
float FalagardStaticText::getUnitIntervalVerticalScrollPosition() const
{
    return getVertScrollbar()->getUnitIntervalScrollPosition();
}

//----------------------------------------------------------------------------//
void FalagardStaticText::setHorizontalScrollPosition(float position)
{
    getHorzScrollbar()->setScrollPosition(position);
}

//----------------------------------------------------------------------------//
void FalagardStaticText::setVerticalScrollPosition(float position)
{
    getVertScrollbar()->setScrollPosition(position);
}

//----------------------------------------------------------------------------//
void FalagardStaticText::setUnitIntervalHorizontalScrollPosition(float position)
{
    getHorzScrollbar()->setUnitIntervalScrollPosition(position);
}

//----------------------------------------------------------------------------//
void FalagardStaticText::setUnitIntervalVerticalScrollPosition(float position)
{
    getVertScrollbar()->setUnitIntervalScrollPosition(position);
}

//----------------------------------------------------------------------------//
float FalagardStaticText::getLineHeight() const
{
    return d_window->getFont()->getFontHeight();
}

//----------------------------------------------------------------------------//
float FalagardStaticText::getVerticalAdvance() const
{
    return d_window->getFont()->getFontHeight();
}

//----------------------------------------------------------------------------//
void FalagardStaticText::updateFormatting() const
{
    if (d_formatValid)
        return;
    if (getActualHorizontalFormatting() != getHorizontalFormatting()  ||
        !d_formattedRenderedString)
    {
        d_actualHorzFormatting = getHorizontalFormatting();
        setupStringFormatter();
    }
    d_actualVertFormatting = getVerticalFormatting();
    // "Touch" the window's rendered string to ensure it's re-parsed if needed.
    d_window->getRenderedString();
    configureScrollbars();
    if (!isSizeAdjustedToContentKeepingAspectRatio())
    {
        if (getWindow()->isWidthAdjustedToContent()  &&
            (getNumOfFormattedTextLines() == 1))
        {
            d_actualHorzFormatting = isWordWrapOn() ? HorizontalTextFormatting::WordWrapCentreAligned : HorizontalTextFormatting::CentreAligned;
            setupStringFormatter();
            d_formattedRenderedString->format(getWindow(), getTextRenderArea().getSize());
        }
        if (getWindow()->isHeightAdjustedToContent()    &&
            (getNumOfTextLinesToShow().isAuto()  ||
               (getNumOfTextLinesToShow() <= getNumOfFormattedTextLines())))
            d_actualVertFormatting = VerticalTextFormatting::CentreAligned;
    }
    d_formatValid = true;
}

//----------------------------------------------------------------------------//
void FalagardStaticText::updateFormatting(const Sizef&) const
{
    updateFormatting();
}

//----------------------------------------------------------------------------//
bool FalagardStaticText::handleFontRenderSizeChange(const Font* const font)
{
    const bool res = WindowRenderer::handleFontRenderSizeChange(font);

    if (d_window->getFont() == font)
    {
        invalidateFormatting();
        getWindow()->adjustSizeToContent();
        return true;
    }

    return res;
}



    /************************************************************************
        Returns the vertical scrollbar component
    *************************************************************************/
    Scrollbar* FalagardStaticText::getVertScrollbarWithoutUpdate() const
    {
        // return component created by look'n'feel assignment.
        return static_cast<Scrollbar*>(d_window->getChild(VertScrollbarName));
    }

    /************************************************************************
        Returns the horizontal scrollbar component
    *************************************************************************/
    Scrollbar* FalagardStaticText::getHorzScrollbarWithoutUpdate() const
    {
        // return component created by look'n'feel assignment.
        return static_cast<Scrollbar*>(d_window->getChild(HorzScrollbarName));
    }

    /************************************************************************
        Gets the text rendering area
    *************************************************************************/
    Rectf FalagardStaticText::getTextRenderAreaWithoutUpdate() const
    {
        return getTextComponentAreaWithoutUpdate().getPixelRect(*d_window);
    }

//----------------------------------------------------------------------------//
const ComponentArea& FalagardStaticText::getTextComponentAreaWithoutUpdate() const
{
    Scrollbar* vertScrollbar = getVertScrollbarWithoutUpdate();
    Scrollbar* horzScrollbar = getHorzScrollbarWithoutUpdate();
    bool v_visible = vertScrollbar->isVisible();
    bool h_visible = horzScrollbar->isVisible();

    // get WidgetLookFeel for the assigned look.
    const WidgetLookFeel& wlf = getLookNFeel();

    String area_name(d_frameEnabled ? "WithFrameTextRenderArea" : "NoFrameTextRenderArea");

    // if either of the scrollbars are visible, we might want to use a special rendering area
    if (v_visible || h_visible)
    {
        if (h_visible)
        {
            area_name += "H";
        }
        if (v_visible)
        {
            area_name += "V";
        }
        area_name += "Scroll";
    }

    if (wlf.isNamedAreaPresent(area_name))
    {
        return wlf.getNamedArea(area_name).getArea();
    }

    // default to plain WithFrameTextRenderArea
    return wlf.getNamedArea("WithFrameTextRenderArea").getArea();
}

    /************************************************************************
        Gets the pixel size of the document
    *************************************************************************/
    Sizef FalagardStaticText::getDocumentSizeWithoutUpdate() const
    {
        return Sizef(d_formattedRenderedString->getHorizontalExtent(d_window),
                     d_formattedRenderedString->getVerticalExtent(d_window));
    }

/*----------------------------------------------------------------------------//
    An implementation of "adjustSizeToContent" where we adjust both the window
    width and the window height simultaneously, keeping the window's aspect
    ratio according to "getWindow()->getAspectRatio()".

    We do that by try-and-error, using bisection.
------------------------------------------------------------------------------*/
void FalagardStaticText::adjustSizeToContent_wordWrap_keepingAspectRatio(const LeftAlignedRenderedString& orig_str,
  USize& size_func, float content_max_width, float window_max_width, float epsilon)
{
    // Start by trying height that can fit 0 text lines.
    Sizef window_size(0.f, size_func.d_height.d_scale*epsilon + size_func.d_height.d_offset);
    window_size.d_width = window_size.d_height * getWindow()->getAspectRatio();
    if (getWindow()->contentFitsForSpecifiedElementSize(window_size))
    {
        // It fits - so we go for that size.
        getWindow()->setSize(USize(UDim(0.f, window_size.d_width), UDim(0.f, window_size.d_height)));
        return;
    }

    /* It doesn't fit - so we try a positive integer number of text lines. Here
       we use try-and-error, using bisection.
       We only try heights in which we can fit exactly an integer number of text
       lines, as there's no logic in trying differently. */
    UDim height_sequence_precise(size_func.d_height.d_scale*getVerticalAdvance() + size_func.d_height.d_offset,
                                 size_func.d_height.d_scale*getLineHeight() + size_func.d_height.d_offset);
    UDim height_sequence(size_func.d_height.d_scale*getVerticalAdvance() + size_func.d_height.d_offset,
                         size_func.d_height.d_scale*(getLineHeight()+epsilon) + size_func.d_height.d_offset);
    float max_num_of_lines(std::max(
      static_cast<float>(d_formattedRenderedString->getNumOfOriginalTextLines() -1),
      (window_max_width / getWindow()->getAspectRatio() - height_sequence_precise.d_offset)
        / height_sequence_precise.d_scale));
    window_size = getWindow()->getSizeAdjustedToContent_bisection(
      USize(height_sequence *getWindow()->getAspectRatio(), height_sequence), -1.f, max_num_of_lines);
    getWindow()->setSize(USize(UDim(0.f, window_size.d_width), UDim(0.f, window_size.d_height)), false);

    /* It's possible that due to a too low "getWindow()->getMaxSize().d_height",
       we're unable to make the whole text fit without the need for a vertical
       scrollbar. In that case, we need to redo the computations, with the
       vertical scrollbar visible. We go for the maximal size that makes sense,
       which is the size of "orig_str", expanded to keep the aspect ratio
       "getWindow()->getAspectRatio()". */
    updateFormatting();
    if (getVertScrollbar()->isVisible())
    {
        size_func.d_width = getWindow()->getElementWidthLowerBoundAsFuncOfWidthOfAreaReservedForContent();
        size_func.d_height = getWindow()->getElementHeightLowerBoundAsFuncOfHeightOfAreaReservedForContent();
        window_size.d_width = (content_max_width+epsilon)*size_func.d_width.d_scale + size_func.d_width.d_offset;
        window_size.d_height = (orig_str.getVerticalExtent(getWindow())+epsilon)*size_func.d_height.d_scale +
                                size_func.d_height.d_offset;
        window_size.scaleToAspect(AspectMode::Expand, getWindow()->getAspectRatio());
        getWindow()->setSize(USize(UDim(0.f, window_size.d_width), UDim(0.f, window_size.d_height)), false);
    }
}

/*----------------------------------------------------------------------------//
    An implementation of "adjustSizeToContent" where we do the following:

    1) If "getWindow()->isHeightAdjustedToContent()" is true, adjust the height
       of the window so that the text fits in without the need for a vertical
       scrollbar. This case only happens when
       "getNumOfTextLinesToShow().isAuto()" is false, which means we know
       exactly how many text lines we want to reserve space for, regardless of
       word-wrapping.
    2) Adjust the window width by try-and-error, using bisection.
------------------------------------------------------------------------------*/
void FalagardStaticText::adjustSizeToContent_wordWrap_notKeepingAspectRatio(
  USize& size_func, float content_max_width, float window_max_width, float epsilon)
{
    float height(getWindow()->isHeightAdjustedToContent()  ?
      size_func.d_height.d_scale*(getContentHeight()+epsilon) + size_func.d_height.d_offset  :
      getWindow()->getPixelSize().d_height);
    UDim height_as_u_dim(getWindow()->isHeightAdjustedToContent()  ?  UDim(0.f, height) : getWindow()->getHeight());
    float window_width(getWindow()->getSizeAdjustedToContent_bisection(
                         USize(UDim(1.f, 0.f), UDim(0.f, height)), -1.f, window_max_width)
                       .d_width);
    getWindow()->setSize(USize(UDim(0.f, window_width), height_as_u_dim), false);

     /* It's possible that due to a too low height we're unable to make the
        whole text fit without the need for a vertical scrollbar. In that case,
        we need to redo the computations, with the vertical scrollbar visible.
        We go for the maximal width that makes sense, which is the width of the
        original string (i.e. not divided to lines by word-wrapping). */
    updateFormatting();
    if (getVertScrollbar()->isVisible())
    {
        size_func.d_width = getWindow()->getElementWidthLowerBoundAsFuncOfWidthOfAreaReservedForContent();
        window_max_width = (content_max_width+epsilon)*size_func.d_width.d_scale + size_func.d_width.d_offset;
        getWindow()->setSize(USize(UDim(0.f, std::ceil(window_max_width)), height_as_u_dim));
    }
}

/*----------------------------------------------------------------------------//
    An implementation of "adjustSizeToContent" where we do the following:

    Adjust the size of the window to the text by adjusting the width and the
    height independently of each other, and then, if necessary, fix it to comply
    with "getWindow()->getAspectMode()".
------------------------------------------------------------------------------*/
void FalagardStaticText::adjustSizeToContent_direct()
{
    updateFormatting();
    getWindow()->adjustSizeToContent_direct();

    /* The process may have to be repeated, because if, for example, word wrap
       is on, and "getWindow()->isHeightAdjustedToContent()" is true, adjusting
       the height might make the vertical scrollbar visible, in which case the
       word wrapping must be recomputed and then the height adjusted again. */
    if ((getVertScrollbar()->isVisible() || getHorzScrollbar()->isVisible())  &&
        (isWordWrapOn() ||
          (getWindow()->isWidthAdjustedToContent() && getWindow()->isHeightAdjustedToContent())))
    {
        updateFormatting();
        getWindow()->adjustSizeToContent_direct();
    }
}

const String PropertyHelper<FalagardStaticText::NumOfTextLinesToShow>::s_autoString("Auto");

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
