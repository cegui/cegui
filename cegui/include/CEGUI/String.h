/***********************************************************************
    created:    26/2/2004, reworked on 21/9/2015
    author:     Lukas Meindl (completely reworked version), Paul D Turner
                (original version)

    purpose:    Defines the String class be used within the GUI system.
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
#ifndef _String_h_
#define _String_h_

#include "CEGUI/Base.h"
#include <vector>
#include <stdexcept>

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8) || (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)

//Set a definition to determine if the compiler fully supports C++11's
//basic_string definition
#if defined (_MSC_VER)
#define CEGUI_STRING_CPP_11
#endif

namespace CEGUI
{

#if defined (_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
/*!
\brief
    UTF-8 based String class to be used inside (and outside)
    the CEGUI library.

    This class wraps around std::string and can be used identically.
    In doing so it provides conversion functionality for regular char
    arrays (interpreted as UTF-8 / ASCII) and accepts such char arrays
    and strings in the constructor and in the assign, insert and
    other functions. It also accepts UTF-32 encoded strings and characters
    and converts them to the internal UTF-8 representation for storage.
\note The term "character" in the context of positions and counts/numbers, if
    not explicitly specified otherwise, refers to the semantic meaning of a
    character (typically a code point) and not to a code unit.
\note When using functions accepting char32_t, char32_t* or std::u32string as
    parameter types, a conversion from UTF-32 to UTF-8 will be performed inside
    the function. This has implications regarding the performance of the
    functions, in specific such an overload will always work slowlier than the
    one using the native type (char, char* or std::string).
\note The class was specifically created to provide the methods of an 
    std::string (or std::basic_string to be specific) while providing support
    for passing char, char32_t, char*, char32_t*, std::string, std::u32string
    and String to all methods, operators and constructors via overloads, whenever
    it is meaningful and possible. 
\note A few function overloads that take a char* array and depend on a count
    are not supported, since they would be unsafe (if taking the count as count
    for the code units) or misleading (if  taking the count for code points).
    Such functions are, for example, variations of find, assign and replace.
\note Inheritation of std::basic_string cannot be done safely due to the
    non-virtual constructor, therefore the wrapping was chosen.
*/
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
/*!
\brief
    UTF-32 based String class to be used inside (and outside) the CEGUI library.

    This class wraps around std::u32string and can be used identically. In addition to that it
    provides conversion functionality for regular char arrays (interpreted as UTF-8 / ASCII) and accepts
    such char arrays and strings in the constructor and in the assign, insert and other functions.
\note The term "character" in the context of positions and counts/numbers, if not explicitly specified otherwise,
    refers to code points and not code units, which is of relevance in the context of std::string and char*,
    as they are may represent UTF-8 code units (if not ASCII characters).
\note The class was specifically created to provide the methods of an std::string (or std::basic_string to be specific)
    while providing support for passing char, char32_t, char*, char32_t*, std::string, std::u32string and String to all
    methods, operators and constructors via overloads, whenever it is meaningful and possible. 
\note A few function overloads that take a char* array and depend on a count are not supported,
    since they would be unsafe (if taking the count as count for the code units) or misleading (if
    taking the count for code points). Such functions are, for example, variations of find,
    assign and replace.
\note Inheritation of std::basic_string cannot be done safely due to the non-virtual constructor, 
    therefore the wrapping was chosen.
*/
#endif
class CEGUIEXPORT String
{
public:
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*************************************************************************
        Integral Types
    *************************************************************************/
    //! Basic 'code unit' type used for the UTF-8 encoded String
    typedef char                                    value_type;
    //! Unsigned type used for size values and indices
    typedef size_t                                  size_type;
    //! Signed type used for differences
    typedef std::ptrdiff_t                          difference_type;
    //! Type used for references to UTF-8 code units
    typedef char&                                   reference;
    //! Type used for constant references to UTF-8 code units
    typedef const char&                             const_reference;
    //! Type used for UTF-8 code units pointers 
    typedef char*                                   pointer;
    //! Type used for constant UTF-8 code units pointers
    typedef const char*                             const_pointer; 
    //! Type used for iterators pointing to an UTF-8 code unit of the String
    typedef std::basic_string<char>::iterator                iterator;         
    //! Type used for const iterators pointing to an UTF-8 code unit of the String
    typedef std::basic_string<char>::const_iterator          const_iterator;
    //! Type used for reverse iterators pointing to an UTF-8 code unit of the String
    typedef std::basic_string<char>::reverse_iterator        reverse_iterator;     
    //! Type used for const reverse iterators pointing to an UTF-8 code unit of the String
    typedef std::basic_string<char>::const_reverse_iterator  const_reverse_iterator; 
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*************************************************************************
        Integral Types
    *************************************************************************/
    //! Basic 'code point' type used for the UTF-32 encoded String
    typedef char32_t                                value_type;
    //! Unsigned type used for size values and indices
    typedef size_t                                  size_type;
    //! Signed type used for differences
    typedef std::ptrdiff_t                          difference_type;
    //! Type used for references to UTF-32 code points
    typedef char32_t&                               reference;
    //! Type used for constant references to UTF-32 code points
    typedef const char32_t&                         const_reference;
    //! Type used for UTF-32 code point pointers 
    typedef char32_t*                               pointer;
    //! Type used for constant UTF-32 code point pointers
    typedef const char32_t*                         const_pointer; 
    //! Type used for iterators pointing to an UTF-32 code point of the String
    typedef std::basic_string<char32_t>::iterator                iterator;         
    //! Type used for const iterators pointing to an UTF-32 code point of the String
    typedef std::basic_string<char32_t>::const_iterator          const_iterator;      
    //! Type used for reverse iterators pointing to an UTF-32 code point of the String
    typedef std::basic_string<char32_t>::reverse_iterator        reverse_iterator;     
    //! Type used for const reverse iterators pointing to an UTF-32 code point of the String
    typedef std::basic_string<char32_t>::const_reverse_iterator  const_reverse_iterator; 
#endif

    //////////////////////////////////////////////////////////////////////////
    // Conversion helper functions/interface
    //////////////////////////////////////////////////////////////////////////

    /*
    \brief
        Converts an UTF-8 encoded or ASCII char array to an UTF-32 encoded string (std::u32string). The array has to be null-terminated.
    \param utf8String
        An UTF-8 encoded or ASCII char array, which is null-terminated.
    \return
        Returns an UTF-32 string (std::u32string) converted from the ASCII or UTF-8 encoded string or char array.
    */
    static inline std::u32string convertUtf8ToUtf32(const char* utf8String, std::vector<size_t>* mapping = nullptr)
    {
        if (!utf8String)
            return std::u32string();

        return convertUtf8ToUtf32(utf8String, std::char_traits<char>::length(utf8String), mapping);
    }

    /*
    \brief
        Converts an UTF-8 encoded or ASCII string or char
        array to an UTF-32 encoded string (std::u32string).
    \param utf8StringStart
        A pointer to the beginning of the UTF-8 encoded or
        ASCII char array to be converted.
    \param utf8StringEnd
        A pointer to the end of the UTF-8 or ASCII char
        array to be converted.
    \return
        Returns an UTF-32 string (std::u32string) converted
        from the ASCII or UTF-8 encoded string or char array.
    */
    static inline std::u32string convertUtf8ToUtf32(const char* utf8StringStart, const char* utf8StringEnd, std::vector<size_t>* mapping = nullptr)
    {
        if (!utf8StringStart)
            return std::u32string();

        return convertUtf8ToUtf32(utf8StringStart, utf8StringEnd - utf8StringStart, mapping);
    }

    /*
    \brief
        Converts an UTF-8 encoded or ASCII string or char array
        to an UTF-32 encoded string (std::u32string).
    \param utf8String
        An UTF-8 encoded or ASCII string. The char array will
        be converted only until the first null-character is encountered.
    \return
        Returns an UTF-32 string (std::u32string) converted from
        the ASCII or UTF-8 encoded string or char array.
    */
    static inline std::u32string convertUtf8ToUtf32(const std::string& utf8String, std::vector<size_t>* mapping = nullptr)
    {
        return convertUtf8ToUtf32(utf8String.data(), utf8String.size(), mapping);
    }

    /*
    \brief
        Converts an UTF-8 code unit to an UTF-32 encoded string (std::u32string).
    \param utf8Char
        A UTF-8 code unit.
    \return
        Returns an UTF-32 string (std::u32string) converted from the UTF-8 code unit.
    */
    static inline std::u32string convertUtf8ToUtf32(const char utf8Char)
    {
        return convertUtf8ToUtf32(&utf8Char, 1);
    }

    /*
    \brief
        Converts an UTF-8 encoded or ASCII har array to an
        UTF-32 encoded string (std::u32string).
    \param utf8String
        A pointer to the beginning of the UTF-8 encoded or
        ASCII char array to be converted.
    \param stringLength
        The amount of chars (UTF-8 code units) contained

        in this array that shall be converted.
    \return
        Returns an UTF-32 string (std::u32string)
        converted from the ASCII or UTF-8 encoded char array.
    */
    static std::u32string convertUtf8ToUtf32(const char* utf8String, const size_t stringLength, std::vector<size_t>* mapping = nullptr);

    /*
    \brief
        Converts an UTF-32 encoded char32_t array to an
        UTF-8 string (std::string). The array has to be
        null-terminated.
    \param utf32String
        A UTF-32 encoded char32_t array, which is
        null-terminated.
    \return
        Returns an UTF-8 string (std::string) converted
        from the UTF-32 encoded char32_t array.
    */
    static std::string convertUtf32ToUtf8(const char32_t* utf32String);

    /*
    \brief
        Converts an UTF-32 string or char32_t array to
        an UTF-8 string (std::string).
    \param utf32StringStart
        A pointer to the beginning of the UTF-32 encoded 
        char32_t array to be converted.
    \param utf32StringEnd
        A pointer to the end of the UTF-32 encoded
        char32_t array to be converted.
    \return
        Returns an UTF-8 string (std::string) converted
        from the UTF-32 string or char32_t array.
    */
    static std::string convertUtf32ToUtf8(const char32_t* utf32StringStart, const char32_t* utf32StringEnd);

    /*
    \brief
        Converts an UTF-32 string or char32_t array to
        an UTF-8 string (std::string).
        \param utf32String
        A UTF-32 string or char32_t array. The char32_t
        array will be converted only until the first
        null-character is encountered.
    \return
        Returns an UTF-8 string (std::string) converted
        from the UTF-32 string or char32_t array.
    */
    static std::string convertUtf32ToUtf8(const std::u32string& utf32String);

    /*
    \brief
        Converts an UTF-32 code unit to a UTF-8 string
        (std::string).
    \param utf32Char
        A UTF-32 code unit.
    \return
        Returns an UTF-8 string (std::string)
        converted from the UTF-32 code unit.
    */
    static std::string convertUtf32ToUtf8(const char32_t utf32Char);

    /*
    \brief
        Converts an UTF-32 string or char32_t array
        to an UTF-8 string (std::string).
    \param utf32String
        A pointer to the beginning of the UTF-32
        encoded char32_t array to be converted.
    \param stringLength
        The length of the char32_t array of UTF-32
        code units to be converted.
    \return
        Returns an UTF-8 string (std::string) converted
        from the UTF-32 string or char32_t array.
    */
    static std::string convertUtf32ToUtf8(const char32_t* utf32String, const size_t stringLength);

    static char32_t getCodePointFromCodeUnits(const char* firstCodeUnit,
                                              const size_t remainingCodeUnits,
                                              size_t& consumedCodeUnits);

    static char32_t getCodePointFromCodeUnits(const char* firstCodeUnit,
                                              const size_t remainingCodeUnits);


    static char32_t getCodePointFromCodeUnits(std::string::const_iterator currentCodeUnit,
                                              std::string::const_iterator codeUnitRangeEnd,
                                              size_t& consumedCodeUnits);

    static char32_t getCodePointFromCodeUnits(std::string::const_iterator currentCodeUnit,
                                              std::string::const_iterator codeUnitRangeEnd);

    static void checkUtf8CodePointSizeForValidity(
        const size_t codeUnitsInCodePoint,
        const size_t remainingCodeUnits);

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    bool isUtf8StringValid() const;
#endif

    /*
    \brief
        Determines the code point size of an UTF-8 encoded code
        point according to the supplied initial UTF-8 code unit.
    \param initialCodeUnit
        The initial code unit to be used for determining the
        UTF-32 code point.
    \return
        Returns the prospective size of the code point, or 0
        if the supplied initial byte is of an invalid code point.
    */
    static size_t getCodePointSize(const char initialCodeUnit)
    {
        if ((initialCodeUnit & 0x80) == 0x00)
            return 1;
        else if ((initialCodeUnit & 0xE0) == 0xC0)
            return 2;
        else if ((initialCodeUnit & 0xF0) == 0xE0)
            return 3;
        else if ((initialCodeUnit & 0xF8) == 0xF0)
            return 4;
        else
            return 0;
    }

    static size_t getCodePointUtf8Size(char32_t currentCodeUnit)
    {
        if (currentCodeUnit < 0x80)
            return 1;
        if (currentCodeUnit < 0x800)
            return 2;
        if (currentCodeUnit < 0x10000)
            return 3;
        return 4;
    }

    static char32_t convertCodePoint(const char firstCodeUnit,
                                     const char secondCodeUnit);

    static char32_t convertCodePoint(const char firstCodeUnit,
                                     const char secondCodeUnit,
                                     const char thirdCodeUnit);

    static char32_t convertCodePoint(const char firstCodeUnit,
                                     const char secondCodeUnit,
                                     const char thirdCodeUnit,
                                     const char fourthCodeUnit);

    static bool isContinuingUTF8CodeUnit(const char codeUnit);

    static const String& GetEmpty();

    //////////////////////////////////////////////////////////////////////////
    // Default constructors and destructors
    //////////////////////////////////////////////////////////////////////////

    //! Default constructor. Constructs empty string (zero size and unspecified capacity)
    String()
        : String(std::allocator<value_type>())
    {}

    //! Default constructor. Constructs empty string (zero size and unspecified capacity)
    explicit String(const std::allocator<value_type>& alloc)
        : d_string(alloc)
    {}

    //! Destructor
    ~String()
    {}

    //////////////////////////////////////////////////////////////////////////
    // Non-default constructors for u32string and CEGUI::String meant to
    // replicate all constructors otherwise provided by basic_string
    //////////////////////////////////////////////////////////////////////////
    
    /*!
    \brief
        Constructs a String containing count copies of code unit "ch". The
        behavior is undefined for count >= npos.
    */
    String(size_type count, value_type ch, const std::allocator<value_type>& alloc = std::allocator<value_type>())
        : d_string(count, ch, alloc)
    {}

    /*!
    \brief
        Constructs a String containing count copies of code unit "ch". The
        behavior is undefined for count >= npos.
    */
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    String(size_type count, char32_t ch, const std::allocator<value_type>& alloc = std::allocator<value_type>())
        : d_string(alloc)
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    String(size_type count, char ch, const std::allocator<value_type>& alloc = std::allocator<value_type>())
        : d_string(alloc)
#endif
    {
        assign(count, ch);
    }


