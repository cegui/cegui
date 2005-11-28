# Microsoft Developer Studio Project File - Name="CEGUIBase" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CEGUIBase - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BaseSystem.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BaseSystem.mak" CFG="CEGUIBase - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CEGUIBase - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CEGUIBase - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CEGUIBase - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../bin"
# PROP Intermediate_Dir "Output/release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\include" /I "..\..\..\..\dependencies\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "CEGUIBASE_EXPORTS" /FD /c
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
# ADD LINK32 freetype2110.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /pdb:none /machine:I386 /nodefaultlib:"LIBCMT" /out:"../../../../bin/CEGUIBase.dll" /implib:"../../../../lib/CEGUIBase.lib" /libpath:"..\..\..\..\lib" /libpath:"..\..\..\..\dependencies\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "..\..\..\..\bin\CEGUIBase.dll" "..\..\..\..\Samples\bin"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "CEGUIBase - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../bin"
# PROP Intermediate_Dir "Output/debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\include" /I "..\..\..\..\dependencies\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "CEGUIBASE_EXPORTS" /D "_STLP_DEBUG" /FD /GZ /c
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
# ADD LINK32 freetype2110_D.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /nodefaultlib:"LIBCMTD" /out:"../../../../bin/CEGUIBase_d.dll" /implib:"../../../../lib/CEGUIBase_d.lib" /pdbtype:sept /libpath:"..\..\..\..\lib" /libpath:"..\..\..\..\dependencies\lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "..\..\..\..\bin\CEGUIBase_d.dll" "..\..\..\..\Samples\bin"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "CEGUIBase - Win32 Release"
# Name "CEGUIBase - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "tinyxml"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\tinyxml\tinystr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\tinyxml\tinystr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\tinyxml\tinyxml.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\tinyxml\tinyxml.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\tinyxml\tinyxmlerror.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\tinyxml\tinyxmlparser.cpp
# End Source File
# End Group
# Begin Group "pcre (source)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\pcre\chartables.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\pcre\get.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\pcre\maketables.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\pcre\pcre.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\pcre\pcreposix.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\pcre\study.c
# End Source File
# End Group
# Begin Group "Falagard (source)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\falagard\CEGUIFalagard_xmlHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\falagard\CEGUIFalComponentBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\falagard\CEGUIFalDimensions.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\falagard\CEGUIFalFrameComponent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\falagard\CEGUIFalImageryComponent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\falagard\CEGUIFalImagerySection.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\falagard\CEGUIFalLayerSpecification.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\falagard\CEGUIFalNamedArea.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\falagard\CEGUIFalPropertyDefinition.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\falagard\CEGUIFalPropertyDefinitionBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\falagard\CEGUIFalPropertyLinkDefinition.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\falagard\CEGUIFalPropertyInitialiser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\falagard\CEGUIFalSectionSpecification.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\falagard\CEGUIFalStateImagery.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\falagard\CEGUIFalTextComponent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\falagard\CEGUIFalWidgetComponent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\falagard\CEGUIFalWidgetLookFeel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\falagard\CEGUIFalWidgetLookManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\falagard\CEGUIFalXMLEnumHelper.cpp
# End Source File
# End Group
# Begin Group "elements (source)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIButtonBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUICheckbox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUICheckboxProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUICombobox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIComboboxProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIComboDropList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIDragContainer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIDragContainerProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIEditbox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIEditboxProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIFrameWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIFrameWindowProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIGUISheet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIItemEntry.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIItemListBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIItemListBaseProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIListbox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIListboxItem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIListboxProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIListboxTextItem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIListHeader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIListHeaderProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIListHeaderSegment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIListHeaderSegmentProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIMenubar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIMenuBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIMenuBaseProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIMenuItem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIMultiColumnList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIMultiColumnListProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIMultiLineEditbox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIMultiLineEditboxProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIPopupMenu.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIPopupMenuProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIProgressBarProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIPushButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIRadioButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIRadioButtonProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIScrollablePane.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIScrollablePaneProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIScrollbar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIScrollbarProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIScrolledContainer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIScrolledContainerProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUISlider.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUISliderProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUISpinner.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUISpinnerProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUITabButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUITabControl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUITabControlProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIThumb.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUIThumbProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUITitlebar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUITitlebarProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUITooltip.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\elements\CEGUITooltipProperties.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIcolour.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIColourRect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIConfig_xmlHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUICoordConverter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIDefaultResourceProvider.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIEventArgs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIEventSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIEventSignal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIEventSignalSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIExceptions.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIFactoryModule.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIFont.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIFont_xmlHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIFontManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIGlobalEventSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIGUILayout_xmlHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIImage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIImageset.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIImageset_xmlHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIImagesetManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUILogger.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIMouseCursor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIProperty.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIPropertyHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIPropertySet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIRect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIRefPtr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIRenderCache.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIRenderer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIScheme.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIScheme_xmlHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUISchemeManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIScriptModule.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUISize.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUISystem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUITexture.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUITextUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIVector.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIWin32XMLSelectHack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIWindowFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIWindowFactoryManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIWindowManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIWindowProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIXMLAttributes.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIXMLHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CEGUIXMLParser.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "pcre (headers)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\pcre\pcre.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\pcre\pcreconfig.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\pcre\pcreposix.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\pcre\ucp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\pcre\ucpinternal.h
# End Source File
# End Group
# Begin Group "Falagard (headers)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\include\falagard\CEGUIFalagard_xmlHandler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\falagard\CEGUIFalComponentBase.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\falagard\CEGUIFalDimensions.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\falagard\CEGUIFalEnums.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\falagard\CEGUIFalFrameComponent.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\falagard\CEGUIFalImageryComponent.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\falagard\CEGUIFalImagerySection.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\falagard\CEGUIFalLayerSpecification.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\falagard\CEGUIFalNamedArea.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\falagard\CEGUIFalPropertyDefinition.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\falagard\CEGUIFalPropertyDefinitionBase.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\falagard\CEGUIFalPropertyLinkDefinition.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\falagard\CEGUIFalPropertyInitialiser.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\falagard\CEGUIFalSectionSpecification.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\falagard\CEGUIFalStateImagery.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\falagard\CEGUIFalTextComponent.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\falagard\CEGUIFalWidgetComponent.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\falagard\CEGUIFalWidgetLookFeel.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\falagard\CEGUIFalWidgetLookManager.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\falagard\CEGUIFalXMLEnumHelper.h
# End Source File
# End Group
# Begin Group "elements (headers)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIButtonBase.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUICheckbox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUICheckboxProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUICombobox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIComboboxProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIComboDropList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIDragContainer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIDragContainerProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIEditbox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIEditboxProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIFrameWindow.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIFrameWindowProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIGUISheet.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIItemEntry.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIItemListBase.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIItemListBaseProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIListbox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIListboxItem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIListboxProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIListboxTextItem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIListHeader.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIListHeaderProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIListHeaderSegment.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIListHeaderSegmentProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIMenubar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIMenuBase.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIMenuBaseProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIMenuItem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIMultiColumnList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIMultiColumnListProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIMultiLineEditbox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIMultiLineEditboxProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIPopupMenu.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIPopupMenuProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIProgressBar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIProgressBarProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIPushButton.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIRadioButton.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIRadioButtonProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIScrollablePane.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIScrollablePaneProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIScrollbar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIScrollbarProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIScrolledContainer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIScrolledContainerProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUISlider.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUISliderProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUISpinner.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUISpinnerProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUITabButton.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUITabControl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUITabControlProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUITabPane.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIThumb.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUIThumbProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUITitlebar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUITitlebarProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUITooltip.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\elements\CEGUITooltipProperties.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\include\CEGUI.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIBase.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIcolour.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIColourRect.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIConfig.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIConfig_xmlHandler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUICoordConverter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIDataContainer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIDefaultResourceProvider.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIEvent.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIEventArgs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIEventSet.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIExceptions.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIFactoryModule.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIFont.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIFont_implData.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIFont_xmlHandler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIFontManager.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIFontManager_implData.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIForwardRefs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIGlobalEventSet.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIGUILayout_xmlHandler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIImage.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIImageset.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIImageset_xmlHandler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIImagesetManager.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIInputEvent.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIIteratorBase.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUILogger.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIMouseCursor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIProperty.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIPropertyHelper.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIPropertySet.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIRect.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIRefPtr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIRenderCache.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIRenderer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIResourceProvider.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIScheme.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIScheme_xmlHandler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUISchemeManager.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIScriptModule.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUISingleton.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUISize.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIString.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUISystem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUITexture.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUITextUtils.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUITinyXMLParser.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIUDim.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIVector.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIVersion.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIWidgetModule.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIWindow.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIWindowFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIWindowFactoryManager.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIWindowManager.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIWindowProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIXercesParser.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIXMLAttributes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIXMLHandler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\CEGUIXMLParser.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
