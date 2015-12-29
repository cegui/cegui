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
#include <string>


#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UNICODE

namespace CEGUI
{

#if defined (_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#	pragma warning(disable : 4996)
#endif

/*!
\brief
    UTF-32 based String class used within the GUI system.

    This class wraps around std::u32string and can be used identically. In addition to that it
    provides conversion functionality for regular char arrays (interpreted as UTF-8 / ASCII) and accepts
    such char arrays and strings in the constructor and in the assign, insert and other functions.
\note The std::string is not inherited as this cannot be safely done due to the non-virtual constructor.
\note The term "character" in the context of positions and counts/numbers, if not explicitly specified otherwise,
    refers to code points and not code units, which is of relevance in the context of std::string and char*,
    as they are allowed to contain UTF-8 code units.
\note The class was specifically created to provide the methods of an std::string (or std::basic_string to be specific)
    while providing support for passing char, char32_t, char*, char32_t*, std::string, std::u32string and String to all
    methods, operators and constructors via overloads. 
 \note Very few function overloads that take a char* and depend on a count are not supported, 
    since that would be unsafe (if taking the count for the code units) or misleading (if 
    taking the count for code points). Such functions are, for example, variations of find,
    assign and replace.
*/
class CEGUIEXPORT String
{
public:
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
    typedef std::u32string::iterator                iterator;         
    //! Type used for const iterators pointing to an UTF-32 code point of the String
    typedef std::u32string::const_iterator          const_iterator;      
    //! Type used for reverse iterators pointing to an UTF-32 code point of the String
    typedef std::u32string::reverse_iterator        reverse_iterator;     
    //! Type used for const reverse iterators pointing to an UTF-32 code point of the String
    typedef std::u32string::const_reverse_iterator  const_reverse_iterator; 

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
    static std::u32string convertUtf8ToUtf32(const char* utf8String);

    /*
    \brief
        Converts an UTF-8 encoded or ASCII string or char array to an UTF-32 encoded string (std::u32string).
    \param utf8StringStart
        A pointer to the beginning of the UTF-8 encoded or ASCII char array to be converted.
    \param utf8StringEnd
        A pointer to the end of the UTF-8 or ASCII char array to be converted.
    \return
        Returns an UTF-32 string (std::u32string) converted from the ASCII or UTF-8 encoded string or char array.
    */
    static std::u32string convertUtf8ToUtf32(const char* utf8StringStart, const char* utf8StringEnd);

    /*
    \brief
        Converts an UTF-8 encoded or ASCII string or char array to an UTF-32 encoded string (std::u32string).
    \param utf8String
        An UTF-8 encoded or ASCII string. The char array will be converted only until the first null-character is encountered.
    \return
        Returns an UTF-32 string (std::u32string) converted from the ASCII or UTF-8 encoded string or char array.
    */
    static std::u32string convertUtf8ToUtf32(const std::string& utf8String);

    /*
    \brief
        Converts an UTF-8 character to an UTF-32 encoded string (std::u32string).
    \param utf8Char
        A UTF-8 character.
    \return
        Returns an UTF-32 string (std::u32string) converted from the UTF-8 character.
    */
    static std::u32string convertUtf8ToUtf32(const char utf8Char);

    /*
    \brief
        Converts an UTF-8 encoded or ASCII har array to an UTF-32 encoded string (std::u32string).
    \param utf8String
        A pointer to the beginning of the UTF-8 encoded or ASCII char array to be converted.
    \param stringLength
        The amount of chars (UTF-8 code units) contained in this array that shall be converted.
    \return
        Returns an UTF-32 string (std::u32string) converted from the ASCII or UTF-8 encoded char array.
    */
    static std::u32string convertUtf8ToUtf32(const char* utf8String, const size_t stringLength);

    /*
    \brief
        Converts an UTF-32 encoded char32_t array to an UTF-8 string (std::string). The array has to be null-terminated.
    \param utf32String
        A UTF-32 encoded char32_t array, which is null-terminated.
    \return
        Returns an UTF-8 string (std::string) converted from the UTF-32 encoded char32_t array.
    */
    static std::string convertUtf32ToUtf8(const char32_t* utf32String);

    /*
    \brief
        Converts an UTF-32 string or char32_t array to an UTF-8 string (std::string).
    \param utf32StringStart
        A pointer to the beginning of the UTF-32 encoded char32_t array to be converted.
    \param utf32StringEnd
        A pointer to the end of the UTF-32 encoded char32_t array to be converted.
    \return
        Returns an UTF-8 string (std::string) converted from the UTF-32 string or char32_t array.
    */
    static std::string convertUtf32ToUtf8(const char32_t* utf32StringStart, const char32_t* utf32StringEnd);

    /*
    \brief
        Converts an UTF-32 string or char32_t array to an UTF-8 string (std::string).
        \param utf32String
        A UTF-32 string or char32_t array. The char32_t array will be converted only until the first null-character is encountered.
    \return
        Returns an UTF-8 string (std::string) converted from the UTF-32 string or char32_t array.
    */
    static std::string convertUtf32ToUtf8(const std::u32string& utf32String);

    /*
    \brief
        Converts an UTF-32 character to a UTF-8 string (std::string).
    \param utf32Char
        A UTF-32 character.
    \return
        Returns an UTF-8 string (std::string) converted from the UTF-32 character.
    */
    static std::string convertUtf32ToUtf8(const char32_t utf32Char);

    /*
    \brief
        Converts an UTF-32 string or char32_t array to an UTF-8 string (std::string).
    \param utf32String
        A pointer to the beginning of the UTF-32 encoded char32_t array to be converted.
    \param stringLength
        The length of the char32_t array of UTF-32 characters to be converted.
    \return
        Returns an UTF-8 string (std::string) converted from the UTF-32 string or char32_t array.
    */
    static std::string convertUtf32ToUtf8(const char32_t* utf32String, const size_t stringLength);

    //////////////////////////////////////////////////////////////////////////
    // Default constructors and destructors
    //////////////////////////////////////////////////////////////////////////

    //! Default constructor. Constructs empty string (zero size and unspecified capacity)
    String()
        : String(std::allocator<char32_t>())
    {}

    //! Default constructor. Constructs empty string (zero size and unspecified capacity)
    explicit String(const std::allocator<char32_t>& alloc)
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
        Constructs a String containing count copies of character "ch". The
        behavior is undefined for count >= npos.
    */
    String(size_type count, char32_t ch, const std::allocator<char32_t>& alloc = std::allocator<char32_t>())
        : d_string(count, ch, alloc)
    {}

    /*!
    \brief
        Constructs the String based on a substring [pos, pos+count) of another
        String. If the requested substring would be longer than the other String
        or if count == npos, then the resulting substring reaches from the
        requested position to its end.
    */
    String(const CEGUI::String& other, size_type pos, size_type count = npos, const std::allocator<char32_t>& alloc = std::allocator<char32_t>())
        : d_string(other.d_string, pos, count, alloc)
    {}

    /*!
    \brief
        Constructs the String based on a substring [pos, pos+count) of another
        String. If the requested substring would be longer than the other String
        or if count == npos, then the resulting substring reaches from the
        requested position to its end.
    */
    String(const std::u32string& other, size_type pos, size_type count = npos, const std::allocator<char32_t>& alloc = std::allocator<char32_t>())
        : d_string(other, pos, count, alloc)
    {}

    /*!
    \brief
        Constructs the String based on the specified number of characters by
        copying them from charArray. The resulting String may contain
        null-characters. The length of the new String is count. The behaviour is
        undefined if charArray does not point to an array with at least length
        count.
    */
    String(const char32_t* charArray, size_type count,
           const std::allocator<char32_t>& alloc = std::allocator<char32_t>())
        : d_string(charArray, count, alloc)
    {}

    /*!
    \brief
        Constructs the String based on a supplied null-terminated array of
        characters "charArray". The characters are copied until the first
        null-terminating character is reached.
        The behavior is undefined if "charArray" does not point to an array of
        at least Traits::length(s)+1 elements of char32_t.
    */
    String(const char32_t* charArray, const std::allocator<char32_t>& alloc = std::allocator<char32_t>())
        : d_string(charArray, alloc)
    {}

    /*!
    \brief
        Constructs a String with the contents of the range [first, last). 
    */
    template<class InputIt>
    String(InputIt first, InputIt last, const std::allocator<char32_t>& alloc = std::allocator<char32_t>())
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
        Copy constructor based on a std::u32string. 
    */
    String(const std::u32string& other)
        : d_string(other)
    {}

#if defined(_MSC_VER)
    /*!
    \brief
        Copy constructor. 
    */
    String(const CEGUI::String& other, const std::allocator<char32_t>& alloc)
        : d_string(other.d_string, alloc)
    {}

    /*!
    \brief
        Copy constructor based on a std::u32string. 
    */
    String(const std::u32string& other, const std::allocator<char32_t>& alloc)
        : d_string(other, alloc)
    {}
#endif

    /*!
    \brief
        Move constructor. The String "other" is left in valid but unspecified state.
    */
    String(CEGUI::String&& other)
        : d_string(other.d_string)
    {}

    /*!
    \brief
        Move constructor. The String "other" is left in valid but unspecified state.
    */
    String(std::u32string&& other)
        : d_string(other)
    {}

#if defined(_MSC_VER)
    /*!
    \brief
        Move constructor. The String "other" is left in valid but unspecified state.
    */
    String(CEGUI::String&& other, const std::allocator<char32_t>& alloc)
        : d_string(other.d_string, alloc)
    {}


    /*!
    \brief
        Move constructor. The String "other" is left in valid but unspecified state.
    */
    String(std::u32string&& other, const std::allocator<char32_t>& alloc)
        : d_string(other, alloc)
    {}

    /*!
    \brief
        Constructs a String with the contents of the initialiser-list "init"
    */
    String(std::initializer_list<char32_t> init, const std::allocator<char32_t>& alloc = std::allocator<char32_t>())
        : d_string(init, alloc)
    {}
#endif

    /*!
    \brief
        Constructor that creates a new string based on an std::string that can
        either contain ASCII or UTF-8 encoded characters. 

    \param str
        std::string object containing either ASCII or UTF-8 encoded characters.
    */
    String(const std::string& str) : d_string(convertUtf8ToUtf32(str))
    {}

    /*!
    \brief
        Constructor that creates a new string based on an char array "str" that can
        either contain ASCII or UTF-8 encoded characters. 

    \param str
        Pointer to the char array containing either ASCII or UTF-8 encoded characters.
    */
    String(const char* str) : d_string(convertUtf8ToUtf32(str))
    {}

    /*!
    \brief
        Constructs a new String object and initialise it using characters from
        the provided char array, which can contain ASCII or UTF-8 encoded
        characters. The input array will only be processed until the first
        encountered null-character.

    \param charArray
        Array of characters (or code units), which can be ASCII or UTF-8 encoded.

    \param chars_len
        Number of chars (ASCII characters or UTF-8 code units) from the array to be used.

    \exception
        std::length_error Thrown if resulting String object would be too big.
    */
    String(const char* charArray, size_type chars_len)
    {
        assign(charArray, chars_len);
    }


