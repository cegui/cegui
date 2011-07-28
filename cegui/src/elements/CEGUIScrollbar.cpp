/***********************************************************************
    filename:   CEGUIScrollbar.cpp
    created:    13/4/2004
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
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
#include "elements/CEGUIScrollbar.h"
#include "elements/CEGUIThumb.h"
#include "CEGUIWindowManager.h"
#include "CEGUIExceptions.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
const String Scrollbar::EventNamespace("Scrollbar");
const String Scrollbar::WidgetTypeName("CEGUI/Scrollbar");

//----------------------------------------------------------------------------//
const String Scrollbar::EventScrollPositionChanged( "ScrollPositionChanged" );
const String Scrollbar::EventThumbTrackStarted("ThumbTrackStarted");
const String Scrollbar::EventThumbTrackEnded("ThumbTrackEnded");
const String Scrollbar::EventScrollConfigChanged("ScrollConfigChanged");

//----------------------------------------------------------------------------//
const String Scrollbar::ThumbName("__auto_thumb__");
const String Scrollbar::IncreaseButtonName("__auto_incbtn__");
const String Scrollbar::DecreaseButtonName("__auto_decbtn__");

//----------------------------------------------------------------------------//
ScrollbarWindowRenderer::ScrollbarWindowRenderer(const String& name) :
        WindowRenderer(name, Scrollbar::EventNamespace)
{
}

//----------------------------------------------------------------------------//
Scrollbar::Scrollbar(const String& type, const String& name) :
    Window(type, name),
    d_documentSize(1.0f),
    d_pageSize(0.0f),
    d_stepSize(1.0f),
    d_overlapSize(0.0f),
    d_position(0.0f),
    d_endLockPosition(false)
{
    addScrollbarProperties();
}

//----------------------------------------------------------------------------//
Scrollbar::~Scrollbar(void)
{
}

//----------------------------------------------------------------------------//
void Scrollbar::initialiseComponents(void)
{
    // Set up thumb
    Thumb* const t = getThumb();
    t->subscribeEvent(Thumb::EventThumbPositionChanged,
                      Event::Subscriber(&CEGUI::Scrollbar::handleThumbMoved,
                      this));

    t->subscribeEvent(Thumb::EventThumbTrackStarted,
                      Event::Subscriber(&CEGUI::Scrollbar::handleThumbTrackStarted,
                      this));

    t->subscribeEvent(Thumb::EventThumbTrackEnded,
                      Event::Subscriber(&CEGUI::Scrollbar::handleThumbTrackEnded,
                      this));

    // set up Increase button
    getIncreaseButton()->
        subscribeEvent(PushButton::EventMouseButtonDown,
                       Event::Subscriber(&CEGUI::Scrollbar::handleIncreaseClicked,
                       this));

    // set up Decrease button
    getDecreaseButton()->
        subscribeEvent(PushButton::EventMouseButtonDown,
                       Event::Subscriber(&CEGUI::Scrollbar::handleDecreaseClicked,
                       this));

    // do initial layout
    performChildWindowLayout();
}

//----------------------------------------------------------------------------//
void Scrollbar::setDocumentSize(float document_size)
{
    if (d_documentSize != document_size)
    {
        const bool reset_max_position = d_endLockPosition && isAtEnd();

        d_documentSize = document_size;

        if (reset_max_position)
            setScrollPosition(getMaxScrollPosition());
        else
            updateThumb();

        WindowEventArgs args(this);
        onScrollConfigChanged(args);
    }
}

//----------------------------------------------------------------------------//
void Scrollbar::setPageSize(float page_size)
{
    if (d_pageSize != page_size)
    {
        const bool reset_max_position = d_endLockPosition && isAtEnd();

        d_pageSize = page_size;

        if (reset_max_position)
            setScrollPosition(getMaxScrollPosition());
        else
            updateThumb();

        WindowEventArgs args(this);
        onScrollConfigChanged(args);
    }
}

//----------------------------------------------------------------------------//
void Scrollbar::setStepSize(float step_size)
{
    if (d_stepSize != step_size)
    {
        d_stepSize = step_size;

        WindowEventArgs args(this);
        onScrollConfigChanged(args);
    }
}

//----------------------------------------------------------------------------//
void Scrollbar::setOverlapSize(float overlap_size)
{
    if (d_overlapSize != overlap_size)
    {
        d_overlapSize = overlap_size;

        WindowEventArgs args(this);
        onScrollConfigChanged(args);
    }
}

//----------------------------------------------------------------------------//
void Scrollbar::setScrollPosition(float position)
{
    const bool modified = setScrollPosition_impl(position);
    updateThumb();

    // notification if required
    if (modified)
    {
        WindowEventArgs args(this);
        onScrollPositionChanged(args);
    }
}

//----------------------------------------------------------------------------//
void Scrollbar::onScrollPositionChanged(WindowEventArgs& e)
{
    fireEvent(EventScrollPositionChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Scrollbar::onThumbTrackStarted(WindowEventArgs& e)
{
    fireEvent(EventThumbTrackStarted, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Scrollbar::onThumbTrackEnded(WindowEventArgs& e)
{
    fireEvent(EventThumbTrackEnded, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Scrollbar::onScrollConfigChanged(WindowEventArgs& e)
{
    performChildWindowLayout();
    fireEvent(EventScrollConfigChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Scrollbar::onMouseButtonDown(MouseEventArgs& e)
{
    // base class processing
    Window::onMouseButtonDown(e);

    if (e.button == LeftButton)
    {
        const float adj = getAdjustDirectionFromPoint(e.position);

        // adjust scroll bar position in whichever direction as required.
        if (adj != 0)
            setScrollPosition(
                d_position + ((d_pageSize - d_overlapSize) * adj));

        ++e.handled;
    }
}

//----------------------------------------------------------------------------//
void Scrollbar::onMouseWheel(MouseEventArgs& e)
{
    // base class processing
    Window::onMouseWheel(e);

    // scroll by e.wheelChange * stepSize
    setScrollPosition(d_position + d_stepSize * -e.wheelChange);

    // ensure the message does not go to our parent.
    ++e.handled;
}

//----------------------------------------------------------------------------//
bool Scrollbar::handleThumbMoved(const EventArgs&)
{
    // adjust scroll bar position as required.
    setScrollPosition(getValueFromThumb());

    return true;
}

//----------------------------------------------------------------------------//
bool Scrollbar::handleIncreaseClicked(const EventArgs& e)
{
    if (((const MouseEventArgs&)e).button == LeftButton)
    {
        // adjust scroll bar position as required.
        setScrollPosition(d_position + d_stepSize);

        return true;
    }

    return false;
}

//----------------------------------------------------------------------------//
bool Scrollbar::handleDecreaseClicked(const EventArgs& e)
{
    if (((const MouseEventArgs&)e).button == LeftButton)
    {
        // adjust scroll bar position as required.
        setScrollPosition(d_position - d_stepSize);

        return true;
    }

    return false;
}

//----------------------------------------------------------------------------//
bool Scrollbar::handleThumbTrackStarted(const EventArgs&)
{
    // simply trigger our own version of this event
    WindowEventArgs args(this);
    onThumbTrackStarted(args);

    return true;
}

//----------------------------------------------------------------------------//
bool Scrollbar::handleThumbTrackEnded(const EventArgs&)
{
    // simply trigger our own version of this event
    WindowEventArgs args(this);
    onThumbTrackEnded(args);

    return true;
}

//----------------------------------------------------------------------------//
void Scrollbar::addScrollbarProperties(void)
{
    const String propertyOrigin("Scrollbar");

    CEGUI_DEFINE_PROPERTY(Scrollbar, float,
        "DocumentSize", "Property to get/set the document size for the Scrollbar.  Value is a float.",
        &Scrollbar::setDocumentSize, &Scrollbar::getDocumentSize, 1.0f
    );
    CEGUI_DEFINE_PROPERTY(Scrollbar, float,
        "PageSize", "Property to get/set the page size for the Scrollbar.  Value is a float.",
        &Scrollbar::setPageSize, &Scrollbar::getPageSize, 0.0f
    );
    CEGUI_DEFINE_PROPERTY(Scrollbar, float,
        "StepSize", "Property to get/set the step size for the Scrollbar.  Value is a float.",
        &Scrollbar::setStepSize, &Scrollbar::getStepSize, 1.0f
    );
    CEGUI_DEFINE_PROPERTY(Scrollbar, float,
        "OverlapSize", "Property to get/set the overlap size for the Scrollbar.  Value is a float.",
        &Scrollbar::setOverlapSize, &Scrollbar::getOverlapSize, 0.0f
    );
    CEGUI_DEFINE_PROPERTY(Scrollbar, float,
        "ScrollPosition", "Property to get/set the scroll position of the Scrollbar.  Value is a float.",
        &Scrollbar::setScrollPosition, &Scrollbar::getScrollPosition, 0.0f
    );
    CEGUI_DEFINE_PROPERTY(Scrollbar, float,
        "EndLockEnabled", "Property to get/set the 'end lock' mode setting for the Scrollbar. "
        "Value is either \"True\" or \"False\".",
        &Scrollbar::setAlpha, &Scrollbar::getAlpha, false
    );

    // we ban all these properties from xml for auto windows
    if (isAutoWindow())
    {
        banPropertyFromXML("DocumentSize");
        banPropertyFromXML("PageSize");
        banPropertyFromXML("StepSize");
        banPropertyFromXML("OverlapSize");
        banPropertyFromXML("ScrollPosition");

        // scrollbars tend to have their visibility toggled alot, so we ban
        // that as well
        banPropertyFromXML("Visible");
    }
}

//----------------------------------------------------------------------------//
PushButton* Scrollbar::getIncreaseButton() const
{
    return static_cast<PushButton*>(getChild(IncreaseButtonName));
}

//----------------------------------------------------------------------------//
PushButton* Scrollbar::getDecreaseButton() const
{
    return static_cast<PushButton*>(getChild(DecreaseButtonName));
}

//----------------------------------------------------------------------------//
Thumb* Scrollbar::getThumb() const
{
    return static_cast<Thumb*>(getChild(ThumbName));
}

//----------------------------------------------------------------------------//
void Scrollbar::updateThumb(void)
{
    if (!d_windowRenderer)
        CEGUI_THROW(InvalidRequestException("Scrollbar::updateThumb: This "
            "function must be implemented by the window renderer object (no "
            "window renderer is assigned.)"));

    static_cast<ScrollbarWindowRenderer*>(d_windowRenderer)->updateThumb();
}

//----------------------------------------------------------------------------//
float Scrollbar::getValueFromThumb(void) const
{
    if (!d_windowRenderer)
        CEGUI_THROW(InvalidRequestException("Scrollbar::getValueFromThumb: This "
            "function must be implemented by the window renderer object (no "
            "window renderer is assigned.)"));

    return static_cast<ScrollbarWindowRenderer*>(
        d_windowRenderer)->getValueFromThumb();
}

//----------------------------------------------------------------------------//
float Scrollbar::getAdjustDirectionFromPoint(const Vector2f& pt) const
{
    if (!d_windowRenderer)
        CEGUI_THROW(InvalidRequestException(
            "Scrollbar::getAdjustDirectionFromPoint: "
            "This function must be implemented by the window renderer object "
            "(no window renderer is assigned.)"));

    return static_cast<ScrollbarWindowRenderer*>(
        d_windowRenderer)->getAdjustDirectionFromPoint(pt);
}

//----------------------------------------------------------------------------//
bool Scrollbar::setScrollPosition_impl(const float position)
{
    const float old_pos = d_position;
    const float max_pos = getMaxScrollPosition();

    // limit position to valid range:  0 <= position <= max_pos
    d_position = (position >= 0) ?
                    ((position <= max_pos) ?
                        position :
                        max_pos) :
                    0.0f;

    return d_position != old_pos;
}

//----------------------------------------------------------------------------//
void Scrollbar::setConfig(const float* const document_size,
                          const float* const page_size,
                          const float* const step_size,
                          const float* const overlap_size,
                          const float* const position)
{
    const bool reset_max_position = d_endLockPosition && isAtEnd();
    bool config_changed = false;
    bool position_changed = false;

    if (document_size && (d_documentSize != *document_size))
    {
        d_documentSize = *document_size;
        config_changed = true;
    }

    if (page_size && (d_pageSize != *page_size))
    {
        d_pageSize = *page_size;
        config_changed = true;
    }

    if (step_size && (d_stepSize != *step_size))
    {
        d_stepSize = *step_size;
        config_changed = true;
    }
    
    if (overlap_size && (d_overlapSize != *overlap_size))
    {
        d_overlapSize = *overlap_size;
        config_changed = true;
    }

    if (position)
        position_changed = setScrollPosition_impl(*position);
    else if (reset_max_position)
        position_changed = setScrollPosition_impl(getMaxScrollPosition());

    // _always_ update the thumb to keep things in sync.  (though this
    // can cause a double-trigger of EventScrollPositionChanged, which
    // also happens with setScrollPosition anyway).
    updateThumb();

    //
    // Fire appropriate events based on actions we took.
    //
    if (config_changed)
    {
        WindowEventArgs args(this);
        onScrollConfigChanged(args);
    }

    if (position_changed)
    {
        WindowEventArgs args(this);
        onScrollPositionChanged(args);
    }
}

//----------------------------------------------------------------------------//
float Scrollbar::getMaxScrollPosition() const
{
    // max position is (docSize - pageSize)
    // but must be at least 0 (in case doc size is very small)
    return ceguimax((d_documentSize - d_pageSize), 0.0f);
}

//----------------------------------------------------------------------------//
bool Scrollbar::isAtEnd() const
{
    return d_position >= getMaxScrollPosition(); 
}

//----------------------------------------------------------------------------//
void Scrollbar::setEndLockEnabled(const bool enabled)
{
    d_endLockPosition = enabled;
}

//----------------------------------------------------------------------------//
bool Scrollbar::isEndLockEnabled() const
{
    return d_endLockPosition;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

