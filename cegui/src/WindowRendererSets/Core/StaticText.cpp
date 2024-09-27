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
#include "CEGUI/falagard/WidgetLookFeel.h"
#include "CEGUI/falagard/XMLEnumHelper.h"
#include "CEGUI/widgets/Scrollbar.h"
#include "CEGUI/text/Font.h"
#include "CEGUI/System.h"
#include "CEGUI/TplWindowRendererProperty.h"
#include "CEGUI/CoordConverter.h"

namespace CEGUI
{
const String FalagardStaticText::TypeName("Core/StaticText");
const String FalagardStaticText::VertScrollbarName( "__auto_vscrollbar__" );
const String FalagardStaticText::HorzScrollbarName( "__auto_hscrollbar__" );
const String PropertyHelper<FalagardStaticText::NumOfTextLinesToShow>::s_autoString("Auto");

//----------------------------------------------------------------------------//
FalagardStaticText::FalagardStaticText(const String& type)
    : FalagardStatic(type)
{
    d_renderedText.setHorizontalFormatting(d_horzFormatting);
    d_renderedText.setWordWrapEnabled(d_wordWrap);

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

    CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardStaticText, bool,
        "WordWrap", "Property to enable/disable text word wrapping. Value is bool.",
        &FalagardStaticText::setWordWrapEnabled, &FalagardStaticText::isWordWrapEnabled,
        false);

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
        FalagardStaticText::NumOfTextLinesToShow());
}

//----------------------------------------------------------------------------//
FalagardStaticText::~FalagardStaticText() = default;

//----------------------------------------------------------------------------//
void FalagardStaticText::createRenderGeometry()
{
    // Create common geometry for Static
    FalagardStatic::createRenderGeometry();

    updateFormatting();

    // get destination area for the text.
    const Rectf clipper(getTextRenderArea());
    Rectf destRect(clipper);

    // see if we may need to adjust horizontal position
    const Scrollbar* const horzScrollbar = getHorzScrollbar();
    if (horzScrollbar->isEffectiveVisible())
    {
        const float range = horzScrollbar->getDocumentSize() - horzScrollbar->getPageSize();

        switch (d_actualHorzFormatting)
        {
            case HorizontalTextFormatting::LeftAligned:
            case HorizontalTextFormatting::Justified:
                destRect.offset(glm::vec2(-horzScrollbar->getScrollPosition(), 0));
                break;

            case HorizontalTextFormatting::CentreAligned:
                destRect.setWidth(horzScrollbar->getDocumentSize());
                destRect.offset(glm::vec2(range / 2 - horzScrollbar->getScrollPosition(), 0));
                break;

            case HorizontalTextFormatting::RightAligned:
                destRect.offset(glm::vec2(range - horzScrollbar->getScrollPosition(), 0));
                break;
            default:
                throw InvalidRequestException("Invalid actual horizontal text formatting.");
        }
    }

    // adjust y positioning according to formatting option
    const float textHeight = d_renderedText.getExtents().d_height;
    const Scrollbar* const vertScrollbar = getVertScrollbar();
    const float vertScrollPosition = vertScrollbar->getScrollPosition();
    if (vertScrollbar->isEffectiveVisible())
    {
        // if scroll bar is in use, position according to that.
        destRect.d_min.y -= vertScrollPosition;
    }
    else
    {
        // no scrollbar, so adjust position according to formatting set.
        switch (d_actualVertFormatting)
        {
            case VerticalTextFormatting::CentreAligned:
                destRect.d_min.y += CoordConverter::alignToPixels((destRect.getHeight() - textHeight) * 0.5f);
                break;
            case VerticalTextFormatting::BottomAligned:
                destRect.d_min.y = destRect.d_max.y - textHeight;
                break;
        }
    }

    // cache the text for rendering.
    d_renderedText.createRenderGeometry(d_window->getGeometryBuffers(), destRect.getPosition(), &d_textCols, &clipper);
}

//----------------------------------------------------------------------------//
void FalagardStaticText::onIsFrameEnabledChanged()
{
    FalagardStatic::onIsFrameEnabledChanged();
    invalidateFormatting();
    d_window->adjustSizeToContent();
}

//----------------------------------------------------------------------------//
std::size_t FalagardStaticText::getNumOfOriginalTextLines() const
{
    return d_renderedText.getParagraphCount();
}

//----------------------------------------------------------------------------//
std::size_t FalagardStaticText::getNumOfFormattedTextLines() const
{
    return d_renderedText.getLineCount();
}

