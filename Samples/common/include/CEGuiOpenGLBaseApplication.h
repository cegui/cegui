/***********************************************************************
    filename:   CEGuiOpenGLBaseApplication.h
    created:    24/9/2004
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
    static void handleMouseWheel_freeglut(int wheel, int dir, int x, int y);

    static void doFPSUpdate();

    /*************************************************************************
        Data fields
    *************************************************************************/
    CEGUI::Renderer* d_renderer;
    static bool d_quitFlag;
    static int  d_lastFrameTime;
    static int  d_modifiers;
    // FPS stuff
    static int d_fps_lastTime;
    static int d_fps_frames;
    static int d_fps_value;
    static char d_fps_textbuff[16];
};


#endif  // end of guard _CEGuiOpenGLBaseApplication_h_
