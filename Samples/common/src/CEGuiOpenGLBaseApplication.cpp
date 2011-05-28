/***********************************************************************
    filename:   CEGuiOpenGLBaseApplication.cpp
    created:    24/9/2004
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#if defined(__linux__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__HAIKU__)
# include <unistd.h>
#endif

// this controls conditional compile of file for Apple
#include "CEGUISamplesConfig.h"
#ifdef CEGUI_SAMPLES_USE_OPENGL

#include "CEGuiOpenGLBaseApplication.h"
#include "RendererModules/OpenGL/CEGUIOpenGLRenderer.h"
#include "CEGuiSample.h"
#include "CEGUI.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#ifdef __MINGW32__
#define GLUT_DISABLE_ATEXIT_HACK
#endif
#include <GL/glut.h>
#ifdef __FREEGLUT_STD_H__
#   include <GL/freeglut_ext.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <stdexcept>

/*************************************************************************
    Simple key-map used to translate glut special keys to CEGUI keys.
    See: CEGuiOpenGLBaseApplication::keySpecial method.
*************************************************************************/
struct GlutKeyMapping
{
    int glutKey;
    CEGUI::uint  ceguiKey;
};

GlutKeyMapping specialKeyMap[] =
{
    {GLUT_KEY_F1, CEGUI::Key::F1},
    {GLUT_KEY_F2, CEGUI::Key::F2},
    {GLUT_KEY_F3, CEGUI::Key::F3},
    {GLUT_KEY_F4, CEGUI::Key::F4},
    {GLUT_KEY_F5, CEGUI::Key::F5},
    {GLUT_KEY_F6, CEGUI::Key::F6},
    {GLUT_KEY_F7, CEGUI::Key::F7},
    {GLUT_KEY_F8, CEGUI::Key::F8},
    {GLUT_KEY_F9, CEGUI::Key::F9},
    {GLUT_KEY_F10, CEGUI::Key::F10},
    {GLUT_KEY_F11, CEGUI::Key::F11},
    {GLUT_KEY_F12, CEGUI::Key::F12},
    {GLUT_KEY_LEFT, CEGUI::Key::ArrowLeft},
    {GLUT_KEY_UP, CEGUI::Key::ArrowUp},
    {GLUT_KEY_RIGHT, CEGUI::Key::ArrowRight},
    {GLUT_KEY_DOWN, CEGUI::Key::ArrowDown},
    {GLUT_KEY_PAGE_UP, CEGUI::Key::PageUp},
    {GLUT_KEY_PAGE_DOWN, CEGUI::Key::PageDown},
    {GLUT_KEY_HOME, CEGUI::Key::Home},
    {GLUT_KEY_END, CEGUI::Key::End},
    {GLUT_KEY_INSERT, CEGUI::Key::Insert},
    {-1, 0}
};

/*************************************************************************
    Static Data
*************************************************************************/
CEGuiOpenGLBaseApplication* CEGuiOpenGLBaseApplication::d_appInstance = 0;
int  CEGuiOpenGLBaseApplication::d_frameTime = 0;
int CEGuiOpenGLBaseApplication::d_modifiers = 0;

//----------------------------------------------------------------------------//
CEGuiOpenGLBaseApplication::CEGuiOpenGLBaseApplication()
{
    if (d_appInstance)
        throw CEGUI::InvalidRequestException(
            "CEGuiOpenGLBaseApplication instance already exists!");

    d_appInstance = this;

    // fake args for glutInit
    int argc = 1;
    const char* argv = "SampleApp";

    // Do GLUT init
    glutInit(&argc, (char**)&argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Crazy Eddie's GUI Mk-2 - Sample Application");
    glutSetCursor(GLUT_CURSOR_NONE);
    // set up glut callbacks
    glutDisplayFunc(&CEGuiOpenGLBaseApplication::drawFrame);
    glutReshapeFunc(&CEGuiOpenGLBaseApplication::reshape);
    glutMotionFunc(&CEGuiOpenGLBaseApplication::mouseMotion);
    glutPassiveMotionFunc(&CEGuiOpenGLBaseApplication::mouseMotion);
    glutMouseFunc(&CEGuiOpenGLBaseApplication::mouseButton);
    glutKeyboardFunc(&CEGuiOpenGLBaseApplication::keyChar);
    glutSpecialFunc(&CEGuiOpenGLBaseApplication::keySpecial);
    #ifdef __FREEGLUT_EXT_H__
        glutMouseWheelFunc(&CEGuiOpenGLBaseApplication::handleMouseWheel_freeglut);
    #endif

    // create renderer (other option objects are left at defaults)
    d_renderer = &CEGUI::OpenGLRenderer::create();

    // Set the clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

//----------------------------------------------------------------------------//
CEGuiOpenGLBaseApplication::~CEGuiOpenGLBaseApplication()
{
    CEGUI::OpenGLRenderer::destroy((CEGUI::OpenGLRenderer&)*d_renderer);
}

//----------------------------------------------------------------------------//
bool CEGuiOpenGLBaseApplication::execute_impl(CEGuiSample* sampleApp)
{
    sampleApp->initialiseSample();

    // set starting time
    d_frameTime = glutGet(GLUT_ELAPSED_TIME);

    glutMainLoop();

    return true;
}

//----------------------------------------------------------------------------//
void CEGuiOpenGLBaseApplication::cleanup_impl()
{
    // nothing to do here.
}

//----------------------------------------------------------------------------//
void CEGuiOpenGLBaseApplication::beginRendering(const float /*elapsed*/)
{
    glClear(GL_COLOR_BUFFER_BIT);
}

//----------------------------------------------------------------------------//
void CEGuiOpenGLBaseApplication::endRendering()
{
    glutPostRedisplay();
    glutSwapBuffers();
}

//----------------------------------------------------------------------------//
void CEGuiOpenGLBaseApplication::drawFrame(void)
{
    // calculate time elapsed since last frame
    const int time_now = glutGet(GLUT_ELAPSED_TIME);
    const float elapsed = static_cast<float>(time_now - d_frameTime) / 1000.0f;
    d_frameTime = time_now;

    d_appInstance->renderSingleFrame(elapsed);

    // here we check the 'quitting' state and cleanup as required.
    // this is probably not the best way to do this, but since we're
    // using glut, and glutMainLoop can never return, we need some
    // way of checking when to exit.  And this is it...
    if (d_appInstance->isQuitting())
    {
        // cleanup cegui system
        CEGUI::OpenGLRenderer* renderer =
            static_cast<CEGUI::OpenGLRenderer*>(
                CEGUI::System::getSingleton().getRenderer());
        CEGUI::System::destroy();
        CEGUI::OpenGLRenderer::destroy(*renderer);

        // exit
        exit(0);
    }
}

//----------------------------------------------------------------------------//
void CEGuiOpenGLBaseApplication::reshape(int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    CEGUI::System::getSingleton().
        notifyDisplaySizeChanged(CEGUI::Sizef((float)w,(float)h));
}

//----------------------------------------------------------------------------//
void CEGuiOpenGLBaseApplication::mouseMotion(int x, int y)
{
    CEGUI::System::getSingleton().injectMousePosition(x, y);
}

//----------------------------------------------------------------------------//
void CEGuiOpenGLBaseApplication::mouseButton(int button, int state, int /*x*/, int /*y*/)
{
    switch(button)
    {
    case  GLUT_LEFT_BUTTON:
        if (state == GLUT_UP)
            CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
        else
            CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
        break;

    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_UP)
            CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::RightButton);
        else
            CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::RightButton);
        break;

    case GLUT_MIDDLE_BUTTON:
        if (state == GLUT_UP)
            CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::MiddleButton);
        else
            CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MiddleButton);
        break;
    }
}

