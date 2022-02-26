/***********************************************************************
	created:	17th August 2015
	author:		Lukas Meindl (reworked from code by Martin Preisler who reworked his code from code by Paul D Turner)
	
	purpose:	Interface to the PropertyHelper class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2015 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIPropertyHelper_h_
#define _CEGUIPropertyHelper_h_

#include "CEGUI/String.h"
#include "CEGUI/UVector.h"
#include "CEGUI/URect.h"
#include "CEGUI/text/FontSizeUnit.h"
#include "CEGUI/AspectMode.h"
#include "CEGUI/HorizontalAlignment.h"
#include "CEGUI/VerticalAlignment.h"
#include "CEGUI/text/DefaultParagraphDirection.h"
#include <glm/gtc/quaternion.hpp>

namespace CEGUI
{

#ifndef SWIG
/*!
\brief
	Helper class for the conversion and checks performed on parseable types
*/
namespace ParserHelper
{
    bool IsEmptyOrContainingOnlyDecimalPointOrSign(const CEGUI::String& text);
    bool IsEmptyOrContainingSign(const CEGUI::String& text);
};
#endif

/*!
\brief
	Helper class used to convert various data types to and from the format expected in Property strings

\par
    Usage:

    float value = PropertyHelper<float>::fromString("0.1");
    String value = PropertyHelper<float>::toString(0.1f);
*/
template<typename T>
class PropertyHelper;

// this redirects PropertyHelper<const T> to PropertyHelper<T>
template<typename T>
class PropertyHelper<const T>
{
public:
    typedef typename PropertyHelper<T>::return_type return_type;
    typedef typename PropertyHelper<T>::safe_method_return_type safe_method_return_type;
    typedef typename PropertyHelper<T>::pass_type pass_type;
    typedef typename PropertyHelper<T>::string_return_type string_return_type;

    static const String& getDataTypeName()
    {
        return PropertyHelper<T>::getDataTypeName();
    }

    static return_type fromString(const String& str)
    {
        return PropertyHelper<T>::fromString(str);
    }

    static String toString(pass_type val)
    {
        return PropertyHelper<T>::toString(val);
    }
};


// this redirects PropertyHelper<const T&> to PropertyHelper<T>
template<typename T>
class PropertyHelper<const T&>
{
public:
    typedef typename PropertyHelper<T>::return_type return_type;
    typedef typename PropertyHelper<T>::safe_method_return_type safe_method_return_type;
    typedef typename PropertyHelper<T>::pass_type pass_type;
    typedef typename PropertyHelper<T>::string_return_type string_return_type;

    static const String& getDataTypeName()
    {
        return PropertyHelper<T>::getDataTypeName();
    }

    static return_type fromString(const String& str)
    {
        return PropertyHelper<T>::fromString(str);
    }

    static String toString(pass_type val)
    {
        return PropertyHelper<T>::toString(val);
    }
};

// this redirects PropertyHelper<const T*> to PropertyHelper<T*>
template<typename T>
class PropertyHelper<const T*>
{
public:
    typedef typename PropertyHelper<T*>::return_type return_type;
    typedef typename PropertyHelper<T*>::safe_method_return_type safe_method_return_type;
    typedef typename PropertyHelper<T*>::pass_type pass_type;
    typedef typename PropertyHelper<T*>::string_return_type string_return_type;

    static const String& getDataTypeName()
    {
        return PropertyHelper<T>::getDataTypeName();
    }

    static return_type fromString(const String& str)
    {
        return PropertyHelper<T*>::fromString(str);
    }

    static String toString(pass_type val)
    {
        return PropertyHelper<T*>::toString(val);
    }
};

