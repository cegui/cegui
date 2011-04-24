/***********************************************************************
    filename:   CEGUIFalPropertyDefinition.cpp
    created:    Sun Jun 26 2005
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
#include "falagard/CEGUIFalPropertyDefinition.h"
#include "CEGUIWindow.h"
#include "CEGUIExceptions.h"
#include "CEGUILogger.h"
#include "CEGUIWindowManager.h"
#include <iostream>

// Start of CEGUI namespace section
namespace CEGUI
{
    PropertyDefinition::PropertyDefinition(const String& name,
                                           const String& initialValue,
                                           bool redrawOnWrite,
                                           bool layoutOnWrite)
      : PropertyDefinitionBase(name,
                               "Falagard custom property definition - gets/sets a named user string.",
                               initialValue, redrawOnWrite,
                               layoutOnWrite),
        d_userStringName(name + "_fal_auto_prop__")
    {
    }

    PropertyDefinition::PropertyDefinition(const String& name,
                                           const String& initialValue,
                                           const String& help,
                                           bool redrawOnWrite,
                                           bool layoutOnWrite)
        : PropertyDefinitionBase(name, help, initialValue, redrawOnWrite,
                                 layoutOnWrite),
          d_userStringName(name + "_fal_auto_prop__")
    {
    }

    // abstract members from Property
    String PropertyDefinition::get(const PropertyReceiver* receiver) const
    {
        const Window* const wnd = static_cast<const Window*>(receiver);

        // the try/catch is used instead of querying the existence of the user
        // string in order that for the 'usual' case - where the user string
        // exists - there is basically no additional overhead, and that any
        // overhead is incurred only for the initial creation of the user
        // string.
        // Maybe the only negative here is that an error gets logged, though
        // this can be treated as a 'soft' error.
        CEGUI_TRY
        {
            return wnd->getUserString(d_userStringName);
        }
        CEGUI_CATCH (UnknownObjectException&)
        {
            Logger::getSingleton().logEvent(
                "PropertyDefiniton::get: Defining new user string: " +
                d_userStringName);

            // HACK: FIXME: TODO: This const_cast is basically to allow the
            // above mentioned performance measure; the alternative would be
            // to just return d_default, and while technically correct, it
            // would be very slow.
            const_cast<Window*>(wnd)->
                setUserString(d_userStringName, d_default);

            return d_default;
        }
    }

    void PropertyDefinition::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<Window*>(receiver)->setUserString(d_userStringName, value);
        PropertyDefinitionBase::set(receiver, value);
    }

    void PropertyDefinition::writeXMLElementType(XMLSerializer& xml_stream) const
    {
        xml_stream.openTag("PropertyDefinition");
    }

} // End of  CEGUI namespace section
