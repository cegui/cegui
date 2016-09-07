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
#include "EffectsDemo.h"
#include "CEGUI/CEGUI.h"

#include "CEGUI/RenderingWindow.h"

#include <cstdlib>

//----------------------------------------------------------------------------//
WobblyWindowEffect::WobblyWindowEffect(CEGUI::Window* window) :
    d_initialised(false),
    d_window(dynamic_cast<CEGUI::FrameWindow*>(window))
{
    if (!d_window)
        CEGUI_THROW(CEGUI::InvalidRequestException("This effect is only applicable to FrameWindows!"));
}

//----------------------------------------------------------------------------//
int WobblyWindowEffect::getPassCount() const
{
    return 1;
}

//----------------------------------------------------------------------------//
void WobblyWindowEffect::performPreRenderFunctions(const int /*pass*/)
{
    // nothing we need here
}

//----------------------------------------------------------------------------//
void WobblyWindowEffect::performPostRenderFunctions()
{
    // nothing we need here
}

void WobblyWindowEffect::syncPivots(CEGUI::RenderingWindow& window)
{
    const CEGUI::Rectf pixelRect = CEGUI::Rectf(window.getPosition(), window.getSize());

    for (size_t y = 0; y < ds_yPivotCount; ++y)
    {
        for (size_t x = 0; x < ds_xPivotCount; ++x)
        {
            const float factorMinX = static_cast<float>(ds_xPivotCount - x) / (ds_xPivotCount - 1);
            const float factorMaxX = static_cast<float>(x) / (ds_xPivotCount - 1);
            const float factorMinY = static_cast<float>(ds_yPivotCount - y) / (ds_yPivotCount - 1);
            const float factorMaxY = static_cast<float>(y) / (ds_yPivotCount - 1);

            d_pivots[x][y] = CEGUI::Vector2f(
                    factorMinX * pixelRect.d_min.d_x + factorMaxX * pixelRect.d_max.d_x,
                    factorMinY * pixelRect.d_min.d_y + factorMaxY * pixelRect.d_max.d_y);

            d_pivotVelocities[x][y] = CEGUI::Vector2f(
                    0.0f,
                    0.0f);
        }
    }
}

//----------------------------------------------------------------------------//
bool WobblyWindowEffect::realiseGeometry(CEGUI::RenderingWindow& window,
                               CEGUI::GeometryBuffer& geometry)
{
    using namespace CEGUI;
    Texture& tex = window.getTextureTarget().getTexture();

    static const CEGUI::Colour c(1, 1, 1, 1);

    // qw is the width of one subdivision "box", qh is the height of it
    const float qw = window.getSize().d_width / (ds_xPivotCount - 1);
    const float qh = window.getSize().d_height / (ds_yPivotCount - 1);
    const float tcx = qw * tex.getTexelScaling().d_x;
    const float tcy =
        (window.getTextureTarget().isRenderingInverted() ? -qh : qh) *
            tex.getTexelScaling().d_y;

    const Vector3f windowPosition = Vector3f(window.getPosition(), 0.0f);

    for (size_t y = 0; y < ds_yPivotCount - 1; ++y)
    {
        for (size_t x = 0; x < ds_xPivotCount - 1; ++x)
        {
            // index of the first vertex of the quad we will construct with
            // this iteration
            const size_t idx = (y * (ds_xPivotCount - 1) + x) * 6;

            // first triangle

            // vertex 0 - top left
            d_vertices[idx + 0].position   = Vector3f(d_pivots[x][y], 0.0f) - windowPosition;
            d_vertices[idx + 0].colour_val = c;
            d_vertices[idx + 0].tex_coords = Vector2f(x * tcx, y * tcy);

            // vertex 1 - bottom left
            d_vertices[idx + 1].position   = Vector3f(d_pivots[x][y + 1], 0.0f) - windowPosition;
            d_vertices[idx + 1].colour_val = c;
            d_vertices[idx + 1].tex_coords = Vector2f(x * tcx, (y + 1) * tcy);

            // vertex 2 - bottom right
            d_vertices[idx + 2].position   = Vector3f(d_pivots[x + 1][y + 1], 0.0f) - windowPosition;
            d_vertices[idx + 2].colour_val = c;
            d_vertices[idx + 2].tex_coords = Vector2f((x + 1) * tcx, (y + 1) * tcy);

            // second triangle

            // vertex 3 - bottom right
            d_vertices[idx + 3].position   = Vector3f(d_pivots[x + 1][y + 1], 0.0f) - windowPosition;
            d_vertices[idx + 3].colour_val = c;
            d_vertices[idx + 3].tex_coords = Vector2f((x + 1) * tcx, (y + 1) * tcy);

            // vertex 4 - top right
            d_vertices[idx + 4].position   = Vector3f(d_pivots[x + 1][y], 0.0f) - windowPosition;
            d_vertices[idx + 4].colour_val = c;
            d_vertices[idx + 4].tex_coords = Vector2f((x + 1) * tcx, y * tcy);

            // vertex 5 - top left
            d_vertices[idx + 5].position   = Vector3f(d_pivots[x][y], 0.0f) - windowPosition;
            d_vertices[idx + 5].colour_val = c;
            d_vertices[idx + 5].tex_coords = Vector2f(x * tcx, y * tcy);
        }
    }

    geometry.setActiveTexture(&tex);
    geometry.appendGeometry(d_vertices, ds_vertexCount);

    // false, because we do not want the default geometry added!
    return false;
}

