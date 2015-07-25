/***********************************************************************
    created:    21/7/2015
    author:     Yaron Cohen-Tal
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2015 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/RendererModules/OpenGL/GL.h"
#include "CEGUI/String.h"
#include "CEGUI/Exceptions.h"

#include <list>
#include <iostream>

#if defined CEGUI_USE_GLEW
#include <sstream>
#include <cstring>
#endif

namespace CEGUI
{

OpenGLInfo OpenGLInfo::s_instance;

//----------------------------------------------------------------------------//
OpenGLInfo::OpenGLInfo() :
    d_type(TYPE_NONE),
    d_verMajor(-1),
    d_verMinor(-1),
    d_verMajorForce(-1),
    d_verMinorForce(-1),
    d_S3TC_supported(false),
    d_textures_NPOT_supported(false),
    d_glReadBuffer_supported(false),
    d_glPolygonMode_supported(false),
    d_seperateReadAndDrawFramebuffersSupported(false),
    d_VAOs_supported(false)
{
}

//----------------------------------------------------------------------------//
void OpenGLInfo::init()
{
    initTypeAndVer();
    initSupportedFeatures();
}

void OpenGLInfo::verForce(GLint verMajor_, GLint verMinor_)
{
    d_verMajorForce = verMajor_;
    d_verMinorForce = verMinor_;
}

//----------------------------------------------------------------------------//
void OpenGLInfo::initTypeAndVer()
{
#if defined CEGUI_USE_EPOXY
    d_type = epoxy_is_desktop_gl() ? TYPE_DESKTOP : TYPE_ES;
    if (d_verMajorForce >= 0)
    {
        d_verMajor = d_verMajorForce;
        d_verMinor = d_verMinorForce;
    }
    else
    {
        int ver(epoxy_gl_version());
        if (!ver)
            CEGUI_THROW(RendererException("Failed to obtain OpenGL(ES) version."));
        d_verMajor = ver / 10;
        d_verMinor = ver % 10;
    }
#elif defined CEGUI_USE_GLEW
    d_type = TYPE_DESKTOP;
    glGetError ();
    d_verMajor = d_verMinor = -1;
#endif
}

//----------------------------------------------------------------------------//
void OpenGLInfo::initSupportedFeatures()
{

#if defined CEGUI_USE_EPOXY

    d_S3TC_supported = epoxy_has_gl_extension("GL_EXT_texture_compression_s3tc");
    d_textures_NPOT_supported =
          (isDesktop()  &&  verMajor() >= 2)
      ||  (is_ES() && verMajor() >= 3)
      ||  epoxy_has_gl_extension("GL_ARB_texture_non_power_of_two");
    d_glReadBuffer_supported =
          (isDesktop() && verAtLeast(1, 3))
      ||  (is_ES() && verMajor() >= 3);
    d_glPolygonMode_supported = isDesktop() && verAtLeast(1, 3);
    d_seperateReadAndDrawFramebuffersSupported =
          (isDesktop() && verAtLeast(3, 1))
      ||  (is_ES() && verMajor() >= 3);
    d_VAOs_supported =     (isDesktop() && verAtLeast(3, 2))
                       ||  (is_ES() && verMajor() >= 3);
      
#elif defined CEGUI_USE_GLEW

    d_S3TC_supported = false;

    // Why do we do this and not use GLEW_EXT_texture_compression_s3tc?
    // Because of glewExperimental, of course!
    int ext_count;
    glGetIntegerv(GL_NUM_EXTENSIONS, &ext_count);
    for(int i = 0; i < ext_count; ++i)
    {
        if (!std::strcmp(
                reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i)),
                                              "GL_EXT_texture_compression_s3tc"))
        {
            d_S3TC_supported = true;
            break;
        }
    }
    
    d_textures_NPOT_supported =
          (GLEW_VERSION_2_0 == GL_TRUE)
      ||  (GLEW_ARB_texture_non_power_of_two == GL_TRUE);
    d_glPolygonMode_supported
      = (GLEW_VERSION_1_3 == GL_TRUE);
    d_seperateReadAndDrawFramebuffersSupported = (GLEW_VERSION_3_1 == GL_TRUE);
    d_VAOs_supported = (GLEW_VERSION_3_2 == GL_TRUE);
    
#endif

}

} // namespace CEGUI
