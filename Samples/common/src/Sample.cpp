/***********************************************************************
filename:   Samples.cpp
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
#include "Sample.h"

#include "CEGUI/GUIContext.h"
#include "CEGUI/System.h"
#include "CEGUI/TextureTarget.h"
#include "CEGUI/Texture.h"
#include "CEGUI/Image.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/BasicImage.h"
#include "CEGUI/Vector.h"


using namespace CEGUI;



Sample::Sample()
    :d_guiContext(0),
    d_textureTarget(0),
    d_textureTargetImage(0)
{
    CEGUI::System& system(System::getSingleton());

    d_textureTarget = system.getRenderer()->createTextureTarget();
    d_guiContext = &system.createGUIContext((RenderTarget&)*d_textureTarget);

    CEGUI::String imageName(d_textureTarget->getTexture().getName());
    d_textureTargetImage = static_cast<CEGUI::BasicImage*>(&CEGUI::ImageManager::getSingleton().create("BasicImage", "SampleBrowser/" + imageName));
    d_textureTargetImage->setTexture(&d_textureTarget->getTexture());
}

Sample::~Sample()
{
    //We won't cleanup correctly as the system will be destroyed at this point
/*
    CEGUI::System& system(System::getSingleton());

    if(d_guiContext)
    {
        system.destroyGUIContext(*d_guiContext);
        d_guiContext = 0;
    }

    if(d_textureTarget)
    {
        system.getRenderer()->destroyTextureTarget(d_textureTarget);
        d_textureTarget = 0;
    }*/

}

GUIContext* Sample::getGuiContext()
{
    return d_guiContext;
}

void Sample::handleNewWindowSize(float width, float height)
{
    CEGUI::Sizef windowSize(width, height);

    d_textureTarget->declareRenderSize(windowSize);
    d_textureTargetImage->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.f, 0.f), windowSize));
}

CEGUI::Image& Sample::getRTTImage()
{
    return *d_textureTargetImage;
}