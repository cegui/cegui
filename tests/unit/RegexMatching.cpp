/***********************************************************************
 *    created:    15/11/2012
 *    author:     Martin Preisler
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

#include "CEGUI/Base.h"

#ifdef CEGUI_HAS_PCRE_REGEX

#include "CEGUI/RegexMatcher.h"
#include "CEGUI/System.h"

#include <boost/test/unit_test.hpp>

struct RegexMatchingFixture
{
    RegexMatchingFixture()
    {
        d_regexMatcher = CEGUI::System::getSingleton().createRegexMatcher();
    }

    ~RegexMatchingFixture()
    {
        CEGUI::System::getSingleton().destroyRegexMatcher(d_regexMatcher);
    }

    CEGUI::RegexMatcher* d_regexMatcher;
};

BOOST_FIXTURE_TEST_SUITE(RegexMatching, RegexMatchingFixture)

BOOST_AUTO_TEST_CASE(RegexStringPersistence)
{
    d_regexMatcher->setRegexString("abc");
    BOOST_CHECK_EQUAL(d_regexMatcher->getRegexString(), "abc");

    d_regexMatcher->setRegexString("(abc)");
    BOOST_CHECK_EQUAL(d_regexMatcher->getRegexString(), "(abc)");
}

BOOST_AUTO_TEST_CASE(Trivial)
{
    d_regexMatcher->setRegexString(".*");
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString(""), CEGUI::RegexMatcher::MS_VALID);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("a"), CEGUI::RegexMatcher::MS_VALID);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("aaaaa"), CEGUI::RegexMatcher::MS_VALID);
}

BOOST_AUTO_TEST_CASE(Partial)
{
    d_regexMatcher->setRegexString(".*aaa");
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString(""), CEGUI::RegexMatcher::MS_INVALID);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("a"), CEGUI::RegexMatcher::MS_PARTIAL);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("aaa"), CEGUI::RegexMatcher::MS_VALID);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("aaaaa"), CEGUI::RegexMatcher::MS_VALID);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("aaaaab"), CEGUI::RegexMatcher::MS_INVALID);

    d_regexMatcher->setRegexString(".+aaa");
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString(""), CEGUI::RegexMatcher::MS_INVALID);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("a"), CEGUI::RegexMatcher::MS_PARTIAL);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("aaa"), CEGUI::RegexMatcher::MS_PARTIAL);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("aaaa"), CEGUI::RegexMatcher::MS_VALID);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("aaaaa"), CEGUI::RegexMatcher::MS_VALID);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("aaaaab"), CEGUI::RegexMatcher::MS_INVALID);
}

BOOST_AUTO_TEST_CASE(CharacterGroups)
{
    d_regexMatcher->setRegexString("[a-z]");
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("a"), CEGUI::RegexMatcher::MS_VALID);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("b"), CEGUI::RegexMatcher::MS_VALID);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("z"), CEGUI::RegexMatcher::MS_VALID);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("za"), CEGUI::RegexMatcher::MS_INVALID);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("A"), CEGUI::RegexMatcher::MS_INVALID);
}

BOOST_AUTO_TEST_CASE(RepeatedSeqs)
{
    d_regexMatcher->setRegexString("(abc)+");
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("ab"), CEGUI::RegexMatcher::MS_PARTIAL);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("abc"), CEGUI::RegexMatcher::MS_VALID);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("bc"), CEGUI::RegexMatcher::MS_INVALID);

    d_regexMatcher->setRegexString("(abc){2}");
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("abc"), CEGUI::RegexMatcher::MS_PARTIAL);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("abcabc"), CEGUI::RegexMatcher::MS_VALID);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("bc"), CEGUI::RegexMatcher::MS_INVALID);
}

BOOST_AUTO_TEST_CASE(SmokeTests)
{
    // IPv4 address
    d_regexMatcher->setRegexString("\\b(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("192."), CEGUI::RegexMatcher::MS_PARTIAL);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("192.168"), CEGUI::RegexMatcher::MS_PARTIAL);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("192.168.1"), CEGUI::RegexMatcher::MS_PARTIAL);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("192.168.1."), CEGUI::RegexMatcher::MS_PARTIAL);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("192.168.1.1"), CEGUI::RegexMatcher::MS_VALID);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("192.168.1.111"), CEGUI::RegexMatcher::MS_VALID);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("292.168.1.111"), CEGUI::RegexMatcher::MS_INVALID);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("192:168.1.111"), CEGUI::RegexMatcher::MS_INVALID);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("a.168.1.111"), CEGUI::RegexMatcher::MS_INVALID);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString("192.168.1.111:"), CEGUI::RegexMatcher::MS_INVALID);
    BOOST_CHECK_EQUAL(d_regexMatcher->getMatchStateOfString(":192.168.1.111"), CEGUI::RegexMatcher::MS_INVALID);
}

BOOST_AUTO_TEST_SUITE_END()

#endif
