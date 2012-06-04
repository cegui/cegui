/***********************************************************************
filename:   SampleData.h
created:    2/6/2012
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
#ifndef _Sample_Data_h_
#define _Sample_Data_h_

#include "CEGUI/String.h"

// forward declarations
namespace CEGUI
{
    class DynamicModule;
}

enum SampleType
{
    ST_Module,
    ST_Lua,
    ST_Python,

    ST_Count
};

class Sample;

class SampleData
{
public:
    SampleData(CEGUI::String sampleName, CEGUI::String summary,
        CEGUI::String description, SampleType sampleTypeEnum);
    virtual ~SampleData();

    virtual void initialise() = 0;
    virtual void deinitialise() = 0;

    virtual CEGUI::Window*  getGUIRoot() = 0;


protected:
    CEGUI::String           d_name;
    CEGUI::String           d_summary;
    CEGUI::String           d_description;
    SampleType              d_type;
};

class SampleDataModule : public SampleData
{
public:
    SampleDataModule(CEGUI::String sampleName, CEGUI::String summary,
    CEGUI::String description, SampleType sampleTypeEnum);
    virtual ~SampleDataModule();

    virtual void SampleDataModule::getSampleInstanceFromDLL();

    virtual void initialise();
    virtual void deinitialise();

    virtual CEGUI::Window*  getGUIRoot();

private:

    CEGUI::DynamicModule*   d_dynamicModule;
    Sample*                 d_sample;
};

#endif

