/***********************************************************************
    filename:   Shaders.inl
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

const char VertexShaderTextured[] = ""
"float4x4 modelViewPerspMatrix;\n"
"\n"
"struct VertOut\n"
"{\n"
"	float4 pos : SV_Position;\n"
"	float4 colour : COLOR;\n"
"	float2 texcoord0 : TEXCOORD;\n"
"};\n"
"\n"
"// Vertex shader\n"
"VertOut VSMain(float4 inPos : POSITION, float4 inColour : COLOR, float2 inTexCoord0 : TEXCOORD)\n"
"{\n"
"	VertOut output;\n"
"\n"
"   output.pos = mul(modelViewPerspMatrix, inPos);\n"
"   output.texcoord0 = inTexCoord0;\n"
"	output.colour.rgba = inColour.bgra;\n"
"\n"
"	return output;\n"
"}\n"
;

const char PixelShaderTextured[] = ""
"Texture2D texture0;\n"
"SamplerState textureSamplerState;\n"
"\n"
"struct VertOut\n"
"{\n"
"	float4 pos : SV_Position;\n"
"	float4 colour : COLOR;\n"
"	float2 texcoord0 : TEXCOORD;\n"
"};\n"
"\n"
"float4 PSMain(VertOut input) : SV_Target\n"
"{\n"
"	float4 colour = texture0.Sample(textureSamplerState, input.texcoord0) * input.colour;\n"
"	return colour;\n"
"}\n"
"\n"
;

const char VertexShaderColoured[] = ""
"float4x4 modelViewPerspMatrix;\n"
"\n"
"struct VertOut\n"
"{\n"
"	float4 pos : SV_Position;\n"
"	float4 colour : COLOR;\n"
"};\n"
"\n"
"VertOut VSMain(float4 inPos : POSITION, float4 inColour : COLOR)\n"
"{\n"
"	VertOut output;\n"
"\n"
"   output.pos = mul(modelViewPerspMatrix, inPos);\n"
"	output.colour.rgba = inColour.bgra;\n"
"\n"
"	return output;\n"
"}\n"
;

const char PixelShaderColoured[] = ""
"Texture2D texture0;\n"
"\n"
"struct VertOut\n"
"{\n"
"	float4 pos : SV_Position;\n"
"	float4 colour : COLOR;\n"
"};\n"
"\n"
"float4 PSMain(VertOut input) : SV_Target\n"
"{\n"
"	return input.colour;\n"
"}\n"
"\n"
;