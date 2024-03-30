/***************************************************************************
**                                                                        **
* Klasse:       MyImageProvider
*
* Beschreibung: Ableitung der Klasse QQuickImageProvider
*               Schnittstelle um ein Bild an die QML-GUI zusenden
*
**************************************************************************/

#include <QImage>
#include <QPixmap>
#include <QDir>
#include <QCoreApplication>

#include "MyImageProvider.h"


/****************************************************************
* Funktion      : Konstruktor
* Beschreibung  : Erzeuge graues Bild in der Größe 128x128
* Returnwert    :
* Parameter     :
****************************************************************/

MyImageProvider::MyImageProvider(QObject *parent) : QObject(parent), QQuickImageProvider(QQuickImageProvider::Image)
{    
    QPixmap pixmap(128,128);
    pixmap.fill(QColor(Qt::darkGray).rgba());

    m_image = pixmap.toImage();
}

/****************************************************************
* Funktion      : Destruktor
****************************************************************/
MyImageProvider::~MyImageProvider()
{

}


/****************************************************************
* Funktion      :
* Beschreibung  : QML fordert ueber reload() ein neues Bild an
*                 Scaliere das empfangene Bild in eine Groesse von 128x128 Pixel
* Returnwert    : Sende Bild (m_imageScaled) an QML-GUI
* Parameter     :
****************************************************************/

QImage MyImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{

    Q_UNUSED(id);
    Q_UNUSED(size);
    Q_UNUSED(requestedSize);

    QImage m_imageScaled = m_image.scaled(128, 128, Qt::KeepAspectRatio);

    return m_imageScaled;
}

/****************************************************************
* Funktion      : setImage
* Beschreibung  : Empfange das Bild
* Returnwert    : Signal: Bild empfangen
* Parameter     :
****************************************************************/

void MyImageProvider::setImage(QImage img)
{

    m_image = img;

    emit imageChanged(); //Signal fuer QML Funktion: onImageChanged()
                         // Danach holt sich die QML Funktion über requestImage
                         // das aktuelle Bild

}
