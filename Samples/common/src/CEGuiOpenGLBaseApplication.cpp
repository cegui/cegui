/************************************************************************
    filename:   CEGuiOpenGLBaseApplication.cpp
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
// this controls conditional compile of file for MSVC
#include "CEGUIConfig.h"
#ifdef CEGUI_SAMPLES_USE_OPENGL

#include "CEGuiOpenGLBaseApplication.h"
#include "renderers/OpenGLGUIRenderer/openglrenderer.h"
#include "CEGuiSample.h"
#include "CEGUI.h"

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdexcept>


/*************************************************************************
    Static Data
*************************************************************************/



/*************************************************************************
    Constructor.
*************************************************************************/
CEGuiOpenGLBaseApplication::CEGuiOpenGLBaseApplication()
{
    // fake args for glutInit
    int argc = 1;
    char* argv = "SampleApp";

    // Do GLUT init
    glutInit(&argc, &argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Crazy Eddie's GUI Mk-2 - Sample Application");
    glutSetCursor(GLUT_CURSOR_NONE);

    glutDisplayFunc(&CEGuiOpenGLBaseApplication::drawFrame);
    glutReshapeFunc(&CEGuiOpenGLBaseApplication::reshape);
    glutMotionFunc(&CEGuiOpenGLBaseApplication::mouseMotion);
    glutPassiveMotionFunc(&CEGuiOpenGLBaseApplication::mouseMotion);
    glutMouseFunc(&CEGuiOpenGLBaseApplication::mouseButton);
    glutKeyboardFunc(&CEGuiOpenGLBaseApplication::keyChar);
    glutSpecialFunc(&CEGuiOpenGLBaseApplication::keySpecial);

    // Set the clear color
    glClearColor(0.7f, 0.2f, 0.5f, 1.0f);

    d_renderer = new CEGUI::OpenGLRenderer(1024);
    new CEGUI::System(d_renderer);
}


/*************************************************************************
    Destructor.
*************************************************************************/
CEGuiOpenGLBaseApplication::~CEGuiOpenGLBaseApplication()
{
    delete CEGUI::System::getSingletonPtr();
    delete d_renderer;
}


/*************************************************************************
    Start the base application
*************************************************************************/
bool CEGuiOpenGLBaseApplication::execute(CEGuiSample* sampleApp)
{
    sampleApp->initialiseSample();

    glutMainLoop();

    return true;
}


/*************************************************************************
    Performs any required cleanup of the base application system.
*************************************************************************/
void CEGuiOpenGLBaseApplication::cleanup()
{
    // nothing to do here.
}


/*************************************************************************
    Does whatever is required in one single frame
*************************************************************************/
void CEGuiOpenGLBaseApplication::drawFrame(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 12, 0.0, 0.0, -100, 0.0, 1.0, 0.0);

    CEGUI::System::getSingleton().renderGUI();

    glFlush();
    glutPostRedisplay();
    glutSwapBuffers();
}

void CEGuiOpenGLBaseApplication::reshape(int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
}

void CEGuiOpenGLBaseApplication::mouseMotion(int x, int y)
{
    CEGUI::System::getSingleton().injectMousePosition(x, y);
}

void CEGuiOpenGLBaseApplication::mouseButton(int button, int state, int x, int y)
{
    switch(button)
    {
    case  GLUT_LEFT_BUTTON:
        if (state == GLUT_UP)
        {
            CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
        }
        else
        {
            CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);

        }
        break;

    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_UP)
        {
            CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::RightButton);
        }
        else
        {
            CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::RightButton);
        }
        break;

    case GLUT_MIDDLE_BUTTON:
        if (state == GLUT_UP)
        {
            CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MiddleButton);
        }
        else
        {
            CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MiddleButton);
        }
        break;
    }

}

void CEGuiOpenGLBaseApplication::keyChar(unsigned char key, int x, int y)
{
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
        CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::Escape);
        break;
    }

    // always inject Character even if we have done key-down injection
    CEGUI::System::getSingleton().injectChar(static_cast<CEGUI::utf32>(key));
}

void CEGuiOpenGLBaseApplication::keySpecial(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::ArrowLeft);
        break;
    case GLUT_KEY_RIGHT:
        CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::ArrowRight);
        break;
    case GLUT_KEY_UP:
        CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::ArrowUp);
        break;
    case GLUT_KEY_DOWN:
        CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::ArrowDown);
        break;
    case GLUT_KEY_HOME:
        CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::Home);
        break;
    case GLUT_KEY_END:
        CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::End);
        break;
    case GLUT_KEY_PAGE_UP:
        CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::PageUp);
        break;
    case GLUT_KEY_PAGE_DOWN:
        CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::PageDown);
        break;
    }
}

#endif