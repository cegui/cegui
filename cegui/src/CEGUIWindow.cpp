/***********************************************************************
    filename:   CEGUIWindow.cpp
    created:    21/2/2004
    author:     Paul D Turner

    purpose:    Implements the Window base class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include "CEGUIWindow.h"
#include "CEGUIExceptions.h"
#include "CEGUIWindowManager.h"
#include "CEGUISystem.h"
#include "CEGUIFontManager.h"
#include "CEGUIImageManager.h"
#include "CEGUIMouseCursor.h"
#include "CEGUICoordConverter.h"
#include "CEGUIWindowRendererManager.h"
#include "elements/CEGUITooltip.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "falagard/CEGUIFalWidgetComponent.h"
#include "CEGUIGeometryBuffer.h"
#include "CEGUIRenderingRoot.h"
#include "CEGUIRenderingContext.h"
#include "CEGUIRenderingWindow.h"
#include <algorithm>
#include <iterator>
#include <cmath>
#include <stdio.h>

#if defined (CEGUI_USE_FRIBIDI)
    #include "CEGUIFribidiVisualMapping.h"
#elif defined (CEGUI_USE_MINIBIDI)
    #include "CEGUIMinibidiVisualMapping.h"
#else
    #include "CEGUIBidiVisualMapping.h"
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
const String Window::EventNamespace("Window");
const String Window::EventUpdated ("Updated");
const String Window::EventParentSized("ParentSized");
const String Window::EventSized("Sized");
const String Window::EventMoved("Moved");
const String Window::EventTextChanged("TextChanged");
const String Window::EventFontChanged("FontChanged");
const String Window::EventAlphaChanged("AlphaChanged");
const String Window::EventIDChanged("IDChanged");
const String Window::EventActivated("Activated");
const String Window::EventDeactivated("Deactivated");
const String Window::EventShown("Shown");
const String Window::EventHidden("Hidden");
const String Window::EventEnabled("Enabled");
const String Window::EventDisabled("Disabled");
const String Window::EventClippedByParentChanged( "ClippedByParentChanged" );
const String Window::EventDestroyedByParentChanged("DestroyedByParentChanged");
const String Window::EventInheritsAlphaChanged( "InheritsAlphaChanged" );
const String Window::EventAlwaysOnTopChanged("AlwaysOnTopChanged");
const String Window::EventInputCaptureGained( "InputCaptureGained" );
const String Window::EventInputCaptureLost( "InputCaptureLost" );
const String Window::EventRenderingStarted( "RenderingStarted" );
const String Window::EventRenderingEnded( "RenderingEnded" );
const String Window::EventChildAdded( "ChildAdded" );
const String Window::EventChildRemoved( "ChildRemoved" );
const String Window::EventDestructionStarted( "DestructionStarted" );
const String Window::EventZOrderChanged( "ZOrderChanged" );
const String Window::EventDragDropItemEnters("DragDropItemEnters");
const String Window::EventDragDropItemLeaves("DragDropItemLeaves");
const String Window::EventDragDropItemDropped("DragDropItemDropped");
const String Window::EventVerticalAlignmentChanged("VerticalAlignmentChanged");
const String Window::EventHorizontalAlignmentChanged("HorizontalAlignmentChanged");
const String Window::EventWindowRendererAttached("WindowRendererAttached");
const String Window::EventWindowRendererDetached("WindowRendererDetached");
const String Window::EventRotated("Rotated");
const String Window::EventNonClientChanged( "NonClientChanged" );
const String Window::EventTextParsingChanged("TextParsingChanged");
const String Window::EventMarginChanged("MarginChanged");
const String Window::EventMouseEntersArea("MouseEntersArea");
const String Window::EventMouseLeavesArea("MouseLeavesArea");
const String Window::EventMouseEntersSurface( "MouseEntersSurface" );
const String Window::EventMouseLeavesSurface( "MouseLeavesSurface" );
const String Window::EventMouseMove("MouseMove");
const String Window::EventMouseWheel("MouseWheel");
const String Window::EventMouseButtonDown("MouseButtonDown");
const String Window::EventMouseButtonUp("MouseButtonUp");
const String Window::EventMouseClick("MouseClick");
const String Window::EventMouseDoubleClick("MouseDoubleClick");
const String Window::EventMouseTripleClick("MouseTripleClick");
const String Window::EventKeyDown("KeyDown");
const String Window::EventKeyUp("KeyUp");
const String Window::EventCharacterKey("CharacterKey");

//----------------------------------------------------------------------------//
const String Window::TooltipNameSuffix("__auto_tooltip__");
const String Window::AutoWidgetNameSuffix("__auto_");

//----------------------------------------------------------------------------//
Window* Window::d_captureWindow     = 0;
BasicRenderedStringParser Window::d_basicStringParser;
DefaultRenderedStringParser Window::d_defaultStringParser;

//----------------------------------------------------------------------------//
TplProperty<Window, bool>           Window::d_alwaysOnTopProperty("AlwaysOnTop", "Property to get/set the 'always on top' setting for the Window.  Value is either \"True\" or \"False\".",
                                                            &Window::setAlwaysOnTop, &Window::isAlwaysOnTop, false);
TplProperty<Window, bool>           Window::d_clippedByParentProperty("ClippedByParent", "Property to get/set the 'clipped by parent' setting for the Window.  Value is either \"True\" or \"False\".",
                                                            &Window::setClippedByParent, &Window::isClippedByParent, true);
TplProperty<Window, bool>           Window::d_destroyedByParentProperty("DestroyedByParent", "Property to get/set the 'destroyed by parent' setting for the Window.  Value is either \"True\" or \"False\".",
                                                            &Window::setDestroyedByParent, &Window::isDestroyedByParent, true);
/*TplProperty<Window, bool>           Window::d_disabledProperty("Disabled", "Property to get/set the 'disabled state' setting for the Window.  Value is either \"True\" or \"False\".",
                                                            &Window::setDisabled, &Window::isDisabled, false);*/

WindowProperties::Disabled          Window::d_disabledProperty;
WindowProperties::Font              Window::d_fontProperty;

TplProperty<Window, uint>           Window::d_IDProperty("ID", "Property to get/set the ID value of the Window.  Value is an unsigned integer number.",
                                                            &Window::setID, &Window::getID, 0);
TplProperty<Window, bool>           Window::d_inheritsAlphaProperty("InheritsAlpha", "Property to get/set the 'inherits alpha' setting for the Window.  Value is either \"True\" or \"False\".",
                                                            &Window::setInheritsAlpha, &Window::inheritsAlpha, true);
/*TplProperty<Window, Image*>         Window::d_mouseCursorProperty("MouseCursor", "Property to get/set the mouse cursor image for the Window.  Value should be \"set:<imageset name> image:<image name>\".",
                                                            &Window::getMouseCursor, &Window::setMouseCursor, 0);*/
WindowProperties::MouseCursorImage  Window::d_mouseCursorProperty;
TplProperty<Window, bool>           Window::d_restoreOldCaptureProperty("RestoreOldCapture", "Property to get/set the 'restore old capture' setting for the Window.  Value is either \"True\" or \"False\".",
                                                            &Window::setRestoreOldCapture, &Window::restoresOldCapture, false);
TplProperty<Window, String>         Window::d_textProperty("Text", "Property to get/set the text / caption for the Window. Value is the text string to use. Meaning of this property heavily depends on the type of the Window.",
                                                            &Window::setText, &Window::getText, "");
/*
TplProperty<Window, bool>           Window::d_visibleProperty("Visible", "Property to get/set the 'visible state' setting for the Window.  Value is either \"True\" or \"False\".",
                                                            &Window::setVisible, &Window::isVisible, true);*/
WindowProperties::Visible           Window::d_visibleProperty;
// TODO: inconsistency between property and setter getter names
TplProperty<Window, bool>           Window::d_zOrderChangeProperty("ZOrderChangeEnabled", "Property to get/set the 'z-order changing enabled' setting for the Window.  Value is either \"True\" or \"False\".",
                                                            &Window::setZOrderingEnabled, &Window::isZOrderingEnabled, true);
TplProperty<Window, bool>           Window::d_wantsMultiClicksProperty("WantsMultiClickEvents", "Property to get/set whether the window will receive double-click and triple-click events.  Value is either \"True\" or \"False\".",
                                                            &Window::setWantsMultiClickEvents, &Window::wantsMultiClickEvents, true);
TplProperty<Window, bool>           Window::d_mouseButtonAutoRepeatProperty("MouseButtonDownAutoRepeat", "Property to get/set whether the window will receive autorepeat mouse button down events.  Value is either \"True\" or \"False\".",
                                                            &Window::setMouseAutoRepeatEnabled, &Window::isMouseAutoRepeatEnabled, false);
WindowProperties::AutoRepeatDelay   Window::d_autoRepeatDelayProperty;
WindowProperties::AutoRepeatRate    Window::d_autoRepeatRateProperty;
WindowProperties::DistributeCapturedInputs Window::d_distInputsProperty;
WindowProperties::CustomTooltipType Window::d_tooltipTypeProperty;
WindowProperties::Tooltip           Window::d_tooltipProperty;
WindowProperties::InheritsTooltipText Window::d_inheritsTooltipProperty;
WindowProperties::RiseOnClick       Window::d_riseOnClickProperty;
WindowProperties::VerticalAlignment   Window::d_vertAlignProperty;
WindowProperties::HorizontalAlignment Window::d_horzAlignProperty;
WindowProperties::UnifiedAreaRect   Window::d_unifiedAreaRectProperty;
WindowProperties::UnifiedPosition   Window::d_unifiedPositionProperty;
WindowProperties::UnifiedXPosition  Window::d_unifiedXPositionProperty;
WindowProperties::UnifiedYPosition  Window::d_unifiedYPositionProperty;
WindowProperties::UnifiedSize       Window::d_unifiedSizeProperty;
WindowProperties::UnifiedWidth      Window::d_unifiedWidthProperty;
WindowProperties::UnifiedHeight     Window::d_unifiedHeightProperty;
WindowProperties::UnifiedMinSize    Window::d_unifiedMinSizeProperty;
WindowProperties::UnifiedMaxSize    Window::d_unifiedMaxSizeProperty;
WindowProperties::MousePassThroughEnabled   Window::d_mousePassThroughEnabledProperty;
WindowProperties::WindowRenderer    Window::d_windowRendererProperty;
WindowProperties::LookNFeel         Window::d_lookNFeelProperty;
WindowProperties::DragDropTarget    Window::d_dragDropTargetProperty;
WindowProperties::AutoRenderingSurface Window::d_autoRenderingSurfaceProperty;
WindowProperties::Rotation Window::d_rotationProperty;
WindowProperties::NonClient Window::d_nonClientProperty;
WindowProperties::TextParsingEnabled Window::d_textParsingEnabledProperty;
WindowProperties::Margin Window:: d_marginProperty;
WindowProperties::UpdateMode Window::d_updateModeProperty;
WindowProperties::MouseInputPropagationEnabled Window::d_mouseInputPropagationProperty;

//----------------------------------------------------------------------------//
Window::Window(const String& type, const String& name) :
    // basic types and initial window name
    d_type(type),
    d_name(name),
    d_autoWindow(d_name.rfind(AutoWidgetNameSuffix) != String::npos),

    // basic state
    d_initialising(false),
    d_destructionStarted(false),
    d_enabled(true),
    d_visible(true),
    d_active(false),

    // parent related fields
    d_parent(0),
    d_destroyedByParent(true),

    // clipping options
    d_clippedByParent(true),
    d_nonClientContent(false),

    // rendering components and options
    d_windowRenderer(0),
    d_geometry(&System::getSingleton().getRenderer()->createGeometryBuffer()),
    d_surface(0),
    d_needsRedraw(true),
    d_autoRenderingWindow(false),
    d_mouseCursor((const Image*)DefaultMouseCursor),

    // alpha transparency set up
    d_alpha(1.0f),
    d_inheritsAlpha(true),

    // mouse input capture set up
    d_oldCapture(0),
    d_restoreOldCapture(false),
    d_distCapturedInputs(false),

    // text system set up
    d_font(0),
#ifndef CEGUI_BIDI_SUPPORT
    d_bidiVisualMapping(0),
#elif defined (CEGUI_USE_FRIBIDI)
    d_bidiVisualMapping(CEGUI_NEW_AO FribidiVisualMapping),
#elif defined (CEGUI_USE_MINIBIDI)
    d_bidiVisualMapping(CEGUI_NEW_AO MinibidiVisualMapping),
#else
    #error "BIDI Configuration is inconsistant, check your config!"
#endif
    d_bidiDataValid(false),
    d_renderedStringValid(false),
    d_customStringParser(0),
    d_textParsingEnabled(true),

    // margin
    d_margin(UBox(UDim(0, 0))),

    // user specific data
    d_ID(0),
    d_userData(0),

    // z-order related options
    d_alwaysOnTop(false),
    d_riseOnClick(true),
    d_zOrderingEnabled(true),

    // mouse input options
    d_wantsMultiClicks(true),
    d_mousePassThroughEnabled(false),
    d_autoRepeat(false),
    d_repeatDelay(0.3f),
    d_repeatRate(0.06f),
    d_repeatButton(NoButton),
    d_repeating(false),
    d_repeatElapsed(0.0f),

    // drag and drop
    d_dragDropTarget(true),

    // tool tip related
    d_customTip(0),
    d_weOwnTip(false),
    d_inheritsTipText(true),

    // XML writing options
    d_allowWriteXML(true),

    // Window position, size, aligment, etc...
    d_area(cegui_reldim(0), cegui_reldim(0), cegui_reldim(0), cegui_reldim(0)),
    d_pixelSize(0.0f, 0.0f),
    d_minSize(cegui_reldim(0), cegui_reldim(0)),
    d_maxSize(cegui_reldim(1), cegui_reldim(1)),
    d_horzAlign(HA_LEFT),
    d_vertAlign(VA_TOP),
    d_rotation(Quaternion::IDENTITY),

    // initialise area cache rects
    d_outerUnclippedRect(0, 0, 0, 0),
    d_innerUnclippedRect(0, 0, 0, 0),
    d_outerRectClipper(0, 0, 0, 0),
    d_innerRectClipper(0, 0, 0, 0),
    d_hitTestRect(0, 0, 0, 0),

    // cached pixel rect validity flags
    d_outerUnclippedRectValid(false),
    d_innerUnclippedRectValid(false),
    d_outerRectClipperValid(false),
    d_innerRectClipperValid(false),
    d_hitTestRectValid(false),

    // Initial update mode
    d_updateMode(WUM_VISIBLE),

    // Don't propagate mouse inputs by default.
    d_propagateMouseInputs(false)
{
    // add properties
    addStandardProperties();
}

//----------------------------------------------------------------------------//
Window::~Window(void)
{
    // most cleanup actually happened earlier in Window::destroy.

    System::getSingleton().getRenderer()->destroyGeometryBuffer(*d_geometry);
    CEGUI_DELETE_AO d_bidiVisualMapping;
}

//----------------------------------------------------------------------------//
const String& Window::getType(void) const
{
    return d_falagardType.empty() ? d_type : d_falagardType;
}

//----------------------------------------------------------------------------//
bool Window::isDisabled(bool localOnly) const
{
    const bool parent_disabled =
        (!d_parent || localOnly) ? false : d_parent->isDisabled();

    return !d_enabled || parent_disabled;
}

//----------------------------------------------------------------------------//
bool Window::isVisible(bool localOnly) const
{
    const bool parent_visible =
        (!d_parent || localOnly) ? true : d_parent->isVisible();

    return d_visible && parent_visible;
}

//----------------------------------------------------------------------------//
bool Window::isActive(void) const
{
    const bool parent_active = !d_parent ? true : d_parent->isActive();

    return d_active && parent_active;
}

//----------------------------------------------------------------------------//
bool Window::isChild(const String& name) const
{
    const size_t child_count = getChildCount();

    for (size_t i = 0; i < child_count; ++i)
        if (d_children[i]->getName() == name)
            return true;

    return false;
}

//----------------------------------------------------------------------------//
bool Window::isChild(uint ID) const
{
    const size_t child_count = getChildCount();

    for (size_t i = 0; i < child_count; ++i)
        if (d_children[i]->getID() == ID)
            return true;

    return false;
}

//----------------------------------------------------------------------------//
bool Window::isChildRecursive(uint ID) const
{
    const size_t child_count = getChildCount();

    for (size_t i = 0; i < child_count; ++i)
        if (d_children[i]->getID() == ID || d_children[i]->isChildRecursive(ID))
            return true;

    return false;
}

//----------------------------------------------------------------------------//
bool Window::isChild(const Window* window) const
{
    const size_t child_count = getChildCount();

    for (size_t i = 0; i < child_count; ++i)
        if (d_children[i] == window)
            return true;

    return false;
}

//----------------------------------------------------------------------------//
Window* Window::getChild(const String& name) const
{
    const size_t child_count = getChildCount();

    for (size_t i = 0; i < child_count; ++i)
        if (d_children[i]->getName() == name)
            return d_children[i];

    CEGUI_THROW(UnknownObjectException("Window::getChild - The Window object "
        "named '" + name + "' is not attached to Window '" + d_name + "'."));
}

//----------------------------------------------------------------------------//
Window* Window::getChild(uint ID) const
{
    const size_t child_count = getChildCount();

    for (size_t i = 0; i < child_count; ++i)
        if (d_children[i]->getID() == ID)
            return d_children[i];

    char strbuf[16];
    sprintf(strbuf, "%X", ID);
    CEGUI_THROW(UnknownObjectException("Window::getChild: A Window with ID: '" +
        String(strbuf) + "' is not attached to Window '" + d_name + "'."));
}

//----------------------------------------------------------------------------//
Window* Window::getChildRecursive(const String& name) const
{
    const size_t child_count = getChildCount();

    for (size_t i = 0; i < child_count; ++i)
    {
        const String childName(d_children[i]->getName());
        if (childName == name)
            return d_children[i];

        Window* tmp = d_children[i]->getChildRecursive(name);
        if (tmp)
            return tmp;
    }

    return 0;
}

//----------------------------------------------------------------------------//
Window* Window::getChildRecursive(uint ID) const
{
    const size_t child_count = getChildCount();

    for (size_t i = 0; i < child_count; ++i)
    {
        if (d_children[i]->getID() == ID)
            return d_children[i];

        Window* tmp = d_children[i]->getChildRecursive(ID);
        if (tmp)
            return tmp;
    }

    return 0;
}

//----------------------------------------------------------------------------//
Window* Window::getActiveChild(void)
{
    return const_cast<Window*>(
        static_cast<const Window*>(this)->getActiveChild());
}

//----------------------------------------------------------------------------//
const Window* Window::getActiveChild(void) const
{
    // are children can't be active if we are not
    if (!isActive())
        return 0;

    size_t pos = getChildCount();

    while (pos-- > 0)
    {
        // don't need full backward scan for activeness as we already know
        // 'this' is active.  NB: This uses the draw-ordered child list, as that
        // should be quicker in most cases.
        if (d_drawList[pos]->d_active)
            return d_drawList[pos]->getActiveChild();
    }

    // no child was active, therefore we are the topmost active window
    return this;
}

//----------------------------------------------------------------------------//
bool Window::isAncestor(const String& name) const
{
    // if we have no ancestor then 'name' can't be ancestor
    if (!d_parent)
        return false;

    // check our immediate parent
    if (d_parent->getName() == name)
        return true;

    // not out parent, check back up the family line
    return d_parent->isAncestor(name);
}

//----------------------------------------------------------------------------//
bool Window::isAncestor(uint ID) const
{
    // return false if we have no ancestor
    if (!d_parent)
        return false;

    // check our immediate parent
    if (d_parent->getID() == ID)
        return true;

    // not our parent, check back up the family line
    return d_parent->isAncestor(ID);
}

//----------------------------------------------------------------------------//
bool Window::isAncestor(const Window* window) const
{
    // if we have no parent, then return false
    if (!d_parent)
        return false;

    // check our immediate parent
    if (d_parent == window)
        return true;

    // not our parent, check back up the family line
    return d_parent->isAncestor(window);
}

//----------------------------------------------------------------------------//
Font* Window::getFont(bool useDefault) const
{
    if (!d_font)
        return useDefault ? System::getSingleton().getDefaultFont() : 0;

    return d_font;
}

//----------------------------------------------------------------------------//
float Window::getEffectiveAlpha(void) const
{
    if (!d_parent || !inheritsAlpha())
        return d_alpha;

    return d_alpha * d_parent->getEffectiveAlpha();
}

//----------------------------------------------------------------------------//
Rectf Window::getUnclippedOuterRect() const
{
    if (!d_outerUnclippedRectValid)
    {
        d_outerUnclippedRect = getUnclippedOuterRect_impl();
        d_outerUnclippedRectValid = true;
    }

    return d_outerUnclippedRect;
}

//----------------------------------------------------------------------------//
Rectf Window::getUnclippedInnerRect() const
{
    if (!d_innerUnclippedRectValid)
    {
        d_innerUnclippedRect = getUnclippedInnerRect_impl();
        d_innerUnclippedRectValid = true;
    }

    return d_innerUnclippedRect;
}

//----------------------------------------------------------------------------//
Rectf Window::getUnclippedRect(const bool inner) const
{
    return inner ? getUnclippedInnerRect() : getUnclippedOuterRect();
}

//----------------------------------------------------------------------------//
Rectf Window::getOuterRectClipper() const
{
    if (!d_outerRectClipperValid)
    {
        d_outerRectClipper = getOuterRectClipper_impl();
        d_outerRectClipperValid = true;
    }

    return d_outerRectClipper;
}

//----------------------------------------------------------------------------//
Rectf Window::getInnerRectClipper() const
{
    if (!d_innerRectClipperValid)
    {
        d_innerRectClipper = getInnerRectClipper_impl();
        d_innerRectClipperValid = true;
    }

    return d_innerRectClipper;
}

//----------------------------------------------------------------------------//
Rectf Window::getClipRect(const bool non_client) const
{
    return non_client ? getOuterRectClipper() : getInnerRectClipper();
}

//----------------------------------------------------------------------------//
Rectf Window::getHitTestRect() const
{
    if (!d_hitTestRectValid)
    {
        d_hitTestRect = getHitTestRect_impl();
        d_hitTestRectValid = true;
    }

    return d_hitTestRect;
}

//----------------------------------------------------------------------------//
Rectf Window::getParentElementClipIntersection(const Rectf& unclipped_area) const
{
    return unclipped_area.getIntersection(
        (d_parent && d_clippedByParent) ?
            d_parent->getClipRect(d_nonClientContent) :
            Rectf(Vector2f(0, 0),
                   System::getSingleton().getRenderer()->getDisplaySize()));
}

//----------------------------------------------------------------------------//
Rectf Window::getUnclippedOuterRect_impl() const
{
    const Rectf local(0, 0, d_pixelSize.d_width, d_pixelSize.d_height);
    return CoordConverter::windowToScreen(*this, local);
}

//----------------------------------------------------------------------------//
Rectf Window::getOuterRectClipper_impl() const
{
    return (d_surface && d_surface->isRenderingWindow()) ?
        getUnclippedOuterRect() :
        getParentElementClipIntersection(getUnclippedOuterRect());
}

//----------------------------------------------------------------------------//
Rectf Window::getUnclippedInnerRect_impl(void) const
{
    return d_windowRenderer ? d_windowRenderer->getUnclippedInnerRect() :
                              getUnclippedOuterRect();
}

//----------------------------------------------------------------------------//
Rectf Window::getInnerRectClipper_impl() const
{
    return (d_surface && d_surface->isRenderingWindow()) ?
        getUnclippedInnerRect() :
        getParentElementClipIntersection(getUnclippedInnerRect());
}

//----------------------------------------------------------------------------//
Rectf Window::getHitTestRect_impl() const
{
    // if clipped by parent wnd, hit test area is the intersection of our
    // outer rect with the parent's hit test area intersected with the
    // parent's clipper.
    if (d_parent && d_clippedByParent)
    {
        return getUnclippedOuterRect().getIntersection(
            d_parent->getHitTestRect().getIntersection(
                d_parent->getClipRect(d_nonClientContent)));
    }
    // not clipped to parent wnd, so get intersection with screen area.
    else
    {
        return getUnclippedOuterRect().getIntersection(
            Rectf(Vector2f(0, 0),
                 System::getSingleton().getRenderer()->getDisplaySize()));
    }
}

//----------------------------------------------------------------------------//
bool Window::isHit(const Vector2f& position, const bool allow_disabled) const
{
    // cannot be hit if we are disabled.
    if (!allow_disabled && isDisabled())
        return false;

    const Rectf test_area(getHitTestRect());

    if ((test_area.getWidth() == 0.0f) || (test_area.getHeight() == 0.0f))
        return false;

    return test_area.isPointInRect(position);
}

//----------------------------------------------------------------------------//
Window* Window::getChildAtPosition(const Vector2f& position) const
{
    const ChildList::const_reverse_iterator end = d_drawList.rend();

    Vector2f p;
    // if the window has RenderingWindow backing
    if (d_surface && d_surface->isRenderingWindow())
        static_cast<RenderingWindow*>(d_surface)->unprojectPoint(position, p);
    else
        p = position;

    ChildList::const_reverse_iterator child;
    for (child = d_drawList.rbegin(); child != end; ++child)
    {
        if ((*child)->isVisible())
        {
            // recursively scan children of this child windows...
            Window* const wnd = (*child)->getChildAtPosition(p);

            // return window pointer if we found a hit down the chain somewhere
            if (wnd)
                return wnd;
            // see if this child is hit and return it's pointer if it is
            else if ((*child)->isHit(p))
                return (*child);
        }
    }

    // nothing hit
    return 0;
}

//----------------------------------------------------------------------------//
Window* Window::getTargetChildAtPosition(const Vector2f& position,
                                         const bool allow_disabled) const
{
    const ChildList::const_reverse_iterator end = d_drawList.rend();

    Vector2f p;
    // if the window has RenderingWindow backing
    if (d_surface && d_surface->isRenderingWindow())
        static_cast<RenderingWindow*>(d_surface)->unprojectPoint(position, p);
    else
        p = position;

    ChildList::const_reverse_iterator child;
    for (child = d_drawList.rbegin(); child != end; ++child)
    {
        if ((*child)->isVisible())
        {
            // recursively scan children of this child windows...
            Window* const wnd =
                (*child)->getTargetChildAtPosition(p, allow_disabled);

            // return window pointer if we found a 'hit' down the chain somewhere
            if (wnd)
                return wnd;
            // see if this child is hit and return it's pointer if it is
            else if (!(*child)->isMousePassThroughEnabled() &&
                     (*child)->isHit(p, allow_disabled))
                return (*child);
        }
    }

    // nothing hit
    return 0;
}

//----------------------------------------------------------------------------//
void Window::setAlwaysOnTop(bool setting)
{
    // only react to an actual change
    if (isAlwaysOnTop() == setting)
        return;

    d_alwaysOnTop = setting;

    // move us in front of sibling windows with the same 'always-on-top'
    // setting as we have.
    if (d_parent)
    {
        Window* const org_parent = d_parent;

        org_parent->removeChild_impl(this);
        org_parent->addChild_impl(this);

        onZChange_impl();
    }

    WindowEventArgs args(this);
    onAlwaysOnTopChanged(args);
}

//----------------------------------------------------------------------------//
void Window::setEnabled(bool setting)
{
    // only react if setting has changed
    if (d_enabled == setting)
        return;

    d_enabled = setting;
    WindowEventArgs args(this);

    if (d_enabled)
    {
        // check to see if the window is actually enabled (which depends
        // upon all ancestor windows being enabled) we do this so that
        // events we fire give an accurate indication of the state of a
        // window.
        if ((d_parent && !d_parent->isDisabled()) || !d_parent)
            onEnabled(args);
    }
    else
    {
        onDisabled(args);
    }

    System::getSingleton().updateWindowContainingMouse();
}

//----------------------------------------------------------------------------//
void Window::setVisible(bool setting)
{
    // only react if setting has changed
    if (d_visible == setting)
        return;

    d_visible = setting;
    WindowEventArgs args(this);
    d_visible ? onShown(args) : onHidden(args);

    System::getSingleton().updateWindowContainingMouse();
}

//----------------------------------------------------------------------------//
void Window::activate(void)
{
    // exit if the window is not visible, since a hidden window may not be the
    // active window.
    if (!isVisible())
        return;

    // force complete release of input capture.
    // NB: This is not done via releaseCapture() because that has
    // different behaviour depending on the restoreOldCapture setting.
    if (d_captureWindow && d_captureWindow != this)
    {
        Window* const tmpCapture = d_captureWindow;
        d_captureWindow = 0;

        WindowEventArgs args(0);
        tmpCapture->onCaptureLost(args);
    }

    moveToFront();
}

//----------------------------------------------------------------------------//
void Window::deactivate(void)
{
    ActivationEventArgs args(this);
    args.otherWindow = 0;
    onDeactivated(args);
}

//----------------------------------------------------------------------------//
void Window::setClippedByParent(bool setting)
{
    // only react if setting has changed
    if (d_clippedByParent == setting)
        return;

    d_clippedByParent = setting;
    WindowEventArgs args(this);
    onClippingChanged(args);
}

//----------------------------------------------------------------------------//
void Window::setText(const String& text)
{
    d_textLogical = text;
    d_renderedStringValid = false;
    d_bidiDataValid = false;

    WindowEventArgs args(this);
    onTextChanged(args);
}

//----------------------------------------------------------------------------//
void Window::setFont(Font* font)
{
    d_font = font;
    d_renderedStringValid = false;
    WindowEventArgs args(this);
    onFontChanged(args);
}

//----------------------------------------------------------------------------//
void Window::setFont(const String& name)
{
    setFont(!name.empty() ? &FontManager::getSingleton().get(name) : 0);
}

//----------------------------------------------------------------------------//
void Window::addChild(const String& name)
{
    addChild(WindowManager::getSingleton().getWindow(name));
}

//----------------------------------------------------------------------------//
void Window::addChild(Window* window)
{
    // don't add null window or ourself as a child
    if (!window || window == this)
        return;

    addChild_impl(window);
    WindowEventArgs args(window);
    onChildAdded(args);
    window->onZChange_impl();
}

//----------------------------------------------------------------------------//
void Window::removeChild(const String& name)
{
    const size_t child_count = getChildCount();

    for (size_t i = 0; i < child_count; ++i)
    {
        if (d_children[i]->getName() == name)
        {
            removeChild(d_children[i]);
            return;
        }
    }
}

