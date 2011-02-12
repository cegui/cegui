if (CEGUI_BUILD_XMLPARSER_EXPAT)
    add_subdirectory(ExpatParser)
endif()

if (CEGUI_BUILD_XMLPARSER_XERCES)
    add_subdirectory(XercesParser)
endif()

if (CEGUI_BUILD_XMLPARSER_LIBXML2)
    add_subdirectory(LibxmlParser)
endif()

if (CEGUI_BUILD_XMLPARSER_TINYXML)
    add_subdirectory(TinyXMLParser)
endif()

if (CEGUI_BUILD_XMLPARSER_RAPIDXML)
    add_subdirectory(RapidXMLParser)
endif()

