/***********************************************************************
    created:    18th July 2013
    author:     Lukas Meindl
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
#ifndef _CEGUIShaderParameterBindings_h_
#define _CEGUIShaderParameterBindings_h_

#include "CEGUI/Base.h"
#include "CEGUI/ModuleConfig.h"
#include <glm/glm.hpp>
#include <map>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{
class Texture;

//----------------------------------------------------------------------------//
/*!
\brief
    Enumerated type that contains the valid types a shader parameter can have
*/
enum class ShaderParamType : int
{
    //! A regular integer type
    Int,
    //! A regular float type
    Float,
    //! A pointer to a CEGUI Texture
    Texture,
    //! A pointer to a 4x4 Matrix
    Matrix4X4,

    //! Total number of shader parameter types
    COUNT
};
//----------------------------------------------------------------------------//

/*!
\brief
    The class is the abstract interface used for all shader parameters that
    are added to the RenderMaterials.
*/
class ShaderParameter
{
public:

    virtual ~ShaderParameter() = default;

    /*!
    \brief
        Returns the type of the shader parameter.

    \return
        The type of the shader parameter.
    */
    virtual ShaderParamType getType() const = 0;

    /*!
    \brief
        Returns a copy of this ShaderParameter on the heap and returns the
        pointer to it. The caller has to take care of memory deallocation.

    \return
        A pointer to the copy of this ShaderParameter.
    */
    virtual ShaderParameter* clone() const = 0;

    /*!
    \brief
        Checks if the ShaderParameters are equal, depending on their type
        and parameter.

    \return
        True if the ShaderParameters are equal.
    */
    virtual bool equal(const ShaderParameter* other_parameter) const = 0;

    /*!
    \brief
        Copies the parameter value from the passed ShaderParameter to this
        one if the classes are of the same type.
    \param other_parameter
        The ShaderParameter from which the parameter value should be taken over.
    */
    virtual void takeOverParameterValue(const ShaderParameter* other_parameter) = 0;
};

/*!
\brief
    The class implements the functionality of the
    ShaderParameter interface for float parameters.
*/
class ShaderParameterFloat : public ShaderParameter
{
public:

    ShaderParameterFloat(float value) : d_parameterValue(value) {}

    ShaderParamType getType() const override { return ShaderParamType::Float; }
    ShaderParameterFloat* clone() const override { return new ShaderParameterFloat(*this); }
    bool equal(const ShaderParameter* other_parameter) const override;
    void takeOverParameterValue(const ShaderParameter* other_parameter) override;

    //! The float parameter value
    float d_parameterValue;
};

/*!
\brief
    The class implements the functionality of the
    ShaderParameter interface for int parameters.
*/
class ShaderParameterInt : public ShaderParameter
{
public:

    ShaderParameterInt(int value) : d_parameterValue(value) {}

    ShaderParamType getType() const override { return ShaderParamType::Int; }
    ShaderParameterInt* clone() const override { return new ShaderParameterInt(*this); }
    bool equal(const ShaderParameter* other_parameter) const override;
    void takeOverParameterValue(const ShaderParameter* other_parameter) override;

    //! The int parameter value
    int d_parameterValue;
};

/*!
\brief
    The class implements the functionality of the
    ShaderParameter interface for Texture parameters.
*/
class ShaderParameterTexture : public ShaderParameter
{
public:

    ShaderParameterTexture(const Texture* value) : d_parameterValue(value) {}

    ShaderParamType getType() const override { return ShaderParamType::Texture; }
    ShaderParameterTexture* clone() const override { return new ShaderParameterTexture(*this); }
    bool equal(const ShaderParameter* other_parameter) const override;
    void takeOverParameterValue(const ShaderParameter* other_parameter) override;

    //! The texture parameter value
    const Texture* d_parameterValue;
};

/*!
\brief
    The class implements the functionality of the
    ShaderParameter interface for 4x4 matrix parameters.
*/
class ShaderParameterMatrix : public ShaderParameter
{
public:

    ShaderParameterMatrix(const glm::mat4& value) : d_parameterValue(value) {}

    ShaderParamType getType() const override { return ShaderParamType::Matrix4X4; }
    ShaderParameterMatrix* clone() const override { return new ShaderParameterMatrix(*this); }
    bool equal(const ShaderParameter* other_parameter) const override;
    void takeOverParameterValue(const ShaderParameter* other_parameter) override;

    //! The matrix parameter value
    glm::mat4 d_parameterValue;
};

//! \brief A set of values associated with shader parameters
class CEGUIEXPORT ShaderParameterBindings final
{
public:

    typedef std::map<std::string, ShaderParameter*> ShaderParameterBindingsMap;

    ~ShaderParameterBindings();

    /*!
    \brief
        Adds a matrix shader parameter to the parameter bindings

    \param parameter_name
        The name of the parameter as used by the shader

    \param matrix
        The pointer to the matrix
    */
    void setParameter(const std::string& parameter_name, const glm::mat4& matrix);

    /*!
    \brief
        Adds a texture shader parameter to the parameter bindings

    \param parameter_name
        The name of the parameter as used by the shader

    \param texture
        The pointer to the CEGUI::Texture
    */
    void setParameter(const std::string& parameter_name, const Texture* texture);

    /*!
    \brief
        Adds a float shader parameter to the parameter bindings

    \param parameter_name
        The name of the parameter as used by the shader

    \param fvalue
        The value of the float parameter
    */
    void setParameter(const std::string& parameter_name, float fvalue);

    /*!
    \brief
        Adds an int shader parameter to the parameter bindings

    \param parameter_name
        The name of the parameter as used by the shader

    \param value
        The value of the int parameter
    */
    void setParameter(const std::string& parameter_name, int value);

    /*!
    \brief
        Returns a pointer to the shader_parameter with the specified parameter name

    \param parameter_name
        The name of the parameter as used by the shader

    \return
        The pointer to the required shader_parameter. Will return 0 if a parameter
        with the specified name was not set.
    */
    ShaderParameter* getParameter(const std::string& parameter_name);

    /*!
    \brief
        Sets the shader_parameter in the map to 0, which means that the shader parameter
        will remain unchanged during rendering

    \param parameter_name
        The name of the parameter as used by the shader
    */
    void removeParameter(const std::string& parameter_name);

    const std::map<std::string, ShaderParameter*>& getShaderParameterBindings() const { return d_shaderParameterBindings; }

protected:

    /*!
    \brief
        Adds a new shader parameter to the parameter bindings. If an old one exists it will be
        deleted and replaced by the new one.

    \param parameter_name
        The name of the parameter as used by the shader

    \param shader_parameter
        The pointer to the shader_parameter
    */
    void setNewParameter(const std::string& parameter_name, ShaderParameter* shader_parameter);

    //! Map of the names of the shader parameter and the respective shader parameter value
    std::map<std::string, ShaderParameter*> d_shaderParameterBindings;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
