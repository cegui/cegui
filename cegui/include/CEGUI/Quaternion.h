/***********************************************************************
	filename: 	CEGUIQuaternion.h
	created:	2/1/2011
	author:		Martin Preisler
	
	purpose:	Defines interface for the Quaternion class
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
#ifndef _CEGUIQuaternion_h_
#define _CEGUIQuaternion_h_

#include "CEGUI/Base.h"
#include "CEGUI/Interpolator.h"
#include "CEGUI/GLM.h"
#include <cmath>

// Start of CEGUI namespace section
namespace CEGUI
{

inline glm::quat quatFromEulerAnglesRadians(const glm::vec3& euler)
{
    return glm::quat(euler);
}

inline glm::quat quatFromEulerAnglesDegrees(const glm::vec3& euler)
{
    static const float d2r = (4.0f * std::atan2(1.0f, 1.0f)) / 180.0f;
    return quatFromEulerAnglesRadians(glm::vec3(euler.x * d2r, euler.y * d2r, euler.z * d2r));
}

/*!
 \brief Special interpolator class for Quaternion
 
 glm::quats can't be interpolated as floats and/or vectors, we have to use
 "Spherical linear interpolator" instead.
 */
class QuaternionSlerpInterpolator : public Interpolator
{
public:
    typedef PropertyHelper<glm::quat> Helper;
    
    //! destructor
    virtual ~QuaternionSlerpInterpolator() {}

    //! \copydoc Interpolator::getType
    virtual const String& getType() const;
    
    //! \copydoc Interpolator::interpolateAbsolute
    virtual String interpolateAbsolute(const String& value1,
                                       const String& value2,
                                       float position);
    
    //! \copydoc Interpolator::interpolateRelative
    virtual String interpolateRelative(const String& base,
                                       const String& value1,
                                       const String& value2,
                                       float position);
    
    //! \copydoc Interpolator::interpolateRelativeMultiply
    virtual String interpolateRelativeMultiply(const String& base,
                                               const String& value1,
                                               const String& value2,
                                               float position);
};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIQuaternion_h_