//----------------------------------------------------------------------------//
void Window::removeChild(Window* window)
{
    removeChild_impl(window);
    WindowEventArgs args(window);
    onChildRemoved(args);
    window->onZChange_impl();
}

//----------------------------------------------------------------------------//
void Window::removeChild(uint ID)
{
    const size_t child_count = getChildCount();

    for (size_t i = 0; i < child_count; ++i)
    {
        if (d_children[i]->getID() == ID)
        {
            removeChild(d_children[i]);
            return;
        }

    }
}

//----------------------------------------------------------------------------//
Window* Window::createChild(const String& type, const String& name, bool nameLocal)
{
    Window* ret = WindowManager::getSingleton().createWindow(type,
        nameLocal ? (getName() + "/" + name) : name );

    addChild(ret);
    return ret;
}

//----------------------------------------------------------------------------//
void Window::destroyChild(Window* wnd)
{
    assert(isChild(wnd) && "Window you are attempting to destroy is not a child!");

    WindowManager::getSingleton().destroyWindow(wnd);
}

//----------------------------------------------------------------------------//
void Window::destroyChild(const String& name, bool nameLocal)
{
    Window* wnd = WindowManager::getSingleton().getWindow(
        nameLocal ? (getName() + "/" + name) : name);

    destroyChild(wnd);
}

//----------------------------------------------------------------------------//
void Window::moveToFront()
{
    moveToFront_impl(false);
}

//----------------------------------------------------------------------------//
bool Window::moveToFront_impl(bool wasClicked)
{
    bool took_action = false;

    // if the window has no parent then we can have no siblings
    if (!d_parent)
    {
        // perform initial activation if required.
        if (!isActive())
        {
            took_action = true;
            ActivationEventArgs args(this);
            args.otherWindow = 0;
            onActivated(args);
        }

        return took_action;
    }

    // bring parent window to front of it's siblings
    took_action = d_parent->moveToFront_impl(wasClicked);

    // get immediate child of parent that is currently active (if any)
    Window* const activeWnd = getActiveSibling();

    // if a change in active window has occurred
    if (activeWnd != this)
    {
        took_action = true;

        // notify ourselves that we have become active
        ActivationEventArgs args(this);
        args.otherWindow = activeWnd;
        onActivated(args);

        // notify any previously active window that it is no longer active
        if (activeWnd)
        {
            args.window = activeWnd;
            args.otherWindow = this;
            args.handled = 0;
            activeWnd->onDeactivated(args);
        }
    }

    // bring us to the front of our siblings
    if (d_zOrderingEnabled &&
        (!wasClicked || d_riseOnClick) &&
        !isTopOfZOrder())
    {
        took_action = true;

        // remove us from our parent's draw list
        d_parent->removeWindowFromDrawList(*this);
        // re-attach ourselves to our parent's draw list which will move us in
        // front of sibling windows with the same 'always-on-top' setting as we
        // have.
        d_parent->addWindowToDrawList(*this);
        // notify relevant windows about the z-order change.
        onZChange_impl();
    }

    return took_action;
}

//----------------------------------------------------------------------------//
void Window::moveToBack()
{
    // if the window is active, de-activate it.
    if (isActive())
    {
        ActivationEventArgs args(this);
        args.otherWindow = 0;
        onDeactivated(args);
    }

    // we only proceed if we have a parent (otherwise we can have no siblings)
    if (d_parent)
    {
        if (d_zOrderingEnabled)
        {
            // remove us from our parent's draw list
            d_parent->removeWindowFromDrawList(*this);
            // re-attach ourselves to our parent's draw list which will move us
            // in behind sibling windows with the same 'always-on-top' setting
            // as we have.
            d_parent->addWindowToDrawList(*this, true);
            // notify relevant windows about the z-order change.
            onZChange_impl();
        }

        d_parent->moveToBack();
    }
}

//----------------------------------------------------------------------------//
bool Window::captureInput(void)
{
    // we can only capture if we are the active window (LEAVE THIS ALONE!)
    if (!isActive())
        return false;

    if (d_captureWindow != this)
    {
        Window* const current_capture = d_captureWindow;
        d_captureWindow = this;
        WindowEventArgs args(this);

        // inform window which previously had capture that it doesn't anymore.
        if (current_capture && current_capture != this && !d_restoreOldCapture)
            current_capture->onCaptureLost(args);

        if (d_restoreOldCapture)
            d_oldCapture = current_capture;

        onCaptureGained(args);
    }

    return true;
}

//----------------------------------------------------------------------------//
void Window::releaseInput(void)
{
    // if we are not the window that has capture, do nothing
    if (!isCapturedByThis())
        return;

    // restore old captured window if that mode is set
    if (d_restoreOldCapture)
    {
        d_captureWindow = d_oldCapture;

        // check for case when there was no previously captured window
        if (d_oldCapture)
        {
            d_oldCapture = 0;
            d_captureWindow->moveToFront();
        }

    }
    else
        d_captureWindow = 0;

    WindowEventArgs args(this);
    onCaptureLost(args);
}

//----------------------------------------------------------------------------//
void Window::setRestoreOldCapture(bool setting)
{
    d_restoreOldCapture = setting;

    const size_t child_count = getChildCount();

    for (size_t i = 0; i < child_count; ++i)
        d_children[i]->setRestoreOldCapture(setting);
}

//----------------------------------------------------------------------------//
void Window::setAlpha(const float alpha)
{
    // clamp this to the valid range [0.0, 1.0]
    d_alpha = ceguimax(ceguimin(alpha, 1.0f), 0.0f);
    WindowEventArgs args(this);
    onAlphaChanged(args);
}

//----------------------------------------------------------------------------//
void Window::setInheritsAlpha(bool setting)
{
    if (d_inheritsAlpha != setting)
    {
        // store old effective alpha so we can test if alpha value changes due
        // to new setting.
        const float oldAlpha = getEffectiveAlpha();

        // notify about the setting change.
        d_inheritsAlpha = setting;

        WindowEventArgs args(this);
        onInheritsAlphaChanged(args);

        // if effective alpha has changed fire notification about that too
        if (oldAlpha != getEffectiveAlpha())
        {
            args.handled = 0;
            onAlphaChanged(args);
        }
    }
}

//----------------------------------------------------------------------------//
void Window::invalidate(void)
{
    invalidate(false);
}

//----------------------------------------------------------------------------//
void Window::invalidate(const bool recursive)
{
    invalidate_impl(recursive);
    System::getSingleton().signalRedraw();
}

//----------------------------------------------------------------------------//
void Window::invalidate_impl(const bool recursive)
{
    d_needsRedraw = true;
    invalidateRenderingSurface();

    if (recursive)
    {
        const size_t child_count = getChildCount();
        for (size_t i = 0; i < child_count; ++i)
            d_children[i]->invalidate_impl(true);
    }
}

//----------------------------------------------------------------------------//
void Window::render()
{
    // don't do anything if window is not visible
    if (!isVisible())
        return;

    // get rendering context
    RenderingContext ctx;
    getRenderingContext(ctx);

    // clear geometry from surface if it's ours
    if (ctx.owner == this)
        ctx.surface->clearGeometry();

    // redraw if no surface set, or if surface is invalidated
    if (!d_surface || d_surface->isInvalidated())
    {
        // perform drawing for 'this' Window
        drawSelf(ctx);

        // render any child windows
        const size_t child_count = getChildCount();
        for (size_t i = 0; i < child_count; ++i)
            d_drawList[i]->render();
    }

    // do final rendering for surface if it's ours
    if (ctx.owner == this)
        ctx.surface->draw();
}

//----------------------------------------------------------------------------//
void Window::drawSelf(const RenderingContext& ctx)
{
    bufferGeometry(ctx);
    queueGeometry(ctx);
}

//----------------------------------------------------------------------------//
void Window::bufferGeometry(const RenderingContext&)
{
    if (d_needsRedraw)
    {
        // dispose of already cached geometry.
        d_geometry->reset();

        // signal rendering started
        WindowEventArgs args(this);
        onRenderingStarted(args);

        // HACK: ensure our rendered string content is up to date
        getRenderedString();

        // get derived class or WindowRenderer to re-populate geometry buffer.
        if (d_windowRenderer)
            d_windowRenderer->render();
        else
            populateGeometryBuffer();

        // signal rendering ended
        args.handled = 0;
        onRenderingEnded(args);

        // mark ourselves as no longer needed a redraw.
        d_needsRedraw = false;
    }
}

//----------------------------------------------------------------------------//
void Window::queueGeometry(const RenderingContext& ctx)
{
    // add geometry so that it gets drawn to the target surface.
    ctx.surface->addGeometryBuffer(ctx.queue, *d_geometry);
}

//----------------------------------------------------------------------------//
void Window::setParent(Window* parent)
{
    d_parent = parent;

    // if we do not have a surface, xfer any surfaces from our children to
    // whatever our target surface now is.
    if (!d_surface)
        transferChildSurfaces();
    // else, since we have a surface, child surfaces stay with us.  Though we
    // must now ensure /our/ surface is xferred if it is a RenderingWindow.
    else if (d_surface->isRenderingWindow())
    {
        // target surface is eihter the parent's target, or the default root.
        RenderingSurface& tgt = d_parent ?
            d_parent->getTargetRenderingSurface() :
            System::getSingleton().getRenderer()->getDefaultRenderingRoot();

        tgt.transferRenderingWindow(static_cast<RenderingWindow&>(*d_surface));
    }
}

//----------------------------------------------------------------------------//
float Window::getParentPixelWidth(void) const
{
    return getParentPixelSize().d_width;
}

//----------------------------------------------------------------------------//
float Window::getParentPixelHeight(void) const
{
    return getParentPixelSize().d_height;
}

//----------------------------------------------------------------------------//
Sizef Window::getParentPixelSize(void) const
{
    return d_parent ?
           d_parent->d_pixelSize :
           System::getSingleton().getRenderer()->getDisplaySize();
}

//----------------------------------------------------------------------------//
void Window::cleanupChildren(void)
{
    while(getChildCount() != 0)
    {
        Window* wnd = d_children[0];

        // always remove child
        removeChild(wnd);

        // destroy child if that is required
        if (wnd->isDestroyedByParent())
            WindowManager::getSingleton().destroyWindow(wnd);
    }
}

//----------------------------------------------------------------------------//
void Window::addChild_impl(Window* wnd)
{
    // if window is already attached, detach it first (will fire normal events)
    Window* const old_parent = wnd->getParent();
    if (old_parent)
        old_parent->removeChild(wnd);

    addWindowToDrawList(*wnd);

    // add window to child list
    d_children.push_back(wnd);

    // set the parent window
    wnd->setParent(this);

    // update area rects and content for the added window
    wnd->notifyScreenAreaChanged(true);
    wnd->invalidate(true);
    
    // correctly call parent sized notification if needed.
    if (!old_parent || old_parent->getPixelSize() != getPixelSize())
    {
        WindowEventArgs args(this);
        wnd->onParentSized(args);
    }
}

//----------------------------------------------------------------------------//
void Window::removeChild_impl(Window* wnd)
{
    // remove from draw list
    removeWindowFromDrawList(*wnd);

    // if window has no children, we can't remove 'wnd' from the child list
    if (d_children.empty())
        return;

    // find this window in the child list
    const ChildList::iterator position =
        std::find(d_children.begin(), d_children.end(), wnd);

    // if the window was found in the child list
    if (position != d_children.end())
    {
        // remove window from child list
        d_children.erase(position);
        // reset windows parent so it's no longer this window.
        wnd->setParent(0);
    }
}

//----------------------------------------------------------------------------//
void Window::onZChange_impl(void)
{
    if (!d_parent)
    {
        WindowEventArgs args(this);
        onZChanged(args);
    }
    else
    {
        const size_t child_count = d_parent->getChildCount();

        for (size_t i = 0; i < child_count; ++i)
        {
            WindowEventArgs args(d_parent->d_children[i]);
            d_parent->d_children[i]->onZChanged(args);
        }

    }

    System::getSingleton().updateWindowContainingMouse();
}

//----------------------------------------------------------------------------//
const Image* Window::getMouseCursor(bool useDefault) const
{
    if (d_mouseCursor != (const Image*)DefaultMouseCursor)
        return d_mouseCursor;
    else
        return useDefault ? System::getSingleton().getDefaultMouseCursor() : 0;
}

//----------------------------------------------------------------------------//
void Window::setMouseCursor(const String& name)
{
    setMouseCursor(
        &ImageManager::getSingleton().get(name));
}

//----------------------------------------------------------------------------//
void Window::setMouseCursor(const Image* image)
{
    d_mouseCursor = image;

    if (System::getSingleton().getWindowContainingMouse() == this)
    {
        const Image* const default_cursor =
            reinterpret_cast<const Image*>(DefaultMouseCursor);

        if (default_cursor == image)
            image = System::getSingleton().getDefaultMouseCursor();

        MouseCursor::getSingleton().setImage(image);
    }
}

//----------------------------------------------------------------------------//
void Window::setMouseCursor(MouseCursorImage image)
{
    setMouseCursor((const Image*)image);
}

//----------------------------------------------------------------------------//
void Window::setID(uint ID)
{
    if (d_ID == ID)
        return;

    d_ID = ID;

    WindowEventArgs args(this);
    onIDChanged(args);
}

//----------------------------------------------------------------------------//
void Window::setDestroyedByParent(bool setting)
{
    if (d_destroyedByParent == setting)
        return;

    d_destroyedByParent = setting;

    WindowEventArgs args(this);
    onParentDestroyChanged(args);
}

//----------------------------------------------------------------------------//
void Window::generateAutoRepeatEvent(MouseButton button)
{
    MouseEventArgs ma(this);
    ma.position =
        getUnprojectedPosition(MouseCursor::getSingleton().getPosition());
    ma.moveDelta = Vector2f(0.0f, 0.0f);
    ma.button = button;
    ma.sysKeys = System::getSingleton().getSystemKeys();
    ma.wheelChange = 0;
    onMouseButtonDown(ma);
}

//----------------------------------------------------------------------------//
void Window::addStandardProperties(void)
{
    // experimental, even easier to maintan, property definition, neat eh?
    CEGUI_DEFINE_PROPERTY((CEGUI_NEW_AO TplProperty<Window, float>(
        "Alpha", "Property to get/set the alpha value of the Window. Value is floating point number.",
        &Window::setAlpha, &Window::getAlpha, 1.0f)
    ));

    //addProperty(&d_alphaProperty);
    addProperty(&d_alwaysOnTopProperty);
    addProperty(&d_clippedByParentProperty);
    addProperty(&d_destroyedByParentProperty);
    addProperty(&d_disabledProperty);
    addProperty(&d_fontProperty);
    addProperty(&d_IDProperty);
    addProperty(&d_inheritsAlphaProperty);
    addProperty(&d_mouseCursorProperty);
    addProperty(&d_restoreOldCaptureProperty);
    addProperty(&d_textProperty);
    addProperty(&d_visibleProperty);
    addProperty(&d_zOrderChangeProperty);
    addProperty(&d_wantsMultiClicksProperty);
    addProperty(&d_mouseButtonAutoRepeatProperty);
    addProperty(&d_autoRepeatDelayProperty);
    addProperty(&d_autoRepeatRateProperty);
    addProperty(&d_distInputsProperty);
    addProperty(&d_tooltipTypeProperty);
    addProperty(&d_tooltipProperty);
    addProperty(&d_inheritsTooltipProperty);
    addProperty(&d_riseOnClickProperty);
    addProperty(&d_vertAlignProperty);
    addProperty(&d_horzAlignProperty);
    addProperty(&d_unifiedAreaRectProperty);
    addProperty(&d_unifiedPositionProperty);
    addProperty(&d_unifiedXPositionProperty);
    addProperty(&d_unifiedYPositionProperty);
    addProperty(&d_unifiedSizeProperty);
    addProperty(&d_unifiedWidthProperty);
    addProperty(&d_unifiedHeightProperty);
    addProperty(&d_unifiedMinSizeProperty);
    addProperty(&d_unifiedMaxSizeProperty);
    addProperty(&d_mousePassThroughEnabledProperty);
    addProperty(&d_windowRendererProperty);
    addProperty(&d_lookNFeelProperty);
    addProperty(&d_dragDropTargetProperty);
    addProperty(&d_autoRenderingSurfaceProperty);
    addProperty(&d_rotationProperty);
    addProperty(&d_nonClientProperty);
    addProperty(&d_textParsingEnabledProperty);
    addProperty(&d_marginProperty);
    addProperty(&d_updateModeProperty);
    addProperty(&d_mouseInputPropagationProperty);

    // we ban some of these properties from xml for auto windows by default
    if (isAutoWindow())
    {
        banPropertyFromXML(&d_destroyedByParentProperty);
        banPropertyFromXML(&d_vertAlignProperty);
        banPropertyFromXML(&d_horzAlignProperty);
        banPropertyFromXML(&d_unifiedAreaRectProperty);
        banPropertyFromXML(&d_unifiedPositionProperty);
        banPropertyFromXML(&d_unifiedXPositionProperty);
        banPropertyFromXML(&d_unifiedYPositionProperty);
        banPropertyFromXML(&d_unifiedSizeProperty);
        banPropertyFromXML(&d_unifiedWidthProperty);
        banPropertyFromXML(&d_unifiedHeightProperty);
        banPropertyFromXML(&d_unifiedMinSizeProperty);
        banPropertyFromXML(&d_unifiedMaxSizeProperty);
        banPropertyFromXML(&d_windowRendererProperty);
        banPropertyFromXML(&d_lookNFeelProperty);
    }
}

//----------------------------------------------------------------------------//
bool Window::isZOrderingEnabled(void) const
{
    return d_zOrderingEnabled;
}

//----------------------------------------------------------------------------//
void Window::setZOrderingEnabled(bool setting)
{
    d_zOrderingEnabled = setting;
}

//----------------------------------------------------------------------------//
bool Window::wantsMultiClickEvents(void) const
{
    return d_wantsMultiClicks;
}

//----------------------------------------------------------------------------//
void Window::setWantsMultiClickEvents(bool setting)
{
    d_wantsMultiClicks = setting;
}

//----------------------------------------------------------------------------//
bool Window::isMouseAutoRepeatEnabled(void) const
{
    return d_autoRepeat;
}

//----------------------------------------------------------------------------//
float Window::getAutoRepeatDelay(void) const
{
    return d_repeatDelay;
}

//----------------------------------------------------------------------------//
float Window::getAutoRepeatRate(void) const
{
    return d_repeatRate;
}

//----------------------------------------------------------------------------//
void Window::setMouseAutoRepeatEnabled(bool setting)
{
    if (d_autoRepeat == setting)
        return;

    d_autoRepeat = setting;
    d_repeatButton = NoButton;

    // FIXME: There is a potential issue here if this setting is
    // FIXME: changed _while_ the mouse is auto-repeating, and
    // FIXME: the 'captured' state of input could get messed up.
    // FIXME: The alternative is to always release here, but that
    // FIXME: has a load of side effects too - so for now nothing
    // FIXME: is done.  This whole aspect of the system needs a
    // FIXME: review an reworking - though such a change was
    // FIXME: beyond the scope of the bug-fix that originated this
    // FIXME: comment block.  PDT - 30/10/06
}

//----------------------------------------------------------------------------//
void Window::setAutoRepeatDelay(float delay)
{
    d_repeatDelay = delay;
}

//----------------------------------------------------------------------------//
void Window::setAutoRepeatRate(float rate)
{
    d_repeatRate = rate;
}

//----------------------------------------------------------------------------//
void Window::update(float elapsed)
{       
    // perform update for 'this' Window
    updateSelf(elapsed);

    // update underlying RenderingWinodw if needed
    if (d_surface && d_surface->isRenderingWindow())
        static_cast<RenderingWindow*>(d_surface)->update(elapsed);

    UpdateEventArgs e(this,elapsed);
    fireEvent(EventUpdated,e,EventNamespace);

    // update child windows
    for (size_t i = 0; i < getChildCount(); ++i)
    {
        // update children based on their WindowUpdateMode setting.
        if (d_children[i]->d_updateMode == WUM_ALWAYS ||
                (d_children[i]->d_updateMode == WUM_VISIBLE &&
                 d_children[i]->isVisible(true)))
        {
            d_children[i]->update(elapsed);
        }
    }
}

//----------------------------------------------------------------------------//
void Window::updateSelf(float elapsed)
{
    // Mouse button autorepeat processing.
    if (d_autoRepeat && d_repeatButton != NoButton)
    {
        d_repeatElapsed += elapsed;

        if (d_repeating)
        {
            if (d_repeatElapsed > d_repeatRate)
            {
                d_repeatElapsed -= d_repeatRate;
                // trigger the repeated event
                generateAutoRepeatEvent(d_repeatButton);
            }
        }
        else
        {
            if (d_repeatElapsed > d_repeatDelay)
            {
                d_repeatElapsed = 0;
                d_repeating = true;
                // trigger the repeated event
                generateAutoRepeatEvent(d_repeatButton);
            }
        }
    }

    // allow for updates within an assigned WindowRenderer
    if (d_windowRenderer)
        d_windowRenderer->update(elapsed);
}

//----------------------------------------------------------------------------//
bool Window::distributesCapturedInputs(void) const
{
    return d_distCapturedInputs;
}

//----------------------------------------------------------------------------//
void Window::setDistributesCapturedInputs(bool setting)
{
    d_distCapturedInputs = setting;
}

//----------------------------------------------------------------------------//
void Window::notifyDragDropItemEnters(DragContainer* item)
{
    if (!item)
        return;

    DragDropEventArgs args(this);
    args.dragDropItem = item;
    onDragDropItemEnters(args);
}

//----------------------------------------------------------------------------//
void Window::notifyDragDropItemLeaves(DragContainer* item)
{
    if (!item)
        return;

    DragDropEventArgs args(this);
    args.dragDropItem = item;
    onDragDropItemLeaves(args);
}

//----------------------------------------------------------------------------//
void Window::notifyDragDropItemDropped(DragContainer* item)
{
    if (!item)
        return;

    DragDropEventArgs args(this);
    args.dragDropItem = item;
    onDragDropItemDropped(args);
}

//----------------------------------------------------------------------------//
void Window::destroy(void)
{
    // because we know that people do not read the API ref properly,
    // here is some protection to ensure that WindowManager does the
    // destruction and not anyone else.
    WindowManager& wmgr = WindowManager::getSingleton();

    if (wmgr.isWindowPresent(this->getName()))
    {
        wmgr.destroyWindow(this);

        // now return, the rest of what we need to do will happen
        // once WindowManager re-calls this method.
        return;
    }

    // signal our imminent destruction
    WindowEventArgs args(this);
    onDestructionStarted(args);

    releaseInput();

    // let go of the tooltip if we have it
    Tooltip* const tip = getTooltip();
    if (tip && tip->getTargetWindow()==this)
        tip->setTargetWindow(0);

    // ensure custom tooltip is cleaned up
    setTooltip(static_cast<Tooltip*>(0));

    // clean up looknfeel related things
    if (!d_lookName.empty())
    {
        d_windowRenderer->onLookNFeelUnassigned();
        WidgetLookManager::getSingleton().getWidgetLook(d_lookName).
            cleanUpWidget(*this);
    }

    // free any assigned WindowRenderer
    if (d_windowRenderer != 0)
    {
        d_windowRenderer->onDetach();
        WindowRendererManager::getSingleton().
            destroyWindowRenderer(d_windowRenderer);
        d_windowRenderer = 0;
    }

    // double check we are detached from parent
    if (d_parent)
        d_parent->removeChild(this);

    cleanupChildren();

    releaseRenderingWindow();
    invalidate();
}

//----------------------------------------------------------------------------//
bool Window::isUsingDefaultTooltip(void) const
{
    return d_customTip == 0;
}

//----------------------------------------------------------------------------//
Tooltip* Window::getTooltip(void) const
{
    return isUsingDefaultTooltip() ? System::getSingleton().getDefaultTooltip():
                                     d_customTip;
}

//----------------------------------------------------------------------------//
void Window::setTooltip(Tooltip* tooltip)
{
    // destroy current custom tooltip if one exists and we created it
    if (d_customTip && d_weOwnTip)
        WindowManager::getSingleton().destroyWindow(d_customTip);

    // set new custom tooltip
    d_weOwnTip = false;
    d_customTip = tooltip;
}

//----------------------------------------------------------------------------//
void Window::setTooltipType(const String& tooltipType)
{
    // destroy current custom tooltip if one exists and we created it
    if (d_customTip && d_weOwnTip)
        WindowManager::getSingleton().destroyWindow(d_customTip);

    if (tooltipType.empty())
    {
        d_customTip = 0;
        d_weOwnTip = false;
    }
    else
    {
        CEGUI_TRY
        {
            d_customTip = static_cast<Tooltip*>(
                WindowManager::getSingleton().createWindow(
                    tooltipType, getName() + TooltipNameSuffix));
            d_weOwnTip = true;
        }
        CEGUI_CATCH (UnknownObjectException&)
        {
            d_customTip = 0;
            d_weOwnTip = false;
        }
    }
}

//----------------------------------------------------------------------------//
String Window::getTooltipType(void) const
{
    return isUsingDefaultTooltip() ? String("") : d_customTip->getType();
}

//----------------------------------------------------------------------------//
void Window::setTooltipText(const String& tip)
{
    d_tooltipText = tip;

    Tooltip* const tooltip = getTooltip();

    if (tooltip && tooltip->getTargetWindow() == this)
        tooltip->setText(tip);
}

//----------------------------------------------------------------------------//
const String& Window::getTooltipText(void) const
{
    if (d_inheritsTipText && d_parent && d_tooltipText.empty())
        return d_parent->getTooltipText();
    else
        return d_tooltipText;
}

//----------------------------------------------------------------------------//
bool Window::inheritsTooltipText(void) const
{
    return d_inheritsTipText;
}

//----------------------------------------------------------------------------//
void Window::setInheritsTooltipText(bool setting)
{
    d_inheritsTipText = setting;
}

//----------------------------------------------------------------------------//
void Window::setArea_impl(const UVector2& pos, const UVector2& size,
                          bool topLeftSizing, bool fireEvents)
{
    // we make sure the screen areas are recached when this is called as we need
    // it in most cases
    d_outerUnclippedRectValid = false;
    d_innerUnclippedRectValid = false;
    d_outerRectClipperValid = false;
    d_innerRectClipperValid = false;
    d_hitTestRectValid = false;

    // notes of what we did
    bool moved = false, sized;

    // save original size so we can work out how to behave later on
    const Sizef oldSize(d_pixelSize);

    // calculate pixel sizes for everything, so we have a common format for
    // comparisons.
    Vector2f absMax(CoordConverter::asAbsolute(d_maxSize,
        System::getSingleton().getRenderer()->getDisplaySize()));
    Vector2f absMin(CoordConverter::asAbsolute(d_minSize,
        System::getSingleton().getRenderer()->getDisplaySize()));

    const Sizef base_size((d_parent && !d_nonClientContent) ?
                            d_parent->getUnclippedInnerRect().getSize() :
                            getParentPixelSize());

    const Vector2f pixelSizeVector = CoordConverter::asAbsolute(size, base_size);
    d_pixelSize = Sizef(pixelSizeVector.d_x, pixelSizeVector.d_y);

    // limit new pixel size to: minSize <= newSize <= maxSize
    if (d_pixelSize.d_width < absMin.d_x)
        d_pixelSize.d_width = absMin.d_x;
    else if (d_pixelSize.d_width > absMax.d_x)
        d_pixelSize.d_width = absMax.d_x;
    if (d_pixelSize.d_height < absMin.d_y)
        d_pixelSize.d_height = absMin.d_y;
    else if (d_pixelSize.d_height > absMax.d_y)
        d_pixelSize.d_height = absMax.d_y;

    // TODO: size vs vector
    d_area.setSize(USize(size.d_x, size.d_y));
    sized = (d_pixelSize != oldSize);

    // If this is a top/left edge sizing op, only modify position if the size
    // actually changed.  If it is not a sizing op, then position may always
    // change.
    if (!topLeftSizing || sized)
    {
        // only update position if a change has occurred.
        if (pos != d_area.d_min)
        {
            d_area.setPosition(pos);
            moved = true;
        }
    }

    // fire events as required
    if (fireEvents)
    {
        WindowEventArgs args(this);

        if (moved)
        {
            onMoved(args);
            // reset handled so 'sized' event can re-use (since  wo do not care
            // about it)
            args.handled = 0;
        }

        if (sized)
            onSized(args);
    }

    if (moved || sized)
        System::getSingleton().updateWindowContainingMouse();

    // update geometry position and clipping if nothing from above appears to
    // have done so already (NB: may be occasionally wasteful, but fixes bugs!)
    if (!d_outerUnclippedRectValid)
        updateGeometryRenderSettings();
}

//----------------------------------------------------------------------------//
void Window::setArea(const UDim& xpos, const UDim& ypos,
                     const UDim& width, const UDim& height)
{
    setArea(UVector2(xpos, ypos), UVector2(width, height));
}

//----------------------------------------------------------------------------//
void Window::setArea(const UVector2& pos, const UVector2& size)
{
    // Limit the value we set to something that's within the constraints
    // specified via the min and max size settings.

    // get size of 'base' - i.e. the size of the parent region.
    const Sizef base_sz((d_parent && !d_nonClientContent) ?
                              d_parent->getUnclippedInnerRect().getSize() :
                              getParentPixelSize());

    UVector2 newsz(size);
    constrainUVector2ToMinSize(base_sz, newsz);
    constrainUVector2ToMaxSize(base_sz, newsz);

    setArea_impl(pos, newsz);
}

//----------------------------------------------------------------------------//
void Window::setArea(const URect& area)
{
    // TODO: size vs vector
    const USize sz = area.getSize();
    setArea(area.d_min, UVector2(sz.d_width, sz.d_height));
}

//----------------------------------------------------------------------------//
void Window::setPosition(const UVector2& pos)
{
    // todo: vector vs size
    const USize sz = d_area.getSize();
    setArea_impl(pos, UVector2(sz.d_width, sz.d_height));
}

//----------------------------------------------------------------------------//
void Window::setXPosition(const UDim& x)
{
    // todo: vector vs size
    const USize sz = d_area.getSize();
    setArea_impl(UVector2(x, d_area.d_min.d_y), UVector2(sz.d_width, sz.d_height));
}

//----------------------------------------------------------------------------//
void Window::setYPosition(const UDim& y)
{
    // todo: vector vs size
    const USize sz = d_area.getSize();
    setArea_impl(UVector2(d_area.d_min.d_x, y), UVector2(sz.d_width, sz.d_height));
}

