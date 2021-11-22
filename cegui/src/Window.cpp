/***********************************************************************
    created:    21/2/2004
    author:     Paul D Turner

    purpose:    Implements the Window base class
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
#include "CEGUI/Window.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/FontManager.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/CoordConverter.h"
#include "CEGUI/WindowRendererManager.h"
#include "CEGUI/WindowFactoryManager.h"
#include "CEGUI/widgets/Tooltip.h"
#include "CEGUI/falagard/WidgetLookManager.h"
#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/RenderingContext.h"
#include "CEGUI/RenderingWindow.h"
#include "CEGUI/RenderTarget.h"
#include "CEGUI/GlobalEventSet.h"
#include "CEGUI/SharedStringStream.h"
#if defined (CEGUI_USE_FRIBIDI)
#include "CEGUI/FribidiVisualMapping.h"
#elif defined (CEGUI_USE_MINIBIDI)
#include "CEGUI/MinibidiVisualMapping.h"
#endif
#if defined(CEGUI_USE_RAQM)
    #include "CEGUI/RaqmTextData.h"
#endif

#include <queue>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
const String Window::AlphaPropertyName("Alpha");
const String Window::AlwaysOnTopPropertyName("AlwaysOnTop");
const String Window::ClippedByParentPropertyName("ClippedByParent");
const String Window::DestroyedByParentPropertyName("DestroyedByParent");
const String Window::DisabledPropertyName("Disabled");
const String Window::FontPropertyName("Font");
const String Window::IDPropertyName("ID");
const String Window::InheritsAlphaPropertyName("InheritsAlpha");
const String Window::CursorImagePropertyName("CursorImage");
const String Window::VisiblePropertyName("Visible");
const String Window::ActivePropertyName("Active");
const String Window::RestoreOldCapturePropertyName("RestoreOldCapture");
const String Window::TextPropertyName("Text");
const String Window::ZOrderingEnabledPropertyName("ZOrderingEnabled");
const String Window::CursorAutoRepeatEnabledPropertyName("CursorAutoRepeatEnabled");
const String Window::AutoRepeatDelayPropertyName("AutoRepeatDelay");
const String Window::AutoRepeatRatePropertyName("AutoRepeatRate");
const String Window::DistributeCapturedInputsPropertyName("DistributeCapturedInputs");
const String Window::TooltipTypePropertyName("TooltipType");
const String Window::TooltipTextPropertyName("TooltipText");
const String Window::InheritsTooltipTextPropertyName("InheritsTooltipText");
const String Window::RiseOnClickEnabledPropertyName("RiseOnClickEnabled");
const String Window::CursorPassThroughEnabledPropertyName("CursorPassThroughEnabled");
const String Window::DragDropTargetPropertyName("DragDropTarget");
const String Window::AutoRenderingSurfacePropertyName("AutoRenderingSurface");
const String Window::AutoRenderingSurfaceStencilEnabledPropertyName("AutoRenderingSurfaceStencilEnabled");
const String Window::TextParsingEnabledPropertyName("TextParsingEnabled");
const String Window::MarginPropertyName("MarginProperty");
const String Window::UpdateModePropertyName("UpdateMode");
const String Window::CursorInputPropagationEnabledPropertyName("CursorInputPropagationEnabled");
const String Window::AutoWindowPropertyName("AutoWindow");
const String Window::DrawModeMaskPropertyName("DrawModeMask");
//----------------------------------------------------------------------------//
const String Window::EventNamespace("Window");
const String Window::EventUpdated ("Updated");
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
const String Window::EventInvalidated( "Invalidated" );
const String Window::EventRenderingStarted( "RenderingStarted" );
const String Window::EventRenderingEnded( "RenderingEnded" );
const String Window::EventDestructionStarted( "DestructionStarted" );
const String Window::EventDragDropItemEnters("DragDropItemEnters");
const String Window::EventDragDropItemLeaves("DragDropItemLeaves");
const String Window::EventDragDropItemDropped("DragDropItemDropped");
const String Window::EventWindowRendererAttached("WindowRendererAttached");
const String Window::EventWindowRendererDetached("WindowRendererDetached");
const String Window::EventTextParsingChanged("TextParsingChanged");
const String Window::EventMarginChanged("MarginChanged");
const String Window::EventCursorEntersArea("CursorEntersArea");
const String Window::EventCursorLeavesArea("CursorLeavesArea");
const String Window::EventCursorEntersSurface("CursorEntersSurface");
const String Window::EventCursorLeavesSurface("CursorLeavesSurface");
const String Window::EventCursorMove("CursorMove");
const String Window::EventCursorPressHold("CursorPressHold");
const String Window::EventSelectWord("SelectWord");
const String Window::EventSelectAll("SelectAll");
const String Window::EventCursorActivate("CursorActivate");
const String Window::EventCharacterKey("CharacterKey");
const String Window::EventScroll("Scroll");
const String Window::EventSemanticEvent("SemanticEvent");

//----------------------------------------------------------------------------//
// XML element and attribute names that relate to Window.
const String Window::WindowXMLElementName("Window");
const String Window::AutoWindowXMLElementName("AutoWindow");
const String Window::UserStringXMLElementName("UserString");
const String Window::WindowTypeXMLAttributeName("type");
const String Window::WindowNameXMLAttributeName("name");
const String Window::AutoWindowNamePathXMLAttributeName("namePath");
const String Window::UserStringNameXMLAttributeName("name");
const String Window::UserStringValueXMLAttributeName("value");

//----------------------------------------------------------------------------//
const String Window::TooltipNameSuffix("__auto_tooltip__");

//----------------------------------------------------------------------------//
BasicRenderedStringParser Window::d_basicStringParser;
DefaultRenderedStringParser Window::d_defaultStringParser;

//----------------------------------------------------------------------------//
Window::WindowRendererProperty Window::d_windowRendererProperty;
Window::LookNFeelProperty Window::d_lookNFeelProperty;

//----------------------------------------------------------------------------//
Window::WindowRendererProperty::WindowRendererProperty() : TplWindowProperty<Window,String>(
    "WindowRenderer",
    "Property to get/set the assigned WindowRenderer object type. For "
    "advanced use only. See the api reference for Window::setWindowRenderer "
    "for more information. "
    "Value is a string.",
    "Window", &Window::setWindowRenderer,&Window::getWindowRendererName, "")
    {}

//----------------------------------------------------------------------------//
void Window::WindowRendererProperty::writeXMLToStream(const PropertyReceiver* receiver,
                                                      XMLSerializer& xml_stream) const
{
    const Window* wnd = static_cast<const Window*>(receiver);

    if (!WindowFactoryManager::getSingleton().isFalagardMappedType(wnd->getType()))
        Property::writeXMLToStream(receiver, xml_stream);
}

//----------------------------------------------------------------------------//
Window::LookNFeelProperty::LookNFeelProperty() : TplWindowProperty<Window,String>(
    "LookNFeel",
    "Property to get/set the assigned look'n'feel. For advanced use only. See "
    "the api reference for Window::setLookNFeel for more informaton. "
    "Value is a string.",
    "Window", &Window::setLookNFeel,&Window::getLookNFeel, "")
    {}

//----------------------------------------------------------------------------//
void Window::LookNFeelProperty::writeXMLToStream(const PropertyReceiver* receiver,
                                                 XMLSerializer& xml_stream) const
{
    const Window* wnd = static_cast<const Window*>(receiver);

    if (!WindowFactoryManager::getSingleton().isFalagardMappedType(wnd->getType()))
        Property::writeXMLToStream(receiver, xml_stream);
}

//----------------------------------------------------------------------------//
Window::Window(const String& type, const String& name):
    NamedElement(name),

    // basic types and initial window name
    d_type(type),
    d_autoWindow(false),

    // basic state
    d_initialising(false),
    d_destructionStarted(false),
    d_enabled(true),
    d_visible(true),
    d_active(false),

    // parent related fields
    d_destroyedByParent(true),

    // clipping options
    d_clippedByParent(true),

    // rendering components and options
    d_windowRenderer(nullptr),
    d_surface(nullptr),
    d_needsRedraw(true),
    d_autoRenderingWindow(false),
    d_autoRenderingSurfaceStencilEnabled(false),
    d_cursor(nullptr),

    // alpha transparency set up
    d_alpha(1.0f),
    d_inheritsAlpha(true),

    // cursor input capture set up
    d_oldCapture(nullptr),
    d_restoreOldCapture(false),
    d_distCapturedInputs(false),

    // text system set up
    d_font(nullptr),

#if defined (CEGUI_USE_FRIBIDI)
    d_bidiVisualMapping(new FribidiVisualMapping),
    d_bidiDataValid(false),
#elif defined (CEGUI_USE_MINIBIDI)
    d_bidiVisualMapping(new MinibidiVisualMapping),
    d_bidiDataValid(false),
#elif defined (CEGUI_BIDI_SUPPORT)
    #error "BIDI Configuration is inconsistant, check your config!"
#endif
#ifdef CEGUI_USE_RAQM
    d_raqmTextData(new RaqmTextData()),
#endif

    d_renderedStringValid(false),
    d_customStringParser(nullptr),
    d_textParsingEnabled(true),

    // margin
    d_margin(UBox(UDim(0, 0))),

    // user specific data
    d_ID(0),
    d_userData(nullptr),

    // z-order related options
    d_alwaysOnTop(false),
    d_riseOnPointerActivation(true),
    d_zOrderingEnabled(true),

    d_cursorPassThroughEnabled(false),
    d_autoRepeat(false),
    d_repeatDelay(0.3f),
    d_repeatRate(0.06f),
    d_repeatPointerSource(CursorInputSource::NotSpecified),
    d_repeating(false),
    d_repeatElapsed(0.0f),

    // drag and drop
    d_dragDropTarget(true),

    // tool tip related
    d_customTip(nullptr),
    d_weOwnTip(false),
    d_inheritsTipText(true),
    d_tooltipEnabled(true),

    // XML writing options
    d_allowWriteXML(true),

    // initialise area cache rects
    d_outerRectClipper(0, 0, 0, 0),
    d_innerRectClipper(0, 0, 0, 0),
    d_hitTestRect(0, 0, 0, 0),

    // cached pixel rect validity flags
    d_outerRectClipperValid(false),
    d_innerRectClipperValid(false),
    d_hitTestRectValid(false),

    // Initial update mode
    d_updateMode(WindowUpdateMode::Visible),

    // Don't propagate cursor inputs by default.
    d_propagatePointerInputs(false),

    d_guiContext(nullptr),

    d_containsPointer(false),
    d_isFocused(false),

    d_drawModeMask(DrawModeFlagWindowRegular)
{

    d_fontRenderSizeChangeConnection =
        GlobalEventSet::getSingleton().subscribeEvent(
            "Font/RenderSizeChanged",
            Event::Subscriber(&Window::handleFontRenderSizeChange, this));

    // add properties
    addWindowProperties();
}

//----------------------------------------------------------------------------//
Window::~Window()
{
    // most cleanup actually happened earlier in Window::destroy.
    destroyGeometryBuffers();

#ifdef CEGUI_BIDI_SUPPORT
    delete d_bidiVisualMapping;
#endif
}

//----------------------------------------------------------------------------//
void Window::endInitialisation()
{
    d_initialising = false;
    performChildLayout(false, false);
}

//----------------------------------------------------------------------------//
bool Window::isChild(unsigned int ID) const
{
    const size_t child_count = getChildCount();

    for (size_t i = 0; i < child_count; ++i)
        if (getChildAtIndex(i)->getID() == ID)
            return true;

    return false;
}

//----------------------------------------------------------------------------//
bool Window::isChildRecursive(unsigned int ID) const
{
    const size_t child_count = getChildCount();

    for (size_t i = 0; i < child_count; ++i)
        if (getChildAtIndex(i)->getID() == ID || getChildAtIndex(i)->isChildRecursive(ID))
            return true;

    return false;
}

//----------------------------------------------------------------------------//
Window* Window::getChild(unsigned int id) const
{
    if (Window* w = findChild(id))
        return w;

    std::stringstream& sstream = SharedStringstream::GetPreparedStream();
    sstream << std::hex << id << std::dec;

    throw UnknownObjectException("A Window with ID: '" +
        sstream.str() + "' is not attached to Window '" + d_name + "'.");
}

//----------------------------------------------------------------------------//
Window* Window::findChild(unsigned int id) const
{
    const size_t child_count = getChildCount();

    for (size_t i = 0; i < child_count; ++i)
        if (getChildAtIndex(i)->getID() == id)
            return getChildAtIndex(i);

    return nullptr;
}

//----------------------------------------------------------------------------//
Window* Window::getChildRecursive(unsigned int ID) const
{
    std::queue<Element*> ElementsToSearch;

    for (Element* child : d_children) // load all children into the queue
        ElementsToSearch.push(child);

    while (!ElementsToSearch.empty()) // breadth-first search for the child to find
    {
        Element* child = ElementsToSearch.front();
        ElementsToSearch.pop();

        if (Window* window = dynamic_cast<Window*>(child))
        {
            if (window->getID() == ID)
            {
                return window;
            }
        }

        const size_t element_child_count = child->getChildCount();
        for(size_t i = 0; i < element_child_count; ++i)
        {
            ElementsToSearch.push(child->getChildElementAtIndex(i));
        }
    }

    return nullptr;
}

//----------------------------------------------------------------------------//
Window* Window::getActiveChild()
{
    return const_cast<Window*>(
        static_cast<const Window*>(this)->getActiveChild());
}

//----------------------------------------------------------------------------//
const Window* Window::getActiveChild() const
{
    // are children can't be active if we are not
    if (!isActive())
        return nullptr;

    for (auto it = d_drawList.crbegin(); it != d_drawList.crend(); ++it)
    {
        // don't need full backward scan for activeness as we already know
        // 'this' is active.  NB: This uses the draw-ordered child list, as that
        // should be quicker in most cases.

        const Window* wnd = *it;
        if (wnd->d_active)
            return wnd->getActiveChild();
    }

    // no child was active, therefore we are the topmost active window
    return this;
}

//----------------------------------------------------------------------------//
bool Window::isAncestor(unsigned int ID) const
{
    // return false if we have no ancestor
    if (!d_parent)
        return false;

    // check our immediate parent
    if (getParent()->getID() == ID)
        return true;

    // not our parent, check back up the family line
    return getParent()->isAncestor(ID);
}

//----------------------------------------------------------------------------//
const Font* Window::getActualFont() const
{
    if (!d_font)
    {
        GUIContext* context = getGUIContextPtr();
        return context ? context->getDefaultFont() : nullptr;
    }

    return d_font;
}

//----------------------------------------------------------------------------//
float Window::getEffectiveAlpha() const
{
    if (!d_parent || !inheritsAlpha())
        return d_alpha;

    return d_alpha * getParent()->getEffectiveAlpha();
}

//----------------------------------------------------------------------------//
const Rectf& Window::getOuterRectClipper() const
{
    if (!d_outerRectClipperValid)
    {
        d_outerRectClipper = getOuterRectClipper_impl();
        d_outerRectClipperValid = true;
    }

    return d_outerRectClipper;
}

//----------------------------------------------------------------------------//
const Rectf& Window::getInnerRectClipper() const
{
    if (!d_innerRectClipperValid)
    {
        d_innerRectClipper = getInnerRectClipper_impl();
        d_innerRectClipperValid = true;
    }

    return d_innerRectClipper;
}

//----------------------------------------------------------------------------//
const Rectf& Window::getHitTestRect() const
{
    if (!d_hitTestRectValid)
    {
        d_hitTestRect = getHitTestRect_impl();
        d_hitTestRectValid = true;
    }

    return d_hitTestRect;
}

//----------------------------------------------------------------------------//
Rectf Window::getParentClipRect() const
{
    return (d_parent && d_clippedByParent) ?
        getParent()->getClipRect(d_nonClient) :
        Rectf(glm::vec2(0.f, 0.f), getRootContainerSize());
}

//----------------------------------------------------------------------------//
Window* Window::getCaptureWindow() const
{
    GUIContext* context = getGUIContextPtr();
    return context ? context->getInputCaptureWindow() : nullptr;
}

//----------------------------------------------------------------------------//
Rectf Window::getUnclippedInnerRect_impl(bool skipAllPixelAlignment) const
{
    // TODO: handle skipAllPixelAlignment in a window renderer!
    return d_windowRenderer ?
        d_windowRenderer->getUnclippedInnerRect() :
        Element::getUnclippedInnerRect_impl(skipAllPixelAlignment);
}

//----------------------------------------------------------------------------//
Rectf Window::getOuterRectClipper_impl() const
{
    return (d_surface && d_surface->isRenderingWindow()) ?
        getUnclippedOuterRect().get() :
        getUnclippedOuterRect().get().getIntersection(getParentClipRect());
}

//----------------------------------------------------------------------------//
Rectf Window::getInnerRectClipper_impl() const
{
    return (d_surface && d_surface->isRenderingWindow()) ?
        getUnclippedInnerRect().get() :
        getUnclippedInnerRect().get().getIntersection(getParentClipRect());
}

//----------------------------------------------------------------------------//
Rectf Window::getHitTestRect_impl() const
{
    // if clipped by parent wnd, hit test area is the intersection of our
    // outer rect with the parent's hit test area intersected with the
    // parent's clipper.
    if (d_parent && d_clippedByParent)
    {
        return getUnclippedOuterRect().get().getIntersection(
            getParent()->getHitTestRect().getIntersection(
                getParent()->getClipRect(d_nonClient)));
    }
    // not clipped to parent wnd, so get intersection with screen area.
    else
    {
        return getUnclippedOuterRect().get().getIntersection(
            Rectf(glm::vec2(0, 0), getRootContainerSize()));
    }
}

//----------------------------------------------------------------------------//
bool Window::isHit(const glm::vec2& position, const bool allow_disabled) const
{
    // cannot be hit if we are disabled.
    if (!allow_disabled && isEffectiveDisabled())
        return false;

    const Rectf& test_area = getHitTestRect();

    if ((test_area.getWidth() == 0.0f) || (test_area.getHeight() == 0.0f))
        return false;

    return test_area.isPointInRectf(position);
}

//----------------------------------------------------------------------------//
Window* Window::getChildAtPosition(const glm::vec2& position) const
{
    return getChildAtPosition(position, &Window::isHit);
}

//----------------------------------------------------------------------------//
Window* Window::getChildAtPosition(const glm::vec2& position,
                    bool (Window::*hittestfunc)(const glm::vec2&, bool) const,
                    bool allow_disabled,
                    const Window* const exclude) const
{
    glm::vec2 p;
    // if the window has RenderingWindow backing
    if (d_surface && d_surface->isRenderingWindow())
        static_cast<RenderingWindow*>(d_surface)->unprojectPoint(position, p);
    else
        p = position;

    const auto end = d_drawList.crend();
    for (auto child = d_drawList.rbegin(); child != end; ++child)
    {
        if ((*child) != exclude && (*child)->isEffectiveVisible())
        {
            // recursively scan for hit on children of this child window...
            if (Window* const wnd = (*child)->getChildAtPosition(p, hittestfunc, allow_disabled, exclude))
                return wnd;
            // see if this child is hit and return it's cursor if it is
            else if (((*child)->*hittestfunc)(p, allow_disabled))
                return *child;
        }
    }

    // nothing hit
    return nullptr;
}

//----------------------------------------------------------------------------//
Window* Window::getTargetChildAtPosition(const glm::vec2& position,
                                         const bool allow_disabled,
                                         const Window* const exclude) const
{
    return getChildAtPosition(position, &Window::isHitTargetWindow, allow_disabled, exclude);
}

//----------------------------------------------------------------------------//
bool Window::isHitTargetWindow(const glm::vec2& position, bool allow_disabled) const
{
    return !isCursorPassThroughEnabled() && isHit(position, allow_disabled);
}

//----------------------------------------------------------------------------//
void Window::setEnabled(bool enabled)
{
    // only react if setting has changed
    if (d_enabled == enabled)
        return;

    d_enabled = enabled;
    WindowEventArgs args(this);

    if (d_enabled)
    {
        // check to see if the window is actually enabled (which depends
        // upon all ancestor windows being enabled) we do this so that
        // events we fire give an accurate indication of the state of a
        // window.
        if ((d_parent && !getParent()->isDisabled()) || !d_parent)
            onEnabled(args);
    }
    else
    {
        onDisabled(args);
    }

    if (GUIContext* context = getGUIContextPtr())
        context->updateWindowContainingCursor();
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

    if (GUIContext* context = getGUIContextPtr())
        context->updateWindowContainingCursor();
}

//----------------------------------------------------------------------------//
void Window::setActive(bool setting)
{
    if (isActive() == setting)
        return;

    if (setting)
        activate();
    else
        deactivate();
}

//----------------------------------------------------------------------------//
bool Window::activate_impl(bool byClick)
{
    // exit if the window is not visible, since a hidden window may not be the
    // active window.
    if (!isEffectiveVisible())
        return false;

    // force complete release of input capture.
    // NB: This is not done via releaseCapture() because that has
    // different behaviour depending on the restoreOldCapture setting.
    Window* const tmpCapture = getCaptureWindow();
    if (tmpCapture && tmpCapture != this)
    {
        getGUIContext().setInputCaptureWindow(nullptr);

        WindowEventArgs args(nullptr);
        tmpCapture->onCaptureLost(args);
    }

    // get immediate child of parent that is currently active (if any)
    Window* const activeWnd = getActiveSibling();
    if (activeWnd == this)
        return false;

    const bool handled = moveToFront_impl(byClick);

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

    return handled;
}

//----------------------------------------------------------------------------//
void Window::deactivate()
{
    if (isActive())
    {
        ActivationEventArgs args(this);
        args.otherWindow = nullptr;
        onDeactivated(args);
    }
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

#ifdef CEGUI_BIDI_SUPPORT
    d_bidiDataValid = false;
#endif

    WindowEventArgs args(this);
    onTextChanged(args);
}

//----------------------------------------------------------------------------//
void Window::setFont(const Font* font)
{
    if (d_font == font)
        return;

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
void Window::removeChild(unsigned int ID)
{
    const size_t child_count = getChildCount();

    for (size_t i = 0; i < child_count; ++i)
    {
        if (getChildAtIndex(i)->getID() == ID)
        {
            removeChild(d_children[i]);
            return;
        }

    }
}

//----------------------------------------------------------------------------//
Window* Window::createChild(const String& type, const String& name)
{
    Window* ret = WindowManager::getSingleton().createWindow(type, name);
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
void Window::destroyChild(const String& name_path)
{
    destroyChild(getChild(name_path));
}

//----------------------------------------------------------------------------//
void Window::moveToFront()
{
    moveToFront_impl(false);
}

//----------------------------------------------------------------------------//
bool Window::moveToFront_impl(bool byClick)
{
    // no siblings - no Z-ordering
    if (!d_parent)
        return false;

    // bring parent window to front of it's siblings
    bool took_action = getParent()->moveToFront_impl(byClick);

    // bring us to the front of our siblings
    if (d_zOrderingEnabled &&
        (!byClick || d_riseOnPointerActivation) &&
        !isTopOfZOrder())
    {
        took_action = true;

        // remove us from our parent's draw list
        getParent()->removeWindowFromDrawList(*this);
        // re-attach ourselves to our parent's draw list which will move us in
        // front of sibling windows with the same 'always-on-top' setting as we
        // have.
        getParent()->addWindowToDrawList(*this);
        // notify relevant windows about the z-order change.
        onZChange_impl();
    }

    return took_action;
}

//----------------------------------------------------------------------------//
void Window::moveToBack()
{
    deactivate();

    // we only proceed if we have a parent (otherwise we can have no siblings)
    if (d_parent)
    {
        if (d_zOrderingEnabled)
        {
            // remove us from our parent's draw list
            getParent()->removeWindowFromDrawList(*this);
            // re-attach ourselves to our parent's draw list which will move us
            // in behind sibling windows with the same 'always-on-top' setting
            // as we have.
            getParent()->addWindowToDrawList(*this, true);
            // notify relevant windows about the z-order change.
            onZChange_impl();
        }

        // FIXME: what for?
        getParent()->moveToBack();
    }
}

//----------------------------------------------------------------------------//
void Window::moveInFront(const Window* const window)
{
    if (!window || !window->d_parent || window->d_parent != d_parent ||
        window == this || window->d_alwaysOnTop != d_alwaysOnTop ||
        !d_zOrderingEnabled)
        return;

    // erase us from our current position
    const auto p = std::find(getParent()->d_drawList.begin(),
        getParent()->d_drawList.end(),
        this);
    if (p != getParent()->d_drawList.end())
        getParent()->d_drawList.erase(p);

    // find window we're to be moved in front of in parent's draw list
    auto i = std::find(getParent()->d_drawList.begin(),
        getParent()->d_drawList.end(),
        window);
    // sanity check that target window was also attached to correct parent.
    assert(i != getParent()->d_drawList.end());

    // reinsert ourselves at the right location
    getParent()->d_drawList.insert(++i, this);

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
    const auto p = std::find(getParent()->d_drawList.begin(),
        getParent()->d_drawList.end(),
        this);
    // sanity checK that we were attached to our parent.
    assert(p != getParent()->d_drawList.end());

    // erase us from our current position
    getParent()->d_drawList.erase(p);

    // find window we're to be moved in front of in parent's draw list
    const auto i = std::find(getParent()->d_drawList.begin(),
        getParent()->d_drawList.end(),
        window);
    // sanity check that target window was also attached to correct parent.
    assert(i != getParent()->d_drawList.end());

    // reinsert ourselves at the right location
    getParent()->d_drawList.insert(i, this);

    // handle event notifications for affected windows.
    onZChange_impl();
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
size_t Window::getZIndex() const
{
    if (!d_parent)
        return 0;

    const auto& parentDrawList = getParent()->d_drawList;
    auto it = std::find(parentDrawList.begin(), parentDrawList.end(), this);
    if (it == getParent()->d_drawList.end())
        throw InvalidRequestException(
            "Window is not in its parent's draw list.");

    return std::distance(parentDrawList.begin(), it);
}

//----------------------------------------------------------------------------//
void Window::setAlwaysOnTop(bool setting)
{
    // only react to an actual change
    if (isAlwaysOnTop() == setting)
        return;

    d_alwaysOnTop = setting;

    // we only proceed if we have a parent (otherwise we can have no siblings)
    if (d_parent)
    {
        if (d_zOrderingEnabled)
        {
            // remove us from our parent's draw list
            getParent()->removeWindowFromDrawList(*this);
            // re-attach ourselves to our parent's draw list which will move us
            // in behind sibling windows with the same 'always-on-top' setting
            // as we have.
            getParent()->addWindowToDrawList(*this, true);
            // notify relevant windows about the z-order change.
            onZChange_impl();
        }
    }

    WindowEventArgs args(this);
    onAlwaysOnTopChanged(args);
}

//----------------------------------------------------------------------------//
bool Window::captureInput()
{
    // we can only capture if we are the active window (LEAVE THIS ALONE!)
    if (!isActive())
        return false;

    if (!isCapturedByThis())
    {
        Window* const current_capture = getCaptureWindow();
        getGUIContext().setInputCaptureWindow(this);
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
void Window::releaseInput()
{
    // if we are not the window that has capture, do nothing
    if (!isCapturedByThis())
        return;

    // restore old captured window if that mode is set
    if (d_restoreOldCapture)
    {
        getGUIContext().setInputCaptureWindow(d_oldCapture);

        // check for case when there was no previously captured window
        if (d_oldCapture)
        {
            d_oldCapture = nullptr;
            getCaptureWindow()->moveToFront();
        }

    }
    else
        getGUIContext().setInputCaptureWindow(nullptr);

    WindowEventArgs args(this);
    onCaptureLost(args);
}

//----------------------------------------------------------------------------//
void Window::setRestoreOldCapture(bool setting)
{
    d_restoreOldCapture = setting;

    const size_t child_count = getChildCount();

    for (size_t i = 0; i < child_count; ++i)
        getChildAtIndex(i)->setRestoreOldCapture(setting);
}

//----------------------------------------------------------------------------//
void Window::setAlpha(const float alpha)
{
    // clamp this to the valid range [0.0, 1.0]
    float clampedAlpha = std::max(std::min(alpha, 1.0f), 0.0f);

    // Ensure that the new alpha value is actually different from the currently set one
    // to avoid unnecessary invalidating and re-caching of textures and children
    if (d_alpha == clampedAlpha)
        return;

    d_alpha = clampedAlpha;

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
void Window::invalidate(const bool recursive)
{
    invalidate_impl(recursive);

    if (GUIContext* context = getGUIContextPtr())
        context->markAsDirty();
}

//----------------------------------------------------------------------------//
void Window::invalidate_impl(const bool recursive)
{
    d_needsRedraw = true;
    invalidateRenderingSurface();

    WindowEventArgs args(this);
    onInvalidated(args);

    if (recursive)
    {
        for (Element* child : d_children)
            static_cast<Window*>(child)->invalidate_impl(true);
    }
}

//----------------------------------------------------------------------------//
void Window::draw(std::uint32_t drawModeMask)
{
    // don't do anything if window is not visible
    if (!isEffectiveVisible())
        return;

    // get rendering context
    RenderingContext ctx;
    getRenderingContext(ctx);

    // clear geometry from surface if it's ours
    if (ctx.owner == this)
        ctx.surface->clearGeometry();

    const bool allowDrawing = checkIfDrawMaskAllowsDrawing(drawModeMask);

    // redraw if no surface set, or if surface is invalidated
    if (!d_surface || d_surface->isInvalidated())
    {
        if (allowDrawing)
        {
            // perform drawing for 'this' Window
            drawSelf(ctx, drawModeMask);
        }

        // render any child windows
        for (auto wnd : d_drawList)
            wnd->draw(drawModeMask);
    }

    // do final rendering for surface if it's ours
    if (ctx.owner == this && allowDrawing)
        ctx.surface->draw(drawModeMask);
}

//----------------------------------------------------------------------------//
void Window::drawSelf(const RenderingContext& ctx, std::uint32_t drawModeMask)
{
    bufferGeometry(ctx, drawModeMask);
    queueGeometry(ctx);
}

//----------------------------------------------------------------------------//
void Window::bufferGeometry(const RenderingContext&, std::uint32_t /*drawModeMask*/)
{
    if (d_needsRedraw)
    {
        // dispose of already cached geometry.
        destroyGeometryBuffers();

        // signal rendering started
        WindowEventArgs args(this);
        onRenderingStarted(args);

        // HACK: ensure our rendered string content is up to date
        getRenderedString();

        // get derived class or WindowRenderer to re-populate geometry buffer.
        if (d_windowRenderer)
            d_windowRenderer->createRenderGeometry();
        else
            populateGeometryBuffer();

        // Setup newly created geometry with our settings
        const float finalAlpha = getEffectiveAlpha();
        for (CEGUI::GeometryBuffer* currentBuffer : d_geometryBuffers)
        {
            currentBuffer->setTranslation(d_translation);
            currentBuffer->setClippingRegion(d_clippingRegion);
            currentBuffer->setAlpha(finalAlpha);
        }

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
    ctx.surface->addGeometryBuffers(ctx.queue, d_geometryBuffers);
}

//----------------------------------------------------------------------------//
void Window::cleanupChildren()
{
    while (getChildCount() != 0)
    {
        Window* wnd = static_cast<Window*>(d_children[0]);

        // always remove child
        removeChild(wnd);

        // destroy child if that is required
        if (wnd->isDestroyedByParent())
            WindowManager::getSingleton().destroyWindow(wnd);
    }
}

//----------------------------------------------------------------------------//
void Window::addChild_impl(Element* element)
{
    Window* wnd = dynamic_cast<Window*>(element);

    if (!wnd)
        throw InvalidRequestException(
            "Window can only have Elements of type Window added as children "
            "(Window path: " + getNamePath() + ").");

    NamedElement::addChild_impl(wnd);

    // TODO: also propagate GUI context, see setGUIContext
    wnd->onTargetSurfaceChanged(getTargetRenderingSurface());

    addWindowToDrawList(*wnd);

    wnd->invalidate(true);

    wnd->onZChange_impl();

    // If window uses default font, handle its possible change
    // TODO: remove if GUI context will be propagated, setGUIContext will have to handle this!
    wnd->notifyDefaultFontChanged();
}

//----------------------------------------------------------------------------//
void Window::removeChild_impl(Element* element)
{
    Window* wnd = static_cast<Window*>(element);

    Window* captureWnd = getCaptureWindow();
    if (captureWnd && (captureWnd == wnd || captureWnd->isAncestor(wnd)))
        captureWnd->releaseInput();

    // remove from draw list
    removeWindowFromDrawList(*wnd);

    NamedElement::removeChild_impl(wnd);

    // TODO: also propagate GUI context, see setGUIContext
    wnd->onTargetSurfaceChanged(nullptr);

    wnd->onZChange_impl();

    // Removed windows should not be active anymore (they are not attached
    // to anything so this would not make sense)
    if (wnd->isActive())
        wnd->deactivate();
}

//----------------------------------------------------------------------------//
void Window::onZChange_impl()
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
            WindowEventArgs args(getParent()->getChildAtIndex(i));
            getParent()->getChildAtIndex(i)->onZChanged(args);
        }

    }

    if (GUIContext* context = getGUIContextPtr())
        context->updateWindowContainingCursor();
}

