/***********************************************************************
	created:	30/6/2004
	author:		Paul D Turner

	purpose:	Interface to the Multi-lien edit box base class.
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
#ifndef _CEGUIMultiLineEditbox_h_
#define _CEGUIMultiLineEditbox_h_

#include "CEGUI/widgets/EditboxBase.h"

#include "../Font.h"

#include <vector>
#include <deque>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
// forward declaration
class UndoHandler;

/*!
\brief
    Base class for multi-line edit box window renderer objects.
*/
class CEGUIEXPORT MultiLineEditboxWindowRenderer : public WindowRenderer
{
public:
    /*!
    \brief
        Constructor
    */
    MultiLineEditboxWindowRenderer(const String& name);

    /*!
    \brief
        Return a Rect object describing, in un-clipped pixels, the window relative area
        that the text should be rendered in to.

    \return
        Rect object describing the area of the Window to be used for rendering text.
    */
    virtual Rectf getTextRenderArea() const = 0;

protected:
    // base class overrides
    void onLookNFeelAssigned() override;
};

/*!
\brief
	Base class for the multi-line edit box widget.
*/
class CEGUIEXPORT MultiLineEditbox : public EditboxBase
{
public:
	static const String EventNamespace;				//!< Namespace for global events
    static const String WidgetTypeName;             //!< Window factory name

	/*************************************************************************
		Constants
	*************************************************************************/
	// event names
    /** Event fired when the read-only mode for the edit box has been changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the MultiLineEditbox whose read-only mode
     * was changed.
     */
	static const String EventReadOnlyModeChanged;
    /** Event fired when the word wrap mode of the edit box has been changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the MultiLineEditbox whose word wrap
     * mode was changed.
     */
	static const String EventWordWrapModeChanged;
    /** Event fired when the maximum allowable string length for the edit box
     * has been changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the MultiLineEditbox whose maximum string
     * length was changed.
     */
	static const String EventMaximumTextLengthChanged;
    /** Event fired when the text caret / current insertion position is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the MultiLineEditbox whose caret position
     * has changed.
     */
	static const String EventCaretMoved;
    /** Event fired when the current text selection for the edit box is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the MultiLineEditbox whose text selection
     * was changed.
     */
	static const String EventTextSelectionChanged;
    /** Event fired when the number of characters in the edit box reaches the
     * current maximum length.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the MultiLineEditbox whose text length
     * has reached the set maximum allowable length for the edit box.
     */
	static const String EventEditboxFull;
    /** Event fired when the mode setting that forces the display of the
     * vertical scroll bar for the edit box is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the MultiLineEditbox whose vertical
     * scrollbar mode has been changed.
     */
	static const String EventVertScrollbarModeChanged;
    /** Event fired when the mode setting that forces the display of the
     * horizontal scroll bar for the edit box is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the MultiLineEditbox whose horizontal
     * scrollbar mode has been changed.
     */
	static const String EventHorzScrollbarModeChanged;

    //! Widget name for the vertical scrollbar component.
    static const String VertScrollbarName;  
    //! Widget name for the horizontal scrollbar component.
    static const String HorzScrollbarName;   

    /*!
    \brief
        struct used to store information about a formatted line within the
        paragraph.
    */
    struct LineInfo
    {
        size_t  d_startIdx;     //!< Starting index for this line.
        size_t  d_length;       //!< Code point length of this line.
        float   d_extent;       //!< Rendered extent of this line.
    };
    typedef std::vector<LineInfo>   LineList;   //!< Type for collection of LineInfos.


	//! Constructor for the MultiLineEditbox base class.
	MultiLineEditbox(const String& type, const String& name);


    //! Destructor for the MultiLineEditbox base class.
    virtual ~MultiLineEditbox();

    // Inherited methods
    void setCaretIndex(size_t caret_pos) override;
    void setMaxTextLength(size_t max_len) override;
    bool performPaste(Clipboard& clipboard) override;

	/*************************************************************************
		Accessor Functions
	*************************************************************************/

	/*!
	\brief
		Return whether the text in the edit box will be word-wrapped.

	\return
		- true if the text will be word-wrapped at the edges of the widget frame.
		- false if text will not be word-wrapped (a scroll bar will be used to access long text lines).
	*/
	bool isWordWrapped() const;


    /*!
    \brief
        Return a pointer to the vertical scrollbar component widget for this
        MultiLineEditbox.

    \return
        Pointer to a Scrollbar object.

    \exception UnknownObjectException
        Thrown if the vertical Scrollbar component does not exist.
    */
    Scrollbar* getVertScrollbar() const;

    /*!
	\brief
		Return whether the vertical scroll bar is always shown.

	\return
		- true if the scroll bar will always be shown even if it is not required.
		- false if the scroll bar will only be shown when it is required.
	*/
	bool isVertScrollbarAlwaysShown() const;

    /*!
    \brief
        Return a pointer to the horizontal scrollbar component widget for this
        MultiLineEditbox.

    \return
        Pointer to a Scrollbar object.

    \exception UnknownObjectException
        Thrown if the horizontal Scrollbar component does not exist.
    */
    Scrollbar* getHorzScrollbar() const;


    /*!
    \brief
        Return a Rect object describing, in un-clipped pixels, the window relative area
        that the text should be rendered in to.

    \return
        Rect object describing the area of the Window to be used for rendering text.
    */
    Rectf getTextRenderArea() const;

    // get d_lines
    const LineList& getFormattedLines() const   {return d_lines;}

    /*!
    \brief
        Return the line number a given index falls on with the current formatting.  Will return last line
        if index is out of range.
    */
    size_t  getLineNumberFromIndex(size_t index) const;

