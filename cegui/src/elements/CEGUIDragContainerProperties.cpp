/***********************************************************************
	filename: 	CEGUIDragContainerProperties.cpp
	created:	15/2/2005
	author:		Paul D Turner
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
#include "elements/CEGUIDragContainerProperties.h"
#include "elements/CEGUIDragContainer.h"
#include "CEGUIPropertyHelper.h"

// Start of CEGUI namespace section
namespace CEGUI
{
// Start of DragContainerProperties namespace section
namespace DragContainerProperties
{
    String DraggingEnabled::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper<bool>::toString(static_cast<const DragContainer*>(receiver)->isDraggingEnabled());
    }

    void DraggingEnabled::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<DragContainer*>(receiver)->setDraggingEnabled(PropertyHelper<bool>::fromString(value));
    }

    //////////////////////////////////////////////////////////////////////////
    
    String DragAlpha::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper<float>::toString(static_cast<const DragContainer*>(receiver)->getDragAlpha());
    }

    void DragAlpha::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<DragContainer*>(receiver)->setDragAlpha(PropertyHelper<float>::fromString(value));
    }

    //////////////////////////////////////////////////////////////////////////

    String DragThreshold::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper<float>::toString(static_cast<const DragContainer*>(receiver)->getPixelDragThreshold());
    }

    void DragThreshold::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<DragContainer*>(receiver)->setPixelDragThreshold(PropertyHelper<float>::fromString(value));
    }

    //////////////////////////////////////////////////////////////////////////

    String DragCursorImage::get(const PropertyReceiver* receiver) const
    {
        const Image* img =
            static_cast<const DragContainer*>(receiver)->getDragCursorImage();
        return PropertyHelper<Image*>::toString(img);
    }

    void DragCursorImage::set(PropertyReceiver* receiver, const String& value)
    {
        if (!value.empty())
        {
            static_cast<DragContainer*>(receiver)->
                setDragCursorImage(PropertyHelper<Image*>::fromString(value));
        }
    }

    //////////////////////////////////////////////////////////////////////////

    String StickyMode::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper<bool>::toString(
            static_cast<const DragContainer*>(receiver)->isStickyModeEnabled());
    }

    void StickyMode::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<DragContainer*>(receiver)->
            setStickyModeEnabled(PropertyHelper<bool>::fromString(value));
    }

    //////////////////////////////////////////////////////////////////////////

    String FixedDragOffset::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper<UVector2>::toString(
            static_cast<const DragContainer*>(receiver)->getFixedDragOffset());
    }

    void FixedDragOffset::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<DragContainer*>(receiver)->
            setFixedDragOffset(PropertyHelper<UVector2>::fromString(value));
    }

    //////////////////////////////////////////////////////////////////////////

    String UseFixedDragOffset::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper<bool>::toString(
            static_cast<const DragContainer*>(receiver)->
                isUsingFixedDragOffset());
    }

    void UseFixedDragOffset::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<DragContainer*>(receiver)->
            setUsingFixedDragOffset(PropertyHelper<bool>::fromString(value));
    }

} // End of  DragContainerProperties namespace section


} // End of  CEGUI namespace section
