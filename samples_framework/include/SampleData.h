/***********************************************************************
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
#include "CEGUI/ForwardRefs.h"

namespace CEGUI
{
    class BasicImage;
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
        CEGUI::String description, SampleType sampleTypeEnum, CEGUI::String credits);
    virtual ~SampleData();

    virtual void initialise(int width, int height);
    virtual void deinitialise();

    virtual CEGUI::GUIContext*  getGuiContext();
    virtual void handleNewWindowSize(float width, float height);

    void setTextureTargetImageArea(float height, float width);
    virtual CEGUI::Image& getRTTImage();

    virtual void setGUIContextRTT();
    virtual void clearRTTTexture();

    virtual void onEnteringSample() = 0;
    virtual void update(float /*timeSinceLastUpdate*/) {}

    void setSampleWindow(CEGUI::Window* sampleWindow);
    CEGUI::Window* getSampleWindow();

    CEGUI::String getName();
    CEGUI::String getSummary();
    CEGUI::String getSampleTypeString();
    CEGUI::String getDescription();
    CEGUI::String getUsedFilesString();
    CEGUI::String getCredits();

protected:
    CEGUI::String           d_name;
    CEGUI::String           d_summary;
    CEGUI::String           d_description;
    SampleType              d_type;
    CEGUI::String           d_usedFilesString;
    CEGUI::String           d_credits;

    CEGUI::Window*          d_sampleWindow;

    CEGUI::GUIContext*      d_guiContext;
    CEGUI::TextureTarget*   d_textureTarget;
    CEGUI::BasicImage*      d_textureTargetImage;
};

class SampleDataModule : public SampleData
{
public:
    SampleDataModule(CEGUI::String sampleName, CEGUI::String summary,
    CEGUI::String description, SampleType sampleTypeEnum, CEGUI::String credits);
    virtual ~SampleDataModule();

    virtual void getSampleInstanceFromDLL();

    virtual void initialise(int width, int height);
    virtual void deinitialise();

    virtual void onEnteringSample();

    virtual void update(float timeSinceLastUpdate);

private:
    CEGUI::DynamicModule*   d_dynamicModule;
    Sample*                 d_sample;
};

#endif

