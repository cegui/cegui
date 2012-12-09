/************************************************************************
    filename:   CEGUISubscriberSlot.h
    created:    Tue Feb 28 2006
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUISubscriberSlot_h_
#define _CEGUISubscriberSlot_h_

#include "CEGUIBase.h"
#include "CEGUIFreeFunctionSlot.h"
#include "CEGUIFunctorCopySlot.h"
#include "CEGUIFunctorReferenceSlot.h"
#include "CEGUIFunctorPointerSlot.h"
#include "CEGUIMemberFunctionSlot.h"
#include "CEGUIFunctorReferenceBinder.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
    SubscriberSlot class which is used when subscribing to events.

    For many uses, the construction of the SubscriberSlot may be implicit, so
    you do not have to specify Subscriber in your subscription calls. Notable
    exceptions are for subscribing member functions and references to functor
    objects.
*/
class CEGUIEXPORT SubscriberSlot
{
public:
    /*!
    \brief
        Default constructor.  Creates a SubscriberSlot with no bound slot.
    */
    SubscriberSlot();

    /*!
    \brief
        Creates a SubscriberSlot that is bound to a free function.
    */
    SubscriberSlot(FreeFunctionSlot::SlotFunction* func);

    /*!
    \brief
        Destructor.  Note this is non-virtual, which should be telling you not
        to sub-class!
    */
    ~SubscriberSlot();

    /*!
    \brief
        Invokes the slot functor that is bound to this Subscriber.  Returns
        whatever the slot returns, unless there is not slot bound when false is
        always returned.
    */
    bool operator()(const EventArgs& args) const
    {
        return (*d_functor_impl)(args);
    }

    /*!
    \brief
        Returns whether the SubscriberSlot is internally connected (bound).
    */
    bool connected() const
    {
        return d_functor_impl != 0;
    }

    /*!
    \brief
        Disconnects the slot internally and performs any required cleanup
        operations.
    */
    void cleanup();

    // templatised constructors
    /*!
    \brief
        Creates a SubscriberSlot that is bound to a member function.
    */
    template<typename T>
    SubscriberSlot(bool (T::*function)(const EventArgs&), T* obj) :
        d_functor_impl(new MemberFunctionSlot<T>(function, obj))
    {}

    /*!
    \brief
        Creates a SubscriberSlot that is bound to a functor object reference.
    */
    template<typename T>
    SubscriberSlot(const FunctorReferenceBinder<T>& binder) :
        d_functor_impl(new FunctorReferenceSlot<T>(binder.d_functor))
    {}

    /*!
    \brief
        Creates a SubscriberSlot that is bound to a copy of a functor object.
    */
    template<typename T>
    SubscriberSlot(const T& functor) :
        d_functor_impl(new FunctorCopySlot<T>(functor))
    {}

    /*!
    \brief
        Creates a SubscriberSlot that is bound to a functor pointer.
    */
    template<typename T>
    SubscriberSlot(T* functor) :
        d_functor_impl(new FunctorPointerSlot<T>(functor))
    {}

private:
    //! Points to the internal functor object to which we are bound
    SlotFunctorBase* d_functor_impl;
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUISubscriberSlot_h_
