#!/usr/bin/env python

#/***********************************************************************
#    filename:   generate.py
#    created:    13/8/2010
#    author:     Martin Preisler (with many bits taken from python ogre)
#
#    purpose:    Generates all CEGUI related python bindings
#*************************************************************************/
#/***************************************************************************
# *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
# *
# *   Thanks to Roman Yakovenko for advices and great work on Py++!
# *   Thanks to Andy Miller for his python-ogre CEGUI bindings!
# *
# *   License: generator is GPL3 (python ogre code generators are also GPL)
# *            generated code is MIT as the rest of CEGUI

import generateCEGUI

import generateCEGUIOpenGLRenderer
import generateCEGUIOgreRenderer
import generateCEGUINullRenderer 
 
import commonUtils

import sys, time

if __name__ == "__main__":
    print "Generating CEGUI bindings..."
    start_time = time.clock()
    generateCEGUI.generateCode()
    print "Finished CEGUI bindings... (took %f seconds)\n\n" % ((time.clock() - start_time))
    
    print "Generating CEGUI OpenGLRenderer bindings..."
    start_time = time.clock()
    generateCEGUIOpenGLRenderer.generateCode()
    print "Finished CEGUI OpenGLRenderer bindings... (took %f seconds)\n\n" % ((time.clock() - start_time))

    print "Generating CEGUI OgreRenderer bindings..."
    start_time = time.clock()
    generateCEGUIOgreRenderer.generateCode()
    print "Finished CEGUI OgreRenderer bindings... (took %f seconds)\n\n" % ((time.clock() - start_time))

    print "Generating CEGUI NullRenderer bindings..."
    start_time = time.clock()
    generateCEGUINullRenderer.generateCode()
    print "Finished CEGUI NullRenderer bindings... (took %f seconds)\n\n" % ((time.clock() - start_time))
