/************************************************************************
	filename: 	CEGUIRenderer.cpp
	created:	20/2/2004
	author:		Paul D Turner
	
	purpose:	Some base class implementation for Renderer objects
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
    Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

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
#include "CEGUIRenderer.h"
#include "CEGUIEventSet.h"
#include "CEGUIEvent.h"


// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
	Event name constants (static data definitions)
*************************************************************************/
const utf8	Renderer::EventDisplaySizeChanged[]		= "DisplayModeChanged";


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
}

} // End of  CEGUI namespace section
