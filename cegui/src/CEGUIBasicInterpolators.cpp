/***********************************************************************
    filename:   CEGUIBasicInterpolators.cpp
    created:    7/8/2010
    author:     Martin Preisler

    purpose:    Implements basic intergrated interpolators
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
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
#include "CEGUIBasicInterpolators.h"
#include "CEGUIPropertyHelper.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/********************************************
* String interpolator
********************************************/
const String& StringInterpolator::getType() const
{
    static String type = "String";
    return type;
}

//----------------------------------------------------------------------------//
String StringInterpolator::interpolateAbsolute(const String& value1,
        const String& value2,
        float position)
{
    return position < 0.5 ? value1 : value2;
}

//----------------------------------------------------------------------------//
String StringInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    return base + (position < 0.5 ? value1 : value2);
}

//----------------------------------------------------------------------------//
String StringInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    //const float val1 = PropertyHelper::stringToFloat(value1);
    //const float val2 = PropertyHelper::stringToFloat(value2);

    //const float mul = val1 * (1.0f - position) + val2 * (position);

    // todo: some fancy length cutting?
    return base;
}

/********************************************
* Float interpolator
********************************************/
const String& FloatInterpolator::getType() const
{
    static String type = "float";
    return type;
}

//----------------------------------------------------------------------------//
String FloatInterpolator::interpolateAbsolute(const String& value1,
        const String& value2,
        float position)
{
    const float val1 = PropertyHelper::stringToFloat(value1);
    const float val2 = PropertyHelper::stringToFloat(value2);

    const float result = val1 * (1.0f - position) + val2 * (position);

    return PropertyHelper::floatToString(result);
}

//----------------------------------------------------------------------------//
String FloatInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const float bas = PropertyHelper::stringToFloat(base);
    const float val1 = PropertyHelper::stringToFloat(value1);
    const float val2 = PropertyHelper::stringToFloat(value2);

    const float result = bas + (val1 * (1.0f - position) + val2 * (position));

    return PropertyHelper::floatToString(result);
}

//----------------------------------------------------------------------------//
String FloatInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const float bas = PropertyHelper::stringToFloat(base);
    const float val1 = PropertyHelper::stringToFloat(value1);
    const float val2 = PropertyHelper::stringToFloat(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const float result = bas * mul;

    return PropertyHelper::floatToString(result);
}

/********************************************
* Uint interpolator
********************************************/
const String& UintInterpolator::getType() const
{
    static String type = "uint";
    return type;
}

//----------------------------------------------------------------------------//
String UintInterpolator::interpolateAbsolute(const String& value1,
        const String& value2,
        float position)
{
    const uint val1 = PropertyHelper::stringToUint(value1);
    const uint val2 = PropertyHelper::stringToUint(value2);

    const uint result = static_cast<uint>(
                            static_cast<float>(val1) * (1.0f - position) +
                            static_cast<float>(val2) * (position));

    return PropertyHelper::uintToString(result);
}

//----------------------------------------------------------------------------//
String UintInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const uint bas = PropertyHelper::stringToUint(base);
    const uint val1 = PropertyHelper::stringToUint(value1);
    const uint val2 = PropertyHelper::stringToUint(value2);

    const uint result = bas + static_cast<uint>(
                            static_cast<float>(val1) * (1.0f - position) +
                            static_cast<float>(val2) * (position));

    return PropertyHelper::uintToString(result);
}

//----------------------------------------------------------------------------//
String UintInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const uint bas = PropertyHelper::stringToUint(base);
    const float val1 = PropertyHelper::stringToFloat(value1);
    const float val2 = PropertyHelper::stringToFloat(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const uint result = static_cast<uint>(bas * mul);

    return PropertyHelper::uintToString(result);
}

/********************************************
* Int interpolator
********************************************/
const String& IntInterpolator::getType() const
{
    static String type = "int";
    return type;
}

