/************************************************************************
    filename:   CEGuiIrrlichtBaseApplication.h
    created:    24/9/2004
    author:     Paul D Turner
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
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
#ifndef _CEGuiIrrlichtBaseApplication_h_
#define _CEGuiIrrlichtBaseApplication_h_

#include "CEGuiBaseApplication.h"
#include "renderers/IrrlichtRenderer/irrlichtrenderer.h"
#include "CEGUI.h"

class CEGuiIrrlichtBaseApplication : public CEGuiBaseApplication,  public irr::IEventReceiver
{
public:
    CEGuiIrrlichtBaseApplication();
    ~CEGuiIrrlichtBaseApplication();

    // implementation of base class abstract methods.
    bool    execute(CEGuiSample* sampleApp);
    void    cleanup();

    // irrlicht event listener
    bool OnEvent(irr::SEvent event);

protected:
    irr::IrrlichtDevice*       d_device;
    irr::video::IVideoDriver*  d_driver;
    irr::scene::ISceneManager* d_smgr;
    CEGUI::IrrlichtRenderer*   d_renderer;
};


#endif  // end of guard _CEGuiIrrlichtBaseApplication_h_
