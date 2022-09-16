/***********************************************************************
    created:    Sun Jan 11 2009
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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

#include "CEGUI/EventSet.h"
#include "CEGUI/EventArgs.h"
#include "CEGUI/Rectf.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
//! EventArgs class passed to subscribers of RenderTarget events.
class CEGUIEXPORT RenderTargetEventArgs : public EventArgs
{
public:
    RenderTargetEventArgs(RenderTarget* target):
        target(target)
    {}

    //! pointer to the RenderTarget that triggered the event.
    RenderTarget* target;
};

/*!
\brief
    Defines a virtual RenderTarget class serving different render target classes
    as base. Concrete instances of objects that implement the RenderTarget interface are
    normally created via the Renderer object. This class should be inherited
    only using virtual inheritance.
*/
class CEGUIEXPORT RenderTarget :
    public EventSet
{
public:
    RenderTarget();
    virtual ~RenderTarget();

    //! Namespace for global events
    static const String EventNamespace;

    /** Event to be fired when the RenderTarget object's area has changed.
     * Handlers are passed a const RenderTargetEventArgs reference with
     * RenderTargetEventArgs::target set to the RenderTarget whose area changed.
     */
    static const String EventAreaChanged;

    /*!
    \brief
        Draw geometry from the given GeometryBuffer onto the surface that
        this RenderTarget represents.

    \param buffer
        GeometryBuffer object holding the geometry that should be drawn to the
        RenderTarget.

    \param drawModeMask
        Defines the draw mode mask that may be used by the renderer to separate
        draw calls.
    */
    virtual void draw(const GeometryBuffer& buffer,
        std::uint32_t drawModeMask = DrawModeMaskAll);

    /*!
    \brief
        Draw geometry from the given RenderQueue onto the surface that
        this RenderTarget represents.

    \param queue
        RenderQueue object holding the geometry that should be drawn to the
        RenderTarget.

    \param drawModeMask
        Defines the draw mode mask that may be used by the renderer to separate
        draw calls.
    */
    virtual void draw(const RenderQueue& queue,
        std::uint32_t drawModeMask = DrawModeMaskAll);

    /*!
    \brief
        Set the area for this RenderTarget.  The exact action this function
        will take depends upon what the concrete class is representing.  For
        example, with a 'view port' style RenderTarget, this should set the area
        that the view port occupies on the display (or rendering window).

    \param area
        Rect object describing the new area to be assigned to the RenderTarget.

    \note
        When implementing this function, you should be sure to fire the event
        RenderTarget::EventAreaChanged so that interested parties can know that
        the change has occurred.

    \exception InvalidRequestException
        May be thrown if the RenderTarget does not support setting or changing
        its area, or if the area change can not be satisfied for some reason.
    */
    virtual void setArea(const Rectf& area);

    /*!
    \brief
        Return the area defined for this RenderTarget.

    \return
        Rect object describing the currently defined area for this RenderTarget.
    */
    const Rectf& getArea() const;

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
        This must be called before doing any rendering - if it is not called,
        in the unlikely event that your application actually works, it will
        likely stop working in some future version.
    */
    virtual void activate();

    /*!
    \brief
        Deactivate the render target after having completed rendering.

    \note
        This must be called after you have finished rendering to a target -
        if it is not called, in the unlikely event that your application actually
        works, it will likely stop working in some future version.
    */
    virtual void deactivate();

    /*!
    \brief
        Take point \a p_in unproject it and put the result in \a p_out.
        Resulting point is local to GeometryBuffer \a buff.
    */
    void unprojectPoint(const GeometryBuffer& buff, const glm::vec2& p_in, glm::vec2& p_out) const;

    /*!
    \brief
        Returns a reference to the Renderer that is the owner/creator of this RenderTarget instance.

    \return
        A reference to the Renderer that is the owner/creator of this RenderTarget instance.
    */
    virtual Renderer& getOwner() = 0;

    /*!
    \brief
        Returns the current count of activations for this RenderTarget

    \return
        The current count of activations.
    */
    unsigned int getActivationCounter() const;

    /*!
    \brief
        Creates a view projection matrix for the OpenGL graphics library (Depth Range from -1 to 1) based
        on this RenderTarget's current settings.

    \return
        A freshly created OpenGL view projection matrix for this RenderTarget.
    */
    glm::mat4 createViewProjMatrixForOpenGL() const;

    /*!
    \brief
        Creates a view projection matrix for the Direct3D graphics library (Depth Range from 0 to 1) based
        on this RenderTarget's current settings.

    \return
        A freshly created Direct3D view projection matrix for this RenderTarget.
    */
    glm::mat4 createViewProjMatrixForDirect3D() const;

    //! \brief Updates the view projection matrix of this RenderTarget.
    void updateMatrix(const glm::mat4& matrix) const;

    //! \brief Updates the view projection matrix of this RenderTarget with a default matrix for this renderer.
    virtual void updateMatrix() const = 0;

    /*!
    \brief
        Returns the vertical FOV angle of the perspective projection.
    \return
        The vertical FOV angle of the perspective projection. Value in Radians.
    */
    float getFovY() const;

    /*!
    \brief
        Sets the vertical FOV angle of the perspective projection.
    \param fovY
        The vertical FOV angle of the perspective projection. Value in Radians.
    */
    void setFovY(const float fovY);

protected:
    /*!
    \brief
        The current number of activation of this RenderTarget. This is increased on every call to activate() and
        will in turn be used to remove the most common redundant matrix updates of GeometryBuffers.
    */
    mutable unsigned int d_activationCounter;

    //! holds defined area for the RenderTarget
    Rectf d_area;

    //! Determines if the matrix is up to date
    mutable bool d_matrixValid;
    //! The view projection matrix
    mutable glm::mat4 d_matrix;

    //! tracks viewing distance (this is set up at the same time as d_matrix)
    mutable float d_viewDistance;

    /*!
    \brief
        The vertical FOV (field of view) angle for the perspective projection. Value in Radians.
        This effect of this value will not visible in the projection, unless a window is rotated around the X or Y axis.
    */
    float d_fovY;

    //! The tangent of the y-axis FOV half-angle; used to calculate viewing distance.
    float d_fovY_halftan;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif


#endif  // end of guard _CEGUIRenderTarget_h_
