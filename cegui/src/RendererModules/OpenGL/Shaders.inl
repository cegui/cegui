/***********************************************************************
    created:    Thu, 26th June 2014
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

namespace CEGUI
{

//! A string containing an OpenGL3 vertex shader for solid colouring of a polygon
const char StandardShaderSolidVert[] = 
"#version 150 core\n"

"uniform mat4 modelViewProjMatrix;\n"

"in vec3 inPosition;\n"
"in vec4 inColour;\n"

"out vec4 exColour;\n"

"void main(void)\n"
"{\n"
    "exColour = inColour;\n"

    "gl_Position = modelViewProjMatrix * vec4(inPosition, 1.0);\n"
"}"
;

//! A string containing an OpenGL3 fragment shader for solid colouring of a polygon
const char StandardShaderSolidFrag[] = 
"#version 150 core\n"

"in vec4 exColour;\n"

"out vec4 out0;\n"

"uniform float alphaPercentage;\n"

"void main(void)\n"
"{\n"
    "out0 = exColour;\n"
    "out0.a *= alphaPercentage;\n"
"}"
;

/*!
A string containing an OpenGL3 vertex shader for polygons that should be coloured
based on a texture. The fetched texture colour will be multiplied by a colour
supplied to the shader, resulting in the final colour.
*/
const char StandardShaderTexturedVert[] = 
"#version 150 core\n"

"uniform mat4 modelViewProjMatrix;\n"

"in vec3 inPosition;\n"
"in vec2 inTexCoord;\n"
"in vec4 inColour;\n"

"out vec2 exTexCoord;\n"
"out vec4 exColour;\n"

"void main(void)\n"
"{\n"
    "exTexCoord = inTexCoord;\n"
    "exColour = inColour;\n"

    "gl_Position = modelViewProjMatrix * vec4(inPosition, 1.0);\n"
"}"
;

/*!
A string containing an OpenGL3 fragment shader for polygons that should be coloured
based on a texture. The fetched texture colour will be multiplied by a colour
supplied to the shader, resulting in the final colour.
*/
const char StandardShaderTexturedFrag[] = 
"#version 150 core\n"

"uniform sampler2D texture0;\n"

"in vec2 exTexCoord;\n"
"in vec4 exColour;\n"

"out vec4 out0;\n"

"uniform float alphaPercentage;\n"

"void main(void)\n"
"{\n"
    "out0 = texture(texture0, exTexCoord) * exColour;\n"
    "out0.a *= alphaPercentage;\n"
"}"
;

//! A string containing an GLSLES 3.0 vertex shader for solid colouring of a polygon
const char StandardShaderSolidVertGLSLES3[] = 
"#version 300 es\n"

"uniform mat4 modelViewProjMatrix;\n"

"in vec3 inPosition;\n"
"in vec4 inColour;\n"

"out vec4 exColour;\n"

"void main(void)\n"
"{\n"
    "exColour = inColour;\n"

    "gl_Position = modelViewProjMatrix * vec4(inPosition, 1.0);\n"
"}"
;

//! A string containing an GLSLES 3.0 fragment shader for solid colouring of a polygon
const char StandardShaderSolidFragGLSLES3[] = 
"#version 300 es\n"

"in vec4 exColour;\n"

"out vec4 out0;\n"

"uniform float alphaPercentage;\n"

"void main(void)\n"
"{\n"
    "out0 = exColour;\n"
    "out0.a *= alphaPercentage;\n"
"}"
;

/*!
A string containing an GLSLES 3.0 vertex shader for polygons that should be coloured
based on a texture. The fetched texture colour will be multiplied by a colour
supplied to the shader, resulting in the final colour.
*/
const char StandardShaderTexturedVertGLSLES3[] = 
"#version 300 es\n"

"uniform mat4 modelViewProjMatrix;\n"

"in vec3 inPosition;\n"
"in vec2 inTexCoord;\n"
"in vec4 inColour;\n"

"out vec2 exTexCoord;\n"
"out vec4 exColour;\n"

"void main(void)\n"
"{\n"
    "exTexCoord = inTexCoord;\n"
    "exColour = inColour;\n"

    "gl_Position = modelViewProjMatrix * vec4(inPosition, 1.0);\n"
"}"
;

/*!
A string containing an GLSLES 3.0 fragment shader for polygons that should be coloured
based on a texture. The fetched texture colour will be multiplied by a colour
supplied to the shader, resulting in the final colour.
*/
const char StandardShaderTexturedFragGLSLES3[] = 
"#version 300 es\n"

"uniform sampler2D texture0;\n"

"in vec2 exTexCoord;\n"
"in vec4 exColour;\n"

"out vec4 out0;\n"

"uniform float alphaPercentage;\n"

"void main(void)\n"
"{\n"
    "out0 = texture(texture0, exTexCoord) * exColour;\n"
    "out0.a *= alphaPercentage;\n"
"}"
;

/*!  A string containing an OpenGL ES 2.0 / GLES 1.0  vertex shader for solid
 */
const char StandardShaderSolidVertGLSLES1[] = 
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
;

/*!  A string containing an OpenGL ES 2.0 / GLES 1.0  fragment shader for solid
 */
const char StandardShaderSolidFragGLSLES1[] = 
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
;

/*!
A string containing an OpenGL ES 2.0 / GLES 1.0 vertex shader for polygons that should be coloured
based on a texture. 
*/
const char StandardShaderTexturedVertGLSLES1[] = 
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
;

/*!
A string containing an OpenGL ES 2.0 / GLES 1.0 fragment shader for polygons that should be coloured
based on a texture. 
*/
const char StandardShaderTexturedFragGLSLES1[] = 
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
;

}

