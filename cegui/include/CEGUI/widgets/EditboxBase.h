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

#include "CEGUI/Base.h"
#include "CEGUI/Window.h"

#include <cstdint>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
class UndoHandler;

//----------------------------------------------------------------------------//

//! Base class for an Editbox widget
class CEGUIEXPORT EditboxBase : public Window
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
    static const String EventTextMaskingEnabledChanged;
    /** Event fired whrn the code point (character) used for masked text is
     * changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Editbox whose text masking codepoint
     * has been changed.
     */
    static const String EventTextMaskingCodepointChanged;
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
    /** Event fired when the validity of the Exitbox text (as determined by a
     * RegexMatcher object) has changed.
     * Handlers are passed a const RegexMatchStateEventArgs reference with
     * WindowEventArgs::window set to the Editbox whose text validity has
     * changed and RegexMatchStateEventArgs::matchState set to the new match
     * validity. Handler return is significant, as follows:
     * - true indicates the new state - and therfore text - is to be accepted.
     * - false indicates the new state is not acceptable, and the previous text
     *   should remain in place. NB: This is only possible when the validity
     *   change is due to a change in the text, if the validity change is due to
     *   a change in the validation regular expression string, then returning
     *   false will have no effect.
     */
    static const String EventTextValidityChanged;
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
    /** Event fired when the user accepts the current text by pressing Return,
     * Enter, or Tab.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Editbox in which the user has accepted
     * the current text.
     */
    static const String EventTextAccepted;
    /** Mouse cursor image property name to use when the edit box is
     * in read-only mode.
     */
    static const String ReadOnlyCursorImagePropertyName;

    //! Constructor for Editbox class.
    EditboxBase(const String& type, const String& name);

    //! Destructor for Editbox class.
    virtual ~EditboxBase();

    /*!
    \brief
        return true if the Editbox has input focus.

    \return
        - true if the Editbox has input focus.
        - false if the Editbox does not have input focus.
    */
    virtual bool hasInputFocus() const;

    bool performPaste(Clipboard& clipboard) override = 0;

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
    size_t getCaretIndex() const;

    /*!
    \brief
        return the current selection start point.

    \return
        Index of the selection start point relative to the start of the text.
        If no selection is defined this function returns the position of the
        caret.
    */
    size_t getSelectionStart() const;

    /*!
    \brief
        return the current selection end point.

    \return
        Index of the selection end point relative to the start of the text.  If
        no selection is defined this function returns the position of the caret.
    */
    size_t getSelectionEnd() const;

    /*!
    \brief
        return the length of the current selection (in code points /
        characters).

    \return
        Number of code points (or characters) contained within the currently
        defined selection.
    */
    size_t getSelectionLength() const;

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
    virtual void setCaretIndex(size_t caret_pos) = 0;

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
    virtual void setMaxTextLength(size_t max_len) = 0;

    //! \copydoc Window::performCopy
    bool performCopy(Clipboard& clipboard) override;

    //! \copydoc Window::performCut
    bool performCut(Clipboard& clipboard) override;

    //! \copydoc Window::setEnabled
    void setEnabled(bool enabled) override;
 
    //! \copydoc Window::performUndo
    bool performUndo() override;

    //! \copydoc Window::performRedo
    bool performRedo() override;

protected:
    // Inherited methods
    bool validateWindowRenderer(const WindowRenderer* renderer) const override = 0;

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

    /*!
    \brief
        Erase the currently selected text.

    \param modify_text
        when true, the actual text will be modified.  When false, everything is
        done except erasing the characters.
    */
    virtual void eraseSelectedText(bool modify_text = true) = 0;

	/*!
	\brief
		Processing for Backspace key
	*/
    virtual void handleBackspace() = 0;

	/*!
	\brief
		Processing for Delete key
	*/
    virtual void handleDelete() = 0;

    //! Clear the currently defined selection (just the region, not the text).
    void clearSelection();

    /*!
    \brief
        Processing to move caret one character left

    \param select
        when true, the left character will be also selected
    */
    void handleCharLeft(bool select);

    /*!
    \brief
        Processing to move caret one character right

    \param select
        when true, the right character will be also selected
    */
    void handleCharRight(bool select);

    /*!
    \brief
        Processing to move caret one word left

    \param select
        If true the left word will be also selected
    */
    void handleWordLeft(bool select);

    /*!
    \brief
        Processing to move caret one word right

    \param select
        when true, the right word will be also selected
    */
    void handleWordRight(bool select);

    /*!
    \brief
        Processing to move caret to the start of the entire text.

    \param select
        When true, the text until the beginning of the entire text will be also selected
    */
    void handleHome(bool select);

    /*!
    \brief
        Processing to move caret to the end of the entire text.

    \param select
        When true, the text until the end of the entire text will be also selected
    */
    void handleEnd(bool select);

    /*!
    \brief
        Selects the entire text.
    */
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

    /*!
    \brief
        Processes Semantic Value that can be handled the same way for all Editboxes. If a
        general Semantic Value was handled this will return true, otherwise it returns false.
    \param e
        The SemanticEventArgs with the semantic input event to handle
    \return
        True if a Semantic Value was of a general type and thus handled, False if not.
    */
    bool handleBasicSemanticValue(SemanticEventArgs& e);

    // Overridden event handlers
    void onCursorPressHold(CursorInputEventArgs& e) override;
    void onCursorActivate(CursorInputEventArgs& e) override;
    void onCursorMove(CursorInputEventArgs& e) override;
    void onCaptureLost(WindowEventArgs& e) override;

    void onCharacter(TextEventArgs& e) override = 0;
    void onTextChanged(WindowEventArgs& e) override = 0;
    void onSemanticInputEvent(SemanticEventArgs& e) override = 0;

    //! True if the editbox is in read-only mode
    bool d_readOnly;
    //! The read only mouse cursor image.
    const Image* d_readOnlyCursorImage;
    //! True if the editbox text should be rendered masked.
    bool d_textMaskingEnabled;
    //! Code point to use when rendering masked text.
    std::uint32_t d_textMaskingCodepoint;
    //! Maximum number of characters for this Editbox.
    size_t d_maxTextLen;
    /*! Position of the caret / insert-point, relative to the beginning
        of the string and measured in Unicode code units (or ASCII characters).
    */
    size_t d_caretPos;
    //! Start of selection area.
    size_t d_selectionStart;
    //! End of selection area.
    size_t d_selectionEnd;
    //! true when a selection is being dragged.
    bool d_dragging;
    //! Selection index for drag selection anchor point.
    size_t d_dragAnchorIdx;
    //! Undo handler
    UndoHandler *d_undoHandler;

private:
    void addEditboxBaseProperties();
};

bool isSelectionSemanticValue(SemanticValue value);

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIEditbox_h_
