/************************************************************************
	filename: 	ogrerenderer.h
	created:	11/5/2004
	author:		Paul D Turner
	
	purpose:	Interface for main Ogre GUI renderer class
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
/*************************************************************************
	This file contains code that is specific to Ogre (http://www.ogre3d.org)
*************************************************************************/
#ifndef _ogrerenderer_h_
#define _ogrerenderer_h_

#ifdef OGRE_GUIRENDERER_EXPORTS
#define OGRE_GUIRENDERER_API __declspec(dllexport)
#else
#define OGRE_GUIRENDERER_API __declspec(dllimport)
#endif

#include "CEGUIBase.h"
#include "CEGUIRenderer.h"
#include "CEGUITexture.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Renderer class to interface with Ogre engine.
*/
class OGRE_GUIRENDERER_API OgreRenderer : public Renderer
{
public:

private:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
};

} // End of  CEGUI namespace section


#endif	// end of guard _ogrerenderer_h_