//----------------------------------------------------------------------------//
const Image* Window::getActualCursor() const
{
    if (!d_cursor)
    {
        GUIContext* context = getGUIContextPtr();
        return context ? context->getCursor().getDefaultImage() : nullptr;
    }

    return d_cursor;
}

//----------------------------------------------------------------------------//
void Window::setCursor(const String& name)
{
    setCursor(
        &ImageManager::getSingleton().get(name));
}

//----------------------------------------------------------------------------//
void Window::setCursor(const Image* image)
{
    d_cursor = image;

    GUIContext* context = getGUIContextPtr();
    if (context && context->getWindowContainingCursor() == this)
        context->getCursor().setImage(image);
}

//----------------------------------------------------------------------------//
void Window::setID(unsigned int ID)
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
void Window::generateAutoRepeatEvent(CursorInputSource source)
{
    CursorInputEventArgs ciea(this);
    ciea.position = getUnprojectedPosition(
        getGUIContext().getCursor().getPosition());
    ciea.moveDelta = glm::vec2(0, 0);
    ciea.source = source;
    ciea.scroll = 0;
    onCursorPressHold(ciea);
}

//----------------------------------------------------------------------------//
void Window::addWindowProperties()
{
    const String propertyOrigin("Window");

    CEGUI_DEFINE_PROPERTY(Window, float,
        AlphaPropertyName, "Property to get/set the alpha value of the Window. Value is floating point number.",
        &Window::setAlpha, &Window::getAlpha, 1.0f
    );

    CEGUI_DEFINE_PROPERTY(Window, bool,
        AlwaysOnTopPropertyName, "Property to get/set the 'always on top' setting for the Window. Value is either \"true\" or \"false\".",
        &Window::setAlwaysOnTop, &Window::isAlwaysOnTop, false
    );

    CEGUI_DEFINE_PROPERTY(Window, bool,
        ClippedByParentPropertyName, "Property to get/set the 'clipped by parent' setting for the Window. Value is either \"true\" or \"false\".",
        &Window::setClippedByParent, &Window::isClippedByParent, true
    );

    CEGUI_DEFINE_PROPERTY(Window, bool,
        DestroyedByParentPropertyName, "Property to get/set the 'destroyed by parent' setting for the Window. Value is either \"true\" or \"false\".",
        &Window::setDestroyedByParent, &Window::isDestroyedByParent, true
    );

    CEGUI_DEFINE_PROPERTY(Window, bool,
        DisabledPropertyName, "Property to get/set the 'disabled state' setting for the Window.  Value is either \"true\" or \"false\".",
        &Window::setDisabled, &Window::isDisabled, false
    );

    CEGUI_DEFINE_PROPERTY(Window, Font*,
        FontPropertyName,"Property to get/set the font for the Window.  Value is the name of the font to use (must be loaded already).",
        &Window::setFont, &Window::getFont, nullptr
    );

    CEGUI_DEFINE_PROPERTY(Window, unsigned int,
        IDPropertyName, "Property to get/set the ID value of the Window. Value is an unsigned integer number.",
        &Window::setID, &Window::getID, 0
    );

    CEGUI_DEFINE_PROPERTY(Window, bool,
        InheritsAlphaPropertyName, "Property to get/set the 'inherits alpha' setting for the Window. Value is either \"true\" or \"false\".",
        &Window::setInheritsAlpha, &Window::inheritsAlpha, true
    );

    CEGUI_DEFINE_PROPERTY(Window, Image*,
        CursorImagePropertyName,"Property to get/set the mouse cursor image for the Window.  Value should be \"<image name>\".",
        &Window::setCursor, &Window::getCursor, nullptr
    );

    CEGUI_DEFINE_PROPERTY(Window, bool,
        VisiblePropertyName, "Property to get/set the 'visible state' setting for the Window. Value is either \"true\" or \"false\".",
        &Window::setVisible, &Window::isVisible, true
    );

    CEGUI_DEFINE_PROPERTY(Window, bool,
        ActivePropertyName, "Property to get/set the 'active' setting for the Window. Value is either \"true\" or \"false\".",
        &Window::setActive, &Window::isActive, false
    );

    CEGUI_DEFINE_PROPERTY(Window, bool,
        RestoreOldCapturePropertyName, "Property to get/set the 'restore old capture' setting for the Window. Value is either \"true\" or \"false\".",
        &Window::setRestoreOldCapture, &Window::restoresOldCapture, false
    );

    CEGUI_DEFINE_PROPERTY(Window, String,
        TextPropertyName, "Property to get/set the text / caption for the Window. Value is the text string to use. Meaning of this property heavily depends on the type of the Window.",
        &Window::setText, &Window::getText, ""
    );

    CEGUI_DEFINE_PROPERTY(Window, bool,
        ZOrderingEnabledPropertyName, "Property to get/set the 'z-order changing enabled' setting for the Window. Value is either \"true\" or \"false\".",
        &Window::setZOrderingEnabled, &Window::isZOrderingEnabled, true
    );

    CEGUI_DEFINE_PROPERTY(Window, bool,
        CursorAutoRepeatEnabledPropertyName, "Property to get/set whether the window will receive autorepeat cursor down events."
        "Note: clicks are not repeated because they require down & up. Value is either \"true\" or \"false\".",
        &Window::setCursorAutoRepeatEnabled, &Window::isCursorAutoRepeatEnabled, false
    );

    CEGUI_DEFINE_PROPERTY(Window, float,
        AutoRepeatDelayPropertyName, "Property to get/set the autorepeat delay. Value is a floating point number indicating the delay required in seconds.",
        &Window::setAutoRepeatDelay, &Window::getAutoRepeatDelay, 0.3f
    );

    CEGUI_DEFINE_PROPERTY(Window, float,
        AutoRepeatRatePropertyName, "Property to get/set the autorepeat rate. Value is a floating point number indicating the rate required in seconds.",
        &Window::setAutoRepeatRate, &Window::getAutoRepeatRate, 0.06f
    );

    CEGUI_DEFINE_PROPERTY(Window, bool,
        DistributeCapturedInputsPropertyName, "Property to get/set whether captured inputs are passed to child windows. Value is either \"true\" or \"false\".",
        &Window::setDistributesCapturedInputs, &Window::distributesCapturedInputs, false
    );

    CEGUI_DEFINE_PROPERTY(Window, String,
        TooltipTypePropertyName, "Property to get/set the custom tooltip for the window. Value is the type name of the custom tooltip. If \"\", the default System tooltip is used.",
        &Window::setTooltipType, &Window::getTooltipType, ""
    );

    CEGUI_DEFINE_PROPERTY(Window, String,
        TooltipTextPropertyName, "Property to get/set the tooltip text for the window. Value is the tooltip text for the window.",
        &Window::setTooltipText, &Window::getTooltipText, ""
    );

    CEGUI_DEFINE_PROPERTY(Window, bool,
        InheritsTooltipTextPropertyName, "Property to get/set whether the window inherits its parents tooltip text when it has none of its own. Value is either \"true\" or \"false\".",
        &Window::setInheritsTooltipText, &Window::inheritsTooltipText, true
    );

    CEGUI_DEFINE_PROPERTY(Window, bool,
        RiseOnClickEnabledPropertyName, "Property to get/set whether the window will come to the top of the Z-order when clicked. Value is either \"true\" or \"false\".",
        &Window::setRiseOnClickEnabled, &Window::isRiseOnPointerActivationEnabled, true
    );

    CEGUI_DEFINE_PROPERTY(Window, bool,
        CursorPassThroughEnabledPropertyName, "Property to get/set whether the window ignores cursor events and pass them through to any windows behind it. Value is either \"true\" or \"false\".",
        &Window::setCursorPassThroughEnabled, &Window::isCursorPassThroughEnabled, false
    );

    addProperty(&d_windowRendererProperty);
    addProperty(&d_lookNFeelProperty);

    CEGUI_DEFINE_PROPERTY(Window, bool,
        DragDropTargetPropertyName, "Property to get/set whether the Window will receive drag and drop related notifications.  Value is either \"true\" or \"false\".",
        &Window::setDragDropTarget, &Window::isDragDropTarget, true
    );

    CEGUI_DEFINE_PROPERTY(Window, bool,
        AutoRenderingSurfacePropertyName, "Property to get/set whether the Window will automatically attempt to "
        "use a full imagery caching RenderingSurface (if supported by the "
        "renderer).  Here, full imagery caching usually will mean caching a "
        "window's representation onto a texture (although no such "
        "implementation requirement is specified.)"
        "  Value is either \"true\" or \"false\".",
        &Window::setUsingAutoRenderingSurface, &Window::isUsingAutoRenderingSurface, false /* TODO: Inconsistency*/
    );

    CEGUI_DEFINE_PROPERTY(Window, bool,
        AutoRenderingSurfaceStencilEnabledPropertyName, "Property to get/set whether the Window's texture caching (if activated) "
        "will have a stencil buffer attached. This may be required for proper rendering of SVG images and Custom Shapes."
        "  Value is either \"true\" or \"false\".",
        &Window::setAutoRenderingSurfaceStencilEnabled, &Window::isAutoRenderingSurfaceStencilEnabled, false
        );

    CEGUI_DEFINE_PROPERTY(Window, bool,
        TextParsingEnabledPropertyName, "Property to get/set the text parsing setting for the Window.  "
        "Value is either \"true\" or \"false\".",
        &Window::setTextParsingEnabled, &Window::isTextParsingEnabled, true
    );

    CEGUI_DEFINE_PROPERTY(Window, UBox,
        MarginPropertyName, "Property to get/set margin for the Window. Value format:"
        "{top:{[tops],[topo]},left:{[lefts],[lefto]},bottom:{[bottoms],[bottomo]},right:{[rights],[righto]}}.",
        &Window::setMargin, &Window::getMargin, UBox(UDim(0, 0))
    );

    CEGUI_DEFINE_PROPERTY(Window, WindowUpdateMode,
        UpdateModePropertyName, "Property to get/set the window update mode setting.  "
        "Value is one of \"Always\", \"Never\" or \"Visible\".",
        &Window::setUpdateMode,&Window::getUpdateMode, WindowUpdateMode::Visible
    );

    CEGUI_DEFINE_PROPERTY(Window, bool,
        CursorInputPropagationEnabledPropertyName, "Property to get/set whether unhandled cursor inputs should be "
        "propagated back to the Window's parent.  "
        "Value is either \"true\" or \"false\".",
        &Window::setCursorInputPropagationEnabled, &Window::isCursorInputPropagationEnabled, false
    );

    CEGUI_DEFINE_PROPERTY(Window, bool,
        AutoWindowPropertyName, "Property to access whether the system considers this window to be an "
        "automatically created sub-component window."
        "Value is either \"true\" or \"false\".",
        &Window::setAutoWindow, &Window::isAutoWindow, false
    );

    CEGUI_DEFINE_PROPERTY(Window, std::uint32_t,
        DrawModeMaskPropertyName, "Property to get/set a bitmask that specifies whether the window should be "
        "drawn or not be drawn in a draw call. The draw call may have its own bitmask specified otherwise "
        "a bitmask with all bits at 1 is taken. The bitmask of the draw call and the Window are compared "
        "using a bitwise AND, only if the result is not zero the Window will be drawn."
        "Value is a bitmask of 32 bit size, which will be checked against the bitmask specified for the draw call.",
        &Window::setDrawModeMask, &Window::getDrawModeMask, DrawModeFlagWindowRegular
    );
}

