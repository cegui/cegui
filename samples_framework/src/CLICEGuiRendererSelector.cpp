/***********************************************************************
    created:    5/3/2005
    author:     Paul D Turner
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
#include "CLICEGuiRendererSelector.h"
#include <iostream>

CLICEGuiRendererSelector::CLICEGuiRendererSelector()
{}

CLICEGuiRendererSelector::~CLICEGuiRendererSelector()
{}

bool CLICEGuiRendererSelector::invokeDialog()
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
    if (d_rendererAvailability[OpenGL3GuiRendererType])
    {
        ++rendererNumber;
        last_available = OpenGL3GuiRendererType;
        std::cout << rendererNumber << ". OpenGL 3.2 Core Renderer." << std::endl;
    }
    if (d_rendererAvailability[OpenglEs2GuiRendererType])
    {
        ++rendererNumber;
        last_available = OpenglEs2GuiRendererType;
        std::cout << rendererNumber << ". OpenGL ES 2.0 Renderer." << std::endl;
    }
    if (d_rendererAvailability[IrrlichtGuiRendererType])
    {
        ++rendererNumber;
        last_available = IrrlichtGuiRendererType;
        std::cout << rendererNumber << ". Irrlicht GUI Renderer." << std::endl;
    }

    if (d_rendererAvailability[DirectFBGuiRendererType])
    {
        ++rendererNumber;
        last_available = DirectFBGuiRendererType;
        std::cout << rendererNumber << ". DirectFB GUI Renderer." << std::endl;
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
    else if ((d_rendererAvailability[OpenGL3GuiRendererType]) && (--selection == 0))
    {
        d_lastSelected = OpenGL3GuiRendererType;
    }
    else if ((d_rendererAvailability[OpenglEs2GuiRendererType]) && (--selection == 0))
    {
        d_lastSelected = OpenglEs2GuiRendererType;
    }
    else if ((d_rendererAvailability[IrrlichtGuiRendererType]) && (--selection == 0))
    {
        d_lastSelected = IrrlichtGuiRendererType;
    }
    else if ((d_rendererAvailability[DirectFBGuiRendererType]) && (--selection == 0))
    {
        d_lastSelected = DirectFBGuiRendererType;
    }
    else
    {
        std::cout << "Something went horribly wrong :(" << std::endl;
        return false;
    }

    return true;
}
