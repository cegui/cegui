/***********************************************************************
    filename:   CEGUIRenderTarget.h
    created:    Tue Feb 12 2008
    author:     Paul D Turner
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
#ifndef _CEGUIRenderTarget_h_
#define _CEGUIRenderTarget_h_

#include "CEGUIBase.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Enumerated type that contains the valid diagonal-mode that specify how a
    quad is split into triangles when rendered with fx. a 3D API
*/
enum QuadSplitMode
{
    //! Diagonal goes from top-left to bottom-right.
    TopLeftToBottomRight,
    //!< Diagonal goes from bottom-left to top-right
    BottomLeftToTopRight
};

/*!
\brief
    Abstract class that represents a target for some CEGUI rendering operations.
    The actual destination of the output will depend upon the concrete class in
    use - and could be the active display, a texture, or something else
    entirely.
*/
class CEGUIEXPORT RenderTarget
{
public:
    /*!
    \brief
        RenderTarget destructor.
    */
    virtual ~RenderTarget();

    /*!
    \brief
        Add a quad to be rendered to this target.  All clipping and other
        adjustments should have been made prior to calling this.

    \param dest
        Rect object describing the destination area (values are in pixels)

    \param z
        float value specifying the z co-ordinate / z order of the quad

    \param tex
        pointer to the Texture object that holds the imagery to be rendered

    \param tex_rect
        Rect object holding the area of \a tex that is to be rendered
        (values are in texture co-ordinates).

    \param cols
        ColourRect object describing the colour values that are to be applied
        when rendering.

    \param quad_split_mode
        One of the QuadSplitMode values specifying the way quads are split into
        triangles.
    */
    virtual void queueQuad(const Rect& dest, float z, const Texture* tex,
                           const Rect& tex_rect, const ColourRect& cols,
                           const QuadSplitMode quad_split_mode) = 0;

    /*!
    \brief
        Immediately render a quad to this target.  All clipping and other
        adjustments should have been made prior to calling this.

    \param dest
        Rect object describing the destination area (values are in pixels)

    \param z
        float value specifying the z co-ordinate / z order of the quad

    \param tex
        pointer to the Texture object that holds the imagery to be rendered

    \param tex_rect
        Rect object holding the area of \a tex that is to be rendered
        (values are in texture co-ordinates).

    \param cols
        ColourRect object describing the colour values that are to be applied
        when rendering.

    \param quad_split_mode
        One of the QuadSplitMode values specifying the way quads are split into
        triangles.
    */
    virtual void renderQuad(const Rect& dest, float z, const Texture* tex,
                           const Rect& tex_rect, const ColourRect& cols,
                           const QuadSplitMode quad_split_mode) = 0;

    /*!
    \brief
        Queue a quad to the RenderTarget \a target that will render section of
        this RenderTarget onto \a target.

    \param src
        Rect describing the area, in pixels, of this RenderTarget that is to be
        rendered into the destination RenderTarget \a target.

    \param dst
        Rect describing the destination area on the RenderTarget \a target that
        will receive the specified area of imagery from this RenderTarget.  The
        imagery will be scaled to fit the specified area.

    \param z
        float specifying the z co-ordinate to use for the rendering, for
        RenderTargets where such z co-ordinates are significant.

    \param cols
        ColourRect object describing the colours that are to be applied to the
        area as it is rendered.

    \param quad_split_mode
        One of the QuadSplitMode values specifying the way quads are split into
        triangles.

    \param target
        RenderTarget object that is the destination for the rendering operation.

    \exception InvalidRequestException
        May be thrown if the RenderTarget does not support rendering of its
        content onto another RenderTarget.
    */
    virtual void queueToTarget(const Rect& src, const Rect& dst, float z,
                               const ColourRect& cols,
                               const QuadSplitMode quad_split_mode,
                               RenderTarget& target) = 0;

    /*!
    \brief
        Immediately render a section of this RenderTarget onto the destination
        RenderTarget \a target.  This should be called between calls to the
        beginDirectRendering and endDirectRendering pair of functions.

    \param src
        Rect describing the area, in pixels, of this RenderTarget that is to be
        rendered into the destination RenderTarget \a target.

    \param dst
        Rect describing the destination area on the RenderTarget \a target that
        will receive the specified area of imagery from this RenderTarget.  The
        imagery will be scaled to fit the specified area.

    \param z
        float specifying the z co-ordinate to use for the rendering, for
        RenderTargets where such z co-ordinates are significant.

    \param cols
        ColourRect object describing the colours that are to be applied to the
        area as it is rendered.

    \param quad_split_mode
        One of the QuadSplitMode values specifying the way quads are split into
        triangles.

    \param target
        RenderTarget object that is the destination for the rendering operation.

    \exception InvalidRequestException
        May be thrown if the RenderTarget does not support rendering of its
        content onto another RenderTarget.
    */
    virtual void renderToTarget(const Rect& src, const Rect& dst, float z,
                                const ColourRect& cols,
                                const QuadSplitMode quad_split_mode,
                                RenderTarget& target) = 0;

    /*!
    \brief
        Execute rendering operations for all quads that have been queued for
        rendering.  The contents of the rendering queue is retained and can be
        rendered again as required.  If the contents is not required call
        clearRenderList().
    */
    virtual void execute(void) = 0;

    /*!
    \brief
        Clears all queued quads from the render queue.
    */
    virtual void clearRenderList(void) = 0;

    /*!
    \brief
        Perform any required steps to initialise direct rendering to the target
        using the renderQuad function.
    */
    virtual void beginDirectRendering() = 0;

    /*!
    \brief
        Perform any required steps to cleanup or recover after direct rendering
        to the target using the renderQuad function.
    */
    virtual void endDirectRendering() = 0;

    /*!
    \brief
        Enables or disables queueing for this RenderTarget.

        Disabling queueing effectively makes all calls to queueQuad and
        queueToTarget behave as if they were calls to renderQuads and
        renderToTarget respectively.
    \par
        Disabling the queue does not clear the queue - it is quite possible to
        queue some quads, disable the queue so data is rendered directly, then
        re-enable the queue to continue adding to those already queued.
    \par
        Note for interface implementors: Disabling queueing should automatically
        call beginDirectRendering, while re-enabling queueing should call
        endDirectRendering.  If you do not do this, please ensure that you \e
        do actually know what you are doing.

    \param enable
        - true to enable queueing.
        - false to disable queueing.
    */
    virtual void enableQueue(bool enable) = 0;

    /*!
    \brief
        Set the area for this RenderTarget.  The exact action this function
        will take depends upon what the concrete class is representing.  For
        example, with a 'view port' style RenderTarget, this should set the area
        that the view port occupies on the display (or rendering window).

    \param area
        Rect object describing the new area to be assigned to the RenderTarget.

    \exception InvalidRequestException
        May be thrown if the RenderTarget does not support setting or changing
        its area, or if the area change can not be satisfied for some reason.
    */
    virtual void setArea(const Rect& area) = 0;

    /*!
    \brief
        Return the area defined for this RenderTarget.

    \return
        Rect object describing the currently defined area for this RenderTarget.
    */
    virtual const Rect& getArea() const = 0;

    /*!
    \brief
        Used to declare to the RenderTarget the largest size, in pixels, of the
        next set of incoming rendering operations.  The main purpose of this is
        to allow for certain types of RenderTarget to more dynamically control
        their internal buffers in a more efficient way.  Not all RenderTarget
        types will need to react in any meaningful way to this.

    \param sz
        Size object describing the largest area that will be rendererd in the
        next batch of rendering operations.

    \exception InvalidRequestException
        May be thrown if the RenderTarget would not be able to handle the
        operations rendering content of the given size.
    */
    virtual void declareRenderSize(const Size& sz) = 0;

    /*!
    \brief
        Return whether the RenderTarget is an implementation that caches
        actual rendered imagery.

        Typically it is expected that texture based RenderTargets would return
        true in response to this call.  Other types of RenderTarget, like
        view port based targets, will more likely return false.

    \return
        - true if the RenderTarget does cache rendered imagery.
        - false if the RenderTarget does not cache rendered imagery.
    */
    virtual bool isImageryCache() const = 0;

protected:
    /*!
    \brief
        RenderTarget base constructor.  Protected, you do not instantiate this
        base class directly.
    */
    RenderTarget();
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIRenderTarget_h_
