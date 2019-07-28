/***********************************************************************
    created:    27/03/2009
    author:     Paul Turner
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
#include "CEGUI/XMLParserModules/Xerces/XMLParserProperties.h"
#include "CEGUI/XMLParserModules/Xerces/XMLParser.h"

// Start of CEGUI namespace section
namespace CEGUI
{

namespace XercesParserProperties
{   

////////////////////////////////////////////////////////////////////////////////
// SchemaDefaultGroup
////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------//
String SchemaDefaultResourceGroup::get(const PropertyReceiver* receiver) const
{
    return static_cast<const XercesParser*>(receiver)->
        getSchemaDefaultResourceGroup();
}

//----------------------------------------------------------------------------//
void SchemaDefaultResourceGroup::set(PropertyReceiver* receiver,
                                     const String& value)
{
    static_cast<XercesParser*>(receiver)->setSchemaDefaultResourceGroup(value);
}

//----------------------------------------------------------------------------//
Property* SchemaDefaultResourceGroup::clone() const
{
    return new SchemaDefaultResourceGroup(*this);
}

} // End of XercesParserProperties namespace section
    
} // End of  CEGUI namespace section