	/*************************************************************************
		Manipulators
	*************************************************************************/
	/*!
	\brief
		Initialise the Window based object ready for use.

	\note
		This must be called for every window created.  Normally this is handled automatically by the WindowFactory for each Window type.

	\return
		Nothing
	*/
    void initialiseComponents() override;


	/*!
	\brief
		Scroll the view so that the current caret position is visible.
	*/
	virtual void ensureCaretIsVisible();


	/*!
	\brief
		Set whether the text will be word wrapped or not.

	\param setting
		- true if the text should word-wrap at the edges of the text box.
		- false if the text should not wrap, but a scroll bar should be used.

	\return
		Nothing.
	*/
	void setWordWrapping(bool setting);

    /*!
	\brief
		Set whether the vertical scroll bar should always be shown.

	\param setting
		true if the vertical scroll bar should be shown even when it is not required.  false if the vertical
		scroll bar should only be shown when it is required.

	\return
		Nothing.
	*/
	void	setShowVertScrollbar(bool setting);

    /*!
	\brief
		Sets a selection brush Image

	\param image
		The brush image to be used for selections
	*/
    void setSelectionBrushImage(const Image* image);

    /*!
	\brief
		Returns the selection brush Image

	\return
		The brush image currently used for selections
	*/
    const Image* getSelectionBrushImage() const;


    /*!
    \brief
        Format the text into lines as dictated by the formatting options.

    \param update_scrollbars
        - true if scrollbar configuration should be performed.
        - false if scrollbar configuration should not be performed.
    */
    void formatText(const bool update_scrollbars);


protected:
    // Overriding methods inherited from EditboxBase
    size_t getTextIndexFromPosition(const glm::vec2& pt) const override;
    void eraseSelectedText(bool modify_text = true) override;
    void handleBackspace() override;
    void handleDelete() override;
    void onCharacter(TextEventArgs& e) override;
    void onTextChanged(WindowEventArgs& e) override;
    void onSized_impl(ElementEventArgs& e) override;

    // Overriding methods inherited from Window
    void onScroll(CursorInputEventArgs& e) override;
    void onFontChanged(WindowEventArgs& e) override;
    void onSemanticInputEvent(SemanticEventArgs& e) override;

    /*!
    \brief
        Processing to move caret to the start of the current line.

    \param select
        when true, the text until the start of the line be also selected
    */
    void handleLineHome(bool select);


    /*!
    \brief
        Processing to move caret to the end of the current line

    \param select
        when true, the text until the end of the line will be also selected
    */
    void handleLineEnd(bool select);


    /*!
    \brief
        Processing to move caret up a line.

    \param select
        when true, a line up will be also selected
    */
    void handleLineUp(bool select);


    /*!
    \brief
        Processing to move caret down a line.

    \param select
        when true, a line down will be also selected
    */
    void handleLineDown(bool select);


	/*!
	\brief
		Processing to insert a new line / paragraph.
	*/
    void handleNewLine();


    /*!
    \brief
        Processing to move caret one page up

    \param select
        when true, a page up will be also selected
    */
    void handlePageUp(bool select);


    /*!
    \brief
        Processing to move caret one page down

    \param select
        when true, a page down will be also selected
    */
    void handlePageDown(bool select);

	/*!
	\brief
	   Internal handler that is triggered when the user interacts with the scrollbars.
    */
    bool handle_scrollChange(const EventArgs& args);

    // handler triggered when vertical scrollbar is shown or hidden
    bool handle_vertScrollbarVisibilityChanged(const EventArgs&);

    // validate window renderer
    bool validateWindowRenderer(const WindowRenderer* renderer) const override;


	/*!
	\brief
		Return the length of the next token in String \a text starting at index \a start_idx.

	\note
		Any single whitespace character is one token, any group of other characters is a token.

	\return
		The code point length of the token.
	*/
	size_t getNextTokenLength(const String& text, size_t start_idx) const;


    /*!
	\brief
		display required integrated scroll bars according to current state of the edit box and update their values.
	*/
	void configureScrollbars();



	/*************************************************************************
		New event handlers
	*************************************************************************/
	/*!
	\brief
		Handler called when the word wrap mode for the the edit box changes
	*/
	void onWordWrapModeChanged(WindowEventArgs& e);


	/*!
	\brief
		Handler called when the 'always show' setting for the vertical scroll bar changes.
	*/
	void onVertScrollbarModeChanged(WindowEventArgs& e);


	/*!
	\brief
		Handler called when 'always show' setting for the horizontal scroll bar changes.
	*/
	void onHorzScrollbarModeChanged(WindowEventArgs& e);

	/*************************************************************************
		Implementation data
	*************************************************************************/
	static String   d_lineBreakChars;	//!< Holds what we consider to be line break characters.
    bool            d_wordWrap;		//!< true when formatting uses word-wrapping.
	LineList        d_lines;			//!< Holds the lines for the current formatting.
	float           d_lastRenderWidth;  //!< Holds last render area width
	float           d_widestExtent;	//!< Holds the extent of the widest line as calculated in the last formatting pass.

	// component widget settings
	bool d_forceVertScroll;		//!< true if vertical scrollbar should always be displayed
	bool d_forceHorzScroll;		//!< true if horizontal scrollbar should always be displayed

	// images
	const Image* d_selectionBrush;	//!< Image to use as the selection brush (should be set by derived class).


private:
	/*************************************************************************
		Private methods
	*************************************************************************/
	void addMultiLineEditboxProperties();

    void handleSelectAllText(SemanticEventArgs& e);
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUIMultiLineEditbox_h_
