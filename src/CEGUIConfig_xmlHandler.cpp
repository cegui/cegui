/***********************************************************************
    filename:   CEGUIConfig_xmlHandler.cpp
    created:    17/7/2004
    author:     Paul D Turner
    
    purpose:    Implements configuration file parser
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#include "CEGUIConfig_xmlHandler.h"
#include "CEGUIExceptions.h"
#include "CEGUISystem.h"
#include "CEGUIXMLAttributes.h"


// Start of CEGUI namespace section
namespace CEGUI
{
    /*************************************************************************
        Implementation Constants
    *************************************************************************/
    const String Config_xmlHandler::CEGUIConfigElement( "CEGUIConfig" );
    const char  Config_xmlHandler::ConfigLogfileAttribute[]         = "Logfile";
    const char  Config_xmlHandler::ConfigSchemeAttribute[]          = "Scheme";
    const char  Config_xmlHandler::ConfigLayoutAttribute[]          = "Layout";
    const char  Config_xmlHandler::ConfigDefaultFontAttribute[]     = "DefaultFont";
    const char  Config_xmlHandler::ConfigInitScriptAttribute[]      = "InitScript";
    const char  Config_xmlHandler::ConfigTerminateScriptAttribute[] = "TerminateScript";
    const char  Config_xmlHandler::ConfigDefaultResourceGroupAttribute[] = "DefaultResourceGroup";
    const char  Config_xmlHandler::ConfigLoggingLevelAttribute[]    = "LoggingLevel";

    void Config_xmlHandler::elementStart(const String& element, const XMLAttributes& attributes)
    {
        // handle root CEGUIConfig element
        if (element == CEGUIConfigElement)
        {
            d_logFilename           = attributes.getValueAsString(ConfigLogfileAttribute);
            d_schemeFilename        = attributes.getValueAsString(ConfigSchemeAttribute);
            d_layoutFilename        = attributes.getValueAsString(ConfigLayoutAttribute);
            d_initScriptFilename    = attributes.getValueAsString(ConfigInitScriptAttribute);
            d_termScriptFilename    = attributes.getValueAsString(ConfigTerminateScriptAttribute);
            d_defaultFontName       = attributes.getValueAsString(ConfigDefaultFontAttribute);
            d_defaultResourceGroup  = attributes.getValueAsString(ConfigDefaultResourceGroupAttribute);

            // handle logging level
            String logLevelStr = attributes.getValueAsString(ConfigLoggingLevelAttribute, "Standard");

            if (logLevelStr == "Errors")
            {
                d_logLevel = Errors;
            }
            else if (logLevelStr == "Warnings")
            {
                d_logLevel = Warnings;
            }
            else if (logLevelStr == "Informative")
            {
                d_logLevel = Informative;
            }
            else if (logLevelStr == "Insane")
            {
                d_logLevel = Insane;
            }
            else
            {
                d_logLevel = Standard;
            }
        }
        // anything else is an error which *should* have already been caught by XML validation
        else
        {
            String message("Config_xmlHandler::startElement - Unexpected data was found while parsing the configuration file: '" + element + "' is unknown.");

            // throw a std::exception (because it won't try and use logger)
            throw message.c_str();
        }

    }

} // End of  CEGUI namespace section
