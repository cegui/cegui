/***********************************************************************
	created:	30/6/2004
	author:		Paul D Turner

	purpose:	Implementation of the Multi-line edit box base class
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
#include "CEGUI/widgets/MultiLineEditbox.h"
#include "CEGUI/widgets/Scrollbar.h"
#include "CEGUI/TextUtils.h"
#include "CEGUI/Image.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/CoordConverter.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/Clipboard.h"
#include "CEGUI/UndoHandler.h"


namespace CEGUI
{
const String MultiLineEditbox::EventNamespace("MultiLineEditbox");
const String MultiLineEditbox::WidgetTypeName("CEGUI/MultiLineEditbox");


MultiLineEditboxWindowRenderer::MultiLineEditboxWindowRenderer(const String& name) :
    WindowRenderer(name, MultiLineEditbox::EventNamespace)
{
}


void MultiLineEditboxWindowRenderer::onLookNFeelAssigned()
{
    assert(d_window != nullptr);

    // ensure window's text has a terminating \n
    String text(d_window->getText());
    if (text.empty() || text[text.length() - 1] != '\n')
    {
        text.append(1, '\n');
        d_window->setText(text);
    }
}

// event names
const String MultiLineEditbox::EventReadOnlyModeChanged( "ReadOnlyModeChanged" );
const String MultiLineEditbox::EventWordWrapModeChanged( "WordWrapModeChanged" );
const String MultiLineEditbox::EventMaximumTextLengthChanged( "MaximumTextLengthChanged" );
const String MultiLineEditbox::EventCaretMoved( "CaretMoved" );
const String MultiLineEditbox::EventTextSelectionChanged( "TextSelectionChanged" );
const String MultiLineEditbox::EventEditboxFull( "EditboxFull" );
const String MultiLineEditbox::EventVertScrollbarModeChanged( "VertScrollbarModeChanged" );
const String MultiLineEditbox::EventHorzScrollbarModeChanged( "HorzScrollbarModeChanged" );

// Static data initialisation
String MultiLineEditbox::d_lineBreakChars("\n");

const String MultiLineEditbox::VertScrollbarName( "__auto_vscrollbar__" );
const String MultiLineEditbox::HorzScrollbarName( "__auto_hscrollbar__" );


MultiLineEditbox::MultiLineEditbox(const String& type, const String& name) :
	EditboxBase(type, name),
	d_wordWrap(true),
	d_lastRenderWidth(0.0),
	d_widestExtent(0.0f),
	d_forceVertScroll(false),
	d_forceHorzScroll(false),
	d_selectionBrush(nullptr)
{
	addMultiLineEditboxProperties();
}


MultiLineEditbox::~MultiLineEditbox(void)
{
}


void MultiLineEditbox::initialiseComponents(void)
{
	// create the component sub-widgets
	Scrollbar* vertScrollbar = getVertScrollbar();
	Scrollbar* horzScrollbar = getHorzScrollbar();

    vertScrollbar->subscribeEvent(
        Window::EventShown, Event::Subscriber(
            &MultiLineEditbox::handle_vertScrollbarVisibilityChanged, this));

    vertScrollbar->subscribeEvent(
        Window::EventHidden, Event::Subscriber(
            &MultiLineEditbox::handle_vertScrollbarVisibilityChanged, this));

    vertScrollbar->subscribeEvent(Scrollbar::EventScrollPositionChanged, Event::Subscriber(&MultiLineEditbox::handle_scrollChange, this));
    horzScrollbar->subscribeEvent(Scrollbar::EventScrollPositionChanged, Event::Subscriber(&MultiLineEditbox::handle_scrollChange, this));

	formatText(true);
	performChildWindowLayout();
}


void MultiLineEditbox::setCaretIndex(size_t caret_pos)
{
	// make sure new position is valid
    if (caret_pos > getText().length() - 1)
	{
       caret_pos = getText().length() - 1;
	}

	// if new position is different
	if (d_caretPos != caret_pos)
	{
		d_caretPos = caret_pos;
		ensureCaretIsVisible();

		// Trigger "caret moved" event
		WindowEventArgs args(this);
		onCaretMoved(args);
	}

}


void MultiLineEditbox::ensureCaretIsVisible(void)
{
    Scrollbar* vertScrollbar = getVertScrollbar();
    Scrollbar* horzScrollbar = getHorzScrollbar();

	// calculate the location of the caret
	const Font* fnt = getFont();
	size_t caretLine = getLineNumberFromIndex(d_caretPos);

	if (caretLine < d_lines.size())
	{
		Rectf textArea(getTextRenderArea());

		size_t caretLineIdx = d_caretPos - d_lines[caretLine].d_startIdx;

		float ypos = caretLine * fnt->getLineSpacing();


        float xpos = 0;
        String caretLineSubstr = getText().substr(d_lines[caretLine].d_startIdx, caretLineIdx);
#if (CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_8)
        xpos = fnt->getTextAdvance(caretLineSubstr);
#else
        if(caretLineSubstr.isUtf8StringValid())
        {
            xpos = fnt->getTextAdvance(caretLineSubstr);
        }
        else
        {
            xpos = 0;
        }
#endif

		// adjust position for scroll bars
		xpos -= horzScrollbar->getScrollPosition();
		ypos -= vertScrollbar->getScrollPosition();

		// if caret is above window, scroll up
		if (ypos < 0)
		{
			vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition() + ypos);
		}
		// if caret is below the window, scroll down
		else if ((ypos += fnt->getLineSpacing()) > textArea.getHeight())
		{
			vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition() + (ypos - textArea.getHeight()) + fnt->getLineSpacing());
		}

		// if caret is left of the window, scroll left
		if (xpos < 0)
		{
			horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition() + xpos - 50);
		}
		// if caret is right of the window, scroll right
		else if (xpos > textArea.getWidth())
		{
			horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition() + (xpos - textArea.getWidth()) + 50);
		}

	}

}


void MultiLineEditbox::setWordWrapping(bool setting)
{
	if (setting != d_wordWrap)
	{
		d_wordWrap = setting;
		formatText(true);

		WindowEventArgs args(this);
		onWordWrapModeChanged(args);
	}

}


void MultiLineEditbox::configureScrollbars(void)
{
    Scrollbar* const vertScrollbar = getVertScrollbar();
    Scrollbar* const horzScrollbar = getHorzScrollbar();
    const float lspc = getFont()->getLineSpacing();

    //
    // First show or hide the scroll bars as needed (or requested)
    //
    // show or hide vertical scroll bar as required (or as specified by option)
    if (d_forceVertScroll ||
        (static_cast<float>(d_lines.size()) * lspc > getTextRenderArea().getHeight()))
    {
        vertScrollbar->show();

        // show or hide horizontal scroll bar as required (or as specified by option)
        horzScrollbar->setVisible(d_forceHorzScroll ||
                                  (d_widestExtent > getTextRenderArea().getWidth()));
    }
    // show or hide horizontal scroll bar as required (or as specified by option)
    else if (d_forceHorzScroll ||
             (d_widestExtent > getTextRenderArea().getWidth()))
    {
        horzScrollbar->show();

        // show or hide vertical scroll bar as required (or as specified by option)
        vertScrollbar->setVisible(d_forceVertScroll ||
            (static_cast<float>(d_lines.size()) * lspc > getTextRenderArea().getHeight()));
    }
    else
    {
        vertScrollbar->hide();
        horzScrollbar->hide();
    }

	//
	// Set up scroll bar values
	//
	Rectf renderArea(getTextRenderArea());

	vertScrollbar->setDocumentSize(static_cast<float>(d_lines.size()) * lspc);
	vertScrollbar->setPageSize(renderArea.getHeight());
	vertScrollbar->setStepSize(std::max(1.0f, renderArea.getHeight() / 10.0f));
	vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition());

	horzScrollbar->setDocumentSize(d_widestExtent);
	horzScrollbar->setPageSize(renderArea.getWidth());
	horzScrollbar->setStepSize(std::max(1.0f, renderArea.getWidth() / 10.0f));
	horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition());
}


void MultiLineEditbox::formatText(const bool update_scrollbars)
{
	d_widestExtent = 0.0f;

	String paraText;

	const Font* fnt = getFont();

	if (fnt)
	{
		float areaWidth = getTextRenderArea().getWidth();
        String::size_type   currPos = 0;
        String::size_type   paraLen;
        LineInfo line{};

        d_lines.clear();

        while (currPos < getText().length())
		{
           if ((paraLen = getText().find_first_of(d_lineBreakChars, currPos)) == String::npos)
			{
               paraLen = getText().length() - currPos;
			}
			else
			{
				++paraLen -= currPos;
			}

            paraText = getText().substr(currPos, paraLen);

			if (!d_wordWrap || (areaWidth <= 0.0f))
			{
				// no word wrapping, so we are just one long line.
				line.d_startIdx = currPos;
				line.d_length	= paraLen;
				line.d_extent	= fnt->getTextExtent(paraText);
				d_lines.push_back(line);

				// update widest, if needed.
				if (line.d_extent > d_widestExtent)
				{
					d_widestExtent = line.d_extent;
				}

			}
			// must word-wrap the paragraph text
			else
			{
				String::size_type lineIndex = 0;

				// while there is text in the string
				while (lineIndex < paraLen)
				{
					String::size_type  lineLen = 0;
					float lineExtent = 0.0f;

					// loop while we have not reached the end of the paragraph string
					while (lineLen < (paraLen - lineIndex))
					{
						// get cp / char count of next token
						size_t nextTokenSize = getNextTokenLength(paraText, lineIndex + lineLen);

						// get pixel width of the token
						float tokenExtent  = fnt->getTextExtent(paraText.substr(lineIndex + lineLen, nextTokenSize));

						// would adding this token would overflow the available width
						if ((lineExtent + tokenExtent) > areaWidth)
						{
							// Was this the first token?
							if (lineLen == 0)
							{
								// get point at which to break the token
								lineLen = fnt->getCharAtPixel(paraText.substr(lineIndex, nextTokenSize), areaWidth);
							}

							// text wraps, exit loop early with line info up until wrap point
							break;
						}

						// add this token to current line
						lineLen    += nextTokenSize;
						lineExtent += tokenExtent;
					}

					// set up line info and add to collection
					line.d_startIdx = currPos + lineIndex;
					line.d_length	= lineLen;
					line.d_extent	= lineExtent;
					d_lines.push_back(line);

					// update widest, if needed.
					if (lineExtent > d_widestExtent)
					{
						d_widestExtent = lineExtent;
					}

					// update position in string
					lineIndex += lineLen;
				}

			}

			// skip to next 'paragraph' in text
			currPos += paraLen;
		}

		d_lastRenderWidth = areaWidth;
	}

    if (update_scrollbars)
        configureScrollbars();

    invalidate();
}


size_t MultiLineEditbox::getNextTokenLength(const String& text, size_t start_idx) const
{
	String::size_type pos = text.find_first_of(TextUtils::DefaultWrapDelimiters, start_idx);

	// handle case where no more whitespace exists (so this is last token)
	if (pos == String::npos)
	{
		return (text.length() - start_idx);
	}
	// handle 'delimiter' token cases
	else if ((pos - start_idx) == 0)
	{
		return 1;
	}
	else
	{
		return (pos - start_idx);
	}

}


size_t MultiLineEditbox::getTextIndexFromPosition(const glm::vec2& pt) const
{
	//
	// calculate final window position to be checked
	//
    glm::vec2 wndPt = CoordConverter::screenToWindow(*this, pt);

	Rectf textArea(getTextRenderArea());

    wndPt -= glm::vec2(textArea.d_min.x, textArea.d_min.y);

	// factor in scroll bar values
    wndPt.x += getHorzScrollbar()->getScrollPosition();
    wndPt.y += getVertScrollbar()->getScrollPosition();

	size_t lineNumber = static_cast<size_t>(
        std::max(0.0f, wndPt.y) / getFont()->getLineSpacing());

	if (lineNumber >= d_lines.size())
	{
		lineNumber = d_lines.size() - 1;
	}

    const String lineText(getText().substr(d_lines[lineNumber].d_startIdx, d_lines[lineNumber].d_length));

    size_t lineIdx = getFont()->getCharAtPixel(lineText, wndPt.x);

	if (lineIdx >= lineText.length() - 1)
	{
		lineIdx = lineText.length() - 1;
	}

	return d_lines[lineNumber].d_startIdx + lineIdx;
}


size_t MultiLineEditbox::getLineNumberFromIndex(size_t index) const
{
	size_t lineCount = d_lines.size();

	if (lineCount == 0)
	{
		return 0;
	}
    else if (index >= getText().length() - 1)
	{
		return lineCount - 1;
	}
	else
	{
		size_t indexCount = 0;
		size_t caretLine = 0;

		for (; caretLine < lineCount; ++caretLine)
		{
			indexCount += d_lines[caretLine].d_length;

			if (index < indexCount)
			{
				return caretLine;
			}

		}

	}

	throw InvalidRequestException(
        "Unable to identify a line from the given, invalid, index.");
}


void MultiLineEditbox::eraseSelectedText(bool modify_text)
{
	if (getSelectionLength() != 0)
	{
		// setup new caret position and remove selection highlight.
		setCaretIndex(getSelectionStart());

		// erase the selected characters (if required)
		if (modify_text)
		{
            String newText = getText();
            UndoHandler::UndoAction undo;
            undo.d_type = UndoHandler::UndoActionType::Delete;
            undo.d_startIdx = getSelectionStart();
            undo.d_text = newText.substr(getSelectionStart(), getSelectionLength());
            d_undoHandler->addUndoHistory(undo);
            newText.erase(getSelectionStart(), getSelectionLength());
            setText(newText);

			// trigger notification that text has changed.
			WindowEventArgs args(this);
			onTextChanged(args);
		}

		clearSelection();
	}

}


bool MultiLineEditbox::performPaste(Clipboard& clipboard)
{
    if (isReadOnly())
        return false;

    String clipboardText = clipboard.getText();

    if (clipboardText.empty())
        return false;

    // backup and erase selected text
    String tmp(getText());
    tmp.erase(getSelectionStart(), getSelectionLength());
    eraseSelectedText(false);

    // if there is room
    if (tmp.length() + clipboardText.length() < d_maxTextLen)
    {
        String newText = getText();
        tmp.insert(getCaretIndex(), clipboardText);
        UndoHandler::UndoAction undo;
        undo.d_type = UndoHandler::UndoActionType::Insert;
        undo.d_startIdx = getCaretIndex();
        undo.d_text = clipboardText;
        d_undoHandler->addUndoHistory(undo);
        newText.insert(getCaretIndex(), clipboardText);
        setText(newText);

        d_caretPos += clipboardText.length();

        return true;
    }
    else
    {
        // Trigger text box full event
        WindowEventArgs args(this);
        onEditboxFullEvent(args);

        return false;
    }
}


void MultiLineEditbox::handleBackspace()
{
    if (!isReadOnly())
    {
        String newText(getText());

        if (getSelectionLength() != 0)
        {
            newText.erase(getSelectionStart(), getSelectionLength());
            // erase selection using mode that does not modify getText()
            // (we just want to update state)
            eraseSelectedText(false);
            setText(newText);
        }
        else if (d_caretPos > 0)
        {
            UndoHandler::UndoAction undo;
            undo.d_type = UndoHandler::UndoActionType::Delete;

#if CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_8
            size_t deleteStartPos = d_caretPos - 1;
            size_t deleteLength = 1;
#else
            String::codepoint_iterator caretIter(newText.begin() + d_caretPos,
                                                 newText.begin(), newText.end());
            --caretIter;
            
            size_t deleteStartPos = caretIter.getCodeUnitIndexFromStart();
            size_t deleteLength = d_caretPos - deleteStartPos;
#endif

            undo.d_startIdx = deleteStartPos;
            undo.d_text = newText.substr(deleteStartPos, deleteLength);
            d_undoHandler->addUndoHistory(undo);
            newText.erase(deleteStartPos, deleteLength);
            setCaretIndex(deleteStartPos);
            setText(newText);
        }
    }
}


void MultiLineEditbox::handleDelete()
{
    if (!isReadOnly())
    {
        String newText(getText());

        if (getSelectionLength() != 0)
        {
            newText.erase(getSelectionStart(), getSelectionLength());
            // erase selection using mode that does not modify getText()
            // (we just want to update state)
            eraseSelectedText(false);
            setText(newText);
        }
        else if (getCaretIndex() < getText().length() - 1)
        {
            UndoHandler::UndoAction undo;
            undo.d_type = UndoHandler::UndoActionType::Delete;
            undo.d_startIdx = d_caretPos;

#if CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_8
            size_t eraseLength = 1;
#else
            size_t eraseLength = String::getCodePointSize(newText[d_caretPos]);
#endif

            undo.d_text = newText.substr(d_caretPos, eraseLength);
            d_undoHandler->addUndoHistory(undo);
            newText.erase(d_caretPos, eraseLength);
            setText(newText);

            ensureCaretIsVisible();
        }
    }
}


void MultiLineEditbox::handleLineHome(bool select)
{
	size_t line = getLineNumberFromIndex(d_caretPos);

	if (line < d_lines.size())
	{
		size_t lineStartIdx = d_lines[line].d_startIdx;

		if (d_caretPos > lineStartIdx)
		{
			setCaretIndex(lineStartIdx);
		}

        if (select)
		{
			setSelection(d_caretPos, d_dragAnchorIdx);
		}
		else
		{
			clearSelection();
		}
	}
}


void MultiLineEditbox::handleLineEnd(bool select)
{
	size_t line = getLineNumberFromIndex(d_caretPos);

	if (line < d_lines.size())
	{
		size_t lineEndIdx = d_lines[line].d_startIdx + d_lines[line].d_length - 1;

		if (d_caretPos < lineEndIdx)
		{
			setCaretIndex(lineEndIdx);
		}

        if (select)
		{
			setSelection(d_caretPos, d_dragAnchorIdx);
		}
		else
		{
			clearSelection();
		}
	}
}


void MultiLineEditbox::handleLineUp(bool select)
{
	size_t caretLine = getLineNumberFromIndex(d_caretPos);

	if (caretLine > 0)
	{
        float caretPixelOffset = getFont()->getTextAdvance(getText().substr(d_lines[caretLine].d_startIdx, d_caretPos - d_lines[caretLine].d_startIdx));

		--caretLine;

        size_t newLineIndex = getFont()->getCharAtPixel(getText().substr(d_lines[caretLine].d_startIdx, d_lines[caretLine].d_length), caretPixelOffset);

		setCaretIndex(d_lines[caretLine].d_startIdx + newLineIndex);
	}

    if (select)
	{
		setSelection(d_caretPos, d_dragAnchorIdx);
	}
	else
	{
		clearSelection();
	}
}


void MultiLineEditbox::handleLineDown(bool select)
{
	size_t caretLine = getLineNumberFromIndex(d_caretPos);

	if ((d_lines.size() > 1) && (caretLine < (d_lines.size() - 1)))
	{
        float caretPixelOffset = getFont()->getTextAdvance(getText().substr(d_lines[caretLine].d_startIdx, d_caretPos - d_lines[caretLine].d_startIdx));

		++caretLine;

        size_t newLineIndex = getFont()->getCharAtPixel(getText().substr(d_lines[caretLine].d_startIdx, d_lines[caretLine].d_length), caretPixelOffset);

		setCaretIndex(d_lines[caretLine].d_startIdx + newLineIndex);
	}

    if (select)
	{
		setSelection(d_caretPos, d_dragAnchorIdx);
	}
	else
	{
		clearSelection();
	}
}


void MultiLineEditbox::handleNewLine()
{
    if (!isReadOnly())
    {
        // erase selected text
        String newText(getText());
        newText.erase(getSelectionStart(), getSelectionLength());
        eraseSelectedText(false);

        // if there is room
        if (newText.length() - 1 < d_maxTextLen)
        {
            UndoHandler::UndoAction undo;
            undo.d_type = UndoHandler::UndoActionType::Insert;
            undo.d_startIdx = getCaretIndex();
            undo.d_text = "\x0a";
            d_undoHandler->addUndoHistory(undo);
            newText.insert(getCaretIndex(), 1, static_cast<String::value_type>(0x0a));
            d_caretPos++;
        }

        setText(newText);
    }
}


void MultiLineEditbox::handlePageUp(bool select)
{
    size_t caretLine = getLineNumberFromIndex(d_caretPos);
    size_t nbLine = static_cast<size_t>(getTextRenderArea().getHeight() / getFont()->getLineSpacing());
    size_t newline = 0;
    if (nbLine < caretLine)
    {
        newline = caretLine - nbLine;
    }

    setCaretIndex(d_lines[newline].d_startIdx);

    if (select)
    {
        setSelection(d_caretPos, d_selectionEnd);
    }
    else
    {
        clearSelection();
    }

    ensureCaretIsVisible();
}


void MultiLineEditbox::handlePageDown(bool select)
{
    size_t caretLine = getLineNumberFromIndex(d_caretPos);
    size_t nbLine =  static_cast<size_t>(getTextRenderArea().getHeight() / getFont()->getLineSpacing());
    size_t newline = caretLine + nbLine;
    if (!d_lines.empty())
    {
        newline = std::min(newline,d_lines.size() - 1);
    }
    setCaretIndex(d_lines[newline].d_startIdx + d_lines[newline].d_length - 1);
    if (select)
    {
        setSelection(d_selectionStart, d_caretPos);
    }
    else
    {
        clearSelection();
    }
    ensureCaretIsVisible();
}


void MultiLineEditbox::onCharacter(TextEventArgs& e)
{
    // NB: We are not calling the base class handler here because it propagates
    // inputs back up the window hierarchy, whereas, as a consumer of input
    // events, we want such propagation to cease with us regardless of whether
    // we actually handle the event.

    // fire event.
    fireEvent(EventCharacterKey, e, Window::EventNamespace);

    // only need to take notice if we have focus
    if (e.handled == 0 && hasInputFocus() && !isReadOnly() &&
        getFont()->isCodepointAvailable(e.d_character))
    {
        // erase selected text
        String newText(getText());
        newText.erase(getSelectionStart(), getSelectionLength());
        eraseSelectedText(false);

        // if there is room
        if (newText.length() - 1 < d_maxTextLen)
        {
            UndoHandler::UndoAction undo;
            undo.d_type = UndoHandler::UndoActionType::Insert;
            undo.d_startIdx = getCaretIndex();
            undo.d_text = e.d_character;
            d_undoHandler->addUndoHistory(undo);
            newText.insert(getCaretIndex(), 1, e.d_character);

            d_caretPos++;
            setText(newText);
            ++e.handled;
        }
        else
        {
            setText(newText);
        }
    }
    else
    {
        // Trigger text box full event
        WindowEventArgs args(this);
        onEditboxFullEvent(args);
    }
}


void MultiLineEditbox::onTextChanged(WindowEventArgs& e)
{
    // ensure last character is a new line
    if ((getText().length() == 0) || (getText()[getText().length() - 1] != '\n'))
    {
        String newText = getText();
        newText.append(1, '\n');
        setText(newText);
    }


    // base class processing
    Window::onTextChanged(e);

    // clear selection
    clearSelection();
    // layout new text
    formatText(true);
    // layout child windows (scrollbars) since text layout may have changed
    performChildWindowLayout();
    // ensure caret is still within the text
    setCaretIndex(getCaretIndex());
    // ensure caret is visible
    // NB: this will already have been called at least once, but since we
    // may have changed the formatting of the text, it needs to be called again.
    ensureCaretIsVisible();

    ++e.handled;
}


void MultiLineEditbox::onSized_impl(ElementEventArgs& e)
{
	formatText(true);

	// base class handling
	Window::onSized_impl(e);

	++e.handled;
}


void MultiLineEditbox::onScroll(CursorInputEventArgs& e)
{
	// base class processing.
	Window::onScroll(e);

    Scrollbar* vertScrollbar = getVertScrollbar();
    Scrollbar* horzScrollbar = getHorzScrollbar();

	if (vertScrollbar->isEffectiveVisible() && (vertScrollbar->getDocumentSize() > vertScrollbar->getPageSize()))
	{
		vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition() + vertScrollbar->getStepSize() * -e.scroll);
	}
	else if (horzScrollbar->isEffectiveVisible() && (horzScrollbar->getDocumentSize() > horzScrollbar->getPageSize()))
	{
		horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition() + horzScrollbar->getStepSize() * -e.scroll);
	}

	++e.handled;
}


void MultiLineEditbox::onFontChanged(WindowEventArgs& e)
{
    Window::onFontChanged(e);
    formatText(true);
}


bool MultiLineEditbox::validateWindowRenderer(const WindowRenderer* renderer) const
{
	return dynamic_cast<const MultiLineEditboxWindowRenderer*>(renderer) != nullptr;
}


void MultiLineEditbox::onVertScrollbarModeChanged(WindowEventArgs& e)
{
	invalidate();
	fireEvent(EventVertScrollbarModeChanged, e, EventNamespace);
}


void MultiLineEditbox::onHorzScrollbarModeChanged(WindowEventArgs& e)
{
	invalidate();
	fireEvent(EventHorzScrollbarModeChanged, e, EventNamespace);
}


bool MultiLineEditbox::isWordWrapped(void) const
{
	return d_wordWrap;
}


void MultiLineEditbox::addMultiLineEditboxProperties(void)
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(MultiLineEditbox, bool,
        "WordWrap", "Property to get/set the word-wrap setting of the edit box.  Value is either \"true\" or \"false\".",
        &MultiLineEditbox::setWordWrapping, &MultiLineEditbox::isWordWrapped, true /* TODO: Inconsistency */
    );

    CEGUI_DEFINE_PROPERTY(MultiLineEditbox, Image*,
        "SelectionBrushImage", "Property to get/set the selection brush image for the editbox.  Value should be \"set:[imageset name] image:[image name]\".",
        &MultiLineEditbox::setSelectionBrushImage, &MultiLineEditbox::getSelectionBrushImage, nullptr
        );

    CEGUI_DEFINE_PROPERTY(MultiLineEditbox, bool,
        "ForceVertScrollbar", "Property to get/set the 'always show' setting for the vertical scroll bar of the list box."
        "Value is either \"true\" or \"false\".",
        &MultiLineEditbox::setShowVertScrollbar, &MultiLineEditbox::isVertScrollbarAlwaysShown, false /* TODO: Inconsistency */
    );
}


bool MultiLineEditbox::handle_scrollChange(const EventArgs&)
{
    // simply trigger a redraw of the MultiLineEditbox
    invalidate();
    return true;
}


Scrollbar* MultiLineEditbox::getVertScrollbar() const
{
    return static_cast<Scrollbar*>(getChild(VertScrollbarName));
}


bool MultiLineEditbox::isVertScrollbarAlwaysShown(void) const
{
	return d_forceVertScroll;
}


Scrollbar* MultiLineEditbox::getHorzScrollbar() const
{
    return static_cast<Scrollbar*>(getChild(HorzScrollbarName));
}


Rectf MultiLineEditbox::getTextRenderArea() const
{
    if (d_windowRenderer != nullptr)
    {
        MultiLineEditboxWindowRenderer* wr = static_cast<MultiLineEditboxWindowRenderer*>(d_windowRenderer);
        return wr->getTextRenderArea();
    }
    else
    {
        //return getTextRenderArea_impl();
        throw InvalidRequestException(
            "This function must be implemented by the window renderer module");
    }
}


const Image* MultiLineEditbox::getSelectionBrushImage() const
{
    return d_selectionBrush;
}


void MultiLineEditbox::setSelectionBrushImage(const Image* image)
{
    d_selectionBrush = image;
    invalidate();
}


void MultiLineEditbox::setShowVertScrollbar(bool setting)
{
	if (d_forceVertScroll != setting)
	{
		d_forceVertScroll = setting;

		configureScrollbars();
		WindowEventArgs args(this);
		onVertScrollbarModeChanged(args);
	}
}


bool MultiLineEditbox::handle_vertScrollbarVisibilityChanged(const EventArgs&)
{
    if (d_wordWrap)
        formatText(false);

    return true;
}


void MultiLineEditbox::onSemanticInputEvent(SemanticEventArgs& e)
{
    if (isDisabled())
        return;

    if (e.d_semanticValue == SemanticValue::SelectAll && e.d_payload.source == CursorInputSource::Left)
    {
        handleSelectAllText(e);

        ++e.handled;
    }
    else if (e.d_semanticValue == SemanticValue::SelectWord && e.d_payload.source == CursorInputSource::Left)
    {
        d_dragAnchorIdx = TextUtils::getWordStartIdx(getText(),
            (d_caretPos == getText().length()) ? d_caretPos : d_caretPos + 1);
        d_caretPos      = TextUtils::getNextWordStartIdx(getText(), d_caretPos);

        // perform actual selection operation.
        setSelection(d_dragAnchorIdx, d_caretPos);

        ++e.handled;
    }

    if (e.handled == 0 && hasInputFocus())
    {
        if (isReadOnly())
        {
            Window::onSemanticInputEvent(e);
            return;
        }

        if (getSelectionLength() == 0 && isSelectionSemanticValue(e.d_semanticValue))
            d_dragAnchorIdx = d_caretPos;

        // Check if the semantic value to be handled is of a general type and can thus be
        // handled via common EditboxBase handlers
        bool isSemanticValueHandled = handleBasicSemanticValue(e);

        // If the semantic value was not handled, check for specific values
        if (!isSemanticValueHandled)
        {
            // We assume it will be handled now, if not it will be set to false in default-case
            isSemanticValueHandled = true;

            switch (e.d_semanticValue)
            {

            case SemanticValue::Confirm:
                handleNewLine();
                break;

            case SemanticValue::GoUp:
                handleLineUp(false);
                break;

            case SemanticValue::SelectUp:
                handleLineUp(true);
                break;

            case SemanticValue::GoDown:
                handleLineDown(false);
                break;

            case SemanticValue::SelectDown:
                handleLineDown(true);
                break;

            case SemanticValue::GoToStartOfLine:
                handleLineHome(false);
                break;

            case SemanticValue::SelectToStartOfLine:
                handleLineHome(true);
                break;

            case SemanticValue::GoToEndOfLine:
                handleLineEnd(false);
                break;

            case SemanticValue::SelectToEndOfLine:
                handleLineEnd(true);
                break;

            case SemanticValue::GoToPreviousPage:
                handlePageUp(false);
                break;

            case SemanticValue::GoToNextPage:
                handlePageDown(false);
                break;

            default:
                Window::onSemanticInputEvent(e);
                isSemanticValueHandled = false;
            }
        }

        if (isSemanticValueHandled)
            ++e.handled;
    }
}


