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
#ifndef _CEGUIShaderWrapper_h_
#define _CEGUIShaderWrapper_h_

#include "CEGUI/Base.h"

namespace CEGUI
{
class ShaderParameterBindings;

/*!
\brief
    The ShaderWrapper is an abstract interface that is used in RenderMaterials
    and has to be implemented by the specific Renderer. It updates all the
    ShaderParameters set for a RenderMaterial before rendering.
*/
class CEGUIEXPORT ShaderWrapper
{
public:
    ShaderWrapper()
    {}

    virtual ~ShaderWrapper()
    {}

    /*!
    \brief
        This function applies the shader parameters depending on their type,
        so that they will be used during rendering

    \param ShaderParameterBindings
        The ShaderParameterBindings that will be applied

    */
    virtual void prepareForRendering(const ShaderParameterBindings* shaderParameterBindings) = 0;
};

}

#endif
