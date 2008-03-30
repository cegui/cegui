/***********************************************************************
    filename: CEGUIRenderer.h
    created:  20/2/2004
    author:   Paul D Turner

    purpose: Defines interface for abstract Renderer class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2008 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIRenderer_h_
#define _CEGUIRenderer_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUIRect.h"
#include "CEGUIEventSet.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Abstract class defining the interface for Renderer objects.

    Objects derived from Renderer are the means by which the GUI system
    interfaces with specific rendering technologies.  To use a rendering system
    or API to draw CEGUI imagery requires that an appropriate Renderer object be
    available.
*/
class CEGUIEXPORT Renderer : public EventSet
{
public:
    //! Namespace for global events
    static const String EventNamespace;

    /*************************************************************************
        Event name constants
    *************************************************************************/
    /*!
        event that fires when the underlying display size had changed.

        It is important that all Renderer implementers fire this properly as the
        system itself subscribes to this event.
    */
    static const String EventDisplaySizeChanged;

    /*!
    \brief
        Destructor for Renderer based objects.
    */
    virtual ~Renderer();

    /*************************************************************************
        Abstract interface methods
    *************************************************************************/
    /*!
    \brief
        Return the primary render target for the renderer.  Typically this will
        be some target that represents the physical display.
    */
    virtual RenderTarget* getPrimaryRenderTarget() const = 0;

    /*!
    \brief
        Create, and return a pointer to, a RenderTarget that can be used to
        cache imagery; that is a RenderTarget that does not lose it's content
        from one frame to another.

        If the renderer is unable to offer such a thing, 0 should be returned.

    \return
        Pointer to a RenderTarget object that is suitable for caching imagery,
        or 0 if the renderer is unable to offer such a thing.
    */
    virtual RenderTarget* createCachingRenderTarget() = 0;

    /*!
    \brief
        Function that cleans up RenderTarget objects created with the
        createCachingRenderTarget function.

    \param target
        A pointer to a RenderTarget object that was previously returned from a
        call to createCachingRenderTarget.
    */
    virtual void destroyCachingRenderTarget(RenderTarget* target) = 0;

    /*!
    \brief
        Perform final rendering for all quads that have been queued for
        rendering.

        The contents of the rendering queue is retained and can be rendered
        again as required.  If the contents is not required it can be cleared
        by calling clearRenderList().

    \return
        Nothing
    */
    virtual void doRender() = 0;

    /*!
    \brief
        Clears all queued quads from the render queue.

    \return
        Nothing
    */
    virtual void clearRenderList() = 0;

    /*!
    \brief
        Creates a 'null' Texture object.

    \return
        a newly created Texture object.  The returned Texture object has no size
        or imagery associated with it, and is generally of little or no
        immediate use.
    */
    virtual Texture* createTexture() = 0;

    /*!
    \brief
        Create a Texture object using the given image file.

    \param filename
        String object that specifies the path and filename of the image file to
        use when creating the texture.

    \param resourceGroup
        Resource group identifier to be passed to the resource provider when
        loading the texture file.

    \return
        a newly created Texture object.  The initial contents of the texture
        memory is the requested image file.

    \note
        Textures are always created with a size that is a power of 2.  If the
        file you specify is of a size that is not a power of two, the final size
        will be rounded up.  Additionally, textures are always square, so the
        ultimate size is governed by the larger of the width and height of the
        specified file.  You can check the ultimate sizes by querying the
        texture after creation.
    */
    virtual Texture* createTexture(const String& filename,
                                   const String& resourceGroup) = 0;

    /*!
    \brief
        Create a Texture object with the given pixel dimensions as specified by
        \a size.  NB: Textures are always square.

    \param size
        float value that specifies the size to use for the width and height when
        creating the new texture.

    \return
        a newly created Texture object.  The initial contents of the texture
        memory is undefined / random.

    \note
        Textures are always created with a size that is a power of 2.  If you
        specify a size that is not a power of two, the final size will be
        rounded up.  So if you specify a size of 1024, the texture will be
        (1024 x 1024), however, if you specify a size of 1025, the texture will
        be (2048 x 2048).  You can check the ultimate size by querying the
        texture after creation.
    */
    virtual Texture* createTexture(float size) = 0;

    /*!
    \brief
        Destroy the given Texture object.

    \param texture
        pointer to the Texture object to be destroyed

    \return
        Nothing
    */
    virtual void destroyTexture(Texture* texture) = 0;

    /*!
    \brief
        Destroy all Texture objects.

    \return
        Nothing
    */
    virtual void destroyAllTextures() = 0;

    /*!
    \brief
        Return the maximum texture size available

    \return
        Size of the maximum supported texture in pixels (textures are always
        assumed to be square)
    */
    virtual uint getMaxTextureSize() const = 0;

    /*!
    \brief
        Return the horizontal display resolution dpi

    \return
        horizontal resolution of the display in dpi.
    */
    virtual uint getHorzScreenDPI() const = 0;

    /*!
    \brief
        Return the vertical display resolution dpi

    \return
        vertical resolution of the display in dpi.
    */
    virtual uint getVertScreenDPI() const = 0;

    /*************************************************************************
        Basic stuff we provide in base class
    *************************************************************************/
    /*!
    \brief
        Reset the z co-ordinate for rendering.

    \return
        Nothing
    */
    void resetZValue()
        { d_current_z = GuiZInitialValue; }

    /*!
    \brief
        Update the z co-ordinate for the next major UI element (window).

    \return
        Nothing
    */
    void advanceZValue()
        { d_current_z -= GuiZElementStep; }

    /*!
    \brief
        return the current Z value to use (equates to layer 0 for this UI
        element).

    \return
        float value that specifies the z co-ordinate to be used for layer 0 on
        the current GUI element.
    */
    float getCurrentZ() const
        { return d_current_z; }

    /*!
    \brief
        return the z co-ordinate to use for the requested layer on the current
        GUI element.

    \param layer
        Specifies the layer to return the Z co-ordinate for.  Each GUI element
        can use up to 10 layers, so valid inputs are 0 to 9 inclusive.  If you
        specify an invalid value for \a layer, results are undefined.

    \return
        float value that specifies the Z co-ordinate for layer \a layer on the
        current GUI element.
    */
    float getZLayer(uint layer) const
        { return d_current_z - ((float)layer * GuiZLayerStep); }


    /*!
    \brief
        Return identification string for the renderer module.  If the internal
        id string has not been set by the Renderer module creator, a generic
        string of "Unknown renderer" will be returned.

    \return
        String object holding a string that identifies the Renderer in use.
    */
    const String& getIdentifierString() const;

    virtual ResourceProvider* createResourceProvider();

protected:
    /*!
    \brief
        Renderer base class constructor.  This is protected; you do not
        instantiate objects of the Renderer base class.
    */
    Renderer();

private:
    /*************************************************************************
        Implementation constants
    *************************************************************************/
    //! Initial value to use for 'z' each frame.
    static const float GuiZInitialValue;
    //! Value to step 'z' for each GUI element.
    static const float GuiZElementStep;
    //! Value to step 'z' for each GUI layer.
    static const float GuiZLayerStep;

    /*************************************************************************
        Implementation Data
    *************************************************************************/
    //! The current z co-ordinate value.
    float d_current_z;

protected:
    //! Holds the pointer to the ResourceProvider object.
    ResourceProvider* d_resourceProvider;
    //! String that holds some id information about the renderer.
    String d_identifierString;
};

} // End of  CEGUI namespace section

#endif // end of guard _CEGUIRenderer_h_