    /*!
    \brief
        Constructs the String based on a substring [pos, pos+count) of another
        String. If the requested substring would be longer than the other String
        or if count == npos, then the resulting substring reaches from the
        requested position to its end.
    */
    String(const CEGUI::String& other, size_type pos, size_type count = npos, const std::allocator<value_type>& alloc = std::allocator<value_type>())
        : d_string(other.d_string, pos, count, alloc)
    {}

    /*!
    \brief
        Constructs the String based on a substring [pos, pos+count) of another
        String. If the requested substring would be longer than the other String
        or if count == npos, then the resulting substring reaches from the
        requested position to its end.
    */
    String(const std::basic_string<value_type>& other, size_type pos, size_type count = npos, const std::allocator<value_type>& alloc = std::allocator<value_type>())
        : d_string(other, pos, count, alloc)
    {}

    /*!
    \brief
        Constructs the String based on the specified number of code units by
        copying them from charArray. The resulting String may contain
        null-characters. The length of the new String is count. The behaviour is
        undefined if charArray does not point to an array with at least length
        count.
    */
    String(const value_type* charArray, size_type count,
           const std::allocator<value_type>& alloc = std::allocator<value_type>())
        : d_string(charArray, count, alloc)
    {}

    /*!
    \brief
        Constructs the String based on a supplied null-terminated array of
        code units "charArray". The code units are copied until the first
        null-character is reached.
        The behavior is undefined if "charArray" does not point to an array of
        at least Traits::length(s) + 1 elements of String::value_type.
    */
    String(const value_type* charArray, const std::allocator<value_type>& alloc = std::allocator<value_type>())
        : d_string(charArray, alloc)
    {}

    /*!
    \brief
        Constructs a String with the contents of the range [first, last). 
    */
    template<class InputIt>
    String(InputIt first, InputIt last, const std::allocator<value_type>& alloc = std::allocator<value_type>())
        : d_string(first, last, alloc)
    {}

    /*!
    \brief
        Copy constructor. 
    */
    String(const CEGUI::String& other)
        : d_string(other.d_string)
    {}

    /*!
    \brief
        Copy constructor based on a std::basic_string. 
    */
    String(const std::basic_string<value_type>& other)
        : d_string(other)
    {}

#if defined(CEGUI_STRING_CPP_11)
    /*!
    \brief
        Copy constructor. 
    */
    String(const CEGUI::String& other, const std::allocator<value_type>& alloc)
        : d_string(other.d_string, alloc)
    {}

    /*!
    \brief
        Copy constructor based on a std::basic_string. 
    */
    String(const std::basic_string<value_type>& other, const std::allocator<value_type>& alloc)
        : d_string(other, alloc)
    {}
#endif

    /*!
    \brief
        Move constructor. The String "other" is left in valid but unspecified state.
    */
    String(CEGUI::String&& other) noexcept
        : d_string(std::move(other.d_string))
    {}

    /*!
    \brief
        Move constructor. The String "other" is left in valid but unspecified state.
    */
    String(std::basic_string<value_type>&& other)
        : d_string(std::move(other))
    {}

#if defined(CEGUI_STRING_CPP_11)
    /*!
    \brief
        Move constructor. The String "other" is left in valid but unspecified state.
    */
    String(CEGUI::String&& other, const std::allocator<value_type>& alloc)
        : d_string(std::move(other.d_string), alloc)
    {}


    /*!
    \brief
        Move constructor. The String "other" is left in valid but unspecified state.
    */
    String(std::basic_string<value_type>&& other, const std::allocator<value_type>& alloc)
        : d_string(std::move(other), alloc)
    {}

    /*!
    \brief
        Constructs a String with the contents of the initialiser-list "init"
    */
    String(std::initializer_list<value_type> init, const std::allocator<value_type>& alloc = std::allocator<value_type>())
        : d_string(init, alloc)
    {}
#endif

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Constructor that creates a new string based
        on an std::string that can
        either contain ASCII or UTF-8 encoded code
        units. 

    \param str
        std::string object containing either ASCII
        or UTF-8 encoded code units.
    */
    String(const std::u32string& str)
        : d_string(convertUtf32ToUtf8(str))
    {}
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Constructor that creates a new string based
        on an std::string that can
        either contain ASCII or UTF-8 encoded code
        units. 

    \param str
        std::string object containing either ASCII
        or UTF-8 encoded code units.
    */
    String(const std::string& str)
        : d_string(convertUtf8ToUtf32(str))
    {}
#endif
    
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Constructor that creates a new string based on
        a char array "str" that contains UTF-32 encoded
        code units. 

    \param str
        Pointer to the char array containing UTF-32
        encoded code units.
    */
    String(const char32_t* str)
        : d_string(convertUtf32ToUtf8(str))
    {}
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Constructor that creates a new string based on an char array "str" that can
        either contain ASCII or UTF-8 encoded code units. 

    \param str
        Pointer to the char array containing either ASCII or UTF-8 encoded code units.
    */
    String(const char* str)
        : d_string(convertUtf8ToUtf32(str))
    {}
#endif

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Constructs a new String object and initialise it using
        code units from the provided char array, which contains
        UTF-32 encoded code units.

    \param charArray
        Array of UTF-32 encoded code units.

    \param chars_len
        Number of characters (UTF-32 code units) from
        the array to be used.

