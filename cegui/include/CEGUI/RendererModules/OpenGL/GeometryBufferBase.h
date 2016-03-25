/***********************************************************************
    created:    Tue Apr 30 2013
    authors:    Paul D Turner <paul@cegui.org.uk>
                Lukas Meindl
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2013 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIGeometryBufferBase_h_
#define _CEGUIGeometryBufferBase_h_

#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/RendererModules/OpenGL/RendererBase.h"
#include "CEGUI/Rectf.h"

#include "CEGUI/RefCounted.h"

#include <utility>
#include <vector>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{
class OpenGLTexture;
class RenderMaterial;

/*!
\brief
    OpenGL based implementation of the GeometryBuffer interface.
*/
class OPENGL_GUIRENDERER_API OpenGLGeometryBufferBase : public GeometryBuffer
{
public:
    //! Constructor
    OpenGLGeometryBufferBase(OpenGLRendererBase& owner, CEGUI::RefCounted<RenderMaterial> renderMaterial);
    virtual ~OpenGLGeometryBufferBase();

    /*
    \brief
        The update function that is to be called when all the vertex attributes
        are set.
    */
    virtual void finaliseVertexAttributes() const {}

protected:
    //! Update the cached matrices
    void updateMatrix() const;

    //! OpenGLRendererBase that owns the GeometryBuffer.
    OpenGLRendererBase& d_owner;
    //! Cache of the model view projection matrix
    mutable glm::mat4 d_matrix;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif

