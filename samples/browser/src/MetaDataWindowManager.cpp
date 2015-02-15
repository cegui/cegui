/***********************************************************************
created:    7/6/2012
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
#include "MetaDataWindowManager.h"

#include "SampleHandler.h"

#include "CEGUI/widgets/Scrollbar.h"
#include "CEGUI/Window.h"
#include "CEGUI/SchemeManager.h"
#include "CEGUI/WindowManager.h"

using namespace CEGUI;

MetaDataWindowManager::MetaDataWindowManager(CEGUI::Window* metaDataWindow)
    : d_root(metaDataWindow)
{
    init();
}

void MetaDataWindowManager::init()
{
    d_wndSummary = d_root->getChild("Summary");
    d_wndDescription = d_root->getChild("Description");
    d_wndUsedFiles = d_root->getChild("UsedFiles");
    d_wndCredits = d_root->getChild("Credits");
}


CEGUI::Window* MetaDataWindowManager::getWindow()
{
    return d_root;
}

void MetaDataWindowManager::setSampleInfo(SampleHandler* sampleData)
{
    d_root->setText(sampleData->getNameText());
    d_wndSummary->setText(sampleData->getSummaryText());
    static_cast<CEGUI::Scrollbar*>(d_wndSummary->getChild("__auto_vscrollbar__"))->setScrollPosition(0.f);
    d_wndUsedFiles->setText(sampleData->getUsedFilesText());
    static_cast<CEGUI::Scrollbar*>(d_wndDescription->getChild("__auto_vscrollbar__"))->setScrollPosition(0.f);
    d_wndUsedFiles->setText(sampleData->getUsedFilesText());
    static_cast<CEGUI::Scrollbar*>(d_wndUsedFiles->getChild("__auto_vscrollbar__"))->setScrollPosition(0.f);
    d_wndCredits->setText(sampleData->getCreditsText());
    static_cast<CEGUI::Scrollbar*>(d_wndCredits->getChild("__auto_vscrollbar__"))->setScrollPosition(0.f);
}