//----------------------------------------------------------------------------//
void Window::setSize(const UVector2& size)
{
    // Limit the value we set to something that's within the constraints
    // specified via the min and max size settings.

    // get size of 'base' - i.e. the size of the parent region.
    const Sizef base_sz((d_parent && !d_nonClientContent) ?
                              d_parent->getUnclippedInnerRect().getSize() :
                              getParentPixelSize());

    UVector2 newsz(size);
    constrainUVector2ToMinSize(base_sz, newsz);
    constrainUVector2ToMaxSize(base_sz, newsz);

    // set the new size.
    setArea_impl(d_area.getPosition(), newsz);
}

//----------------------------------------------------------------------------//
void Window::setWidth(const UDim& width)
{
    setSize(UVector2(width, d_area.getSize().d_height));
}

//----------------------------------------------------------------------------//
void Window::setHeight(const UDim& height)
{
    setSize(UVector2(d_area.getSize().d_width, height));
}

//----------------------------------------------------------------------------//
void Window::setMaxSize(const UVector2& size)
{
    d_maxSize = size;

    // Apply set maximum size to the windows set size.
    // We can't use code in setArea[_impl] to adjust the set size, because
    // that code has to ensure that it's possible for a size constrained
    // window to 'recover' it's original (scaled) sizing when the constraint
    // no longer needs to be applied.

    // get size of 'base' - i.e. the size of the parent region.
    const Sizef base_sz((d_parent && !d_nonClientContent) ?
                              d_parent->getUnclippedInnerRect().getSize() :
                              getParentPixelSize());

    UVector2 wnd_sz(getSize());

    if (constrainUVector2ToMaxSize(base_sz, wnd_sz))
        setSize(wnd_sz);
}

//----------------------------------------------------------------------------//
void Window::setMinSize(const UVector2& size)
{
    d_minSize = size;

    // Apply set minimum size to the windows set size.
    // We can't use code in setArea[_impl] to adjust the set size, because
    // that code has to ensure that it's possible for a size constrained
    // window to 'recover' it's original (scaled) sizing when the constraint
    // no longer needs to be applied.

    // get size of 'base' - i.e. the size of the parent region.
    const Sizef base_sz((d_parent && !d_nonClientContent) ?
                              d_parent->getUnclippedInnerRect().getSize() :
                              getParentPixelSize());

    UVector2 wnd_sz(getSize());

    if (constrainUVector2ToMinSize(base_sz, wnd_sz))
        setSize(wnd_sz);
}

//----------------------------------------------------------------------------//
const URect& Window::getArea() const
{
    return d_area;
}

//----------------------------------------------------------------------------//
const UVector2& Window::getPosition() const
{
    return d_area.d_min;
}

//----------------------------------------------------------------------------//
const UDim& Window::getXPosition() const
{
    return d_area.d_min.d_x;
}

//----------------------------------------------------------------------------//
const UDim& Window::getYPosition() const
{
    return d_area.d_min.d_y;
}

//----------------------------------------------------------------------------//
UVector2 Window::getSize() const
{
    // todo: vector vs size
    const USize sz = d_area.getSize();
    return UVector2(sz.d_width, sz.d_height);
}

//----------------------------------------------------------------------------//
UDim Window::getWidth() const
{
    return d_area.getSize().d_width;
}

//----------------------------------------------------------------------------//
UDim Window::getHeight() const
{
    return d_area.getSize().d_height;
}

//----------------------------------------------------------------------------//
const UVector2& Window::getMaxSize() const
{
    return d_maxSize;
}

//----------------------------------------------------------------------------//
const UVector2& Window::getMinSize() const
{
    return d_minSize;
}

//----------------------------------------------------------------------------//
void Window::setVerticalAlignment(const VerticalAlignment alignment)
{
    if (d_vertAlign == alignment)
        return;

    d_vertAlign = alignment;

    WindowEventArgs args(this);
    onVerticalAlignmentChanged(args);
}

//----------------------------------------------------------------------------//
void Window::setHorizontalAlignment(const HorizontalAlignment alignment)
{
    if (d_horzAlign == alignment)
        return;

    d_horzAlign = alignment;

    WindowEventArgs args(this);
    onHorizontalAlignmentChanged(args);
}

//----------------------------------------------------------------------------//
const String& Window::getLookNFeel() const
{
    return d_lookName;
}

//----------------------------------------------------------------------------//
void Window::setLookNFeel(const String& look)
{
    if (!d_windowRenderer)
        CEGUI_THROW(NullObjectException("Window::setLookNFeel: There must be a "
            "window renderer assigned to the window '" + d_name +
            "' to set its look'n'feel"));

    WidgetLookManager& wlMgr = WidgetLookManager::getSingleton();
    if (!d_lookName.empty())
    {
        d_windowRenderer->onLookNFeelUnassigned();
        const WidgetLookFeel& wlf = wlMgr.getWidgetLook(d_lookName);
        wlf.cleanUpWidget(*this);
    }

    d_lookName = look;
    Logger::getSingleton().logEvent("Assigning LookNFeel '" + look +
        "' to window '" + d_name + "'.", Informative);

    // Work to initialise the look and feel...
    const WidgetLookFeel& wlf = wlMgr.getWidgetLook(look);
    // Get look and feel to initialise the widget as it needs.
    wlf.initialiseWidget(*this);
    // do the necessary binding to the stuff added by the look and feel
    initialiseComponents();
    // let the window renderer know about this
    d_windowRenderer->onLookNFeelAssigned();

    invalidate();
}

//----------------------------------------------------------------------------//
void Window::setModalState(bool state)
{
    // do nothing if state isn't changing
    if (getModalState() == state)
        return;

    // if going modal
    if (state)
    {
        activate();
        System::getSingleton().setModalTarget(this);
    }
    // clear the modal target
    else
        System::getSingleton().setModalTarget(0);
}

//----------------------------------------------------------------------------//
void Window::performChildWindowLayout()
{
    if (d_lookName.empty())
        return;

    // here we just grab the look and feel and get it to layout it's children
    CEGUI_TRY
    {
        const WidgetLookFeel& wlf =
            WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
        // get look'n'feel to layout any child windows it created.
        wlf.layoutChildWidgets(*this);
    }
    CEGUI_CATCH (UnknownObjectException&)
    {
        Logger::getSingleton().logEvent("Window::performChildWindowLayout: "
            "assigned widget look was not found.", Errors);
    }

    if (d_windowRenderer)
        d_windowRenderer->performChildWindowLayout();
}

//----------------------------------------------------------------------------//
const String& Window::getUserString(const String& name) const
{
    UserStringMap::const_iterator iter = d_userStrings.find(name);

    if (iter == d_userStrings.end())
        CEGUI_THROW(UnknownObjectException(
            "Window::getUserString: a user string named '" + name +
            "' is not defined for Window '" + d_name + "'."));

    return (*iter).second;
}

//----------------------------------------------------------------------------//
bool Window::isUserStringDefined(const String& name) const
{
     return d_userStrings.find(name) != d_userStrings.end();
}

//----------------------------------------------------------------------------//
void Window::setUserString(const String& name, const String& value)
{
    d_userStrings[name] = value;
}

//----------------------------------------------------------------------------//
void Window::writeXMLToStream(XMLSerializer& xml_stream) const
{
    // just stop now if we are'nt allowed to write XML
    if (!d_allowWriteXML)
        return;

    // output opening Window tag
    xml_stream.openTag("Window")
        .attribute("Type", getType());
    // write name if not auto-generated
    if (getName().compare(0, WindowManager::GeneratedWindowNameBase.length(),
                          WindowManager::GeneratedWindowNameBase) != 0)
    {
        xml_stream.attribute("Name", getName());
    }
    // write out properties.
    writePropertiesXML(xml_stream);
    // write out attached child windows.
    writeChildWindowsXML(xml_stream);
    // now ouput closing Window tag
    xml_stream.closeTag();
}

//----------------------------------------------------------------------------//
int Window::writePropertiesXML(XMLSerializer& xml_stream) const
{
    int propertiesWritten = 0;
    PropertySet::Iterator iter =  PropertySet::getIterator();

    while(!iter.isAtEnd())
    {
        // first we check to make sure the property is'nt banned from XML
        if (!isPropertyBannedFromXML(iter.getCurrentValue()))
        {
            CEGUI_TRY
            {
                // only write property if it's not at the default state
                if (!isPropertyAtDefault(iter.getCurrentValue()))
                {
                    iter.getCurrentValue()->writeXMLToStream(this, xml_stream);
                    ++propertiesWritten;
                }
            }
            CEGUI_CATCH (InvalidRequestException&)
            {
                // This catches errors from the MultiLineColumnList for example
                Logger::getSingleton().logEvent(
                    "Window::writePropertiesXML: property receiving failed.  "
                    "Continuing...", Errors);
            }
        }

        ++iter;
    }

    return propertiesWritten;
}

//----------------------------------------------------------------------------//
int Window::writeChildWindowsXML(XMLSerializer& xml_stream) const
{
    int windowsWritten = 0;

    for (uint i = 0; i < getChildCount(); ++i)
    {
        const Window* const child = d_children[i];

        // conditional to ensure that auto created windows are handled
        // seperately.
        if (!child->isAutoWindow())
        {
            child->writeXMLToStream(xml_stream);
            ++windowsWritten;
        }
        // this is one of those auto created windows so we do some special
        // handling
        else if (child->writeAutoChildWindowXML(xml_stream))
        {
            ++windowsWritten;
        }
    }

    return windowsWritten;
}

//----------------------------------------------------------------------------//
bool Window::writeAutoChildWindowXML(XMLSerializer& xml_stream) const
{

    // just stop now if we are'nt allowed to write XML
    if (!d_allowWriteXML)
        return false;

    // we temporarily output to this string stream to see if have to do the tag
    // at all.  Make sure this stream does UTF-8
    std::ostringstream ss;
    XMLSerializer xml(ss);
    xml.openTag("AutoWindow");
    // Create the XML Child Tree
    // write out properties.
    writePropertiesXML(xml);
    // write out attached child windows.
    writeChildWindowsXML(xml);
    xml.closeTag();
    if (xml.getTagCount() <= 1)
        return false;

    // output opening AutoWindow tag
    xml_stream.openTag("AutoWindow");
    // extract the name suffix
    String suffix(getName(), getParent()->getName().length(), String::npos);
    // write name suffix attribute
    xml_stream.attribute("NameSuffix", suffix);
    // Inefficient : do the XML serialization again
    // write out properties.
    writePropertiesXML(xml_stream);
    // write out attached child windows.
    writeChildWindowsXML(xml_stream);
    xml_stream.closeTag();
    return true;
}

//----------------------------------------------------------------------------//
void Window::addWindowToDrawList(Window& wnd, bool at_back)
{
    // add behind other windows in same group
    if (at_back)
    {
         // calculate position where window should be added for drawing
        ChildList::iterator pos = d_drawList.begin();
        if (wnd.isAlwaysOnTop())
        {
            // find first topmost window
            while ((pos != d_drawList.end()) && (!(*pos)->isAlwaysOnTop()))
                ++pos;
        }
        // add window to draw list
        d_drawList.insert(pos, &wnd);
    }
    // add in front of other windows in group
    else
    {
        // calculate position where window should be added for drawing
        ChildList::reverse_iterator position = d_drawList.rbegin();
        if (!wnd.isAlwaysOnTop())
        {
            // find last non-topmost window
            while ((position != d_drawList.rend()) && ((*position)->isAlwaysOnTop()))
                ++position;
        }
        // add window to draw list
        d_drawList.insert(position.base(), &wnd);
    }
}

//----------------------------------------------------------------------------//
void Window::removeWindowFromDrawList(const Window& wnd)
{
    // if draw list is not empty
    if (!d_drawList.empty())
    {
        // attempt to find the window in the draw list
        const ChildList::iterator position =
            std::find(d_drawList.begin(), d_drawList.end(), &wnd);

        // remove the window if it was found in the draw list
        if (position != d_drawList.end())
            d_drawList.erase(position);
    }
}

//----------------------------------------------------------------------------//
Window* Window::getActiveSibling()
{
    // initialise with this if we are active, else 0
    Window* activeWnd = isActive() ? this : 0;

    // if active window not already known, and we have a parent window
    if (!activeWnd && d_parent)
    {
        // scan backwards through the draw list, as this will
        // usually result in the fastest result.
        size_t idx = d_parent->getChildCount();
        while (idx-- > 0)
        {
            // if this child is active
            if (d_parent->d_drawList[idx]->isActive())
            {
                // set the return value
                activeWnd = d_parent->d_drawList[idx];
                // exit loop early, as we have found what we needed
                break;
            }
        }
    }

    // return whatever we discovered
    return activeWnd;
}

//----------------------------------------------------------------------------//
void Window::rename(const String& new_name)
{
    WindowManager& winMgr = WindowManager::getSingleton();
    /*
     * Client code should never call this, but again, since we know people do
     * not read and stick to the API reference, here is some built-in protection
     * which ensures that things are handled via the WindowManager anyway.
     */
    if (winMgr.isWindowPresent(d_name))
    {
        winMgr.renameWindow(this, new_name);
        // now we return, since the work was already done when WindowManager
        // re-called this function in the proper manner.
        return;
    }

    if (winMgr.isWindowPresent(new_name))
        CEGUI_THROW(AlreadyExistsException("Window::rename - Failed to rename "
            "Window: " + d_name + " as: " + new_name + ".  A Window named:" +
            new_name + "' already exists within the system."));

    // rename Falagard created child windows
    if (!d_lookName.empty())
    {
        const WidgetLookFeel& wlf =
                WidgetLookManager::getSingleton().getWidgetLook(d_lookName);

        // get WidgetLookFeel to rename the children it created
        wlf.renameChildren(*this, new_name);
    }

    // how to detect other auto created windows.
    const String autoPrefix(d_name + AutoWidgetNameSuffix);
    // length of current name
    const size_t oldNameLength = d_name.length();

    // now rename all remaining auto-created windows attached
    for (size_t i = 0; i < getChildCount(); ++i)
    {
        // is this an auto created window that we created?
        if (!d_children[i]->d_name.compare(0, autoPrefix.length(), autoPrefix))
        {
            winMgr.renameWindow(d_children[i],
                                new_name +
                                d_children[i]->d_name.substr(oldNameLength));
        }
    }

    // log this under informative level
    Logger::getSingleton().logEvent("Renamed window: " + d_name +
                                    " as: " + new_name,
                                    Informative);

    // finally, set our new name
    d_name = new_name;
}

