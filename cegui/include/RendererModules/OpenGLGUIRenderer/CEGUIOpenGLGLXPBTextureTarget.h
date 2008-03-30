/***********************************************************************
    filename:   CEGUIOpenGLGLXPBTextureTarget.h
    created:    Sat Feb 16 2008
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2008 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIOpenGLGLXPBTextureTarget_h_
#define _CEGUIOpenGLGLXPBTextureTarget_h_

#include "CEGUIOpenGLTextureTarget.h"
#include <GL/glx.h>

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    OpenGLGLXPBTextureTarget - allows rendering to an OpenGL texture via the
    pbuffer support in GLX 1.3.
*/
class OPENGL_GUIRENDERER_API OpenGLGLXPBTextureTarget :
    public OpenGLTextureTarget
{
public:
    /*!
    \brief
        OpenGLGLXPBTextureTarget default constructor.  Constructs an OpenGL
        texture target that draws using the pbuffer support in GLX 1.3.
    */
    OpenGLGLXPBTextureTarget();

    /*!
    \brief
        OpenGLGLXPBTextureTarget destructor.
    */
    virtual ~OpenGLGLXPBTextureTarget();

    // interface implementation
    virtual void clearRenderList(void);
    virtual void declareRenderSize(const Size& sz);

protected:
    /*!
    \brief
        Initialise the PBuffer with the needed size
    */
    void initialisePBuffer();

    /*!
    \brief
        Switch rendering so it targets the pbuffer
    */
    void enablePBuffer();

    /*!
    \brief
        Switch rendering so it targets whatever was active before the pbuffer
        was enabled.
    */
    void disablePBuffer();

    /*!
    \brief
        Perform basic init of texture we're going to use.
    */
    void initialiseTexture();

    /*!
    \brief
        Checks for required extensions and / or versions.  No return, will
        throw InvalidRequestException when something is not right.  d_dpy
        should be initialised before calling this.
    */
    void checkRequirements() const;

    /*!
    \brief
        Selects the appropriate FB config to use and stores in d_fbconfig.
    */
    void selectFBConfig();

    /*!
    \brief
        Creates the context to use with the pbuffer.
    */
    void createContext();

    // override state management funcs from base
    virtual void beginRenderState();
    virtual void endRenderState();

    //! X server display.
    Display* d_dpy;
    //! GLX config used in creating pbuffer
    GLXFBConfig d_fbconfig;
    //! GLX context
    GLXContext d_context;
    //! The GLX pbuffer we're using.
    GLXPbuffer d_pbuffer;
    //! stores previous X display when switching to pbuffer
    Display* d_prevDisplay;
    //! stores previous GLX drawable when switching to pbuffer
    GLXDrawable d_prevDrawable;
    //! stores previous GLX context when switching to pbuffer
    GLXContext d_prevContext;
};


} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIOpenGLGLXPBTextureTarget_h_
