#ifndef _CEGUIOpenGL3StateChangeWrapper_h_
#define _CEGUIOpenGL3StateChangeWrapper_h_


#include <GL/glew.h>

#include "CEGUI/RendererModules/OpenGL3/Renderer.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{

	/*!
	\brief
	OpenGL3StateChangeWrapper - wraps OpenGL calls and checks for redundant calls beforehand
	*/
	class OPENGL_GUIRENDERER_API OpenGL3StateChangeWrapper
	{
	protected:
		struct BlendFuncParams
		{
			BlendFuncParams();
			void reset();
			bool equal(GLenum sFactor, GLenum dFactor);
			GLenum d_sFactor, d_dFactor;
		};
		struct BlendFuncSeperateParams
		{
			BlendFuncSeperateParams();
			void reset();
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

	public:
    //! constructor.
    OpenGL3StateChangeWrapper(OpenGL3Renderer& owner);
    //! destructor
    virtual ~OpenGL3StateChangeWrapper();

	void reset();

	void bindVertexArray(GLuint vertexArray);
	void blendFunc(GLenum sfactor, GLenum dfactor);
	void blendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
	void viewport(GLint x, GLint y, GLsizei width, GLsizei height);
	void scissor(GLint x, GLint y, GLsizei width, GLsizei height);
	void bindBuffer(GLenum target, GLuint buffer);

protected:
	GLuint						d_vertexArrayObject;
	BlendFuncParams				d_blendFuncParams;
	BlendFuncSeperateParams		d_blendFuncSeperateParams;
	PortParams					d_viewPortParams;
	PortParams					d_scissorParams;
	BindBufferParams			d_bindBufferParams;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