//----------------------------------------------------------------------------//
bool WobblyWindowEffect::update(const float elapsed, CEGUI::RenderingWindow& window)
{
    using namespace CEGUI;

    // initialise ourself upon the first update call.
    if (!d_initialised)
    {
        syncPivots(window);
        d_initialised = true;
        return true;
    }

    const CEGUI::Rectf pixelRect = CEGUI::Rectf(window.getPosition(), window.getSize());

    bool changed = false;

    for (size_t y = 0; y < ds_yPivotCount; ++y)
    {
        for (size_t x = 0; x < ds_xPivotCount; ++x)
        {
            const float factorMinX = static_cast<float>(ds_xPivotCount - 1- x) / (ds_xPivotCount - 1);
            const float factorMaxX = static_cast<float>(x) / (ds_xPivotCount - 1);
            const float factorMinY = static_cast<float>(ds_yPivotCount - 1 - y) / (ds_yPivotCount - 1);
            const float factorMaxY = static_cast<float>(y) / (ds_yPivotCount - 1);

            const Vector2f desiredPos = Vector2f(
                    factorMinX * pixelRect.d_min.d_x + factorMaxX * pixelRect.d_max.d_x,
                    factorMinY * pixelRect.d_min.d_y + factorMaxY * pixelRect.d_max.d_y);

            const Vector2f delta = desiredPos - d_pivots[x][y];

            float speed = 300.0f;
            const Vector2f cursorDelta = d_window->getTitlebar()->isDragged() ? window.getPosition() + d_window->getTitlebar()->getDragPoint() - d_pivots[x][y] : Vector2f(0.0f, 0.0f);
            const float cursorDeltaLength = sqrtf(cursorDelta.d_x * cursorDelta.d_x + cursorDelta.d_y * cursorDelta.d_y);
            speed /= cursorDeltaLength > 64 ? sqrtf(cursorDeltaLength) * 0.125f : 1;

            d_pivotVelocities[x][y] *= pow(0.00001f, elapsed);
            d_pivotVelocities[x][y] += delta * (speed * elapsed);

            const Vector2f old_pivot(d_pivots[x][y]);
            d_pivots[x][y] += d_pivotVelocities[x][y] * elapsed;
            changed |= (old_pivot != d_pivots[x][y]);
        }
    }

    // note we just need system to redraw the geometry; we do not need a
    // full redraw of all window/widget content - which is unchanged.
    if (changed)
        d_window->getGUIContext().markAsDirty();

    return false;
}

//----------------------------------------------------------------------------//
// The following are related to the RenderEffect
//
// Note: This be the land of magic numbers and compound hacks upon hacks :-p
//       Any final version of this we might provide will likely be cleaned up
//       considerably.
//----------------------------------------------------------------------------//
const float OldWobblyWindowEffect::tess_x = 8;
const float OldWobblyWindowEffect::tess_y = 8;

//----------------------------------------------------------------------------//
OldWobblyWindowEffect::OldWobblyWindowEffect(CEGUI::Window* window) :
    initialised(false),
    dragX(0), dragY(0),
    elasX(0), elasY(0),
    d_window(window)
{
}

//----------------------------------------------------------------------------//
int OldWobblyWindowEffect::getPassCount() const
{
    return 1;
}

//----------------------------------------------------------------------------//
void OldWobblyWindowEffect::performPreRenderFunctions(const int /*pass*/)
{
    // nothing we need here
}

//----------------------------------------------------------------------------//
void OldWobblyWindowEffect::performPostRenderFunctions()
{
    // nothing we need here
}

//----------------------------------------------------------------------------//
bool OldWobblyWindowEffect::realiseGeometry(CEGUI::RenderingWindow& window,
                               CEGUI::GeometryBuffer& geometry)
{
    using namespace CEGUI;
    Texture& tex = window.getTextureTarget().getTexture();

    static const CEGUI::Colour c(1, 1, 1, 1);

    const float qw = window.getSize().d_width / tess_x;
    const float qh = window.getSize().d_height / tess_y;
    const float tcx = qw * tex.getTexelScaling().d_x;
    const float tcy =
        (window.getTextureTarget().isRenderingInverted() ? -qh : qh) *
            tex.getTexelScaling().d_y;

    for (int j = 0; j < tess_y; ++j)
    {
        for (int i = 0; i < tess_x; ++i)
        {
            int idx = static_cast<int>( (j * tess_x + i) * 6 );

            float top_adj = dragX * ((1.0f / tess_x) * j);
            float bot_adj = dragX * ((1.0f / tess_x) * (j+1));
            top_adj = ((top_adj*top_adj) / 3) * (dragX < 0 ? -1 : 1);
            bot_adj = ((bot_adj*bot_adj) / 3) * (dragX < 0 ? -1 : 1);

            float lef_adj = dragY * ((1.0f / tess_y) * i);
            float rig_adj = dragY * ((1.0f / tess_y) * (i+1));
            lef_adj = ((lef_adj*lef_adj) / 3) * (dragY < 0 ? -1 : 1);
            rig_adj = ((rig_adj*rig_adj) / 3) * (dragY < 0 ? -1 : 1);

            // vertex 0
            vb[idx + 0].position   = Vector3f(i * qw - top_adj, j * qh - lef_adj, 0.0f);
            vb[idx + 0].colour_val = c;
            vb[idx + 0].tex_coords = Vector2f(i * tcx, j*tcy);

            // vertex 1
            vb[idx + 1].position   = Vector3f(i * qw - bot_adj, j * qh + qh - lef_adj, 0.0f);
            vb[idx + 1].colour_val = c;
            vb[idx + 1].tex_coords = Vector2f(i*tcx, j*tcy+tcy);

            // vertex 2
            vb[idx + 2].position   = Vector3f(i * qw + qw - bot_adj, j * qh + qh - rig_adj, 0.0f);
            vb[idx + 2].colour_val = c;
            vb[idx + 2].tex_coords = Vector2f(i*tcx+tcx, j*tcy+tcy);

            // vertex 3
            vb[idx + 3].position   = Vector3f(i * qw + qw - bot_adj, j * qh + qh - rig_adj, 0.0f);
            vb[idx + 3].colour_val = c;
            vb[idx + 3].tex_coords = Vector2f(i*tcx+tcx, j*tcy+tcy);

            // vertex 4
            vb[idx + 4].position   = Vector3f(i * qw + qw - top_adj, j * qh - rig_adj, 0.0f);
            vb[idx + 4].colour_val = c;
            vb[idx + 4].tex_coords = Vector2f(i*tcx+tcx, j*tcy);

            // vertex 5
            vb[idx + 5].position   = Vector3f(i * qw - top_adj, j * qh - lef_adj, 0.0f);
            vb[idx + 5].colour_val = c;
            vb[idx + 5].tex_coords = Vector2f(i * tcx, j*tcy);
        }
    }

    geometry.setActiveTexture(&tex);
    geometry.appendGeometry(vb, buffsize);

    // false, because we do not want the default geometry added!
    return false;
}

