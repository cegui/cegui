/***********************************************************************
    created:    Fri, 4th July 2014
    author:     Henri I Hyyryläinen
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
#ifndef _CEGUIOgreShaderWrapper_h_
#define _CEGUIOgreShaderWrapper_h_

#include <string>
#include "Renderer.h"
#include "CEGUI/ShaderWrapper.h"
#include "OgreHighLevelGpuProgram.h"

#include "glm/glm.hpp"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
class ShaderParameterBindings;
class ShaderParameter;

//----------------------------------------------------------------------------//
class OGRE_GUIRENDERER_API OgreShaderWrapper : public ShaderWrapper
{
public:
    OgreShaderWrapper(OgreRenderer& owner, Ogre::RenderSystem& rs, 
        Ogre::HighLevelGpuProgramPtr vs, Ogre::HighLevelGpuProgramPtr ps);

    ~OgreShaderWrapper();

    //Implementation of ShaderWrapper interface
    void prepareForRendering(const ShaderParameterBindings* shaderParameterBindings);

    Ogre::GpuProgramParametersSharedPtr getVertexParameters() const;

protected:

    //! Renderer object that owns this GeometryBuffer
    OgreRenderer& d_owner;
    //! Ogre render system we're to use.
    Ogre::RenderSystem& d_renderSystem;

    //! The GPU program that is our vertex shader
    Ogre::HighLevelGpuProgramPtr d_vertexShader;

    //! Parameters for vertex shader
    Ogre::GpuProgramParametersSharedPtr d_vertexParameters;

    //! The GPU program that is our pixel shader
    Ogre::HighLevelGpuProgramPtr d_pixelShader;

    //! Parameters for pixel shader
    Ogre::GpuProgramParametersSharedPtr d_pixelParameters;

    //! The currently active matrix
    glm::mat4 d_lastMatrix;

    //! The current alpha value
    float d_previousAlpha;

    //! The physical index to which the matrix will be written
    size_t d_physicalIndex;

    //! Stores the index where a given type of parameter is bound
    //! \note Could be using ShaderParamType but the include is avoided by using an int instead
    std::map<int, size_t> d_paramTypeToIndex;
};

}

#endif

