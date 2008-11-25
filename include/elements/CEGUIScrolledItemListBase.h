/************************************************************************
    filename:   CEGUIIScrolledItemListBase.h
    created:    Sat Oct 29 2005
    author:     Tomas Lindquist Olsen
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#ifndef _CEGUIScrolledItemListBase_h_
#define _CEGUIScrolledItemListBase_h_

#include "elements/CEGUIItemListBase.h"
#include "elements/CEGUIScrollbar.h"
#include "elements/CEGUIScrolledItemListBaseProperties.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif
// begin CEGUI namespace
namespace CEGUI
{

/*!
\brief
    ScrolledItemListBase window class
*/
class CEGUIEXPORT ScrolledItemListBase : public ItemListBase
{
public:
    static const String EventNamespace; //!< Namespace for global events

    /************************************************************************
        Constants
    *************************************************************************/
    static const String VertScrollbarNameSuffix; //!< Name suffix for vertical scrollbar component
    static const String HorzScrollbarNameSuffix; //!< Name suffix for horizontal scrollbar component
    static const String ContentPaneNameSuffix;   //!< Name suffix for the content pane component

    static const String EventVertScrollbarModeChanged; //!< Event fired when the vertical scroll bar mode changes.
    static const String EventHorzScrollbarModeChanged; //!< Event fired when the horizontal scroll bar mode change.

    /************************************************************************
        Accessors
    *************************************************************************/
    /*!
    \brief
        Returns whether the vertical scrollbar is being forced visible. Despite content size.
    */
    bool isVertScrollbarAlwaysShown(void) const {return d_forceVScroll;}

    /*!
    \brief
        Returns whether the horizontal scrollbar is being forced visible. Despite content size.
    */
    bool isHorzScrollbarAlwaysShown(void) const {return d_forceHScroll;}

    /*!
    \brief
        Get the vertical scrollbar component attached to this window.
    */
    Scrollbar* getVertScrollbar() const;

    /*!
    \brief
        Get the horizontal scrollbar component attached to this window.
    */
    Scrollbar* getHorzScrollbar() const;

    /************************************************************************
        Manipulators
    *************************************************************************/
    /*!
    \brief
        Sets whether the vertical scrollbar should be forced visible. Despite content size.
    */
    void setShowVertScrollbar(bool mode);

    /*!
    \brief
        Sets whether the horizontal scrollbar should be forced visible. Despite content size.
    */
    void setShowHorzScrollbar(bool mode);

    /*!
    \brief
        Scroll the vertical list position if needed to ensure that the ItemEntry
        \a item is, if possible,  fully visible witin the ScrolledItemListBase
        viewable area.

    \param item
        const reference to an ItemEntry attached to this ScrolledItemListBase
        that should be made visible in the view area.

    \return
        Nothing.
    */
    void ensureItemIsVisibleVert(const ItemEntry& item);

    /*!
    \brief
        Scroll the horizontal list position if needed to ensure that the
        ItemEntry \a item is, if possible, fully visible witin the
        ScrolledItemListBase viewable area.

    \param item
        const reference to an ItemEntry attached to this ScrolledItemListBase
        that should be made visible in the view area.

    \return
        Nothing.
    */
    void ensureItemIsVisibleHorz(const ItemEntry& item);

    /************************************************************************
        Object Construction and Destruction
    *************************************************************************/
    /*!
    \brief
        Constructor for the ScrolledItemListBase base class constructor
    */
    ScrolledItemListBase(const String& type, const String& name);

    /*!
    \brief
        Destructor for the ScrolledItemListBase base class.
     */
    virtual ~ScrolledItemListBase(void);

    // overridden from ItemListBase
    virtual void initialiseComponents(void);

protected:
    /************************************************************************
        Implementation functions
    ************************************************************************/
    /*!
    \brief
        Return whether this window was inherited from the given class name at some point in the inheritance hierarchy.

    \param class_name
        The class name that is to be checked.

    \return
        true if this window was inherited from \a class_name. false if not.
    */
    virtual bool testClassName_impl(const String& class_name) const
    {
        if (class_name=="ScrolledItemListBase")
        {
            return true;
        }
        return ItemListBase::testClassName_impl(class_name);
    }

    /*!
    \brief
        Configure scrollbars
    */
    void configureScrollbars(const Size& doc_size);

    /************************************************************************
        New event handlers
    ************************************************************************/
    virtual void onVertScrollbarModeChanged(WindowEventArgs& e);
    virtual void onHorzScrollbarModeChanged(WindowEventArgs& e);

    /************************************************************************
        Overridden event handlers
    ************************************************************************/
    virtual void onMouseWheel(MouseEventArgs& e);

    /************************************************************************
        Event subscribers
    ************************************************************************/
    bool handle_VScroll(const EventArgs& e);
    bool handle_HScroll(const EventArgs& e);

    /************************************************************************
        Implementation data
    ************************************************************************/
    bool d_forceVScroll;
    bool d_forceHScroll;

private:
    /************************************************************************
        Static Properties for this class
    ************************************************************************/
    static ScrolledItemListBaseProperties::ForceVertScrollbar d_forceVertScrollbarProperty;
    static ScrolledItemListBaseProperties::ForceHorzScrollbar d_forceHorzScrollbarProperty;

    void addScrolledItemListBaseProperties(void);
};

} // end CEGUI namespace

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif // end of guard _CEGUIScrolledItemListBase_h_
