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
    d_isS3tcSupported(false),
    d_isNpotTextureSupported(false),
    d_isReadBufferSupported(false),
    d_isPolygonModeSupported(false),
    d_isSeperateReadAndDrawFramebufferSupported(false),
    d_isVaoSupported(false),
    d_isSizedInternalFormatSupported(false)
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
        {
            if (isUsingDesktopOpengl())
                CEGUI_THROW(RendererException
                  ("Failed to obtain desktop OpenGL version."));
            else
                CEGUI_THROW(RendererException
                  ("Failed to obtain OpenGL ES version."));
        }
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

    d_isS3tcSupported = epoxy_has_gl_extension("GL_EXT_texture_compression_s3tc");
    d_isNpotTextureSupported =
          (isUsingDesktopOpengl()  &&  verMajor() >= 2)
      ||  (isUsingOpenglEs() && verMajor() >= 3)
      ||  epoxy_has_gl_extension("GL_ARB_texture_non_power_of_two");
    d_isReadBufferSupported = d_isSizedInternalFormatSupported =
          (isUsingDesktopOpengl() && verAtLeast(1, 3))
      ||  (isUsingOpenglEs() && verMajor() >= 3);
    d_isPolygonModeSupported = isUsingDesktopOpengl() && verAtLeast(1, 3);
    d_isSeperateReadAndDrawFramebufferSupported =
          (isUsingDesktopOpengl() && verAtLeast(3, 1))
      ||  (isUsingOpenglEs() && verMajor() >= 3);
    d_isVaoSupported =     (isUsingDesktopOpengl() && verAtLeast(3, 2))
                       ||  (isUsingOpenglEs() && verMajor() >= 3);
      
#elif defined CEGUI_USE_GLEW

    d_isS3tcSupported = false;
    glGetError();

    // Why do we do this and not use GLEW_EXT_texture_compression_s3tc?
    // Because of glewExperimental, of course!
    int ext_count;
    glGetIntegerv(GL_NUM_EXTENSIONS, &ext_count);
    if ((glGetError() == GL_NO_ERROR)  &&
        (ext_count >= 0)  &&
        glGetStringi)
    {  
        for (int i = 0; i < ext_count; ++i)
        {
            const char* extension
              (reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i)));
            if ((glGetError() == GL_NO_ERROR)  &&
                extension  &&
                !std::strcmp(extension, "GL_EXT_texture_compression_s3tc"))
            {
                d_isS3tcSupported = true;
                break;
            }
        }
    }
    
    d_isNpotTextureSupported =
          (GLEW_VERSION_2_0 == GL_TRUE)
      ||  (GLEW_ARB_texture_non_power_of_two == GL_TRUE);
    d_isPolygonModeSupported = d_isSizedInternalFormatSupported
      = (GLEW_VERSION_1_3 == GL_TRUE);
    d_isSeperateReadAndDrawFramebufferSupported = (GLEW_VERSION_3_1 == GL_TRUE);
    d_isVaoSupported = (GLEW_VERSION_3_2 == GL_TRUE);
    
#endif

}

} // namespace CEGUI
