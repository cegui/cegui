AUTOMAKE_OPTIONS = dist-bzip2 dist-zip
SUBDIRS = . src include XMLParserModules ImageCodecModules RendererModules WindowRendererSets ScriptingModules Samples makefiles

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = CEGUI.pc CEGUI-OPENGL.pc

nobase_dist_pkgdata_DATA = XMLRefSchema/CEGUIConfig.xsd \
                      XMLRefSchema/Falagard.xsd \
                      XMLRefSchema/Font.xsd \
                      XMLRefSchema/GUILayout.xsd \
                      XMLRefSchema/GUIScheme.xsd \
                      XMLRefSchema/Imageset.xsd \
                      XMLRefSchema/Readme.txt

EXTRA_DIST=bootstrap \
           doxyfile \
           BuildDocs.bat \
           ReadMe.html \
           TinyXML-License \
           bin \
           documentation \
           lib
