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
    //const float val1 = PropertyHelper<float>::fromString(value1);
    //const float val2 = PropertyHelper<float>::fromString(value2);

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
    const float val1 = PropertyHelper<float>::fromString(value1);
    const float val2 = PropertyHelper<float>::fromString(value2);

    const float result = val1 * (1.0f - position) + val2 * (position);

    return PropertyHelper<float>::toString(result);
}

//----------------------------------------------------------------------------//
String FloatInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const float bas = PropertyHelper<float>::fromString(base);
    const float val1 = PropertyHelper<float>::fromString(value1);
    const float val2 = PropertyHelper<float>::fromString(value2);

    const float result = bas + (val1 * (1.0f - position) + val2 * (position));

    return PropertyHelper<float>::toString(result);
}

//----------------------------------------------------------------------------//
String FloatInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const float bas = PropertyHelper<float>::fromString(base);
    const float val1 = PropertyHelper<float>::fromString(value1);
    const float val2 = PropertyHelper<float>::fromString(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const float result = bas * mul;

    return PropertyHelper<float>::toString(result);
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
    const uint val1 = PropertyHelper<uint>::fromString(value1);
    const uint val2 = PropertyHelper<uint>::fromString(value2);

    const uint result = static_cast<uint>(
                            static_cast<float>(val1) * (1.0f - position) +
                            static_cast<float>(val2) * (position));

    return PropertyHelper<uint>::toString(result);
}

//----------------------------------------------------------------------------//
String UintInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const uint bas = PropertyHelper<uint>::fromString(base);
    const uint val1 = PropertyHelper<uint>::fromString(value1);
    const uint val2 = PropertyHelper<uint>::fromString(value2);

    const uint result = bas + static_cast<uint>(
                            static_cast<float>(val1) * (1.0f - position) +
                            static_cast<float>(val2) * (position));

    return PropertyHelper<uint>::toString(result);
}

//----------------------------------------------------------------------------//
String UintInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const uint bas = PropertyHelper<uint>::fromString(base);
    const float val1 = PropertyHelper<float>::fromString(value1);
    const float val2 = PropertyHelper<float>::fromString(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const uint result = static_cast<uint>(bas * mul);

    return PropertyHelper<uint>::toString(result);
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
    const int val1 = PropertyHelper<int>::fromString(value1);
    const int val2 = PropertyHelper<int>::fromString(value2);

    const int result = static_cast<int>(
                           static_cast<float>(val1) * (1.0f - position) +
                           static_cast<float>(val2) * (position));

    return PropertyHelper<int>::toString(result);
}

//----------------------------------------------------------------------------//
String IntInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const int bas = PropertyHelper<int>::fromString(base);
    const int val1 = PropertyHelper<int>::fromString(value1);
    const int val2 = PropertyHelper<int>::fromString(value2);

    const int result = bas + static_cast<int>(
                           static_cast<float>(val1) * (1.0f - position) +
                           static_cast<float>(val2) * (position));

    return PropertyHelper<int>::toString(result);
}

//----------------------------------------------------------------------------//
String IntInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const int bas = PropertyHelper<int>::fromString(base);
    const float val1 = PropertyHelper<float>::fromString(value1);
    const float val2 = PropertyHelper<float>::fromString(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const int result = static_cast<int>(bas * mul);

    return PropertyHelper<int>::toString(result);
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
    const bool val1 = PropertyHelper<bool>::fromString(value1);
    const bool val2 = PropertyHelper<bool>::fromString(value2);

    return PropertyHelper<bool>::toString(position < 0.5 ? val1 : val2);
}

//----------------------------------------------------------------------------//
String BoolInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    //const bool bas = PropertyHelper<bool>::fromString(base);
    const bool val1 = PropertyHelper<bool>::fromString(value1);
    const bool val2 = PropertyHelper<bool>::fromString(value2);

    return PropertyHelper<bool>::toString(position < 0.5 ? val1 : val2);
}