template<>
class CEGUIEXPORT PropertyHelper<String>
{
public:
    typedef const String& return_type;
    typedef String safe_method_return_type;
    typedef const String& pass_type;
    typedef const String& string_return_type;

    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<float>
{
public:
    typedef float return_type;
    typedef return_type safe_method_return_type;
    typedef const float pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<double>
{
public:
    typedef double return_type;
    typedef return_type safe_method_return_type;
    typedef const double pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<std::int16_t>
{
public:
    typedef std::int16_t return_type;
    typedef return_type safe_method_return_type;
    typedef const std::int16_t pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<std::int32_t>
{
public:
    typedef std::int32_t return_type;
    typedef return_type safe_method_return_type;
    typedef const std::int32_t pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<std::int64_t>
{
public:
    typedef std::int64_t return_type;
    typedef return_type safe_method_return_type;
    typedef const std::int64_t pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<std::uint32_t>
{
public:
    typedef std::uint32_t return_type;
    typedef return_type safe_method_return_type;
    typedef const std::uint32_t pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<std::uint64_t>
{
public:
    typedef std::uint64_t return_type;
    typedef return_type safe_method_return_type;
    typedef const std::uint64_t pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<> 
class CEGUIEXPORT PropertyHelper<bool>
{
public:
    typedef bool return_type;
    typedef return_type safe_method_return_type;
    typedef const bool pass_type;
    typedef const String& string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);

    /*! Definitions of the possible values represented as Strings
        We must not use "True" and "False" here due to conflicts
        with definitions in other libraries using these identifiers.
    */
    static const CEGUI::String ValueTrue;
    static const CEGUI::String ValueFalse;
};



template<> 
class CEGUIEXPORT PropertyHelper<AspectMode>
{
public:
    typedef AspectMode return_type;
    typedef return_type safe_method_return_type;
    typedef AspectMode pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);

    //! Definitions of the possible values represented as Strings
    static const String Shrink;
    static const String Expand;
    static const String AdjustHeight;
    static const String AdjustWidth;
    static const String Ignore;
};

template<>
class CEGUIEXPORT PropertyHelper<glm::vec2>
{
public:
    typedef glm::vec2 return_type;
    typedef return_type safe_method_return_type;
    typedef const glm::vec2& pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<glm::vec3>
{
public:
    typedef glm::vec3 return_type;
    typedef return_type safe_method_return_type;
    typedef const glm::vec3& pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<glm::quat>
{
public:
    typedef glm::quat return_type;
    typedef return_type safe_method_return_type;
    typedef const glm::quat& pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<Image*>
{
public:
    typedef const Image* return_type;
    typedef return_type safe_method_return_type;
    typedef const Image* const pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<Colour>
{
public:
    typedef Colour return_type;
    typedef return_type safe_method_return_type;
    typedef const Colour& pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<ColourRect>
{
public:
    typedef ColourRect return_type;
    typedef return_type safe_method_return_type;
    typedef const ColourRect& pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<UDim>
{
public:
    typedef UDim return_type;
    typedef return_type safe_method_return_type;
    typedef const UDim& pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<UVector2>
{
public:
    typedef UVector2 return_type;
    typedef return_type safe_method_return_type;
    typedef const UVector2& pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<UVector3>
{
public:
    typedef UVector3 return_type;
    typedef return_type safe_method_return_type;
    typedef const UVector3& pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<USize>
{
public:
    typedef USize return_type;
    typedef return_type safe_method_return_type;
    typedef const USize& pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<URect>
{
public:
    typedef URect return_type;
    typedef return_type safe_method_return_type;
    typedef const URect& pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<Sizef>
{
public:
    typedef Sizef return_type;
    typedef return_type safe_method_return_type;
    typedef const Sizef& pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<Rectf>
{
public:
    typedef Rectf return_type;
    typedef return_type safe_method_return_type;
    typedef const Rectf& pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};


template<>
class CEGUIEXPORT PropertyHelper<UBox>
{
public:
    typedef UBox return_type;
    typedef return_type safe_method_return_type;
    typedef const UBox& pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<FontSizeUnit>
{
public:
    typedef FontSizeUnit return_type;
    typedef return_type safe_method_return_type;
    typedef const FontSizeUnit pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);

    static const String Points;
    static const String Pixels;
};


template<>
class CEGUIEXPORT PropertyHelper<Font*>
{
public:
    typedef Font* return_type;
    typedef return_type safe_method_return_type;
    typedef Font* const pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};


template<>
class CEGUIEXPORT PropertyHelper<HorizontalAlignment>
{
public:
    typedef HorizontalAlignment return_type;
    typedef return_type safe_method_return_type;
    typedef HorizontalAlignment pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};


template<>
class CEGUIEXPORT PropertyHelper<VerticalAlignment>
{
public:
    typedef VerticalAlignment return_type;
    typedef return_type safe_method_return_type;
    typedef VerticalAlignment pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

// Explicit instantiation declarations
extern template CEGUIEXPORT class PropertyHelper<String>;
extern template CEGUIEXPORT class PropertyHelper<float>;
extern template CEGUIEXPORT class PropertyHelper<double>;
extern template CEGUIEXPORT class PropertyHelper<std::int16_t>;
extern template CEGUIEXPORT class PropertyHelper<std::int32_t>;
extern template CEGUIEXPORT class PropertyHelper<std::int64_t>;
extern template CEGUIEXPORT class PropertyHelper<std::uint32_t>;
extern template CEGUIEXPORT class PropertyHelper<std::uint64_t>;
extern template CEGUIEXPORT class PropertyHelper<bool>;
extern template CEGUIEXPORT class PropertyHelper<AspectMode>;
extern template CEGUIEXPORT class PropertyHelper<glm::vec2>;
extern template CEGUIEXPORT class PropertyHelper<glm::vec3>;
extern template CEGUIEXPORT class PropertyHelper<glm::quat>;
extern template CEGUIEXPORT class PropertyHelper<Image*>;
extern template CEGUIEXPORT class PropertyHelper<Colour>;
extern template CEGUIEXPORT class PropertyHelper<ColourRect>;
extern template CEGUIEXPORT class PropertyHelper<UDim>;
extern template CEGUIEXPORT class PropertyHelper<UVector2>;
extern template CEGUIEXPORT class PropertyHelper<UVector3>;
extern template CEGUIEXPORT class PropertyHelper<Rectf>;
extern template CEGUIEXPORT class PropertyHelper<Sizef>;
extern template CEGUIEXPORT class PropertyHelper<URect>;
extern template CEGUIEXPORT class PropertyHelper<USize>;
extern template CEGUIEXPORT class PropertyHelper<UBox>;
extern template CEGUIEXPORT class PropertyHelper<Font*>;
extern template CEGUIEXPORT class PropertyHelper<HorizontalAlignment>;
extern template CEGUIEXPORT class PropertyHelper<VerticalAlignment>;


}

#endif