//----------------------------------------------------------------------------//
Sizef FalagardStaticText::getContentSize() const
{
    return Sizef(
        d_renderedText.getExtents().d_width,

        d_numOfTextLinesToShow.isAuto() ?
            d_renderedText.getExtents().d_height + 1.f :
            (d_numOfTextLinesToShow <= 1.f) ?
                getLineHeight() * d_numOfTextLinesToShow :
                getLineHeight() + (d_numOfTextLinesToShow - 1.f) * getVerticalAdvance());
}

//----------------------------------------------------------------------------//
UDim FalagardStaticText::getWidthOfAreaReservedForContentLowerBoundAsFuncOfWindowWidth() const
{
    return getTextComponentArea().getWidthLowerBoundAsFuncOfWindowWidth(*d_window);
}

//----------------------------------------------------------------------------//
UDim FalagardStaticText::getHeightOfAreaReservedForContentLowerBoundAsFuncOfWindowHeight() const
{
    return getTextComponentArea().getHeightLowerBoundAsFuncOfWindowHeight(*d_window);
}

//----------------------------------------------------------------------------//
void FalagardStaticText::adjustSizeToContent()
{
    const float epsilon = d_window->adjustSizeToContent_getEpsilon();
    getHorzScrollbarWithoutUpdate()->hide();
    getVertScrollbarWithoutUpdate()->hide();
    if (d_wordWrap)
    {
        const Sizef& contentMaxSize = d_renderedText.getExtents();
        USize sizeFunc(
            d_window->getElementWidthLowerBoundAsFuncOfWidthOfAreaReservedForContent(),
            d_window->getElementHeightLowerBoundAsFuncOfHeightOfAreaReservedForContent());
        float windowMaxWidth((contentMaxSize.d_width + epsilon) * sizeFunc.d_width.d_scale + sizeFunc.d_width.d_offset);
        if (isSizeAdjustedToContentKeepingAspectRatio())
        {
            adjustSizeToContent_wordWrap_keepingAspectRatio(contentMaxSize, sizeFunc, windowMaxWidth, epsilon);
            return;
        }
        if (d_window->isWidthAdjustedToContent())
        {
            adjustSizeToContent_wordWrap_notKeepingAspectRatio(sizeFunc, contentMaxSize.d_width, windowMaxWidth, epsilon);
            return;
        }
    }
    adjustSizeToContent_direct();
}

//----------------------------------------------------------------------------//
bool FalagardStaticText::isSizeAdjustedToContentKeepingAspectRatio() const
{
    if (!(d_window->isSizeAdjustedToContent() && d_wordWrap))
        return false;
    if (d_numOfTextLinesToShow.isAuto())
        return (d_window->isWidthAdjustedToContent() &&
            d_window->isHeightAdjustedToContent())  ||
               (d_window->getAspectMode() != AspectMode::Ignore);
    return d_window->isWidthAdjustedToContent()  &&
           !d_window->isHeightAdjustedToContent()  &&
           (d_window->getAspectMode() != AspectMode::Ignore);
}

//----------------------------------------------------------------------------//
bool FalagardStaticText::contentFitsForSpecifiedWindowSize(const Sizef& window_size) const
{
    return d_window->contentFitsForSpecifiedElementSize_tryByResizing(window_size);
}

//----------------------------------------------------------------------------//
bool FalagardStaticText::contentFits() const
{
    // Scrollbars are configured inside
    const Sizef contentSize = getTextExtent();

    if (getHorzScrollbar()->isVisible() || getVertScrollbar()->isVisible())
        return false;

    const Rectf area(getTextRenderArea());
    return !d_renderedText.isFittingIntoAreaWidth() &&
      contentSize.d_width <= area.getWidth() &&
      contentSize.d_height <= area.getHeight();
}

//------------------------------------------------------------------------//
void FalagardStaticText::invalidateFormatting()
{
    d_formatValid = false;
    d_window->invalidate();
}

//------------------------------------------------------------------------//
Scrollbar* FalagardStaticText::getVertScrollbar() const
{
    updateFormatting();
    return getVertScrollbarWithoutUpdate();
}

//------------------------------------------------------------------------//
Scrollbar* FalagardStaticText::getHorzScrollbar() const
{
    updateFormatting();
    return getHorzScrollbarWithoutUpdate();
}

//------------------------------------------------------------------------//
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

//------------------------------------------------------------------------//
void FalagardStaticText::setTextColours(const ColourRect& colours)
{
    d_textCols = colours;
    d_window->invalidate();
}

