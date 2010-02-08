/***********************************************************************
    filename:   CEGUIEditbox.h
    created:    13/4/2004
    author:     Paul D Turner

    purpose:    Interface to base class for Editbox widget
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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

#include "../CEGUIBase.h"
#include "../CEGUIWindow.h"
#include "CEGUIEditboxProperties.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
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
    virtual size_t getTextIndexFromPosition(const Point& pt) const = 0;
};

//----------------------------------------------------------------------------//

//! Base class for an Editbox widget
class CEGUIEXPORT Editbox : public Window
{
public:
    //! Namespace for global events
    static const String EventNamespace;
    //! Window factory name
    static const String WidgetTypeName;
    /** Event fired when the read-only mode for the edit box is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Editbox whose read only setting
     * has been changed.
     */
    static const String EventReadOnlyModeChanged;
    /** Event fired when the masked rendering mode (password mode) is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Editbox that has been put into or
     * taken out of masked text (password) mode.
     */
    static const String EventMaskedRenderingModeChanged;
    /** Event fired whrn the code point (character) used for masked text is
     * changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Editbox whose text masking codepoint
     * has been changed.
     */
    static const String EventMaskCodePointChanged;
    /** Event fired when the validation string is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Editbox whose validation string has
     * been changed.
     */
    static const String EventValidationStringChanged;
    /** Event fired when the maximum allowable string length is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Editbox whose maximum string length
     * has been changed.
     */
    static const String EventMaximumTextLengthChanged;
    /** Event fired when the current text has become invalid as regards to the
     * validation string.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Editbox whose text has become invalid.
     */
    static const String EventTextInvalidated;
    /** Event fired when the user attempts to chage the text in a way that would
     * make it invalid as regards to the validation string.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Editbox in which the users input would
     * have invalidated the text.
     */
    static const String EventInvalidEntryAttempted;
    /** Event fired when the text caret position / insertion point is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Editbox whose current insertion point
     * has changed.
     */
    static const String EventCaratMoved;
    /** Event fired when the current text selection is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Editbox whose current text selection
     * was changed.
     */
    static const String EventTextSelectionChanged;
    /** Event fired when the number of characters in the edit box reaches the
     * currently set maximum.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Editbox that has become full.
     */
    static const String EventEditboxFull;
    /** Event fired when the user accepts the current text by pressing Return,
     * Enter, or Tab.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Editbox in which the user has accepted
     * the current text.
     */
    static const String EventTextAccepted;

    /*!
    \brief
        return true if the Editbox has input focus.

    \return
        - true if the Editbox has keyboard input focus.
        - false if the Editbox does not have keyboard input focus.
    */
    bool hasInputFocus(void) const;

    /*!
    \brief
        return true if the Editbox is read-only.

    \return
        true if the Editbox is read only and can't be edited by the user, false
        if the Editbox is not read only and may be edited by the user.
    */
    bool isReadOnly(void) const
        {return d_readOnly;}

    /*!
    \brief
        return true if the text for the Editbox will be rendered masked.

    \return
        true if the Editbox text will be rendered masked using the currently set
        mask code point, false if the Editbox text will be rendered as ordinary
        text.
    */
    bool isTextMasked(void) const
        {return d_maskText;}

    /*!
    \brief
        return true if the Editbox text is valid given the currently set
        validation string.

    \note
        It is possible to programmatically set 'invalid' text for the Editbox by
        calling setText.  This has certain implications since if invalid text is
        set, whatever the user types into the box will be rejected when the
        input is validated.

    \note
        Validation is performed by means of a regular expression.  If the text
        matches the regex, the text is said to have passed validation.  If the
        text does not match with the regex then the text fails validation.

    \return
        - true if the current Editbox text passes validation.
        - false if the text does not pass validation.
    */
    bool isTextValid(void) const;

    /*!
    \brief
        return the currently set validation string

    \note
        Validation is performed by means of a regular expression.  If the text
        matches the regex, the text is said to have passed validation.  If the
        text does not match with the regex then the text fails validation.

    \return
        String object containing the current validation regex data
    */
    const String& getValidationString(void) const
        {return d_validationString;}

    /*!
    \brief
        return the current position of the carat.

    \return
        Index of the insert carat relative to the start of the text.
    */
    size_t getCaratIndex(void) const;

    /*!
    \brief
        return the current selection start point.

    \return
        Index of the selection start point relative to the start of the text.
        If no selection is defined this function returns the position of the
        carat.
    */
    size_t getSelectionStartIndex(void) const;

    /*!
    \brief
        return the current selection end point.

    \return
        Index of the selection end point relative to the start of the text.  If
        no selection is defined this function returns the position of the carat.
    */
    size_t getSelectionEndIndex(void) const;

    /*!
    \brief
        return the length of the current selection (in code points /
        characters).

    \return
        Number of code points (or characters) contained within the currently
        defined selection.
    */
    size_t getSelectionLength(void) const;

    /*!
    \brief
        return the utf32 code point used when rendering masked text.

    \return
        utf32 code point value representing the Unicode code point that will be
        rendered instead of the Editbox text when rendering in masked mode.
    */
    utf32 getMaskCodePoint(void) const
        {return d_maskCodePoint;}

    /*!
    \brief
        return the maximum text length set for this Editbox.

    \return
        The maximum number of code points (characters) that can be entered into
        this Editbox.

    \note
        Depending on the validation string set, the actual length of text that
        can be entered may be less than the value returned here
        (it will never be more).
    */
    size_t getMaxTextLength(void) const
        {return d_maxTextLen;}

    /*!
    \brief
        Specify whether the Editbox is read-only.

    \param setting
        true if the Editbox is read only and can't be edited by the user, false
        if the Editbox is not read only and may be edited by the user.

    \return
        Nothing.
    */
    void setReadOnly(bool setting);

    /*!
    \brief
        Specify whether the text for the Editbox will be rendered masked.

    \param setting
        - true if the Editbox text should be rendered masked using the currently
          set mask code point.
        - false if the Editbox text should be rendered as ordinary text.

    \return
        Nothing.
    */
    void setTextMasked(bool setting);

    /*!
    \brief
        Set the text validation string.

    \note
        Validation is performed by means of a regular expression.  If the text
        matches the regex, the text is said to have passed validation.  If the
        text does not match with the regex then the text fails validation.

    \param validation_string
        String object containing the validation regex data to be used.

    \return
        Nothing.
    */
    void setValidationString(const String& validation_string);

    /*!
    \brief
        Set the current position of the carat.

    \param carat_pos
        New index for the insert carat relative to the start of the text.  If
        the value specified is greater than the number of characters in the
        Editbox, the carat is positioned at the end of the text.

    \return
        Nothing.
    */
    void setCaratIndex(size_t carat_pos);

    /*!
    \brief
        Define the current selection for the Editbox

    \param start_pos
        Index of the starting point for the selection.  If this value is greater
        than the number of characters in the Editbox, the selection start will
        be set to the end of the text.

    \param end_pos
        Index of the ending point for the selection.  If this value is greater
        than the number of characters in the Editbox, the selection end will be
        set to the end of the text.

    \return
        Nothing.
    */
    void setSelection(size_t start_pos, size_t end_pos);

    /*!
    \brief
        set the utf32 code point used when rendering masked text.

    \param code_point
        utf32 code point value representing the Unicode code point that should
        be rendered instead of the Editbox text when rendering in masked mode.

    \return
        Nothing.
    */
    void setMaskCodePoint(utf32 code_point);

    /*!
    \brief
        set the maximum text length for this Editbox.

    \param max_len
        The maximum number of code points (characters) that can be entered into
        this Editbox.

    \note
        Depending on the validation string set, the actual length of text that
        can be entered may be less than the value set here
        (it will never be more).

    \return
        Nothing.
    */
    void setMaxTextLength(size_t max_len);

    //! Constructor for Editbox class.
    Editbox(const String& type, const String& name);

    //! Destructor for Editbox class.
    virtual ~Editbox(void);

