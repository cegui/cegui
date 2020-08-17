%module(directors="1") CEGUIOpenGL
%{
#include "CEGUI/CEGUI.h"
#include "CEGUI/Config.h"
#include "CEGUI/RendererModules/OpenGL/GL.h"
#include "CEGUI/RendererModules/OpenGL/RendererBase.h"
#include "CEGUI/RendererModules/OpenGL/GL3Renderer.h"
#include "CEGUI/RendererModules/OpenGL/GLRenderer.h"

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wcast-function-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-label"
#endif
%}

%import "CEGUI.i"
%import "CEGUI/Config.h"

%include "CEGUI/RendererModules/OpenGL/GL.h"
%include "CEGUI/RendererModules/OpenGL/RendererBase.h"
%include "CEGUI/RendererModules/OpenGL/GL3Renderer.h"
%include "CEGUI/RendererModules/OpenGL/GLRenderer.h"