//----------------------------------------------------------------------------//
String IntInterpolator::interpolateAbsolute(const String& value1,
        const String& value2,
        float position)
{
    const int val1 = PropertyHelper::stringToInt(value1);
    const int val2 = PropertyHelper::stringToInt(value2);

    const int result = static_cast<int>(
                           static_cast<float>(val1) * (1.0f - position) +
                           static_cast<float>(val2) * (position));

    return PropertyHelper::intToString(result);
}

//----------------------------------------------------------------------------//
String IntInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const int bas = PropertyHelper::stringToInt(base);
    const int val1 = PropertyHelper::stringToInt(value1);
    const int val2 = PropertyHelper::stringToInt(value2);

    const int result = bas + static_cast<int>(
                           static_cast<float>(val1) * (1.0f - position) +
                           static_cast<float>(val2) * (position));

    return PropertyHelper::intToString(result);
}

//----------------------------------------------------------------------------//
String IntInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const int bas = PropertyHelper::stringToInt(base);
    const float val1 = PropertyHelper::stringToFloat(value1);
    const float val2 = PropertyHelper::stringToFloat(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const int result = static_cast<int>(bas * mul);

    return PropertyHelper::intToString(result);
}

/********************************************
* Bool interpolator
********************************************/
const String& BoolInterpolator::getType() const
{
    static String type = "bool";
    return type;
}

//----------------------------------------------------------------------------//
String BoolInterpolator::interpolateAbsolute(const String& value1,
        const String& value2,
        float position)
{
    const bool val1 = PropertyHelper::stringToBool(value1);
    const bool val2 = PropertyHelper::stringToBool(value2);

    return PropertyHelper::boolToString(position < 0.5 ? val1 : val2);
}

//----------------------------------------------------------------------------//
String BoolInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    //const bool bas = PropertyHelper::stringToBool(base);
    const bool val1 = PropertyHelper::stringToBool(value1);
    const bool val2 = PropertyHelper::stringToBool(value2);

    return PropertyHelper::boolToString(position < 0.5 ? val1 : val2);
}

//----------------------------------------------------------------------------//
String BoolInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    //const bool bas = PropertyHelper::stringToBool(base);
    //const float val1 = PropertyHelper::stringToFloat(value1);
    //const float val2 = PropertyHelper::stringToFloat(value2);

    //const float mul = val1 * (1.0f - position) + val2 * (position);

    //const bool result = static_cast<bool>(bas * mul);

    //return PropertyHelper::boolToString(result);
    return base;
}

/********************************************
* Size interpolator
********************************************/
const String& SizeInterpolator::getType() const
{
    static String type = "Size";
    return type;
}

//----------------------------------------------------------------------------//
String SizeInterpolator::interpolateAbsolute(const String& value1,
        const String& value2,
        float position)
{
    const Size val1 = PropertyHelper::stringToSize(value1);
    const Size val2 = PropertyHelper::stringToSize(value2);

    const Size result = val1 * (1.0f - position) + val2 * (position);

    return PropertyHelper::sizeToString(result);
}

//----------------------------------------------------------------------------//
String SizeInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const Size bas = PropertyHelper::stringToSize(base);
    const Size val1 = PropertyHelper::stringToSize(value1);
    const Size val2 = PropertyHelper::stringToSize(value2);

    const Size result = bas + (val1 * (1.0f - position) + val2 * (position));

    return PropertyHelper::sizeToString(result);
}

//----------------------------------------------------------------------------//
String SizeInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const Size bas = PropertyHelper::stringToSize(base);
    const float val1 = PropertyHelper::stringToFloat(value1);
    const float val2 = PropertyHelper::stringToFloat(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const Size result = bas * mul;

    return PropertyHelper::sizeToString(result);
}

/********************************************
* Point interpolator
********************************************/
const String& PointInterpolator::getType() const
{
    static String type = "Point";
    return type;
}

