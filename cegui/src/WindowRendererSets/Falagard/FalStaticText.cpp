/***********************************************************************
    filename:   FalStaticText.cpp
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
#include "FalStaticText.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "CEGUIWindowManager.h"
#include "elements/CEGUIScrollbar.h"
#include "CEGUIEvent.h"
#include "CEGUIFont.h"
#include "CEGUILeftAlignedRenderedString.h"
#include "CEGUIRightAlignedRenderedString.h"
#include "CEGUICentredRenderedString.h"
#include "CEGUIJustifiedRenderedString.h"
#include "CEGUIRenderedStringWordWrapper.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const String FalagardStaticText::TypeName("Falagard/StaticText");

    /************************************************************************
        Properties
    *************************************************************************/
    FalagardStaticTextProperties::TextColours       FalagardStaticText::d_textColoursProperty;
    FalagardStaticTextProperties::VertFormatting    FalagardStaticText::d_vertFormattingProperty;
    FalagardStaticTextProperties::HorzFormatting    FalagardStaticText::d_horzFormattingProperty;
    FalagardStaticTextProperties::VertScrollbar     FalagardStaticText::d_vertScrollbarProperty;
    FalagardStaticTextProperties::HorzScrollbar     FalagardStaticText::d_horzScrollbarProperty;
    FalagardStaticTextProperties::HorzExtent        FalagardStaticText::d_horzExtentProperty;
    FalagardStaticTextProperties::VertExtent        FalagardStaticText::d_vertExtentProperty;

    /*************************************************************************
        Child Widget name suffix constants
    *************************************************************************/
    const String FalagardStaticText::VertScrollbarNameSuffix( "__auto_vscrollbar__" );
    const String FalagardStaticText::HorzScrollbarNameSuffix( "__auto_hscrollbar__" );

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
        registerProperty(&d_textColoursProperty);
        registerProperty(&d_vertFormattingProperty);
        registerProperty(&d_horzFormattingProperty);
        registerProperty(&d_vertScrollbarProperty);
        registerProperty(&d_horzScrollbarProperty);
        registerProperty(&d_horzExtentProperty, true);
        registerProperty(&d_vertExtentProperty, true);
    }

