#ifndef STANDARD_SHADER_VERT
#define STANDARD_SHADER_VERT

namespace CEGUI
{
 const char StandardShaderVert[] = 
	"#version 150 core\n"

	"uniform mat4 modelViewPerspMatrix;\n"

	"in vec3 inPosition;\n"
	"in vec2 inTexCoord;\n"
	"in vec4 inColour;\n"

	"out vec2 exTexCoord;\n"
	"out vec4 exColour;\n"

	"void main(void)\n"
	"{\n"
		"	exTexCoord = inTexCoord;\n"
		"	exColour = inColour;\n"

		"gl_Position = modelViewPerspMatrix * vec4(inPosition, 1.0);\n"
	"}"
	;
}

#endif