INCLUDES = -I$(top_srcdir)/cegui/src/ScriptingModules/PythonScriptModule/bindings/output/CEGUI \
           -I$(top_srcdir)/cegui/include \
		   -I$(PYTHON_INCLUDE_DIR)

pyexec_LTLIBRARIES = PyCEGUI.la

# NB: No '@CEGUI_LIB_LINK_FLAGS@' here
PyCEGUI_la_LDFLAGS = -module -avoid-version
PyCEGUI_la_CXXFLAGS = -fvisibility=hidden
PyCEGUI_la_CPPFLAGS = 

if CEGUI_BUILD_STATIC
PyCEGUI_la_CPPFLAGS += -DCEGUI_STATIC
endif

PyCEGUI_la_SOURCES = \
output/CEGUI/AbsoluteDim.pypp.cpp \
output/CEGUI/ActivationEventArgs.pypp.cpp \
output/CEGUI/Affector.pypp.cpp \
output/CEGUI/AnimationEventArgs.pypp.cpp \
output/CEGUI/AnimationInstance.pypp.cpp \
output/CEGUI/AnimationManager.pypp.cpp \
output/CEGUI/Animation.pypp.cpp \
output/CEGUI/BaseDim.pypp.cpp \
output/CEGUI/BasicRenderedStringParser.pypp.cpp \
output/CEGUI/BiDiVisualMapping.pypp.cpp \
output/CEGUI/BoundSlot.pypp.cpp \
output/CEGUI/ButtonBase.pypp.cpp \
output/CEGUI/PyCEGUI_enumerations.pypp.cpp \
output/CEGUI/PyCEGUI.main.cpp \
output/CEGUI/CentredRenderedString.pypp.cpp \
output/CEGUI/Checkbox.pypp.cpp \
output/CEGUI/colour.pypp.cpp \
output/CEGUI/ColourRect.pypp.cpp \
output/CEGUI/Combobox.pypp.cpp \
output/CEGUI/ComboDropList.pypp.cpp \
output/CEGUI/ComponentArea.pypp.cpp \
output/CEGUI/Connection.pypp.cpp \
output/CEGUI/CoordConverter.pypp.cpp \
output/CEGUI/DefaultLogger.pypp.cpp \
output/CEGUI/DefaultResourceProvider.pypp.cpp \
output/CEGUI/Dimension.pypp.cpp \
output/CEGUI/DisplayEventArgs.pypp.cpp \
output/CEGUI/DragContainer.pypp.cpp \
output/CEGUI/DragDropEventArgs.pypp.cpp \
output/CEGUI/Editbox.pypp.cpp \
output/CEGUI/EventArgs.pypp.cpp \
output/CEGUI/EventIterator.pypp.cpp \
output/CEGUI/Event.pypp.cpp \
output/CEGUI/EventSet.pypp.cpp \
output/CEGUI/FalagardComponentBase.pypp.cpp \
output/CEGUI/FalagardMappingIterator.pypp.cpp \
output/CEGUI/FalagardXMLHelper.pypp.cpp \
output/CEGUI/FontDim.pypp.cpp \
output/CEGUI/FontGlyph.pypp.cpp \
output/CEGUI/FontIterator.pypp.cpp \
output/CEGUI/FontManager.pypp.cpp \
output/CEGUI/Font.pypp.cpp \
output/CEGUI/FrameComponent.pypp.cpp \
output/CEGUI/FrameWindow.pypp.cpp \
output/CEGUI/GeometryBuffer.pypp.cpp \
output/CEGUI/GlobalEventSet.pypp.cpp \
output/CEGUI/GridLayoutContainer.pypp.cpp \
output/CEGUI/GroupBox.pypp.cpp \
output/CEGUI/GUISheet.pypp.cpp \
output/CEGUI/HeaderSequenceEventArgs.pypp.cpp \
output/CEGUI/HorizontalLayoutContainer.pypp.cpp \
output/CEGUI/ImageCodec.pypp.cpp \
output/CEGUI/ImageDim.pypp.cpp \
output/CEGUI/ImageIterator.pypp.cpp \
output/CEGUI/Image.pypp.cpp \
output/CEGUI/ImageryComponent.pypp.cpp \
output/CEGUI/ImagerySection.pypp.cpp \
output/CEGUI/ImagesetIterator.pypp.cpp \
output/CEGUI/ImagesetManager.pypp.cpp \
output/CEGUI/Imageset.pypp.cpp \
output/CEGUI/Interpolator.pypp.cpp \
output/CEGUI/ItemEntry.pypp.cpp \
output/CEGUI/ItemListBase.pypp.cpp \
output/CEGUI/ItemListbox.pypp.cpp \
output/CEGUI/KeyEventArgs.pypp.cpp \
output/CEGUI/KeyFrame.pypp.cpp \
output/CEGUI/Key.pypp.cpp \
output/CEGUI/LayerSpecification.pypp.cpp \
output/CEGUI/LayoutContainer.pypp.cpp \
output/CEGUI/LBItemList.pypp.cpp \
output/CEGUI/LineList.pypp.cpp \
output/CEGUI/ListboxItem.pypp.cpp \
output/CEGUI/Listbox.pypp.cpp \
output/CEGUI/ListboxTextItem.pypp.cpp \
output/CEGUI/ListHeader.pypp.cpp \
output/CEGUI/ListHeaderSegment.pypp.cpp \
output/CEGUI/Logger.pypp.cpp \
output/CEGUI/MCLGridRef.pypp.cpp \
output/CEGUI/Menubar.pypp.cpp \
output/CEGUI/MenuBase.pypp.cpp \
output/CEGUI/MenuItem.pypp.cpp \
output/CEGUI/MouseCursorEventArgs.pypp.cpp \
output/CEGUI/MouseCursor.pypp.cpp \
output/CEGUI/MouseEventArgs.pypp.cpp \
output/CEGUI/MultiColumnList.pypp.cpp \
output/CEGUI/MultiLineEditbox.pypp.cpp \
output/CEGUI/NamedArea.pypp.cpp \
output/CEGUI/NamedXMLResourceManagerFont.pypp.cpp \
output/CEGUI/NamedXMLResourceManagerImageset.pypp.cpp \
output/CEGUI/NamedXMLResourceManagerScheme.pypp.cpp \
output/CEGUI/PopupMenu.pypp.cpp \
output/CEGUI/ProgressBar.pypp.cpp \
output/CEGUI/PropertyDefinitionBase.pypp.cpp \
output/CEGUI/PropertyDefinitionList.pypp.cpp \
output/CEGUI/PropertyDefinition.pypp.cpp \
output/CEGUI/PropertyDim.pypp.cpp \
output/CEGUI/PropertyHelper.pypp.cpp \
output/CEGUI/PropertyInitialiser.pypp.cpp \
output/CEGUI/PropertyInitialiserVector.pypp.cpp \
output/CEGUI/PropertyIterator.pypp.cpp \
output/CEGUI/PropertyLinkDefinitionList.pypp.cpp \
output/CEGUI/PropertyLinkDefinition.pypp.cpp \
output/CEGUI/Property.pypp.cpp \
output/CEGUI/PropertyReceiver.pypp.cpp \
output/CEGUI/PropertySet.pypp.cpp \
output/CEGUI/PushButton.pypp.cpp \
output/CEGUI/RadioButton.pypp.cpp \
output/CEGUI/RawDataContainer.pypp.cpp \
output/CEGUI/Rect.pypp.cpp \
output/CEGUI/RenderedStringComponent.pypp.cpp \
output/CEGUI/RenderedStringImageComponent.pypp.cpp \
output/CEGUI/RenderedStringParser.pypp.cpp \
output/CEGUI/RenderedString.pypp.cpp \
output/CEGUI/RenderedStringTextComponent.pypp.cpp \
output/CEGUI/RenderedStringWidgetComponent.pypp.cpp \
output/CEGUI/RenderEffectFactory.pypp.cpp \
output/CEGUI/RenderEffectManager.pypp.cpp \
output/CEGUI/RenderEffect.pypp.cpp \
output/CEGUI/Renderer.pypp.cpp \
output/CEGUI/RenderingContext.pypp.cpp \
output/CEGUI/RenderingRoot.pypp.cpp \
output/CEGUI/RenderingSurface.pypp.cpp \
output/CEGUI/RenderingWindow.pypp.cpp \
output/CEGUI/RenderQueueEventArgs.pypp.cpp \
output/CEGUI/RenderQueue.pypp.cpp \
output/CEGUI/RenderTarget.pypp.cpp \
output/CEGUI/ResourceEventArgs.pypp.cpp \
output/CEGUI/ResourceEventSet.pypp.cpp \
output/CEGUI/ResourceProvider.pypp.cpp \
output/CEGUI/SchemeIterator.pypp.cpp \
output/CEGUI/SchemeManager.pypp.cpp \
output/CEGUI/Scheme.pypp.cpp \
output/CEGUI/ScriptModule.pypp.cpp \
output/CEGUI/ScrollablePane.pypp.cpp \
output/CEGUI/Scrollbar.pypp.cpp \
output/CEGUI/ScrolledContainer.pypp.cpp \
output/CEGUI/ScrolledItemListBase.pypp.cpp \
output/CEGUI/SectionSpecification.pypp.cpp \
output/CEGUI/SequentialLayoutContainer.pypp.cpp \
output/CEGUI/SingletonAnimationManager.pypp.cpp \
output/CEGUI/SingletonFontManager.pypp.cpp \
output/CEGUI/SingletonGlobalEventSet.pypp.cpp \
output/CEGUI/SingletonImagesetManager.pypp.cpp \
output/CEGUI/SingletonLogger.pypp.cpp \
output/CEGUI/SingletonMouseCursor.pypp.cpp \
output/CEGUI/SingletonRenderEffectManager.pypp.cpp \
output/CEGUI/SingletonSchemeManager.pypp.cpp \
output/CEGUI/SingletonSystem.pypp.cpp \
output/CEGUI/SingletonWidgetLookManager.pypp.cpp \
output/CEGUI/SingletonWindowFactoryManager.pypp.cpp \
output/CEGUI/SingletonWindowManager.pypp.cpp \
output/CEGUI/SingletonWindowRendererManager.pypp.cpp \
output/CEGUI/Size.pypp.cpp \
output/CEGUI/Slider.pypp.cpp \
output/CEGUI/Spinner.pypp.cpp \
output/CEGUI/StateImagery.pypp.cpp \
output/CEGUI/StdPairFloatFloat.pypp.cpp \
output/CEGUI/StrIndexList.pypp.cpp \
output/CEGUI/Subscriber.pypp.cpp \
output/CEGUI/System.pypp.cpp \
output/CEGUI/TabButton.pypp.cpp \
output/CEGUI/TabControl.pypp.cpp \
output/CEGUI/TargetTypeStack.pypp.cpp \
output/CEGUI/TextComponent.pypp.cpp \
output/CEGUI/Texture.pypp.cpp \
output/CEGUI/TextureTarget.pypp.cpp \
output/CEGUI/TextUtils.pypp.cpp \
output/CEGUI/Thumb.pypp.cpp \
output/CEGUI/Titlebar.pypp.cpp \
output/CEGUI/Tooltip.pypp.cpp \
output/CEGUI/TreeEventArgs.pypp.cpp \
output/CEGUI/TreeItem.pypp.cpp \
output/CEGUI/Tree.pypp.cpp \
output/CEGUI/TypeAliasIterator.pypp.cpp \
output/CEGUI/UBox.pypp.cpp \
output/CEGUI/UDim.pypp.cpp \
output/CEGUI/UnifiedDim.pypp.cpp \
output/CEGUI/UpdateEventArgs.pypp.cpp \
output/CEGUI/URect.pypp.cpp \
output/CEGUI/UVector2.pypp.cpp \
output/CEGUI/Vector2.pypp.cpp \
output/CEGUI/Vector3.pypp.cpp \
output/CEGUI/Vertex.pypp.cpp \
output/CEGUI/VerticalLayoutContainer.pypp.cpp \
output/CEGUI/WidgetComponent.pypp.cpp \
output/CEGUI/WidgetDim.pypp.cpp \
output/CEGUI/WidgetLookFeel.pypp.cpp \
output/CEGUI/WidgetLookManager.pypp.cpp \
output/CEGUI/WindowEventArgs.pypp.cpp \
output/CEGUI/WindowFactoryIterator.pypp.cpp \
output/CEGUI/WindowFactoryManager.pypp.cpp \
output/CEGUI/WindowFactory.pypp.cpp \
output/CEGUI/WindowIterator.pypp.cpp \
output/CEGUI/WindowManager.pypp.cpp \
output/CEGUI/Window.pypp.cpp \
output/CEGUI/WindowRendererFactory.pypp.cpp \
output/CEGUI/WindowRendererManager.pypp.cpp \
output/CEGUI/WindowRenderer.pypp.cpp \
output/CEGUI/XMLAttributes.pypp.cpp \
output/CEGUI/XMLHandler.pypp.cpp \
output/CEGUI/XMLParser.pypp.cpp \
output/CEGUI/XMLSerializer.pypp.cpp

