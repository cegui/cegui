/***********************************************************************
    filename:   CEGUIFalagard_xmlHandler.cpp
    created:    Fri Jun 17 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#include "falagard/CEGUIFalagard_xmlHandler.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "falagard/CEGUIFalWidgetComponent.h"
#include "falagard/CEGUIFalTextComponent.h"
#include "falagard/CEGUIFalFrameComponent.h"
#include "falagard/CEGUIFalNamedArea.h"
#include "falagard/CEGUIFalPropertyDefinition.h"
#include "falagard/CEGUIFalPropertyLinkDefinition.h"
#include "falagard/CEGUIFalXMLEnumHelper.h"
#include "CEGUIXMLAttributes.h"
#include "CEGUILogger.h"
#include <sstream>

// Start of CEGUI namespace section
namespace CEGUI
{
    ////////////////////////////////////////////////////////////////////////////////
    // element names
    const String Falagard_xmlHandler::FalagardElement("Falagard");
    const String Falagard_xmlHandler::WidgetLookElement("WidgetLook");
    const String Falagard_xmlHandler::ChildElement("Child");
    const String Falagard_xmlHandler::ImagerySectionElement("ImagerySection");
    const String Falagard_xmlHandler::StateImageryElement("StateImagery");
    const String Falagard_xmlHandler::LayerElement("Layer");
    const String Falagard_xmlHandler::SectionElement("Section");
    const String Falagard_xmlHandler::ImageryComponentElement("ImageryComponent");
    const String Falagard_xmlHandler::TextComponentElement("TextComponent");
    const String Falagard_xmlHandler::FrameComponentElement("FrameComponent");
    const String Falagard_xmlHandler::AreaElement("Area");
    const String Falagard_xmlHandler::ImageElement("Image");
    const String Falagard_xmlHandler::ColoursElement("Colours");
    const String Falagard_xmlHandler::VertFormatElement("VertFormat");
    const String Falagard_xmlHandler::HorzFormatElement("HorzFormat");
    const String Falagard_xmlHandler::VertAlignmentElement("VertAlignment");
    const String Falagard_xmlHandler::HorzAlignmentElement("HorzAlignment");
    const String Falagard_xmlHandler::PropertyElement("Property");
    const String Falagard_xmlHandler::DimElement("Dim");
    const String Falagard_xmlHandler::UnifiedDimElement("UnifiedDim");
    const String Falagard_xmlHandler::AbsoluteDimElement("AbsoluteDim");
    const String Falagard_xmlHandler::ImageDimElement("ImageDim");
    const String Falagard_xmlHandler::WidgetDimElement("WidgetDim");
    const String Falagard_xmlHandler::FontDimElement("FontDim");
    const String Falagard_xmlHandler::PropertyDimElement("PropertyDim");
    const String Falagard_xmlHandler::TextElement("Text");
    const String Falagard_xmlHandler::ColourPropertyElement("ColourProperty");
    const String Falagard_xmlHandler::ColourRectPropertyElement("ColourRectProperty");
    const String Falagard_xmlHandler::NamedAreaElement("NamedArea");
    const String Falagard_xmlHandler::PropertyDefinitionElement("PropertyDefinition");
    const String Falagard_xmlHandler::PropertyLinkDefinitionElement("PropertyLinkDefinition");
    const String Falagard_xmlHandler::DimOperatorElement("DimOperator");
    const String Falagard_xmlHandler::VertFormatPropertyElement("VertFormatProperty");
    const String Falagard_xmlHandler::HorzFormatPropertyElement("HorzFormatProperty");
    const String Falagard_xmlHandler::AreaPropertyElement("AreaProperty");
    const String Falagard_xmlHandler::ImagePropertyElement("ImageProperty");
    const String Falagard_xmlHandler::TextPropertyElement("TextProperty");
    const String Falagard_xmlHandler::FontPropertyElement("FontProperty");
    const String Falagard_xmlHandler::ColourElement("Colour");
    // attribute names
    const String Falagard_xmlHandler::TopLeftAttribute("topLeft");
    const String Falagard_xmlHandler::TopRightAttribute("topRight");
    const String Falagard_xmlHandler::BottomLeftAttribute("bottomLeft");
    const String Falagard_xmlHandler::BottomRightAttribute("bottomRight");
    const String Falagard_xmlHandler::ImagesetAttribute("imageset");
    const String Falagard_xmlHandler::ImageAttribute("image");
    const String Falagard_xmlHandler::TypeAttribute("type");
    const String Falagard_xmlHandler::NameAttribute("name");
    const String Falagard_xmlHandler::PriorityAttribute("priority");
    const String Falagard_xmlHandler::SectionNameAttribute("section");
    const String Falagard_xmlHandler::NameSuffixAttribute("nameSuffix");
    const String Falagard_xmlHandler::RendererAttribute("renderer");
    const String Falagard_xmlHandler::LookAttribute("look");
    const String Falagard_xmlHandler::ScaleAttribute("scale");
    const String Falagard_xmlHandler::OffsetAttribute("offset");
    const String Falagard_xmlHandler::ValueAttribute("value");
    const String Falagard_xmlHandler::DimensionAttribute("dimension");
    const String Falagard_xmlHandler::WidgetAttribute("widget");
    const String Falagard_xmlHandler::StringAttribute("string");
    const String Falagard_xmlHandler::FontAttribute("font");
    const String Falagard_xmlHandler::InitialValueAttribute("initialValue");
    const String Falagard_xmlHandler::ClippedAttribute("clipped");
    const String Falagard_xmlHandler::OperatorAttribute("op");
    const String Falagard_xmlHandler::PaddingAttribute("padding");
    const String Falagard_xmlHandler::LayoutOnWriteAttribute("layoutOnWrite");
    const String Falagard_xmlHandler::RedrawOnWriteAttribute("redrawOnWrite");
    const String Falagard_xmlHandler::TargetPropertyAttribute("targetProperty");
    const String Falagard_xmlHandler::ControlPropertyAttribute("controlProperty");
    const String Falagard_xmlHandler::ColourAttribute("colour");

    ////////////////////////////////////////////////////////////////////////////////


    Falagard_xmlHandler::Falagard_xmlHandler(WidgetLookManager* mgr) :
        d_manager(mgr),
        d_widgetlook(0),
        d_childcomponent(0),
        d_imagerysection(0),
        d_stateimagery(0),
        d_layer(0),
        d_section(0),
        d_imagerycomponent(0),
        d_area(0),
        d_textcomponent(0),
        d_namedArea(0),
        d_framecomponent(0)
    {
        // register element start handlers
        registerElementStartHandler(FalagardElement, &Falagard_xmlHandler::elementFalagardStart);
        registerElementStartHandler(WidgetLookElement, &Falagard_xmlHandler::elementWidgetLookStart);
        registerElementStartHandler(ChildElement, &Falagard_xmlHandler::elementChildStart);
        registerElementStartHandler(ImagerySectionElement, &Falagard_xmlHandler::elementImagerySectionStart);
        registerElementStartHandler(StateImageryElement, &Falagard_xmlHandler::elementStateImageryStart);
        registerElementStartHandler(LayerElement, &Falagard_xmlHandler::elementLayerStart);
        registerElementStartHandler(SectionElement, &Falagard_xmlHandler::elementSectionStart);
        registerElementStartHandler(ImageryComponentElement, &Falagard_xmlHandler::elementImageryComponentStart);
        registerElementStartHandler(TextComponentElement, &Falagard_xmlHandler::elementTextComponentStart);
        registerElementStartHandler(FrameComponentElement, &Falagard_xmlHandler::elementFrameComponentStart);
        registerElementStartHandler(AreaElement, &Falagard_xmlHandler::elementAreaStart);
        registerElementStartHandler(ImageElement, &Falagard_xmlHandler::elementImageStart);
        registerElementStartHandler(ColoursElement, &Falagard_xmlHandler::elementColoursStart);
        registerElementStartHandler(VertFormatElement, &Falagard_xmlHandler::elementVertFormatStart);
        registerElementStartHandler(HorzFormatElement, &Falagard_xmlHandler::elementHorzFormatStart);
        registerElementStartHandler(VertAlignmentElement, &Falagard_xmlHandler::elementVertAlignmentStart);
        registerElementStartHandler(HorzAlignmentElement, &Falagard_xmlHandler::elementHorzAlignmentStart);
        registerElementStartHandler(PropertyElement, &Falagard_xmlHandler::elementPropertyStart);
        registerElementStartHandler(DimElement, &Falagard_xmlHandler::elementDimStart);
        registerElementStartHandler(UnifiedDimElement, &Falagard_xmlHandler::elementUnifiedDimStart);
        registerElementStartHandler(AbsoluteDimElement, &Falagard_xmlHandler::elementAbsoluteDimStart);
        registerElementStartHandler(ImageDimElement, &Falagard_xmlHandler::elementImageDimStart);
        registerElementStartHandler(WidgetDimElement, &Falagard_xmlHandler::elementWidgetDimStart);
        registerElementStartHandler(FontDimElement, &Falagard_xmlHandler::elementFontDimStart);
        registerElementStartHandler(PropertyDimElement, &Falagard_xmlHandler::elementPropertyDimStart);
        registerElementStartHandler(TextElement, &Falagard_xmlHandler::elementTextStart);
        registerElementStartHandler(ColourPropertyElement, &Falagard_xmlHandler::elementColourPropertyStart);
        registerElementStartHandler(ColourRectPropertyElement, &Falagard_xmlHandler::elementColourRectPropertyStart);
        registerElementStartHandler(NamedAreaElement, &Falagard_xmlHandler::elementNamedAreaStart);
        registerElementStartHandler(PropertyDefinitionElement, &Falagard_xmlHandler::elementPropertyDefinitionStart);
        registerElementStartHandler(PropertyLinkDefinitionElement, &Falagard_xmlHandler::elementPropertyLinkDefinitionStart);
        registerElementStartHandler(DimOperatorElement, &Falagard_xmlHandler::elementDimOperatorStart);
        registerElementStartHandler(VertFormatPropertyElement, &Falagard_xmlHandler::elementVertFormatPropertyStart);
        registerElementStartHandler(HorzFormatPropertyElement, &Falagard_xmlHandler::elementHorzFormatPropertyStart);
        registerElementStartHandler(AreaPropertyElement, &Falagard_xmlHandler::elementAreaPropertyStart);
        registerElementStartHandler(ImagePropertyElement, &Falagard_xmlHandler::elementImagePropertyStart);
        registerElementStartHandler(TextPropertyElement, &Falagard_xmlHandler::elementTextPropertyStart);
        registerElementStartHandler(FontPropertyElement, &Falagard_xmlHandler::elementFontPropertyStart);
        registerElementStartHandler(ColourElement, &Falagard_xmlHandler::elementColourStart);

        // register element end handlers
        registerElementEndHandler(FalagardElement, &Falagard_xmlHandler::elementFalagardEnd);
        registerElementEndHandler(WidgetLookElement, &Falagard_xmlHandler::elementWidgetLookEnd);
        registerElementEndHandler(ChildElement, &Falagard_xmlHandler::elementChildEnd);
        registerElementEndHandler(ImagerySectionElement, &Falagard_xmlHandler::elementImagerySectionEnd);
        registerElementEndHandler(StateImageryElement, &Falagard_xmlHandler::elementStateImageryEnd);
        registerElementEndHandler(LayerElement, &Falagard_xmlHandler::elementLayerEnd);
        registerElementEndHandler(SectionElement, &Falagard_xmlHandler::elementSectionEnd);
        registerElementEndHandler(ImageryComponentElement, &Falagard_xmlHandler::elementImageryComponentEnd);
        registerElementEndHandler(TextComponentElement, &Falagard_xmlHandler::elementTextComponentEnd);
        registerElementEndHandler(FrameComponentElement, &Falagard_xmlHandler::elementFrameComponentEnd);
        registerElementEndHandler(AreaElement, &Falagard_xmlHandler::elementAreaEnd);
        registerElementEndHandler(UnifiedDimElement, &Falagard_xmlHandler::elementAnyDimEnd);
        registerElementEndHandler(AbsoluteDimElement, &Falagard_xmlHandler::elementAnyDimEnd);
        registerElementEndHandler(ImageDimElement, &Falagard_xmlHandler::elementAnyDimEnd);
        registerElementEndHandler(WidgetDimElement, &Falagard_xmlHandler::elementAnyDimEnd);
        registerElementEndHandler(FontDimElement, &Falagard_xmlHandler::elementAnyDimEnd);
        registerElementEndHandler(PropertyDimElement, &Falagard_xmlHandler::elementAnyDimEnd);
        registerElementEndHandler(NamedAreaElement, &Falagard_xmlHandler::elementNamedAreaEnd);
    }

    Falagard_xmlHandler::~Falagard_xmlHandler()
    {}

    /*************************************************************************
        Handle an opening XML element tag.
    *************************************************************************/
    void Falagard_xmlHandler::elementStart(const String& element, const XMLAttributes& attributes)
    {
        // find registered handler for this element.
        ElementStartHandlerMap::const_iterator iter = d_startHandlersMap.find(element);

        // if a handler existed
        if (iter != d_startHandlersMap.end())
        {
            // call the handler for this element
            (this->*(iter->second))(attributes);
        }
        // no handler existed
        else
        {
            Logger::getSingleton().logEvent("Falagard::xmlHandler::elementStart - The unknown XML element '" + element + "' was encountered while processing the look and feel file.", Errors);
        }
    }

    /*************************************************************************
        Handle a closing XML element tag
    *************************************************************************/
    void Falagard_xmlHandler::elementEnd(const String& element)
    {
        // find registered handler for this element.
        ElementEndHandlerMap::const_iterator iter = d_endHandlersMap.find(element);

        // if a handler existed
        if (iter != d_endHandlersMap.end())
            // call the handler for this element
            (this->*(iter->second))();
    }

    /*************************************************************************
        Convert a hex string "AARRGGBB" to type argb_t
    *************************************************************************/
    argb_t Falagard_xmlHandler::hexStringToARGB(const String& str)
    {
        argb_t val;
        std::istringstream s(str.c_str());
        s >> std::hex >> val;

        return val;
    }

    /*************************************************************************
        Assign a dimension to a ComponentArea depending upon the dimension's
        type.
    *************************************************************************/
    void Falagard_xmlHandler::assignAreaDimension(Dimension& dim)
    {
        if (d_area)
        {
            switch (dim.getDimensionType())
            {
            case DT_LEFT_EDGE:
            case DT_X_POSITION:
                d_area->d_left = dim;
                break;
            case DT_TOP_EDGE:
            case DT_Y_POSITION:
                d_area->d_top = dim;
                break;
            case DT_RIGHT_EDGE:
            case DT_WIDTH:
                d_area->d_right_or_width = dim;
                break;
            case DT_BOTTOM_EDGE:
            case DT_HEIGHT:
                d_area->d_bottom_or_height = dim;
                break;
            default:
                throw InvalidRequestException("Falagard::xmlHandler::assignAreaDimension - Invalid DimensionType specified for area component.");
            }
        }
    }

    /*************************************************************************
        Assign a ColourRect to the current element supporting such a thing
    *************************************************************************/
    void Falagard_xmlHandler::assignColours(const ColourRect& cols)
    {
        // need to decide what to apply colours to
        if (d_framecomponent)
        {
            d_framecomponent->setColours(cols);
        }
        else if (d_imagerycomponent)
        {
            d_imagerycomponent->setColours(cols);
        }
        else if (d_textcomponent)
        {
            d_textcomponent->setColours(cols);
        }
        else if (d_imagerysection)
        {
            d_imagerysection->setMasterColours(cols);
        }
        else if (d_section)
        {
            d_section->setOverrideColours(cols);
            d_section->setUsingOverrideColours(true);
        }
    }

    /*************************************************************************
        Method that performs common handling for all *Dim elements.
    *************************************************************************/
    void Falagard_xmlHandler::doBaseDimStart(const BaseDim* dim)
    {
        BaseDim* cloned = dim->clone();
        d_dimStack.push_back(cloned);
    }

    /*************************************************************************
        Method that handles the opening Falagard XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementFalagardStart(const XMLAttributes& attributes)
    {
        Logger::getSingleton().logEvent("===== Falagard 'root' element: look and feel parsing begins =====");
    }

    /*************************************************************************
        Method that handles the opening WidgetLook XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementWidgetLookStart(const XMLAttributes& attributes)
    {
        assert(d_widgetlook == 0);
        d_widgetlook = new WidgetLookFeel(attributes.getValueAsString(NameAttribute));

        Logger::getSingleton().logEvent("---> Start of definition for widget look '" + d_widgetlook->getName() + "'.", Informative);
    }

    /*************************************************************************
        Method that handles the opening Child XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementChildStart(const XMLAttributes& attributes)
    {
        assert(d_childcomponent == 0);
        d_childcomponent = new WidgetComponent(attributes.getValueAsString(TypeAttribute), attributes.getValueAsString(LookAttribute), attributes.getValueAsString(NameSuffixAttribute), attributes.getValueAsString(RendererAttribute));

        CEGUI_LOGINSANE("-----> Start of definition for child widget. Type: " + d_childcomponent->getBaseWidgetType() + " Suffix: " + d_childcomponent->getWidgetNameSuffix() + " Look: " + d_childcomponent->getWidgetLookName());
    }

    /*************************************************************************
        Method that handles the opening ImagerySection XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementImagerySectionStart(const XMLAttributes& attributes)
    {
        assert(d_imagerysection == 0);
        d_imagerysection = new ImagerySection(attributes.getValueAsString(NameAttribute));

        CEGUI_LOGINSANE("-----> Start of definition for imagery section '" + d_imagerysection->getName() + "'.");
    }

    /*************************************************************************
        Method that handles the opening StateImagery XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementStateImageryStart(const XMLAttributes& attributes)
    {
        assert(d_stateimagery == 0);
        d_stateimagery = new StateImagery(attributes.getValueAsString(NameAttribute));
        d_stateimagery->setClippedToDisplay(!attributes.getValueAsBool(ClippedAttribute, true));

        CEGUI_LOGINSANE("-----> Start of definition for imagery for state '" + d_stateimagery->getName() + "'.");
    }

    /*************************************************************************
        Method that handles the opening Layer XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementLayerStart(const XMLAttributes& attributes)
    {
        assert(d_layer == 0);
        d_layer = new LayerSpecification(attributes.getValueAsInteger(PriorityAttribute, 0));

        CEGUI_LOGINSANE("-------> Start of definition of new imagery layer, priority: " + attributes.getValueAsString(PriorityAttribute, "0"));
    }

    /*************************************************************************
        Method that handles the opening Section XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementSectionStart(const XMLAttributes& attributes)
    {
        assert(d_section == 0);
        assert(d_widgetlook != 0);
        String owner(attributes.getValueAsString(LookAttribute));
        d_section =
            new SectionSpecification(owner.empty() ? d_widgetlook->getName() : owner,
                                     attributes.getValueAsString(SectionNameAttribute),
                                     attributes.getValueAsString(ControlPropertyAttribute));

        CEGUI_LOGINSANE("---------> Layer references imagery section '" + d_section->getSectionName() + "'.");
    }

    /*************************************************************************
        Method that handles the opening ImageryComponent XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementImageryComponentStart(const XMLAttributes& attributes)
    {
        assert(d_imagerycomponent == 0);
        d_imagerycomponent = new ImageryComponent();

        CEGUI_LOGINSANE("-------> Image component definition...");
    }

    /*************************************************************************
        Method that handles the opening TextComponent XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementTextComponentStart(const XMLAttributes& attributes)
    {
        assert(d_textcomponent == 0);
        d_textcomponent = new TextComponent();

        CEGUI_LOGINSANE("-------> Text component definition...");
    }

    /*************************************************************************
        Method that handles the opening FrameComponent XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementFrameComponentStart(const XMLAttributes& attributes)
    {
        assert(d_framecomponent == 0);
        d_framecomponent = new FrameComponent();

        CEGUI_LOGINSANE("-------> Frame component definition...");
    }

    /*************************************************************************
        Method that handles the opening Area XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementAreaStart(const XMLAttributes& attributes)
    {
        assert(d_area == 0);
        d_area = new ComponentArea();
    }

    /*************************************************************************
        Method that handles the opening Image XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementImageStart(const XMLAttributes& attributes)
    {
        if (d_imagerycomponent)
        {
            d_imagerycomponent->setImage(attributes.getValueAsString(ImagesetAttribute), attributes.getValueAsString(ImageAttribute));
            CEGUI_LOGINSANE("---------> Using image: " + attributes.getValueAsString(ImageAttribute) + " from imageset: " + attributes.getValueAsString(ImagesetAttribute));
        }
        else if (d_framecomponent)
        {
            d_framecomponent->setImage(
                FalagardXMLHelper::stringToFrameImageComponent(attributes.getValueAsString(TypeAttribute)),
                attributes.getValueAsString(ImagesetAttribute),
                attributes.getValueAsString(ImageAttribute));

            CEGUI_LOGINSANE("---------> Using image: " +
                attributes.getValueAsString(ImageAttribute) + " from imageset: " +
                attributes.getValueAsString(ImagesetAttribute) + " for: " +
                attributes.getValueAsString(TypeAttribute));
        }
    }

    /*************************************************************************
        Method that handles the opening Colours XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementColoursStart(const XMLAttributes& attributes)
    {
        ColourRect cols(
            hexStringToARGB(attributes.getValueAsString(TopLeftAttribute)),
            hexStringToARGB(attributes.getValueAsString(TopRightAttribute)),
            hexStringToARGB(attributes.getValueAsString(BottomLeftAttribute)),
            hexStringToARGB(attributes.getValueAsString(BottomRightAttribute)));

        assignColours(cols);
    }

    /*************************************************************************
        Method that handles the opening VertFormat XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementVertFormatStart(const XMLAttributes& attributes)
    {
        if (d_framecomponent)
        {
            d_framecomponent->setBackgroundVerticalFormatting(FalagardXMLHelper::stringToVertFormat(attributes.getValueAsString(TypeAttribute)));
        }
        else if (d_imagerycomponent)
        {
            d_imagerycomponent->setVerticalFormatting(FalagardXMLHelper::stringToVertFormat(attributes.getValueAsString(TypeAttribute)));
        }
        else if (d_textcomponent)
        {
            d_textcomponent->setVerticalFormatting(FalagardXMLHelper::stringToVertTextFormat(attributes.getValueAsString(TypeAttribute)));
        }
    }

    /*************************************************************************
        Method that handles the opening HorzFormat XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementHorzFormatStart(const XMLAttributes& attributes)
    {
        if (d_framecomponent)
        {
            d_framecomponent->setBackgroundHorizontalFormatting(FalagardXMLHelper::stringToHorzFormat(attributes.getValueAsString(TypeAttribute)));
        }
        else if (d_imagerycomponent)
        {
            d_imagerycomponent->setHorizontalFormatting(FalagardXMLHelper::stringToHorzFormat(attributes.getValueAsString(TypeAttribute)));
        }
        else if (d_textcomponent)
        {
            d_textcomponent->setHorizontalFormatting(FalagardXMLHelper::stringToHorzTextFormat(attributes.getValueAsString(TypeAttribute)));
        }
    }

    /*************************************************************************
        Method that handles the opening VertAlignment XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementVertAlignmentStart(const XMLAttributes& attributes)
    {
        assert(d_childcomponent != 0);
        d_childcomponent->setVerticalWidgetAlignment(FalagardXMLHelper::stringToVertAlignment(attributes.getValueAsString(TypeAttribute)));
    }

    /*************************************************************************
        Method that handles the opening HorzAlignment XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementHorzAlignmentStart(const XMLAttributes& attributes)
    {
        assert(d_childcomponent != 0);
        d_childcomponent->setHorizontalWidgetAlignment(FalagardXMLHelper::stringToHorzAlignment(attributes.getValueAsString(TypeAttribute)));
    }

    /*************************************************************************
        Method that handles the opening Property XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementPropertyStart(const XMLAttributes& attributes)
    {
        assert(d_widgetlook != 0);
        PropertyInitialiser prop(attributes.getValueAsString(NameAttribute), attributes.getValueAsString(ValueAttribute));

        if (d_childcomponent)
        {
            d_childcomponent->addPropertyInitialiser(prop);
            CEGUI_LOGINSANE("-------> Added property initialiser for property: " + prop.getTargetPropertyName() + " with value: " + prop.getInitialiserValue());
        }
        else
        {
            d_widgetlook->addPropertyInitialiser(prop);
            CEGUI_LOGINSANE("---> Added property initialiser for property: " + prop.getTargetPropertyName() + " with value: " + prop.getInitialiserValue());
        }
    }

    /*************************************************************************
        Method that handles the opening Dim XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementDimStart(const XMLAttributes& attributes)
    {
        d_dimension.setDimensionType(FalagardXMLHelper::stringToDimensionType(attributes.getValueAsString(TypeAttribute)));
    }

    /*************************************************************************
        Method that handles the opening UnifiedDim XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementUnifiedDimStart(const XMLAttributes& attributes)
    {
        UnifiedDim base(
            UDim(attributes.getValueAsFloat(ScaleAttribute, 0.0f),
                 attributes.getValueAsFloat(OffsetAttribute, 0.0f)),
            FalagardXMLHelper::stringToDimensionType(attributes.getValueAsString(TypeAttribute)));

        doBaseDimStart(&base);
    }

    /*************************************************************************
        Method that handles the opening AbsoluteDim XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementAbsoluteDimStart(const XMLAttributes& attributes)
    {
        AbsoluteDim base(attributes.getValueAsFloat(ValueAttribute, 0.0f));
        doBaseDimStart(&base);
    }

    /*************************************************************************
        Method that handles the opening ImageDim XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementImageDimStart(const XMLAttributes& attributes)
    {
        ImageDim base(attributes.getValueAsString(ImagesetAttribute),
                      attributes.getValueAsString(ImageAttribute),
                      FalagardXMLHelper::stringToDimensionType(attributes.getValueAsString(DimensionAttribute)));

        doBaseDimStart(&base);
    }

    /*************************************************************************
        Method that handles the opening WidgetDim XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementWidgetDimStart(const XMLAttributes& attributes)
    {
        WidgetDim base(attributes.getValueAsString(WidgetAttribute),
                       FalagardXMLHelper::stringToDimensionType(attributes.getValueAsString(DimensionAttribute)));

        doBaseDimStart(&base);
    }

    /*************************************************************************
        Method that handles the opening FontDim XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementFontDimStart(const XMLAttributes& attributes)
    {
        FontDim base(
            attributes.getValueAsString(WidgetAttribute),
            attributes.getValueAsString(FontAttribute),
            attributes.getValueAsString(StringAttribute),
            FalagardXMLHelper::stringToFontMetricType(attributes.getValueAsString(TypeAttribute)),
            attributes.getValueAsFloat(PaddingAttribute, 0.0f));

        doBaseDimStart(&base);
    }

    /*************************************************************************
        Method that handles the opening PropertyDim XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementPropertyDimStart(const XMLAttributes& attributes)
    {
        String str_type = attributes.getValueAsString(TypeAttribute);
        DimensionType type = DT_INVALID;
        if (!str_type.empty())
            type = FalagardXMLHelper::stringToDimensionType(str_type);

        PropertyDim base(attributes.getValueAsString(WidgetAttribute),
                         attributes.getValueAsString(NameAttribute),
                         type);

        doBaseDimStart(&base);
    }

    /*************************************************************************
        Method that handles the opening Text XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementTextStart(const XMLAttributes& attributes)
    {
        assert (d_textcomponent != 0);
        d_textcomponent->setText(attributes.getValueAsString(StringAttribute));
        d_textcomponent->setFont(attributes.getValueAsString(FontAttribute));
    }

    /*************************************************************************
        Method that handles the opening ColourProperty XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementColourPropertyStart(const XMLAttributes& attributes)
    {
        // need to decide what to apply colours to
        if (d_framecomponent)
        {
            d_framecomponent->setColoursPropertySource(attributes.getValueAsString(NameAttribute));
            d_framecomponent->setColoursPropertyIsColourRect(false);
        }
        else if (d_imagerycomponent)
        {
            d_imagerycomponent->setColoursPropertySource(attributes.getValueAsString(NameAttribute));
            d_imagerycomponent->setColoursPropertyIsColourRect(false);
        }
        else if (d_textcomponent)
        {
            d_textcomponent->setColoursPropertySource(attributes.getValueAsString(NameAttribute));
            d_textcomponent->setColoursPropertyIsColourRect(false);
        }
        else if (d_imagerysection)
        {
            d_imagerysection->setMasterColoursPropertySource(attributes.getValueAsString(NameAttribute));
            d_imagerysection->setMasterColoursPropertyIsColourRect(false);
        }
        else if (d_section)
        {
            d_section->setOverrideColoursPropertySource(attributes.getValueAsString(NameAttribute));
            d_section->setOverrideColoursPropertyIsColourRect(false);
            d_section->setUsingOverrideColours(true);
        }
    }

    /*************************************************************************
        Method that handles the opening ColourRectProperty XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementColourRectPropertyStart(const XMLAttributes& attributes)
    {
        // need to decide what to apply colours to
        if (d_framecomponent)
        {
            d_framecomponent->setColoursPropertySource(attributes.getValueAsString(NameAttribute));
            d_framecomponent->setColoursPropertyIsColourRect(true);
        }
        else if (d_imagerycomponent)
        {
            d_imagerycomponent->setColoursPropertySource(attributes.getValueAsString(NameAttribute));
            d_imagerycomponent->setColoursPropertyIsColourRect(true);
        }
        else if (d_textcomponent)
        {
            d_textcomponent->setColoursPropertySource(attributes.getValueAsString(NameAttribute));
            d_textcomponent->setColoursPropertyIsColourRect(true);
        }
        else if (d_imagerysection)
        {
            d_imagerysection->setMasterColoursPropertySource(attributes.getValueAsString(NameAttribute));
            d_imagerysection->setMasterColoursPropertyIsColourRect(true);
        }
        else if (d_section)
        {
            d_section->setOverrideColoursPropertySource(attributes.getValueAsString(NameAttribute));
            d_section->setOverrideColoursPropertyIsColourRect(true);
            d_section->setUsingOverrideColours(true);
        }
    }

    /*************************************************************************
        Method that handles the opening NamedArea XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementNamedAreaStart(const XMLAttributes& attributes)
    {
        assert(d_namedArea == 0);
        d_namedArea = new NamedArea(attributes.getValueAsString(NameAttribute));

        CEGUI_LOGINSANE("-----> Creating named area: " + d_namedArea->getName());
    }

    /*************************************************************************
        Method that handles the opening PropertyDefinition XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementPropertyDefinitionStart(const XMLAttributes& attributes)
    {
        assert(d_widgetlook);

        PropertyDefinition prop(
            attributes.getValueAsString(NameAttribute),
            attributes.getValueAsString(InitialValueAttribute),
            attributes.getValueAsBool(RedrawOnWriteAttribute, false),
            attributes.getValueAsBool(LayoutOnWriteAttribute, false)
        );

        CEGUI_LOGINSANE("-----> Adding PropertyDefiniton. Name: " + prop.getName() + " Default Value: " + attributes.getValueAsString(InitialValueAttribute));

        d_widgetlook->addPropertyDefinition(prop);
    }

    /*************************************************************************
        Method that handles the opening PropertyLinkDefinition XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementPropertyLinkDefinitionStart(const XMLAttributes& attributes)
    {
        assert(d_widgetlook);

        PropertyLinkDefinition prop(
            attributes.getValueAsString(NameAttribute),
            attributes.getValueAsString(WidgetAttribute),
            attributes.getValueAsString(TargetPropertyAttribute),
            attributes.getValueAsString(InitialValueAttribute),
            attributes.getValueAsBool(RedrawOnWriteAttribute, false),
            attributes.getValueAsBool(LayoutOnWriteAttribute, false)
        );

        CEGUI_LOGINSANE("-----> Adding PropertyLinkDefiniton. Name: " + prop.getName() +
                        " Target widget: " + attributes.getValueAsString(WidgetAttribute) +
                        " Target property: " + attributes.getValueAsString(TargetPropertyAttribute) +
                        " Default Value: " + attributes.getValueAsString(InitialValueAttribute));

        d_widgetlook->addPropertyLinkDefinition(prop);
    }

    /*************************************************************************
        Method that handles the opening DimOperator XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementDimOperatorStart(const XMLAttributes& attributes)
    {
        if (!d_dimStack.empty())
        {
            d_dimStack.back()->setDimensionOperator(FalagardXMLHelper::stringToDimensionOperator(attributes.getValueAsString(OperatorAttribute)));
        }
    }

    /*************************************************************************
        Method that handles the opening VertFormatProperty XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementVertFormatPropertyStart(const XMLAttributes& attributes)
    {
        if (d_framecomponent)
            d_framecomponent->setVertFormattingPropertySource(attributes.getValueAsString(NameAttribute));
        else if (d_imagerycomponent)
            d_imagerycomponent->setVertFormattingPropertySource(attributes.getValueAsString(NameAttribute));
        else if (d_textcomponent)
            d_textcomponent->setVertFormattingPropertySource(attributes.getValueAsString(NameAttribute));
    }

    /*************************************************************************
        Method that handles the opening HorzFormatProperty XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementHorzFormatPropertyStart(const XMLAttributes& attributes)
    {
        if (d_framecomponent)
            d_framecomponent->setHorzFormattingPropertySource(attributes.getValueAsString(NameAttribute));
        else if (d_imagerycomponent)
            d_imagerycomponent->setHorzFormattingPropertySource(attributes.getValueAsString(NameAttribute));
        else if (d_textcomponent)
            d_textcomponent->setHorzFormattingPropertySource(attributes.getValueAsString(NameAttribute));
    }

    /*************************************************************************
        Method that handles the opening AreaProperty XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementAreaPropertyStart(const XMLAttributes& attributes)
    {
        assert (d_area != 0);

        d_area->setAreaPropertySource(attributes.getValueAsString(NameAttribute));
    }

    /*************************************************************************
        Method that handles the opening ImageProperty XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementImagePropertyStart(const XMLAttributes& attributes)
    {
        assert(d_imagerycomponent != 0);

        d_imagerycomponent->setImagePropertySource(attributes.getValueAsString(NameAttribute));
    }

    /*************************************************************************
        Method that handles the opening TextProperty XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementTextPropertyStart(const XMLAttributes& attributes)
    {
        assert(d_textcomponent != 0);

        d_textcomponent->setTextPropertySource(attributes.getValueAsString(NameAttribute));
    }

    /*************************************************************************
        Method that handles the opening FontProperty XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementFontPropertyStart(const XMLAttributes& attributes)
    {
        assert(d_textcomponent != 0);

        d_textcomponent->setFontPropertySource(attributes.getValueAsString(NameAttribute));
    }

    /*************************************************************************
        Method that handles the opening Colours XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementColourStart(const XMLAttributes& attributes)
    {
        ColourRect cols(hexStringToARGB(attributes.getValueAsString(ColourAttribute)));
        assignColours(cols);
    }

    /*************************************************************************
        Method that handles the closing Falagard XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementFalagardEnd()
    {
        Logger::getSingleton().logEvent("===== Look and feel parsing completed =====");
    }

    /*************************************************************************
        Method that handles the closing WidgetLook XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementWidgetLookEnd()
    {
        if (d_widgetlook)
        {
            Logger::getSingleton().logEvent("---< End of definition for widget look '" + d_widgetlook->getName() + "'.", Informative);
            d_manager->addWidgetLook(*d_widgetlook);
            delete d_widgetlook;
            d_widgetlook = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing Child XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementChildEnd()
    {
        assert(d_widgetlook != 0);

        if (d_childcomponent)
        {
            CEGUI_LOGINSANE("-----< End of definition for child widget. Type: " + d_childcomponent->getBaseWidgetType() + ".");
            d_widgetlook->addWidgetComponent(*d_childcomponent);
            delete d_childcomponent;
            d_childcomponent = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing ImagerySection XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementImagerySectionEnd()
    {
        assert(d_widgetlook != 0);

        if (d_imagerysection)
        {
            CEGUI_LOGINSANE("-----< End of definition for imagery section '" + d_imagerysection->getName() + "'.");
            d_widgetlook->addImagerySection(*d_imagerysection);
            delete d_imagerysection;
            d_imagerysection = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing StateImagery XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementStateImageryEnd()
    {
        assert(d_widgetlook != 0);

        if (d_stateimagery)
        {
            CEGUI_LOGINSANE("-----< End of definition for imagery for state '" + d_stateimagery->getName() + "'.");
            d_widgetlook->addStateSpecification(*d_stateimagery);
            delete d_stateimagery;
            d_stateimagery = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing Layer XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementLayerEnd()
    {
        assert(d_stateimagery != 0);

        if (d_layer)
        {
            CEGUI_LOGINSANE("-------< End of definition of imagery layer.");
            d_stateimagery->addLayer(*d_layer);
            delete d_layer;
            d_layer = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing Section XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementSectionEnd()
    {
        assert(d_layer != 0);

        if (d_section)
        {
            d_layer->addSectionSpecification(*d_section);
            delete d_section;
            d_section = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing ImageryComponent XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementImageryComponentEnd()
    {
        assert(d_imagerysection != 0);

        if (d_imagerycomponent)
        {
            d_imagerysection->addImageryComponent(*d_imagerycomponent);
            delete d_imagerycomponent;
            d_imagerycomponent = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing TextComponent XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementTextComponentEnd()
    {
        assert(d_imagerysection != 0);

        if (d_textcomponent)
        {
            d_imagerysection->addTextComponent(*d_textcomponent);
            delete d_textcomponent;
            d_textcomponent = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing FrameComponent XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementFrameComponentEnd()
    {
        assert(d_imagerysection != 0);

        if (d_framecomponent)
        {
            d_imagerysection->addFrameComponent(*d_framecomponent);
            delete d_framecomponent;
            d_framecomponent = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing Area XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementAreaEnd()
    {
        assert((d_childcomponent != 0) || (d_imagerycomponent != 0) || (d_textcomponent != 0) || d_namedArea != 0 || d_framecomponent != 0);
        assert(d_area != 0);

        if (d_childcomponent)
        {
            d_childcomponent->setComponentArea(*d_area);
        }
        else if (d_framecomponent)
        {
            d_framecomponent->setComponentArea(*d_area);
        }
        else if (d_imagerycomponent)
        {
            d_imagerycomponent->setComponentArea(*d_area);
        }
        else if (d_textcomponent)
        {
            d_textcomponent->setComponentArea(*d_area);
        }
        else if (d_namedArea)
        {
            d_namedArea->setArea(*d_area);
        }

        delete d_area;
        d_area = 0;
    }

    /*************************************************************************
        Method that handles the closing NamedArea XML element.
    *************************************************************************/
    void Falagard_xmlHandler::elementNamedAreaEnd()
    {
        assert(d_widgetlook != 0);

        if (d_namedArea)
        {
            d_widgetlook->addNamedArea(*d_namedArea);
            delete d_namedArea;
            d_namedArea = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing of all *Dim XML elements.
    *************************************************************************/
    void Falagard_xmlHandler::elementAnyDimEnd()
    {
        if (!d_dimStack.empty())
        {
            BaseDim* currDim = d_dimStack.back();
            d_dimStack.pop_back();

            if (!d_dimStack.empty())
            {
                d_dimStack.back()->setOperand(*currDim);
            }
            else
            {
                d_dimension.setBaseDimension(*currDim);
                assignAreaDimension(d_dimension);
            }

            // release the dim we popped.
            delete currDim;
        }
    }


    /*************************************************************************
        register a handler for the opening tag of an XML element
    *************************************************************************/
    void Falagard_xmlHandler::registerElementStartHandler(const String& element, ElementStartHandler handler)
    {
        d_startHandlersMap[element] = handler;
    }

    /*************************************************************************
        register a handler for the closing tag of an XML element
    *************************************************************************/
    void Falagard_xmlHandler::registerElementEndHandler(const String& element, ElementEndHandler handler)
    {
        d_endHandlersMap[element] = handler;
    }

} // End of  CEGUI namespace section