//----------------------------------------------------------------------------//
void Window::setZOrderingEnabled(bool setting)
{
    d_zOrderingEnabled = setting;
}

//----------------------------------------------------------------------------//
void Window::setCursorAutoRepeatEnabled(bool setting)
{
    if (d_autoRepeat == setting)
        return;

    d_autoRepeat = setting;
    d_repeatPointerSource = CursorInputSource::NotSpecified;

    // FIXME: There is a potential issue here if this setting is
    // FIXME: changed _while_ the cursor is auto-repeating, and
    // FIXME: the 'captured' state of input could get messed up.
    // FIXME: The alternative is to always release here, but that
    // FIXME: has a load of side effects too - so for now nothing
    // FIXME: is done. This whole aspect of the system needs a
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

    // update underlying RenderingWindow if needed
    if (d_surface && d_surface->isRenderingWindow())
        static_cast<RenderingWindow*>(d_surface)->update(elapsed);

    UpdateEventArgs e(this,elapsed);
    fireEvent(EventUpdated,e,EventNamespace);

    // update child windows
    for (size_t i = 0; i < getChildCount(); ++i)
    {
        // update children based on their WindowUpdateMode setting.
        if (getChildAtIndex(i)->d_updateMode == WindowUpdateMode::Always ||
                (getChildAtIndex(i)->d_updateMode == WindowUpdateMode::Visible &&
                 getChildAtIndex(i)->isVisible()))
        {
            getChildAtIndex(i)->update(elapsed);
        }
    }
}

//----------------------------------------------------------------------------//
void Window::updateSelf(float elapsed)
{
    // cursor autorepeat processing.
    if (d_autoRepeat && d_repeatPointerSource != CursorInputSource::NotSpecified)
    {
        d_repeatElapsed += elapsed;

        if (d_repeating)
        {
            if (d_repeatElapsed > d_repeatRate)
            {
                d_repeatElapsed -= d_repeatRate;
                // trigger the repeated event
                generateAutoRepeatEvent(d_repeatPointerSource);
            }
        }
        else
        {
            if (d_repeatElapsed > d_repeatDelay)
            {
                d_repeatElapsed = 0;
                d_repeating = true;
                // trigger the repeated event
                generateAutoRepeatEvent(d_repeatPointerSource);
            }
        }
    }

    // allow for updates within an assigned WindowRenderer
    if (d_windowRenderer)
        d_windowRenderer->update(elapsed);
}

//----------------------------------------------------------------------------//
bool Window::performCopy(Clipboard& /*clipboard*/)
{
    // deny copying by default
    return false;
}

//----------------------------------------------------------------------------//
bool Window::performCut(Clipboard& /*clipboard*/)
{
    // deny cutting by default
    return false;
}

//----------------------------------------------------------------------------//
bool Window::performPaste(Clipboard& /*clipboard*/)
{
    // deny pasting by default
    return false;
}

//----------------------------------------------------------------------------//
bool Window::performUndo()
{
    // deny undo by default
    return false;
}

//----------------------------------------------------------------------------//
bool Window::performRedo()
{
    // deny redo by default
    return false;
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
void Window::destroy()
{
    // because we know that people do not read the API ref properly,
    // here is some protection to ensure that WindowManager does the
    // destruction and not anyone else.
    WindowManager& wmgr = WindowManager::getSingleton();

    if (wmgr.isAlive(this))
    {
        wmgr.destroyWindow(this);

        // now return, the rest of what we need to do will happen
        // once WindowManager re-calls this method.
        return;
    }

    // signal our imminent destruction
    WindowEventArgs args(this);
    onDestructionStarted(args);

    // Check we are detached from parent
    if (d_parent)
        d_parent->removeChild(this);

    releaseInput();

    // let go of the tooltip if we have it
    Tooltip* const tip = getTooltip();
    if (tip && tip->getTargetWindow()==this)
        tip->setTargetWindow(nullptr);

    // ensure custom tooltip is cleaned up
    setTooltip(nullptr);

    // clean up looknfeel related things
    if (!d_lookName.empty())
    {
        d_windowRenderer->onLookNFeelUnassigned();
        WidgetLookManager::getSingleton().getWidgetLook(d_lookName).
            cleanUpWidget(*this);
    }

    // free any assigned WindowRenderer
    if (d_windowRenderer)
    {
        d_windowRenderer->onDetach();
        WindowRendererManager::getSingleton().
            destroyWindowRenderer(d_windowRenderer);
        d_windowRenderer = nullptr;
    }

    cleanupChildren();

    releaseRenderingWindow();
    invalidate();
}

//----------------------------------------------------------------------------//
Tooltip* Window::getTooltip() const
{
    if (!isUsingDefaultTooltip())
        return d_customTip;

    GUIContext* context = getGUIContextPtr();
    return context ? context->getDefaultTooltipObject(): nullptr;
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
        d_customTip = nullptr;
        d_weOwnTip = false;
    }
    else
    {
        try
        {
            d_customTip = static_cast<Tooltip*>(
                WindowManager::getSingleton().createWindow(
                    tooltipType, getName() + TooltipNameSuffix));
            d_customTip->setAutoWindow(true);
            d_weOwnTip = true;
        }
        catch (UnknownObjectException&)
        {
            d_customTip = nullptr;
            d_weOwnTip = false;
        }
    }
}

//----------------------------------------------------------------------------//
String Window::getTooltipType() const
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
const String& Window::getTooltipTextIncludingInheritance() const
{
    if (d_inheritsTipText && d_parent && d_tooltipText.empty())
        return getParent()->getTooltipText();
    else
        return d_tooltipText;
}

//----------------------------------------------------------------------------//
void Window::setInheritsTooltipText(bool setting)
{
    d_inheritsTipText = setting;
}

//----------------------------------------------------------------------------//
void Window::setLookNFeel(const String& look)
{
    if (d_lookName == look)
        return;

    if (!d_windowRenderer)
        throw NullObjectException("There must be a "
            "window renderer assigned to the window '" + d_name +
            "' to set its look'n'feel");

    WidgetLookManager& wlMgr = WidgetLookManager::getSingleton();
    if (!d_lookName.empty())
    {
        d_windowRenderer->onLookNFeelUnassigned();
        wlMgr.getWidgetLook(d_lookName).cleanUpWidget(*this);
    }

    d_lookName = look;
    Logger::getSingleton().logEvent("Assigning LookNFeel '" + look +
        "' to window '" + d_name + "'.", LoggingLevel::Informative);

    // Get look and feel to initialise the widget as it needs.
    // Set init flag to prevent premature child layouting by LNF.
    const bool prevInit = d_initialising;
    d_initialising = true;
    wlMgr.getWidgetLook(look).initialiseWidget(*this);
    d_initialising = prevInit;

    // do the necessary binding to the stuff added by the look and feel
    initialiseComponents();
    // let the window renderer know about this
    d_windowRenderer->onLookNFeelAssigned();

    performChildLayout(false, false);
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
        getGUIContext().setModalWindow(this);
    }
    // clear the modal target
    else
        getGUIContext().setModalWindow(nullptr);
}

//----------------------------------------------------------------------------//
const String& Window::getUserString(const String& name) const
{
    auto iter = d_userStrings.find(name);

    if (iter == d_userStrings.end())
        throw UnknownObjectException(
            "a user string named '" + name + "' is not defined for Window '" +
            d_name + "'.");

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
    xml_stream.openTag(WindowXMLElementName)
        .attribute(WindowTypeXMLAttributeName, getType());
    // write name if not auto-generated
    if (getName().compare(0, WindowManager::GeneratedWindowNameBase.length(),
                          WindowManager::GeneratedWindowNameBase) != 0)
    {
        xml_stream.attribute(WindowNameXMLAttributeName, getName());
    }
    // write out properties.
    writePropertiesXML(xml_stream);
    // user strings
    const String UserStringXMLElementName("UserString");
    for (const auto& pair : d_userStrings)
    {
        const String& name = pair.first;
        // ignore auto props
        if (name.rfind("_auto_prop__") != CEGUI::String::npos)
            continue;

        xml_stream.openTag(UserStringXMLElementName)
            .attribute(Property::NameXMLAttributeName, name);
        // Detect whether it is a long property or not
        const String& value = pair.second;
        if (value.find(static_cast<String::value_type>('\n')) != String::npos)
            xml_stream.text(value);
        else
            xml_stream.attribute(Property::ValueXMLAttributeName, value);
        xml_stream.closeTag();
    }
    // write out attached child windows.
    writeChildWindowsXML(xml_stream);
    // now ouput closing Window tag
    xml_stream.closeTag();
}