PyCEGUI_la_LIBADD = $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la \
                    -l$(PYTHON_LIB) -l$(BOOST_PYTHON_LIB)

if BUILD_OPENGL_RENDERER
pyexec_LTLIBRARIES += PyCEGUIOpenGLRenderer.la

# NB: No '@CEGUI_LIB_LINK_FLAGS@' here
PyCEGUIOpenGLRenderer_la_LDFLAGS = -module -avoid-version
PyCEGUIOpenGLRenderer_la_CXXFLAGS = -fvisibility=hidden
PyCEGUIOpenGLRenderer_la_CPPFLAGS = 

if CEGUI_BUILD_STATIC
PyCEGUIOpenGLRenderer_la_CPPFLAGS += -DCEGUI_STATIC
endif

PyCEGUIOpenGLRenderer_la_SOURCES = \
output/CEGUIOpenGLRenderer/PyCEGUIOpenGLRenderer.main.cpp \
output/CEGUIOpenGLRenderer/OpenGLRenderer.pypp.cpp

PyCEGUIOpenGLRenderer_la_LIBADD = $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la \
                                  $(top_builddir)/cegui/src/RendererModules/OpenGL/libCEGUIOpenGLRenderer@cegui_bsfx@.la \
                                  -l$(PYTHON_LIB) -l$(BOOST_PYTHON_LIB)