//----------------------------------------------------------------------------//
String PointInterpolator::interpolateAbsolute(const String& value1,
        const String& value2,
        float position)
{
    const Point val1 = PropertyHelper::stringToPoint(value1);
    const Point val2 = PropertyHelper::stringToPoint(value2);

    const Point result = val1 * (1.0f - position) + val2 * (position);

    return PropertyHelper::pointToString(result);
}

//----------------------------------------------------------------------------//
String PointInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const Point bas = PropertyHelper::stringToPoint(base);
    const Point val1 = PropertyHelper::stringToPoint(value1);
    const Point val2 = PropertyHelper::stringToPoint(value2);

    const Point result = bas + (val1 * (1.0f - position) + val2 * (position));

    return PropertyHelper::pointToString(result);
}

//----------------------------------------------------------------------------//
String PointInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const Point bas = PropertyHelper::stringToPoint(base);
    const float val1 = PropertyHelper::stringToFloat(value1);
    const float val2 = PropertyHelper::stringToFloat(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const Point result = bas * mul;

    return PropertyHelper::pointToString(result);
}

/********************************************
* Vector3 interpolator
********************************************/
const String& Vector3Interpolator::getType() const
{
    static String type = "Vector3";
    return type;
}

//----------------------------------------------------------------------------//
String Vector3Interpolator::interpolateAbsolute(const String& value1,
        const String& value2,
        float position)
{
    const Vector3 val1 = PropertyHelper::stringToVector3(value1);
    const Vector3 val2 = PropertyHelper::stringToVector3(value2);

    const Vector3 result = val1 * (1.0f - position) + val2 * (position);

    return PropertyHelper::vector3ToString(result);
}

//----------------------------------------------------------------------------//
String Vector3Interpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const Vector3 bas = PropertyHelper::stringToVector3(base);
    const Vector3 val1 = PropertyHelper::stringToVector3(value1);
    const Vector3 val2 = PropertyHelper::stringToVector3(value2);

    const Vector3 result = bas + (val1 * (1.0f - position) + val2 * (position));

    return PropertyHelper::vector3ToString(result);
}

//----------------------------------------------------------------------------//
String Vector3Interpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const Vector3 bas = PropertyHelper::stringToVector3(base);
    const float val1 = PropertyHelper::stringToFloat(value1);
    const float val2 = PropertyHelper::stringToFloat(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const Vector3 result = bas * mul;

    return PropertyHelper::vector3ToString(result);
}

/********************************************
* Rect interpolator
********************************************/
const String& RectInterpolator::getType() const
{
    static String type = "Rect";
    return type;
}

//----------------------------------------------------------------------------//
String RectInterpolator::interpolateAbsolute(const String& value1,
        const String& value2,
        float position)
{
    const Rect val1 = PropertyHelper::stringToRect(value1);
    const Rect val2 = PropertyHelper::stringToRect(value2);

    const Rect result = val1 * (1.0f - position) + val2 * (position);

    return PropertyHelper::rectToString(result);
}

//----------------------------------------------------------------------------//
String RectInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const Rect bas = PropertyHelper::stringToRect(base);
    const Rect val1 = PropertyHelper::stringToRect(value1);
    const Rect val2 = PropertyHelper::stringToRect(value2);

    const Rect result = bas + (val1 * (1.0f - position) + val2 * (position));

    return PropertyHelper::rectToString(result);
}

//----------------------------------------------------------------------------//
String RectInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const Rect bas = PropertyHelper::stringToRect(base);
    const float val1 = PropertyHelper::stringToFloat(value1);
    const float val2 = PropertyHelper::stringToFloat(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const Rect result = bas * mul;

    return PropertyHelper::rectToString(result);
}

/********************************************
* Colour interpolator
********************************************/
const String& ColourInterpolator::getType() const
{
    static String type = "colour";
    return type;
}

//----------------------------------------------------------------------------//
String ColourInterpolator::interpolateAbsolute(const String& value1,
        const String& value2,
        float position)
{
    const colour val1 = PropertyHelper::stringToColour(value1);
    const colour val2 = PropertyHelper::stringToColour(value2);

    const colour result = val1 * (1.0f - position) + val2 * (position);

    return PropertyHelper::colourToString(result);
}