//----------------------------------------------------------------------------//
String BoolInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    //const bool bas = PropertyHelper<bool>::fromString(base);
    //const float val1 = PropertyHelper<float>::fromString(value1);
    //const float val2 = PropertyHelper<float>::fromString(value2);

    //const float mul = val1 * (1.0f - position) + val2 * (position);

    //const bool result = static_cast<bool>(bas * mul);

    //return PropertyHelper<bool>::toString(result);
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
    const Size val1 = PropertyHelper<Size>::fromString(value1);
    const Size val2 = PropertyHelper<Size>::fromString(value2);

    const Size result = val1 * (1.0f - position) + val2 * (position);

    return PropertyHelper<Size>::toString(result);
}

//----------------------------------------------------------------------------//
String SizeInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const Size bas = PropertyHelper<Size>::fromString(base);
    const Size val1 = PropertyHelper<Size>::fromString(value1);
    const Size val2 = PropertyHelper<Size>::fromString(value2);

    const Size result = bas + (val1 * (1.0f - position) + val2 * (position));

    return PropertyHelper<Size>::toString(result);
}

//----------------------------------------------------------------------------//
String SizeInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const Size bas = PropertyHelper<Size>::fromString(base);
    const float val1 = PropertyHelper<float>::fromString(value1);
    const float val2 = PropertyHelper<float>::fromString(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const Size result = bas * mul;

    return PropertyHelper<Size>::toString(result);
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
    const Point val1 = PropertyHelper<Vector2>::fromString(value1);
    const Point val2 = PropertyHelper<Vector2>::fromString(value2);

    const Point result = val1 * (1.0f - position) + val2 * (position);

    return PropertyHelper<Vector2>::toString(result);
}

//----------------------------------------------------------------------------//
String PointInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const Point bas = PropertyHelper<Vector2>::fromString(base);
    const Point val1 = PropertyHelper<Vector2>::fromString(value1);
    const Point val2 = PropertyHelper<Vector2>::fromString(value2);

    const Point result = bas + (val1 * (1.0f - position) + val2 * (position));

    return PropertyHelper<Vector2>::toString(result);
}

//----------------------------------------------------------------------------//
String PointInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const Point bas = PropertyHelper<Vector2>::fromString(base);
    const float val1 = PropertyHelper<float>::fromString(value1);
    const float val2 = PropertyHelper<float>::fromString(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const Point result = bas * mul;

    return PropertyHelper<Vector2>::toString(result);
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
    const Vector3 val1 = PropertyHelper<Vector3>::fromString(value1);
    const Vector3 val2 = PropertyHelper<Vector3>::fromString(value2);

    const Vector3 result = val1 * (1.0f - position) + val2 * (position);

    return PropertyHelper<Vector3>::toString(result);
}

//----------------------------------------------------------------------------//
String Vector3Interpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const Vector3 bas = PropertyHelper<Vector3>::fromString(base);
    const Vector3 val1 = PropertyHelper<Vector3>::fromString(value1);
    const Vector3 val2 = PropertyHelper<Vector3>::fromString(value2);

    const Vector3 result = bas + (val1 * (1.0f - position) + val2 * (position));

    return PropertyHelper<Vector3>::toString(result);
}

//----------------------------------------------------------------------------//
String Vector3Interpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const Vector3 bas = PropertyHelper<Vector3>::fromString(base);
    const float val1 = PropertyHelper<float>::fromString(value1);
    const float val2 = PropertyHelper<float>::fromString(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const Vector3 result = bas * mul;

    return PropertyHelper<Vector3>::toString(result);
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
    const Rect val1 = PropertyHelper<Rect>::fromString(value1);
    const Rect val2 = PropertyHelper<Rect>::fromString(value2);

    const Rect result = val1 * (1.0f - position) + val2 * (position);

    return PropertyHelper<Rect>::toString(result);
}

