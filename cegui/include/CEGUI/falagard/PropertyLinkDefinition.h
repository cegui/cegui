/***********************************************************************
    filename:   CEGUIPropertyLinkDefinition.h
    created:    Sat Oct 8 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
#ifndef _CEGUIFalPropertyLinkDefinition_h_
#define _CEGUIFalPropertyLinkDefinition_h_

#include "./PropertyDefinitionBase.h"
#include "../IteratorBase.h"

#include <vector>

#if defined (_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        Class representing a property that links to another property defined on
        an attached child widget.
    */
    class CEGUIEXPORT PropertyLinkDefinition : public PropertyDefinitionBase
    {
    public:
        PropertyLinkDefinition(const String& propertyName, const String& widgetName, const String& targetProperty, const String& initialValue, bool redrawOnWrite, bool layoutOnWrite);

        //! add a new link target to \a property on \a widget (name).
        void addLinkTarget(const String& widget, const String& property);
        //! clear all link targets from this link definition.
        void clearLinkTargets();

        // return whether a the given widget / property pair is a target of this property link.
        bool isTargetProperty(const String& widget, const String& property) const;

        // override members from PropertyDefinitionBase
        String get(const PropertyReceiver* receiver) const;
        void set(PropertyReceiver* receiver, const String& value);

    protected:
        void writeXMLElementType(XMLSerializer& xml_stream) const;
        void writeXMLAttributes(XMLSerializer& xml_stream) const;

        /*!
        \brief
            return a pointer to the window containing the target property to
            be accessed.

        \exception UnknownObjectException
            thrown if no such target window exists within the system.

        \deprecated
            This will be removed in 0.8.x.  Use the version taking a suffix
            string instead!
        */
        const Window* getTargetWindow(const PropertyReceiver* receiver) const;

        /*!
        \deprecated
            This will be removed in 0.8.x.  Use the version taking a suffix
            string instead!
        */
        Window* getTargetWindow(PropertyReceiver* receiver);

        //! Return a pointer to the target window with the given name.
        const Window* getTargetWindow(const PropertyReceiver* receiver,
                                      const String& name) const;

        //! Return a pointer to the target window with the given name.
        Window* getTargetWindow(PropertyReceiver* receiver,
                                const String& name);

        typedef std::pair<String,String> StringPair;
        //! type used for the collection of targets.
        typedef std::vector<StringPair CEGUI_VECTOR_ALLOC(StringPair)> LinkTargetCollection;

        //! collection of targets for this PropertyLinkDefinition.
        LinkTargetCollection d_targets;
    public:
        typedef ConstVectorIterator<LinkTargetCollection> LinkTargetIterator;

        LinkTargetIterator getLinkTargetIterator() const;


    };

} // End of  CEGUI namespace section

#if defined (_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _CEGUIFalPropertyLinkDefinition_h_