//----------------------------------------------------------------------------//
String ColourInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const colour bas = PropertyHelper::stringToColour(base);
    const colour val1 = PropertyHelper::stringToColour(value1);
    const colour val2 = PropertyHelper::stringToColour(value2);

    const colour result = bas + (val1 * (1.0f - position) + val2 * (position));

    return PropertyHelper::colourToString(result);
}

//----------------------------------------------------------------------------//
String ColourInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const colour bas = PropertyHelper::stringToColour(base);
    const float val1 = PropertyHelper::stringToFloat(value1);
    const float val2 = PropertyHelper::stringToFloat(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const colour result = bas * mul;

    return PropertyHelper::colourToString(result);
}

/********************************************
* ColourRect interpolator
********************************************/
const String& ColourRectInterpolator::getType() const
{
    static String type = "ColourRect";
    return type;
}

//----------------------------------------------------------------------------//
String ColourRectInterpolator::interpolateAbsolute(const String& value1,
        const String& value2,
        float position)
{
    const ColourRect val1 = PropertyHelper::stringToColourRect(value1);
    const ColourRect val2 = PropertyHelper::stringToColourRect(value2);

    const ColourRect result = val1 * (1.0f - position) + val2 * (position);

    return PropertyHelper::colourRectToString(result);
}

//----------------------------------------------------------------------------//
String ColourRectInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const ColourRect bas = PropertyHelper::stringToColourRect(base);
    const ColourRect val1 = PropertyHelper::stringToColourRect(value1);
    const ColourRect val2 = PropertyHelper::stringToColourRect(value2);

    const ColourRect result = bas + (val1 * (1.0f - position) + val2 * (position));

    return PropertyHelper::colourRectToString(result);
}

//----------------------------------------------------------------------------//
String ColourRectInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const ColourRect bas = PropertyHelper::stringToColourRect(base);
    const float val1 = PropertyHelper::stringToFloat(value1);
    const float val2 = PropertyHelper::stringToFloat(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const ColourRect result = bas * mul;

    return PropertyHelper::colourRectToString(result);
}

/********************************************
* UDim interpolator
********************************************/
const String& UDimInterpolator::getType() const
{
    static String type = "UDim";
    return type;
}

//----------------------------------------------------------------------------//
String UDimInterpolator::interpolateAbsolute(const String& value1,
        const String& value2,
        float position)
{
    const UDim val1 = PropertyHelper::stringToUDim(value1);
    const UDim val2 = PropertyHelper::stringToUDim(value2);

    const UDim result =
        val1 * UDim(1.0f - position, 1.0f - position) +
        val2 * UDim(position, position);

    return PropertyHelper::udimToString(result);
}

//----------------------------------------------------------------------------//
String UDimInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const UDim bas = PropertyHelper::stringToUDim(base);
    const UDim val1 = PropertyHelper::stringToUDim(value1);
    const UDim val2 = PropertyHelper::stringToUDim(value2);

    const UDim result = bas + (
                            val1 * UDim(1.0f - position, 1.0f - position) +
                            val2 * UDim(position, position));

    return PropertyHelper::udimToString(result);
}

//----------------------------------------------------------------------------//
String UDimInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const UDim bas = PropertyHelper::stringToUDim(base);
    const float val1 = PropertyHelper::stringToFloat(value1);
    const float val2 = PropertyHelper::stringToFloat(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const UDim result = bas * UDim(mul, mul);

    return PropertyHelper::udimToString(result);
}

/********************************************
* UVector2 interpolator
********************************************/
const String& UVector2Interpolator::getType() const
{
    static String type = "UVector2";
    return type;
}

