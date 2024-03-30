/***************************************************************************
**                                                                        **
* Klasse: MyFilter
*
* Beschreibung: Basiert auf: https://github.com/stephenquan/MyVideoFilterApp
*               Ableitung der Klasse QAbstractVideoFilter
*               Schnittstelle zwischen Kamera und QML
*               Aufbreitung des Kamerabildes fuer das neuronale Netz
*
**************************************************************************/


#include <QDebug>
#include "MyFilter.h"

#include <opencv2/opencv.hpp> //fuer imshow
#include <opencv2/core/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>
#include <Eigen/Dense>


/****************************************************************
* Funktion      :
* Beschreibung  :
* Returnwert    :
* Parameter     :
****************************************************************/


MyFilter::MyFilter(QObject* parent): QAbstractVideoFilter( parent )
{
 m_VideoOutputOrientation=0;

 qDebug()<<"\n OpenCV Version String"<<cv::getVersionString().c_str();

}

/****************************************************************
* Funktion      : Destruktor
****************************************************************/
MyFilter::~MyFilter()
{

}


/****************************************************************
* Funktion      : capture
* Beschreibung  : Setzen der Capture Variable
* Returnwert    : -
* Parameter     : -
****************************************************************/


void MyFilter::capture()
{
    setCapturing( true );
}


/****************************************************************
* Funktion      : setCapturing
* Beschreibung  : Setzen der Capture Variable
* Returnwert    : -
* Parameter     : -
****************************************************************/

void MyFilter::setCapturing( bool capturing )
{

    if ( m_Capturing == capturing )
    {
        return;
    }

    m_Capturing = capturing;

    emit capturingChanged();
}


/****************************************************************
* Funktion      : Bildverarbeitung
* Beschreibung  : - Wandelt das Kamerabild in ein Schwarzweiss-Bild um
*               : - Reduziert die Bildgroeße auf die fuer das neuronale Netz
*               :   benoetigte Groesse von 28x28 Pixeln
* Returnwert    : sendet das verarbeitet Kamerabild
* Parameter     : Kamerabild
****************************************************************/

void MyFilter::imageProcessing(QImage image)
{
    QImage qt_Img28;

    cv::Mat img_ScreenShot;
    cv::Mat img_gray_webcam;
    cv::Mat Threshold_out;
    cv::Mat Img28;

    img_ScreenShot= cv::Mat(image.height(), image.width(),CV_8UC4, image.bits()); //QImage::Format_ARGB32 entspricht cv::CV_8UC4

    cv::resize(img_ScreenShot, img_ScreenShot, cv::Size(128,128), 0, 0, cv::INTER_AREA);

    cv::cvtColor(img_ScreenShot, img_gray_webcam, cv::COLOR_BGR2GRAY);

    cv::adaptiveThreshold(img_gray_webcam, Threshold_out,255,cv::ADAPTIVE_THRESH_MEAN_C,cv::THRESH_BINARY,13,5);

    cv::resize(Threshold_out, Img28, cv::Size(28, 28), 0, 0, cv::INTER_AREA);

    qt_Img28 = QImage((const unsigned char*) (Img28.data), Img28.cols, Img28.rows, QImage::Format_Grayscale8);

    emit sendImage(qt_Img28);
}


