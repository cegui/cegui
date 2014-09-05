/***********************************************************************
 *    created:    21/4/2013
 *    author:     Martin Preisler
 *************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2013 Paul D Turner & The CEGUI Development Team
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

#include "CEGUI/Animation.h"
#include "CEGUI/AnimationInstance.h"
#include "CEGUI/AnimationManager.h"
#include "CEGUI/Affector.h"

#include <boost/test/unit_test.hpp>

struct SampleAnimationSetupFixture
{
    SampleAnimationSetupFixture()
    {
        {
            d_zeroToOne = CEGUI::AnimationManager::getSingleton().createAnimation("ZeroToOne");
            d_zeroToOne->setDuration(1.0f);
            CEGUI::Affector* affector = d_zeroToOne->createAffector("Alpha", "float");
            affector->createKeyFrame(0.0f, "0");
            affector->createKeyFrame(1.0f, "1");
        }
        {
            d_zeroDuration = CEGUI::AnimationManager::getSingleton().createAnimation("ZeroLength");
            d_zeroDuration->setDuration(0.0f);
            CEGUI::Affector* affector = d_zeroDuration->createAffector("Alpha", "float");
        }
    }

    ~SampleAnimationSetupFixture()
    {
        CEGUI::AnimationManager::getSingleton().destroyAnimation(d_zeroToOne);
        CEGUI::AnimationManager::getSingleton().destroyAnimation(d_zeroDuration);
    }

    CEGUI::Animation* d_zeroToOne;
    CEGUI::Animation* d_zeroDuration;
};

BOOST_FIXTURE_TEST_SUITE(AnimationSystem, SampleAnimationSetupFixture)

BOOST_AUTO_TEST_CASE(SkipFirstStep)
{
    CEGUI::AnimationInstance* instance = CEGUI::AnimationManager::getSingleton().instantiateAnimation(d_zeroToOne);
    d_zeroToOne->setReplayMode(CEGUI::Animation::RM_Once);

    instance->start(true);
    BOOST_CHECK_CLOSE(instance->getPosition(), 0.0f, 0.0001f);
    instance->step(123.0f);
    BOOST_CHECK_CLOSE(instance->getPosition(), 0.0f, 0.0001f);

    CEGUI::AnimationManager::getSingleton().destroyAnimationInstance(instance);
}

BOOST_AUTO_TEST_CASE(PlayOnceReplayMode)
{
    CEGUI::AnimationInstance* instance = CEGUI::AnimationManager::getSingleton().instantiateAnimation(d_zeroToOne);
    d_zeroToOne->setReplayMode(CEGUI::Animation::RM_Once);

    instance->start(false);
    BOOST_CHECK_SMALL(instance->getPosition(), 0.0001f);
    instance->step(0.0f);
    BOOST_CHECK_SMALL(instance->getPosition(), 0.0001f);
    instance->step(0.5f);
    BOOST_CHECK_CLOSE(instance->getPosition(), 0.5f, 0.0001f);
    instance->step(0.5f);
    BOOST_CHECK_CLOSE(instance->getPosition(), 1.0f, 0.0001f);
    instance->step(1.0f);
    BOOST_CHECK_CLOSE(instance->getPosition(), 1.0f, 0.0001f);

    CEGUI::AnimationManager::getSingleton().destroyAnimationInstance(instance);
}

BOOST_AUTO_TEST_CASE(PlayLoopReplayMode)
{
    CEGUI::AnimationInstance* instance = CEGUI::AnimationManager::getSingleton().instantiateAnimation(d_zeroToOne);
    d_zeroToOne->setReplayMode(CEGUI::Animation::RM_Loop);

    instance->start(false);
    BOOST_CHECK_SMALL(instance->getPosition(), 0.0001f);
    instance->step(0.0f);
    BOOST_CHECK_SMALL(instance->getPosition(), 0.0001f);
    instance->step(0.5f);
    BOOST_CHECK_CLOSE(instance->getPosition(), 0.5f, 0.0001f);
    instance->step(0.5f);
    BOOST_CHECK_CLOSE(instance->getPosition(), 1.0f, 0.0001f);
    instance->step(0.3f);
    BOOST_CHECK_CLOSE(instance->getPosition(), 0.3f, 0.0001f);
    instance->step(0.7f);
    BOOST_CHECK_CLOSE(instance->getPosition(), 1.0f, 0.0001f);

    CEGUI::AnimationManager::getSingleton().destroyAnimationInstance(instance);
}

BOOST_AUTO_TEST_CASE(PlayBounceReplayMode)
{
    CEGUI::AnimationInstance* instance = CEGUI::AnimationManager::getSingleton().instantiateAnimation(d_zeroToOne);
    d_zeroToOne->setReplayMode(CEGUI::Animation::RM_Bounce);

    instance->start(false);
    BOOST_CHECK_SMALL(instance->getPosition(), 0.0001f);
    instance->step(0.0f);
    BOOST_CHECK_SMALL(instance->getPosition(), 0.0001f);
    instance->step(0.5f);
    BOOST_CHECK_CLOSE(instance->getPosition(), 0.5f, 0.0001f);
    instance->step(0.5f);
    BOOST_CHECK_CLOSE(instance->getPosition(), 1.0f, 0.0001f);
    instance->step(0.3f);
    BOOST_CHECK_CLOSE(instance->getPosition(), 0.7f, 0.0001f);
    instance->step(0.7f);
    BOOST_CHECK_SMALL(instance->getPosition(), 0.0001f);

    CEGUI::AnimationManager::getSingleton().destroyAnimationInstance(instance);
}

BOOST_AUTO_TEST_CASE(ZeroDurationReplayModes)
{
    {
        CEGUI::AnimationInstance* instance = CEGUI::AnimationManager::getSingleton().instantiateAnimation(d_zeroDuration);
        d_zeroDuration->setReplayMode(CEGUI::Animation::RM_Once);

        instance->start(false);
        BOOST_CHECK_SMALL(instance->getPosition(), 0.0001f);
        instance->step(0.0f);
        BOOST_CHECK_SMALL(instance->getPosition(), 0.0001f);
        instance->step(0.5f);
        BOOST_CHECK_SMALL(instance->getPosition(), 0.0001f);

        CEGUI::AnimationManager::getSingleton().destroyAnimationInstance(instance);
    }
    {
        CEGUI::AnimationInstance* instance = CEGUI::AnimationManager::getSingleton().instantiateAnimation(d_zeroDuration);
        d_zeroDuration->setReplayMode(CEGUI::Animation::RM_Loop);

        instance->start(false);
        BOOST_CHECK_SMALL(instance->getPosition(), 0.0001f);
        instance->step(0.0f);
        BOOST_CHECK_SMALL(instance->getPosition(), 0.0001f);
        instance->step(0.5f);
        BOOST_CHECK_SMALL(instance->getPosition(), 0.0001f);

        CEGUI::AnimationManager::getSingleton().destroyAnimationInstance(instance);
    }
    {
        CEGUI::AnimationInstance* instance = CEGUI::AnimationManager::getSingleton().instantiateAnimation(d_zeroDuration);
        d_zeroDuration->setReplayMode(CEGUI::Animation::RM_Bounce);

        instance->start(false);
        BOOST_CHECK_SMALL(instance->getPosition(), 0.0001f);
        instance->step(0.0f);
        BOOST_CHECK_SMALL(instance->getPosition(), 0.0001f);
        instance->step(0.5f);
        BOOST_CHECK_SMALL(instance->getPosition(), 0.0001f);

        CEGUI::AnimationManager::getSingleton().destroyAnimationInstance(instance);
    }
}

BOOST_AUTO_TEST_SUITE_END()
