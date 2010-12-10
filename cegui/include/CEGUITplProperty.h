/***********************************************************************
    filename:   CEGUITplProperty.h
    created:    23/11/2010
    author:     Martin Preisler

    purpose:    Finger saving template property
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
#ifndef _CEGUITplProperty_h_
#define _CEGUITplProperty_h_

#include "CEGUITypedProperty.h"
#include <iostream>

// Start of CEGUI namespace section
namespace CEGUI
{

// improve readability - http://www.parashift.com/c++-faq-lite/pointers-to-members.html#faq-33.6
#define CEGUI_CALL_MEMBER_FN(object, ptrToMember) ((object).*(ptrToMember)) 
    
template<class C, typename T>
class TplProperty : public TypedProperty<T>
{
public:
    typedef PropertyHelper<T> Helper;
    
    typedef void (C::*Setter)(typename Helper::pass_type);
    typedef typename Helper::return_type (C::*Getter)() const;
    
    // do we want less bug prone code but a bit slower (string conversion for default values at construction) or faster
    // but more typo prone (passing string default value)
    TplProperty(const String& name, const String& help, Setter setter, Getter getter, typename Helper::pass_type defaultValue = T(), bool writesXML = true):
        TypedProperty<T>(name, help, defaultValue, writesXML),
        
        d_setter(setter),
        d_getter(getter)
    {}
    
    virtual ~TplProperty()
    {}
    
    //! \copydoc TypedProperty::setNative
    virtual void setNative(PropertyReceiver* receiver, typename Helper::pass_type value)
    {
        C* instance = static_cast<C*>(receiver);
        CEGUI_CALL_MEMBER_FN(*instance, d_setter)(value);
    }

    //! \copydoc TypedProperty::getNative
    virtual typename Helper::return_type getNative(const PropertyReceiver* receiver) const
    {
        const C* instance = static_cast<const C*>(receiver);
        return CEGUI_CALL_MEMBER_FN(*instance, d_getter)();
    }
    
private:
    Setter d_setter;
    Getter d_getter;
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUITplProperty_h_