//----------------------------------------------------------------------------//
int Window::writePropertiesXML(XMLSerializer& xml_stream) const
{
    int propertiesWritten = 0;
    PropertySet::PropertyIterator iter =  getPropertyIterator();

    while(!iter.isAtEnd())
    {
        // first we check to make sure the property is'nt banned from XML
        if (!isPropertyBannedFromXML(iter.getCurrentValue()))
        {
            try
            {
                // only write property if it's not at the default state
                if (!isPropertyAtDefault(iter.getCurrentValue()))
                {
                    iter.getCurrentValue()->writeXMLToStream(this, xml_stream);
                    ++propertiesWritten;
                }
            }
            catch (InvalidRequestException&)
            {
                // This catches errors from the MultiLineColumnList for example
                Logger::getSingleton().logEvent(
                    "Window::writePropertiesXML: property receiving failed.  "
                    "Continuing...", LoggingLevel::Error);
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

    for (unsigned int i = 0; i < getChildCount(); ++i)
    {
        const Window* const child = getChildAtIndex(i);

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
    // just stop now if we aren't allowed to write XML
    if (!d_allowWriteXML)
        return false;

    // we temporarily output to this string stream to see if have to do the tag
    // at all.  Make sure this stream does UTF-8
    std::ostringstream ss;
    XMLSerializer xml(ss);
    xml.openTag(AutoWindowXMLElementName);
    // Create the XML Child Tree
    // write out properties.
    writePropertiesXML(xml);
    // write out attached child windows.
    writeChildWindowsXML(xml);
    xml.closeTag();
    if (xml.getTagCount() <= 1)
        return false;

    // output opening AutoWindow tag
    xml_stream.openTag(AutoWindowXMLElementName);
    // write name suffix attribute
    xml_stream.attribute(AutoWindowNamePathXMLAttributeName, getName());
    // Inefficient : do the XML serialization again
    // write out properties.
    writePropertiesXML(xml_stream);
    // write out attached child windows.
    writeChildWindowsXML(xml_stream);
    xml_stream.closeTag();

    return true;
}

//----------------------------------------------------------------------------//
Window* Window::getChildAutoWindow(const String& name)
{
    return getChild(name);
}

//----------------------------------------------------------------------------//
void Window::addWindowToDrawList(Window& wnd, bool at_back)
{
    // add behind other windows in same group
    if (at_back)
    {
        // calculate position where window should be added for drawing
        auto pos = d_drawList.begin();
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
        auto position = d_drawList.rbegin();
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
        const auto position =
            std::find(d_drawList.begin(), d_drawList.end(), &wnd);

        // remove the window if it was found in the draw list
        if (position != d_drawList.end())
            d_drawList.erase(position);
    }
}

//----------------------------------------------------------------------------//
Window* Window::getActiveSibling()
{
    Window* activeWnd = isActive() ? this : nullptr;

    // if active window not already known, and we have a parent window
    if (!activeWnd && d_parent)
    {
        // scan backwards through the draw list, as this will
        // usually result in the fastest result.
        for (auto it = getParent()->d_drawList.rbegin(); it != getParent()->d_drawList.rend(); ++it)
        {
            Window* wnd = *it;
            // if this child is active
            if (wnd->isActive())
            {
                // set the return value
                activeWnd = wnd;
                // exit loop early, as we have found what we needed
                break;
            }
        }
    }

    // return whatever we discovered
    return activeWnd;
}

//----------------------------------------------------------------------------//
void Window::onMoved(ElementEventArgs& e)
{
    Element::onMoved(e);

    // handle invalidation of surfaces and trigger needed redraws
    if (d_parent)
    {
        getParent()->invalidateRenderingSurface();
        // need to redraw some geometry if parent uses a caching surface
        if (auto ctx = getGUIContextPtr())
        {
            CEGUI::RenderingSurface* rs = getParent()->getTargetRenderingSurface();
            if (rs && rs->isRenderingWindow())
                ctx->markAsDirty();
        }
    }
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
    // properly. A better, more selective, solution would
    // probably be to do something funky with events ;)
    if (!d_initialising)
        performChildLayout(false, false);

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
        if (getChildAtIndex(i)->inheritsAlpha())
        {
            WindowEventArgs args(getChildAtIndex(i));
            getChildAtIndex(i)->onAlphaChanged(args);
        }

    }

    const float finalAlpha = getEffectiveAlpha();
    for (CEGUI::GeometryBuffer* currentBuffer : d_geometryBuffers)
        currentBuffer->setAlpha(finalAlpha);

    invalidateRenderingSurface();

    if (GUIContext* context = getGUIContextPtr())
        context->markAsDirty();

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
        if (getChildAtIndex(i)->d_enabled)
        {
            WindowEventArgs args(getChildAtIndex(i));
            getChildAtIndex(i)->onEnabled(args);
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
        if (getChildAtIndex(i)->d_enabled)
        {
            WindowEventArgs args(getChildAtIndex(i));
            getChildAtIndex(i)->onDisabled(args);
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
    GUIContext* context = getGUIContextPtr();
    if (context)
        context->markAsDirty();
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
    d_repeatPointerSource = CursorInputSource::NotSpecified;

    // handle restore of previous capture window as required.
    if (d_restoreOldCapture && d_oldCapture)
    {
        d_oldCapture->onCaptureGained(e);
        d_oldCapture = nullptr;
    }

    // handle case where cursor is now in a different window
    // (this is a bit of a hack that uses the injection of a semantic event to handle
    // this for us).
    SemanticInputEvent moveEvent(SemanticValue::CursorMove);
    const glm::vec2 cursorPosition = getGUIContext().getCursor().getPosition();
    moveEvent.d_payload.array[0] = cursorPosition.x;
    moveEvent.d_payload.array[1] = cursorPosition.y;
    getGUIContext().injectInputEvent(moveEvent);

    fireEvent(EventInputCaptureLost, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onInvalidated(WindowEventArgs& e)
{
    fireEvent(EventInvalidated, e, EventNamespace);
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

    GUIContext* context = getGUIContextPtr();
    if (context)
        context->markAsDirty();
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

    // activate all ancestors
    auto ancestor = getParent();
    while (ancestor)
    {
        ActivationEventArgs parent_e(ancestor);
        parent_e.otherWindow = e.otherWindow;
        ancestor->onActivated(parent_e);

        ancestor = ancestor->getParent();
    }

    fireEvent(EventActivated, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onDeactivated(ActivationEventArgs& e)
{
    // first de-activate all children
    for (Element* child : d_children)
    {
        auto childWnd = static_cast<Window*>(child);
        if (childWnd->isActive())
        {
            ActivationEventArgs child_e(childWnd);
            child_e.otherWindow = e.otherWindow;
            childWnd->onDeactivated(child_e);
        }
    }

    d_active = false;
    invalidate();
    fireEvent(EventDeactivated, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onChildAdded(ElementEventArgs& e)
{
    // we no longer want a total redraw here, instead we just get each window
    // to resubmit it's imagery to the Renderer.
    GUIContext* context = getGUIContextPtr();
    if (context)
        context->markAsDirty();

    Element::onChildAdded(e);
}

//----------------------------------------------------------------------------//
void Window::onChildRemoved(ElementEventArgs& e)
{
    // we no longer want a total redraw here, instead we just get each window
    // to resubmit it's imagery to the Renderer.
    if (auto context = getGUIContextPtr())
        context->markAsDirty();

    // Though we do need to invalidate the rendering surface!
    if (auto rs = getTargetRenderingSurface())
        rs->invalidate();

    Element::onChildRemoved(e);
}

//----------------------------------------------------------------------------//
void Window::onCursorEntersArea(CursorInputEventArgs& e)
{
    d_containsPointer = true;
    fireEvent(EventCursorEntersArea, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onCursorLeavesArea(CursorInputEventArgs& e)
{
    d_containsPointer = false;
    fireEvent(EventCursorLeavesArea, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onCursorEnters(CursorInputEventArgs& e)
{
    // set the cursor
    if (GUIContext* context = getGUIContextPtr())
        context->getCursor().setImage(getActualCursor());

    // perform tooltip control
    Tooltip* const tip = getTooltip();
    if (tip && !isAncestor(tip))
        tip->setTargetWindow(this);

    fireEvent(EventCursorEntersSurface, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onCursorLeaves(CursorInputEventArgs& e)
{
    // perform tooltip control
    GUIContext* context = getGUIContextPtr();
    const Window* const mw = context ? context->getWindowContainingCursor() : nullptr;
    Tooltip* const tip = getTooltip();
    if (tip && mw != tip && !(mw && mw->isAncestor(tip)))
        tip->setTargetWindow(nullptr);

    fireEvent(EventCursorLeavesSurface, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onCursorMove(CursorInputEventArgs& e)
{
    // perform tooltip control
    Tooltip* const tip = getTooltip();
    if (tip)
        tip->resetTimer();

    fireEvent(EventCursorMove, e, EventNamespace);

    // optionally propagate to parent
    if (!e.handled && d_propagatePointerInputs &&
        d_parent && this != getGUIContext().getModalWindow())
    {
        e.window = getParent();
        getParent()->onCursorMove(e);

        return;
    }

    // by default we now mark cursor events as handled
    // (derived classes may override, of course!)
    ++e.handled;
}

//----------------------------------------------------------------------------//
void Window::onScroll(CursorInputEventArgs& e)
{
    fireEvent(EventScroll, e, EventNamespace);

    // optionally propagate to parent
    if (!e.handled && d_propagatePointerInputs &&
        d_parent && this != getGUIContext().getModalWindow())
    {
        e.window = getParent();
        getParent()->onScroll(e);

        return;
    }

    // by default we now mark cursor events as handled
    // (derived classes may override, of course!)
    ++e.handled;
}

//----------------------------------------------------------------------------//
void Window::onCursorPressHold(CursorInputEventArgs& e)
{
    // perform tooltip control
    if (auto tip = getTooltip())
        tip->setTargetWindow(nullptr);

    if ((e.source == CursorInputSource::Left) && activate_impl(true))
        ++e.handled;

    // if auto repeat is enabled and we are not currently tracking
    // the source that was just pushed (need this source check because
    // it could be us that generated this event via auto-repeat).
    if (d_autoRepeat)
    {
        if (d_repeatPointerSource == CursorInputSource::NotSpecified)
            captureInput();

        if ((d_repeatPointerSource != e.source) && isCapturedByThis())
        {
            d_repeatPointerSource = e.source;
            d_repeatElapsed = 0.f;
            d_repeating = false;
        }
    }

    fireEvent(EventCursorPressHold, e, EventNamespace);

    // optionally propagate to parent
    if (!e.handled && d_propagatePointerInputs &&
        d_parent && this != getGUIContext().getModalWindow())
    {
        e.window = getParent();
        getParent()->onCursorPressHold(e);

        return;
    }

    // by default we now mark cursor events as handled
    // (derived classes may override, of course!)
    ++e.handled;
}

//----------------------------------------------------------------------------//
void Window::onSelectWord(CursorInputEventArgs& e)
{
    fireEvent(EventSelectWord, e, EventNamespace);

    // optionally propagate to parent
    if (!e.handled && d_propagatePointerInputs &&
        d_parent && this != getGUIContext().getModalWindow())
    {
        e.window = getParent();
        getParent()->onSelectWord(e);

        return;
    }

    // by default we now mark cursor events as handled
    // (derived classes may override, of course!)
    ++e.handled;
}

//----------------------------------------------------------------------------//
void Window::onSelectAll(CursorInputEventArgs& e)
{
    fireEvent(EventSelectAll, e, EventNamespace);

    // optionally propagate to parent
    if (!e.handled && d_propagatePointerInputs &&
        d_parent && this != getGUIContext().getModalWindow())
    {
        e.window = getParent();
        getParent()->onSelectAll(e);

        return;
    }

    // by default we now mark cursor events as handled
    // (derived classes may override, of course!)
    ++e.handled;
}

//----------------------------------------------------------------------------//
void Window::onCursorActivate(CursorInputEventArgs& e)
{
    // onCursorPressHold() hides the tooltip, restore it here
    Tooltip* const tip = getTooltip();
    if (tip && !isAncestor(tip))
        tip->setTargetWindow(this);

    // reset auto-repeat state
    if (d_autoRepeat && d_repeatPointerSource != CursorInputSource::NotSpecified)
    {
        releaseInput();
        d_repeatPointerSource = CursorInputSource::NotSpecified;
    }

    fireEvent(EventCursorActivate, e, EventNamespace);

    // optionally propagate to parent
    if (!e.handled && d_propagatePointerInputs &&
        d_parent && this != getGUIContext().getModalWindow())
    {
        e.window = getParent();
        getParent()->onCursorActivate(e);
    }
}

//----------------------------------------------------------------------------//
void Window::onCharacter(TextEventArgs& e)
{
    fireEvent(EventCharacterKey, e, EventNamespace);

    // As of 0.7.0 CEGUI::System no longer does input event propogation, so by
    // default we now do that here.  Generally speaking key handling widgets
    // may need to override this behaviour to halt further propagation.
    if (!e.handled && d_parent &&
        this != getGUIContext().getModalWindow())
    {
        e.window = getParent();
        getParent()->onCharacter(e);
    }
}

//----------------------------------------------------------------------------//
void Window::onSemanticInputEvent(SemanticEventArgs& e)
{
    fireEvent(EventSemanticEvent, e, EventNamespace);

    // optionally propagate to parent
    if (!e.handled && d_parent && this != getGUIContext().getModalWindow())
    {
        e.window = getParent();
        getParent()->onSemanticInputEvent(e);
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
void Window::setWindowRenderer(const String& name)
{
    if (d_windowRenderer && d_windowRenderer->getName() == name)
        return;

    WindowRendererManager& wrm = WindowRendererManager::getSingleton();
    if (d_windowRenderer != nullptr)
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
            name + "' to the window '" + d_name + "'", LoggingLevel::Informative);
        d_windowRenderer = wrm.createWindowRenderer(name);
        WindowEventArgs e(this);
        onWindowRendererAttached(e);
    }
    else
        throw InvalidRequestException(
            "Attempt to assign a 'null' window renderer to window '" +
            d_name + "'.");
}

//----------------------------------------------------------------------------//
void Window::onWindowRendererAttached(WindowEventArgs& e)
{
    if (!validateWindowRenderer(d_windowRenderer))
        throw InvalidRequestException(
            "The window renderer '" + d_windowRenderer->getName() + "' is not "
            "compatible with this widget type (" + getType() + ")");

    d_windowRenderer->d_window = this;
    d_windowRenderer->onAttach();
    fireEvent(EventWindowRendererAttached, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Window::onWindowRendererDetached(WindowEventArgs& e)
{
    d_windowRenderer->onDetach();
    d_windowRenderer->d_window = nullptr;
    fireEvent(EventWindowRendererDetached, e, EventNamespace);
}

//----------------------------------------------------------------------------//
bool Window::validateWindowRenderer(const WindowRenderer*) const
{
    return true;
}

//----------------------------------------------------------------------------//
const String& Window::getWindowRendererName() const
{
    if (d_windowRenderer)
        return d_windowRenderer->getName();

    static String empty("");
    return empty;
}


//----------------------------------------------------------------------------//
void Window::banPropertyFromXML(const String& property_name)
{
    Property* instance = getPropertyInstance(property_name);
    if (!instance->isWritable())
    {
        Logger::getSingleton().logEvent("Property '" + property_name + "' "
                "is not writable so it's implicitly banned from XML. No need "
                "to ban it manually", LoggingLevel::Warning);

        return;
    }

    // check if the insertion failed
    if (!d_bannedXMLProperties.insert(property_name).second)
        // just log the incidence
        CEGUI_LOGINSANE("Window::banPropertyFromXML: The property '" +
            property_name + "' is already banned in window '" + d_name + "'");
}

//----------------------------------------------------------------------------//
void Window::banPropertyFromXMLRecursive(const String& property_name)
{
    banPropertyFromXML(property_name);

    const size_t childCount = getChildCount();
    for(size_t i = 0; i < childCount; ++i)
    {
        getChildAtIndex(i)->banPropertyFromXMLRecursive(property_name);
    }
}

//----------------------------------------------------------------------------//
void Window::unbanPropertyFromXML(const String& property_name)
{
    d_bannedXMLProperties.erase(property_name);
}

//----------------------------------------------------------------------------//
void Window::unbanPropertyFromXMLRecursive(const String& property_name)
{
    unbanPropertyFromXML(property_name);

    const size_t childCount = getChildCount();
    for(size_t i = 0; i < childCount; ++i)
    {
        getChildAtIndex(i)->unbanPropertyFromXMLRecursive(property_name);
    }
}

//----------------------------------------------------------------------------//
bool Window::isPropertyBannedFromXML(const String& property_name) const
{
    const auto it = d_bannedXMLProperties.find(property_name);

    // generally, there will always less banned properties than all properties,
    // so it makes sense to check that first before querying the property instance
    if (it != d_bannedXMLProperties.end())
    {
        return true;
    }

    // properties that don't write any XML code are implicitly banned

    // read-only properties are implicitly banned
    // (such stored information wouldn't be of any value in the XML anyways,
    // no way to apply it to the widget)
    Property* instance = getPropertyInstance(property_name);
    return (!instance->doesWriteXML() || !instance->isWritable());
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
        if (d_parent && !getParent()->getLookNFeel().empty())
        {
            const WidgetLookFeel& wlf =
                WidgetLookManager::getSingleton().
                    getWidgetLook(getParent()->getLookNFeel());

            // If this property is a target of a PropertyLink, we always report it as being at default.
            WidgetLookFeel::StringSet propLinkDefNames = wlf.getPropertyLinkDefinitionNames(true);
            if(propLinkDefNames.find(property->getName()) != propLinkDefNames.end())
                return true;

            // for an auto-window see if the property is is set via a Property
            // tag within the WidgetComponent that defines it.
            if (d_autoWindow)
            {
                // Check if a widget component with the name was added to the look
                if(wlf.isWidgetComponentPresent(getName()))
                {
                    const WidgetComponent& widgetComp = wlf.getWidgetComponent(getName());

                    const PropertyInitialiser* const propInitialiser = widgetComp.findPropertyInitialiser(property->getName());

                    if (propInitialiser)
                        return (getProperty(property->getName()) == propInitialiser->getInitialiserValue());
                }
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

    // we don't have a looknfeel with a new value for this property so we rely
    // on the hardcoded default
    return property->isDefault(this);
}

//----------------------------------------------------------------------------//
void Window::notifyClippingChanged()
{
    d_outerRectClipperValid = false;
    d_innerRectClipperValid = false;
    d_hitTestRectValid = false;

    // inform children that their clipped screen areas must be updated
    for (Element* child : d_children)
        if (static_cast<Window*>(child)->isClippedByParent())
            static_cast<Window*>(child)->notifyClippingChanged();
}

//----------------------------------------------------------------------------//
void Window::notifyDefaultFontChanged()
{
    if (!d_font)
    {
        d_renderedStringValid = false;
        WindowEventArgs args(this);
        onFontChanged(args);
    }

    for (Element* child : d_children)
        static_cast<Window*>(child)->notifyDefaultFontChanged();
}

//----------------------------------------------------------------------------//
uint8_t Window::handleAreaChanges(bool moved, bool sized)
{
    // NB: we don't call Element::handleAreaChanges because we completely override its behaviour

    if (moved || sized)
        d_unclippedInnerRect.invalidateCache();

    // Either our position, size or parent clip rects changed.
    // In any case we need to recalculate our clipping rects.
    d_outerRectClipperValid = false;
    d_innerRectClipperValid = false;

    // Always invalidate hit rect because we can't guess how it is calculated
    d_hitTestRectValid = false;
    if (GUIContext* context = getGUIContextPtr())
        context->updateWindowContainingCursor();

    // Check base area changes to update our children
    uint8_t flags = 0;
    if (!d_children.empty())
    {
        // When check move, we can occasionally update an inner rect.
        // To check sizing we must remember its old size here.
        const Sizef innerRectOldSize = d_unclippedInnerRect.getCurrent().getSize();

        if (moved)
        {
            // If we moved, consider all our children moved too
            flags |= (NonClientMoved | ClientMoved | NonClientClippingChanged | ClientClippingChanged);
        }
        else
        {
            // NB: if some of children use an outer rect, 'moved' flag already took care of them

            const auto& clientArea = getChildContentArea(false);
            if (&d_unclippedOuterRect != &clientArea)
            {
                const Rectf oldClientContent = clientArea.getCurrent();
                clientArea.invalidateCache();
                if (oldClientContent != clientArea.get())
                    flags |= ClientMoved;
            }

            const auto& nonClientArea = getChildContentArea(true);
            if (&d_unclippedOuterRect != &nonClientArea)
            {
                if (&clientArea == &nonClientArea)
                {
                    // All children use the same area, reuse comparison results
                    if (flags & ClientMoved)
                        flags |= NonClientMoved;
                }
                else
                {
                    // Check a non-client area if it is not the same as client
                    const Rectf oldNonClientContent = nonClientArea.getCurrent();
                    nonClientArea.invalidateCache();
                    if (oldNonClientContent != nonClientArea.get())
                        flags |= NonClientMoved;
                }
            }
        }

        if (sized)
        {
            // If we were resized, consider all our children are resized too
            flags |= (NonClientSized | ClientSized | NonClientClippingChanged | ClientClippingChanged);
        }
        else
        {
            // Non-client size depends on the outer rect, and 'sized' flags handles it.
            // Client size depends on the inner rect, and we check its resizing here.
            if (innerRectOldSize != d_unclippedInnerRect.get().getSize())
                flags |= ClientSized;
        }

        // Even if our area didn't change our clippers could still be changed
        // due to changes in the parent clipper
        if (!moved && !sized)
        {
            const Rectf oldOuterClipper = d_outerRectClipper;
            if (oldOuterClipper != getOuterRectClipper())
                flags |= NonClientClippingChanged;

            const Rectf oldInnerClipper = d_innerRectClipper;
            if (oldInnerClipper != getInnerRectClipper())
                flags |= ClientClippingChanged;
        }
    }

    if (sized)
    {
        // Resize the underlying RenderingWindow if we're using such a thing
        if (d_surface && d_surface->isRenderingWindow())
            static_cast<RenderingWindow*>(d_surface)->setSize(d_pixelSize);

        invalidate();
    }

    // Apply our screen area changes to rendering surface and geometry settings
    {
        RenderingContext ctx;
        getRenderingContext(ctx);

        const auto& pos = getUnclippedOuterRect().get().getPosition();

        if (ctx.owner == this && ctx.surface->isRenderingWindow())
        {
            RenderingWindow* const rw = static_cast<RenderingWindow*>(ctx.surface);

            // move the underlying RenderingWindow if we're using such a thing
            rw->setPosition(pos);
            updatePivot();
            d_translation = glm::vec3(0.0f, 0.0f, 0.0f);

            rw->setClippingRegion(getParentClipRect());

            d_clippingRegion = Rectf(glm::vec2(0, 0), d_pixelSize);
        }
        else
        {
            // if we're not texture backed, update geometry position.
            // position is the offset of the window on the dest surface.
            d_translation = glm::vec3(pos - ctx.offset, 0.0f);

            d_clippingRegion = getOuterRectClipper();
            if (d_clippingRegion.getWidth() != 0.0f && d_clippingRegion.getHeight() != 0.0f)
                d_clippingRegion.offset(-ctx.offset);
        }

        for (CEGUI::GeometryBuffer* currentBuffer : d_geometryBuffers)
        {
            currentBuffer->setTranslation(d_translation);
            currentBuffer->setClippingRegion(d_clippingRegion);
        }
    }

    return flags;
}

//----------------------------------------------------------------------------//
void Window::performChildLayout(bool client, bool nonClient)
{
    // Will call it once after init in endInitialisation() / setLookNFeel()
    if (d_initialising)
        return;

    bool changed = false;

    // Layout child widgets with LNF
    if (!d_lookName.empty())
    {
        try
        {
            changed |= WidgetLookManager::getSingleton().getWidgetLook(d_lookName).layoutChildWidgets(*this);
        }
        catch (UnknownObjectException&)
        {
            Logger::getSingleton().logEvent(
                "Window::performChildLayout: "
                "WidgetLook '" + d_lookName + "' was not found.", LoggingLevel::Error);
        }
    }

    // Layout child widgets with a window renderer
    if (d_windowRenderer)
        changed |= d_windowRenderer->performChildWindowLayout();

    // Inner rect may depend on children (e.g. Titlebar), so recalculate if their areas changed
    if (changed)
    {
        d_unclippedInnerRect.invalidateCache();
        d_innerRectClipperValid = false;
        d_hitTestRectValid = false;

        // Relayout client children if an inner rect size has changed
        if (!client)
        {
            const Sizef innerRectOldSize = d_unclippedInnerRect.getCurrent().getSize();
            if (innerRectOldSize != d_unclippedInnerRect.get().getSize())
                client = true;
        }
    }

    // Layout child widgets normally
    Element::performChildLayout(client, nonClient);
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
    auto pos = getParent()->d_drawList.rbegin();
    if (!d_alwaysOnTop)
    {
        // find last non-topmost window
        while ((pos != getParent()->d_drawList.rend()) && (*pos)->isAlwaysOnTop())
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

#ifdef CEGUI_BIDI_SUPPORT
    d_bidiDataValid = false;
#endif

    WindowEventArgs args(this);
    onTextChanged(args);
}

//----------------------------------------------------------------------------//
void Window::appendText(const String& text)
{
    d_textLogical.append(text);
    d_renderedStringValid = false;

#ifdef CEGUI_BIDI_SUPPORT
    d_bidiDataValid = false;
#endif

    WindowEventArgs args(this);
    onTextChanged(args);
}

//----------------------------------------------------------------------------//
void Window::appendGeometryBuffers(std::vector<GeometryBuffer*>& geomBuffers)
{
    d_geometryBuffers.insert(d_geometryBuffers.end(), geomBuffers.begin(),
        geomBuffers.end());
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
        ctx.offset = getUnclippedOuterRect().get().getPosition();
        ctx.queue = RenderQueueID::Base;
    }
    else if (d_parent)
    {
        getParent()->getRenderingContext(ctx);
    }
    else
    {
        ctx.surface = d_guiContext;
        ctx.owner = nullptr;
        ctx.offset = glm::vec2(0, 0);
        ctx.queue = RenderQueueID::Base;
    }
}

//----------------------------------------------------------------------------//
RenderingSurface* Window::getTargetRenderingSurface() const
{
    if (d_surface)
        return d_surface;
    else if (d_parent)
        return getParent()->getTargetRenderingSurface();
    else
        return d_guiContext;
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
        notifyScreenAreaChanged(true, true); //???or only update geometry if size not changed?
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
        getParent()->invalidateRenderingSurface();
}

//----------------------------------------------------------------------------//
const Window* Window::getRootWindow() const
{
    return d_parent ? getParent()->getRootWindow() : this;
}

//----------------------------------------------------------------------------//
Window* Window::getRootWindow()
{
    return const_cast<Window*>(
        static_cast<const Window*>(this)->getRootWindow());
}

//----------------------------------------------------------------------------//
void Window::setUsingAutoRenderingSurface(bool setting)
{
    if (setting)
    {
        allocateRenderingWindow(d_autoRenderingSurfaceStencilEnabled);
    }
    else
    {
        releaseRenderingWindow();
        d_autoRenderingWindow = false;
    }

    // while the actual area on screen may not have changed, the arrangement of
    // surfaces and geometry did...
    notifyScreenAreaChanged(true, true); //???or only update geometry if size not changed?
}

//----------------------------------------------------------------------------//
void Window::setAutoRenderingSurfaceStencilEnabled(bool setting)
{
    if (d_autoRenderingSurfaceStencilEnabled == setting)
        return;

    d_autoRenderingSurfaceStencilEnabled = setting;

    if (!d_autoRenderingWindow || !d_surface)
        return;

    // We need to recreate the auto rendering window since we just changed a crucial setting for it
    releaseRenderingWindow();
    allocateRenderingWindow(setting);
}

//----------------------------------------------------------------------------//
void Window::allocateRenderingWindow(bool addStencilBuffer)
{
    if (d_autoRenderingWindow && d_surface)
        return;

    d_autoRenderingWindow = true;

    CEGUI::RenderingSurface* rs = getTargetRenderingSurface();
    if (!rs)
    {
        d_surface = nullptr;
        return;
    }

    TextureTarget* const t =
        System::getSingleton().getRenderer()->createTextureTarget(addStencilBuffer);

    // TextureTargets may not be available, so check that first.
    if (!t)
    {
        Logger::getSingleton().logEvent("Window::allocateRenderingWindow - "
            "Failed to create a suitable TextureTarget for use by Window '"
            + d_name + "'", LoggingLevel::Error);

        d_surface = nullptr;
        return;
    }

    d_surface = &rs->createRenderingWindow(*t);
    transferChildSurfaces();

    // set size and position of RenderingWindow
    auto rw = static_cast<RenderingWindow*>(d_surface);
    rw->setSize(getPixelSize());
    rw->setPosition(getUnclippedOuterRect().get().getPosition());
    rw->setClippingRegion(getParentClipRect());
    rw->setRotation(d_rotation);
    updatePivot();

    if (GUIContext* context = getGUIContextPtr())
        context->markAsDirty();
}

//----------------------------------------------------------------------------//
void Window::releaseRenderingWindow()
{
    if (!d_autoRenderingWindow || !d_surface)
        return;

    RenderingWindow* const old_surface =
        static_cast<RenderingWindow*>(d_surface);
    d_surface = nullptr;
    // detach child surfaces prior to destroying the owning surface
    transferChildSurfaces();
    // destroy surface and texture target it used
    TextureTarget* tt = &old_surface->getTextureTarget();
    old_surface->getOwner().destroyRenderingWindow(*old_surface);
    System::getSingleton().getRenderer()->destroyTextureTarget(tt);

    if (GUIContext* context = getGUIContextPtr())
        context->markAsDirty();
}

//----------------------------------------------------------------------------//
void Window::transferChildSurfaces()
{
    RenderingSurface* rs = getTargetRenderingSurface();
    if (!rs)
        return;

    for (auto child : d_children)
    {
        Window* childWnd = static_cast<Window*>(child);

        if (childWnd->d_surface && childWnd->d_surface->isRenderingWindow())
            rs->transferRenderingWindow(
                *static_cast<RenderingWindow*>(childWnd->d_surface));
        else
            childWnd->transferChildSurfaces();
    }
}

//----------------------------------------------------------------------------//
void Window::onRotated(ElementEventArgs& e)
{
    Element::onRotated(e);
    
    if(!getGUIContextPtr())
    {
        return;
    }

    // TODO: Checking quaternion for equality with IDENTITY is stupid,
    //       change this to something else, checking with tolerance.
    if (d_rotation != glm::quat() && !d_surface)
    {
        // if we have no surface set and the rotation differs from identity,
        // enable the auto surface

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
                LoggingLevel::Error);

            return;
        }
    }

    if (d_surface)
    {
        // ensure surface we have is the right type
        if (!d_surface->isRenderingWindow())
        {
            Logger::getSingleton().logEvent("Window::setRotation - "
                "Window '" + d_name + "' has a manual RenderingSurface that is not "
                "a RenderingWindow.  Rotation will not be available.", LoggingLevel::Error);

            return;
        }

        // Checks / setup complete!  Now we can finally set the rotation.
        static_cast<RenderingWindow*>(d_surface)->setRotation(d_rotation);
        updatePivot();
    }
}

//----------------------------------------------------------------------------//
const RenderedString& Window::getRenderedString() const
{
    if (!d_renderedStringValid)
    {
        d_renderedString = getRenderedStringParser().parse(
            getTextVisual(), nullptr, nullptr);
        d_renderedStringValid = true;
    }

    return d_renderedString;
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
glm::vec2 Window::getUnprojectedPosition(const glm::vec2& pos) const
{
    RenderingSurface* rs = getTargetRenderingSurface();
    if (!rs)
        return pos;

    // if window is not backed by RenderingWindow, return same pos.
    if (!rs->isRenderingWindow())
        return pos;

    // get first target RenderingWindow
    RenderingWindow* rw = static_cast<RenderingWindow*>(rs);

    // setup for loop
    glm::vec2 out_pos(pos);

    // while there are rendering windows
    while (rw)
    {
        // unproject the point for the current rw
        const glm::vec2 in_pos(out_pos);
        rw->unprojectPoint(in_pos, out_pos);

        // get next rendering window, if any
        rw = (rs = &rw->getOwner())->isRenderingWindow() ?
                static_cast<RenderingWindow*>(rs) : nullptr;
    }

    return out_pos;
}

//----------------------------------------------------------------------------//
const String& Window::getTextVisual() const
{
#if defined(CEGUI_BIDI_SUPPORT)
    // no bidi support
    if (!d_bidiVisualMapping)
        return d_textLogical;

    if (!d_bidiDataValid)
    {
        d_bidiVisualMapping->updateVisual(d_textLogical);
        d_bidiDataValid = true;
    }

    return d_bidiVisualMapping->getTextVisual();
#else
    return d_textLogical;
#endif
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
void Window::setUpdateMode(const WindowUpdateMode mode)
{
    d_updateMode = mode;
}

//----------------------------------------------------------------------------//
void Window::setCursorInputPropagationEnabled(const bool enabled)
{
    d_propagatePointerInputs = enabled;
}

//----------------------------------------------------------------------------//
Window* Window::clone(const bool deepCopy) const
{
    Window* ret =
        WindowManager::getSingleton().createWindow(getType(), getName());

    //Setting some properties on DragContainer trigger events that require the GUI Context to be set
    if (this->d_guiContext)
        ret->setGUIContext(this->d_guiContext);
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
    for (PropertySet::PropertyIterator propertyIt = getPropertyIterator();
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

        target.setProperty(propertyName, propertyValue);
    }
}

//----------------------------------------------------------------------------//
void Window::cloneChildWidgetsTo(Window& target) const
{
    // TODO: need to check &target != this?

    // TODO: ChildWindowIterator?
    for (size_t childI = 0; childI < getChildCount(); ++childI)
    {
        Window* child = getChildAtIndex(childI);
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

        Window* newChild = child->clone(true);
        target.addChild(newChild);
    }
}

//----------------------------------------------------------------------------//
const Window* Window::getWindowAttachedToCommonAncestor(const Window& wnd) const
{
    const Window* w = &wnd;
    const Window* tmp = w->getParent();

    while (tmp)
    {
        if (isAncestor(tmp))
            break;

        w = tmp;
        tmp = tmp->getParent();
    }

    return tmp ? w : 0;
}

//----------------------------------------------------------------------------//
void Window::updatePivot()
{
    static_cast<RenderingWindow*>(d_surface)->setPivot(
      glm::vec3(CoordConverter::asAbsolute(d_pivot.d_x, d_pixelSize.d_width,  false),
                CoordConverter::asAbsolute(d_pivot.d_y, d_pixelSize.d_height, false),
                CoordConverter::asAbsolute(d_pivot.d_z, 0,                    false)));
}

//----------------------------------------------------------------------------//
GUIContext* Window::getGUIContextPtr() const
{
    // GUIContext is always the one on the root window, we do not allow parts
    // of a hierarchy to be drawn to separate contexts (which is not much of
    // a limitation).
    return getParent() ? getParent()->getGUIContextPtr() : d_guiContext;
}

//----------------------------------------------------------------------------//
GUIContext& Window::getGUIContext() const
{
    GUIContext* context = getGUIContextPtr();

    if (!context)
        throw NullObjectException("There is no GUI context for the window " + d_name);

    return *context;
}

//----------------------------------------------------------------------------//
void Window::setGUIContext(GUIContext* context)
{
    if (d_guiContext == context)
        return;

    d_guiContext = context;

    // TODO: store context recursively in children? Field exists anyway.

    notifyDefaultFontChanged(); // TODO: can check if font really changed? Move to GUIContext::setRootWindow?
    onTargetSurfaceChanged(getTargetRenderingSurface());
}

//----------------------------------------------------------------------------//
void Window::onTargetSurfaceChanged(RenderingSurface* newSurface)
{
    // Surface was set manually, we don't control it
    //???if (d_surface && !d_surface->isRenderingWindow())?
    //???any window must be processed, even the one that was set externally?
    //!!!transferRenderingWindow!
    if (d_surface && !d_autoRenderingWindow)
        return;

    if (d_autoRenderingWindow)
    {
        // We use our own auto-window and must update its state
        if (!d_surface)
        {
            if (newSurface)
            {
                allocateRenderingWindow(d_autoRenderingSurfaceStencilEnabled);

                // Propagate our auto-window as a new host surface for our children
                for (auto child : d_children)
                    static_cast<Window*>(child)->onTargetSurfaceChanged(d_surface);
            }
        }
        else if (!newSurface)
        {
            if (d_surface)
            {
                // We are about to destroy our auto-window, so enforce children that use it
                // as a host surface to destroy their windows first.
                for (auto child : d_children)
                    static_cast<Window*>(child)->onTargetSurfaceChanged(nullptr);

                releaseRenderingWindow();
            }
        }
        else if (newSurface != d_surface)
        {
            // Since we have a surface, child surfaces stay with us. Though we
            // must now ensure /our/ surface is transferred.
            newSurface->transferRenderingWindow(static_cast<RenderingWindow&>(*d_surface));
        }
    }
    else
    {
        // If we do not have a surface, transfer any surfaces from our children to
        // whatever our target surface now is.
        for (auto child : d_children)
            static_cast<Window*>(child)->onTargetSurfaceChanged(newSurface);
    }
}

//----------------------------------------------------------------------------//
Sizef Window::getRootContainerSize() const
{
    if (auto root = getRootWindow())
    {
        if (GUIContext* context = root->getGUIContextPtr())
        {
            return context->getSurfaceSize();
        }
        else if (root->getRenderingSurface())
        {
            return root->getRenderingSurface()->getRenderTarget().getArea().getSize();
        }
    }

    return Sizef(0.f, 0.f);
}

//----------------------------------------------------------------------------//
float Window::getContentWidth() const
{ 
    if (getWindowRenderer())
        return getWindowRenderer()->getContentWidth();
    return Element::getContentWidth();
}

//----------------------------------------------------------------------------//
float Window::getContentHeight() const
{
    if (getWindowRenderer())
        return getWindowRenderer()->getContentHeight();
    return Element::getContentHeight();
}

//----------------------------------------------------------------------------//
UDim Window::getWidthOfAreaReservedForContentLowerBoundAsFuncOfElementWidth()
  const
{
    if (getWindowRenderer())
        return getWindowRenderer()->getWidthOfAreaReservedForContentLowerBoundAsFuncOfWindowWidth();
    return Element::getWidthOfAreaReservedForContentLowerBoundAsFuncOfElementWidth();
}

//----------------------------------------------------------------------------//
UDim Window::getHeightOfAreaReservedForContentLowerBoundAsFuncOfElementHeight()
  const
{
    if (getWindowRenderer())
        return getWindowRenderer()-> getHeightOfAreaReservedForContentLowerBoundAsFuncOfWindowHeight();
    return Element::getHeightOfAreaReservedForContentLowerBoundAsFuncOfElementHeight();
}

//----------------------------------------------------------------------------//
void Window::adjustSizeToContent()
{
    if (!isSizeAdjustedToContent())
        return;
    if (getWindowRenderer())
    {
        getWindowRenderer()->adjustSizeToContent();
        return;
    }
    Element::adjustSizeToContent();
}

//----------------------------------------------------------------------------//
bool Window::contentFitsForSpecifiedElementSize(const Sizef& element_size) const
{
    if (getWindowRenderer())
        return getWindowRenderer()->contentFitsForSpecifiedWindowSize(element_size);
    return Element::contentFitsForSpecifiedElementSize(element_size);
}

//----------------------------------------------------------------------------//
bool Window::contentFits() const
{
    if (getWindowRenderer())
        return getWindowRenderer()->contentFits();
    return Element::contentFits();
}

//----------------------------------------------------------------------------//
void Window::setAutoWindow(bool is_auto)
{
    d_autoWindow = is_auto;

    if (d_autoWindow)
        banPropertiesForAutoWindow();
}

//----------------------------------------------------------------------------//
void Window::banPropertiesForAutoWindow()
{
    banPropertyFromXML("AutoWindow"); // :-D
    banPropertyFromXML("DestroyedByParent");
    banPropertyFromXML("VerticalAlignment");
    banPropertyFromXML("HorizontalAlignment");
    banPropertyFromXML("Area");
    banPropertyFromXML("Position");
    banPropertyFromXML("Size");
    banPropertyFromXML("MinSize");
    banPropertyFromXML("MaxSize");
    banPropertyFromXML(&d_windowRendererProperty);
    banPropertyFromXML(&d_lookNFeelProperty);
}

//----------------------------------------------------------------------------//
bool Window::handleFontRenderSizeChange(const EventArgs& args)
{
    return d_windowRenderer && d_windowRenderer->handleFontRenderSizeChange(
        static_cast<const FontEventArgs&>(args).font);
}

//----------------------------------------------------------------------------//
void Window::focus()
{
    if (isDisabled())
        return;

    d_isFocused = true;

    ActivationEventArgs event_args(this);
    onActivated(event_args);
}

//----------------------------------------------------------------------------//
void Window::unfocus()
{
    d_isFocused = false;

    if (d_active)
    {
        ActivationEventArgs event_args(this);
        onDeactivated(event_args);
    }
}

//----------------------------------------------------------------------------//
bool Window::canFocus() const
{
    // by default all widgets can be focused if they are not disabled
    return !isDisabled();
}

//----------------------------------------------------------------------------//
void Window::destroyGeometryBuffers()
{
    const size_t geom_buffer_count = d_geometryBuffers.size();
    for (size_t i = 0; i < geom_buffer_count; ++i)
        System::getSingleton().getRenderer()->destroyGeometryBuffer(*d_geometryBuffers.at(i));

    d_geometryBuffers.clear();
}

//----------------------------------------------------------------------------//
void Window::setDrawModeMask(std::uint32_t drawModeMask)
{
    if (d_drawModeMask == drawModeMask)
        return;

    d_drawModeMask = drawModeMask;
    if (auto ctx = getGUIContextPtr())
        ctx->markAsDirty();
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
