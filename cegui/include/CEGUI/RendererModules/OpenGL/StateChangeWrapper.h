/***********************************************************************
    created:    Wed, 8th Feb 2012
    author:     Lukas E Meindl
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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

#ifndef _CEGUIOpenGLBaseStateChangeWrapper_h_
#define _CEGUIOpenGLBaseStateChangeWrapper_h_

#include "CEGUI/RendererModules/OpenGL/GL.h"
#include "RendererBase.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
OpenGLBaseStateChangeWrapper - wraps OpenGL calls and checks for redundant calls beforehand
*/
class OPENGL_GUIRENDERER_API OpenGLBaseStateChangeWrapper
{
public:
    /*!
    \brief
        This has to be used for both glBlendFunc and glBlendFuncSeperate, as the second call is
        just a more specific version of the first.
    */
    struct BlendFuncSeperateParams
    {
        BlendFuncSeperateParams();
        void reset();
        bool equal(GLenum sFactor, GLenum dFactor);
        bool equal(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
        GLenum d_sfactorRGB, d_dfactorRGB, d_sfactorAlpha, d_dfactorAlpha;
    };
    struct PortParams
    {
        PortParams();
        void reset();
        bool equal(GLint x, GLint y, GLsizei width, GLsizei height);
        GLint d_x, d_y;
        GLsizei d_width, d_height;
    };
    struct BindBufferParams
    {
        BindBufferParams();
        void reset();
        bool equal(GLenum target, GLuint buffer);
        GLenum d_target;
        GLuint d_buffer;
    };
    struct BoundTexture
    {
        BoundTexture();
        void bindTexture(GLenum target, GLuint texture);
        GLenum d_target;
        GLuint d_texture;
    };


    OpenGLBaseStateChangeWrapper();
    virtual ~OpenGLBaseStateChangeWrapper();


    //! Due to unknown changes of states between each time CEGUI gets rendered, we will invalidate
    //! all states on CPU-side so that the following calls will definitely change the states on GPU
    void reset();

    //! Functions wrapping the gl* function calls to improve performance by storing the parameters and
    //!  only calling the OpenGL functions when actual state changes are taking place.
    virtual void bindVertexArray(GLuint vertexArray) = 0;
    void useProgram(GLuint program);
    void blendFunc(GLenum sfactor, GLenum dfactor);
    void blendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
    void viewport(GLint x, GLint y, GLsizei width, GLsizei height);
    void scissor(GLint x, GLint y, GLsizei width, GLsizei height);
    void bindBuffer(GLenum target, GLuint buffer);
    void enable(GLenum capability);
    void disable(GLenum capability);
    void bindTexture(GLenum target, GLuint texture);

    /*
    \brief
        This function takes the number representing the texture position as unsigned integer, 
        not the actual OpenGL value for the position (GL_TEXTURE0, GL_TEXTURE1).

    \param texture_position
        Value representing the texture position as integer, it will be used in the following way to get the
        OpenGL Texture position: (GL_TEXTURE0 + texture_position)
    */
    void activeTexture(unsigned int texture_position);

    /*
    \brief
        Returns the number representing the last active texture's position. This value is the one that was last
        set using this wrapper. No OpenGL getter function is called to retrieve the actual state of the variable,
        which means that changes resulting from OpenGL calls done outside this wrapper, will not be considered.

    \return
        An unsigned int representing the currently active texture.
    */
    unsigned int getActiveTexture() const;

    /*
    \brief
        Returns the ID of the bound vertex array. No OpenGL getter function is called to retrieve the actual state of 
        the variable, which means that changes resulting from OpenGL calls done outside this wrapper, will not be
        considered.

    \return
        An unsigned int representing the bound vertex array.
    */
    GLuint getBoundVertexArray() const;

    /*
    \brief
        Returns the ID of the OpenGL shader program that is set to be used . No OpenGL getter function is called to
        retrieve the actual state of the variable, which means that changes resulting from OpenGL calls done outside 
        this wrapper, will not be considered.

    \return
        An unsigned int representing the ID of the OpenGL shader program.
    */
    GLuint getUsedProgram() const;

    /*
    \brief
        Returns a struct containing the parameters that were set for the blend function. No OpenGL getter function is
        called to retrieve the actual state of the variables, which means that changes resulting from OpenGL calls done
        outside this wrapper, will not be considered.

    \return
        A struct containing the parameters for the blend function.
    */
    BlendFuncSeperateParams getBlendFuncParams() const;

    /*
    \brief
        Returns a struct containing the parameters that were set for the viewport function. No OpenGL getter function is
        called to retrieve the actual state of the variables, which means that changes resulting from OpenGL calls done
        outside this wrapper, will not be considered.

    \return
        A struct containing the parameters for the viewport function.
    */
    PortParams getViewportParams() const;

    /*
    \brief
        Returns a struct containing the parameters that were set for the scissor function. No OpenGL getter function is
        called to retrieve the actual state of the variables, which means that changes resulting from OpenGL calls done
        outside this wrapper, will not be considered.

    \return
        A struct containing the parameters for the scissor function.
    */
    PortParams getScissorParams() const; 

    /*
    \brief
        Returns a struct containing the parameters that were set for the bindBuffer function. No OpenGL getter function is
        called to retrieve the actual state of the variables, which means that changes resulting from OpenGL calls done
        outside this wrapper, will not be considered.

    \return
        A struct containing the parameters for the bindBuffer function.
    */
    BindBufferParams getBoundBuffer() const;
    
    /*
    \brief
        Returns an integers representing if an OpenGL state was enabled, disabled or not set. No OpenGL getter function is
        called to retrieve the actual state of the variables, which means that changes resulting from OpenGL calls done
        outside this wrapper, will not be considered.

    \param capability
        The OpenGL state's OpenGL enum.

    \return
        0 if the requested state has been disabled,
        1 if the requested state has been enabled,
        -1 if the requested state has never been set using this class.
    */
    int isStateEnabled(GLenum capability) const;

protected:
    GLuint                      d_vertexArrayObject;
    GLuint                      d_shaderProgram;
    BlendFuncSeperateParams     d_blendFuncSeperateParams;
    PortParams                  d_viewPortParams;
    PortParams                  d_scissorParams;
    BindBufferParams            d_bindBufferParams;
    //! List of enabled/disabled OpenGL states
    std::map<GLenum, bool>      d_enabledOpenGLStates;
    //! The active texture saved as integer and not as OpenGL enum
    unsigned int                d_activeTexturePosition;
    //! List of bound textures, the position in the vector defines the active texture it is bound to
    std::vector<BoundTexture>   d_boundTextures;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
