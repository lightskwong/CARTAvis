/***
 * Manages and loads a single source of data.
 */

#pragma once

#include "CartaLib/Nullable.h"
#include "State/ObjectManager.h"
#include "State/StateInterface.h"
#include "Data/IColoredView.h"

#include <QImage>
#include <memory>

namespace NdArray {
    class RawViewInterface;
}

namespace Image {
    class ImageInterface;
}

class CoordinateFormatterInterface;

namespace Carta {
    namespace Lib {
        namespace PixelPipeline {
            class CustomizablePixelPipeline;
        }
    }
}

namespace Carta {
    namespace Core {
        namespace ImageRenderService {
            class Service;
        }
    }
}

namespace Carta {

namespace Data {

class DataSource : public QObject, public CartaObject, public IColoredView {

Q_OBJECT

public:

    /**
     * Returns whether or not the data was successfully loaded.
     * @param fileName an identifier for the location of a data source.
     * @return true if the data souce was successfully loaded; false otherwise.
     */
    bool setFileName( const QString& fileName );

    /**
     * Sets a new color map.
     * @param name the identifier for the color map.
     */
    virtual void setColorMap( const QString& name ) Q_DECL_OVERRIDE;

    /**
     * Sets whether the colors in the map are inverted.
     * @param inverted true if the colors in the map are inverted; false
     *        otherwise.
     */
    virtual void setColorInverted( bool inverted )  Q_DECL_OVERRIDE;

    /**
     * Sets whether the colors in the map are reversed.
     * @param reversed true if the colors in the map are reversed; false
     *        otherwise.
     */
    virtual void setColorReversed( bool reversed ) Q_DECL_OVERRIDE;

    /**
     * Set the amount of red, green, and blue in the color scale.
     * @param newRed the amount of red; should be in the range [0,1].
     * @param newGreen the amount of green; should be in the range [0,1].
     * @param newBlue the amount of blue; should be in the range[0,1].
     */
    virtual void setColorAmounts( double newRed, double newGreen, double newBlue ) Q_DECL_OVERRIDE;

    /**
     * Set the gamma color map parameter.
     * @param gamma a color map parameter.
     */
    virtual void setGamma( double gamma )  Q_DECL_OVERRIDE;

    /**
     * Set whether or not to use pixel caching.
     * @param enabled true if pixel caching should be used; false otherwise.
     */
    virtual void setPixelCaching( bool enabled )  Q_DECL_OVERRIDE;

    /**
     * Set the pixel cache size.
     * @param size the new pixel cache size.
     */
    virtual void setCacheSize( int size )  Q_DECL_OVERRIDE;

    /**
     * Set whether or not to use pixel cache interpolation.
     * @param enabled true if pixel cache interpolation should be used; false otherwise.
     */
    virtual void setCacheInterpolation( bool enabled )  Q_DECL_OVERRIDE;

    /**
     * Set the data transform.
     * @param name QString a unique identifier for a data transform.
     */
    void setTransformData( const QString& name );

    /**
     * Loads the data source as a QImage.
     * @param frameIndex the channel to load.
     * @param true to force a recompute of the image clip.
     */
    Nullable<QImage> load(int frameIndex, bool forceReload);

    /**
     * Returns true if this data source manages the data corresponding
     * to the fileName; false, otherwise.
     * @param fileName a locator for data.
     */
    bool contains(const QString& fileName) const;

    /**
     * Saves the state.
     * @param winId an identifier for the DataController displaying the data.
     * @param index of the data in the DataController.
     */
    void saveState(/*QString winId, int dataIndex*/);

    /**
     * Return a QImage representation of this data.
     * @param frameIndex the index of the spectral coordinate to load.
     * @param forceClipRecompute true if the clip should be recomputed; false if
     *      a cached value can be used.
     */
    void load(int frameIndex, bool forceClipRecompute, bool autoClip, double clipMinPercentile, double clipMaxPercentile );

