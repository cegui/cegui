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
        d_horzFormatting(HTF_LEFT_ALIGNED),
        d_vertFormatting(VTF_CENTRE_ALIGNED),
        d_textCols(0xFFFFFFFF),
        d_enableVertScrollbar(false),
        d_enableHorzScrollbar(false),
        d_formattedRenderedString(0),
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
            HTF_LEFT_ALIGNED);


        CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardStaticText, VerticalTextFormatting,
            "VertFormatting", "Property to get/set the vertical formatting mode."
            "  Value is one of the VertFormatting strings.",
            &FalagardStaticText::setVerticalFormatting, &FalagardStaticText::getVerticalFormatting,
            VTF_CENTRE_ALIGNED);

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
            0, &FalagardStaticText::getHorizontalTextExtent,
            0);

        CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardStaticText, float,
            "VertExtent", "Property to get the current vertical extent of the formatted text string."
            "  Value is a float indicating the pixel extent.",
            0, &FalagardStaticText::getVerticalTextExtent,
            0);
    }

//----------------------------------------------------------------------------//
    FalagardStaticText::~FalagardStaticText()
    {
        if (d_formattedRenderedString)
            CEGUI_DELETE_AO d_formattedRenderedString;
    }

//----------------------------------------------------------------------------//

    /************************************************************************
        Populates the rendercache with imagery for this widget
    *************************************************************************/
    void FalagardStaticText::render()
    {
        // base class rendering
        FalagardStatic::render();

        // text rendering
        renderScrolledText();
    }

    /************************************************************************
        Caches the text according to scrollbar positions
    *************************************************************************/
    void FalagardStaticText::renderScrolledText()
    {
        // get destination area for the text.
        const Rectf clipper(getTextRenderArea());
        Rectf absarea(clipper);

        if (!d_formatValid)
            updateFormatting(clipper.getSize());

        // see if we may need to adjust horizontal position
        const Scrollbar* const horzScrollbar = getHorzScrollbar();
        if (horzScrollbar->isEffectiveVisible())
        {
            const float range = horzScrollbar->getDocumentSize() -
                                horzScrollbar->getPageSize();

            switch(d_horzFormatting)
            {
            case HTF_LEFT_ALIGNED:
            case HTF_WORDWRAP_LEFT_ALIGNED:
            case HTF_JUSTIFIED:
            case HTF_WORDWRAP_JUSTIFIED:
                absarea.offset(Vector2f(-horzScrollbar->getScrollPosition(), 0));
                break;

            case HTF_CENTRE_ALIGNED:
            case HTF_WORDWRAP_CENTRE_ALIGNED:
                absarea.setWidth(horzScrollbar->getDocumentSize());
                absarea.offset(Vector2f(range / 2 - horzScrollbar->getScrollPosition(), 0));
                break;

            case HTF_RIGHT_ALIGNED:
            case HTF_WORDWRAP_RIGHT_ALIGNED:
                absarea.offset(Vector2f(range - horzScrollbar->getScrollPosition(), 0));
                break;
            }
        }

        // adjust y positioning according to formatting option
        float textHeight = d_formattedRenderedString->getVerticalExtent(d_window);
        const Scrollbar* const vertScrollbar = getVertScrollbar();
        const float vertScrollPosition = vertScrollbar->getScrollPosition();
        // if scroll bar is in use, position according to that.
        if (vertScrollbar->isEffectiveVisible())
            absarea.d_min.d_y -= vertScrollPosition;
        // no scrollbar, so adjust position according to formatting set.
        else
            switch(d_vertFormatting)
            {
            case VTF_CENTRE_ALIGNED:
                absarea.d_min.d_y += CoordConverter::alignToPixels((absarea.getHeight() - textHeight) * 0.5f);
                break;

            case VTF_BOTTOM_ALIGNED:
                absarea.d_min.d_y = absarea.d_max.d_y - textHeight;
                break;
            }

        // calculate final colours
        ColourRect final_cols(d_textCols);
        final_cols.modulateAlpha(d_window->getEffectiveAlpha());
        // cache the text for rendering.
        d_formattedRenderedString->draw(d_window, d_window->getGeometryBuffer(),
                                        absarea.getPosition(),
                                        &final_cols, &clipper);
    }

    /************************************************************************
        Returns the vertical scrollbar component
    *************************************************************************/
    Scrollbar* FalagardStaticText::getVertScrollbar(void) const
    {
        // return component created by look'n'feel assignment.
        return static_cast<Scrollbar*>(d_window->getChild(VertScrollbarName));
    }

    /************************************************************************
        Returns the horizontal scrollbar component
    *************************************************************************/
    Scrollbar* FalagardStaticText::getHorzScrollbar(void) const
    {
        // return component created by look'n'feel assignment.
        return static_cast<Scrollbar*>(d_window->getChild(HorzScrollbarName));
    }

    /************************************************************************
        Gets the text rendering area
    *************************************************************************/
    Rectf FalagardStaticText::getTextRenderArea(void) const
    {
        Scrollbar* vertScrollbar = getVertScrollbar();
        Scrollbar* horzScrollbar = getHorzScrollbar();
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

        if (wlf.isNamedAreaDefined(area_name))
        {
            return wlf.getNamedArea(area_name).getArea().getPixelRect(*d_window);
        }

        // default to plain WithFrameTextRenderArea
        return wlf.getNamedArea("WithFrameTextRenderArea").getArea().getPixelRect(*d_window);
    }

    /************************************************************************
        Gets the pixel size of the document
    *************************************************************************/
    Sizef FalagardStaticText::getDocumentSize(const Rectf& renderArea) const
    {
        if (!d_formatValid)
            updateFormatting(renderArea.getSize());

        return Sizef(d_formattedRenderedString->getHorizontalExtent(d_window),
                      d_formattedRenderedString->getVerticalExtent(d_window));
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
        d_vertFormatting = v_fmt;
        configureScrollbars();
        d_window->invalidate();
    }

    /*************************************************************************
        Set the formatting required for the text.    
    *************************************************************************/
    void FalagardStaticText::setHorizontalFormatting(HorizontalTextFormatting h_fmt)
    {
        if (h_fmt == d_horzFormatting)
            return;

        d_horzFormatting = h_fmt;
        setupStringFormatter();
        configureScrollbars();
        d_window->invalidate();
    }

    /*************************************************************************
        Set whether the vertical scroll bar will be shown if needed.    
    *************************************************************************/
    void FalagardStaticText::setVerticalScrollbarEnabled(bool setting)
    {
        d_enableVertScrollbar = setting;
        configureScrollbars();
        d_window->performChildWindowLayout();
        d_formatValid = false;
        d_window->invalidate();
    }

    /*************************************************************************
        Set whether the horizontal scroll bar will be shown if needed.    
    *************************************************************************/
    void FalagardStaticText::setHorizontalScrollbarEnabled(bool setting)
    {
        d_enableHorzScrollbar = setting;
        configureScrollbars();
        d_window->performChildWindowLayout();
        d_formatValid = false;
        d_window->invalidate();
    }

    /*************************************************************************
        display required integrated scroll bars according to current state
        of the edit box and update their values.
    *************************************************************************/
    void FalagardStaticText::configureScrollbars(void)
    {
        // get the scrollbars
        Scrollbar* vertScrollbar = getVertScrollbar();
        Scrollbar* horzScrollbar = getHorzScrollbar();

        // get the sizes we need
        Rectf renderArea(getTextRenderArea());
        Sizef renderAreaSize(renderArea.getSize());
        Sizef documentSize(getDocumentSize(renderArea));

        // show or hide vertical scroll bar as required (or as specified by option)
        const bool showVert = ((documentSize.d_height > renderAreaSize.d_height) && d_enableVertScrollbar);
        const bool showHorz = ((documentSize.d_width > renderAreaSize.d_width) && d_enableHorzScrollbar);
 
        vertScrollbar->setVisible(showVert);
        horzScrollbar->setVisible(showHorz);

        // if scrollbar visibility just changed we have might have a better TextRenderArea
        // if so we go with that instead
        const Rectf updatedRenderArea = getTextRenderArea();
        if (renderArea!=updatedRenderArea)
        {
            d_formatValid = false;
            renderArea = updatedRenderArea;
            renderAreaSize = renderArea.getSize();
            documentSize = getDocumentSize(renderArea);
        }

        // Set up scroll bar values
        vertScrollbar->setDocumentSize(documentSize.d_height);
        vertScrollbar->setPageSize(renderAreaSize.d_height);
        vertScrollbar->setStepSize(ceguimax(1.0f, renderAreaSize.d_height / 10.0f));

        horzScrollbar->setDocumentSize(documentSize.d_width);
        horzScrollbar->setPageSize(renderAreaSize.d_width);
        horzScrollbar->setStepSize(ceguimax(1.0f, renderAreaSize.d_width / 10.0f));
    }

    /*************************************************************************
        Handler called when text is changed.
    *************************************************************************/
    bool FalagardStaticText::onTextChanged(const EventArgs&)
    {
        d_formatValid = false;
        configureScrollbars();
        d_window->invalidate();
        return true;
    }


    /*************************************************************************
        Handler called when size is changed
    *************************************************************************/
    bool FalagardStaticText::onSized(const EventArgs&)
    {
        d_formatValid = false;
        configureScrollbars();
        return true;
    }


    /*************************************************************************
        Handler called when font is changed.
    *************************************************************************/
    bool FalagardStaticText::onFontChanged(const EventArgs&)
    {
        d_formatValid = false;
        configureScrollbars();
        d_window->invalidate();
        return true;
    }


    /*************************************************************************
        Handler for mouse wheel changes
    *************************************************************************/
    bool FalagardStaticText::onMouseWheel(const EventArgs& event)
    {
        const MouseEventArgs& e = static_cast<const MouseEventArgs&>(event);

        Scrollbar* vertScrollbar = getVertScrollbar();
        Scrollbar* horzScrollbar = getHorzScrollbar();

        const bool vertScrollbarVisible = vertScrollbar->isEffectiveVisible();
        const bool horzScrollbarVisible = horzScrollbar->isEffectiveVisible();

        if (vertScrollbarVisible && (vertScrollbar->getDocumentSize() > vertScrollbar->getPageSize()))
        {
            vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition() + vertScrollbar->getStepSize() * -e.wheelChange);
        }
        else if (horzScrollbarVisible && (horzScrollbar->getDocumentSize() > horzScrollbar->getPageSize()))
        {
            horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition() + horzScrollbar->getStepSize() * -e.wheelChange);
        }

        return vertScrollbarVisible || horzScrollbarVisible;
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
        Scrollbar* vertScrollbar = getVertScrollbar();
        Scrollbar* horzScrollbar = getHorzScrollbar();

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
            d_window->subscribeEvent(Window::EventMouseWheel,
                Event::Subscriber(&FalagardStaticText::onMouseWheel, this)));
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
        d_formattedRenderedString = 0;
        d_formatValid = false;

        // create new formatter of whichever type...
        switch(d_horzFormatting)
        {
        case HTF_LEFT_ALIGNED:
            d_formattedRenderedString =
                CEGUI_NEW_AO LeftAlignedRenderedString(d_window->getRenderedString());
            break;

        case HTF_RIGHT_ALIGNED:
            d_formattedRenderedString =
                CEGUI_NEW_AO RightAlignedRenderedString(d_window->getRenderedString());
            break;

        case HTF_CENTRE_ALIGNED:
            d_formattedRenderedString =
                CEGUI_NEW_AO CentredRenderedString(d_window->getRenderedString());
            break;

        case HTF_JUSTIFIED:
            d_formattedRenderedString =
                CEGUI_NEW_AO JustifiedRenderedString(d_window->getRenderedString());
            break;

        case HTF_WORDWRAP_LEFT_ALIGNED:
            d_formattedRenderedString =
                CEGUI_NEW_AO RenderedStringWordWrapper
                    <LeftAlignedRenderedString>(d_window->getRenderedString());
            break;

        case HTF_WORDWRAP_RIGHT_ALIGNED:
            d_formattedRenderedString =
                CEGUI_NEW_AO RenderedStringWordWrapper
                    <RightAlignedRenderedString>(d_window->getRenderedString());
            break;

        case HTF_WORDWRAP_CENTRE_ALIGNED:
            d_formattedRenderedString =
                CEGUI_NEW_AO RenderedStringWordWrapper
                    <CentredRenderedString>(d_window->getRenderedString());
            break;

        case HTF_WORDWRAP_JUSTIFIED:
            d_formattedRenderedString =
                CEGUI_NEW_AO RenderedStringWordWrapper
                    <JustifiedRenderedString>(d_window->getRenderedString());
            break;
        }
    }

