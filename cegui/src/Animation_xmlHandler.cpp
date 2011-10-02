/***********************************************************************
    filename:   CEGUIAnimation_xmlHandler.cpp
    created:    Wed Aug 11 2010
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/Animation_xmlHandler.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/Logger.h"
#include "CEGUI/XMLAttributes.h"

#include "CEGUI/AnimationManager.h"
#include "CEGUI/Animation.h"
#include "CEGUI/Affector.h"
#include "CEGUI/KeyFrame.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
// Static XML element name member strings for the sub-handlers
const String AnimationDefinitionHandler::ElementName("AnimationDefinition");
const String AnimationAffectorHandler::ElementName("Affector");
const String AnimationKeyFrameHandler::ElementName("KeyFrame");
const String AnimationSubscriptionHandler::ElementName("Subscription");

//----------------------------------------------------------------------------//
// Internal strings for XML element names
static const String AnimationsElement("Animations");

//----------------------------------------------------------------------------//
// Internal strings for the XML attribute names
static const String NameAttribute("name");
static const String DurationAttribute("duration");
static const String ReplayModeAttribute("replayMode");
static const String AutoStartAttribute("autoStart");
static const String ApplicationMethodAttribute("applicationMethod");
static const String PropertyAttribute("property");
static const String InterpolatorAttribute("interpolator");
static const String PositionAttribute("position");
static const String ValueAttribute("value");
static const String SourcePropertyAttribute("sourceProperty");
static const String ProgressionAttribute("progression");
static const String EventAttribute("event");
static const String ActionAttribute("action");

//----------------------------------------------------------------------------//
// Internal strings for the XML enumeration for replay modes
static const String ReplayModeOnce("once");
static const String ReplayModeLoop("loop");
static const String ReplayModeBounce("bounce");

//----------------------------------------------------------------------------//
// Internal strings for the XML enumeration for application methods.
static const String ApplicationMethodAbsolute("absolute");
static const String ApplicationMethodRelative("relative");
static const String ApplicationMethodRelativeMultiply("relative multiply");

//----------------------------------------------------------------------------//
// Internal strings for the XML enumeration for progression types.
static const String ProgressionLinear("linear");
static const String ProgressionDiscrete("discrete");
static const String ProgressionQuadraticAccelerating("quadratic accelerating");
static const String ProgressionQuadraticDecelerating("quadratic decelerating");

//----------------------------------------------------------------------------//
Animation_xmlHandler::Animation_xmlHandler()
{}

//----------------------------------------------------------------------------//
Animation_xmlHandler::~Animation_xmlHandler()
{}

//----------------------------------------------------------------------------//
const String& Animation_xmlHandler::getSchemaName() const
{
    return AnimationManager::XMLSchemaName;
}

//----------------------------------------------------------------------------//
const String& Animation_xmlHandler::getDefaultResourceGroup() const
{
    return AnimationManager::getDefaultResourceGroup();
}

//----------------------------------------------------------------------------//
void Animation_xmlHandler::elementStartLocal(const String& element,
                                             const XMLAttributes& attributes)
{
    if (element == AnimationsElement)
    {
        Logger::getSingleton().logEvent("===== Begin Animations parsing =====");
    }
    else if (element == AnimationDefinitionHandler::ElementName)
    {
        d_chainedHandler = CEGUI_NEW_AO AnimationDefinitionHandler(attributes, "");
    }
    else
        Logger::getSingleton().logEvent("Animation_xmlHandler::elementStart: "
            "<" + element + "> is invalid at this location.", Errors);
}

//----------------------------------------------------------------------------//
void Animation_xmlHandler::elementEndLocal(const String& element)
{
    if (element == AnimationsElement)
    {
        Logger::getSingleton().logEvent("===== End Animations parsing =====");
    }
    else
        Logger::getSingleton().logEvent("Animation_xmlHandler::elementEnd: "
            "</" + element + "> is invalid at this location.", Errors);
}

//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
AnimationDefinitionHandler::AnimationDefinitionHandler(
                                const XMLAttributes& attributes,
                                const String& name_prefix) :
    d_anim(0)
{
    const String anim_name(name_prefix +
                           attributes.getValueAsString(NameAttribute));

    Logger::getSingleton().logEvent(
        "Defining animation named: " +
        anim_name +
        "  Duration: " +
        attributes.getValueAsString(DurationAttribute) +
        "  Replay mode: " +
        attributes.getValueAsString(ReplayModeAttribute) +
        "  Auto start: " +
        attributes.getValueAsString(AutoStartAttribute, "false"));

    d_anim = AnimationManager::getSingleton().createAnimation(anim_name);

    d_anim->setDuration(attributes.getValueAsFloat(DurationAttribute));

    const String replayMode(attributes.getValueAsString(ReplayModeAttribute,
                                                        ReplayModeLoop));
    if (replayMode == ReplayModeOnce)
        d_anim->setReplayMode(Animation::RM_Once);
    else if (replayMode == ReplayModeBounce)
        d_anim->setReplayMode(Animation::RM_Bounce);
    else
        d_anim->setReplayMode(Animation::RM_Loop);

    d_anim->setAutoStart(attributes.getValueAsBool(AutoStartAttribute));
}

//----------------------------------------------------------------------------//
AnimationDefinitionHandler::~AnimationDefinitionHandler()
{
}

//----------------------------------------------------------------------------//
void AnimationDefinitionHandler::elementStartLocal(
                                            const String& element,
                                            const XMLAttributes& attributes)
{
    if (element == AnimationAffectorHandler::ElementName)
        d_chainedHandler = CEGUI_NEW_AO AnimationAffectorHandler(attributes, *d_anim);
    else if (element == AnimationSubscriptionHandler::ElementName)
        d_chainedHandler = CEGUI_NEW_AO AnimationSubscriptionHandler(attributes, *d_anim);
    else
        Logger::getSingleton().logEvent(
            "AnimationDefinitionHandler::elementStart: "
            "<" + element + "> is invalid at this location.", Errors);
}

//----------------------------------------------------------------------------//
void AnimationDefinitionHandler::elementEndLocal(const String& element)
{
    // set completed status when we encounter our own end element
    if (element == ElementName)
        d_completed = true;
}

//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
AnimationAffectorHandler::AnimationAffectorHandler(
                                            const XMLAttributes& attributes,
                                            Animation& anim) :
    d_affector(0)
{
    Logger::getSingleton().logEvent(
        "\tAdding affector for property: " +
        attributes.getValueAsString(PropertyAttribute) +
        "  Interpolator: " +
        attributes.getValueAsString(InterpolatorAttribute) +
        "  Application method: " +
        attributes.getValueAsString(ApplicationMethodAttribute, "absolute"));

    d_affector = anim.createAffector(
        attributes.getValueAsString(PropertyAttribute),
        attributes.getValueAsString(InterpolatorAttribute));

    if (attributes.getValueAsString(ApplicationMethodAttribute) ==
        ApplicationMethodRelative)
    {
        d_affector->setApplicationMethod(Affector::AM_Relative);
    }
	else if (attributes.getValueAsString(ApplicationMethodAttribute) ==
        ApplicationMethodRelativeMultiply)
    {
        d_affector->setApplicationMethod(Affector::AM_RelativeMultiply);
    }
    else
    {
        d_affector->setApplicationMethod(Affector::AM_Absolute);
    }
}

//----------------------------------------------------------------------------//
AnimationAffectorHandler::~AnimationAffectorHandler()
{
}

//----------------------------------------------------------------------------//
void AnimationAffectorHandler::elementStartLocal(
                                        const String& element,
                                        const XMLAttributes& attributes)
{
    if (element == AnimationKeyFrameHandler::ElementName)
        d_chainedHandler = CEGUI_NEW_AO AnimationKeyFrameHandler(attributes, *d_affector);
    else
        Logger::getSingleton().logEvent(
            "AnimationAffectorHandler::elementStart: "
            "<" + element + "> is invalid at this location.", Errors);
}

//----------------------------------------------------------------------------//
void AnimationAffectorHandler::elementEndLocal(const String& element)
{
    // set completed status when we encounter our own end element
    if (element == ElementName)
        d_completed = true;
}

//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
AnimationKeyFrameHandler::AnimationKeyFrameHandler(
                                        const XMLAttributes& attributes,
                                        Affector& affector)
{
    const String progressionStr(
        attributes.getValueAsString(ProgressionAttribute));

    String log_event(
        "\t\tAdding KeyFrame at position: " +
        attributes.getValueAsString(PositionAttribute) +
        "  Value: " +
        attributes.getValueAsString(ValueAttribute));

    if (!progressionStr.empty())
        log_event.append("  Progression: " +
            attributes.getValueAsString(ProgressionAttribute, ProgressionLinear));

    Logger::getSingleton().logEvent(log_event);

    KeyFrame::Progression progression;
    if (progressionStr == ProgressionDiscrete)
        progression = KeyFrame::P_Discrete;
    else if (progressionStr == ProgressionQuadraticAccelerating)
        progression = KeyFrame::P_QuadraticAccelerating;
    else if (progressionStr == ProgressionQuadraticDecelerating)
        progression = KeyFrame::P_QuadraticDecelerating;
    else
        progression = KeyFrame::P_Linear;

    affector.createKeyFrame(
        attributes.getValueAsFloat(PositionAttribute),
        attributes.getValueAsString(ValueAttribute),
        progression,
        attributes.getValueAsString(SourcePropertyAttribute));
    
    if (affector.getNumKeyFrames() == 1 && !progressionStr.empty())
        Logger::getSingleton().logEvent(
            "WARNING: progression type specified for first keyframe in "
            "animation will be ignored.");

    d_completed = true;
}

//----------------------------------------------------------------------------//
AnimationKeyFrameHandler::~AnimationKeyFrameHandler()
{
}

//----------------------------------------------------------------------------//
void AnimationKeyFrameHandler::elementStartLocal(
                                        const String& element,
                                        const XMLAttributes& /*attributes*/)
{
    Logger::getSingleton().logEvent(
        "AnimationAffectorHandler::elementStart: "
        "<" + element + "> is invalid at this location.", Errors);
}

