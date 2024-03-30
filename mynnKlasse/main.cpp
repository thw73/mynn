#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>

#include "mynn.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    //---------------Comandline Interface ----------------------

    QCoreApplication::setApplicationName("Neuronales Netzwerk");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Neuronales Netzwerk\n \
    mynn -t <filename> --cs <filename>\n \
    mynn -t <filename> --cs <filename>  --swih <filename> --swho <filename>\n \
    mynn --cs <filename> --wih <filename> --who <filename>");

    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption wihfile(QStringList() << "lwih", //<FileName>
              QCoreApplication::translate("main", "Load wih weights <FileName>."),
              QCoreApplication::translate("main", "FileName"));
    parser.addOption(wihfile);

    QCommandLineOption whofile(QStringList() << "lwho",
              QCoreApplication::translate("main", "Load who weights <FileName>."),
              QCoreApplication::translate("main", "FileName"));
    parser.addOption(whofile);

    QCommandLineOption wihfilesave(QStringList() << "swih",
              QCoreApplication::translate("main", "Save weights <FileName>."),
              QCoreApplication::translate("main", "FileName"));
    parser.addOption(wihfilesave);

    QCommandLineOption whofilesave(QStringList() << "swho",
              QCoreApplication::translate("main", "Save who weights <FileName>."),
              QCoreApplication::translate("main", "FileName"));
    parser.addOption(whofilesave);



    QCommandLineOption calcScorecardOption(QStringList() << "cs",
              QCoreApplication::translate("main", "Calculate scorecard <FileName>."),
            QCoreApplication::translate("main", "FileName"));
    parser.addOption(calcScorecardOption);

    QCommandLineOption loadTrainOption(QStringList() << "t" << "train",
              QCoreApplication::translate("main", "Load and train <FileName>"),
            QCoreApplication::translate("main", "FileName"));
    parser.addOption(loadTrainOption);

    parser.process(a);

    bool calcScorecardOptionValue = parser.isSet(calcScorecardOption);
    bool wihValue = parser.isSet(wihfile);
    bool whoValue = parser.isSet(whofile);
    bool wihfilesaveValue = parser.isSet(wihfilesave);
    bool whofilesaveValue = parser.isSet(whofilesave);
    bool loadTrainValue = parser.isSet(loadTrainOption);

    QString wihFileName  = parser.value(wihfile);
    QString whoFileName  = parser.value(whofile);
    QString TestFileName  = parser.value(calcScorecardOption);
    QString wihFileNameSave  = parser.value(wihfilesave);
    QString whoFileNameSave  = parser.value(whofilesave);
    QString loadTrainOptionFileName  = parser.value(loadTrainOption);

    //Erzeuge neuronales Netzwerk

    int inputnodes = 784;
    int hiddennodes = 200;
    int outputnodes = 10;
    float learningrate =0.1;

    neuralNetwork mynn(inputnodes, hiddennodes, outputnodes,learningrate);


    bool ungueltigeOption=true;

    if ((calcScorecardOptionValue==true) && (wihValue == true) && (whoValue==true) )
    {
            ungueltigeOption=false;

            mynn.loadWeights(wihFileName,whoFileName);
            mynn.calcScorecard(TestFileName);
     }

     if ((loadTrainValue==true) && (calcScorecardOptionValue == true)&& (wihfilesaveValue==false) && (whofilesaveValue == false))
     {
            ungueltigeOption=false;

            mynn.loadAndTrain(loadTrainOptionFileName);
            mynn.calcScorecard(TestFileName);
     }

     if ((loadTrainValue==true) && (calcScorecardOptionValue == true) && (wihfilesaveValue==true) && (whofilesaveValue == true) )
     {
            ungueltigeOption=false;

            mynn.loadAndTrain(loadTrainOptionFileName);
            mynn.calcScorecard(TestFileName);
            mynn.saveWeights(wihFileNameSave,whoFileNameSave);
     }


     if(ungueltigeOption==true)
     {
          parser.showHelp(1);
     }

     return 0;
}


