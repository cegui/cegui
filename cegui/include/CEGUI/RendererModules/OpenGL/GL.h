/***********************************************************************
    created:    Fri Jan 23 2009
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
#ifndef _CEGUIOpenGL_h_
#define _CEGUIOpenGL_h_

#include "CEGUI/Config.h"

#if defined CEGUI_USE_EPOXY

#include <epoxy/gl.h>

#elif defined CEGUI_USE_GLEW

#include <GL/glew.h>

// When using GLEW, there's no need to "#include" the OpenGL headers.
#ifndef __APPLE__
#   if (defined( __WIN32__ ) || defined( _WIN32 ))
#       include <windows.h>
#   endif
#   include <GL/glu.h>
#else
#   include <OpenGL/glu.h>
#endif

#else
#error Either "CEGUI_USE_EPOXY" or "CEGUI_USE_GLEW" must be defined. Defining both or none is invalid.
#endif

#if (defined( __WIN32__ ) || defined( _WIN32 )) && !defined(CEGUI_STATIC)
#   if defined(CEGUIOPENGLRENDERER_EXPORTS) || defined(CEGUIOPENGLES2RENDERER_EXPORTS)
#       define OPENGL_GUIRENDERER_API __declspec(dllexport)
#   else
#       define OPENGL_GUIRENDERER_API __declspec(dllimport)
#   endif
#else
#   define OPENGL_GUIRENDERER_API
#endif

namespace CEGUI {

/*!
\brief
    Provides information about the type of OpenGL used by an OpenGL(ES) context
    (desktop OpenGL or OpenGL ES), the OpenGL(ES) version, and the OpenGL(ES)
    extensions.
*/
class OPENGL_GUIRENDERER_API OpenGLInfo
{

public:
    /*!
    \brief
        Type of the OpenGL(ES) context
    */
    enum Type
    {
        TYPE_NONE, /*!< Not initalized yet */
        TYPE_DESKTOP, /*!< Desktop OpenGL */
        TYPE_ES /*!< OpenGL ES */
    };

    static OpenGLInfo& getSingleton() { return s_instance; }

    /*!
    \brief
        Must be called before any other method.

        Note that the information returned by other methods is with respect to
        the OpenGL(ES) context that was current when this method was called.
    */
    void init();
    
    /*!
    \brief
        Type of the OpenGL(ES) context
    */
    Type type() const { return d_type; }
    
    /*!
    \brief
        Returns true if using Desktop OpenGL.
    */
    bool isDesktop() const { return type() == TYPE_DESKTOP; }
    
    /*!
    \brief
        Returns true if using OpenGL ES.
    */
    bool is_ES() const { return type() == TYPE_ES; }

    /*!
    \brief
        Returns OpenGL(ES) major version. Only supports Epoxy!
        Otherwise returns -1;
    */
    GLint verMajor() const { return d_verMajor; }
    
    /*!
    \brief
        Returns OpenGL(ES) minor version. Only supports Epoxy!
        Otherwise returns -1;
    */
    GLint verMinor() const { return d_verMinor; }

    /*!
    \brief
        Returns true off the OpenGL(ES) version is at least "major.minor".
        Only supports Epoxy! Otherwise returns false.
    */
    bool verAtLeast(GLint major, GLint minor) {
        return verMajor() > major  ||  (verMajor() == major && verMinor() >= minor); }

    /*!
    \brief
        Returns true if "S3TC" texture compression is supported.
    */
    bool s3tc_supported() const { return d_S3TC_supported; }

    /*!
    \brief
        Returns true if NPOT (non-power-of-two) textures are supported.
    */
    bool textures_NPOT_supported() const { return d_textures_NPOT_supported; }

    /*!
    \brief
        Returns true if "glReadBuffer" is supported.
    */
    bool glReadBuffer_supported() const
      { return d_glReadBuffer_supported; }

    /*!
    \brief
        Returns true if "glPolygonMode" is supported.
    */
    bool glPolygonMode_supported() const
      { return d_glPolygonMode_supported; }

    /*!
    \brief
        Returns true if VAO-s (Vertex Array Objects) are supported.
    */
    bool vaos_supported() const { return d_VAOs_supported; }

    /*!
    \brief
        Returns true if working with the read/draw framebuffers seperately is
        supported.
    */
    bool seperateReadAndDrawFramebuffersSupported() const
      { return d_seperateReadAndDrawFramebuffersSupported; }

    void verForce(GLint verMajor_, GLint verMinor_);
      
private:

    static OpenGLInfo s_instance;
    OpenGLInfo();
    void initTypeAndVer();
    void initSupportedFeatures();
    
    Type d_type;
    GLint d_verMajor;
    GLint d_verMinor;
    GLint d_verMajorForce;
    GLint d_verMinorForce;
    bool d_S3TC_supported;
    bool d_textures_NPOT_supported;
    bool d_glReadBuffer_supported;
    bool d_glPolygonMode_supported;
    bool d_seperateReadAndDrawFramebuffersSupported;
    bool d_VAOs_supported;
};

} // namespace CEGUI

#endif  // end of guard _CEGUIOpenGL_h_