//----------------------------------------------------------------------------//
void AnimationKeyFrameHandler::elementEndLocal(const String& element)
{
    // set completed status when we encounter our own end element
    if (element == ElementName)
        d_completed = true;
}

//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
AnimationSubscriptionHandler::AnimationSubscriptionHandler(
                                        const XMLAttributes& attributes,
                                        Animation& anim)
{
    Logger::getSingleton().logEvent(
        "\tAdding subscription to event: " +
        attributes.getValueAsString(EventAttribute) + 
        "  Action: " +
        attributes.getValueAsString(ActionAttribute));

    anim.defineAutoSubscription(
        attributes.getValueAsString(EventAttribute),
        attributes.getValueAsString(ActionAttribute));

    d_completed = true;
}

//----------------------------------------------------------------------------//
AnimationSubscriptionHandler::~AnimationSubscriptionHandler()
{
}

//----------------------------------------------------------------------------//
void AnimationSubscriptionHandler::elementStartLocal(
                                            const String& element,
                                            const XMLAttributes& /*attributes*/)
{
    Logger::getSingleton().logEvent(
        "AnimationAffectorHandler::elementStart: "
        "</" + element + "> is invalid at this location.", Errors);
}

//----------------------------------------------------------------------------//
void AnimationSubscriptionHandler::elementEndLocal(const String& element)
{
    // set completed status when we encounter our own end element
    if (element == ElementName)
        d_completed = true;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