//----------------------------------------------------------------------------//
String RectInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const Rect bas = PropertyHelper<Rect>::fromString(base);
    const Rect val1 = PropertyHelper<Rect>::fromString(value1);
    const Rect val2 = PropertyHelper<Rect>::fromString(value2);

    const Rect result = bas + (val1 * (1.0f - position) + val2 * (position));

    return PropertyHelper<Rect>::toString(result);
}

//----------------------------------------------------------------------------//
String RectInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const Rect bas = PropertyHelper<Rect>::fromString(base);
    const float val1 = PropertyHelper<float>::fromString(value1);
    const float val2 = PropertyHelper<float>::fromString(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const Rect result = bas * mul;

    return PropertyHelper<Rect>::toString(result);
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
    const Colour val1 = PropertyHelper<Colour>::fromString(value1);
    const Colour val2 = PropertyHelper<Colour>::fromString(value2);

    const Colour result = val1 * (1.0f - position) + val2 * (position);

    return PropertyHelper<Colour>::toString(result);
}

//----------------------------------------------------------------------------//
String ColourInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const Colour bas = PropertyHelper<Colour>::fromString(base);
    const Colour val1 = PropertyHelper<Colour>::fromString(value1);
    const Colour val2 = PropertyHelper<Colour>::fromString(value2);

    const Colour result = bas + (val1 * (1.0f - position) + val2 * (position));

    return PropertyHelper<Colour>::toString(result);
}

//----------------------------------------------------------------------------//
String ColourInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const Colour bas = PropertyHelper<Colour>::fromString(base);
    const float val1 = PropertyHelper<float>::fromString(value1);
    const float val2 = PropertyHelper<float>::fromString(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const Colour result = bas * mul;

    return PropertyHelper<Colour>::toString(result);
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
    const ColourRect val1 = PropertyHelper<ColourRect>::fromString(value1);
    const ColourRect val2 = PropertyHelper<ColourRect>::fromString(value2);

    const ColourRect result = val1 * (1.0f - position) + val2 * (position);

    return PropertyHelper<ColourRect>::toString(result);
}

//----------------------------------------------------------------------------//
String ColourRectInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const ColourRect bas = PropertyHelper<ColourRect>::fromString(base);
    const ColourRect val1 = PropertyHelper<ColourRect>::fromString(value1);
    const ColourRect val2 = PropertyHelper<ColourRect>::fromString(value2);

    const ColourRect result = bas + (val1 * (1.0f - position) + val2 * (position));

    return PropertyHelper<ColourRect>::toString(result);
}

//----------------------------------------------------------------------------//
String ColourRectInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const ColourRect bas = PropertyHelper<ColourRect>::fromString(base);
    const float val1 = PropertyHelper<float>::fromString(value1);
    const float val2 = PropertyHelper<float>::fromString(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const ColourRect result = bas * mul;

    return PropertyHelper<ColourRect>::toString(result);
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
    const UDim val1 = PropertyHelper<UDim>::fromString(value1);
    const UDim val2 = PropertyHelper<UDim>::fromString(value2);

    const UDim result =
        val1 * UDim(1.0f - position, 1.0f - position) +
        val2 * UDim(position, position);

    return PropertyHelper<UDim>::toString(result);
}

//----------------------------------------------------------------------------//
String UDimInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const UDim bas = PropertyHelper<UDim>::fromString(base);
    const UDim val1 = PropertyHelper<UDim>::fromString(value1);
    const UDim val2 = PropertyHelper<UDim>::fromString(value2);

    const UDim result = bas + (
                            val1 * UDim(1.0f - position, 1.0f - position) +
                            val2 * UDim(position, position));

    return PropertyHelper<UDim>::toString(result);
}