    //////////////////////////////////////////////////////////////////////////
    // Static public members
    //////////////////////////////////////////////////////////////////////////

    //! Value used to represent 'not found' conditions and 'all code points' etc.
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
        Replaces the contents of this String with a copy of String "str". 
    */
    String& operator=(const std::string& str)
    {
        d_string = convertUtf8ToUtf32(str);
        return *this;
    }

    /*!
    \brief
        Replaces the contents with this String with those of str using move semantics. Leaves str in a valid but unspecified state.
        This function has no effect if *this and the supplied String "str" are the same object.
    */
    String& operator=(String&& str)
    {
        d_string = str.d_string;
        return *this;
    }

    /*!
    \brief
        Replaces the contents with those of a null-terminated string stored in a char32_t array "charArray". Calls 
        *this = String(charArray) internally, which involves a call to Traits::length(charArray).
    */ 
    String& operator=(const char32_t* charArray)
    {
        d_string = charArray;
        return *this;
    }

    /*!
    \brief
        Replaces the contents with those of a null-terminated UTF-8 encoded or ASCII character string stored in a char array "charArray". Calls 
        *this = String(charArray) internally, which involves a call to Traits::length(charArray).
    */ 
    String& operator=(const char* charArray)
    {
        *this = String(charArray);
        return *this;
    }

    /*!
    \brief
        Replaces this Strings contents by the character ch, equivalent to *this = String(1, c).
    */ 
    String& operator=(char32_t ch)
    {
        d_string = ch;
        return *this;
    }

    /*!
    \brief
        Replaces this Strings contents by the character ch, equivalent to *this = String(1, c).
    */ 
    String& operator=(char ch)
    {
        *this = String(1, ch);
        return *this;
    }

    /*!
    \brief
        Replaces the contents with those of the initializer list initialiserList as if by *this = basic_string(ilist)
    */ 
    String& operator=(std::initializer_list<char32_t> initialiserList)
    {
        d_string = initialiserList;
        return *this;
    }

    //////////////////////////////////////////////////////////////////////////
    // Assigning characters
    //////////////////////////////////////////////////////////////////////////

    /*!
    \brief
        Replaces the contents with count copies of character "ch".
    */ 
    String& assign(size_type count, char32_t ch)
    {
        d_string.assign(count, ch);
        return *this;
    }

    /*!
    \brief
        Replaces the contents with count copies of character "ch".
    */ 
    String& assign(size_type count, char ch)
    {
        d_string.assign(count, convertUtf8ToUtf32(ch)[0]);
        return *this;
    }

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
    */ 
    String& assign(const std::string& str)
    {
        d_string.assign(convertUtf8ToUtf32(str));
        return *this;
    }

    /*!
    \brief
        Replaces the contents of this String with a substring [pos, pos+count) of String "str". If the requested substring is
        longer than String "str" or if count == npos, the resulting substring has the range [pos, size()). If pos > str.size(),
        std::out_of_range is thrown. 
    */ 
    String& assign(const String& str, size_type pos, size_type count = npos)
    {
        d_string.assign(str.d_string, pos, count);
        return *this;
    }

    /*!
    \brief
        Replaces the contents of this String with a substring [pos, pos+count) of an ASCII or UTF-8 encoded String "str". The position and length
        relate to the characters counts after the conversion to UTF32, which takes place before assignment. the requested substring is
        longer than String "str" or if count == npos, the resulting substring has the range [pos, size()). If pos > str.size(), std::out_of_range
        is thrown. In the case of UTF-8 encoded characters, the count refers to the number of code points, not to the number of code units (chars).
    */ 
    String& assign(const std::string& str, size_type pos, size_type count = npos)
    {
        std::u32string convertedStr = convertUtf8ToUtf32(str);
        d_string.assign(convertedStr, pos, count);
        return *this;
    }

    /*!
    \brief
        Replaces the contents of this String with those of a String "str" using move semantics. The String
        "str" is in undefined state after the operation.
    */ 
    String& assign(String&& str)
    {
        d_string.assign(str.d_string);
        return *this;
    }

    /*!
    \brief
        Replaces the contents of this String with "count" number of characters from the char32_t array "charArray".
    */ 
    String& assign(const char32_t* charArray, size_type count)
    {
        d_string.assign(charArray, count);
        return *this;
    }

    /*!
    \brief
        Replaces the contents of this String with "count" number of characters from the null-terminated char array
        "charArray" containing either UTF-8 encoded characters or ASCII chars. In the case of UTF-8 encoded characters,
        the count refers to the number of code units (char), not to the number of code points. The input array will
        only be processed until the first encountered null-character.
    */ 
    String& assign(const char* charArray, size_type count)
    {
        d_string.assign(convertUtf8ToUtf32(charArray, charArray + count).data(), count);
        return *this;
    }

    /*!
    \brief
        Replaces the contents of this String with the contents of a null-terminated character string "charArray". The length
        of the string is determined by the first null character, which is encountered. 
    */ 
    String& assign(const char32_t* charArray)
    {
        d_string.assign(charArray);
        return *this;
    }

    /*!
    \brief
        Replaces the contents of this String with the contents of a null-terminated character string "charArray" containing either
        UTF-8 encoded characters of ASCII characters. The length of the string is determined by the first null character and is measured
        in code units.
    */ 
    String& assign(const char* charArray)
    {
        d_string.assign(convertUtf8ToUtf32(charArray).data());
        return *this;
    }

    /*!
    \brief
        Replaces the contents of this string with copies of the characters in the range [first, last).
    */
    template<class InputIt>
    String& assign(InputIt first, InputIt last)
    {
        d_string.assign(first, last);
        return *this;
    }

    /*!
    \brief
        Replaces the contents of this string with the contents of the initialiser-list "initialiserList".
    */
    String& assign(std::initializer_list<char32_t> ilist)
    {
        d_string.assign(ilist);
        return *this;
    }

#if defined(_MSC_VER)
    /*!
    \brief
        Replaces the contents of this string with the contents of the initialiser-list "initialiserList".
    */
    std::allocator<char32_t> get_allocator() const
    {
        return d_string.get_allocator();
    }
#endif

    //////////////////////////////////////////////////////////////////////////
    // Element access
    //////////////////////////////////////////////////////////////////////////

    /*!
    \brief
        Returns a reference to the code point at specified location "pos". The location is boundary-checked and an
        exception of type std::out_of_range will be thrown on invalid access.

    \param idx
        Zero based index of the code point to be returned.
    */
    reference at(size_type pos)
    {
        return d_string[pos];
    }

    /*!
    \brief
        Returns a reference to the code point at specified location "pos". The location is boundary-checked and an
        exception of type std::out_of_range will be thrown on invalid access.

    \param idx
        Zero based index of the code point to be returned.
    */
    const_reference at(size_type pos) const
    {
        return d_string[pos];
    }

    /*!
    \brief
        Returns the code point at the given index.

    \param idx
        Zero based index of the code point to be returned.

    \return
        The char32_t code point at the given index within the String.
    */
    reference operator[](size_type pos)
    {
        return d_string[pos];
    }

    /*!
    \brief
        Returns the code point at the given index.

    \param idx
        Zero based index of the code point to be returned.

    \return
        The char32_t code point at the given index within the String.
    */
    const_reference operator[](size_type pos) const
    {
        return d_string[pos];
    }

    /*!
    \brief
        Returns the first code point of the String.

    \return
        The char32_t code point at the front of the String.
    */
    char32_t& front()
    {
        return d_string.front();
    }

    /*!
    \brief
        Returns the first code point of the String.

    \return
        The char32_t code point at the front of the String.
    */
    const char32_t& front() const;
   
    /*!
    \brief
        Returns the last code point of the String.

    \return
        The char32_t code point at back of the String.
    */
    char32_t& back()
    {
        return d_string.back();
    }

    /*!
    \brief
        Returns the last code point of the String.

    \return
        The char32_t code point at back of the String.
    */
    const char32_t& back() const
    {
        return d_string.back();
    }

    /*!
    \brief
        Returns a pointer to the underlying char32_t array of the String. The returned array is null-terminated.

    \return
        Pointer  code point at back of the String.
    */
    const char32_t* data() const
    {
        return d_string.data();
    }

    /*!
    \brief
        Returns a pointer to the underlying char32_t array of the String. The returned array is null-terminated.

    \return
        Pointer  code point at back of the String.
    */
    const char32_t* c_str() const
    {
        return d_string.c_str();
    }


    //////////////////////////////////////////////////////////////////////////
    // Iterators
    //////////////////////////////////////////////////////////////////////////


    /*!
    \brief
        Returns an iterator to the first character of the String. 

    \return
        An iterator object pointing to the beginning of the String.
    */
    std::u32string::iterator begin()
    {
        return d_string.begin();
    }

    /*!
    \brief
        Returns an iterator to the first character of the String. 

    \return
        A const_iterator object pointing to the beginning of the String.
    */
    std::u32string::const_iterator begin() const
    {
        return d_string.begin();
    }

    /*!
    \brief
        Returns a constant iterator to the first character of the String. 

    \return
        A const_iterator object pointing to the beginning of the String.
    */
    std::u32string::const_iterator cbegin() const
    {
        return d_string.cbegin();
    }

    /*!
    \brief
        Returns an iterator to the character following the last character of the String. Since this
        iterator is merely a placeholder, accessing it results in undefined behavior. 

    \return
        An iterator object pointing to the end of the String.
    */
    std::u32string::iterator end()
    {
        return d_string.end();
    }

    /*!
    \brief
        Returns an iterator to the character following the last character of the String. Since this
        iterator is merely a placeholder, accessing it results in undefined behavior. 

    \return
        A const_iterator object pointing to the end of the String.
    */
    std::u32string::const_iterator end() const
    {
        return d_string.end();
    }

    /*!
    \brief
        Returns a constant iterator to the character following the last character of the String. Since this
        iterator is merely a placeholder, accessing it results in undefined behavior. 

    \return
        A const_iterator object pointing to the end of the String.
    */
    std::u32string::const_iterator cend() const
    {
        return d_string.cend();
    }

    /*!
    \brief
        Returns a reverse iterator to the first character of the reversed String. This corresponds to
        the last character of the non-reversed String. 

    \return
        reverse_iterator object pointing to the first character of the reversed String.
    */
    std::u32string::reverse_iterator rbegin()
    {
        return d_string.rbegin();
    }

    /*!
    \brief
        Returns a reverse iterator to the first character of the reversed String. This corresponds to
        the last character of the non-reversed String. 

    \return
        const_reverse_iterator object pointing to the first character of the reversed String.
    */
    std::u32string::const_reverse_iterator rbegin() const
    {
        return d_string.rbegin();
    }

    /*!
    \brief
        Returns a constant reverse iterator to the first character of the reversed String. This corresponds to
        the last character of the non-reversed String. 

    \return
        const_reverse_iterator object pointing to the first character of the reversed String.
    */
    std::u32string::const_reverse_iterator crbegin() const
    {
        return d_string.crbegin();
    }

