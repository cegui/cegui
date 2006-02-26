

Readme : CodeBlocKs with Mingw
==============================

Version 0.2 - 30 Dec 2005 - Steven Gay - mec3@bluewin.ch
Version 0.3 - 1  Jan 2006 - Steven Gay - mec3@bluewin.ch
version 0.4 - 9 Jan 2006 - Steven + WhiteKnight

   ok - CEGUIBase
   ok - TaharezLook
   ok - DirectX9GUIRenderer
   ok - WindowsLook
   ok - FalagardBase
   ok - OpenGLGuiRenderer
   ok - CEGUISampleHelper
   ok - Sample_FirstWindow
   ok - Sample_FalagardDemo1
   ok - Sample_Demo4
   ok - Sample_Demo6
   ok - Sample_Demo7
   ok - lua_and_tolua++
   ok - CEGUILua
   ok - Sample_Demo8
 
   ko - DirectX81GUIRenderer - I didn't try
   ko - IrrlichtGUIRenderer - no lib because no use for me




Specific build Options :
========================

Project CEGUISampleHelper :
-------------------------

 - CEGuiSample.h is missing : #include "CEGUIExceptions.h"

 - If you get a compile error saying something like 
   "undefined reference to `Ogre::MemoryManager::setOwner(char const*, unsigned int, char const*)'"
   then try to add #include "OgreNoMemoryMacros.h" at the beggining of CEGUISample.cpp.
  
 - To run the sample you need to modify plugins.cfg

 - Depending on what renderer you want you need to set diverse build options :

   DirectX9 : Set CEGUI_SAMPLES_USE_DIRECTX_9 in the #defines in the project build options
              Link to the libraries : gdi32, dxguid, dinput8, d3d9
   OGRE     : Set CEGUI_SAMPLES_USE_OPENGL and link to OgreMain and OgreMain_d
   OpenGl   : Set CEGUI_SAMPLES_USE_OGRE and link to glu32, opengl32
   Irrlicht : Set CEGUI_SAMPLES_USE_IRRLICHT and link to ?? (i don't have it)
   

 - If you have troubles with the OpenGl version and have the following compile errors :
	undefined reference to `__glutInitWithExit@12'
	undefined reference to `__glutCreateWindowWithExit@8'
	undefined reference to `__glutCreateMenuWithExit@8'

   It is NOT because you forget to link against glut32 but search 
   for GLUT_DISABLE_ATEXIT_HACK in the file glut.h and read the explanation
   that say you should try to use the #define GLUT_DISABLE_ATEXIT_HACK

   You also need #define : _STDCALL_SUPPORTED .

   If someone finds a better solution, please, tell me ( mec3@bluewin.ch ).

 - To be able to run the ogre sample (i am using Ogre 1.0.5) :
   You must recompile ogreguirenderer.a and .lib (and ogreguirenderer_d.a and dll) 
   with the 0.4.0 CEGui source code. Don't forget to copy the .h and .a 
   to the relevant Ogre dependencies directory!
   Then don't forget to copy back the new ogreguirenderer.a and .dll 
   to the appropriate Cegui project directories!


   However, you must also modify the file cegui/samples/bin/Plugins.cfg
   The line PluginFolder must not contain double-quotes ". For example :

         # To successfully use the Ogre renderer with the samples, you 
         # need to point "PluginFolder" to where the Ogre plugin Modules
         # are to be found. Also, add the D3D systems on additional
         # 'Plugin' lines if you want those available under MS Windows.

         PluginFolder=C:\Documents and Settings\gst\Mes documents\WorkProg\Cpp\library\ogre-1.0.5\release
         Plugin=RenderSystem_GL

   CAREFUL : If you want to run the debug version you will need to change 
   this path to the directory where your debug ddls and exe are!
   A better way would be to create a bin/release and bin/debug for each projects... your turn to work :)


Devil library :
---------------

If you want to use the DEVIL library you will need in some projects 
to add USE_DEVIL_LIBRARY in the compiler options #define 
and link against them in the dependencies/include and dependencies/lib .





Source files to modify :
========================

As usual, someone confused the preprocess define WIN32 and MSVC 
as indicators of the used compiler. It should be :

  _WIN32, __WIN32__   = Windows OS !
  MSVC, _MSC_VER      = compiler Visual C / Studio
  MINGW, __MINGW__    = compiler mingw (__MINGW__ is defined by the compiler)


I will only show the modified code, you will find easily where to make the changes.
Except the two first files, all changes are related to the WIN32/MSVC confusion.

directx9GUIRenderer/d3d9renderer.cpp :
--------------------------------------

- Replace the three occurences of "throw std::exception" by "throw String"

CEGuiSample.cpp :
-----------------

- Because of the preceding change you will need to replace the occurence of
  "catch (std::exception& exc)" by "catch (String& exc)" and adapte the next line.


src/renderers/directx81GUIRenderer/renderer.cpp :
-------------------------------------------------

- Replace the occurences of "throw std::exception" by "throw String" 
  and "catch (std::exception e)" by "catch (String e)"


CEGUIConfig.h : 
-------------
- Comment out the line #define CEGUI_WITH_XERCES if you don't have/want xerces
- You have perhaps else something to set.


CEGUIBase.h :
-------------

#if defined( __WIN32__ ) || defined( _WIN32 )
#   ifdef CEGUIBASE_EXPORTS
#       define CEGUIEXPORT __declspec(dllexport)
#   else
#       if defined (__MINGW32__)
#           define CEGUIEXPORT
#       else
#           define CEGUIEXPORT __declspec(dllimport)
#       endif
#   endif
#       define CEGUIPRIVATE
#else
#       define CEGUIEXPORT
#       define CEGUIPRIVATE
#endif


CEGuiSample.h :
---------------

#if defined( __WIN32__ ) || defined( _WIN32 )
#   ifdef CEGUISAMPLE_EXPORTS
#       define CEGUISAMPLE_API __declspec(dllexport)
#   else
#     if defined( __MINGW__ )
#       define CEGUISAMPLE_API
#     else
#       define CEGUISAMPLE_API __declspec(dllimport)
#     endif
#   endif
#else
#       define CEGUISAMPLE_API
#endif


IrrlichtRendererDef.h :
-----------------------

#if (defined( __WIN32__ ) || defined( _WIN32 ) || defined (WIN32)) && !defined(__MINGW32__)
#   ifdef IRRRENDERER_EXPORTS
#       define IRRLICHT_GUIRENDERER_API __declspec(dllexport)
#   else
#       define IRRLICHT_GUIRENDERER_API __declspec(dllimport)
#   endif
#else
#   define IRRLICHT_GUIRENDERER_API
#endif

openglrenderer.h :
------------------

#if defined( __WIN32__ ) || defined( _WIN32 )
#   ifdef OPENGL_GUIRENDERER_EXPORTS
#       define OPENGL_GUIRENDERER_API __declspec(dllexport)
#   else
#       if !defined( __MINGW32__ )
#           define OPENGL_GUIRENDERER_API __declspec(dllimport)
#       else
#           define OPENGL_GUIRENDERER_API
#       endif
#   endif
#endif

The next change was necessary for me (Steven) but not for WhiteKnight,
so use it if you need.

//#if defined(_WIN32)//  All this taken from glut.h
#if (defined( __WIN32__ ) || defined( _WIN32 )) && !defined( __MINGW32__ )
#  ifndef APIENTRY
#   define GLUT_APIENTRY_DEFINED
#   if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__) || defined(__LCC__)
#    define APIENTRY    __stdcall
#   else
#    define APIENTRY
#   endif
#  endif

directX81GUIRenderer/renderer.h :
---------------------------------

#ifdef DIRECTX81_GUIRENDERER_EXPORTS 
#  define DIRECTX81_GUIRENDERER_API __declspec(dllexport)
#else
# if !defined(__MINGW32__)
#   define DIRECTX81_GUIRENDERER_API __declspec(dllimport)
# else
#   define DIRECTX81_GUIRENDERER_API 
# endif
#endif

directx9GUIRenderer/d3d9renderer.h :
------------------------------------

#if defined (DIRECTX9_GUIRENDERER_EXPORTS) 
#  define DIRECTX9_GUIRENDERER_API __declspec(dllexport)
# else
#   if !defined(__MINGW32__)
#       define DIRECTX9_GUIRENDERER_API __declspec(dllimport)
#   else
#       define DIRECTX9_GUIRENDERER_API
#   endif
# endif



ogrenew_0_1_6\Samples\Common\CEGUIRenderer\include\OgreCEGUIRenderer.h :
------------------------------------------------------------------------

#if defined( __WIN32__ ) || defined( _WIN32 )
#   ifdef OGRE_GUIRENDERER_EXPORTS
#       define OGRE_GUIRENDERER_API __declspec(dllexport)
#   else
#       if defined( __MINGW32__ )
#          define OGRE_GUIRENDERER_API
#       else
#              define OGRE_GUIRENDERER_API __declspec(dllimport)
#       endif
#   endif
#else
#   define OGRE_GUIRENDERER_API
#endif

FalModule.h :
-------------

#if defined( __WIN32__ ) || defined( _WIN32 )
#   ifdef FALAGARDBASE_EXPORTS
#       define FALAGARDBASE_API __declspec(dllexport)
#   else
#       if !defined (__MINGW32__)
#          define FALAGARDBASE_API __declspec(dllimport)
#       else
#          define FALAGARDBASE_API
#       endif
#   endif
#else
#   define FALAGARDBASE_API
#endif

TLModule.h :
------------

#if defined( __WIN32__ ) || defined( _WIN32 )
#   ifdef TAHAREZLOOK_EXPORTS
#       define TAHAREZLOOK_API __declspec(dllexport)
#   else
#       if !defined (__MINGW32__)
#          define TAHAREZLOOK_API __declspec(dllimport)
#       else
#          define TAHAREZLOOK_API
#       endif
#   endif
#else
#   define TAHAREZLOOK_API
#endif

WLModule.h :
------------

#if defined( __WIN32__ ) || defined( _WIN32 )
#   ifdef WINDOWSLOOK_EXPORTS
#       define WINDOWSLOOK_API __declspec(dllexport)
#   else
#       if !defined (__MINGW32__)
#           define WINDOWSLOOK_API __declspec(dllimport)
#       else
#           define WINDOWSLOOK_API
#       endif
#   endif
#else
#   define WINDOWSLOOK_API
#endif




Good luck


// EOF
