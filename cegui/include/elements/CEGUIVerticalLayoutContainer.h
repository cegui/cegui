/***********************************************************************
    filename:   CEGUIVerticalLayoutContainer.h
    created:    29/7/2010
    author:     Martin Preisler

    purpose:    Interface to a vertical layout container
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
#ifndef _CEGUIVerticalLayoutContainer_h_
#define _CEGUIVerticalLayoutContainer_h_

#include "CEGUISequentialLayoutContainer.h"
#include "../CEGUIWindowFactory.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    A Layout Container window layouting it's children vertically
*/
class CEGUIEXPORT VerticalLayoutContainer : public SequentialLayoutContainer
{
public:
    /*************************************************************************
        Constants
    *************************************************************************/
    //! The unique typename of this widget
    static const String WidgetTypeName;

    /*************************************************************************
        Construction and Destruction
    *************************************************************************/
    /*!
    \brief
        Constructor for GUISheet windows.
    */
    VerticalLayoutContainer(const String& type, const String& name);

    /*!
    \brief
        Destructor for GUISheet windows.
    */
    virtual ~VerticalLayoutContainer(void);

    //! @copydoc LayoutContainer::layout
    virtual void layout();

protected:
    /*!
    \brief
        Return whether this window was inherited from the given class name at
        some point in the inheritance hierarchy.

    \param class_name
        The class name that is to be checked.

    \return
        true if this window was inherited from \a class_name. false if not.
    */
    virtual bool    testClassName_impl(const String& class_name) const
    {
        if (class_name == "VerticalLayoutContainer")  return true;

        return LayoutContainer::testClassName_impl(class_name);
    }
};

} // End of  CEGUI namespace section


#endif  // end of guard _CEGUIVerticalLayoutContainer_h_

