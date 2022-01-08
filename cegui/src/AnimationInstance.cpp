/***********************************************************************
    created:    7/8/2010
    author:     Martin Preisler

    purpose:    Implements the AnimationInstance class
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
#include "CEGUI/AnimationInstance.h"
#include "CEGUI/Animation.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/Window.h"
#include "CEGUI/Logger.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
const String AnimationInstance::EventNamespace("AnimationInstance");

const String AnimationInstance::EventAnimationStarted("AnimationStarted");
const String AnimationInstance::EventAnimationStopped("AnimationStopped");
const String AnimationInstance::EventAnimationPaused("AnimationPaused");
const String AnimationInstance::EventAnimationUnpaused("AnimationUnpaused");
const String AnimationInstance::EventAnimationFinished("AnimationFinished");
const String AnimationInstance::EventAnimationEnded("AnimationEnded");
const String AnimationInstance::EventAnimationLooped("AnimationLooped");

//----------------------------------------------------------------------------//
AnimationInstance::AnimationInstance(Animation* definition):
    d_definition(definition)
{}

//----------------------------------------------------------------------------//
AnimationInstance::~AnimationInstance()
{
    if (d_running)
        stop();

    if (d_eventSender)
        d_definition->autoUnsubscribe(this);
}

//----------------------------------------------------------------------------//
void AnimationInstance::setTarget(PropertySet* target)
{
    d_target = target;

    purgeSavedPropertyValues();

    if (d_definition->getAutoStart() && !isRunning())
        start();
}

//----------------------------------------------------------------------------//
void AnimationInstance::setEventSender(EventSet* sender)
{
    if (d_eventSender)
        d_definition->autoUnsubscribe(this);

    d_eventSender = sender;

    if (d_eventSender)
        d_definition->autoSubscribe(this);
}

//----------------------------------------------------------------------------//
void AnimationInstance::setTargetWindow(Window* target)
{
    setTarget(target);
    setEventReceiver(target);
    setEventSender(target);
}

//----------------------------------------------------------------------------//
void AnimationInstance::setPosition(float position)
{
    if (position < 0.0 || position > d_definition->getDuration())
    {
        throw InvalidRequestException(
                        "Unable to set position of this animation instance "
                        "because given position isn't in interval "
                        "[0.0, duration of animation].");
    }

    d_position = position;
}

//----------------------------------------------------------------------------//
void AnimationInstance::setSpeed(float speed)
{
    // first sort out the adventurous users
    if (speed < 0.0f)
    {
        throw InvalidRequestException(
                        "You can't set playback speed to a value that's lower "
                        "than 0.0");
    }

    if (speed == 0.0f)
    {
        throw InvalidRequestException(
                        "AnimationInstance::setSpeed: You can't set playback speed "
                        "to zero, please use AnimationInstance::pause instead");
    }

    d_speed = speed;
}

//----------------------------------------------------------------------------//
void AnimationInstance::start(bool skipNextStep)
{
    setPosition(0.0);
    d_skipNextStep = skipNextStep;

    if (d_definition && d_definition->getDuration() > 0)
    {
        d_running = true;

        purgeSavedPropertyValues();
        d_definition->savePropertyValues(this);

        onAnimationStarted();
    }
    else
    {
        Logger::getSingleton().logEvent(
            "AnimationInstance::start - Starting an animation instance with "
            "no animation definition or 0 duration has no effect!", LoggingLevel::Informative);
        onAnimationStarted();
        onAnimationEnded();
    }
}

//----------------------------------------------------------------------------//
void AnimationInstance::stop()
{
    setPosition(0.0);
    d_running = false;
    onAnimationStopped();
}

//----------------------------------------------------------------------------//
void AnimationInstance::pause()
{
    d_running = false;
    onAnimationPaused();
}

//----------------------------------------------------------------------------//
void AnimationInstance::unpause(bool skipNextStep)
{
    d_skipNextStep = skipNextStep;

    if (d_definition && d_definition->getDuration() > 0)
    {
        d_running = true;
        onAnimationUnpaused();
    }
    else
    {
        Logger::getSingleton().logEvent(
            "AnimationInstance::unpause - Unpausing an animation instance with "
            "no animation definition or 0 duration has no effect!", LoggingLevel::Informative);
        onAnimationUnpaused();
        onAnimationEnded();
    }
}

//----------------------------------------------------------------------------//
void AnimationInstance::togglePause(bool skipNextStep)
{
    if (d_running)
        pause();
    else
        unpause(skipNextStep);
}

//----------------------------------------------------------------------------//
void AnimationInstance::finish()
{
    if (d_definition)
    {
        setPosition(d_definition->getDuration());
        apply();
    }

    d_running = false;
    setPosition(0.0);
    onAnimationFinished();
}

//----------------------------------------------------------------------------//
void AnimationInstance::step(float delta)
{
    if (!d_running)
    {
        // nothing to do if this animation instance isn't running
        return;
    }

    if (delta < 0.0f)
    {
        throw InvalidRequestException(
                        "You can't step the Animation Instance with negative "
                        "delta! You can't reverse the flow of time, stop "
                        "trying!");
    }

    // first we deal with delta size
    if (d_maxStepDeltaSkip > 0.0f && delta > d_maxStepDeltaSkip)
    {
        // skip the step entirely if delta gets over the threshold
        // note that default value is 0.0f which means this never gets triggered
        delta = 0.0f;
    }

    if (d_maxStepDeltaClamp > 0.0f)
    {
        // clamp to threshold, note that default value is -1.0f which means
        // this line does nothing (delta should always be larger or equal than 0.0f
        delta = std::min(delta, d_maxStepDeltaClamp);
    }

    // if asked to do so, we skip this step, but mark that the next one
    // shouldn't be skipped
    // NB: This gets rid of annoying animation skips when FPS gets too low
    //     after complex layout loading, etc...
    if (d_skipNextStep)
    {
        d_skipNextStep = false;
        // we skip the step by setting delta to 0, this doesn't step the time
        // but still sets the animation position accordingly
        delta = 0.0f;
    }

    const float duration = d_definition->getDuration();

    // we modify the delta according to playback speed
    delta *= d_speed;

    // the position could have gotten out of the desired range, we have to
    // alter it depending on replay method of our animation definition

    // first a simple clamp with ReplayMode::PLAY_ONCE
    if (d_definition->getReplayMode() == Animation::ReplayMode::PlayOnce)
    {
        float newPosition = d_position + delta;

        newPosition = std::max(0.0f, newPosition);

        if (newPosition >= duration)
        {
            newPosition = duration;

            stop();
            onAnimationEnded();
        }

        setPosition(newPosition);
    }
    // a both sided wrap with ReplayMode::LOOP
    else if (d_definition->getReplayMode() == Animation::ReplayMode::Loop)
    {
        float newPosition = d_position + delta;

        while (newPosition > duration)
        {
            newPosition -= duration;
            onAnimationLooped();
        }

        setPosition(newPosition);
    }
    // bounce back and forth with ReplayMode::BOUNCE
    else if (d_definition->getReplayMode() == Animation::ReplayMode::Bounce)
    {
        if (d_bounceBackwards)
        {
            delta = -delta;
        }

        float newPosition = d_position + delta;

        while (newPosition < 0.0f || newPosition > duration)
        {
            if (newPosition < 0.0f)
            {
                d_bounceBackwards = false;

                newPosition = -newPosition;
                onAnimationLooped();
            }

            if (newPosition > duration)
            {
                d_bounceBackwards = true;

                newPosition = 2.0f * duration - newPosition;
                onAnimationLooped();
            }
        }

        setPosition(newPosition);
    }

    apply();
}

//----------------------------------------------------------------------------//
bool AnimationInstance::handleStart(const EventArgs&)
{
    start();
    return true;
}

//----------------------------------------------------------------------------//
bool AnimationInstance::handleStop(const EventArgs&)
{
    stop();
    return true;
}

//----------------------------------------------------------------------------//
bool AnimationInstance::handlePause(const EventArgs&)
{
    pause();
    return true;
}

//----------------------------------------------------------------------------//
bool AnimationInstance::handleUnpause(const EventArgs&)
{
    unpause();
    return true;
}

//----------------------------------------------------------------------------//
bool AnimationInstance::handleTogglePause(const EventArgs&)
{
    togglePause();
    return true;
}

//----------------------------------------------------------------------------//
bool AnimationInstance::handleFinish(const EventArgs&)
{
    finish();
    return true;
}

//----------------------------------------------------------------------------//
void AnimationInstance::savePropertyValue(const String& propertyName)
{
    assert(d_target);
    d_savedPropertyValues[propertyName] = d_target->getProperty(propertyName);
}

//----------------------------------------------------------------------------//
void AnimationInstance::purgeSavedPropertyValues()
{
    d_savedPropertyValues.clear();
}

//----------------------------------------------------------------------------//
const String& AnimationInstance::getSavedPropertyValue(const String& propertyName)
{
    auto it = d_savedPropertyValues.find(propertyName);
    if (it == d_savedPropertyValues.end())
    {
        // even though we explicitly save all used property values when
        // starting the animation, this can happen when user changes
        // animation definition whilst the animation is running
        // (Yes, it's nasty, but people do nasty things)
        savePropertyValue(propertyName);
        return getSavedPropertyValue(propertyName);
    }

    return it->second;
}

//----------------------------------------------------------------------------//
void AnimationInstance::addAutoConnection(Event::Connection conn)
{
    d_autoConnections.push_back(conn);
}

//----------------------------------------------------------------------------//
void AnimationInstance::unsubscribeAutoConnections()
{
    for (auto& connection : d_autoConnections)
        connection->disconnect();
    d_autoConnections.clear();
}

//----------------------------------------------------------------------------//
void AnimationInstance::apply()
{
    if (d_target)
        d_definition->apply(this);
}

//----------------------------------------------------------------------------//
void AnimationInstance::onAnimationStarted()
{
    if (d_eventReceiver)
    {
        AnimationEventArgs args(this);
        d_eventReceiver->fireEvent(EventAnimationStarted, args, EventNamespace);
    }
}

//----------------------------------------------------------------------------//
void AnimationInstance::onAnimationStopped()
{
    if (d_eventReceiver)
    {
        AnimationEventArgs args(this);
        d_eventReceiver->fireEvent(EventAnimationStopped, args, EventNamespace);
    }
}

//----------------------------------------------------------------------------//
void AnimationInstance::onAnimationPaused()
{
    if (d_eventReceiver)
    {
        AnimationEventArgs args(this);
        d_eventReceiver->fireEvent(EventAnimationPaused, args, EventNamespace);
    }
}

//----------------------------------------------------------------------------//
void AnimationInstance::onAnimationUnpaused()
{
    if (d_eventReceiver)
    {
        AnimationEventArgs args(this);
        d_eventReceiver->fireEvent(EventAnimationUnpaused, args, EventNamespace);
    }
}

//----------------------------------------------------------------------------//
void AnimationInstance::onAnimationFinished()
{
    if (d_eventReceiver)
    {
        AnimationEventArgs args(this);
        d_eventReceiver->fireEvent(EventAnimationFinished, args, EventNamespace);
    }
}

//----------------------------------------------------------------------------//
void AnimationInstance::onAnimationEnded()
{
    if (d_eventReceiver)
    {
        AnimationEventArgs args(this);
        d_eventReceiver->fireEvent(EventAnimationEnded, args, EventNamespace);
    }
}

//----------------------------------------------------------------------------//
void AnimationInstance::onAnimationLooped()
{
    if (d_eventReceiver)
    {
        AnimationEventArgs args(this);
        d_eventReceiver->fireEvent(EventAnimationLooped, args, EventNamespace);
    }
}

}
