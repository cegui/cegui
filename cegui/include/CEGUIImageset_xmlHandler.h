/***********************************************************************
filename: 	CEGUIImageset_xmlHandler.h
created:	21/2/2004
author:		Paul D Turner

purpose:	Defines the interface for the Imageset class
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
#ifndef _CEGUIImageset_xmlHandler_h_
#define _CEGUIImageset_xmlHandler_h_

#include "CEGUIImageset.h"
#include "CEGUIXMLHandler.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
Implementation Classes
*************************************************************************/
/*!
\brief
Handler class used to parse the Imageset XML files using SAX2
*/
class Imageset_xmlHandler : public XMLHandler
{
public:
	/*************************************************************************
	Construction & Destruction
	*************************************************************************/
	/*!
	\brief
	Constructor for Imageset::xmlHandler objects

	\param imageset
	Pointer to the Imageset object creating this xmlHandler object
	*/
	Imageset_xmlHandler(Imageset* imageset) : d_imageset(imageset) {}

	/*!
	\brief
	Destructor for Imageset::xmlHandler objects
	*/
	virtual ~Imageset_xmlHandler(void) {}

	/*************************************************************************
	SAX2 Handler overrides
	*************************************************************************/ 
	/*!
	\brief
	document processing (only care about elements, schema validates format)
	*/
    virtual void elementStart(const String& element, const XMLAttributes& attributes);
    virtual void elementEnd(const String& element);

	/*************************************************************************
	Functions used by our implementation
	*************************************************************************/
	Imageset*	getImageset(void) const				{return d_imageset;}

private:
	/*************************************************************************
	Implementation Constants
	*************************************************************************/
	static const String ImagesetElement;				//!< Tag name for Imageset elements.
	static const String ImageElement;					//!< Tag name for Image elements.
	static const String ImagesetNameAttribute;		//!< Attribute name that stores the name of the Imageset
	static const String ImagesetImageFileAttribute;	//!< Attribute name that stores the filename for the image file.
    static const String ImagesetResourceGroupAttribute;   //!< Attribute name that stores the resource group identifier used when loading image file.
	static const String ImagesetNativeHorzResAttribute;	//!< Optional attribute that stores 'native' horizontal resolution for the Imageset.
	static const String ImagesetNativeVertResAttribute;	//!< Optional attribute that stores 'native' vertical resolution for the Imageset.
	static const String ImagesetAutoScaledAttribute;	//!< Optional attribute that specifies whether the Imageset should be auto-scaled.
	static const String ImageNameAttribute;			//!< Attribute name that stores the name of the new Image.
	static const String ImageXPosAttribute;			//!< Attribute name that stores the x position of the new Image.
	static const String ImageYPosAttribute;			//!< Attribute name that stores the y position of the new Image.
	static const String ImageWidthAttribute;			//!< Attribute name that stores the width of the new Image.
	static const String ImageHeightAttribute;			//!< Attribute name that stores the height of the new Image.
	static const String ImageXOffsetAttribute;		//!< Attribute name that stores the x rendering offset of the new Image.
	static const String ImageYOffsetAttribute;		//!< Attribute name that stores the y rendering offset of the new Image.

    /*!
    \brief
        Method that handles the opening Imageset XML element.
    */
    void elementImagesetStart(const XMLAttributes& attributes);

    /*!
    \brief
        Method that handles the Image XML element.
    */
    void elementImageStart(const XMLAttributes& attributes);

    /*!
    \brief
        Method that handles the closing Imageset XML element.
    */
    void elementImagesetEnd();

	/*************************************************************************
	Implementation Data
	*************************************************************************/
	Imageset*	d_imageset;			//!< Holds a pointer to the Imageset that created the handler object
};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIImageset_xmlHandler_h_
