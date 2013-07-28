/***********************************************************************
    filename:   ShaderParameterBindings.h
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

#include "glm/glm.hpp"

#include <map>
#include <string>

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
enum ShaderParamType
{
    //! A regular integer type
    SPT_INT,
    //! A regular float type
    SPT_FLOAT,
    //! A pointer to a CEGUI Texture
    SPT_TEXTURE,
    //! A pointer to a 4x4 Matrix
    SPT_MATRIX,

    //! Total number of shader parameter types
    SPT_COUNT
};
//----------------------------------------------------------------------------//

/*!
\brief
    The class 
*/  
class ShaderParameter
{
public:
    ShaderParameter()
    {}

    virtual ~ShaderParameter()
    {}

    /*!
    \brief
        Returns the type of the shader parameter

    \return ShaderParamType
        The type of the shader parameter
    */
    virtual ShaderParamType getType() const = 0;
};

/*!
\brief
    The class 
*/  
class ShaderParameterFloat : public ShaderParameter
{
public:
    ShaderParameterFloat(float parameterValue)
    : d_parameterValue(parameterValue)
    {}

    // Implementation of the ShaderParameter interface
    virtual ShaderParamType getType() const
    { return SPT_FLOAT; }

    //! The float parameter value
    float d_parameterValue;
};

/*!
\brief
    The class 
*/  
class ShaderParameterInt : public ShaderParameter
{
public:
    ShaderParameterInt(int parameterValue)
    : d_parameterValue(parameterValue)
    {}

    // Implementation of the ShaderParameter interface
    virtual ShaderParamType getType() const
    { return SPT_INT; }

    //! The float parameter value
    int d_parameterValue;
};

/*!
\brief
    The class 
*/  
class ShaderParameterTexture : public ShaderParameter
{
public:
    ShaderParameterTexture(const CEGUI::Texture* parameterValue)
        : d_parameterValue(parameterValue)
    {}

    // Implementation of the ShaderParameter interface
    virtual ShaderParamType getType() const
    { return SPT_TEXTURE; }

    //! The float parameter value
    const CEGUI::Texture* d_parameterValue;
};

/*!
\brief
    The class 
*/  
class ShaderParameterMatrix : public ShaderParameter
{
public:
    ShaderParameterMatrix(const glm::mat4& parameterValue)
        : d_parameterValue(parameterValue)
    {}

    // Implementation of the ShaderParameter interface
    virtual ShaderParamType getType() const
    { return SPT_MATRIX; }

    //! The float parameter value
    glm::mat4 d_parameterValue;
};

/*!
\brief

*/
class CEGUIEXPORT ShaderParameterBindings
{
public:
    typedef std::map<std::string, ShaderParameter*> ShaderParameterBindingsMap;

    ShaderParameterBindings();
    ~ShaderParameterBindings();



    /*!
    \brief
        Adds a shader parameter to the parameter bindings

    \param parameterName
        The name of the parameter as used by the shader
        
    \param shaderParameter
        The pointer to the ShaderParameter
    */
    void setParameter(const std::string& parameterName, ShaderParameter* shaderParameter);

    /*!
    \brief
        Adds a matrix shader parameter to the parameter bindings

    \param parameterName
        The name of the parameter as used by the shader
        
    \param matrix
        The pointer to the matrix
    */
    void setParameter(const std::string& parameterName, const glm::mat4& matrix);

    /*!
    \brief
        Adds a texture shader parameter to the parameter bindings

    \param parameterName
        The name of the parameter as used by the shader
        
    \param texture
        The pointer to the CEGUI::Texture
    */
    void setParameter(const std::string& parameterName, const CEGUI::Texture* texture);



        /*!
    \brief
        Adds a texture shader parameter to the parameter bindings

    \param parameterName
        The name of the parameter as used by the shader
        
    \return
        The pointer to the required ShaderParameter. Will return 0 if a parameter
        with the specified name was not set.
    */
    ShaderParameter* getParameter(const std::string& parameterName) const;
    
    /*!
    \brief
        Sets the ShaderParameter in the map to 0, which means that the shader parameter
        will remain unchanged during rendering

    \param parameterName
        The name of the parameter as used by the shader
    */
    void removeParameter(const std::string& parameterName);

    const ShaderParameterBindingsMap& getShaderParameterBindings();

protected:
    //! Map of the names of the shader parameter and the respective shader parameter value
    ShaderParameterBindingsMap d_shaderParameterBindings;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif


#endif
