/************************************************************************
    filename:   CEGUIItemListbox.h
    created:    Tue Sep 27 2005
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
#ifndef _CEGUIItemListbox_h_
#define _CEGUIItemListbox_h_

#include "elements/CEGUIScrolledItemListBase.h"
#include "elements/CEGUIItemListboxProperties.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

// begin CEGUI namespace
namespace CEGUI
{

/*!
\brief
    ItemListbox window class
*/
class CEGUIEXPORT ItemListbox : public ScrolledItemListBase
{
public:
    static const String EventNamespace; //!< Namespace for global events
    static const String WidgetTypeName; //!< Window factory name

    /************************************************************************
        Constants
    *************************************************************************/
    static const String EventSelectionChanged; //!< Event fired when the selection changes.
    static const String EventMultiSelectModeChanged; //!< Event fired when the multiselect mode changes.

    /************************************************************************
        Accessors
    *************************************************************************/
    /*!
    \brief
        Return the number of selected items in this ItemListbox
    */
    size_t getSelectedCount(void) const;

    /*!
    \brief
        Returns a pointer to the first selected item
    */
    ItemEntry* getFirstSelectedItem(void) const;

    /*!
    \brief
        Returns a pointer to the next selected item after the item 'start_item' given.
    */
    ItemEntry* getNextSelectedItem(const ItemEntry* start_item) const;

    /*!
    \brief
        Returns 'true' if multiple selections are allowed. 'false' if not.
    */
    bool isMultiSelectEnabled(void) const   {return d_multiSelect;}

    /*!
    \brief
        Returns 'true' if the item at the given index is selectable and currently selected.
    */
    bool isItemSelected(size_t index) const;

    /************************************************************************
        Manipulators
    *************************************************************************/
    /*!
    \brief
        Set whether or not multiple selections should be allowed.
    */
    void setMultiSelectEnabled(bool state);

    /*!
    \brief
        Clears all selections.
    */
    void clearAllSelections(void);

    /*!
    \brief
        Select a range of items.

    \param a
        Start item. (inclusive)

    \param z
        End item. (inclusive)
    */
    void selectRange(size_t a, size_t z);

    /*!
    \brief
        Select all items.
        Does nothing if multiselect is disabled.
    */
    void selectAllItems(void);

    /************************************************************************
        Object Construction and Destruction
    *************************************************************************/
    /*!
    \brief
        Constructor for the ItemListbox base class constructor.
    */
    ItemListbox(const String& type, const String& name);

    /*!
    \brief
        Destructor for the ItemListbox base class.
     */
    virtual ~ItemListbox(void) {}

    /************************************************************************
        Implementation functions
    ************************************************************************/
    /*!
    \brief
        Setup size and position for the item widgets attached to this ItemListbox
    */
    virtual void layoutItemWidgets();

    /*!
    \brief
        Returns the Size in unclipped pixels of the content attached to this ItemListbox.
    */
    virtual Size getContentSize() const;

    /*!
    \brief
        Return whether this window was inherited from the given class name at some point in the inheritance heirarchy.

    \param class_name
        The class name that is to be checked.

    \return
        true if this window was inherited from \a class_name. false if not.
    */
    virtual bool testClassName_impl(const String& class_name) const
    {
        if (class_name=="ItemListbox")
        {
            return true;
        }
        return ScrolledItemListBase::testClassName_impl(class_name);
    }

    /*!
    \brief
        Notify this ItemListbox that the given ListItem was just clicked.
        Internal function - not to be used from client code.
    */
    virtual void notifyItemClicked(ItemEntry* li);

    /*!
    \brief
        Notify this ItemListbox that the given ListItem just changed selection state.
        Internal function - not to be used from client code.
    */
    virtual void notifyItemSelectState(ItemEntry* li, bool state);

protected:
    /************************************************************************
        New event handlers
    ************************************************************************/
    virtual void onSelectionChanged(WindowEventArgs& e);
    virtual void onMultiSelectModeChanged(WindowEventArgs& e);

    /************************************************************************
        Overridden event handlers
    ************************************************************************/
    virtual void onKeyDown(KeyEventArgs& e);

    /************************************************************************
        Static Properties for this class
    ************************************************************************/
    static ItemListboxProperties::MultiSelect d_multiSelectProperty;

    /************************************************************************
        Implementation data
    ************************************************************************/
    bool d_multiSelect;
    ItemEntry* d_lastSelected;

private:
    void addItemListboxProperties(void);
    void addItemListboxEvents(void);
};

} // end CEGUI namespace

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif // end of guard _CEGUIItemListbox_h_