    /**
     * Return the number of channels in the image.
     * @return the number of channels in the image.
     */
    int getFrameCount() const;

    /**
     * Return the number of dimensions in the image.
     * @return the number of image dimensions.
     */
    int getDimensions() const;

    /**
     * Returns the location on the image corresponding to a screen point in
     * pixels.
     * @param screenPt an (x,y) pair of pixel coordinates.
     * @param valid set to true if an image is loaded that can do the translation; otherwise false;
     * @return the corresponding location on the image.
     */
    QPointF getImagePt( QPointF screenPt, bool* valid ) const;

    /**
     * Return the current pan center.
     * @return the centered image location.
     */
    QPointF getCenter() const;

    /**
     * Return the zoom factor for this image.
     * @return the zoom multiplier.
     */
    double getZoom() const;

    /**
     * Set the center for this image's display.
     * @param imgX the x-coordinate of the center.
     * @param imgY the y-coordinate of the center.
     */
    void setPan( double imgX, double imgY );

    /**
     * Set the zoom factor for this image.
     * @param zoomFactor the zoom multiplier.
     */
    void setZoom( double zoomFactor );

    /**
     * Return the image size for the given coordinate index.
     * @param coordIndex an index of a coordinate of the image.
     * @return the corresponding dimension for that coordinate or -1 if none exists.
     */
    int getDimension( int coordIndex ) const;

    /**
     * Returns the underlying image.
     */
    std::shared_ptr<Image::ImageInterface> getImage();

    /**
     * Returns the image's file name.
     * @return the path to the image.
     */
    QString getFileName() const;

    /**
     * Returns the raw data as an array.
     * @param channel the index of the channel needed.
     * @return the raw data or nullptr if there is none.
     */
    NdArray::RawViewInterface *  getRawData( int channel ) const;

    /**
     * Returns information about the image at the current location of the cursor.
     * @param mouseX the mouse x-position in screen coordinates.
     * @param mouseY the mouse y-position in screen coordinates.
     * @param frameIndex the current channel index.
     * @param pictureWidth the width of the QImage displaying the source.
     * @param pictureHeight the height of the QImage displaying the source.
     */
    QString getCursorText( int mouseX, int mouseY, int frameIndex, int pictureWidth, int pictureHeight );

    /**
     * Resize the view of the image.
     */
    void viewResize( const QSize& newSize );

    /**
     * Generate a new QImage.
     */
    void render();

    virtual ~DataSource();

    const static QString CLASS_NAME;

signals:

    //Notification that a new image has been produced.
    void renderingDone( QImage img);

private slots:

    //Notification from the rendering service that a new image has been produced.
    void _renderingDone( QImage img, int64_t jobId );

private:

    /**
     * Constructor.
     * @param the base path for state identification.
     * @param id the particular id for this object.
     */
    DataSource(const QString& path, const QString& id );

    class Factory;

    void _initializeState();

    void _updateClips( std::shared_ptr<NdArray::RawViewInterface>& view, int frameIndex,
            double minClipPercentile, double maxClipPercentile );

    //Path for loading data - todo-- do we need to store this?
    QString m_fileName;
    bool m_cmapUseCaching;
    bool m_cmapUseInterpolatedCaching;
    int m_cmapCacheSize;

    static bool m_registered;
    static const QString DATA_PATH;

    //Pointer to image interface.
    std::shared_ptr<Image::ImageInterface> m_image;

    /// coordinate formatter
    std::shared_ptr<CoordinateFormatterInterface> m_coordinateFormatter;

    /// clip cache, hard-coded to single quantile
    std::vector< std::vector<double> > m_quantileCache;

    /// the rendering service
    std::unique_ptr<Carta::Core::ImageRenderService::Service> m_renderService;

    ///pixel pipeline
    std::shared_ptr<Carta::Lib::PixelPipeline::CustomizablePixelPipeline> m_pixelPipeline;
};
}
}
