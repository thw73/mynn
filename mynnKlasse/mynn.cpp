/*******************************************************************************************************
**                                                                                                    **
* Klasse:       Neuronales Netz
*
* Beschreibung: Einfaches neuronales Netz
*             : Basiert auf dem Buch: Neuronale Netze selbst programmieren von Tariq Rashid
*             : Python Code zum Buch: https://github.com/makeyourownneuralnetwork/makeyourownneuralnetwork
*
*
*********************************************************************************************************/

#include <vector>
#include <random>
#include <cmath>
#include <fstream>

#include <QDebug>
#include <QFile>
#include <QString>

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
        inodes = inputnodes;
        hnodes = hiddennodes;
        onodes = outputnodes;

        lr = learningrate;
}



/****************************************************************
* Funktion      : Trainieren vom neuronalen Netz
* Beschreibung  : Traniert die Werte fueer die Gewichte vom 
*				: neuronalen Netz (wih, who)
* Returnwert    : -
* Parameter     : inputs_list: Beispiele von zu lernende Ziffern
*				: target_list: entsprechender ???????	
****************************************************************/

void neuralNetwork::train( Eigen::Matrix<double,1,Eigen::Dynamic> inputs_list, Eigen::Matrix<double,1,Eigen::Dynamic> targets_list)
{

        // Konvertiere Listen nach Spalten-Vektor

        Eigen::Matrix<double,Eigen::Dynamic, Eigen::Dynamic> inputs;
        Eigen::Matrix<double,Eigen::Dynamic, Eigen::Dynamic> targets;

		inputs=inputs_list.transpose();
		
        targets =targets_list.transpose();
	
        //Berechne die Werte vom Hidden Layer	

		Eigen::Matrix<double,Eigen::Dynamic, 1> hidden_inputs; //Spaltenvector

        hidden_inputs = wih * inputs;

	    // Berechne die Werte die aus den Hidden Layer herausgehen
		// (Multipilkation mit der Sigmoid Funktion)
		
		Eigen::Matrix<double,Eigen::Dynamic, 1> hidden_outputs;//Spaltenvector

        hidden_outputs=hidden_inputs.unaryExpr([](double sum){return (0.5*(1.0+ std::tanh( 0.5*sum)) );}) ;
		
		// Berechner die Werte fuers Output-Layer
		// Berechne die Werte von Hidden- zum Output-Layer
        
		Eigen::Matrix<double,Eigen::Dynamic, 1> final_inputs;

        final_inputs = who * hidden_outputs;
		
		// Berechne die Werte aus dem Output Layer herausgehen
		// (Multipilkation mit der Sigmoid Funktion)

		Eigen::Matrix<double,Eigen::Dynamic, 1> final_outputs;
		
        final_outputs=final_inputs.unaryExpr([](double sum){return (0.5*(1+ std::tanh( 0.5*sum)) );}) ;
		
        
		
        //--------------Anpassen der Gewichte-----------------------
		
        //Berechnung Fehler vom Output-Layer (Zielwert - akuteller Wert)
		Eigen::Matrix<double,Eigen::Dynamic, 1> output_errors;
		
		output_errors = targets - final_outputs;

        // Der Fehler vom Hidden-Layer, ist der von den Gewichten gesplittete Wert von den Output Fehlern,
		// zusammengeführt an den Hidden Knoten
        
		Eigen::Matrix<double,Eigen::Dynamic, 1> hidden_errors;  
		 
        hidden_errors = who.transpose() * output_errors;

        //Anpassen der Gewichte zwischen dem Hidden-Layer und dem Output-Layer

        Eigen::Matrix<double,Eigen::Dynamic, Eigen::Dynamic> matrix_ones; // Hilfsmatrix

        //Zwischenspreicher fuer  output_errors * final_outputs * (1.0 - final_outputs) * hidden_outputs.transpose()
        Eigen::Matrix<double,Eigen::Dynamic, Eigen::Dynamic> dotvalueResultwho;
        Eigen::Matrix<double,Eigen::Dynamic, Eigen::Dynamic> dotvalueResultwih;
		
        // Anpassen der Gewichte zwischen Hidden-Layer und Output-Layer (who)
        dotvalueResultwho = (output_errors.cwiseProduct(final_outputs.cwiseProduct(matrix_ones.setOnes(final_outputs.rows(), final_outputs.cols()) - final_outputs)))*hidden_outputs.transpose();
        who += lr * dotvalueResultwho;
		
        // Anpassen der Gewichte zwischen Input-Layer und Hidden-Layer (wih)
        dotvalueResultwih = (hidden_errors.cwiseProduct(hidden_outputs.cwiseProduct(matrix_ones.setOnes(hidden_outputs.rows(), hidden_outputs.cols()) - hidden_outputs)))*inputs.transpose();
        wih += lr * dotvalueResultwih;

}

