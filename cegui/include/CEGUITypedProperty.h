/***********************************************************************
    filename:   CEGUITypedProperty.h
    created:    24/11/2010
    author:     Martin Preisler
    
    purpose:    Defines the TypedProperty base class which allows native
                property setting / getting whilst still falling back
                to Strings gracefully
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
#ifndef _CEGUITypedProperty_h_
#define _CEGUITypedProperty_h_

#include "CEGUIProperty.h"
#include "CEGUIPropertyHelper.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief base class for properties able to do native set/get

\todo I split this from CEGUIProperty.h to avoid bloating it with PropertyHelper,
I also split it from TplProperty since it can be used outside of that.
Is that the "right thing" to do?
*/
template<typename T>
class TypedProperty : public Property
{
public:
    typedef PropertyHelper<T> Helper;
    
    // TODO: do we want less bug prone code but a bit slower (string conversion for default values at construction) or faster
    //       but more typo prone (passing string default value)?
    TypedProperty(const String& name, const String& help, typename Helper::pass_type defaultValue = T(), bool writesXML = true):
        Property(name, help, Helper::toString(defaultValue), writesXML, Helper::getDataTypeName())
    {}
    
    virtual ~TypedProperty()
    {}
    
    //! \copydoc Property::get
    virtual String get(const PropertyReceiver* receiver) const
    {
        return Helper::toString(getNative(receiver));
    }

    //! \copydoc Property::set
    virtual void set(PropertyReceiver* receiver, const String& value)
    {
        setNative(receiver, Helper::fromString(value));
    }
    
    /*!
    \brief native set method, sets the property given a native type
    
    \see Property::set
    */
    virtual void setNative(PropertyReceiver* receiver, typename Helper::pass_type value) = 0;

    /*!
    \brief native get method, returns the native type by copy
    
    \see Property::get
    */
    virtual typename Helper::return_type getNative(const PropertyReceiver* receiver) const = 0;
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUITypedProperty_h_
