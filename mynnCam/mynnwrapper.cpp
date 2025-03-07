/***************************************************************************
**                                                                        **
* Klasse: mynnwrapper
*
* Beschreibung: - Initialisiert das neuronale Netz
*               - Berechnet aus dem abereitetem Capture Bild
*                 ueber das neuronale Netz die Ziffer
*               - sendet die Ziffer als Textinformation aus
*
**************************************************************************/

#include "mynnwrapper.h"
#include <QDebug>


#include <opencv2/core/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/eigen.hpp>


/****************************************************************
* Funktion      : Konstruktor
* Beschreibung  : Initialisiert das neuronale Netzwerk (mynn)
****************************************************************/

mynnwrapper::mynnwrapper(): mynn(784,200,10,0.1)
{

}

/****************************************************************
* Funktion      : Destruktor
****************************************************************/
mynnwrapper::~mynnwrapper()
{

}

/****************************************************************
* Funktion      : Ermittle die Ziffer
* Beschreibung  : Erkenne aus dem Ziffernbild ueber das neuronale Netz die Ziffer
* Eingang       : Schwarz-Weiss Ziffernbild in der Aufloesung 28x28 Pixel
* Ausgang       : Erkannte Ziffer wird als Text gesendet
****************************************************************/

void mynnwrapper::calcmynn(QImage qt_imageSmal)
{

    cv::Mat sw_img;
    cv::Mat cv_imageSmal;

    cv_imageSmal = cv::Mat(qt_imageSmal.height(), qt_imageSmal.width(),CV_8UC1, qt_imageSmal.bits(), qt_imageSmal.bytesPerLine()); //QImage::Format_Grayscale8 = cv::CV_8UC1

    cv::bitwise_not(cv_imageSmal, sw_img);

    // Konvertiere opencv to Eigen::vector

    // cv2eigen(sw_img, sw_img_eigen); Geht leider nicht. Bild wird um 90 Grad gedreht und gespiegelt

    Eigen::Map<Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> sw_img_eigen(sw_img.ptr<unsigned char>(), sw_img.rows, sw_img.cols);


    // Skaliere die Input-Werte
    Eigen::Matrix<double,1, 784> inputs_eigen;
    int inputsize = sw_img_eigen.size();

    for(int i=0;i<inputsize;i++)
    {
         inputs_eigen(i) =((sw_img_eigen(i) / 255.0 * 0.99)) + 0.01;
    }

    // Durchlaufe das neuronale Netzwerk

    Eigen::Matrix<double,Eigen::Dynamic, Eigen::Dynamic> outputs;

    outputs =mynn.query(inputs_eigen);

    // Finde den Index mit dem hoechsten Wert, dieser Index (0-9) entspricht der erkannten Zahl
    Eigen::Index maxRow, maxCol;

    outputs.maxCoeff(&maxRow, &maxCol);

    int label=maxRow;  //outputs ist ein Zeilenvector (Row Vector), deshalb: maxRow

    QString text="Erkannt: "+QString::number(label);

    emit textChanged(text); //Sende erkannte Zahl an die QML GUI

}





