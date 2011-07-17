/***********************************************************************
	filename: 	CEGUIWindowProperties.cpp
	created:	5/7/2004
	author:		Paul D Turner
	
	purpose:	Implementation of available window base class properties
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
#include "CEGUIWindowProperties.h"
#include "CEGUIWindow.h"
#include "CEGUIFont.h"
#include "CEGUIPropertyHelper.h"
#include "CEGUIExceptions.h"
#include "CEGUIWindowFactoryManager.h"
#include "CEGUIImageManager.h"
#include "CEGUIImage.h"
#include <cstdlib>
#include <cstdio>

// Start of CEGUI namespace section
namespace CEGUI
{
// Start of WindowProperties namespace section
namespace WindowProperties
{

String WindowRenderer::get(const PropertyReceiver* receiver) const
{
    CEGUI::WindowRenderer* wr = static_cast<const Window*>(receiver)->getWindowRenderer();
    return wr ? wr->getName() : "";
}

void WindowRenderer::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setWindowRenderer(value);
}

void WindowRenderer::writeXMLToStream(const PropertyReceiver* receiver, XMLSerializer& xml_stream) const
{
    const Window* wnd = static_cast<const Window*>(receiver);
    if (!WindowFactoryManager::getSingleton().isFalagardMappedType(wnd->getType()))
    {
        Property::writeXMLToStream(receiver, xml_stream);
    }
}


String LookNFeel::get(const PropertyReceiver* receiver) const
{
    return static_cast<const Window*>(receiver)->getLookNFeel();
}

void LookNFeel::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setLookNFeel(value);
}

void LookNFeel::writeXMLToStream(const PropertyReceiver* receiver, XMLSerializer& xml_stream) const
{
    const Window* wnd = static_cast<const Window*>(receiver);
    if (!WindowFactoryManager::getSingleton().isFalagardMappedType(wnd->getType()))
    {
        Property::writeXMLToStream(receiver, xml_stream);
    }
}

//----------------------------------------------------------------------------//

} // End of  WindowProperties namespace section

} // End of  CEGUI namespace section