//----------------------------------------------------------------------------//
float FalagardStaticText::getHorizontalTextExtent() const
{
    if (!d_formatValid)
        updateFormatting();

    return d_formattedRenderedString ?
        d_formattedRenderedString->getHorizontalExtent(d_window) :
        0.0f;
}

//----------------------------------------------------------------------------//
float FalagardStaticText::getVerticalTextExtent() const
{
    if (!d_formatValid)
        updateFormatting();

    return d_formattedRenderedString ?
        d_formattedRenderedString->getVerticalExtent(d_window) :
        0.0f;
}

//----------------------------------------------------------------------------//
void FalagardStaticText::updateFormatting() const
{
    updateFormatting(getTextRenderArea().getSize());
}

//----------------------------------------------------------------------------//
void FalagardStaticText::updateFormatting(const Sizef& sz) const
{
    if (!d_window)
        return;

    if (!d_formattedRenderedString)
        setupStringFormatter();

    // 'touch' the window's rendered string to ensure it's re-parsed if needed.
    d_window->getRenderedString();

    d_formattedRenderedString->format(d_window, sz);
    d_formatValid = true;
}

//----------------------------------------------------------------------------//
bool FalagardStaticText::handleFontRenderSizeChange(const Font* const font)
{
    const bool res = WindowRenderer::handleFontRenderSizeChange(font);

    if (d_window->getFont() == font)
    {
        d_window->invalidate();
        d_formatValid = false;
        return true;
    }

    return res;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
