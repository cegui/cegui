/************************************************************************
    filename:   CLICEGuiRendererSelector.cpp
    created:    5/3/2005
    author:     Paul D Turner
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)
 
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
#include "CLICEGuiRendererSelector.h"
#include <iostream>

CLICEGuiRendererSelector::CLICEGuiRendererSelector()
{}

CLICEGuiRendererSelector::~CLICEGuiRendererSelector()
{}

bool CLICEGuiRendererSelector::inkokeDialog()
{
    unsigned int selection;
    unsigned int rendererNumber = 0;

    std::cout << "-- CEGUI Sample Application Framework --" << std::endl;
    std::cout << "Please select a renderer:" << std::endl;
    
    CEGuiRendererType last_available;

    // print options for enabled renderers
    if (d_rendererAvailability[OgreGuiRendererType])
    {
        ++rendererNumber;
        last_available = OgreGuiRendererType;
        std::cout << rendererNumber << ". Ogre3D GUI Renderer." << std::endl;
    }
    if (d_rendererAvailability[OpenGLGuiRendererType])
    {
        ++rendererNumber;
        last_available = OpenGLGuiRendererType;
        std::cout << rendererNumber << ". OpenGL GUI Renderer." << std::endl;
    }
    if (d_rendererAvailability[IrrlichtGuiRendererType])
    {
        ++rendererNumber;
        last_available = IrrlichtGuiRendererType;
        std::cout << rendererNumber << ". Irrlicht GUI Renderer." << std::endl;
    }

    // abort if no renderers are available.
    if (rendererNumber == 0)
    {
        std::cout << "Oops!  There are no renderer modules available, check your config!" << std::endl;
        return false;
    }

    // if we only have one available renderer, then pick that one automatically
    if (rendererNumber == 1)
    {
        d_lastSelected = last_available;
        std::cout << "Picked the only renderer. Starting..." << std::endl;
        return true;
    }
    // get user to pick a valid option.
    std::cin >> selection;
    while (selection > rendererNumber)
    {
        std::cout << "Oops!  That was not a valid selection, please try again..." << std::endl;
        selection = 0;
        std::cin >> selection;
    }

    // discover which renderer was actually selected.
    if ((d_rendererAvailability[OgreGuiRendererType]) && (--selection == 0))
    {
        d_lastSelected = OgreGuiRendererType;
    }
    else if ((d_rendererAvailability[OpenGLGuiRendererType]) && (--selection == 0))
    {
        d_lastSelected = OpenGLGuiRendererType;
    }
    else if ((d_rendererAvailability[IrrlichtGuiRendererType]) && (--selection == 0))
    {
        d_lastSelected = IrrlichtGuiRendererType;
    }
    else
    {
        std::cout << "Something went horribly wrong :(" << std::endl;
        return false;
    }

    return true;
}