    /*!
    \brief
        Returns a reverse iterator to the character following the last character of the reversed String. This corresponds to
        the character preceding the first character of the non-reversed string. Since this acts as a placeholder, accessing
        it results in undefined behavior. 

    \return
        reverse_iterator object pointing to the character following the last character of the reversed String.
    */
    std::u32string::reverse_iterator rend()
    {
        return d_string.rend();
    }

    /*!
    \brief
        Returns a reverse iterator to the character following the last character of the reversed String. This corresponds to
        the character preceding the first character of the non-reversed string. Since this acts as a placeholder, accessing
        it results in undefined behavior. 

    \return
        const_reverse_iterator object pointing to the character following the last character of the reversed String.
    */
    std::u32string::const_reverse_iterator rend() const
    {
        return d_string.rend();
    }

    /*!
    \brief
        Returns a const reverse iterator to the character following the last character of the reversed String. This corresponds to
        the character preceding the first character of the non-reversed string. Since this acts as a placeholder, accessing
        it results in undefined behavior. 

    \return
        const_reverse_iterator object pointing to the character following the last character of the reversed String.
    */
    std::u32string::const_reverse_iterator crend() const
    {
        return d_string.rend();
    }


    //////////////////////////////////////////////////////////////////////////
    // Capacity
    //////////////////////////////////////////////////////////////////////////


    /*!
    \brief
        Checks whether the String contains characters or not.

    \return
        true if the String is empty, false if not.
    */
    bool empty() const
    {
        return d_string.empty();
    }

    /*!
    \brief
        Returns the size of the String in code points.

    \return
        Number of code points contained in the String.
    */
    size_type size() const
    {
        return d_string.size();
    }

    /*!
    \brief
        Returns the size of the String in code points.

    \return
        Number of code points contained in the String.
    */
    size_type length() const
    {
        return d_string.length();
    }


    /*!
    \brief
        Returns the maximum of code points this type of String is able to hold.

    \return
        The maximum number of code points that a string can contain
    */
    size_type max_size() const
    {
        return d_string.max_size();
    }

    /*!
    \brief
        Informs this String of a planned size-change, so that the storage allocation can be managed appropriately.

    \param new_cap
        The number of code points to allocate space for.  If \a num is larger that the current reserve, then a re-allocation will occur.  If
        \a num is smaller than the current reserve (but not 0) the buffer may be shrunk to the larger of the specified number, or the current
        String size (operation is currently not implemented).  If \a num is 0, then the buffer is re-allocated to fit the current String size.

    \exception std::length_error Thrown if resulting String object would be too big.
    */
    void reserve(size_type new_cap = 0)
    {
        d_string.reserve(new_cap);
    }

    /*!
    \brief
        Return the number of code points that the String can hold before a re-allocation is be required.

    \return
        Capacity of the currently allocated space.
    */
    size_type capacity() const
    {
        return d_string.capacity();
    }

    /*!
    \brief
        Requests the removal of unused capacity. Depending on the implementation, this call may reduce the
        capacity to size() or may have no consequences at all.
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
        Removes all characters of the string. No memory will be deallocated, leaving the
        capacity of the String unchanged.
    */
    void clear()
    {
        d_string.clear();
    }

    /*!
    \brief
        Inserts "count" number of copies of the code point ch at the position index.
    \param index
        The position index at which the characters are inserted.
    \param count
        The number of code point copies to insert
    \param ch
        The character (code point) to copy "count" times and insert
    \return Returns this String (*this).
    */
    String& insert(size_type index, size_type count, char32_t ch)
    {
        d_string.insert(index, count, ch);
        return *this;
    }

    /*!
    \brief
        Inserts a null-terminated character array "charArray" at the specified position in this String. The
        length of the inserted string taken from the array is determined by the first null-character that
        is encountered.
    \param index
        The position index at which the characters are inserted.
    \param charArray
        The character (code point) array to be inserted.
    \return Returns this String (*this).
    */
    String& insert(size_type index, const char32_t* charArray)
    {
        d_string.insert(index, charArray);
        return *this;
    }

    /*!
    \brief
        Inserts a null-terminated character array "charArray" consisting either of UTF-8 encoded or ASCII characters. The
        string stored in the array is inserted at the specified position in this String. The length of the inserted string
        is determined by the first null-character that is encountered.
    \param index
        The position index at which the characters are inserted.
    \param charArray
        The character (code point) array to be inserted.
    \return Returns this String (*this).
    */
    String& insert(size_type index, const char* charArray)
    {
        d_string.insert(index, convertUtf8ToUtf32(charArray).data());
        return *this;
    }

    /*!
    \brief
        Inserts the specified number of characters from the character array "charArray". The characters are
        inserted at the specified position. The string may contain null-characters.
    \param index
        The position index at which the characters are inserted.
    \param charArray
        The character (code point) array of which the characters are to be inserted.
    \param count
        The count of the characters to be inserted.
    \return Returns this String (*this).
    */
    String& insert(size_type index, const char32_t* charArray, size_type count)
    {
        d_string.insert(index, charArray, count);
        return *this;
    }

    /*!
    \brief
        Inserts the specified number of characters from the character array "charArray" containing either
        UTF-8 encoded or ASCII characters. The characters are inserted at the specified position. The string
        may NOT contain null-characters, the input string will only be processed until the first encountered
        null-character due to the UTF-32 conversion. The count of characters refers to the number of code points
        to be inserted, not the number of code units.
    \param index
        The position index at which the characters are inserted.
    \param charArray
        The character (code point) array of which the characters are to be inserted.
    \param count
        The count of the code points (not code units!) to be inserted.
    \return Returns this String (*this).
    */
    String& insert(size_type index, const char* charArray, size_type count)
    {
        d_string.insert(index, convertUtf8ToUtf32(charArray).data(), count);
        return *this;
    }

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
        Inserts the supplied UTF-32 string at the specified position of this String.
    \param index
        The position index at which the string will be inserted.
    \param str
        A reference to the string object that will be inserted.
    \return Returns this String (*this).
    */
    String& insert(size_type index, const std::u32string& str)
    {
        d_string.insert(index, str);
        return *this;
    }

    /*!
    \brief
        Inserts the supplied string, containing UTF-8 encoded or ASCII characters, at the
        specified position of this String.
    \param index
        The position index at which the string will be inserted.
    \param str
        A reference to the string object, containing UTF-8 encoded or ASCII characters, which will be inserted.
    \return Returns this String (*this).
    */
    String& insert(size_type index, const std::string& str)
    {
        d_string.insert(index, convertUtf8ToUtf32(str));
        return *this;
    }

    /*!
    \brief
        Inserts a substring of the supplied String at the position index.
    \param index
        The position index at which the substring String will be inserted.
    \param str
        A reference to the String object that will be inserted as substring.
    \param index_str
        Position of the first character of the substring to be inserted.
    \param count
        Number of characters of the substring to be inserted.
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
        Position of the first character of the substring to be inserted.
    \param count
        Number of characters of the substring to be inserted.
    \return Returns this String (*this).
    */ 
    String& insert(size_type index, const std::u32string& str, size_type index_str, size_type count = npos)
    {
        d_string.insert(index, str, index_str, count);
        return *this;
    }

    /*!
    \brief
        Inserts a substring of the supplied string, containing UTF-8 encoded or ASCII characters,
        at the position index. The input string will only be processed until the first null-character
        that is encountered, even if the index specifies it otherwise.
    \param index
        The position index at which the substring string will be inserted.
    \param str
        A reference to the string object, containing UTF-8 encoded or ASCII characters, which will be
        inserted as substring.
    \param index_str
        Position of the first code point (not code unit!) of the substring to be inserted.
    \param count
        Number of code points (not code units!) of the substring to be inserted.
    \return Returns this String (*this).
    */ 
    String& insert(size_type index, const std::string& str, size_type index_str, size_type count = npos)
    {
        d_string.insert(index, convertUtf8ToUtf32(str), index_str, count);
        return *this;
    }

    /*!
    \brief
        Inserts character (code point) "ch" before the character (code point) pointed to by
        iterator "iter".
    \param iter
        The position iterator at which the character will be inserted.
    \param ch
        The character (code point) to be inserted.
    \return Returns an iterator pointing to the first inserted character (code point). Returns
        "iter" if no characters were inserted.
    */
#if defined (_MSC_VER)
    iterator insert(const_iterator iter, char32_t ch)
#else
    iterator insert(iterator iter, char32_t ch)
#endif
    {
        return d_string.insert(iter, ch);
    }

    /*!
    \brief
        Inserts UTF-8 encoded or ASCII character (code point) "ch" before the character (code point) pointed to by
        iterator "iter".
    \param iter
        The position iterator at which the character will be inserted.
    \param ch
        The character (code point) to be inserted.
    \return Returns an iterator pointing to the first inserted character (code point). Returns
        "iter" if no characters were inserted.
    */
#if defined (_MSC_VER)
    iterator insert(const_iterator iter, char ch)
#else
    iterator insert(iterator iter, char ch)
#endif
    {
        return d_string.insert(iter, convertUtf8ToUtf32(ch)[0]);
    }

   /*!
    \brief
        Inserts "count" number of copies of the character (code point) "ch" before the character (code point)
        pointed to by iterator "iter".
    \param iter
        The position iterator at which the characters will be inserted.
    \param count
        The number of copies of the character (code point) to insert.
    \param ch
        The character (code point) to be copied and inserted.
    \return Returns an iterator pointing to the first inserted character (code point). Returns
        "iter" if no characters were inserted.
    */
#if defined (_MSC_VER)
    iterator insert(const_iterator iter, size_type count, char32_t ch)
    {
        return d_string.insert(iter, count, ch);
    }
#else 
    void insert(iterator iter, size_type count, char32_t ch)
    {
        d_string.insert(iter, count, ch);
    }
#endif

    /*!
    \brief
        Inserts "count" number of copies of the UTF-8 encoded or ASCII character "ch" before the character (code point)
        pointed to by iterator "iter".
    \param iter
        The position iterator at which the characters will be inserted.
    \param count
        The number of copies of the character (code point) to insert.
    \param ch
        The UTF-8 encoded or ASCII character (code point) to be copied and inserted.
    \return Returns an iterator pointing to the first inserted character (code point). Returns
        "iter" if no characters were inserted.
    */
#if defined (_MSC_VER)
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

