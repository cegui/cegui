/***********************************************************************
    created:    Jan 11 2006
    author:     Tomas Lindquist Olsen

    purpose:    Implements interface for the WindowRenderer base class
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
#include "CEGUI/WindowRenderer.h"
#include "CEGUI/Window.h"
#include "CEGUI/falagard/WidgetLookManager.h"
#include "CEGUI/falagard/WidgetLookFeel.h"
#include "CEGUI/falagard/NamedArea.h"

namespace CEGUI
{

/************************************************************************
    Constructor
*************************************************************************/
WindowRenderer::WindowRenderer(const String& name, const String& class_name) :
    d_window(nullptr),
    d_name(name),
    d_class(class_name)
{
}

/************************************************************************
    Destructor
*************************************************************************/
WindowRenderer::~WindowRenderer()
{
}

/************************************************************************
    Get the Look'N'Feel assigned to our window
*************************************************************************/
const WidgetLookFeel& WindowRenderer::getLookNFeel() const
{
    return WidgetLookManager::getSingleton().getWidgetLook(d_window->getLookNFeel());
}

//----------------------------------------------------------------------------//
ColourRect WindowRenderer::getOptionalColour(const String& propertyName, const Colour& defaultColour) const
{
    ColourRect rect(defaultColour);
    if (d_window->isPropertyPresent(propertyName))
        rect = d_window->getProperty<ColourRect>(propertyName);
    return rect;
}

/************************************************************************
    Get unclipped inner rectangle.
*************************************************************************/
Rectf WindowRenderer::getUnclippedInnerRect() const
{
    const WidgetLookFeel& wlf(getLookNFeel());

    if(wlf.isNamedAreaPresent("inner_rect", true))
        return wlf.getNamedArea("inner_rect").getArea().getPixelRect(*d_window, d_window->getUnclippedOuterRect().get());
    else
        return d_window->getUnclippedOuterRect().get();
}

/************************************************************************
    Register property with window renderer
*************************************************************************/
void WindowRenderer::registerProperty(Property* property,
                                      const bool ban_from_xml)
{
    d_properties.push_back(std::make_pair(property, ban_from_xml));
}

//----------------------------------------------------------------------------//
void WindowRenderer::registerProperty(Property* property)
{
    registerProperty(property, false);
}

/************************************************************************
    On attached to window
*************************************************************************/
void WindowRenderer::onAttach()
{
    for (const auto& pair : d_properties)
    {
        d_window->addProperty(pair.first);
        // ban from xml if neccessary
        if (pair.second)
            d_window->banPropertyFromXML(pair.first->getName());
    }
}

/************************************************************************
    On detached from window
*************************************************************************/
void WindowRenderer::onDetach()
{
    for (const auto& pair : d_properties)
    {
        // unban from xml if neccessary
        if (pair.second)
            d_window->unbanPropertyFromXML(pair.first->getName());

        d_window->removeProperty(pair.first->getName());
    }
}

//----------------------------------------------------------------------------//
void WindowRenderer::getRenderingContext(RenderingContext& ctx) const
{
    // default just calls back to the window implementation version.
    d_window->getRenderingContext_impl(ctx);
}

//----------------------------------------------------------------------------//
bool WindowRenderer::handleFontRenderSizeChange(const Font* const font)
{
    return getLookNFeel().handleFontRenderSizeChange(*d_window, font);
}

//----------------------------------------------------------------------------//
Sizef WindowRenderer::getContentSize() const
{
    throw InvalidRequestException("This function isn't implemented for this type of window renderer.");
}

//----------------------------------------------------------------------------//
UDim WindowRenderer::getWidthOfAreaReservedForContentLowerBoundAsFuncOfWindowWidth() const
{
    throw InvalidRequestException("This function isn't implemented for this type of window renderer.");
}

//----------------------------------------------------------------------------//
UDim WindowRenderer::getHeightOfAreaReservedForContentLowerBoundAsFuncOfWindowHeight() const
{
    throw InvalidRequestException("This function isn't implemented for this type of window renderer.");
}

//----------------------------------------------------------------------------//
void WindowRenderer::adjustSizeToContent()
{
    getWindow()->adjustSizeToContent_direct();
}

//----------------------------------------------------------------------------//
bool WindowRenderer::contentFitsForSpecifiedWindowSize(const Sizef& /*window_size*/) const
{
    throw InvalidRequestException("This function isn't implemented for this type of window renderer.");
}

//----------------------------------------------------------------------------//
bool WindowRenderer::contentFits() const
{
    throw InvalidRequestException("This function isn't implemented for this type of window renderer.");
}

//----------------------------------------------------------------------------//

} // End of CEGUI namespace