//----------------------------------------------------------------------------//
void Window::onSized(WindowEventArgs& e)
{
    // resize the underlying RenderingWindow if we're using such a thing
    if (d_surface && d_surface->isRenderingWindow())
        static_cast<RenderingWindow*>(d_surface)->setSize(getPixelSize());

    // screen area changes when we're resized.
    // NB: Called non-recursive since the onParentSized notifications will deal
    // more selectively with child Window cases.
    notifyScreenAreaChanged(false);

    // we need to layout loonfeel based content first, in case anything is
    // relying on that content for size or positioning info (i.e. some child
    // is used to establish inner-rect position or size).
    //
    // TODO: The subsequent onParentSized notification for those windows cause
    // additional - unneccessary - work; we should look to optimise that.
    performChildWindowLayout();

    // inform children their parent has been re-sized
    const size_t child_count = getChildCount();
    for (size_t i = 0; i < child_count; ++i)
    {
        WindowEventArgs args(this);
        d_children[i]->onParentSized(args);
    }

    invalidate();

    fireEvent(EventSized, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onMoved(WindowEventArgs& e)
{
    notifyScreenAreaChanged();

    // handle invalidation of surfaces and trigger needed redraws
    if (d_parent)
    {
        d_parent->invalidateRenderingSurface();
        // need to redraw some geometry if parent uses a caching surface
        if (d_parent->getTargetRenderingSurface().isRenderingWindow())
            System::getSingleton().signalRedraw();
    }

    fireEvent(EventMoved, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onTextChanged(WindowEventArgs& e)
{
    invalidate();
    fireEvent(EventTextChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onFontChanged(WindowEventArgs& e)
{
    // This was added to enable the Falagard FontDim to work
    // properly.  A better, more selective, solution would
    // probably be to do something funky with events ;)
    performChildWindowLayout();

    invalidate();
    fireEvent(EventFontChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onAlphaChanged(WindowEventArgs& e)
{
    // scan child list and call this method for all children that inherit alpha
    const size_t child_count = getChildCount();

    for (size_t i = 0; i < child_count; ++i)
    {
        if (d_children[i]->inheritsAlpha())
        {
            WindowEventArgs args(d_children[i]);
            d_children[i]->onAlphaChanged(args);
        }

    }

    invalidate();
    fireEvent(EventAlphaChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onIDChanged(WindowEventArgs& e)
{
    fireEvent(EventIDChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onShown(WindowEventArgs& e)
{
    invalidate();
    fireEvent(EventShown, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onHidden(WindowEventArgs& e)
{
    // first deactivate window if it is the active window.
    if (isActive())
        deactivate();

    invalidate();
    fireEvent(EventHidden, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onEnabled(WindowEventArgs& e)
{
    // signal all non-disabled children that they are now enabled
    // (via inherited state)
    const size_t child_count = getChildCount();
    for (size_t i = 0; i < child_count; ++i)
    {
        if (d_children[i]->d_enabled)
        {
            WindowEventArgs args(d_children[i]);
            d_children[i]->onEnabled(args);
        }
    }

    invalidate();
    fireEvent(EventEnabled, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onDisabled(WindowEventArgs& e)
{
    // signal all non-disabled children that they are now disabled
    // (via inherited state)
    const size_t child_count = getChildCount();
    for (size_t i = 0; i < child_count; ++i)
    {
        if (d_children[i]->d_enabled)
        {
            WindowEventArgs args(d_children[i]);
            d_children[i]->onDisabled(args);
        }
    }

    invalidate();
    fireEvent(EventDisabled, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onClippingChanged(WindowEventArgs& e)
{
    invalidate();
    notifyClippingChanged();
    fireEvent(EventClippedByParentChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onParentDestroyChanged(WindowEventArgs& e)
{
    fireEvent(EventDestroyedByParentChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onInheritsAlphaChanged(WindowEventArgs& e)
{
    invalidate();
    fireEvent(EventInheritsAlphaChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onAlwaysOnTopChanged(WindowEventArgs& e)
{
    // we no longer want a total redraw here, instead we just get each window
    // to resubmit it's imagery to the Renderer.
    System::getSingleton().signalRedraw();
    fireEvent(EventAlwaysOnTopChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onCaptureGained(WindowEventArgs& e)
{
    fireEvent(EventInputCaptureGained, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onCaptureLost(WindowEventArgs& e)
{
    // reset auto-repeat state
    d_repeatButton = NoButton;

    // handle restore of previous capture window as required.
    if (d_restoreOldCapture && (d_oldCapture != 0)) {
        d_oldCapture->onCaptureLost(e);
        d_oldCapture = 0;
    }

    // handle case where mouse is now in a different window
    // (this is a bit of a hack that uses the mouse input injector to handle
    // this for us).
    System::getSingleton().injectMouseMove(0, 0);

    fireEvent(EventInputCaptureLost, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onRenderingStarted(WindowEventArgs& e)
{
    fireEvent(EventRenderingStarted, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onRenderingEnded(WindowEventArgs& e)
{
    fireEvent(EventRenderingEnded, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onZChanged(WindowEventArgs& e)
{
    // we no longer want a total redraw here, instead we just get each window
    // to resubmit it's imagery to the Renderer.
    System::getSingleton().signalRedraw();
    fireEvent(EventZOrderChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onDestructionStarted(WindowEventArgs& e)
{
    d_destructionStarted = true;
    fireEvent(EventDestructionStarted, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onActivated(ActivationEventArgs& e)
{
    d_active = true;
    invalidate();
    fireEvent(EventActivated, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onDeactivated(ActivationEventArgs& e)
{
    // first de-activate all children
    const size_t child_count = getChildCount();
    for (size_t i = 0; i < child_count; ++i)
    {
        if (d_children[i]->isActive())
        {
            // make sure the child gets itself as the .window member
            ActivationEventArgs child_e(d_children[i]);
            child_e.otherWindow = e.otherWindow;
            d_children[i]->onDeactivated(child_e);
        }

    }

    d_active = false;
    invalidate();
    fireEvent(EventDeactivated, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onParentSized(WindowEventArgs& e)
{
    // set window area back on itself to cause minimum and maximum size
    // constraints to be applied as required.  (fire no events though)

    // todo: vector vs size
    const USize sz = d_area.getSize();
    setArea_impl(d_area.getPosition(), UVector2(sz.d_width, sz.d_height), false, false);

    const bool moved =
        ((d_area.d_min.d_x.d_scale != 0) || (d_area.d_min.d_y.d_scale != 0) ||
         (d_horzAlign != HA_LEFT) || (d_vertAlign != VA_TOP));
    const bool sized =
        ((d_area.d_max.d_x.d_scale != 0) || (d_area.d_max.d_y.d_scale != 0) ||
         isInnerRectSizeChanged());

    // now see if events should be fired.
    if (moved)
    {
        WindowEventArgs args(this);
        onMoved(args);
    }

    if (sized)
    {
        WindowEventArgs args(this);
        onSized(args);
    }

    // if we were not moved or sized, do child layout anyway!
    if (!(moved || sized))
        performChildWindowLayout();

    fireEvent(EventParentSized, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onChildAdded(WindowEventArgs& e)
{
    // we no longer want a total redraw here, instead we just get each window
    // to resubmit it's imagery to the Renderer.
    System::getSingleton().signalRedraw();
    fireEvent(EventChildAdded, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onChildRemoved(WindowEventArgs& e)
{
    // we no longer want a total redraw here, instead we just get each window
    // to resubmit it's imagery to the Renderer.
    System::getSingleton().signalRedraw();
    // Though we do need to invalidate the rendering surface!
    getTargetRenderingSurface().invalidate();
    fireEvent(EventChildRemoved, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onMouseEntersArea(MouseEventArgs& e)
{
    fireEvent(EventMouseEntersArea, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onMouseLeavesArea(MouseEventArgs& e)
{
    fireEvent(EventMouseLeavesArea, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onMouseEnters(MouseEventArgs& e)
{
    // set the mouse cursor
    MouseCursor::getSingleton().setImage(getMouseCursor());

    // perform tooltip control
    Tooltip* const tip = getTooltip();
    if (tip && !isAncestor(tip))
        tip->setTargetWindow(this);

    fireEvent(EventMouseEntersSurface, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onMouseLeaves(MouseEventArgs& e)
{
    // perform tooltip control
    const Window* const mw = System::getSingleton().getWindowContainingMouse();
    Tooltip* const tip = getTooltip();
    if (tip && mw != tip && !(mw && mw->isAncestor(tip)))
        tip->setTargetWindow(0);

    fireEvent(EventMouseLeavesSurface, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onMouseMove(MouseEventArgs& e)
{
    // perform tooltip control
    Tooltip* const tip = getTooltip();
    if (tip)
        tip->resetTimer();

    fireEvent(EventMouseMove, e, EventNamespace);

    // optionally propagate to parent
    if (!e.handled && d_propagateMouseInputs &&
        d_parent && this != System::getSingleton().getModalTarget())
    {
        e.window = d_parent;
        d_parent->onMouseMove(e);

        return;
    }

    // by default we now mark mouse events as handled
    // (derived classes may override, of course!)
    ++e.handled;
}

//----------------------------------------------------------------------------//
void Window::onMouseWheel(MouseEventArgs& e)
{
    fireEvent(EventMouseWheel, e, EventNamespace);

    // optionally propagate to parent
    if (!e.handled && d_propagateMouseInputs &&
        d_parent && this != System::getSingleton().getModalTarget())
    {
        e.window = d_parent;
        d_parent->onMouseWheel(e);

        return;
    }

    // by default we now mark mouse events as handled
    // (derived classes may override, of course!)
    ++e.handled;
}

//----------------------------------------------------------------------------//
void Window::onMouseButtonDown(MouseEventArgs& e)
{
    // perform tooltip control
    Tooltip* const tip = getTooltip();
    if (tip)
        tip->setTargetWindow(0);

    if ((e.button == LeftButton) && moveToFront_impl(true))
        ++e.handled;

    // if auto repeat is enabled and we are not currently tracking
    // the button that was just pushed (need this button check because
    // it could be us that generated this event via auto-repeat).
    if (d_autoRepeat)
    {
        if (d_repeatButton == NoButton)
            captureInput();

        if ((d_repeatButton != e.button) && isCapturedByThis())
        {
            d_repeatButton = e.button;
            d_repeatElapsed = 0;
            d_repeating = false;
        }
    }

    fireEvent(EventMouseButtonDown, e, EventNamespace);

    // optionally propagate to parent
    if (!e.handled && d_propagateMouseInputs &&
        d_parent && this != System::getSingleton().getModalTarget())
    {
        e.window = d_parent;
        d_parent->onMouseButtonDown(e);

        return;
    }

    // by default we now mark mouse events as handled
    // (derived classes may override, of course!)
    ++e.handled;
}

//----------------------------------------------------------------------------//
void Window::onMouseButtonUp(MouseEventArgs& e)
{
    // reset auto-repeat state
    if (d_autoRepeat && d_repeatButton != NoButton)
    {
        releaseInput();
        d_repeatButton = NoButton;
    }

    fireEvent(EventMouseButtonUp, e, EventNamespace);

    // optionally propagate to parent
    if (!e.handled && d_propagateMouseInputs &&
        d_parent && this != System::getSingleton().getModalTarget())
    {
        e.window = d_parent;
        d_parent->onMouseButtonUp(e);

        return;
    }

    // by default we now mark mouse events as handled
    // (derived classes may override, of course!)
    ++e.handled;
}

//----------------------------------------------------------------------------//
void Window::onMouseClicked(MouseEventArgs& e)
{
    fireEvent(EventMouseClick, e, EventNamespace);

    // optionally propagate to parent
    if (!e.handled && d_propagateMouseInputs &&
        d_parent && this != System::getSingleton().getModalTarget())
    {
        e.window = d_parent;
        d_parent->onMouseClicked(e);

        return;
    }

    // if event was directly injected, mark as handled to be consistent with
    // other mouse button injectors
    if (!System::getSingleton().isMouseClickEventGenerationEnabled())
        ++e.handled;
}

//----------------------------------------------------------------------------//
void Window::onMouseDoubleClicked(MouseEventArgs& e)
{
    fireEvent(EventMouseDoubleClick, e, EventNamespace);

    // optionally propagate to parent
    if (!e.handled && d_propagateMouseInputs &&
        d_parent && this != System::getSingleton().getModalTarget())
    {
        e.window = d_parent;
        d_parent->onMouseDoubleClicked(e);

        return;
    }

    ++e.handled;
}

//----------------------------------------------------------------------------//
void Window::onMouseTripleClicked(MouseEventArgs& e)
{
    fireEvent(EventMouseTripleClick, e, EventNamespace);

    // optionally propagate to parent
    if (!e.handled && d_propagateMouseInputs &&
        d_parent && this != System::getSingleton().getModalTarget())
    {
        e.window = d_parent;
        d_parent->onMouseTripleClicked(e);

        return;
    }

    ++e.handled;
}

//----------------------------------------------------------------------------//
void Window::onKeyDown(KeyEventArgs& e)
{
    fireEvent(EventKeyDown, e, EventNamespace);

    // As of 0.7.0 CEGUI::System no longer does input event propogation, so by
    // default we now do that here.  Generally speaking key handling widgets
    // may need to override this behaviour to halt further propogation.
    if (!e.handled && d_parent &&
        this != System::getSingleton().getModalTarget())
    {
        e.window = d_parent;
        d_parent->onKeyDown(e);
    }
}

//----------------------------------------------------------------------------//
void Window::onKeyUp(KeyEventArgs& e)
{
    fireEvent(EventKeyUp, e, EventNamespace);

    // As of 0.7.0 CEGUI::System no longer does input event propogation, so by
    // default we now do that here.  Generally speaking key handling widgets
    // may need to override this behaviour to halt further propogation.
    if (!e.handled && d_parent &&
        this != System::getSingleton().getModalTarget())
    {
        e.window = d_parent;
        d_parent->onKeyUp(e);
    }
}

//----------------------------------------------------------------------------//
void Window::onCharacter(KeyEventArgs& e)
{
    fireEvent(EventCharacterKey, e, EventNamespace);

    // As of 0.7.0 CEGUI::System no longer does input event propogation, so by
    // default we now do that here.  Generally speaking key handling widgets
    // may need to override this behaviour to halt further propogation.
    if (!e.handled && d_parent &&
        this != System::getSingleton().getModalTarget())
    {
        e.window = d_parent;
        d_parent->onCharacter(e);
    }
}

//----------------------------------------------------------------------------//
void Window::onDragDropItemEnters(DragDropEventArgs& e)
{
    fireEvent(EventDragDropItemEnters, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onDragDropItemLeaves(DragDropEventArgs& e)
{
    fireEvent(EventDragDropItemLeaves, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onDragDropItemDropped(DragDropEventArgs& e)
{
    fireEvent(EventDragDropItemDropped, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onVerticalAlignmentChanged(WindowEventArgs& e)
{
    notifyScreenAreaChanged();

    fireEvent(EventVerticalAlignmentChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onHorizontalAlignmentChanged(WindowEventArgs& e)
{
    notifyScreenAreaChanged();

    fireEvent(EventHorizontalAlignmentChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::setWindowRenderer(const String& name)
{
    WindowRendererManager& wrm = WindowRendererManager::getSingleton();
    if (d_windowRenderer != 0)
    {
        // Allow reset of renderer
        if (d_windowRenderer->getName() == name)
            return;

        WindowEventArgs e(this);
        onWindowRendererDetached(e);
        wrm.destroyWindowRenderer(d_windowRenderer);
    }

    if (!name.empty())
    {
        Logger::getSingleton().logEvent("Assigning the window renderer '" +
            name + "' to the window '" + d_name + "'", Informative);
        d_windowRenderer = wrm.createWindowRenderer(name);
        WindowEventArgs e(this);
        onWindowRendererAttached(e);
    }
    else
        CEGUI_THROW(InvalidRequestException(
            "Window::setWindowRenderer: Attempt to "
            "assign a 'null' window renderer to window '" + d_name + "'."));
}

//----------------------------------------------------------------------------//
WindowRenderer* Window::getWindowRenderer(void) const
{
    return d_windowRenderer;
}

//----------------------------------------------------------------------------//
void Window::onWindowRendererAttached(WindowEventArgs& e)
{
    if (!validateWindowRenderer(d_windowRenderer->getClass()))
        CEGUI_THROW(InvalidRequestException(
            "Window::onWindowRendererAttached: The "
            "window renderer '" + d_windowRenderer->getName() + "' is not "
            "compatible with this widget type (" + getType() + ")"));

    if (!testClassName(d_windowRenderer->getClass()))
        CEGUI_THROW(InvalidRequestException(
            "Window::onWindowRendererAttached: The "
            "window renderer '" + d_windowRenderer->getName() + "' is not "
            "compatible with this widget type (" + getType() + "). It requires "
            "a '" + d_windowRenderer->getClass() + "' based window type."));

    d_windowRenderer->d_window = this;
    d_windowRenderer->onAttach();
    fireEvent(EventWindowRendererAttached, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onWindowRendererDetached(WindowEventArgs& e)
{
    d_windowRenderer->onDetach();
    d_windowRenderer->d_window = 0;
    fireEvent(EventWindowRendererDetached, e, EventNamespace);
}

//----------------------------------------------------------------------------//
bool Window::validateWindowRenderer(const String&) const
{
    return true;
}

//----------------------------------------------------------------------------//
String Window::getWindowRendererName(void) const
{
    if (d_windowRenderer)
        return d_windowRenderer->getName();

    return String();
}


//----------------------------------------------------------------------------//
void Window::banPropertyFromXML(const String& property_name)
{
    // check if the insertion failed
    if (!d_bannedXMLProperties.insert(property_name).second)
        // just log the incidence
        AlreadyExistsException("Window::banPropertyFromXML: The property '" +
            property_name + "' is already banned in window '" +
            d_name + "'");
}

//----------------------------------------------------------------------------//
void Window::unbanPropertyFromXML(const String& property_name)
{
    d_bannedXMLProperties.erase(property_name);
}

//----------------------------------------------------------------------------//
bool Window::isPropertyBannedFromXML(const String& property_name) const
{
    const BannedXMLPropertySet::const_iterator i =
        d_bannedXMLProperties.find(property_name);

    return (i != d_bannedXMLProperties.end());
}

//----------------------------------------------------------------------------//
void Window::banPropertyFromXML(const Property* property)
{
    if (property)
        banPropertyFromXML(property->getName());
}

//----------------------------------------------------------------------------//
void Window::unbanPropertyFromXML(const Property* property)
{
    if (property)
        unbanPropertyFromXML(property->getName());
}

//----------------------------------------------------------------------------//
bool Window::isPropertyBannedFromXML(const Property* property) const
{
    if (property)
        return isPropertyBannedFromXML(property->getName());
    else
        return false;
}

//----------------------------------------------------------------------------//
bool Window::isPropertyAtDefault(const Property* property) const
{
    // if we have a looknfeel we examine it for defaults
    if (!d_lookName.empty())
    {
        // if we're an autowindow, we check our parent's looknfeel's Child
        // section which we came from as we might be initialised there
        if (d_autoWindow && getParent() && !getParent()->getLookNFeel().empty())
        {
            const WidgetLookFeel& wlf =
                WidgetLookManager::getSingleton().
                    getWidgetLook(getParent()->getLookNFeel());

            // find our name suffix
            const String suffix(getName(),
                                getParent()->getName().length(),
                                String::npos);

            // find the widget component if possible
            const WidgetComponent* const wc = wlf.findWidgetComponent(suffix);
            if (wc)
            {
                const PropertyInitialiser* const propinit =
                    wc->findPropertyInitialiser(property->getName());

                if (propinit)
                    return (getProperty(property->getName()) ==
                            propinit->getInitialiserValue());
            }
        }

        // if the looknfeel has a new default for this property we compare
        // against that
        const WidgetLookFeel& wlf =
            WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
        const PropertyInitialiser* const propinit =
            wlf.findPropertyInitialiser(property->getName());
        if (propinit)
            return (getProperty(property->getName()) ==
                    propinit->getInitialiserValue());
    }

    // we dont have a looknfeel with a new value for this property so we rely
    // on the hardcoded default
    return property->isDefault(this);
}

//----------------------------------------------------------------------------//
void Window::notifyClippingChanged(void)
{
    d_outerRectClipperValid = false;
    d_innerRectClipperValid = false;
    d_hitTestRectValid = false;

    // inform children that their clipped screen areas must be updated
    const size_t num = d_children.size();
    for (size_t i=0; i<num; ++i)
        if (d_children[i]->isClippedByParent())
            d_children[i]->notifyClippingChanged();
}

//----------------------------------------------------------------------------//
void Window::notifyScreenAreaChanged(bool recursive /* = true */)
{
    d_outerUnclippedRectValid = false;
    d_innerUnclippedRectValid = false;
    d_outerRectClipperValid = false;
    d_innerRectClipperValid = false;
    d_hitTestRectValid = false;

    updateGeometryRenderSettings();

    // inform children that their screen area must be updated
    if (recursive)
    {
        const size_t child_count = getChildCount();
        for (size_t i = 0; i < child_count; ++i)
            d_children[i]->notifyScreenAreaChanged();
    }
}

//----------------------------------------------------------------------------//
void Window::updateGeometryRenderSettings()
{
    RenderingContext ctx;
    getRenderingContext(ctx);

    // move the underlying RenderingWindow if we're using such a thing
    if (ctx.owner == this && ctx.surface->isRenderingWindow())
    {
        static_cast<RenderingWindow*>(ctx.surface)->
            setPosition(getUnclippedOuterRect().getPosition());
        static_cast<RenderingWindow*>(d_surface)->setPivot(
            Vector3f(d_pixelSize.d_width / 2.0f,
                    d_pixelSize.d_height / 2.0f,
                    0.0f));
        d_geometry->setTranslation(Vector3f(0.0f, 0.0f, 0.0f));
    }
    // if we're not texture backed, update geometry position.
    else
    {
        // position is the offset of the window on the dest surface.
        const Rectf ucrect(getUnclippedOuterRect());
        d_geometry->setTranslation(Vector3f(ucrect.d_min.d_x - ctx.offset.d_x,
                                             ucrect.d_min.d_y - ctx.offset.d_y, 0.0f));
    }
    initialiseClippers(ctx);
}

//----------------------------------------------------------------------------//
EventSet::Iterator Window::getEventIterator() const
{
    return EventSet::getIterator();
}

//----------------------------------------------------------------------------//
PropertySet::Iterator Window::getPropertyIterator() const
{
    return PropertySet::getIterator();
}

//----------------------------------------------------------------------------//
bool Window::isDragDropTarget() const
{
    return d_dragDropTarget;
}

//----------------------------------------------------------------------------//
void Window::setDragDropTarget(bool setting)
{
    d_dragDropTarget = setting;
}

//-----------------------------------------------------------------------
void Window::setFalagardType(const String& type, const String& rendererType)
{
    // Retrieve the new widget look
    const String separator("/");
    String::size_type pos = type.find(separator);
    const String newLook(type, 0, pos);

    // Check if old one is the same. If so, ignore since we don't need to do
    // anything (type is already assigned)
    pos = d_falagardType.find(separator);
    String oldLook(d_falagardType, 0, pos);
    if(oldLook == newLook)
        return;

    // Obtain widget kind
    String widget(d_falagardType, pos + 1);

    // Build new type (look/widget)
    d_falagardType = newLook + separator + widget;

    // Set new renderer
    if(rendererType.length() > 0)
        setWindowRenderer(rendererType);

    // Apply the new look to the widget
    setLookNFeel(type);
}

//----------------------------------------------------------------------------//
bool Window::isTopOfZOrder() const
{
    // if not attached, then always on top!
    if (!d_parent)
        return true;

    // get position of window at top of z-order in same group as this window
    ChildList::reverse_iterator pos = d_parent->d_drawList.rbegin();
    if (!d_alwaysOnTop)
    {
        // find last non-topmost window
        while ((pos != d_parent->d_drawList.rend()) && (*pos)->isAlwaysOnTop())
            ++pos;
    }

    // return whether the window at the top of the z order is us
    return *pos == this;
}

//----------------------------------------------------------------------------//
void Window::insertText(const String& text, const String::size_type position)
{
    d_textLogical.insert(position, text);
    d_renderedStringValid = false;
    d_bidiDataValid = false;

    WindowEventArgs args(this);
    onTextChanged(args);
}

//----------------------------------------------------------------------------//
void Window::appendText(const String& text)
{
    d_textLogical.append(text);
    d_renderedStringValid = false;
    d_bidiDataValid = false;

    WindowEventArgs args(this);
    onTextChanged(args);
}

//----------------------------------------------------------------------------//
GeometryBuffer& Window::getGeometryBuffer()
{
    return *d_geometry;
}

//----------------------------------------------------------------------------//
void Window::getRenderingContext(RenderingContext& ctx) const
{
    if (d_windowRenderer)
        d_windowRenderer->getRenderingContext(ctx);
    else
        getRenderingContext_impl(ctx);
}

//----------------------------------------------------------------------------//
void Window::getRenderingContext_impl(RenderingContext& ctx) const
{
    if (d_surface)
    {
        ctx.surface = d_surface;
        ctx.owner = this;
        ctx.offset = getUnclippedOuterRect().getPosition();
        ctx.queue = RQ_BASE;
    }
    else if (d_parent)
    {
        d_parent->getRenderingContext(ctx);
    }
    else
    {
        ctx.surface =
            &System::getSingleton().getRenderer()->getDefaultRenderingRoot();
        ctx.owner = 0;
        ctx.offset = Vector2f(0, 0);
        ctx.queue = RQ_BASE;
    }
}

//----------------------------------------------------------------------------//
RenderingSurface* Window::getRenderingSurface() const
{
    return d_surface;
}

//----------------------------------------------------------------------------//
RenderingSurface& Window::getTargetRenderingSurface() const
{
    if (d_surface)
        return *d_surface;
    else if (d_parent)
        return d_parent->getTargetRenderingSurface();
    else
        return System::getSingleton().getRenderer()->getDefaultRenderingRoot();
}

//----------------------------------------------------------------------------//
void Window::setRenderingSurface(RenderingSurface* surface)
{
    if (d_surface == surface)
        return;

    if (d_autoRenderingWindow)
        setUsingAutoRenderingSurface(false);

    d_surface = surface;

    // transfer child surfaces to this new surface
    if (d_surface)
    {
        transferChildSurfaces();
        notifyScreenAreaChanged();
    }
}

//----------------------------------------------------------------------------//
void Window::invalidateRenderingSurface()
{
    // invalidate our surface chain if we have one
    if (d_surface)
        d_surface->invalidate();
    // else look through the hierarchy for a surface chain to invalidate.
    else if (d_parent)
        d_parent->invalidateRenderingSurface();
}

//----------------------------------------------------------------------------//
const Window* Window::getRootWindow() const
{
    return d_parent ? d_parent->getRootWindow() : this;
}

//----------------------------------------------------------------------------//
Window* Window::getRootWindow()
{
    return const_cast<Window*>(
        static_cast<const Window*>(this)->getRootWindow());
}

//----------------------------------------------------------------------------//
bool Window::isUsingAutoRenderingSurface() const
{
    return d_autoRenderingWindow;
}

//----------------------------------------------------------------------------//
void Window::setUsingAutoRenderingSurface(bool setting)
{
    if (setting)
        allocateRenderingWindow();
    else
        releaseRenderingWindow();

    // while the actal area on screen may not have changed, the arrangement of
    // surfaces and geometry did...
    notifyScreenAreaChanged();
}

void Window::setRotation(const Quaternion& rotation)
{
    d_rotation = rotation;

    WindowEventArgs args(this);
    onRotated(args);
}

const Quaternion& Window::getRotation() const
{
    return d_rotation;
}

//----------------------------------------------------------------------------//
void Window::allocateRenderingWindow()
{
    if (!d_autoRenderingWindow)
    {
        d_autoRenderingWindow = true;

        TextureTarget* const t =
            System::getSingleton().getRenderer()->createTextureTarget();

        // TextureTargets may not be available, so check that first.
        if (!t)
        {
            Logger::getSingleton().logEvent("Window::allocateRenderingWindow - "
                "Failed to create a suitable TextureTarget for use by Window '"
                + d_name + "'", Errors);

            d_surface = 0;
            return;
        }

        d_surface = &getTargetRenderingSurface().createRenderingWindow(*t);
        transferChildSurfaces();

        // set size and position of RenderingWindow
        static_cast<RenderingWindow*>(d_surface)->setSize(getPixelSize());
        static_cast<RenderingWindow*>(d_surface)->
            setPosition(getUnclippedOuterRect().getPosition());

        System::getSingleton().signalRedraw();
    }
}

//----------------------------------------------------------------------------//
void Window::releaseRenderingWindow()
{
    if (d_autoRenderingWindow && d_surface)
    {
        RenderingWindow* const old_surface =
            static_cast<RenderingWindow*>(d_surface);
        d_autoRenderingWindow = false;
        d_surface = 0;
        // detach child surfaces prior to destroying the owning surface
        transferChildSurfaces();
        // destroy surface and texture target it used
        TextureTarget* tt = &old_surface->getTextureTarget();
        old_surface->getOwner().destroyRenderingWindow(*old_surface);
        System::getSingleton().getRenderer()->destroyTextureTarget(tt);

        System::getSingleton().signalRedraw();
    }
}

//----------------------------------------------------------------------------//
void Window::transferChildSurfaces()
{
    RenderingSurface& s = getTargetRenderingSurface();

    const size_t child_count = getChildCount();
    for (size_t i = 0; i < child_count; ++i)
    {
        Window* const c = d_children[i];

        if (c->d_surface && c->d_surface->isRenderingWindow())
            s.transferRenderingWindow(
                *static_cast<RenderingWindow*>(c->d_surface));
        else
            c->transferChildSurfaces();
    }
}

//----------------------------------------------------------------------------//
void Window::initialiseClippers(const RenderingContext& ctx)
{
    if (ctx.surface->isRenderingWindow() && ctx.owner == this)
    {
        RenderingWindow* const rendering_window =
            static_cast<RenderingWindow*>(ctx.surface);

        if (d_clippedByParent && d_parent)
            rendering_window->setClippingRegion(
                d_parent->getInnerRectClipper());
        else
            rendering_window->setClippingRegion(
                Rectf(Vector2f(0, 0),
                       System::getSingleton().getRenderer()->getDisplaySize()));

        d_geometry->setClippingRegion(Rectf(Vector2f(0, 0), d_pixelSize));
    }
    else
    {
        Rectf geo_clip(getOuterRectClipper());

        if (geo_clip.getWidth() != 0.0f && geo_clip.getHeight() != 0.0f)
            geo_clip.offset(Vector2f(-ctx.offset.d_x, -ctx.offset.d_y));

        d_geometry->setClippingRegion(geo_clip);
    }
}

//----------------------------------------------------------------------------//
bool Window::isNonClientWindow() const
{
    return d_nonClientContent;
}

//----------------------------------------------------------------------------//
void Window::setNonClientWindow(const bool setting)
{
    if (setting == d_nonClientContent)
        return;

    d_nonClientContent = setting;

    WindowEventArgs args(this);
    onNonClientChanged(args);
}

//----------------------------------------------------------------------------//
void Window::onRotated(WindowEventArgs& e)
{
    // if we have no surface set, enable the auto surface
    if (!d_surface)
    {
        Logger::getSingleton().logEvent("Window::setRotation - "
            "Activating AutoRenderingSurface on Window '" + d_name +
            "' to enable rotation support.");

        setUsingAutoRenderingSurface(true);

        // still no surface?  Renderer or HW must not support what we need :(
        if (!d_surface)
        {
            Logger::getSingleton().logEvent("Window::setRotation - "
                "Failed to obtain a suitable ReneringWindow surface for "
                "Window '" + d_name + "'.  Rotation will not be available.",
                Errors);

            return;
        }
    }

    // ensure surface we have is the right type
    if (!d_surface->isRenderingWindow())
    {
        Logger::getSingleton().logEvent("Window::setRotation - "
            "Window '" + d_name + "' has a manual RenderingSurface that is not "
            "a RenderingWindow.  Rotation will not be available.", Errors);

        return;
    }

    // Checks / setup complete!  Now we can finally set the rotation.
    static_cast<RenderingWindow*>(d_surface)->setRotation(d_rotation);
    static_cast<RenderingWindow*>(d_surface)->setPivot(
        Vector3f(d_pixelSize.d_width / 2.0f, d_pixelSize.d_height / 2.0f, 0.0f));

    fireEvent(EventRotated, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onNonClientChanged(WindowEventArgs& e)
{
    // TODO: Trigger update of size and position information if needed

    fireEvent(EventNonClientChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
const RenderedString& Window::getRenderedString() const
{
    if (!d_renderedStringValid)
    {
        d_renderedString = getRenderedStringParser().parse(
            getTextVisual(), getFont(), 0);
        d_renderedStringValid = true;
    }

    return d_renderedString;
}

//----------------------------------------------------------------------------//
RenderedStringParser* Window::getCustomRenderedStringParser() const
{
    return d_customStringParser;
}

//----------------------------------------------------------------------------//
void Window::setCustomRenderedStringParser(RenderedStringParser* parser)
{
    d_customStringParser = parser;
    d_renderedStringValid = false;
}

//----------------------------------------------------------------------------//
RenderedStringParser& Window::getRenderedStringParser() const
{
    // if parsing is disabled, we use a DefaultRenderedStringParser that creates
    // a RenderedString to renderi the input text verbatim (i.e. no parsing).
    if (!d_textParsingEnabled)
        return d_defaultStringParser;

    // Next prefer a custom RenderedStringParser assigned to this Window.
    if (d_customStringParser)
        return *d_customStringParser;

    // Next prefer any globally set RenderedStringParser.
    RenderedStringParser* const global_parser =
        CEGUI::System::getSingleton().getDefaultCustomRenderedStringParser();
    if (global_parser)
        return *global_parser;

    // if parsing is enabled and no custom RenderedStringParser is set anywhere,
    // use the system's BasicRenderedStringParser to do the parsing.
    return d_basicStringParser;
}

//----------------------------------------------------------------------------//
Vector2f Window::getUnprojectedPosition(const Vector2f& pos) const
{
    RenderingSurface* rs = &getTargetRenderingSurface();

    // if window is not backed by RenderingWindow, return same pos.
    if (!rs->isRenderingWindow())
        return pos;

    // get first target RenderingWindow
    RenderingWindow* rw = static_cast<RenderingWindow*>(rs);

    // setup for loop
    Vector2f out_pos(pos);

    // while there are rendering windows
    while (rw)
    {
        // unproject the point for the current rw
        const Vector2f in_pos(out_pos);
        rw->unprojectPoint(in_pos, out_pos);

        // get next rendering window, if any
        rw = (rs = &rw->getOwner())->isRenderingWindow() ?
                static_cast<RenderingWindow*>(rs) : 0;
    }

    return out_pos;
}

//----------------------------------------------------------------------------//
const String& Window::getTextVisual() const
{
    // no bidi support
    if (!d_bidiVisualMapping)
        return d_textLogical;

    if (!d_bidiDataValid)
    {
        d_bidiVisualMapping->updateVisual(d_textLogical);
        d_bidiDataValid = true;
    }

    return d_bidiVisualMapping->getTextVisual();
}

//----------------------------------------------------------------------------//
bool Window::isTextParsingEnabled() const
{
    return d_textParsingEnabled;
}

//----------------------------------------------------------------------------//
void Window::setTextParsingEnabled(const bool setting)
{
    d_textParsingEnabled = setting;
    d_renderedStringValid = false;

    WindowEventArgs args(this);
    onTextParsingChanged(args);
}

//----------------------------------------------------------------------------//
void Window::setMargin(const UBox& margin)
{
    d_margin = margin;

    WindowEventArgs args(this);
    onMarginChanged(args);
}

//----------------------------------------------------------------------------//
const UBox& Window::getMargin() const
{
    return d_margin;
}

//----------------------------------------------------------------------------//
void Window::onTextParsingChanged(WindowEventArgs& e)
{
    fireEvent(EventTextParsingChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onMarginChanged(WindowEventArgs& e)
{
    fireEvent(EventMarginChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
bool Window::isInnerRectSizeChanged() const
{
    const Sizef old_sz(d_innerUnclippedRect.getSize());
    d_innerUnclippedRectValid = false;
    return old_sz != getUnclippedInnerRect().getSize();
}

//----------------------------------------------------------------------------//
void Window::moveInFront(const Window* const window)
{
    if (!window || !window->d_parent || window->d_parent != d_parent ||
        window == this || window->d_alwaysOnTop != d_alwaysOnTop ||
        !d_zOrderingEnabled)
            return;

    // find our position in the parent child draw list
    const ChildList::iterator p(std::find(d_parent->d_drawList.begin(),
                                          d_parent->d_drawList.end(),
                                          this));
    // sanity checK that we were attached to our parent.
    assert(p != d_parent->d_drawList.end());

    // erase us from our current position
    d_parent->d_drawList.erase(p);

    // find window we're to be moved in front of in parent's draw list
    ChildList::iterator i(std::find(d_parent->d_drawList.begin(),
                                    d_parent->d_drawList.end(),
                                    window));
    // sanity check that target window was also attached to correct parent.
    assert(i != d_parent->d_drawList.end());

    // reinsert ourselves at the right location
    d_parent->d_drawList.insert(++i, this);

    // handle event notifications for affected windows.
    onZChange_impl();
}

//----------------------------------------------------------------------------//
void Window::moveBehind(const Window* const window)
{
    if (!window || !window->d_parent || window->d_parent != d_parent ||
        window == this || window->d_alwaysOnTop != d_alwaysOnTop ||
        !d_zOrderingEnabled)
            return;

    // find our position in the parent child draw list
    const ChildList::iterator p(std::find(d_parent->d_drawList.begin(),
                                          d_parent->d_drawList.end(),
                                          this));
    // sanity checK that we were attached to our parent.
    assert(p != d_parent->d_drawList.end());

    // erase us from our current position
    d_parent->d_drawList.erase(p);

    // find window we're to be moved in front of in parent's draw list
    const ChildList::iterator i(std::find(d_parent->d_drawList.begin(),
                                          d_parent->d_drawList.end(),
                                          window));
    // sanity check that target window was also attached to correct parent.
    assert(i != d_parent->d_drawList.end());

    // reinsert ourselves at the right location
    d_parent->d_drawList.insert(i, this);

    // handle event notifications for affected windows.
    onZChange_impl();
}

//----------------------------------------------------------------------------//
void Window::setUpdateMode(const WindowUpdateMode mode)
{
    d_updateMode = mode;
}

//----------------------------------------------------------------------------//
WindowUpdateMode Window::getUpdateMode() const
{
    return d_updateMode;
}

//----------------------------------------------------------------------------//
bool Window::constrainUVector2ToMinSize(const Sizef& base_sz, UVector2& sz)
{
    const Vector2f pixel_sz(CoordConverter::asAbsolute(sz, base_sz));
    const Vector2f min_sz(CoordConverter::asAbsolute(d_minSize,
        System::getSingleton().getRenderer()->getDisplaySize()));

    bool size_changed = false;

    // check width is not less than the minimum
    if (pixel_sz.d_x < min_sz.d_x)
    {
        sz.d_x.d_offset = ceguimin(sz.d_x.d_offset, d_minSize.d_x.d_offset);

        sz.d_x.d_scale = (base_sz.d_width != 0.0f) ?
            (min_sz.d_x - sz.d_x.d_offset) / base_sz.d_width :
            0.0f;

        size_changed = true;
    }

    // check height is not less than the minimum
    if (pixel_sz.d_y < min_sz.d_y)
    {
        sz.d_y.d_offset = ceguimin(sz.d_y.d_offset, d_minSize.d_y.d_offset);

        sz.d_y.d_scale = (base_sz.d_height != 0.0f) ?
            (min_sz.d_y - sz.d_y.d_offset) / base_sz.d_height :
            0.0f;

        size_changed = true;
    }

    return size_changed;
}

//----------------------------------------------------------------------------//
bool Window::constrainUVector2ToMaxSize(const Sizef& base_sz, UVector2& sz)
{
    const Vector2f pixel_sz(CoordConverter::asAbsolute(sz, base_sz));
    const Vector2f max_sz(CoordConverter::asAbsolute(d_maxSize,
        System::getSingleton().getRenderer()->getDisplaySize()));

    bool size_changed = false;

    // check width is not greater than the maximum
    if (pixel_sz.d_x > max_sz.d_x)
    {
        sz.d_x.d_offset = ceguimax(sz.d_x.d_offset, d_maxSize.d_x.d_offset);

        sz.d_x.d_scale = (base_sz.d_width != 0.0f) ?
            (max_sz.d_x - sz.d_x.d_offset) / base_sz.d_width :
            0.0f;

        size_changed = true;
    }

    // check height is not greater than the maximum
    if (pixel_sz.d_y > max_sz.d_y)
    {
        sz.d_y.d_offset = ceguimax(sz.d_y.d_offset, d_maxSize.d_y.d_offset);

        sz.d_y.d_scale = (base_sz.d_height != 0.0f) ?
            (max_sz.d_y - sz.d_y.d_offset) / base_sz.d_height :
            0.0f;

        size_changed = true;
    }

    return size_changed;
}

//----------------------------------------------------------------------------//
void Window::setMouseInputPropagationEnabled(const bool enabled)
{
    d_propagateMouseInputs = enabled;
}

//----------------------------------------------------------------------------//
bool Window::isMouseInputPropagationEnabled() const
{
    return d_propagateMouseInputs;
}

//----------------------------------------------------------------------------//
Window* Window::clone(const String& newName, const bool deepCopy) const
{
    Window* ret =
        WindowManager::getSingleton().createWindow(getType(), newName);

    // always copy properties
    clonePropertiesTo(*ret);

    // if user requested deep copy, we should copy children as well
    if (deepCopy)
        cloneChildWidgetsTo(*ret);

    return ret;
}

//----------------------------------------------------------------------------//
void Window::clonePropertiesTo(Window& target) const
{
    PropertySet::Iterator propertyIt = getPropertyIterator();

    for (PropertySet::Iterator propertyIt = getPropertyIterator();
         !propertyIt.isAtEnd();
         ++propertyIt) 
    {
        const String& propertyName = propertyIt.getCurrentKey();
        const String propertyValue = getProperty(propertyName);

        // we never copy stuff that doesn't get written into XML
        if (isPropertyBannedFromXML(propertyName))
            continue;

        // some cases when propertyValue is "" could lead to exception throws
        if (propertyValue.empty())
        {
            // special case, this causes exception throw when no window renderer
            // is assigned to the window
            if (propertyName == "LookNFeel")
                continue;

            // special case, this causes exception throw because we are setting
            // 'null' window renderer
            if (propertyName == "WindowRenderer")
                continue;
        }

        target.setProperty(propertyName, getProperty(propertyName));
    }
}

//----------------------------------------------------------------------------//
void Window::cloneChildWidgetsTo(Window& target) const
{
    const String& oldName = getName();
    const String& newName = target.getName();

    // todo: ChildWindowIterator?
    for (size_t childI = 0; childI < getChildCount(); ++childI)
    {
        Window* child = getChildAtIdx(childI);
        if (child->isAutoWindow())
        {
            // we skip auto windows, they are already created
            // automatically

            // note: some windows store non auto windows inside auto windows,
            //       standard solution is to copy these non-auto windows to
            //       the parent window
            //
            //       If you need alternative behaviour, you have to override
            //       this method!

            // so just copy it's child widgets
            child->cloneChildWidgetsTo(target);
            // and skip the auto widget
            continue;
        }

        String newChildName = child->getName();
        String::size_type idxBegin = newChildName.find(oldName + "/");
        if (idxBegin == String::npos)
        {
            // not found, user is using non-standard naming
            // use a pretty safe but long and non readable new name
            newChildName = newChildName + "_clone_" + newName;
        }
        else
        {
            // +1 because of the "/"
            String::size_type idxEnd = idxBegin + oldName.length() + 1;

            // we replace the first occurence of the old parent's name with the new name
            // this works great in case user uses the default recommended naming scheme
            // like this: "Parent/ChildWindow/ChildChildWindow"
            newChildName.replace(idxBegin, idxEnd, newName + "/");
        }

        Window* newChild = child->clone(newChildName, true);
        target.addChild(newChild);
    }
}

//----------------------------------------------------------------------------//
Rectf Window::getChildWindowContentArea(const bool non_client) const
{
    return non_client ?
        getNonClientChildWindowContentArea_impl() :
        getClientChildWindowContentArea_impl();
}

//----------------------------------------------------------------------------//
Rectf Window::getNonClientChildWindowContentArea_impl() const
{
    return getUnclippedOuterRect_impl();
}

//----------------------------------------------------------------------------//
Rectf Window::getClientChildWindowContentArea_impl() const
{
    return getUnclippedInnerRect_impl();
}

//----------------------------------------------------------------------------//
size_t Window::getZIndex() const
{
    if (!d_parent)
        return 0;

    ChildList::iterator i = std::find(
        d_parent->d_drawList.begin(),
        d_parent->d_drawList.end(),
        this);

    if (i == d_parent->d_drawList.end())
        CEGUI_THROW(InvalidRequestException("Window::getZIndex: Window is not "
            "in its parent's draw list."));

    return std::distance(d_parent->d_drawList.begin(), i);
}

//----------------------------------------------------------------------------//
bool Window::isInFront(const Window& wnd) const
{
    // children are always in front of their ancestors
    if (isAncestor(&wnd))
        return true;

    // conversely, ancestors are always behind their children
    if (wnd.isAncestor(this))
        return false;

    const Window* const w1 = getWindowAttachedToCommonAncestor(wnd);

    // seems not to be in same window hierarchy
    if (!w1)
        return false;

    const Window* const w2 = wnd.getWindowAttachedToCommonAncestor(*this);

    // at this point, w1 and w2 share the same parent.
    return w2->getZIndex() > w1->getZIndex();
}

//----------------------------------------------------------------------------//
const Window* Window::getWindowAttachedToCommonAncestor(const Window& wnd) const
{
    const Window* w = &wnd;
    const Window* tmp = w->d_parent;

    while (tmp)
    {
        if (isAncestor(tmp))
            break;

        w = tmp;
        tmp = tmp->d_parent;
    }

    return tmp ? w : 0;
}

//----------------------------------------------------------------------------//
bool Window::isBehind(const Window& wnd) const
{
    return !isInFront(wnd);
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
