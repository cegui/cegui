/***********************************************************************
	filename: 	CEGUIScrolledContainerProperties.cpp
	created:	3/3/2005
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
#include "elements/CEGUIScrolledContainerProperties.h"
#include "elements/CEGUIScrolledContainer.h"
#include "CEGUILogger.h"
#include "CEGUIPropertyHelper.h"

// Start of CEGUI namespace section
namespace CEGUI
{
// Start of ScrolledContainerProperties namespace section
namespace ScrolledContainerProperties
{
    String ContentPaneAutoSized::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper::boolToString(static_cast<const ScrolledContainer*>(receiver)->isContentPaneAutoSized());
    }

    void ContentPaneAutoSized::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<ScrolledContainer*>(receiver)->setContentPaneAutoSized(PropertyHelper::stringToBool(value));
    }

    //////////////////////////////////////////////////////////////////////////
    
    String ContentArea::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper::rectToString(static_cast<const ScrolledContainer*>(receiver)->getContentArea());
    }

    void ContentArea::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<ScrolledContainer*>(receiver)->setContentArea(PropertyHelper::stringToRect(value));
    }

    //////////////////////////////////////////////////////////////////////////

    String ChildExtentsArea::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper::rectToString(static_cast<const ScrolledContainer*>(receiver)->getChildExtentsArea());
    }

    void ChildExtentsArea::set(PropertyReceiver* receiver, const String& value)
    {
        Logger::getSingleton().logEvent("ScrolledContainerProperties::ChildExtentsArea property does not support being set.", Errors);        
    }

} // End of  ScrolledContainerProperties namespace section
} // End of  CEGUI namespace section
