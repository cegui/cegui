/************************************************************************
    created:    Tue Feb 28 2006
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2014 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIFunctorCopySlot_h_
#define _CEGUIFunctorCopySlot_h_

#include "CEGUI/SlotFunctorBase.h"

// Start of CEGUI namespace section
namespace CEGUI
{


/*!
\brief
    Slot template class that creates a functor that calls back via a copy of a
    functor object.
*/
template<typename T>
class FunctorCopySlot : public SlotFunctorBase
{
public:
    FunctorCopySlot(const T& functor):
        d_functor(functor)
    {}

    virtual bool operator()(const EventArgs& args)
    {
        return call(&T::operator(), args);
    }

private:
    T d_functor;

    // The following is just overload trickery to accomodate 8 allowed
    // operator() signatures:
    //
    // 1) bool (const EventArgs&)
    // 2) bool (const EventArgs&) const
    // 3) void (const EventArgs&)
    // 4) void (const EventArgs&) const
    // 5) bool ()
    // 6) bool () const
    // 7) void ()
    // 8) void () const

    typedef bool(T::*Op)(const EventArgs&);
    typedef bool(T::*OpConst)(const EventArgs&)const;

    inline bool call(Op member_fn, const EventArgs& args)
    {
        return CEGUI_CALL_MEMBER_FN(d_functor, member_fn)(args);
    }

    inline bool call(OpConst member_fn, const EventArgs& args)
    {
        return CEGUI_CALL_MEMBER_FN(d_functor, member_fn)(args);
    }

    typedef void(T::*OpVoid)(const EventArgs&);
    typedef void(T::*OpVoidConst)(const EventArgs&)const;

    inline bool call(OpVoid member_fn, const EventArgs& args)
    {
        CEGUI_CALL_MEMBER_FN(d_functor, member_fn)(args);

        return true;
    }

    inline bool call(OpVoidConst member_fn, const EventArgs& args)
    {
        CEGUI_CALL_MEMBER_FN(d_functor, member_fn)(args);

        return true;
    }

    typedef bool(T::*OpNoArgs)();
    typedef bool(T::*OpNoArgsConst)()const;

    inline bool call(OpNoArgs member_fn, const EventArgs& /*args*/)
    {
        return CEGUI_CALL_MEMBER_FN(d_functor, member_fn)();
    }

    inline bool call(OpNoArgsConst member_fn, const EventArgs& /*args*/)
    {
        return CEGUI_CALL_MEMBER_FN(d_functor, member_fn)();
    }

    typedef void(T::*OpVoidNoArgs)();
    typedef void(T::*OpVoidNoArgsConst)()const;

    inline bool call(OpVoidNoArgs member_fn, const EventArgs& /*args*/)
    {
        CEGUI_CALL_MEMBER_FN(d_functor, member_fn)();

        return true;
    }

    inline bool call(OpVoidNoArgsConst member_fn, const EventArgs& /*args*/)
    {
        CEGUI_CALL_MEMBER_FN(d_functor, member_fn)();

        return true;
    }
};

}

#endif  // end of guard _CEGUIFunctorCopySlot_h_
