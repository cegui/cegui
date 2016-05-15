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

#include "CEGUI/RegexMatcher.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
class UndoHandler;

//! Base class for the EditboxWindowRenderer class
class CEGUIEXPORT EditboxWindowRenderer : public WindowRenderer
{
public:
    //! Constructor
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
};

//----------------------------------------------------------------------------//

//! Class for an Editbox widget
class CEGUIEXPORT Editbox : public EditboxBase
{
public:
    typedef RegexMatcher::MatchState MatchState;

    //! Namespace for global events
    static const String EventNamespace;
    //! Window factory name
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
    /** Event fired when the text caret position / insertion point is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Editbox whose current insertion point
     * has changed.
     */
    /** Mouse cursor image property name to use when the edit box is
     * in read-only mode.
     */
    static const String ReadOnlyMouseCursorImagePropertyName;

    //! Constructor for Editbox class.
    Editbox(const String& type, const String& name);

    //! Destructor for Editbox class.
    virtual ~Editbox();

    // Inherited methods
    bool performPaste(Clipboard& clipboard) override;
    void setCaretIndex(size_t caret_pos) override;
    void setMaxTextLength(size_t max_len) override;

    /*!
    \brief
        return the validation MatchState for the current Editbox text, given the
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
        One of the MatchState enumerated values indicating the current match state.
    */
    MatchState getTextMatchState() const;

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



protected:
    // Overriding methods inherited from EditboxBase
    size_t getTextIndexFromPosition(const glm::vec2& pt) const override;
    void eraseSelectedText(bool modify_text = true) override;
    void handleBackspace() override;
    void handleDelete() override;
    void onCharacter(TextEventArgs& e) override;
    void onTextChanged(WindowEventArgs& e) override;
    void onSemanticInputEvent(SemanticEventArgs& e) override;

    //! validate window renderer
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
    MatchState getStringMatchState(const String& str) const;

    /** Helper to update validator match state as needed for the given string
     * and event handler return codes.
     *
     * This effectively asks permission from event handlers to proceed with the
     * change, updates d_validatorMatchState and returns an appropriate bool.
     * The return value basically says whether or not to set the input string
     * as the current text for the Editbox.
     */
    bool handleValidityChangeForString(const String& str);



    /*!
    \brief
        return the the read-only mouse cursor image.
    \return
        The read-only mouse cursor image.
    */
    const Image* getReadOnlyMouseCursorImage(void) const
        { return d_readOnlyMouseCursorImage; }

    /*!
    \brief
        Set the read only mouse cursor image.
    \param image
        The Image* to be used.
    */
    void setReadOnlyMouseCursorImage(const Image* image)
        { d_readOnlyMouseCursorImage = image; }


    //! The read only mouse cursor image.
    const Image* d_readOnlyMouseCursorImage;
    //! Copy of validation reg-ex string.
    String d_validationString;
    //! Pointer to class used for validation of text.
    RegexMatcher* d_validator;
    //! specifies whether validator was created by us, or supplied by user.
    bool d_weOwnValidator;
    //! Current match state of EditboxText
    MatchState d_validatorMatchState;
    //! Previous match state change response
    bool d_previousValidityChangeResponse;

private:
    void addEditboxProperties();
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIEditbox_h_