//----------------------------------------------------------------------------//
bool OldWobblyWindowEffect::update(const float elapsed, CEGUI::RenderingWindow& window)
{
    using namespace CEGUI;
    
    // initialise ourself upon the first update call.
    if (!initialised)
    {
        initialised=true;
        lastX = window.getPosition().d_x;
        lastY = window.getPosition().d_y;
        return true;
    }

    const Vector2f pos(window.getPosition());

    //
    // Set up for X axis animation.
    //
    if (pos.d_x != lastX)
    {
        dragX += (pos.d_x - lastX) * 0.2f;
        elasX = 0.05f;
        lastX = pos.d_x;

        if (dragX > 25)
            dragX = 25;
        else if (dragX < -25)
            dragX = -25;
    }

    //
    // Set up for y axis animation
    //
    if (pos.d_y != lastY)
    {
        dragY += (pos.d_y - lastY) * 0.2f;
        elasY = 0.05f;
        lastY = pos.d_y;

        if (dragY > 25)
            dragY = 25;
        else if (dragY < -25)
            dragY = -25;
    }

    //
    // Perform required animation steps
    //
    if ((dragX != 0) || (dragY != 0))
    {
        if (dragX < 0)
        {
            dragX += (elasX * 800 * elapsed);
            elasX += 0.075f * elapsed;
            if (dragX >0)
                dragX = 0;
        }
        else
        {
            dragX -= (elasX * 800 * elapsed);
            elasX += 0.075f * elapsed;
            if (dragX < 0)
                dragX = 0;
        }

        if (dragY < 0)
        {
            dragY += elasY * 800 * elapsed;
            elasY += 0.075f * elapsed;
            if (dragY >0)
                dragY = 0;
        }
        else
        {
            dragY -= elasY * 800 * elapsed;
            elasY += 0.075f * elapsed;
            if (dragY < 0)
                dragY = 0;
        }

        // note we just need system to redraw the geometry; we do not need a
        // full redraw of all window/widget content - which is unchanged.
        d_window->getGUIContext().markAsDirty();
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------//
ElasticWindowEffect::ElasticWindowEffect(CEGUI::Window* window) :
    d_initialised(false),
    d_window(window)
{}

//----------------------------------------------------------------------------//
int ElasticWindowEffect::getPassCount() const
{
    return 1;
}

//----------------------------------------------------------------------------//
void ElasticWindowEffect::performPreRenderFunctions(const int /*pass*/)
{
    // nothing we need here
}

//----------------------------------------------------------------------------//
void ElasticWindowEffect::performPostRenderFunctions()
{
    // nothing we need here
}

//----------------------------------------------------------------------------//
bool ElasticWindowEffect::realiseGeometry(CEGUI::RenderingWindow& window,
                               CEGUI::GeometryBuffer& geometry)
{
    using namespace CEGUI;
    Texture& tex = window.getTextureTarget().getTexture();

    static const CEGUI::Colour c(1, 1, 1, 1);

    float uvTop = window.getTextureTarget().isRenderingInverted() ? 1.0f : 0.0f;
    float uvBot = window.getTextureTarget().isRenderingInverted() ? 0.0f : 1.0f;

    const Vector3f windowPosition = Vector3f(window.getPosition(), 0.0f);
    const Vector2f& currentTopLeft = d_currentPosition ;
    const Vector2f currentBottomRight = d_currentPosition +
        Vector2f(window.getSize().d_width, window.getSize().d_height);

    {
        // first triangle

        // vertex 0 - top left
        d_vertices[0].position   = Vector3f(currentTopLeft, 0.0f) - windowPosition;
        d_vertices[0].colour_val = c;
        d_vertices[0].tex_coords = Vector2f(0.0f, uvTop);

        // vertex 1 - bottom left
        d_vertices[1].position   = Vector3f(currentTopLeft.d_x, currentBottomRight.d_y, 0.0f) - windowPosition;
        d_vertices[1].colour_val = c;
        d_vertices[1].tex_coords = Vector2f(0.0f, uvBot);

        // vertex 2 - bottom right
        d_vertices[2].position   = Vector3f(currentBottomRight, 0.0f) - windowPosition;
        d_vertices[2].colour_val = c;
        d_vertices[2].tex_coords = Vector2f(1.0f, uvBot);

        // second triangle

        // vertex 3 - bottom right
        d_vertices[3].position   = Vector3f(currentBottomRight, 0.0f) - windowPosition;
        d_vertices[3].colour_val = c;
        d_vertices[3].tex_coords = Vector2f(1.0f, uvBot);

        // vertex 4 - top right
        d_vertices[4].position   = Vector3f(currentBottomRight.d_x, currentTopLeft.d_y, 0.0f) - windowPosition;
        d_vertices[4].colour_val = c;
        d_vertices[4].tex_coords = Vector2f(1.0f, uvTop);

        // vertex 5 - top left
        d_vertices[5].position   = Vector3f(currentTopLeft, 0.0f) - windowPosition;
        d_vertices[5].colour_val = c;
        d_vertices[5].tex_coords = Vector2f(0.0f, uvTop);
    }

    geometry.setActiveTexture(&tex);
    geometry.appendGeometry(d_vertices, ds_vertexCount);

    // false, because we do not want the default geometry added!
    return false;
}

//----------------------------------------------------------------------------//
bool ElasticWindowEffect::update(const float elapsed, CEGUI::RenderingWindow& window)
{
    using namespace CEGUI;

    // initialise ourself upon the first update call.
    if (!d_initialised)
    {
        d_currentPosition = window.getPosition();
        d_currentVelocity = Vector2f(0, 0);

        d_initialised = true;
        return true;
    }

    const Vector2f delta = window.getPosition() - d_currentPosition;

    const float speed = 300.0f;
    d_currentVelocity *= pow(0.00001f, elapsed);
    d_currentVelocity += delta * (speed * elapsed);

    const Vector2f old_position(d_currentPosition);
    d_currentPosition += d_currentVelocity * elapsed;
    const bool changed = d_currentPosition != old_position;

    // note we just need system to redraw the geometry; we do not need a
    // full redraw of all window/widget content - which is unchanged.
    if (changed)
        d_window->getGUIContext().markAsDirty();

    return false;
}


//----------------------------------------------------------------------------//
// The following are for the main EffectsDemo class.
//----------------------------------------------------------------------------//

const CEGUI::String EffectsDemo::s_effectNameElastic = "ElasticWindow";
const CEGUI::String EffectsDemo::s_effectNameWobblyNew = "WobblyWindow";
const CEGUI::String EffectsDemo::s_effectNameWobblyOld = "OldWobblyWindow";

/*************************************************************************
Sample specific initialisation goes here.
*************************************************************************/
bool EffectsDemo::initialise(CEGUI::GUIContext* guiContext)
{
    using namespace CEGUI;

    d_usedFiles = CEGUI::String(__FILE__);
    d_guiContext = guiContext;

    // Register our effects with the system
    RenderEffectManager::getSingleton().addEffect<ElasticWindowEffect>(s_effectNameElastic);
    RenderEffectManager::getSingleton().addEffect<WobblyWindowEffect>(s_effectNameWobblyNew);
    RenderEffectManager::getSingleton().addEffect<OldWobblyWindowEffect>(s_effectNameWobblyOld);

    // Now we make a Falagard mapping for a frame window that uses this effect.
    // We create a type "Vanilla/WobblyFrameWindow".  Note that it would be
    // more usual for this mapping to be specified in the scheme xml file,
    // though here we are doing in manually to save from needing either multiple
    // versions of the schemes or from having demo specific definitions in
    // the schemes.
    WindowFactoryManager::getSingleton().addFalagardWindowMapping(
        "Vanilla/WobblyFrameWindow",    // type to create
        "CEGUI/FrameWindow",            // 'base' widget type
        "Vanilla/FrameWindow",          // WidgetLook to use.
        "Core/FrameWindow",             // WindowRenderer to use.
        s_effectNameWobblyNew);         // effect to use.

    // Since we want to be able to load the EffectsDemo.layout in the editor
    // tools (where the above mapping is not available), we now alias the
    // new window type onto the original TaharezLook/FrameWindow.  This has
    // the effect that - after the alias is added - any time a window of
    // type "TaharezLook/FrameWindow" is requested, the system will create a
    // "TaharezLook/WobblyFrameWindow" instead.

    WindowFactoryManager::getSingleton().addWindowTypeAlias(
        "Vanilla/FrameWindow",  // alias name - can shadow existing types
        "Vanilla/WobblyFrameWindow"); // target type to create.

    // we will use of the WindowManager.
    WindowManager& winMgr = WindowManager::getSingleton();

    // load scheme and set up defaults
    SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
    guiContext->getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

    // load font and setup default if not loaded via scheme
    Font& defaultFont = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    // Set default font for the gui context
    guiContext->setDefaultFont(&defaultFont);

    // load an image to use as a background
    if( !ImageManager::getSingleton().isDefined("SpaceBackgroundImage") )
        ImageManager::getSingleton().addFromImageFile("SpaceBackgroundImage", "SpaceBackground.jpg");

    if( !ImageManager::getSingleton().isDefined("AliasingTestImage") )
        ImageManager::getSingleton().addFromImageFile("AliasingTestImage", "Aliasing.jpg");

    // here we will use a StaticImage as the root, then we can use it to place a background image
    Window* background = winMgr.createWindow("TaharezLook/StaticImage", "background_wnd");
    // set position and size
    background->setPosition(UVector2(cegui_reldim(0), cegui_reldim( 0)));
    background->setSize(USize(cegui_reldim(1), cegui_reldim( 1)));
    // disable frame and standard background
    background->setProperty("FrameEnabled", "false");
    background->setProperty("BackgroundEnabled", "false");
    // set the background image
    background->setProperty("Image", "SpaceBackgroundImage");
    // set the background window as the root window for our GUIContext
    guiContext->setRootWindow(background);

    // load the windows for the EffectsDemo from the layout file.
    Window* sheet = winMgr.loadLayoutFromFile("EffectsDemo.layout");
 
    // Get the combobox created from within the layout
    CEGUI::Combobox* effectsCombobox = static_cast<CEGUI::Combobox*>(sheet->getChild("EffectsFrameWindow/EffectsCombobox"));

    // attach this to the 'real' root
    background->addChild(sheet);
    //Initialise the render effects
    initialiseEffects(effectsCombobox->getParent());

    // Initialise the items and subscribe the event for the combobox
    initialiseEffectsCombobox(effectsCombobox);
    
    // We can switch the automatic effects mapping off now
    WindowFactoryManager::getSingleton().removeWindowTypeAlias(
        "Vanilla/FrameWindow",  // alias name - can shadow existing types
        "Vanilla/WobblyFrameWindow"); // target type to create.




    // We create a mapping for the elastic windows effect too,
    // and we will apply it to a window we create from code
    WindowFactoryManager::getSingleton().addFalagardWindowMapping(
        "Vanilla/ElasticFrameWindow",   // type to create
        "CEGUI/FrameWindow",            // 'base' widget type
        "Vanilla/FrameWindow",          // WidgetLook to use.
        "Core/FrameWindow",             // WindowRenderer to use.
        s_effectNameElastic);    // effect to use.

    // We will create another window using the effects-mapping we created before, this time directly from code
    CEGUI::FrameWindow* aliasingFrameWnd = static_cast<CEGUI::FrameWindow*>(
        WindowManager::getSingleton().createWindow("Vanilla/ElasticFrameWindow", "AliasingTestWindow") );

    // Add it to the layout root
    sheet->addChild(aliasingFrameWnd);

    // We will add an image to it using a StaticImage window
    Window* aliasingWnd = WindowManager::getSingleton().createWindow("Vanilla/StaticImage", "AliasingTestImage");
    aliasingFrameWnd->addChild(aliasingWnd);
    aliasingFrameWnd->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim(0.15f)));
    aliasingFrameWnd->setSize(CEGUI::USize(cegui_reldim(0.2f), cegui_reldim(0.28f)));
    aliasingFrameWnd->setSizingEnabled(true);
    aliasingFrameWnd->setCloseButtonEnabled(false);
    aliasingFrameWnd->setTitleBarEnabled(true);
    aliasingFrameWnd->setText("Elastic Window - Aliasing Testimage");

    // Image window setup
    aliasingWnd->setSize(CEGUI::USize(cegui_reldim(1.0f), cegui_reldim(1.0f)));
    aliasingWnd->setProperty("FrameEnabled", "false");
    aliasingWnd->setProperty("BackgroundEnabled", "false");
    aliasingWnd->setProperty("Image", "AliasingTestImage");

    // success!
    return true;
}

