/***********************************************************************
    created:    3/1/2011
    author:     Martin Preisler

    purpose:    Implements the Quaternion class
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

#include "CEGUI/Quaternion.h"
#include "CEGUI/String.h"
#include "CEGUI/PropertyHelper.h"
#include "CEGUI/Exceptions.h"
#include <limits>

// Start of CEGUI namespace section
namespace CEGUI
{

//----------------------------------------------------------------------------//
const String& QuaternionSlerpInterpolator::getType() const
{
    static String type("QuaternionSlerp");

    return type;
}

//----------------------------------------------------------------------------//
String QuaternionSlerpInterpolator::interpolateAbsolute(const String& value1,
                                    const String& value2,
                                    float position)
{
    Helper::return_type val1 = Helper::fromString(value1);
    Helper::return_type val2 = Helper::fromString(value2);

    return Helper::toString(glm::slerp(val1, val2, position));
}

//----------------------------------------------------------------------------//
String QuaternionSlerpInterpolator::interpolateRelative(const String& base,
                                    const String& value1,
                                    const String& value2,
                                    float position)
{
    Helper::return_type bas = Helper::fromString(base);
    Helper::return_type val1 = Helper::fromString(value1);
    Helper::return_type val2 = Helper::fromString(value2);

    return Helper::toString(bas * glm::slerp(val1, val2, position));
}

//----------------------------------------------------------------------------//
String QuaternionSlerpInterpolator::interpolateRelativeMultiply(
                                            const String& /*base*/,
                                            const String& /*value1*/,
                                            const String& /*value2*/,
                                            float /*position*/)
{
    throw InvalidRequestException("ApplicationMethod::RELATIVE_MULTIPLY doesn't make sense "
        "with Quaternions! Please use absolute or relative application method.");

    return Helper::toString(glm::quat(1, 0, 0, 0));
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