    /*!
    \brief
        Inserts characters from the range [first, last) before the element pointed to by "iter".
    \param iter
        The position iterator at which the characters will be inserted.
    \param first
        Iterator pointing to the beginning of the range
    \param last
        Iterator pointing to the end of the range
    \return Returns an iterator pointing to the first inserted character (code point). Returns
        "iter" if no characters were inserted.
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
        The position iterator at which the characters will be inserted.
    \param initialiserList
        The initialiser list to be inserted.
    \return Returns an iterator pointing to the first inserted character (code point). Returns
        "iter" if no characters were inserted.
    */
#if defined (_MSC_VER)
    iterator insert(const_iterator iter, std::initializer_list<char32_t> initialiserList)
    {
        return d_string.insert(iter, initialiserList);
    }
#else 
    void insert(iterator iter, std::initializer_list<char32_t> initialiserList)
    {
        d_string.insert(iter, initialiserList);
    }
#endif
    /*!
    \brief
        Removes the specified number of characters starting at the index.
    \param index
        The position of the first character to remove.
    \param count
        The number of characters to remove.
    \return Returns a reference to this String (*this)
    */
    String& erase(size_type index = 0, size_type count = npos)
    {
        d_string.erase(index, count);
        return *this;
    }

    /*!
    \brief
        Removes the specified character at the position the iterator "iter" is pointing to.
    \param iter
        Iterator pointing at the character to remove.
    \return Iterator pointing to the character after the last removed character.
    */
#if defined (_MSC_VER)
    iterator erase(const_iterator iter)
#else
    iterator erase(iterator iter)
#endif
    {
        return d_string.erase(iter);
    }

    /*!
    \brief
        Removes the characters in the range specified by the iterators "first" and "last".
    \param first
        Iterator pointing to the first character of the range of characters to remove.
    \param first
        Iterator pointing to the last character of the range of characters to remove.
    \return Iterator pointing to the character after the last removed character.
    */
#if defined (_MSC_VER)
    iterator erase(const_iterator first, const_iterator last)
#else
    iterator erase(iterator first, iterator last)
#endif
    {
        return d_string.erase(first, last);
    }

    /*!
    \brief
        Appends the given character "ch" to the end of the String. 
    \param ch
        The character to append to the String.
    */
    void push_back(char32_t ch)
    {
        d_string.push_back(ch);
    }

    /*!
    \brief
        Appends the given character "ch" to the end of the String. 
    \param ch
        The character to append to the String.
    */
    void push_back(char ch)
    {
        d_string.push_back(convertUtf8ToUtf32(ch)[0]);
    }

    /*!
    \brief
        Removes the last character from the String. The behaviour is undefined for empty Strings.
    \param ch
        The character to append to the String.
    */
    void pop_back()
    {
        d_string.pop_back();
    }

    /*!
    \brief
        Appends "count" number of copies of character "ch" to this String.
    \param count
        The number of character copies to append.
    \param ch
        The character to copy and append.
    \return
        Returns a reference to this String (*this).
    */
    String& append(size_type count, char32_t ch)
    {
        d_string.append(count, ch);
        return *this;
    }

