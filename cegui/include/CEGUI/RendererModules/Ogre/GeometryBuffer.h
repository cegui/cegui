/***********************************************************************
    created:    Tue Feb 17 2009
    author:     Henri I Hyyryläinen (based on code by Paul D Turner)
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIOgreGeometryBuffer_h_
#define _CEGUIOgreGeometryBuffer_h_

#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/RendererModules/Ogre/Renderer.h"
#include "CEGUI/RendererModules/Ogre/ShaderWrapper.h"
#include "CEGUI/Rectf.h"

#include <OgreColourValue.h>
#include <OgreRenderOperation.h>
#include <OgreTexture.h>
#include <OgreMatrix4.h>

#ifdef CEGUI_USE_OGRE_HLMS
#include <OgreHardwareBuffer.h>
#endif //CEGUI_USE_OGRE_HLMS

#include <utility>
#include <vector>

// Ogre forward refs
namespace Ogre
{
class RenderSystem;
class Viewport;
}

// Start of CEGUI namespace section
namespace CEGUI
{
//! Implementation of CEGUI::GeometryBuffer for the Ogre engine
class OGRE_GUIRENDERER_API OgreGeometryBuffer : public GeometryBuffer
{
public:

    enum MANUALOBJECT_TYPE
    {
        MT_COLOURED,
        MT_TEXTURED,
        MT_INVALID
    };

    //! Constructor
    OgreGeometryBuffer(OgreRenderer& owner, Ogre::RenderSystem& rs, 
        CEGUI::RefCounted<RenderMaterial> renderMaterial);

    //! Destructor
    virtual ~OgreGeometryBuffer();

    virtual void draw() const override;
    virtual void appendGeometry(const float* vertex_data,
        std::size_t array_size) override;
    virtual void reset() override;
    virtual int getVertexAttributeElementCount() const override;

    void finaliseVertexAttributes(MANUALOBJECT_TYPE type);

protected:

    //! Updates the cached matrix. This should only be called after the RenderTarget was set.
    void updateMatrix() const;

#ifdef CEGUI_USE_OGRE_HLMS
    //! Sets the current scissor rect active
    void setScissorRects(Ogre::Viewport* current_viewport) const; 
#else
    //! Sets the current scissor rect active
    void setScissorRects() const; 
#endif //CEGUI_USE_OGRE_HLMS


    void syncVertexData() const;

    void setVertexBuffer(size_t count) const;

    void cleanUpVertexAttributes();

    //! Renderer object that owns this GeometryBuffer
    OgreRenderer& d_owner;
    //! Ogre render system we're to use.
    Ogre::RenderSystem& d_renderSystem;

    //! model matrix cache
    mutable glm::mat4 d_matrix;

    //! The type of vertex data we expect
    MANUALOBJECT_TYPE d_expectedData;

#ifdef CEGUI_USE_OGRE_HLMS
    //! Render operation for this buffer.
    mutable Ogre::v1::RenderOperation d_renderOp;

    //! H/W buffer where the vertices are rendered from.
    mutable Ogre::v1::HardwareVertexBufferSharedPtr d_hwBuffer;
#else
    //! Render operation for this buffer.
    mutable Ogre::RenderOperation d_renderOp;

    //! H/W buffer where the vertices are rendered from.
    mutable Ogre::HardwareVertexBufferSharedPtr d_hwBuffer;    
#endif //CEGUI_USE_OGRE_HLMS

    //! Marks the d_hwBuffer as being out of date
    mutable bool d_dataAppended;

    //! The old alpha value
    mutable float d_previousAlphaValue;
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIOgreGeometryBuffer_h_
