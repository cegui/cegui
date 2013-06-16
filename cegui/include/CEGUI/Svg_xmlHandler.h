/***********************************************************************
    filename:   Svg_xmlHandler.h
    created:    Sunday, 16th June 2013
    author:     Lukas Meindl <cegui@lukasmeindl.at>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2013 Paul D Turner & The CEGUI Development Team
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
#ifndef _Svg_xmlHandler_h_
#define _Svg_xmlHandler_h_

#include "CEGUI/ChainedXMLHandler.h"
#include "CEGUI/String.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//! Class used to parse stand alone Animation XML files.
class CEGUIEXPORT Svg_xmlHandler : public ChainedXMLHandler
{
public:
	//! String holding the element handled by this class.
    static const String ElementName;

    Svg_xmlHandler();
    virtual ~Svg_xmlHandler();

    // XMLHandler overrides
    const String& getSchemaName() const;
    const String& getDefaultResourceGroup() const;

protected:
    // implement ChainedXMLHandler interface.
    void elementStartLocal(const String& element,
                           const XMLAttributes& attributes);
    void elementEndLocal(const String& element);
};

}

#endif

