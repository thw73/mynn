/***************************************************************************
*                                                                          *
* Beschreibung: Neuronales Netz zur Erkennung von Ziffern.
*				Der Algorithmus basiert auf dem, im Buch von Tariq Rashid 
*				Neuronale Netze selbst programmieren
*
*				
**************************************************************************/


#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>  //wird benoetigt fuer: ->setContextProperty
#include <QObject>

#include <QDebug>

#include "MyFilterRunable.h"
#include "MyFilter.h"
#include "MyImageProvider.h"

#include "mynnwrapper.h"


/****************************************************************
* Funktion      : Hauptfunktion
* Beschreibung  : Instanziierung der benötigen Klassen
*               : Herstellen der Schnittstelle zwischen den Instanzen und QML GUI
*
****************************************************************/

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);


    qRegisterMetaType<QImage>("QImage");

    QQmlApplicationEngine qmlEngine;


    MyFilter m_MyFiler;


    mynnwrapper m_MynnWrapper;

    qmlEngine.rootContext()->setContextProperty("myClassObj",&m_MynnWrapper); // Fuer Connections in QML
																			  // target: myClassObj
															

    MyImageProvider *m_MyImageProvider(new MyImageProvider);

    qmlEngine.rootContext()->setContextProperty("ImageProvider",m_MyImageProvider); // Fuer Connections in QML
																				    // target: ImageProvider

    qmlEngine.addImageProvider("myimg", m_MyImageProvider);


    qmlRegisterUncreatableType<MyFilter>("MyFilterLib", 1, 0, "MyFilter","Hallo");

    qmlEngine.rootContext()->setContextProperty("filter", &m_MyFiler); // In QML muss dann "filter" für die Methoden von MyFilter 
																	   // genutzt werden: [filter] bzw. filter.capture


    qmlEngine.load(QStringLiteral("qrc:/main.qml"));


    QObject::connect(&m_MyFiler, SIGNAL(sendImage(QImage)), m_MyImageProvider, SLOT(setImage(QImage)));

    QObject::connect(&m_MyFiler, SIGNAL(sendImage(QImage)), &m_MynnWrapper, SLOT(calcmynn(QImage)));


    return app.exec();
}
