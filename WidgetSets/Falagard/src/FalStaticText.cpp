/************************************************************************
    filename:   FalStaticText.cpp
    created:    Tue Jul 5 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
 
    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "FalStaticText.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "CEGUIWindowManager.h"
#include "elements/CEGUIScrollbar.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const utf8 FalagardStaticText::WidgetTypeName[] = "Falagard/StaticText";

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
    FalagardStaticText::FalagardStaticText(const String& type, const String& name) :
        FalagardStatic(type, name),
        d_horzFormatting(LeftAligned),
        d_vertFormatting(VertCentred),
        d_textCols(0xFFFFFFFF),
        d_enableVertScrollbar(false),
        d_enableHorzScrollbar(false)
    {
        addProperty(&d_textColoursProperty);
        addProperty(&d_vertFormattingProperty);
        addProperty(&d_horzFormattingProperty);
        addProperty(&d_vertScrollbarProperty);
        addProperty(&d_horzScrollbarProperty);
    }

    /************************************************************************
        Deconstructor
    *************************************************************************/
    FalagardStaticText::~FalagardStaticText()
    {
    }

    /*************************************************************************
        Perform initialisation for the widget.
    *************************************************************************/
    void FalagardStaticText::initialise(void)
    {
        FalagardStatic::initialise();

        // do initial scrollbar setup
        Scrollbar* vertScrollbar = getVertScrollbar();
        Scrollbar* horzScrollbar = getHorzScrollbar();

        vertScrollbar->hide();
        horzScrollbar->hide();

        addChildWindow(vertScrollbar);
        addChildWindow(horzScrollbar);

        performChildWindowLayout();

        // event subscription
        vertScrollbar->subscribeEvent(Scrollbar::EventScrollPositionChanged, Event::Subscriber(&FalagardStaticText::handleScrollbarChange, this));
        horzScrollbar->subscribeEvent(Scrollbar::EventScrollPositionChanged, Event::Subscriber(&FalagardStaticText::handleScrollbarChange, this));
    }

    /************************************************************************
        Populates the rendercache with imagery for this widget
    *************************************************************************/
    void FalagardStaticText::populateRenderCache()
    {
        // base class rendering
        FalagardStatic::populateRenderCache();

        // text rendering
        renderScrolledText();
    }

    /************************************************************************
        Caches the text according to scrollbar positions
    *************************************************************************/
    void FalagardStaticText::renderScrolledText()
    {
        const Font* font = getFont();
        // can't render text without a font :)
        if (font == 0)
            return;

        // get destination area for the text.
        Rect absarea(getTextRenderArea());
        Rect clipper(absarea);

        float textHeight = font->getFormattedLineCount(d_text, absarea, (TextFormatting)d_horzFormatting) * font->getLineSpacing();

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

        // calculate final colours
        ColourRect final_cols(d_textCols);
        final_cols.modulateAlpha(getEffectiveAlpha());
        // cache the text for rendering.
        d_renderCache.cacheText(d_text, font, (TextFormatting)d_horzFormatting, absarea, 0, final_cols, &clipper);
    }

    /************************************************************************
        Returns the vertical scrollbar component
    *************************************************************************/
    Scrollbar* FalagardStaticText::getVertScrollbar(void) const
    {
        // return component created by look'n'feel assignment.
        return static_cast<Scrollbar*>(WindowManager::getSingleton().getWindow(getName() + VertScrollbarNameSuffix));
    }

    /************************************************************************
        Returns the horizontal scrollbar component
    *************************************************************************/
    Scrollbar* FalagardStaticText::getHorzScrollbar(void) const
    {
        // return component created by look'n'feel assignment.
        return static_cast<Scrollbar*>(WindowManager::getSingleton().getWindow(getName() + HorzScrollbarNameSuffix));
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
        const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);

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
            return wlf.getNamedArea(area_name).getArea().getPixelRect(*this);
        }

        // default to plain WithFrameTextRenderArea
        return wlf.getNamedArea("WithFrameTextRenderArea").getArea().getPixelRect(*this);
    }

    /************************************************************************
        Gets the pixel size of the document
    *************************************************************************/
    Size FalagardStaticText::getDocumentSize(const Rect& renderArea) const
    {
        // we need a font to really measure anything
        const Font* font = getFont();
        if (font==0)
        {
            return Size(0,0);
        }
        // return the total extent of the text
        float totalHeight = font->getFormattedLineCount(d_text, renderArea, (TextFormatting)d_horzFormatting) * font->getLineSpacing();
        float widestItem  = font->getFormattedTextExtent(d_text, renderArea, (TextFormatting)d_horzFormatting);
        return Size(widestItem,totalHeight);
    }

    /*************************************************************************
        Sets the colours to be applied when rendering the text.    
    *************************************************************************/
    void FalagardStaticText::setTextColours(const ColourRect& colours)
    {
        d_textCols = colours;
        requestRedraw();
    }

    /*************************************************************************
        Set the formatting required for the text.    
    *************************************************************************/
    void FalagardStaticText::setVerticalFormatting(VertFormatting v_fmt)
    {
        d_vertFormatting = v_fmt;
        configureScrollbars();
        requestRedraw();
    }

    /*************************************************************************
        Set the formatting required for the text.    
    *************************************************************************/
    void FalagardStaticText::setHorizontalFormatting(HorzFormatting h_fmt)
    {
        d_horzFormatting = h_fmt;
        configureScrollbars();
        requestRedraw();
    }

    /*************************************************************************
        Set whether the vertical scroll bar will be shown if needed.    
    *************************************************************************/
    void FalagardStaticText::setVerticalScrollbarEnabled(bool setting)
    {
        d_enableVertScrollbar = setting;
        configureScrollbars();
        performChildWindowLayout();
    }

    /*************************************************************************
        Set whether the horizontal scroll bar will be shown if needed.    
    *************************************************************************/
    void FalagardStaticText::setHorizontalScrollbarEnabled(bool setting)
    {
        d_enableHorzScrollbar = setting;
        configureScrollbars();
        performChildWindowLayout();
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
    void FalagardStaticText::onTextChanged(WindowEventArgs& e)
    {
        FalagardStatic::onTextChanged(e);

        configureScrollbars();
        requestRedraw();
    }


    /*************************************************************************
        Handler called when size is changed
    *************************************************************************/
    void FalagardStaticText::onSized(WindowEventArgs& e)
    {
        FalagardStatic::onSized(e);

        configureScrollbars();
    }


    /*************************************************************************
        Handler called when font is changed.
    *************************************************************************/
    void FalagardStaticText::onFontChanged(WindowEventArgs& e)
    {
        FalagardStatic::onFontChanged(e);

        configureScrollbars();
        requestRedraw();
    }


    /*************************************************************************
        Handler for mouse wheel changes
    *************************************************************************/
    void FalagardStaticText::onMouseWheel(MouseEventArgs& e)
    {
        // base class processing.
        FalagardStatic::onMouseWheel(e);

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

        e.handled = true;
    }


    /*************************************************************************
        Handler called when the scroll bar positions change
    *************************************************************************/
    bool FalagardStaticText::handleScrollbarChange(const EventArgs& e)
    {
        requestRedraw();
        return true;
    }

} // End of  CEGUI namespace section
