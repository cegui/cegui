/***********************************************************************
    created:    31/5/2012
    author:     Lukas E Meindl
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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
#ifndef _Samples_xmlHandler_h_
#define _Samples_xmlHandler_h_

#include "CEGUI/ChainedXMLHandler.h"
#include "CEGUI/String.h"

class SamplesFramework;

using namespace CEGUI;

class Samples_xmlHandler : public ChainedXMLHandler
{
public:
    Samples_xmlHandler(SamplesFramework* samplesFramework);
    virtual ~Samples_xmlHandler();

    //! String holding the element handled by this class.
    static const String ElementName;

    // XMLHandler overrides
    const String& getSchemaName() const;

protected:
    // implement ChainedXMLHandler interface.
    void elementStartLocal(const String& element,
                           const XMLAttributes& attributes);
    void elementEndLocal(const String& element);

    SamplesFramework* d_samplesFramework;
};

//----------------------------------------------------------------------------//
//! Chained sub-handler for AnimationDefinition XML elements
class SampleDataHandler : public ChainedXMLHandler
{
public:
    //! String holding the element handled by this class.
    static const String ElementName;

    static const String NameAttribute;
    static const String SummaryAttribute;
    static const String DescriptionAttribute;
    static const String CreditsAttribute;

    static const String SampleTypeAttribute;
    static const String SampleTypeCppModule;
    static const String SampleTypeLua;
    static const String SampleTypePython;


    SampleDataHandler(const XMLAttributes& attributes,
                      SamplesFramework* samplesFramework);
    virtual ~SampleDataHandler();

protected:
    // implement ChainedXMLHandler interface.
    void elementStartLocal(const String& element,
                           const XMLAttributes& attributes);
    void elementEndLocal(const String& element);

    SamplesFramework* d_samplesFramework;
};

#endif

