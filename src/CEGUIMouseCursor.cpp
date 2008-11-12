/***********************************************************************
    filename:   CEGUIMouseCursor.cpp
    created:    21/2/2004
    author:     Paul D Turner

    purpose:    Implements MouseCursor class
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
#include "CEGUIMouseCursor.h"
#include "CEGUIExceptions.h"
#include "CEGUILogger.h"
#include "CEGUISystem.h"
#include "CEGUIRenderer.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIImage.h"

// Start of CEGUI namespace section
namespace CEGUI
{
const String MouseCursor::EventNamespace("MouseCursor");

/*************************************************************************
    Static Data Definitions
*************************************************************************/
// singleton instance pointer
template<> MouseCursor* Singleton<MouseCursor>::ms_Singleton    = 0;


/*************************************************************************
    Event name constants
*************************************************************************/
const String MouseCursor::EventImageChanged( "ImageChanged" );


/*************************************************************************
    constructor
*************************************************************************/
MouseCursor::MouseCursor(void)
{
    Rect screenArea(System::getSingleton().getRenderer()->getRect());

    // mouse defaults to middle of the constrained area
    d_position.d_x = screenArea.getWidth() / 2;
    d_position.d_y = screenArea.getHeight() / 2;
    d_position.d_z = 0.0f;

    // default constraint is to whole screen
    setConstraintArea(&screenArea);

    // mouse defaults to visible
    d_visible = true;

    // no default image though
    d_cursorImage = 0;

    char addr_buff[32];
    sprintf(addr_buff, "(%p)", static_cast<void*>(this));
    Logger::getSingleton().logEvent(
       "CEGUI::MouseCursor singleton created. " + String(addr_buff));
}


/*************************************************************************
    Destructor
*************************************************************************/
MouseCursor::~MouseCursor(void)
{
    char addr_buff[32];
    sprintf(addr_buff, "(%p)", static_cast<void*>(this));
    Logger::getSingleton().logEvent(
       "CEGUI::MouseCursor singleton destroyed. " + String(addr_buff));
}


/*************************************************************************
    Set the current mouse cursor image
*************************************************************************/
void MouseCursor::setImage(const Image* image)
{
    d_cursorImage = image;
    MouseCursorEventArgs args(this);
    args.image = image;
    onImageChanged(args);
}


/*************************************************************************
    Set the current mouse cursor image
*************************************************************************/
void MouseCursor::setImage(const String& imageset, const String& image_name)
{
    setImage(&ImagesetManager::getSingleton().getImageset(imageset)->getImage(image_name));
}


/*************************************************************************
    Draw the mouse cursor
*************************************************************************/
void MouseCursor::draw(void) const
{
    if (d_visible && (d_cursorImage != 0))
    {
        d_cursorImage->draw( d_position, System::getSingleton().getRenderer()->getRect() );
    }
}


/*************************************************************************
    Set the current mouse cursor position
*************************************************************************/
void MouseCursor::setPosition(const Point& position)
{
    d_position.d_x = position.d_x;
    d_position.d_y = position.d_y;
    constrainPosition();
}


/*************************************************************************
    Offset the mouse cursor position by the deltas specified in 'offset'.
*************************************************************************/
void MouseCursor::offsetPosition(const Point& offset)
{
    d_position.d_x += offset.d_x;
    d_position.d_y += offset.d_y;
    constrainPosition();
}


/*************************************************************************
    Checks the mouse cursor position is within the current 'constrain'
    Rect and adjusts as required.
*************************************************************************/
void MouseCursor::constrainPosition(void)
{
    Rect absarea(getConstraintArea());

    if (d_position.d_x >= absarea.d_right)
        d_position.d_x = absarea.d_right -1;

    if (d_position.d_y >= absarea.d_bottom)
        d_position.d_y = absarea.d_bottom -1;

    if (d_position.d_y < absarea.d_top)
        d_position.d_y = absarea.d_top;

    if (d_position.d_x < absarea.d_left)
        d_position.d_x = absarea.d_left;
}


/*************************************************************************
    Set the area that the mouse cursor is constrained to.
*************************************************************************/
void MouseCursor::setConstraintArea(const Rect* area)
{
    Rect renderer_area = System::getSingleton().getRenderer()->getRect();

    if (!area)
    {
        d_constraints.d_min.d_x = cegui_reldim(renderer_area.d_left / renderer_area.getWidth());
        d_constraints.d_min.d_y = cegui_reldim(renderer_area.d_top / renderer_area.getHeight());
        d_constraints.d_max.d_x = cegui_reldim(renderer_area.d_right / renderer_area.getWidth());
        d_constraints.d_max.d_y = cegui_reldim(renderer_area.d_bottom / renderer_area.getHeight());
    }
    else
    {
        Rect finalArea(area->getIntersection(renderer_area));
        d_constraints.d_min.d_x = cegui_reldim(finalArea.d_left / renderer_area.getWidth());
        d_constraints.d_min.d_y = cegui_reldim(finalArea.d_top / renderer_area.getHeight());
        d_constraints.d_max.d_x = cegui_reldim(finalArea.d_right / renderer_area.getWidth());
        d_constraints.d_max.d_y = cegui_reldim(finalArea.d_bottom / renderer_area.getHeight());
    }

    constrainPosition();
}


/*************************************************************************
    Set the area that the mouse cursor is constrained to.
*************************************************************************/
void MouseCursor::setUnifiedConstraintArea(const URect* area)
{
    Rect renderer_area = System::getSingleton().getRenderer()->getRect();

    if (area)
    {
        d_constraints = *area;
    }
    else
    {
        d_constraints.d_min.d_x = cegui_reldim(renderer_area.d_left / renderer_area.getWidth());
        d_constraints.d_min.d_y = cegui_reldim(renderer_area.d_top / renderer_area.getHeight());
        d_constraints.d_max.d_x = cegui_reldim(renderer_area.d_right / renderer_area.getWidth());
        d_constraints.d_max.d_y = cegui_reldim(renderer_area.d_bottom / renderer_area.getHeight());
    }

    constrainPosition();
}

/*************************************************************************
    Set the area that the mouse cursor is constrained to.
*************************************************************************/
Rect MouseCursor::getConstraintArea(void) const
{
    return Rect(d_constraints.asAbsolute(System::getSingleton().getRenderer()->getSize()));
}

/*************************************************************************
    Set the area that the mouse cursor is constrained to.
*************************************************************************/
const URect& MouseCursor::getUnifiedConstraintArea(void) const
{
    return d_constraints;
}

/*************************************************************************
    Return the current mouse cursor position in display resolution
    independant values.
*************************************************************************/
Point MouseCursor::getDisplayIndependantPosition(void) const
{
    Size dsz(System::getSingleton().getRenderer()->getSize());

    return Point(d_position.d_x / (dsz.d_width - 1.0f), d_position.d_y / (dsz.d_height - 1.0f));
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

    Begin event triggers section

*************************************************************************/
//////////////////////////////////////////////////////////////////////////

void MouseCursor::onImageChanged(MouseCursorEventArgs& e)
{
    fireEvent(EventImageChanged, e, EventNamespace);
}


MouseCursor& MouseCursor::getSingleton(void)
{
    return Singleton<MouseCursor>::getSingleton();
}


MouseCursor* MouseCursor::getSingletonPtr(void)
{
    return Singleton<MouseCursor>::getSingletonPtr();
}

} // End of  CEGUI namespace section
