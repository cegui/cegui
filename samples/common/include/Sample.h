/***********************************************************************
created:    29/5/2012
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
#ifndef _Sample_h_
#define _Sample_h_

#include "CEGUI/String.h"

// This header serves as a base for all samples and is needed inside the
// SamplesFramework as interface for Samples that will be loaded.
#include "CEGUI/ForwardRefs.h"

class Sample
{
public:
    Sample(int priority = 0) :
        d_priority(priority)
    {}

    virtual bool initialise(CEGUI::GUIContext* guiContext) = 0;
    virtual void deinitialise() = 0;

    virtual void onEnteringSample() {}
    virtual void update(float /*timeSinceLastUpdate*/) {}

    const CEGUI::String& getUsedFilesString() {return d_usedFiles;}
    //! Get an explicit instance of input aggregator to be used with this sample
    CEGUI::InputAggregator* getInputAggregator() { return d_inputAggregator; }

    //! Get the name of the sample
    CEGUI::String getName() const { return d_name; }
    //! Get the credits (author(s)) of the sample
    CEGUI::String getCredits() const { return d_credits; }
    //! Get the summary of what the samples wants to demonstrate
    CEGUI::String getSummary() const { return d_summary; }
    //! Get the description of how the sample achieves what it aims to demonstrate
    CEGUI::String getDescription() const { return d_description; }

    //! Returns the priority of this sample in the browser
    int getPriority() const { return d_priority; }

    //! Operator for sorting the samples in the browser after their priority
    bool operator< (const Sample& other) const { return d_priority > other.getPriority(); }

protected:
    //! The GUIContext that the Sample is a part of
    CEGUI::GUIContext*          d_guiContext = nullptr;

    CEGUI::String               d_usedFiles;
    CEGUI::InputAggregator*     d_inputAggregator = nullptr;

    //! The name of the sample
    CEGUI::String               d_name;
    //! The credits of the sample (author(s))
    CEGUI::String               d_credits;
    //! The summary of what the sample wants to demonstrate
    CEGUI::String               d_summary;
    //! The summary of how the sample achieves what it aims to demonstrate
    CEGUI::String               d_description;

    //! The priority of display of this sample in the browser. Higher priority means earlier display
    int                         d_priority;
};

struct SamplePointerCompare
{
    bool operator()(const Sample* left, const Sample* right) { return *left < *right; }
};

#endif
