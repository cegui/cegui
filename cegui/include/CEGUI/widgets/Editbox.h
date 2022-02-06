/***********************************************************************
    created:    13/4/2004
    author:     Paul D Turner

    purpose:    Interface to base class for Editbox widget
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2015 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIEditbox_h_
#define _CEGUIEditbox_h_

#include "CEGUI/widgets/EditboxBase.h"
#include "CEGUI/WindowRenderer.h"
#include "CEGUI/RegexMatcher.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{

//! Base class for the EditboxWindowRenderer class
class CEGUIEXPORT EditboxWindowRenderer : public WindowRenderer
{
public:

    EditboxWindowRenderer(const String& name);

    /*!
    \brief
        Return the text code point index that is rendered closest to screen
        position \a pt.

    \param pt
        Point object describing a position on the screen in pixels.

    \return
        Code point index into the text that is rendered closest to screen
        position \a pt.
    */
    virtual size_t getTextIndexFromPosition(const glm::vec2& pt) const = 0;

    //! Editbox text parsing is forcefully disabled
    virtual bool isTextParsingEnabled() const { return false; }
};

//----------------------------------------------------------------------------//

//! Class for an Editbox widget
class CEGUIEXPORT Editbox : public EditboxBase
{
public:

    static const String EventNamespace;
    static const String WidgetTypeName;

    /** Event fired when the read-only mode for the edit box is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Editbox whose read only setting
     * has been changed.
     */
    static const String EventValidationStringChanged;
    /** Event fired when the maximum allowable string length is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Editbox whose maximum string length
     * has been changed.
     */
    static const String EventTextValidityChanged;
    /** Event fired when the user accepts the current text by pressing Return,
     * Enter, or Tab.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Editbox in which the user has accepted
     * the current text.
     */
    static const String EventTextAccepted;

    Editbox(const String& type, const String& name);
    virtual ~Editbox() override;

    bool performPaste(Clipboard& clipboard) override;
    void setMaxTextLength(size_t max_len) override;

    /*!
    \brief
        return the validation RegexMatchState for the current Editbox text, given the
        currently set validation string.

    \note
        Validation is performed by means of a regular expression.  If the text
        matches the regex, the text is said to have passed validation.  If the
        text does not match with the regex then the text fails validation.
        The default RegexMatcher uses the pcre library to perform regular
        expression operations, details about the pattern syntax can be found
        on unix-like systems by way of <tt>man pcrepattern</tt> (or online at
        http://www.pcre.org/pcre.txt (scroll / search "PCREPATTERN(3)").
        Alternatively, see the perl regex documentation at
        http://perldoc.perl.org/perlre.html

    \return
        One of the RegexMatchState enumerated values indicating the current match state.
    */
    RegexMatchState getTextMatchState() const { return d_validatorMatchState; }

    /*!
    \brief
        return the currently set validation string

    \note
        Validation is performed by means of a regular expression.  If the text
        matches the regex, the text is said to have passed validation.  If the
        text does not match with the regex then the text fails validation.
        The default RegexMatcher uses the pcre library to perform regular
        expression operations, details about the pattern syntax can be found
        on unix-like systems by way of <tt>man pcrepattern</tt> (or online at
        http://www.pcre.org/pcre.txt (scroll / search "PCREPATTERN(3)").
        Alternatively, see the perl regex documentation at
        http://perldoc.perl.org/perlre.html

    \return
        String object containing the current validation regex data
    */
    const String& getValidationString() const { return d_validationString; }

    /*!
    \brief
        Set the text validation string.

    \note
        Validation is performed by means of a regular expression.  If the text
        matches the regex, the text is said to have passed validation.  If the
        text does not match with the regex then the text fails validation.
        The default RegexMatcher uses the pcre library to perform regular
        expression operations, details about the pattern syntax can be found
        on unix-like systems by way of <tt>man pcrepattern</tt> (or online at
        http://www.pcre.org/pcre.txt (scroll / search "PCREPATTERN(3)").
        Alternatively, see the perl regex documentation at
        http://perldoc.perl.org/perlre.html

    \param validation_string
        String object containing the validation regex data to be used.

    \return
        Nothing.
    */
    void setValidationString(const String& validation_string);

    /*!
    \brief
        Set the RegexMatcher based validator for this Editbox.

    \param matcher
        Pointer to an object that implements the RegexMatcher interface, or 0
        to restore a system supplied RegexMatcher (if support is available).

    \note
        If the previous RegexMatcher validator is one supplied via the system,
        it is deleted and replaced with the given RegexMatcher.  User supplied
        RegexMatcher objects will never be deleted by the system and you must
        ensure that the object is not deleted while the Editbox holds a pointer
        to it.  Once the Editbox is destroyed or the validator is set to
        something else it is the responsibility of client code to ensure any
        previous custom validator is deleted.
    */
    void setValidator(RegexMatcher* matcher);

    /*!
    \brief
        Sets the horizontal text formatting to be used from now onwards.

    \param format
        Specifies the formatting to use.  Currently can only be one of the
        following HorizontalTextFormatting values:
            - HorizontalTextFormatting::LeftAligned (default)
            - HorizontalTextFormatting::RightAligned
            - HorizontalTextFormatting::CentreAligned
    */
    virtual void setTextFormatting(HorizontalTextFormatting format) override;

    virtual void ensureCaretIsVisible() override;

protected:

    size_t getTextIndexFromPosition(const glm::vec2& pt) const override;
    void eraseSelectedText(bool modify_text = true) override;
    void handleBackspace() override;
    void handleDelete() override;
    void onCharacter(TextEventArgs& e) override;
    void onSemanticInputEvent(SemanticEventArgs& e) override;

    bool validateWindowRenderer(const WindowRenderer* renderer) const override;

    /*!
    \brief
        Event fired internally when the validation string is changed.
    */
    virtual void onValidationStringChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when something has caused the validity state of the
        current text to change.
    */
    virtual void onTextValidityChanged(RegexMatchStateEventArgs& e);

    /*!
    \brief
        Handler called when the user accepts the edit box text by pressing
        Return, Enter, or Tab.
    */
    virtual void onTextAcceptedEvent(WindowEventArgs& e);

    /*!
    \brief
        return the match state of the given string for the validation regular
        expression.
    */
    RegexMatchState getStringMatchState(const String& str) const;

    /** Helper to update validator match state as needed for the given string
     * and event handler return codes.
     *
     * This effectively asks permission from event handlers to proceed with the
     * change, updates d_validatorMatchState and returns an appropriate bool.
     * The return value basically says whether or not to set the input string
     * as the current text for the Editbox.
     */
    bool handleValidityChangeForString(const String& str);

    //! Copy of validation reg-ex string.
    String d_validationString;
    //! Pointer to class used for validation of text.
    RegexMatcher* d_validator = nullptr;
    //! specifies whether validator was created by us, or supplied by user.
    bool d_weOwnValidator = true;
    //! Current match state of EditboxText
    RegexMatchState d_validatorMatchState = RegexMatchState::Valid;
    //! Previous match state change response
    bool d_previousValidityChangeResponse = true;

private:

    void addEditboxProperties();
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
