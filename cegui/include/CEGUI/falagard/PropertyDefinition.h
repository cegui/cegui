/***********************************************************************
    filename:   CEGUIPropertyDefinition.h
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
#ifndef _CEGUIFalPropertyDefinition_h_
#define _CEGUIFalPropertyDefinition_h_

#include "./PropertyDefinitionBase.h"
#include "../Logger.h"
// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        Class representing a generic get/set property.
    */
template <typename T>
    class PropertyDefinition : public PropertyDefinitionBase<T>
    {
    public:
        typedef typename PropertyDefinitionBase<T>::Helper Helper;

        //! Constructor.
        PropertyDefinition(const String& name, const String& initialValue,
                           const String& help, const String& origin,
                           bool redrawOnWrite = false, bool layoutOnWrite = false)
        : PropertyDefinitionBase<T>(name, help, initialValue, origin, redrawOnWrite,
                                   layoutOnWrite),
            d_userStringName(name + "_fal_auto_prop__")
      {
      }
        virtual ~PropertyDefinition() {}
    protected:
        typename Helper::safe_method_return_type getNative_impl(const PropertyReceiver* receiver) const
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
                return Helper::fromString(wnd->getUserString(d_userStringName));
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
                    setUserString(d_userStringName, TypedProperty<T>::d_default);

                return Helper::fromString(TypedProperty<T>::d_default);
            }
        }
        virtual void setNative_impl(PropertyReceiver* receiver,typename Helper::pass_type value)
        {
            static_cast<Window*>(receiver)->setUserString(d_userStringName, Helper::toString(value));
            PropertyDefinitionBase<T>::setNative_impl(receiver, value);
        }

        void writeFalagardXMLElementType(XMLSerializer& xml_stream) const
        {
            xml_stream.openTag("PropertyDefinition");
        }

        String d_userStringName;
    };

} // End of  CEGUI namespace section


#endif  // end of guard _CEGUIFalPropertyDefinition_h_
