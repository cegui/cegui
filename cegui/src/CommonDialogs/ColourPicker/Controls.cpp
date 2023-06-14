/***********************************************************************
    created:    20th February 2010
    author:     Lukas E Meindl

    purpose:    Implementation of ColourPickerControls base class
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/widgets/PushButton.h"
#include "CEGUI/widgets/Editbox.h"
#include "CEGUI/widgets/RadioButton.h"
#include "CEGUI/widgets/Titlebar.h"
#include "CEGUI/widgets/Slider.h"
#include "CEGUI/widgets/Thumb.h"

#include "CEGUI/System.h"
#include "CEGUI/Renderer.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/BitmapImage.h"
#include "CEGUI/Logger.h"

#include "CEGUI/CommonDialogs/ColourPicker/ColourPicker.h"
#include "CEGUI/CommonDialogs/ColourPicker/Controls.h"
#include "CEGUI/CommonDialogs/ColourPicker/Conversions.h"

#include "CEGUI/TextureTarget.h"
#include "CEGUI/Texture.h"
#include "CEGUI/PropertyHelper.h"

#if defined(CEGUI_REGEX_MATCHER_PCRE)
#   include "CEGUI/PCRERegexMatcher.h"
#elif defined(CEGUI_REGEX_MATCHER_STD)
#   include "CEGUI/StdRegexMatcher.h"
#endif

#include <sstream>
#include <algorithm>
#include <math.h>

namespace CEGUI
{
//----------------------------------------------------------------------------//
const String ColourPickerControls::EventNamespace("ColourPickerControls");
const String ColourPickerControls::WidgetTypeName("CEGUI/ColourPickerControls");
const String ColourPickerControls::EventColourAccepted("ColourAccepted");
const String ColourPickerControls::EventClosed("Closed");
const String ColourPickerControls::ColourRectPropertyName("Colour");
//----------------------------------------------------------------------------//
const String ColourPickerControls::ColourPickerControlsPickingTextureImageName("ColourPickingTexture");
const String ColourPickerControls::ColourPickerControlsColourSliderTextureImageName("ColourSliderTexture");
const String ColourPickerControls::ColourPickerControlsAlphaSliderTextureImageName("AlphaSliderTexture");
//----------------------------------------------------------------------------//
const float ColourPickerControls::LAB_L_MIN(0.0f);
const float ColourPickerControls::LAB_L_MAX(100.0f);
const float ColourPickerControls::LAB_L_DIFF(LAB_L_MAX - LAB_L_MIN);
const float ColourPickerControls::LAB_A_MIN(-130.0f);
const float ColourPickerControls::LAB_A_MAX(130.0f);
const float ColourPickerControls::LAB_A_DIFF(LAB_A_MAX - LAB_A_MIN);
const float ColourPickerControls::LAB_B_MIN(-130.0f);
const float ColourPickerControls::LAB_B_MAX(130.0f);
const float ColourPickerControls::LAB_B_DIFF(LAB_B_MAX - LAB_B_MIN);
//----------------------------------------------------------------------------//
// Child Widget name constants
const String ColourPickerControls::CancelButtonName("__auto_cancelbutton__");
const String ColourPickerControls::AcceptButtonName("__auto_acceptbutton__");
const String ColourPickerControls::HexaDecimalDescriptionName("__auto_hexadecimalstatictext__");
const String ColourPickerControls::HexaDecimalEditBoxName("__auto_hexadecimaleditbox__");
const String ColourPickerControls::TitleBarName("__auto_titlebar__");

const String ColourPickerControls::ColourPickerStaticImageName("__auto_colourpickerstaticimage__");
const String ColourPickerControls::ColourPickerImageSliderName("__auto_colourpickerimageslider__");
const String ColourPickerControls::ColourPickerAlphaSliderName("__auto_colourpickeralphaslider__");

const String ColourPickerControls::NewColourDescriptionName("__auto_newcolourdescription__");
const String ColourPickerControls::OldColourDescriptionName("__auto_oldcolourdescription__");

const String ColourPickerControls::NewColourRectName("__auto_newcolourrect__");
const String ColourPickerControls::OldColourRectName("__auto_oldcolourrect__");

const String ColourPickerControls::ColourEditBoxRDescriptionName("__auto_coloureditboxRdescription__");
const String ColourPickerControls::ColourEditBoxRName("__auto_coloureditboxR__");
const String ColourPickerControls::ColourEditBoxGDescriptionName("__auto_coloureditboxGdescription__");
const String ColourPickerControls::ColourEditBoxGName("__auto_coloureditboxG__");
const String ColourPickerControls::ColourEditBoxBDescriptionName("__auto_coloureditboxBdescription__");
const String ColourPickerControls::ColourEditBoxBName("__auto_coloureditboxB__");

const String ColourPickerControls::HSVRadioButtonHName("__auto_HSVradiobuttonH__");
const String ColourPickerControls::HSVEditBoxHName("__auto_HSVeditboxH__");
const String ColourPickerControls::HSVRadioButtonSName("__auto_HSVradiobuttonS__");
const String ColourPickerControls::HSVEditBoxSName("__auto_HSVeditboxS__");
const String ColourPickerControls::HSVRadioButtonVName("__auto_HSVradiobuttonV__");
const String ColourPickerControls::HSVEditBoxVName("__auto_HSVeditboxV__");

const String ColourPickerControls::LabRadioButtonLName("__auto_LabradiobuttonL__");
const String ColourPickerControls::LabEditBoxLName("__auto_LabeditboxL__");
const String ColourPickerControls::LabRadioButtonAName("__auto_Labradiobuttona__");
const String ColourPickerControls::LabEditBoxAName("__auto_Labeditboxa__");
const String ColourPickerControls::LabRadioButtonBName("__auto_Labradiobuttonb__");
const String ColourPickerControls::LabEditBoxBName("__auto_Labeditboxb__");

const String ColourPickerControls::AlphaEditBoxName("__auto_alphaeditbox__");

const String ColourPickerControls::ColourPickerCursorName("__colourpickercursor__");

//----------------------------------------------------------------------------//
static RegexMatcher* createRegexMatcher()
{
#if defined(CEGUI_REGEX_MATCHER_PCRE)
    return new PCRERegexMatcher();
#elif defined(CEGUI_REGEX_MATCHER_STD)
    return new StdRegexMatcher();
#else
    return nullptr;
#endif
}

//----------------------------------------------------------------------------//
ColourPickerControls::ColourPickerControls(const String& type, const String& name) :
    Window(type, name),
    d_callingColourPicker(nullptr),
    d_colourPickerIndicator(nullptr),
    d_sliderMode(SliderMode::LAB_L),
    d_selectedColour(0.75f, 0.75f, 0.75f),
    d_colourPickerControlsTextureTarget(nullptr),
    d_colourPickerImageOffset(2),
    d_colourPickerPickingImageHeight(260),
    d_colourPickerPickingImageWidth(260),
    d_colourPickerColourSliderImageWidth(1),
    d_colourPickerColourSliderImageHeight(260),
    d_colourPickerAlphaSliderImageWidth(260),
    d_colourPickerAlphaSliderImageHeight(60),
    d_colourPickerControlsTextureSize(512),
    d_draggingColourPickerIndicator(false),
    d_colourPickingTexture(new RGB_Colour[d_colourPickerControlsTextureSize *
                                          d_colourPickerControlsTextureSize]),
    d_ignoreEvents(false),
    d_regexMatcher(*createRegexMatcher())
{
}

//----------------------------------------------------------------------------//
ColourPickerControls::~ColourPickerControls()
{
}

//----------------------------------------------------------------------------//
Lab_Colour ColourPickerControls::getColourPickingPositionColourLAB(float xAbs,
                                                                   float yAbs)
{
    float L = 0.0f;
    float a = 0.0f;
    float b = 0.0f;

    float xRel = xAbs / static_cast<float>(d_colourPickerPickingImageWidth - 1);
    float yRel = yAbs / static_cast<float>(d_colourPickerPickingImageHeight - 1);

    if (d_sliderMode != LAB_L)
    {
        L = LAB_L_MAX - LAB_L_DIFF * yRel ;

        if (d_sliderMode != LAB_A)
        {
            b = d_selectedColourLAB.b;
            a = LAB_A_MAX - LAB_A_DIFF * xRel;
        }
        else
        {
            a = d_selectedColourLAB.a;
            b = LAB_B_MAX - LAB_B_DIFF * xRel;
        }
    }
    else
    {
        L = d_selectedColourLAB.L;
        a = LAB_A_MAX - LAB_A_DIFF * xRel;
        b = LAB_B_MAX - LAB_B_DIFF * yRel;
    }

    return Lab_Colour(L, a, b);
}

//----------------------------------------------------------------------------//
HSV_Colour ColourPickerControls::getColourPickingPositionColourHSV(float xAbs,
                                                                   float yAbs)
{
    HSV_Colour colour;

    float xRel = xAbs / static_cast<float>(d_colourPickerPickingImageWidth - 1);
    float yRel = yAbs / static_cast<float>(d_colourPickerPickingImageHeight - 1);

    if (d_sliderMode != SliderMode::HSV_H)
    {
        float xCoord = (xRel - 0.5f) * 2.0f;
        float yCoord = (yRel - 0.5f) * 2.0f;

        float angle = std::atan2(yCoord, xCoord);

        if (angle < 0.0f)
            angle += 2.0f * 3.1415926535897932384626433832795f;

        angle /= 2.0f * 3.1415926535897932384626433832795f;
        colour.H = angle;

        float length = std::sqrt(xCoord * xCoord + yCoord * yCoord);
        float value = std::min(length, 1.0f);

        if (d_sliderMode != SliderMode::HSV_S)
        {
            colour.V = d_selectedColourHSV.V;
            colour.S = value;
        }
        else
        {
            colour.S = d_selectedColourHSV.S;
            colour.V = value;
        }
    }
    else
    {
        colour.H = d_selectedColourHSV.H;
        colour.S = xRel;
        colour.V = 1.0f - yRel;
    }

    return colour;
}

//----------------------------------------------------------------------------//
glm::vec2 ColourPickerControls::getColourPickingColourPosition()
{
    float x = 0.0f;
    float y = 0.0f;

    switch (d_sliderMode)
    {
    case LAB_L:
        x = 1.0f - (d_selectedColourLAB.a - LAB_A_MIN) / LAB_A_DIFF;
        y = 1.0f - (d_selectedColourLAB.b - LAB_B_MIN) / LAB_B_DIFF;
        break;

    case LAB_A:
        x = 1.0f - (d_selectedColourLAB.b - LAB_B_MIN) / LAB_B_DIFF;
        y = 1.0f - (d_selectedColourLAB.L - LAB_L_MIN) / LAB_L_DIFF;
        break;

    case SliderMode::LAB_B:
        x = 1.0f - (d_selectedColourLAB.a - LAB_A_MIN) / LAB_A_DIFF;
        y = 1.0f - (d_selectedColourLAB.L - LAB_L_MIN) / LAB_L_DIFF;
        break;

    case SliderMode::HSV_H:
        x = d_selectedColourHSV.S;
        y = 1.0f - d_selectedColourHSV.V;
        break;

    case SliderMode::HSV_S:
        getColourPickingColourPositionHSV(x, y);
        break;

    case SliderMode::HSV_V:
        getColourPickingColourPositionHSV(x, y);
        break;

    default:
        break;
    }

    return glm::vec2(x, y);
}

//----------------------------------------------------------------------------//
void ColourPickerControls::getColourPickingColourPositionHSV(float& x, float& y)
{
    float radius;

    switch (d_sliderMode)
    {
    case SliderMode::HSV_S:
        radius = d_selectedColourHSV.V;
        break;

    case SliderMode::HSV_V:
        radius = d_selectedColourHSV.S;
        break;

    default:
        radius = 0.0f; //This shouldn't happen.  (PDT: Why is it here, then?)
        break;
    }

    float angle = d_selectedColourHSV.H;
    angle *= 2.0f * 3.1415926535897932384626433832795f;

    x = radius * cos(angle) * 0.5f + 0.5f;
    y = radius * sin(angle) * 0.5f + 0.5f;
}

//----------------------------------------------------------------------------//
Lab_Colour ColourPickerControls::getColourSliderPositionColourLAB(float value)
{
    Lab_Colour colour = d_selectedColourLAB;

    switch (d_sliderMode)
    {
    case LAB_L:
        colour.L = LAB_L_MAX - LAB_L_DIFF * value;
        break;

    case LAB_A:
        colour.a = LAB_A_MAX - LAB_A_DIFF * value;
        break;

    case SliderMode::LAB_B:
        colour.b = LAB_B_MAX - LAB_B_DIFF * value;
        break;

    default:
        break;
    }

    return colour;
}

//----------------------------------------------------------------------------//
HSV_Colour ColourPickerControls::getColourSliderPositionColourHSV(float value)
{
    HSV_Colour colour = d_selectedColourHSV;

    switch (d_sliderMode)
    {
    case SliderMode::HSV_H:
        colour.H = 1.0f - value;
        break;

    case SliderMode::HSV_S:
        colour.S = 1.0f - value;
        break;

    case SliderMode::HSV_V:
        colour.V = 1.0f - value;
        break;

    default:
        break;
    }

    return colour;
}

//----------------------------------------------------------------------------//
void ColourPickerControls::initColourPickerControlsImageSet()
{
    d_colourPickerControlsTextureTarget =
        System::getSingleton().getRenderer()->createTextureTarget(false);

    const String baseName(
        d_colourPickerControlsTextureTarget->getTexture().getName());

    BitmapImage* image = static_cast<BitmapImage*>(
        &ImageManager::getSingleton().create("BitmapImage", baseName + '/' +
            ColourPickerControlsPickingTextureImageName));

    image->setTexture(&d_colourPickerControlsTextureTarget->getTexture());
    image->setImageArea(
        Rectf(glm::vec2(0.0f, 0.0f),
              Sizef(static_cast<float>(d_colourPickerPickingImageWidth),
                    static_cast<float>(d_colourPickerPickingImageHeight))));

    image = static_cast<BitmapImage*>(
                &ImageManager::getSingleton().create("BitmapImage", baseName + '/' +
                        ColourPickerControlsColourSliderTextureImageName));

    image->setTexture(&d_colourPickerControlsTextureTarget->getTexture());
    image->setImageArea(
        Rectf(glm::vec2(static_cast<float>(d_colourPickerPickingImageWidth + d_colourPickerImageOffset), 0.0f),
              Sizef(static_cast<float>(d_colourPickerColourSliderImageWidth),
                    static_cast<float>(d_colourPickerColourSliderImageHeight))));

    image = static_cast<BitmapImage*>(
                &ImageManager::getSingleton().create("BitmapImage", baseName + '/' +
                        ColourPickerControlsAlphaSliderTextureImageName));

    image->setTexture(&d_colourPickerControlsTextureTarget->getTexture());
    image->setImageArea(
        Rectf(glm::vec2(0.0f, static_cast<float>(d_colourPickerPickingImageHeight + d_colourPickerImageOffset)),
              Sizef(static_cast<float>(d_colourPickerAlphaSliderImageWidth),
                    static_cast<float>(d_colourPickerAlphaSliderImageHeight))));

    getColourPickerStaticImage()->setProperty(
        "Image", baseName + '/' + ColourPickerControlsPickingTextureImageName);
    getColourPickerImageSlider()->setProperty(
        "ScrollImage", baseName + '/' + ColourPickerControlsColourSliderTextureImageName);
    getColourPickerAlphaSlider()->setProperty(
        "ScrollImage", baseName + '/' + ColourPickerControlsAlphaSliderTextureImageName);

    refreshColourPickerControlsTextures();
}

//----------------------------------------------------------------------------//
void ColourPickerControls::deinitColourPickerControlsTexture()
{
    if (d_colourPickerControlsTextureTarget)
    {
        ImageManager::getSingleton().destroyImageCollection(
            d_colourPickerControlsTextureTarget->getTexture().getName(), false);

        System::getSingleton().getRenderer()->
        destroyTextureTarget(d_colourPickerControlsTextureTarget);
    }
}

//----------------------------------------------------------------------------//
void ColourPickerControls::refreshColourPickerControlsTextures()
{
    Logger::getSingleton().logEvent(
        "[ColourPicker] Refreshing ColourPickerTexture");

    refreshColourPickingImage();
    refreshColourSliderImage();
    refreshAlphaSliderImage();

    reloadColourPickerControlsTexture();
}

//----------------------------------------------------------------------------//
void ColourPickerControls::reloadColourPickerControlsTexture()
{
    d_colourPickerControlsTextureTarget->getTexture().loadFromMemory(
        d_colourPickingTexture,
        Sizef(static_cast<float>(d_colourPickerControlsTextureSize),
              static_cast<float>(d_colourPickerControlsTextureSize)),
        Texture::PixelFormat::Rgb);

    getColourPickerImageSlider()->invalidate();
    getColourPickerAlphaSlider()->invalidate();
    getColourPickerStaticImage()->invalidate();
}

//----------------------------------------------------------------------------//
void ColourPickerControls::initialiseComponents()
{
    getCancelButton()->subscribeEvent(
        PushButton::EventClicked,
        Event::Subscriber(&ColourPickerControls::handleCancelButtonClicked, this));

    getAcceptButton()->subscribeEvent(
        PushButton::EventClicked,
        Event::Subscriber(&ColourPickerControls::handleAcceptButtonClicked, this));

    getHexadecimalEditbox()->subscribeEvent(
        Editbox::EventDeactivated,
        Event::Subscriber(&ColourPickerControls::handleEditboxDeactivated, this));

    getColourEditBoxR()->subscribeEvent(
        Editbox::EventDeactivated,
        Event::Subscriber(&ColourPickerControls::handleEditboxDeactivated, this));

    getColourEditBoxG()->subscribeEvent(
        Editbox::EventDeactivated,
        Event::Subscriber(&ColourPickerControls::handleEditboxDeactivated, this));

    getColourEditBoxB()->subscribeEvent(
        Editbox::EventDeactivated,
        Event::Subscriber(&ColourPickerControls::handleEditboxDeactivated, this));

    getHSVEditBoxH()->subscribeEvent(
        Editbox::EventDeactivated,
        Event::Subscriber(&ColourPickerControls::handleEditboxDeactivated, this));

    getHSVEditBoxS()->subscribeEvent(
        Editbox::EventDeactivated,
        Event::Subscriber(&ColourPickerControls::handleEditboxDeactivated, this));

    getHSVEditBoxV()->subscribeEvent(
        Editbox::EventDeactivated,
        Event::Subscriber(&ColourPickerControls::handleEditboxDeactivated, this));

    getLabEditBoxL()->subscribeEvent(
        Editbox::EventDeactivated,
        Event::Subscriber(&ColourPickerControls::handleEditboxDeactivated, this));

    getLabEditBoxA()->subscribeEvent(
        Editbox::EventDeactivated,
        Event::Subscriber(&ColourPickerControls::handleEditboxDeactivated, this));

    getLabEditBoxB()->subscribeEvent(
        Editbox::EventDeactivated,
        Event::Subscriber(&ColourPickerControls::handleEditboxDeactivated, this));

    getAlphaEditBox()->subscribeEvent(
        Editbox::EventDeactivated,
        Event::Subscriber(&ColourPickerControls::handleEditboxDeactivated, this));

    getHSVRadioButtonH()->subscribeEvent(
        RadioButton::EventSelectStateChanged,
        Event::Subscriber(&ColourPickerControls::handleRadioButtonModeSelection, this));

    getHSVRadioButtonS()->subscribeEvent(
        RadioButton::EventSelectStateChanged,
        Event::Subscriber(&ColourPickerControls::handleRadioButtonModeSelection, this));

    getHSVRadioButtonV()->subscribeEvent(
        RadioButton::EventSelectStateChanged,
        Event::Subscriber(&ColourPickerControls::handleRadioButtonModeSelection, this));

    getLabRadioButtonL()->subscribeEvent(
        RadioButton::EventSelectStateChanged,
        Event::Subscriber(&ColourPickerControls::handleRadioButtonModeSelection, this));

    getLabRadioButtonA()->subscribeEvent(
        RadioButton::EventSelectStateChanged,
        Event::Subscriber(&ColourPickerControls::handleRadioButtonModeSelection, this));

    getLabRadioButtonB()->subscribeEvent(
        RadioButton::EventSelectStateChanged,
        Event::Subscriber(&ColourPickerControls::handleRadioButtonModeSelection, this));

    getHexadecimalEditbox()->subscribeEvent(
        Editbox::EventTextChanged,
        Event::Subscriber(&ColourPickerControls::handleHexadecimalEditboxTextChanged, this));

    getColourEditBoxR()->subscribeEvent(
        Editbox::EventTextChanged,
        Event::Subscriber(&ColourPickerControls::handleRGBEditboxTextChanged, this));

    getColourEditBoxG()->subscribeEvent(
        Editbox::EventTextChanged,
        Event::Subscriber(&ColourPickerControls::handleRGBEditboxTextChanged, this));

    getColourEditBoxB()->subscribeEvent(
        Editbox::EventTextChanged,
        Event::Subscriber(&ColourPickerControls::handleRGBEditboxTextChanged, this));

    getLabEditBoxL()->subscribeEvent(
        Editbox::EventTextChanged,
        Event::Subscriber(&ColourPickerControls::handleLABEditboxTextChanged, this));

    getLabEditBoxA()->subscribeEvent(
        Editbox::EventTextChanged,
        Event::Subscriber(&ColourPickerControls::handleLABEditboxTextChanged, this));

    getLabEditBoxB()->subscribeEvent(
        Editbox::EventTextChanged,
        Event::Subscriber(&ColourPickerControls::handleLABEditboxTextChanged, this));

    getHSVEditBoxH()->subscribeEvent(
        Editbox::EventTextChanged,
        Event::Subscriber(&ColourPickerControls::handleHSVEditboxTextChanged, this));

    getHSVEditBoxS()->subscribeEvent(
        Editbox::EventTextChanged,
        Event::Subscriber(&ColourPickerControls::handleHSVEditboxTextChanged, this));

    getHSVEditBoxV()->subscribeEvent(
        Editbox::EventTextChanged,
        Event::Subscriber(&ColourPickerControls::handleHSVEditboxTextChanged, this));

    getAlphaEditBox()->subscribeEvent(
        Editbox::EventTextChanged,
        Event::Subscriber(&ColourPickerControls::handleAlphaEditboxTextChanged, this));

    getColourPickerImageSlider()->subscribeEvent(
        Slider::EventValueChanged,
        Event::Subscriber(&ColourPickerControls::handleColourPickerSliderValueChanged, this));

    getColourPickerAlphaSlider()->subscribeEvent(
        Slider::EventValueChanged,
        Event::Subscriber(&ColourPickerControls::handleAlphaSliderValueChanged, this));

    getColourPickerStaticImage()->subscribeEvent(
        Window::EventCursorLeavesSurface,
        Event::Subscriber(&ColourPickerControls::handleColourPickerStaticImagePointerLeaves, this));

    getColourPickerStaticImage()->subscribeEvent(
        Window::EventClick,
        Event::Subscriber(&ColourPickerControls::handleColourPickerStaticImagePointerActivate, this));

    getColourPickerStaticImage()->subscribeEvent(
        Window::EventMouseButtonDown,
        Event::Subscriber(&ColourPickerControls::handleColourPickerStaticImagePointerPressHold, this));

    getColourPickerStaticImage()->subscribeEvent(
        Window::EventCursorMove,
        Event::Subscriber(&ColourPickerControls::handleColourPickerStaticImagePointerMove, this));

    initColourPicker();

    Window::initialiseComponents();
}

void ColourPickerControls::destroy()
{
    deinitColourPickerControlsTexture();

    if (d_colourPickerIndicator)
    {
        WindowManager::getSingleton().destroyWindow(d_colourPickerIndicator);
        d_colourPickerIndicator = nullptr;
    }

    delete[] d_colourPickingTexture;
    d_colourPickingTexture = nullptr;

    delete &d_regexMatcher;

    Window::destroy();
}

//----------------------------------------------------------------------------//
void ColourPickerControls::setPreviousColour(const Colour& previousColour)
{
    d_previouslySelectedColour = previousColour;
}

//----------------------------------------------------------------------------//
void ColourPickerControls::setColours(const Colour& newColour)
{
    d_selectedColourRGB = RGB_Colour(newColour);
    d_selectedColourLAB = Lab_Colour(d_selectedColourRGB);
    d_selectedColourHSV = HSV_Colour(d_selectedColourRGB);
    d_selectedColour = newColour;
}

//----------------------------------------------------------------------------//
void ColourPickerControls::setColours(const Lab_Colour& newColourLAB)
{
    d_selectedColourLAB = newColourLAB;
    d_selectedColourRGB = RGB_Colour(newColourLAB);
    d_selectedColourHSV = HSV_Colour(d_selectedColourRGB);

    float alpha = d_selectedColour.getAlpha();
    d_selectedColour = ColourPickerConversions::toCeguiColour(d_selectedColourRGB);
    d_selectedColour.setAlpha(alpha);
}

//----------------------------------------------------------------------------//
void ColourPickerControls::setColours(const RGB_Colour& newColourRGB)
{
    d_selectedColourLAB = Lab_Colour(newColourRGB);
    d_selectedColourRGB = newColourRGB;
    d_selectedColourHSV = HSV_Colour(newColourRGB);

    float alpha = d_selectedColour.getAlpha();
    d_selectedColour = ColourPickerConversions::toCeguiColour(d_selectedColourRGB);
    d_selectedColour.setAlpha(alpha);
}

//----------------------------------------------------------------------------//
void ColourPickerControls::setColours(const HSV_Colour& newColourHSV)
{
    d_selectedColourRGB = RGB_Colour(newColourHSV);
    d_selectedColourLAB = Lab_Colour(d_selectedColourRGB);
    d_selectedColourHSV = newColourHSV;

    float alpha = d_selectedColour.getAlpha();
    d_selectedColour = ColourPickerConversions::toCeguiColour(d_selectedColourRGB);
    d_selectedColour.setAlpha(alpha);
}

//----------------------------------------------------------------------------//
void ColourPickerControls::setCallingColourPicker(ColourPicker* colourPicker)
{
    d_callingColourPicker = colourPicker;
}

//----------------------------------------------------------------------------//
PushButton* ColourPickerControls::getCancelButton()
{
    return static_cast<PushButton*>(getChild(CancelButtonName));
}

//----------------------------------------------------------------------------//
PushButton* ColourPickerControls::getAcceptButton()
{
    return static_cast<PushButton*>(getChild(AcceptButtonName));
}

//----------------------------------------------------------------------------//
Window* ColourPickerControls::getHexadecimalDescription()
{
    return getChild(HexaDecimalDescriptionName);
}

//----------------------------------------------------------------------------//
Editbox* ColourPickerControls::getHexadecimalEditbox()
{
    return static_cast<Editbox*>(getChild(HexaDecimalEditBoxName));
}

//----------------------------------------------------------------------------//
Titlebar* ColourPickerControls::getTitleBar()
{
    return static_cast<Titlebar*>(getChild(TitleBarName));
}

//----------------------------------------------------------------------------//
Window* ColourPickerControls::getColourPickerStaticImage()
{
    return getChild(ColourPickerStaticImageName);
}

//----------------------------------------------------------------------------//
Slider* ColourPickerControls::getColourPickerImageSlider()
{
    return static_cast<Slider*>(getChild(ColourPickerImageSliderName));
}

//----------------------------------------------------------------------------//
Slider* ColourPickerControls::getColourPickerAlphaSlider()
{
    return static_cast<Slider*>(getChild(ColourPickerAlphaSliderName));
}

//----------------------------------------------------------------------------//
Window* ColourPickerControls::getNewColourDescription()
{
    return getChild(NewColourDescriptionName);
}

//----------------------------------------------------------------------------//
Window* ColourPickerControls::getOldColourDescription()
{
    return getChild(OldColourDescriptionName);
}

//----------------------------------------------------------------------------//
Window* ColourPickerControls::getNewColourRect()
{
    return getChild(NewColourRectName);
}

//----------------------------------------------------------------------------//
Window* ColourPickerControls::getOldColourRect()
{
    return getChild(OldColourRectName);
}

//----------------------------------------------------------------------------//
Window* ColourPickerControls::getColourEditBoxRDescription()
{
    return getChild(ColourEditBoxRDescriptionName);
}

//----------------------------------------------------------------------------//
Window* ColourPickerControls::getColourEditBoxGDescription()
{
    return getChild(ColourEditBoxGDescriptionName);
}

//----------------------------------------------------------------------------//
Window* ColourPickerControls::getColourEditBoxBDescription()
{
    return getChild(ColourEditBoxBDescriptionName);
}

//----------------------------------------------------------------------------//
Editbox* ColourPickerControls::getColourEditBoxR()
{
    return static_cast<Editbox*>(getChild(ColourEditBoxRName));
}

//----------------------------------------------------------------------------//
Editbox* ColourPickerControls::getColourEditBoxG()
{
    return static_cast<Editbox*>(getChild(ColourEditBoxGName));
}

//----------------------------------------------------------------------------//
Editbox* ColourPickerControls::getColourEditBoxB()
{
    return static_cast<Editbox*>(getChild(ColourEditBoxBName));
}

//----------------------------------------------------------------------------//
RadioButton* ColourPickerControls::getHSVRadioButtonH()
{
    return static_cast<RadioButton*>(getChild(HSVRadioButtonHName));
}

//----------------------------------------------------------------------------//
RadioButton* ColourPickerControls::getHSVRadioButtonS()
{
    return static_cast<RadioButton*>(getChild(HSVRadioButtonSName));
}

//----------------------------------------------------------------------------//
RadioButton* ColourPickerControls::getHSVRadioButtonV()
{
    return static_cast<RadioButton*>(getChild(HSVRadioButtonVName));
}

//----------------------------------------------------------------------------//
Editbox* ColourPickerControls::getHSVEditBoxH()
{
    return static_cast<Editbox*>(getChild(HSVEditBoxHName));
}

//----------------------------------------------------------------------------//
Editbox* ColourPickerControls::getHSVEditBoxS()
{
    return static_cast<Editbox*>(getChild(HSVEditBoxSName));
}

//----------------------------------------------------------------------------//
Editbox* ColourPickerControls::getHSVEditBoxV()
{
    return static_cast<Editbox*>(getChild(HSVEditBoxVName));
}


//----------------------------------------------------------------------------//
RadioButton* ColourPickerControls::getLabRadioButtonL()
{
    return static_cast<RadioButton*>(getChild(LabRadioButtonLName));
}

//----------------------------------------------------------------------------//
RadioButton* ColourPickerControls::getLabRadioButtonA()
{
    return static_cast<RadioButton*>(getChild(LabRadioButtonAName));
}

//----------------------------------------------------------------------------//
RadioButton* ColourPickerControls::getLabRadioButtonB()
{
    return static_cast<RadioButton*>(getChild(LabRadioButtonBName));
}

//----------------------------------------------------------------------------//
Editbox* ColourPickerControls::getLabEditBoxL()
{
    return static_cast<Editbox*>(getChild(LabEditBoxLName));
}

//----------------------------------------------------------------------------//
Editbox* ColourPickerControls::getLabEditBoxA()
{
    return static_cast<Editbox*>(getChild(LabEditBoxAName));
}

//----------------------------------------------------------------------------//
Editbox* ColourPickerControls::getLabEditBoxB()
{
    return static_cast<Editbox*>(getChild(LabEditBoxBName));
}


//----------------------------------------------------------------------------//
Editbox* ColourPickerControls::getAlphaEditBox()
{
    return static_cast<Editbox*>(getChild(AlphaEditBoxName));
}

//----------------------------------------------------------------------------//
Window* ColourPickerControls::getColourPickerCursorStaticImage()
{
    return d_colourPickerIndicator;
}

//----------------------------------------------------------------------------//
bool ColourPickerControls::handleCancelButtonClicked(const EventArgs&)
{
    WindowEventArgs args(this);
    onCancelButtonClicked(args);

    return true;
}

//----------------------------------------------------------------------------//
bool ColourPickerControls::handleEditboxDeactivated(const EventArgs& args)
{
    Editbox* editbox = static_cast<Editbox*>(
        static_cast<const WindowEventArgs&>(args).window);
    editbox->setSelection(0, 0);

    return true;
}

//----------------------------------------------------------------------------//
bool ColourPickerControls::handleHexadecimalEditboxTextChanged(const EventArgs&)
{
    if (d_ignoreEvents == true)
        return true;

    String hexaText = getHexadecimalEditbox()->getText();

    if (hexaText.length() == 8)
    {
        Colour hexaColour = PropertyHelper<Colour>::fromString(hexaText);
        setColours(hexaColour);
        refreshAllElements();
    }

    return true;
}

//----------------------------------------------------------------------------//
bool ColourPickerControls::handleRGBEditboxTextChanged(const EventArgs&)
{
    if (d_ignoreEvents == true)
        return true;

    int colourR = PropertyHelper<std::int32_t>::fromString(
                      getColourEditBoxR()->getText());
    int colourG = PropertyHelper<std::int32_t>::fromString(
                      getColourEditBoxG()->getText());
    int colourB = PropertyHelper<std::int32_t>::fromString(
                      getColourEditBoxB()->getText());
    RGB_Colour newColour(colourR, colourG, colourB);

    setColours(newColour);

    refreshAllElements();

    return true;
}

//----------------------------------------------------------------------------//
bool ColourPickerControls::handleLABEditboxTextChanged(const EventArgs&)
{
    if (d_ignoreEvents == true)
        return true;

    static const String labRegEx = "[-+]?[0-9]*\\.?[0-9]+";
    d_regexMatcher.setRegexString(labRegEx);

    String LabLString = getLabEditBoxL()->getText();
    String LabAString = getLabEditBoxA()->getText();
    String LabBString = getLabEditBoxB()->getText();

    bool matchingRegEx = true;
    matchingRegEx &= d_regexMatcher.getMatchStateOfString(LabLString) == RegexMatchState::Valid;
    matchingRegEx &= d_regexMatcher.getMatchStateOfString(LabAString) == RegexMatchState::Valid;
    matchingRegEx &= d_regexMatcher.getMatchStateOfString(LabBString) == RegexMatchState::Valid;

    if (!matchingRegEx)
        return true;

    float LabColourL = PropertyHelper<float>::fromString(LabLString);
    float LabColourA = PropertyHelper<float>::fromString(LabAString);
    float LabColourB = PropertyHelper<float>::fromString(LabBString);

    LabColourL = std::min(std::max(LabColourL, LAB_L_MIN), LAB_L_MAX);
    LabColourA = std::min(std::max(LabColourA, LAB_A_MIN), LAB_A_MAX);
    LabColourB = std::min(std::max(LabColourB, LAB_B_MIN), LAB_B_MAX);

    Lab_Colour newColour(LabColourL, LabColourA, LabColourB);

    setColours(newColour);

    refreshAllElements();

    return true;
}

//----------------------------------------------------------------------------//
bool ColourPickerControls::handleHSVEditboxTextChanged(const EventArgs&)
{
    if (d_ignoreEvents == true)
        return true;

    static const String labRegEx = "[-+]?[0-9]*\\.?[0-9]+";
    d_regexMatcher.setRegexString(labRegEx);

    String HString = getHSVEditBoxH()->getText();
    String SString = getHSVEditBoxS()->getText();
    String VString = getHSVEditBoxV()->getText();

    bool matchingRegEx = true;
    matchingRegEx &= d_regexMatcher.getMatchStateOfString(HString) == RegexMatchState::Valid;
    matchingRegEx &= d_regexMatcher.getMatchStateOfString(SString) == RegexMatchState::Valid;
    matchingRegEx &= d_regexMatcher.getMatchStateOfString(VString) == RegexMatchState::Valid;

    if (!matchingRegEx)
        return true;

    float Hue = PropertyHelper<float>::fromString(HString);
    float Saturation = PropertyHelper<float>::fromString(SString);
    float Value = PropertyHelper<float>::fromString(VString);

    Hue = std::min(std::max(Hue, 0.0f), 1.0f);
    Saturation = std::min(std::max(Saturation, 0.0f), 1.0f);
    Value = std::min(std::max(Value, 0.0f), 1.0f);

    HSV_Colour newColour(Hue, Saturation, Value);

    setColours(newColour);

    refreshAllElements();

    return true;
}

//----------------------------------------------------------------------------//
bool ColourPickerControls::handleAlphaEditboxTextChanged(const EventArgs&)
{
    if (d_ignoreEvents == true)
        return true;

    static const String labRegEx = "[-+]?[0-9]*\\.?[0-9]+";
    d_regexMatcher.setRegexString(labRegEx);

    String ValueString = getAlphaEditBox()->getText();
    bool matchingRegEx = d_regexMatcher.getMatchStateOfString(ValueString) == RegexMatchState::Valid;

    if (!matchingRegEx)
        return true;

    float value = PropertyHelper<float>::fromString(ValueString);

    value = std::max(std::min(value, 1.0f), 0.0f);

    setColourAlpha(value);

    refreshAllElements();

    return true;
}

//----------------------------------------------------------------------------//
void ColourPickerControls::setColourAlpha(float alphaValue)
{
    d_selectedColour.setAlpha(alphaValue);
}

//----------------------------------------------------------------------------//
bool ColourPickerControls::handleAcceptButtonClicked(const EventArgs&)
{
    WindowEventArgs args(this);
    onAcceptButtonClicked(args);

    return true;
}

//----------------------------------------------------------------------------//
bool ColourPickerControls::handleRadioButtonModeSelection(const EventArgs& args)
{
    RadioButton* radioButton = static_cast<RadioButton*>(
                                   static_cast<const WindowEventArgs&>(args).window);

    if (getLabRadioButtonL() == radioButton)
        d_sliderMode = LAB_L;
    else if (getLabRadioButtonA() == radioButton)
        d_sliderMode = LAB_A;
    else if (getLabRadioButtonB() == radioButton)
        d_sliderMode = SliderMode::LAB_B;
    else if (getHSVRadioButtonH() == radioButton)
        d_sliderMode = SliderMode::HSV_H;
    else if (getHSVRadioButtonS() == radioButton)
        d_sliderMode = SliderMode::HSV_S;
    else if (getHSVRadioButtonV() == radioButton)
        d_sliderMode = SliderMode::HSV_V;

    refreshColourPickerIndicatorPosition();

    refreshColourSliderPosition();

    refreshColourPickerControlsTextures();

    return true;
}

//----------------------------------------------------------------------------//
bool ColourPickerControls::handleColourPickerSliderValueChanged(
    const EventArgs& args)
{
    if (d_ignoreEvents == true)
        return true;

    Slider* imageSlider = static_cast<Slider*>(
        static_cast<const WindowEventArgs&>(args).window);

    const float sliderValue = static_cast<float>(imageSlider->getCurrentValue());

    Lab_Colour colourLAB = d_selectedColourLAB;
    HSV_Colour colourHSV = d_selectedColourHSV;

    switch (d_sliderMode)
    {
    case LAB_L:
        colourLAB.L = LAB_L_MIN + LAB_L_DIFF * sliderValue;
        setColours(colourLAB);
        break;

    case LAB_A:
        colourLAB.a = LAB_A_MIN + LAB_A_DIFF * sliderValue;
        setColours(colourLAB);
        break;

    case SliderMode::LAB_B:
        colourLAB.b = LAB_B_MIN + LAB_B_DIFF * sliderValue;
        setColours(colourLAB);
        break;

    case SliderMode::HSV_H:
        colourHSV.H = sliderValue;
        setColours(colourHSV);
        break;

    case SliderMode::HSV_S:
        colourHSV.S = sliderValue;
        setColours(colourHSV);
        break;

    case SliderMode::HSV_V:
        colourHSV.V = sliderValue;
        setColours(colourHSV);
        break;

    default:
        break;
    }

    onColourSliderChanged();

    return true;
}

//----------------------------------------------------------------------------//
bool ColourPickerControls::handleAlphaSliderValueChanged(const EventArgs& args)
{
    if (d_ignoreEvents == true)
        return true;

    Slider* imageSlider = static_cast<Slider*>(
        static_cast<const WindowEventArgs&>(args).window);
    const float sliderValue = 1.0f - static_cast<float>(imageSlider->getCurrentValue());

    setColourAlpha(sliderValue);

    refreshAlpha();

    refreshColourRects();


    return true;
}

//----------------------------------------------------------------------------//
bool ColourPickerControls::handleColourPickerStaticImagePointerActivate(
    const EventArgs& args)
{
    const MouseButtonEventArgs& cursor_args = static_cast<const MouseButtonEventArgs&>(args);

    if (cursor_args.d_button == MouseButton::Left)
        d_draggingColourPickerIndicator = false;

    return true;
}

//----------------------------------------------------------------------------//
bool ColourPickerControls::handleColourPickerStaticImagePointerPressHold(
    const EventArgs& args)
{
    const MouseButtonEventArgs& cursor_args = static_cast<const MouseButtonEventArgs&>(args);

    if (cursor_args.d_button == MouseButton::Left)
    {
        d_draggingColourPickerIndicator = true;

        refreshColourPickerIndicatorPosition(cursor_args);
    }

    return true;
}

//----------------------------------------------------------------------------//
bool ColourPickerControls::handleColourPickerStaticImagePointerMove(
    const EventArgs& args)
{
    if (d_colourPickerIndicator && d_draggingColourPickerIndicator)
        refreshColourPickerIndicatorPosition(
            static_cast<const CursorInputEventArgs&>(args));

    return true;
}

//----------------------------------------------------------------------------//
bool ColourPickerControls::handleColourPickerStaticImagePointerLeaves(
    const EventArgs&)
{
    if (d_colourPickerIndicator)
        d_draggingColourPickerIndicator = false;

    return true;
}

//----------------------------------------------------------------------------//
void ColourPickerControls::onCancelButtonClicked(WindowEventArgs& e)
{
    if (this->getParent() != nullptr)
        this->getParent()->removeChild(this);

    fireEvent(EventClosed, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void ColourPickerControls::onAcceptButtonClicked(WindowEventArgs& e)
{
    d_callingColourPicker->setColour(d_selectedColour);

    if (this->getParent() != nullptr)
        this->getParent()->removeChild(this);

    fireEvent(EventColourAccepted, e, EventNamespace);
    fireEvent(EventClosed, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void ColourPickerControls::refreshColourPickingImage()
{
    if (d_sliderMode &
            (LAB_L | LAB_A | SliderMode::LAB_B))
    {
        for (int y = 0; y < d_colourPickerPickingImageHeight; ++y)
        {
            for (int x = 0; x < d_colourPickerPickingImageWidth; ++x)
            {
                int i = d_colourPickerControlsTextureSize * y + x;

                Lab_Colour colour =
                    getColourPickingPositionColourLAB(static_cast<float>(x),
                                                      static_cast<float>(y));

                d_colourPickingTexture[i] = RGB_Colour(colour);
            }
        }
    }
    else if (d_sliderMode &
             (SliderMode::HSV_H | SliderMode::HSV_S | SliderMode::HSV_V))
    {
        for (int y = 0; y < d_colourPickerPickingImageHeight; ++y)
        {
            for (int x = 0; x < d_colourPickerPickingImageWidth; ++x)
            {
                int i = d_colourPickerControlsTextureSize * y + x;

                HSV_Colour colour =
                    getColourPickingPositionColourHSV(static_cast<float>(x),
                                                      static_cast<float>(y));

                d_colourPickingTexture[i] = RGB_Colour(colour);
            }
        }
    }
}

//----------------------------------------------------------------------------//
void ColourPickerControls::refreshColourSliderImage()
{
    if (d_sliderMode &
            (LAB_L | LAB_A | SliderMode::LAB_B))
    {
        for (int y = 0; y < d_colourPickerPickingImageHeight; ++y)
        {
            for (int x = 0; x < d_colourPickerColourSliderImageWidth; ++x)
            {
                int i = d_colourPickerControlsTextureSize * y +
                        (x + d_colourPickerPickingImageWidth + 2);

                Lab_Colour colour = getColourSliderPositionColourLAB(
                    y / static_cast<float>(d_colourPickerPickingImageHeight - 1));

                d_colourPickingTexture[i] = RGB_Colour(colour);
            }
        }
    }
    else if (d_sliderMode &
             (SliderMode::HSV_H | SliderMode::HSV_S | SliderMode::HSV_V))
    {
        for (int y = 0; y < d_colourPickerPickingImageHeight; ++y)
        {
            for (int x = 0; x < d_colourPickerColourSliderImageWidth; ++x)
            {
                int i = d_colourPickerControlsTextureSize * y +
                        (x + d_colourPickerPickingImageWidth + 2);

                HSV_Colour colour = getColourSliderPositionColourHSV(
                    y / static_cast<float>(d_colourPickerPickingImageHeight - 1));

                d_colourPickingTexture[i] = RGB_Colour(colour);
            }
        }
    }
}

//----------------------------------------------------------------------------//
void ColourPickerControls::refreshAlphaSliderImage()
{
    for (int y = 0; y < d_colourPickerAlphaSliderImageHeight; ++y)
    {
        for (int x = 0; x < d_colourPickerAlphaSliderImageWidth; ++x)
        {
            int i = x + d_colourPickerControlsTextureSize *
                (y + d_colourPickerImageOffset + d_colourPickerPickingImageHeight);

            RGB_Colour curColour = getAlphaSliderPositionColour(x, y);
            d_colourPickingTexture[i] = curColour;
        }
    }
}

//----------------------------------------------------------------------------//
RGB_Colour ColourPickerControls::getAlphaSliderPositionColour(int x, int y)
{
    static const RGB_Colour background1(255, 255, 255);
    static const RGB_Colour background2(122, 122, 122);

    float xRel = x / static_cast<float>(d_colourPickerAlphaSliderImageWidth - 1);
    RGB_Colour final;

    bool isBackground1;

    if ((x % 30) >= 15)
        isBackground1 = false;
    else
        isBackground1 = true;

    if ((y % 30) >= 15)
        isBackground1 = !isBackground1;

    if (isBackground1)
        final = d_selectedColourRGB * (1.0f - xRel) + background1 * xRel;
    else
        final = d_selectedColourRGB * (1.0f - xRel) + background2 * xRel;

    return final;
}


//----------------------------------------------------------------------------//
void ColourPickerControls::initColourPicker()
{
    initColourPickerControlsImageSet();

    d_sliderMode = LAB_L;
    getLabRadioButtonL()->setSelected(true);

    d_colourPickerIndicator = WindowManager::getSingleton().createWindow(
        getProperty("ColourPickerCursorStyle"),
        getName() + ColourPickerCursorName);

    d_colourPickerIndicator->setProperty(
        "BackgroundEnabled",
        PropertyHelper<bool>::toString(false));

    d_colourPickerIndicator->setProperty(
        "FrameEnabled",
        PropertyHelper<bool>::toString(false));

    d_colourPickerIndicator->setProperty(
        "Image",
        getProperty("ColourPickerCursorImage"));

    d_colourPickerIndicator->setProperty(
        "ImageColours",
        PropertyHelper<ColourRect>::toString(
            ColourRect(Colour(0.0f, 0.0f, 0.0f))));

    d_colourPickerIndicator->setWidth(UDim(0.05f, 0));
    d_colourPickerIndicator->setHeight(UDim(0.05f, 0));
    d_colourPickerIndicator->setCursorPassThroughEnabled(true);
    d_colourPickerIndicator->setClippedByParent(false);

    getColourPickerImageSlider()->getThumb()->setHotTracked(false);

    getColourPickerStaticImage()->addChild(d_colourPickerIndicator);
}

//----------------------------------------------------------------------------//
void ColourPickerControls::positionColourPickerIndicatorRelative(float x, float y)
{
    if (d_colourPickerIndicator)
    {
        d_colourPickerIndicator->setXPosition(
            UDim(d_colourPickerIndicator->getWidth().d_scale * -0.5f + x, 0.0f));
        d_colourPickerIndicator->setYPosition(
            UDim(d_colourPickerIndicator->getHeight().d_scale * -0.5f + y, 0.0f));
    }
}

//----------------------------------------------------------------------------//
void ColourPickerControls::positionColourPickerIndicatorAbsolute(float x, float y)
{
    if (d_colourPickerIndicator)
    {
        d_colourPickerIndicator->setXPosition(
            UDim(d_colourPickerIndicator->getWidth().d_scale * -0.5f, x));
        d_colourPickerIndicator->setYPosition(
            UDim(d_colourPickerIndicator->getHeight().d_scale * -0.5f, y));
    }
}

//----------------------------------------------------------------------------//
RGB_Colour ColourPickerControls::getSelectedColourRGB()
{
    return d_selectedColourRGB;
}

//----------------------------------------------------------------------------//
void ColourPickerControls::refreshColourPickerIndicatorPosition()
{
    const glm::vec2 pos = getColourPickingColourPosition();

    positionColourPickerIndicatorRelative(pos.x, pos.y);
}

//----------------------------------------------------------------------------//
void ColourPickerControls::refreshColourPickerIndicatorPosition(const CursorInputEventArgs& e)
{
    positionColourPickerIndicatorAbsolute(e.d_localPos.x, e.d_localPos.y);

    if (d_sliderMode &
        (LAB_L | LAB_A | SliderMode::LAB_B))
    {
        Lab_Colour col = getColourPickingPositionColourLAB(e.d_localPos.x, e.d_localPos.y);
        setColours(col);
    }
    else if (d_sliderMode &
             (SliderMode::HSV_H | SliderMode::HSV_S | SliderMode::HSV_V))
    {
        HSV_Colour col = getColourPickingPositionColourHSV(e.d_localPos.x, e.d_localPos.y);
        setColours(col);
    }

    onColourIndicatorPositionChanged();
}

//----------------------------------------------------------------------------//
void ColourPickerControls::refreshColourSliderPosition()
{
    String editboxText;

    switch (d_sliderMode)
    {
    case LAB_L:
        editboxText = getLabEditBoxL()->getText();
        break;

    case LAB_A:
        editboxText = getLabEditBoxA()->getText();
        break;

    case SliderMode::LAB_B:
        editboxText = getLabEditBoxB()->getText();
        break;

    case SliderMode::HSV_H:
        editboxText = getHSVEditBoxH()->getText();
        break;

    case SliderMode::HSV_S:
        editboxText = getHSVEditBoxS()->getText();
        break;

    case SliderMode::HSV_V:
        editboxText = getHSVEditBoxV()->getText();
        break;

    default:
        Logger::getSingleton().logEvent(
            "[ColourPicker] Unknown slider mode - action not processed");
        break;
    }

    float value = PropertyHelper<float>::fromString(editboxText);
    float sliderValue = 0.0f;

    switch (d_sliderMode)
    {
    case LAB_L:
        sliderValue = 1.0f - (LAB_L_MAX - value) / LAB_L_DIFF;
        break;

    case LAB_A:
        sliderValue = 1.0f - (LAB_A_MAX - value) / LAB_A_DIFF;
        break;

    case SliderMode::LAB_B:
        sliderValue = 1.0f - (LAB_B_MAX - value) / LAB_B_DIFF;
        break;

    case SliderMode::HSV_H:
        sliderValue = value;
        break;

    case SliderMode::HSV_S:
        sliderValue = value;
        break;

    case SliderMode::HSV_V:
        sliderValue = value;
        break;

    default:
        Logger::getSingleton().logEvent(
            "[ColourPicker] Unknown slider mode - action not processed");
        break;
    }

    d_ignoreEvents = true;
    getColourPickerImageSlider()->setCurrentValue(sliderValue);
    d_ignoreEvents = false;
}

//----------------------------------------------------------------------------//
void ColourPickerControls::refreshAlpha()
{
    d_ignoreEvents = true;

    getColourPickerAlphaSlider()->setCurrentValue(
        1.0f - d_selectedColour.getAlpha());

    std::stringstream floatStringStream;
    floatStringStream.precision(4);
    floatStringStream.setf(std::ios::fixed, std::ios::floatfield);
    floatStringStream << d_selectedColour.getAlpha();
    getAlphaEditBox()->setText(floatStringStream.str().c_str());

    d_ignoreEvents = false;
}

//----------------------------------------------------------------------------//
void ColourPickerControls::onColourSliderChanged()
{
    refreshEditboxesAndColourRects();

    refreshColourPickerIndicatorPosition();

    refreshColourPickerControlsTextures();
}

//----------------------------------------------------------------------------//
void ColourPickerControls::refreshEditboxesAndColourRects()
{
    refreshColourRects();

    d_ignoreEvents = true;

    getColourEditBoxR()->setText(
        PropertyHelper<std::int32_t>::toString(d_selectedColourRGB.r));
    getColourEditBoxG()->setText(
        PropertyHelper<std::int32_t>::toString(d_selectedColourRGB.g));
    getColourEditBoxB()->setText(
        PropertyHelper<std::int32_t>::toString(d_selectedColourRGB.b));

    std::stringstream floatStringStream;
    floatStringStream.precision(3);
    floatStringStream.setf(std::ios::fixed, std::ios::floatfield);

    floatStringStream << d_selectedColourLAB.L;
    getLabEditBoxL()->setText(floatStringStream.str().c_str());
    floatStringStream.str("");
    floatStringStream << d_selectedColourLAB.a;
    getLabEditBoxA()->setText(floatStringStream.str().c_str());
    floatStringStream.str("");
    floatStringStream << d_selectedColourLAB.b;
    getLabEditBoxB()->setText(floatStringStream.str().c_str());
    floatStringStream.str("");

    floatStringStream.precision(5);

    floatStringStream << d_selectedColourHSV.H;
    getHSVEditBoxH()->setText(floatStringStream.str().c_str());
    floatStringStream.str("");
    floatStringStream << d_selectedColourHSV.S;
    getHSVEditBoxS()->setText(floatStringStream.str().c_str());
    floatStringStream.str("");
    floatStringStream << d_selectedColourHSV.V;
    getHSVEditBoxV()->setText(floatStringStream.str().c_str());
    floatStringStream.str("");

    floatStringStream.precision(4);
    floatStringStream << d_selectedColour.getAlpha();
    getAlphaEditBox()->setText(floatStringStream.str().c_str());
    floatStringStream.str("");

    d_ignoreEvents = false;
}

//----------------------------------------------------------------------------//
void ColourPickerControls::refreshAllElements()
{
    refreshEditboxesAndColourRects();

    refreshColourSliderPosition();

    refreshColourPickerIndicatorPosition();

    refreshAlpha();

    refreshColourPickerControlsTextures();
}

//----------------------------------------------------------------------------//
void ColourPickerControls::onColourIndicatorPositionChanged()
{
    refreshEditboxesAndColourRects();

    refreshAlphaSliderImage();

    refreshOnlyColourSliderImage();
}

//----------------------------------------------------------------------------//
void ColourPickerControls::refreshOnlyColourSliderImage()
{
    refreshColourSliderImage();
    reloadColourPickerControlsTexture();
}

//----------------------------------------------------------------------------//
void ColourPickerControls::refreshColourRects()
{
    d_ignoreEvents = true;

    Colour newColourNoAlpha(d_selectedColour.getRed(),
                            d_selectedColour.getGreen(),
                            d_selectedColour.getBlue());

    String newColourRectString =
        PropertyHelper<ColourRect>::toString(ColourRect(newColourNoAlpha));

    Colour previousColourNoAlpha(d_previouslySelectedColour.getRed(),
                                 d_previouslySelectedColour.getGreen(),
                                 d_previouslySelectedColour.getBlue());

    String previousColourRectString =
        PropertyHelper<ColourRect>::toString(ColourRect(previousColourNoAlpha));

    getNewColourRect()->setProperty(
        ColourRectPropertyName,
        newColourRectString);

    getOldColourRect()->setProperty(
        ColourRectPropertyName,
        previousColourRectString);

    String colourString = PropertyHelper<Colour>::toString(d_selectedColour);
    getHexadecimalEditbox()->setText(colourString);

    d_ignoreEvents = false;
}

//----------------------------------------------------------------------------//

}


