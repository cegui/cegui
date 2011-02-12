if (CEGUI_BUILD_IMAGECODEC_CORONA)
    add_subdirectory(CoronaImageCodec)
endif()

if (CEGUI_BUILD_IMAGECODEC_DEVIL)
    add_subdirectory(DevILImageCodec)
endif()

if (CEGUI_BUILD_IMAGECODEC_FREEIMAGE)
    add_subdirectory(FreeImageImageCodec)
endif()

if (CEGUI_BUILD_IMAGECODEC_SILLY)
    add_subdirectory(SILLYImageCodec)
endif()

if (CEGUI_BUILD_IMAGECODEC_STB)
    add_subdirectory(STBImageCodec)
endif()

if (CEGUI_BUILD_IMAGECODEC_TGA)
    add_subdirectory(TGAImageCodec)
endif()

