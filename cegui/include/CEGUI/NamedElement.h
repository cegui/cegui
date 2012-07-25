/***********************************************************************
    filename:   CEGUI/NamedElement.h
    created:    30/10/2011
    author:     Martin Preisler

    purpose:    Adds naming and name path traversal to Element
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUINamedElement_h_
#define _CEGUINamedElement_h_

#include "CEGUI/Element.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
    EventArgs based class that is used for objects passed to handlers triggered for events
    concerning some NamedElement object.
*/
class CEGUIEXPORT NamedElementEventArgs : public EventArgs
{
public:
    NamedElementEventArgs(NamedElement* element):
        element(element)
    {}

    NamedElement* element;     //!< pointer to an Element object of relevance to the event.
};

/*!
\brief Adds name to the Element class, including name path traversal
*/
class CEGUIEXPORT NamedElement :
    public Element
{
public:
    //! Namespace for global events
    static const String EventNamespace;

    // generated internally by NamedElement
    /** Event fired when the Element name has changed.
     * Handlers are passed a const NamedElementEventArgs reference with
     * NamedElementEventArgs::element set to the Element whose name was changed.
     */
    static const String EventNameChanged;
    
    /*!
    \brief Constructor
    
    \param name The initial name this element will have
    */
    NamedElement(const String& name = "");

    /*!
    \brief Destructor
    */
    virtual ~NamedElement();
    
    /*!
    \brief
        Renames the element.

    \param name
        String object holding the new name for the element.

    \exception AlreadyExistsException
        thrown if an element named \a name already exists in the parent of this element.
    */
    virtual void setName(const String& name);
    
    /*!
    \brief
        return a String object holding the name of this Element.
    */
    inline const String& getName() const
    {
        return d_name;
    }
    
    /**
    \brief
        return a String object that describes the name path for this Element.

        A name path is a string that describes a path down the element
        hierarchy using element names and the forward slash '/' as a separator.
    */
    String getNamePath() const;
    
    using Element::isChild;
    /*!
    \brief
        returns whether the specified name path references a NamedElement that is
        currently attached to this Element.

        A name path is a string that describes a path down the element
        hierarchy using names and the forward slash '/' as a separator.
        \par
        For example, if this element has a child attached to it named "Panel"
        which has its own children attached named "Okay" and "Cancel",
        you can check for the element "Okay" from this element by using the
        name path "Panel/Okay".  To check for "Panel", you would simply pass
        the name "Panel".

    \param name_path
        String object holding the name path of the child element to test.

    \return
         - true if the element referenced by \a name_path is attached.
         - false if the element referenced by \a name_path is not attached.
    */
    bool isChild(const String& name_path) const;
    
    using Element::isAncestor;
    /*!
    \brief
        return true if the specified element name is a name of some ancestor of this Element

    \param name
        String object holding the name to check for.

    \return
        - true if an element named \a name is an ancestor (parent, or parent of
          parent, etc) of this element.
        - false if an element named \a name is in no way an ancestor of this
          element.
    */
    bool isAncestor(const String& name) const;
    
    /*!
    \brief
        return the attached child element that the given name path references.

        A name path is a string that describes a path down the element
        hierarchy using names and the forward slash '/' as a separator.
        \par
        For example, if this element has a child attached to it named "Panel"
        which has its own children attached named "Okay" and "Cancel",
        you can check for the element "Okay" from this element by using the
        name path "Panel/Okay".  To check for "Panel", you would simply pass
        the name "Panel".

    \param name_path
        String object holding the name path of the child element to return.

    \return
        the NamedElement object referenced by \a name_path.

    \exception UnknownObjectException
        thrown if \a name_path does not reference an Element attached to this
        Element.
    */
    NamedElement* getChildElement(const String& name_path) const;
    
    using Element::removeChild;
    /*!
    \brief
        Remove the Element referenced by the given name path from this Element's
        child list.

    \param name_path
        String the name path that references the the Element to be removed.
        If the Element specified is not attached to this Window,
        UnknownObjectException is thrown
    */
    void removeChild(const String& name_path);

protected:
    //! \copydoc Element::addChild_impl
    virtual void addChild_impl(Element* element);
    
    /*!
    \brief
        retrieves a child at \a name_path or 0 if none such exists
    */
    virtual NamedElement* getChildByNamePath_impl(const String& name_path) const;
    
    /*!
    \brief
        Add standard CEGUI::NamedElement properties.
    */
    void addNamedElementProperties();
    
    /*!
    \brief
        Handler called when the element's name changes.

    \param e
        NamedElementEventArgs object whose 'element' pointer field is set to the element
        that triggered the event. For this event the trigger element is always
        'this'.
    */
    virtual void onNameChanged(NamedElementEventArgs& e);
    
    //! The name of the element, unique in the parent of this element
    String d_name;

private:
    /*************************************************************************
        May not copy or assign Element objects
    *************************************************************************/
    NamedElement(const Element&) {}
    NamedElement& operator=(const NamedElement&) {return *this;}
};

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUINamedElement_h_