//----------------------------------------------------------------------------//
    FalagardStaticText::~FalagardStaticText()
    {
        delete d_formattedRenderedString;
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
        if (horzScrollbar->isVisible())
        {
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
                absarea.offset(Vector2f(-horzScrollbar->getScrollPosition(), 0));
                break;

            case HTF_RIGHT_ALIGNED:
            case HTF_WORDWRAP_RIGHT_ALIGNED:
                absarea.offset(Vector2f(horzScrollbar->getScrollPosition(), 0));
                break;
            }
        }

        // adjust y positioning according to formatting option
        float textHeight = d_formattedRenderedString->getVerticalExtent();
        const Scrollbar* const vertScrollbar = getVertScrollbar();
        switch(d_vertFormatting)
        {
        case VTF_TOP_ALIGNED:
            absarea.d_min.d_y -= vertScrollbar->getScrollPosition();
            break;

        case VTF_CENTRE_ALIGNED:
            // if scroll bar is in use, act like TopAligned
            if (vertScrollbar->isVisible())
                absarea.d_min.d_y -= vertScrollbar->getScrollPosition();
            // no scroll bar, so centre text instead.
            else
                absarea.d_min.d_y += PixelAligned((absarea.getHeight() - textHeight) * 0.5f);

            break;

        case VTF_BOTTOM_ALIGNED:
            absarea.d_min.d_y = absarea.d_max.d_y - textHeight + vertScrollbar->getScrollPosition();
            break;
        }

        // calculate final colours
        ColourRect final_cols(d_textCols);
        final_cols.modulateAlpha(d_window->getEffectiveAlpha());
        // cache the text for rendering.
        d_formattedRenderedString->draw(d_window->getGeometryBuffer(),
                                        absarea.getPosition(),
                                        &final_cols, &clipper);
    }

    /************************************************************************
        Returns the vertical scrollbar component
    *************************************************************************/
    Scrollbar* FalagardStaticText::getVertScrollbar(void) const
    {
        // return component created by look'n'feel assignment.
        return static_cast<Scrollbar*>(WindowManager::getSingleton().getWindow(d_window->getName() + VertScrollbarNameSuffix));
    }

    /************************************************************************
        Returns the horizontal scrollbar component
    *************************************************************************/
    Scrollbar* FalagardStaticText::getHorzScrollbar(void) const
    {
        // return component created by look'n'feel assignment.
        return static_cast<Scrollbar*>(WindowManager::getSingleton().getWindow(d_window->getName() + HorzScrollbarNameSuffix));
    }

    /************************************************************************
        Gets the text rendering area
    *************************************************************************/
    Rectf FalagardStaticText::getTextRenderArea(void) const
    {
        Scrollbar* vertScrollbar = getVertScrollbar();
        Scrollbar* horzScrollbar = getHorzScrollbar();
        bool v_visible = vertScrollbar->isVisible(true);
        bool h_visible = horzScrollbar->isVisible(true);

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

        return Sizef(d_formattedRenderedString->getHorizontalExtent(),
                      d_formattedRenderedString->getVerticalExtent());
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
    }

    /*************************************************************************
        Set whether the horizontal scroll bar will be shown if needed.    
    *************************************************************************/
    void FalagardStaticText::setHorizontalScrollbarEnabled(bool setting)
    {
        d_enableHorzScrollbar = setting;
        configureScrollbars();
        d_window->performChildWindowLayout();
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
        bool showVert = ((documentSize.d_height > renderAreaSize.d_height) && d_enableVertScrollbar);
        bool showHorz = ((documentSize.d_width > renderAreaSize.d_width) && d_enableHorzScrollbar);
        // show or hide vertical scroll bar
        if (showVert)
        {
            vertScrollbar->show();
        }
        else
        {
            vertScrollbar->hide();
        }

        // show or hide horizontal scroll bar
        if (showHorz)
        {
            horzScrollbar->show();
        }
        else
        {
            horzScrollbar->hide();
        }

        // if scrollbar visibility just changed we have might have a better TextRenderArea
        // if so we go with that instead
        Rectf updatedRenderArea = getTextRenderArea();
        if (renderArea!=updatedRenderArea)
        {
            renderArea = updatedRenderArea;
            renderAreaSize = renderArea.getSize();
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
        if (vertScrollbar->isVisible() && (vertScrollbar->getDocumentSize() > vertScrollbar->getPageSize()))
        {
            vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition() + vertScrollbar->getStepSize() * -e.wheelChange);
        }
        else if (horzScrollbar->isVisible() && (horzScrollbar->getDocumentSize() > horzScrollbar->getPageSize()))
        {
            horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition() + horzScrollbar->getStepSize() * -e.wheelChange);
        }

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
                new LeftAlignedRenderedString(d_window->getRenderedString());
            break;

        case HTF_RIGHT_ALIGNED:
            d_formattedRenderedString =
                new RightAlignedRenderedString(d_window->getRenderedString());
            break;

        case HTF_CENTRE_ALIGNED:
            d_formattedRenderedString =
                new CentredRenderedString(d_window->getRenderedString());
            break;

        case HTF_JUSTIFIED:
            d_formattedRenderedString =
                new JustifiedRenderedString(d_window->getRenderedString());
            break;

        case HTF_WORDWRAP_LEFT_ALIGNED:
            d_formattedRenderedString =
                new RenderedStringWordWrapper
                    <LeftAlignedRenderedString>(d_window->getRenderedString());
            break;

        case HTF_WORDWRAP_RIGHT_ALIGNED:
            d_formattedRenderedString =
                new RenderedStringWordWrapper
                    <RightAlignedRenderedString>(d_window->getRenderedString());
            break;

        case HTF_WORDWRAP_CENTRE_ALIGNED:
            d_formattedRenderedString =
                new RenderedStringWordWrapper
                    <CentredRenderedString>(d_window->getRenderedString());
            break;

        case HTF_WORDWRAP_JUSTIFIED:
            d_formattedRenderedString =
                new RenderedStringWordWrapper
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
        d_formattedRenderedString->getHorizontalExtent() :
        0.0f;
}

//----------------------------------------------------------------------------//
float FalagardStaticText::getVerticalTextExtent() const
{
    if (!d_formatValid)
        updateFormatting();

    return d_formattedRenderedString ?
        d_formattedRenderedString->getVerticalExtent() :
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

    d_formattedRenderedString->format(sz);
    d_formatValid = true;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
