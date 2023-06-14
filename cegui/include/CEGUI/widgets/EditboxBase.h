/***********************************************************************
    created:    2nd July 2015
    author:     Lukas Meindl

    purpose:    Interface for EditboxBase class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2015 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIEditboxBase_h_
#define _CEGUIEditboxBase_h_

#include "CEGUI/Window.h"
#include "CEGUI/text/RenderedText.h"
#include "CEGUI/falagard/Enums.h"
#include "CEGUI/RegexMatcher.h"
#include "CEGUI/WindowRenderer.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{
class UndoHandler;

//! Base class for editbox window renderers
class CEGUIEXPORT EditboxWindowRenderer : public WindowRenderer
{
public:

    EditboxWindowRenderer(const String& name);

    //! Editbox text parsing is forcefully disabled
    virtual bool isTextParsingEnabled() const override { return false; }

    /*!
    \brief
        Return a Rect object describing, in un-clipped pixels, the window relative area
        that the text should be rendered in to.

    \return
        Rect object describing the area of the Window to be used for rendering text.
    */
    virtual Rectf getTextRenderArea() const = 0;

    virtual Rectf getCaretRect() const = 0;
    virtual float getCaretWidth() const = 0;
};

//----------------------------------------------------------------------------//

//! Base class for an Editbox widget
class CEGUIEXPORT EditboxBase : public Window
{
public:

    static const String EventNamespace;
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
    static const String EventTextMaskingEnabledChanged;
    /** Event fired whrn the code point (character) used for masked text is
     * changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Editbox whose text masking codepoint
     * has been changed.
     */
    static const String EventTextMaskingCodepointChanged;
    /** Event fired when the maximum allowable string length is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Editbox whose maximum string length
     * has been changed.
     */
    static const String EventMaximumTextLengthChanged;
    //! Fired when the default paragraph direction of this window changes.
    static const String EventDefaultParagraphDirectionChanged;
    /** Event fired when the text caret position / insertion point is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Editbox whose current insertion point
     * has changed.
     */
    static const String EventCaretMoved;
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

    EditboxBase(const String& type, const String& name);
    virtual ~EditboxBase() override;

    /*!
    \brief
        return true if the Editbox has input focus.

    \return
        - true if the Editbox has input focus.
        - false if the Editbox does not have input focus.
    */
    virtual bool hasInputFocus() const { return isFocused(); }

    /*!
    \brief
        Returns if the Editbox is read-only.

    \return
        true if the Editbox is read-only and can't be edited by the user, false
        if the Editbox is not read-only and may be edited by the user.
    */
    bool isReadOnly() const { return d_readOnly; }

    /*!
    \brief
        return true if the text for the Editbox will be rendered masked.

    \return
        true if the Editbox text will be rendered masked using the currently set
        mask code point, false if the Editbox text will be rendered as ordinary
        text.
    */
    bool isTextMaskingEnabled() const { return d_textMaskingEnabled; }

    /*!
    \brief
        return the current position of the caret.

    \return
        Index of the insert caret relative to the start of the text.
    */
    size_t getCaretIndex() const { return d_caretPos; }

    /*!
    \brief
        return the current selection start point.

    \return
        Index of the selection start point relative to the start of the text.
        If no selection is defined this function returns the position of the
        caret.
    */
    size_t getSelectionStart() const { return (d_selectionStart != d_selectionEnd) ? d_selectionStart : d_caretPos; }

    /*!
    \brief
        return the current selection end point.

    \return
        Index of the selection end point relative to the start of the text.  If
        no selection is defined this function returns the position of the caret.
    */
    size_t getSelectionEnd() const { return (d_selectionStart != d_selectionEnd) ? d_selectionEnd : d_caretPos; }

    /*!
    \brief
        return the length of the current selection (in code points /
        characters).

    \return
        Number of code points (or characters) contained within the currently
        defined selection.
    */
    size_t getSelectionLength() const { return d_selectionEnd - d_selectionStart; }

    /*!
    \brief
        return the code point used when rendering masked text.

    \return
        Unsigned integer value representing an UTF-32 code point that will be rendered instead of
        the Editbox text when rendering in masked mode.
    */
    std::uint32_t getTextMaskingCodepoint() const { return d_textMaskingCodepoint; }

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
    size_t getMaxTextLength() const { return d_maxTextLen; }

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

    void setDragPanningEnabled(bool setting);
    bool isDragPanningEnabled() const { return d_dragPanningEnabled; }

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
    void setTextMaskingEnabled(bool setting);

    /*!
    \brief
        Set the current position of the caret.

    \param caret_pos
        New index for the insert caret relative to the start of the text.  If
        the value specified is greater than the number of characters in the
        Editbox, the caret is positioned at the end of the text.

    \return
        Nothing.
    */
    void setCaretIndex(size_t caretPos);

    //! Scroll the view so that the current caret position is visible.
    virtual void ensureCaretIsVisible() = 0;

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
        Define the current selection start for the Editbox

        \param start_pos
        Index of the starting point for the selection.  If this value is greater than the number of characters in the Editbox, the
        selection start will be set to the end of the text.

    \return
        Nothing.
    */
    void setSelectionStart(size_t start_pos);

    /*!
    \brief
        Define the current selection for the Editbox

    \param start_pos
        Length of the selection.

    \return
        Nothing.
    */
    void setSelectionLength(size_t length);

    /*!
    \brief
        set the code point used when rendering masked text.

    \param code_point
        Unsigned int value representing an UTF-32 code point that should be rendered instead of the
        Editbox text when rendering in masked mode.

    \return
        Nothing.
    */
    void setTextMaskingCodepoint(std::uint32_t code_point);

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
    virtual void setMaxTextLength(size_t maxLen);

    virtual float getTextOffsetX() const = 0;
    virtual float getTextOffsetY() const = 0;
    virtual void setTextOffsetX(float value) = 0;
    virtual void setTextOffsetY(float value) = 0;

    void setTextOffset(const glm::vec2& offset) { setTextOffsetX(offset.x); setTextOffsetY(offset.y); }
    glm::vec2 getTextOffset() const { return glm::vec2(getTextOffsetX(), getTextOffsetY()); }

    RenderedText& getRenderedText();
    const RenderedText& getRenderedText() const { return d_renderedText; }

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
    virtual void setTextFormatting(HorizontalTextFormatting format);
    HorizontalTextFormatting getTextFormatting() const;

    //! Sets the default paragraph direction for the displayed text.
    void setDefaultParagraphDirection(DefaultParagraphDirection defaultParagraphDirection);
    //! Gets the default paragraph direction for the displayed text.
    DefaultParagraphDirection getDefaultParagraphDirection() const { return d_defaultParagraphDirection; }

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

    void setEnabled(bool enabled) override;

    bool performCopy(Clipboard& clipboard) const;
    bool performCut(Clipboard& clipboard);
    bool performPaste(Clipboard& clipboard);
    bool performUndo();
    bool performRedo();

protected:

    void updateRenderedText();
    virtual void updateFormatting() = 0;

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
    size_t getTextIndexFromPosition(const glm::vec2& localPos);

    virtual bool validateWindowRenderer(const WindowRenderer* renderer) const override;
    virtual bool handleFontRenderSizeChange(const Font& font) override;

    //! Clear the currently defined selection (just the region, not the text).
    void clearSelection();

    size_t getPrevTextIndex(size_t idx) const;
    size_t getNextTextIndex(size_t idx) const;

    virtual bool insertString(String&& strToInsert);
    void deleteRange(size_t start, size_t length);
    void handleCaretMovement(size_t newIndex, bool select);

    void handleSelectAll();

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
    virtual void onTextMaskingEnabledChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the code point to use for masked rendering has been
        changed.
    */
    virtual void onTextMaskingCodepointChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the maximum text length for the edit box is changed.
    */
    virtual void onMaximumTextLengthChanged(WindowEventArgs& e);

    /*!
    \brief
        Handler called when the caret (insert point) position changes.
    */
    virtual void onCaretMoved(WindowEventArgs& e);

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

    /*!
    \brief
        return the the read-only mouse cursor image.
    \return
        The read-only mouse cursor image.
    */
    const Image* getReadOnlyCursorImage() const { return d_readOnlyCursorImage; }

    /*!
    \brief
        Set the read only mouse cursor image.
    \param image
        The Image* to be used.
    */
    void setReadOnlyCursorImage(const Image* image) { d_readOnlyCursorImage = image; }

    // Overridden event handlers
    void onMouseButtonDown(MouseButtonEventArgs& e) override;
    void onMouseButtonUp(MouseButtonEventArgs& e) override;
    void onDoubleClick(MouseButtonEventArgs& e) override;
    void onTripleClick(MouseButtonEventArgs& e) override;
    void onCursorMove(CursorMoveEventArgs& e) override;
    void onKeyDown(KeyEventArgs& e) override;
    void onKeyUp(KeyEventArgs& e) override;
    void onCaptureLost(WindowEventArgs& e) override;
    void onSized(ElementEventArgs& e) override;

    void onFontChanged(WindowEventArgs& e) override;
    void onTextChanged(WindowEventArgs& e) override;
    void onCharacter(TextEventArgs& e) override;

    virtual void processKeyDownEvent(KeyEventArgs& e);

    RenderedText d_renderedText;

    //! The read only mouse cursor image.
    const Image* d_readOnlyCursorImage = nullptr;
    //! Maximum number of characters for this Editbox.
    size_t d_maxTextLen;
    /*! Position of the caret / insert-point, relative to the beginning
        of the string and measured in Unicode code units (or ASCII characters).
    */
    size_t d_caretPos = 0;
    //! Start of selection area.
    size_t d_selectionStart = 0;
    //! End of selection area.
    size_t d_selectionEnd = 0;
    //! Selection index for drag selection anchor point.
    size_t d_dragAnchorIdx = 0;
    //! Undo handler
    std::unique_ptr<UndoHandler> d_undoHandler;

    //! Pointer to class used for validation of text.
    RegexMatcher* d_validator = nullptr;
    //! Copy of validation reg-ex string.
    String d_validationString;
    //! Current match state of EditboxText
    RegexMatchState d_validatorMatchState = RegexMatchState::Valid;

    //! Code point to use when rendering masked text.
    std::uint32_t d_textMaskingCodepoint = '*';

    //! Default direction of the paragraph, relevant for bidirectional text.
    DefaultParagraphDirection d_defaultParagraphDirection = DefaultParagraphDirection::LeftToRight;
    //! True if the editbox is in read-only mode
    bool d_readOnly = false;
    //! True if the editbox text should be rendered masked.
    bool d_textMaskingEnabled = false;
    //! true when a selection is being dragged.
    bool d_dragSelecting = false;
    //! true when a selection is being dragged.
    bool d_dragPanning = false;
    //! true when a selection is being dragged.
    bool d_dragPanningEnabled = true;

    bool d_renderedTextDirty = true;
    bool d_formattingDirty = true;

    //! specifies whether validator was created by us, or supplied by user.
    bool d_weOwnValidator = true;

private:

    void addEditboxBaseProperties();
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
