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

#include "Samples_xmlHandler.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/Logger.h"
#include "CEGUI/XMLAttributes.h"

#include "SamplesFramework.h"
#include "Sample.h"
#include "SampleData.h"

const String Samples_xmlHandler::ElementName("Samples");

const String SampleDataHandler::ElementName("SampleDefinition");

const String SampleDataHandler::NameAttribute("name");
const String SampleDataHandler::SummaryAttribute("summary");
const String SampleDataHandler::DescriptionAttribute("description");
const String SampleDataHandler::CreditsAttribute("credits");


const String SampleDataHandler::SampleTypeAttribute("type");
const String SampleDataHandler::SampleTypeCppModule("C++ Module");
const String SampleDataHandler::SampleTypeLua("Lua");
const String SampleDataHandler::SampleTypePython("Python");


//----------------------------------------------------------------------------//
Samples_xmlHandler::Samples_xmlHandler(SamplesFramework* samplesFramework)
    : d_samplesFramework(samplesFramework)
{}

//----------------------------------------------------------------------------//
Samples_xmlHandler::~Samples_xmlHandler()
{}

//----------------------------------------------------------------------------//
const String& Samples_xmlHandler::getSchemaName() const
{
    return SamplesFramework::XMLSchemaName;
}

//----------------------------------------------------------------------------//
void Samples_xmlHandler::elementStartLocal(const String& element,
                                             const XMLAttributes& attributes)
{
    if (element == ElementName)
    {
        CEGUI_LOGINSANE("===== Begin Animations parsing =====");
    }
    else if (element == SampleDataHandler::ElementName)
    {
        d_chainedHandler = new SampleDataHandler(attributes, d_samplesFramework);
    }
    else
        Logger::getSingleton().logEvent("Animation_xmlHandler::elementStart: "
            "<" + element + "> is invalid at this location.", Errors);
}

//----------------------------------------------------------------------------//
void Samples_xmlHandler::elementEndLocal(const String& element)
{
    if (element == ElementName)
    {
        CEGUI_LOGINSANE("===== End Animations parsing =====");
    }
    else
        Logger::getSingleton().logEvent("Animation_xmlHandler::elementEnd: "
            "</" + element + "> is invalid at this location.", Errors);
}

//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
SampleDataHandler::SampleDataHandler(
    const XMLAttributes& attributes,
    SamplesFramework* samplesFramework)
    : d_samplesFramework(samplesFramework)
{
    const String sampleName(attributes.getValueAsString(NameAttribute));
    const String summary(attributes.getValueAsString(SummaryAttribute));
    const String description(attributes.getValueAsString(DescriptionAttribute));
    const String sampleType(attributes.getValueAsString(SampleTypeAttribute));
    const String credits(attributes.getValueAsString(CreditsAttribute));

    CEGUI_LOGINSANE(
        "Defining sample named: " +
        sampleName +
        "  Summary: " +
        summary +
        "  Description: " +
        description +
        "  Sample Type: " +
        sampleType +
        "  Credits: " +
        credits
        );

    SampleType sampleTypeEnum;

    if (sampleType == SampleTypeCppModule)
        sampleTypeEnum = ST_Module;
    else if(sampleType == SampleTypePython)
        sampleTypeEnum = ST_Python;
    else
        sampleTypeEnum = ST_Lua;

    d_samplesFramework->addSampleDataCppModule(sampleName, summary, description, sampleTypeEnum, credits);
}

//----------------------------------------------------------------------------//
SampleDataHandler::~SampleDataHandler()
{
}

//----------------------------------------------------------------------------//
void SampleDataHandler::elementStartLocal(
    const String& element,
    const XMLAttributes& /*attributes*/)
{
    Logger::getSingleton().logEvent(
        "AnimationDefinitionHandler::elementStart: "
        "<" + element + "> is invalid at this location.", Errors);
}

void SampleDataHandler::elementEndLocal(const String& element)
{
    // set completed status when we encounter our own end element
    if (element == ElementName)
        d_completed = true;
}