    \exception
        std::length_error Thrown if resulting String object would
        be too big.
    */
    String(const char32_t* charArray, size_type chars_len)
    {
        assign(charArray, chars_len);
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Constructs a new String object and initialise it using
        code units from the provided char array, which can contain
        ASCII or UTF-8 encoded code units.

    \param charArray
        Array of code units, which can be ASCI
        or UTF-8 encoded.

    \param chars_len
        Number of ASCII or UTF-8 encoded code units from
        the array to be used.

    \exception
        std::length_error Thrown if resulting String object would
        be too big.
    */
    String(const char* charArray, size_type chars_len)
    {
        assign(charArray, chars_len);
    }
#endif

    //////////////////////////////////////////////////////////////////////////
    // Static public members
    //////////////////////////////////////////////////////////////////////////

    /*!
        Value used to represent 'not found' conditions and 'all code units /
        code points' etc.
    */
    static const size_type npos = -1;

    //////////////////////////////////////////////////////////////////////////
    // Assignment operators
    //////////////////////////////////////////////////////////////////////////

    /*!
    \brief
        Replaces the contents of this String with a copy of String "str". This function has no effect
        if *this and the supplied String "str" are the same object,.
    */
    String& operator=(const String& str)
    {
        d_string = str.d_string;
        return *this;
    }

    /*!
    \brief
        Replaces the contents of this String with a copy of basic_string "str". 
    */
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    String& operator=(const std::u32string& str)
    {
        d_string = convertUtf32ToUtf8(str);
        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    String& operator=(const std::string& str)
    {
        d_string = convertUtf8ToUtf32(str);
        return *this;
    }
#endif

    /*!
    \brief
        Replaces the contents with this String with those of str using move semantics. Leaves str in a valid but unspecified state.
        This function has no effect if *this and the supplied String "str" are the same object.
    */
    String& operator=(String&& str) noexcept
    {
        d_string = std::move(str.d_string);
        return *this;
    }

    /*!
    \brief
        Replaces the contents with those of a null-terminated string stored
        in a String::value_type array "charArray". Calls *this = String(charArray)
        internally, which involves a call to Traits::length(charArray).
    */ 
    String& operator=(const value_type* charArray)
    {
        d_string = charArray;
        return *this;
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Replaces the contents with those of a null-terminated UTF-32
        encoded string stored in a char32_t array "charArray".
        Calls  *this = String(charArray) internally, which involves a
        call to Traits::length(charArray).
    */ 
    String& operator=(const char32_t* charArray)
    {
        *this = String(charArray);
        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Replaces the contents with those of a null-terminated UTF-8
        encoded or ASCII code unit string stored in a char array "charArray".
        Calls  *this = String(charArray) internally, which involves a
        call to Traits::length(charArray).
    */ 
    String& operator=(const char* charArray)
    {
        *this = String(charArray);
        return *this;
    }
#endif

    /*!
    \brief
        Replaces this Strings contents by the code unit ch,
        equivalent to *this = String(1, c).
    */ 
    String& operator=(value_type ch)
    {
        d_string = ch;
        return *this;
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Replaces this Strings contents by the code unit ch,
        using assign(1, ch)
    */ 
    String& operator=(char32_t ch)
    {
        assign(1, ch);
        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Replaces this Strings contents by the code unit ch,
        using assign(ch)
    */ 
    String& operator=(char ch)
    {
        assign(1, ch);
        return *this;
    }
#endif

    /*!
    \brief
        Replaces the contents with those of the initializer list
        initialiserList as if by *this = basic_string(ilist)
    */ 
    String& operator=(std::initializer_list<value_type> initialiserList)
    {
        d_string = initialiserList;
        return *this;
    }

    //////////////////////////////////////////////////////////////////////////
    // Assigning characters
    //////////////////////////////////////////////////////////////////////////

    /*!
    \brief
        Replaces the contents with count copies of code unit "ch".
    */ 
    String& assign(size_type count, value_type ch)
    {
        d_string.assign(count, ch);
        return *this;
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Replaces the contents with count copies of code unit "ch".
    */ 
    String& assign(size_type count, char32_t ch)
    {
        std::string utf8String = convertUtf32ToUtf8(ch);
        d_string.assign(utf8String);
        for(size_t i = 1; i < count; ++i)
        {
            d_string.append(utf8String);
        }

        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Replaces the contents with count copies of code unit "ch".
    */ 
    String& assign(size_type count, char ch)
    {
        d_string.assign(count, convertUtf8ToUtf32(ch)[0]);
        return *this;
    }
#endif

    /*!
    \brief
        Replaces the String with a copy of the supplied String "str".
    */ 
    String& assign(const String& str)
    {
        d_string.assign(str.d_string);
        return *this;
    }


    /*!
    \brief
        Replaces the String with a copy of the supplied String "str".
        Performs a conversion of the unicode encoding to the native
        encoding.
    */ 
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    String& assign(const std::u32string& str)
    {
        d_string.assign(convertUtf32ToUtf8(str));
        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    String& assign(const std::string& str)
    {
        d_string.assign(convertUtf8ToUtf32(str));
        return *this;
    }
#endif

    /*!
    \brief
        Replaces the contents of this String with a substring
        [pos, pos+count) of String "str". If the requested substring is
        longer than String "str" or if count == npos, the resulting 
        substring has the range [pos, size()). If pos > str.size(),
        std::out_of_range is thrown. 
    */ 
    String& assign(const String& str, size_type pos, size_type count = npos)
    {
        d_string.assign(str.d_string, pos, count);
        return *this;
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Replaces the contents of this String with a substring [pos, pos+count)
        of an UTF-32 encoded String "str". The position and length relate to
        the code point count (number of char32_t code units in the string).
        If the requested substring is longer than String "str" or if count == npos,
        the resulting substring has the range [pos, size()).
        If pos > str.size(), std::out_of_range is thrown.
    */ 
    String& assign(const std::u32string& str, size_type pos, size_type count = npos)
    {
        std::string convertedStr = convertUtf32ToUtf8(str);
        d_string.assign(convertedStr, pos, count);
        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Replaces the contents of this String with a substring [pos, pos+count)
        of an ASCII or UTF-8 encoded String "str". The position and count
        relate to the code point count (number of char32_t code units) and not
        the number of code units (number of char code units).
        If the requested substring is longer than String "str" or if count == npos,
        the resulting substring has the range [pos, size()). If pos > str.size(),
        std::out_of_range is thrown.
    */ 
    String& assign(const std::string& str, size_type pos, size_type count = npos)
    {
        std::u32string convertedStr = convertUtf8ToUtf32(str);
        d_string.assign(convertedStr, pos, count);
        return *this;
    }
#endif

    /*!
    \brief
        Replaces the contents of this String with those of a
        String "str" using move semantics. The String
        "str" is in undefined state after the operation.
    */ 
    String& assign(String&& str)
    {
        d_string.assign(std::move(str.d_string));
        return *this;
    }

    /*!
    \brief
        Replaces the contents of this String with "count" number
        of code units from the String::value_type array "charArray".
    */ 
    String& assign(const value_type* charArray, size_type count)
    {
        d_string.assign(charArray, count);
        return *this;
    }


#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Replaces the contents of this String with "count" number
        of code units from the null-terminated char32_t
        array "charArray", which contains UTF-32 encoded code points.
        The count refers to the number of code points to be converted
        and assigned and is therefore not always equal to the number
        of code units that are actually assigned to the internal
        string representation. The input array will only be processed
        until the first null-character in the array.
    */ 
    String& assign(const char32_t* charArray, size_type count)
    {
        std::string convertedString = convertUtf32ToUtf8(charArray, charArray + count);
        d_string.assign(convertedString.data(), convertedString.length());
        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Replaces the contents of this String with "count" number of
        code units or ASCII code units from the null-terminated char
        array "charArray", which may contain either UTF-8 encoded
        code units or ASCII code units.
        In the case of UTF-8 encoded code units, the count refers
        to the number of code units (char), not to the number of actual
        code units. The input array will only be processed
        until the first null-character in the array.
    */ 
    String& assign(const char* charArray, size_type count)
    {
        std::u32string convertedString = convertUtf8ToUtf32(charArray, charArray + count);
        d_string.assign(convertedString.data(), convertedString.length());
        return *this;
    }
#endif

    /*!
    \brief
        Replaces the contents of this String with the contents of a
        null-terminated code unit string "charArray". The length
        of the string is determined by the first null-character,
        which is encountered. 
    */ 
    String& assign(const value_type* charArray)
    {
        d_string.assign(charArray);
        return *this;
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Replaces the contents of this String with the contents of a
        null-terminated code unit string "charArray" containing UTF-32
        encoded code units. The length of the string is determined by
        the first null code unit in the array and is measured in the
        number of code points.
    */ 
    String& assign(const char32_t* charArray)
    {
        d_string.assign(convertUtf32ToUtf8(charArray).data());
        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Replaces the contents of this String with the contents of a
        null-terminated code unit string "charArray" containing either
        UTF-8 encoded code units or ASCII code units. The length of
        the string is determined by the first null-character and is
        measured in the number of code units.
    */ 
    String& assign(const char* charArray)
    {
        d_string.assign(convertUtf8ToUtf32(charArray).data());
        return *this;
    }
#endif

    /*!
    \brief
        Replaces the contents of this string with copies
        of the code units in the range [first, last).
    */
    template<class InputIt>
    String& assign(InputIt first, InputIt last)
    {
        d_string.assign(first, last);
        return *this;
    }

    /*!
    \brief
        Replaces the contents of this string with the contents
        of the initialiser-list "initialiserList".
    */
    String& assign(std::initializer_list<value_type> ilist)
    {
        d_string.assign(ilist);
        return *this;
    }

#if defined(CEGUI_STRING_CPP_11)
    /*!
    \brief
        Replaces the contents of this string with the contents
        of the initialiser-list "initialiserList".
    */
    std::allocator<value_type> get_allocator() const
    {
        return d_string.get_allocator();
    }
#endif

    //////////////////////////////////////////////////////////////////////////
    // Element access
    //////////////////////////////////////////////////////////////////////////

    /*!
    \brief
        Returns a reference to the code unit at
        specified location "pos". The location is boundary-checked and an
        exception of type std::out_of_range will be thrown on invalid access.

    \param idx
        Zero based index of the element to be returned.
    */
    reference at(size_type pos)
    {
        return d_string[pos];
    }

    /*!
    \brief
        Returns a reference to the code unit at
        specified location "pos". The location is boundary-checked and an
        exception of type std::out_of_range will be thrown on invalid access.

    \param idx
        Zero based index of the element to be returned.
    */
    const_reference at(size_type pos) const
    {
        return d_string[pos];
    }

    /*!
    \brief
        Returns the code unit at the given index.

    \param idx
        Zero based index of the element to be returned.

    \return
        The String::value_type element at the given index within the String.
    */
    reference operator[](size_type pos)
    {
        return d_string[pos];
    }

    /*!
    \brief
        Returns the code unit at the given index.

    \param idx
        Zero based index of the element to be returned.

    \return
        The String::value_type element at the given index within the String.
    */
    const_reference operator[](size_type pos) const
    {
        return d_string[pos];
    }

    /*!
    \brief
        Returns the first code unit of the String.

    \return
        The String::value_type element at the front of the String.
    */
    value_type& front()
    {
        return d_string.front();
    }

    /*!
    \brief
        Returns the first code unit of the String.

    \return
        The String::value_type element at the front of the String.
    */
    const value_type& front() const
    {
        return d_string.front();
    }
   
    /*!
    \brief
        Returns the last code unit of the String.

    \return
        The String::value_type element at back of the String.
    */
    value_type& back()
    {
        return d_string.back();
    }

    /*!
    \brief
        Returns the last code unit of the String.

    \return
        The String::value_type element at back of the String.
    */
    const value_type& back() const
    {
        return d_string.back();
    }

    /*!
    \brief
        Returns a pointer to the underlying String::value_type
        array of the String. The returned array is null-terminated.

    \return
        Pointer to the first element of the internal array of
        String::value_type elements.
    */
    const value_type* data() const
    {
        return d_string.data();
    }

    /*!
    \brief
        Returns a pointer to the underlying String::value_type
        array of the String. The returned array is null-terminated.

    \return
        Pointer to the first element of the internal array of
        String::value_type elements.
    */
    const value_type* c_str() const
    {
        return d_string.c_str();
    }


    //////////////////////////////////////////////////////////////////////////
    // Iterators
    //////////////////////////////////////////////////////////////////////////


    /*!
    \brief
        Returns an iterator to the first code unit of the String. 

    \return
        An iterator object pointing to the beginning of the String.
    */
    iterator begin()
    {
        return d_string.begin();
    }

    /*!
    \brief
        Returns a constant iterator to the first code unit of the
        String. 

    \return
        A const_iterator object pointing to the beginning of the String.
    */
    const_iterator begin() const
    {
        return d_string.begin();
    }

    /*!
    \brief
        Returns a constant iterator to the first code unit of the
        String. 

    \return
        A const_iterator object pointing to the beginning of the String.
    */
    const_iterator cbegin() const
    {
        return d_string.cbegin();
    }

    /*!
    \brief
        Returns an iterator to the element following the last code unit /
        code point of the String. Since this iterator is merely a placeholder,
        accessing it results in undefined behavior. 

    \return
        An iterator object pointing to the element after the last code unit /
        code point of the String.
    */
    iterator end()
    {
        return d_string.end();
    }

    /*!
    \brief
        Returns a constant iterator to the element following the last code unit /
        code point of the String. Since this iterator is merely a placeholder,
        accessing it results in undefined behavior. 

    \return
        A const_iterator object pointing to the element after the last code unit /
        code point of the String.
    */
    const_iterator end() const
    {
        return d_string.end();
    }

    /*!
    \brief
        Returns a constant iterator to the element following the last code unit /
        code point of the String. Since this iterator is merely a placeholder,
        accessing it results in undefined behavior. 

    \return
        A const_iterator object pointing to the element after the last code unit /
        code point of the String.
    */
    const_iterator cend() const
    {
        return d_string.cend();
    }

    /*!
    \brief
        Returns a reverse iterator to the first code unit of the
        reversed String. This corresponds to the last element of the non-reversed
        String. 

    \return
        A reverse_iterator object pointing to the first code unit of the reversed
        String.
    */
    reverse_iterator rbegin()
    {
        return d_string.rbegin();
    }

    /*!
    \brief
        Returns a constant reverse iterator to the first code unit
        of the reversed String. This corresponds to the last element of the
        non-reversed String. 

    \return
        A constant reverse_iterator object pointing to the first code unit of the
        reversed String.
    */
    const_reverse_iterator rbegin() const
    {
        return d_string.rbegin();
    }

    /*!
    \brief
        Returns a constant reverse iterator to the first code unit
        of the reversed String. This corresponds to the last element of the
        non-reversed String. 

    \return
        A constant reverse_iterator object pointing to the first code unit of the
        reversed String.
    */
    const_reverse_iterator crbegin() const
    {
        return d_string.crbegin();
    }

    /*!
    \brief
        Returns a reverse iterator to the code unit following the
        last element of the reversed String. This corresponds to the code unit /
        code point preceding the first code unit of the non-reversed
        string. Since this acts as a placeholder, accessing it results in undefined
        behavior. 

    \return
        A reverse_iterator object pointing to the code unit following
        the last element of the reversed String.
    */
    reverse_iterator rend()
    {
        return d_string.rend();
    }

    /*!
    \brief
        Returns a constant reverse iterator to the code unit following
        the last element of the reversed String. This corresponds to the code unit /
        code point preceding the first code unit of the non-reversed
        string. Since this acts as a placeholder, accessing it results in undefined
        behavior. 

    \return
        A const_reverse_iterator object pointing to the code unit
        following the last element of the reversed String.
    */
    const_reverse_iterator rend() const
    {
        return d_string.rend();
    }

    /*!
    \brief
        Returns a constant reverse iterator to the code unit following
        the last element of the reversed String. This corresponds to the code unit /
        code point preceding the first code unit of the non-reversed
        string. Since this acts as a placeholder, accessing it results in undefined
        behavior. 

    \return
        A const_reverse_iterator object pointing to the code unit
        following the last element of the reversed String.
    */
    const_reverse_iterator crend() const
    {
        return d_string.rend();
    }


    //////////////////////////////////////////////////////////////////////////
    // Capacity
    //////////////////////////////////////////////////////////////////////////


    /*!
    \brief
        Checks whether the String contains code units or not.

    \return
        true if the String is empty, false if not.
    */
    bool empty() const
    {
        return d_string.empty();
    }

    /*!
    \brief
        Returns the size of the String in number of code units.

    \return
        Number of code units contained in the String.
    */
    size_type size() const
    {
        return d_string.size();
    }

    /*!
    \brief
        Returns the size of the String in number of code units.

    \return
        Number of code units contained in the String.
    */
    size_type length() const
    {
        return d_string.length();
    }


    /*!
    \brief
        Returns the maximum of code units this type of String is able to hold.

    \return
        The maximum number of code units that a String can contain.
    */
    size_type max_size() const
    {
        return d_string.max_size();
    }

    /*!
    \brief
        Informs this String of a planned size-change, so that the storage
        allocation can be managed appropriately.

    \param new_cap
        The number of code units to allocate space for.  If \a num is
        larger that the current reserve, then a re-allocation will occur.
        If \a num is smaller than the current reserve (but not 0) the buffer
        may be shrunk to the larger of the specified number, or the current
        String size (operation is currently not implemented).  If \a num i
        0, then the buffer is re-allocated to fit the current String size.

    \exception std::length_error Thrown if resulting String object would be
        too big.
    */
    void reserve(size_type new_cap = 0)
    {
        d_string.reserve(new_cap);
    }

    /*!
    \brief
        Return the number of code units that the String can hold before a
        re-allocation is be required.

    \return
        Capacity of the currently allocated space.
    */
    size_type capacity() const
    {
        return d_string.capacity();
    }

    /*!
    \brief
        Requests the removal of unused capacity. Depending on the 
        implementation, this call may reduce the capacity to size()
        or may have no consequences at all.
    */
    void shrink_to_fit()
    {
        d_string.shrink_to_fit();
    }

    //////////////////////////////////////////////////////////////////////////
    // Operations
    //////////////////////////////////////////////////////////////////////////

    /*!
    \brief
        Removes all code units of the string. No memory will be deallocated,
        leaving the capacity of the String unchanged.
    */
    void clear()
    {
        d_string.clear();
    }

    /*!
    \brief
        Inserts "count" number of copies of the code units at the position index.
    \param index
        The position index at which the code units are inserted.
    \param count
        The number of code unit copies to insert
    \param ch
        The code unit to copy "count" times and insert
    \return Returns this String (*this).
    */
    String& insert(size_type index, size_type count, value_type ch)
    {
        d_string.insert(index, count, ch);
        return *this;
    }

    /*!
    \brief
        Inserts "count" number of copies of the
        code units at the position index.
    \param index
        The position index at which the code units are
        inserted.
    \param count
        The number of code unit copies to insert
    \param ch
        The code unit to copy "count" times and insert
    \return Returns this String (*this).
    */

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    String& insert(size_type index, size_type count, char32_t ch)
    {
        std::string convertedString = convertUtf32ToUtf8(ch);
        for(size_t i = 0; i < count; ++i)
        {
            d_string.insert(index, convertedString);
        }
        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    String& insert(size_type index, size_type count, char ch)
    {
        std::u32string convertedString = convertUtf8ToUtf32(ch);
        d_string.insert(index, count, convertedString[0]);
        return *this;
    }
#endif

    /*!
    \brief
        Inserts a null-terminated code unit array "charArray" at the
        specified position in this String. The length of the inserted
        string taken from the array is determined by the first
        null-character that is encountered.
    \param index
        The position index at which the code units are inserted.
    \param charArray
        The code unit array to be inserted.
    \return Returns this String (*this).
    */
    String& insert(size_type index, const value_type* charArray)
    {
        d_string.insert(index, charArray);
        return *this;
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Inserts a null-terminated code unit array "charArray" consisting
        of UTF-32 encoded code units. The string stored in the
        array is inserted at the specified position in this String.
        The length of the inserted string is determined by the first
        null-character that is encountered.
    \param index
        The position index at which the code units are inserted.
    \param charArray
        The code unit array to be inserted.
    \return Returns this String (*this).
    */
    String& insert(size_type index, const char32_t* charArray)
    {
        d_string.insert(index, convertUtf32ToUtf8(charArray).data());
        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Inserts a null-terminated code unit array "charArray" consisting
        either of UTF-8 encoded or ASCII code units. The string stored in
        the array is inserted at the specified position in this String.
        The length of the inserted string is determined by the first
        null-character that is encountered.
    \param index
        The position index at which the code units are inserted.
    \param charArray
        The code unit array to be inserted.
    \return Returns this String (*this).
    */
    String& insert(size_type index, const char* charArray)
    {
        d_string.insert(index, convertUtf8ToUtf32(charArray).data());
        return *this;
    }
#endif

    /*!
    \brief
        Inserts the specified number of code units from a code unit
        array. The code units are inserted at the specified position.
        The string may contain null-characters.
    \param index
        The position index at which the code units are inserted.
    \param charArray
        The code unit array containing the code units to
        be inserted.
    \param count
        The count of the code units to be inserted.
    \return Returns this String (*this).
    */
    String& insert(size_type index, const value_type* charArray, size_type count)
    {
        d_string.insert(index, charArray, count);
        return *this;
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Inserts the specified number of code units from the code unit
        array "charArray" containing either UTF-32 encoded
        code units. The code units are inserted at the specified position.
        The count of code units refers to the number of code units to be
        inserted, not the number of actual code units.
    \param index
        The position index at which the code units are inserted.
    \param charArray
        The code unit array containing the code units to be inserted.
    \param count
        The count of the code units to be inserted.
    \return Returns this String (*this).
    */
    String& insert(size_type index, const char32_t* charArray, size_type count)
    {
        std::string convertedString = convertUtf32ToUtf8(charArray, count);
        d_string.insert(index, convertedString.data(), convertedString.length());
        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Inserts the specified number of code units from the code unit
        array "charArray" containing either UTF-8 or ASCII encoded
        code units. The code units are inserted at the specified position.
        The count of code units refers to the number of code units to be
        inserted, not the number of actual code units.
    \param index
        The position index at which the code units are inserted.
    \param charArray
        The code unit array containing the code units to be inserted.
    \param count
        The count of the code units to be inserted.
    \return Returns this String (*this).
    */
    String& insert(size_type index, const char* charArray, size_type count)
    {
        std::u32string convertedString = convertUtf8ToUtf32(charArray, count);
        d_string.insert(index, convertedString.data(), convertedString.length());
        return *this;
    }
#endif

    /*!
    \brief
        Inserts the supplied String at the specified position of this String.
    \param index
        The position index at which the String will be inserted.
    \param str
        A reference to the String object that will be inserted.
    \return Returns this String (*this).
    */
    String& insert(size_type index, const String& str)
    {
        d_string.insert(index, str.d_string);
        return *this;
    }

    /*!
    \brief
        Inserts the supplied basic_string of value_type at the
        specified position of this String.
    \param index
        The position index at which the string will be inserted.
    \param str
        A reference to the string object that will be inserted.
    \return Returns this String (*this).
    */
    String& insert(size_type index, const std::basic_string<value_type>& str)
    {
        d_string.insert(index, str);
        return *this;
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Inserts the supplied string, containing UTF-32
        encoded code units, at the specified position of this String.
    \param index
        The position index at which the string will be inserted.
    \param str
        A reference to the string object, containing UTF-8 encoded
        or ASCII code units, which will be inserted.
    \return Returns this String (*this).
    */
    String& insert(size_type index, const std::u32string& str)
    {
        d_string.insert(index, convertUtf32ToUtf8(str));
        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Inserts the supplied string, containing UTF-8 encoded or
        ASCII code units, at the specified position of this String.
    \param index
        The position index at which the string will be inserted.
    \param str
        A reference to the string object, containing UTF-8 encoded
        or ASCII code units, which will be inserted.
    \return Returns this String (*this).
    */
    String& insert(size_type index, const std::string& str)
    {
        d_string.insert(index, convertUtf8ToUtf32(str));
        return *this;
    }
#endif

    /*!
    \brief
        Inserts a substring of the supplied String at the position index.
    \param index
        The position index at which the substring String will be inserted.
    \param str
        A reference to the String object that will be inserted as substring.
    \param index_str
        Position of the first code unit of the substring to be inserted.
    \param count
        Number of code units of the substring to be inserted.
    \return Returns this String (*this).
    */
    String& insert(size_type index, const String& str, size_type index_str, size_type count = npos)
    {
        d_string.insert(index, str.d_string, index_str, count);
        return *this;
    }

    /*!
    \brief
        Inserts a substring of the supplied string at the position index.
    \param index
        The position index at which the substring string will be inserted.
    \param str
        A reference to the string object that will be inserted as substring.
    \param index_str
        Position of the first code unit of the substring to be inserted.
    \param count
        Number of code units of the substring to be inserted.
    \return Returns this String (*this).
    */ 
    String& insert(size_type index, const std::basic_string<value_type>& str,
        size_type index_str, size_type count = npos)
    {
        d_string.insert(index, str, index_str, count);
        return *this;
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Inserts a substring of the supplied string, containing
        UTF-32 encoded code units, at the position index.
    \param index
        The position index at which the substring string will be inserted.
    \param str
        A reference to the string object, containing UTF-32 encoded
        code units, which will be inserted as substring.
    \param index_str
        Position of the first code unit of the substring to be inserted.
    \param count
        Number of code points (not code units!) of the substring to be inserted.
    \return Returns this String (*this).
    */ 
    String& insert(size_type index, const std::u32string& str,
        size_type index_str, size_type count = npos)
    {
        std::string convertedString = convertUtf32ToUtf8(str.substr(index_str, count));
        d_string.insert(index, convertedString);
        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Inserts a substring of the supplied string, containing
        UTF-8 encoded or ASCII code units, at the position index.
    \param index
        The position index at which the substring string will be inserted.
    \param str
        A reference to the string object, containing UTF-8 encoded
        or ASCII code units, which will be inserted as substring.
    \param index_str
        Position of the first code unit of the substring to be inserted.
    \param count
        Number of code points (not code units!) of the substring to be inserted.
    \return Returns this String (*this).
    */ 
    String& insert(size_type index, const std::string& str,
        size_type index_str, size_type count = npos)
    {
        std::u32string convertedString = convertUtf8ToUtf32(str.substr(index_str, count));
        d_string.insert(index, convertedString);
        return *this;
    }
#endif

    /*!
    \brief
        Inserts code unit "ch" before the code unit pointed to by
        iterator "iter".
    \param iter
        The position iterator at which the code unit will be inserted.
    \param ch
        The code unit to be inserted.
    \return Returns an iterator pointing to the first inserted code unit. Returns
        "iter" if no code units were inserted.
    */
#if defined (CEGUI_STRING_CPP_11)
    iterator insert(const_iterator iter, value_type ch)
#else
    iterator insert(iterator iter, value_type ch)
#endif
    {
        return d_string.insert(iter, ch);
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Inserts an UTF-8 encoded or ASCII code unit code unit before
        the code unit pointed to by an iterator.
    \param iter
        The position iterator at which the code unit will be inserted.
    \param ch
        The code unit to be inserted.
    \return Returns an iterator pointing to the first inserted code unit
        code unit. Returns the supplied iterator if no code units were
        inserted.
    */
#if defined (CEGUI_STRING_CPP_11)
    iterator insert(const_iterator iter, char ch)
#else
    iterator insert(iterator iter, char ch)
#endif
    {
        return d_string.insert(iter, convertUtf8ToUtf32(ch)[0]);
    }
#endif

   /*!
    \brief
        Inserts "count" number of copies of a code unit before the
        code unit of this String pointed to by an iterator.
    \param iter
        The position iterator at which the code units will be inserted.
    \param count
        The number of copies of the code unit to insert.
    \param ch
        The code unit to be copied and inserted.
    \return Returns an iterator pointing to the first inserted code unit. Returns
        "iter" if no code units were inserted.
    */
    #if defined (CEGUI_STRING_CPP_11)
    iterator insert(const_iterator iter, size_type count, value_type ch)
    {
        return d_string.insert(iter, count, ch);
    }
    #else 
    void insert(iterator iter, size_type count, value_type ch)
    {
        d_string.insert(iter, count, ch);
    }
    #endif

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Inserts a number of copies of an UTF-8 encoded code unit or ASCII
        code unit before the code unit of the String pointed to by an
        iterator.
    \param iter
        The position iterator at which the code units will be inserted.
    \param count
        The number of copies of the code unit to be inserted.
    \param ch
        The UTF-8 encoded code unit or ASCII code unit to be copied and inserted.
    \return Returns an iterator pointing to the first inserted code unit.
        Returns the iterator if no code units were inserted.
    */
    #if defined (CEGUI_STRING_CPP_11)
    iterator insert(const_iterator iter, size_type count, char ch)
    {
        return d_string.insert(iter, count, convertUtf8ToUtf32(ch)[0]);
    }
    #else
    void insert(iterator iter, size_type count, char ch)
    {
        d_string.insert(iter, count, convertUtf8ToUtf32(ch)[0]);
    }
    #endif
#endif

    /*!
    \brief
        Inserts code units from the range [first, last) before the element pointed to by "iter".
    \param iter
        The position iterator at which the code units will be inserted.
    \param first
        Iterator pointing to the beginning of the range
    \param last
        Iterator pointing to the end of the range
    \return Returns an iterator pointing to the first inserted code unit. Returns
        "iter" if no code units were inserted.
    */
    template<class InputIt>
    iterator insert(const_iterator iter, InputIt first, InputIt last)
    {
        return d_string.insert(iter, first, last);
    }

    /*!
    \brief
        Inserts elements from initializer_list "initialiserList" before the element pointed by pos.
    \param iter
        The position iterator at which the code units will be inserted.
    \param initialiserList
        The initialiser list to be inserted.
    \return Returns an iterator pointing to the first inserted code unit. Returns
        "iter" if no code units were inserted.
    */
#if defined (CEGUI_STRING_CPP_11)
    iterator insert(const_iterator iter, std::initializer_list<value_type> initialiserList)
    {
        return d_string.insert(iter, initialiserList);
    }
#else 
    void insert(iterator iter, std::initializer_list<value_type> initialiserList)
    {
        d_string.insert(iter, initialiserList);
    }
#endif
    /*!
    \brief
        Removes the specified number of code units starting at the index.
    \param index
        The position of the first code unit to remove.
    \param count
        The number of code units to remove.
    \return Returns a reference to this String (*this)
    */
    String& erase(size_type index = 0, size_type count = npos)
    {
        d_string.erase(index, count);
        return *this;
    }

    /*!
    \brief
        Removes the specified code unit at the position the iterator "iter" is pointing to.
    \param iter
        Iterator pointing at the code unit to remove.
    \return Iterator pointing to the code unit after the last removed code unit.
    */
#if defined (CEGUI_STRING_CPP_11)
    iterator erase(const_iterator iter)
#else
    iterator erase(iterator iter)
#endif
    {
        return d_string.erase(iter);
    }

    /*!
    \brief
        Removes the code units in the range specified by the iterators "first" and "last".
    \param first
        Iterator pointing to the first code unit of the range of code units to remove.
    \param first
        Iterator pointing to the last code unit of the range of code units to remove.
    \return Iterator pointing to the code unit after the last removed code unit.
    */
#if defined (CEGUI_STRING_CPP_11)
    iterator erase(const_iterator first, const_iterator last)
#else
    iterator erase(iterator first, iterator last)
#endif
    {
        return d_string.erase(first, last);
    }

    /*!
    \brief
        Appends the given code unit "ch" to the end of the String. 
    \param ch
        The code unit to append to the String.
    */
    void push_back(value_type ch)
    {
        d_string.push_back(ch);
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Appends the given UTF-8 or ASCII encoded code unit "ch"
        to the end of the String. 
    \param ch
        The code unit to append to the String.
    */
    void push_back(char ch)
    {
        d_string.push_back(convertUtf8ToUtf32(ch)[0]);
    }
#endif

    /*!
    \brief
        Removes the last code unit from the String. The behaviour is undefined for empty Strings.
    \param ch
        The code unit to append to the String.
    */
    void pop_back()
    {
        d_string.pop_back();
    }

    /*!
    \brief
        Appends "count" number of copies of code unit "ch" to this String.
    \param count
        The number of code unit copies to append.
    \param ch
        The code unit to copy and append.
    \return
        Returns a reference to this String (*this).
    */
    String& append(size_type count, value_type ch)
    {
        d_string.append(count, ch);
        return *this;
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Appends "count" number of copies of code unit
        "ch" to this String.
    \param count
        The number of code unit copies to append.
    \param ch
        The code unit to copy and append.
    \return
        Returns a reference to this String (*this).
    */
    String& append(size_type count, char32_t ch)
    {
        std::string convertedCharacter = convertUtf32ToUtf8(ch);
        for(size_t i = 0; i < count; ++i)
        {
            d_string.append(convertedCharacter);
        }
        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Appends "count" number of copies of code unit
        "ch" to this String.
    \param count
        The number of code unit copies to append.
    \param ch
        The code unit to copy and append.
    \return
        Returns a reference to this String (*this).
    */
    String& append(size_type count, char ch)
    {
        d_string.append(count, convertUtf8ToUtf32(ch)[0]);
        return *this;
    }
#endif

    /*!
    \brief
        Appends the String "str" to this String.
    \param str
        The String to append to this String.
    \return
        Returns a reference to this String (*this).
    */
    String& append(const String& str)
    {
        d_string.append(str.d_string);
        return *this;
    }

    /*!
    \brief
        Appends a basic_string of type value_type elements to this String.
    \param str
        The basic_string of type value_type to append to this String.
    \return
        Returns a reference to this String (*this).
    */
    String& append(const std::basic_string<value_type>& str)
    {
        d_string.append(str);
        return *this;
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Appends the std::u32string "str", containing UTF-32 encoded
        code units, to this String.
    \param str
        The std::string, which contains UTF-32 encoded
        code units, which will be appended to this String.
    \return
        Returns a reference to this String (*this).
    */
    String& append(const std::u32string& str)
    {
        d_string.append(convertUtf32ToUtf8(str));
        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Appends the std::string "str", containing either UTF-8 encoded or
        ASCII code units, to this String.
    \param str
        The std::string, which contains either UTF-8 encoded or ASCII
        code units, which will be appended to this String.
    \return
        Returns a reference to this String (*this).
    */
    String& append(const std::string& str)
    {
        d_string.append(convertUtf8ToUtf32(str));
        return *this;
    }
#endif

    /*!
    \brief
        Appends a substring of the supplied String "str". The substring has dimensions [pos, pos+count) and
        is appended to this String.
    \param str
        The String from which the substring will be taken.
    \param pos
        The starting position for the substring.
    \param count
        The length of the substring.
    \return
        Returns a reference to this String (*this).
    */
    String& append(const String& str, size_type pos, size_type count = npos)
    {
        d_string.append(str.d_string, pos, count);
        return *this;
    }

    /*!
    \brief
        Appends a substring of a supplied std::basic_string of type
        value_type. The substring has dimensions [pos, pos+count) and
        is appended to this String.
    \param str
        The std::basic_string of type value_type from which the
        substring will be taken.
    \param pos
        The starting position for the substring.
    \param count
        The length of the substring.
    \return
        Returns a reference to this String (*this).
    */
    String& append(const std::basic_string<value_type>& str, size_type pos, size_type count = npos)
    {
        d_string.append(str, pos, count);
        return *this;
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Appends a substring of the supplied std::string "str",
        which contains UTF-32 encoded code units. The substring
        has dimensions [pos, pos+count) and is appended to this
        String.
    \param str
        The std::string, containing UTF-32 encoded code units,
        from which the substring will be taken.
    \param pos
        The starting position (code unit) for the substring.
    \param count
        The length of the substring measured in code units.
    \return
        Returns a reference to this String (*this).
    */
    String& append(const std::u32string& str, size_type pos, size_type count = npos)
    {
        std::string convertedString = convertUtf32ToUtf8(str.substr(pos, count));
        d_string.append(convertedString);
        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Appends a substring of the supplied std::string "str",
        which can contain either UTF-8 encoded code units or
        ASCII code units. The substring has dimensions
        [pos, pos+count) and is appended to this String.
    \param str
        The std::string, containing either UTF-8 encoded or
        ASCII code units, from which the substring will be taken.
    \param pos
        The starting position (code unit) for the substring.
    \param count
        The length of the substring measured in code units.
    \return
        Returns a reference to this String (*this).
    */
    String& append(const std::string& str, size_type pos, size_type count = npos)
    {
        std::u32string convertedString = convertUtf8ToUtf32(str.substr(pos, count));
        d_string.append(convertedString);
        return *this;
    }
#endif

    /*!
    \brief
        Appends "count" number of code units from the
        String::value_type array "charArray" to this
        String. The input array may contain null-characters.  
    \param charArray
        The String::value_type array containing the string to append.
    \param count
        The number of code units from the array to append.
    \return
        Returns a reference to this String (*this).
    */
    String& append(const value_type* charArray, size_type count)
    {
        d_string.append(charArray, count);
        return *this;
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Appends "count" number of code units from the char32_t
        array "charArray" to this String. The input array consists
        of UTF-32 encoded code units and may contain null-characters. 
    \param charArray
        The char32_t array containing the string to append.
    \param count
        The number of code units from the array to append.
    \return
        Returns a reference to this String (*this).
    */
    String& append(const char32_t* charArray, size_type count)
    {
        d_string.append(convertUtf32ToUtf8(charArray, charArray + count));
        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Appends "count" number of code units from the
        char array "charArray" to this String. The input array
        can consist of UTF-8 encoded and ASCII code units.
        The input array may contain null-characters. 
    \param charArray
        The char array containing the string to append.
    \param count
        The number of code units from the array to append.
    \return
        Returns a reference to this String (*this).
    */
    String& append(const char* charArray, size_type count)
    {
        d_string.append(convertUtf8ToUtf32(charArray, charArray + count));
        return *this;
    }
#endif

    /*!
    \brief
        Appends the null-terminated String::value_type array
        "charArray" to this String. The length of the appended string
        is determined by the first null-character that is encountered.
    \param charArray
        The String::value_type array containing the string to append.
    \return
        Returns a reference to this String (*this).
    */
    String& append(const value_type* charArray)
    {
        d_string.append(charArray);
        return *this;
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Appends a null-terminated char32_t array to this
        String. The array may contain either UTF-32 encoded
        code units. The length of the appended string is determined
        by the first null-character that is encountered.
    \param charArray
        The char array containing the string to append. It contains
        UTF-32 code units.
    \return
        Returns a reference to this String (*this).
    */
    String& append(const char32_t* charArray)
    {
        d_string.append(convertUtf32ToUtf8(charArray));
        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Appends the null-terminated char array "charArray" to this
        String. The array may contain either UTF-8 encoded or ASCII
        code units. The length of the appended string is determined
        by the first null-character that is encountered.
    \param charArray
        The char array containing the string to append. It may 
        contain either UTF-8 encoded or ASCII code units.
    \return
        Returns a reference to this String (*this).
    */
    String& append(const char* charArray)
    {
        d_string.append(convertUtf8ToUtf32(charArray));
        return *this;
    }
#endif

    /*!
    \brief
        Appends code units in the range [first, last) to this String. 
    \param first
        Beginning of the range of code units to append.
    \param first
        End of the range of code units to append.
    \return
        Returns a reference to this String (*this).
    */
    template<class InputIt>
    String& append(InputIt first, InputIt last)
    {
        d_string.append(first, last);
        return *this;
    }

    /*!
    \brief
        Appends the code units in the initializer_list
        "initialiserList" to this String.
    \param initialiserList
        The initializer_list containing the code units to append.
    \return
        Returns a reference to this String (*this).
    */
    String& append(std::initializer_list<String::value_type> initialiserList)
    {
        d_string.append(initialiserList);
        return *this;
    }

    /*!
    \brief
        Appends the String "str" to this String.
    \param str
        The String to append to this String.
    \return
        Returns a reference to this String (*this).
    */
    String& operator+=(const String& str)
    {
        return append(str);
    }

    /*!
    \brief
        Appends a basic_string containing String::value_type
        elements to this String.
    \param str
        The basic_string to append to this String.
    \return
        Returns a reference to this String (*this).
    */
    String& operator+=(const std::basic_string<String::value_type>& str)
    {
        return append(str);
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Appends a string, which contains UTF-32
        encoded or ASCII code units,
        to this String.
    \param str
        The string, which contains UTF-32
        encoded code units, to append to
        this String.
    \return
        Returns a reference to this String (*this).
    */
    String& operator+=(const std::u32string& str)
    {
        return append(str);
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Appends a string, which may contain
        either UTF-8 encoded or ASCII code units,
        to this String.
    \param str
        The string, which may contain either UTF-8
        encoded or ASCII code units, to append to
        this String.
    \return
        Returns a reference to this String (*this).
    */
    String& operator+=(const std::string& str)
    {
        return append(str);
    }
#endif

    /*!
    \brief
        Appends the code unit "ch" to this String.
    \param ch
        The code unit to append to this String.
    \return
        Returns a reference to this String (*this).
    */
    String& operator+=(String::value_type ch)
    {
        return append(1, ch);
    }


    /*!
    \brief
        Appends the code unit "ch" to this String.
    \param ch
        The code unit to append to this String.
    \return
        Returns a reference to this String (*this).
    */
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    String& operator+=(char32_t ch)
    {
        return append(1, ch);
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    String& operator+=(char ch)
    {
        return append(1, ch);
    }
#endif

    /*!
    \brief
        Appends the null-terminated code unit arra
        "charArray" to this String.
    \param ch
        The null-terminated code unit array to append t
        this String.
    \return
        Returns a reference to this String (*this).
    */
    String& operator+=(const String::value_type* charArray)
    {
        return append(charArray);
    }

    /*!
    \brief
        Appends the null-terminated code unit array
        "charArray" to this String.
    \param ch
        The null-terminated code unit array to append
        to this String.
    \return
        Returns a reference to this String (*this).
    */
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    String& operator+=(const char32_t* charArray)
    {
        return append(charArray);
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    String& operator+=(const char* charArray)
    {
        return append(charArray);
    }
#endif

    /*!
    \brief
        Appends the code units in the initializer_list
        "initialiserList" to this String.
    \param initialiserList
        The initializer_list to append to this String.
    \return
        Returns a reference to this String (*this).
    */
    String& operator+=(std::initializer_list<String::value_type> initialiserList)
    {
        return append(initialiserList);
    }

    /*!
    \brief
        Compares this String to the given other String.
    \param str
        The other String to compare this String with.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(const String& str) const
    {
        return d_string.compare(str.d_string);
    }

    /*!
    \brief
        Compares this String to the given basic_string.
    \param str
        The basic_string to compare this String with.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(const std::basic_string<value_type>& str) const
    {
        return d_string.compare(str);
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Compares this String to the given string containing
        UTF-32 encoded code units.
    \param str
        The string, containing UTF-32 encoded code units.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(const std::u32string& str) const
    {
        return d_string.compare(convertUtf32ToUtf8(str));
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Compares this String to the given string containing either UTF-8 encoded
        or ASCII code units.
    \param str
        The string, containing either UTF-8 encoded or ASCII code units, to compare
        this String with.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(const std::string& str) const
    {
        return d_string.compare(convertUtf8ToUtf32(str));
    }
#endif

    /*!
    \brief
        Compares a substring [pos1, pos1+count1) of this String to the given string "str".
    \param pos1
        The position of the first code unit of this String to compare.
    \param count1
        The number of code units of this String to compare.
    \param str
        The String to compare this String with.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(size_type pos1, size_type count1, const String& str) const
    {
        return d_string.compare(pos1, count1, str.d_string);
    }

    /*!
    \brief
        Compares a substring [pos1, pos1+count1) of this String to the given string "str".
    \param pos1
        The position of the first code unit of this String to compare.
    \param count1
        The number of code units of this String to compare.
    \param str
        The u32string to compare this String with.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(size_type pos1, size_type count1,
        const std::basic_string<value_type>& str) const
    {
        return d_string.compare(pos1, count1, str);
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Compares a substring [pos1, pos1+count1) of this
        String to the given string "str", which contains
        UTF-32 encoded code units.
    \param pos1
        The position of the first code unit of this String to
        compare.
    \param count1
        The number of code units of this String to compare.
    \param str
        The string, which contains UTF-32 encoded code units,
        to compare this String with.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(size_type pos1, size_type count1, const std::u32string& str) const
    {
        return d_string.compare(pos1, count1, convertUtf32ToUtf8(str));
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Compares a substring [pos1, pos1+count1) of this
        String to the given string "str", which may
        contain either UTF-8 encoded or ASCII code units.
    \param pos1
        The position of the first code unit of this String to compare.
    \param count1
        The number of code units of this String to compare.
    \param str
        The string, containing either UTF-8 encoded or ASCII
        code units, to compare this String with.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(size_type pos1, size_type count1, const std::string& str) const
    {
        return d_string.compare(pos1, count1, convertUtf8ToUtf32(str));
    }
#endif

    /*!
    \brief
        Compares a substring [pos1, pos1+count1) of this
        String to the given String "str".
    \param pos1
        The position of the first code unit of this String to
        compare.
    \param count1
        The number of code units of this String to compare.
    \param str
        The String to compare this String with.
    \param pos2
        The position of the first code unit of the given String
        to compare.
    \param count2
        The number of code units of the given String to compare.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(size_type pos1, size_type count1, const String& str,
                size_type pos2, size_type count2 = npos) const
    {
        return d_string.compare(pos1, count1, str.d_string,
                                pos2, count2);
    }

    /*!
    \brief
        Compares a substring [pos1, pos1+count1) of this String to the given u32string "str".
    \param pos1
        The position of the first code unit of this String to compare.
    \param count1
        The number of code units of this String to compare.
    \param str
        The u32string to compare this String with.
    \param pos2
        The position of the first code unit of the given u32string to compare.
    \param count2
        The number of code units of the given u32string to compare.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(size_type pos1, size_type count1,
                const std::basic_string<value_type>& str,
                size_type pos2, size_type count2 = npos) const
    {
        return d_string.compare(pos1, count1, str,
                                pos2, count2);
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Compares a substring [pos1, pos1+count1) of this
        String to the given string "str", which may
        contain either UTF-32 encoded or ASCII code units.
    \param pos1
        The position of the first code unit of this String to compare.
    \param count1
        The number of code units of this String to compare.
    \param str
        The string, which contains UTF-32 encoded code units,
        to compare this String with.
    \param pos2
        The position of the first code unit of the given std::u32string
        to compare.
    \param count2
        The number of code units of the given str::u32string to compare.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(size_type pos1, size_type count1, const std::u32string& str,
                size_type pos2, size_type count2 = npos) const
    {
        std::string convertedString = convertUtf32ToUtf8(str.substr(pos2, count2));
        return d_string.compare(pos1, count1, convertedString,
                                0, convertedString.length());
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Compares a substring [pos1, pos1+count1) of this
        String to the given string "str", which may
        contain either UTF-8 encoded or ASCII code units.
    \param pos1
        The position of the first code unit of this String to compare.
    \param count1
        The number of code units of this String to compare.
    \param str
        The string, containing either UTF-8 encoded or
        ASCII code units, to compare this String with.
    \param pos2
        The position of the first code unit of the given std::string
        to compare.
    \param count2
        The number of code units of the given std::string to compare.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(size_type pos1, size_type count1, const std::string& str,
                size_type pos2, size_type count2 = npos) const
    {
        std::u32string convertedString = convertUtf8ToUtf32(str.substr(pos2, count2));
        return d_string.compare(pos1, count1, convertedString,
                                0, convertedString.length());
    }
#endif

    /*!
    \brief
        Compares this String to a string stored in
        the null-terminated code unit array "charArray"
    \param charArray
        Null-terminated code unit array to compare this String with.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(const String::value_type* charArray) const
    {
        return d_string.compare(charArray);
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Compares this String to the string stored in the
        null-terminated code unit array "charArray",
        containing UTF-32 encoded code units.
    \param charArray
        Null-terminated code unit array to compare this String with.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(const char32_t* charArray) const
    {
        return d_string.compare(convertUtf32ToUtf8(charArray).data());
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Compares this String to the string stored in the
        null-terminated code unit array "charArray",
        containing either UTF-8 encoded or ASCII code units.
    \param charArray
        Null-terminated code unit array to compare this String with.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(const char* charArray) const
    {
        return d_string.compare(convertUtf8ToUtf32(charArray).data());
    }
#endif

    /*!
    \brief
        Compares this String to the string stored in the
        null-terminated code unit array "charArray".
    \param pos1
        The position of the first code unit of this String to compare.
    \param count1
        The number of code units of this String to compare.
    \param charArray
        Null-terminated code unit array to compare this String with.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(size_type pos1, size_type count1,
                const String::value_type* charArray) const
    {
        return d_string.compare(pos1, count1, charArray);
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Compares this String to the string stored in the
        null-terminated code unit array "charArray", containing
        UTF-32 encoded code units.
    \param pos1
        The position of the first code unit of this String to compare.
    \param count1
        The number of code units of this String to compare.
    \param charArray
        Null-terminated code unit array to compare this String with.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(size_type pos1, size_type count1,
                const char32_t* charArray) const
    {
        return d_string.compare(pos1, count1,
                                convertUtf32ToUtf8(charArray).data());
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Compares this String to the string stored in the
        null-terminated code unit array "charArray", containing
        either UTF-8 encoded or ASCII code units.
    \param pos1
        The position of the first code unit of this String to compare.
    \param count1
        The number of code units of this String to compare.
    \param charArray
        Null-terminated code unit array to compare this String with.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(size_type pos1, size_type count1, const char* charArray) const
    {
        return d_string.compare(pos1, count1,
                                convertUtf8ToUtf32(charArray).data());
    }
#endif

    /*!
    \brief
        Compares this String to the string stored in the
        null-terminated code unit array "charArray".
    \param pos1
        The position of the first code unit of this String to compare.
    \param count1
        The number of code units of this String to compare.
    \param charArray
        Null-terminated code unit array to compare this String with.
    \param count2
        The number of code units of the given ch String to compare.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(size_type pos1, size_type count1,
                const String::value_type* charArray, size_type count2) const
    {
        return d_string.compare(pos1, count1, charArray, count2);
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Compares this String to the string stored in the
        null-terminated code unit array "charArray", containing
        UTF-32 encoded code units.
    \param pos1
        The position of the first code unit of this String to compare.
    \param count1
        The number of code units of this String to compare.
    \param charArray
        Null-terminated code unit array to compare this String with.
    \param count2
        The number of code units of the given charArray string to compare.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(size_type pos1, size_type count1,
                const char32_t* charArray, size_type count2) const
    {
        std::string convertedString = convertUtf32ToUtf8(charArray, count2);
        return d_string.compare(pos1, count1,
                                convertedString.c_str(), convertedString.length());
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Compares this String to the string stored in the
        null-terminated code unit array "charArray", containing
        either UTF-32 encoded code units.
    \param pos1
        The position of the first code unit of this String to compare.
    \param count1
        The number of code units of this String to compare.
    \param charArray
        Null-terminated code unit array to compare this String with.
    \param count2
        The number of code units of the given charArray string to compare.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(size_type pos1, size_type count1,
                const char* charArray, size_type count2) const
    {
        std::u32string convertedString = convertUtf8ToUtf32(charArray, count2);
        return d_string.compare(pos1, count1,
                                convertedString.c_str(), convertedString.length());
    }
#endif

    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with the supplied String.
    \param pos
        The position of the first code unit of this String to be replaced.
    \param count
        The length of code units of the substring of this String to replace.
    \param str
        The String to use for the replacement.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count, const String& str)
    {
        d_string.replace(pos, count, str.d_string);
        return *this;
    }

    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with the supplied u32string.
    \param pos
        The position of the first code unit of this String to be replaced.
    \param count
        The length of code units of the substring of this String to replace.
    \param str
        The u32string to use for the replacement.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count,
                    const std::basic_string<value_type>& str)
    {
        d_string.replace(pos, count, str);
        return *this;
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with the supplied string,
        containing UTF-32 encoded code units.
    \param pos
        The position of the first code unit of this String
        to be replaced.
    \param count
        The length of code units of the substring of this
        String to replace.
    \param str
        The string, containing UTF-32 encoded code units,
        to use for the replacement.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count,
                    const std::u32string& str)
    {
        d_string.replace(pos, count, convertUtf32ToUtf8(str));
        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with the supplied string,
        containing either UTF-8 encoded or ASCII code units.
    \param pos
        The position of the first code unit of this String to be replaced.
    \param count
        The length of code units of the substring of this String to replace.
    \param str
        The string, containing either UTF-8 encoded or ASCII
        code units, to use for the replacement.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count, const std::string& str)
    {
        d_string.replace(pos, count, convertUtf8ToUtf32(str));
        return *this;
    }
#endif

    /*!
    \brief
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with the supplied String.
    \param first
        The beginning of the range of the code units of this String to be replaced.
    \param last
        The end of the range of the code units of this String to be replaced.
    \param str
        The String to use for the replacement.
    \return
        Returns a reference to this String (*this).
    */
#if defined (CEGUI_STRING_CPP_11)
    String& replace(const_iterator first, const_iterator last, const String& str)
#else
    String& replace(iterator first, iterator last, const String& str)
#endif
    {
        d_string.replace(first, last, str.d_string);
        return *this;
    }

    /*!
    \brief
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with the supplied u32string.
    \param first
        The beginning of the range of the code units of this String to be replaced.
    \param last
        The end of the range of the code units of this String to be replaced.
    \param str
        The u32string to use for the replacement.
    \return
        Returns a reference to this String (*this).
    */
#if defined (CEGUI_STRING_CPP_11)
    String& replace(const_iterator first, const_iterator last,
                    const std::basic_string<value_type>& str)
#else
    String& replace(iterator first, iterator last,
                    const std::basic_string<value_type>& str)
#endif
    {
        d_string.replace(first, last, str);
        return *this;
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with the supplied string,
        containing UTF-32 encoded code units.
    \param first
        The beginning of the range of the code units of this String
        to be replaced.
    \param last
        The end of the range of the code units of this String to be
        replaced.
    \param str
        The string, containing UTF-32 encoded code
        units, to use for the replacement.
    \return
        Returns a reference to this String (*this).
    */
    #if defined (CEGUI_STRING_CPP_11)
    String& replace(const_iterator first, const_iterator last, const std::u32string& str)
    #else 
    String& replace(iterator first, iterator last, const std::u32string& str)
    #endif
    {
        d_string.replace(first, last, convertUtf32ToUtf8(str));
        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with the supplied string,
        containing either UTF-8 encoded or ASCII code units.
    \param first
        The beginning of the range of the code units of this String
        to be replaced.
    \param last
        The end of the range of the code units of this String to be
        replaced.
    \param str
        The string, containing either UTF-8 encoded or ASCII code
        units, to use for the replacement.
    \return
        Returns a reference to this String (*this).
    */
    #if defined (CEGUI_STRING_CPP_11)
    String& replace(const_iterator first, const_iterator last, const std::string& str)
    #else 
    String& replace(iterator first, iterator last, const std::string& str)
    #endif
    {
        d_string.replace(first, last, convertUtf8ToUtf32(str));
        return *this;
    }
#endif

    /*!
    \brief
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with the supplied String.
    \param pos
        The position of the first code unit of this String to be replaced.
    \param count
        The length of code units of the substring of this String to replace.
    \param str
        The String to use for the replacement.
    \param pos2
        The starting position of the substring to be used to replace the code units of this String with.
    \param count2
        The number of code units of the substring to be used to replace the code units of this String with.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count, const String& str,
                    size_type pos2, size_type count2 = npos)
    {
        d_string.replace(pos, count, str.d_string, pos2, count2);
        return *this;
    }

    /*!
    \brief
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with the supplied u32string.
    \param pos
        The position of the first code unit of this String to be replaced.
    \param count
        The length of code units of the substring of this String to replace.
    \param str
        The u32string to use for the replacement.
    \param pos2
        The starting position of the substring to be used to replace the code units of this String with.
    \param count2
        The number of code units of the substring to be used to replace the code units of this String with.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count, const std::basic_string<value_type>& str,
                    size_type pos2, size_type count2 = npos)
    {
        d_string.replace(pos, count, str, pos2, count2);
        return *this;
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with the supplied string,
        containing UTF-32 encoded code units.
    \param pos
        The position of the first code unit of this String to be replaced.
    \param count
        The length of code units of the substring of this String to replace.
    \param str
        The string, containing UTF-32 encoded code units,
        to use for the replacement.
    \param pos2
        The starting position in code units of the substring to be
        used to replace the code units of this String with.
    \param count2
        The number of code units of the substring to be used to replace
        the code units of this String with.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count, const std::u32string& str,
                    size_type pos2, size_type count2 = npos)
    {
        std::string convertedString = convertUtf32ToUtf8(str.substr(pos2, count2));
        d_string.replace(pos, count, convertedString,
                         0, convertedString.length());
        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with the supplied string,
        containing either UTF-8 encoded or ASCII code units.
    \param pos
        The position of the first code unit of this String to be replaced.
    \param count
        The length of code units of the substring of this String to replace.
    \param str
        The string, containing either UTF-8 encoded or ASCII code units,
        to use for the replacement.
    \param pos2
        The starting position in code units of the substring to be
        used to replace the code units of this String with.
    \param count2
        The number of code units of the substring to be used to replace
        the code units of this String with.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count, const std::string& str,
                    size_type pos2, size_type count2 = npos)
    {
        std::u32string convertedString = convertUtf8ToUtf32(str.substr(pos2, count2));
        d_string.replace(pos, count, convertedString,
                         0, convertedString.length());
        return *this;
    }
#endif

    /*!
    \brief
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with the supplied String
        in the range [first2, last2).
    \param first
        The beginning of the range of the code units of this String to be replaced.
    \param last
        The end of the range of the code units of this String to be replaced.
    \param first2
        The beginning of the range of the code units of the given replacement String to be replaced.
    \param last2
        The end of the range of the code units of of the given replacement String to be replaced.
    \return
        Returns a reference to this String (*this).
    */
    template<class InputIt>
    String& replace(const_iterator first, const_iterator last,
                    InputIt first2, InputIt last2)
    {
        d_string.replace(first, last, first2, last2);
        return *this;
    }

    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with "count2" number of code units
        of the string stored in the supplied code unit array.
    \param pos
        The position of the first code unit of this String to be replaced.
    \param count
        The length of code units of the substring of this String to replace.
    \param charArray
        The array of code units to use for the replacement.
    \param count2
        The number of code units of the substring to be used to
        replace the code units of this String with.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count,
                    const value_type* charArray, size_type count2)
    {
        d_string.replace(pos, count, charArray, count2);
        return *this;
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with "count2"
        number of code units
        of the string stored in the supplied code unit array.
        The code unit array contains UTF-32 encoded
        code units. The input array will only be
        processed until the first null-character.
    \param pos
        The position of the first code unit of this String to
        be replaced.
    \param count
        The length of code units of the substring of this String
        to replace.
    \param charArray
        The array of code units, containing UTF-32 encoded
        code units, to use for the replacement.
    \param count2
        The number of code units of the substring to be used to
        replace the code units of this String with.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count,
                    const char32_t* charArray, size_type count2)
    {
        std::string convertedString = convertUtf32ToUtf8(charArray, charArray + count2);
        d_string.replace(pos, count, convertedString.data(),
                         convertedString.length());
        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with "count2"
        number of code units
        of the string stored in the supplied code unit array.
        The code unit array may contain either UTF-8 encoded
        or ASCII code units. The input array will only be
        processed until the first null-character.
    \param pos
        The position of the first code unit of this String to
        be replaced.
    \param count
        The length of code units of the substring of this String
        to replace.
    \param charArray
        The array of code units, containing either UTF-8 encoded
        or ASCII code units, to use for the replacement.
    \param count2
        The number of code units of the substring to be used to
        replace the code units of this String with.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count,
                    const char* charArray, size_type count2)
    {
        std::u32string convertedString = convertUtf8ToUtf32(charArray, charArray + count2);
        d_string.replace(pos, count, convertedString.data(),
                         convertedString.length());
        return *this;
    }
#endif

    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with "count2"
        number of code units
        of the string stored in the supplied code unit array.
    \param first
        The beginning of the range of the code units of this
        String to be replaced.
    \param last
        The end of the range of the code units of this Strin
        to be replaced.
    \param charArray
        The array of code units to use for the replacement.
    \param count2
        The number of code units of the substring to be used
        to replace the code units of this String with.
    \return
        Returns a reference to this String (*this).
    */
    #if defined (CEGUI_STRING_CPP_11)
    String& replace(const_iterator first, const_iterator last,
                    const value_type* charArray, size_type count2)
    #else
    String& replace(iterator first, iterator last,
                    const value_type* charArray, size_type count2)
    #endif
    {
        d_string.replace(first, last, charArray, count2);
        return *this;
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with "count2"
        number of code units
        of the string stored in the supplied code unit array.
        The code unit array may contain either UTF-32 encoded
        code units. The input array will only be
        processed until the first null-character.
    \param first
        The beginning of the range of the code units of this
        String to be replaced.
    \param last
        The end of the range of the code units of this String to
        be replaced.
    \param charArray
        The array of code units to use for the replacement.
        The code unit array contains UTF-32 encoded
        or ASCII code units.
    \param count2
        The number of code units of the substring to be used
        to replace the code units of this String with.
    \return
        Returns a reference to this String (*this).
    */
    #if defined (CEGUI_STRING_CPP_11)
    String& replace(const_iterator first, const_iterator last,
                    const char32_t* charArray, size_type count2)
    #else
    String& replace(iterator first, iterator last,
                    const char32_t* charArray, size_type count2)
    #endif
    {
        std::string convertedString = convertUtf32ToUtf8(charArray, charArray + count2);
        d_string.replace(first, last,
                         convertedString.data(), convertedString.length());
        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with "count2"
        number of code units
        of the string stored in the supplied code unit array.
        The code unit array may contain either UTF-8 encoded
        or ASCII code units. The input array will only be
        processed until the first null-character.
    \param first
        The beginning of the range of the code units of this
        String to be replaced.
    \param last
        The end of the range of the code units of this String to
        be replaced.
    \param charArray
        The array of code units to use for the replacement.
        The code unit array may contain either UTF-8 encoded
        or ASCII code units.
    \param count2
        The number of code units of the substring to be used
        to replace the code units of this String with.
    \return
        Returns a reference to this String (*this).
    */
    #if defined (CEGUI_STRING_CPP_11)
    String& replace(const_iterator first, const_iterator last,
                    const char* charArray, size_type count2)
    #else
    String& replace(iterator first, iterator last,
                    const char* charArray, size_type count2)
    #endif
    {
        std::u32string convertedString = convertUtf8ToUtf32(charArray, charArray + count2);
        d_string.replace(first, last,
                         convertedString.data(), convertedString.length());
        return *this;
    }
#endif

    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with the code units
        of the string stored in the supplied code unit array.
        The code units of the array up to the
        first null-character are used for the replacement
    \param pos
        The position of the first code unit of this String to be replaced.
    \param count
        The length of code units of the substring of this String to replace.
    \param charArray
        The null-terminated array of code units to use for the replacement.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count,
                    const value_type* charArray)
    {
        d_string.replace(pos, count, charArray);
        return *this;
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with the code units
        of the string stored in the supplied code unit array.
        The code units of the array up to the
        first null-character are used for the replacement.
        The code unit array may contain either UTF-32 encoded
        code units.
    \param pos
        The position of the first code unit of this String to
        be replaced.
    \param count
        The length of code units of the substring of this String
        to replace.
    \param charArray
        The null-terminated array of code units to use for the
        replacement. The code unit array may
        contain either UTF-32 encoded code units.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count,
                    const char32_t* charArray)
    {
        std::string convertedString = convertUtf32ToUtf8(charArray);
        d_string.replace(pos, count, convertedString.data());
        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with the code units
        of the string stored in the supplied code unit array.
        The code units of the array up to the
        first null-character are used for the replacement.
        The code unit array may contain either UTF-8 encoded
        or ASCII code units.
    \param pos
        The position of the first code unit of this String to
        be replaced.
    \param count
        The length of code units of the substring of this String
        to replace.
    \param charArray
        The null-terminated array of code units to use for the
        replacement. The code unit array may
        contain either UTF-8 encoded or ASCII code units.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count,
                    const char* charArray)
    {
        std::u32string convertedString = convertUtf8ToUtf32(charArray);
        d_string.replace(pos, count, convertedString.data());
        return *this;
    }
#endif

    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with the code units
        of the string stored in the supplied code unit array.
        The code units of the array up to the
        first null-character are used for the replacement.
    \param first
        The beginning of the range of the code units of this
        String to be replaced.
    \param last
        The end of the range of the code units of this String
        to be replaced.
    \param charArray
        The null-terminated array of code units to use for
        the replacement.
    \return
        Returns a reference to this String (*this).
    */
#if defined (CEGUI_STRING_CPP_11)
    String& replace(const_iterator first, const_iterator last,
                    const value_type* charArray)
#else
    String& replace(iterator first, iterator last,
                    const value_type* charArray)
#endif
    {
        d_string.replace(first, last, charArray);
        return *this;
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with the code units
        of the string stored in the supplied code unit array.
        The code units of the array up to the
        first null-character are used for the replacement.
        The code unit array may contain either UTF-8 encoded
        or ASCII code units.
    \param first
        The beginning of the range of the code units of
        this String to be replaced.
    \param last
        The end of the range of the code units of this
        String to be replaced.
    \param charArray
        The null-terminated array of code units to use
        for the replacement. The code unit array may contain
        either UTF-8 encoded or ASCII code units.
    \return
        Returns a reference to this String (*this).
    */
    #if defined (CEGUI_STRING_CPP_11)
    String& replace(const_iterator first, const_iterator last,
                    const char32_t* charArray)
    #else 
    String& replace(iterator first, iterator last,
                    const char32_t* charArray)
    #endif
    {
        std::string convertedString = convertUtf32ToUtf8(charArray);
        d_string.replace(first, last, convertedString.data());
        return *this;
    }
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with the code units
        of the string stored in the supplied code unit array.
        The code units of the array up to the
        first null-character are used for the replacement.
        The code unit array may contain either UTF-8 encoded
        or ASCII code units.
    \param first
        The beginning of the range of the code units of
        this String to be replaced.
    \param last
        The end of the range of the code units of this
        String to be replaced.
    \param charArray
        The null-terminated array of code units to use
        for the replacement. The code unit array may contain
        either UTF-8 encoded or ASCII code units.
    \return
        Returns a reference to this String (*this).
    */
    #if defined (CEGUI_STRING_CPP_11)
    String& replace(const_iterator first, const_iterator last,
                    const char* charArray)
    #else 
    String& replace(iterator first, iterator last,
                    const char* charArray)
    #endif
    {
        std::u32string convertedString = convertUtf8ToUtf32(charArray);
        d_string.replace(first, last, convertedString.data());
        return *this;
    }
#endif

    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with "count2" number
        of copies of the supplied code unit "ch".
    \param pos
        The position of the first code unit of this String to
        be replaced.
    \param count
        The length of code units of the substring of this
        String to replace.
    \param count2
        The number of copies of code unit "ch" that should
        be used for the replacement.
    \param ch
        The code unit to be copied for the replacement.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count,
                    size_type count2, value_type ch)
    {
        d_string.replace(pos, count, count2, ch);
        return *this;
    }

    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with "count2" number
        of copies of the supplied code unit "ch".
    \param pos
        The position of the first code unit of this String
        to be replaced.
    \param count
        The length of code units of the substring of this
        String to replace.
    \param count2
        The number of copies of code unit "ch" that should
        be used for the replacement.
    \param ch
        The code unit to be copied for the replacement.
    \return
        Returns a reference to this String (*this).
    */
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    String& replace(size_type pos, size_type count,
                    size_type count2, char ch)
    {
        d_string.replace(pos, count, count2, convertUtf8ToUtf32(ch)[0]);
        return *this;
    }
#endif

    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with "count2" number
        of copies of the supplied code unit "ch".
    \param first
        The beginning of the range of the code units of this
        String to be replaced.
    \param last
        The end of the range of the code units of this String
        to be replaced.
    \param count2
        The number of copies of code unit "ch" that should be
        used for the replacement.
    \param ch
        The code unit to be copied for the replacement.
    \return
        Returns a reference to this String (*this).
    */
    #if defined (CEGUI_STRING_CPP_11)
    String& replace(const_iterator first, const_iterator last,
                    size_type count2, value_type ch)
    #else
    String& replace(iterator first, iterator last,
                    size_type count2, value_type ch)
    #endif
    {
        d_string.replace(first, last, count2, ch);
        return *this;
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with "count2" number
        of copies of the supplied code unit "ch".
    \param first
        The beginning of the range of the code units of this
        String to be replaced.
    \param last
        The end of the range of the code units of this String
        to be replaced.
    \param count2
        The number of copies of code unit "ch" that should be
        used for the replacement.
    \param ch
        The code unit to be copied for the replacement.
    \return
        Returns a reference to this String (*this).
    */

    #if defined (CEGUI_STRING_CPP_11)
    String& replace(const_iterator first, const_iterator last,
                    size_type count2, char ch)
    #else
    String& replace(iterator first, iterator last,
                    size_type count2, char ch)
    #endif
    {
        d_string.replace(first, last, count2, convertUtf8ToUtf32(ch)[0]);
        return *this;
    }
#endif

    /*!
    \brief
        Replaces the part of this String defined by either
        [pos, pos + count) or [first, last) with the code units
        in the initializer_list "initialiserList"
    \param first
        The beginning of the range of the code units of
        this String to be replaced.
    \param last
        The end of the range of the code units of this
        String to be replaced.
    \param initialiserList
        The initializer_list with the code units to use
        for the replacement.
    \return
        Returns a reference to this String (*this).
    */
#if defined (CEGUI_STRING_CPP_11)
    String& replace(const_iterator first, const_iterator last,
                    std::initializer_list<value_type> initialiserList)
#else
    String& replace(iterator first, iterator last,
                    std::initializer_list<value_type> initialiserList)
#endif
    {
        d_string.replace(first, last, initialiserList);
        return *this;
    }

    /*
    \brief
        Returns a substring [pos, pos+count) of this String.
        If the requested substring is larger than the String,
        or if count == npos,
        the substring [pos, size()) is returned.
    \param pos
        Position of the first code unit of the substring.
    \param count
        The number of code units of the substring to return.
    \return
        A String containing the requested substring or an empty String if pos == size().
    */
    String substr(size_type pos = 0, size_type count = npos) const
    {
        return String(d_string.substr(pos, count));
    }

    /*
    \brief
        Copies a substring [pos, pos+count) of this String
        to the code unit array pointed to by "dest". If the
        requested substring
        is larger than the String, or if count == npos, the
        substring [pos, size()) is copied. The resulting
        String is not null-terminated (does not end with a
        null-character).
    \param dest
        The target code unit array to which the substring 
        will be copied.
    \param count
        The number of code units of the substring to return.
    \param pos
        The position of the first code unit of the substring
        to be copied.
    \return
        The number of code units copied.
    */
    size_type copy(value_type* dest, size_type count,
                   size_type pos = 0) const
    {
        return d_string.copy(dest, count, pos);
    }

    /*
    \brief
        Resizes the string, potentially adding "count"
        number of code units. If the current size is
        less than "count" then additional code units
        are appended. If the current size is greater
        than count, the string is reduced to fit the
        specified size based on the elements from the
        beginning of the String.
    \param count
        The number of code units to resize this String to.
    */
    void resize(size_type count)
    {
        return d_string.resize(count);
    }

    /*
    \brief
        Resizes the string, potentially adding "count"
        number copies of code unit "ch". If the current
        size is less than "count" then additional code
        units are appended. If the current size is
        greater than count, the string is reduced to fit
        the specified size based on the elements from
        the beginning of the String.
    \param count
        The number of code units to resize this String to.
    \param ch
        The code unit to make copies of if appending code
        units.
    */
    void resize(size_type count, value_type ch)
    {
        return d_string.resize(count, ch);
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    /*
    \brief
        Resizes the string, potentially adding "count"
        number copies of code unit "ch". If the current
        size is less than "count" then additional code
        units are appended. If the current size is
        greater than count, the string is reduced to fit
        the specified size based on the elements from the
        beginning of the String.
    \param count
        The number of code units to resize this String to.
    \param ch
        The code unit to make copies of if appending code
        units.
    */
    void resize(size_type count, char ch)
    {
        return d_string.resize(count, convertUtf8ToUtf32(ch)[0]);
    }
#endif

    /*
    \brief
        Exchanges the contents of this String with the
        contents of the String "other". All iterators
        and references may be invalidated.
    \param other
        The String to exchange the contents of this
        String with.
    */
    void swap(String& other)
    {
        d_string.swap(other.d_string);
    }

    /*
    \brief
        Finds the first substring that is equal to the
        given String. Search starts at position "pos".
    \param str
        The String to search for.
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first code unit
        of the found substring matching the String or 
        returns npos if no matching substring was found. 
    */
    size_type find(const String& str, size_type pos = 0) const
    {
        return d_string.find(str.d_string, pos);
    }

    /*
    \brief
        Finds the first substring that is equal to the
        given u32string. Search starts at position "pos".
    \param str
        The u32string to search for.
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first code unit of
        the found substring matching the String or returns
        npos if no matching substring was found. 
    */
    size_type find(const std::basic_string<value_type>& str, size_type pos = 0) const
    {
        return d_string.find(str, pos);
    }

    /*
    \brief
        Finds the first substring that is equal to the
        given code units in the array "charArray". Search
        starts at position "pos" of this String.
        The array may contain null-characters. The number
        of code units from the array to be compared are
        specified by "count".
    \param charArray
        The code unit array charArray to search for.
    \param pos
        The position at which the search will start.
    \param count
        The number of code units of the array to use for
        comparison.
    \return
        Returns the position of the first code unit of
        the found substring matching the String or returns 
        npos if no matching substring was found. 
    */
    size_type find(const value_type* charArray, size_type pos, size_type count) const
    {
        return d_string.find(charArray, pos, count);
    }

    /*
    \brief
        Finds the first substring that is equal to the
        given code units in the array "charArray". Search
        starts at position "pos" of this String. The amount
        of code units to be compared from the code unit
        array is determined by the first null-character
        encountered
        in it. 
    \param charArray
        The code unit array to search for.
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first code unit of the
        found substring matching the String or returns npos
        if no matching substring was found. 
    */
    size_type find(const value_type* charArray, size_type pos = 0) const
    {
        return d_string.find(charArray, pos);
    }

    /*
    \brief
        Finds the first code unit "ch" in this String.
        Search starts at position "pos" of this String.
    \param ch
        The code unit to search for.
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first code unit of
        the found substring matching the String or
        returns npos if no matching substring was found. 
    */
    size_type find(value_type ch, size_type pos = 0) const
    {
        return d_string.find(ch, pos);
    }

    // rfind

    /*
    \brief
        Finds the last substring that is equal to
        the given String. Search starts at position
        "pos". If npos or any value equal or bigger
        than size() is passed as pos, the whole string
        is searched. 
    \param str
        The String to search for.
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first code unit
        of the found substring matching the String
        or returns npos if no matching substring was
        found. 
    */
    size_type rfind(const String& str, size_type pos = npos) const
    {
        return d_string.rfind(str.d_string, pos);
    }

    /*
    \brief
        Finds the first substring that is equal to
        the given string. Search starts at position
        "pos". If npos or any value equal or bigger
        than size() is passed as pos, the whole string
        is searched. 
    \param str
        The string to search for.
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first code unit
        of the found substring matching the String or
        returns npos if no matching substring was found. 
    */
    size_type rfind(const std::basic_string<value_type>& str, size_type pos = npos) const
    {
        return d_string.rfind(str, pos);
    }

    /*
    \brief
        Finds the first substring that is equal to
        the given code units in the array "charArray".
        Search starts at position "pos" of this String.
        The array may contain null-characters. The
        number of code units from the array to be compared
        are specified by "count". If npos or any value
        equal or bigger than size() is passed as pos,
        the whole string is searched.
    \param charArray
        The code unit array charArray to search for.
    \param pos
        The position at which the search will start.
    \param count
        The number of code units of the array to use for
        comparison.
    \return
        Returns the position of the first code unit of
        the found substring matching the String or returns
        npos if no matching substring was found. 
    */
    size_type rfind(const value_type* charArray, size_type pos, size_type count) const
    {
        return d_string.rfind(charArray, pos, count);
    }

    /*
    \brief
        Finds the first substring that is equal to the
        given code units in the array "charArray". Search
        starts at position "pos" of this String. The amount
        of code units to be compared from the code unit
        array is determined by the first null-character
        encountered in it. If npos or any value equal or
        bigger than size() is passed as pos, the whole
        string is searched.
    \param charArray
        The code unit array to search for.
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first code unit of the
        found substring matching the String or returns npos
        if no matching substring was found. 
    */
    size_type rfind(const value_type* charArray, size_type pos = npos) const
    {
        return d_string.rfind(charArray, pos);
    }

    /*
    \brief
        Finds the first code unit "ch" in this String.
        Search starts at position "pos" of this String.
        If npos or any value equal or bigger than size()
        is passed as pos, the whole string is searched.
    \param ch
        The code unit to search for.
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first code unit of
        the found substring matching the String or returns
        npos if no matching substring was found. 
    */
    size_type rfind(value_type ch, size_type pos = npos) const
    {
        return d_string.rfind(ch, pos);
    }

    // find_first_of

    /*
    \brief
        Finds the first code unit in this String that
        is equal to one of the code units in the given
        string. The search considers only the interval
        [pos, size()) of this String in the search.
    \param str
        The String containing the code units to search for. 
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first code unit found
        or returns npos if no matching code unit was found. 
    */
    size_type find_first_of(const String& str, size_type pos = 0) const
    {
        return d_string.find_first_of(str.d_string, pos);
    }

    /*
    \brief
        Finds the first code unit in this String that is
        equal to one of the code units in the given string.
        The search considers only the interval [pos, size())
        of this String in the search.
    \param str
        The String containing the code units to search for. 
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first code unit found
        or returns npos if no matching code unit was found. 
    */
    size_type find_first_of(const std::basic_string<value_type>& str, size_type pos = 0) const
    {
        return d_string.find_first_of(str, pos);
    }

    /*
    \brief
        Finds the first code unit in this String that is
        equal to one of the code units in the given string.
        The search considers only the interval [pos, size())
        of this String in the search. The length of the
        supplied string is determined by the first
        null-character that is encountered.
    \param charArray
        The code unit array containing the string to search
        for. 
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first code unit found
        or returns npos if no matching code unit was found. 
    */
    size_type find_first_of(const value_type* charArray, size_type pos = 0) const
    {
        return d_string.find_first_of(charArray, pos);
    }

    /*
    \brief
        Finds the first code unit in this String that is
        equal to the code unit "ch". The search considers
        only the interval [pos, size())
        of this String in the search.
    \param ch
        The code unit to search for. 
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first code unit found
        or returns npos if no matching code unit was found. 
    */
    size_type find_first_of(value_type ch, size_type pos = 0) const
    {
        return d_string.find_first_of(ch, pos);
    }


    // find_last_of

    /*
    \brief
        Finds the last code unit in this String that is
        equal to one of the code units in the given string.
        The search considers only the interval [0; pos]
        of this String in the search.
    \param str
        The String containing the code units to search for. 
    \param pos
        The position at which the search will stop.
    \return
        Returns the position of the last code unit found
        or returns npos if no matching code unit was found. 
    */
    size_type find_last_of(const String& str, size_type pos = npos) const
    {
        return d_string.find_last_of(str.d_string, pos);
    }

    /*
    \brief
        Finds the last code unit in this String that is
        equal to one of the code units in the given string.
        The search considers only the interval [0; pos]
        of this String in the search.
    \param str
        The String containing the code units to search for. 
    \param pos
        The position at which the search will stop.
    \return
        Returns the position of the last code unit found
        or returns npos if no matching code unit was found. 
    */
    size_type find_last_of(const std::basic_string<value_type>& str, size_type pos = npos) const
    {
        return d_string.find_last_of(str, pos);
    }

    /*
    \brief
        Finds the last code unit in this String that is
        equal to one of the code units in the given string.
        The search considers only the interval [0; pos]
        of this String in the search. The length of the
        supplied string is determined by the last
        null-character that is encountered.
    \param charArray
        The code unit array containing the string to search
        for. 
    \param pos
        The position at which the search will stop.
    \return
        Returns the position of the last code unit found or
        returns npos if no matching code unit was found. 
    */
    size_type find_last_of(const value_type* charArray, size_type pos = npos) const
    {
        return d_string.find_last_of(charArray, pos);
    }

    /*
    \brief
        Finds the last code unit in this String that 
        is equal to the code unit "ch". The search 
        considers only the interval [0; pos]
        of this String in the search.
    \param ch
        The code unit to search for. 
    \param pos
        The position at which the search will stop.
    \return
        Returns the position of the last code unit
        found or returns npos if no matching code 
        unit was found. 
    */
    size_type find_last_of(value_type ch, size_type pos = npos) const
    {
        return d_string.find_last_of(ch, pos);
    }

    // first_not_of

    /*
    \brief
        Finds the first code unit in this String that is not
        equal to any of the code units in the given string.
        The search considers only the interval [pos, size())
        of this String in the search.
    \param str
        The String containing the code units to search for. 
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first code unit found or
        returns npos if no matching code unit was found. 
    */
    size_type find_first_not_of(const String& str, size_type pos = 0) const
    {
        return d_string.find_first_not_of(str.d_string, pos);
    }

    /*
    \brief
        Finds the first code unit in this String that is
        not equal to any of the code units in the given
        string. The search considers only the interval
        [pos, size()) of this String in the search.
    \param str
        The String containing the code units to search for. 
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first code unit found
        or returns npos if no matching code unit was found. 
    */
    size_type find_first_not_of(const std::basic_string<value_type>& str, size_type pos = 0) const
    {
        return d_string.find_first_not_of(str, pos);
    }

    /*
    \brief
        Finds the first code unit in this String that is not
        equal to any of the code units in the given string. 
        The search considers only the interval [pos, size()) 
        of this String in the search. The length of the
        supplied string is determined by the first null-character
        that is encountered.
    \param charArray
        The code unit array containing the string to search for. 
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first code unit found 
        or returns npos if no matching code unit was found. 
    */
    size_type find_first_not_of(const value_type* charArray, size_type pos = 0) const
    {
        return d_string.find_first_not_of(charArray, pos);
    }

    /*
    \brief
        Finds the first code unit in this String that is not equal to the code unit "ch". The search considers only the interval [pos, size())
        of this String in the search.
    \param ch
        The code unit to search for. 
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first code unit found or returns npos if no matching code unit was found. 
    */
    size_type find_first_not_of(value_type ch, size_type pos = 0) const
    {
        return d_string.find_first_not_of(ch, pos);
    }

    // find_last_not_of

    /*
    \brief
        Finds the last code unit in this String that is
        not equal to any of the code units in the given
        string. The search considers only the interval
        [0; pos] of this String in the search.
    \param str
        The String containing the code units to search for. 
    \param pos
        The position at which the search will stop.
    \return
        Returns the position of the last code unit found
        or returns npos if no matching code unit was found. 
    */
    size_type find_last_not_of(const String& str, size_type pos = npos) const
    {
        return d_string.find_last_not_of(str.d_string, pos);
    }

    /*
    \brief
        Finds the last code unit in this String that is
        not equal to any of the code units in the given string.
        The search considers only the interval [0; pos] of this
        String in the search.
    \param str
        The String containing the code units to search for. 
    \param pos
        The position at which the search will stop.
    \return
        Returns the position of the last code unit found or
        returns npos if no matching code unit was found. 
    */
    size_type find_last_not_of(const std::basic_string<value_type>& str, size_type pos = npos) const
    {
        return d_string.find_last_not_of(str, pos);
    }

    /*
    \brief
        Finds the last code unit in this String that
        is not equal to any of the code units in the
        given string. The search considers only the
        interval [0; pos] of this String in
        the search. The length of the supplied string
        is determined by the last
        null-character that is encountered.
    \param charArray
        The code unit array containing the string to
        search for. 
    \param pos
        The position at which the search will stop.
    \return
        Returns the position of the last code unit
        found or returns npos if no matching code unit
        was found. 
    */
    size_type find_last_not_of(const value_type* charArray, size_type pos = npos) const
    {
        return d_string.find_last_not_of(charArray, pos);
    }

    /*
    \brief
        Finds the last code unit in this String that is
        not equal to the code unit "ch". The search
        considers only the interval [0; pos]
        of this String in the search.
    \param ch
        The code unit to search for. 
    \param pos
        The position at which the search will stop.
    \return
        Returns the position of the last code unit found
        or returns npos if no matching code unit was found. 
    */
    size_type find_last_not_of(value_type ch, size_type pos = npos) const
    {
        return d_string.find_last_not_of(ch, pos);
    }

    /*!
    \brief
        Returns the underlying std::basic_string that
        is used to store the code units for this
        String class.
    */
    const std::basic_string<value_type>& getString() const { return d_string; }
    std::basic_string<value_type>& getString() { return d_string; }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    /*
    \brief
        The iterator class for iteration of code points in an UTF-8 String. 
    */
    class CEGUIEXPORT codepoint_iterator : public std::iterator<std::bidirectional_iterator_tag, char32_t>
    {
    public:
        codepoint_iterator() {}
        explicit codepoint_iterator(const String::const_iterator& codeUnitIterator,
                                    const String::const_iterator& rangeStart,
                                    const String::const_iterator& rangeEnd)
            : m_iter(codeUnitIterator)
            , m_rangeStart(rangeStart)
            , m_rangeEnd(rangeEnd)
        {
            if (m_iter < m_rangeStart || m_iter > m_rangeEnd)
            {
                throw std::out_of_range(
                    "The String iterator supplied for the "
                    "code point iterator is outside the supplied "
                    "range");
            }
        }

        String::const_iterator getCodeUnitIterator() const
        {
            return m_iter;
        }

        size_t getCodeUnitIndexFromStart() const
        {
            return m_iter - m_rangeStart;
        }

        codepoint_iterator& increment(size_t count)
        {
            size_t codePointIndex = 0;
            while (codePointIndex < count)
            {
                ++(*this);
                ++codePointIndex;
            }

            return *this;
        }

        operator bool() const { return m_iter != m_rangeEnd; }

        bool isAtEnd() const
        {
            return m_iter == m_rangeEnd;
        }

        char32_t operator* () const
        {
            return getCodePointFromCodeUnits(m_iter, m_rangeEnd);
        }

        bool operator== (const codepoint_iterator& rhs) const;

        bool operator!= (const codepoint_iterator& rhs) const
        {
            return !(operator == (rhs));
        }

        codepoint_iterator& operator++ ()
        {
            incrementByOneCodePoint();
            
            return *this;
        }

        codepoint_iterator operator++ (int)
        {
            codepoint_iterator temp = *this;
            incrementByOneCodePoint();

            return temp;
        }

        codepoint_iterator& operator-- ()
        {
            decrementByOneCodePoint();

            return *this;
        }

        codepoint_iterator operator-- (int)
        {
            codepoint_iterator temp = *this;
            decrementByOneCodePoint();

            return temp;
        }

    private:
        String::const_iterator m_iter;
        String::const_iterator m_rangeStart;
        String::const_iterator m_rangeEnd;

        void incrementByOneCodePoint();
        void decrementByOneCodePoint();
    };
#endif


private:
    /*!
    \brief
        Performs UTF-8 based stream output on Strings.
    */
    friend CEGUIEXPORT std::basic_ostream<char>& operator<<(std::basic_ostream<char>& outputStream, const String& str);
    /*!
    \brief
        Performs UTF-8 based stream input on Strings.
    */
    friend CEGUIEXPORT std::basic_istream<char>& operator>>(std::basic_istream<char>& inputStream, String& str);

    //! The wrapped basic_string object holding the unicode encoded code units.
    std::basic_string<value_type> d_string;
};


//////////////////////////////////////////////////////////////////////////
// Concatenation operator functions
//////////////////////////////////////////////////////////////////////////
/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const String& lhs, const String& rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const std::string& lhs, const String& rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const String& lhs, const std::string& rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const std::u32string& lhs, const String& rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const String& lhs, const std::u32string& rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const char32_t* lhs, const String& rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const char* lhs, const String& rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(char32_t lhs, const String& rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(char lhs, const String& rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const String& lhs, const char32_t* rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const String& lhs, const char* rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const String& lhs, char32_t rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const String& lhs, char rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(String&& lhs, const String& rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(String&& lhs, const std::string& rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(String&& lhs, const std::u32string& rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const String& lhs, String&& rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const std::string& lhs, String&& rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const std::u32string& lhs, String&& rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(String&& lhs, String&& rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const char32_t* lhs, String&& rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const char* lhs, String&& rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(char32_t lhs, String&& rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(char lhs, String&& rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(String&& lhs, const char32_t* rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(String&& lhs, const char* rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(String&& lhs, char32_t rhs);

/*!
\brief
    Returns a String containing the code units from lhs with the code units from rhs appended. 
\param lhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\param rhs
    A String, a code unit, or a pointer to the first code unit in a null-terminated array of code units.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(String&& lhs, char rhs);



//////////////////////////////////////////////////////////////////////////
// Comparison operators
//////////////////////////////////////////////////////////////////////////
/*!
\brief
    Returns true if String \a str1 is equal to String \a str2
*/
CEGUIEXPORT bool operator==(const String& str1, const String& str2);

/*!
\brief
    Returns true if String \a str is equal to std::string \a std_str
*/
CEGUIEXPORT bool operator==(const String& str, const std::string& std_str);

/*!
\brief
    Returns true if String \a str is equal to std::u32string \a std_u32str
*/
CEGUIEXPORT bool operator==(const String& str, const std::u32string& std_u32str);

/*!
\brief
    Returns true if std::string \a std_str is equal to String \a str
*/
CEGUIEXPORT bool operator==(const std::string& std_str, const String& str);

/*!
\brief
    Returns true if std::u32string \a std_u32str is equal to String \a str
*/
CEGUIEXPORT bool operator==(const std::u32string& std_u32str, const String& str);


/*!
\brief
    Returns true if String \a str1 is equal to String \a str2
*/
CEGUIEXPORT bool operator!=(const String& str1, const String& str2);

/*!
\brief
    Returns true if String \a str is equal to std::string \a std_str
*/
CEGUIEXPORT bool operator!=(const String& str, const std::string& std_str);

/*!
\brief
    Returns true if String \a str is equal to std::u32string \a std_u32str
*/
CEGUIEXPORT bool operator!=(const String& str, const std::u32string& std_u32str);

/*!
\brief
    Returns true if std::string \a std_str is equal to String \a str
*/
CEGUIEXPORT bool operator!=(const std::string& std_str, const String& str);

/*!
\brief
    Returns true if std::u32string \a std_u32str is equal to String \a str
*/
CEGUIEXPORT bool operator!=(const std::u32string& std_u32str, const String& str);


/*!
\brief
    Returns true if String \a str1 is equal to String \a str2
*/
CEGUIEXPORT bool operator<(const String& str1, const String& str2);

/*!
\brief
    Returns true if String \a str is lexicographically smaller than std::string \a std_str
*/
CEGUIEXPORT bool operator<(const String& str, const std::string& std_str);

/*!
\brief
    Returns true if String \a str is lexicographically smaller than std::u32string \a std_u32str
*/
CEGUIEXPORT bool operator<(const String& str, const std::u32string& std_u32str);

/*!
\brief
    Returns true if std::string \a std_str is lexicographically smaller than String \a str
*/
CEGUIEXPORT bool operator<(const std::string& std_str, const String& str);

/*!
\brief
    Returns true if std::u32string \a std_u32str is lexicographically smaller than String \a str
*/
CEGUIEXPORT bool operator<(const std::u32string& std_u32str, const String& str);


/*!
\brief
    Returns true if String \a str1 is lexicographically larger than String \a str2
*/
CEGUIEXPORT bool operator>(const String& str1, const String& str2);

/*!
\brief
    Returns true if String \a str is lexicographically larger than std::string \a std_str
*/
CEGUIEXPORT bool operator>(const String& str, const std::string& std_str);

/*!
\brief
    Returns true if String \a str is lexicographically larger than std::u32string \a std_u32str
*/
CEGUIEXPORT bool operator>(const String& str, const std::u32string& std_u32str);

/*!
\brief
    Returns true if std::string \a std_str is lexicographically larger than String \a str
*/
CEGUIEXPORT bool operator>(const std::string& std_str, const String& str);

/*!
\brief
    Returns true if std::u32string \a std_u32str is lexicographically larger than String \a str
*/
CEGUIEXPORT bool operator>(const std::u32string& std_u32str, const String& str);


/*!
\brief
    Returns true if String \a str1 is lexicographically equal or smaller than String \a str2
*/
CEGUIEXPORT bool operator<=(const String& str1, const String& str2);

/*!
\brief
    Returns true if String \a str is lexicographically equal or smaller than std::string \a std_str
*/
CEGUIEXPORT bool operator<=(const String& str, const std::string& std_str);

/*!
\brief
    Returns true if String \a str is lexicographically equal or smaller than std::u32string \a std_u32str
*/
CEGUIEXPORT bool operator<=(const String& str, const std::u32string& std_u32str);

/*!
\brief
    Returns true if std::string \a std_str is lexicographically equal or smaller than String \a str
*/
CEGUIEXPORT bool operator<=(const std::string& std_str, const String& str);

/*!
\brief
    Returns true if std::u32string \a std_u32str is lexicographically equal or smaller than String \a str
*/
CEGUIEXPORT bool operator<=(const std::u32string& std_u32str, const String& str);


/*!
\brief
    Returns true if String \a str1 is lexicographically equal or larger than String \a str2
*/
CEGUIEXPORT bool operator>=(const String& str1, const String& str2);

/*!
\brief
    Returns true if String \a str is lexicographically equal or larger than std::string \a std_str
*/
CEGUIEXPORT bool operator>=(const String& str, const std::string& std_str);

/*!
\brief
    Returns true if String \a str is lexicographically equal or larger than std::u32string \a std_u32str
*/
CEGUIEXPORT bool operator>=(const String& str, const std::u32string& std_u32str);

/*!
\brief
    Returns true if std::string \a std_str is lexicographically equal or larger than String \a str
*/
CEGUIEXPORT bool operator>=(const std::string& std_str, const String& str);

/*!
\brief
    Returns true if std::u32string \a std_u32str is lexicographically equal or larger than  String \a str
*/
CEGUIEXPORT bool operator>=(const std::u32string& std_u32str, const String& str);

//////////////////////////////////////////////////////////////////////////
// char* and char32_t* comparison operators
//////////////////////////////////////////////////////////////////////////

/*!
\brief
    Returns true if the null-terminated char32_t array \a lhs is equal to String \a rhs
*/
CEGUIEXPORT bool operator==(const char32_t* lhs, const String& rhs);

/*!
\brief
    Returns true if String \a lhs is equal to the null-terminated char32_t array \a rhs
*/
CEGUIEXPORT bool operator==(const String& lhs, const char32_t* rhs);

/*!
\brief
    Returns true if the null-terminated char array \a lhs is equal to String \a rhs
*/
CEGUIEXPORT bool operator==(const char* lhs, const String& rhs);

/*!
\brief
    Returns true if String \a lhs is equal to the null-terminated char array \a rhs
*/
CEGUIEXPORT bool operator==(const String& lhs, const char* rhs);


/*!
\brief
    Returns true if String \a lhs is not equal to the null-terminated char array \a rhs
*/
CEGUIEXPORT bool operator!=(const char32_t* lhs, const String& rhs);

/*!
\brief
    Returns true if String \a lhs is not equal to the null-terminated char32_t array \a rhs
*/
CEGUIEXPORT bool operator!=(const String& lhs, const char32_t* rhs);

/*!
\brief
    Returns true if the null-terminated char array \a lhs is not equal to String \a rhs
*/
CEGUIEXPORT bool operator!=(const char* lhs, const String& rhs);

/*!
\brief
    Returns true if String \a lhs is not equal to the null-terminated char array \a rhs
*/
CEGUIEXPORT bool operator!=(const String& lhs, const char* rhs);


/*!
\brief
    Returns true if String \a lhs is lexicographically smaller than the null-terminated char array \a rhs
*/
CEGUIEXPORT bool operator<(const char32_t* lhs, const String& rhs);

/*!
\brief
    Returns true if String \a lhs is lexicographically smaller than the null-terminated char32_t array \a rhs
*/
CEGUIEXPORT bool operator<(const String& lhs, const char32_t* rhs);

/*!
\brief
    Returns true if the null-terminated char array \a lhs is lexicographically smaller than String \a rhs
*/
CEGUIEXPORT bool operator<(const char* lhs, const String& rhs);

/*!
\brief
    Returns true if String \a lhs is lexicographically smaller than the null-terminated char array \a rhs
*/
CEGUIEXPORT bool operator<(const String& lhs, const char* rhs);


/*!
\brief
    Returns true if String \a lhs is lexicographically equal or smaller than the null-terminated char array \a rhs
*/
CEGUIEXPORT bool operator<=(const char32_t* lhs, const String& rhs);

/*!
\brief
    Returns true if String \a lhs is lexicographically equal or smaller than the null-terminated char32_t array \a rhs
*/
CEGUIEXPORT bool operator<=(const String& lhs, const char32_t* rhs);

/*!
\brief
    Returns true if the null-terminated char array \a lhs is lexicographically equal or smaller than String \a rhs
*/
CEGUIEXPORT bool operator<=(const char* lhs, const String& rhs);

/*!
\brief
    Returns true if String \a lhs is lexicographically equal or smaller than the null-terminated char array \a rhs
*/
CEGUIEXPORT bool operator<=(const String& lhs, const char* rhs);


/*!
\brief
    Returns true if String \a lhs is lexicographically larger than the null-terminated char array \a rhs
*/
CEGUIEXPORT bool operator>(const char32_t* lhs, const String& rhs);

/*!
\brief
    Returns true if String \a lhs is lexicographically larger than the null-terminated char32_t array \a rhs
*/
CEGUIEXPORT bool operator>(const String& lhs, const char32_t* rhs);

/*!
\brief
    Returns true if the null-terminated char array \a lhs is lexicographically larger than String \a rhs
*/
CEGUIEXPORT bool operator>(const char* lhs, const String& rhs);

/*!
\brief
    Returns true if String \a lhs is lexicographically larger than the null-terminated char array \a rhs
*/
CEGUIEXPORT bool operator>(const String& lhs, const char* rhs);


/*!
\brief
    Returns true if String \a lhs is lexicographically equal or larger than the null-terminated char array \a rhs
*/
CEGUIEXPORT bool operator>=(const char32_t* lhs, const String& rhs);

/*!
\brief
    Returns true if String \a lhs is lexicographically equal or larger than the null-terminated char32_t array \a rhs
*/
CEGUIEXPORT bool operator>=(const String& lhs, const char32_t* rhs);

/*!
\brief
    Returns true if the null-terminated char array \a lhs is lexicographically equal or larger than String \a rhs
*/
CEGUIEXPORT bool operator>=(const char* lhs, const String& rhs);

/*!
\brief
    Returns true if String \a lhs is lexicographically equal or larger than the null-terminated char array \a rhs
*/
CEGUIEXPORT bool operator>=(const String& lhs, const char* rhs);

//////////////////////////////////////////////////////////////////////////
// Swap
//////////////////////////////////////////////////////////////////////////
/*!
\brief
    Swap the contents for two String objects

\param str1
    String object who's contents are to be swapped with \a str2

\param str2
    String object who's contents are to be swapped with \a str1

\return
    Nothing
*/
CEGUIEXPORT void swap(String& str1, String& str2);

//////////////////////////////////////////////////////////////////////////
// Stream functions
//////////////////////////////////////////////////////////////////////////

CEGUIEXPORT std::basic_ostream<char>& operator<<(std::basic_ostream<char>& outputStream, const String& str);

CEGUIEXPORT std::basic_istream<char>& operator>>(std::basic_istream<char>& inputStream, String& str);

}

namespace std
{

template<>
struct hash<CEGUI::String>
{
    std::size_t operator()(const CEGUI::String& str) const
    {
        return std::hash<std::basic_string<CEGUI::String::value_type>>()(str.c_str());
    }
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#elif CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_ASCII

namespace CEGUI
{
typedef std::string String;
}

#if defined(_MSC_VER)
#	pragma warning(disable : 4251)
#endif

#endif

#ifdef CEGUI_STRING_CPP_11
#undef CEGUI_STRING_CPP_11
#endif

#endif
