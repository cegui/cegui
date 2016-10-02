/***********************************************************************
    created:    Sun, 6th April 2014
    author:     Lukas E Meindl
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2014 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIDirect3D11GeometryBuffer_h_
#define _CEGUIDirect3D11GeometryBuffer_h_

#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/RendererModules/Direct3D11/Renderer.h"
#include "CEGUI/Rectf.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif


namespace CEGUI
{
class Direct3D11Texture;

//! Implementation of CEGUI::GeometryBuffer for the Direct3D 11 API.
class D3D11_GUIRENDERER_API Direct3D11GeometryBuffer : public GeometryBuffer
{
public:
    //! Constructor
    Direct3D11GeometryBuffer(Direct3D11Renderer& owner, CEGUI::RefCounted<RenderMaterial> renderMaterial);

    //! Destructor
    ~Direct3D11GeometryBuffer();


    // Implement GeometryBuffer interface.
    virtual void draw() const;
    virtual void appendGeometry(const float* vertex_data, std::size_t array_size);

    /*
    \brief
        The update function that is to be called when all the vertex attributes
        are set.

     \return Returns the created D3D input layout for the attribute layout of the used shader.
    */
    void finaliseVertexAttributes();

protected:
    //! Update the cached matrices
    void updateMatrix() const;
    //! Synchronise data in the hardware buffer with what's been added
    void updateVertexBuffer() const;
    //! Allocates a hardware vertex buffer of size 'dataSize' (size in bytes).
    void allocateVertexBuffer(const UINT dataSize) const;
    //! cleanup the hardware vertex buffer.
    void cleanupVertexBuffer() const;
    //! Draws the vertex data depending on the fill rule that was set for this object.
    void drawDependingOnFillRule() const;
    //! Calls the D3D11 function fot setting the scissor rectangle
    void setScissorRects() const;
    // Direct3D11Renderer object that created and owns this GeometryBuffer.
    Direct3D11Renderer& d_owner;
    //! The D3D Device
    ID3D11Device* d_device;
    //! The D3D DeviceContext
    ID3D11DeviceContext* d_deviceContext;
    //! hardware buffer where vertices will be drawn from.
    mutable ID3D11Buffer* d_vertexBuffer;
    //! Size of the currently allocated vertex buffer.
    mutable UINT d_bufferSize;
    //! Cache of the model view projection matrix
    mutable glm::mat4 d_matrix;
    //! D3D11 input layout describing the vertex format we use.
    ID3D11InputLayout* d_inputLayout;
};


}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
