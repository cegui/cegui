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

namespace CEGUI
{
// Shaders for Ogre renderer adapted from OpenGL and Direct3D11 shaders

//! A string containing an HLSL vertex shader for solid colouring of a polygon
static Ogre::String VertexShaderColoured_HLSL(""
"float4x4 modelViewProjMatrix;\n"
"\n"
"struct VertOut\n"
"{\n"
"	float4 pos : SV_Position;\n"
"	float4 colour : COLOR;\n"
"};\n"
"\n"
"VertOut main(float3 inPos : POSITION, float4 inColour : COLOR)\n"
"{\n"
"	VertOut output;\n"
"\n"
"   output.pos = mul(modelViewProjMatrix, float4(inPos, 1.0));\n"
"	output.colour = inColour;\n"
"\n"
"	return output;\n"
"}\n"
);

//! A string containing an HLSL fragment shader for solid colouring of a polygon
static Ogre::String PixelShaderColoured_HLSL(""
"uniform float alphaPercentage;\n"
"\n"
"struct VS_OUT\n"
"{\n"
"	float4 position : POSITION;\n"
"	float4 colour : COLOR;\n"
"};\n"
"\n"
"float4 main(VS_OUT input) : COLOR\n"
"{\n"
"   float4 colour = input.colour;\n"
"   colour.a *= alphaPercentage;\n"
"	return colour;\n"
"}\n"
"\n"
);

/*!
A string containing an HLSL vertex shader for polygons that should be coloured
based on a texture. The fetched texture colour will be multiplied by a colour
supplied to the shader, resulting in the final colour.
*/
static Ogre::String VertexShaderTextured_HLSL(""
"float4x4 modelViewProjMatrix;\n"
"\n"
"struct VertOut\n"
"{\n"
"	float4 pos : SV_Position;\n"
"	float4 colour : COLOR;\n"
"	float2 texcoord0 : TEXCOORD;\n"
"};\n"
"\n"
"// Vertex shader\n"
"VertOut main(float4 inPos : POSITION, float4 inColour : COLOR, float2 inTexCoord0 : TEXCOORD)\n"
"{\n"
"	VertOut output;\n"
"\n"
"   output.pos = mul(modelViewProjMatrix, inPos);\n"
"   output.texcoord0 = inTexCoord0;\n"
"	output.colour = inColour;\n"
"\n"
"	return output;\n"
"}\n"
);

/*!
A string containing an HLSL fragment shader for polygons that should be coloured
based on a texture. The fetched texture colour will be multiplied by a colour
supplied to the shader, resulting in the final colour.
*/
static Ogre::String PixelShaderTextured_HLSL(""
"uniform float alphaPercentage;\n"
"struct VS_OUT\n"
"{\n"
"	float4 position : POSITION;\n"
"	float4 colour : COLOR;\n"
"   float2 uv : TexCoord0;\n"
"};\n"
"\n"
"float4 main(float4 colour : COLOR, float2 uv : TexCoord0, "
"               uniform sampler2D texture0 : TEXUNIT0) : COLOR\n"
"{\n"
"   colour = tex2D(texture0, uv) * colour;\n"
"   colour.a *= alphaPercentage;\n"
"	return colour;\n"
"}\n"
"\n"
);

//! Shader for older OpenGL versions < 3
static Ogre::String VertexShaderTextured_GLSL_Compat(""
    "uniform mat4 modelViewProjMatrix;\n"
    "void main(void)"
    "{"
    "    gl_TexCoord[0] = gl_MultiTexCoord0;"
    "    gl_FrontColor = gl_Color;"
    "    gl_Position = modelViewProjMatrix * gl_Vertex;"
    "}"
);

//! Shader for older OpenGL versions < 3
static Ogre::String PixelShaderTextured_GLSL_Compat(""
    "uniform sampler2D texture0;"
    "uniform float alphaPercentage;\n"
    "void main(void)"
    "{"
    "    gl_FragColor = texture2D(texture0, gl_TexCoord[0].st) * gl_Color;"
    "    gl_FragColor.a *= alphaPercentage;\n"
    "}"
);

//! Shader for older OpenGL versions < 3
static Ogre::String VertexShaderColoured_GLSL_Compat(""
    "uniform mat4 modelViewProjMatrix;\n"
    "void main(void)"
    "{"
    "    gl_FrontColor = gl_Color;"
    "    gl_Position = modelViewProjMatrix * gl_Vertex;"
    "}"
);

//! Shader for older OpenGL versions < 3
static Ogre::String PixelShaderColoured_GLSL_Compat(""
    "uniform float alphaPercentage;\n"
    "void main(void)\n"
    "{"
    "    gl_FragColor = gl_Color;"
    "    gl_FragColor.a *= alphaPercentage;\n"
    "}"
);

//! A string containing an OpenGL3 vertex shader for solid colouring of a polygon
static Ogre::String VertexShaderColoured_GLSL(""
    "#version 150 core\n"

    "uniform mat4 modelViewProjMatrix;\n"

    "in vec4 vertex;\n"
    "in vec4 colour;\n"

    "out vec4 exColour;"

    "void main(void)\n"
    "{\n"
    "   exColour = colour;\n"

    "   gl_Position = modelViewProjMatrix * vertex;\n"
    "}"
);

//! A string containing an OpenGL3 fragment shader for solid colouring of a polygon
static Ogre::String PixelShaderColoured_GLSL(""
    "#version 150 core\n"

    "in vec4 exColour;\n"

    "out vec4 fragColour;\n"

    "uniform float alphaPercentage;\n"

    "void main(void)\n"
    "{\n"
    "   fragColour = exColour;\n"
    "   fragColour.a *= alphaPercentage;\n"
    "}"
);

/*!
A string containing an OpenGL3 vertex shader for polygons that should be coloured
based on a texture. The fetched texture colour will be multiplied by a colour
supplied to the shader, resulting in the final colour.
*/
static Ogre::String VertexShaderTextured_GLSL("" 
    "#version 150 core\n"

    "uniform mat4 modelViewProjMatrix;\n"

    "in vec4 vertex;\n"
    "in vec4 colour;\n"
    "in vec2 uv0;\n"

    "out vec2 exTexCoord;\n"
    "out vec4 exColour;\n"

    "void main()\n"
    "{\n"
    "   exTexCoord = uv0;\n"
    "   exColour = colour;\n"

    "   gl_Position = modelViewProjMatrix * vertex;\n"
    "}"
);

/*!
A string containing an OpenGL3 fragment shader for polygons that should be coloured
based on a texture. The fetched texture colour will be multiplied by a colour
supplied to the shader, resulting in the final colour.
*/
static Ogre::String PixelShaderTextured_GLSL("" 
    "#version 150 core\n"

    "uniform sampler2D texture0;\n"
    "uniform float alphaPercentage;\n"


    "in vec2 exTexCoord;\n"
    "in vec4 exColour;\n"

    "out vec4 fragColour;\n"

    "void main(void)\n"
    "{\n"
    "   fragColour = texture(texture0, exTexCoord) * exColour;\n"
    "   fragColour.a *= alphaPercentage;\n"
    "}"
);

//! A string containing an OpenGL ES 2.0 / GLES 1.0 vertex shader for solid colouring of a polygon
static Ogre::String VertexShaderColoured_GLSLES1(""
    "#version 100\n"
    "precision mediump int;\n"
    "precision mediump float;\n"
    "uniform mat4 modelViewProjMatrix;\n"
    "attribute vec4 vertex;\n"
    "attribute vec4 colour;\n"
    "varying vec4 exColour;\n"
    "void main(void)\n"
    "{\n"
    "   exColour = colour;\n"
    "   gl_Position = modelViewProjMatrix * vertex;\n"
    "}"
);

//! A string containing an OpenGL ES 2.0 / GLES 1.0 fragment shader for solid colouring of a polygon
static Ogre::String PixelShaderColoured_GLSLES1(""
    "#version 100\n"
    "precision mediump int;\n"
    "precision mediump float;\n"
    "varying vec4 exColour;\n"
    "uniform float alphaPercentage;\n"
    "void main(void)\n"
    "{\n"
    "     gl_FragColor = exColour;\n"
    "     gl_FragColor.a *= alphaPercentage;\n"
    "}"
);

/*!
A string containing an OpenGL ES 2.0 / GLES 1.0 vertex shader for polygons that should be coloured
based on a texture. The fetched texture colour will be multiplied by a colour
supplied to the shader, resulting in the final colour.
*/
static Ogre::String VertexShaderTextured_GLSLES1("" 
    "#version 100\n"
    "precision mediump int;\n"
    "precision mediump float;\n"
    "uniform mat4 modelViewProjMatrix;\n"
    "attribute vec4 vertex;\n"
    "attribute vec4 colour;\n"
    "attribute vec2 uv0;\n"
    "varying vec2 exTexCoord;\n"
    "varying vec4 exColour;\n"
    "void main()\n"
    "{\n"
    "   exTexCoord = uv0;\n"
    "   exColour = colour;"
    "   gl_Position = modelViewProjMatrix * vertex;\n"
    "}"
);

/*!
A string containing an OpenGL ES 2.0 / GLES 1.0 fragment shader for polygons that should be coloured
based on a texture. The fetched texture colour will be multiplied by a colour
supplied to the shader, resulting in the final colour.
*/
static Ogre::String PixelShaderTextured_GLSLES1("" 
    "#version 100\n"
    "precision mediump int;\n"
    "precision mediump float;\n"
    "precision lowp sampler2D;\n"
    "uniform sampler2D texture0;\n"
    "uniform float alphaPercentage;\n"
    "varying vec2 exTexCoord;\n"
    "varying vec4 exColour;\n"
    "void main(void)\n"
    "{\n"
    "     gl_FragColor = texture2D(texture0, exTexCoord) * exColour;\n"
    "     gl_FragColor.a *= alphaPercentage;\n"
    "}"
);


}

