%module(directors="1") CEGUIOpenGL
%{
#include "CEGUI/CEGUI.h"
#include "CEGUI/ModuleConfig.h"
#include "CEGUI/RendererModules/OpenGL/GL.h"
#include "CEGUI/RendererModules/OpenGL/RendererBase.h"
#ifdef CEGUI_BUILD_RENDERER_OPENGL3
#include "CEGUI/RendererModules/OpenGL/GL3Renderer.h"
#endif
#ifdef CEGUI_BUILD_RENDERER_OPENGL
#include "CEGUI/RendererModules/OpenGL/GLRenderer.h"
#endif

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wcast-function-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-label"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif
%}

%import "CEGUI.i"
%import "CEGUI/Config.h"
%import "CEGUI/ModuleConfig.h"

%include "CEGUI/RendererModules/OpenGL/GL.h"
%include "CEGUI/RendererModules/OpenGL/RendererBase.h"
#ifdef CEGUI_BUILD_RENDERER_OPENGL3
%include "CEGUI/RendererModules/OpenGL/GL3Renderer.h"
#endif
#ifdef CEGUI_BUILD_RENDERER_OPENGL
%include "CEGUI/RendererModules/OpenGL/GLRenderer.h"
#endif
