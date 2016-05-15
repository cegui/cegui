/***********************************************************************
    created:    9/7/2012
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
#ifndef _Effects_Sample_
#define _Effects_Sample_

#include "Sample.h"
#include "CEGUI/CEGUI.h"

class WobblyWindowEffect : public CEGUI::RenderEffect
{
public:
    WobblyWindowEffect(CEGUI::Window* window);

    // implement required functions from RenderEffect interface.
    int getPassCount() const override;
    void performPreRenderFunctions(const int pass) override;
    void performPostRenderFunctions() override;
    bool realiseGeometry(CEGUI::RenderingWindow& window, CEGUI::GeometryBuffer& geometry) override;
    bool update(const float elapsed, CEGUI::RenderingWindow& window) override;

protected:
    void syncPivots(CEGUI::RenderingWindow& window);

    // number of horizontal subdivisions
    static const int ds_xPivotCount = 11;
    // number of vertical subdivisions
    static const int ds_yPivotCount = 11;

    glm::vec2 d_pivots[ds_xPivotCount][ds_yPivotCount];
    glm::vec2 d_pivotVelocities[ds_xPivotCount][ds_yPivotCount];

    bool d_initialised;

    // we need 6 vertices for two triangles = one quad
    //
    // lets say we have 3x3 subdivision
    //
    // *---*---*
    // |   |   |
    // *---*---*
    // |   |   |
    // *---*---*
    //
    // as we can see, we have 2x2 quads for 3x3 subdivision (because of reused
    // neighbouring vertices)
    static const unsigned int ds_vertexCount = (ds_xPivotCount - 1) * (ds_yPivotCount - 1) * 6;
    CEGUI::TexturedColouredVertex d_vertices[ds_vertexCount];

    CEGUI::FrameWindow* d_window;
};

// Sample subclass for CEGUI::RenderEffect.  This particluar implementation
// provides a basic 'wobbly window' type effect.
class OldWobblyWindowEffect : public CEGUI::RenderEffect
{
public:
    OldWobblyWindowEffect(CEGUI::Window* window);

    // implement required functions from RenderEffect interface.
    int getPassCount() const override;
    void performPreRenderFunctions(const int pass) override;
    void performPostRenderFunctions() override;
    bool realiseGeometry(CEGUI::RenderingWindow& window, CEGUI::GeometryBuffer& geometry) override;
    bool update(const float elapsed, CEGUI::RenderingWindow& window) override;

protected:
    static const float tess_x;
    static const float tess_y;
    // tess_x * tess_y * vertex_per_quad
    static const int buffsize = (8 * 8 * 6);

    bool initialised;

    float lastX, lastY;
    float dragX, dragY;
    float elasX, elasY;

    CEGUI::TexturedColouredVertex vb[buffsize];

    CEGUI::Window* d_window;
};

class ElasticWindowEffect : public CEGUI::RenderEffect
{
public:
    ElasticWindowEffect(CEGUI::Window* window);

    // implement required functions from RenderEffect interface.
    int getPassCount() const override;
    void performPreRenderFunctions(const int pass) override;
    void performPostRenderFunctions() override;
    bool realiseGeometry(CEGUI::RenderingWindow& window, CEGUI::GeometryBuffer& geometry) override;
    bool update(const float elapsed, CEGUI::RenderingWindow& window) override;

protected:
    glm::vec2 d_currentPosition;
    glm::vec2 d_currentVelocity;

    bool d_initialised;

    static const unsigned int ds_vertexCount = 6;
    CEGUI::TexturedColouredVertex d_vertices[ds_vertexCount];

    CEGUI::Window* d_window;
};

// Sample class
class RenderEffectsSample : public Sample
{
public:
    RenderEffectsSample();
    virtual ~RenderEffectsSample() {}

    // method to initialse the samples windows and events.
    bool initialise(CEGUI::GUIContext* guiContext) override;

    // method to perform any required cleanup operations.
    void deinitialise() override;

protected:
    // Initialiser for the effects in the combobox
    void initialiseEffectsCombobox(CEGUI::Combobox* effectsCombobox);

    // Handler for the selection changes in the effects combobox
    bool handleEffectsComboboxSelectionChanged(const CEGUI::EventArgs& args);

    // Initialise the RenderEffects we will switch between using the combobox
    void initialiseEffects(CEGUI::Window* effectsWindow);

    // Names for the effects
    static const CEGUI::String s_effectNameElastic;
    static const CEGUI::String s_effectNameWobblyNew;
    static const CEGUI::String s_effectNameWobblyOld;

    CEGUI::RenderEffect* d_renderEffectElastic;
    CEGUI::RenderEffect* d_renderEffectWobblyNew;
    CEGUI::RenderEffect* d_renderEffectWobblyOld;

    // The list items of the effects combobox
    CEGUI::StandardItem* d_listItemEffectElastic;
    CEGUI::StandardItem* d_listItemEffectWobblyNew;
    CEGUI::StandardItem* d_listItemEffectWobblyOld;
    CEGUI::StandardItem* d_listItemEffectNone;

    CEGUI::GUIContext* d_guiContext;
};


#endif