//------------------------------------------------------------------------//
void FalagardStaticText::setHorizontalFormatting(HorizontalTextFormatting h_fmt)
{
    if (h_fmt == d_horzFormatting)
        return;

    bool wordWrap = false;
    d_horzFormatting = decomposeHorizontalFormatting(h_fmt, &wordWrap);
    if (wordWrap)
        setWordWrapEnabled(true);

    d_renderedText.setHorizontalFormatting(d_horzFormatting);
    invalidateFormatting();
    d_window->adjustSizeToContent();
}

//------------------------------------------------------------------------//
void FalagardStaticText::setWordWrapEnabled(bool wrap)
{
    if (d_wordWrap == wrap)
        return;

    d_wordWrap = wrap;
    d_renderedText.setWordWrapEnabled(d_wordWrap);
    invalidateFormatting();
    d_window->adjustSizeToContent();
}

//------------------------------------------------------------------------//
void FalagardStaticText::setVerticalFormatting(VerticalTextFormatting v_fmt)
{
    if (d_vertFormatting == v_fmt)
        return;

    d_vertFormatting = v_fmt;
    invalidateFormatting();
    d_window->adjustSizeToContent();
}

//----------------------------------------------------------------------------//
void FalagardStaticText::setNumOfTextLinesToShow(NumOfTextLinesToShow newValue)
{
    if (d_numOfTextLinesToShow == newValue)
        return;

    d_numOfTextLinesToShow = newValue;
    invalidateFormatting();
    d_window->adjustSizeToContent();
}

//----------------------------------------------------------------------------//
void FalagardStaticText::setVerticalScrollbarEnabled(bool setting)
{
    if (d_enableVertScrollbar == setting)
        return;

    d_enableVertScrollbar = setting;
    invalidateFormatting();
    d_window->adjustSizeToContent();
}

//----------------------------------------------------------------------------//
void FalagardStaticText::setHorizontalScrollbarEnabled(bool setting)
{
    if (d_enableHorzScrollbar == setting)
        return;

    d_enableHorzScrollbar = setting;
    invalidateFormatting();
    d_window->adjustSizeToContent();
}

//----------------------------------------------------------------------------//
// Update string formatting and horizontal and vertical scrollbar visibility.
// This may require repeating a process several times because showing one of
// the scrollbars shrinks the area reserved for the text, and thus may require
// reformatting of the string, as well as cause the 2nd scrollbar to also be required.
void FalagardStaticText::configureScrollbars() const
{
    Scrollbar* vertScrollbar = getVertScrollbarWithoutUpdate();
    Scrollbar* horzScrollbar = getHorzScrollbarWithoutUpdate();
    vertScrollbar->hide();
    horzScrollbar->hide();

    Rectf renderArea(getTextRenderAreaWithoutUpdate());
    Sizef renderAreaSize(renderArea.getSize());

    d_renderedText.updateDynamicObjectExtents(d_window);
    d_renderedText.updateFormatting(renderAreaSize.d_width);

    Sizef documentSize(getTextExtentWithoutUpdate());

    bool showVert = d_enableVertScrollbar && (documentSize.d_height > renderAreaSize.d_height);
    bool showHorz = d_enableHorzScrollbar && (documentSize.d_width > renderAreaSize.d_width);
    vertScrollbar->setVisible(showVert);
    horzScrollbar->setVisible(showHorz);

    Rectf updatedRenderArea = getTextRenderAreaWithoutUpdate();
    if (renderArea != updatedRenderArea)
    {
        renderArea = updatedRenderArea;
        renderAreaSize = renderArea.getSize();
        d_renderedText.updateFormatting(renderAreaSize.d_width);
        documentSize = getTextExtentWithoutUpdate();

        showVert = d_enableVertScrollbar && (documentSize.d_height > renderAreaSize.d_height);
        showHorz = d_enableHorzScrollbar && (documentSize.d_width > renderAreaSize.d_width);
        vertScrollbar->setVisible(showVert);
        horzScrollbar->setVisible(showHorz);

        updatedRenderArea = getTextRenderAreaWithoutUpdate();
        if (renderArea != updatedRenderArea)
        {
            renderArea = updatedRenderArea;
            renderAreaSize = renderArea.getSize();
            d_renderedText.updateFormatting(renderAreaSize.d_width);
            documentSize = getTextExtentWithoutUpdate();
        }
    }

    d_window->performChildLayout(false, false);

    vertScrollbar->setDocumentSize(documentSize.d_height);
    vertScrollbar->setPageSize(renderAreaSize.d_height);
    vertScrollbar->setStepSize(std::max(1.0f, renderAreaSize.d_height / 10.0f));
    horzScrollbar->setDocumentSize(documentSize.d_width);
    horzScrollbar->setPageSize(renderAreaSize.d_width);
    horzScrollbar->setStepSize(std::max(1.0f, renderAreaSize.d_width / 10.0f));
}

