/***********************************************************************
    filename:   GeometryBufferBase.h
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

#include "../../GeometryBuffer.h"
#include "CEGUI/RendererModules/OpenGL/RendererBase.h"
#include "../../Rect.h"
#include "../../Quaternion.h"

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

    // implementation of abstract members from GeometryBuffer
    void setCustomTransform(const glm::mat4x4& transformation);
    void setClippingRegion(const Rectf& region);
    void appendGeometry(const std::vector<float>& vertex_data);
    void reset();

    //! Returns the model matrix, which is in effect for this GeometryBuffer
    const glm::mat4& getMatrix() const;

    /*
    \brief
        The update function that is to be called when all the vertex attributes
        are set.
    */
    virtual void finaliseVertexAttributes() = 0;

protected:
    //! update cached matrix
    void updateMatrix() const;

    //! OpenGLRendererBase that owns the GeometryBuffer.
    OpenGLRendererBase& d_owner;
    //! rectangular clip region
    Rectf d_clipRect;
    //! cache of the model matrix
    mutable glm::mat4 d_matrix;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif

