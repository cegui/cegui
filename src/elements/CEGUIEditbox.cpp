/************************************************************************
	filename: 	CEGUIEditbox.cpp
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Editbox base class widget
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
    Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "elements/CEGUIEditbox.h"
#include "CEGUITextUtils.h"
#include "CEGUIExceptions.h"
#include "CEGUIFont.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	TODO:

	Clipboard support
	Undo support
*************************************************************************/
/*************************************************************************
	Constants
*************************************************************************/
// default colours
const ulong	Editbox::DefaultNormalTextColour			= 0x00FFFFFF;
const ulong	Editbox::DefaultSelectedTextColour			= 0x00000000;
const ulong	Editbox::DefaultNormalSelectionColour		= 0x006060FF;
const ulong	Editbox::DefaultInactiveSelectionColour		= 0x00808080;


/*************************************************************************
	Event name constants
*************************************************************************/
const utf8	Editbox::ReadOnlyChanged[]				= "ReadOnlyChanged";
const utf8	Editbox::MaskedRenderingModeChanged[]	= "MaskRenderChanged";
const utf8	Editbox::MaskCodePointChanged[]			= "MaskCPChanged";
const utf8	Editbox::ValidationStringChanged[]		= "ValidatorChanged";
const utf8	Editbox::MaximumTextLengthChanged[]		= "MaxTextLenChanged";
const utf8	Editbox::TextInvalidatedEvent[]			= "TextInvalidated";
const utf8	Editbox::InvalidEntryAttempted[]		= "InvalidInputAttempt";
const utf8	Editbox::CaratMoved[]					= "TextCaratMoved";
const utf8	Editbox::TextSelectionChanged[]			= "TextSelectChanged";
const utf8	Editbox::EditboxFullEvent[]				= "EditboxFull";
const utf8	Editbox::TextAcceptedEvent[]			= "TextAccepted";


/*************************************************************************
	Constructor for Editbox class.
*************************************************************************/
Editbox::Editbox(const String& type, const String& name) :
	Window(type, name),
	d_readOnly(false),
	d_maskText(false),
	d_maskCodePoint('*'),
	d_maxTextLen(-1),		// TODO: Change to String::max_length() once that's a static
	d_caratPos(0),
	d_selectionStart(0),
	d_selectionEnd(0),
	d_dragging(false),
	d_normalTextColour(DefaultNormalTextColour),
	d_selectTextColour(DefaultSelectedTextColour),
	d_selectBrushColour(DefaultNormalSelectionColour),
	d_inactiveSelectBrushColour(DefaultInactiveSelectionColour)
{
	addEditboxEvents();

	// default to accepting all characters
	setValidationString((utf8*)".*");
}


/*************************************************************************
	Destructor for Editbox class.
*************************************************************************/
Editbox::~Editbox(void)
{
}


/*************************************************************************
	return true if the Editbox has input focus.
*************************************************************************/
bool Editbox::hasInputFocus(void) const
{
	return isActive();
}


/*************************************************************************
	return true if the Editbox text is valid given the currently set
	validation string.
*************************************************************************/
bool Editbox::isTextValid(void) const
{
	return isStringValid(d_text);
}


/*************************************************************************
	return the current selection start point.
*************************************************************************/
ulong Editbox::getSelectionStartIndex(void) const
{
	return (d_selectionStart != d_selectionEnd) ? d_selectionStart : d_caratPos;
}


/*************************************************************************
	return the current selection end point.
*************************************************************************/
ulong Editbox::getSelectionEndIndex(void) const
{
	return (d_selectionStart != d_selectionEnd) ? d_selectionEnd : d_caratPos;
}


/*************************************************************************
	return the length of the current selection
	(in code points / characters).
*************************************************************************/
ulong Editbox::getSelectionLength(void) const
{
	return d_selectionEnd - d_selectionStart;
}


/*************************************************************************
	Specify whether the Editbox is read-only.
*************************************************************************/
void Editbox::setReadOnly(bool setting)
{
	// if setting is changed
	if (d_readOnly != setting)
	{
		d_readOnly = setting;
		onReadOnlyChanged(WindowEventArgs(this));
	}

}


/*************************************************************************
	Specify whether the text for the Editbox will be rendered masked.
*************************************************************************/
void Editbox::setTextMasked(bool setting)
{
	// if setting is changed
	if (d_maskText != setting)
	{
		d_maskText = setting;
		onMaskedRenderingModeChanged(WindowEventArgs(this));
	}

}


/*************************************************************************
	Set the text validation string.
*************************************************************************/
void Editbox::setValidationString(const String& validation_string)
{
	if (d_validationString != validation_string)
	{
		d_validationString = validation_string;

		try
		{
			d_validator = validation_string.c_str();
		}
		catch (boost::bad_expression x)
		{
			throw InvalidRequestException((utf8*)"The Editbox named '" + getName() + "' had the following bad validation expression set: '" + validation_string + "'.  Additional Information: " + x.what());
		}

		// notification
		onValidationStringChanged(WindowEventArgs(this));

		if (!isTextValid())
		{
			// also notify if text is now invalid.
			onTextInvalidatedEvent(WindowEventArgs(this));
		}

	}

}


/*************************************************************************
	Set the current position of the carat.
*************************************************************************/
void Editbox::setCaratIndex(ulong carat_pos)
{
	// make sure new position is valid
	if (carat_pos > d_text.length())
	{
		carat_pos = d_text.length();
	}

	// if new position is different
	if (d_caratPos != carat_pos)
	{
		d_caratPos = carat_pos;

		// Trigger "carat moved" event
		onCaratMoved(WindowEventArgs(this));
	}

}


/*************************************************************************
	Define the current selection for the Editbox
*************************************************************************/
void Editbox::setSelection(ulong start_pos, ulong end_pos)
{
	// ensure selection start point is within the valid range
	if (start_pos > d_text.length())
	{
		start_pos = d_text.length();
	}

	// ensure selection end point is within the valid range
	if (end_pos > d_text.length())
	{
		end_pos = d_text.length();
	}

	// ensure start is before end
	if (start_pos > end_pos)
	{
		ulong tmp = end_pos;
		end_pos = start_pos;
		start_pos = tmp;
	}

	// only change state if values are different.
	if ((start_pos != d_selectionStart) || (end_pos != d_selectionEnd))
	{
		// setup selection
		d_selectionStart = start_pos;
		d_selectionEnd	 = end_pos;

		// Trigger "selection changed" event
		onTextSelectionChanged(WindowEventArgs(this));
	}

}


/*************************************************************************
	set the utf32 code point used when rendering masked text.
*************************************************************************/
void Editbox::setMaskCodePoint(utf32 code_point)
{
	if (code_point != d_maskCodePoint)
	{
		d_maskCodePoint = code_point;

		// Trigger "mask code point changed" event
		onMaskCodePointChanged(WindowEventArgs(this));
	}

}


/*************************************************************************
	set the maximum text length for this Editbox.
*************************************************************************/
void Editbox::setMaxTextLength(ulong max_len)
{
	if (d_maxTextLen != max_len)
	{
		d_maxTextLen = max_len;
		
		// Trigger max length changed event
		onMaximumTextLengthChanged(WindowEventArgs(this));

		// trim string
		if (d_text.length() > d_maxTextLen)
		{
			d_text.resize(d_maxTextLen);
			onTextChanged(EventArgs());

			// see if new text is valid
			if (!isTextValid())
			{
				// Trigger Text is invalid event.
				onTextInvalidatedEvent(WindowEventArgs(this));
			}

		}

	}

}


/*************************************************************************
	Clear the current selection setting	
*************************************************************************/
void Editbox::clearSelection(void)
{
	// perform action only if required.
	if (getSelectionLength() != 0)
	{
		setSelection(0, 0);
	}

}


/*************************************************************************
	Erase the currently selected text.
*************************************************************************/
void Editbox::eraseSelectedText(bool modify_text)
{
	if (getSelectionLength() != 0)
	{
		// setup new carat position and remove selection highlight.
		setCaratIndex(getSelectionStartIndex());
		clearSelection();

		// erase the selected characters (if required)
		if (modify_text)
		{
			d_text.erase(getSelectionStartIndex(), getSelectionLength());

			// trigger notification that text has changed.
			onTextChanged(EventArgs());
		}

	}

}


/*************************************************************************
	return true if the given string matches the validation regular
	expression.	
*************************************************************************/
bool Editbox::isStringValid(const String& str) const
{
	// TODO: update for unicode
	return boost::regex_match(str.c_str(), d_validator);
}


/*************************************************************************
	Handler for mouse button pushed events
*************************************************************************/
void Editbox::onMouseButtonDown(MouseEventArgs& e)
{
	// base class handling
	Window::onMouseButtonDown(e);

	if (e.button == LeftButton)
	{
		// grab inputs
		captureInput();

		// handle mouse down
		clearSelection();
		d_dragging = true;
		d_dragAnchorIdx = getTextIndexFromPosition(e.position);
		setCaratIndex(d_dragAnchorIdx);

		e.handled = true;
	}

}

/*************************************************************************
	Handler for mouse button release events
*************************************************************************/
void Editbox::onMouseButtonUp(MouseEventArgs& e)
{
	// base class processing
	Window::onMouseButtonUp(e);

	if (e.button == LeftButton)
	{
		releaseInput();
		e.handled = true;
	}

}


/*************************************************************************
	Handler for mouse double-clicks
*************************************************************************/
void Editbox::onMouseDoubleClicked(MouseEventArgs& e)
{
	// base class processing
	Window::onMouseDoubleClicked(e);

	if (e.button == LeftButton)
	{
		// if masked, set up to select all
		if (isTextMasked())
		{
			d_dragAnchorIdx = 0;
			setCaratIndex(d_text.length());
		}
		// not masked, so select the word that was double-clicked.
		else
		{
			d_dragAnchorIdx = TextUtils::getWordStartIdx(d_text, (d_caratPos == d_text.length()) ? d_caratPos : d_caratPos + 1);
			d_caratPos		= TextUtils::getNextWordStartIdx(d_text, d_caratPos);
		}

		// perform actual selection operation.
		setSelection(d_dragAnchorIdx, d_caratPos);

		e.handled = true;
	}

}


/*************************************************************************
	Handler for mouse triple-clicks
*************************************************************************/
void Editbox::onMouseTripleClicked(MouseEventArgs& e)
{
	// base class processing
	Window::onMouseTripleClicked(e);

	if (e.button == LeftButton)
	{
		d_dragAnchorIdx = 0;
		setCaratIndex(d_text.length());
		setSelection(d_dragAnchorIdx, d_caratPos);
		e.handled = true;
	}

}


/*************************************************************************
	Handler for mouse movements
*************************************************************************/
void Editbox::onMouseMove(MouseEventArgs& e)
{
	// base class processing
	Window::onMouseMove(e);

	if (d_dragging)
	{
		setCaratIndex(getTextIndexFromPosition(e.position));
		setSelection(d_caratPos, d_dragAnchorIdx);
	}

	e.handled = true;
}

/*************************************************************************
	Handler for when input capture is lost
*************************************************************************/
void Editbox::onCaptureLost(EventArgs& e)
{
	d_dragging = false;

	// base class processing
	Window::onCaptureLost(e);

	e.handled = true;
}

/*************************************************************************
	Handler for type characters
*************************************************************************/
void Editbox::onCharacter(KeyEventArgs& e)
{
	// base class processing
	Window::onCharacter(e);

	// only need to take notice if we have focus
	if (hasInputFocus() && getFont()->isCodepointAvailable(e.codepoint))
	{
		// backup current text
		String tmp(d_text);
		tmp.erase(getSelectionStartIndex(), getSelectionLength());

		// if there is room
		if (tmp.length() < d_maxTextLen)
		{
			tmp.insert(getSelectionStartIndex(), 1, e.codepoint);

			if (isStringValid(tmp))
			{
				// erase selection using mode that does not modify d_text (we just want to update state)
				eraseSelectedText(false);

				// set text to the newly modified string
				setText(tmp);

				// advance carat;
				d_caratPos++;
			}
			else
			{
				// Trigger invalid modification attempted event.
				onInvalidEntryAttempted(WindowEventArgs(this));
			}

		}
		else
		{
			// Trigger text box full event
			onEditboxFullEvent(WindowEventArgs(this));
		}

	}

	e.handled = true;
}


/*************************************************************************
	Handler for key-down events
*************************************************************************/
void Editbox::onKeyDown(KeyEventArgs& e)
{
	// base class processing
	Window::onKeyDown(e);

	if (hasInputFocus())
	{
		switch (e.scancode)
		{
		case Key::LeftShift:
		case Key::RightShift:
			if (getSelectionLength() == 0)
			{
				d_dragAnchorIdx = getCaratIndex();
			}
			break;

		case Key::Backspace:
			handleBackspace();
			break;

		case Key::Delete:
			handleDelete();
			break;

		case Key::Tab:
		case Key::Return:
		case Key::NumpadEnter:
			// Fire 'input accepted' event
			onTextAcceptedEvent(WindowEventArgs(this));
			break;

		case Key::ArrowLeft:
			if (e.sysKeys & Control)
			{
				handleWordLeft(e.sysKeys);
			}
			else
			{
				handleCharLeft(e.sysKeys);
			}
			break;

		case Key::ArrowRight:
			if (e.sysKeys & Control)
			{
				handleWordRight(e.sysKeys);
			}
			else
			{
				handleCharRight(e.sysKeys);
			}
			break;

		case Key::Home:
			handleHome(e.sysKeys);
			break;

		case Key::End:
			handleEnd(e.sysKeys);
			break;
		}

		e.handled = true;
	}

}


/*************************************************************************
	Handler for when we are deactivated
*************************************************************************/
void Editbox::onDeactivated(WindowEventArgs& e)
{
	// base processing
	Window::onDeactivated(e);
	e.handled;

	// text accepted.
	onTextAcceptedEvent(WindowEventArgs(this));
}


/*************************************************************************
	Processing for backspace key	
*************************************************************************/
void Editbox::handleBackspace(void)
{
	String tmp(d_text);

	if (getSelectionLength() != 0)
	{
		tmp.erase(getSelectionStartIndex(), getSelectionLength());

		if (isStringValid(tmp))
		{
			// erase selection using mode that does not modify d_text (we just want to update state)
			eraseSelectedText(false);

			// set text to the newly modified string
			setText(tmp);
		}
		else
		{
			// Trigger invalid modification attempted event.
			onInvalidEntryAttempted(WindowEventArgs(this));
		}

	}
	else if (getCaratIndex() > 0)
	{
		tmp.erase(d_caratPos - 1, 1);

		if (isStringValid(tmp))
		{
			setCaratIndex(d_caratPos - 1);

			// set text to the newly modified string
			setText(tmp);
		}
		else
		{
			// Trigger invalid modification attempted event.
			onInvalidEntryAttempted(WindowEventArgs(this));
		}

	}

}


/*************************************************************************
	Processing for Delete key	
*************************************************************************/
void Editbox::handleDelete(void)
{
	String tmp(d_text);

	if (getSelectionLength() != 0)
	{
		tmp.erase(getSelectionStartIndex(), getSelectionLength());

		if (isStringValid(tmp))
		{
			// erase selection using mode that does not modify d_text (we just want to update state)
			eraseSelectedText(false);

			// set text to the newly modified string
			setText(tmp);
		}
		else
		{
			// Trigger invalid modification attempted event.
			onInvalidEntryAttempted(WindowEventArgs(this));
		}

	}
	else if (getCaratIndex() < tmp.length())
	{
		tmp.erase(d_caratPos, 1);

		if (isStringValid(tmp))
		{
			// set text to the newly modified string
			setText(tmp);
		}
		else
		{
			// Trigger invalid modification attempted event.
			onInvalidEntryAttempted(WindowEventArgs(this));
		}

	}

}


/*************************************************************************
	Move the carat one character to the left.
*************************************************************************/
void Editbox::handleCharLeft(uint sysKeys)
{
	if (d_caratPos > 0)
	{
		setCaratIndex(d_caratPos - 1);
	}

	if (sysKeys & Shift)
	{
		setSelection(d_caratPos, d_dragAnchorIdx);	
	}
	else
	{
		clearSelection();
	}

}


/*************************************************************************
	Move the carat one word to the left
*************************************************************************/
void Editbox::handleWordLeft(uint sysKeys)
{
	if (d_caratPos > 0)
	{
		setCaratIndex(TextUtils::getWordStartIdx(d_text, getCaratIndex()));
	}

	if (sysKeys & Shift)
	{
		setSelection(d_caratPos, d_dragAnchorIdx);	
	}
	else
	{
		clearSelection();
	}

}


/*************************************************************************
	Move the carat one character to the right.
*************************************************************************/
void Editbox::handleCharRight(uint sysKeys)
{
	if (d_caratPos < d_text.length())
	{
		setCaratIndex(d_caratPos + 1);
	}

	if (sysKeys & Shift)
	{
		setSelection(d_caratPos, d_dragAnchorIdx);	
	}
	else
	{
		clearSelection();
	}

}


/*************************************************************************
	Move the carat one word to the right
*************************************************************************/
void Editbox::handleWordRight(uint sysKeys)
{
	if (d_caratPos < d_text.length())
	{
		setCaratIndex(TextUtils::getNextWordStartIdx(d_text, getCaratIndex()));
	}

	if (sysKeys & Shift)
	{
		setSelection(d_caratPos, d_dragAnchorIdx);	
	}
	else
	{
		clearSelection();
	}

}


/*************************************************************************
	Move the carat to the start of the text
*************************************************************************/
void Editbox::handleHome(uint sysKeys)
{
	if (d_caratPos > 0)
	{
		setCaratIndex(0);
	}

	if (sysKeys & Shift)
	{
		setSelection(d_caratPos, d_dragAnchorIdx);	
	}
	else
	{
		clearSelection();
	}

}


/*************************************************************************
	Move the carat to the end of the text.	
*************************************************************************/
void Editbox::handleEnd(uint sysKeys)
{
	if (d_caratPos < d_text.length())
	{
		setCaratIndex(d_text.length());
	}

	if (sysKeys & Shift)
	{
		setSelection(d_caratPos, d_dragAnchorIdx);	
	}
	else
	{
		clearSelection();
	}

}


/*************************************************************************
	Add edit box specific events
*************************************************************************/
void Editbox::addEditboxEvents(void)
{
	addEvent(ReadOnlyChanged);				addEvent(MaskedRenderingModeChanged);
	addEvent(MaskCodePointChanged);			addEvent(ValidationStringChanged);
	addEvent(MaximumTextLengthChanged);		addEvent(TextInvalidatedEvent);
	addEvent(InvalidEntryAttempted);		addEvent(CaratMoved);
	addEvent(TextSelectionChanged);			addEvent(EditboxFullEvent);
	addEvent(TextAcceptedEvent);
}


/*************************************************************************
	Event fired internally when the read only state of the Editbox has
	been changed
*************************************************************************/
void Editbox::onReadOnlyChanged(WindowEventArgs& e)
{
	requestRedraw();
	fireEvent(ReadOnlyChanged, e);
}


/*************************************************************************
	Event fired internally when the masked rendering mode (password mode)
	has been changed
*************************************************************************/
void Editbox::onMaskedRenderingModeChanged(WindowEventArgs& e)
{
	requestRedraw();
	fireEvent(MaskedRenderingModeChanged , e);
}


/*************************************************************************
	Event fired internally when the code point to use for masked
	rendering has been changed.
*************************************************************************/
void Editbox::onMaskCodePointChanged(WindowEventArgs& e)
{
	// if we are in masked mode, trigger a GUI redraw.
	if (isTextMasked())
	{
		requestRedraw();
	}

	fireEvent(MaskCodePointChanged , e);
}


/*************************************************************************
	Event fired internally when the validation string is changed.	
*************************************************************************/
void Editbox::onValidationStringChanged(WindowEventArgs& e)
{
	fireEvent(ValidationStringChanged , e);
}


/*************************************************************************
	Event fired internally when the maximum text length for the edit box
	is changed.	
*************************************************************************/
void Editbox::onMaximumTextLengthChanged(WindowEventArgs& e)
{
	fireEvent(MaximumTextLengthChanged , e);
}


/*************************************************************************
	Event fired internally when something has caused the current text to
	now fail validation	
*************************************************************************/
void Editbox::onTextInvalidatedEvent(WindowEventArgs& e)
{
	fireEvent(TextInvalidatedEvent , e);
}


/*************************************************************************
	Event fired internally when the user attempted to make a change to
	the edit box that would have caused it to fail validation.
*************************************************************************/
void Editbox::onInvalidEntryAttempted(WindowEventArgs& e)
{
	fireEvent(InvalidEntryAttempted , e);
}


/*************************************************************************
	Event fired internally when the carat (insert point) position changes.	
*************************************************************************/
void Editbox::onCaratMoved(WindowEventArgs& e)
{
	requestRedraw();
	fireEvent(CaratMoved , e);
}


/*************************************************************************
	Event fired internally when the current text selection changes.	
*************************************************************************/
void Editbox::onTextSelectionChanged(WindowEventArgs& e)
{
	requestRedraw();
	fireEvent(TextSelectionChanged , e);
}


/*************************************************************************
	Event fired internally when the edit box text has reached the set
	maximum length.	
*************************************************************************/
void Editbox::onEditboxFullEvent(WindowEventArgs& e)
{
	fireEvent(EditboxFullEvent , e);
}


/*************************************************************************
	Event fired internally when the user accepts the edit box text by
	pressing Return, Enter, or Tab.	
*************************************************************************/
void Editbox::onTextAcceptedEvent(WindowEventArgs& e)
{
	fireEvent(TextAcceptedEvent , e);
}


/*************************************************************************
	Set the colour to be used for rendering Editbox text in the normal,
	unselected state.	
*************************************************************************/
void Editbox::setNormalTextColour(colour col)
{
	d_normalTextColour = col;
	requestRedraw();
}


/*************************************************************************
	Set the colour to be used for rendering the Editbox text when within
	the selected region.
*************************************************************************/
void Editbox::setSelectedTextColour(colour col)
{
	d_selectTextColour = col;
	requestRedraw();
}


/*************************************************************************
	Set the colour to be used for rendering the Editbox selection highlight
	when the Editbox is active.
*************************************************************************/
void Editbox::setNormalSelectBrushColour(colour col)
{
	d_selectBrushColour = col;
	requestRedraw();
}


/*************************************************************************
	Set the colour to be used for rendering the Editbox selection highlight
	when the Editbox is inactive.
*************************************************************************/
void Editbox::setInactiveSelectBrushColour(colour col)
{
	d_inactiveSelectBrushColour = col;
	requestRedraw();
}

} // End of  CEGUI namespace section
