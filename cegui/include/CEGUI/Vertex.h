/***********************************************************************
    created:    Thu Jan 8 2009
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIVertex_h_
#define _CEGUIVertex_h_

#include "CEGUI/Colour.h"
#include <glm/glm.hpp>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace CEGUI
{
/*!
\brief
    Structure that is used to hold the attributes of coloured geometry
    in 3D space.
*/
struct CEGUIEXPORT ColouredVertex
{
    ColouredVertex() = default;
    ColouredVertex(const glm::vec3& position, const glm::vec4& colour) :
        d_position(position),
        d_colour(colour)
    {}

    //! Sets the colour of the struct
    void setColour(const Colour& colour)
    {
        d_colour.r = colour.getRed();
        d_colour.g = colour.getGreen();
        d_colour.b = colour.getBlue();
        d_colour.a = colour.getAlpha();
    }

    glm::vec3 d_position; //<! Position of the vertex in 3D space.
    glm::vec4 d_colour;   //<! Colour attribute of the vertex.
};

/*!
\brief
    Structure that is used to hold the attributes of a vertex for coloured and
    textured geometry in 3D space.
*/
struct CEGUIEXPORT TexturedColouredVertex : public ColouredVertex
{
    TexturedColouredVertex() = default;
    TexturedColouredVertex(const glm::vec3& position, const glm::vec4& colour, const glm::vec2& texCoords) :
        ColouredVertex(position, colour),
        d_texCoords(texCoords)
    {}
    
    glm::vec2 d_texCoords; //<! Texture coordinates of the vertex.
};

}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _CEGUIVertex_h_
