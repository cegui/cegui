/***********************************************************************
    created:    Wed Feb 16 2011
    author:     Paul D Turner <paul@cegui.org.uk>
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
#ifndef _CEGUIImage_h_
#define _CEGUIImage_h_

#include "CEGUI/ChainedXMLHandler.h"
#include "CEGUI/String.h"
#include "CEGUI/ColourRect.h"
#include "CEGUI/Rectf.h"

#include <vector>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{

enum class AutoScaledMode : int
{
    //! No auto scaling takes place
    Disabled,
    /*!
    Objects are auto scaled depending on their original height and current
    resolution height. Horizontal scaling is computed to keep aspect ratio.
    */
    Vertical,
    /*!
    Objects are auto scaled depending on their original width and current
    resolution width. Horizontal scaling is computed to keep aspect ratio.
    */
    Horizontal,
    /*!
    Objects are auto scaled by the smaller scaling factor of the two.
    */
    Min,
    /*!
    Objects are auto scaled by the larger scaling factor of the two.
    */
    Max,
    /*!
    Objects are auto scaled depending on their original size and current
    resolution. Both dimensions are scaled accordingly.

    This was the only auto scaling behavior available in 0.7 and it might
    break aspect ratios!
    */
    Both
};


template<>
class PropertyHelper<AutoScaledMode>
{
public:
    typedef AutoScaledMode return_type;
    typedef return_type safe_method_return_type;
    typedef AutoScaledMode pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName()
    {
        static String type("AutoScaledMode");

        return type;
    }

    static return_type fromString(const String& str)
    {
        if (str == "vertical")
        {
            return AutoScaledMode::Vertical;
        }
        else if (str == "horizontal")
        {
            return AutoScaledMode::Horizontal;
        }
        else if (str == "min")
        {
            return AutoScaledMode::Min;
        }
        else if (str == "max")
        {
            return AutoScaledMode::Max;
        }
        else if (str == "true" || str == "True")
        {
            return AutoScaledMode::Both;
        }
        else
        {
            return AutoScaledMode::Disabled;
        }
    }

    static string_return_type toString(pass_type val)
    {
        if (val == AutoScaledMode::Disabled)
        {
            return "false";
        }
        else if (val == AutoScaledMode::Vertical)
        {
            return "vertical";
        }
        else if (val == AutoScaledMode::Horizontal)
        {
            return "horizontal";
        }
        else if (val == AutoScaledMode::Min)
        {
            return "min";
        }
        else if (val == AutoScaledMode::Max)
        {
            return "max";
        }
        else if (val == AutoScaledMode::Both)
        {
            return "true";
        }
        else
        {
            assert(false && "Invalid auto scaled mode");
            return "false";
        }
    }
};


/*!
\brief
    A struct that contains the render settings for the Image class.
*/
struct ImageRenderSettings
{
    //! Constructor
    ImageRenderSettings(const Rectf& dest_area,
                        const Rectf* clip_area = nullptr,
                        bool clipping_enabled = false,
                        const ColourRect& multiplication_colours = ColourRect(0XFFFFFFFF),
                        float alpha = 1.0f) :
        d_destArea(dest_area),
        d_clipArea(clip_area),
        d_clippingEnabled(clipping_enabled),
        d_multiplyColours(multiplication_colours),
        d_alpha(alpha)
    {
    }

    ImageRenderSettings(const ImageRenderSettings& source) :
        d_destArea(source.d_destArea),
        d_clipArea(source.d_clipArea),
        d_clippingEnabled(source.d_clippingEnabled),
        d_multiplyColours(source.d_multiplyColours),
        d_alpha(source.d_alpha)
    {
    }

    //! The destination area for the Image.
    Rectf d_destArea;
    //! The clipping area of the Image.
    const Rectf* d_clipArea;
    //! True of clipping should be enabled for the geometry of this Image.
    bool d_clippingEnabled;
    //! The colour rectangle set for this Image. The colours of the rectangle will be multiplied with
    //! the Image's colours when rendered, i.e. if the colours are all '0xFFFFFFFF' no effect will be seen.
    //! If this will be used depends on the underlying image.
    ColourRect d_multiplyColours;
    //! The alpha value for this image, should be set as the GeometryBuffer's
    //! alpha by the underlying image class
    float d_alpha;
};

