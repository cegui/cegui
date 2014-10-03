/***********************************************************************
    created:    Wed, 8th Feb 2012
    author:     Lukas E Meindl
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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

#ifndef _CEGUIGLES2StateChangeWrapper_h_
#define _CEGUIGLES2StateChangeWrapper_h_

#include "CEGUI/RendererModules/OpenGL/StateChangeWrapper.h"
#include "RendererBase.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
GLES2StateChangeWrapper - wraps OpenGL calls and checks for redundant calls beforehand
*/
class OPENGL_GUIRENDERER_API GLES2StateChangeWrapper : public OpenGLBaseStateChangeWrapper
{
public:
    GLES2StateChangeWrapper();
    virtual ~GLES2StateChangeWrapper();

    //! glBindVertexArray only recognized in GLES3 (not 2)
    virtual void bindVertexArray(GLuint vertexArray);
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
