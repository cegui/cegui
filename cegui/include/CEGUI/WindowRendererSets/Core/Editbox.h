/***********************************************************************
    created:    Sat Jun 25 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
#ifndef _FalEditbox_h_
#define _FalEditbox_h_

#include "CEGUI/WindowRendererSets/Core/Module.h"
#include "CEGUI/widgets/Editbox.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace CEGUI
{

/*!
\brief
    Editbox class for the FalagardBase module.

    This class requires LookNFeel to be assigned. The LookNFeel should provide
    the following:

    States:
        - Enabled: Rendering for when the editbox is in enabled and is in
                   read-write mode.
        - EnabledFocused: Rendering for when the editbox is focused and is in
                   read-write mode.
        - ReadOnly: Rendering for when the editbox is in enabled and is in
                    read-only mode.
        - ReadOnlyFocused: Rendering for when the editbox is focused and is in
                    read-only mode.
        - Disabled: Rendering for when the editbox is disabled.

    NamedAreas:
        - TextArea: area where text, selection, and caret imagery will appear.

    PropertyDefinitions (optional)
        - NormalTextColour: property that accesses a colour value to be used to
                            render normal unselected text.  If this property is
                            not defined, the colour defaults to black.
        - SelectedTextColour: property that accesses a colour value to be used
                              to render selected text.  If this property is
                              not defined, the colour defaults to black.

    Imagery Sections:
        - Caret
*/
class COREWRSET_API FalagardEditbox : public EditboxWindowRenderer
{
public:
    
    static const String TypeName; //!< type name for this widget.

    //! Name of the optional property to access for the unselected text colour.
    static const String UnselectedTextColourPropertyName;
    //! Name of the optional property to access for the selected text colour.
    static const String SelectedTextColourPropertyName;
    //! Name of the optional property to access to obtain active selection rendering colour.
    static const String ActiveSelectionColourPropertyName;
    //! Name of the optional property to access to obtain inactive selection rendering colour.
    static const String InactiveSelectionColourPropertyName;
    //! The default timeout (in seconds) used when blinking the caret.
    static const float DefaultCaretBlinkTimeout;

    FalagardEditbox(const String& type);

    //! return whether the blinking caret is enabled.
    bool isCaretBlinkEnabled() const { return d_blinkCaret; }
    //! return the caret blink timeout period (only used if blink is enabled).
    float getCaretBlinkTimeout() const { return d_caretBlinkTimeout; }
    //! set whether the blinking caret is enabled.
    void setCaretBlinkEnabled(bool enable) { d_blinkCaret = enable; }
    //! set the caret blink timeout period (only used if blink is enabled).
    void setCaretBlinkTimeout(float seconds) { d_caretBlinkTimeout = seconds; }

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
    const Image* getSelectionBrushImage() const { return d_selectionBrush; }

    Rectf getTextRenderArea() const override;
    Rectf getCaretRect() const override;
    float getCaretWidth() const override;

    void createRenderGeometry() override;
    void update(float elapsed) override;

protected:

    //! helper to draw the base imagery (container and what have you)
    void renderBaseImagery() const;

    void createRenderGeometryForText(const Rectf& textArea);

    const Image* d_selectionBrush = nullptr;  //!< Image to use as the selection brush (should be set by derived class).
    //! time-out in seconds used for blinking the caret.
    float d_caretBlinkTimeout = DefaultCaretBlinkTimeout;
    //! current time elapsed since last caret blink state change.
    float d_caretBlinkElapsed = 0.f;
    //! true if caret should be shown.
    bool d_showCaret = true;
    //! true if the caret imagery should blink.
    bool d_blinkCaret = false;
};

}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _FalEditbox_h_
