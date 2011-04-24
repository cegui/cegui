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

	/*!
	Template madness to sort out getters returning ref T, const ref T or just T.
	*/
	struct GetterFunctor
	{	
		template<typename DT> struct EnsurePlain { typedef DT result; };
		template<typename DT> struct EnsurePlain<DT&> { typedef DT result; };
		template<typename DT> struct EnsurePlain<const DT&> { typedef DT result; };

		template<typename DT> struct EnsureRef { typedef DT& result; };
		template<typename DT> struct EnsureRef<DT&> { typedef DT& result; };
		template<typename DT> struct EnsureRef<const DT&> { typedef DT& result; };

		template<typename DT> struct EnsureConstRef { typedef const DT& result; };
		template<typename DT> struct EnsureConstRef<DT&> { typedef const DT& result; };
		template<typename DT> struct EnsureConstRef<const DT&> { typedef const DT& result; };

		typedef typename EnsurePlain<typename Helper::safe_method_return_type>::result (C::*PlainGetter)() const;
		typedef typename EnsureRef<typename Helper::safe_method_return_type>::result (C::*RefGetter)() const;
		typedef typename EnsureConstRef<typename Helper::safe_method_return_type>::result (C::*ConstRefGetter)() const;

		GetterFunctor(PlainGetter getter):
			d_plainGetter(getter)
			//d_refGetter(0), no need to initialise these, we will never use them
			//d_constRefGetter(0)
		{}

		GetterFunctor(RefGetter getter):
			d_plainGetter(0),
			d_refGetter(getter)
			//d_constRefGetter(0) no need to initialise this, we will never use it
		{}

		GetterFunctor(ConstRefGetter getter):
			d_plainGetter(0),
			d_refGetter(0),
			d_constRefGetter(getter)
		{}

		typename Helper::safe_method_return_type operator()(const C* instance) const
		{
			// FIXME: Ideally we want this to be done during compilation, not runtime

			if (d_plainGetter)
				return CEGUI_CALL_MEMBER_FN(*instance, d_plainGetter)();
			if (d_refGetter)
				return CEGUI_CALL_MEMBER_FN(*instance, d_refGetter)();
			if (d_constRefGetter)
				return CEGUI_CALL_MEMBER_FN(*instance, d_constRefGetter)();

			assert(false);
			// just to get rid of the warning
			return CEGUI_CALL_MEMBER_FN(*instance, d_plainGetter)();
		}

		PlainGetter d_plainGetter;
		RefGetter d_refGetter;
		ConstRefGetter d_constRefGetter;
	};

    // do we want less bug prone code but a bit slower (string conversion for default values at construction) or faster
    // but more typo prone (passing string default value)
    TplProperty(const String& name, const String& help, const String& origin, Setter setter, GetterFunctor getter, typename Helper::pass_type defaultValue = T(), bool writesXML = true):
        TypedProperty<T>(name, help, origin, defaultValue, writesXML),
        
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
    virtual typename Helper::safe_method_return_type getNative(const PropertyReceiver* receiver) const
    {
        const C* instance = static_cast<const C*>(receiver);
		return d_getter(instance);
    }
    
private:
    Setter d_setter;
    GetterFunctor d_getter;
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUITplProperty_h_