endif

if BUILD_OGRE_RENDERER
pyexec_LTLIBRARIES += PyCEGUIOgreRenderer.la

# NB: No '@CEGUI_LIB_LINK_FLAGS@' here
PyCEGUIOgreRenderer_la_LDFLAGS = -module -avoid-version
PyCEGUIOgreRenderer_la_CXXFLAGS = -fvisibility=hidden
PyCEGUIOgreRenderer_la_CPPFLAGS = 

if CEGUI_BUILD_STATIC
PyCEGUIOgreRenderer_la_CPPFLAGS += -DCEGUI_STATIC
endif

PyCEGUIOgreRenderer_la_SOURCES = \
output/CEGUIOgreRenderer/OgreImageCodec.pypp.cpp \
output/CEGUIOgreRenderer/OgreRenderer.pypp.cpp \
output/CEGUIOgreRenderer/OgreResourceProvider.pypp.cpp \
output/CEGUIOgreRenderer/PyCEGUIOgreRenderer.main.cpp

PyCEGUIOgreRenderer_la_LIBADD = $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la \
                                $(top_builddir)/cegui/src/RendererModules/Ogre/libCEGUIOgreRenderer@cegui_bsfx@.la \
                                -l$(PYTHON_LIB) -l$(BOOST_PYTHON_LIB)
endif

if BUILD_NULL_RENDERER
pyexec_LTLIBRARIES += PyCEGUINullRenderer.la

# NB: No '@CEGUI_LIB_LINK_FLAGS@' here
PyCEGUINullRenderer_la_LDFLAGS = -module -avoid-version
PyCEGUINullRenderer_la_CXXFLAGS = -fvisibility=hidden
PyCEGUINullRenderer_la_CPPFLAGS = 

if CEGUI_BUILD_STATIC
PyCEGUINullRenderer_la_CPPFLAGS += -DCEGUI_STATIC
endif

PyCEGUINullRenderer_la_SOURCES = \
output/CEGUINullRenderer/PyCEGUINullRenderer.main.cpp \
output/CEGUINullRenderer/NullRenderer.pypp.cpp

PyCEGUINullRenderer_la_LIBADD = $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la \
                                $(top_builddir)/cegui/src/RendererModules/Null/libCEGUINullRenderer@cegui_bsfx@.la \
                                -l$(PYTHON_LIB) -l$(BOOST_PYTHON_LIB)
endif

noinst_HEADERS = python_CEGUI.h python_CEGUIOpenGLRenderer.h python_CEGUINullRenderer.h python_CEGUIOgreRenderer.h \
output/CEGUI/AbsoluteDim.pypp.hpp \
output/CEGUI/ActivationEventArgs.pypp.hpp \
output/CEGUI/Affector.pypp.hpp \
output/CEGUI/AnimationEventArgs.pypp.hpp \
output/CEGUI/AnimationInstance.pypp.hpp \
output/CEGUI/AnimationManager.pypp.hpp \
output/CEGUI/Animation.pypp.hpp \
output/CEGUI/BaseDim.pypp.hpp \
output/CEGUI/BasicRenderedStringParser.pypp.hpp \
output/CEGUI/BiDiVisualMapping.pypp.hpp \
output/CEGUI/BoundSlot.pypp.hpp \
output/CEGUI/ButtonBase.pypp.hpp \
output/CEGUI/PyCEGUI_enumerations.pypp.hpp \
output/CEGUI/CentredRenderedString.pypp.hpp \
output/CEGUI/Checkbox.pypp.hpp \
output/CEGUI/colour.pypp.hpp \
output/CEGUI/ColourRect.pypp.hpp \
output/CEGUI/Combobox.pypp.hpp \
output/CEGUI/ComboDropList.pypp.hpp \
output/CEGUI/ComponentArea.pypp.hpp \
output/CEGUI/Connection.pypp.hpp \
output/CEGUI/CoordConverter.pypp.hpp \
output/CEGUI/DefaultLogger.pypp.hpp \
output/CEGUI/DefaultResourceProvider.pypp.hpp \
output/CEGUI/Dimension.pypp.hpp \
output/CEGUI/DisplayEventArgs.pypp.hpp \
output/CEGUI/DragContainer.pypp.hpp \
output/CEGUI/DragDropEventArgs.pypp.hpp \
output/CEGUI/Editbox.pypp.hpp \
output/CEGUI/EventArgs.pypp.hpp \
output/CEGUI/EventIterator.pypp.hpp \
output/CEGUI/Event.pypp.hpp \
output/CEGUI/EventSet.pypp.hpp \
output/CEGUI/FalagardComponentBase.pypp.hpp \
output/CEGUI/FalagardMappingIterator.pypp.hpp \
output/CEGUI/FalagardXMLHelper.pypp.hpp \
output/CEGUI/FontDim.pypp.hpp \
output/CEGUI/FontGlyph.pypp.hpp \
output/CEGUI/FontIterator.pypp.hpp \
output/CEGUI/FontManager.pypp.hpp \
output/CEGUI/Font.pypp.hpp \
output/CEGUI/FrameComponent.pypp.hpp \
output/CEGUI/FrameWindow.pypp.hpp \
output/CEGUI/GeometryBuffer.pypp.hpp \
output/CEGUI/GlobalEventSet.pypp.hpp \
output/CEGUI/GridLayoutContainer.pypp.hpp \
output/CEGUI/GroupBox.pypp.hpp \
output/CEGUI/GUISheet.pypp.hpp \
output/CEGUI/HeaderSequenceEventArgs.pypp.hpp \
output/CEGUI/HorizontalLayoutContainer.pypp.hpp \
output/CEGUI/ImageCodec.pypp.hpp \
output/CEGUI/ImageDim.pypp.hpp \
output/CEGUI/ImageIterator.pypp.hpp \
output/CEGUI/Image.pypp.hpp \
output/CEGUI/ImageryComponent.pypp.hpp \
output/CEGUI/ImagerySection.pypp.hpp \
output/CEGUI/ImagesetIterator.pypp.hpp \
output/CEGUI/ImagesetManager.pypp.hpp \
output/CEGUI/Imageset.pypp.hpp \
output/CEGUI/Interpolator.pypp.hpp \
output/CEGUI/ItemEntry.pypp.hpp \
output/CEGUI/ItemListBase.pypp.hpp \
output/CEGUI/ItemListbox.pypp.hpp \
output/CEGUI/KeyEventArgs.pypp.hpp \
output/CEGUI/KeyFrame.pypp.hpp \
output/CEGUI/Key.pypp.hpp \
output/CEGUI/LayerSpecification.pypp.hpp \
output/CEGUI/LayoutContainer.pypp.hpp \
output/CEGUI/LBItemList.pypp.hpp \
output/CEGUI/_LineInfo__value_traits.pypp.hpp \
output/CEGUI/LineList.pypp.hpp \
output/CEGUI/ListboxItem.pypp.hpp \
output/CEGUI/Listbox.pypp.hpp \
output/CEGUI/ListboxTextItem.pypp.hpp \
output/CEGUI/ListHeader.pypp.hpp \
output/CEGUI/ListHeaderSegment.pypp.hpp \
output/CEGUI/Logger.pypp.hpp \
output/CEGUI/MCLGridRef.pypp.hpp \
output/CEGUI/Menubar.pypp.hpp \
output/CEGUI/MenuBase.pypp.hpp \
output/CEGUI/MenuItem.pypp.hpp \
output/CEGUI/MouseCursorEventArgs.pypp.hpp \
output/CEGUI/MouseCursor.pypp.hpp \
output/CEGUI/MouseEventArgs.pypp.hpp \
output/CEGUI/MultiColumnList.pypp.hpp \
output/CEGUI/MultiLineEditbox.pypp.hpp \
output/CEGUI/NamedArea.pypp.hpp \
output/CEGUI/NamedXMLResourceManagerFont.pypp.hpp \
output/CEGUI/NamedXMLResourceManagerImageset.pypp.hpp \
output/CEGUI/NamedXMLResourceManagerScheme.pypp.hpp \
output/CEGUI/PopupMenu.pypp.hpp \
output/CEGUI/ProgressBar.pypp.hpp \
output/CEGUI/PropertyDefinitionBase.pypp.hpp \
output/CEGUI/PropertyDefinitionList.pypp.hpp \
output/CEGUI/PropertyDefinition.pypp.hpp \
output/CEGUI/_PropertyDefinition__value_traits.pypp.hpp \
output/CEGUI/PropertyDim.pypp.hpp \
output/CEGUI/PropertyHelper.pypp.hpp \
output/CEGUI/PropertyInitialiser.pypp.hpp \
output/CEGUI/_PropertyInitialiser__value_traits.pypp.hpp \
output/CEGUI/PropertyInitialiserVector.pypp.hpp \
output/CEGUI/PropertyIterator.pypp.hpp \
output/CEGUI/PropertyLinkDefinitionList.pypp.hpp \
output/CEGUI/PropertyLinkDefinition.pypp.hpp \
output/CEGUI/_PropertyLinkDefinition__value_traits.pypp.hpp \
output/CEGUI/Property.pypp.hpp \
output/CEGUI/PropertyReceiver.pypp.hpp \
output/CEGUI/PropertySet.pypp.hpp \
output/CEGUI/PushButton.pypp.hpp \
output/CEGUI/RadioButton.pypp.hpp \
output/CEGUI/RawDataContainer.pypp.hpp \
output/CEGUI/Rect.pypp.hpp \
output/CEGUI/RenderedStringComponent.pypp.hpp \
output/CEGUI/RenderedStringImageComponent.pypp.hpp \
output/CEGUI/RenderedStringParser.pypp.hpp \
output/CEGUI/RenderedString.pypp.hpp \
output/CEGUI/RenderedStringTextComponent.pypp.hpp \
output/CEGUI/RenderedStringWidgetComponent.pypp.hpp \
output/CEGUI/RenderEffectFactory.pypp.hpp \
output/CEGUI/RenderEffectManager.pypp.hpp \
output/CEGUI/RenderEffect.pypp.hpp \
output/CEGUI/Renderer.pypp.hpp \
output/CEGUI/RenderingContext.pypp.hpp \
output/CEGUI/RenderingRoot.pypp.hpp \
output/CEGUI/RenderingSurface.pypp.hpp \
output/CEGUI/RenderingWindow.pypp.hpp \
output/CEGUI/RenderQueueEventArgs.pypp.hpp \
output/CEGUI/RenderQueue.pypp.hpp \
output/CEGUI/RenderTarget.pypp.hpp \
output/CEGUI/ResourceEventArgs.pypp.hpp \
output/CEGUI/ResourceEventSet.pypp.hpp \
output/CEGUI/ResourceProvider.pypp.hpp \
output/CEGUI/SchemeIterator.pypp.hpp \
output/CEGUI/SchemeManager.pypp.hpp \
output/CEGUI/Scheme.pypp.hpp \
output/CEGUI/ScriptModule.pypp.hpp \
output/CEGUI/ScrollablePane.pypp.hpp \
output/CEGUI/Scrollbar.pypp.hpp \
output/CEGUI/ScrolledContainer.pypp.hpp \
output/CEGUI/ScrolledItemListBase.pypp.hpp \
output/CEGUI/SectionSpecification.pypp.hpp \
output/CEGUI/SequentialLayoutContainer.pypp.hpp \
output/CEGUI/SingletonAnimationManager.pypp.hpp \
output/CEGUI/SingletonFontManager.pypp.hpp \
output/CEGUI/SingletonGlobalEventSet.pypp.hpp \
output/CEGUI/SingletonImagesetManager.pypp.hpp \
output/CEGUI/SingletonLogger.pypp.hpp \
output/CEGUI/SingletonMouseCursor.pypp.hpp \
output/CEGUI/SingletonRenderEffectManager.pypp.hpp \
output/CEGUI/SingletonSchemeManager.pypp.hpp \
output/CEGUI/SingletonSystem.pypp.hpp \
output/CEGUI/SingletonWidgetLookManager.pypp.hpp \
output/CEGUI/SingletonWindowFactoryManager.pypp.hpp \
output/CEGUI/SingletonWindowManager.pypp.hpp \
output/CEGUI/SingletonWindowRendererManager.pypp.hpp \
output/CEGUI/Size.pypp.hpp \
output/CEGUI/Slider.pypp.hpp \
output/CEGUI/Spinner.pypp.hpp \
output/CEGUI/StateImagery.pypp.hpp \
output/CEGUI/StdPairFloatFloat.pypp.hpp \
output/CEGUI/StrIndexList.pypp.hpp \
output/CEGUI/Subscriber.pypp.hpp \
output/CEGUI/System.pypp.hpp \
output/CEGUI/TabButton.pypp.hpp \
output/CEGUI/TabControl.pypp.hpp \
output/CEGUI/TargetTypeStack.pypp.hpp \
output/CEGUI/TextComponent.pypp.hpp \
output/CEGUI/Texture.pypp.hpp \
output/CEGUI/TextureTarget.pypp.hpp \
output/CEGUI/TextUtils.pypp.hpp \
output/CEGUI/Thumb.pypp.hpp \
output/CEGUI/Titlebar.pypp.hpp \
output/CEGUI/Tooltip.pypp.hpp \
output/CEGUI/TreeEventArgs.pypp.hpp \
output/CEGUI/TreeItem.pypp.hpp \
output/CEGUI/Tree.pypp.hpp \
output/CEGUI/TypeAliasIterator.pypp.hpp \
output/CEGUI/UBox.pypp.hpp \
output/CEGUI/UDim.pypp.hpp \
output/CEGUI/UnifiedDim.pypp.hpp \
output/CEGUI/UpdateEventArgs.pypp.hpp \
output/CEGUI/URect.pypp.hpp \
output/CEGUI/UVector2.pypp.hpp \
output/CEGUI/Vector2.pypp.hpp \
output/CEGUI/Vector3.pypp.hpp \
output/CEGUI/Vertex.pypp.hpp \
output/CEGUI/VerticalLayoutContainer.pypp.hpp \
output/CEGUI/WidgetComponent.pypp.hpp \
output/CEGUI/WidgetDim.pypp.hpp \
output/CEGUI/WidgetLookFeel.pypp.hpp \
output/CEGUI/WidgetLookManager.pypp.hpp \
output/CEGUI/WindowEventArgs.pypp.hpp \
output/CEGUI/WindowFactoryIterator.pypp.hpp \
output/CEGUI/WindowFactoryManager.pypp.hpp \
output/CEGUI/WindowFactory.pypp.hpp \
output/CEGUI/WindowIterator.pypp.hpp \
output/CEGUI/WindowManager.pypp.hpp \
output/CEGUI/Window.pypp.hpp \
output/CEGUI/WindowRendererFactory.pypp.hpp \
output/CEGUI/WindowRendererManager.pypp.hpp \
output/CEGUI/WindowRenderer.pypp.hpp \
output/CEGUI/XMLAttributes.pypp.hpp \
output/CEGUI/XMLHandler.pypp.hpp \
output/CEGUI/XMLParser.pypp.hpp \
output/CEGUI/XMLSerializer.pypp.hpp \
output/CEGUI/indexing_suite/algorithms.hpp \
output/CEGUI/indexing_suite/container_proxy.hpp \
output/CEGUI/indexing_suite/container_suite.hpp \
output/CEGUI/indexing_suite/container_traits.hpp \
output/CEGUI/indexing_suite/deque.hpp \
output/CEGUI/indexing_suite/element_proxy.hpp \
output/CEGUI/indexing_suite/element_proxy_traits.hpp \
output/CEGUI/indexing_suite/int_slice_helper.hpp \
output/CEGUI/indexing_suite/iterator_range.hpp \
output/CEGUI/indexing_suite/iterator_traits.hpp \
output/CEGUI/indexing_suite/list.hpp \
output/CEGUI/indexing_suite/map.hpp \
output/CEGUI/indexing_suite/methods.hpp \
output/CEGUI/indexing_suite/multimap.hpp \
output/CEGUI/indexing_suite/pair.hpp \
output/CEGUI/indexing_suite/proxy_iterator.hpp \
output/CEGUI/indexing_suite/python_iterator.hpp \
output/CEGUI/indexing_suite/registry_utils.hpp \
output/CEGUI/indexing_suite/set.hpp \
output/CEGUI/indexing_suite/shared_proxy_impl.hpp \
output/CEGUI/indexing_suite/slice_handler.hpp \
output/CEGUI/indexing_suite/slice.hpp \
output/CEGUI/indexing_suite/suite_utils.hpp \
output/CEGUI/indexing_suite/value_traits.hpp \
output/CEGUI/indexing_suite/vector.hpp \
output/CEGUI/indexing_suite/visitor.hpp \
output/CEGUI/indexing_suite/workaround.hpp \
output/CEGUINullRenderer/NullRenderer.pypp.hpp \
output/CEGUIOgreRenderer/OgreImageCodec.pypp.hpp \
output/CEGUIOgreRenderer/OgreRenderer.pypp.hpp \
output/CEGUIOgreRenderer/OgreResourceProvider.pypp.hpp \
output/CEGUIOpenGLRenderer/OpenGLRenderer.pypp.hpp

EXTRA_DIST = commonUtils.py \
             extract_documentation.py \
             generateCEGUIOpenGLRenderer.py \
             generateCEGUI.py \
             generate.py \
			 output/CEGUI/exposed_decl.pypp.txt \
			 output/CEGUI/named_tuple.py \
			 output/CEGUINullRenderer/exposed_decl.pypp.txt \
			 output/CEGUINullRenderer/named_tuple.py \
			 output/CEGUIOgreRenderer/exposed_decl.pypp.txt \
			 output/CEGUIOgreRenderer/named_tuple.py \
			 output/CEGUIOpenGLRenderer/exposed_decl.pypp.txt \
			 output/CEGUIOpenGLRenderer/named_tuple.py \
             distutils/README \
             distutils/setup.py \
             distutils/WINDOWS_ONLY \
             distutils/PyCEGUI/fake.py \
             distutils/PyCEGUI/__init__.py