//----------------------------------------------------------------------------//
bool FalagardStaticText::onTextChanged(const EventArgs&)
{
    invalidateFormatting();
    d_window->adjustSizeToContent();
    return true;
}

//----------------------------------------------------------------------------//
bool FalagardStaticText::onSized(const EventArgs&)
{
    invalidateFormatting();
    return true;
}

//----------------------------------------------------------------------------//
bool FalagardStaticText::onFontChanged(const EventArgs&)
{
    invalidateFormatting();
    d_window->adjustSizeToContent();
    return true;
}

//----------------------------------------------------------------------------//
bool FalagardStaticText::onScroll(const EventArgs& event)
{
    const ScrollEventArgs& e = static_cast<const ScrollEventArgs&>(event);
    return Scrollbar::standardProcessing(getVertScrollbar(), getHorzScrollbar(), -e.d_delta, e.d_modifiers.hasAlt());
}

//----------------------------------------------------------------------------//
bool FalagardStaticText::onIsSizeAdjustedToContentChanged(const EventArgs&)
{
    invalidateFormatting();
    return true;
}

//----------------------------------------------------------------------------//
bool FalagardStaticText::handleScrollbarChange(const EventArgs&)
{
    d_window->invalidate();
    return true;
}

//----------------------------------------------------------------------------//
void FalagardStaticText::onLookNFeelAssigned()
{
    // do initial scrollbar setup
    Scrollbar* vertScrollbar = getVertScrollbarWithoutUpdate();
    Scrollbar* horzScrollbar = getHorzScrollbarWithoutUpdate();

    vertScrollbar->hide();
    horzScrollbar->hide();

    d_connections.clear();

    // scrollbar events
    d_connections.push_back(
        vertScrollbar->subscribeEvent(Scrollbar::EventScrollPositionChanged,
            Event::Subscriber(&FalagardStaticText::handleScrollbarChange, this)));
    d_connections.push_back(
        horzScrollbar->subscribeEvent(Scrollbar::EventScrollPositionChanged,
            Event::Subscriber(&FalagardStaticText::handleScrollbarChange, this)));

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
}

//----------------------------------------------------------------------------//
void FalagardStaticText::onLookNFeelUnassigned()
{
    // clean up connections that rely on widgets created by the look and feel
    d_connections.clear();
}

//----------------------------------------------------------------------------//
Sizef FalagardStaticText::getTextExtent() const
{
    updateFormatting();
    return getTextExtentWithoutUpdate();
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
    return d_window->getEffectiveFont()->getFontHeight();
}

//----------------------------------------------------------------------------//
float FalagardStaticText::getVerticalAdvance() const
{
    return d_window->getEffectiveFont()->getFontHeight();
}

//----------------------------------------------------------------------------//
void FalagardStaticText::updateFormatting() const
{
    if (d_formatValid)
        return;

    d_actualHorzFormatting = d_horzFormatting;
    d_actualVertFormatting = d_vertFormatting;

    TextParser* parser = nullptr;
    if (isTextParsingEnabled())
    {
        parser = getTextParser();
        if (!parser)
            parser = CEGUI::System::getSingleton().getDefaultTextParser();
    }

    d_renderedText.renderText(d_window->getText(), parser, d_window->getEffectiveFont());

    configureScrollbars();

    if (d_window->isSizeAdjustedToContent() && !isSizeAdjustedToContentKeepingAspectRatio())
    {
        const auto lineCount = getNumOfFormattedTextLines();

        if (d_window->isWidthAdjustedToContent() && lineCount == 1)
            d_actualHorzFormatting = HorizontalTextFormatting::CentreAligned;

        d_renderedText.setHorizontalFormatting(d_actualHorzFormatting);
        d_renderedText.updateFormatting(getTextRenderAreaWithoutUpdate().getWidth());


        if (d_window->isHeightAdjustedToContent() && (d_numOfTextLinesToShow.isAuto() || d_numOfTextLinesToShow <= lineCount))
            d_actualVertFormatting = VerticalTextFormatting::CentreAligned;
    }

    d_formatValid = true;
}

