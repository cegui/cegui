/***********************************************************************
     filename:   CEGUITreeItem.cpp
     created:	5-13-07
     author:		Jonathan Welch (Based on Code by David Durant)
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
#include "elements/CEGUITree.h"
#include "elements/CEGUITreeItem.h"
#include "CEGUISystem.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIFontManager.h"
#include "CEGUIFont.h"
#include "CEGUIWindow.h"
#include "CEGUIImage.h"
#include <algorithm>


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
    Constants
*************************************************************************/
const colour TreeItem::DefaultSelectionColour = 0xFF4444AA;
const colour TreeItem::DefaultTextColour = 0xFFFFFFFF;

/*************************************************************************
    Base class constructor
*************************************************************************/
TreeItem::TreeItem(const String& text, uint item_id, void* item_data,
                   bool disabled, bool auto_delete) :
    d_itemText(text),
    d_itemID(item_id),
    d_itemData(item_data),
    d_selected(false),
    d_disabled(disabled),
    d_autoDelete(auto_delete),
    d_buttonLocation(Rect(0,0,0,0)),
    d_owner(0),
    d_selectCols(DefaultSelectionColour, DefaultSelectionColour,
                 DefaultSelectionColour, DefaultSelectionColour),
    d_selectBrush(0),
    d_textCols(DefaultTextColour, DefaultTextColour,
               DefaultTextColour, DefaultTextColour),
    d_font(0),
    d_iconImage(0),
    d_isOpen(false)
{
}

/*************************************************************************
    Set the selection highlighting brush image.
*************************************************************************/
void TreeItem::setSelectionBrushImage(const String& imageset,
                                      const String& image)
{
    setSelectionBrushImage(
        &ImagesetManager::getSingleton().getImageset(imageset)->getImage(image));
}

/*************************************************************************
    Return a ColourRect object describing the colours in 'cols' after
    having their alpha component modulated by the value 'alpha'.
*************************************************************************/
ColourRect TreeItem::getModulateAlphaColourRect(const ColourRect& cols,
                                                float alpha) const
{
    return ColourRect
    (
     calculateModulatedAlphaColour(cols.d_top_left, alpha),
     calculateModulatedAlphaColour(cols.d_top_right, alpha),
     calculateModulatedAlphaColour(cols.d_bottom_left, alpha),
     calculateModulatedAlphaColour(cols.d_bottom_right, alpha)
     );
}

/*************************************************************************
    Return a colour value describing the colour specified by 'col' after
    having its alpha component modulated by the value 'alpha'.
*************************************************************************/
colour TreeItem::calculateModulatedAlphaColour(colour col, float alpha) const
{
    colour temp(col);
    temp.setAlpha(temp.getAlpha() * alpha);
    return temp;
}

/*************************************************************************
    Return a pointer to the font being used by this TreeItem
*************************************************************************/
Font* TreeItem::getFont(void) const
{
    // prefer out own font
    if (d_font != 0)
        return d_font;
    // try our owner window's font setting
    // (may be null if owner uses non existant default font)
    else if (d_owner != 0)
        return d_owner->getFont();
    // no owner, just use the default (which may be NULL anyway)
    else
        return System::getSingleton().getDefaultFont();   
}

/*************************************************************************
    Set the font to be used by this TreeItem
*************************************************************************/
void TreeItem::setFont(const String& font_name)
{
    setFont(FontManager::getSingleton().getFont(font_name));
}


/*************************************************************************
    Return the rendered pixel size of this tree item.
*************************************************************************/
Size TreeItem::getPixelSize(void) const
{
    Size tmp(0,0);
    
    Font* fnt = getFont();
    
    if (fnt != 0)
    {
        tmp.d_height = PixelAligned(fnt->getLineSpacing());
        tmp.d_width = PixelAligned(fnt->getTextExtent(d_itemText));
    }
    
    return tmp;
}

/*************************************************************************
    Add the given TreeItem to this item's list.
*************************************************************************/
void TreeItem::addItem(TreeItem* item)
{
    if (item != 0)
    {
        Tree* parentWindow = (Tree*)getOwnerWindow();
        
        // establish ownership
        item->setOwnerWindow(parentWindow);
        
        // if sorting is enabled, re-sort the tree
        if (parentWindow->isSortEnabled())
        {
            d_listItems.insert(
                std::upper_bound(d_listItems.begin(),
                                 d_listItems.end(), item, &lbi_less),
                item);
        }
        // not sorted, just stick it on the end.
        else
        {
            d_listItems.push_back(item);
        }
        
        WindowEventArgs args(parentWindow);
        parentWindow->onListContentsChanged(args);
    }
}

/*************************************************************************
    Remove the given TreeItem from this item's list.
*************************************************************************/
void TreeItem::removeItem(const TreeItem* item) 
{ 
    if (item)
    {
        Tree* parentWindow = (Tree*)getOwnerWindow();

        LBItemList::iterator pos = std::find(d_listItems.begin(),
                                             d_listItems.end(),
                                             item);
        if (pos != d_listItems.end())
        {
            (*pos)->setOwnerWindow(0);
            d_listItems.erase(pos);

            if (item == parentWindow->d_lastSelected)
                parentWindow->d_lastSelected = 0;

            if (item->isAutoDeleted())
                delete item;

            WindowEventArgs args(parentWindow);
            parentWindow->onListContentsChanged(args);
        }
    }
}

TreeItem *TreeItem::getTreeItemFromIndex(size_t itemIndex)
{
    if (itemIndex > d_listItems.size())
        return 0;
    
    return d_listItems[itemIndex];
}

/*************************************************************************
    Draw the tree item in its current state.
*************************************************************************/
void TreeItem::draw(const Vector3& position, float alpha, const Rect& clipper) const
{
    if (d_selected && (d_selectBrush != 0))
        d_selectBrush->draw(clipper, position.d_z, clipper,
                            getModulateAlphaColourRect(d_selectCols, alpha));
    
    Font* fnt = getFont();
    
    if (fnt != 0)
    {
        Vector3 finalPos(position);
        finalPos.d_y -= PixelAligned(
            (fnt->getLineSpacing() - fnt->getBaseline()) * 0.5f);

        fnt->drawText(d_itemText, finalPos, clipper,
                      getModulateAlphaColourRect(d_textCols, alpha),1,1);
    }
}


void TreeItem::draw(RenderCache &cache, const Rect &targetRect, float zBase,
                    float alpha, const Rect *clipper) const
{
    Rect finalRect(targetRect);
    
    if (d_iconImage != 0)
    {
        // Size iconSize = d_iconImage->getSize();
        Rect finalPos(finalRect);
        finalPos.setWidth(targetRect.getHeight());
        finalPos.setHeight(targetRect.getHeight());
        cache.cacheImage(*d_iconImage, finalPos, zBase,
                         ColourRect(colour(1,1,1,alpha)), clipper);
        finalRect.d_left += targetRect.getHeight();
    }
    
    if (d_selected && d_selectBrush != 0)
        cache.cacheImage(*d_selectBrush, finalRect, zBase,
                         getModulateAlphaColourRect(d_selectCols, alpha),
                         clipper);
    
    Font* font = getFont();
    
    if (font)
    {
        Rect finalPos(finalRect);
        finalPos.d_top -= (font->getLineSpacing() - font->getBaseline()) * 0.5f;
        cache.cacheText(d_itemText, font, LeftAligned, finalPos, zBase,
                        getModulateAlphaColourRect(d_textCols, alpha), clipper);
    }
}

/*************************************************************************
    Set the colours used for selection highlighting.
*************************************************************************/
void TreeItem::setSelectionColours(colour top_left_colour,
                                   colour top_right_colour,
                                   colour bottom_left_colour,
                                   colour bottom_right_colour)
{
    d_selectCols.d_top_left		= top_left_colour;
    d_selectCols.d_top_right	= top_right_colour;
    d_selectCols.d_bottom_left	= bottom_left_colour;
    d_selectCols.d_bottom_right	= bottom_right_colour;
}

/*************************************************************************
    Set the colours used for text rendering.
*************************************************************************/
void TreeItem::setTextColours(colour top_left_colour,
                              colour top_right_colour,
                              colour bottom_left_colour,
                              colour bottom_right_colour)
{
    d_textCols.d_top_left		= top_left_colour;
    d_textCols.d_top_right		= top_right_colour;
    d_textCols.d_bottom_left	= bottom_left_colour;
    d_textCols.d_bottom_right	= bottom_right_colour;
}

} // End of  CEGUI namespace section