protected:
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
    size_t getTextIndexFromPosition(const Point& pt) const;

    //! Clear the currently defined selection (just the region, not the text).
    void clearSelection(void);

    /*!
    \brief
        Erase the currently selected text.

    \param modify_text
        when true, the actual text will be modified.  When false, everything is
        done except erasing the characters.
    */
    void eraseSelectedText(bool modify_text = true);

    /*!
    \brief
        return true if the given string matches the validation regular
        expression.
    */
    bool isStringValid(const String& str) const;

    //! Processing for backspace key
    void handleBackspace(void);

    //! Processing for Delete key
    void handleDelete(void);

    //! Processing to move carat one character left
    void handleCharLeft(uint sysKeys);

    //! Processing to move carat one word left
    void handleWordLeft(uint sysKeys);

    //! Processing to move carat one character right
    void handleCharRight(uint sysKeys);

    //! Processing to move carat one word right
    void handleWordRight(uint sysKeys);

    //! Processing to move carat to the start of the text.
    void handleHome(uint sysKeys);

    //! Processing to move carat to the end of the text
    void handleEnd(uint sysKeys);

    /*!
    \brief
        Return whether this window was inherited from the given class name at
        some point in the inheritance hierarchy.

    \param class_name
        The class name that is to be checked.

    \return
        - true if this window was inherited from \a class_name.
        - false if not.
    */
    virtual bool testClassName_impl(const String& class_name) const
    {
        if (class_name=="Editbox")	return true;
        return Window::testClassName_impl(class_name);
    }

    //! validate window renderer
    virtual bool validateWindowRenderer(const String& name) const
    {
        return (name == "Editbox");
    }

    /*!
    \brief
        Handler called when the read only state of the Editbox has been changed.
    */
    virtual void onReadOnlyChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the masked rendering mode (password mode) has been
        changed.
    */
    virtual void onMaskedRenderingModeChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the code point to use for masked rendering has been
        changed.
    */
    virtual void onMaskCodePointChanged(WindowEventArgs& e);

    /*!
    \brief
        Event fired internally when the validation string is changed.
    */
    virtual void onValidationStringChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the maximum text length for the edit box is changed.
    */
    virtual void onMaximumTextLengthChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when something has caused the current text to now fail
        validation.

        This can be caused by changing the validation string or setting a
        maximum length that causes the current text to be truncated.
    */
    virtual void onTextInvalidatedEvent(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the user attempted to make a change to the edit box
        that would have caused it to fail validation.
    */
    virtual void onInvalidEntryAttempted(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the carat (insert point) position changes.
    */
    virtual void onCaratMoved(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the current text selection changes.
    */
    virtual void onTextSelectionChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the edit box text has reached the set maximum
        length.
    */
    virtual void onEditboxFullEvent(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the user accepts the edit box text by pressing
        Return, Enter, or Tab.
    */
    virtual void onTextAcceptedEvent(WindowEventArgs& e);

    // Overridden event handlers
    void onMouseButtonDown(MouseEventArgs& e);
    void onMouseButtonUp(MouseEventArgs& e);
    void onMouseDoubleClicked(MouseEventArgs& e);
    void onMouseTripleClicked(MouseEventArgs& e);
    void onMouseMove(MouseEventArgs& e);
    void onCaptureLost(WindowEventArgs& e);
    void onCharacter(KeyEventArgs& e);
    void onKeyDown(KeyEventArgs& e);
    void onTextChanged(WindowEventArgs& e);

    //! True if the editbox is in read-only mode
    bool d_readOnly;
    //! True if the editbox text should be rendered masked.
    bool d_maskText;
    //! Code point to use when rendering masked text.
    utf32 d_maskCodePoint;
    //! Maximum number of characters for this Editbox.
    size_t d_maxTextLen;
    //! Position of the carat / insert-point.
    size_t d_caratPos;
    //! Start of selection area.
    size_t d_selectionStart;
    //! End of selection area.
    size_t d_selectionEnd;
    //! Copy of validation reg-ex string.
    String d_validationString;
    //! Pointer to class used for validation of text.
    RegexMatcher* d_validator;
    //! true when a selection is being dragged.
    bool d_dragging;
    //! Selection index for drag selection anchor point.
    size_t d_dragAnchorIdx;

private:
    static EditboxProperties::ReadOnly         d_readOnlyProperty;
    static EditboxProperties::MaskText         d_maskTextProperty;
    static EditboxProperties::MaskCodepoint    d_maskCodepointProperty;
    static EditboxProperties::ValidationString d_validationStringProperty;
    static EditboxProperties::CaratIndex       d_caratIndexProperty;
    static EditboxProperties::SelectionStart   d_selectionStartProperty;
    static EditboxProperties::SelectionLength  d_selectionLengthProperty;
    static EditboxProperties::MaxTextLength    d_maxTextLengthProperty;

    void addEditboxProperties(void);
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIEditbox_h_