//----------------------------------------------------------------------------//
bool FalagardStaticText::handleFontRenderSizeChange(const Font* const font)
{
    const bool res = WindowRenderer::handleFontRenderSizeChange(font);

    if (d_window->getEffectiveFont() == font)
    {
        invalidateFormatting();
        d_window->adjustSizeToContent();
        return true;
    }

    return res;
}

//----------------------------------------------------------------------------//
Scrollbar* FalagardStaticText::getVertScrollbarWithoutUpdate() const
{
    // return component created by look'n'feel assignment.
    return static_cast<Scrollbar*>(d_window->getChild(VertScrollbarName));
}

//----------------------------------------------------------------------------//
Scrollbar* FalagardStaticText::getHorzScrollbarWithoutUpdate() const
{
    // return component created by look'n'feel assignment.
    return static_cast<Scrollbar*>(d_window->getChild(HorzScrollbarName));
}

//----------------------------------------------------------------------------//
Rectf FalagardStaticText::getTextRenderAreaWithoutUpdate() const
{
    return getTextComponentAreaWithoutUpdate().getPixelRect(*d_window);
}

//----------------------------------------------------------------------------//
const ComponentArea& FalagardStaticText::getTextComponentAreaWithoutUpdate() const
{
    const bool v_visible = getVertScrollbarWithoutUpdate()->isVisible();
    const bool h_visible = getHorzScrollbarWithoutUpdate()->isVisible();

    // get WidgetLookFeel for the assigned look.
    const WidgetLookFeel& wlf = getLookNFeel();

    String area_name(d_frameEnabled ? "WithFrameTextRenderArea" : "NoFrameTextRenderArea");

    // if either of the scrollbars are visible, we might want to use a special rendering area
    if (v_visible || h_visible)
    {
        if (h_visible)
            area_name += 'H';
        if (v_visible)
            area_name += 'V';
        area_name += "Scroll";
    }

    if (wlf.isNamedAreaPresent(area_name))
        return wlf.getNamedArea(area_name).getArea();

    // default to plain WithFrameTextRenderArea
    return wlf.getNamedArea("WithFrameTextRenderArea").getArea();
}

//----------------------------------------------------------------------------//
const Sizef& FalagardStaticText::getTextExtentWithoutUpdate() const
{
    return d_renderedText.getExtents();
}

/*----------------------------------------------------------------------------//
    An implementation of "adjustSizeToContent" where we adjust both the window
    width and the window height simultaneously, keeping the window's aspect
    ratio according to "d_window->getAspectRatio()".

    We do that by try-and-error, using bisection.
------------------------------------------------------------------------------*/
void FalagardStaticText::adjustSizeToContent_wordWrap_keepingAspectRatio(
    const Sizef& contentMaxSize, USize& sizeFunc, float windowMaxWidth, float epsilon)
{
    // Start by trying height that can fit 0 text lines.
    Sizef window_size(0.f, sizeFunc.d_height.d_scale*epsilon + sizeFunc.d_height.d_offset);
    window_size.d_width = window_size.d_height * d_window->getAspectRatio();
    if (d_window->contentFitsForSpecifiedElementSize(window_size))
    {
        // It fits - so we go for that size.
        d_window->setSize(USize(UDim(0.f, window_size.d_width), UDim(0.f, window_size.d_height)));
        return;
    }

    /* It doesn't fit - so we try a positive integer number of text lines. Here
       we use try-and-error, using bisection.
       We only try heights in which we can fit exactly an integer number of text
       lines, as there's no logic in trying differently. */
    UDim height_sequence_precise(sizeFunc.d_height.d_scale*getVerticalAdvance() + sizeFunc.d_height.d_offset,
                                 sizeFunc.d_height.d_scale*getLineHeight() + sizeFunc.d_height.d_offset);
    UDim height_sequence(sizeFunc.d_height.d_scale*getVerticalAdvance() + sizeFunc.d_height.d_offset,
                         sizeFunc.d_height.d_scale*(getLineHeight()+epsilon) + sizeFunc.d_height.d_offset);
    float max_num_of_lines(std::max(
      static_cast<float>(d_renderedText.getParagraphCount() - 1),
      (windowMaxWidth / d_window->getAspectRatio() - height_sequence_precise.d_offset)
        / height_sequence_precise.d_scale));
    window_size = d_window->getSizeAdjustedToContent_bisection(
      USize(height_sequence *d_window->getAspectRatio(), height_sequence), -1.f, max_num_of_lines);
    d_window->setSize(USize(UDim(0.f, window_size.d_width), UDim(0.f, window_size.d_height)), false);

    /* It's possible that due to a too low "d_window->getMaxSize().d_height",
       we're unable to make the whole text fit without the need for a vertical
       scrollbar. In that case, we need to redo the computations, with the
       vertical scrollbar visible. We go for the maximal size that makes sense,
       which is the size of "orig_str", expanded to keep the aspect ratio
       "d_window->getAspectRatio()". */
    updateFormatting();
    if (getVertScrollbar()->isVisible())
    {
        sizeFunc.d_width = d_window->getElementWidthLowerBoundAsFuncOfWidthOfAreaReservedForContent();
        sizeFunc.d_height = d_window->getElementHeightLowerBoundAsFuncOfHeightOfAreaReservedForContent();
        window_size.d_width = (contentMaxSize.d_width+epsilon)*sizeFunc.d_width.d_scale + sizeFunc.d_width.d_offset;
        window_size.d_height = (contentMaxSize .d_height+epsilon)*sizeFunc.d_height.d_scale +
                                sizeFunc.d_height.d_offset;
        window_size.scaleToAspect(AspectMode::Expand, d_window->getAspectRatio());
        d_window->setSize(USize(UDim(0.f, window_size.d_width), UDim(0.f, window_size.d_height)), false);
    }
}

