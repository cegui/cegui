/***********************************************************************
    created:    8/6/2004
    author:     Paul D Turner

    purpose:    Interface to base class for list items
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIListboxItem_h_
#define _CEGUIListboxItem_h_

#include "../Base.h"
#include "../String.h"
#include "../ColourRect.h"
#include "../Sizef.h"
#include "../Rectf.h"
#include <vector>

namespace CEGUI
{

// FIXME: this class is used only in MultiColumnList! Rewrite the list as an ItemView!

//! \brief Base class for list box items
class CEGUIEXPORT ListboxItem
{
public:

    static const Colour DefaultSelectionColour; //!< Default selection brush colour.

    ListboxItem(const String& text, unsigned int item_id = 0, void* item_data = nullptr, bool disabled = false, bool auto_delete = true);
    virtual ~ListboxItem() = default;

    /*!
    \brief
        return the text string set for this list box item.

        Note that even if the item does not render text, the text string can still be useful, since it
        is used for sorting list box items.

    \return
        String object containing the current text for the list box item.
    */
    const String& getText() const {return d_textLogical;}

    //!!!TODO TOOLTIPS: IMPLEMENT!
    //const String& getTooltipText() const { return d_tooltipText; }

    /*!
    \brief
        Return the current ID assigned to this list box item.

        Note that the system does not make use of this value, client code can assign any meaning it
        wishes to the ID.

    \return
        ID code currently assigned to this list box item
    */
    unsigned int getID() const { return d_itemID; }


    /*!
    \brief
        Return the pointer to any client assigned user data attached to this lis box item.

        Note that the system does not make use of this data, client code can assign any meaning it
        wishes to the attached data.

    \return
        Pointer to the currently assigned user data.
    */
    void* getUserData() const { return d_itemData; }

    /*!
    \brief
        return whether this item is selected.

    \return
        true if the item is selected, false if the item is not selected.
    */
    bool isSelected() const { return d_selected; }


    /*!
    \brief
        return whether this item is disabled.

    \return
        true if the item is disabled, false if the item is enabled.
    */
    bool    isDisabled() const      {return d_disabled;}


    /*!
    \brief
        return whether this item will be automatically deleted when the list box it is attached to
        is destroyed, or when the item is removed from the list box.

    \return
        true if the item object will be deleted by the system when the list box it is attached to is
        destroyed, or when the item is removed from the list.  false if client code must destroy the
        item after it is removed from the list.
    */
    bool    isAutoDeleted() const   {return d_autoDelete;}


    /*!
    \brief
        Get the owner window for this ListboxItem.
        
        The owner of a ListboxItem is typically set by the list box widgets when an item is added or inserted.

    \return
        Ponter to the window that is considered the owner of this ListboxItem.
    */
    const Window*   getOwnerWindow() const      {return d_owner;}


    /*!
    \brief
        Return the current colours used for selection highlighting.

    \return
        ColourRect object describing the currently set colours
    */
    ColourRect  getSelectionColours() const     {return d_selectCols;}


    /*!
    \brief
        Return the current selection highlighting brush.

    \return
        Pointer to the Image object currently used for selection highlighting.
    */
    const Image*    getSelectionBrushImage() const      {return d_selectBrush;}


    /*************************************************************************
        Manipulators
    *************************************************************************/
    /*!
    \brief
        set the text string for this list box item.

        Note that even if the item does not render text, the text string can still be useful, since it
        is used for sorting list box items.

    \param text
        String object containing the text to set for the list box item.
    */
    virtual void setText(const String& text) { d_textLogical = text; }

    void setTooltipText(const String& text) { d_tooltipText = text; }

    /*!
    \brief
        Set the ID assigned to this list box item.

        Note that the system does not make use of this value, client code can assign any meaning it
        wishes to the ID.

    \param item_id
        ID code to be assigned to this list box item
    */
    void    setID(unsigned int item_id)     {d_itemID = item_id;}


    /*!
    \brief
        Set the client assigned user data attached to this lis box item.

        Note that the system does not make use of this data, client code can assign any meaning it
        wishes to the attached data.

    \param item_data
        Pointer to the user data to attach to this list item.
    */
    void    setUserData(void* item_data)    {d_itemData = item_data;}


    /*!
    \brief
        set whether this item is selected.

    \param setting
        true if the item is selected, false if the item is not selected.
    */
    void    setSelected(bool setting)       {d_selected = setting;}


    /*!
    \brief
        set whether this item is disabled.

    \param setting
        true if the item is disabled, false if the item is enabled.
    */
    void    setDisabled(bool setting)       {d_disabled = setting;}

    /*!
    \brief
        Set whether this item will be automatically deleted when the list box it is attached to
        is destroyed, or when the item is removed from the list box.

    \param setting
        true if the item object should be deleted by the system when the list box it is attached to is
        destroyed, or when the item is removed from the list.  false if client code will destroy the
        item after it is removed from the list.
    */
    void    setAutoDeleted(bool setting)        {d_autoDelete = setting;}


    /*!
    \brief
        Set the owner window for this ListboxItem.  This is called by all the list box widgets when
        an item is added or inserted.

    \param owner
        Ponter to the window that should be considered the owner of this ListboxItem.
    */
    void    setOwnerWindow(const Window* owner)     {d_owner = owner;}


    /*!
    \brief
        Set the colours used for selection highlighting.

    \param cols
        ColourRect object describing the colours to be used.
    */
    void    setSelectionColours(const ColourRect& cols)     {d_selectCols = cols;}


    /*!
    \brief
        Set the colours used for selection highlighting.

    \param top_left_colour
        Colour (as ARGB value) to be applied to the top-left corner of the selection area.

    \param top_right_colour
        Colour (as ARGB value) to be applied to the top-right corner of the selection area.

    \param bottom_left_colour
        Colour (as ARGB value) to be applied to the bottom-left corner of the selection area.

    \param bottom_right_colour
        Colour (as ARGB value) to be applied to the bottom-right corner of the selection area.
    */
    void    setSelectionColours(Colour top_left_colour, Colour top_right_colour, Colour bottom_left_colour, Colour bottom_right_colour);


    /*!
    \brief
        Set the colours used for selection highlighting.

    \param col
        colour value to be used when rendering.
    */
    void    setSelectionColours(Colour col)     {setSelectionColours(col, col, col, col);}


    /*!
    \brief
        Set the selection highlighting brush image.

    \param image
        Pointer to the Image object to be used for selection highlighting.
    */
    void    setSelectionBrushImage(const Image* image)      {d_selectBrush = image;}


    /*!
    \brief
        Set the selection highlighting brush image.

    \param name
        Name of the image to be used
    */
    void    setSelectionBrushImage(const String& name);

    /*!
    \brief
        Perform any updates needed because the given font's render size has
        changed.

    \note
        The base implementation just returns false.

    \param font
        Pointer to the Font whose render size has changed.

    \return
        - true if some action was taken.
        - false if no action was taken (i.e font is not used here).
    */
    virtual bool handleFontRenderSizeChange(const Font* const /*font*/) { return false; }

    /*************************************************************************
        Abstract portion of interface
    *************************************************************************/
    /*!
    \brief
        Return the rendered pixel size of this list box item.

    \return
        Size object describing the size of the list box item in pixels.
    */
    virtual Sizef getPixelSize() const = 0;


    /*!
    \brief
        Create render geometry for the list box item in its current state

    \param targetRect
        The target rectangle for drawing.

    \param alpha
        Alpha value to be used when rendering the item (between 0.0f and 1.0f).

    \param clipper
        Rect object describing the clipping rectangle for the draw operation.
    */
    virtual void createRenderGeometry(std::vector<GeometryBuffer*>& out,
        const Rectf& targetRect,
        float alpha, const Rectf* clipper) const = 0;

    /*************************************************************************
        Operators
    *************************************************************************/
    /*!
    \brief
        Less-than operator, compares item texts.
    */
    virtual bool    operator<(const ListboxItem& rhs) const     {return getText() < rhs.getText();}


    /*!
    \brief
        Greater-than operator, compares item texts.
    */
    virtual bool    operator>(const ListboxItem& rhs) const     {return getText() > rhs.getText();}


protected:
    /*************************************************************************
        Implementation methods
    *************************************************************************/
    /*!
    \brief
        Return a colour value describing the colour specified by \a col after having its alpha
        component modulated by the value \a alpha.
    */
    Colour calculateModulatedAlphaColour(Colour col, float alpha) const;


    /*************************************************************************
        Implementation Data
    *************************************************************************/
    const Window* d_owner = nullptr;       //!< Pointer to the window that owns this item.
    const Image*  d_selectBrush = nullptr; //!< Image used for rendering selection.
    void*         d_itemData;              //!< Pointer to some client code data. This has no meaning within the GUI system.
    String        d_textLogical;
    String        d_tooltipText;           //!< Text for the individual tooltip of this item
    unsigned int  d_itemID;                //!< ID code assigned by client code.  This has no meaning within the GUI system.
    ColourRect    d_selectCols;            //!< Colours used for selection highlighting.
    bool          d_selected = false;      //!< true if this item is selected.  false if the item is not selected.
    bool          d_disabled;              //!< true if this item is disabled.  false if the item is not disabled.
    bool          d_autoDelete;            //!< true if the system should destroy this item, false if client code will destroy the item.
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIListboxItem_h_
