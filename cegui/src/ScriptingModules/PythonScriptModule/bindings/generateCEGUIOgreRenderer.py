#!/usr/bin/env python

#/***********************************************************************
#    filename:   generateCEGUIOgreRenderer.py
#    created:    12/11/2010
#    author:     Martin Preisler (with many bits taken from python ogre)
#
#    purpose:    Generates CEGUI Ogre Renderer binding code
#*************************************************************************/
#/***************************************************************************
# *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
# *
# *   Thanks to Roman Yakovenko for advices and great work on Py++!
# *   Thanks to Andy Miller for his python-ogre CEGUI bindings!
# *
# *   License: generator is GPL3 (python ogre code generators are also GPL)
# *            generated code is MIT as the rest of CEGUI
 
import os

from pyplusplus import function_transformers as ft
from pyplusplus.module_builder import call_policies
from pygccxml import declarations

import commonUtils
import generateCEGUI

PACKAGE_NAME = "PyCEGUIOgreRenderer"
PACKAGE_VERSION = commonUtils.GLOBAL_PACKAGE_VERSION
MODULE_NAME = PACKAGE_NAME

OUTPUT_DIR = os.path.join(commonUtils.OUTPUT_DIR, "CEGUIOgreRenderer")

def filterDeclarations(mb):
    # by default we exclude everything and only include what we WANT in the module
    mb.global_ns.exclude()
    
    CEGUI_ns = mb.global_ns.namespace("CEGUI")
    
    # RendererModules/Ogre/CEGUIOgreRenderer.h
    renderer = CEGUI_ns.class_("OgreRenderer")
    renderer.include()
    renderer.noncopyable = True

    # RendererModules/Ogre/CEGUIOgreResourceProvider.h
    resourceprovider = CEGUI_ns.class_("OgreResourceProvider")
    resourceprovider.include()
    resourceprovider.noncopyable = True

    # RendererModules/Ogre/CEGUIOgreImageCodec.h
    imagecodec = CEGUI_ns.class_("OgreImageCodec")
    imagecodec.include()
    imagecodec.noncopyable = True

    commonUtils.excludeAllPrivate(CEGUI_ns)

    # for users using python-ogre in conjunction to PyCEGUI
    mb.global_ns.class_( "::Ogre::RenderTarget" ).already_exposed = True
    mb.global_ns.class_( "::Ogre::TexturePtr" ).already_exposed = True

def generateCode():
    # "CEGUIBASE_EXPORTS" seems to help with internal compiler error with VS2008SP1 and gccxml 0.9
    mb = commonUtils.createModuleBuilder("python_CEGUIOgreRenderer.h", ["OGRE_GUIRENDERER_EXPORTS", "CEGUIBASE_EXPORTS"])
    CEGUI_ns = mb.global_ns.namespace("CEGUI")
    
    mb.register_module_dependency(generateCEGUI.OUTPUT_DIR)

    commonUtils.addSupportForString(mb)
                            
    filterDeclarations(mb)

    commonUtils.setDefaultCallPolicies(CEGUI_ns)
     
    ## add additional version information to the module to help identify it correctly 
    commonUtils.addVersionInfo(mb, PACKAGE_NAME, PACKAGE_VERSION)
    
    # Creating code creator. After this step you should not modify/customize declarations.
    mb.build_code_creator(module_name = MODULE_NAME, doc_extractor = commonUtils.createDocumentationExtractor())
    
    commonUtils.writeModule(mb, OUTPUT_DIR)

if __name__ == "__main__":
    generateCode()