//----------------------------------------------------------------------------//
String UDimInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const UDim bas = PropertyHelper<UDim>::fromString(base);
    const float val1 = PropertyHelper<float>::fromString(value1);
    const float val2 = PropertyHelper<float>::fromString(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const UDim result = bas * UDim(mul, mul);

    return PropertyHelper<UDim>::toString(result);
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
    const UVector2 val1 = PropertyHelper<UVector2>::fromString(value1);
    const UVector2 val2 = PropertyHelper<UVector2>::fromString(value2);

    const UVector2 result =
        val1 * UDim(1.0f - position, 1.0f - position) +
        val2 * UDim(position, position);

    return PropertyHelper<UVector2>::toString(result);
}

//----------------------------------------------------------------------------//
String UVector2Interpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const UVector2 bas = PropertyHelper<UVector2>::fromString(base);
    const UVector2 val1 = PropertyHelper<UVector2>::fromString(value1);
    const UVector2 val2 = PropertyHelper<UVector2>::fromString(value2);

    const UVector2 result = bas + (
                                val1 * UDim(1.0f - position, 1.0f - position) +
                                val2 * UDim(position, position));

    return PropertyHelper<UVector2>::toString(result);
}

//----------------------------------------------------------------------------//
String UVector2Interpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const UVector2 bas = PropertyHelper<UVector2>::fromString(base);
    const float val1 = PropertyHelper<float>::fromString(value1);
    const float val2 = PropertyHelper<float>::fromString(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const UVector2 result = bas * UDim(mul, mul);

    return PropertyHelper<UVector2>::toString(result);
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
    const URect val1 = PropertyHelper<URect>::fromString(value1);
    const URect val2 = PropertyHelper<URect>::fromString(value2);

    const URect result =
        val1 * UDim(1.0f - position, 1.0f - position) +
        val2 * UDim(position, position);

    return PropertyHelper<URect>::toString(result);
}

//----------------------------------------------------------------------------//
String URectInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const URect bas = PropertyHelper<URect>::fromString(base);
    const URect val1 = PropertyHelper<URect>::fromString(value1);
    const URect val2 = PropertyHelper<URect>::fromString(value2);

    const URect result = bas + (
                             val1 * UDim(1.0f - position, 1.0f - position) +
                             val2 * UDim(position, position));

    return PropertyHelper<URect>::toString(result);
}

//----------------------------------------------------------------------------//
String URectInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const URect bas = PropertyHelper<URect>::fromString(base);
    const float val1 = PropertyHelper<float>::fromString(value1);
    const float val2 = PropertyHelper<float>::fromString(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const URect result = bas * UDim(mul, mul);

    return PropertyHelper<URect>::toString(result);
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
    const UBox val1 = PropertyHelper<UBox>::fromString(value1);
    const UBox val2 = PropertyHelper<UBox>::fromString(value2);

    const UBox result =
        val1 * UDim(1.0f - position, 1.0f - position) +
        val2 * UDim(position, position);

    return PropertyHelper<UBox>::toString(result);
}

//----------------------------------------------------------------------------//
String UBoxInterpolator::interpolateRelative(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const UBox bas = PropertyHelper<UBox>::fromString(base);
    const UBox val1 = PropertyHelper<UBox>::fromString(value1);
    const UBox val2 = PropertyHelper<UBox>::fromString(value2);

    const UBox result = bas + (
                            val1 * UDim(1.0f - position, 1.0f - position) +
                            val2 * UDim(position, position));

    return PropertyHelper<UBox>::toString(result);
}

//----------------------------------------------------------------------------//
String UBoxInterpolator::interpolateRelativeMultiply(const String& base,
        const String& value1,
        const String& value2,
        float position)
{
    const UBox bas = PropertyHelper<UBox>::fromString(base);
    const float val1 = PropertyHelper<float>::fromString(value1);
    const float val2 = PropertyHelper<float>::fromString(value2);

    const float mul = val1 * (1.0f - position) + val2 * (position);

    const UBox result = bas * UDim(mul, mul);

    return PropertyHelper<UBox>::toString(result);
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

