--
-- CEGUI premake configuration script
--

--
-- OpenGL renderer stuff
-- we always build the opengl renderer
--
-- can be either "tga", "silly", "devil", "freeimage" or "corona"
OPENGL_IMAGECODEC = "tga"

--
-- DirectX
--
DIRECTX81_RENDERER = false
DIRECTX9_RENDERER = false

--
-- Samples
-- remember you have to edit CEGUISamplesConfig.h as well this just controls
-- dependencies etc. if the renderer is disabled this has no effect
--
SAMPLES_GL = true
SAMPLES_DX81 = true
SAMPLES_DX9 = true

--
-- Lua
--
TOLUA_STATIC = true
