/***********************************************************************
	created:	1/4/2005
	author:		Tomas Lindquist Olsen (based on code by Paul D Turner)

	purpose:	Implementation of PopupMenu widget base class
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
#include "CEGUI/widgets/PopupMenu.h"
#include "CEGUI/widgets/MenuItem.h"
#include "CEGUI/CoordConverter.h"

namespace CEGUI
{
const String PopupMenu::WidgetTypeName("CEGUI/PopupMenu");
const String PopupMenu::EventNamespace("PopupMenu");

//----------------------------------------------------------------------------//
PopupMenu::PopupMenu(const String& type, const String& name)
	: MenuBase(type, name)
    , d_origAlpha(d_alpha)
{
    addPopupMenuProperties();

    d_itemSpacing = 2;
    d_autoResize = true;

    setCursorInputPropagationEnabled(false);
	setClippedByParent(false);
	hide();
}

//----------------------------------------------------------------------------//
void PopupMenu::openPopupMenu(bool notify)
{
    // already open and not fading, or fading in?
    if (d_isOpen && (!d_fading || !d_fadingOut))
        return;

    // should we let the parent menu item initiate the open?
    Window* parent = getParent();
    if (notify && parent && dynamic_cast<MenuItem*>(parent))
    {
        static_cast<MenuItem*>(parent)->openPopupMenu();
        return; // the rest will be handled when MenuItem calls us itself
    }

    // we'll handle it ourselves then.
	// are we fading, and fading out?
	if (d_fading && d_fadingOut)
	{
        if (d_fadeInTime > 0.f && d_fadeOutTime > 0.f)
		{
			// jump to the point of the fade in that has the same alpha as right now - this keeps it smooth
            d_fadeElapsed = ((d_fadeOutTime - d_fadeElapsed) / d_fadeOutTime) * d_fadeInTime;
		}
        else
        {
            // start the fade in from the beginning
            d_fadeElapsed = 0.f;
        }
		// change to fade in
        d_fadingOut = false;
	}
	// otherwise just start normal fade in!
	else if (d_fadeInTime > 0.f)
	{
		d_fading = true;
		d_fadingOut=false;
		setAlpha(0.f);
		d_fadeElapsed = 0.f;
	}
	// should not fade!
	else
	{
		d_fading = false;
		setAlpha(d_origAlpha);
	}

	show();
	activate();
}

//----------------------------------------------------------------------------//
void PopupMenu::closePopupMenu(bool notify)
{
    if (!d_isOpen)
        return;

    // should we let the parent menu item close initiate the close?
    Window* parent = getParent();
    if (notify && parent && dynamic_cast<MenuItem*>(parent))
    {
        static_cast<MenuItem*>(parent)->closePopupMenu();
        return; // the rest will be handled when MenuItem calls us itself
    }

    // we'll do it our selves then.
	// are we fading, and fading in?
    if (d_fading && !d_fadingOut)
    {
        // make sure the "fade back out" is smooth - if possible !
        if (d_fadeOutTime > 0.f && d_fadeInTime > 0.f)
        {
            // jump to the point of the fade in that has the same alpha as right now - this keeps it smooth
            d_fadeElapsed = ((d_fadeInTime - d_fadeElapsed) / d_fadeInTime) * d_fadeOutTime;
        }
        else
        {
            // start the fade in from the beginning
            d_fadeElapsed = 0.f;
        }
        // change to fade out
        d_fadingOut = true;
    }
    // otherwise just start normal fade out!
    else if (d_fadeOutTime > 0.f)
	{
	    d_fading = true;
	    d_fadingOut = true;
	    setAlpha(d_origAlpha);
	    d_fadeElapsed = 0.f;
	}
	// should not fade!
	else
	{
	    d_fading = false;
	    hide();
	}
}

//----------------------------------------------------------------------------//
void PopupMenu::updateSelf(float elapsed)
{
    MenuBase::updateSelf(elapsed);

    if (d_fading)
    {
        d_fadeElapsed += elapsed;

		if (d_fadingOut)
		{
			if (d_fadeElapsed >= d_fadeOutTime)
			{
				hide();
				d_fading = false;
				setAlpha(d_origAlpha); // set real alpha so users can show directly without having to restore it
			}
            else
            {
                setAlpha(d_origAlpha * (d_fadeOutTime - d_fadeElapsed) / d_fadeOutTime);
            }
		}
		else
		{
			if (d_fadeElapsed >= d_fadeInTime)
			{
				d_fading = false;
				setAlpha(d_origAlpha);
			}
			else
			{
                setAlpha(d_origAlpha * d_fadeElapsed / d_fadeInTime);
			}
		}
	}
}

//----------------------------------------------------------------------------//
void PopupMenu::layoutItemWidgets()
{
	// get render area
	Rectf render_rect = getItemRenderArea();

	// get starting position
	const float x0 = CoordConverter::alignToPixels(render_rect.d_min.x);
	float y0 = CoordConverter::alignToPixels(render_rect.d_min.y);

	URect rect;
	UVector2 sz(cegui_absdim(CoordConverter::alignToPixels(render_rect.getWidth())), cegui_absdim(0)); // set item width

	// iterate through all items attached to this window
    for (auto item : d_listItems)
    {
		// get the "optimal" height of the item and use that!
		sz.d_y.d_offset = CoordConverter::alignToPixels(item->getItemPixelSize().d_height); // rounding errors ?

		// set destination rect
		item->setArea(UVector2(cegui_absdim(x0), cegui_absdim(y0)), USize(sz.d_x, sz.d_y));

		// next position
		y0 += CoordConverter::alignToPixels(sz.d_y.d_offset + d_itemSpacing);
	}
}

//----------------------------------------------------------------------------//
Sizef PopupMenu::getContentSize() const
{
	// find the content sizes
	float widest = 0;
	float total_height = 0;

	size_t i = 0;
	const size_t max = d_listItems.size();
	while (i < max)
	{
		const Sizef sz = d_listItems[i]->getItemPixelSize();
		if (sz.d_width > widest)
			widest = sz.d_width;
		total_height += sz.d_height;

		i++;
	}

	// vert item spacing
    if (i > 1)
        total_height += (i - 1) * d_itemSpacing;

	// return the content size
	return Sizef(widest, total_height);
}

//----------------------------------------------------------------------------//
void PopupMenu::onAlphaChanged(WindowEventArgs& e)
{
	MenuBase::onAlphaChanged(e);

	// if we are not fading, this is a real alpha change request and we save a copy of the value
	if (!d_fading)
		d_origAlpha = d_alpha;
}

//----------------------------------------------------------------------------//
void PopupMenu::onDestructionStarted(WindowEventArgs& e)
{
    // if we are attached to a menuitem, we make sure that gets updated
    Window* p = getParent();
    if (p && dynamic_cast<MenuItem*>(p))
        static_cast<MenuItem*>(p)->setPopupMenu(nullptr);
    MenuBase::onDestructionStarted(e);
}

//----------------------------------------------------------------------------//
void PopupMenu::onShown(WindowEventArgs& e)
{
    d_isOpen = true;
    MenuBase::onShown(e);
}

//----------------------------------------------------------------------------//
void PopupMenu::onHidden(WindowEventArgs& e)
{
    d_isOpen = false;
    MenuBase::onHidden(e);
}

//----------------------------------------------------------------------------//
void PopupMenu::addPopupMenuProperties()
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(PopupMenu, float,
        "FadeInTime", "Property to get/set the fade in time in seconds of the popup menu.  Value is a float.",
        &PopupMenu::setFadeInTime, &PopupMenu::getFadeInTime, 0.0f
    );

    CEGUI_DEFINE_PROPERTY(PopupMenu, float,
        "FadeOutTime", "Property to get/set the fade out time in seconds of the popup menu.  Value is a float.",
        &PopupMenu::setFadeOutTime, &PopupMenu::getFadeOutTime, 0.0f
    );
}

}
