/***********************************************************************
    filename:   CEGUIBasicInterpolators.h
    created:    7/8/2010
    author:     Martin Preisler

    purpose:    Defines basic integrated interpolators
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
#ifndef _CEGUIBasicInterpolators_h_
#define _CEGUIBasicInterpolators_h_

#include "CEGUIInterpolator.h"

// Start of CEGUI namespace section
namespace CEGUI
{

// this does no interpolation! it just chooses left or right value based on
// position!
class CEGUIEXPORT StringInterpolator : public Interpolator
{
public:
    virtual ~StringInterpolator(void) {};

    virtual const String& getType() const;

    virtual String interpolateAbsolute(const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelative(const String& base,
                                       const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelativeMultiply(const String& base,
            const String& value1,
            const String& value2,
            float position);
};

class CEGUIEXPORT FloatInterpolator : public Interpolator
{
public:
    virtual ~FloatInterpolator(void) {};

    virtual const String& getType() const;

    virtual String interpolateAbsolute(const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelative(const String& base,
                                       const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelativeMultiply(const String& base,
            const String& value1,
            const String& value2,
            float position);
};

class CEGUIEXPORT UintInterpolator : public Interpolator
{
public:
    virtual ~UintInterpolator(void) {};

    virtual const String& getType() const;

    virtual String interpolateAbsolute(const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelative(const String& base,
                                       const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelativeMultiply(const String& base,
            const String& value1,
            const String& value2,
            float position);
};

class CEGUIEXPORT IntInterpolator : public Interpolator
{
public:
    virtual ~IntInterpolator(void) {};

    virtual const String& getType() const;

    virtual String interpolateAbsolute(const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelative(const String& base,
                                       const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelativeMultiply(const String& base,
            const String& value1,
            const String& value2,
            float position);
};

// this does no interpolation! it just chooses left or right value based on
// position!
class CEGUIEXPORT BoolInterpolator : public Interpolator
{
public:
    virtual ~BoolInterpolator(void) {};

    virtual const String& getType() const;

    virtual String interpolateAbsolute(const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelative(const String& base,
                                       const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelativeMultiply(const String& base,
            const String& value1,
            const String& value2,
            float position);
};

class CEGUIEXPORT SizeInterpolator : public Interpolator
{
public:
    virtual ~SizeInterpolator(void) {};

    virtual const String& getType() const;

    virtual String interpolateAbsolute(const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelative(const String& base,
                                       const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelativeMultiply(const String& base,
            const String& value1,
            const String& value2,
            float position);
};

class CEGUIEXPORT PointInterpolator : public Interpolator
{
public:
    virtual ~PointInterpolator(void) {};

    virtual const String& getType() const;

    virtual String interpolateAbsolute(const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelative(const String& base,
                                       const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelativeMultiply(const String& base,
            const String& value1,
            const String& value2,
            float position);
};

class CEGUIEXPORT Vector3Interpolator : public Interpolator
{
public:
    virtual ~Vector3Interpolator(void) {};

    virtual const String& getType() const;

    virtual String interpolateAbsolute(const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelative(const String& base,
                                       const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelativeMultiply(const String& base,
            const String& value1,
            const String& value2,
            float position);
};

class CEGUIEXPORT RectInterpolator : public Interpolator
{
public:
    virtual ~RectInterpolator(void) {};

    virtual const String& getType() const;

    virtual String interpolateAbsolute(const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelative(const String& base,
                                       const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelativeMultiply(const String& base,
            const String& value1,
            const String& value2,
            float position);
};

class CEGUIEXPORT ColourInterpolator : public Interpolator
{
public:
    virtual ~ColourInterpolator(void) {};

    virtual const String& getType() const;

    virtual String interpolateAbsolute(const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelative(const String& base,
                                       const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelativeMultiply(const String& base,
            const String& value1,
            const String& value2,
            float position);
};

class CEGUIEXPORT ColourRectInterpolator : public Interpolator
{
public:
    virtual ~ColourRectInterpolator(void) {};

    virtual const String& getType() const;

    virtual String interpolateAbsolute(const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelative(const String& base,
                                       const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelativeMultiply(const String& base,
            const String& value1,
            const String& value2,
            float position);
};

class CEGUIEXPORT UDimInterpolator : public Interpolator
{
public:
    virtual ~UDimInterpolator(void) {};

    virtual const String& getType() const;

    virtual String interpolateAbsolute(const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelative(const String& base,
                                       const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelativeMultiply(const String& base,
            const String& value1,
            const String& value2,
            float position);
};

class CEGUIEXPORT UVector2Interpolator : public Interpolator
{
public:
    virtual ~UVector2Interpolator(void) {};

    virtual const String& getType() const;

    virtual String interpolateAbsolute(const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelative(const String& base,
                                       const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelativeMultiply(const String& base,
            const String& value1,
            const String& value2,
            float position);
};

class CEGUIEXPORT URectInterpolator : public Interpolator
{
public:
    virtual ~URectInterpolator(void) {};

    virtual const String& getType() const;

    virtual String interpolateAbsolute(const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelative(const String& base,
                                       const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelativeMultiply(const String& base,
            const String& value1,
            const String& value2,
            float position);
};

class CEGUIEXPORT UBoxInterpolator : public Interpolator
{
public:
    virtual ~UBoxInterpolator(void) {};

    virtual const String& getType() const;

    virtual String interpolateAbsolute(const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelative(const String& base,
                                       const String& value1,
                                       const String& value2,
                                       float position);
    virtual String interpolateRelativeMultiply(const String& base,
            const String& value1,
            const String& value2,
            float position);
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIBasicInterpolators_h_

