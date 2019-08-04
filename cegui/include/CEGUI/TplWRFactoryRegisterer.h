/***********************************************************************
    created:    Thu Mar 19 2009
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUITplWRFactoryRegisterer_h_
#define _CEGUITplWRFactoryRegisterer_h_

#include "CEGUI/FactoryRegisterer.h"
#include "CEGUI/WindowRendererManager.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Template based implementation of FactoryRegisterer that allows easy
    registration of a factory for any WindowRenderer type.
*/
template <typename T>
class TplWRFactoryRegisterer : public FactoryRegisterer
{
public:
    //! Constructor.
    TplWRFactoryRegisterer();

    void unregisterFactory() const override;

protected:
    void doFactoryAdd() const override;
    bool isAlreadyRegistered() const override;
};


//----------------------------------------------------------------------------//
template <typename T>
TplWRFactoryRegisterer<T>::TplWRFactoryRegisterer() :
    FactoryRegisterer(T::TypeName)
{}

//----------------------------------------------------------------------------//
template <typename T>
void TplWRFactoryRegisterer<T>::unregisterFactory() const
{
    WindowRendererManager::getSingleton().removeFactory(d_type);
}

//----------------------------------------------------------------------------//
template <typename T>
void TplWRFactoryRegisterer<T>::doFactoryAdd() const
{
    WindowRendererManager::addWindowRendererType<T>();
}

//----------------------------------------------------------------------------//
template <typename T>
bool TplWRFactoryRegisterer<T>::isAlreadyRegistered() const
{
    return WindowRendererManager::getSingleton().isFactoryPresent(d_type);
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUITplWRFactoryRegisterer_h_

