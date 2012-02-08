#ifndef STANDARD_SHADER_FRAG
#define STANDARD_SHADER_FRAG

namespace CEGUI
{
const char StandardShaderFrag[] = 
    "#version 150 core\n"

    "uniform sampler2D texture0;\n"

    "in vec2 exTexCoord;\n"
    "in vec4 exColour;\n"

    "out vec4 out0;\n"

    "void main(void)\n"
    "{\n"
        "out0 = texture(texture0, exTexCoord) * exColour;\n"
    "}"
    ;
}
#endif