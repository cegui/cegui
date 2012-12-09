/***********************************************************************
    filename:   CEGUIRenderTarget.h
    created:    Sun Jan 11 2009
    author:     Paul D Turner
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
#ifndef _CEGUIRenderTarget_h_
#define _CEGUIRenderTarget_h_

#include "CEGUIBase.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Defines interface to some surface that can be rendered to.  Concrete
    instances of objects that implement the RenderTarget interface are
    normally created via the Renderer object.
*/
class CEGUIEXPORT RenderTarget
{
public:
    //! Destructor
    virtual ~RenderTarget()
    {}

    /*!
    \brief
        Draw geometry from the given GeometryBuffer onto the surface that
        this RenderTarget represents.

    \param buffer
        GeometryBuffer object holding the geometry that should be drawn to the
        RenderTarget.
    */
    virtual void draw(const GeometryBuffer& buffer) = 0;

    /*!
    \brief
        Draw geometry from the given RenderQueue onto the surface that
        this RenderTarget represents.

    \param queue
        RenderQueue object holding the geometry that should be drawn to the
        RenderTarget.
    */
    virtual void draw(const RenderQueue& queue) = 0;

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

    /*!
    \brief
        Activate the render target and put it in a state ready to be drawn to.

    \note
        You MUST call this before doing any rendering - if you do not call this,
        in the unlikely event that your application actually works, it will
        likely stop working in some future version.
    */
    virtual void activate() = 0;

    /*!
    \brief
        Deactivate the render target after having completed rendering.

    \note
        You MUST call this after you finish rendering to the target - if you do
        not call this, in the unlikely event that your application actually
        works, it will likely stop working in some future version.
    */
    virtual void deactivate() = 0;

    /*!
    \brief
        Take point \a p_in unproject it and put the result in \a p_out.
        Resulting point is local to GeometryBuffer \a buff.
    */
    virtual void unprojectPoint(const GeometryBuffer& buff,
                                const Vector2& p_in, Vector2& p_out) const = 0;
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIRenderTarget_h_
