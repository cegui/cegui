# Microsoft Developer Studio Project File - Name="CEGUISampleHelper" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CEGUISampleHelper - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SampleHelper.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SampleHelper" CFG="CEGUISampleHelper - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CEGUISampleHelper - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CEGUISampleHelper - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CEGUISampleHelper - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../Samples/bin"
# PROP Intermediate_Dir "Output/release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\Samples\common\include" /I "..\..\..\..\include" /I "..\..\..\..\dependencies\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "CEGUISAMPLE_EXPORTS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 CEGUIBase.lib DirectX81GUIRenderer.lib DirectX9GUIRenderer.lib OpenGLGUIRenderer.lib d3d8.lib d3d9.lib dinput8.lib dxguid.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /nodefaultlib:"LIBCMT" /out:"../../../../Samples/bin/CEGUISampleHelper.dll" /implib:"../../../../lib/CEGUISampleHelper.lib" /libpath:"..\..\..\..\lib" /libpath:"..\..\..\..\dependencies\lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "CEGUISampleHelper - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../Samples/bin"
# PROP Intermediate_Dir "Output/debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\Samples\common\include" /I "..\..\..\..\include" /I "..\..\..\..\dependencies\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "CEGUIBASE_EXPORTS" /D "_STLP_DEBUG" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 CEGUIBase_d.lib DirectX81GUIRenderer_d.lib DirectX9GUIRenderer_d.lib OpenGLGUIRenderer_d.lib d3d8.lib d3d9.lib dinput8.lib dxguid.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /nodefaultlib:"LIBCMTD" /out:"../../../../Samples/bin/CEGUISampleHelper_d.dll" /implib:"../../../../lib/CEGUISampleHelper_d.lib" /pdbtype:sept /libpath:"..\..\..\..\lib" /libpath:"..\..\..\..\dependencies\lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "CEGUISampleHelper - Win32 Release"
# Name "CEGUISampleHelper - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\Samples\common\src\CEGuiD3D81BaseApplication.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Samples\common\src\CEGuiD3D9BaseApplication.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Samples\common\src\CEGuiIrrlichtBaseApplication.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Samples\common\src\CEGuiOgreBaseApplication.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Samples\common\src\CEGuiOpenGLBaseApplication.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Samples\common\src\CEGuiRendererSelector.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Samples\common\src\CEGuiSample.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Samples\common\src\Win32AppHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Samples\common\src\Win32CEGuiRendererSelector.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\Samples\common\include\CEGuiBaseApplication.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Samples\common\include\CEGuiD3D81BaseApplication.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Samples\common\include\CEGuiD3D9BaseApplication.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Samples\common\include\CEGuiIrrlichtBaseApplication.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Samples\common\include\CEGuiOgreBaseApplication.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Samples\common\include\CEGuiOpenGLBaseApplication.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Samples\common\include\CEGuiRendererSelector.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Samples\common\include\CEGuiSample.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Samples\common\include\Win32AppHelper.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Samples\common\include\Win32CEGuiRendererSelector.h
# End Source File
# End Group
# End Target
# End Project