/****************************************************************
* Funktion      : Durchlaufe, dass neuronale Netz
* Beschreibung  :
* Returnwert    : Erkannte Wert
* Parameter     : zu erkennen Ziffer als Input-Liste (Zeilen-Vektor)
****************************************************************/

Eigen::Matrix<double,Eigen::Dynamic, 1> neuralNetwork::query( Eigen::Matrix<double,1,Eigen::Dynamic> inputs_list)
{

		// Konvertiere Eingangsliste nach Spalten-Vektor

        Eigen::Matrix<double,Eigen::Dynamic, Eigen::Dynamic> inputs;

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

/****************************************************************
* Funktion      : Laden der Gewichte
* Beschreibung  : Laed die Gewichte der Knoten vom neuronalen Netz
* Returnwert    : 
* Parameter     : Dateiname der Gewichte WIH & WHO im CSV Format
****************************************************************/

void neuralNetwork::loadWeights(QString wih_data,QString who_data)
{
	 
    QVector<double> who_tmp;
    QVector<double> wih_tmp;


    QFile who_data_file(who_data);
    QFile wih_data_file(wih_data);


    if(!who_data_file.open(QFile::ReadOnly |  QFile::Text))
    {
            qDebug() << "Datei konnte zum Lesen nicht geoeffnet werden: "<<who_data;
    }

    else
    {
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
            who = Eigen::Map<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic, Eigen::RowMajor>>(tmpWhoEigen.data(), onodes, tmpWhoEigen.size() / onodes);
        }


        if(!wih_data_file.open(QFile::ReadOnly |  QFile::Text))
        {
                qDebug() << "Datei konnte zum Lesen nicht geoeffnet werden: "<<wih_data;
        }
        else
        {
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

             // Konvertiere QVector (wih_tmp) zu Eigen::Matrix (wih)
             std::vector<double> tmpWihEigen {wih_tmp.begin(), wih_tmp.end()};
             wih = Eigen::Map<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic, Eigen::RowMajor>>(tmpWihEigen.data(),hnodes, tmpWihEigen.size() / hnodes);
        }
}


/****************************************************************
* Funktion      : Speichner der gelernten Knoten-Gewichte
* Beschreibung  : Speichert die Gewichte der Knoten vom neuronalen Netz
* Returnwert    : -
* Parameter     : Dateiname der Gewichte WIH & WHO im CSV Format
* Siehe         : https://eigen.tuxfamily.org/dox/structEigen_1_1IOFormat.html
****************************************************************/

void neuralNetwork::saveWeights(QString pathWih,QString pathWho)
{

    const static Eigen::IOFormat CSVFormat(Eigen::FullPrecision, Eigen::DontAlignCols, ", ", "\n");

    std::ofstream wihFile;
    wihFile.open(pathWih.toLocal8Bit().data(), std::ios::binary);

    if (wihFile.is_open())
    {
        wihFile << this->wih.format(CSVFormat);
        wihFile.close();
    }

    std::ofstream whoFile;
    whoFile.open(pathWho.toLocal8Bit().data(), std::ios::binary);

    if (whoFile.is_open())
    {
        whoFile << this->who.format(CSVFormat);
        whoFile.close();
    }

     qDebug()<<"Werte gespeicher\n";
}


/****************************************************************
* Funktion      : Lade und traniere
* Beschreibung  : Lade die Trainingsdaten und traniere das neuronale Netz
* Returnwert    : Fehlercode 
*				: -1 Error
*				: 0 Erfolgreich
* Parameter     : Dateiname der Trainingsdaten
****************************************************************/

int neuralNetwork::loadAndTrain(QString TrainDataFileName)
{

    QVector<double> line;

    QFile TrainData(TrainDataFileName);


    // Zufallswerte erzeugen
    std::random_device random_device;
    std::mt19937 random_engine(random_device());
    std::normal_distribution<double> distribution_wih(0.0, pow(inodes, -0.5));
    std::normal_distribution<double> distribution_who(0.0, pow(hnodes, -0.5));

    wih.resize(hnodes,inodes);
    who.resize(onodes,hnodes);

    for (int i = 0; i < hnodes; ++i)
     {
        for (int j=0; j< inodes; ++j)
        {
            wih(i,j)=distribution_wih(random_engine);
        }
     }

    for (int i = 0; i < onodes; ++i)
    {
        for (int j=0;j<hnodes;++j)
        {
            who(i,j)=distribution_who(random_engine);
        }
    }

   //Laden der Trainingsdaten

    if(!TrainData.open(QFile::ReadOnly |  QFile::Text))
    {
        qDebug() << "Datei konnte zum Lesen nicht geoeffnet werden: "<<TrainDataFileName;
        return -1;
    }

      //Epoche ist die Anzahl der Trainingsdurchelaeufe
      int epoche=5;

      for(int e=0;e<epoche;e++)
      {
        QTextStream Stream(&TrainData);

        while(Stream.atEnd()==false)
        {
            QString LineData = Stream.readLine();
            QStringList Data = LineData.split(",");

            for(int i=0; i< Data.length() ; i++)
            {
                QString str;
                str=Data[i];

                double d = str.toDouble();

                line << d;
            }

            int linesize = line.size();

            QVector<double> inputs(linesize - 1);
            inputs.clear();

            //Scaliere die Inputwerte.
            //Grund: Aktivierungfunktion kann die Wert 0 und 1 nicht erreichen
            //siehe Buch von Tariq Rashid Seite 144

            //Alle Wert ab zweitem Eintrag sind die Inputwerte fuer das neuronale Netz. Der erste Wert ist der Target-Wert
            //Deshalb startet die For-Schleife mit 1

            for(int i=1;i<linesize;i++)
            {
                inputs<< (line.at(i) / (255.0 * 0.99)) + 0.01;
            }

            QVector<double> targets(onodes,0.01);

            targets[line.at(0)]=0.99;

            line.clear();

            // Konvertiere QVector zu Eigen::Matrix (Zeilenvektor)
            std::vector<double> tmpInputEigen {inputs.begin(), inputs.end()};
            std::vector<double> tmptargetsEigen {targets.begin(), targets.end()};

            Eigen::MatrixXd inputEigen = Eigen::Map<Eigen::Matrix<double, 1, 784> >(tmpInputEigen.data());
            Eigen::MatrixXd targetsEigen = Eigen::Map<Eigen::Matrix<double, 1, 10> >(tmptargetsEigen.data());

            // Trainiere das neuronale Netz
            train(inputEigen,targetsEigen);
        }

      } //End Epoche

    return(0);
}


/****************************************************************
* Funktion      : Berechne die Scorecard
* Beschreibung  : Berechnet wie gut das tranierte Netz
*				: die Testdaten erkannt hat
* Returnwert    : Fehlercode 
*				: -1 Error
*				: 0 Erfolgreich
*
* Parameter     : Dateiname der Testdaten
****************************************************************/

int neuralNetwork::calcScorecard(QString TestDataFileName)
{
    QList<double> scorecard;


    // Lade die MNIST Testdaten

    QFile testDataFile(TestDataFileName);

    if(!testDataFile.open(QFile::ReadOnly |  QFile::Text))
    {
      qDebug() << "Datei konnte zum Lesen nicht geoeffnet werden: "<<TestDataFileName;
      return -1;
    }

    QTextStream StreamTestData(&testDataFile);

    QVector<double> inputsTestData;

    while(testDataFile.atEnd()==false)
    {

        QString LineData = StreamTestData.readLine();
        QStringList Data = LineData.split(",");

        for(int i=0; i< Data.length() ; i++)
        {
            QString str;
            str=Data[i];

            double d = str.toDouble();

            inputsTestData << d;
        }

        //Der gelabelte Wert, steht als erster Wert in Zeile   
        int correct_label = inputsTestData[0];

        //Scaliere die Inputwerte.
        //Grund: Aktivierungfunktion kann die Wert 0 und 1 nicht erreichen
        //siehe Buch von Tariq Rashid Seite 144

        int inputsize = inputsTestData.size();

        QVector<double> inputs(inputsTestData.size()-1);
        inputs.clear();

        for(int i=1;i<inputsize;i++)
        {
            inputs<< (inputsTestData.at(i) / (255.0 * 0.99)) + 0.01;
        }

        //Wandle QVector zu Eigen Vector um
        std::vector<double> tmpintputseigen {inputs.begin(), inputs.end()};
        Eigen::MatrixXd intputs_eigen = Eigen::Map<Eigen::Matrix<double, 1, 784> >(tmpintputseigen.data());

        Eigen::Matrix<double,Eigen::Dynamic, 1> outputs;

        //Durchlaufe das neuronale Netz
        outputs = this->query(intputs_eigen);


        //Finde den Index mit dem hoechsten Wert, dieser Index (0-9) entspricht der erkannten Zahl

        double maxValue = outputs.maxCoeff();

        int label=0;

        for (int i=0; i < outputs.size();i++)
        {
            if (maxValue == outputs(i))
            {
                label=i;
                break;
            }
        }


        if (label == correct_label)
        {
            scorecard.append(1);  // Die Antwort vom neuronalen Netz ist richtig. Erhoehe die Scorecard um 1
        }
        else
        {
            scorecard.append(0); // Die Antwort vom neuronalen Netz ist falsch. Keine Erhoehung der Scorecard
        }

        inputsTestData.clear();
   }

      //Berechne die Performance vom neuronalen Netz

      int scorecardsum=0;

      for(int i=0;i<scorecard.size();i++)
      {
          scorecardsum+=scorecard[i];
      }

      qDebug()<<"Scorecard Summe "<<scorecardsum<<"\n";

      double performance=(double)scorecardsum / (double)scorecard.size();

      qDebug()<<"Performance "<<performance<<"\n";

      return 0;
}

