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

// Start of CEGUI namespace section
namespace CEGUI
{
    const utf8 FalagardStaticText::TypeName[] = "Falagard/StaticText";

    /************************************************************************
        Properties
    *************************************************************************/
    FalagardStaticTextProperties::TextColours       FalagardStaticText::d_textColoursProperty;
    FalagardStaticTextProperties::VertFormatting    FalagardStaticText::d_vertFormattingProperty;
    FalagardStaticTextProperties::HorzFormatting    FalagardStaticText::d_horzFormattingProperty;
    FalagardStaticTextProperties::VertScrollbar     FalagardStaticText::d_vertScrollbarProperty;
    FalagardStaticTextProperties::HorzScrollbar     FalagardStaticText::d_horzScrollbarProperty;

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
        d_horzFormatting(LeftAligned),
        d_vertFormatting(VertCentred),
        d_textCols(0xFFFFFFFF),
        d_enableVertScrollbar(false),
        d_enableHorzScrollbar(false)
    {
        registerProperty(&d_textColoursProperty);
        registerProperty(&d_vertFormattingProperty);
        registerProperty(&d_horzFormattingProperty);
        registerProperty(&d_vertScrollbarProperty);
        registerProperty(&d_horzScrollbarProperty);
    }

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
        Font* font = d_window->getFont();
        // can't render text without a font :)
        if (font == 0)
            return;

        // get destination area for the text.
        Rect absarea(getTextRenderArea());
        Rect clipper(absarea);

        float textHeight = font->getFormattedLineCount(d_window->getText(), absarea, (TextFormatting)d_horzFormatting) * font->getLineSpacing();

        Scrollbar* vertScrollbar = getVertScrollbar();
        Scrollbar* horzScrollbar = getHorzScrollbar();

        // see if we may need to adjust horizontal position
        if (horzScrollbar->isVisible())
        {
            switch(d_horzFormatting)
            {
            case LeftAligned:
            case WordWrapLeftAligned:
            case Justified:
            case WordWrapJustified:
                absarea.offset(Point(-horzScrollbar->getScrollPosition(), 0));
                break;

            case Centred:
            case WordWrapCentred:
                absarea.setWidth(horzScrollbar->getDocumentSize());
                absarea.offset(Point(-horzScrollbar->getScrollPosition(), 0));
                break;

            case RightAligned:
            case WordWrapRightAligned:
                absarea.offset(Point(horzScrollbar->getScrollPosition(), 0));
                break;
            }

        }

        // adjust y positioning according to formatting option
        switch(d_vertFormatting)
        {
        case TopAligned:
            absarea.d_top -= vertScrollbar->getScrollPosition();
            break;

        case VertCentred:
            // if scroll bar is in use, act like TopAligned
            if (vertScrollbar->isVisible())
            {
                absarea.d_top -= vertScrollbar->getScrollPosition();
            }
            // no scroll bar, so centre text instead.
            else
            {
                absarea.d_top += PixelAligned((absarea.getHeight() - textHeight) * 0.5f);
            }

            break;

        case BottomAligned:
            absarea.d_top = absarea.d_bottom - textHeight;
            absarea.d_top += vertScrollbar->getScrollPosition();
            break;
        }

        // offset the font little down so that it's centered within its own spacing
        absarea.d_top += (font->getLineSpacing() - font->getFontHeight()) * 0.5f;
        // calculate final colours
        ColourRect final_cols(d_textCols);
        final_cols.modulateAlpha(d_window->getEffectiveAlpha());
        // cache the text for rendering.
        d_window->getRenderCache().cacheText(d_window->getText(), font, (TextFormatting)d_horzFormatting, absarea, 0, final_cols, &clipper);
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
    Rect FalagardStaticText::getTextRenderArea(void) const
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
    Size FalagardStaticText::getDocumentSize(const Rect& renderArea) const
    {
        // we need a font to really measure anything
        Font* font = d_window->getFont();
        if (font==0)
        {
            return Size(0,0);
        }
        // return the total extent of the text
        float totalHeight = font->getFormattedLineCount(d_window->getText(), renderArea, (TextFormatting)d_horzFormatting) * font->getLineSpacing();
        float widestItem  = font->getFormattedTextExtent(d_window->getText(), renderArea, (TextFormatting)d_horzFormatting);
        return Size(widestItem,totalHeight);
    }

    /*************************************************************************
        Sets the colours to be applied when rendering the text.    
    *************************************************************************/
    void FalagardStaticText::setTextColours(const ColourRect& colours)
    {
        d_textCols = colours;
        d_window->requestRedraw();
    }

    /*************************************************************************
        Set the formatting required for the text.    
    *************************************************************************/
    void FalagardStaticText::setVerticalFormatting(VertFormatting v_fmt)
    {
        d_vertFormatting = v_fmt;
        configureScrollbars();
        d_window->requestRedraw();
    }

    /*************************************************************************
        Set the formatting required for the text.    
    *************************************************************************/
    void FalagardStaticText::setHorizontalFormatting(HorzFormatting h_fmt)
    {
        d_horzFormatting = h_fmt;
        configureScrollbars();
        d_window->requestRedraw();
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
        Rect renderArea(getTextRenderArea());
        Size renderAreaSize(renderArea.getSize());
        Size documentSize(getDocumentSize(renderArea));

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
        Rect updatedRenderArea = getTextRenderArea();
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
    bool FalagardStaticText::onTextChanged(const EventArgs& e)
    {
        configureScrollbars();
        d_window->requestRedraw();
        return true;
    }


    /*************************************************************************
        Handler called when size is changed
    *************************************************************************/
    bool FalagardStaticText::onSized(const EventArgs& e)
    {
        configureScrollbars();
        return true;
    }


    /*************************************************************************
        Handler called when font is changed.
    *************************************************************************/
    bool FalagardStaticText::onFontChanged(const EventArgs& event)
    {
        configureScrollbars();
        d_window->requestRedraw();
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
    bool FalagardStaticText::handleScrollbarChange(const EventArgs& e)
    {
        d_window->requestRedraw();
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

} // End of  CEGUI namespace section
