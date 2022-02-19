/***********************************************************************
    created:    18th July 2013
    author:     Lukas Meindl
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
#ifndef _CEGUIRenderMaterial_h_
#define _CEGUIRenderMaterial_h_

#include "CEGUI/ShaderParameterBindings.h"

namespace CEGUI
{
class ShaderWrapper;

/*!
\brief
    A RenderMaterial is used for rendering GeometryBuffers. It contains
    a pointer to the used shader (ShaderWrapper) and owns shader parameters.
*/
class CEGUIEXPORT RenderMaterial final
{
public:

    RenderMaterial(ShaderWrapper* shaderWrapper);

    /*!
	\brief
		Return the ShaderParameterBindings of this Renderer.

	\return
		The pointer to the ShaderParameterBindings.
	*/
    ShaderParameterBindings* getShaderParamBindings() { return &d_shaderParamBindings; }
    const ShaderParameterBindings* getShaderParamBindings() const { return &d_shaderParamBindings; }

    void setMainTexture(const Texture* texture);
    const Texture* getMainTexture() const;

    /*!
	\brief
		Returns a pointer to the ShaderWrapper used for this Renderer.

	\return
		The pointer to the ShaderWrapper used for this Renderer.
	*/
    const ShaderWrapper* getShaderWrapper() const { return d_shaderWrapper; }

    //! \brief Applies the shader parameter bindings to the shader of this material.
    void prepareForRendering() const;

protected:

    //! pointer to the Shader that is used in this material.
    ShaderWrapper* d_shaderWrapper = nullptr;

    //! Cached shader parameter for the main texture, optimization for frequent access
    ShaderParameterTexture* d_mainTextureParam = nullptr;

    //! data structure that contains all the shader parameters that are
    //  to be sent to the shader program using the parameter's name
    ShaderParameterBindings d_shaderParamBindings;
};

}

#endif
