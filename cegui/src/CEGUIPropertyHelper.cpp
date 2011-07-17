/***********************************************************************
    filename:   CEGUIPropertyHelper.cpp
    created:    6/7/2004
    author:     Paul D Turner

    purpose:    Implementation of PropertyHelper methods
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#include "CEGUIPropertyHelper.h"
#include "CEGUIImageManager.h"
#include "CEGUIImage.h"
#include "CEGUIFontManager.h"
#include "CEGUIFont.h"
#include "CEGUIExceptions.h"

#include <cstdio>
#include <sstream>

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

// Start of CEGUI namespace section
namespace CEGUI
{

//----------------------------------------------------------------------------//
PropertyHelper<Image*>::return_type
PropertyHelper<Image*>::fromString(const String& str)
{
    using namespace std;

    // handle empty string case
    if (str.empty())
        return 0;

    PropertyHelper<Image*>::return_type image;

    CEGUI_TRY
    {
        image = &ImageManager::getSingleton().get(str);
    }
    CEGUI_CATCH(UnknownObjectException&)
    {
        image = 0;
    }

    return image;
}

//----------------------------------------------------------------------------//
PropertyHelper<Image*>::string_return_type PropertyHelper<Image*>::toString(
    PropertyHelper<Image*>::pass_type val)
{
    return val ? val->getName() : String("");
}

//----------------------------------------------------------------------------//
PropertyHelper<Font*>::return_type
PropertyHelper<Font*>::fromString(const String& str)
{
    using namespace std;

    // handle empty string case
    if (str.empty())
        return 0;

    PropertyHelper<Font*>::return_type image;

    CEGUI_TRY
    {
        image = &FontManager::getSingleton().get(str);
    }
    CEGUI_CATCH(UnknownObjectException&)
    {
        image = 0;
    }

    return image;
}

//----------------------------------------------------------------------------//
PropertyHelper<Font*>::string_return_type PropertyHelper<Font*>::toString(
    PropertyHelper<Font*>::pass_type val)
{
    return val ? val->getName() : String("");
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