void MultiLineEditbox::handleSelectAllText(SemanticEventArgs& e)
{
    size_t caretLine = getLineNumberFromIndex(d_caretPos);
    size_t lineStart = d_lines[caretLine].d_startIdx;

    // find end of last paragraph
    String::size_type paraStart = getText().find_last_of(d_lineBreakChars, lineStart);

    // if no previous paragraph, selection will start at the beginning.
    if (paraStart == String::npos)
        paraStart = 0;
    // find end of this paragraph
    String::size_type paraEnd = getText().find_first_of(d_lineBreakChars, lineStart);

    // if paragraph has no end, which actually should never happen, fix the
    // erroneous situation and select up to end at end of text.
    if (paraEnd == String::npos)
    {
        String newText = getText();
        newText.append(1, '\n');
        setText(newText);

        paraEnd = getText().length() - 1;
    }

    // set up selection using new values.
    d_dragAnchorIdx = paraStart;
    setCaretIndex(paraEnd);
    setSelection(d_dragAnchorIdx, d_caretPos);
    ++e.handled;
}


void MultiLineEditbox::setMaxTextLength(size_t max_len)
{
    if (d_maxTextLen != max_len)
    {
        d_maxTextLen = max_len;

        // Trigger max length changed event
        WindowEventArgs args(this);
        onMaximumTextLengthChanged(args);

        // trim string
        if (getText().length() > d_maxTextLen)
        {
            String newText = getText();
            newText.resize(d_maxTextLen);
            setText(newText);
            onTextChanged(args);
            d_undoHandler->clearUndoHistory();
        }
    }
}


void MultiLineEditbox::onWordWrapModeChanged(WindowEventArgs& e)
{
    fireEvent(EventWordWrapModeChanged, e, EventNamespace);
}


}