    /*!
    \brief
        Appends "count" number of copies of character "ch" to this String.
    \param count
        The number of character copies to append.
    \param ch
        The character to copy and append.
    \return
        Returns a reference to this String (*this).
    */
    String& append(size_type count, char ch)
    {
        d_string.append(count, convertUtf8ToUtf32(ch)[0]);
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
    String& append(const String& str)
    {
        d_string.append(str.d_string);
        return *this;
    }

    /*!
    \brief
        Appends the std::u32string "str" to this String.
    \param str
        The std::u32string to append to this String.
    \return
        Returns a reference to this String (*this).
    */
    String& append(const std::u32string& str)
    {
        d_string.append(str);
        return *this;
    }

    /*!
    \brief
        Appends the std::string "str", containing either UTF-8 encoded or
        ASCII characters, to this String.
    \param str
        The std::string, which contains either UTF-8 encoded or ASCII characters, which
        will be appended to this String.
    \return
        Returns a reference to this String (*this).
    */
    String& append(const std::string& str)
    {
        d_string.append(convertUtf8ToUtf32(str));
        return *this;
    }

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
        Appends a substring of the supplied std::u32string "str". The substring has dimensions [pos, pos+count) and
        is appended to this String.
    \param str
        The std::u32string from which the substring will be taken.
    \param pos
        The starting position for the substring.
    \param count
        The length of the substring.
    \return
        Returns a reference to this String (*this).
    */
    String& append(const std::u32string& str, size_type pos, size_type count = npos)
    {
        d_string.append(str, pos, count);
        return *this;
    }

    /*!
    \brief
        Appends a substring of the supplied std::string "str", which can contain either UTF-8 encode characters or
        ASCII characters. The substring has dimensions [pos, pos+count) and is appended to this String.
    \param str
        The std::string, containing either UTF-8 encoded or ASCII characters, from which the substring will be taken.
    \param pos
        The starting position (code point, not code unit) for the substring.
    \param count
        The length of the substring (measured in code points not code units).
    \return
        Returns a reference to this String (*this).
    */
    String& append(const std::string& str, size_type pos, size_type count = npos)
    {
        d_string.append(convertUtf8ToUtf32(str), pos, count);
        return *this;
    }

    /*!
    \brief
        Appends "count" number of characters (code points) from the char32_t array "charArray" to this
        String. "charArray" may contain null-characters. 
    \param charArray
        The char32_t array containing the string to append.
    \param count
        The number of characters (code points) from the array to append.
    \return
        Returns a reference to this String (*this).
    */
    String& append(const char32_t* charArray, size_type count)
    {
        d_string.append(charArray, count);
        return *this;
    }

    /*!
    \brief
        Appends "count" number of characters (code units!) from the char array "charArray" to this
        String. "charArray" can consist either of UTF-8 encoded or ASCII code points. The input array
        is only processed until the first null-character encountered.
    \param charArray
        The char array containing the string to append.
    \param count
        The number of characters (code units) from the array to append.
    \return
        Returns a reference to this String (*this).
    */
    String& append(const char* charArray, size_type count)
    {
        d_string.append(convertUtf8ToUtf32(charArray, charArray + count));
        return *this;
    }

    /*!
    \brief
        Appends the null-terminated char32_t array "charArray" to this String. The length of the
        appended string is determined by the first null-character that is encountered.
    \param charArray
        The char32_t array containing the string to append.
    \return
        Returns a reference to this String (*this).
    */
    String& append(const char32_t* charArray)
    {
        d_string.append(charArray);
        return *this;
    }

    /*!
    \brief
        Appends the null-terminated char array "charArray" to this String. The array may contain
        either UTF-8 encoded or ASCII characters. The length of the appended string is determined
        by the first null-character that is encountered.
    \param charArray
        The char array containing the string to append. It may contain either UTF-8 encoded or ASCII
        characters.
    \return
        Returns a reference to this String (*this).
    */
    String& append(const char* charArray)
    {
        d_string.append(convertUtf8ToUtf32(charArray).data());
        return *this;
    }

    /*!
    \brief
        Appends characters in the range [first, last) to this String. 
    \param first
        Beginning of the range of characters to append.
    \param first
        End of the range of characters to append.
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
        Appends the characters in the initializer_list "initialiserList" to this String.
    \param initialiserList
        The initializer_list containing the characters to append.
    \return
        Returns a reference to this String (*this).
    */
    String& append(std::initializer_list<char32_t> initialiserList)
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
        Appends the u32string "str" to this String.
    \param str
        The u32string to append to this String.
    \return
        Returns a reference to this String (*this).
    */
    String& operator+=(const std::u32string& str)
    {
        return append(str);
    }

    /*!
    \brief
        Appends the string "str", which may contain either UTF-8 encoded or ASCII characters,
        to this String.
    \param str
        The string, which may contain either UTF-8 encoded or ASCII characters, to append to this String.
    \return
        Returns a reference to this String (*this).
    */
    String& operator+=(const std::string& str)
    {
        return append(str);
    }

    /*!
    \brief
        Appends the character (code point) "ch" to this String.
    \param ch
        The character (code point) to append to this String.
    \return
        Returns a reference to this String (*this).
    */
    String& operator+=(char32_t ch)
    {
        return append(1, ch);
    }

    /*!
    \brief
        Appends the character "ch" to this String.
    \param ch
        The character to append to this String.
    \return
        Returns a reference to this String (*this).
    */
    String& operator+=(char ch)
    {
        return append(1, ch);
    }

    /*!
    \brief
        Appends the null-terminated character (code point) array "charArray" to this String.
    \param ch
        The null-terminated character (code point) array to append to this String.
    \return
        Returns a reference to this String (*this).
    */
    String& operator+=(const char32_t* charArray)
    {
        return append(charArray);
    }

    /*!
    \brief
        Appends the null-terminated character array "charArray" to this String.
    \param ch
        The null-terminated character array to append to this String.
    \return
        Returns a reference to this String (*this).
    */
    String& operator+=(const char* charArray)
    {
        return append(charArray);
    }

    /*!
    \brief
        Appends the characters in the initializer_list "initialiserList" to this String.
    \param initialiserList
        The initializer_list to append to this String.
    \return
        Returns a reference to this String (*this).
    */
    String& operator+=(std::initializer_list<char32_t> initialiserList)
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
        Compares this String to the given u32string.
    \param str
        The u32string to compare this String with.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(const std::u32string& str) const
    {
        return d_string.compare(str);
    }

    /*!
    \brief
        Compares this String to the given string containing either UTF-8 encoded
        or ASCII characters.
    \param str
        The string, containing either UTF-8 encoded or ASCII characters, to compare
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

    /*!
    \brief
        Compares a substring [pos1, pos1+count1) of this String to the given string "str".
    \param pos1
        The position of the first character of this String to compare.
    \param count1
        The number of characters of this String to compare.
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
        The position of the first character of this String to compare.
    \param count1
        The number of characters of this String to compare.
    \param str
        The u32string to compare this String with.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(size_type pos1, size_type count1, const std::u32string& str) const
    {
        return d_string.compare(pos1, count1, str);
    }

    /*!
    \brief
        Compares a substring [pos1, pos1+count1) of this String to the given string "str", which may
        contain either UTF-8 encoded or ASCII characters.
    \param pos1
        The position of the first character of this String to compare.
    \param count1
        The number of characters of this String to compare.
    \param str
        The string, containing either UTF-8 encoded or ASCII characters, to compare this String with.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(size_type pos1, size_type count1, const std::string& str) const
    {
        return d_string.compare(pos1, count1, convertUtf8ToUtf32(str));
    }

    /*!
    \brief
        Compares a substring [pos1, pos1+count1) of this String to the given String "str".
    \param pos1
        The position of the first character of this String to compare.
    \param count1
        The number of characters of this String to compare.
    \param str
        The String to compare this String with.
    \param pos2
        The position of the first character of the given String to compare.
    \param count2
        The number of characters of the given String to compare.
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
        The position of the first character of this String to compare.
    \param count1
        The number of characters of this String to compare.
    \param str
        The u32string to compare this String with.
    \param pos2
        The position of the first character of the given u32string to compare.
    \param count2
        The number of characters of the given u32string to compare.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(size_type pos1, size_type count1, const std::u32string& str,
                size_type pos2, size_type count2 = npos) const
    {
        return d_string.compare(pos1, count1, str,
                                pos2, count2);
    }

    /*!
    \brief
        Compares a substring [pos1, pos1+count1) of this String to the given string "str", which may
        contain either UTF-8 encoded or ASCII characters.
    \param pos1
        The position of the first character of this String to compare.
    \param count1
        The number of characters of this String to compare.
    \param str
        The string, containing either UTF-8 encoded or ASCII characters, to compare this String with.
    \param pos2
        The position of the first character of the given u32string to compare.
    \param count2
        The number of characters of the given u32string to compare.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(size_type pos1, size_type count1, const std::string& str,
                size_type pos2, size_type count2 = npos) const
    {
        return d_string.compare(pos1, count1, convertUtf8ToUtf32(str),
                                pos2, count2);
    }

    /*!
    \brief
        Compares this String to the string stored in the null-terminated character array "charArray"
    \param charArray
        Null-terminated character array to compare this String with.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(const char32_t* charArray) const
    {
        return d_string.compare(charArray);
    }

    /*!
    \brief
        Compares this String to the string stored in the null-terminated character array "charArray", containing
        either UTF-8 encoded or ASCII characters.
    \param charArray
        Null-terminated character array to compare this String with.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(const char* charArray) const
    {
        return d_string.compare(convertUtf8ToUtf32(charArray).data());
    }

    /*!
    \brief
        Compares this String to the string stored in the null-terminated character array "charArray".
    \param pos1
        The position of the first character of this String to compare.
    \param count1
        The number of characters of this String to compare.
    \param charArray
        Null-terminated character array to compare this String with.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(size_type pos1, size_type count1, const char32_t* charArray) const
    {
        return d_string.compare(pos1, count1, charArray);
    }

    /*!
    \brief
        Compares this String to the string stored in the null-terminated character array "charArray", containing
        either UTF-8 encoded or ASCII characters.
    \param pos1
        The position of the first character (code point) of this String to compare.
    \param count1
        The number of characters (code points) of this String to compare.
    \param charArray
        Null-terminated character array to compare this String with.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(size_type pos1, size_type count1, const char* charArray) const
    {
        return d_string.compare(pos1, count1, convertUtf8ToUtf32(charArray).data());
    }

    /*!
    \brief
        Compares this String to the string stored in the null-terminated character array "charArray".
    \param pos1
        The position of the first character of this String to compare.
    \param count1
        The number of characters of this String to compare.
    \param charArray
        Null-terminated character array to compare this String with.
    \param count2
        The number of characters of the given ch String to compare.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(size_type pos1, size_type count1, const char32_t* charArray, size_type count2) const
    {
        return d_string.compare(pos1, count1, charArray, count2);
    }

    /*!
    \brief
        Compares this String to the string stored in the null-terminated character array "charArray", containing
        either UTF-8 encoded or ASCII characters.
    \param pos1
        The position of the first character of this String to compare.
    \param count1
        The number of characters of this String to compare.
    \param charArray
        Null-terminated character array to compare this String with.
    \param count2
        The number of characters (code points) of the given charArray string to compare.
    \return
        -  0 if the strings are equal
        - <0 if this String is lexicographically smaller than the other
        - >0 if this String is lexicographically greater than the other
    */
    int compare(size_type pos1, size_type count1, const char* charArray, size_type count2) const
    {
        return d_string.compare(pos1, count1, convertUtf8ToUtf32(charArray).data(), count2);
    }

    /*!
    \brief
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with the supplied String.
    \param pos
        The position of the first character of this String to be replaced.
    \param count
        The length of characters (code points) of the substring of this String to replace.
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
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with the supplied u32string.
    \param pos
        The position of the first character of this String to be replaced.
    \param count
        The length of characters (code points) of the substring of this String to replace.
    \param str
        The u32string to use for the replacement.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count, const std::u32string& str)
    {
        d_string.replace(pos, count, str);
        return *this;
    }

    /*!
    \brief
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with the supplied string,
        containing either UTF-8 encoded or ASCII characters.
    \param pos
        The position of the first character of this String to be replaced.
    \param count
        The length of characters (code points) of the substring of this String to replace.
    \param str
        The string, containing either UTF-8 encoded or ASCII characters, to use for the replacement.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count, const std::string& str)
    {
        d_string.replace(pos, count, convertUtf8ToUtf32(str));
        return *this;
    }

    /*!
    \brief
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with the supplied String.
    \param first
        The beginning of the range of the characters of this String to be replaced.
    \param last
        The end of the range of the characters of this String to be replaced.
    \param str
        The String to use for the replacement.
    \return
        Returns a reference to this String (*this).
    */
#if defined (_MSC_VER)
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
        The beginning of the range of the characters of this String to be replaced.
    \param last
        The end of the range of the characters of this String to be replaced.
    \param str
        The u32string to use for the replacement.
    \return
        Returns a reference to this String (*this).
    */
#if defined (_MSC_VER)
    String& replace(const_iterator first, const_iterator last, const std::u32string& str)
#else
    String& replace(iterator first, iterator last, const std::u32string& str)
#endif
    {
        d_string.replace(first, last, str);
        return *this;
    }

    /*!
    \brief
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with the supplied string,
        containing either UTF-8 encoded or ASCII characters.
    \param first
        The beginning of the range of the characters of this String to be replaced.
    \param last
        The end of the range of the characters of this String to be replaced.
    \param str
        The string, containing either UTF-8 encoded or ASCII characters, to use for the replacement.
    \return
        Returns a reference to this String (*this).
    */
#if defined (_MSC_VER)
    String& replace(const_iterator first, const_iterator last, const std::string& str)
#else 
    String& replace(iterator first, iterator last, const std::string& str)
#endif
    {
        d_string.replace(first, last, convertUtf8ToUtf32(str));
        return *this;
    }

    /*!
    \brief
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with the supplied String.
    \param pos
        The position of the first character of this String to be replaced.
    \param count
        The length of characters (code points) of the substring of this String to replace.
    \param str
        The String to use for the replacement.
    \param pos2
        The starting position of the substring to be used to replace the characters (code points) of this String with.
    \param count2
        The number of characters of the substring to be used to replace the characters (code points) of this String with.
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
        The position of the first character of this String to be replaced.
    \param count
        The length of characters (code points) of the substring of this String to replace.
    \param str
        The u32string to use for the replacement.
    \param pos2
        The starting position of the substring to be used to replace the characters (code points) of this String with.
    \param count2
        The number of characters of the substring to be used to replace the characters (code points) of this String with.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count, const std::u32string& str,
                    size_type pos2, size_type count2 = npos)
    {
        d_string.replace(pos, count, str, pos2, count2);
        return *this;
    }

    /*!
    \brief
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with the supplied string,
        containing either UTF-8 encoded or ASCII characters.
    \param pos
        The position of the first character of this String to be replaced.
    \param count
        The length of characters (code points) of the substring of this String to replace.
    \param str
        The string, containing either UTF-8 encoded or ASCII characters, to use for the replacement.
    \param pos2
        The starting position (in code points) of the substring to be used to replace the characters (code points) of this String with.
    \param count2
        The number of characters (code points) of the substring to be used to replace the characters (code points) of this String with.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count, const std::string& str,
                    size_type pos2, size_type count2 = npos)
    {
        d_string.replace(pos, count, convertUtf8ToUtf32(str), pos2, count2);
        return *this;
    }

    /*!
    \brief
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with the supplied String
        in the range [first2, last2).
    \param first
        The beginning of the range of the characters of this String to be replaced.
    \param last
        The end of the range of the characters of this String to be replaced.
    \param first2
        The beginning of the range of the characters of the given replacement String to be replaced.
    \param last2
        The end of the range of the characters of of the given replacement String to be replaced.
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
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with "count2" number of characters
        (code points) of the string stored in the supplied character array.
    \param pos
        The position of the first character of this String to be replaced.
    \param count
        The length of characters (code points) of the substring of this String to replace.
    \param charArray
        The array of characters (code points) to use for the replacement.
    \param count2
        The number of characters of the substring to be used to replace the characters (code points) of this String with.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count,
                    const char32_t* charArray, size_type count2)
    {
        d_string.replace(pos, count, charArray, count2);
        return *this;
    }

    /*!
    \brief
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with "count2" number of characters
        (code points) of the string stored in the supplied character array. The character array may contain either UTF-8 encoded
        or ASCII characters. The input array will only be processed until the first null-character.
    \param pos
        The position of the first character of this String to be replaced.
    \param count
        The length of characters (code points) of the substring of this String to replace.
    \param charArray
        The array of characters (code points), containing contain either UTF-8 encoded or ASCII characters, to use for the replacement.
    \param count2
        The number of characters (CODE UNITS!) of the substring to be used to replace the characters (code points) of this String with.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count,
                    const char* charArray, size_type count2)
    {
        d_string.replace(pos, count, convertUtf8ToUtf32(charArray, charArray + count2).data(), npos);
        return *this;
    }

    /*!
    \brief
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with "count2" number of characters
        (code points) of the string stored in the supplied character array.
    \param first
        The beginning of the range of the characters of this String to be replaced.
    \param last
        The end of the range of the characters of this String to be replaced.
    \param charArray
        The array of characters (code points) to use for the replacement.
    \param count2
        The number of characters of the substring to be used to replace the characters (code points) of this String with.
    \return
        Returns a reference to this String (*this).
    */
#if defined (_MSC_VER)
    String& replace(const_iterator first, const_iterator last,
                    const char32_t* charArray, size_type count2)
#else
    String& replace(iterator first, iterator last,
                    const char32_t* charArray, size_type count2)
#endif
    {
        d_string.replace(first, last, charArray, count2);
        return *this;
    }

    /*!
    \brief
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with "count2" number of characters
        (code points) of the string stored in the supplied character array. The character array may contain either UTF-8 encoded
        or ASCII characters. The input array will only be processed until the first null-character.
    \param first
        The beginning of the range of the characters of this String to be replaced.
    \param last
        The end of the range of the characters of this String to be replaced.
    \param charArray
        The array of characters (code points) to use for the replacement. The character array may contain either UTF-8 encoded
        or ASCII characters.
    \param count2
        The number of characters (CODE UNITS!) of the substring to be used to replace the characters (code points) of this String with.
    \return
        Returns a reference to this String (*this).
    */
#if defined (_MSC_VER)
    String& replace(const_iterator first, const_iterator last,
                    const char* charArray, size_type count2)
#else
    String& replace(iterator first, iterator last,
                    const char* charArray, size_type count2)
#endif
    {
        d_string.replace(first, last, convertUtf8ToUtf32(charArray, charArray + count2).data(), npos);
        return *this;
    }

    /*!
    \brief
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with the characters
        (code points) of the string stored in the supplied character array. The characters of the array up to the
        first null-character are used for the replacement
    \param pos
        The position of the first character of this String to be replaced.
    \param count
        The length of characters (code points) of the substring of this String to replace.
    \param charArray
        The null-terminated array of characters (code points) to use for the replacement.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count,
                    const char32_t* charArray)
    {
        d_string.replace(pos, count, charArray);
        return *this;
    }

    /*!
    \brief
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with the characters
        (code points) of the string stored in the supplied character array. The characters of the array up to the
        first null-character are used for the replacement. The character array may contain either UTF-8 encoded
        or ASCII characters.
    \param pos
        The position of the first character of this String to be replaced.
    \param count
        The length of characters (code points) of the substring of this String to replace.
    \param charArray
        The null-terminated array of characters (or code units) to use for the replacement. The character array may
        contain either UTF-8 encoded or ASCII characters.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count,
                    const char* charArray)
    {
        d_string.replace(pos, count, convertUtf8ToUtf32(charArray).data());
        return *this;
    }

    /*!
    \brief
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with the characters
        (code points) of the string stored in the supplied character array. The characters of the array up to the
        first null-character are used for the replacement.
    \param first
        The beginning of the range of the characters of this String to be replaced.
    \param last
        The end of the range of the characters of this String to be replaced.
    \param charArray
        The null-terminated array of characters (code points) to use for the replacement.
    \return
        Returns a reference to this String (*this).
    */
#if defined (_MSC_VER)
    String& replace(const_iterator first, const_iterator last,
                    const char32_t* charArray)
#else
    String& replace(iterator first, iterator last,
                    const char32_t* charArray)
#endif
    {
        d_string.replace(first, last, charArray);
        return *this;
    }

    /*!
    \brief
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with the characters
        (code points) of the string stored in the supplied character array. The characters of the array up to the
        first null-character are used for the replacement. The character array may contain either UTF-8 encoded
        or ASCII characters.
    \param first
        The beginning of the range of the characters of this String to be replaced.
    \param last
        The end of the range of the characters of this String to be replaced.
    \param charArray
        The null-terminated array of characters (code points) to use for the replacement. The character array may contain
        either UTF-8 encoded or ASCII characters.
    \return
        Returns a reference to this String (*this).
    */
#if defined (_MSC_VER)
    String& replace(const_iterator first, const_iterator last,
                    const char* charArray)
#else 
    String& replace(iterator first, iterator last,
                    const char* charArray)
#endif
    {
        d_string.replace(first, last, convertUtf8ToUtf32(charArray).data());
        return *this;
    }

    /*!
    \brief
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with "count2" number
        of copies of the supplied character "ch".
    \param pos
        The position of the first character of this String to be replaced.
    \param count
        The length of characters (code points) of the substring of this String to replace.
    \param count2
        The number of copies of character "ch" that should be used for the replacement.
    \param ch
        The character to be copied for the replacement.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count,
                    size_type count2, char32_t ch)
    {
        d_string.replace(pos, count, count2, ch);
        return *this;
    }

    /*!
    \brief
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with "count2" number
        of copies of the supplied character "ch".
    \param pos
        The position of the first character of this String to be replaced.
    \param count
        The length of characters (code points) of the substring of this String to replace.
    \param count2
        The number of copies of character "ch" that should be used for the replacement.
    \param ch
        The character to be copied for the replacement.
    \return
        Returns a reference to this String (*this).
    */
    String& replace(size_type pos, size_type count,
                    size_type count2, char ch)
    {
        d_string.replace(pos, count, count2, convertUtf8ToUtf32(ch)[0]);
        return *this;
    }

    /*!
    \brief
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with "count2" number
        of copies of the supplied character "ch".
    \param first
        The beginning of the range of the characters of this String to be replaced.
    \param last
        The end of the range of the characters of this String to be replaced.
    \param count2
        The number of copies of character "ch" that should be used for the replacement.
    \param ch
        The character to be copied for the replacement.
    \return
        Returns a reference to this String (*this).
    */
#if defined (_MSC_VER)
    String& replace(const_iterator first, const_iterator last,
                    size_type count2, char32_t ch)
#else
    String& replace(iterator first, iterator last,
                    size_type count2, char32_t ch)
#endif
    {
        d_string.replace(first, last, count2, ch);
        return *this;
    }

    /*!
    \brief
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with "count2" number
        of copies of the supplied character "ch".
    \param first
        The beginning of the range of the characters of this String to be replaced.
    \param last
        The end of the range of the characters of this String to be replaced.
    \param count2
        The number of copies of character "ch" that should be used for the replacement.
    \param ch
        The character to be copied for the replacement.
    \return
        Returns a reference to this String (*this).
    */
#if defined (_MSC_VER)
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

    /*!
    \brief
        Replaces the part of this String defined by either [pos, pos + count) or [first, last) with the characters
        in the initializer_list "initialiserList"
    \param first
        The beginning of the range of the characters of this String to be replaced.
    \param last
        The end of the range of the characters of this String to be replaced.
    \param initialiserList
        The initializer_list with the characters to use for the replacement.
    \return
        Returns a reference to this String (*this).
    */
#if defined (_MSC_VER)
    String& replace(const_iterator first, const_iterator last,
                    std::initializer_list<char32_t> initialiserList)
#else
    String& replace(iterator first, iterator last,
                    std::initializer_list<char32_t> initialiserList)
#endif
    {
        d_string.replace(first, last, initialiserList);
        return *this;
    }

    /*
    \brief
        Returns a substring [pos, pos+count) of this String. If the requested substring is larger than the String, or if count == npos,
        the substring [pos, size()) is returned.
    \param pos
        Position of the first character of the substring.
    \param count
        The number of characters of the substring to return.
    \return
        A String containing the requested substring or an empty String if pos == size().
    */
    String substr(size_type pos = 0, size_type count = npos) const
    {
        return String(d_string.substr(pos, count));
    }

    /*
    \brief
        Copies a substring [pos, pos+count) of this String to the character array pointed to by "dest". If the requested substring
        is larger than the String, or if count == npos, the substring [pos, size()) is copied. The resulting String is not
        null-terminated (does not end with a null-character).
    \param dest
        The target character array to which the substring will be copied.
    \param count
        The number of characters of the substring to return.
    \param pos
        The position of the first character of the substring to be copied.
    \return
        The number of characters copied.
    */
    size_type copy(char32_t* dest, size_type count, size_type pos = 0) const
    {
        return d_string.copy(dest, count, pos);
    }

    /*
    \brief
        Resizes the string, potentially adding "count" number of characters ( char() ). If the current size is less than "count"
        then additional characters are appended. If the current size is greater than count, the string is reduced
        to fit the specified size based on the elements from the beginning of the String.
    \param count
        The number of characters (code points) to resize this String to.
    */
    void resize(size_type count)
    {
        return d_string.resize(count);
    }

    /*
    \brief
        Resizes the string, potentially adding "count" number copies of character "ch". If the current size is less than "count"
        then additional characters are appended. If the current size is greater than count, the string is reduced
        to fit the specified size based on the elements from the beginning of the String.
    \param count
        The number of characters (code points) to resize this String to.
    \param ch
        The character to make copies of if appending characters.
    */
    void resize(size_type count, char32_t ch)
    {
        return d_string.resize(count, ch);
    }

    /*
    \brief
        Resizes the string, potentially adding "count" number copies of character "ch". If the current size is less than "count"
        then additional characters are appended. If the current size is greater than count, the string is reduced
        to fit the specified size based on the elements from the beginning of the String.
    \param count
        The number of characters (code points) to resize this String to.
    \param ch
        The character to make copies of if appending characters.
    */
    void resize(size_type count, char ch)
    {
        return d_string.resize(count, convertUtf8ToUtf32(ch)[0]);
    }

    /*
    \brief
        Exchanges the contents of this String with the contents of the String "other". All iterators and references may be invalidated.
    \param other
        The String to exchange the contents of this String with.
    */
    void swap(String& other)
    {
        d_string.swap(other.d_string);
    }

    /*
    \brief
        Finds the first substring that is equal to the given String. Search starts at position "pos".
    \param str
        The String to search for.
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character of the found substring matching the String or returns npos if no
        matching substring was found. 
    */
    size_type find(const String& str, size_type pos = 0) const
    {
        return d_string.find(str.d_string, pos);
    }

    /*
    \brief
        Finds the first substring that is equal to the given u32string. Search starts at position "pos".
    \param str
        The u32string to search for.
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character of the found substring matching the String or returns npos if no
        matching substring was found. 
    */
    size_type find(const std::u32string& str, size_type pos = 0) const
    {
        return d_string.find(str, pos);
    }

    /*
    \brief
        Finds the first substring that is equal to the given string, containing either UTF-8 encoded or ASCII characters.
        Search starts at position "pos". The UTF-8 encoded / ASCII characters are converted to UTF-32 for comparison.
    \param str
        The string, containing either UTF-8 encoded or ASCII characters, to search for.
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character of the found substring matching the String or returns npos if no
        matching substring was found. 
    */
    size_type find(const std::string& str, size_type pos = 0) const
    {
        return d_string.find(convertUtf8ToUtf32(str), pos);
    }

    /*
    \brief
        Finds the first substring that is equal to the given characters in the array "charArray". Search starts at position "pos" of this String.
        The array may contain null-characters. The number of characters from the array to be compared are specified by "count".
    \param charArray
        The character array charArray to search for.
    \param pos
        The position at which the search will start.
    \param count
        The number of characters of the array to use for comparison.
    \return
        Returns the position of the first character of the found substring matching the String or returns npos if no
        matching substring was found. 
    */
    size_type find(const char32_t* charArray, size_type pos, size_type count) const
    {
        return d_string.find(charArray, pos, count);
    }

    /*
    \brief
        Finds the first substring that is equal to the given characters in the array "charArray". Search starts at position "pos" of
        this String. The amount of characters to be compared from the character array is determined by the first null-character encountered
        in it. 
    \param charArray
        The character array to search for.
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character of the found substring matching the String or returns npos if no
        matching substring was found. 
    */
    size_type find(const char32_t* charArray, size_type pos = 0) const
    {
        return d_string.find(charArray, pos);
    }

    /*
    \brief
        Finds the first substring that is equal to the given characters in the array "charArray", containing either UTF-8 encoded or ASCII
        characters. Search starts at position "pos" of this String. The amount of characters to be compared from the character array is
        determined by the first null-character encountered in it. 
    \param charArray
        The character array to search for, which may contain either UTF-8 encoded or ASCII characters.
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character of the found substring matching the String or returns npos if no
        matching substring was found. 
    */
    size_type find(const char* charArray, size_type pos = 0) const
    {
        return d_string.find(convertUtf8ToUtf32(charArray).data(), pos);
    }

    /*
    \brief
        Finds the first character "ch" in this String. Search starts at position "pos" of this String.
    \param ch
        The character to search for.
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character of the found substring matching the String or returns npos if no
        matching substring was found. 
    */
    size_type find(char32_t ch, size_type pos = 0) const
    {
        return d_string.find(ch, pos);
    }

    /*
    \brief
        Finds the first character "ch" in this String. Search starts at position "pos" of this String.
    \param ch
        The character to search for.
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character of the found substring matching the String or returns npos if no
        matching substring was found. 
    */
    size_type find(char ch, size_type pos = 0) const
    {
        return d_string.find(convertUtf8ToUtf32(ch)[0], pos);
    }

    // rfind

    /*
    \brief
        Finds the last substring that is equal to the given String. Search starts at position "pos".
        If npos or any value equal or bigger than size() is passed as pos, the whole string is searched. 
    \param str
        The String to search for.
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character of the found substring matching the String or returns npos if no
        matching substring was found. 
    */
    size_type rfind(const String& str, size_type pos = npos) const
    {
        return d_string.rfind(str.d_string, pos);
    }

    /*
    \brief
        Finds the first substring that is equal to the given u32string. Search starts at position "pos".
        If npos or any value equal or bigger than size() is passed as pos, the whole string is searched. 
    \param str
        The u32string to search for.
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character of the found substring matching the String or returns npos if no
        matching substring was found. 
    */
    size_type rfind(const std::u32string& str, size_type pos = npos) const
    {
        return d_string.rfind(str, pos);
    }

    /*
    \brief
        Finds the first substring that is equal to the given string, containing either UTF-8 encoded or ASCII characters.
        Search starts at position "pos". The UTF-8 encoded / ASCII characters are converted to UTF-32 for comparison.
        If npos or any value equal or bigger than size() is passed as pos, the whole string is searched.
    \param str
        The string, containing either UTF-8 encoded or ASCII characters, to search for.
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character of the found substring matching the String or returns npos if no
        matching substring was found. 
    */
    size_type rfind(const std::string& str, size_type pos = npos) const
    {
        return d_string.rfind(convertUtf8ToUtf32(str), pos);
    }

    /*
    \brief
        Finds the first substring that is equal to the given characters in the array "charArray". Search starts at position "pos" of this String.
        The array may contain null-characters. The number of characters from the array to be compared are specified by "count".
        If npos or any value equal or bigger than size() is passed as pos, the whole string is searched.
    \param charArray
        The character array charArray to search for.
    \param pos
        The position at which the search will start.
    \param count
        The number of characters of the array to use for comparison.
    \return
        Returns the position of the first character of the found substring matching the String or returns npos if no
        matching substring was found. 
    */
    size_type rfind(const char32_t* charArray, size_type pos, size_type count) const
    {
        return d_string.rfind(charArray, pos, count);
    }

    /*
    \brief
        Finds the first substring that is equal to the given characters in the array "charArray". Search starts at position "pos" of
        this String. The amount of characters to be compared from the character array is determined by the first null-character encountered
        in it.
        If npos or any value equal or bigger than size() is passed as pos, the whole string is searched.
    \param charArray
        The character array to search for.
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character of the found substring matching the String or returns npos if no
        matching substring was found. 
    */
    size_type rfind(const char32_t* charArray, size_type pos = npos) const
    {
        return d_string.rfind(charArray, pos);
    }

    /*
    \brief
        Finds the first substring that is equal to the given characters in the array "charArray", containing either UTF-8 encoded or ASCII
        characters. Search starts at position "pos" of this String. The amount of characters to be compared from the character array is
        determined by the first null-character encountered in it.
        If npos or any value equal or bigger than size() is passed as pos, the whole string is searched.
    \param charArray
        The character array to search for, which may contain either UTF-8 encoded or ASCII characters.
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character of the found substring matching the String or returns npos if no
        matching substring was found. 
    */
    size_type rfind(const char* charArray, size_type pos = npos) const
    {
        return d_string.rfind(convertUtf8ToUtf32(charArray).data(), pos);
    }

    /*
    \brief
        Finds the first character "ch" in this String. Search starts at position "pos" of this String.
        If npos or any value equal or bigger than size() is passed as pos, the whole string is searched.
    \param ch
        The character to search for.
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character of the found substring matching the String or returns npos if no
        matching substring was found. 
    */
    size_type rfind(char32_t ch, size_type pos = npos) const
    {
        return d_string.rfind(ch, pos);
    }

    /*
    \brief
        Finds the first character "ch" in this String. Search starts at position "pos" of this String.
        If npos or any value equal or bigger than size() is passed as pos, the whole string is searched.
    \param ch
        The character to search for.
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character of the found substring matching the String or returns npos if no
        matching substring was found. 
    */
    size_type rfind(char ch, size_type pos = npos) const
    {
        return d_string.rfind(convertUtf8ToUtf32(ch)[0], pos);
    }

    // find_first_of

    /*
    \brief
        Finds the first character in this String that is equal to one of the characters in the given string. The search considers
        only the interval [pos, size()) of this String in the search.
    \param str
        The String containing the characters to search for. 
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character found or returns npos if no matching character was found. 
    */
    size_type find_first_of(const String& str, size_type pos = 0) const
    {
        return d_string.find_first_of(str.d_string, pos);
    }

    /*
    \brief
        Finds the first character in this String that is equal to one of the characters in the given string. The search considers
        only the interval [pos, size()) of this String in the search.
    \param str
        The String containing the characters to search for. 
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character found or returns npos if no matching character was found. 
    */
    size_type find_first_of(const std::u32string& str, size_type pos = 0) const
    {
        return d_string.find_first_of(str, pos);
    }

    /*
    \brief
        Finds the first character in this String that is equal to one of the characters in the given string. The search considers
        only the interval [pos, size()) of this String in the search.
    \param str
        The String containing the characters to search for, which may contain either UTF-8 encoded or ASCII characters. 
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character found or returns npos if no matching character was found. 
    */
    size_type find_first_of(const std::string& str, size_type pos = 0) const
    {
        return d_string.find_first_of(convertUtf8ToUtf32(str), pos);
    }

    /*
    \brief
        Finds the first character in this String that is equal to one of the characters in the given string. The search considers
        only the interval [pos, size()) of this String in the search. The string may contain null-characters.
    \param charArray
        The character array containing the string to search for, which may contain either UTF-8 encoded or ASCII characters. 
    \param pos
        The position at which the search will start.
    \param count
        The number of characters (code points) in the supplied string to use for the comparison.
    \return
        Returns the position of the first character found or returns npos if no matching character was found. 
    */
    size_type find_first_of(const char32_t* charArray, size_type pos, size_type count) const
    {
        return d_string.find_first_of(charArray, pos, count);
    }

    /*
    \brief
        Finds the first character in this String that is equal to one of the characters in the given string. The search considers
        only the interval [pos, size()) of this String in the search. The length of the supplied string is determined by the first
        null-character that is encountered.
    \param charArray
        The character array containing the string to search for. 
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character found or returns npos if no matching character was found. 
    */
    size_type find_first_of(const char32_t* charArray, size_type pos = 0) const
    {
        return d_string.find_first_of(charArray, pos);
    }

    /*
    \brief
        Finds the first character in this String that is equal to one of the characters in the given string. The search considers
        only the interval [pos, size()) of this String in the search. The length of the supplied string is determined by the first
        null-character that is encountered.
    \param charArray
        The character array containing the string to search for, which may contain either UTF-8 encoded or ASCII characters. 
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character found or returns npos if no matching character was found. 
    */
    size_type find_first_of(const char* charArray, size_type pos = 0) const
    {
        return d_string.find_first_of(convertUtf8ToUtf32(charArray).data(), pos);
    }

    /*
    \brief
        Finds the first character in this String that is equal to the character "ch". The search considers only the interval [pos, size())
        of this String in the search.
    \param ch
        The character to search for. 
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character found or returns npos if no matching character was found. 
    */
    size_type find_first_of(char32_t ch, size_type pos = 0) const
    {
        return d_string.find_first_of(ch, pos);
    }

    /*
    \brief
        Finds the first character in this String that is equal to the character "ch". The search considers only the interval [pos, size())
        of this String in the search.
    \param ch
        The character to search for. 
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character found or returns npos if no matching character was found. 
    */
    size_type find_first_of(char ch, size_type pos = 0) const
    {
        return d_string.find_first_of(convertUtf8ToUtf32(ch)[0], pos);
    }

    // find_last_of

    /*
    \brief
        Finds the last character in this String that is equal to one of the characters in the given string. The search considers
        only the interval [0; pos] of this String in the search.
    \param str
        The String containing the characters to search for. 
    \param pos
        The position at which the search will stop.
    \return
        Returns the position of the last character found or returns npos if no matching character was found. 
    */
    size_type find_last_of(const String& str, size_type pos = npos) const
    {
        return d_string.find_last_of(str.d_string, pos);
    }

    /*
    \brief
        Finds the last character in this String that is equal to one of the characters in the given string. The search considers
        only the interval [0; pos] of this String in the search.
    \param str
        The String containing the characters to search for. 
    \param pos
        The position at which the search will stop.
    \return
        Returns the position of the last character found or returns npos if no matching character was found. 
    */
    size_type find_last_of(const std::u32string& str, size_type pos = npos) const
    {
        return d_string.find_last_of(str, pos);
    }

    /*
    \brief
        Finds the last character in this String that is equal to one of the characters in the given string. The search considers
        only the interval [0; pos] of this String in the search.
    \param str
        The String containing the characters to search for, which may contain either UTF-8 encoded or ASCII characters. 
    \param pos
        The position at which the search will stop.
    \return
        Returns the position of the last character found or returns npos if no matching character was found. 
    */
    size_type find_last_of(const std::string& str, size_type pos = npos) const
    {
        return d_string.find_last_of(convertUtf8ToUtf32(str), pos);
    }

    /*
    \brief
        Finds the last character in this String that is equal to one of the characters in the given string. The search considers
        only the interval [0; pos] of this String in the search. The string may contain null-characters.
    \param charArray
        The character array containing the string to search for, which may contain either UTF-8 encoded or ASCII characters. 
    \param pos
        The position at which the search will stop.
    \param count
        The number of characters (code points) in the supplied string to use for the comparison.
    \return
        Returns the position of the last character found or returns npos if no matching character was found. 
    */
    size_type find_last_of(const char32_t* charArray, size_type pos, size_type count) const
    {
        return d_string.find_last_of(charArray, pos, count);
    }

    /*
    \brief
        Finds the last character in this String that is equal to one of the characters in the given string. The search considers
        only the interval [0; pos] of this String in the search. The length of the supplied string is determined by the last
        null-character that is encountered.
    \param charArray
        The character array containing the string to search for. 
    \param pos
        The position at which the search will stop.
    \return
        Returns the position of the last character found or returns npos if no matching character was found. 
    */
    size_type find_last_of(const char32_t* charArray, size_type pos = npos) const
    {
        return d_string.find_last_of(charArray, pos);
    }

    /*
    \brief
        Finds the last character in this String that is equal to one of the characters in the given string. The search considers
        only the interval [0; pos] of this String in the search. The length of the supplied string is determined by the last
        null-character that is encountered.
    \param charArray
        The character array containing the string to search for, which may contain either UTF-8 encoded or ASCII characters. 
    \param pos
        The position at which the search will stop.
    \return
        Returns the position of the last character found or returns npos if no matching character was found. 
    */
    size_type find_last_of(const char* charArray, size_type pos = npos) const
    {
        return d_string.find_last_of(convertUtf8ToUtf32(charArray).data(), pos);
    }

    /*
    \brief
        Finds the last character in this String that is equal to the character "ch". The search considers only the interval [0; pos]
        of this String in the search.
    \param ch
        The character to search for. 
    \param pos
        The position at which the search will stop.
    \return
        Returns the position of the last character found or returns npos if no matching character was found. 
    */
    size_type find_last_of(char32_t ch, size_type pos = npos) const
    {
        return d_string.find_last_of(ch, pos);
    }

    /*
    \brief
        Finds the last character in this String that is equal to the character "ch". The search considers only the interval [0; pos]
        of this String in the search.
    \param ch
        The character to search for. 
    \param pos
        The position at which the search will stop.
    \return
        Returns the position of the last character found or returns npos if no matching character was found. 
    */
    size_type find_last_of(char ch, size_type pos = npos) const
    {
        return d_string.find_last_of(convertUtf8ToUtf32(ch)[0], pos);
    }

    // first_not_of

    /*
    \brief
        Finds the first character in this String that is not equal to any of the characters in the given string. The search considers
        only the interval [pos, size()) of this String in the search.
    \param str
        The String containing the characters to search for. 
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character found or returns npos if no matching character was found. 
    */
    size_type find_first_not_of(const String& str, size_type pos = 0) const
    {
        return d_string.find_first_not_of(str.d_string, pos);
    }

    /*
    \brief
        Finds the first character in this String that is not equal to any of the characters in the given string. The search considers
        only the interval [pos, size()) of this String in the search.
    \param str
        The String containing the characters to search for. 
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character found or returns npos if no matching character was found. 
    */
    size_type find_first_not_of(const std::u32string& str, size_type pos = 0) const
    {
        return d_string.find_first_not_of(str, pos);
    }

    /*
    \brief
        Finds the first character in this String that is not equal to any of the characters in the given string. The search considers
        only the interval [pos, size()) of this String in the search.
    \param str
        The String containing the characters to search for, which may contain either UTF-8 encoded or ASCII characters. 
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character found or returns npos if no matching character was found. 
    */
    size_type find_first_not_of(const std::string& str, size_type pos = 0) const
    {
        return d_string.find_first_not_of(convertUtf8ToUtf32(str), pos);
    }

    /*
    \brief
        Finds the first character in this String that is not equal to any of the characters in the given string. The search considers
        only the interval [pos, size()) of this String in the search. The string may contain null-characters.
    \param charArray
        The character array containing the string to search for, which may contain either UTF-8 encoded or ASCII characters. 
    \param pos
        The position at which the search will start.
    \param count
        The number of characters (code points) in the supplied string to use for the comparison.
    \return
        Returns the position of the first character found or returns npos if no matching character was found. 
    */
    size_type find_first_not_of(const char32_t* charArray, size_type pos, size_type count) const
    {
        return d_string.find_first_not_of(charArray, pos, count);
    }

    /*
    \brief
        Finds the first character in this String that is not equal to any of the characters in the given string. The search considers
        only the interval [pos, size()) of this String in the search. The length of the supplied string is determined by the first
        null-character that is encountered.
    \param charArray
        The character array containing the string to search for. 
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character found or returns npos if no matching character was found. 
    */
    size_type find_first_not_of(const char32_t* charArray, size_type pos = 0) const
    {
        return d_string.find_first_not_of(charArray, pos);
    }

    /*
    \brief
        Finds the first character in this String that is not equal to any of the characters in the given string. The search considers
        only the interval [pos, size()) of this String in the search. The length of the supplied string is determined by the first
        null-character that is encountered.
    \param charArray
        The character array containing the string to search for, which may contain either UTF-8 encoded or ASCII characters. 
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character found or returns npos if no matching character was found. 
    */
    size_type find_first_not_of(const char* charArray, size_type pos = 0) const
    {
        return d_string.find_first_not_of(convertUtf8ToUtf32(charArray).data(), pos);
    }

    /*
    \brief
        Finds the first character in this String that is not equal to the character "ch". The search considers only the interval [pos, size())
        of this String in the search.
    \param ch
        The character to search for. 
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character found or returns npos if no matching character was found. 
    */
    size_type find_first_not_of(char32_t ch, size_type pos = 0) const
    {
        return d_string.find_first_not_of(ch, pos);
    }

    /*
    \brief
        Finds the first character in this String that is not equal to the character "ch". The search considers only the interval [pos, size())
        of this String in the search.
    \param ch
        The character to search for. 
    \param pos
        The position at which the search will start.
    \return
        Returns the position of the first character found or returns npos if no matching character was found. 
    */
    size_type find_first_not_of(char ch, size_type pos = 0) const
    {
        return d_string.find_first_not_of(convertUtf8ToUtf32(ch)[0], pos);
    }

    // find_last_not_of

    /*
    \brief
        Finds the last character in this String that is not equal to any of the characters in the given string. The search considers
        only the interval [0; pos] of this String in the search.
    \param str
        The String containing the characters to search for. 
    \param pos
        The position at which the search will stop.
    \return
        Returns the position of the last character found or returns npos if no matching character was found. 
    */
    size_type find_last_not_of(const String& str, size_type pos = npos) const
    {
        return d_string.find_last_not_of(str.d_string, pos);
    }

    /*
    \brief
        Finds the last character in this String that is not equal to any of the characters in the given string. The search considers
        only the interval [0; pos] of this String in the search.
    \param str
        The String containing the characters to search for. 
    \param pos
        The position at which the search will stop.
    \return
        Returns the position of the last character found or returns npos if no matching character was found. 
    */
    size_type find_last_not_of(const std::u32string& str, size_type pos = npos) const
    {
        return d_string.find_last_not_of(str, pos);
    }

    /*
    \brief
        Finds the last character in this String that is not equal to any of the characters in the given string. The search considers
        only the interval [0; pos] of this String in the search.
    \param str
        The String containing the characters to search for, which may contain either UTF-8 encoded or ASCII characters. 
    \param pos
        The position at which the search will stop.
    \return
        Returns the position of the last character found or returns npos if no matching character was found. 
    */
    size_type find_last_not_of(const std::string& str, size_type pos = npos) const
    {
        return d_string.find_last_not_of(convertUtf8ToUtf32(str), pos);
    }

    /*
    \brief
        Finds the last character in this String that is not equal to any of the characters in the given string. The search considers
        only the interval [0; pos] of this String in the search. The string may contain null-characters.
    \param charArray
        The character array containing the string to search for, which may contain either UTF-8 encoded or ASCII characters. 
    \param pos
        The position at which the search will stop.
    \param count
        The number of characters (code points) in the supplied string to use for the comparison.
    \return
        Returns the position of the last character found or returns npos if no matching character was found. 
    */
    size_type find_last_not_of(const char32_t* charArray, size_type pos, size_type count) const
    {
        return d_string.find_last_not_of(charArray, pos, count);
    }

    /*
    \brief
        Finds the last character in this String that is not equal to any of the characters in the given string. The search considers
        only the interval [0; pos] of this String in the search. The length of the supplied string is determined by the last
        null-character that is encountered.
    \param charArray
        The character array containing the string to search for. 
    \param pos
        The position at which the search will stop.
    \return
        Returns the position of the last character found or returns npos if no matching character was found. 
    */
    size_type find_last_not_of(const char32_t* charArray, size_type pos = npos) const
    {
        return d_string.find_last_not_of(charArray, pos);
    }

    /*
    \brief
        Finds the last character in this String that is not equal to any of the characters in the given string. The search considers
        only the interval [0; pos] of this String in the search. The length of the supplied string is determined by the last
        null-character that is encountered.
    \param charArray
        The character array containing the string to search for, which may contain either UTF-8 encoded or ASCII characters. 
    \param pos
        The position at which the search will stop.
    \return
        Returns the position of the last character found or returns npos if no matching character was found. 
    */
    size_type find_last_not_of(const char* charArray, size_type pos = npos) const
    {
        return d_string.find_last_not_of(convertUtf8ToUtf32(charArray).data(), pos);
    }

    /*
    \brief
        Finds the last character in this String that is not equal to the character "ch". The search considers only the interval [0; pos]
        of this String in the search.
    \param ch
        The character to search for. 
    \param pos
        The position at which the search will stop.
    \return
        Returns the position of the last character found or returns npos if no matching character was found. 
    */
    size_type find_last_not_of(char32_t ch, size_type pos = npos) const
    {
        return d_string.find_last_not_of(ch, pos);
    }

    /*
    \brief
        Finds the last character in this String that is not equal to the character "ch". The search considers only the interval [0; pos]
        of this String in the search.
    \param ch
        The character to search for. 
    \param pos
        The position at which the search will stop.
    \return
        Returns the position of the last character found or returns npos if no matching character was found. 
    */
    size_type find_last_not_of(char ch, size_type pos = npos) const
    {
        return d_string.find_last_not_of(convertUtf8ToUtf32(ch)[0], pos);
    }

    /*
    \brief
        Returns the std::u32string that is stored in this String class.
    \return
        A const reference to the std::u32string stored in this String class.
    */
    const std::u32string& getString() const
    {
        return d_string;
    }

    /*
    \brief
        Converts this String to UTF-8 encoding and returns it.
    \return
        A std::string containing the UTF-8 encoded characters converted from
        this String class.
    */
    std::string toUtf8String() const
    {
        return convertUtf32ToUtf8(d_string);
    }

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

    //! The wrapped UTF-32 String object.
    std::u32string d_string;
};


//////////////////////////////////////////////////////////////////////////
// Concatenation operator functions
//////////////////////////////////////////////////////////////////////////
/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const String& lhs, const String& rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const std::string& lhs, const String& rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const String& lhs, const std::string& rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const std::u32string& lhs, const String& rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const String& lhs, const std::u32string& rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const char32_t* lhs, const String& rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const char* lhs, const String& rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(char32_t lhs, const String& rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(char lhs, const String& rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const String& lhs, const char32_t* rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const String& lhs, const char* rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const String& lhs, char32_t rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const String& lhs, char rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(String&& lhs, const String& rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(String&& lhs, const std::string& rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(String&& lhs, const std::u32string& rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const String& lhs, String&& rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const std::string& lhs, String&& rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const std::u32string& lhs, String&& rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(String&& lhs, String&& rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const char32_t* lhs, String&& rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(const char* lhs, String&& rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(char32_t lhs, String&& rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(char lhs, String&& rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(String&& lhs, const char32_t* rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(String&& lhs, const char* rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\return
    A String object that is the concatenation of "lhs" and "rhs"
\exception std::length_error Thrown if the resulting String would be too large.
*/
CEGUIEXPORT String operator+(String&& lhs, char32_t rhs);

/*!
\brief
    Returns a String containing the characters from lhs with the characters from rhs appended. 
\param lhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
\param rhs
    A String, a character, or a pointer to the first character in a null-terminated array of characters.
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
        return std::hash<std::u32string>()(str.c_str());
    }
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#elif CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_STD

namespace CEGUI
{
typedef std::string String;
}

#if defined(_MSC_VER)
#	pragma warning(disable : 4251)
#endif

#endif

#endif