//----------------------------------------------------------------------------//
void CEGuiOpenGLBaseApplication::keyChar(unsigned char key, int /*x*/, int /*y*/)
{
    handleModifierKeys();

    int mods = glutGetModifiers();
    
    // extract some keys may be handled via key code and generate those too
    switch (key)
    {
    case 0x08:  // backspace
        CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::Backspace);
        break;
    case 0x7F:  // delete
        CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::Delete);
        break;
    case 0x1B:  // Escape
        d_appInstance->setQuitting();
        break;
    case 0x0D:  // CR (Return)
        CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::Return);
        break;
    case 0x03:  // CTRL+C
        CEGUI::System::getSingleton().injectCopyRequest();
        break;
    case 0x18:  // CTRL+X
        CEGUI::System::getSingleton().injectCutRequest();
        break;
    case 0x16:  // CTRL+V
        CEGUI::System::getSingleton().injectPasteRequest();
        break;
        
    default:
        CEGUI::System::getSingleton().injectChar(static_cast<CEGUI::utf32>(key));
        break;
    }
}

//----------------------------------------------------------------------------//
void CEGuiOpenGLBaseApplication::keySpecial(int key, int /*x*/, int /*y*/)
{
    handleModifierKeys();

    GlutKeyMapping* mapping = specialKeyMap;

    while (mapping->glutKey != -1)
    {
        if (mapping->glutKey == key)
        {
            CEGUI::System::getSingleton().injectKeyDown(mapping->ceguiKey);
            return;
        }

        ++mapping;
    }
}

//----------------------------------------------------------------------------//
void CEGuiOpenGLBaseApplication::handleModifierKeys(void)
{
    int mods = glutGetModifiers();

    CEGUI::System& cesys = CEGUI::System::getSingleton();

    bool shift = mods&GLUT_ACTIVE_SHIFT;
    bool ctrl  = mods&GLUT_ACTIVE_CTRL;
    bool alt   = mods&GLUT_ACTIVE_ALT;

    // shift
    if (shift != bool(d_modifiers&GLUT_ACTIVE_SHIFT))
    {
        CEGUI::Key::Scan sc = CEGUI::Key::LeftShift;
        if (shift)
        {
            cesys.injectKeyDown(sc);
        }
        else
        {
            cesys.injectKeyUp(sc);
        }
    }

    // control
    if (ctrl != bool(d_modifiers&GLUT_ACTIVE_CTRL))
    {
        CEGUI::Key::Scan sc = CEGUI::Key::LeftControl;
        if (shift)
        {
            cesys.injectKeyDown(sc);
        }
        else
        {
            cesys.injectKeyUp(sc);
        }
    }

    // alt
    if (alt != bool(d_modifiers&GLUT_ACTIVE_ALT))
    {
        CEGUI::Key::Scan sc = CEGUI::Key::LeftAlt;
        if (shift)
        {
            cesys.injectKeyDown(sc);
        }
        else
        {
            cesys.injectKeyUp(sc);
        }
    }

    d_modifiers = mods;
}

//----------------------------------------------------------------------------//
// FreeGLUT supports wheel events
void CEGuiOpenGLBaseApplication::handleMouseWheel_freeglut(int wheel, int dir, int /*x*/, int /*y*/)
{
    if (wheel == 0)
        CEGUI::System::getSingleton().injectMouseWheelChange((float)dir);
}

//----------------------------------------------------------------------------//

#endif

