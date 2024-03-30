/***************************************************************************
**                                                                        **
* Klasse:       MyFilterRunnable
*
* Beschreibung: Basiert auf: https://github.com/stephenquan/MyVideoFilterApp
*
*               Ableitung der Klasse QAbstractVideoFilter
*               Schnittstelle zwischen Kamera und QML
*               Aufbreitung des Kamerabildes fuer das neuronale Netz
*
*
**************************************************************************/

#include <QDebug>
#include <QStandardPaths>
#include <QImageWriter>
#include <QFile>
#include <QDir>

#include "MyFilterRunable.h"
#include "MyFilter.h"


#include <QOpenGLContext>
#include <QOpenGLFunctions>


/****************************************************************
* Funktion      : Konstruktor
* Beschreibung  :
* Returnwert    : -
* Parameter     :
****************************************************************/

MyFilterRunnable::MyFilterRunnable( MyFilter* filter ) :
    QVideoFilterRunnable(),
    m_Filter( filter )
{

}


/****************************************************************
* Funktion      :
* Beschreibung  :
* Returnwert    :
* Parameter     :
****************************************************************/

QVideoFilterRunnable* MyFilter::createFilterRunnable()
{

    return new MyFilterRunnable( this );
}

/****************************************************************
* Funktion      : run
* Beschreibung  : Wenn der Capture aktive ist, dann wandle das
*               : Kamerabild in das QImage Format und dreh das
*               : Kamerabild entsprechend der Kameraorientierung
* Returnwert    : Kamerabild im QImage Format
* Parameter     : VideoFrame von der Kamera
****************************************************************/

QVideoFrame MyFilterRunnable::run( QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags )
{
    Q_UNUSED( flags )
    Q_UNUSED( surfaceFormat )

    int angle=0;

    if ( !input )
    {
        return QVideoFrame();
    }

    if ( !m_Filter || !m_Filter->capturing() )
    {
        return *input;
    }

    QImage image = QVideoFrameToQImage( *input );

    angle=m_Filter->videoOutputOrientation();

    QTransform matrix;

    switch ( angle )
    {
        case 0:
            matrix.rotate(0);
            break;
        case 180:
            matrix.rotate(180);
            break;
        case 270:
            matrix.rotate(90);
            break;
        default:
            break;
    }

    image=image.transformed(matrix,Qt::FastTransformation);

    m_Filter->setCapturing( false );

    m_Filter->imageProcessing(image);

   return image;
}


/****************************************************************
* Funktion      : QVideoFrameToQImage
* Beschreibung  : Konvertiert den VideoFrame der Kamera in ein QImage Bild
*               :
* Returnwert    : QImage Bild
* Parameter     : VideoFrame
****************************************************************/

QImage MyFilterRunnable::QVideoFrameToQImage( const QVideoFrame& videoFrame )
{
    if ( videoFrame.handleType() == QAbstractVideoBuffer::NoHandle )
    {
        QImage image = videoFrame.image();//qt_imageFromVideoFrame( videoFrame ) does not longer exist
        if ( image.isNull() )
        {
            return QImage();
        }

        if ( image.format() != QImage::Format_ARGB32 )
        {
            image = image.convertToFormat( QImage::Format_ARGB32 );
        }

        return image;
    }

    //Android: Using the GLTextureHandle to obtain a QImage.
    if ( videoFrame.handleType() == QAbstractVideoBuffer::GLTextureHandle )
    {
        QImage image( videoFrame.width(), videoFrame.height(), QImage::Format_ARGB32 );
        GLuint textureId = static_cast<GLuint>( videoFrame.handle().toInt() );
        QOpenGLContext* ctx = QOpenGLContext::currentContext();
        QOpenGLFunctions* f = ctx->functions();
        GLuint fbo;
        f->glGenFramebuffers( 1, &fbo );
        GLint prevFbo;
        f->glGetIntegerv( GL_FRAMEBUFFER_BINDING, &prevFbo );
        f->glBindFramebuffer( GL_FRAMEBUFFER, fbo );
        f->glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0 );
        f->glReadPixels( 0, 0,  videoFrame.width(),  videoFrame.height(), GL_RGBA, GL_UNSIGNED_BYTE, image.bits() );
        f->glBindFramebuffer( GL_FRAMEBUFFER, static_cast<GLuint>( prevFbo ) );
        return image.rgbSwapped();
    }

    return QImage();
}