/*!
\brief
    Interface for Image.

    In CEGUI, an Image is some object that can render itself into a given
    GeometryBuffer object.  This may be something as simple as a basic textured
    quad, or something more complex.
*/
class CEGUIEXPORT Image :
    public ChainedXMLHandler
{
public:
    //! Constructor
    Image(const String& name);
    Image(const String& name, const glm::vec2& pixel_offset,
          const Rectf& image_area, AutoScaledMode auto_scaled,
          const Sizef& native_resolution);

    virtual ~Image();

    /*!
    \brief
        Creates a container of GeometryBuffers based on the Image, providing the 
        geometry data needed for rendering.

    \param render_settings
        The ImageRenderSettings that contain render settings for new GeometryBuffers.
    */
    virtual std::vector<GeometryBuffer*> createRenderGeometry(
        const ImageRenderSettings& render_settings) const = 0;

    /*!
    \brief
        Appends additional render geometry for this image to an GeometryBuffers.
        The GeometryBuffer must be created beforehand and must feature render
        settings that allow adding this image geometry into the same render batch.
        Batching compatibility has to be ensured before this call.

    \param geomBuffer
        The existing GeometryBuffers to which the new render geometry will be appended.

    \param renderArea
        The target area at which the image should be rendered.

    \param colours
        Multiplicative colours to be applied to the text.
    */
    virtual void addToRenderGeometry(
        GeometryBuffer& geomBuffer,
        const Rectf& renderArea,
        const Rectf* clipArea,
        const ColourRect& colours) const = 0;
        
    /*!
    \brief
        Returns the name of the Image.

    \return
        The name of the Image.
    */
    virtual const String& getName() const;
        
    /*!
    \brief
        Returns the rendered size of this Image considering the autoscale
        options that were set.

    \return
        The rendered size of this Image.
    */
    virtual const Sizef& getRenderedSize() const;

        
    /*!
    \brief
        Returns the rendered offset of this Image considering the autoscale
        options that were set.

    \return
        The rendered offset of this Image.
    */
    virtual const glm::vec2& getRenderedOffset() const;

    /*!
    \brief
        Notifies the class that the display size of the renderer has changed so that
        the window can adapt to the new display size accordingly.

    \param size
        The new display size.
     */
    virtual void notifyDisplaySizeChanged(const Sizef& renderer_display_size);

    /*!
    \brief
        Sets the rectangular image area of this Image.

    \param image_area
        The rectangular image area of this Image.
     */
    void setImageArea(const Rectf& image_area);

    /*!
    \brief
        Gets the rectangular image area of this Image.

    \return
        The rectangular image area of this Image.
     */
    Rectf getImageArea() const;

    /*!
    \brief
        Sets the pixel offset of this Image.

    \param pixel_offset
        The pixel offset of this Image.
    */
    void setOffset(const glm::vec2& pixel_offset);

    
    /*!
    \brief
        Sets the autoscale mode of this Image.

    \param autoscaled
        The  autoscale mode of this Image.
    */
    void setAutoScaled(const AutoScaledMode autoscaled);

    /*!
    \brief
        Sets the autoscale native resolution of this Image.
    */
    void setNativeResolution(const Sizef& native_res);


    /*!
    \brief
        Helper able to compute scaling factors for auto scaling

    \note
        This is mostly for internal use, unless you know what you are doing,
        please don't touch this method!
     */
    static void computeScalingFactors(AutoScaledMode mode,
                                      const Sizef& display_size,
                                      const Sizef& native_display_size,
                                      float& x_scale,
                                      float& y_scale);

protected:
    // implement chained xml handler abstract interface
    void elementStartLocal(const String& element,
                           const XMLAttributes& attributes) override;
    void elementEndLocal(const String& element) override;

    //! Updates the scaled size and offset values according to the new display size of the renderer 
    void updateScaledSizeAndOffset(const Sizef& renderer_display_size);
    //! Updates only the scaled size values according to the new display size of the renderer 
    void updateScaledSize(const Sizef& renderer_display_size);
    //! Updates only the scaled offset values according to the new display size of the renderer 
    void updateScaledOffset(const Sizef& renderer_display_size);

    //! Name used for the Image as defined during creation.
    String d_name;
    //! The rectangular area defined for this Image defining position and size of it in relation to
    //! the underlying data of the Image.
    Rectf d_imageArea;
    //! The pixel offset of the Image. It defines ???
    glm::vec2 d_pixelOffset;
    //! Whether image is auto-scaled or not and how.
    AutoScaledMode d_autoScaled;
    //! Native resolution used for autoscaling.
    Sizef d_nativeResolution;
    //! The size in pixels after having autoscaling applied.
    Sizef d_scaledSize;
    //! The offset in pixels after having autoscaling applied.
    glm::vec2 d_scaledOffset;

};

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _CEGUIImage_h_