/*----------------------------------------------------------------------------//
    An implementation of "adjustSizeToContent" where we do the following:

    1) If "d_window->isHeightAdjustedToContent()" is true, adjust the height
       of the window so that the text fits in without the need for a vertical
       scrollbar. This case only happens when
       "d_numOfTextLinesToShow.isAuto()" is false, which means we know
       exactly how many text lines we want to reserve space for, regardless of
       word-wrapping.
    2) Adjust the window width by try-and-error, using bisection.
------------------------------------------------------------------------------*/
void FalagardStaticText::adjustSizeToContent_wordWrap_notKeepingAspectRatio(
    USize& sizeFunc, float contentMaxWidth, float windowMaxWidth, float epsilon)
{
    float height(d_window->isHeightAdjustedToContent()  ?
      sizeFunc.d_height.d_scale*(getContentSize().d_height+epsilon) + sizeFunc.d_height.d_offset  :
      d_window->getPixelSize().d_height);
    UDim height_as_u_dim(d_window->isHeightAdjustedToContent()  ?  UDim(0.f, height) : d_window->getHeight());
    float window_width(d_window->getSizeAdjustedToContent_bisection(
                         USize(UDim(1.f, 0.f), UDim(0.f, height)), -1.f, windowMaxWidth)
                       .d_width);
    d_window->setSize(USize(UDim(0.f, window_width), height_as_u_dim), false);

     /* It's possible that due to a too low height we're unable to make the
        whole text fit without the need for a vertical scrollbar. In that case,
        we need to redo the computations, with the vertical scrollbar visible.
        We go for the maximal width that makes sense, which is the width of the
        original string (i.e. not divided to lines by word-wrapping). */
    updateFormatting();
    if (getVertScrollbar()->isVisible())
    {
        sizeFunc.d_width = d_window->getElementWidthLowerBoundAsFuncOfWidthOfAreaReservedForContent();
        windowMaxWidth = (contentMaxWidth+epsilon)*sizeFunc.d_width.d_scale + sizeFunc.d_width.d_offset;
        d_window->setSize(USize(UDim(0.f, std::ceil(windowMaxWidth)), height_as_u_dim));
    }
}

/*----------------------------------------------------------------------------//
    An implementation of "adjustSizeToContent" where we do the following:

    Adjust the size of the window to the text by adjusting the width and the
    height independently of each other, and then, if necessary, fix it to comply
    with "d_window->getAspectMode()".
------------------------------------------------------------------------------*/
void FalagardStaticText::adjustSizeToContent_direct()
{
    updateFormatting();
    d_window->adjustSizeToContent_direct();

    /* The process may have to be repeated, because if, for example, word wrap
       is on, and "d_window->isHeightAdjustedToContent()" is true, adjusting
       the height might make the vertical scrollbar visible, in which case the
       word wrapping must be recomputed and then the height adjusted again. */
    if ((getVertScrollbar()->isVisible() || getHorzScrollbar()->isVisible())  &&
        (d_wordWrap ||
          (d_window->isWidthAdjustedToContent() && d_window->isHeightAdjustedToContent())))
    {
        updateFormatting();
        d_window->adjustSizeToContent_direct();
    }
}

}