/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void EffectsDemo::deinitialise()
{
    // nothing to do here!
}

/*************************************************************************
Initialises the effects ListItems for the Combobox.
*************************************************************************/
void EffectsDemo::initialiseEffectsCombobox(CEGUI::Combobox* effectsCombobox)
{
    d_listItemEffectWobblyNew = new MyListItem("Wobbly Window Effect");
    d_listItemEffectElastic = new MyListItem("Elastic Window Effect");
    d_listItemEffectWobblyOld = new MyListItem("Old Wobbly Window Effect");
    d_listItemEffectNone = new MyListItem("None");


    effectsCombobox->addItem(d_listItemEffectWobblyNew);
    effectsCombobox->addItem(d_listItemEffectElastic);
    effectsCombobox->addItem(d_listItemEffectWobblyOld);
    effectsCombobox->addItem(d_listItemEffectNone);

    effectsCombobox->setItemSelectState(d_listItemEffectWobblyNew, true);
    effectsCombobox->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&EffectsDemo::handleEffectsComboboxSelectionChanged, this));
}

/*************************************************************************
    Selection EventHandler for the effects Combobox.
*************************************************************************/
bool EffectsDemo::handleEffectsComboboxSelectionChanged(const CEGUI::EventArgs& args)
{
    const CEGUI::WindowEventArgs& winArgs(static_cast<const CEGUI::WindowEventArgs&>(args));

    CEGUI::Combobox* effectsCombobox = static_cast<CEGUI::Combobox*>(winArgs.window);
    CEGUI::ListboxItem* selectionItem = effectsCombobox->getSelectedItem();

    CEGUI::FrameWindow* effectsWindow = static_cast<CEGUI::FrameWindow*>(effectsCombobox->getParent());
    CEGUI::RenderingWindow* effectsWindowRenderingWnd = static_cast<CEGUI::RenderingWindow*>(effectsWindow->getRenderingSurface());

    if(selectionItem == d_listItemEffectElastic)
    {
        effectsWindowRenderingWnd->setRenderEffect(d_renderEffectElastic);
    }
    else if(selectionItem == d_listItemEffectWobblyNew)
    {
        effectsWindowRenderingWnd->setRenderEffect(d_renderEffectWobblyNew);
    }
    else if(selectionItem == d_listItemEffectWobblyOld)
    {
        effectsWindowRenderingWnd->setRenderEffect(d_renderEffectWobblyOld);
    }
    else
    {
        effectsWindowRenderingWnd->setRenderEffect(0);
    }


    return true;
}

void EffectsDemo::initialiseEffects(CEGUI::Window* effectsWindow)
{
    d_renderEffectElastic = &CEGUI::RenderEffectManager::getSingleton().create(
        s_effectNameElastic,
        effectsWindow);

    d_renderEffectWobblyNew = &CEGUI::RenderEffectManager::getSingleton().create(
        s_effectNameWobblyNew,
        effectsWindow);

    d_renderEffectWobblyOld = &CEGUI::RenderEffectManager::getSingleton().create(
        s_effectNameWobblyOld,
        effectsWindow);
}

/*************************************************************************
    Define the module function that returns an instance of the sample
*************************************************************************/
extern "C" SAMPLE_EXPORT Sample& getSampleInstance()
{
    static EffectsDemo sample;
    return sample;
}
