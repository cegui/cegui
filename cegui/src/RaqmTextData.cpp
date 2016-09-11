/***********************************************************************
    created:    Wed Aug 24 2016
    author:     Lukas Meindl <cegui@lukasmeindl.at>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2016 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/RaqmTextData.h"
#include "CEGUI/Logger.h"
#include "CEGUI/Exceptions.h"

namespace CEGUI
{
RaqmTextData::RaqmTextData()
    : d_raqmObject(nullptr)
{
    d_raqmObject = raqm_create();

    if (d_raqmObject == nullptr)
    {
        throw InvalidRequestException("Could not create raqm object");
    }
}

void RaqmTextData::update(FT_Face fontFace, raqm_direction_t parseDirection)
{
    if (!raqm_set_freetype_face(d_raqmObject, fontFace))
    {
        throw InvalidRequestException("Could not set the Freetype font Face for "
            "a raqm object");
    }
    
    if (!raqm_set_par_direction(d_raqmObject, parseDirection))
    {
        throw InvalidRequestException("Could not set the parse direction for "
            "a raqm object");
    }
}

RaqmTextData::~RaqmTextData()
{
    if (d_raqmObject)
    {
        raqm_destroy(d_raqmObject);
    }
}

void RaqmTextData::updateText(const String& newText)
{
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8) || (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_ASCII)
    bool wasSuccess = raqm_set_text_utf8(d_raqmObject, newText.c_str(), newText.length())
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32) 
    const uint32_t* newTextPointer = reinterpret_cast<const std::uint32_t*>(newText.c_str());
    bool wasSuccess = raqm_set_text(d_raqmObject, newTextPointer, newText.length());
#endif

    if(!wasSuccess)
    {
        throw InvalidRequestException("Setting raqm text was unsuccessful");
    }
}

}