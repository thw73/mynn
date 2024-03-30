/***************************************************************************
**                                                                        **
* Klasse: Neuronales Netz
*
* Beschreibung:
*
*
**************************************************************************/


#include <vector>
#include <random>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <iomanip>

#include <fstream>


#include <QDebug>
#include <QFile>
#include <QCoreApplication>
#include <QDir>

#include "mynn.h"

#include <Eigen/Dense>

using namespace std;



/****************************************************************
* Funktion      : Konstruktor
* Beschreibung  : Initialisiert das neuronale Netz
*               : Laden der gelernten Gewichte
* Returnwert    : -
* Parameter     : Anzahl Eingangsknoten, Anzahl Zwischenknoten,
*               : Anzahl Ausgangsknoten
*               : Lernrate
****************************************************************/

neuralNetwork::neuralNetwork(int inputnodes, int hiddennodes, int outputnodes, float learningrate)
{
    Q_UNUSED(learningrate);

    inodes = inputnodes;
    hnodes = hiddennodes;
    onodes = outputnodes;

    QVector<double> who_tmp;
    QVector<double> wih_tmp;


#ifdef Q_OS_ANDROID
    QFile who_data_file("assets:/who.csv");

#elif defined(Q_OS_LINUX)
    QDir exeDir(QCoreApplication::applicationDirPath());
    QFile who_data_file(exeDir.filePath("assets/who.csv"));
#elif defined(Q_OS_WIN)
    QDir exeDir(QCoreApplication::applicationDirPath());
    QFile who_data_file(exeDir.filePath("../assets/who.csv"));

#endif



    if(!who_data_file.open(QFile::ReadOnly |  QFile::Text))
    {
        qDebug() << " Could not open the file for reading (who)";

    }


        QTextStream StreamWHO(&who_data_file);

        while(StreamWHO.atEnd()==false)
        {

            QString LineDataWHO = StreamWHO.readLine();
            QStringList DataWHO = LineDataWHO.split(",");

            for(int i=0; i< DataWHO.length() ; i++)
            {
                QString str;
                str=DataWHO[i];

                double d = str.toDouble();

                who_tmp << d;
            }
        }

        // Konvertiere QVector zu Eigen::Matrix
        std::vector<double> tmpWhoEigen {who_tmp.begin(), who_tmp.end()};

        who = Eigen::Map<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic, Eigen::RowMajor>>(tmpWhoEigen.data(), 10, tmpWhoEigen.size() / 10);



#ifdef Q_OS_ANDROID
    QFile wih_data_file("assets:/wih.csv");

#elif defined(Q_OS_LINUX)
    QFile wih_data_file(exeDir.filePath("assets/wih.csv"));

#elif defined(Q_OS_WIN)
    QFile wih_data_file(exeDir.filePath("../assets/wih.csv"));

#endif
        if(!wih_data_file.open(QFile::ReadOnly |  QFile::Text))
        {
            qDebug() << " Could not open the file for reading (wih)";

        }

        QTextStream StreamWIH(&wih_data_file);

        while(StreamWIH.atEnd()==false)
        {

            QString LineDataWIH = StreamWIH.readLine();
            QStringList DataWIH = LineDataWIH.split(",");

            for(int i=0; i< DataWIH.length() ; i++)
            {
                QString str;
                str=DataWIH[i];

                double d = str.toDouble();

                wih_tmp << d;
            }
         }

         // Konvertiere QVector zu Eigen::Matrix

         std::vector<double> tmpWihEigen {wih_tmp.begin(), wih_tmp.end()};

         wih = Eigen::Map<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic, Eigen::RowMajor>>(tmpWihEigen.data(),200, tmpWihEigen.size() / 200);
}





/****************************************************************
* Funktion      : Durchlaufe, dass neuronale Netz
* Beschreibung  :
* Returnwert    : Erkannte Wert
* Parameter     : Bild als Input-Liste (Zeilen-Vektor)
****************************************************************/


Eigen::Matrix<double,Eigen::Dynamic, 1> neuralNetwork::query( Eigen::Matrix<double,1,Eigen::Dynamic> inputs_list)
{

    // Konvertiere Eingangsliste nach Spalten-Vektor
    Eigen::Matrix<double,Eigen::Dynamic, 1> inputs;

    inputs=inputs_list.transpose();


    //Berechne die Werte vom Hidden Layer
    Eigen::Matrix<double,Eigen::Dynamic, 1> hidden_inputs;
		
    hidden_inputs = wih * inputs;


    // Berechne die Werte die aus den Hidden Layer herausgehen
    // (Multipilkation mit der Sigmoid Funktion)
    Eigen::Matrix<double,Eigen::Dynamic, 1> hidden_outputs;//Spaltenvector

    hidden_outputs=hidden_inputs.unaryExpr([](double sum){return (0.5*(1+ std::tanh( 0.5*sum)) );}) ;


    // Berechner die Werte fuers Output-Layer
    // Berechne die Werte von Hidden- zum Output-Layer

    Eigen::Matrix<double,Eigen::Dynamic, 1> final_inputs;

    final_inputs = who * hidden_outputs;

    // Berechne die Werte aus dem Output Layer herausgehen

    // (Multipilkation mit der Sigmoid Funktion)
    Eigen::Matrix<double,Eigen::Dynamic, 1> final_outputs;

    final_outputs=final_inputs.unaryExpr([](double sum){return (0.5*(1+ std::tanh( 0.5*sum)) );}) ;

    return(final_outputs);
}