//----------------------------------------------------------------------------//
String UVector2Interpolator::interpolateAbsolute(const String& value1,
        const String& value2,
        float position)
{
    const UVector2 val1 = PropertyHelper::stringToUVector2(value1);
    const UVector2 val2 = PropertyHelper::stringToUVector2(value2);

    const UVector2 result =
        val1 * UDim(1.0f - position, 1.0f - position) +
        val2 * UDim(position, position);

    return PropertyHelper::uvector2ToString(result);
}

//----------------------------------------------------------------------------//
String UVector2Interpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const UVector2 bas = PropertyHelper::stringToUVector2(base);
    const UVector2 val1 = PropertyHelper::stringToUVector2(value1);
    const UVector2 val2 = PropertyHelper::stringToUVector2(value2);

    const UVector2 result = bas + (
                                val1 * UDim(1.0f - position, 1.0f - position) +
                                val2 * UDim(position, position));

    return PropertyHelper::uvector2ToString(result);
}

//----------------------------------------------------------------------------//
String UVector2Interpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const UVector2 bas = PropertyHelper::stringToUVector2(base);
    const float val1 = PropertyHelper::stringToFloat(value1);
    const float val2 = PropertyHelper::stringToFloat(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const UVector2 result = bas * UDim(mul, mul);

    return PropertyHelper::uvector2ToString(result);
}

/********************************************
* URect interpolator
********************************************/
const String& URectInterpolator::getType() const
{
    static String type = "URect";
    return type;
}

//----------------------------------------------------------------------------//
String URectInterpolator::interpolateAbsolute(const String& value1,
        const String& value2,
        float position)
{
    const URect val1 = PropertyHelper::stringToURect(value1);
    const URect val2 = PropertyHelper::stringToURect(value2);

    const URect result =
        val1 * UDim(1.0f - position, 1.0f - position) +
        val2 * UDim(position, position);

    return PropertyHelper::urectToString(result);
}

//----------------------------------------------------------------------------//
String URectInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const URect bas = PropertyHelper::stringToURect(base);
    const URect val1 = PropertyHelper::stringToURect(value1);
    const URect val2 = PropertyHelper::stringToURect(value2);

    const URect result = bas + (
                             val1 * UDim(1.0f - position, 1.0f - position) +
                             val2 * UDim(position, position));

    return PropertyHelper::urectToString(result);
}

//----------------------------------------------------------------------------//
String URectInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const URect bas = PropertyHelper::stringToURect(base);
    const float val1 = PropertyHelper::stringToFloat(value1);
    const float val2 = PropertyHelper::stringToFloat(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const URect result = bas * UDim(mul, mul);

    return PropertyHelper::urectToString(result);
}

/********************************************
* UBox interpolator
********************************************/
const String& UBoxInterpolator::getType() const
{
    static String type = "UBox";
    return type;
}

//----------------------------------------------------------------------------//
String UBoxInterpolator::interpolateAbsolute(const String& value1,
        const String& value2,
        float position)
{
    const UBox val1 = PropertyHelper::stringToUBox(value1);
    const UBox val2 = PropertyHelper::stringToUBox(value2);

    const UBox result =
        val1 * UDim(1.0f - position, 1.0f - position) +
        val2 * UDim(position, position);

    return PropertyHelper::uboxToString(result);
}

//----------------------------------------------------------------------------//
String UBoxInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const UBox bas = PropertyHelper::stringToUBox(base);
    const UBox val1 = PropertyHelper::stringToUBox(value1);
    const UBox val2 = PropertyHelper::stringToUBox(value2);

    const UBox result = bas + (
                            val1 * UDim(1.0f - position, 1.0f - position) +
                            val2 * UDim(position, position));

    return PropertyHelper::uboxToString(result);
}

//----------------------------------------------------------------------------//
String UBoxInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const UBox bas = PropertyHelper::stringToUBox(base);
    const float val1 = PropertyHelper::stringToFloat(value1);
    const float val2 = PropertyHelper::stringToFloat(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const UBox result = bas * UDim(mul, mul);

    return PropertyHelper::uboxToString(result);
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

