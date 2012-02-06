/***********************************************************************
    filename:   CEGUIOpenGL3Renderer.h
    created:    Sun Jan 11 2009
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIOpenGL3Renderer_h_
#define _CEGUIOpenGL3Renderer_h_

#include "../../Base.h"
#include "../../Renderer.h"
#include "../../Size.h"
#include "../../Vector.h"
#include <GL/glew.h>
#include <vector>
#include <map>

#include "glm/glm.hpp"

#if (defined( __WIN32__ ) || defined( _WIN32 )) && !defined(CEGUI_STATIC)
#   ifdef CEGUIOPENGL3RENDERER_EXPORTS
#       define OPENGL_GUIRENDERER_API __declspec(dllexport)
#   else
#       define OPENGL_GUIRENDERER_API __declspec(dllimport)
#   endif
#else
#   define OPENGL_GUIRENDERER_API
#endif

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
	class Shader;
	class OpenGL3Texture;
	class OpenGLTextureTarget;
	class OpenGLGeometryBuffer;
	class OGLTextureTargetFactory;

/*!
\brief
    Renderer class to interface with OpenGL
*/
class OPENGL_GUIRENDERER_API OpenGL3Renderer : public Renderer
{
public:
    /*!
    \brief
        Convenience function that creates the required objects to initialise the
        CEGUI system.

        The created Renderer will use the current OpenGL viewport as it's
        default surface size.

        This will create and initialise the following objects for you:
        - CEGUI::OpenGL3Renderer
        - CEGUI::DefaultResourceProvider
        - CEGUI::System

    \param tt_type
        Specifies one of the TextureTargetType enumerated values indicating the
        desired TextureTarget type to be used.  Defaults to TTT_AUTO.

    \return
        Reference to the CEGUI::OpenGL3Renderer object that was created.
    */
    static OpenGL3Renderer& bootstrapSystem();

    /*!
    \brief
        Convenience function that creates the required objects to initialise the
        CEGUI system.

        The created Renderer will use the current OpenGL viewport as it's
        default surface size.

        This will create and initialise the following objects for you:
        - CEGUI::OpenGL3Renderer
        - CEGUI::DefaultResourceProvider
        - CEGUI::System

    \param display_size
        Size object describing the initial display resolution.

    \param tt_type
        Specifies one of the TextureTargetType enumerated values indicating the
        desired TextureTarget type to be used.  Defaults to TTT_AUTO.

    \return
        Reference to the CEGUI::OpenGL3Renderer object that was created.
    */
    static OpenGL3Renderer& bootstrapSystem(const Sizef& display_size);

    /*!
    \brief
        Convenience function to cleanup the CEGUI system and related objects
        that were created by calling the bootstrapSystem function.

        This function will destroy the following objects for you:
        - CEGUI::System
        - CEGUI::DefaultResourceProvider
        - CEGUI::OpenGL3Renderer

    \note
        If you did not initialise CEGUI by calling the bootstrapSystem function,
        you should \e not call this, but rather delete any objects you created
        manually.
    */
    static void destroySystem();

    /*!
    \brief
        Create an OpenGL3Renderer object.

    \param tt_type
        Specifies one of the TextureTargetType enumerated values indicating the
        desired TextureTarget type to be used.
    */
    static OpenGL3Renderer& create();

    /*!
    \brief
        Create an OpenGL3Renderer object.

    \param display_size
        Size object describing the initial display resolution.

    \param tt_type
        Specifies one of the TextureTargetType enumerated values indicating the
        desired TextureTarget type to be used.
    */
    static OpenGL3Renderer& create(const Sizef& display_size);

    /*!
    \brief
        Destroy an OpenGL3Renderer object.

    \param renderer
        The OpenGL3Renderer object to be destroyed.
    */
    static void destroy(OpenGL3Renderer& renderer);

    // implement Renderer interface
    RenderingRoot& getDefaultRenderingRoot();
    GeometryBuffer& createGeometryBuffer();
    void destroyGeometryBuffer(const GeometryBuffer& buffer);
    void destroyAllGeometryBuffers();
    TextureTarget* createTextureTarget();
    void destroyTextureTarget(TextureTarget* target);
    void destroyAllTextureTargets();
    Texture& createTexture(const String& name);
    Texture& createTexture(const String& name,
                           const String& filename,
                           const String& resourceGroup);
    Texture& createTexture(const String& name, const Sizef& size);
    void destroyTexture(Texture& texture);
    void destroyTexture(const String& name);
    void destroyAllTextures();
    Texture& getTexture(const String& name) const;
    void beginRendering();
    void endRendering();
    void setDisplaySize(const Sizef& sz);
    const Sizef& getDisplaySize() const;
    const Vector2f& getDisplayDPI() const;
    uint getMaxTextureSize() const;
    const String& getIdentifierString() const;

    /*!
    \brief
        Create a texture that uses an existing OpenGL texture with the specified
        size.  Note that it is your responsibility to ensure that the OpenGL
        texture is valid and that the specified size is accurate.

    \param sz
        Size object that describes the pixel size of the OpenGL texture
        identified by \a tex.

    \param name
        String holding the name for the new texture.  Texture names must be
        unique within the Renderer.

    \return
        Texture object that wraps the OpenGL texture \a tex, and whose size is
        specified to be \a sz.

    \exceptions
        - AlreadyExistsException - thrown if a Texture object named \a name
          already exists within the system.
    */
    Texture& createTexture(const String& name, GLuint tex, const Sizef& sz);

    /*!
    \brief
        Tells the renderer to initialise some extra states beyond what it
        directly needs itself for CEGUI.

        This option is useful in cases where you've made changes to the default
        OpenGL state and do not want to save/restore those between CEGUI
        rendering calls.  Note that this option will not deal with every
        possible state or extension - if you want a state added here, make a
        request and we'll consider it ;)
    */
    void enableExtraStateSettings(bool setting);

    /*!
    \brief
        Grabs all the loaded textures from Texture RAM and stores them in a
        local data buffer.  This function invalidates all textures, and
        restoreTextures must be called before any CEGUI rendering is done for
        predictable results.
    */
    void grabTextures();

    /*!
    \brief
        Restores all the loaded textures from the local data buffers previously
        created by 'grabTextures'
    */
    void restoreTextures();

    /*!
    \brief
        Helper to return a valid texture size according to reported OpenGL
        capabilities.

    \param sz
        Size object containing input size.

    \return
        Size object containing - possibly different - output size.
    */
    Sizef getAdjustedTextureSize(const Sizef& sz) const;

    /*!
    \brief
        Utility function that will return \a f if it's a power of two, or the
        next power of two up from \a f if it's not.
    */
    static float getNextPOTSize(const float f);

	//! set the render states for the specified BlendMode.
	void setupRenderingBlendMode(const BlendMode mode, const bool force = false);

	/*!
	\brief
	Helper to return the reference to the pointer to the standard shader of the Renderer

	\return
	Reference to the pointer to the standard shader of the Renderer
	*/
	Shader*& getShaderStandard();

	/*!
	\brief
	Helper to return the attribute location of the position variable in the standard shader

	\return
	Attribute location of the position variable in the standard shader
	*/
	const int getShaderStandardPositionLoc();


	/*!
	\brief
	Helper to return the attribute location of the texture coordinate variable in the standard shader

	\return
	Attribute location of the texture coordinate variable in the standard shader
	*/
	const int getShaderStandardTexCoordLoc();


	/*!
	\brief
	Helper to return the attribute location of the colour variable in the standard shader

	\return
	Attribute location of the colour variable in the standard shader
	*/
	const int getShaderStandardColourLoc();


		/*!
	\brief
	Helper to return the uniform location of the matrix variable in the standard shader

	\return
	Uniform location of the matrix variable in the standard shader
	*/
	const int getShaderStandardMatrixUniformLoc();


			/*!
	\brief
	Helper to return view projection matrix.

	\return
	The view projection matrix.
	*/
	const glm::mat4& getViewProjectionMatrix();


			/*!
	\brief
	Helper to set the view projection matrix.

	\param viewProjectionMatrix
	The view projection matrix.
	*/
	void setViewProjectionMatrix(glm::mat4 viewProjectionMatrix);



	//! Gets the reference to the pointer
private:
    /*!
    \brief
        Constructor for OpenGL Renderer objects

    \param tt_type
        Specifies one of the TextureTargetType enumerated values indicating the
        desired TextureTarget type to be used.
    */
    OpenGL3Renderer();

    /*!
    \brief
        Constructor for OpenGL Renderer objects.

    \param display_size
        Size object describing the initial display resolution.

    \param tt_type
        Specifies one of the TextureTargetType enumerated values indicating the
        desired TextureTarget type to be used.
    */
    OpenGL3Renderer(const Sizef& display_size);

	void initialiseOpenGLShaders();

    /*!
    \brief
        Destructor for OpenGL3Renderer objects
    */
    virtual ~OpenGL3Renderer();

    //! init the extra GL states enabled via enableExtraStateSettings
    void setupExtraStates();

    //! initialise OGLTextureTargetFactory that will generate TextureTargets
    void initialiseTextureTargetFactory();

    //! helper to safely log the creation of a named texture
    static void logTextureCreation(const String& name);
    //! helper to safely log the destruction of a named texture
    static void logTextureDestruction(const String& name);

    //! String holding the renderer identification text.
    static String d_rendererID;
    //! What the renderer considers to be the current display size.
    Sizef d_displaySize;
    //! What the renderer considers to be the current display DPI resolution.
    Vector2f d_displayDPI;
    //! The default rendering root object
    RenderingRoot* d_defaultRoot;
    //! The default RenderTarget (used by d_defaultRoot)
    RenderTarget* d_defaultTarget;
    //! container type used to hold TextureTargets we create.
    typedef std::vector<TextureTarget*> TextureTargetList;
    //! Container used to track texture targets.
    TextureTargetList d_textureTargets;
    //! container type used to hold GeometryBuffers we create.
    typedef std::vector<OpenGLGeometryBuffer*> GeometryBufferList;
    //! Container used to track geometry buffers.
    GeometryBufferList d_geometryBuffers;
    //! container type used to hold Textures we create.
    typedef std::map<String, OpenGL3Texture*, StringFastLessCompare
                     CEGUI_MAP_ALLOC(String, OpenGL3Texture*)> TextureMap;
    //! Container used to track textures.
    TextureMap d_textures;
    //! What the renderer thinks the max texture size is.
    uint d_maxTextureSize;
    //! option of whether to initialise extra states that may not be at default
    bool d_initExtraStates;
    //! pointer to a helper that creates TextureTargets supported by the system.
	OGLTextureTargetFactory* d_textureTargetFactory;
	//! What blend mode we think is active.
	BlendMode d_activeBlendMode;
	//! The OpenGL shader we will use usually
	Shader*			d_shaderStandard;
	//! Position variable location inside the shader, for OpenGL
	int				d_shaderStandardPosLoc;
	//! TexCoord variable location inside the shader, for OpenGL
	int				d_shaderStandardTexCoordLoc;
	//! Color variable location inside the shader, for OpenGL
	int				d_shaderStandardColourLoc;
	//! Matrix uniform location inside the shader, for OpenGL
	int				d_shaderStandardMatrixLoc;
	//! View projection matrix
	glm::mat4		d_viewProjectionMatrix;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif // end of guard _CEGUIOpenGL3Renderer_h_
