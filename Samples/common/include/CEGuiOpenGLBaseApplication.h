/************************************************************************
    filename:   CEGuiOpenGLBaseApplication.h
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
#ifndef _CEGuiOpenGLBaseApplication_h_
#define _CEGuiOpenGLBaseApplication_h_

#include "CEGuiBaseApplication.h"
// remove Microsoft idiocy
#undef max
#undef min


#include "CEGUI.h"


class CEGuiOpenGLBaseApplication : public CEGuiBaseApplication
{
public:
    /*!
    \brief
        Constructor.
    */
    CEGuiOpenGLBaseApplication();


    /*!
    \brief
        Destructor.
    */
    virtual ~CEGuiOpenGLBaseApplication();


    // implementation of base class abstract methods.
    bool execute(CEGuiSample* sampleApp);
    void cleanup();

    // overrides of other base class methods.
    // These are required to make the 'quit' flag field static to enable us
    // to access it via the static methods employed by glut.
    void setQuitting(bool quit = true);
    bool isQuitting() const;

protected:
    /*************************************************************************
        Implementation Methods
    *************************************************************************/
    static void drawFrame(void);
    static void reshape(int w, int h);
    static void mouseMotion(int x, int y);
    static void mouseButton(int button, int state, int x, int y);
    static void keyChar(unsigned char key, int x, int y);
    static void keySpecial(int key, int x, int y);
    static void handleModifierKeys(void);

    /*************************************************************************
        Data fields
    *************************************************************************/
    CEGUI::Renderer* d_renderer;
    static bool d_quitFlag;
    static int  d_lastFrameTime;
    static int  d_modifiers;
};


#endif  // end of guard _CEGuiOpenGLBaseApplication_h_
