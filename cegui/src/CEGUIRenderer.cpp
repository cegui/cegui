/***********************************************************************
	filename: 	CEGUIRenderer.cpp
	created:	20/2/2004
	author:		Paul D Turner
	
	purpose:	Some base class implementation for Renderer objects
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
#include "CEGUIRenderer.h"
#include "CEGUIEventSet.h"
#include "CEGUIEvent.h"
#include "CEGUIDefaultResourceProvider.h"


// Start of CEGUI namespace section
namespace CEGUI
{
const String Renderer::EventNamespace("Renderer");

/*************************************************************************
	Event name constants (static data definitions)
*************************************************************************/
const String Renderer::EventDisplaySizeChanged( "DisplayModeChanged" );


/*************************************************************************
	Implementation constants
*************************************************************************/
const float	Renderer::GuiZInitialValue		= 1.0f;
const float	Renderer::GuiZElementStep		= 0.001f;		// this is enough for 1000 Windows.
const float	Renderer::GuiZLayerStep			= 0.0001f;		// provides space for 10 layers per Window.


/*************************************************************************
	Constructor
*************************************************************************/
Renderer::Renderer(void)
    : d_resourceProvider(0),
      d_identifierString("Unknown renderer (vendor did not set the ID string!)")
{
	// setup standard events available
	addEvent(EventDisplaySizeChanged);

	// default initialisation
	resetZValue();
}

/*************************************************************************
	Destructor
*************************************************************************/
Renderer::~Renderer(void)
{
    if(d_resourceProvider)
    {
        delete d_resourceProvider;
        d_resourceProvider = 0;
    }
}

ResourceProvider* Renderer::createResourceProvider(void)
{
    d_resourceProvider = new DefaultResourceProvider();
    return d_resourceProvider;
}

const String& Renderer::getIdentifierString() const
{
    return d_identifierString;
}

} // End of  CEGUI namespace section
